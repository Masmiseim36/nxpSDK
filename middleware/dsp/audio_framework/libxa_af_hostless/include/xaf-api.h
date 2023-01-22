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
#ifndef __XA_API_H__
#define __XA_API_H__

#include "xa_type_def.h"

/* Constants */
#define XAF_MAX_INBUFS                      2
#define XAF_INBUF_SIZE                      4096
#define XAF_SHMEM_STRUCT_SIZE               12288

/* Port BITMASK creation macro */
#define XAF_PORT_MASK(idx)                  (1 << (idx))

/* Ext param set flag */
#define XAF_EXT_PARAM_SET_FLAG(idx)         (1 << (idx))

/* Ext param clear flag */
#define XAF_EXT_PARAM_CLEAR_FLAG(idx)       (~(1 << (idx)))

/* Check if the ext param flag is set */
#define XAF_CHK_EXT_PARAM_FLAG(flag, idx)   ((flag) & (1 << (idx)))

#define XAF_MAX_WORKER_THREADS              16

#ifndef XF_CFG_CORES_NUM
#define XF_CFG_CORES_NUM                    2
#endif //XF_CFG_CORES_NUM

#define XF_CORE_ID_MASTER                   0

#if (XF_CFG_CORES_NUM > 1)
#define XF_EXTERNAL_INTERRUPT_NUMBER	    7
#endif //XF_CFG_CORES_NUM

enum {
    XAF_EXT_PARAM_FLAG_OFFSET_ZERO_COPY = 0,
};

typedef enum {
    XAF_DECODER         = 0,
    XAF_ENCODER         = 1,
    XAF_MIXER           = 2,
    XAF_PRE_PROC        = 3,
    XAF_POST_PROC       = 4,
    XAF_RENDERER        = 5,
    XAF_CAPTURER        = 6,
    XAF_MIMO_PROC_12    = 7,
    XAF_MIMO_PROC_21    = 8,
    XAF_MIMO_PROC_22    = 9,
    XAF_MIMO_PROC_23    = 10,
    XAF_MIMO_PROC_10    = 11,
    XAF_MIMO_PROC_11    = 12,
    XAF_MAX_COMPTYPE
} xaf_comp_type;

typedef enum {
    XAF_STARTING        = 0,
    XAF_INIT_DONE       = 1,
    XAF_NEED_INPUT      = 2,
    XAF_OUTPUT_READY    = 3,
    XAF_PROBE_READY     = 4,
    XAF_PROBE_DONE      = 5,
    XAF_EXEC_DONE       = 6,
    XAF_INIT_NEED_INPUT = 7,
} xaf_comp_status;

typedef enum {
    XAF_START_FLAG          = 1,
    XAF_EXEC_FLAG           = 2,
    XAF_INPUT_OVER_FLAG     = 3,
    XAF_INPUT_READY_FLAG    = 4,
    XAF_NEED_OUTPUT_FLAG    = 5,
    XAF_NEED_PROBE_FLAG     = 6,
} xaf_comp_flag;

typedef enum {
    XAF_NO_ERR          =  0,
    XAF_RTOS_ERR        = -1,
    XAF_INVALIDVAL_ERR  = -2,
    XAF_ROUTING_ERR     = -3,
    XAF_INVALIDPTR_ERR  = -4, 
    XAF_API_ERR         = -5,
    XAF_TIMEOUT_ERR     = -6,   // Get status timeout
    XAF_MEMORY_ERR      = -7,   // Memory allocation or availability error
    XAF_NUM_ERRS        = -8,   // Make sure this is always the last one
} XAF_ERR_CODE;

typedef enum {
    XAF_MEM_ID_DEV  = 0,
    XAF_MEM_ID_COMP = 1,
} XAF_MEM_ID;

typedef enum {
    XAF_ADEV_NORMAL_CLOSE = 0, 
    XAF_ADEV_FORCE_CLOSE = 1
} xaf_adev_close_flag;

