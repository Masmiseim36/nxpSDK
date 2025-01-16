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
#ifndef _OSAL_MSGQ_H
#define _OSAL_MSGQ_H

#include <xtensa/xos.h>
#include "xaf-api.h"

/*******************************************************************************
 * External Definitions
 ******************************************************************************/
typedef struct {
    XosMsgQueue queue;
} *xf_msgq_t;


static inline int __xf_msgq_get_size(size_t n_items, size_t item_size)
{
    xf_msgq_t q;
    return (sizeof(*q) + XOS_MSGQ_SIZE(n_items, item_size));
}

/* ...open proxy interface on proper DSP partition */
static inline xf_msgq_t __xf_msgq_create(size_t n_items, size_t item_size, void *pbuf)
{
    xf_msgq_t q = (xf_msgq_t )pbuf;
    if (xos_msgq_create(&q->queue, (uint16_t)n_items, (uint32_t)item_size, (uint16_t)0) == XOS_OK)
        return q;
    return NULL;
}

/* ...close proxy handle */
static inline void __xf_msgq_destroy(xf_msgq_t q)
{
    xos_msgq_delete(&q->queue);
}

static inline int __xf_msgq_send(xf_msgq_t q, const void *data, size_t sz)
{
    return xos_msgq_put(&q->queue, data) == XOS_OK ? XAF_NO_ERR : XAF_RTOS_ERR;
}

#define MAXIMUM_TIMEOUT 10000

static inline int __xf_msgq_recv_blocking(xf_msgq_t q, void *data, size_t sz)
{
    int ret = xos_msgq_get(&q->queue, data);

    if ( ret == XOS_OK )
    {
        ret = XAF_NO_ERR;
    }
    else
    {
        ret = XAF_RTOS_ERR;
    }

    return  ret;
}

static inline int __xf_msgq_recv(xf_msgq_t q, void *data, size_t sz)
{
    int ret = xos_msgq_get_timeout(&q->queue, data, xos_msecs_to_cycles(MAXIMUM_TIMEOUT));

    if ( ret == XOS_OK )
    {
        ret = XAF_NO_ERR;
    }
    else if ( ret == XOS_ERR_TIMEOUT )
    {
        ret = XAF_TIMEOUT_ERR;
    }
    else
    {
        ret = XAF_RTOS_ERR;
    }

    return  ret;
}

static inline int __xf_msgq_empty(xf_msgq_t q)
{
    return xos_msgq_empty(&q->queue);
}

static inline int __xf_msgq_full(xf_msgq_t q)
{
    return xos_msgq_full(&q->queue);
}

#endif
