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
 * xf-shmem.c
 *
 * DSP shared memory interface implementation
 ******************************************************************************/

#define MODULE_TAG                      SHMEM

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf-dp.h"

/*******************************************************************************
 * Local constants definitions
 ******************************************************************************/

/* ...local interface status change flag */
#define XF_PROXY_STATUS_LOCAL           (1 << 0)

/* ...remote status change notification flag */
#define XF_PROXY_STATUS_REMOTE          (1 << 1)

/*******************************************************************************
 * Internal helpers
 ******************************************************************************/

/* ...put message into proxy queue */
static inline void xf_msg_proxy_put(xf_message_t *m)
{
    UWORD32                 dst = XF_MSG_DST_CORE(m->id);
    xf_core_rw_data_t  *rw = XF_CORE_RW_DATA(dst);

    /* ...assure memory coherency if needed */
#if (XF_REMOTE_IPC_NON_COHERENT)
    {
        /* ...invalidate rw-shared memory region */
        XF_PROXY_INVALIDATE(rw, sizeof(*rw));

        /* ...put message into shared queue */
        xf_sync_enqueue(&rw->remote, m);

        /* ...flush both message and shared queue data */
        XF_PROXY_FLUSH(rw, sizeof(*rw));
        XF_PROXY_FLUSH(m, sizeof(*m));
    }
#else
    {
        /* ...no memory coherency concerns; just place a message in the queue */
        xf_sync_enqueue(&rw->remote, m);
    }
#endif //XF_REMOTE_IPC_NON_COHERENT

#if 1 //AR7
    //xf_ipi_assert is used only if other cores access rw->remote[core0]
#else
    /* ...assert IPI interrupt on target ("destination") core if needed */
    UWORD32  src = XF_MSG_SRC_CORE(m->id);
    if (dst != src) {
        xf_ipi_assert(dst);
    } else
#endif
    {
        /* ...assert event on Master core */
        xf_ipi_resume_dsp(dst);
    }
}

/* ...retrieve message from proxy queue */
static inline xf_message_t * xf_msg_proxy_get(UWORD32 core)
{
    xf_core_rw_data_t  *rw = XF_CORE_RW_DATA(core);
    xf_message_t       *m;

    /* ...assure memory coherency if needed */
#if (XF_REMOTE_IPC_NON_COHERENT)
    {
        /* ...invalidate rw-memory */
        XF_PROXY_INVALIDATE(rw, sizeof(*rw));

        /* ...dequeue message from response queue */
        m = xf_sync_dequeue(&rw->remote);

        /* ...flush rw memory */
        XF_PROXY_FLUSH(rw, sizeof(*rw));

        /* ...invalidate message data if found */
        if(m)
        {
            XF_PROXY_INVALIDATE(m, sizeof(*m));
        }
    }
#else
    {
        /* ...just dequeue message from response queue */
        m = xf_sync_dequeue(&rw->remote);
    }
#endif //XF_REMOTE_IPC_NON_COHERENT

    return m;
}

/*******************************************************************************
 * Internal functions definitions
 ******************************************************************************/

/* ...retrieve all incoming commands from shared memory ring-buffer */
static UWORD32 xf_shmem_process_input(UWORD32 core)
{
    xf_message_t   *m;
    UWORD32             status = 0;
    xf_msgq_t cmd_msgq;

    xf_core_ro_data_t  *ro = XF_CORE_RO_DATA(core);

    cmd_msgq = ro->ipc.cmd_msgq;

    /* ...process all committed commands */
    while (!__xf_msgq_empty(cmd_msgq))
    {
        xf_proxy_message_t command;

        /* ...allocate message; the call should not fail */
        if ((m = xf_msg_pool_get(&XF_CORE_RO_DATA(core)->pool)) == NULL)
            break;

        /* ...if queue was full, set global proxy update flag */
        if (__xf_msgq_full(cmd_msgq))
            status |= XF_PROXY_STATUS_REMOTE | XF_PROXY_STATUS_LOCAL;
        else
            status |= XF_PROXY_STATUS_LOCAL;

        /*  v-tbd ...synchronize memory contents */

        /* ...get oldest not processed command */
        XF_CHK_API(__xf_msgq_recv(cmd_msgq, (UWORD32 *) &command, sizeof(command)));

        /* ...fill message parameters */
        m->id = command.session_id;
        m->opcode = command.opcode;
        m->length = command.length;
        m->error  = 0;
        m->buffer = xf_ipc_a2b(core, command.address);

        TRACE(CMD, _b("C[%016llx]:(%08x,%u,%p,%d)"), (UWORD64)m->id, m->opcode, m->length, m->buffer, m->error);

        /* ...invalidate message buffer contents as required - not here - tbd */
#if (XF_REMOTE_IPC_NON_COHERENT)
        {
            (XF_OPCODE_CDATA(m->opcode) ? XF_PROXY_INVALIDATE(m->buffer, m->length) : 0);
        }
#endif //XF_REMOTE_IPC_NON_COHERENT

        /* ...and schedule message execution on proper core */
        xf_msg_submit(m);
    }

    return status;
}

