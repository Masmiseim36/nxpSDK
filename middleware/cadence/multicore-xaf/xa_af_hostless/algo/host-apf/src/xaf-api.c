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
#include "xf.h"
#include "xaf-api.h"
#include "xaf-structs.h"
#include "xaf-version.h"
#include "xaf-threads-priority.h"
#if (XF_CFG_CORES_NUM > 1)
#include "xf-shared.h"
#endif

#define MODULE_TAG                      DEVAPI

/***** GLOBALS *****/
/* Info table */

#define TENA_2356   1

#define XAF_4BYTE_ALIGN    4
#define XAF_8BYTE_ALIGN    8
#define XAF_64BYTE_ALIGN    64

/* ...check null pointer */
#define XAF_CHK_PTR(ptr)                                     \
({                                                          \
    int __ret;                                              \
                                                            \
    if ((__ret = (int)(ptr)) == 0)                          \
    {                                                       \
        TRACE(ERROR, _x("Null pointer error: %d"), __ret);  \
        return XAF_INVALIDPTR_ERR;                               \
    }                                                       \
    __ret;                                                  \
})

/* ...check range */
#define XAF_CHK_RANGE(val, min, max)                         \
({                                                          \
    int __ret = val;                                        \
                                                            \
    if ((__ret < (int)min) || (__ret > (int)max))           \
    {                                                       \
        TRACE(ERROR, _x("Invalid value: %d"), __ret);       \
        return XAF_INVALIDVAL_ERR;                           \
    }                                                       \
    __ret;                                                  \
})

#define XAF_CHK_MIN(val, min)                               \
({                                                          \
    int __ret = val;                                        \
                                                            \
    if (__ret < (int)min)                                   \
    {                                                       \
        TRACE(ERROR, _x("Invalid value: %d"), __ret);       \
        return XAF_INVALIDVAL_ERR;                          \
    }                                                       \
    __ret;                                                  \
})

/* ...check alignment */
#define XAF_CHK_ALIGN(val, bytes)                                                                 \
({                                                                                                \
    int __ret = (int) val;                                                                        \
                                                                                                  \
    if ((__ret) & (bytes - 1))                                                                    \
    {                                                                                             \
        TRACE(ERROR, _x("Invalid alignment: %d. Required alignment: %d bytes"), __ret, bytes);    \
        return XAF_INVALIDVAL_ERR;                                                                 \
    }                                                                                             \
    __ret;                                                                                        \
})

#define XAF_ADEV_STATE_CHK(ptr, adev_st, lock_ptr)                  \
({                                                                  \
    XAF_CHK_RANGE(adev_st, XAF_ADEV_RESET, XAF_ADEV_INIT);          \
    if ((ptr)->adev_state == adev_st)                               \
    {                                                               \
        TRACE(ERROR, _x("Device API Error"));                       \
        if(lock_ptr != NULL) __xf_unlock(lock_ptr);                 \
        return XAF_API_ERR;                                         \
    };                                                              \
})

#define XAF_COMP_STATE_CHK(ptr)                                 \
({                                                              \
    if ((ptr)->comp_state != XAF_COMP_CREATE)                   \
    {                                                           \
        TRACE(ERROR, _x("Component API Error"));                \
        return XAF_API_ERR;                                     \
    };                                                          \
})

#define XAF_COMP_STATE_CHK_WITH_LOCK(ptr)                       \
({                                                              \
    if ((ptr)->comp_state != XAF_COMP_CREATE)                   \
    {                                                           \
        __xf_unlock(&xf_g_ap->g_comp_delete_lock);              \
        TRACE(ERROR, _x("Component API Error"));                \
        return XAF_API_ERR;                                     \
    };                                                          \
})

/* Minimum component memory requirement is based on:
 * (common scratch bytes) + (internal housekeeping data-structure bytes) + (XF_CFG_MESSAGE_POOL_SIZE*(sizeof msg 64b-aligned)(256*64) bytes)
 * as referred to in ProgrammersGuide */
#if (XF_CFG_CORES_NUM > 1)
#define XA_AUDIO_COMP_BUF_SIZE_MIN      (3*1024)  /* ...minumum size required for xf_dsp_t structure per core, reduced memory when components are only on worker cores */
#else
#define XA_AUDIO_COMP_BUF_SIZE_MIN      (pconfig->worker_thread_scratch_size[0] + 3*1024 + 256*64)
#endif
#define XA_AUDIO_COMP_BUF_SIZE_FAST_MIN 128 /* ...secondary comp mem pools which can be of lesser size */

/* Minimum framework memory requirement is based on:
 * ipc-buffer 16384 bytes
 * as referred to in ProgrammersGuide */
#define XA_AUDIO_FRMWK_BUF_SIZE_MIN         (16384)
#define XA_AUDIO_FRMWK_BUF_SIZE_FAST_MIN    128

typedef struct xf_frmwk_mem_s{
    int nitems;
    int size;
    int align;
} xf_frmwk_mem_t;

xf_ap_t    *xf_g_ap;
xf_lock_t g_adev_close_lock;

extern void *dsp_thread_entry(void  *arg);
extern void *dsp_worker_entry(void  *arg);
extern const int xf_io_ports[XAF_MAX_COMPTYPE][2];

#ifndef XA_DISABLE_EVENT

#define XF_EVENT_BUFFER_ALIGNMENT           8
#define XF_COMP_ERROR_CHANNEL_BUF_SIZE      4
#define XF_EVENT_DST_CFG_PARAM_UNDEFINED    0xFFFFFFFF
#define XF_MAX_EVENT_BUFS_PER_COMP          16
#define XF_MAX_ERROR_EVENT_BUFS_PER_COMP    4

typedef enum
{
    XF_EVENT_CHANNEL_STATE_ACTIVE     =  0,
    XF_EVENT_CHANNEL_STATE_INACTIVE   =  1
} xf_event_channel_state;

struct xf_app_event_channel
{
    xf_app_event_channel_t  *next;
    xf_pool_t               *pool;
    xaf_comp_t              *p_comp_src;
    xaf_comp_t              *p_comp_dst;
    UWORD32                 event_id_src;
    UWORD32                 event_id_dst;
    UWORD32                 pending_buffers;
    xf_event_channel_state  channel_state;
};
#else /* XA_DISABLE_EVENT */

#define XF_MAX_EVENT_BUFS_PER_COMP          0
#define XF_MAX_ERROR_EVENT_BUFS_PER_COMP    0

#endif /* XA_DISABLE_EVENT */

#ifndef XF_CFG_MAX_COMPS
#define XF_CFG_MAX_COMPS                    16
#endif

#define XAF_COMMON_RTOS_IPC_MSGQ_NUM        (2 + 2 + 1) /* ... 2 for cmd/resp with DSP + 2 for synchronous respnose proxy->application + 1 per component for async response proxy->application */
#define _MAX(a,b)                           (((a)>(b))?(a):(b))
#define XAF_MAX_OUTBUFS                     1
#define XAF_MAX_PROBE_BUFS                  1
#define MAX_EVENTS_PER_COMP                 4
#define MAX_EVENT_CHANNELS_TO_APP           (XF_CFG_MAX_COMPS * MAX_EVENTS_PER_COMP)

//#define XA_FRMWK_LOCAL_BUF_SIZE_MIN         ((sizeof(xaf_adev_t) +(XAF_4BYTE_ALIGN-1)) + sizeof(xf_ap_t)+(XAF_8BYTE_ALIGN-1))
#ifdef HAVE_FREERTOS
#define XA_FRMWK_LOCAL_BUF_SIZE_MIN         (5*1024)
#else
#define XA_FRMWK_LOCAL_BUF_SIZE_MIN         (26*1024)
#endif

/* ...Maximum number of memory pools supported(to avoid errors from test-application, no restrictions otherwise) */
#define XAF_MEM_ID_DEV_MAX_POOLS	        8
#define XAF_MEM_ID_COMP_MAX_POOLS	        8

/* ****************************************************
 * Generic node chain operations
 * ***************************************************/
static int xaf_sync_chain_init(xaf_node_chain_t *chain, UWORD32 next_offset)
{
    chain->head = NULL;
    __xf_lock_init(&chain->lock);
    chain->next_offset = next_offset;

    return 0;
}

static int xaf_sync_chain_deinit(xaf_node_chain_t *chain)
{
    chain->head = NULL;
    __xf_lock_destroy(&chain->lock);

    return 0;
}

static int xaf_sync_chain_add_node(xaf_node_chain_t *chain, void *node)
{
    __xf_lock(&chain->lock);

    *(xaf_node_chain_t **)((UWORD32)node + chain->next_offset) = chain->head;
    chain->head = (xaf_node_chain_t *)node;

    __xf_unlock(&chain->lock);

    return 0;
}

static int xaf_sync_chain_delete_node(xaf_node_chain_t *chain, void *node)
{
    xaf_node_chain_t *p_curr_node = NULL;
    xaf_node_chain_t **pp_curr_node;
    UWORD32 next_offset = chain->next_offset;

    __xf_lock(&chain->lock);

    pp_curr_node = &chain->head;

    do
    {
        p_curr_node = *pp_curr_node;

        if (!p_curr_node) break;

        if (p_curr_node == node) break;

        pp_curr_node = (xaf_node_chain_t **)((UWORD32)p_curr_node + next_offset);

    } while(1);

    if (p_curr_node != node)
    {
        __xf_unlock(&chain->lock);
        return -1;
    }

    *pp_curr_node = *(xaf_node_chain_t **)((UWORD32)p_curr_node + next_offset);

    __xf_unlock(&chain->lock);

    return 0;
}

/* ****************************************************
 * component chain specific node chain operations
 * ***************************************************/
static inline void *xaf_sync_chain_get_head(xaf_node_chain_t *chain)
{
    void *p_node;

    __xf_lock(&chain->lock);
    p_node = chain->head;
    __xf_unlock(&chain->lock);

    return p_node;
}

#ifndef XA_DISABLE_EVENT
/* ****************************************************
 * Event channel specific node chain operations
 * ***************************************************/
static int xaf_sync_chain_set_node_state(xaf_node_chain_t *chain, xf_app_event_channel_t *node, xf_event_channel_state state)
{
    __xf_lock(&chain->lock);

    node->channel_state = state;

    __xf_unlock(&chain->lock);

    return 0;
}

static xf_event_channel_state xaf_sync_chain_get_node_state(xaf_node_chain_t *chain, xf_app_event_channel_t *node)
{
    xf_event_channel_state state;

    __xf_lock(&chain->lock);

    state = node->channel_state;

    __xf_unlock(&chain->lock);

    return state;
}

static xf_app_event_channel_t * xaf_sync_chain_find_node_by_comp(xaf_node_chain_t *chain, pVOID p_comp)
{
    xf_app_event_channel_t *curr_node = NULL;

    __xf_lock(&chain->lock);

    curr_node = (xf_app_event_channel_t *)chain->head;

    do
    {
        if (!curr_node) break;

        if ((curr_node->p_comp_src == p_comp) || (curr_node->p_comp_dst == p_comp)) break;

        curr_node = curr_node->next;
    } while(1);

    __xf_unlock(&chain->lock);

    return curr_node;
}

static xf_app_event_channel_t * xaf_sync_chain_find_node_by_param(xaf_node_chain_t *chain, pVOID p_src, UWORD32 id_src, UWORD32 id_dst)
{
    xf_app_event_channel_t *curr_node = NULL;

    __xf_lock(&chain->lock);

    curr_node = (xf_app_event_channel_t *)chain->head;

    do
    {
        if (!curr_node) break;

        if (id_dst == XF_EVENT_DST_CFG_PARAM_UNDEFINED)
        {
            if ((curr_node->p_comp_src == p_src) && (curr_node->event_id_src == id_src)) break;
        }
        else
        {
            if ((curr_node->p_comp_src == p_src) && (curr_node->event_id_src == id_src) && (curr_node->event_id_dst == id_dst)) break;
        }

        curr_node = curr_node->next;
    } while(1);

    __xf_unlock(&chain->lock);

    return curr_node;
}

/* ...receive the event from proxy and relay it to the application with matching component handle */
static XAF_ERR_CODE xaf_event_relay(xa_app_submit_event_cb_t *cdata, xf_msg_id_dtype comp_id, UWORD32 event_id, void *event_buf, UWORD32 buf_size, WORD32 error)
{
    xaf_adev_t *p_adev = container_of(cdata, xaf_adev_t, cdata);
    xaf_comp_t *p_comp = NULL;
    UWORD32 i, ncomp;
    UWORD32 fatal_error = (error < 0) ? 1 : 0;
    ncomp = p_adev->n_comp;

    __xf_lock(&p_adev->comp_chain.lock);
    if ((ncomp > 0) && (p_adev->comp_chain.head != NULL))
    {
        p_comp = (xaf_comp_t *)p_adev->comp_chain.head;
        for(i = 0; i < ncomp; i++)
        {
            if((NULL == p_comp) || (p_comp->handle.id == comp_id))
                break;

            p_comp = p_comp->next;
        }

        if(i == ncomp)
            p_comp = NULL;
    }
    __xf_unlock(&p_adev->comp_chain.lock);

    if (p_comp)
    {
        xf_app_event_channel_t   *p_channel_curr = NULL;

        p_channel_curr = xaf_sync_chain_find_node_by_param(&p_adev->event_chain, p_comp, event_id, XF_EVENT_DST_CFG_PARAM_UNDEFINED);

        XF_CHK_ERR((p_channel_curr != NULL), XAF_INVALIDPTR_ERR);

        p_channel_curr->pending_buffers--;

        if ((xaf_sync_chain_get_node_state(&p_adev->event_chain, p_channel_curr)) == XF_EVENT_CHANNEL_STATE_ACTIVE)
        {
            /* ...submit the event to application via callback */
            if (xf_g_ap->app_event_handler_cb)
            {
#ifdef XF_MSG_ERR_HANDLING
                if (error &&
                    ((event_id == XF_CFG_COMP_ERR_FATAL) || (event_id == XF_CFG_COMP_ERR_ALL)))
                {
                    /* ...update error buffer with error code to prevent application accessing stale data */
                    *(UWORD32*)(event_buf + sizeof(event_id)) = error;
                }

                xf_g_ap->app_event_handler_cb((void *)p_comp, event_id, (void *)((UWORD32)(event_buf + sizeof(event_id))), (buf_size - sizeof(event_id)), ((error) || (event_id == XF_CFG_COMP_ERR_FATAL) || (event_id == XF_CFG_COMP_ERR_ALL)));
#else
                xf_g_ap->app_event_handler_cb((void *)p_comp, event_id, (void *)((UWORD32)(event_buf + sizeof(event_id))), (buf_size - sizeof(event_id)), ((event_id == XF_CFG_COMP_ERR_FATAL) || (event_id == XF_CFG_COMP_ERR_ALL)));
#endif
            }

            /* ...resend the buffer to DSP unless there's an fatal error or the channel is inactive */
            if (!fatal_error && ((xaf_sync_chain_get_node_state(&p_adev->event_chain, p_channel_curr)) == XF_EVENT_CHANNEL_STATE_ACTIVE))
            {
                XF_CHK_API(xf_command(&p_comp->handle, 0, XF_EVENT, event_buf, buf_size));
                p_channel_curr->pending_buffers++;
            }
            else
            {
                TRACE(INFO, _b("event-relay: Skip sending event buffer back to DSP"));
            }
        }
    }
    else
    {
        TRACE(WARNING, _b("event-relay: Unknown event, ignored, event_id: %08x"), event_id);
    }

    return XAF_NO_ERR;
}
#endif /* XA_DISABLE_EVENT */


