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
 * xaf-ipc-if.c
 *
 *  Created on: Mar 5, 2021
 *      Author: nddivya
 */

/*******************************************************************************
                            XAF IPC LAYER
 ******************************************************************************/
#define MODULE_TAG                      IPC

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf-dp.h"

/*******************************************************************************
                            platform-specific
 ******************************************************************************/

//typedef _platform_lock_t          xf_ipc_lock_t;

/*******************************************************************************
                           macros
 ******************************************************************************/
#if XF_CFG_CORES_NUM>1
#define CACHE_FIX   1

//TODO xf_ipc_handle_t g_ipc_handle[XF_CFG_CORES_NUM] = {NULL};
xf_ipc_handle xf_g_ipc_handle;

/* ...additional params - mem_alloc_wrapper, TBD */

static inline void xf_ipc_assert(UWORD32 core, UWORD32 *pinterrupt)
{
#if 0
    /* ...xipc_proc_notify shudnt be used directly, use the alternate API to trigger interrupt */
    switch(core)
    {
        case 0:
            XIPC_PROC_INTERRUPT(core0, XF_PROC_INTERRUPT_NUMBER);
        break;
#if XF_CFG_CORES_NUM>1
        case 1:
            XIPC_PROC_INTERRUPT(core1, XF_PROC_INTERRUPT_NUMBER);
        break;
#endif
#if XF_CFG_CORES_NUM>2
        case 2:
            XIPC_PROC_INTERRUPT(core2, XF_PROC_INTERRUPT_NUMBER);
        break;
#endif
#if XF_CFG_CORES_NUM>3
        case 3:
            XIPC_PROC_INTERRUPT(core3, XF_PROC_INTERRUPT_NUMBER);
        break;
#endif
    }
#else
    __xf_ipc_interrupt_notify(core, pinterrupt);
#endif

}

/* ...for core's internal use - wake up dsp thread from worker threads */
int xf_ipc_resume2(unsigned int core)
{
    /* ...set ipc RTOS event */
    if(core == XF_CORE_ID_MASTER)
    {
        xf_core_ro_data_t *ro = XF_CORE_RO_DATA(core);

        __xf_event_set_isr(ro->ipc.msgq_event, CMD_MSGQ_READY);
    }
    else
    {
        __xf_event_set_isr(xf_g_dsp->pmsgq_event, CMD_MSGQ_READY);
    }
    return 0;
}

/* ...interrupt handler for each core */
void xf_ipc_interrupt_handler(void *pcore)
{
    __xf_disable_interrupt(XF_PROC_INTERRUPT_NUMBER);

    xf_ipc_resume2(*(UWORD32 *)pcore);

    /* ...clear interrupt for this core */
    __xf_ipc_interrupt_clear(*(UWORD32 *)pcore);

    __xf_enable_interrupt(XF_PROC_INTERRUPT_NUMBER);
}

/* ...push message in IPC FIFO queue */
static inline int xf_ipc_msg_enqueue(xf_ipc_msg_queue_t *queue, void *m, unsigned int next_offset, unsigned int msg_size)
{
    int     empty = (queue->head == NULL);

    BUG( *(void **)((unsigned int)m + next_offset) != NULL, _x("message is active: %p"), (m));

    /* ...set list terminating pointer */
    *(void **)((unsigned int)m + next_offset) = NULL;

    if (empty)
        queue->head = m;
    else
    {
        /* ...invalidate before updating current m->next */
        XF_IPC_INVALIDATE(queue->tail, msg_size);
        *(void **)((unsigned int)(queue->tail) + next_offset) = m;
#if CACHE_FIX
        XF_IPC_FLUSH(queue->tail, msg_size); //write m->next to memory
#endif
    }

    /* ...advance tail pointer */
    queue->tail = m;

    /* ...return emptiness status */
    return empty;
}

/* ...retrieve (pop) next message from IPC FIFO queue */
static inline void * xf_ipc_msg_dequeue(xf_ipc_msg_queue_t *queue, unsigned int next_offset, unsigned int msg_size)
{
    void *m = queue->head;

    /* ...check if there is anything in the queue and dequeue it */
    if (m)
    {
#if CACHE_FIX
        XF_IPC_INVALIDATE(m, msg_size);
#endif
        /* ...advance head to the next entry in the queue */
        if ((queue->head = (*(void **)((unsigned int)m + next_offset))) == NULL)
            queue->tail = NULL;

        /* ...debug - wipe out next pointer */
        *(void **)((unsigned int)m + next_offset) = NULL;

#if CACHE_FIX
        XF_IPC_FLUSH(m, msg_size);//to write m->next = NULL to memory
#endif
    }

    return m;
}

