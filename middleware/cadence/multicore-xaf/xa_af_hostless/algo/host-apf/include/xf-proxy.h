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
 * xf-proxy.h
 *
 * Xtensa audio processing framework. Main API
 ******************************************************************************/

#ifndef __XF_H
#error  "xf-proxy.h mustn't be included directly"
#endif

/*******************************************************************************
 * User-message description - move from here to API - tbd
 ******************************************************************************/

/* ...need that at all? hope no */
struct xf_user_msg
{
    /* ...source component specification */
    xf_msg_id_dtype     id;

    /* ...message opcode */
    UWORD32             opcode;

    /* ...buffer length */
    UWORD32             length;

    /* ...buffer pointer */
    void           *buffer;

    /* ...error response*/
    WORD32             error;
};

/* ...proxy message - bad placing of that thing here - tbd */
struct xf_proxy_msg
{
    /* ...session-id field */
    xf_msg_id_dtype     id;

    /* ...message opcode */
    UWORD32             opcode;

    /* ...buffer length */
    UWORD32             length;

    /* ...buffer pointer */
    UWORD32             address;

    /* ...error response*/
    WORD32             error;

}   __attribute__((__packed__, __aligned__(sizeof(xf_msg_id_dtype))));

/*******************************************************************************
 * Buffer pools
 ******************************************************************************/

/* ...buffer pool type */
enum xf_pool_type
{
    XF_POOL_AUX = 0,
    XF_POOL_INPUT = 1,
    XF_POOL_OUTPUT = 2,
    XF_POOL_CONFIG = 3
};

typedef enum {
    XF_POOL_FRMWK_COMP=0,
    XF_POOL_FRMWK_MSGQ,
    XF_POOL_FRMWK_AUX,
    XF_POOL_FRMWK_INPUT,
    XF_POOL_FRMWK_OUTPUT,
#ifndef XA_DISABLE_EVENT
    XF_POOL_FRMWK_EVENT_CHANNEL,
    XF_POOL_FRMWK_EVENT_BUF,
#endif
    XF_POOL_FRMWK_EXT_CFG,
    XF_POOL_FRMWK_MAX
} XAF_MEM_POOL_FRMWK_LOCAL;

/* ...buffer link pointer */
typedef union xf_buffer_link
{
    /* ...pointer to next free buffer in a pool (for free buffer) */
    xf_buffer_t        *next;

    /* ...reference to a buffer pool (for allocated buffer) */
    xf_pool_t          *pool;

}   xf_buffer_link_t;

/* ...buffer descriptor */
struct xf_buffer
{
    /* ...virtual address of contiguous buffer */
    void               *address;

    /* ...link pointer */
    xf_buffer_link_t    link;
};

/* ...buffer pool */
struct xf_pool
{
    /* ...reference to proxy data */
    xf_proxy_t         *proxy;

    /* ...length of individual buffer in a pool */
    UWORD32                 length;

    /* ...number of buffers in a pool */
    UWORD32                 number;

    /* ...pointer to pool memory */
    void               *p;

    /* ...pointer to first free buffer in a pool */
    xf_buffer_t        *free;

    /* ...individual buffers */
    xf_buffer_t         buffer[0];
};

/* ...accessor to buffer data */
static inline void * xf_buffer_data(xf_buffer_t *buffer)
{
    return buffer->address;
}

/* ...length of buffer data */
static inline UWORD32 xf_buffer_length(xf_buffer_t *buffer)
{
    return buffer->link.pool->length;
}

/*******************************************************************************
 * Proxy handle definition
 ******************************************************************************/

/* ...free clients list */
typedef union xf_proxy_cmap_link
{
    /* ...index of next free client in the list */
    xf_msg_id_dtype             next;

    /* ...pointer to allocated component handle */
    xf_handle_t            *handle;

}   xf_proxy_cmap_link_t;

/* ...proxy data structure */
struct xf_proxy
{
    /* ...platform-specific IPC data */
    xf_proxy_ipc_data_t     ipc;

    /* ...auxiliary buffer pool for clients */
    xf_pool_t              *aux;

    /* ...global proxy lock */
    xf_lock_t               lock;

    /* ...proxy thread handle */
    xf_thread_t             thread;

    /* ...proxy identifier (core of remote DSP hosting SHMEM interface) */
    UWORD32                     core;

