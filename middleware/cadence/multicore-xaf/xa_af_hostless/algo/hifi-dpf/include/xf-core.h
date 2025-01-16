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
 * xf-core.h
 *
 * DSP processing framework core definitions
 *******************************************************************************/

#ifndef __XF_H
#error "xf-core.h mustn't be included directly"
#endif

/*******************************************************************************
 * Shared core data
 ******************************************************************************/

/* ...core data with read-only access from remote cores */
typedef struct xf_core_ro_data
{
    /* ...platform-specific multi-core mutex data (want to have an array? - tbd) */
    xf_mutex_t          lock;

    /* ...opaque platform-specific IPC-data handle */
    xf_ipc_handle_t     ipc;

    /* ...shared memory message pool data - here? - tbd */
    xf_msg_pool_t       pool;

    /* ...anything else? - tbd */

}   xf_core_ro_data_t;

/* ...core data with read-write access from remote cores */
typedef struct xf_core_rw_data
{
    /* ...message queue containing local commands/responses */
#if 1 //AR3, AR7
#else
    xf_sync_queue_t     local;
#endif

    /* ...message queue containing responses to App Interface Layer (if enabled) */
    xf_sync_queue_t     remote;

    /* ...pointer to shared memory data? anything else? - tbd */

}   xf_core_rw_data_t;

/* ...proper cache-line aligned core data */
XF_MAX_CACHE_ALIGNED_TYPEDEF(xf_core_ro_data_t, __xf_core_ro_data_t);
XF_MAX_CACHE_ALIGNED_TYPEDEF(xf_core_rw_data_t, __xf_core_rw_data_t);

/*******************************************************************************
 * Global data definition - hmm... - tbd
 ******************************************************************************/


/* ...shared read-only memory access */
//#define XF_CORE_RO_DATA(core)   ((xf_core_ro_data_t *)(&(xf_g_dsp->xf_core_ro_data[(core)])))
#define XF_CORE_RO_DATA(core)   ((xf_core_ro_data_t *)(&(xf_g_dsp->xf_core_ro_data[(0)])))

/* ...shared read-write memory access */
//#define XF_CORE_RW_DATA(core)   ((xf_core_rw_data_t *)(&(xf_g_dsp->xf_core_rw_data[(core)])))
#define XF_CORE_RW_DATA(core)   ((xf_core_rw_data_t *)(&(xf_g_dsp->xf_core_rw_data[(0)])))

/*******************************************************************************
 * Local core data (not accessible from remote cores)
 ******************************************************************************/

/* ...component map entry */
typedef union xf_cmap_link
{
    /* ...poiner to active client */
    xf_component_t     *c;

    /* ...index to a client in the list (values 0..XF_CFG_MAX_CLIENTS) */
    UWORD32                 next;

}   xf_cmap_link_t;

struct xf_worker {
    void *stack;
    void *scratch;
    xf_msgq_t queue;
    xf_thread_t thread;
    UWORD32 core;
    xf_msg_queue_t local_msg_queue;
    xf_sched_t          sched;
};

/* ...per-core local data */
typedef struct xf_core_data
{
    /* ...scheduler queue (sorted by execution timestamp) */
    xf_sched_t          sched;

    /* ...command/response queue for communication within local core (including ISRs) */
    xf_sync_queue_t     queue;

#if 0
    /* ...pending response queue (submitted from ISR context) */
    xf_sync_queue_t     response;
#endif

    /* ...per-core component mapping */
    xf_cmap_link_t      cmap[XF_CFG_MAX_CLIENTS];

    /* ...index of first free client */
    UWORD32                 free;

    /* ...local DSP memory pool */
    xf_mm_pool_t        local_pool[XAF_MEM_ID_MAX];

    /* ...shared AP-DSP memory pool (if enabled) */
    xf_mm_pool_t        shared_pool[XAF_MEM_ID_MAX];

    /* ...opaque system-specific shared memory data handle */
    xf_shmem_handle_t   shmem;

    /* ...scratch memory pointer */
    void               *scratch;

    /* ...tracer data */
    xf_trace_data_t     trace;

    UWORD32 n_workers;

    struct xf_worker *worker;
    /* ...any debugging information? for memory allocation etc... ? */

    /* ...the default priority to be set on component creation, before its actual priority can be assigned */
    UWORD32 component_default_priority_idx;

    /* ...worker thread scratch sizes */
    UWORD32 worker_thread_scratch_size[XAF_MAX_WORKER_THREADS];

#if (XF_CFG_CORES_NUM > 1)
    /* ...DSP-DSP shmem MSG queue */
    //xf_msg_queue_t dsp_dsp_shmem_queue;

    /* ...DSP-DSP shmem MSG pool */
    xf_msg_pool_t   dsp_dsp_shmem_pool;
#endif

    /* ...dsp thread priority */
    UWORD32 dsp_thread_priority;

    /* ...worker thread stack sizes */
    UWORD32 worker_thread_stack_size[XAF_MAX_WORKER_THREADS];

}   xf_core_data_t;