XAF_ERR_CODE xaf_malloc(void **buf_ptr, int size, int pool_id)
{
    XAF_CHK_PTR(buf_ptr);

    xf_buffer_t *b;
    xf_pool_t *pool = xf_g_ap->pfrmwk_pool_free[pool_id];
    /* ...take buffer from a head of the free list */
    if ((b = pool->free))
    {
        /* ...advance free list head */
        pool->free = b->link.next, b->link.pool = pool;

        *buf_ptr = xf_buffer_data(b);
        memset((*buf_ptr), 0, size);

        b->link.next = xf_g_ap->pfrmwk_pool_alloc[pool_id]->free;
        xf_g_ap->pfrmwk_pool_alloc[pool_id]->free = b;

        TRACE(BUFFER, _b("malloc pool_id:%d buf:%p size:%d"), pool_id, *buf_ptr, size);
    }
    else
    {
        *buf_ptr = NULL;
        return XAF_INVALIDPTR_ERR;
    }

    if((UWORD32)(*buf_ptr) & (XAF_4BYTE_ALIGN - 1))
    {
       TRACE(INFO, _b("Memory allocation failed : %p is not 4-byte aligned size:%d pool_id:%d"), *buf_ptr, size, pool_id);
       return XAF_INVALIDPTR_ERR;
    }

    return XAF_NO_ERR;
}

void xaf_free(void *buf, int pool_id)
{
    xf_buffer_t *b, *b_prev;
    xf_pool_t  *pool_free;

    b_prev = NULL;
    b = xf_g_ap->pfrmwk_pool_alloc[pool_id]->free;
    while(b)
    {
       if(buf == xf_buffer_data(b))
        {
            /* ...update free pool */
            //pool_free = b->link.pool;
            pool_free = xf_g_ap->pfrmwk_pool_free[pool_id];

            /* ...update alloc pool */
            if (b_prev)
            {
                b_prev->link.next = b->link.next;
            }
            else
            {
                xf_g_ap->pfrmwk_pool_alloc[pool_id]->free = b->link.next;
            }

            b->link.next = pool_free->free, pool_free->free = b;

            TRACE(BUFFER, _b("free pool_id:%d buf:%p size:%d"), pool_id, buf, pool_free->length);
            return;
        }
        b_prev = b;
        b = b->link.next;
    }

    if(b == NULL)
    {
        TRACE(INFO, _b("Memory Free failed buf:%p pool_id:%d"), buf, pool_id);
    }
    return;
}

int xaf_frmwk_mem_init(void)
{
    xf_pool_t *p;
    xf_buffer_t    *b;
    void *data, *pbuf;
    int number, size;
    int i, nitems, isize, align;
    int msgq_size_max = 0;

#ifdef HAVE_XOS
    int msgq_size;
    /* ...allocate buffer */
    msgq_size = __xf_msgq_get_size(SEND_MSGQ_ENTRIES, sizeof(xf_proxy_msg_t));
    msgq_size_max = _MAX(msgq_size, msgq_size_max);
    msgq_size = __xf_msgq_get_size(RECV_MSGQ_ENTRIES, sizeof(xf_proxy_msg_t));
    msgq_size_max = _MAX(msgq_size, msgq_size_max);
    msgq_size = __xf_msgq_get_size(SEND_LOCAL_MSGQ_ENTRIES, sizeof(xf_user_msg_t));
    msgq_size_max = _MAX(msgq_size, msgq_size_max);
#endif //ifdef HAVE_XOS

    xf_frmwk_mem_t frmwk_mem[] = {
        {
            XF_CFG_MAX_COMPS, offset_of(xf_pool_t, buffer) + sizeof(xaf_comp_t), XAF_8BYTE_ALIGN, /* ...component structure */
        },
        {
            (XF_CFG_MAX_COMPS + XAF_COMMON_RTOS_IPC_MSGQ_NUM), offset_of(xf_pool_t, buffer) + msgq_size_max, 1,   /* ...msgq */
        },
        {
            1, offset_of(xf_pool_t, buffer) + XAF_AUX_POOL_SIZE * sizeof(xf_buffer_t), 1,  /* ...aux pool */
        },
        {
            XF_CFG_MAX_COMPS, (offset_of(xf_pool_t, buffer) + XAF_MAX_INBUFS * sizeof(xf_buffer_t)), 1, /* ...input buffer pool */
        },
        {
            XF_CFG_MAX_COMPS * (XAF_MAX_OUTBUFS + XAF_MAX_PROBE_BUFS), (offset_of(xf_pool_t, buffer) + XAF_MAX_OUTBUFS * sizeof(xf_buffer_t)), 1, /* ... output, probe buffer pool */
        },
#ifndef XA_DISABLE_EVENT
        {
            MAX_EVENT_CHANNELS_TO_APP, offset_of(xf_pool_t, buffer) + sizeof(xf_app_event_channel_t), 1,   /* ...event channel */
        },
        {
            XF_CFG_MAX_COMPS, (offset_of(xf_pool_t, buffer) + XF_MAX_EVENT_BUFS_PER_COMP * sizeof(xf_buffer_t)), 1, /* ...event buffer pool */
        },
#endif //XA_DISABLE_EVENT
        {
            XF_CFG_MAX_COMPS, (offset_of(xf_pool_t, buffer) + XAF_EXT_CFG_POOL_SIZE * sizeof(xf_buffer_t)), 1, /* ...get/set config ext pool */
        },
    };

    /* ...create linked lists from frmwk_local memory */
    for(i=0; i<XF_POOL_FRMWK_MAX; i++)
    {
#if !defined(HAVE_XOS)
        if(i == XF_POOL_FRMWK_MSGQ)
            continue;
#endif

        nitems = frmwk_mem[i].nitems;
        align = frmwk_mem[i].align;
        isize = (frmwk_mem[i].size + align - 1) & ~(align - 1);

        /* ...allocate pool (free or unallocated pool init) */
        p = (xf_pool_t *)(((unsigned int)&xf_g_ap->pfrmwk_local_buffer[xf_g_ap->frmwk_local_memory_curr] + XAF_4BYTE_ALIGN - 1) & ~(XAF_4BYTE_ALIGN - 1));
        size = ((offset_of(xf_pool_t, buffer) + nitems * sizeof(xf_buffer_t))) + ((unsigned int)p - (unsigned int)&xf_g_ap->pfrmwk_local_buffer[xf_g_ap->frmwk_local_memory_curr]);
        if(xf_g_ap->frmwk_local_memory_size < (xf_g_ap->frmwk_local_memory_curr + size))
        {
            return XAF_INVALIDPTR_ERR;
        }
        xf_g_ap->frmwk_local_memory_curr += size;

        /* ...allocate buffer */
        pbuf = (void *)(((unsigned int)&xf_g_ap->pfrmwk_local_buffer[xf_g_ap->frmwk_local_memory_curr] + align - 1) & ~(align - 1));
        //size = offset_of(xf_pool_t, buffer) + nitems * (isize + align - 1) + ((unsigned int)pbuf - (unsigned int)&xf_g_ap->pfrmwk_local_buffer[xf_g_ap->frmwk_local_memory_curr]);
        size = nitems * (isize + align - 1) + ((unsigned int)pbuf - (unsigned int)&xf_g_ap->pfrmwk_local_buffer[xf_g_ap->frmwk_local_memory_curr]);
        if(xf_g_ap->frmwk_local_memory_size < (xf_g_ap->frmwk_local_memory_curr + size))
        {
            return XAF_INVALIDPTR_ERR;
        }
        xf_g_ap->frmwk_local_memory_curr += size;
        p->p = pbuf;
        p->number = nitems;
        p->length = isize;
        p->proxy = NULL; //or proxy handle?

        /* ...create individual buffers and link them into free list */
        for (p->free = b = &p->buffer[0], data = p->p, number = p->number; number > 0; number--, b++)
        {
            /* ...set address of the buffer (no length there) */
            b->address = data;
            /* ...file buffer into the free list */
            b->link.next = b + 1;
            /* ...advance data pointer in contiguous buffer */
            data += p->length;
        }
        /* ...terminate list of buffers (not too good - tbd) */
        b[-1].link.next = NULL;
        xf_g_ap->pfrmwk_pool_free[i] = p;

        /* ...allocate pool (alloc or allocated pool init) */
        p = (xf_pool_t *)(((unsigned int)&xf_g_ap->pfrmwk_local_buffer[xf_g_ap->frmwk_local_memory_curr] + XAF_4BYTE_ALIGN - 1) & ~(XAF_4BYTE_ALIGN - 1));
        size = (offset_of(xf_pool_t, buffer)) + ((unsigned int)p - (unsigned int)&xf_g_ap->pfrmwk_local_buffer[xf_g_ap->frmwk_local_memory_curr]);
        if(xf_g_ap->frmwk_local_memory_size < (xf_g_ap->frmwk_local_memory_curr + size))
        {
            return XAF_INVALIDPTR_ERR;
        }
        xf_g_ap->frmwk_local_memory_curr += size;
        p->free = NULL;
        p->number = 0;
        p->length = isize;
        p->proxy = NULL; //or proxy handle?
        xf_g_ap->pfrmwk_pool_alloc[i] = p;

        xf_g_ap->frmwk_local_memory_peak = xf_g_ap->frmwk_local_memory_curr;
    }

    return XAF_NO_ERR;
}

int xaf_frmwk_mem_deinit(void)
{
    int i, nitems, isize, align;
    int msgq_size_max = 0;

#ifdef HAVE_XOS
    int msgq_size;
    /* ...allocate buffer */
    msgq_size = __xf_msgq_get_size(SEND_MSGQ_ENTRIES, sizeof(xf_proxy_msg_t));
    msgq_size_max = _MAX(msgq_size, msgq_size_max);
    msgq_size = __xf_msgq_get_size(RECV_MSGQ_ENTRIES, sizeof(xf_proxy_msg_t));
    msgq_size_max = _MAX(msgq_size, msgq_size_max);
    msgq_size = __xf_msgq_get_size(SEND_LOCAL_MSGQ_ENTRIES, sizeof(xf_user_msg_t));
    msgq_size_max = _MAX(msgq_size, msgq_size_max);
#endif //ifdef HAVE_XOS

    xf_frmwk_mem_t frmwk_mem[] = {
        {
            XF_CFG_MAX_COMPS, offset_of(xf_pool_t, buffer) + sizeof(xaf_comp_t), XAF_8BYTE_ALIGN, /* ...component structure */
        },
        {
            (XF_CFG_MAX_COMPS + XAF_COMMON_RTOS_IPC_MSGQ_NUM), offset_of(xf_pool_t, buffer) + msgq_size_max, 1,   /* ...msgq */
        },
        {
            1, offset_of(xf_pool_t, buffer) + XAF_AUX_POOL_SIZE * sizeof(xf_buffer_t), 1,  /* ...aux pool */
        },
        {
            XF_CFG_MAX_COMPS, (offset_of(xf_pool_t, buffer) + XAF_MAX_INBUFS * sizeof(xf_buffer_t)), 1, /* ...input buffer pool */
        },
        {
            XF_CFG_MAX_COMPS * (XAF_MAX_OUTBUFS + XAF_MAX_PROBE_BUFS), (offset_of(xf_pool_t, buffer) + XAF_MAX_OUTBUFS * sizeof(xf_buffer_t)), 1, /* ... output, probe buffer pool */
        },
#ifndef XA_DISABLE_EVENT
        {
            MAX_EVENT_CHANNELS_TO_APP, offset_of(xf_pool_t, buffer) + sizeof(xf_app_event_channel_t), 1,   /* ...event channel */
        },
        {
            XF_CFG_MAX_COMPS, (offset_of(xf_pool_t, buffer) + XF_MAX_EVENT_BUFS_PER_COMP * sizeof(xf_buffer_t)), 1, /* ...event buffer pool */
        },
#endif //XA_DISABLE_EVENT
        {
            XF_CFG_MAX_COMPS, (offset_of(xf_pool_t, buffer) + XAF_EXT_CFG_POOL_SIZE * sizeof(xf_buffer_t)), 1, /* ...get/set config ext pool */
        },
    };

    /* ...release frmwk_local memory of linked lists */
    for(i=0; i<XF_POOL_FRMWK_MAX; i++)
    {
#if !defined(HAVE_XOS)
        if(i == XF_POOL_FRMWK_MSGQ)
            continue;
#endif

        nitems = frmwk_mem[i].nitems;
        align = frmwk_mem[i].align;
        isize = (frmwk_mem[i].size + align - 1) & ~(align - 1);

        /* ... release buffer */
        //xf_g_ap->frmwk_local_memory_curr -= offset_of(xf_pool_t, buffer) + nitems * (isize + align - 1);
        xf_g_ap->frmwk_local_memory_curr -= nitems * (isize + align - 1);

        /* ... release pool (free list) */
        xf_g_ap->frmwk_local_memory_curr -= ((offset_of(xf_pool_t, buffer) + nitems * sizeof(xf_buffer_t)));

        /* ... release pool (alloc list) */
        xf_g_ap->frmwk_local_memory_curr -= (offset_of(xf_pool_t, buffer));
    }

    return XAF_NO_ERR;
}

XAF_ERR_CODE xaf_get_verinfo(pUWORD8 ver_info[3])
{
    static const xa_api_info_t xa_af_info_tbl =
    {
        "Copyright (c) 2016 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED", /* Copyright string */
        LIBNAME " (Hostless)",            /* Library name */
        LIBVERSION,         /* Library version */
        LIB_APIVERSION      /* API version */
    };

    XAF_CHK_PTR(ver_info);

    ver_info[0] = (pUWORD8)xa_af_info_tbl.p_lib_name;
    ver_info[1] = (pUWORD8)xa_af_info_tbl.p_lib_ver;
    ver_info[2] = (pUWORD8)xa_af_info_tbl.p_api_ver;

    XAF_CHK_PTR(*ver_info[0]);
    XAF_CHK_PTR(*ver_info[1]);
    XAF_CHK_PTR(*ver_info[2]);

    return XAF_NO_ERR;
}

static void xaf_comp_response(xf_handle_t *h, xf_user_msg_t *msg)
{
    /* ...submit response to asynchronous delivery queue */
    xf_response_put(h, msg);
}

