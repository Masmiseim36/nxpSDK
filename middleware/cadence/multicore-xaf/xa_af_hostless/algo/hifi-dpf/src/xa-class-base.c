/*
* Copyright (c) 2015-2024 Cadence Design Systems Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*******************************************************************************
 * xa-class-base.c
 *
 * Generic audio codec task implementation
 ******************************************************************************/

#define MODULE_TAG                      BASE

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf-dp.h"
#include "xa-class-base.h"

#ifndef XA_DISABLE_EVENT
#define XF_EVENT_CHANNEL_INFO_ALIGNMENT 32

/*******************************************************************************
 * Event channel structure
 ******************************************************************************/
struct xf_channel_info
{
    xf_channel_info_t       *next;
    xf_msg_pool_t           pool;
    xf_msg_queue_t          queue;
    xf_message_t            *delete_msg;
    UWORD32                 event_id_src;
    UWORD32                 event_id_dst;
    xf_msg_id_dtype         port_dst;
    UWORD32                 event_buf_count;
    UWORD32                 buf_size;
    UWORD32                 shared_channel;
};
#endif
/*******************************************************************************
 * Internal functions definitions
 ******************************************************************************/
/* ...scratch memory allocation if needed */
static XA_ERRORCODE xf_scratch_mem_alloc( XACodecBase *base, UWORD32 core, UWORD32 priority )
{
    if ( (priority == 0) || (XF_CORE_DATA(core)->n_workers == 0) )
    {
        if ( XF_CORE_DATA(core)->scratch == NULL )
        {
          XF_CHK_ERR( XF_CORE_DATA(core)->scratch = xf_scratch_mem_init(core, priority, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_SCRATCH]), XAF_MEMORY_ERR);
        }
        base->scratch = XF_CORE_DATA(core)->scratch;

    }
    else
    {
        struct xf_worker *worker = XF_CORE_DATA(core)->worker + priority;

        if ( worker->scratch == NULL )
        {
            XF_CHK_ERR( worker->scratch = xf_scratch_mem_init(core, priority, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_SCRATCH]), XAF_MEMORY_ERR);

        }
        base->scratch = worker->scratch;
    }
    return XA_NO_ERROR;

}

/* ...codec pre-initialization */
static XA_ERRORCODE xa_base_preinit(XACodecBase *base, UWORD32 core)
{
    WORD32      n;

    /* ...codec must be empty */
    XF_CHK_ERR(base->state == 0, XA_API_FATAL_INVALID_CMD);

    /* ...get API structure size */
    XA_API(base, XA_API_CMD_GET_API_SIZE, 0, &n);

    /* ...allocate memory for codec API structure (4-bytes aligned) */
    XMALLOC(&base->api, n, 4, core, XAF_MEM_ID_COMP);

    /* ...set default config parameters */
    XA_API(base, XA_API_CMD_INIT, XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS, NULL);

    /* ...get memory info tables size */
    if (XA_API(base, XA_API_CMD_GET_MEMTABS_SIZE, 0, &n), n != 0)
    {
        /* ...allocate memory for tables (4-bytes aligned) */
        XMALLOC(&base->mem_tabs, n, 4, core, XAF_MEM_ID_COMP);

        /* ...set pointer for process memory tables */
        XA_API(base, XA_API_CMD_SET_MEMTABS_PTR, 0, base->mem_tabs.addr);
    }

    TRACE(INIT, _b("Codec[%p] pre-initialization completed"), base);

    return XA_NO_ERROR;
}

/* ...post-initialization setup */
static XA_ERRORCODE xa_base_postinit(XACodecBase *base, UWORD32 core)
{
    WORD32  n, i;

    /* ...issue post-config command and determine the buffer requirements */
    XA_API(base, XA_API_CMD_INIT, XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS, NULL);

    /* ...get number of memory tables required */
    XA_API(base, XA_API_CMD_GET_N_MEMTABS, 0, &n);

    /* ...No scratch memory */
    base->scratch_idx = -1;

    /* ...allocate memory buffers */
    for (i = 0; i < n; i++)
    {
        WORD32      size, align, type;

        TRACE(INIT, _b("i = %u (of %u)"), (UWORD32)i, (UWORD32)n);

        /* ...get memory type */
        XA_API(base, XA_API_CMD_GET_MEM_INFO_TYPE, i, &type);

        /* ...get memory size of i-th buffer */
        XA_API(base, XA_API_CMD_GET_MEM_INFO_SIZE, i, &size);

        /* ...get alignment */
        XA_API(base, XA_API_CMD_GET_MEM_INFO_ALIGNMENT, i, &align);

        /* ...process individual buffer */
        switch (type)
        {
        case XA_MEMTYPE_SCRATCH:

            /* ...scratch memory is shared among all codecs; check its validity */
            XF_CHK_ERR(size <= (WORD32)(XF_CORE_DATA(core)->worker_thread_scratch_size[base->component.priority]), XA_API_FATAL_MEM_ALLOC);

            /* ...make sure alignment is sane */
            XF_CHK_ERR((XF_CFG_CODEC_SCRATCHMEM_ALIGN & (align - 1)) == 0, XA_API_FATAL_MEM_ALIGN);

            /* ...Allocate scratch if scratch is not allocated for this thread */
            XA_CHK( xf_scratch_mem_alloc( base, core, base->component.priority));

            /* ...set the scratch memory pointer */
            XA_API(base, XA_API_CMD_SET_MEM_PTR, i, base->scratch);

            /* ... scratch memory index */
            base->scratch_idx = i;

            TRACE(INIT, _b("Mem tab %d: sz=%d al=%d ty=%d Scratch memory (%p)"), i, size, align, type, base->scratch);

            break;

        case XA_MEMTYPE_PERSIST:
            /* ...allocate persistent memory */
            XMALLOC(&base->persist, size, align, core, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_PERSIST]);

            /* ...and set the pointer instantly */
            XA_API(base, XA_API_CMD_SET_MEM_PTR, i, base->persist.addr);

            TRACE(INIT, _b("Mem tab %d: sz=%d al=%d ty=%d Persistent memory (%p)"), i, size, align, type, base->persist.addr);

            break;

        case XA_MEMTYPE_INPUT:
            /* ... if input port bypass is set, make input buffer size as zero */
            size = (base->component.inport_bypass)? 0:size;
        case XA_MEMTYPE_OUTPUT:
            /* ...input/output buffer specification; pass to codec function */
            CODEC_API(base, memtab, i, type, size, align, core);

            break;

        default:
            /* ...unrecognized memory type */
            TRACE(ERROR, _x("Invalid memory type: [%d]=(%u, %u, %u)"), i, type, size, align);
            return XA_API_FATAL_INVALID_CMD_TYPE;
        }
    }

    TRACE(INIT, _b("Codec[%p] post-initialization completed (api:%p[%u])"), base, base->api.addr, base->api.size);

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/

