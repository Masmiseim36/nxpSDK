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
 * xf-opcode.h
 *
 * Xtensa audio processing framework. Message API
 ******************************************************************************/

#ifndef __XF_H
#error "xf-opcode.h mustn't be included directly"
#endif

/*******************************************************************************
 * Message routing composition - move somewhere else - tbd
 ******************************************************************************/
#if (XF_MSG_ID_BITS > 16)
#define UNIT    (1ULL)
#else
#define UNIT    (1UL)
#endif

/* ...adjust IPC client of message going from user-space */
#define XF_MSG_AP_FROM_USER(id, client) \
    (((id) & ~(((UNIT<<XF_AP_IPC_CLIENT_BITS)-1) << XF_DSP_CORE_BITS)) | (client))

/* ...wipe out IPC client from message going to user-space */
#define XF_MSG_AP_TO_USER(id)           \
    ((id) & ~(((UNIT<<XF_AP_IPC_CLIENT_BITS)-1) << (XF_DSP_CORE_BITS + XF_MSG_ID_BITS)))

/* ...port specification (12 bits) */
#define __XF_PORT_SPEC(core, id, port)  ((xf_msg_id_dtype)(core) | ((id) << XF_DSP_CORE_BITS) | ((xf_msg_id_dtype)(port) << (XF_DSP_PORT_SHIFT_BITS)))
#define __XF_PORT_SPEC2(id, port)       ((xf_msg_id_dtype)(id) | ((xf_msg_id_dtype)(port) << (XF_DSP_PORT_SHIFT_BITS)))
#define XF_PORT_CORE(spec)              (UWORD32)((spec) & ((UNIT<<XF_DSP_CORE_BITS)-1))
#define XF_PORT_CLIENT(spec)            (UWORD32)(((spec) >> XF_DSP_CLIENT_SHIFT_BITS) & ((UNIT<<XF_DSP_CLIENT_BITS)-1))
#define XF_PORT_ID(spec)                (UWORD32)(((spec) >> XF_DSP_PORT_SHIFT_BITS) & ((UNIT<<XF_DSP_PORT_BITS)-1))

/* ...message id contains source and destination ports specification */
#define __XF_MSG_ID(src, dst)           (((xf_msg_id_dtype)(src) & ((UNIT<<XF_MSG_ID_BITS)-1)) | (((xf_msg_id_dtype)(dst) & ((UNIT<<XF_MSG_ID_BITS)-1)) << XF_MSG_ID_BITS))

#define XF_MSG_SRC(id)                  (UWORD32)(((id) >> 0) & ((UNIT<<XF_MSG_ID_BITS)-1))
#define XF_MSG_SRC_CORE(id)             (UWORD32)(((id) >> 0) & ((UNIT<<XF_DSP_CORE_BITS)-1))
#define XF_MSG_SRC_CLIENT(id)           (UWORD32)(((id) >> XF_DSP_CLIENT_SHIFT_BITS) & ((UNIT<<XF_DSP_CLIENT_BITS)-1))
#define XF_MSG_SRC_ID(id)               (UWORD32)(((id) >> 0) & ((UNIT<<(XF_DSP_CLIENT_BITS+XF_DSP_CORE_BITS))-1))
#define XF_MSG_SRC_PORT(id)             (UWORD32)(((id) >> XF_DSP_PORT_SHIFT_BITS) & ((UNIT<<XF_DSP_PORT_BITS)-1))
#define XF_MSG_SRC_PROXY(id)            (UWORD32)(((id) >> (XF_MSG_ID_BITS-1)) & 0x1)