static XAF_ERR_CODE xaf_comp_post_init_config(xaf_adev_t *p_adev, xaf_comp_t *p_comp, void *p_msg)
{
    xf_proxy_t *p_proxy = &p_adev->proxy;
    xf_start_msg_t *smsg = p_msg;
    UWORD32 i;

    p_comp->out_format.sample_rate   = smsg->sample_rate;
    p_comp->out_format.channels      = smsg->channels;
    p_comp->out_format.pcm_width     = smsg->pcm_width;

    TRACE(INFO, _b("Component[%016llx] Params: f=%d, c=%d, w=%d"), (UWORD64)p_comp->handle.id, smsg->sample_rate, smsg->channels, smsg->pcm_width);
    for (i=0; i<p_comp->inp_ports; i++)
    {
        p_comp->out_format.input_length[i]  = smsg->input_length[i];
        TRACE(INFO, _b("Component[%016llx] Params: i[%d]=%d"), (UWORD64)p_comp->handle.id, i, smsg->input_length[i]);
    }
    for (i=0; i<p_comp->out_ports; i++)
    {
        p_comp->out_format.output_length[i] = smsg->output_length[i];
        TRACE(INFO, _b("Component[%016llx] Params: o[%d]=%d"), (UWORD64)p_comp->handle.id, i, smsg->output_length[i]);
    }

    if (p_comp->noutbuf)
    {
        XF_CHK_API(xf_pool_alloc(p_proxy, p_comp->noutbuf, smsg->output_length[0], p_comp->handle.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_APP_OUTPUT], &p_comp->outpool, XF_POOL_FRMWK_OUTPUT));
#if 1 //DISCONNECT
        for (i=0; i<p_comp->noutbuf; i++)
        {
            xf_buffer_t *p_buf;
            p_buf = xf_buffer_get(p_comp->outpool);
            XAF_CHK_PTR(p_buf);
            p_comp->pout_buf[i] = xf_buffer_data(p_buf);
        }
#endif
    }

    if (p_comp->probe_enabled)
    {
        xf_buffer_t *p_buf;

        p_comp->probe_length = smsg->probe_length;
        XF_CHK_API(xf_pool_alloc(p_proxy, 1, smsg->probe_length, p_comp->handle.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_APP_OUTPUT], &p_comp->probepool, XF_POOL_FRMWK_OUTPUT));

        p_buf = xf_buffer_get(p_comp->probepool);
        XAF_CHK_PTR(p_buf);
        p_comp->probe_buf = xf_buffer_data(p_buf);
    }

    p_comp->init_done   = 1;
    p_comp->comp_status = XAF_INIT_DONE;

    /* ...return buffer to proxy */
    xf_buffer_put(p_comp->start_buf);

    return XAF_NO_ERR;
}


XAF_ERR_CODE xaf_adev_config_default_init(xaf_adev_config_t *padev_config)
{
    XAF_CHK_PTR(padev_config);
    UWORD32 i;

    memset(padev_config, 0, sizeof(xaf_adev_config_t));

    /* ...initialize adev default config params */
    padev_config->mem_pool[XAF_MEM_ID_DEV].size = XA_AUDIO_FRMWK_BUF_SIZE_MIN + 256*1024; /* ...this pool is used for IPC, hence the type is not configurable */
    padev_config->mem_pool[XAF_MEM_ID_DEV].mem_id = XAF_MEM_ID_DEV; /* ...this pool is used for IPC, hence the type is not configurable */
    for(i = XAF_MEM_ID_DEV+1; i <= XAF_MEM_ID_DEV_MAX; i++)
    {
        padev_config->mem_pool[i].size = 512*1024;
        padev_config->mem_pool[i].mem_id = i;
        /* ...mem pointers are null and should be set by the application properly */
    }

    for(i = XAF_MEM_ID_COMP; i <= XAF_MEM_ID_COMP_MAX; i++)
    {
        padev_config->mem_pool[i].size = 512*1024;
        padev_config->mem_pool[i].mem_id = i;
        /* ...mem pointers are null and should be set by the application properly */
    }

    padev_config->proxy_thread_priority = XAF_PROXY_THREAD_PRIORITY;

    padev_config->dsp_thread_priority = XAF_DSP_THREAD_PRIORITY;

    for(i=0; i<XAF_MAX_WORKER_THREADS; i++)
    {
	    padev_config->worker_thread_scratch_size[i] = XF_CFG_CODEC_SCRATCHMEM_SIZE;
    }

    padev_config->dsp_thread_stack_size = DSP_THREAD_STACK_SIZE;
    padev_config->proxy_thread_stack_size = PROXY_THREAD_STACK_SIZE;
    for(i=0; i<XAF_MAX_WORKER_THREADS; i++)
    {
	    padev_config->worker_thread_stack_size[i] = WORKER_THREAD_STACK_SIZE;
    }

    /* ...Memory required by framework app-interface for internal use */
    padev_config->framework_local_buffer_size = XA_FRMWK_LOCAL_BUF_SIZE_MIN;

    return XAF_NO_ERR;
}

XAF_ERR_CODE xaf_adev_open(pVOID *pp_adev, xaf_adev_config_t *pconfig)
{
    WORD32 ret, size, i, k;
    xaf_adev_t *p_adev;
    void * pTmp;
    xf_proxy_t *p_proxy;
    UWORD32 core = XF_CORE_ID_MASTER;

    XAF_CHK_PTR(pp_adev);
    XF_CHK_ERR((xf_g_ap == NULL), XAF_INVALIDPTR_ERR);

    UWORD32 dsp_thread_priority;
    UWORD32 proxy_thread_priority;
    UWORD32 xaf_hk_mem_cumulative = 0; /* ... cumutaive malloc size for XAF local housekeeping */

    XAF_CHK_RANGE(XAF_MEM_ID_DEV_MAX, 0, (XAF_MEM_ID_DEV_MAX_POOLS-1));
    XAF_CHK_RANGE((XAF_MEM_ID_COMP_MAX - (XAF_MEM_ID_DEV_MAX + 1)), 0, (XAF_MEM_ID_COMP_MAX_POOLS-1));

    XAF_CHK_PTR(pconfig);
    dsp_thread_priority = pconfig->dsp_thread_priority;
    proxy_thread_priority = pconfig->proxy_thread_priority;
    core = pconfig->core;

#ifndef XA_DISABLE_EVENT
    xaf_app_event_handler_fxn_t app_event_handler_cb = pconfig->app_event_handler_cb;
#endif

    XAF_CHK_RANGE(core, 0, (XF_CFG_CORES_NUM-1));

    XAF_CHK_MIN(pconfig->framework_local_buffer_size, XA_FRMWK_LOCAL_BUF_SIZE_MIN);
    XAF_CHK_PTR(pconfig->pframework_local_buffer);

    /* ...Thumb rule: DSP-thread priority should be less than proxy-thread priority */
#if defined(HAVE_XOS)
    XAF_CHK_RANGE(proxy_thread_priority, 1, (XOS_NUM_PRIORITY-1));
#endif
#if defined(HAVE_FREERTOS)
    XAF_CHK_RANGE(proxy_thread_priority, 1, configMAX_PRIORITIES);
#endif
    XAF_CHK_RANGE(dsp_thread_priority, 0, (proxy_thread_priority-1));

    //Memory allocation for adev struct pointer
    size = (sizeof(xaf_adev_t) +(XAF_4BYTE_ALIGN-1));
    //pTmp = mem_malloc(size, XAF_MEM_ID_DEV);
    pTmp = &((UWORD8 *)pconfig->pframework_local_buffer)[xaf_hk_mem_cumulative];
    XAF_CHK_PTR(pTmp);
    memset(pTmp, 0, size);
    xaf_hk_mem_cumulative += size;

    p_adev = (xaf_adev_t *) (((UWORD32)pTmp + (XAF_4BYTE_ALIGN-1))& ~(XAF_4BYTE_ALIGN-1));
    p_adev->adev_ptr = pTmp;
    *pp_adev = (void *)p_adev;

    p_adev->adev_state = XAF_ADEV_INIT;

    // App Interface Layer memory allocation (BSS)
    size = sizeof(xf_ap_t)+(XAF_8BYTE_ALIGN-1);
    //p_adev->p_apMem = mem_malloc(size, XAF_MEM_ID_DEV);
    p_adev->p_apMem = &((UWORD8 *)pconfig->pframework_local_buffer)[xaf_hk_mem_cumulative];
    XAF_CHK_PTR(p_adev->p_apMem);
    memset(p_adev->p_apMem, 0, size);
    xaf_hk_mem_cumulative += size;

    xf_g_ap = (xf_ap_t *) (((UWORD32)p_adev->p_apMem + (XAF_8BYTE_ALIGN-1))& ~(XAF_8BYTE_ALIGN-1));

    xf_g_ap->core = core;

    /* ..init framework local memory for components, msgq and events */
    xf_g_ap->pfrmwk_local_buffer = pconfig->pframework_local_buffer;
    xf_g_ap->frmwk_local_memory_size = pconfig->framework_local_buffer_size;

    //xaf_hk_mem_cumulative = ((xaf_hk_mem_cumulative + (XAF_4BYTE_ALIGN-1)) & ~(XAF_4BYTE_ALIGN-1));
    xf_g_ap->frmwk_local_memory_curr = xaf_hk_mem_cumulative;
    xf_g_ap->frmwk_local_memory_peak = xaf_hk_mem_cumulative;

#ifndef XA_DISABLE_EVENT
    xf_g_ap->cdata = &p_adev->cdata;
    xf_g_ap->app_event_handler_cb = app_event_handler_cb;

    p_adev->cdata.cb = xaf_event_relay;
#endif
    /* ...call-back function pointer for worker-thread cycles */
    xf_g_ap->cb_compute_cycles = pconfig->cb_compute_cycles;
    xf_g_ap->cb_stats = pconfig->cb_stats;

    // DSP shared memory allocation
#if (XF_CFG_CORES_NUM > 1)
    XAF_CHK_PTR(pconfig->mem_pool[XAF_MEM_ID_DSP].pmem);
    for(i = XAF_MEM_ID_DSP; i <= XAF_MEM_ID_DSP_MAX; i++)
    {
        for(k=0; (k < XAF_MEM_ID_MAX); k++)
        {
            if((pconfig->mem_pool[k].mem_id == i) && (pconfig->mem_pool[i].pmem))
            {
                xf_g_ap->mem_pool[i].size = ((pconfig->mem_pool[i].size + (XF_PROXY_ALIGNMENT-1)) & (~(XF_PROXY_ALIGNMENT-1)));
                XAF_CHK_ALIGN(xf_g_ap->mem_pool[i].size, XF_PROXY_ALIGNMENT);
                XAF_CHK_MIN(xf_g_ap->mem_pool[i].size, ( (i == XAF_MEM_ID_DEV) ? XA_AUDIO_FRMWK_BUF_SIZE_MIN : XA_AUDIO_FRMWK_BUF_SIZE_FAST_MIN));//TENA_2352, TENA_2191
            
                xf_g_ap->mem_pool[i].mem_id = pconfig->mem_pool[i].mem_id;
                xf_g_ap->mem_pool[i].pmem = (UWORD8 *) (((UWORD32)pconfig->mem_pool[i].pmem + (XF_PROXY_ALIGNMENT-1)) & ~(XF_PROXY_ALIGNMENT-1)); /* ...alignment adjustment is done on the DSP side */
                TRACE(INFO, _x("c[%d] dsp_shmem type:%d size:%d pmem:%p"), core, i, xf_g_ap->mem_pool[i].size, xf_g_ap->mem_pool[i].pmem);
            }
        }//for(;k;)
    }
#endif    // #if XF_CFG_CORES_NUM > 1

    // DSP Interface Layer memory allocation (BSS)
    xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_MEM_POOL] = xf_g_ap->mem_pool;
    for(i = XAF_MEM_ID_DEV; i <= XAF_MEM_ID_DEV_MAX; i++)
    {
        for(k=0; (k < XAF_MEM_ID_MAX); k++)
        {
            if(pconfig->mem_pool[k].mem_id == i)
            {
                xf_g_ap->mem_pool[k].size = pconfig->mem_pool[k].size & (~(XAF_64BYTE_ALIGN-1));
                XAF_CHK_ALIGN(xf_g_ap->mem_pool[k].size, XAF_64BYTE_ALIGN);//TENA_2352, TENA_2191
                XAF_CHK_MIN(xf_g_ap->mem_pool[k].size, ( (i == XAF_MEM_ID_DEV) ? XA_AUDIO_FRMWK_BUF_SIZE_MIN : XA_AUDIO_FRMWK_BUF_SIZE_FAST_MIN));//TENA_2352, TENA_2191
                
                xf_g_ap->mem_pool[k].pmem = pconfig->mem_pool[k].pmem;
                xf_g_ap->mem_pool[k].mem_id = pconfig->mem_pool[k].mem_id;
                if(xf_g_ap->mem_pool[k].pmem)
                {
                    memset(xf_g_ap->mem_pool[k].pmem, 0, xf_g_ap->mem_pool[k].size);
                }
                break;
            }
        }//for(;k;)
    }

    for(i = XAF_MEM_ID_COMP; i <= XAF_MEM_ID_COMP_MAX; i++)
    {
        for(k=0; (k < XAF_MEM_ID_MAX); k++)
        {
            if(pconfig->mem_pool[k].mem_id == i)
            {
                xf_g_ap->mem_pool[k].size = pconfig->mem_pool[k].size & (~(XAF_64BYTE_ALIGN-1));
                XAF_CHK_MIN(xf_g_ap->mem_pool[k].size, ( (i == XAF_MEM_ID_COMP) ? XA_AUDIO_COMP_BUF_SIZE_MIN : XA_AUDIO_COMP_BUF_SIZE_FAST_MIN));//TENA_2352, TENA_2191
                XAF_CHK_ALIGN(xf_g_ap->mem_pool[k].size, XAF_64BYTE_ALIGN);//TENA_2352, TENA_2191
                
                xf_g_ap->mem_pool[k].pmem = pconfig->mem_pool[k].pmem;
                xf_g_ap->mem_pool[k].mem_id = pconfig->mem_pool[k].mem_id;
                if(xf_g_ap->mem_pool[k].pmem)
                {
                    memset(xf_g_ap->mem_pool[k].pmem, 0, xf_g_ap->mem_pool[k].size);
                }
                break;
            }
        }//for(;k;)
    }

    xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_CORE_ID] = &xf_g_ap->core;
    xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_COMP_BUF_PEAK] = &xf_g_ap->dsp_comp_buf_size_peak;
    xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_COMP_BUF_CURR] = &xf_g_ap->dsp_comp_buf_size_curr;
    xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_FRMWK_BUF_PEAK] = &xf_g_ap->dsp_frmwk_buf_size_peak;
    xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_FRMWK_BUF_CURR] = &xf_g_ap->dsp_frmwk_buf_size_curr;
    xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_SHMEM_BUF_PEAK] = &xf_g_ap->dsp_shmem_buf_size_peak;
    xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_SHMEM_BUF_CURR] = &xf_g_ap->dsp_shmem_buf_size_curr;
    xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_CB_THREAD_STATS] = xf_g_ap->cb_compute_cycles;
    xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_WORKER_THREAD_STATS] = (xaf_perf_stats_t *)xf_g_ap->cb_stats;

    __xf_lock_init(&xf_g_ap->g_msgq_lock);

    xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_WORKER_SCRATCH_SIZE] = xf_g_ap->worker_thread_scratch_size;
    {
		UWORD32 *p_worker_scratch, j;

        p_worker_scratch = (UWORD32 *)xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_WORKER_SCRATCH_SIZE];
        for(j=0; j<XAF_MAX_WORKER_THREADS; j++)
        {
		    p_worker_scratch[j] = pconfig->worker_thread_scratch_size[j];
        }
    }

    xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_WORKER_STACK_SIZE] = xf_g_ap->worker_thread_stack_size;
    {
		UWORD32 *p_worker_stack, j;

        p_worker_stack = (UWORD32 *)xf_g_ap->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_WORKER_STACK_SIZE];
        for(j=0; j<XAF_MAX_WORKER_THREADS; j++)
        {
		    p_worker_stack[j] = pconfig->worker_thread_stack_size[j];
        }
    }

    p_adev->dsp_thread_priority = dsp_thread_priority;
