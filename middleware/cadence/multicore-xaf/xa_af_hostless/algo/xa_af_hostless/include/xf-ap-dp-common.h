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
/* File contains constants shared between AP and DP sides */

/*******************************************************************************
* DSP Master Processor ID (software ID)
*******************************************************************************/

//#define XF_CORE_ID_MASTER               0

#if (XF_CFG_CORES_NUM < 2) && (XF_CORE_ID_MASTER != 0)
#undef XF_CORE_ID_MASTER
#define XF_CORE_ID_MASTER               0
#endif

/* DSP object sizes */

#if 0
#if defined(HAVE_FREERTOS)
#define XF_DSP_OBJ_SIZE_CORE_DATA           624
#define XF_DSP_OBJ_SIZE_DSP_LOCAL_POOL      144
#define XF_DSP_OBJ_SIZE_CORE_RO_DATA        256
#define XF_DSP_OBJ_SIZE_CORE_RW_DATA        256
#elif defined(HAVE_XOS)
#define XF_DSP_OBJ_SIZE_CORE_DATA           696
#define XF_DSP_OBJ_SIZE_DSP_LOCAL_POOL      328
#define XF_DSP_OBJ_SIZE_CORE_RO_DATA        256
#define XF_DSP_OBJ_SIZE_CORE_RW_DATA        256
#else
#error Unrecognized OS
#endif
#endif

/*******************************************************************************
 * Global configuration parameters (changing is to be done carefully)
 ******************************************************************************/

/* ...maximum in ports for mimo class */
#define XF_CFG_MAX_IN_PORTS             4

/* ...maximum out ports for mimo class */
#define XF_CFG_MAX_OUT_PORTS            4

/* ...maximal size of scratch memory is 56 KB */
#define XF_CFG_CODEC_SCRATCHMEM_SIZE    (56 << 10)

#ifndef XA_DISABLE_EVENT
/*******************************************************************************
* Global event configuration parameters *
*******************************************************************************/

/* ...event to application identifier */
#define XF_CFG_ID_EVENT_TO_APP      (0x11000 + 0x0)

/* ...component execution fatal error */
#define XF_CFG_COMP_ERR_FATAL       (0x11000 + 0x1)

/* ...component execution fatal and non-fatal error */
#define XF_CFG_COMP_ERR_ALL         (0x11000 + 0x2)
#endif

/*******************************************************************************
* Global Macros to manage message-ID and bit-masks for Core, Client, Port, Proxy *
*******************************************************************************/
/* ...message-ID bits for source or dest, together form 2*XF_MSG_ID_BITS header */
#define XF_MSG_ID_BITS              32

/* ...DSP-core bits. increase this along with XF_MSG_ID_BITS for the subsystem
 * to support larger #of cores */
#define XF_DSP_CORE_BITS            8

#define XF_DSP_PORT_BITS            4
#define XF_DSP_CLIENT_BITS          6
#define XF_DSP_CLIENT_SHIFT_BITS    (XF_DSP_CORE_BITS)
#define XF_DSP_PORT_SHIFT_BITS      (XF_DSP_CLIENT_BITS + XF_DSP_CORE_BITS)

#define XF_AP_IPC_CLIENT_BITS       4
#define XF_AP_CLIENT_BITS           9

#if (XF_MSG_ID_BITS > 16)
typedef UWORD64 xf_msg_id_dtype __attribute__((aligned(sizeof(UWORD64))));
#else
typedef UWORD32 xf_msg_id_dtype;
#endif

/* ...enable a parallel response queue mechanism for synchronous commands which can be delayed (eg:unroute) */
#define DELAYED_SYNC_RESPONSE       1

/*******************************************************************************
* DSP-thread argument index enums *
*******************************************************************************/

typedef enum dsp_arg_idx_s{
    XF_DSP_THREAD_ARGS_IDX_MEM_POOL                     = 0,
    XF_DSP_THREAD_ARGS_IDX_WORKER_SCRATCH_SIZE          = 1,
    XF_DSP_THREAD_ARGS_IDX_CORE_ID                      = 2,
    XF_DSP_THREAD_ARGS_IDX_STATS_COMP_BUF_PEAK          = 3,
    XF_DSP_THREAD_ARGS_IDX_STATS_COMP_BUF_CURR          = 4,
    XF_DSP_THREAD_ARGS_IDX_STATS_FRMWK_BUF_PEAK         = 5,
    XF_DSP_THREAD_ARGS_IDX_STATS_FRMWK_BUF_CURR         = 6,
    XF_DSP_THREAD_ARGS_IDX_STATS_SHMEM_BUF_PEAK         = 7,
    XF_DSP_THREAD_ARGS_IDX_STATS_SHMEM_BUF_CURR         = 8,
    XF_DSP_THREAD_ARGS_IDX_STATS_WORKER_THREAD_STATS    = 9,
    XF_DSP_THREAD_ARGS_IDX_STATS_CB_THREAD_STATS        = 10,
    XF_DSP_THREAD_ARGS_IDX_WORKER_STACK_SIZE            = 11,
    XF_DSP_THREAD_ARGS_IDX_MAX,
}dsp_arg_idx_t;

/* sizeof(xf_dsp_t) to be allocated from dspLocalBuffer in xf-main.c */
//#define XF_DSP_STRUCT_SIZE              (1536 * XF_CFG_CORES_NUM)
#define XF_DSP_STRUCT_SIZE              (512 + 1536 * XF_CFG_CORES_NUM)
#define XF_DSP_SHMEM_STRUCT_SIZE        (256 * XF_CFG_CORES_NUM)