static WORD32 is_component_param(WORD32 id)
{
    return ((id == XAF_COMP_CONFIG_PARAM_PRIORITY) ||
            (id == XAF_COMP_CONFIG_PARAM_INPORT_BYPASS)
           );
}

static XA_ERRORCODE xa_send_msg_to_new_priority_queue(xf_core_data_t *cd, XACodecBase *base, UWORD32 curr_priority, UWORD32 new_priority)
{
    struct xf_worker *pcurr_worker = cd->worker + curr_priority;
    struct xf_worker *pnew_worker = cd->worker + new_priority;

    xf_worker_msg_t marker_msg, msg_tmp;

    /* ...creating a marker message */
    marker_msg.msg = NULL;
    marker_msg.component = NULL;

    __xf_msgq_send(pcurr_worker->queue, &marker_msg, sizeof(marker_msg));

    while (1)
    {
        int rc = __xf_msgq_recv(pcurr_worker->queue, &msg_tmp, sizeof(msg_tmp));

        if(((msg_tmp.component == NULL) && (msg_tmp.msg == NULL)) || rc)
        {
            /* ...break from loop once marker message is received */
            break;
        }
        if(msg_tmp.msg && (msg_tmp.component == &base->component))
        {
            TRACE(INFO, _b("Sending message of base:%p with id:%016llx buf:%p opcode:%x from older priority:%u queue to newer priority:%u queue"), base, (UWORD64)msg_tmp.msg->id, msg_tmp.msg->buffer, msg_tmp.msg->opcode, curr_priority, new_priority);
            __xf_msgq_send(pnew_worker->queue, &msg_tmp, sizeof(msg_tmp));
        }
        else
        {
            __xf_msgq_send(pcurr_worker->queue, &msg_tmp, sizeof(msg_tmp));
        }
    }

   return XA_NO_ERROR;
}