#if defined(HAVE_FREERTOS)
    ret = __xf_thread_create(&xf_g_ap->dsp_thread, dsp_thread_entry, (void *) xf_g_ap->dsp_thread_args, "DSP-thread", NULL, pconfig->dsp_thread_stack_size, dsp_thread_priority);
#else
    /* if((ret = xaf_malloc(&(xf_g_ap->dsp_stack), pconfig->dsp_thread_stack_size, XAF_MEM_ID_DEV)))
        return ret;
    */
    {
        void *p = (void *)(((unsigned int)&xf_g_ap->pfrmwk_local_buffer[xf_g_ap->frmwk_local_memory_curr] + XAF_4BYTE_ALIGN - 1) & ~(XAF_4BYTE_ALIGN - 1));
        size = pconfig->dsp_thread_stack_size + ((unsigned int)p - (unsigned int)&xf_g_ap->pfrmwk_local_buffer[xf_g_ap->frmwk_local_memory_curr]);
        if(xf_g_ap->frmwk_local_memory_size < (xf_g_ap->frmwk_local_memory_curr + size))
        {
            return XAF_INVALIDPTR_ERR;
        }
        xf_g_ap->frmwk_local_memory_curr += size;
        xf_g_ap->frmwk_local_memory_peak += size;
        xf_g_ap->dsp_stack = p;
    }
    ret = __xf_thread_create(&xf_g_ap->dsp_thread, dsp_thread_entry, (void *) xf_g_ap->dsp_thread_args, "DSP-thread", xf_g_ap->dsp_stack, pconfig->dsp_thread_stack_size, dsp_thread_priority);
#endif
    if (ret != 0)
        return XAF_RTOS_ERR;

    p_proxy = &p_adev->proxy;

    p_proxy->proxy_thread_priority = proxy_thread_priority;

    p_proxy->proxy_thread_stack_size = pconfig->proxy_thread_stack_size;
#if !defined(HAVE_FREERTOS)
    /* if((ret = xaf_malloc(&(xf_g_ap->proxy_thread_stack), pconfig->proxy_thread_stack_size, XAF_MEM_ID_DEV)))
        return ret;
    */
    {
        void *p = (void *)(((unsigned int)&xf_g_ap->pfrmwk_local_buffer[xf_g_ap->frmwk_local_memory_curr] + XAF_4BYTE_ALIGN - 1) & ~(XAF_4BYTE_ALIGN - 1));
        size = pconfig->proxy_thread_stack_size + ((unsigned int)p - (unsigned int)&xf_g_ap->pfrmwk_local_buffer[xf_g_ap->frmwk_local_memory_curr]);
        if(xf_g_ap->frmwk_local_memory_size < (xf_g_ap->frmwk_local_memory_curr + size))
        {
            return XAF_INVALIDPTR_ERR;
        }
        xf_g_ap->frmwk_local_memory_curr += size;
        xf_g_ap->frmwk_local_memory_peak += size;
        xf_g_ap->proxy_thread_stack = p;
        xf_g_ap->dsp_thread_stack_size = pconfig->dsp_thread_stack_size;
    }
#endif

    /* ...linked list init to be done before calling proxy_init */
    if(xaf_frmwk_mem_init())
    {
        return XAF_INVALIDPTR_ERR;
    }
    xf_g_ap->xaf_memory_used = xf_g_ap->frmwk_local_memory_curr; /* ..frmwk local memory used */

    /* ...open DSP proxy - specify "DSP#0" */
    XF_CHK_API(xf_proxy_init(p_proxy, core));

    /* ...create auxiliary buffers pool for control commands */
    XF_CHK_API(xf_pool_alloc(p_proxy, XAF_AUX_POOL_SIZE, XAF_AUX_POOL_MSG_LENGTH, XAF_MEM_ID_DEV, &p_proxy->aux, XF_POOL_FRMWK_AUX));

#if TENA_2356
    /* ...mutex for orderly comp deletion. */
    __xf_lock_init(&xf_g_ap->g_comp_delete_lock);
#endif
    __xf_lock_init(&g_adev_close_lock);


#ifndef XA_DISABLE_EVENT
    xaf_sync_chain_init(&p_adev->event_chain, (UWORD32)offset_of(xf_app_event_channel_t, next));
#endif
    xaf_sync_chain_init(&p_adev->comp_chain, (UWORD32)offset_of(xaf_comp_t, next));

    return XAF_NO_ERR;
}

#if (XF_CFG_CORES_NUM > 1)
XAF_ERR_CODE xaf_dsp_open(pVOID *pp_adev, xaf_adev_config_t *pconfig)
{
    UWORD32 ret, size, i, k;
    xaf_adsp_t *p_adev;
    void * pTmp;
    xf_dsp_t *xf_g_dsp;
    UWORD32 core = XF_CORE_ID_MASTER;

    XAF_CHK_PTR(pp_adev);

    UWORD32 dsp_thread_priority;
    UWORD32 xaf_hk_mem_cumulative = 0; /* ... cumutaive malloc size for XAF local housekeeping */

    XAF_CHK_PTR(pp_adev);
    XAF_CHK_PTR(pconfig);

    XAF_CHK_RANGE(XAF_MEM_ID_DEV_MAX, 0, (XAF_MEM_ID_DEV_MAX_POOLS-1));
    XAF_CHK_RANGE((XAF_MEM_ID_COMP_MAX - (XAF_MEM_ID_DEV_MAX + 1)), 0, (XAF_MEM_ID_COMP_MAX_POOLS-1));

    dsp_thread_priority = pconfig->dsp_thread_priority;
    core = pconfig->core;
    XAF_CHK_RANGE(core, 0, (XF_CFG_CORES_NUM-1));

    XAF_CHK_MIN(pconfig->framework_local_buffer_size, XA_FRMWK_LOCAL_BUF_SIZE_MIN);

    //Memory allocation for adev struct pointer
    size = (sizeof(xaf_adsp_t) +(XAF_4BYTE_ALIGN-1));
    //pTmp = mem_malloc(size, XAF_MEM_ID_DEV);
    pTmp = &((UWORD8 *)pconfig->pframework_local_buffer)[xaf_hk_mem_cumulative];
    XAF_CHK_PTR(pTmp);
    memset(pTmp, 0, size);
    xaf_hk_mem_cumulative += size;

    p_adev = (xaf_adsp_t *) (((UWORD32)pTmp + (XAF_4BYTE_ALIGN-1))& ~(XAF_4BYTE_ALIGN-1));
    p_adev->adev_ptr = pTmp;
    *pp_adev = (void *)p_adev;

    p_adev->adev_state = XAF_ADEV_INIT;

    //DSP memory allocation (BSS)
    size = sizeof(xf_dsp_t)+(XAF_8BYTE_ALIGN-1);
    //p_adev->p_dspMem = mem_malloc(size, XAF_MEM_ID_DEV);
    p_adev->p_dspMem = &((UWORD8 *)pconfig->pframework_local_buffer)[xaf_hk_mem_cumulative];
    XAF_CHK_PTR(p_adev->p_dspMem);
    memset(p_adev->p_dspMem, 0, size);
    xaf_hk_mem_cumulative += size;

    xf_g_dsp = (xf_dsp_t *) (((UWORD32)p_adev->p_dspMem + (XAF_8BYTE_ALIGN-1))& ~(XAF_8BYTE_ALIGN-1));

    xf_g_dsp->core = core;
    xf_g_dsp->cb_compute_cycles = pconfig->cb_compute_cycles;
    xf_g_dsp->cb_stats = pconfig->cb_stats;

    /* ..init framework local memory for components, msgq and events */
    xf_g_dsp->pfrmwk_local_buffer = pconfig->pframework_local_buffer;
    xf_g_dsp->frmwk_local_memory_size = pconfig->framework_local_buffer_size;
    xf_g_dsp->frmwk_local_memory_curr = xaf_hk_mem_cumulative;
    xf_g_dsp->frmwk_local_memory_peak = xaf_hk_mem_cumulative;

    // DSP shared memory allocation
    XAF_CHK_PTR(pconfig->mem_pool[XAF_MEM_ID_DSP].pmem);
    for(i = XAF_MEM_ID_DSP; i <= XAF_MEM_ID_DSP_MAX; i++)
    {
        for(k=0; (k < XAF_MEM_ID_MAX); k++)
        {
            if((pconfig->mem_pool[k].mem_id == i) && (pconfig->mem_pool[i].pmem))
            {
                xf_g_dsp->mem_pool[i].size = ((pconfig->mem_pool[i].size + (XF_PROXY_ALIGNMENT-1)) & (~(XF_PROXY_ALIGNMENT-1)));
                XAF_CHK_ALIGN(xf_g_dsp->mem_pool[i].size, XF_PROXY_ALIGNMENT);
                XAF_CHK_MIN(xf_g_dsp->mem_pool[i].size, ( (i == XAF_MEM_ID_DEV) ? XA_AUDIO_FRMWK_BUF_SIZE_MIN : XA_AUDIO_FRMWK_BUF_SIZE_FAST_MIN));//TENA_2352, TENA_2191
            
                xf_g_dsp->mem_pool[i].mem_id = pconfig->mem_pool[i].mem_id;
                xf_g_dsp->mem_pool[i].pmem = (UWORD8 *) (((UWORD32)pconfig->mem_pool[i].pmem + (XF_PROXY_ALIGNMENT-1)) & ~(XF_PROXY_ALIGNMENT-1)); /* ...alignment adjustment is done on the DSP side */
                TRACE(INFO, _x("c[%d] dsp_shmem type:%d size:%d pmem:%p"), core, i, xf_g_dsp->mem_pool[i].size, xf_g_dsp->mem_pool[i].pmem);
            }
        }//for(;k;)
    }

    // DSP Interface Layer memory allocation (BSS)
    xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_MEM_POOL] = xf_g_dsp->mem_pool;
    for(i = XAF_MEM_ID_COMP; i <= XAF_MEM_ID_COMP_MAX; i++)
    {
        for(k=0; (k < XAF_MEM_ID_MAX); k++)
        {
            if(pconfig->mem_pool[k].mem_id == i)
            {
                xf_g_dsp->mem_pool[k].size = pconfig->mem_pool[k].size & (~(XAF_64BYTE_ALIGN-1));
                XAF_CHK_MIN(xf_g_dsp->mem_pool[k].size, ( (i == XAF_MEM_ID_COMP) ? XA_AUDIO_COMP_BUF_SIZE_MIN : XA_AUDIO_COMP_BUF_SIZE_FAST_MIN));//TENA_2352, TENA_2191
                XAF_CHK_ALIGN(xf_g_dsp->mem_pool[k].size, XAF_64BYTE_ALIGN);//TENA_2352, TENA_2191
                
                xf_g_dsp->mem_pool[k].pmem = pconfig->mem_pool[k].pmem;
                xf_g_dsp->mem_pool[k].mem_id = pconfig->mem_pool[k].mem_id;
                if(xf_g_dsp->mem_pool[k].pmem)
                {
                    memset(xf_g_dsp->mem_pool[k].pmem, 0, xf_g_dsp->mem_pool[k].size);
                }
                break;
            }
        }//for(;k;)
    }

    xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_CORE_ID] = &xf_g_dsp->core;
    xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_COMP_BUF_PEAK] = &xf_g_dsp->dsp_comp_buf_size_peak;
    xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_COMP_BUF_CURR] = &xf_g_dsp->dsp_comp_buf_size_curr;
    xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_FRMWK_BUF_PEAK] = &xf_g_dsp->dsp_frmwk_buf_size_peak;
    xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_FRMWK_BUF_CURR] = &xf_g_dsp->dsp_frmwk_buf_size_curr;
    xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_SHMEM_BUF_PEAK] = &xf_g_dsp->dsp_shmem_buf_size_peak;
    xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_SHMEM_BUF_CURR] = &xf_g_dsp->dsp_shmem_buf_size_curr;
    xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_CB_THREAD_STATS] = xf_g_dsp->cb_compute_cycles;
    xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_STATS_WORKER_THREAD_STATS] = (void *)xf_g_dsp->cb_stats;

    xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_WORKER_SCRATCH_SIZE] = xf_g_dsp->worker_thread_scratch_size;
    {
		UWORD32 *p_worker_scratch, j;

        p_worker_scratch = (UWORD32 *)xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_WORKER_SCRATCH_SIZE];
        for(j=0; j<XAF_MAX_WORKER_THREADS; j++)
        {
		    p_worker_scratch[j] = pconfig->worker_thread_scratch_size[j];
        }
    }

    xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_WORKER_STACK_SIZE] = xf_g_dsp->worker_thread_stack_size;
    {
		UWORD32 *p_worker_stack, j;

        p_worker_stack = (UWORD32 *)xf_g_dsp->dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_WORKER_STACK_SIZE];
        for(j=0; j<XAF_MAX_WORKER_THREADS; j++)
        {
		    p_worker_stack[j] = pconfig->worker_thread_stack_size[j];
        }
    }

    p_adev->dsp_thread_priority = dsp_thread_priority;
#if defined(HAVE_FREERTOS)
    ret = __xf_thread_create(&xf_g_dsp->dsp_thread, dsp_thread_entry, (void *) xf_g_dsp->dsp_thread_args, "DSP-thread", NULL, pconfig->dsp_thread_stack_size, dsp_thread_priority);
#else
    /* if((ret = xaf_malloc(&(xf_g_dsp->dsp_stack), pconfig->dsp_thread_stack_size, XAF_MEM_ID_DEV)))
        return ret;
    */
    {
        void *p = (void *)(((unsigned int)&xf_g_dsp->pfrmwk_local_buffer[xf_g_dsp->frmwk_local_memory_curr] + XAF_4BYTE_ALIGN - 1) & ~(XAF_4BYTE_ALIGN - 1));
        size = pconfig->dsp_thread_stack_size + ((unsigned int)p - (unsigned int)&xf_g_dsp->pfrmwk_local_buffer[xf_g_dsp->frmwk_local_memory_curr]);
        if(xf_g_dsp->frmwk_local_memory_size < (xf_g_dsp->frmwk_local_memory_curr + size))
        {
            return XAF_INVALIDPTR_ERR;
        }
        xf_g_dsp->frmwk_local_memory_curr += size;
        xf_g_dsp->frmwk_local_memory_peak += size;
        xf_g_dsp->dsp_stack = p;
        xf_g_dsp->dsp_thread_stack_size = pconfig->dsp_thread_stack_size;
    }
    ret = __xf_thread_create(&xf_g_dsp->dsp_thread, dsp_thread_entry, (void *) xf_g_dsp->dsp_thread_args, "DSP-thread", xf_g_dsp->dsp_stack, pconfig->dsp_thread_stack_size, dsp_thread_priority);