    /* ...client association map */
    xf_proxy_cmap_link_t    cmap[XF_CFG_PROXY_MAX_CLIENTS];

    UWORD32 proxy_thread_priority;

    UWORD32 proxy_thread_stack_size;

#ifdef DELAYED_SYNC_RESPONSE
    /* ...global proxy lock for delayed synchronous response queue */
    xf_lock_t               lock_delayed_response;
#endif
};

/*******************************************************************************
 * Auxiliary proxy helpers
 ******************************************************************************/

/* ...get proxy identifier */
static inline UWORD32 xf_proxy_id(xf_proxy_t *proxy)
{
    return proxy->core;
}

/* ...lock proxy data */
static inline void xf_proxy_lock(xf_proxy_t *proxy)
{
    __xf_lock(&proxy->lock);
}

/* ...unlock proxy data */
static inline void xf_proxy_unlock(xf_proxy_t *proxy)
{
    __xf_unlock(&proxy->lock);
}

/* ...translate proxy shared address into local virtual address */
static inline void * xf_proxy_a2b(xf_proxy_t *proxy, UWORD32 address)
{
    return xf_ipc_a2b(&proxy->ipc, address);
}

/* ...translate local virtual address into shared proxy address */
static inline UWORD32 xf_proxy_b2a(xf_proxy_t *proxy, void *b)
{
    return xf_ipc_b2a(&proxy->ipc, b);
}

/* ...submit asynchronous response message */
static inline int xf_proxy_response_put(xf_proxy_t *proxy, xf_proxy_msg_t *msg)
{
    return __xf_msgq_send(proxy->ipc.lresp_msgq, msg, sizeof(*msg));
}

/* ...retrieve asynchronous response message */
static inline int xf_proxy_response_get(xf_proxy_t *proxy, xf_proxy_msg_t *msg)
{
    return __xf_msgq_recv(proxy->ipc.lresp_msgq, msg, sizeof(*msg));
}

#ifdef DELAYED_SYNC_RESPONSE
/* ...lock proxy data */
static inline void xf_proxy_lock_delayed(xf_proxy_t *proxy)
{
    __xf_lock(&proxy->lock_delayed_response);
}

/* ...unlock proxy data */
static inline void xf_proxy_unlock_delayed(xf_proxy_t *proxy)
{
    __xf_unlock(&proxy->lock_delayed_response);
}

/* ...submit asynchronous(delayed) response message */
static inline int xf_proxy_response_put_delayed(xf_proxy_t *proxy, xf_proxy_msg_t *msg)
{
    return __xf_msgq_send(proxy->ipc.lresp_msgq_delayed, msg, sizeof(*msg));
}

/* ...retrieve asynchronous(delayed) response message */
static inline int xf_proxy_response_get_delayed(xf_proxy_t *proxy, xf_proxy_msg_t *msg)
{
    return __xf_msgq_recv(proxy->ipc.lresp_msgq_delayed, msg, sizeof(*msg));
}
#endif //DELAYED_SYNC_RESPONSE
/*******************************************************************************
 * Component handle definition
 ******************************************************************************/

struct xf_handle
{
    /* ...platform-specific IPC data */
    xf_ipc_data_t           ipc;

    /* ...reference to proxy data */
    xf_proxy_t             *proxy;

    /* ...auxiliary control buffer for control transactions */
    xf_buffer_t            *aux;

    /* ...global client-id of the component */
    xf_msg_id_dtype             id;

    /* ...local client number (think about merging into "id" field - tbd) */
    xf_msg_id_dtype             client;

    /* ...response processing hook */
    xf_response_cb          response;

    /* ...pool type of component memory types */
    UWORD32 mem_pool_type[XAF_MEM_POOL_TYPE_COMP_MAX];
};

#define PROXY_THREAD_STACK_SIZE             8192
#define DSP_THREAD_STACK_SIZE               8192
#define WORKER_THREAD_STACK_SIZE            8192

#ifndef XA_DISABLE_EVENT
/* ...events to application callback structure(relay) */
typedef struct xa_app_submit_event_cb_s
{
    WORD32 (*cb)(struct xa_app_submit_event_cb_s *, xf_msg_id_dtype comp_id, UWORD32 event_id, pVOID event_buf, UWORD32 buf_size, WORD32 error);
}xa_app_submit_event_cb_t;
#endif

