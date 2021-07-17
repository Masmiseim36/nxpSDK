/*
* Copyright (c) 2015-2021 Cadence Design Systems Inc.
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
 * xf-proto.h
 *
 * Audio framework prototypes and API
 *******************************************************************************/

#ifndef __XF_H
#error "xf-proto.h mustn't be included directly"
#endif

/*******************************************************************************
 * Forward types declarations
 ******************************************************************************/

/* ...handle to proxy data */
typedef struct xf_proxy         xf_proxy_t;

/* ...handle to component data */
typedef struct xf_handle        xf_handle_t;

/* ...buffer pool */
typedef struct xf_pool          xf_pool_t;

/* ...individual buffer from pool */
typedef struct xf_buffer        xf_buffer_t;

/* ...buffer pool type */
typedef UWORD32                     xf_pool_type_t;

/* ...user-message */
typedef struct xf_user_msg      xf_user_msg_t;

/* ...proxy-message */
typedef struct xf_proxy_msg     xf_proxy_msg_t;

/* ...response callback */
typedef void (*xf_response_cb)(xf_handle_t *h, xf_user_msg_t *msg);

/*******************************************************************************
 * High-level API functions
 ******************************************************************************/

/* ...component operations */
extern int      xf_open(xf_proxy_t *proxy, xf_handle_t *handle, xf_id_t id, UWORD32 core, xf_response_cb cb);
extern void     xf_close(xf_handle_t *handle);
extern int      xf_command(xf_handle_t *handle, UWORD32 dst, UWORD32 opcode, void *buf, UWORD32 length);
extern int      xf_route(xf_handle_t *src, UWORD32 s_port, xf_handle_t *dst, UWORD32 d_port, UWORD32 num, UWORD32 size, UWORD32 align);
extern int      xf_unroute(xf_handle_t *src, UWORD32 s_port);
extern int      xf_pause(xf_handle_t *comp, WORD32 port);
extern int      xf_resume(xf_handle_t *comp, WORD32 port);
extern int      xf_set_config(xf_handle_t *comp, void *buffer, UWORD32 length);
extern int      xf_get_config(xf_handle_t *comp, void *buffer, UWORD32 length);
extern int      xf_set_priorities(xf_proxy_t *proxy, UWORD32 core, UWORD32 n_rt_priorities, UWORD32 rt_priority_base, UWORD32 bg_priority);

/* ...shared buffers operations */
extern int      xf_pool_alloc(xf_proxy_t *proxy, UWORD32 number, UWORD32 length, xf_pool_type_t type, xf_pool_t **pool, WORD32 id);
extern void     xf_pool_free(xf_pool_t *pool, WORD32 id);
extern xf_buffer_t * xf_buffer_get(xf_pool_t *pool);
extern void     xf_buffer_put(xf_buffer_t *buffer);

/* ...proxy operations */
extern int      xf_proxy_init(xf_proxy_t *proxy, UWORD32 core);
extern void     xf_proxy_close(xf_proxy_t *proxy);

#ifndef DISABLE_EVENT
/* ...event channel operations */
extern int xf_create_event_channel(xf_handle_t *src, UWORD32 src_config_param, xf_handle_t *dst, UWORD32 dst_config_param, UWORD32 num, UWORD32 size, UWORD32 align);
extern int xf_delete_event_channel(xf_handle_t *src, UWORD32 src_config_param, xf_handle_t *dst, UWORD32 dst_config_param);
#endif