#define XF_MSG_DST(id)                  (UWORD32)(((id) >> XF_MSG_ID_BITS) & ((UNIT<<XF_MSG_ID_BITS)-1))
#define XF_MSG_DST_CORE(id)             (UWORD32)(((id) >> XF_MSG_ID_BITS) & ((UNIT<<XF_DSP_CORE_BITS)-1))
#define XF_MSG_DST_CLIENT(id)           (UWORD32)(((id) >> (XF_MSG_ID_BITS + XF_DSP_CLIENT_SHIFT_BITS)) & ((UNIT<<XF_DSP_CLIENT_BITS)-1))
#define XF_MSG_DST_ID(id)               (UWORD32)(((id) >> XF_MSG_ID_BITS) & ((UNIT<<(XF_DSP_CLIENT_BITS+XF_DSP_CORE_BITS))-1))
#define XF_MSG_DST_PORT(id)             (UWORD32)(((id) >> (XF_MSG_ID_BITS + XF_DSP_PORT_SHIFT_BITS)) & ((UNIT<<XF_DSP_PORT_BITS)-1))
#define XF_MSG_DST_PROXY(id)            (UWORD32)(((id) >> (2*XF_MSG_ID_BITS-1)) & 0x1)

/* ...special treatment of AP-proxy destination field */
#define XF_AP_IPC_CLIENT(id)            (UWORD32)(((id) >> (XF_MSG_ID_BITS + XF_DSP_CORE_BITS)) & ((UNIT<<XF_AP_IPC_CLIENT_BITS)-1))
#define XF_AP_CLIENT(id)                (UWORD32)(((id) >> (XF_MSG_ID_BITS + XF_DSP_CORE_BITS + XF_AP_IPC_CLIENT_BITS)) & ((UNIT << (XF_AP_CLIENT_BITS))-1))
#define __XF_AP_PROXY(core)             ((xf_msg_id_dtype)(core) | (UNIT << (XF_MSG_ID_BITS-1)))
#define __XF_DSP_PROXY(core)            ((xf_msg_id_dtype)(core) | (UNIT << (XF_MSG_ID_BITS-1)))
#define __XF_AP_CLIENT(core, client)    ((xf_msg_id_dtype)(core) | ((client) << (XF_AP_IPC_CLIENT_BITS + XF_DSP_CORE_BITS)) | (UNIT << (XF_MSG_ID_BITS-1)))

/* ...check if DSP message is shared between cores */
#define XF_MSG_SHARED(id)               \
    ({ xf_msg_id_dtype __id = (id); (XF_CFG_CORES_NUM > 1 ? (__id ^ (__id >> XF_MSG_ID_BITS)) & ((UNIT<<XF_DSP_CORE_BITS)-1) : 0); })

/*******************************************************************************
 * Opcode composition
 ******************************************************************************/

/* ...opcode composition with command/response data tags */
#define __XF_OPCODE(c, r, op)           ((UWORD32)((((UWORD32)c) << 31) | ((r) << 30) | ((op) & 0x3F)))

/* ...accessors */
#define XF_OPCODE_CDATA(opcode)         ((opcode) & (1 << 31))
#define XF_OPCODE_RDATA(opcode)         ((opcode) & (1 << 30))
#define XF_OPCODE_TYPE(opcode)          ((opcode) & (0x3F))

/*******************************************************************************
 * Opcode types
 ******************************************************************************/

/* ...unregister client */
#define XF_UNREGISTER                   __XF_OPCODE(0, 0, 0)

/* ...register client at proxy */
#define XF_REGISTER                     __XF_OPCODE(1, 0, 1)

/* ...port routing command */
#define XF_ROUTE                        __XF_OPCODE(1, 0, 2)

/* ...port unrouting command */
#define XF_UNROUTE                      __XF_OPCODE(1, 0, 3)

/* ...shared buffer allocation */
#define XF_ALLOC                        __XF_OPCODE(0, 0, 4)

/* ...shared buffer freeing */
#define XF_FREE                         __XF_OPCODE(0, 0, 5)

/* ...set component parameters */
#define XF_SET_PARAM                    __XF_OPCODE(1, 0, 6)

/* ...get component parameters */
#define XF_GET_PARAM                    __XF_OPCODE(1, 1, 7)