#endif
    if (ret != 0)
        return XAF_RTOS_ERR;

    xf_g_dsp->xaf_memory_used = xf_g_dsp->frmwk_local_memory_curr; /* ..frmwk local memory used */

    return XAF_NO_ERR;
}
#endif //XF_CFG_CORES_NUM > 1

XAF_ERR_CODE xaf_adev_close(pVOID adev_ptr, xaf_adev_close_flag flag)
{
    xaf_adev_t *p_adev;
    xf_proxy_t *p_proxy;
    xaf_comp_t *p_comp;
    WORD32 i, ncomp;
    XAF_ERR_CODE err_adev = XAF_NO_ERR;

    XF_CHK_ERR((adev_ptr != NULL), XAF_INVALIDPTR_ERR);

    p_adev = (xaf_adev_t *)adev_ptr;

    XAF_CHK_PTR(xf_g_ap);
    __xf_lock(&g_adev_close_lock);

    XAF_ADEV_STATE_CHK(p_adev, XAF_ADEV_RESET, &g_adev_close_lock);
    ncomp = p_adev->n_comp;
    p_adev->adev_state = XAF_ADEV_RESET;

    if(flag == XAF_ADEV_FORCE_CLOSE)
    {
        for(i = 0; i < ncomp; i++)
        {
            p_comp = (xaf_comp_t *) xaf_sync_chain_get_head(&p_adev->comp_chain);

            if(p_comp == NULL)
                break;

#if (XF_CFG_CORES_NUM > 1)
            XAF_ERR_CODE err_comp = xaf_comp_delete(p_comp);

            if((err_comp < 0) && (XAF_NO_ERR == err_adev))
            {
                /* ...register any fatal error and break */
                err_adev = err_comp;
                TRACE(ERROR, _x("Force close comp:%p delete failed with error %x"), p_comp, err_comp);
                break;
            }
#else
            XF_CHK_API_LOCK(xaf_comp_delete(p_comp), &g_adev_close_lock);
#endif
        }

#if (XF_CFG_CORES_NUM > 1)
        ncomp = p_adev->n_comp;
        while(ncomp > 0)
        {
            p_comp = (xaf_comp_t *) xaf_sync_chain_get_head(&p_adev->comp_chain);
            if(p_comp == NULL)
                break;

            /* ...reset the state for rest of the components in the chain to avoid accessing aux buffers after mem-free(proxy_close) */
            p_comp->comp_state = XAF_COMP_RESET;
            xaf_sync_chain_delete_node(&p_adev->comp_chain, p_comp);
            ncomp--;
        }
#endif //(XF_CFG_CORES_NUM > 1)
    }

#if (XF_CFG_CORES_NUM > 1)
    if(XAF_NO_ERR == err_adev)
    {
        err_adev = (p_adev->n_comp) ? XAF_API_ERR : err_adev;
    }
    /* ...reset n_comp variable after updating the error variable */
    p_adev->n_comp = 0;
#else
    XF_CHK_ERR_LOCK((p_adev->n_comp == 0), XAF_API_ERR, &g_adev_close_lock);
#endif

    p_proxy = &p_adev->proxy;

    if(p_proxy->aux != NULL)
    {
#if TENA_2356
        /* ...acquire lock. */
        __xf_lock(&xf_g_ap->g_comp_delete_lock);
#endif
#if (XF_CFG_CORES_NUM == 1)
        int err = xf_pool_free(p_proxy->aux, XF_POOL_FRMWK_AUX, XAF_MEM_ID_DEV);
#else
        xf_pool_free(p_proxy->aux, XF_POOL_FRMWK_AUX, XAF_MEM_ID_DEV);
#endif

#if TENA_2356
        /* ...release lock. */
        __xf_unlock(&xf_g_ap->g_comp_delete_lock);
#endif

#if (XF_CFG_CORES_NUM == 1)
        if(err)
        {
            return err;
        }
        /* ...else need to do deinit for multicore worker cores, hence proceed */
#endif
    }

    TRACE(INFO, _b("dsp buffer usage(bytes): component=%d, framework=%d xaf=%d"),
            xf_g_ap->dsp_comp_buf_size_peak[XAF_MEM_ID_COMP], xf_g_ap->dsp_frmwk_buf_size_peak[XAF_MEM_ID_DEV],
            xf_g_ap->xaf_memory_used);

    {
#if !defined(HAVE_FREERTOS)
        UWORD32 proxy_thread_stack_size = p_proxy->proxy_thread_stack_size;
#endif
        __xf_event_set(p_proxy->ipc.msgq_event, DSP_DIE_MSGQ_ENTRY);
        XF_CHK_API_LOCK(__xf_thread_join(&xf_g_ap->dsp_thread, NULL), &g_adev_close_lock);
        XF_CHK_API_LOCK(__xf_thread_destroy(&xf_g_ap->dsp_thread), &g_adev_close_lock);
        TRACE(INFO, _b("DSP thread destroyed"));
        xf_proxy_close(p_proxy);

        __xf_lock_destroy(&xf_g_ap->g_comp_delete_lock);
        __xf_lock_destroy(&xf_g_ap->g_msgq_lock);

#ifndef XA_DISABLE_EVENT
        xaf_sync_chain_deinit(&p_adev->event_chain);
#endif
        xaf_sync_chain_deinit(&p_adev->comp_chain);

        __xf_unlock(&g_adev_close_lock);
        //__xf_lock_destroy(&g_adev_close_lock);

        xaf_frmwk_mem_deinit();

        xf_g_ap->frmwk_local_memory_curr -= ((sizeof(xf_ap_t) +(XAF_8BYTE_ALIGN-1) + sizeof(xaf_adev_t)+(XAF_4BYTE_ALIGN-1) + (XAF_4BYTE_ALIGN-1)) & ~(XAF_4BYTE_ALIGN-1));
#if !defined(HAVE_FREERTOS)
        xf_g_ap->frmwk_local_memory_curr -= proxy_thread_stack_size;
        xf_g_ap->frmwk_local_memory_curr -= xf_g_ap->dsp_thread_stack_size;
#endif
        /* ...verify all memory is freed/accounted for or if the remianing bytes are under XAF_8BYTE_ALIGN  */
        if(xf_g_ap->frmwk_local_memory_curr >= XAF_8BYTE_ALIGN)
        {
            TRACE(INFO, _b("frmwk_local_buffer deinit memory_leak size:%d"), xf_g_ap->frmwk_local_memory_curr);
            return XAF_MEMORY_ERR;
        }
        xf_g_ap = NULL;
    }

    return err_adev;
}

#if (XF_CFG_CORES_NUM > 1)
XAF_ERR_CODE xaf_dsp_close(pVOID adev_ptr)
{
    xaf_adsp_t *p_adev;
    xf_dsp_t *xf_g_dsp;

    XF_CHK_ERR((adev_ptr != NULL), XAF_INVALIDPTR_ERR);

    p_adev = (xaf_adsp_t *)adev_ptr;

    XAF_ADEV_STATE_CHK(p_adev, XAF_ADEV_RESET, NULL);
    p_adev->adev_state = XAF_ADEV_RESET;
    xf_g_dsp = (xf_dsp_t *) (((UWORD32)p_adev->p_dspMem + (XAF_8BYTE_ALIGN-1))& ~(XAF_8BYTE_ALIGN-1));

    {
        XF_CHK_API(__xf_thread_join(&xf_g_dsp->dsp_thread, NULL));

        TRACE(INFO, _b("dsp[%d] buffer usage(bytes): component=%d, dsp shared_memory=%d xaf=%d"),\
                xf_g_dsp->core, xf_g_dsp->dsp_comp_buf_size_peak[XAF_MEM_ID_COMP], xf_g_dsp->dsp_shmem_buf_size_peak[XAF_MEM_ID_DSP], \
                (xf_g_dsp->xaf_memory_used));

        /* ...call the compute_cycles callback function before thread is deleted */
        if(xf_g_dsp->cb_compute_cycles && xf_g_dsp->cb_stats)
        {
            /* ... update the memory stats (including global shared memory) to the callback stats structure */
            memcpy(((xaf_perf_stats_t *)xf_g_dsp->cb_stats)->dsp_comp_buf_size_peak, xf_g_dsp->dsp_comp_buf_size_peak, sizeof(xf_g_dsp->dsp_comp_buf_size_peak));
            memcpy(((xaf_perf_stats_t *)xf_g_dsp->cb_stats)->dsp_shmem_buf_size_peak, xf_g_dsp->dsp_shmem_buf_size_peak, sizeof(xf_g_dsp->dsp_shmem_buf_size_peak));
            ((xaf_perf_stats_t *)xf_g_dsp->cb_stats)->dsp_framework_local_buf_size_peak = xf_g_dsp->xaf_memory_used;

            XF_CHK_API(xf_g_dsp->cb_compute_cycles(xf_g_dsp->cb_stats));
        }
        XF_CHK_API(__xf_thread_destroy(&xf_g_dsp->dsp_thread));

#if defined(HAVE_FREERTOS)
        xf_g_dsp->frmwk_local_memory_curr -= (sizeof(xf_dsp_t) +(XAF_8BYTE_ALIGN-1) + sizeof(xaf_adsp_t)+(XAF_4BYTE_ALIGN-1));
#else
        xf_g_dsp->frmwk_local_memory_curr -= ((sizeof(xf_dsp_t) +(XAF_8BYTE_ALIGN-1) + sizeof(xaf_adsp_t)+(XAF_4BYTE_ALIGN-1) + (XAF_4BYTE_ALIGN-1)) & ~(XAF_4BYTE_ALIGN-1));
        xf_g_dsp->frmwk_local_memory_curr -= xf_g_dsp->dsp_thread_stack_size;
#endif
        /* ...verify all memory is freed/accounted for or if the remianing bytes are under XAF_8BYTE_ALIGN  */
        if(xf_g_dsp->frmwk_local_memory_curr >= XAF_8BYTE_ALIGN)
        {
            TRACE(INFO, _b("frmwk_local_buffer deinit memory_leak size:%d"), xf_g_dsp->frmwk_local_memory_curr);
            return XAF_MEMORY_ERR;
        }
    }

    return XAF_NO_ERR;
}
#endif //XF_CFG_CORES_NUM > 1

XAF_ERR_CODE xaf_adev_set_priorities(pVOID adev_ptr, WORD32 n_rt_priorities,
                                     WORD32 rt_priority_base, WORD32 bg_priority, UWORD32 core)
{
    xaf_adev_t *p_adev = adev_ptr;

    XAF_CHK_PTR(p_adev);
    XAF_CHK_RANGE(core, 0, (XF_CFG_CORES_NUM-1));

    /* ...Thumb rule: background-thread priority should be at-most DSP-thread priority */
    XAF_CHK_RANGE(bg_priority, 0, p_adev->dsp_thread_priority);

    /* ...Thumb rule: worker-thread priority should be at-most DSP-thread priority */
    XAF_CHK_RANGE(rt_priority_base, 0, p_adev->dsp_thread_priority);
    XAF_CHK_RANGE((rt_priority_base + n_rt_priorities-1), rt_priority_base, p_adev->dsp_thread_priority);

    return xf_set_priorities(&p_adev->proxy, core,
                             n_rt_priorities, rt_priority_base, bg_priority);
}

XAF_ERR_CODE xaf_comp_config_default_init(xaf_comp_config_t *pcomp_config)
{
    XAF_CHK_PTR(pcomp_config);
    UWORD32 i;

    /* ...initialize component default config params */
    memset(pcomp_config, 0, sizeof(xaf_comp_config_t));

    for(i=0; i < XAF_MEM_POOL_TYPE_COMP_MAX; i++)
    {
        switch(i)
        {
            case XAF_MEM_POOL_TYPE_COMP_APP_INPUT:
            case XAF_MEM_POOL_TYPE_COMP_APP_OUTPUT:
                pcomp_config->mem_pool_type[i] = XAF_MEM_ID_DEV;
            break;
            default:
                pcomp_config->mem_pool_type[i] = XAF_MEM_ID_COMP;
            break;
        }
    }

#ifndef XA_DISABLE_EVENT
    /* ...error-event channel is disabled by default */
    pcomp_config->error_channel_ctl = XAF_ERR_CHANNEL_DISABLE;

    pcomp_config->num_err_msg_buf = 2;
#endif

    pcomp_config->core =  XF_CORE_ID_MASTER;

    /* ...configuration for a standalone pcm_gain component */
    pcomp_config->comp_id = "post-proc/pcm_gain";
    pcomp_config->comp_type = XAF_POST_PROC;
    pcomp_config->num_input_buffers = 2;
    pcomp_config->num_output_buffers = 1;

    return XAF_NO_ERR;
}

#ifndef XA_DISABLE_EVENT
static XAF_ERR_CODE xaf_setup_event_channel(xaf_comp_t *src_comp, UWORD32 src_config_param, xaf_comp_t *dest_comp, UWORD32 dst_config_param, UWORD32 nbuf, UWORD32 buf_size)
{
    xaf_adev_t  *p_adev;
    xf_app_event_channel_t *p_channel;
    void        *pTmp;
    int          ret;

    p_adev    = (xaf_adev_t *) src_comp->p_adev;

    /* ...need to maintain app side housekeeping structure event channels */
    //ret = xaf_malloc(&pTmp, sizeof(xf_app_event_channel_t), XAF_MEM_ID_DEV);
    ret = xaf_malloc(&pTmp, sizeof(xf_app_event_channel_t), XF_POOL_FRMWK_EVENT_CHANNEL);
    if(ret != XAF_NO_ERR)
        return ret;

    memset(pTmp, 0, sizeof(xf_app_event_channel_t));

    p_channel = (xf_app_event_channel_t *)pTmp;
    p_channel->event_id_src     = src_config_param;
    p_channel->event_id_dst     = dst_config_param;
    p_channel->p_comp_src       = src_comp;
    p_channel->p_comp_dst       = dest_comp;
    p_channel->channel_state    = XF_EVENT_CHANNEL_STATE_ACTIVE;

    if (dest_comp != NULL)
    {
        /* ...channel for event communication between components */
        XF_CHK_API(xf_create_event_channel(&src_comp->handle, src_config_param, &dest_comp->handle, dst_config_param, nbuf, buf_size, XF_EVENT_BUFFER_ALIGNMENT));
    }
    else
    {
        /* ...channel for event communication from component/framwork to application */

        p_channel->event_id_dst = XF_CFG_ID_EVENT_TO_APP;

        /* ...create channel at DSP Interface Layer */
        XF_CHK_API(xf_create_event_channel(&src_comp->handle, src_config_param, NULL, XF_CFG_ID_EVENT_TO_APP, nbuf, buf_size, XF_EVENT_BUFFER_ALIGNMENT));

        UWORD32 i, length = buf_size + sizeof(src_config_param);
        xf_buffer_t *event_buf;
        void *p_buf;

        /* ...allocate and send buffers to DSP */
        XF_CHK_API(xf_pool_alloc(&p_adev->proxy, nbuf, length, XAF_MEM_ID_DEV, &p_channel->pool, XF_POOL_FRMWK_EVENT_BUF));

        for (i=0; i<nbuf; i++)
        {
            event_buf = xf_buffer_get(p_channel->pool);
            p_buf = xf_buffer_data(event_buf);

            *(UWORD32*)p_buf = src_config_param;

            XF_CHK_API(xf_command(&src_comp->handle, 0, XF_EVENT, p_buf, length));

            p_channel->pending_buffers++;
        }
    }

    xaf_sync_chain_add_node(&p_adev->event_chain, p_channel);

    return XAF_NO_ERR;
}

