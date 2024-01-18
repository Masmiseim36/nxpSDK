/*
* Copyright (c) 2015-2022 Cadence Design Systems Inc.
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
 * xf-main.c
 *
 * DSP processing framework main file
 ******************************************************************************/

#define MODULE_TAG                      MAIN

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf-dp.h"

/*******************************************************************************
 * Local Macro definition
 ******************************************************************************/
#define XAF_64BYTE_ALIGN    64

/*******************************************************************************
 * Global data definition
 ******************************************************************************/

xf_dsp_t *xf_g_dsp;

/*******************************************************************************
 * Timer interrupt - tbd
 ******************************************************************************/

/*******************************************************************************
 * IPC layer initialization
 ******************************************************************************/

/* ...system-specific IPC layer deinitialization */
int xf_ipc_deinit(UWORD32 core)
{
    xf_core_data_t  *cd = XF_CORE_DATA(core);

    XF_CHK_API(xf_mm_deinit(&cd->shared_pool[XAF_MEM_ID_DEV]));

    return 0;
}

/* ...system-specific IPC layer initialization */
int xf_ipc_init(UWORD32 core)
{
    xf_core_data_t  *cd = XF_CORE_DATA(core);
    xf_core_ro_data_t  *ro = XF_CORE_RO_DATA(core);
    UWORD32 mem_pool_type = XAF_MEM_ID_DEV; //TODO, pass this from calling func as argument??

    xf_shmem_data_t *shmem = (xf_shmem_data_t *) xf_g_dsp->xf_ap_shmem_buffer[mem_pool_type];
    //xf_shmem_data_t *shmem = (xf_shmem_data_t *) malloc(XF_CFG_REMOTE_IPC_POOL_SIZE + 4095);
    //shmem = (xf_shmem_data_t *)((int) (shmem+4095) & 0xfffff000);

    /* ...initialize pointer to shared memory */
    cd->shmem = (xf_shmem_handle_t *)shmem;

    /* ...global memory pool initialization */

    XF_CHK_API(xf_mm_init(&cd->shared_pool[mem_pool_type], (shmem->buffer), (xf_g_dsp->xf_ap_shmem_buffer_size[mem_pool_type]-(sizeof(xf_shmem_data_t)-XF_CFG_REMOTE_IPC_POOL_SIZE))));

    /* ...open xos-msgq interface */
    XF_CHK_API(ipc_msgq_init(&ro->ipc.cmd_msgq, &ro->ipc.resp_msgq, &ro->ipc.msgq_event));

    TRACE(INFO, _b("DSP frmwk memory pool type:%d size:%d [%p] initialized [IPC]"), mem_pool_type, xf_g_dsp->xf_ap_shmem_buffer_size[mem_pool_type], xf_g_dsp->xf_ap_shmem_buffer[mem_pool_type]);

    return 0;
}

/*******************************************************************************
 * Core executive loop
 ******************************************************************************/
static void xf_core_loop(UWORD32 core)
{

    /* ...initialize internal core structures */
    xf_core_init(core);

#if (XF_CFG_CORES_NUM > 1)
    /* ...enable IPC interrupts from other cores */
	__xf_enable_interrupt(XF_PROC_INTERRUPT_NUMBER);
#endif

    if(core == XF_CORE_ID_MASTER)
    {
       /* ...wait in a low-power mode until event is triggered */
 	   while ( xf_ipi_wait(core)){

           /* ...service core event */
 	       xf_core_service(core);
 	   }
    }
#if (XF_CFG_CORES_NUM > 1)
    else
    {
       /* ...wait in a low-power mode until interrupt is triggered */
 	   while ( xf_ipc_wait2(core)){

            /* ...service core event */
 	       xf_core_service(core);
 	   }
    }
#endif

    xf_core_deinit(core);
}

/*******************************************************************************
 * DSP entry point
 ******************************************************************************/

