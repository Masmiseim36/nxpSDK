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
#include <xtensa/simcall-errno.h>
#include "xaf-api.h"

/* ...size of auxiliary pool for communication with HiFi */
#define XAF_AUX_POOL_SIZE                   32

/* ...length of auxiliary pool messages */
#define XAF_AUX_POOL_MSG_LENGTH             256

/* ...size of config pool for communication with HiFi */
#define XAF_EXT_CFG_POOL_SIZE               1

/* ... max allowed length of config pool messages */
#define XAF_MAX_EXT_CFG_BUF_LEN             (1024 * 8)

/* ...Overhead for ext config per param (worst case, non-zero copy option)
 * sizeof(xf_ext_param_msg_t) + sizeof(xaf_ext_buffer_t) +
 * sizeof(UWORD32) for holding the buffer + 4 bytes for size alignment */
#define XAF_EXT_CFG_OVERHEAD                (32)

#define XAF_MAX_EXT_CONFIG_PARAMS           (XAF_AUX_POOL_MSG_LENGTH / XAF_EXT_CFG_OVERHEAD)

#define XAF_MAX_CONFIG_PARAMS               (XAF_AUX_POOL_MSG_LENGTH >> 3)

#define MAX_IO_PORTS                        (XF_CFG_MAX_IN_PORTS + XF_CFG_MAX_OUT_PORTS)
#define PORT_NOT_CONNECTED                  (0xFFFFFFFF)

#ifndef EBADFD
#define EBADFD _SIMC_EBADFD
#endif

typedef enum {
    XAF_ADEV_RESET = 0,
    XAF_ADEV_INIT  = 1
} xaf_adev_state;

typedef enum {
    XAF_COMP_RESET          = 0,
    XAF_COMP_CREATE         = 1,
    XAF_COMP_NSTATES
} xaf_comp_state;

typedef struct xaf_comp_format_s {
    UWORD32             sample_rate;
    UWORD32             channels;
    UWORD32             pcm_width;
    UWORD32             input_length[XF_CFG_MAX_IN_PORTS];
    UWORD32             output_length[XF_CFG_MAX_OUT_PORTS];
} xaf_comp_format_t;

typedef struct xaf_connect_map_s {
    void               *ptr;
    UWORD32             port;
} xaf_connect_map_t;

#ifndef XA_DISABLE_EVENT
typedef struct xf_app_event_channel xf_app_event_channel_t;
#endif

typedef struct xaf_node_chain_s  xaf_node_chain_t;
struct xaf_node_chain_s {
    xaf_node_chain_t *head;
    UWORD32           next_offset;
    xf_lock_t         lock;
};

typedef struct xaf_comp xaf_comp_t;

struct xaf_comp {
    UWORD32             inp_ports;
    UWORD32             out_ports;
    UWORD32             init_done;
    UWORD32             pending_resp;
    UWORD32             expect_out_cmd;
    UWORD32             input_over;

    xaf_comp_type   comp_type;
    xaf_comp_status comp_status;
    UWORD32             start_cmd_issued;
    UWORD32             exec_cmd_issued;
    void            *start_buf;

    UWORD32             probe_enabled;
    UWORD32             probe_started;
    UWORD32             probe_length;
    xf_pool_t           *probepool;
    void                *probe_buf;

    xaf_connect_map_t   cmap[MAX_IO_PORTS];

    xaf_comp_format_t    out_format;

    xf_pool_t       *inpool;
    xf_pool_t       *outpool;
    xf_pool_t       *ext_cfg_pool;
    xf_buffer_t     *p_config_buf;
    UWORD32          cfg_param_ext_buf_size_max;
    void                *pout_buf[1];
    void                *p_input[XAF_MAX_INBUFS];   //TENA-2196
    UWORD32                ninbuf;
    UWORD32             noutbuf;

    xaf_comp_t      *next;
    void            *p_adev;

    xaf_comp_state  comp_state;

    void           *comp_ptr;

    xf_handle_t     handle;

#ifndef XA_DISABLE_EVENT
    UWORD32         error_channel_ctl;
#endif
};

typedef struct xaf_adev_s {

    xaf_node_chain_t comp_chain;

#ifndef XA_DISABLE_EVENT
    xaf_node_chain_t event_chain;
#endif

    UWORD32 n_comp;

    void *adev_ptr;
    void *p_apMem;

    xaf_adev_state  adev_state;

    xf_proxy_t   proxy;

#ifndef XA_DISABLE_EVENT
    xa_app_submit_event_cb_t cdata;
#endif

    UWORD32 dsp_thread_priority;
} xaf_adev_t;

typedef struct xaf_adsp_s {

    void *adev_ptr;
    void *p_dspMem;
    void *xf_g_dsp;

    xaf_adev_state  adev_state;

    UWORD32 dsp_thread_priority;
} xaf_adsp_t;