#if (XF_CFG_CORES_NUM > 1)
/* ...shared memmory allocator data */
typedef struct __xf_shared_mm_pool
{
    /* ...platform specific lock */
    xf_ipc_lock_t   *lock;

    /* ...free blocks map sorted by block length */
    rb_tree_t       l_map;
    /* ...free blocks map sorted by address of the block */
    rb_tree_t       a_map;
    /* ...address of memory pool (32-bytes aligned at least); */
    void           *addr;
    /* ...length of the pool (multiple of descriptor size); */
    UWORD32             size;

}   xf_shared_mm_pool_t;
#endif

/*******************************************************************************
 * Global data definition
 ******************************************************************************/

typedef struct {
    /* ...per-core execution data */
    //xf_core_data_t          xf_core_data[XF_CFG_CORES_NUM];
    xf_core_data_t          xf_core_data[1];

    /* ...AP-DSP shared memory pool */
    //xf_mm_pool_t            xf_ap_shmem_pool;

    /* ...per-core local memory pool */
    //xf_mm_pool_t            xf_dsp_local_pool[XF_CFG_CORES_NUM]; //unused

#if (XF_CFG_CORES_NUM > 1)
    /* ...DSP cluster shared memory pool */
    //xf_shared_mm_pool_t        xf_dsp_shmem_pool;
#endif    // #if XF_CFG_CORES_NUM > 1

    /* ...per-core shared memory with read-only access */
    //__xf_core_ro_data_t     xf_core_ro_data[XF_CFG_CORES_NUM];
    __xf_core_ro_data_t     xf_core_ro_data[1];

    /* ...per-core shared memory with read-write access */
    //__xf_core_rw_data_t     xf_core_rw_data[XF_CFG_CORES_NUM];
    __xf_core_rw_data_t     xf_core_rw_data[1];

    WORD32 (*pdsp_comp_buf_size_peak)[XAF_MEM_ID_MAX];    /* ...cumulative buffer size used in bytes from audio_comp_buf_size   */
    WORD32 (*pdsp_comp_buf_size_curr)[XAF_MEM_ID_MAX];    /* ...current usage from audio_comp_buf_size in bytes                 */
    WORD32 (*pdsp_frmwk_buf_size_peak)[XAF_MEM_ID_MAX];   /* ...cumulative buffer size used in bytes from audio_frmwk_buf_size  */
    WORD32 (*pdsp_frmwk_buf_size_curr)[XAF_MEM_ID_MAX];   /* ...current usage from audio_frmwk_buf_size in bytes                */
    xaf_perf_stats_t *pdsp_cb_stats;                      /* ...for cumulative execution cycles of all worker threads of a core */
    int (*cb_compute_cycles)(xaf_perf_stats_t*);          /* ...call-back function used by DSP to update MCPS stats before DSP thread is deleted */

#if (XF_CFG_CORES_NUM > 1)
    xf_event_t msgq_event;
    xf_event_t *pmsgq_event;
#endif

    UWORD32 worker_thread_stack_size[XAF_MAX_WORKER_THREADS]; /* ...user configurable worker stack size */

    xaf_mem_pool_type_t mem_pool[XAF_MEM_ID_MAX];
} xf_dsp_t;

extern xf_dsp_t *xf_g_dsp;

typedef struct xf_worker_msg {
    xf_component_t *component;
    xf_message_t *msg;
} xf_worker_msg_t;

/*******************************************************************************
 * API functions
 ******************************************************************************/

/* ...initialize per-core framework data */
extern int  xf_core_init(UWORD32 core);

/* ...deinitialize per-core framework data */
extern int xf_core_deinit(UWORD32 core);

/* ...global data initialization function */
extern int  xf_global_init(void);

/* ...process core events */
extern void xf_core_service(UWORD32 core);
extern void xf_core_process(xf_component_t *component);
extern void xf_core_process_message(xf_component_t *component, xf_message_t *msg);