/* ...input buffer reception */
#define XF_EMPTY_THIS_BUFFER            __XF_OPCODE(1, 0, 8)

/* ...output buffer reception */
#define XF_FILL_THIS_BUFFER             __XF_OPCODE(0, 1, 9)

/* ...flush specific port */
#define XF_FLUSH                        __XF_OPCODE(0, 0, 10)

/* ...start component operation */
#define XF_START                        __XF_OPCODE(0, 0, 11)

/* ...stop component operation */
#define XF_STOP                         __XF_OPCODE(0, 0, 12)

/* ...pause component operation */
#define XF_PAUSE                        __XF_OPCODE(0, 0, 13)

/* ...resume component operation */
#define XF_RESUME                       __XF_OPCODE(0, 0, 14)

/* ...extended parameter setting function */
#define XF_SET_PARAM_EXT                __XF_OPCODE(1, 0, 15)

/* ...extended parameter retrieval function */
#define XF_GET_PARAM_EXT                __XF_OPCODE(1, 1, 16)

/* ...set priorities */
#define XF_SET_PRIORITIES               __XF_OPCODE(1, 0, 17)

/* ...channel setup */
#define XF_EVENT_CHANNEL_CREATE         __XF_OPCODE(1, 0, 18)

/* ...channel delete */
#define XF_EVENT_CHANNEL_DELETE         __XF_OPCODE(1, 0, 19)

/* ...channel setup */
#define XF_EVENT                        __XF_OPCODE(1, 1, 20)

/* ...total amount of supported decoder commands */
#define __XF_OP_NUM                     21

/*******************************************************************************
 * XF_START message definition
 ******************************************************************************/

typedef struct xf_start_msg
{
    /* ...effective sample rate */
    UWORD32             sample_rate;

    /* ...number of channels */
    UWORD32             channels;

    /* ...sample width */
    UWORD32             pcm_width;

    /* ...minimal size of intput buffer */
    UWORD32             input_length[XF_CFG_MAX_IN_PORTS];

    /* ...size of output buffer */
    UWORD32             output_length[XF_CFG_MAX_OUT_PORTS];

    /* ...size of probe buffer */
    UWORD32				probe_length;

}   __attribute__((__packed__)) xf_start_msg_t;

/*******************************************************************************
 * XF_GET_PARAM message
 ******************************************************************************/

/* ...message body (command/response) */
typedef union xf_get_param_msg
{
    /* ...command structure */
    struct
    {
        /* ...array of parameters requested */
        UWORD32                 id[0];

    }   __attribute__((__packed__)) c;

    /* ...response structure */
    struct
    {
        /* ...array of parameters values */
        UWORD32                 value[0];

    }   __attribute__((__packed__)) r;

}   xf_get_param_msg_t;

/* ...length of the XF_GET_PARAM command/response */
#define XF_GET_PARAM_CMD_LEN(params)    (sizeof(UWORD32) * (params))
#define XF_GET_PARAM_RSP_LEN(params)    (sizeof(UWORD32) * (params))

/*******************************************************************************
 * XF_SET_PARAM message
 ******************************************************************************/

/* ...component initialization parameter */
typedef struct xf_set_param_item
{
    /* ...index of parameter passed to SET_CONFIG_PARAM call */
    UWORD32                 id;

    /* ...value of parameter */
    UWORD32                 value;

}   __attribute__ ((__packed__)) xf_set_param_item_t;

/* ...message body (no response message? - tbd) */
typedef struct xf_set_param_msg
{
    /* ...command message */
    xf_set_param_item_t     item[0];

}   __attribute__ ((__packed__)) xf_set_param_msg_t;

/* ...length of the command message */
#define XF_SET_PARAM_CMD_LEN(params)    (sizeof(xf_set_param_item_t) * (params))

/*******************************************************************************
 * XF_SET_PARAM_EXT/XF_GET_PARAM_EXT message
 ******************************************************************************/