int xf_ipc_open2(unsigned int core, xf_ipc_config_t *pcfg)
{
    xf_ipc_queue_t *rw_ipc;
    UWORD32 aligned_offset, i, k;
    xaf_mem_pool_type_t (*ppmem_pool) [XAF_MEM_ID_MAX] = (xaf_mem_pool_type_t (*)[XAF_MEM_ID_MAX])pcfg->phandle;
    UWORD32 phandle = (UWORD32)((*ppmem_pool)[XAF_MEM_ID_DSP].pmem);
    UWORD32 phandle_aligned = phandle;

    /* ...malloc ipc handle, store pointer in g_ipc_handle[core] */
 	xf_g_ipc_handle = (xf_ipc_handle)(((UWORD32)phandle + XF_IPC_CACHE_ALIGNMENT- 1) & ~(XF_IPC_CACHE_ALIGNMENT - 1));
	phandle_aligned = (UWORD32)xf_g_ipc_handle;
    /* ...adjust the handle according to the structure's cache alignmnent requirement */
    pcfg->handle_size -= ((UWORD32)phandle_aligned - (UWORD32)phandle);

    /* ...initialize the shared memory structure between cores. This is done once at the master and later accessed by all cores */
    aligned_offset = XF_MM(sizeof(xf_ipc_struct_t));
    /* ... update shmem pointer and size for next use */
 	phandle_aligned += aligned_offset;
 	pcfg->handle_size -= aligned_offset;

    if(core == XF_CORE_ID_MASTER)
    {
        /* ... offset of 'next' in the type of message enqueue/dequeud in IPC queue */
        (XF_SHMEM_IPC_HANDLE(core))->msg_next_offset = pcfg->msg_next_offset;
        (XF_SHMEM_IPC_HANDLE(core))->xf_dsp_shmem_buffer[XAF_MEM_ID_DSP] = (xaf_mem_pool_type_t *)phandle_aligned;

        /* ...initialize shared memory heap */
        XF_CHK_API(xf_ipc_mm_init(&(XF_SHMEM_IPC_HANDLE(core)->xf_dsp_shmem_pool[XAF_MEM_ID_DSP]), (xaf_mem_pool_type_t *)phandle_aligned, pcfg->handle_size, XAF_MEM_ID_DSP));

        /* ...master->worker communication message pool. For now used only for core-deinit command from master to worker DSPs. */
        if (xf_msg_pool_init(&XF_CORE_DATA(core)->dsp_dsp_shmem_pool, XF_CFG_CORES_NUM-1, core, 1, XAF_MEM_ID_DSP))
        {
            return XAF_INVALIDPTR_ERR;
        }

        /* ... Add shared memory used for top level IPC structure handle */
        (XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_peak[XAF_MEM_ID_DSP] += ((UWORD32)phandle_aligned - (UWORD32)phandle);
        (XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr[XAF_MEM_ID_DSP] += ((UWORD32)phandle_aligned - (UWORD32)phandle);

        for(i = 0; i <= XAF_MEM_ID_MAX; i++)
        {
            for(k = XAF_MEM_ID_DSP+1; k <= XAF_MEM_ID_DSP_MAX; k++)
            {
                if((*ppmem_pool)[i].mem_id == k)
                {
                    UWORD32 size = (UWORD32)((*ppmem_pool)[i].size);
                    phandle = (UWORD32)((*ppmem_pool)[i].pmem);

                    /* ...adjust the handle according to the structure's cache alignmnent requirement */
                    phandle_aligned = (((UWORD32)phandle + XF_IPC_CACHE_ALIGNMENT- 1) & ~(XF_IPC_CACHE_ALIGNMENT - 1));
                    size -= ((UWORD32)phandle_aligned - (UWORD32)phandle);

                    /* ...update DSP-shmem pointer in the shared ipc structure. */
                    (XF_SHMEM_IPC_HANDLE(core))->xf_dsp_shmem_buffer[i] = (xaf_mem_pool_type_t *)phandle_aligned;
                    /* ...initialize shared memory */
                    XF_CHK_API(xf_ipc_mm_init(&(XF_SHMEM_IPC_HANDLE(core)->xf_dsp_shmem_pool[i]), (xaf_mem_pool_type_t *)phandle_aligned, size, k));
                    TRACE(INFO, _b("DSP shmem pool type:%d size:%d pmem:%p initialized"), i, size, (pVOID )phandle_aligned);

                    (XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_peak[i] += ((UWORD32)phandle_aligned - (UWORD32)phandle);
                    (XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr[i] += ((UWORD32)phandle_aligned - (UWORD32)phandle);
                    break;
                }
            }//for(;k;)
        }//for(;i;)

        /* ...flush the structure instead of its select variables. */
        XF_IPC_FLUSH((XF_SHMEM_IPC_HANDLE(core)), sizeof(*(XF_SHMEM_IPC_HANDLE(core))));
    }
    else
    {
        /* ...initialize RTOS event for ipc to wait on */
        __xf_event_init(&xf_g_dsp->msgq_event, 0xffff);
        xf_g_dsp->pmsgq_event = &xf_g_dsp->msgq_event;
    }

    rw_ipc = XF_CORE_IPC_DATA(core);
    /* ...initialize platform specific lock(each core initializes its inbound/command ipc msgq lock) */
    rw_ipc->lock = (xf_ipc_lock_t *)&shared_mem_ipc_lock_msgq[core][0];
    __xf_ipc_lock_init(rw_ipc->lock);
    /* ...initialize msg queue for 'core' */
    rw_ipc->queue.head = rw_ipc->queue.tail = NULL;
    /* ...interrupt type of this core can be known by other-cores only through shared memory, this is one time flushed if cache is enabled */
    rw_ipc->queue.interrupt[0] = Xthal_inttype[XF_PROC_INTERRUPT_NUMBER];

    XF_IPC_FLUSH(rw_ipc->lock, sizeof(xf_ipc_lock_t));
    XF_IPC_FLUSH(rw_ipc, sizeof(*rw_ipc));

    return 0;
}

/* ...additional params - mem_free_wrapper - TBD */
int xf_ipc_close2(unsigned int core)
{
    xf_ipc_queue_t *rw_ipc = XF_CORE_IPC_DATA(core);

    /* ...de-initialize RTOS event, platform specific lock , message queue */
    __xf_ipc_lock_destroy(rw_ipc->lock);

    if(core != XF_CORE_ID_MASTER)
    {
        __xf_event_destroy(&xf_g_dsp->msgq_event);
    }

    /* ... update shared memory stats for local use */
    XF_IPC_INVALIDATE(&(XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_peak, sizeof((XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_peak));
    XF_IPC_INVALIDATE(&(XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr, sizeof((XF_SHMEM_IPC_HANDLE(core))->dsp_shmem_buf_size_curr));

    __xf_disable_interrupt(XF_PROC_INTERRUPT_NUMBER);

    return 0;
}

int xf_ipc_send2(unsigned int core /* dst */, void *msg, unsigned int msg_size, void *payload, unsigned int payload_size)
{
    xf_ipc_queue_t *rw_ipc = XF_CORE_IPC_DATA(core);

    /* ...assure memory coherency as needed */
    /* ...flush msg, msg_size */
    //XF_IPC_FLUSH(payload, payload_size); //this is done outside IPC-layer just before calling this function

    XF_IPC_INVALIDATE(rw_ipc, sizeof(*rw_ipc)); //1 time
    XF_IPC_INVALIDATE(rw_ipc->lock, sizeof(xf_ipc_lock_t));
    /* ...acquire lock */
    __xf_ipc_lock(rw_ipc->lock);
    XF_IPC_FLUSH(rw_ipc->lock, sizeof(xf_ipc_lock_t));

    /* ...invalidate msg queue struct */
    //XF_IPC_INVALIDATE(rw_ipc, sizeof(*rw_ipc));
    XF_IPC_INVALIDATE(&rw_ipc->queue, sizeof(rw_ipc->queue));

    /* ...submit ipc msg to queue */
    xf_ipc_msg_enqueue(&rw_ipc->queue, msg, (XF_SHMEM_IPC_HANDLE(core))->msg_next_offset, msg_size);

    /* ...flush msg queue struct */
    //XF_IPC_FLUSH(rw_ipc, sizeof(*rw_ipc));
    XF_IPC_FLUSH(&rw_ipc->queue, sizeof(rw_ipc->queue));

    XF_IPC_FLUSH(msg, msg_size);

    //TRACE(CMD, _b("m:%p id:%016llx op:%08x len:%d buf:%p"), msg, ((xf_message_t*)msg)->id, ((xf_message_t*)msg)->opcode, ((xf_message_t*)msg)->length, ((xf_message_t*)msg)->buffer);

    /* ...release lock */
    __xf_ipc_unlock(rw_ipc->lock);
    XF_IPC_FLUSH(rw_ipc->lock, sizeof(xf_ipc_lock_t));

    /* ...assert interrupt on destination 'core' */
    xf_ipc_assert(core, rw_ipc->queue.interrupt);

    return 0;
}

int xf_ipc_recv2(unsigned int core, void **ppm, unsigned int msg_size)
{
    xf_ipc_queue_t *rw_ipc = XF_CORE_IPC_DATA(core);

    void *msg;

    /* ...process memory coherency as required */
    XF_IPC_INVALIDATE(rw_ipc, sizeof(*rw_ipc)); //1 time
    XF_IPC_INVALIDATE(rw_ipc->lock, sizeof(xf_ipc_lock_t));

    /* ...acquire lock */
    __xf_ipc_lock(rw_ipc->lock);
    XF_IPC_FLUSH(rw_ipc->lock, sizeof(xf_ipc_lock_t));

    /* ...invalidate msg queue struct */
    XF_IPC_INVALIDATE(&rw_ipc->queue, sizeof(rw_ipc->queue));

    /* ...retrieve ipc msg from queue */
    msg = xf_ipc_msg_dequeue(&rw_ipc->queue, (XF_SHMEM_IPC_HANDLE(core))->msg_next_offset, msg_size);

    /* ...invalidate retrieved msg, msg_size */
    if (msg)
    {
        /* ...flush msg queue struct */
        //XF_IPC_FLUSH(rw_ipc, sizeof(*rw_ipc));
        XF_IPC_FLUSH(&rw_ipc->queue, sizeof(rw_ipc->queue));
#if !CACHE_FIX
        XF_IPC_INVALIDATE(msg, msg_size); //its done in the dequeue function
#endif
       //TRACE(RSP, _b("m:%p id:%016llx op:%08x len:%d buf:%p"), msg, (UWORD64)((xf_message_t*)msg)->id, ((xf_message_t*)msg)->opcode, ((xf_message_t*)msg)->length, ((xf_message_t*)msg)->buffer);
    }
    /* ...release lock */
    __xf_ipc_unlock(rw_ipc->lock);
    XF_IPC_FLUSH(rw_ipc->lock, sizeof(xf_ipc_lock_t));

    /* ...return message */
    *(void **)ppm = msg;

    return 0;
}

int xf_ipc_wait2(unsigned int core)
{
    volatile xf_event_t *msgq_event;
    int retval = 1;

    msgq_event = xf_g_dsp->pmsgq_event;

    /* ...enable ipc interrupt */

    /* ...wait on ipc RTOS event */
    __xf_event_wait_any((xf_event_t*)msgq_event, CMD_MSGQ_READY | DSP_DIE_MSGQ_ENTRY);
    if (__xf_event_get((xf_event_t*)msgq_event) & DSP_DIE_MSGQ_ENTRY)
    {
        retval = 0;
    }

    /* ...clear RTOS event */
    __xf_event_clear((xf_event_t*)msgq_event, CMD_MSGQ_READY | DSP_DIE_MSGQ_ENTRY);

    /* ...disable ipc interrupt */

    return retval;
}

int xf_ipc_b2a_flush(unsigned int core, void **buffer, unsigned int buf_size)
{
    /* ...translate *buffer address, if required */
    /* ...flush *buffer, size */
    XF_IPC_FLUSH(*buffer, buf_size);

    return 0;
}

int xf_ipc_a2b_invalidate(unsigned int core, void **buffer, unsigned int buf_size)
{
    /* ...translate *buffer address, if required */
    /* ...invalidate *buffer, size */
    XF_IPC_INVALIDATE(*buffer, buf_size);

    return 0;
}

#endif //XF_CFG_CORES_NUM>1
