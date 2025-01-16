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
/*
 * xf_ipc_if.h
 *
 *  Created on: Mar 5, 2021
 *      Author: nddivya
 */

#ifndef __XF_IPC_IF__
#define __XF_IPC_IF__

/*******************************************************************************
                            ipc macros
*******************************************************************************/
#define XF_IPC_CACHE_ALIGNMENT XCHAL_DCACHE_LINESIZE

#if (XF_LOCAL_IPC_NON_COHERENT)
/* ...prevent instructions reordering */
#define barrier()                           \
    __asm__ __volatile__("": : : "memory")

/* ...memory barrier */
#define XF_IPC_BARRIER()                  \
    __asm__ __volatile__("memw": : : "memory")

#define XF_IPC_FLUSH(buf, length) \
        ({ if ((length)) { barrier(); xthal_dcache_region_writeback((buf), (length)); XF_IPC_BARRIER(); } buf; })

#define XF_IPC_INVALIDATE(buf, length) \
        ({ if ((length)) { xthal_dcache_region_invalidate((buf), (length)); barrier(); } buf; })

#else //XF_LOCAL_IPC_NON_COHERENT
#define XF_IPC_FLUSH(buf, length)
#define XF_IPC_INVALIDATE(buf, length)
#endif //XF_LOCAL_IPC_NON_COHERENT

/*******************************************************************************
                            ipc data-types
*******************************************************************************/
typedef void * xf_ipc_handle;

typedef struct xf_ipc_config
{
    /* ...shmem handle */
    void *phandle;

    /* ...shmem size in bytes */
    unsigned int handle_size;

    /* ...next pointer's offset in the messages in IPC queue */
    unsigned int msg_next_offset;

}xf_ipc_config_t;

/* ...ipc message queue (single-linked FIFO list) */
typedef struct  xf_ipc_msg_queue
{
    void *head;
    void *tail;
    unsigned int interrupt[1]; /* ...[0] interrupt type, */
}   xf_ipc_msg_queue_t;

typedef struct __xf_ipc_queue
{
    /* ...platform specific lock */
    xf_ipc_lock_t *lock;

    xf_ipc_msg_queue_t queue __attribute__ ((aligned(XF_IPC_CACHE_ALIGNMENT))) ;
} xf_ipc_queue_t;

typedef struct {
    /* ...shmem data with read-write access from remote cores, g_ipc_handle */
    xf_ipc_queue_t xf_core_rw_data_local[XF_CFG_CORES_NUM];

    /* ...next pointer's offset in the messages in IPC queue */
    unsigned int msg_next_offset __attribute__ ((aligned(XF_IPC_CACHE_ALIGNMENT)));

    /* ... variables required for shared memory stats */
    void *xf_dsp_shmem_buffer[XAF_MEM_ID_MAX] __attribute__ ((aligned(XF_IPC_CACHE_ALIGNMENT)));
    int dsp_shmem_buf_size_curr[XAF_MEM_ID_MAX] __attribute__ ((aligned(XF_IPC_CACHE_ALIGNMENT)));
    int dsp_shmem_buf_size_peak[XAF_MEM_ID_MAX] __attribute__ ((aligned(XF_IPC_CACHE_ALIGNMENT)));

    /* ...DSP cluster shared memory pool, accssed by all DSPs with platform lock */
    xf_shared_mm_pool_t   xf_dsp_shmem_pool[XAF_MEM_ID_MAX] __attribute__ ((aligned(XF_IPC_CACHE_ALIGNMENT)));

} xf_ipc_struct_t __attribute__ ((aligned(XF_IPC_CACHE_ALIGNMENT)));

/* ...global shared memory handle */
#define XF_SHMEM_IPC_HANDLE(core)   ((xf_ipc_struct_t *)xf_g_ipc_handle)

/* ...global IPC handle */
#define XF_CORE_IPC_DATA(core)   (&((XF_SHMEM_IPC_HANDLE(core))->xf_core_rw_data_local[(core)]))

/*******************************************************************************
                            ipc-handle global variable export
*******************************************************************************/
extern xf_ipc_handle xf_g_ipc_handle;

/*******************************************************************************
                            ipc API functions
*******************************************************************************/
/* ...complete IPC waiting and close dsp thread */
static inline void xf_ipi_dsp_close(UWORD32 core)
{
    xf_event_t *msgq_event = xf_g_dsp->pmsgq_event;

	__xf_event_set(msgq_event, DSP_DIE_MSGQ_ENTRY);
}

extern int xf_ipc_open2(unsigned int core, xf_ipc_config_t *cfg);
extern int xf_ipc_close2(unsigned int core);

extern int xf_ipc_send2(unsigned int core /* dst */, void *pmsg, unsigned int msg_size, void *ppayload, unsigned int payload_size);
extern int xf_ipc_recv2(unsigned int core, void **msg, unsigned int msg_size);

extern int xf_ipc_wait2(unsigned int core);
extern int xf_ipc_resume2(unsigned int core);

extern void xf_ipc_interrupt_handler(void *pcore);

extern int xf_ipc_b2a_flush(unsigned int core, void **buffer, unsigned int size);
extern int xf_ipc_a2b_invalidate(unsigned int core, void **buffer, unsigned int size);

#endif /* __XF_IPC_IF__ */

