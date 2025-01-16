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
 * xf-sched.c
 *
 * Non-preemptive earliest-deadline-first scheduler
 ******************************************************************************/

#define MODULE_TAG                      SCHED

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf-dp.h"


/* ...current scheduler timestamp */
static inline UWORD64 xf_sched_timestamp(xf_sched_t *sched)
{
    /* ...don't quite care about last bit */
    return sched->tree.root.color;
}

/* ...set scheduler timestamp */
static inline UWORD64 xf_sched_timestamp_set(xf_sched_t *sched, UWORD64 ts)
{
    /* ...wipe out last bit (black color is 0) */
    return (sched->tree.root.color = ts & ~0x1);
}

/*******************************************************************************
 * Global functions definitions
 ******************************************************************************/

/* ...place task into scheduler queue */
void xf_sched_put(xf_sched_t *sched, xf_task_t *t, UWORD64 dts)
{
    rb_tree_t  *tree = &sched->tree;
    rb_node_t  *node = (rb_node_t *)t;
    rb_idx_t    p_idx, t_idx;
    UWORD64 ts;
    UWORD64         _ts;

    xf_flx_lock(&sched->lock);

    ts = xf_sched_timestamp(sched) + dts;
    /* ...set scheduling timestamp */
    xf_task_timestamp_set(t, ts);

    /* ...find a place in the tree where the message should be inserted */
    for (p_idx = rb_root(tree); p_idx != rb_null(tree); p_idx = t_idx)
    {
        /* ...get timestamp of the p_idx */
        _ts = xf_task_timestamp((xf_task_t *)p_idx);

        /* ...ordering respects FIFO order of messages with same timestamp */
        if (xf_timestamp_before(ts, _ts))
        {
            if ((t_idx = rb_left(tree, p_idx)) == rb_null(tree))
            {
                /* ...p_idx is a direct successor of the message */
                rb_set_left(tree, p_idx, node);

                /* ...adjust head of the tree if needed */
                if (p_idx == rb_cache(tree))    goto insert_head;
                else                            goto insert;
            }
        }
        else
        {
            if ((t_idx = rb_right(tree, p_idx)) == rb_null(tree))
            {
                /* ...p_idx is a direct predeccessor of the message */
                rb_set_right(tree, p_idx, node);

                goto insert;
            }
        }
    }

insert_head:
    /* ...adjust scheduler head element */
    rb_set_cache(tree, node);

insert:
    /* ...insert item and rebalance the tree */
    rb_insert(tree, node, p_idx);

    /* ...head cannot be NULL */
    BUG(rb_cache(tree) == rb_null(tree), _x("Invalid scheduler state"));

    TRACE(DEBUG, _b("in:  %016llx:[%p] (ts:%016llx)"), ts, node, xf_sched_timestamp(sched));
    xf_flx_unlock(&sched->lock);
}

/* ...get first item from the scheduler */
xf_task_t * xf_sched_get(xf_sched_t *sched)
{
    rb_tree_t      *tree = &sched->tree;
    rb_idx_t        n_idx, t_idx;
    UWORD64             ts;

    xf_flx_lock(&sched->lock);
    /* ...head of the tree is cached; replace it with its parent (direct successor) */
    if ((n_idx = rb_cache(tree)) != rb_null(tree)) {
        /* ...delete current node and rebalance the tree */
        t_idx = rb_delete(tree, n_idx), rb_set_cache(tree, t_idx);

        /* ...get task timestamp */
        ts = xf_task_timestamp((xf_task_t *)n_idx);

        /* ...advance scheduler timestamp */
        xf_sched_timestamp_set(sched, ts);

        TRACE(DEBUG, _b("out: %016llx:[%p]"), ts, n_idx);
    } else {
        n_idx = NULL;
    }
    xf_flx_unlock(&sched->lock);
    return n_idx;
}

/* ...cancel specified task execution (must be scheduled!) */
UWORD32 xf_sched_cancel(xf_sched_t *sched, xf_task_t *t)
{
    rb_tree_t      *tree = &sched->tree;
    rb_idx_t        n_idx = t;
    rb_idx_t        t_idx;
    UWORD32         err;

    xf_flx_lock(&sched->lock);

    /* ...delete message from tree */
    t_idx = rb_delete(tree, n_idx);

    if(t_idx == (rb_idx_t)NULL)
    {
        /* ...node is not found on the tree: set deletion failed message */
        err = 1;
    }
    else
    {
        /* ...adjust head if that was the first message */
        if (n_idx == rb_cache(tree))
            rb_set_cache(tree, t_idx);

        /* ...node is found on the tree: set deletion OK message */
        err = 0;
    }

    xf_flx_unlock(&sched->lock);
    return err;
}

/* ...initialize scheduler data */
void xf_sched_init(xf_sched_t *sched)
{
    xf_flx_lock_init(&sched->lock, XF_DUMMY_LOCK);
    rb_init(&sched->tree);
}

/* ...reinitialize scheduler lock */
void xf_sched_preempt_reinit(xf_sched_t *sched)
{
    xf_flx_lock_reinit(&sched->lock, XF_MUTEX_BASED_LOCK);
}

/* ...deinit scheduler data */
void xf_sched_deinit(xf_sched_t *sched)
{
    xf_flx_lock_destroy(&sched->lock);
}