static XAF_ERR_CODE xaf_destroy_event_channel(xaf_comp_t *src_comp, UWORD32 src_config_param, xaf_comp_t *dest_comp, UWORD32 dst_config_param)
{
    xaf_adev_t  *p_adev;
    xf_app_event_channel_t  *p_channel_curr = NULL;

    p_adev    = (xaf_adev_t *) src_comp->p_adev;

    dst_config_param = (dest_comp != NULL) ? dst_config_param : XF_CFG_ID_EVENT_TO_APP;

    p_channel_curr = xaf_sync_chain_find_node_by_param(&p_adev->event_chain, src_comp, src_config_param, dst_config_param);

    XF_CHK_ERR((p_channel_curr != NULL), XAF_INVALIDPTR_ERR);

    xaf_sync_chain_set_node_state(&p_adev->event_chain, p_channel_curr, XF_EVENT_CHANNEL_STATE_INACTIVE);

    if (dest_comp != NULL)
    {
        /* ...inter-component event channel, delete at DSP Interface Layer */
        XF_CHK_API(xf_delete_event_channel(&src_comp->handle, src_config_param, &dest_comp->handle, dst_config_param));
    }
    else
    {
        /* ...channel for events to application, delete at DSP Interface Layer */
        XF_CHK_API(xf_delete_event_channel(&src_comp->handle, src_config_param, NULL, XF_CFG_ID_EVENT_TO_APP));

        /* ...free the event buffer pool for this chanel */
        //xf_pool_free(p_channel_curr->pool, XAF_MEM_ID_COMP);
        xf_pool_free(p_channel_curr->pool, XF_POOL_FRMWK_EVENT_BUF, XAF_MEM_ID_DEV);
    }

    xaf_sync_chain_delete_node(&p_adev->event_chain, p_channel_curr);

    //xaf_free((void *)p_channel_curr, XAF_MEM_ID_DEV);
    xaf_free((void *)p_channel_curr, XF_POOL_FRMWK_EVENT_CHANNEL);

    return XAF_NO_ERR;
}
#endif /* XA_DISABLE_EVENT */

XAF_ERR_CODE xaf_comp_create(pVOID adev_ptr, pVOID *pp_comp, xaf_comp_config_t *pcomp_config)
{
    xaf_adev_t *p_adev;
    xaf_comp_t *p_comp;
    xf_handle_t *p_handle;
    void * pTmp;
    int ret, size;
    UWORD32 i;

    XAF_CHK_PTR(pcomp_config);

    xf_id_t comp_id = pcomp_config->comp_id;
    UWORD32 ninbuf = pcomp_config->num_input_buffers;
    UWORD32 noutbuf = pcomp_config->num_output_buffers;
    pVOID (*pp_inbuf)[XAF_MAX_INBUFS] = pcomp_config->pp_inbuf;
    xaf_comp_type comp_type = pcomp_config->comp_type;
    UWORD32 core = pcomp_config->core;

    p_adev = (xaf_adev_t *)adev_ptr;
    XAF_CHK_PTR(p_adev);
    XAF_CHK_PTR(pp_comp);
    XAF_CHK_PTR(comp_id);
    if (ninbuf) XAF_CHK_PTR(pp_inbuf);

    XAF_CHK_RANGE(core, 0, (XF_CFG_CORES_NUM-1));
    XAF_CHK_RANGE(ninbuf, 0, XAF_MAX_INBUFS);
    XAF_CHK_RANGE(noutbuf, 0, 1);
    XAF_CHK_RANGE(comp_type, XAF_DECODER, XAF_MAX_COMPTYPE-1);

#ifndef XA_DISABLE_EVENT
    XAF_CHK_RANGE(pcomp_config->error_channel_ctl, XAF_ERR_CHANNEL_DISABLE, XAF_ERR_CHANNEL_ALL);
    XAF_CHK_RANGE(pcomp_config->num_err_msg_buf, 1, XF_MAX_ERROR_EVENT_BUFS_PER_COMP);
#endif

    XAF_ADEV_STATE_CHK(p_adev, XAF_ADEV_RESET, NULL);

    //Memory allocation for component struct pointer
    size = (sizeof(xaf_comp_t) + (XAF_8BYTE_ALIGN-1));
    //ret = xaf_malloc(&pTmp, size, XAF_MEM_ID_COMP, XF_POOL_FRMWK_COMP);
    ret = xaf_malloc(&pTmp, size, XF_POOL_FRMWK_COMP);
    if(ret != XAF_NO_ERR)
        return ret;
    //p_comp = (xaf_comp_t *) (((UWORD32)pTmp + (XAF_4BYTE_ALIGN-1))& ~(XAF_4BYTE_ALIGN-1));
    p_comp = (xaf_comp_t *)pTmp;
    p_comp->comp_ptr = pTmp;
    *pp_comp = (void*)p_comp;

    p_handle = &p_comp->handle;

    /* ...copy pool type of each of the component memory type into component's internal structure, before calling xf_open. */
    for(i=0; i < XAF_MEM_POOL_TYPE_COMP_MAX; i++)
    {
        switch(i)
        {
            case XAF_MEM_POOL_TYPE_COMP_APP_INPUT:
            case XAF_MEM_POOL_TYPE_COMP_APP_OUTPUT:
                XAF_CHK_RANGE(pcomp_config->mem_pool_type[i], XAF_MEM_ID_DEV, XAF_MEM_ID_DEV_MAX);
            break;
#if (XF_CFG_CORES_NUM > 1)
            case XAF_MEM_POOL_TYPE_COMP_OUTPUT:
                if( !((pcomp_config->mem_pool_type[i] >= XAF_MEM_ID_COMP) && (pcomp_config->mem_pool_type[i] <= XAF_MEM_ID_COMP_MAX)) &&
                    !((pcomp_config->mem_pool_type[i] >= XAF_MEM_ID_DSP) && (pcomp_config->mem_pool_type[i] <= XAF_MEM_ID_DSP_MAX))
                )
                {
                    /* ...output buffer type is of ID_COMP for same core routing or ID_DSP for diferent core routing. */
                    return XAF_INVALIDVAL_ERR;
                }
            break;
#endif
            default:
                XAF_CHK_RANGE(pcomp_config->mem_pool_type[i], XAF_MEM_ID_COMP, XAF_MEM_ID_COMP_MAX);
            break;
        }
        p_handle->mem_pool_type[i] = pcomp_config->mem_pool_type[i];
    }

    /* ...create component instance (select core-0) */
    XF_CHK_API(xf_open(&p_adev->proxy, p_handle, comp_id, core, xaf_comp_response));

    XF_CHK_API(xaf_sync_chain_add_node(&p_adev->comp_chain, p_comp));

    // Temporary solution in place of component chain handling
    p_comp->p_adev = p_adev;
    p_adev->n_comp += 1;

    p_comp->ninbuf = ninbuf;

    /* ...allocate input buffer */
    if (ninbuf)
    {
        xf_buffer_t *buf;
        XF_CHK_API(xf_pool_alloc(&p_adev->proxy, ninbuf, XAF_INBUF_SIZE, p_comp->handle.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_APP_INPUT], &p_comp->inpool, XF_POOL_FRMWK_INPUT));

        for (i=0; i<ninbuf; i++)
        {
            buf         = xf_buffer_get(p_comp->inpool);
            p_comp->p_input[i] = xf_buffer_data(buf);   //TENA-2196 saving the address of buffer allocated.
            *(*pp_inbuf + i) = p_comp->p_input[i];
        }

    }
    p_comp->noutbuf = noutbuf;

    if(pcomp_config->cfg_param_ext_buf_size_max > 0)
    {
        XAF_CHK_RANGE(pcomp_config->cfg_param_ext_buf_size_max, 1, XAF_MAX_EXT_CFG_BUF_LEN);
        /* ...Allocating memory needed for extended set and get configs.
         * This memory is memory needed for the parameter buffer plus
         * some overhead needed for communication and alignment */
        UWORD32 config_buffer_size = pcomp_config->cfg_param_ext_buf_size_max + (XAF_EXT_CFG_OVERHEAD * XAF_MAX_EXT_CONFIG_PARAMS);
        p_comp->cfg_param_ext_buf_size_max = config_buffer_size;
        XF_CHK_API(xf_pool_alloc(&p_adev->proxy, XAF_EXT_CFG_POOL_SIZE, config_buffer_size, XAF_MEM_ID_DEV, &p_comp->ext_cfg_pool, XF_POOL_FRMWK_EXT_CFG));
        p_comp->p_config_buf = xf_buffer_get(p_comp->ext_cfg_pool);
    }

    p_comp->comp_type   = comp_type;
    p_comp->comp_status = XAF_STARTING;
    p_comp->probe_enabled = 0;
    p_comp->probe_started = 0;

    for (i=0; i<MAX_IO_PORTS; i++)
    {
        p_comp->cmap[i].ptr  = NULL;
        p_comp->cmap[i].port = PORT_NOT_CONNECTED;
    }

    switch (comp_type)
    {
    case XAF_DECODER:
    case XAF_ENCODER:
    case XAF_PRE_PROC:
    case XAF_POST_PROC:
        p_comp->inp_ports = 1; p_comp->out_ports = 1;
        break;
    case XAF_MIMO_PROC_12 ... (XAF_MAX_COMPTYPE-1):
        p_comp->inp_ports  = xf_io_ports[comp_type][0];
        p_comp->out_ports  = xf_io_ports[comp_type][1];
        break;
    case XAF_RENDERER:
        p_comp->inp_ports = 1; p_comp->out_ports = 1; /* optional outport */
        break;
#if 1 //TENA_2314
    case XAF_CAPTURER:
        p_comp->inp_ports = 0; p_comp->out_ports = 1;
        break;
#else
    case XAF_CAPTURER:
        p_comp->inp_ports = 1; p_comp->out_ports = 1;
        break;
#endif
    default :
        break;
    }

#ifndef XA_DISABLE_EVENT
    if(pcomp_config->error_channel_ctl)
    {
        UWORD32 error_channel_event_id = 0;

        p_comp->error_channel_ctl = pcomp_config->error_channel_ctl;

        if(p_comp->error_channel_ctl == XAF_ERR_CHANNEL_FATAL)
            error_channel_event_id = XF_CFG_COMP_ERR_FATAL;
        else if(p_comp->error_channel_ctl == XAF_ERR_CHANNEL_ALL)
            error_channel_event_id = XF_CFG_COMP_ERR_ALL;

        XF_CHK_API(xaf_setup_event_channel(p_comp, error_channel_event_id, NULL, 0, pcomp_config->num_err_msg_buf, XF_COMP_ERROR_CHANNEL_BUF_SIZE));
    }
#endif
    p_comp->comp_state = XAF_COMP_CREATE;

    TRACE(INFO, _b("comp_create:%s core[%d] p_comp %p"), comp_id, core, p_comp);

    return XAF_NO_ERR;
}

XAF_ERR_CODE xaf_comp_delete(pVOID comp_ptr)
{
    xaf_comp_t *p_comp;
    xaf_adev_t *p_adev;

    p_comp = (xaf_comp_t *)comp_ptr;

#if TENA_2356
    XAF_CHK_PTR(xf_g_ap);
    /* ...acquire lock. */
    __xf_lock(&xf_g_ap->g_comp_delete_lock);
#endif

    XF_CHK_ERR_LOCK((p_comp != NULL), XAF_INVALIDPTR_ERR, &xf_g_ap->g_comp_delete_lock);

    p_adev = (xaf_adev_t *) p_comp->p_adev;

    XAF_COMP_STATE_CHK_WITH_LOCK(p_comp);

    p_comp->comp_state = XAF_COMP_RESET;

    // Temporary solution in place of component chain handling
    XF_CHK_ERR_LOCK((p_adev->n_comp > 0), XAF_API_ERR, &xf_g_ap->g_comp_delete_lock);

#ifndef XA_DISABLE_EVENT
    /* ...first delete any event channels associated with the component */
    {
        xf_app_event_channel_t   *p_channel_curr;

        while((p_channel_curr = xaf_sync_chain_find_node_by_comp(&p_adev->event_chain, comp_ptr)) != NULL)
        {
            XF_CHK_API_LOCK(xaf_destroy_event_channel(p_channel_curr->p_comp_src, p_channel_curr->event_id_src, p_channel_curr->p_comp_dst, p_channel_curr->event_id_dst), &xf_g_ap->g_comp_delete_lock);
        }
    }
#endif

    /* ...remove the component from chain */
    XF_CHK_API_LOCK(xaf_sync_chain_delete_node(&p_adev->comp_chain, p_comp), &xf_g_ap->g_comp_delete_lock);

    p_adev->n_comp -= 1;

    XF_CHK_API_LOCK(xf_close(&p_comp->handle), &xf_g_ap->g_comp_delete_lock);

    if (p_comp->inpool)  XF_CHK_API_LOCK(xf_pool_free(p_comp->inpool, XF_POOL_FRMWK_INPUT, p_comp->handle.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_APP_INPUT]), &xf_g_ap->g_comp_delete_lock);
    if (p_comp->outpool)  XF_CHK_API_LOCK(xf_pool_free(p_comp->outpool, XF_POOL_FRMWK_OUTPUT, p_comp->handle.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_APP_OUTPUT]), &xf_g_ap->g_comp_delete_lock);
    if (p_comp->probepool)  XF_CHK_API_LOCK(xf_pool_free(p_comp->probepool, XF_POOL_FRMWK_OUTPUT, p_comp->handle.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_APP_OUTPUT]), &xf_g_ap->g_comp_delete_lock);
    if(p_comp->ext_cfg_pool)  XF_CHK_API_LOCK(xf_pool_free(p_comp->ext_cfg_pool, XF_POOL_FRMWK_EXT_CFG, XAF_MEM_ID_DEV), &xf_g_ap->g_comp_delete_lock);

    //xf_g_ap->xf_mem_free_fxn(p_comp->comp_ptr, XAF_MEM_ID_COMP);
    //xaf_free(p_comp->comp_ptr, (sizeof(xaf_comp_t) + (XAF_4BYTE_ALIGN-1)), XAF_MEM_ID_COMP, XF_POOL_FRMWK_COMP);
    xaf_free(p_comp->comp_ptr, XF_POOL_FRMWK_COMP);

