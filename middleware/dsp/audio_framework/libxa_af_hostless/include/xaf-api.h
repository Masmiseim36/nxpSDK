/*******************************************************************************
* Copyright (c) 2015-2020 Cadence Design Systems, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to use this Software with Cadence processor cores only and 
* not with any other processors and platforms, subject to
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

******************************************************************************/
#ifndef __XA_API_H__
#define __XA_API_H__

#include "xa_type_def.h"

/* Constants */
#define XAF_MAX_INBUFS                      2
#define XAF_INBUF_SIZE                      4096
#define XAF_SHMEM_STRUCT_SIZE               12288

/* Port BITMASK creation macro */
#define XAF_PORT_MASK(idx)                  (1 << (idx))

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
    XAF_EXEC_DONE       = 5,
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
    XAF_NO_ERROR        =  0,
    XAF_RTOS_ERROR      = -1,
    XAF_INVALID_VALUE   = -2,
    XAF_ROUTING_ERROR   = -3,
    XAF_PTR_ERROR       = -4,
    XAF_API_ERR         = -5,
    XAF_STATUS_TIMEOUT  = -6,
} XAF_ERR_CODE;

typedef enum {
    XAF_MEM_ID_DEV  = 0,
    XAF_MEM_ID_COMP = 1,
} XAF_MEM_ID;

typedef enum {
    XAF_ADEV_NORMAL_CLOSE = 0, 
    XAF_ADEV_FORCE_CLOSE = 1
} xaf_adev_close_flag;

typedef struct xaf_format_s {
    UWORD32             sample_rate;
    UWORD32             channels;
    UWORD32             pcm_width;
    UWORD32             input_length;
    UWORD32             output_length;
    UWORD64             output_produced;
} xaf_format_t;

enum xaf_comp_config_param {
    XAF_COMP_CONFIG_PARAM_PROBE_ENABLE = 0x20000 + 0,
    XAF_COMP_CONFIG_PARAM_RELAX_SCHED  = 0x20000 + 1,
    XAF_COMP_CONFIG_PARAM_PRIORITY     = 0x20000 + 2,
};

/* Component string identifier */
typedef const char *xf_id_t; 

/* Types */
typedef pVOID xaf_mem_malloc_fxn_t(WORD32 size, WORD32 id);
typedef VOID  xaf_mem_free_fxn_t(pVOID ptr, WORD32 id);

/* Function prototypes */
XAF_ERR_CODE xaf_adev_open(pVOID *pp_adev, WORD32 audio_frmwk_buf_size, WORD32 audio_comp_buf_size, xaf_mem_malloc_fxn_t mm_malloc, xaf_mem_free_fxn_t mm_free);
XAF_ERR_CODE xaf_adev_close(pVOID p_adev, xaf_adev_close_flag flag);
XAF_ERR_CODE xaf_adev_set_priorities(pVOID p_adev, WORD32 n_rt_priorities, WORD32 rt_priority_base, WORD32 bg_priority);

XAF_ERR_CODE xaf_comp_create(pVOID p_adev, pVOID *pp_comp, xf_id_t comp_id, UWORD32 ninbuf, UWORD32 noutbuf, pVOID pp_inbuf[], xaf_comp_type comp_type);
XAF_ERR_CODE xaf_comp_delete(pVOID p_comp);
XAF_ERR_CODE xaf_comp_set_config(pVOID p_comp, WORD32 num_param, pWORD32 p_param);
XAF_ERR_CODE xaf_comp_get_config(pVOID p_comp, WORD32 num_param, pWORD32 p_param);
XAF_ERR_CODE xaf_comp_process(pVOID p_adev, pVOID p_comp, pVOID p_buf, UWORD32 length, xaf_comp_flag flag);
XAF_ERR_CODE xaf_connect(pVOID p_src, WORD32 src_out_port, pVOID p_dest, WORD32 dest_in_port, WORD32 num_buf);
XAF_ERR_CODE xaf_disconnect(pVOID p_src, WORD32 src_out_port, pVOID p_dest, WORD32 dest_in_port);
XAF_ERR_CODE xaf_get_mem_stats(pVOID p_dev, WORD32 *pmem_info);

XAF_ERR_CODE xaf_comp_get_status(pVOID p_adev, pVOID p_comp, xaf_comp_status *p_status, pVOID p_info);
XAF_ERR_CODE xaf_get_verinfo(pUWORD8 ver_info[3]);

XAF_ERR_CODE xaf_pause(pVOID p_comp, WORD32 port);
XAF_ERR_CODE xaf_resume(pVOID p_comp, WORD32 port);

XAF_ERR_CODE xaf_probe_start(pVOID p_comp);
XAF_ERR_CODE xaf_probe_stop(pVOID p_comp);
#endif /* __XA_API_H__ */
