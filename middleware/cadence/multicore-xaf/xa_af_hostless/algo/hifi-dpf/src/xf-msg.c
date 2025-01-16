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
 * xf-msg.c
 *
 * Message/message pool handling
 ******************************************************************************/

#define MODULE_TAG                      MSG

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf-dp.h"

/*******************************************************************************
 * Entry points
 ******************************************************************************/

/* ...allocate message pool */
int xf_msg_pool_init(xf_msg_pool_t *pool, UWORD32 n, UWORD32 core, UWORD32 shared, UWORD32 mem_pool_type)
{
    UWORD32     i;

    /* ...allocate shared memory from global pool */
    XF_CHK_ERR(pool->p = xf_mem_alloc(XF_MM(sizeof(*pool->p) * n), XF_PROXY_ALIGNMENT, core, shared, mem_pool_type), XAF_MEMORY_ERR);

    TRACE(INFO, _b("sizeof(*pool->p):%d n:%d align:%d"), sizeof(*pool->p), n, XF_PROXY_ALIGNMENT);

    /* ...make sure the pool pointer is properly aligned */
    BUG(!XF_IS_ALIGNED(pool->p), _x("Unaligned pool pointer: %p"), pool->p);

    /* ...place all messages into single-liked list */
    for (pool->head = &pool->p[i = 0]; i < n - 1; i++)
    {
        /* ...set message pointer to next message in the pool */
        xf_msg_pool_item(pool, i)->next = xf_msg_pool_item(pool, i + 1);
    }

    /* ...set tail of the list */
    xf_msg_pool_item(pool, i)->next = NULL;

    /* ...save pool size */
    pool->n = n;

    return 0;
}

/* ...destroy memory pool */
void xf_msg_pool_destroy(xf_msg_pool_t *pool, UWORD32 core, UWORD32 shared, UWORD32 mem_pool_type)
{
    /* ...release pool memory (from shared local-IPC memory) */
    xf_mem_free(pool->p, XF_MM(sizeof(*pool->p) * pool->n), core, shared, mem_pool_type);
}

/* ...allocate message from a pool (no concurrent access from other cores) */
xf_message_t * xf_msg_pool_get(xf_msg_pool_t *pool)
{
    __xf_message_t  *_m;

    /* ...pop message from the head of the pool */
    XF_CHK_ERR(_m = pool->head, NULL);

    /* ...advance list head */
    pool->head = (__xf_message_t *)(((xf_message_t *) _m)->next);

    /* ...debug - wipe out message "next" pointer */
    ((xf_message_t *) _m)->next = NULL;

    /* ...return properly aligned message pointer */
    return (xf_message_t *) _m;
}

/* ...return message back to the pool (no concurrent access from other cores) */
void xf_msg_pool_put(xf_msg_pool_t *pool, xf_message_t *m)
{
    __xf_message_t  *_m = (__xf_message_t *) m;

    /* ...make sure the message is properly aligned object */
    BUG(!XF_IS_ALIGNED(_m), _x("Corrupted message pointer: %p"), _m);

    /* ...make sure it is returned to the same pool (need a length for that - tbd) */
    BUG(!xf_msg_from_pool(pool, m), _x("Bad pool/message: %p/%p"), pool->p, _m);

    /* ...place message into the head */
    m->next = (xf_message_t *) pool->head;

    /* ...advance pool head */
    pool->head = _m;
}