static XA_ERRORCODE xa_component_setparam(XACodecBase *base, WORD32 id, pVOID pv, UWORD32 core)
{
    UWORD32 *v = pv;
    xf_core_data_t *cd = XF_CORE_DATA(core);

    UWORD32 curr_priority = base->component.priority;

    switch (id) {
        case XAF_COMP_CONFIG_PARAM_PRIORITY:
        {
            UWORD32 sched_flag, new_priority, rtos_priority;

            if(XF_CORE_DATA(core)->n_workers <= 0) return XAF_INVALIDVAL_ERR;
        
            sched_flag = (base->state & XA_BASE_FLAG_SCHEDULE);
        
            /* ...check if the component priority request is valid. */
            XF_CHK_ERR((*v + 1) < XF_CORE_DATA(core)->n_workers, XAF_INVALIDVAL_ERR);
        
            if(sched_flag)
            {
                /* ...cancel any pending process on current priority. */
                xa_base_cancel(base);
            }
        
            new_priority = *v + 1;
        
            /* ...save current priority of thread */
            rtos_priority = __xf_thread_get_priority(NULL);
        
            /* ...elevate priority of thread */
            __xf_thread_set_priority(NULL, cd->dsp_thread_priority + 1);
        
            /* ...send message from current priority queue to new priority queue. */
            xa_send_msg_to_new_priority_queue(cd, base, curr_priority, new_priority);
        
            if ( (base->state & XA_BASE_FLAG_POSTINIT) && (base->scratch_idx != -1 ) )
            {
                XA_CHK( xf_scratch_mem_alloc( base, core, new_priority));
        
                XA_API(base, XA_API_CMD_SET_MEM_PTR, base->scratch_idx, base->scratch);
            }
        
            /* ...update the priority to the structure only after the memory allocation (if any)
             * to avoid changing scratch pointer of a component when its running on a low-priority thread */
            base->component.priority = new_priority;
        
            if(sched_flag)
            {
                /* ...resubmit the pending process with changed priority. */
                xa_base_schedule(base, 0);
            }
        
            /* ...put back thread to its older priority */
            __xf_thread_set_priority(NULL, rtos_priority);
        
            return XA_NO_ERROR;
        }
        
        case XAF_COMP_CONFIG_PARAM_INPORT_BYPASS:
        {
            base->component.inport_bypass = *(UWORD32 *) pv;

            return XA_NO_ERROR;
        }

        default:
            return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...SET-PARAM processing (enabled in all states) */
XA_ERRORCODE xa_base_set_param(XACodecBase *base, xf_message_t *m)
{
    xf_set_param_msg_t     *cmd = m->buffer;
    xf_set_param_item_t    *param = &cmd->item[0];
    WORD32                  n, i;
    WORD32                  error = 0;
    UWORD32 core = XF_MSG_DST_CORE(m->id);

    /* ...calculate total amount of parameters */
    n = m->length / sizeof(*param);

    /* ...check the message length is sane */
    XF_CHK_ERR(m->length == XF_SET_PARAM_CMD_LEN(n), XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...revert priority to default after set-config message is popped from worker queue at pre-init */
    if ((base->state & XA_BASE_FLAG_POSTINIT) == 0)
    {
        base->component.priority = 0;
    }

    /* ...apply all parameters; pass to codec-specific function */
    for (i = 0; i < n; i++)
    {
        TRACE(SETUP, _b("set-param[%p]: [%u]=%u"), base, param[i].id, param[i].value);

        if (is_component_param(param[i].id)) {
            XA_CHK(xa_component_setparam(base, param[i].id, &param[i].value, core));
        } else if (base->setparam)
        {
            error = CODEC_API_CRITICAL(base, setparam, param[i].id, &param[i].value);
        }
        else
        {
            error = XA_API_CRITICAL(base, XA_API_CMD_SET_CONFIG_PARAM, param[i].id, &param[i].value);
        }

        /* ...only the first error is captured*/
        if ((error > 0) && (m->error == 0))
            m->error = error;
    }

    /* ...check if we need to do post-initialization */
    if ((base->state & XA_BASE_FLAG_POSTINIT) == 0)
    {
        /* ...do post-initialization step */
        XA_CHK_CRITICAL(xa_base_postinit(base, XF_MSG_DST_CORE(m->id)));

        /* ...mark the codec static configuration is set */
        base->state ^= XA_BASE_FLAG_POSTINIT | XA_BASE_FLAG_RUNTIME_INIT;
    }

    /* ...complete message processing; output buffer is empty */
    xf_response_ok(m);

    return XA_NO_ERROR;
}

/* ...GET-PARAM message processing (enabled in all states) */
XA_ERRORCODE xa_base_get_param(XACodecBase *base, xf_message_t *m)
{
    xf_get_param_msg_t *cmd = m->buffer;
    UWORD32                *id = &cmd->c.id[0];
    UWORD32                *value = &cmd->r.value[0];
    UWORD32                 n, i;
    WORD32                  error = 0;

    /* ...calculate amount of parameters */
    n = m->length / sizeof(*id);

    /* ...check input parameter length */
    XF_CHK_ERR(XF_GET_PARAM_CMD_LEN(n) == m->length, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...retrieve the collection of codec  parameters */
    for (i = 0; i < n; i++)
    {
        /* ...place the result into same location */
        if (base->getparam)
        {
            error = CODEC_API_CRITICAL(base, getparam, id[i], &value[i]);
        }
        else
        {
            error = XA_API_CRITICAL(base, XA_API_CMD_GET_CONFIG_PARAM, id[i], &value[i]);
        }

        /* ...only the first error is captured*/
        if ((error > 0) && (m->error == 0))
                m->error = error;
    }

    /* ...complete message specifying output buffer size */
    xf_response_data(m, XF_GET_PARAM_RSP_LEN(n));

    return XA_NO_ERROR;
}

/* ...SET-PARAM-EXT processing (enabled in all states) */
XA_ERRORCODE xa_base_set_param_ext(XACodecBase *base, xf_message_t *m)
{
    xf_ext_param_msg_t *cmd = m->buffer;
    UWORD32                 length = m->length;
    UWORD32                 remaining = length;
    WORD32                  error = 0;

#if defined(XF_TRACE)
    UWORD16                 i;
    for (i = 0; TRACE_CFG(SETUP) && i < remaining; i += 16)
    {
        TRACE(SETUP, _b("[%03x]: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X"),
              i,
              ((UWORD8 *)m->buffer)[i + 0], ((UWORD8 *)m->buffer)[i + 1],
              ((UWORD8 *)m->buffer)[i + 2], ((UWORD8 *)m->buffer)[i + 3],
              ((UWORD8 *)m->buffer)[i + 4], ((UWORD8 *)m->buffer)[i + 5],
              ((UWORD8 *)m->buffer)[i + 6], ((UWORD8 *)m->buffer)[i + 7],
              ((UWORD8 *)m->buffer)[i + 8], ((UWORD8 *)m->buffer)[i + 9],
              ((UWORD8 *)m->buffer)[i + 10], ((UWORD8 *)m->buffer)[i + 11],
              ((UWORD8 *)m->buffer)[i + 12], ((UWORD8 *)m->buffer)[i + 13],
              ((UWORD8 *)m->buffer)[i + 14], ((UWORD8 *)m->buffer)[i + 15]);
    }
#endif

    /* ...process all parameters encapsulated in buffer */
    while (remaining >= sizeof(*cmd))
    {
        UWORD32     id = cmd->desc.id;
        UWORD32     dsize= cmd->desc.length;

        /* ...cut-off descriptor header */
        remaining -= sizeof(*cmd);

        TRACE(SETUP, _b("remaining:%u, desc_size:%u"), (UWORD32)remaining, (UWORD32)dsize);

        /* ...make sure length is sufficient */
        XF_CHK_ERR(remaining >= dsize, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...pad remaining bytes with zeroes */
//        (pad ? memset(cmd->data + dlen, 0, 4 - pad) : 0);

        TRACE(SETUP, _b("set-ext-param[%p]: [%u]:%u - [%02X:%02X:%02X:%02X:...]"), base, id, dsize, cmd->data[0], cmd->data[1], cmd->data[2], cmd->data[3]);

        if (base->setparam)
        {
            error = CODEC_API_CRITICAL(base, setparam, id, (void *) *(UWORD32 *)(cmd->data));
        }
        else
        {
            error = XA_API_CRITICAL(base, XA_API_CMD_SET_CONFIG_PARAM, id, (void *) *(UWORD32 *)(cmd->data));
        }

        /* ...only the first error is captured*/
        if ((error > 0) && (m->error == 0))
            m->error = error;

        /* ...move to next item (keep 4-bytes alignment for descriptor) */
        cmd = (xf_ext_param_msg_t *)(&cmd->data[0] + dsize), remaining -= dsize;
    }

    /* ...check the message is fully processed */
    XF_CHK_ERR(remaining == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...complete message processing; output buffer is empty */
    xf_response_ok(m);

    /* ...unfortunately, it looks like a bug of the library that updates the memory
     * and leaves it in a dirty state causing subsequent cache inconsistency - tbd
     */
    //xf_response_data(m, length);

    return XA_NO_ERROR;
}

/* ...GET-PARAM-EXT message processing (enabled in all states) */
XA_ERRORCODE xa_base_get_param_ext(XACodecBase *base, xf_message_t *m)
{
    xf_ext_param_msg_t *cmd = m->buffer;
    UWORD32                 length = m->length;
    UWORD32                 remaining = length;
    WORD32                  error = 0;

#if defined(XF_TRACE)
    UWORD32                 i;
    for (i = 0; TRACE_CFG(SETUP) && i < remaining; i += 16)
    {
        TRACE(SETUP, _b("[%03x]: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X"),
              i,
              ((UWORD8 *)m->buffer)[i + 0], ((UWORD8 *)m->buffer)[i + 1],
              ((UWORD8 *)m->buffer)[i + 2], ((UWORD8 *)m->buffer)[i + 3],
              ((UWORD8 *)m->buffer)[i + 4], ((UWORD8 *)m->buffer)[i + 5],
              ((UWORD8 *)m->buffer)[i + 6], ((UWORD8 *)m->buffer)[i + 7],
              ((UWORD8 *)m->buffer)[i + 8], ((UWORD8 *)m->buffer)[i + 9],
              ((UWORD8 *)m->buffer)[i + 10], ((UWORD8 *)m->buffer)[i + 11],
              ((UWORD8 *)m->buffer)[i + 12], ((UWORD8 *)m->buffer)[i + 13],
              ((UWORD8 *)m->buffer)[i + 14], ((UWORD8 *)m->buffer)[i + 15]);
    }
#endif

    /* ...process all parameters encapsulated in buffer */
    while (remaining >= sizeof(*cmd))
    {
        UWORD32     id = cmd->desc.id;
        UWORD32     dsize = cmd->desc.length;

        /* ...cut-off command header */
        remaining -= sizeof(*cmd);

        /* ...make sure data buffer has sufficient length */
        XF_CHK_ERR(remaining >= dsize, XA_API_FATAL_INVALID_CMD_TYPE);

        if (base->getparam)
        {
            error = CODEC_API_CRITICAL(base, getparam, id, (void *)*(UWORD32 *)(cmd->data));
        }
        else
        {
            error = XA_API_CRITICAL(base, XA_API_CMD_GET_CONFIG_PARAM, id, (void *)*(UWORD32 *)(cmd->data));
        }

        /* ...only the first error is captured*/
        if ((error > 0) && (m->error == 0))
            m->error = error;

        /* ...pad remaininig bytes with zeroes */
 //       (pad ? memset(cmd->data + len, 0, 4 - pad) : 0);

        TRACE(SETUP, _b("get-ext-param[%p]: [%u]:%u - [%02X:%02X:%02X:%02X:...]"), base, id, dsize, cmd->data[0], cmd->data[1], cmd->data[2], cmd->data[3]);

        /* ...move to next item (alignment issues? - tbd) */
        cmd = (xf_ext_param_msg_t *)(&cmd->data[0] + dsize), remaining -= dsize;
    }

    /* ...check the message is fully processed */
    XF_CHK_ERR(remaining == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...complete message processing; output buffer has the same length */
    xf_response_data(m, length);

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Command/data processing functions
 ******************************************************************************/

/* ...generic codec data processing */
static XA_ERRORCODE xa_base_process(XACodecBase *base)
{
    XA_ERRORCODE    error;
    WORD32          done=0;

    /* ...clear internal scheduling flag */
    base->state &= ~XA_BASE_FLAG_SCHEDULE;

    /* ...codec-specific preprocessing (buffer maintenance) */
    if ((error = CODEC_API(base, preprocess)) != XA_NO_ERROR)
    {
        /* ...return non-fatal codec error */
        return error;
    }

    /* ...execution step */
    if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
    {
        /* ...kick initialization process */
        XA_COMP_API(base, XA_API_CMD_INIT, XA_CMD_TYPE_INIT_PROCESS, NULL);

        /* ...check if initialization is completed */
        XA_COMP_API(base, XA_API_CMD_INIT, XA_CMD_TYPE_INIT_DONE_QUERY, &done);

        TRACE(EXEC, _b("Initialization result: %d"), done);

        /* ...switch to execution state if required */
        (done ? base->state ^= XA_BASE_FLAG_RUNTIME_INIT | XA_BASE_FLAG_EXECUTION : 0);
    }
    else if (base->state & XA_BASE_FLAG_EXECUTION)
    {
        TRACE(EXEC, _b("do exec"));

        /* ...execute decoding process */
        XA_COMP_API(base, XA_API_CMD_EXECUTE, XA_CMD_TYPE_DO_EXECUTE, NULL);

        /* ...check for end-of-stream condition */
        XA_COMP_API(base, XA_API_CMD_EXECUTE, XA_CMD_TYPE_DONE_QUERY, &done);

        TRACE(EXEC, _b("Execution result: %d"), done);

        /* ...mark the output path is done to release all queued buffers */
        (done ? base->state ^= XA_BASE_FLAG_EXECUTION | XA_BASE_FLAG_COMPLETED : 0);
    }

    /* ...codec-specific buffer post-processing */
    return CODEC_API(base, postprocess, done);
}

#ifndef XA_DISABLE_EVENT
static XA_ERRORCODE xa_base_event_handler(XACodecBase *base, UWORD32 event_id, XA_ERRORCODE error_code)
{
    xf_channel_info_t   *channel_info, *p_channel_info_curr = NULL;
    xf_channel_info_t   **pp_channel_info_curr;
    xf_message_t        *m;
    UWORD32             i, channel_found = 0;

    pp_channel_info_curr = &base->channel_info_chain;

    for(i=0; i<base->num_channels; i++)
    {
        p_channel_info_curr = *pp_channel_info_curr;

        if(p_channel_info_curr->event_id_src == event_id)
        {
            channel_found++;
            channel_info = p_channel_info_curr;

            if (channel_info->delete_msg != NULL)
            {
                TRACE(WARNING, _b("Event undelivered. Channel is being deleted. Event id: %d, base ptr: [%p] comp_type=%d"), event_id, base, base->comp_type);
                pp_channel_info_curr = &p_channel_info_curr->next;
                continue;
            }

            m = xf_msg_dequeue(&channel_info->queue);

            if (m == NULL)
            {
                TRACE(WARNING, _b("Event undelivered. Channel queue is empty for event id: %d, base ptr: [%p] "), event_id, base);
                pp_channel_info_curr = &p_channel_info_curr->next;
                continue;
            }

            /* ...reset message error field */
            m->error = 0;

            if (error_code)
            {
               m->error = error_code;
               *(UWORD32 *)m->buffer = event_id;
               memcpy((void *)(UWORD32)m->buffer + sizeof(channel_info->event_id_src), &error_code, sizeof(error_code));
               channel_info->event_buf_count--;
            }
            else
            {
                if (channel_info->event_id_dst == XF_CFG_ID_EVENT_TO_APP)
                {
                    *(UWORD32 *)m->buffer = event_id;
                    channel_info->event_buf_count--;
                }
                else
                {
                    *(UWORD32 *)m->buffer = channel_info->event_id_dst;
                }

                if (channel_info->buf_size)
                {
                    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, event_id, (void *)((UWORD32)m->buffer + sizeof(channel_info->event_id_dst)));
                }
            }

            /* ...complete message with specified amount of bytes produced */
            xf_response_data(m, (channel_info->buf_size + sizeof(channel_info->event_id_dst)));     // Tbd: should be +sizeof(event_id) for events to app
        }
        pp_channel_info_curr = &p_channel_info_curr->next;
    }

    if (!channel_found)
    {
        TRACE(WARNING, _b("Event not delivered, channel does not exist"));

        /* ...caller should handle this return value appropriately */
        return XAF_INVALIDVAL_ERR;
    }

    return XAF_NO_ERR;
}

static XA_ERRORCODE xa_base_error_handler(xf_component_t *component, XA_ERRORCODE error_code)
{
    XACodecBase    *base = (XACodecBase *) component;
    UWORD32         event_id = XF_CFG_COMP_ERR_FATAL;

    if (base->enable_non_fatal_err_reporting)
        event_id = XF_CFG_COMP_ERR_ALL;

    return (xa_base_event_handler(base, event_id, error_code));
}

static XA_ERRORCODE xa_base_raise_event_cb(xa_raise_event_cb_t *cdata, UWORD32 event_id)
{
    XACodecBase         *base = container_of(cdata, XACodecBase, cdata);

    if (event_id == XAF_COMP_CONFIG_PARAM_SELF_SCHED)
    {
        if ((base->state & XA_BASE_FLAG_POSTINIT) && !(base->state & XA_BASE_FLAG_COMPLETED))
        {
            xa_base_schedule(base, 0);
            return XAF_NO_ERR;
        }
        else
        {
            TRACE(WARNING, _b("self-sched request ignored, state check failure for base ptr: [%p] "), base);
            return XAF_API_ERR;
        }

    }
    else
    {
        return (xa_base_event_handler(base, event_id, 0));
    }
}

static XA_ERRORCODE xa_base_create_event_channel(XACodecBase *base, xf_message_t *m)
{
    xf_channel_info_t       *channel_info, *p_channel_info_curr = NULL;
    xf_channel_info_t       **pp_channel_info_curr;
    xf_event_channel_msg_t  *cmd = m->buffer;
    xf_message_t            *msg;
    UWORD32                 core = XF_MSG_DST_CORE(m->id);
    UWORD32                 i, err = 0;

    pp_channel_info_curr = &base->channel_info_chain;

    /*... Check if channel already exists for same parameters */
    for(i=0; i<base->num_channels; i++)
    {
        p_channel_info_curr = *pp_channel_info_curr;

        if ((p_channel_info_curr->event_id_src == cmd->src_cfg_param) && (p_channel_info_curr->event_id_dst == cmd->dst_cfg_param) && (p_channel_info_curr->port_dst == cmd->dst))
        {
            break;
        }
        pp_channel_info_curr = &p_channel_info_curr->next;
    }

    /*... Check if channel is found in channel chain, return error if found */
    XF_CHK_ERR(i == base->num_channels, XAF_INVALIDPTR_ERR);

    /*... return fatal error if callback function is not available in plugin */
    switch (cmd->src_cfg_param)
    {
        case XF_CFG_COMP_ERR_FATAL:
            break;
        case XF_CFG_COMP_ERR_ALL:
            base->enable_non_fatal_err_reporting = 1;
            break;
        default:
            XF_CHK_ERR ((base->cdata.cb != NULL), XAF_INVALIDVAL_ERR);
            break;
    }

    /* ... check if channel info allocation is ok */
    XF_CHK_ERR(channel_info = (xf_channel_info_t *) xf_mem_alloc(sizeof(xf_channel_info_t), XF_EVENT_CHANNEL_INFO_ALIGNMENT, core, 0 /* shared */, XAF_MEM_ID_COMP), XAF_MEMORY_ERR);

    /* ...initializing channel_info structure */
    memset(channel_info, 0, sizeof(xf_channel_info_t));

    channel_info->event_id_src      = cmd->src_cfg_param;
    channel_info->event_id_dst      = cmd->dst_cfg_param;
    channel_info->buf_size          = cmd->alloc_size;
    channel_info->port_dst          = cmd->dst;
    channel_info->delete_msg        = NULL;
    channel_info->shared_channel    = (XF_PORT_CORE(cmd->dst) != XF_PORT_CORE(cmd->src));

    /* ... reset message queue */
    xf_msg_queue_init(&channel_info->queue);

    if (channel_info->event_id_dst != XF_CFG_ID_EVENT_TO_APP)
    {
        /* ...for inter-component event channels, this buffer count remains fixed as provided by application(nbuf) until the channel is deleted.
         * for events-to-app channels this count would be incremented when buffers arrive from application and decremented when buffers leave DSP,
         * i.e, when an event is raised or when buffers are returned during channel delete */
        channel_info->event_buf_count   = cmd->alloc_number;

        /* ... abort if pool allocation fails */
        if ((xf_msg_pool_init(&channel_info->pool, channel_info->event_buf_count, core, channel_info->shared_channel, ((channel_info->shared_channel)?XAF_MEM_ID_DEV:XAF_MEM_ID_COMP))) < 0)
        {
            xf_mem_free(channel_info, sizeof(xf_channel_info_t), core, 0 /* shared */, XAF_MEM_ID_COMP);

            return XAF_MEMORY_ERR;
        }

        /* ...allocate required amount of buffers */
        for (i = 0; i < channel_info->event_buf_count; i++)
        {
            /* ...get message from pool (directly; bypass that "get" interface) */
            msg = xf_msg_pool_item(&channel_info->pool, i);

            /* ...wipe out message link pointer (debugging) */
            msg->next = NULL;

            /* ...set message parameters */
            msg->id     = __XF_MSG_ID(cmd->dst, cmd->src);
            msg->opcode = XF_EVENT;
            msg->length = channel_info->buf_size;
            msg->buffer = xf_mem_alloc((msg->length + sizeof(channel_info->event_id_dst)), cmd->alloc_align, core, channel_info->shared_channel, ((channel_info->shared_channel)?XAF_MEM_ID_DEV:XAF_MEM_ID_COMP));
            msg->error = 0;

            /* ...if allocation failed, do a cleanup */
            if (!msg->buffer)
            {
                err = 1;
                break;
            }
            /* ...place message into output port */
            xf_msg_enqueue(&channel_info->queue, msg);
        }

        if (err)
        {
            /* ...allocation failed; do a cleanup */
            while (i--)
            {
                msg = xf_msg_pool_item(&channel_info->pool, i);

                /* ...free item */
                xf_mem_free(msg->buffer, msg->length + sizeof(channel_info->event_id_dst), core, channel_info->shared_channel, ((channel_info->shared_channel)?XAF_MEM_ID_DEV:XAF_MEM_ID_COMP));
            }

            /* ...destroy pool data */
            xf_msg_pool_destroy(&channel_info->pool, core, channel_info->shared_channel, ((channel_info->shared_channel)?XAF_MEM_ID_DEV:XAF_MEM_ID_COMP));

            xf_mem_free(channel_info, sizeof(xf_channel_info_t), core, 0 /* shared */, XAF_MEM_ID_COMP);

            return XAF_MEMORY_ERR;
        }
    }

    /* ... update event channel chain */
    channel_info->next = base->channel_info_chain;
    base->channel_info_chain = channel_info;

    base->num_channels++;

    xf_response_ok(m);

    return XAF_NO_ERR;
}

static XA_ERRORCODE xa_base_delete_event_channel(XACodecBase *base, xf_message_t *m)
{
    xf_event_channel_delete_msg_t   *cmd = m->buffer;
    xf_channel_info_t               *channel_info, *p_channel_info_curr = NULL;
    xf_channel_info_t               **pp_channel_info_curr;
    xf_message_t                    *msg;
    UWORD32                         core = XF_MSG_DST_CORE(m->id);
    UWORD32                         i;

    /* ...channel look-up */
    pp_channel_info_curr = &base->channel_info_chain;

    for(i=0; i<base->num_channels; i++)
    {
        p_channel_info_curr = *pp_channel_info_curr;

        if ((p_channel_info_curr->event_id_src == cmd->src_cfg_param) && (p_channel_info_curr->event_id_dst == cmd->dst_cfg_param))
        {
            break;
        }

        pp_channel_info_curr = &p_channel_info_curr->next;
    }

    /*... Check if channel is found in channel chain*/
    XF_CHK_ERR(i < base->num_channels, XAF_INVALIDPTR_ERR);

    /* ...look-up successful */
    channel_info = p_channel_info_curr;

    while((msg = xf_msg_dequeue(&channel_info->queue)) != NULL)
    {
        if (channel_info->event_id_dst == XF_CFG_ID_EVENT_TO_APP)
        {
            *(UWORD32 *)msg->buffer =  channel_info->event_id_src;

            msg->length = sizeof(channel_info->event_id_src);

            xf_response(msg);
        }
        else
        {
            xf_mem_free(msg->buffer, (channel_info->buf_size + sizeof(channel_info->event_id_dst)), core, channel_info->shared_channel, ((channel_info->shared_channel)?XAF_MEM_ID_DEV:XAF_MEM_ID_COMP));
        }

        channel_info->event_buf_count--;
    }

    if (channel_info->event_buf_count)
    {
        /* ...some buffers are still in transit, save control-flow message until all event buffers are returned*/
        channel_info->delete_msg = m;
    }
    else
    {
        /* ...all buffers are accounted for, delete the channel */

        if (channel_info->event_id_dst != XF_CFG_ID_EVENT_TO_APP)
        {
            /* ...destroy pool data */
            xf_msg_pool_destroy(&channel_info->pool, core, channel_info->shared_channel, ((channel_info->shared_channel)?XAF_MEM_ID_DEV:XAF_MEM_ID_COMP));
        }

        --base->num_channels;

        /* ...remove channel from the chain */
        *pp_channel_info_curr = p_channel_info_curr->next;

        xf_mem_free(channel_info, sizeof(xf_channel_info_t), core, 0 /* shared */, XAF_MEM_ID_COMP);

        /* ...send success response to application */
        xf_response_ok(m);
    }

    return XAF_NO_ERR;
}

static XA_ERRORCODE xa_base_process_event(XACodecBase *base, xf_message_t *m)
{
    UWORD32     event_id = (UWORD32)*(UWORD32*)m->buffer;
    UWORD32     core = XF_MSG_SRC_CORE(m->id);

    /* ... event processing at dst component */
    if ((!XF_MSG_SRC_PROXY(m->id)) && (m->length > 0))
    {
        /* ...even if actual buffer size is 0, extra bytes are allocated to carry the event_id, this check identifies that */
        if (m->length > sizeof(event_id))
            XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, event_id, (void *)((UWORD32)m->buffer + sizeof(event_id)));
        else
            XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, event_id, NULL);

        /* ...send event response back to source */
        xf_response_ok(m);

        return XAF_NO_ERR;
    }
    else
    {
        /* ... event response at src component */

        xf_channel_info_t   *channel_info, *p_channel_info_curr = NULL;
        xf_channel_info_t   **pp_channel_info_curr;
        UWORD32             i;

        /* ...channel look-up */
        pp_channel_info_curr = &base->channel_info_chain;

        for(i=0; i<base->num_channels; i++)
        {
            p_channel_info_curr = *pp_channel_info_curr;

            if (((p_channel_info_curr->event_id_dst == event_id) && ((p_channel_info_curr->port_dst == XF_MSG_SRC(m->id)) && (!XF_MSG_SRC_PROXY(m->id)))) ||
                ((p_channel_info_curr->event_id_dst == XF_CFG_ID_EVENT_TO_APP) && (p_channel_info_curr->event_id_src == event_id))) break;

            pp_channel_info_curr = &p_channel_info_curr->next;
        }

        /*... Channel must be found in the channel chain*/
        BUG(i >= base->num_channels, _x("Event buffer source channel not found"));

        channel_info = p_channel_info_curr;

        if (channel_info->delete_msg == NULL)
        {
            xf_msg_enqueue(&channel_info->queue, m);

            if (channel_info->event_id_dst == XF_CFG_ID_EVENT_TO_APP)
                channel_info->event_buf_count++;
        }
        else
        {
            /* ...channel delete is initiated. */
            if (channel_info->event_id_dst == XF_CFG_ID_EVENT_TO_APP)
            {
                *(UWORD32 *)m->buffer =  channel_info->event_id_src;
                m->length = sizeof(channel_info->event_id_src);

                /* ...send buffer back to application */
                xf_response(m);
            }
            else
            {
                xf_mem_free(m->buffer, (channel_info->buf_size + sizeof(channel_info->event_id_dst)), core, channel_info->shared_channel, ((channel_info->shared_channel)?XAF_MEM_ID_DEV:XAF_MEM_ID_COMP));
            }

            if (--channel_info->event_buf_count == 0)
            {
                /* ...send channel delete response to application*/
                xf_response_ok(channel_info->delete_msg);

                if (channel_info->event_id_dst != XF_CFG_ID_EVENT_TO_APP)
                {
                    /* ...destroy pool data */
                    xf_msg_pool_destroy(&channel_info->pool, core, channel_info->shared_channel, ((channel_info->shared_channel)?XAF_MEM_ID_DEV:XAF_MEM_ID_COMP));
                }

                --base->num_channels;

                /* ...remove from the chain */
                *pp_channel_info_curr = p_channel_info_curr->next;

                /* ...delete channel data */
                xf_mem_free(channel_info, sizeof(xf_channel_info_t), core, 0 /* shared */, XAF_MEM_ID_COMP);
            }
        }
    }

    return XAF_NO_ERR;
}
#endif /* XA_DISABLE_EVENT */

/* ...message-processing function (component entry point) */
static int xa_base_command(xf_component_t *component, xf_message_t *m)
{
    XACodecBase    *base = (XACodecBase *) component;
    UWORD32         cmd;

    /* ...invoke data-processing function if message is null */
    if (m == NULL)
    {
        XA_ERRORCODE error;

        error = xa_base_process(base);
        XF_CHK_ERR(!XA_ERROR_SEVERITY(error), error);

        return 0;
    }

    /* ...process the command */
    TRACE(EXEC, _b("[%p]:state[%X]:(%X, %d, %p)"), base, base->state, m->opcode, m->length, m->buffer);

#ifndef XA_DISABLE_EVENT
    if ((cmd = XF_OPCODE_TYPE(m->opcode)) == XF_OPCODE_TYPE(XF_EVENT_CHANNEL_CREATE))
    {
        if(xa_base_create_event_channel(base, m) < 0)
            xf_response_err(m);

        return 0;
    }

    if ((cmd = XF_OPCODE_TYPE(m->opcode)) == XF_OPCODE_TYPE(XF_EVENT_CHANNEL_DELETE))
    {
        if(xa_base_delete_event_channel(base, m) < 0)
            xf_response_err(m);

        return 0;
    }

    if ((cmd = XF_OPCODE_TYPE(m->opcode)) == XF_OPCODE_TYPE(XF_EVENT))
    {
        return (xa_base_process_event(base, m));

    }
#endif
    /* ...bail out if this is forced termination command (I do have a map; maybe I'd better have a hook? - tbd) */
    if ((cmd = XF_OPCODE_TYPE(m->opcode)) == XF_OPCODE_TYPE(XF_UNREGISTER))
    {
        TRACE(INIT, _b("force component[%p] termination"), base);

        /* ...execute deinit process, ignore error as most components do not require and do not implement this API */
        XA_API_NORET(base, XA_API_CMD_DEINIT, 0, NULL);

#ifndef XA_DISABLE_EVENT
        if (base->cdata.cb)
        {
            /* ... disable event callback by the component */
            XA_API_NORET(base, XA_API_CMD_SET_CONFIG_PARAM, XAF_COMP_CONFIG_PARAM_EVENT_CB, NULL);

            /* ...reset base callback pointer as well */
            base->cdata.cb = NULL;
        }
#endif
#ifdef XF_MSG_ERR_HANDLING
        return XAF_UNREGISTER;
#else
        return -1;
#endif
    }

    /* ...check opcode is valid */
    XF_CHK_ERR(cmd < base->command_num, XAF_INVALIDVAL_ERR);

    /* ...and has a hook */
    XF_CHK_ERR(base->command[cmd] != NULL, XAF_INVALIDVAL_ERR);

    /* ...pass control to specific command */
    XF_CHK_ERR(!XA_ERROR_SEVERITY(base->command[cmd](base, m)), XAF_API_ERR);

    /* ...execution completed successfully */
    return 0;
}

/*******************************************************************************
 * Base codec API
 ******************************************************************************/

/* ...data processing scheduling */
void xa_base_schedule(XACodecBase *base, UWORD64 dts)
{
    if ((base->state & XA_BASE_FLAG_SCHEDULE) == 0)
    {
        if(base->state & XA_BASE_FLAG_COMP_FATAL_ERROR)
        {
            TRACE(EXEC, _b("codec[%p] skip processing after fatal error"), base);
            return; /* ...skip scheduling after fatal error, TENA_3023. */
        }

        /* ...and put scheduling flag */
        base->state |= XA_BASE_FLAG_SCHEDULE;

        xf_core_data_t *cd = XF_CORE_DATA(xf_component_core(&base->component));
        if(cd->n_workers)
        {
            /* ...schedule component task execution */
            xf_sched_put(&cd->worker[base->component.priority].sched, &base->component.task, dts);
            //xf_ipi_resume_dsp_isr(xf_component_core(&base->component));
        }
        else
        {
            /* ...schedule component task execution */
            xf_component_schedule(&base->component, dts);
        }
    }
    else
    {
        TRACE(EXEC, _b("codec[%p] processing pending"), base);
    }
}

/* ...cancel data processing */
void xa_base_cancel(XACodecBase *base)
{
    if (base->state & XA_BASE_FLAG_SCHEDULE)
    {
        xf_core_data_t *cd = XF_CORE_DATA(xf_component_core(&base->component));

        /* ...clear scheduling flag */
        base->state &= ~XA_BASE_FLAG_SCHEDULE;

        /* ...and cancel scheduled codec task, if node is on the schedule-tree */
        /* ...for local-schedule tree or single DSP-thread, a schedule node must be on the sched-tree and never in the worker queue */
        if(cd->n_workers)
        {
            if(xf_sched_cancel(&cd->worker[base->component.priority].sched, &base->component.task))
            {
                TRACE(EXEC, _b("codec[%p] processing cancelled priority:%d"), base, base->component.priority);
            }
        }
        else
        {
            if(!xf_sched_cancel(&cd->sched, &base->component.task))
            {
                TRACE(EXEC, _b("codec[%p] processing cancelled"), base);
            }
        }
    }
}

/* ...base codec destructor */
void xa_base_destroy(XACodecBase *base, UWORD32 size, UWORD32 core)
{
    /* ...deallocate all resources */
    xf_mm_free_buffer(&base->persist, core, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_PERSIST]);
    xf_mm_free_buffer(&base->mem_tabs, core, XAF_MEM_ID_COMP);
    xf_mm_free_buffer(&base->api, core, XAF_MEM_ID_COMP);

    /* ...destroy codec structure (and task) itself */
    xf_mem_free(base, size, core, 0, XAF_MEM_ID_COMP);

    TRACE(INIT, _b("codec[%p]:%u destroyed"), base, core);
}

/* ...generic codec initialization routine */
XACodecBase * xa_base_factory(UWORD32 core, UWORD32 size, xa_codec_func_t process)
{
    XACodecBase    *base;

    /* ...make sure the size is sane */
    XF_CHK_ERR(size >= sizeof(XACodecBase), NULL);

    /* ...allocate local memory for codec structure */
    XF_CHK_ERR(base = xf_mem_alloc(size, 0, core, 0, XAF_MEM_ID_COMP), NULL);

    /* ...reset codec memory */
    memset(base, 0, size);

    /* ...set low-level codec API function */
    base->process = process;

    /* ...set message processing function */
    base->component.entry = xa_base_command;

#ifndef XA_DISABLE_EVENT
    /* ...set fatal error processing function */
    base->component.error_handler = xa_base_error_handler;
#endif
    /* ...do basic initialization */
    if (xa_base_preinit(base, core) != XA_NO_ERROR)
    {
        /* ...initialization failed for some reason; do cleanup */
        xa_base_destroy(base, size, core);

        return NULL;
    }

    /* ...default memory pool type for component memories. */
    base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_INPUT]   = XAF_MEM_ID_COMP;
    base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]  = XAF_MEM_ID_COMP;
    base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_PERSIST] = XAF_MEM_ID_COMP;
    base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_SCRATCH] = XAF_MEM_ID_COMP;

#ifndef XA_DISABLE_EVENT
    /* ... register event callback function */
    base->cdata.cb = xa_base_raise_event_cb;
    if(XA_API_NORET(base, XA_API_CMD_SET_CONFIG_PARAM, XAF_COMP_CONFIG_PARAM_EVENT_CB, &(base->cdata.cb)))
    {
        /*... reset base callback function if callback could not be set successfully */
        base->cdata.cb = NULL;
    }
#endif

    /* ...set input port bypass(user configurable) default value to 0. */
    base->component.inport_bypass = 0;

    /* ...initialization completed successfully */
    TRACE(INIT, _b("Codec[%p]:%u initialized"), base, core);

    return base;
}