#ifndef XA_DISABLE_EVENT
typedef enum {               
    XAF_ERR_CHANNEL_DISABLE  =0,     //Error channel disabled
    XAF_ERR_CHANNEL_FATAL    =1,     //Error channel for only Fatal error reporting
    XAF_ERR_CHANNEL_ALL      =2,     //Error channel for Fatal and Non-Fatal error reporting
} xaf_error_channel_ctl;
#endif

enum xaf_comp_config_param {
    XAF_COMP_CONFIG_PARAM_PROBE_ENABLE = 0x20000 + 0x0,
    XAF_COMP_CONFIG_PARAM_RELAX_SCHED  = 0x20000 + 0x1,
    XAF_COMP_CONFIG_PARAM_PRIORITY     = 0x20000 + 0x2,
    XAF_COMP_CONFIG_PARAM_SELF_SCHED   = 0x20000 + 0x3, 
    XAF_COMP_CONFIG_PARAM_DEC_INIT_WO_INP   = 0x20000 + 0x4, 
    XAF_COMP_CONFIG_PARAM_EVENT_CB     = 0x20000 + 0xE, 
};

/* Component string identifier */
typedef const char *xf_id_t; 

/* Types */
typedef pVOID xaf_mem_malloc_fxn_t(WORD32 size, WORD32 id);
typedef VOID  xaf_mem_free_fxn_t(pVOID ptr, WORD32 id);
typedef WORD32 (*xaf_app_event_handler_fxn_t)(pVOID comp_ptr, UWORD32 config_param_id, pVOID config_buf_ptr, UWORD32 buf_size, UWORD32 comp_error_flag);

#ifndef XA_DISABLE_EVENT
/* ...event callback structure */
typedef struct xa_raise_event_cb_s
{
    WORD32 (*cb)(struct xa_raise_event_cb_s *, UWORD32 id);
}xa_raise_event_cb_t;
#endif

/*... performance stats structure for MCPS.
 * For a cached subsystem, the size is cache aligned */

#if (XF_CFG_CORES_NUM > 1)
#include <xtensa/config/core.h>
struct xaf_perf_stats_s{
    long long tot_cycles;
    long long frmwk_cycles;
    long long dsp_comps_cycles;
    int dsp_frmwk_buf_size_peak;
    int dsp_comp_buf_size_peak;
    int dsp_shmem_buf_size_peak;
    int dsp_xaf_buf_size_peak;
} __attribute__ ((aligned(XCHAL_DCACHE_LINESIZE)));
#else //if(XF_CFG_CORES_NUM > 1)
struct xaf_perf_stats_s{
    long long tot_cycles;
    long long frmwk_cycles;
    long long dsp_comps_cycles;
};
#endif //if(XF_CFG_CORES_NUM > 1)
typedef struct xaf_perf_stats_s xaf_perf_stats_t;
#define XF_SHMEM_SIZE_PERF_STATS    (sizeof(xaf_perf_stats_t) * XF_CFG_CORES_NUM)

/* ...api config structs */
typedef struct xaf_adev_config_s{
	xaf_mem_malloc_fxn_t *pmem_malloc;
	xaf_mem_free_fxn_t *pmem_free;
#ifndef XA_DISABLE_EVENT
	xaf_app_event_handler_fxn_t app_event_handler_cb;
#endif
	UWORD32 audio_component_buffer_size;
	UWORD32 audio_framework_buffer_size;
	UWORD32 proxy_thread_priority;
	UWORD32 dsp_thread_priority;
	UWORD32	worker_thread_scratch_size[XAF_MAX_WORKER_THREADS];
    UWORD32 core;
    void * pshmem_frmwk;
    void * pshmem_dsp;
    UWORD32 audio_shmem_buffer_size;

    int (*cb_compute_cycles)(xaf_perf_stats_t*); /* ... callback function pointer to update the thread-wise cycles into stats structure */

    xaf_perf_stats_t *cb_stats; /* ... pointer to worker stats structure */

}xaf_adev_config_t;

typedef struct xaf_comp_config_s{
	xf_id_t comp_id;
	xaf_comp_type comp_type;
	UWORD32 num_input_buffers;
	UWORD32 num_output_buffers;
	UWORD32 cfg_param_ext_buf_size_max;
	pVOID (*pp_inbuf)[XAF_MAX_INBUFS];
    UWORD32 core;
#ifndef XA_DISABLE_EVENT
	UWORD32 error_channel_ctl;
    UWORD32 num_err_msg_buf;
#endif
}xaf_comp_config_t;