#if TENA_2356
    /* ...release lock. */
    __xf_unlock(&xf_g_ap->g_comp_delete_lock);
#endif

    TRACE(INFO, _b("comp_delete: p_comp %p"), p_comp);
    return XAF_NO_ERR;
}

XAF_ERR_CODE xaf_comp_set_config(pVOID comp_ptr, WORD32 num_param, pWORD32 p_param)
{
    xaf_comp_t          *p_comp;
    xf_handle_t         *p_handle;
    void                *buffer;
    WORD32               i, j;
    WORD32               resp_err;

    p_comp = (xaf_comp_t *)comp_ptr;

    XAF_CHK_PTR(p_comp);
    XAF_CHK_PTR(p_param);
    XAF_CHK_RANGE(num_param, 1, XAF_MAX_CONFIG_PARAMS);

    XAF_COMP_STATE_CHK(p_comp);

    p_handle = &p_comp->handle;
    XAF_CHK_PTR(p_handle);

    /*...necessary checks required in API */
    j = 0;
    for (i=0; i<num_param; i++)
    {
        xf_set_param_item_t   smsg_tmp;
        smsg_tmp.id    = p_param[j++];
        smsg_tmp.value = p_param[j++];

        if (smsg_tmp.id == XAF_COMP_CONFIG_PARAM_DEC_INIT_WO_INP)
            XF_CHK_ERR((p_comp->comp_type == XAF_DECODER), XAF_INVALIDVAL_ERR);

        if ((smsg_tmp.id == XAF_COMP_CONFIG_PARAM_PROBE_ENABLE) && (smsg_tmp.value))
            p_comp->probe_enabled = 1;
    }

    buffer = xf_buffer_data(p_handle->aux);

    /* ...pass command to component through proxy */
    resp_err = XF_CHK_API(xf_set_config_with_lock(p_handle, buffer, sizeof(xf_set_param_item_t)*num_param, num_param, p_param, 0 /* ext_flag */));

    return resp_err;
}

/* ...This function allows setting of config parameters which needs more than 4 bytes of data. */
XAF_ERR_CODE xaf_comp_set_config_ext(pVOID comp_ptr, WORD32 num_param, WORD32 *p_param)
{
    xaf_comp_t             *p_comp;
    xf_handle_t            *p_handle;
    void                   *buffer;
    UWORD32                 length;
    WORD32                  resp_err;

    p_comp = (xaf_comp_t *)comp_ptr;

    XAF_CHK_PTR(p_comp);
    XAF_CHK_PTR(p_param);
    XAF_COMP_STATE_CHK(p_comp);
    XAF_CHK_RANGE(num_param, 1, XAF_MAX_EXT_CONFIG_PARAMS);

    p_handle = &p_comp->handle;
    XAF_CHK_PTR(p_handle);

    if (p_comp->p_config_buf)
    {
        buffer = xf_buffer_data(p_comp->p_config_buf);
        length = p_comp->cfg_param_ext_buf_size_max;
    }
    else
    {
        buffer = xf_buffer_data(p_handle->aux);
        length = XAF_AUX_POOL_MSG_LENGTH;
    }

    resp_err = XF_CHK_API(xf_set_config_with_lock(p_handle, buffer, length, num_param, p_param, 1 /* ext_flag */));

    return resp_err;
}


XAF_ERR_CODE xaf_comp_get_config(pVOID comp_ptr, WORD32 num_param, pWORD32 p_param)
{
    xaf_comp_t      *p_comp;
    void            *buffer;
    xf_handle_t     *p_handle;
    WORD32          resp_err;

    p_comp = (xaf_comp_t *)comp_ptr;

    XAF_CHK_PTR(p_comp);
    XAF_CHK_PTR(p_param);
    XAF_CHK_RANGE(num_param, 1, XAF_MAX_CONFIG_PARAMS);

    XAF_COMP_STATE_CHK(p_comp);

    p_handle = &p_comp->handle;
    XAF_CHK_PTR(p_handle);

    buffer = xf_buffer_data(p_handle->aux);

    /* ...pass command to component through proxy */
    resp_err = XF_CHK_API(xf_get_config_with_lock(p_handle, buffer, XF_GET_PARAM_CMD_LEN(num_param), num_param, p_param, 0 /* ext_flag */));

    return resp_err;
}

/* ...This function allows querying of config parameters which needs more than 4 bytes of data. */
XAF_ERR_CODE xaf_comp_get_config_ext(pVOID comp_ptr, WORD32 num_param, WORD32 *p_param)
{
    xaf_comp_t             *p_comp;
    xf_handle_t            *p_handle;
    void                   *buffer;
    UWORD32                 length;
    WORD32                  resp_err;

    p_comp = (xaf_comp_t *)comp_ptr;

    XAF_CHK_PTR(p_comp);
    XAF_CHK_PTR(p_param);
    XAF_COMP_STATE_CHK(p_comp);
    XAF_CHK_RANGE(num_param, 1, XAF_MAX_EXT_CONFIG_PARAMS);

    p_handle = &p_comp->handle;
    XAF_CHK_PTR(p_handle);

    if (p_comp->p_config_buf)
    {
        buffer = xf_buffer_data(p_comp->p_config_buf);
        length = p_comp->cfg_param_ext_buf_size_max;
    }
    else
    {
        buffer = xf_buffer_data(p_handle->aux);
        length = XAF_AUX_POOL_MSG_LENGTH;
    }

    resp_err = XF_CHK_API(xf_get_config_with_lock(p_handle, buffer, length, num_param, p_param, 1 /* ext_flag */));

    return resp_err;
}

XAF_ERR_CODE xaf_comp_get_status(pVOID adev_ptr, pVOID comp_ptr, xaf_comp_status *p_status, pVOID p_info)
{
    xaf_adev_t *p_adev;
    xaf_comp_t *p_comp;
    xf_handle_t *p_handle;

    p_adev = (xaf_adev_t *)adev_ptr;
    p_comp = (xaf_comp_t *)comp_ptr;

    XAF_CHK_PTR(p_comp);
    XAF_CHK_PTR(p_status);
    XAF_CHK_PTR(p_info);

    XAF_COMP_STATE_CHK(p_comp);

    if (!p_comp->init_done) XAF_CHK_PTR(p_adev);

    p_handle = &p_comp->handle;

    TRACE(INFO, _b("enter comp_get_status pending_resp=%d expect_out_cmd=%d"), p_comp->pending_resp, p_comp->expect_out_cmd);
    if (p_comp->pending_resp)
    {
        xf_user_msg_t rmsg;
        /* ...wait until result is delivered */
        XF_CHK_API(xf_response_get(p_handle, &rmsg));

        if (rmsg.opcode == XF_FILL_THIS_BUFFER)
        {
            if (rmsg.buffer == xf_buffer_data(p_comp->start_buf))
            {
                int num_out_ports = p_comp->out_ports;

                /* ... response on first output port is received */
                num_out_ports--;
                p_comp->pending_resp--;

                while (num_out_ports)
                {
                    /* ...wait until result is delivered */
                    XF_CHK_API(xf_response_get(p_handle, &rmsg));

                    /* ...make sure response is expected */
                    XF_CHK_ERR((rmsg.opcode == XF_FILL_THIS_BUFFER && rmsg.buffer == xf_buffer_data(p_comp->start_buf)), XAF_API_ERR);

                    num_out_ports--;
                    p_comp->pending_resp--;
                }

                if(rmsg.length == 0)
                {
                    p_comp->comp_status = XAF_INIT_NEED_INPUT;
                    p_comp->start_cmd_issued = 0;
                    TRACE(INFO, _b("Init Status: XAF_INIT_NEED_INPUT"));

                    /* ...return buffer to proxy */
                    xf_buffer_put(p_comp->start_buf);
                }
                else
                {
                    XF_CHK_API(xaf_comp_post_init_config(p_adev, p_comp, xf_buffer_data(p_comp->start_buf)));
                }
            }
            else
            {
                WORD32 *p_buf = (WORD32 *) p_info;
                p_buf[0] = (WORD32) rmsg.buffer;
                p_buf[1] = (WORD32) rmsg.length;

                p_comp->pending_resp--;

                if (p_comp->inp_ports == XF_MSG_SRC_PORT(rmsg.id))
                {
                    if(rmsg.length == 0)
                    {
                    	p_comp->input_over = 0;
                        p_comp->exec_cmd_issued = 0;

                        TRACE(INFO, _b("FTB R[%016llx]:(%08x,%u,%p) pending resp:%d"), (UWORD64)rmsg.id, rmsg.opcode, rmsg.length, rmsg.buffer, p_comp->pending_resp);

                        /* ...collect pending responses before declaring exec_done */
                        if (p_comp->probe_enabled && p_comp->probe_started)
                        {
                            /* ...trigger probe buffer flush */
                            XF_CHK_API(xf_pause(p_handle, (p_comp->inp_ports + p_comp->out_ports)));

                            p_comp->probe_started = 0;
                        }

                        while (p_comp->pending_resp)
                        {
                        	XF_CHK_API(xf_response_get(p_handle, &rmsg));
                        	p_comp->pending_resp--;

                        	TRACE(INFO, _b("FTB R[%016llx]:(%08x,%u,%p) pending resp:%d"), (UWORD64)rmsg.id, rmsg.opcode, rmsg.length, rmsg.buffer, p_comp->pending_resp);
                        }

                        p_comp->comp_status = XAF_EXEC_DONE;
                    }
                    else
                    {
                        if (p_comp->comp_state != XAF_COMP_RESET)
                            p_comp->comp_status = XAF_OUTPUT_READY;
                        else
                            p_comp->comp_status = XAF_EXEC_DONE;
                    }
                }
                else
                {
                    if(rmsg.length == 0)
                    {
                        p_comp->comp_status = XAF_PROBE_DONE;
                        TRACE(INFO, _b("PROBE R[%016llx]:(%08x,%u,%p)"), (UWORD64)rmsg.id, rmsg.opcode, rmsg.length, rmsg.buffer);
                    }
                    else
                    {
                        if (p_comp->comp_state != XAF_COMP_RESET)
                            p_comp->comp_status = XAF_PROBE_READY;
                        else
                            p_comp->comp_status = XAF_PROBE_DONE;
                    }
                }

                p_comp->expect_out_cmd++;
            }
        }
        else
        {
            /* ...make sure response is expected */
            XF_CHK_ERR((rmsg.opcode == XF_EMPTY_THIS_BUFFER), XAF_API_ERR);
            WORD32 *p_buf = (WORD32 *) p_info;
            p_buf[0] = (WORD32) rmsg.buffer;
            p_buf[1] = (WORD32) rmsg.length;

            p_comp->pending_resp--;

            if (p_comp->input_over && rmsg.buffer == NULL)
            {
            	p_comp->input_over = 0;
            	p_comp->exec_cmd_issued = 0;

                TRACE(INFO, _b("ETB R[%016llx]:(%08x,%u,%p) pending resp:%d"), (UWORD64)rmsg.id, rmsg.opcode, rmsg.length, rmsg.buffer, p_comp->pending_resp);

                /* ...collect pending responses before declaring exec_done */
                if (p_comp->probe_enabled && p_comp->probe_started)
                {
                    /* ...trigger probe buffer flush */
                    XF_CHK_API(xf_pause(p_handle, (p_comp->inp_ports + p_comp->out_ports)));

                    p_comp->probe_started = 0;
                }

                while (p_comp->pending_resp)
                {
                	XF_CHK_API(xf_response_get(p_handle, &rmsg));
                	p_comp->pending_resp--;

                    TRACE(INFO, _b("ETB R2[%016llx]:(%08x,%u,%p) pending resp:%d"), (UWORD64)rmsg.id, rmsg.opcode, rmsg.length, rmsg.buffer, p_comp->pending_resp);
                }

            	p_comp->comp_status = XAF_EXEC_DONE;
            }
            else
            {
                if (p_comp->comp_state != XAF_COMP_RESET)
                    p_comp->comp_status = XAF_NEED_INPUT;
                else
            	    p_comp->comp_status = XAF_EXEC_DONE;
            }
        }
    }
    else if ((p_comp->comp_status == XAF_STARTING && p_comp->start_cmd_issued) ||
             (p_comp->comp_status == XAF_INIT_DONE && p_comp->exec_cmd_issued))
    {
        if (p_comp->inpool) p_comp->comp_status = XAF_NEED_INPUT;
    }
#if 1 //TENA_2355
    else
    {
        TRACE(INFO, _b("(no pending response)exit comp_get_status expect_out_cmd=%d comp_status:%d"), p_comp->expect_out_cmd, p_comp->comp_status);
        *p_status = p_comp->comp_status;

        return XAF_API_ERR;
    }
#endif

    TRACE(INFO, _b("exit comp_get_status pending_resp=%d expect_out_cmd=%d"), p_comp->pending_resp, p_comp->expect_out_cmd);
    *p_status = p_comp->comp_status;

    return XAF_NO_ERR;
}