/* ...extended parameter descriptor */
typedef struct xf_ext_param_desc
{
    /* ...index of parameter passed to SET/GET_CONFIG_PARAM call (16-bits only) */
    UWORD32                 id;

    /* ...length of embedded input/output parameter data (in bytes) */
    UWORD32                 length;

}   __attribute__ ((__packed__, __aligned__(4))) xf_ext_param_desc_t;

/* ...message body (no response message? - tbd) */
typedef struct xf_ext_param_msg
{
    /* ...extended parameter descriptor */
    xf_ext_param_desc_t     desc;

    /* ...parameter data (in the format expected by codec; 4 bytes aligned) */
    UWORD8                      data[0];

}   __attribute__ ((__packed__, __aligned__(4))) xf_ext_param_msg_t;

/*******************************************************************************
 * XF_ROUTE definition
 ******************************************************************************/

/* ...port routing command */
typedef struct xf_route_port_msg
{
	/* ...source port specification */
	xf_msg_id_dtype         src;

	/* ...destination port specification */
	xf_msg_id_dtype         dst;

	/* ...number of buffers to allocate */
	UWORD32                 alloc_number;

	/* ...length of buffer to allocate */
	UWORD32                 alloc_size;

	/* ...alignment restriction for a buffer */
	UWORD32                 alloc_align;

}	__attribute__((__packed__, __aligned__(sizeof(xf_msg_id_dtype)))) xf_route_port_msg_t;

/*******************************************************************************
 * XF_UNROUTE definition
 ******************************************************************************/

/* ...port unrouting command */
typedef struct xf_unroute_port_msg
{
	/* ...source port specification */
	xf_msg_id_dtype         src;

	/* ...destination port specification */
	xf_msg_id_dtype         dst;

}	__attribute__((__packed__, __aligned__(sizeof(xf_msg_id_dtype)))) xf_unroute_port_msg_t;

#ifndef XA_DISABLE_EVENT
/*******************************************************************************
 * XF_EVENT_CHANNEL_CREATE definition
 ******************************************************************************/

/* ...event channel setup command */
typedef struct xf_event_channel_msg
{
	/* ...source port specification */
	xf_msg_id_dtype         src;

	/* ...destination port specification */
	xf_msg_id_dtype         dst;

    /* ... source command */
    UWORD32                 src_cfg_param;

    /* ... dest command */
    UWORD32                 dst_cfg_param;

	/* ...number of buffers to allocate */
	UWORD32                 alloc_number;

	/* ...length of buffer to allocate */
	UWORD32                 alloc_size;

	/* ...alignment restriction for a buffer */
	UWORD32                 alloc_align;

}	__attribute__((__packed__, __aligned__(sizeof(xf_msg_id_dtype)))) xf_event_channel_msg_t;

/*******************************************************************************
 * XF_EVENT_CHANNEL_DELETE definition
 ******************************************************************************/

/* ...event channel setup command */
typedef struct xf_event_channel_delete_msg
{
	/* ...source port specification */
	xf_msg_id_dtype         src;

	/* ...destination port specification */
	xf_msg_id_dtype         dst;

    /* ... source command */
    UWORD32                 src_cfg_param;

    /* ... dest command */
    UWORD32                 dst_cfg_param;

}	__attribute__((__packed__, __aligned__(sizeof(xf_msg_id_dtype)))) xf_event_channel_delete_msg_t;
#endif  /* XA_DISABLE_EVENT */

/*******************************************************************************
 * XF_SET_PRIORITIES definition
 ******************************************************************************/

/* preemptive scheduling priorities setup command */
typedef struct xf_set_priorities_msg
{
    /* number of real-time priority levels */
    UWORD32 n_rt_priorities;

    /* lowest real-time priority */
    UWORD32 rt_priority_base;

    /* priority for background/default processing */
    UWORD32 bg_priority;

} xf_set_priorities_msg_t;
