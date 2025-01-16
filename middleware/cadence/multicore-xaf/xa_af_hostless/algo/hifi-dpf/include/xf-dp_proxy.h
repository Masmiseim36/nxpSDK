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
 * xf-proxy.h
 *
 * Proxy commmand/response messages
 *******************************************************************************/

#ifndef __XF_H
#error "xf-proxy.h mustn't be included directly"
#endif

/*******************************************************************************
 * Types definitions
 ******************************************************************************/

/* ...command/response message */
typedef struct xf_proxy_message
{
    /* ...session ID */
    xf_msg_id_dtype         session_id;

    /* ...proxy API command/reponse code */
    UWORD32                 opcode;

    /* ...length of attached buffer */
    UWORD32                 length;

    /* ...physical address of message buffer */
    UWORD32                 address;

    /* ...error response*/
    WORD32                 error;

}   __attribute__((__packed__, __aligned__(sizeof(xf_msg_id_dtype)))) xf_proxy_message_t;

/*******************************************************************************
 * Ring buffer support
 ******************************************************************************/

/* ...total length of shared memory queue (for commands and responses) */
#define XF_PROXY_MESSAGE_QUEUE_LENGTH   (1 << 8)

/* ...index mask */
#define XF_PROXY_MESSAGE_QUEUE_MASK     0xFF

/* ...ring-buffer index */
#define __XF_QUEUE_IDX(idx, counter)    \
    (((idx) & XF_PROXY_MESSAGE_QUEUE_MASK) | ((counter) << 16))

/* ...retrieve ring-buffer index */
#define XF_QUEUE_IDX(idx)               \
    ((idx) & XF_PROXY_MESSAGE_QUEUE_MASK)

/* ...increment ring-buffer index */
#define XF_QUEUE_ADVANCE_IDX(idx)       \
    (((idx) + 0x10001) & (0xFFFF0000 | XF_PROXY_MESSAGE_QUEUE_MASK))

/* ...test if ring buffer is empty */
#define XF_QUEUE_EMPTY(read, write)     \
    ((read) == (write))

/* ...test if ring buffer is full */
#define XF_QUEUE_FULL(read, write)      \
    ((write) == (read) + (XF_PROXY_MESSAGE_QUEUE_LENGTH << 16))