XAF_ERR_CODE xaf_comp_process(pVOID adev_ptr, pVOID comp_ptr, pVOID p_buf, UWORD32 length, xaf_comp_flag flag)
{
    xaf_adev_t *p_adev;
    xaf_comp_t *p_comp;
    xf_handle_t *p_handle;

    p_adev = (xaf_adev_t *)adev_ptr;
    p_comp = (xaf_comp_t *)comp_ptr;

    XAF_CHK_PTR(p_comp);

    XAF_COMP_STATE_CHK(p_comp);

    if (!p_comp->init_done) XAF_CHK_PTR(p_adev);
    XAF_CHK_RANGE(flag, XAF_START_FLAG, XAF_NEED_PROBE_FLAG);
    if (flag == XAF_INPUT_READY_FLAG) XAF_CHK_RANGE(length, 0, XAF_INBUF_SIZE);

    p_handle = &p_comp->handle;

    if((!p_comp->init_done) && ((flag == XAF_NEED_OUTPUT_FLAG) || (flag == XAF_NEED_PROBE_FLAG)))
    {
        return XAF_API_ERR;
    }

    switch (flag)
    {
    case XAF_START_FLAG:
        if (p_comp->start_cmd_issued)
            break;
        else if(p_comp->out_ports)
        {
            /* ...get control buffer from proxy for start buf */
            xf_buffer_t *p_start_buf = xf_buffer_get(p_handle->proxy->aux);
            XAF_CHK_PTR(p_start_buf);
            p_comp->start_buf = p_start_buf;

            UWORD32 out_port_idx=0;
            for(out_port_idx=0;out_port_idx<p_comp->out_ports;out_port_idx++)
            {
                  XF_CHK_API(xf_command(p_handle, (p_comp->inp_ports + out_port_idx), XF_FILL_THIS_BUFFER, xf_buffer_data(p_comp->start_buf), 0));
                  p_comp->pending_resp++;
            }
        }
        p_comp->start_cmd_issued = 1;
        break;

    case XAF_EXEC_FLAG:
        if (!p_comp->init_done || p_comp->exec_cmd_issued)
            break;
        p_comp->exec_cmd_issued = 1;
        if (p_comp->outpool)
        {
            UWORD32 i;
            void *p_data;

            for (i=0; i<p_comp->noutbuf; i++)
            {
#if 1 //DISCONNECT
                p_data = p_comp->pout_buf[i];
#else
                xf_buffer_t *p_buf;
                p_buf = xf_buffer_get(p_comp->outpool);
                p_data = xf_buffer_data(p_buf);
#endif

                XF_CHK_API(xf_command(&p_comp->handle, (p_comp->inp_ports), XF_FILL_THIS_BUFFER, p_data, p_comp->out_format.output_length[0]));
            }

            p_comp->pending_resp += p_comp->noutbuf;
        }
#if 0
        if (p_comp->probepool)
        {
            xf_buffer_t *p_buf;
            void *p_data;

            p_buf = xf_buffer_get(p_comp->probepool);
            p_data = xf_buffer_data(p_buf);

            XF_CHK_API(xf_command(&p_comp->handle, (p_comp->inp_ports + p_comp->out_ports), XF_FILL_THIS_BUFFER, p_data, p_comp->probe_length));

            p_comp->pending_resp++;
        }
#endif
        break;

    case XAF_INPUT_OVER_FLAG:
        if (!p_comp->input_over)
        {
            XF_CHK_API(xf_command(p_handle, 0, XF_EMPTY_THIS_BUFFER, NULL, 0));
            p_comp->input_over = 1;
            p_comp->pending_resp++;
        }
        break;

    case XAF_INPUT_READY_FLAG:
        {
            /* TENA-2196: To validate p_buf, allowing only pointers allocated by the library. */
            UWORD32 i, k;
            for(i=0, k=1; i<p_comp->ninbuf; i++)
            {
                if(p_buf == p_comp->p_input[i])
                {
                    k=0;
                    break;
                }
            }
            if(k) return XAF_INVALIDPTR_ERR;
        }
        if (!p_comp->input_over)
        {
            XAF_CHK_PTR(p_buf);
            XF_CHK_API(xf_command(p_handle, 0, XF_EMPTY_THIS_BUFFER, p_buf, length));
            p_comp->pending_resp++;
        }
        break;

    case XAF_NEED_OUTPUT_FLAG:
        if (p_comp->expect_out_cmd)
        {
            XAF_CHK_PTR(p_buf);
            XF_CHK_API(xf_command(p_handle, (p_comp->inp_ports), XF_FILL_THIS_BUFFER, p_buf, length));
            p_comp->expect_out_cmd--;

            p_comp->pending_resp++;
        }
        break;

    case XAF_NEED_PROBE_FLAG:
        if (p_comp->expect_out_cmd)
        {
            XAF_CHK_PTR(p_buf);
            XF_CHK_API(xf_command(p_handle, (p_comp->inp_ports + p_comp->out_ports), XF_FILL_THIS_BUFFER, p_buf, length));
            p_comp->expect_out_cmd--;

            p_comp->pending_resp++;
        }
    }

    return XAF_NO_ERR;
}

XAF_ERR_CODE xaf_connect(pVOID p_src, WORD32 src_out_port, pVOID p_dest, WORD32 dest_in_port, WORD32 num_buf)
{
    xaf_comp_t *src_comp;
    xaf_comp_t *dest_comp;
    WORD32      resp_err, length;

    src_comp  = (xaf_comp_t *) p_src;
    dest_comp = (xaf_comp_t *) p_dest;

    XAF_CHK_PTR(src_comp);
    XAF_CHK_PTR(dest_comp);
    XAF_CHK_RANGE(num_buf, 1, 1024);

    XAF_COMP_STATE_CHK(src_comp);
    XAF_COMP_STATE_CHK(dest_comp);

    length = src_comp->out_format.output_length[src_out_port - src_comp->inp_ports];

    if (length && !src_comp->init_done)
        return XAF_ROUTING_ERR;

    /* ...src output port number validity check */
    if (src_out_port < (WORD32)src_comp->inp_ports || src_out_port >= (WORD32)(src_comp->inp_ports + src_comp->out_ports))
        return XAF_ROUTING_ERR;

    /* ...dest input port number validity check */
    if (dest_in_port < 0 || dest_in_port >= (WORD32)dest_comp->inp_ports)
        return XAF_ROUTING_ERR;

    /* ...src component connection validity check */
    if (src_comp->cmap[src_out_port].ptr != NULL || src_comp->cmap[src_out_port].port != PORT_NOT_CONNECTED)
        return XAF_ROUTING_ERR;

    /* ...dest component connection validity check */
    if (dest_comp->cmap[dest_in_port].ptr != NULL || dest_comp->cmap[dest_in_port].port != PORT_NOT_CONNECTED)
        return XAF_ROUTING_ERR;


    resp_err = XF_CHK_API(xf_route(&src_comp->handle, src_out_port, &dest_comp->handle, dest_in_port, num_buf, length, 8));

    /* ...update src component map */
    src_comp->cmap[src_out_port].ptr  = dest_comp;
    src_comp->cmap[src_out_port].port = dest_in_port;

    /* ...update dest component map */
    dest_comp->cmap[dest_in_port].ptr  = src_comp;
    dest_comp->cmap[dest_in_port].port = src_out_port;

    return resp_err;
}

XAF_ERR_CODE xaf_disconnect(pVOID p_src, WORD32 src_out_port, pVOID p_dest, WORD32 dest_in_port)
{
    xaf_comp_t *src_comp;
    xaf_comp_t *dest_comp;
    WORD32      resp_err;

    src_comp  = (xaf_comp_t *) p_src;
    dest_comp = (xaf_comp_t *) p_dest;

    XAF_CHK_PTR(src_comp);
    XAF_CHK_PTR(dest_comp);

    XAF_COMP_STATE_CHK(src_comp);

    if (!src_comp->init_done)
        return XAF_ROUTING_ERR;

    /* ...src output port number validity check */
    if (src_out_port < (WORD32)src_comp->inp_ports || src_out_port >= (WORD32)(src_comp->inp_ports + src_comp->out_ports))
        return XAF_ROUTING_ERR;

    /* ...dest input port number validity check */
    if (dest_in_port < 0 || dest_in_port >= (WORD32)dest_comp->inp_ports)
        return XAF_ROUTING_ERR;

    /* ...src component connection validity check */
    if (src_comp->cmap[src_out_port].ptr != dest_comp || (WORD32)src_comp->cmap[src_out_port].port != dest_in_port)
        return XAF_ROUTING_ERR;

    /* ...dest component connection validity check */
    if (dest_comp->cmap[dest_in_port].ptr != src_comp || (WORD32)dest_comp->cmap[dest_in_port].port != src_out_port)
        return XAF_ROUTING_ERR;

    resp_err = XF_CHK_API(xf_unroute(&src_comp->handle, src_out_port));

    /* ...update src component map */
    src_comp->cmap[src_out_port].ptr  = NULL;
    src_comp->cmap[src_out_port].port = PORT_NOT_CONNECTED;

    /* ...update dest component map */
    dest_comp->cmap[dest_in_port].ptr  = NULL;
    dest_comp->cmap[dest_in_port].port = PORT_NOT_CONNECTED;

    return resp_err;
}

XAF_ERR_CODE xaf_get_mem_stats(pVOID adev_ptr, UWORD32 core, WORD32 *pmem_info)
{
    XAF_CHK_RANGE(core, 0, (XF_CFG_CORES_NUM-1));
    XAF_CHK_PTR(pmem_info);
    XAF_CHK_PTR(adev_ptr);
    XAF_CHK_PTR(xf_g_ap);
    UWORD32 i, k;

    if(core == XF_CORE_ID_MASTER)
    {
        xaf_adev_t *p_adev = (xaf_adev_t *)adev_ptr;

        if((p_adev->adev_state < XAF_ADEV_INIT))
        {
            return XAF_API_ERR;
        }

        /* mem stats info is complete only after components are initialzed.
         * Recommended to capture stats before device is closed. */
#if (XF_CFG_CORES_NUM > 1)
        *((WORD32 *)pmem_info + 0) = xf_g_ap->dsp_comp_buf_size_peak[XAF_MEM_ID_COMP];
        *((WORD32 *)pmem_info + 1) = xf_g_ap->dsp_frmwk_buf_size_peak[XAF_MEM_ID_DEV];
        *((WORD32 *)pmem_info + 2) = xf_g_ap->xaf_memory_used;
        *((WORD32 *)pmem_info + 3) = xf_g_ap->dsp_shmem_buf_size_peak[XAF_MEM_ID_DSP];
#else
        *((WORD32 *)pmem_info + 0) = xf_g_ap->dsp_comp_buf_size_peak[XAF_MEM_ID_COMP];
        *((WORD32 *)pmem_info + 1) = xf_g_ap->dsp_frmwk_buf_size_peak[XAF_MEM_ID_DEV];
        *((WORD32 *)pmem_info + 2) = xf_g_ap->xaf_memory_used;
        *((WORD32 *)pmem_info + 3) = xf_g_ap->dsp_comp_buf_size_curr[XAF_MEM_ID_COMP];
        *((WORD32 *)pmem_info + 4) = xf_g_ap->dsp_frmwk_buf_size_curr[XAF_MEM_ID_DEV];
#endif
        for( i = XAF_MEM_ID_COMP+1, k=5 ; i <= XAF_MEM_ID_COMP_MAX; i++, k++)
        {
            *((WORD32 *)pmem_info + k) = xf_g_ap->dsp_comp_buf_size_peak[i];
            TRACE(INFO, _b("mem_stats[%d] comp_buf_size_peak[%d]:%d"), k, i, xf_g_ap->dsp_comp_buf_size_peak[i]);
        }

        for( i = XAF_MEM_ID_DEV+1 ; i <= XAF_MEM_ID_DEV_MAX; i++, k++)
        {
            *((WORD32 *)pmem_info + k) = xf_g_ap->dsp_frmwk_buf_size_peak[i];
            TRACE(INFO, _b("mem_stats[%d] frmwk_buf_size_peak[%d]:%d"), k, i, xf_g_ap->dsp_frmwk_buf_size_peak[i]);
        }

#if (XF_CFG_CORES_NUM > 1)
        for( i = XAF_MEM_ID_DSP+1 ; i <= XAF_MEM_ID_DSP_MAX; i++, k++)
        {
            *((WORD32 *)pmem_info + k) = xf_g_ap->dsp_shmem_buf_size_peak[i];
            TRACE(INFO, _b("mem_stats[%d] dsp_shmem_size_peak[%d]:%d"), k, i, xf_g_ap->dsp_shmem_buf_size_peak[i]);
        }
#endif
    }
    return XAF_NO_ERR;
}

XAF_ERR_CODE xaf_pause(pVOID comp_ptr, WORD32 port)
{
    xaf_comp_t    *p_comp;
    WORD32         resp_err;

    p_comp = (xaf_comp_t *)comp_ptr;

    XAF_CHK_PTR(p_comp);
    XAF_CHK_RANGE(port, 0, (p_comp->inp_ports + p_comp->out_ports - 1));

    XAF_COMP_STATE_CHK(p_comp);

    resp_err = XF_CHK_API(xf_pause(&p_comp->handle, port));

    /* ...tbd note paused port */

    return resp_err;
}

XAF_ERR_CODE xaf_resume(pVOID comp_ptr, WORD32 port)
{
    xaf_comp_t    *p_comp;
    WORD32         resp_err;

    p_comp = (xaf_comp_t *)comp_ptr;

    XAF_CHK_PTR(p_comp);
    XAF_CHK_RANGE(port, 0, (p_comp->inp_ports + p_comp->out_ports - 1));

    XAF_COMP_STATE_CHK(p_comp);

    /* ...tbd check if the port is paused, else return */

    resp_err = XF_CHK_API(xf_resume(&p_comp->handle, port));

    return resp_err;
}

XAF_ERR_CODE xaf_probe_start(pVOID comp_ptr)
{
    xaf_comp_t    *p_comp;
    xf_handle_t   *p_handle;

    p_comp = (xaf_comp_t *)comp_ptr;

    XAF_CHK_PTR(p_comp);
    if (!p_comp->probe_enabled) return XAF_INVALIDVAL_ERR;
    if (p_comp->probe_started)  return XAF_API_ERR;

    p_handle = &p_comp->handle;
    XAF_CHK_PTR(p_handle);

    XAF_COMP_STATE_CHK(p_comp);

    XF_CHK_API(xf_resume(p_handle, (p_comp->inp_ports + p_comp->out_ports)));

    p_comp->probe_started = 1;

    XF_CHK_API(xf_command(p_handle, (p_comp->inp_ports + p_comp->out_ports), XF_FILL_THIS_BUFFER, p_comp->probe_buf, p_comp->probe_length));

    p_comp->pending_resp++;

    return XAF_NO_ERR;
}

XAF_ERR_CODE xaf_probe_stop(pVOID comp_ptr)
{
    xaf_comp_t    *p_comp;
    xf_handle_t   *p_handle;

    p_comp = (xaf_comp_t *)comp_ptr;

    XAF_CHK_PTR(p_comp);
    if (!p_comp->probe_enabled) return XAF_INVALIDVAL_ERR;
    /* if (!p_comp->probe_started) return XAF_API_ERR; */

    p_handle = &p_comp->handle;
    XAF_CHK_PTR(p_handle);

    XAF_COMP_STATE_CHK(p_comp);

    XF_CHK_API(xf_pause(p_handle, (p_comp->inp_ports + p_comp->out_ports)));

    p_comp->probe_started = 0;

    return XAF_NO_ERR;
}

#ifndef XA_DISABLE_EVENT
XAF_ERR_CODE xaf_create_event_channel(pVOID p_src, UWORD32 src_config_param, pVOID p_dest, UWORD32 dst_config_param, UWORD32 nbuf, UWORD32 buf_size)
{
    xaf_comp_t  *src_comp, *dest_comp;

    src_comp  = (xaf_comp_t *) p_src;
    dest_comp = (xaf_comp_t *) p_dest;

    XAF_CHK_PTR(src_comp);
    XAF_COMP_STATE_CHK(src_comp);
    XAF_CHK_RANGE(nbuf, 1, XF_MAX_EVENT_BUFS_PER_COMP);

    return xaf_setup_event_channel(src_comp, src_config_param, dest_comp, dst_config_param, nbuf, buf_size);
}

XAF_ERR_CODE xaf_delete_event_channel(pVOID p_src, UWORD32 src_config_param, pVOID p_dest, UWORD32 dst_config_param)
{
    xaf_comp_t *src_comp, *dest_comp;

    src_comp  = (xaf_comp_t *) p_src;
    dest_comp = (xaf_comp_t *) p_dest;

    XAF_CHK_PTR(src_comp);
    XAF_COMP_STATE_CHK(src_comp);

    return xaf_destroy_event_channel(src_comp, src_config_param, dest_comp, dst_config_param);
}
#endif