/* ...send out all pending outgoing responses to the shared memory ring-buffer */
static UWORD32 xf_shmem_process_output(UWORD32 core)
{
    xf_message_t   *m;
    UWORD32             status = 0;
    xf_msgq_t resp_msgq;

    xf_core_ro_data_t  *ro = XF_CORE_RO_DATA(core);

    resp_msgq = ro->ipc.resp_msgq;

    /* ...while we have response messages and there's space to write out one */
    while (!__xf_msgq_full(resp_msgq))
    {
        xf_proxy_message_t     response;

        /* ...remove message from internal queue */
        if ((m = xf_msg_proxy_get(core)) == NULL)
            break;

        /* ...notify App Interface Layer each time we send it a message (only if it was empty?) */
        status = XF_PROXY_STATUS_REMOTE | XF_PROXY_STATUS_LOCAL;

#if 0
        /* ...need to decide on best strategy - tbd */
        if (XF_QUEUE_EMPTY(read_idx, write_idx))
            status |= XF_PROXY_STATUS_REMOTE | XF_PROXY_STATUS_LOCAL;
        else
            status |= XF_PROXY_STATUS_LOCAL;
#endif

        /* ...flush message buffer contents to main memory as required - too late - different core - tbd */
#if (XF_REMOTE_IPC_NON_COHERENT)
        {
            (XF_OPCODE_RDATA(m->opcode) ? XF_PROXY_FLUSH(m->buffer, m->length) : 0);
        }
#endif //XF_REMOTE_IPC_NON_COHERENT

        /* ...put the response message fields */
        response.session_id = m->id;
        response.opcode = m->opcode;
        response.length = m->length;
        response.error = m->error;
        response.address = xf_ipc_b2a(core, m->buffer);

        /* ...put the response in message queue */
        XF_CHK_API(__xf_msgq_send(resp_msgq, (UWORD32 *) &response, sizeof(response)));

        /* v-tbd ...flush the content of the caches to main memory */

        TRACE(RSP, _b("R[%016llx]:(%08x,%u,%p,%d)"), (UWORD64)m->id, m->opcode, m->length, m->buffer, m->error);

        /* ...return message back to the pool */
        xf_msg_pool_put(&XF_CORE_RO_DATA(core)->pool, m);
    }

    /* ...return interface status change flags */
    return status;
}

/*******************************************************************************
 * Entry points
 ******************************************************************************/

/* ...process local(DSP Interface Layer)/remote(App Interface Layer) shared memory interface status change */
void xf_shmem_process_queues(UWORD32 core)
{
    UWORD32     status;

    do
    {
        /* ...acknowledge/clear any pending incoming interrupt */
        XF_PROXY_SYNC_PEER(core);

        /* ...send out pending response messages (frees message buffers, so do it first) */
        status = xf_shmem_process_output(core);

        /* ...receive and forward incoming command messages (allocates message buffers) */
        status |= xf_shmem_process_input(core);

        /* ...assert remote mailbox interrupt if global update bit is set */
        if (status & XF_PROXY_STATUS_REMOTE)
        {
            xf_ipi_assert(core);
        }
    }
    while (status);
}

/* ...completion callback for message originating from App Interface Layer */
void xf_msg_proxy_complete(xf_message_t *m)
{
    /* ...place message into proxy response queue */
    xf_msg_proxy_put(m);
}

/* ...initialize shared memory interface (DSP Interface Layer) */
int xf_shmem_init(UWORD32 core)
{
    xf_core_rw_data_t  *rw = XF_CORE_RW_DATA(core);
    xf_core_ro_data_t  *ro = XF_CORE_RO_DATA(core);
    UWORD32 i;

    /* ...initialize local/remote message queues */
    xf_sync_queue_init(&rw->remote);

    /* ...initialize global message list */
    XF_CHK_API(xf_msg_pool_init(&ro->pool, XF_CFG_MESSAGE_POOL_SIZE, core, 1 /* shared */, (XF_CFG_CORES_NUM>1)?XAF_MEM_ID_DSP:XAF_MEM_ID_COMP));

    /* ...flush memory content as needed */
#if XF_REMOTE_IPC_NON_COHERENT
    XF_PROXY_FLUSH(rw, sizeof(*rw));
#endif

    /* ...system-specific initialization of IPC layer */
    XF_CHK_API(xf_ipc_init(core));

    for(i = XAF_MEM_ID_DEV+1 ; i <= XAF_MEM_ID_DEV_MAX; i++)
    {
        XF_CHK_API(xf_mm_init(&(XF_CORE_DATA(core)->shared_pool[i]), xf_g_dsp->mem_pool[i].pmem, xf_g_dsp->mem_pool[i].size));
        TRACE(INFO, _b("DSP frmwk memory pool type:%d size:%d [%p] initialized"), i, xf_g_dsp->mem_pool[i].size, xf_g_dsp->mem_pool[i].pmem);
    }

    TRACE(INIT, _b("SHMEM-%u subsystem initialized"), core);

    return 0;
}

/* ...deinitialize shared memory interface (DSP Interface Layer) */
int xf_shmem_deinit(UWORD32 core)
{
    xf_core_rw_data_t  *rw = XF_CORE_RW_DATA(core);
    UWORD32 i;

    /* ...initialize local/remote message queues */
    xf_sync_queue_deinit(&rw->remote);

   /* ...system-specific deinitialization of IPC layer */
    XF_CHK_API(xf_ipc_deinit(core));

    /* ...deinitialize per-core memory loop */
    for(i = XAF_MEM_ID_DEV+1; i <= XAF_MEM_ID_DEV_MAX; i++)
    {
        XF_CHK_API(xf_mm_deinit(&(XF_CORE_DATA(core)->shared_pool[i])));
    }//for(;i;)

    TRACE(INIT, _b("SHMEM-%u subsystem deinitialized"), core);

    return 0;
}