typedef struct xf_ap_s {
    xf_thread_t     dsp_thread;

#if !defined(HAVE_FREERTOS)
    UWORD32 dsp_thread_stack_size;
	void *dsp_stack;
	void *proxy_thread_stack;
#endif

    ipc_msgq_t g_ipc_msgq;

    xf_lock_t   g_msgq_lock;

    xf_lock_t   g_comp_delete_lock; //TENA_2356

#ifndef XA_DISABLE_EVENT
    xaf_app_event_handler_fxn_t app_event_handler_cb;

    xa_app_submit_event_cb_t *cdata;
#endif

    int (*cb_compute_cycles)(xaf_perf_stats_t*);    /* ... call-back function used by DSP to update MCPS stats before DSP thread is deleted */

    xaf_perf_stats_t *cb_stats;                     /* ... pointer to stats structure updated by the above call-back function */

    void *dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_MAX];

    UWORD32 worker_thread_scratch_size[XAF_MAX_WORKER_THREADS]; /* ...user configurable worker scratch size */
    UWORD32 worker_thread_stack_size[XAF_MAX_WORKER_THREADS]; /* ...user configurable worker stack size */

    WORD32 xaf_memory_used; /* TENA-2155 to track all the non-shmem and non-component mallocs on the App Interface Layer */

    WORD32 dsp_comp_buf_size_peak[XAF_MEM_ID_MAX];  /* ...cumulative buffer size used in bytes from audio_comp_buf_size     */
    WORD32 dsp_frmwk_buf_size_peak[XAF_MEM_ID_MAX]; /* ...cumulative buffer size used in bytes from audio_frmwk_buf_size    */
    WORD32 dsp_comp_buf_size_curr[XAF_MEM_ID_MAX];  /* ...current usage from audio_comp_buf_size in bytes                   */
    WORD32 dsp_frmwk_buf_size_curr[XAF_MEM_ID_MAX]; /* ...current usage from audio_frmwk_buf_size in bytes                  */
    WORD32 dsp_shmem_buf_size_peak[XAF_MEM_ID_MAX]; /* ...cumulative buffer size used in bytes from shared_memory           */
    WORD32 dsp_shmem_buf_size_curr[XAF_MEM_ID_MAX]; /* ...current usage from from shared_memory in bytes                    */

    UWORD32 core;

    char *pfrmwk_local_buffer;
    UWORD32 frmwk_local_memory_peak;
    UWORD32 frmwk_local_memory_curr;
    UWORD32 frmwk_local_memory_size;                /* ...initial available size                    */
    xf_pool_t *pfrmwk_pool_free[XF_POOL_FRMWK_MAX]; /* ...linked list of free memory blocks         */
    xf_pool_t *pfrmwk_pool_alloc[XF_POOL_FRMWK_MAX];/* ...linked list of allocated memory blocks    */

    xaf_mem_pool_type_t mem_pool[XAF_MEM_ID_MAX];
} xf_ap_t;

typedef struct xf_dsp_s {
    xf_thread_t     dsp_thread;

#if !defined(HAVE_FREERTOS)
    UWORD32 dsp_thread_stack_size;
	void *dsp_stack;
#endif

    ipc_msgq_t g_ipc_msgq;

    int (*cb_compute_cycles)(xaf_perf_stats_t*);    /* ... call-back function used by worker DSP to update MCPS stats before DSP thread is deleted */

    xaf_perf_stats_t *cb_stats;                     /* ... pointer to stats structure updated by the above call-back function */

    void *dsp_thread_args[XF_DSP_THREAD_ARGS_IDX_MAX];

    UWORD32 worker_thread_scratch_size[XAF_MAX_WORKER_THREADS]; /* ...user configurable worker scratch size */
    UWORD32 worker_thread_stack_size[XAF_MAX_WORKER_THREADS]; /* ...user configurable worker stack size */

    WORD32 xaf_memory_used; /* TENA-2155 to track all the non-shmem and non-component mallocs on the App Interface Layer */

    WORD32 dsp_comp_buf_size_peak[XAF_MEM_ID_MAX];  /* ...cumulative buffer size used in bytes from audio_comp_buf_size     */
    WORD32 dsp_frmwk_buf_size_peak[XAF_MEM_ID_MAX]; /* ...cumulative buffer size used in bytes from audio_frmwk_buf_size    */
    WORD32 dsp_comp_buf_size_curr[XAF_MEM_ID_MAX];  /* ...current usage from audio_comp_buf_size in bytes                   */
    WORD32 dsp_frmwk_buf_size_curr[XAF_MEM_ID_MAX]; /* ...current usage from audio_frmwk_buf_size in bytes                  */
    WORD32 dsp_shmem_buf_size_peak[XAF_MEM_ID_MAX]; /* ...cumulative buffer size used in bytes from shared_memory           */
    WORD32 dsp_shmem_buf_size_curr[XAF_MEM_ID_MAX]; /* ...current usage from from shared_memory in bytes                    */

    UWORD32 core;

    char *pfrmwk_local_buffer;
    UWORD32 frmwk_local_memory_peak;
    UWORD32 frmwk_local_memory_curr;
    UWORD32 frmwk_local_memory_size;                /* ...initial available size                    */
    xf_pool_t *pfrmwk_pool_free[XF_POOL_FRMWK_MAX]; /* ...linked list of free memory blocks         */
    xf_pool_t *pfrmwk_pool_alloc[XF_POOL_FRMWK_MAX];/* ...linked list of allocated memory blocks    */

    xaf_mem_pool_type_t mem_pool[XAF_MEM_ID_MAX];
} xf_dsp_t;