static int _dsp_thread_entry(void *arg)
{

    int aligned_offset, i;
    void *(*dsp_args)[XF_DSP_THREAD_ARGS_IDX_MAX] = arg;
    UWORD32 core = *(UWORD32 *)(*dsp_args)[XF_DSP_THREAD_ARGS_IDX_CORE_ID];

    /* ...initialize per-core memory */
    for(i = XAF_MEM_ID_COMP; i <= XAF_MEM_ID_COMP_MAX; i++)
    {
        UWORD8 *pxf_dsp_local_buffer, *pbuf;
        UWORD32 xf_dsp_local_buffer_size;

        pbuf = (UWORD8*)(((void **) ((*dsp_args)[XF_DSP_THREAD_ARGS_IDX_DSP_LOCAL_BUF]))[i]);
        xf_dsp_local_buffer_size = ((UWORD32 *)(*dsp_args)[XF_DSP_THREAD_ARGS_IDX_DSP_LOCAL_BUF_SIZE])[i];
        pxf_dsp_local_buffer = (UWORD8 *) (((UWORD32)pbuf + (XAF_64BYTE_ALIGN-1)) & ~(XAF_64BYTE_ALIGN-1));
        xf_dsp_local_buffer_size -= ((UWORD32)pxf_dsp_local_buffer - (UWORD32)pbuf);
        xf_dsp_local_buffer_size = xf_dsp_local_buffer_size & ~(XAF_64BYTE_ALIGN-1);

        if(i == XAF_MEM_ID_COMP)
        {
            /* ...one time assignement */
            xf_g_dsp = (xf_dsp_t *)(((UWORD32)pxf_dsp_local_buffer + XF_PROXY_MAX_CACHE_ALIGNMENT-1) & ~(XF_PROXY_MAX_CACHE_ALIGNMENT-1)); /* ...TENA-3249 */
            aligned_offset = XF_MM(sizeof(xf_dsp_t));
            aligned_offset += ((UWORD32)xf_g_dsp - (UWORD32)pxf_dsp_local_buffer);
            pxf_dsp_local_buffer += aligned_offset;
            xf_dsp_local_buffer_size -= aligned_offset;

            /* ...assign COMPONENT memory stats pointers */
            xf_g_dsp->pdsp_comp_buf_size_peak  = ((WORD32 (*)[XAF_MEM_ID_MAX])((*dsp_args)[XF_DSP_THREAD_ARGS_IDX_STATS_COMP_BUF_PEAK]));
            xf_g_dsp->pdsp_comp_buf_size_curr  = ((WORD32 (*)[XAF_MEM_ID_MAX])((*dsp_args)[XF_DSP_THREAD_ARGS_IDX_STATS_COMP_BUF_CURR]));
        }

        /* ...initialize each memory pool type */
        XF_CHK_API(xf_mm_init(&(XF_CORE_DATA(core)->local_pool[i]), pxf_dsp_local_buffer, xf_dsp_local_buffer_size));
        TRACE(INFO, _b("DSP Component memory pool type:%d size:%d [%p] initialized"), i, xf_dsp_local_buffer_size, pxf_dsp_local_buffer);
        (*xf_g_dsp->pdsp_comp_buf_size_peak)[i] = (*xf_g_dsp->pdsp_comp_buf_size_curr)[i] = 0;
        xf_g_dsp->xf_dsp_local_buffer[i]       = pxf_dsp_local_buffer;
        xf_g_dsp->xf_dsp_local_buffer_size[i]  = xf_dsp_local_buffer_size;

    }//for(;i;)

    if(core == XF_CORE_ID_MASTER)
    {
        /* ...initialize per-core memory */
        for(i = XAF_MEM_ID_DEV; i <= XAF_MEM_ID_DEV_MAX; i++)
        {
            UWORD8 *pshmem_buffer, *pbuf;
            UWORD32 shmem_buffer_size;

            pbuf = (UWORD8*)(((void **) ((*dsp_args)[XF_DSP_THREAD_ARGS_IDX_AP_SHMEM_BUF]))[i]);
            shmem_buffer_size = ((UWORD32 *)(*dsp_args)[XF_DSP_THREAD_ARGS_IDX_AP_SHMEM_BUF_SIZE])[i];
            pshmem_buffer = (UWORD8 *) (((UWORD32)pbuf + (XF_SHMEM_DATA_ALIGNMENT-1)) & ~(XF_SHMEM_DATA_ALIGNMENT-1));
            shmem_buffer_size -= ((UWORD32)pshmem_buffer - (UWORD32)pbuf);
            shmem_buffer_size = shmem_buffer_size & ~(XF_SHMEM_DATA_ALIGNMENT-1);

            if(i == XAF_MEM_ID_DEV)
            {
                /* ...assign COMPONENT memory stats pointers, once */
                xf_g_dsp->pdsp_frmwk_buf_size_peak  = (WORD32 (*)[XAF_MEM_ID_MAX])((*dsp_args)[XF_DSP_THREAD_ARGS_IDX_STATS_FRMWK_BUF_PEAK]);
                xf_g_dsp->pdsp_frmwk_buf_size_curr  = (WORD32 (*)[XAF_MEM_ID_MAX])((*dsp_args)[XF_DSP_THREAD_ARGS_IDX_STATS_FRMWK_BUF_CURR]);
            }//switch(i)

            (*xf_g_dsp->pdsp_comp_buf_size_peak)[i] = (*xf_g_dsp->pdsp_comp_buf_size_curr)[i] = 0;
            xf_g_dsp->xf_ap_shmem_buffer[i] = pshmem_buffer;
            xf_g_dsp->xf_ap_shmem_buffer_size[i] = shmem_buffer_size;

        }//for(;i;)
    }

    //xf_g_dsp->pdsp_frmwk_buf_size_peak = (WORD32 (*)[XAF_MEM_ID_MAX])((*dsp_args)[XF_DSP_THREAD_ARGS_IDX_STATS_FRMWK_BUF_PEAK]);
    //xf_g_dsp->pdsp_frmwk_buf_size_curr = (WORD32 (*)[XAF_MEM_ID_MAX])((*dsp_args)[XF_DSP_THREAD_ARGS_IDX_STATS_FRMWK_BUF_CURR]);
    xf_g_dsp->cb_compute_cycles = ( int (*) (xaf_perf_stats_t *))(*dsp_args)[XF_DSP_THREAD_ARGS_IDX_STATS_CB_THREAD_STATS];
    xf_g_dsp->pdsp_cb_stats = (xaf_perf_stats_t *)(*dsp_args)[XF_DSP_THREAD_ARGS_IDX_STATS_WORKER_THREAD_STATS];

    memcpy(XF_CORE_DATA(core)->worker_thread_scratch_size, (UWORD32 (*)[XAF_MAX_WORKER_THREADS])(*dsp_args)[XF_DSP_THREAD_ARGS_IDX_WORKER_SCRATCH_SIZE], sizeof(XF_CORE_DATA(core)->worker_thread_scratch_size));
    memcpy(XF_CORE_DATA(core)->worker_thread_stack_size, (UWORD32 (*)[XAF_MAX_WORKER_THREADS])(*dsp_args)[XF_DSP_THREAD_ARGS_IDX_WORKER_STACK_SIZE], sizeof(XF_CORE_DATA(core)->worker_thread_stack_size));

    /* ...reset ro/rw core data - tbd */
    memset(xf_g_dsp->xf_core_rw_data, 0, sizeof(xf_g_dsp->xf_core_rw_data));
    memset(xf_g_dsp->xf_core_ro_data, 0, sizeof(xf_g_dsp->xf_core_ro_data));

    TRACE(INFO, _b("Xtensa Audio DSP Codec Server"));

    /* ...initialize board */
    xf_board_init();

    /* ...global framework data initialization */
    xf_global_init();

#if (XF_CFG_CORES_NUM > 1)
    xf_ipc_config_t ipc_cfg;

    {
        /* ...initialize per-core shared memory data structures for IPC */
        UWORD8 *pxf_dsp_shmem_buffer = (UWORD8 *)(*dsp_args)[XF_DSP_THREAD_ARGS_IDX_DSP_SHMEM_BUF];
        UWORD32 xf_dsp_shmem_buffer_size = *(UWORD32 *)(*dsp_args)[XF_DSP_THREAD_ARGS_IDX_DSP_SHMEM_BUF_SIZE];

        /* ...initialize IPC */
        memset(&ipc_cfg, 0, sizeof(ipc_cfg));

        ipc_cfg = (xf_ipc_config_t){.handle_size = xf_dsp_shmem_buffer_size,
        .phandle = (void *)pxf_dsp_shmem_buffer,
        .msg_next_offset = (UWORD32)offset_of(xf_message_t, next),
        .cd = XF_CORE_DATA(core),
        };

        xf_ipc_open2(core, &ipc_cfg);

        /* ...setup interrupt handler for IPC */
#if defined(HAVE_XOS)
        xos_register_interrupt_handler(XF_PROC_INTERRUPT_NUMBER, xf_ipc_interrupt_handler, &core);
#else
        xt_set_interrupt_handler(XF_PROC_INTERRUPT_NUMBER, xf_ipc_interrupt_handler, &core);
#endif

        /* ...core synchronization at reset */
        XF_CHK_ERR((0 == __xf_ipc_reset_sync()), XAF_TIMEOUT_ERR);
    }
#endif //XF_CFG_CORES_NUM

#if  0 //(XF_CFG_CORES_NUM > 1)
    UWORD32     i;
    /* ...bring up all cores */
    for (i = 1; i < XF_CFG_CORES_NUM; i++)
    {
        /* ...wake-up secondary core somehow and make it execute xf_core_loop */
        xf_core_secondary_startup(i, xf_core_loop, i);

    }
#endif

    /* ...enter execution loop on master core #0 */
    xf_core_loop(core);

    /* ...deinitialize per-core memory loop */
    for(i = XAF_MEM_ID_COMP; i <= XAF_MEM_ID_COMP_MAX; i++)
    {
        XF_CHK_API(xf_mm_deinit(&(XF_CORE_DATA(core)->local_pool[i])));
    }//for(;i;)

#if (XF_CFG_CORES_NUM > 1)
    xf_ipc_close2(core);

    /* ... copy the global shared memory usage info to DSP's local memory pointers */
    *(WORD32 *)(*dsp_args)[XF_DSP_THREAD_ARGS_IDX_STATS_SHMEM_BUF_PEAK] = XF_SHMEM_IPC_HANDLE(core)->dsp_shmem_buf_size_peak;
    *(WORD32 *)(*dsp_args)[XF_DSP_THREAD_ARGS_IDX_STATS_SHMEM_BUF_CURR] = XF_SHMEM_IPC_HANDLE(core)->dsp_shmem_buf_size_curr;

    if(core == XF_CORE_ID_MASTER)
    {
        xf_msg_pool_destroy(&XF_CORE_DATA(core)->dsp_dsp_shmem_pool, core, 1, XAF_MEM_ID_COMP);

        /* ...deinit shared memory heap */
        XF_CHK_API(xf_ipc_mm_deinit(&(XF_SHMEM_IPC_HANDLE(core)->xf_dsp_shmem_pool)));
    }
#endif

    return 0;
}

void *dsp_thread_entry(void *arg)
{
    _dsp_thread_entry(arg);
    return NULL;
}