typedef struct xaf_ext_buffer
{
    /* ...max data size */
    UWORD32 max_data_size;

    /* ...valid data size */
    UWORD32 valid_data_size;

    /* ...indicates whether in-place-buffer used */
    UWORD32 ext_config_flags;

    /* ...parameter data (4 bytes aligned) */
    UWORD8 *data;

} __attribute__ ((__packed__, __aligned__(4))) xaf_ext_buffer_t;

/* Function prototypes */
XAF_ERR_CODE xaf_adev_config_default_init(xaf_adev_config_t *pconfig);
XAF_ERR_CODE xaf_adev_open(pVOID *pp_adev, xaf_adev_config_t *pconfig);
XAF_ERR_CODE xaf_adev_close(pVOID p_adev, xaf_adev_close_flag flag);
XAF_ERR_CODE xaf_adev_set_priorities(pVOID p_adev, WORD32 n_rt_priorities, WORD32 rt_priority_base, WORD32 bg_priority, UWORD32 core);

XAF_ERR_CODE xaf_comp_config_default_init(xaf_comp_config_t *pconfig);
XAF_ERR_CODE xaf_comp_create(pVOID p_adev, pVOID *pp_comp, xaf_comp_config_t *pconfig);
XAF_ERR_CODE xaf_comp_delete(pVOID p_comp);
XAF_ERR_CODE xaf_comp_set_config(pVOID p_comp, WORD32 num_param, pWORD32 p_param);
XAF_ERR_CODE xaf_comp_set_config_ext(pVOID comp_ptr, WORD32 num_param, WORD32 *p_param);
XAF_ERR_CODE xaf_comp_get_config(pVOID p_comp, WORD32 num_param, pWORD32 p_param);
XAF_ERR_CODE xaf_comp_get_config_ext(pVOID comp_ptr, WORD32 num_param, WORD32 *p_param);
XAF_ERR_CODE xaf_comp_process(pVOID p_adev, pVOID p_comp, pVOID p_buf, UWORD32 length, xaf_comp_flag flag);
XAF_ERR_CODE xaf_connect(pVOID p_src, WORD32 src_out_port, pVOID p_dest, WORD32 dest_in_port, WORD32 num_buf);
XAF_ERR_CODE xaf_disconnect(pVOID p_src, WORD32 src_out_port, pVOID p_dest, WORD32 dest_in_port);
XAF_ERR_CODE xaf_get_mem_stats(pVOID p_dev, UWORD32 core, WORD32 *pmem_info);

XAF_ERR_CODE xaf_comp_get_status(pVOID p_adev, pVOID p_comp, xaf_comp_status *p_status, pVOID p_info);
XAF_ERR_CODE xaf_get_verinfo(pUWORD8 ver_info[3]);

XAF_ERR_CODE xaf_pause(pVOID p_comp, WORD32 port);
XAF_ERR_CODE xaf_resume(pVOID p_comp, WORD32 port);

XAF_ERR_CODE xaf_probe_start(pVOID p_comp);
XAF_ERR_CODE xaf_probe_stop(pVOID p_comp);

#ifndef XA_DISABLE_EVENT
XAF_ERR_CODE xaf_create_event_channel(pVOID p_src, UWORD32 src_config_param, pVOID p_dest, UWORD32 dst_config_param, UWORD32 nbuf, UWORD32 buf_size);
XAF_ERR_CODE xaf_delete_event_channel(pVOID p_src, UWORD32 src_config_param, pVOID p_dest, UWORD32 dst_config_param);
#endif

#if (XF_CFG_CORES_NUM > 1)
XAF_ERR_CODE xaf_dsp_open(pVOID *pp_adev, xaf_adev_config_t *pconfig);
XAF_ERR_CODE xaf_dsp_close(pVOID p_adev);
#endif //XF_CFG_CORES_NUM
#endif /* __XA_API_H__ */