#if 0
typedef struct {
    /* ...per-core execution data */
    UWORD8 xf_core_data[XF_DSP_OBJ_SIZE_CORE_DATA];

    /* ...AP-DSP shared memory pool */
    //xf_mm_pool_t            xf_ap_shmem_pool;

    /* ...per-core local memory pool */
    UWORD8 xf_dsp_local_pool[XF_DSP_OBJ_SIZE_DSP_LOCAL_POOL];    //at DSP Interface Layer this buffer is 40 bytes

#if XF_CFG_CORES_NUM > 1
    /* ...DSP cluster shared memory pool */
    UWORD8 xf_dsp_shmem_pool[XF_DSP_OBJ_SIZE_DSP_SHMEM_POOL];
#endif    // #if XF_CFG_CORES_NUM > 1

    /* ...per-core shared memory with read-only access */
    UWORD8 xf_core_ro_data[XF_DSP_OBJ_SIZE_CORE_RO_DATA];

    /* ...per-core shared memory with read-write access */
    UWORD8 xf_core_rw_data[XF_DSP_OBJ_SIZE_CORE_RW_DATA];

    UWORD8 *xf_ap_shmem_buffer;
    WORD32 xf_ap_shmem_buffer_size;

#if XF_CFG_CORES_NUM > 1
    UWORD8 *xf_dsp_shmem_buffer;
    UWORD16 xf_dsp_shmem_buffer_size;
#endif    // #if XF_CFG_CORES_NUM > 1

    UWORD8 *xf_dsp_local_buffer;
    WORD32 xf_dsp_local_buffer_size;
    WORD32 dsp_comp_buf_size_peak;  /* cumulative buffer size used in bytes from audio_comp_buf_size */
    WORD32 dsp_frmwk_buf_size_peak;  /* cumulative buffer size used in bytes from audio_frmwk_buf_size */
    WORD32 dsp_comp_buf_size_curr;   /* current usage from audio_comp_buf_size in bytes */
    WORD32 dsp_frmwk_buf_size_curr;  /* current usage from audio_frmwk_buf_size in bytes */

} xf_dsp_t;
#endif

/*******************************************************************************
 * Auxiliary component helpers
 ******************************************************************************/

/* ...component client-id (global scope) */
static inline xf_msg_id_dtype xf_handle_id(xf_handle_t *handle)
{
    return handle->id;
}

/* ...pointer to auxiliary buffer */
static inline void * xf_handle_aux(xf_handle_t *handle)
{
    return xf_buffer_data(handle->aux);
}

/* ...put asynchronous response into local IPC */
static inline int xf_response_put(xf_handle_t *handle, xf_user_msg_t *msg)
{
    return __xf_msgq_send(handle->ipc.resp_msgq, msg, sizeof(*msg));
}

/* ...get asynchronous response from local IPC */
static inline int xf_response_get(xf_handle_t *handle, xf_user_msg_t *msg)
{
    return __xf_msgq_recv(handle->ipc.resp_msgq, msg, sizeof(*msg));
}
