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
 * xf-sched.h
 *
 * Non-preemptive earliest-deadline-first scheduler
 ******************************************************************************/

#ifndef __XF_SCHED_H
#define __XF_SCHED_H

/*******************************************************************************
 * Types definitions
 ******************************************************************************/

/* ...scheduler data */
typedef struct xf_sched {
    xf_flx_lock_t   lock;
    rb_tree_t       tree;
} xf_sched_t;

/* ...scheduling item */
typedef rb_node_t   xf_task_t;

/*******************************************************************************
 * Helpers
 ******************************************************************************/

/* ...retrieve timestamp from task handle */
static inline UWORD64 xf_task_timestamp(xf_task_t *t)
{
    /* ...wipe out last bit of "color" */
    return t->color & ~1;
}

/* ...set task decoding timestamp */
static inline UWORD64 xf_task_timestamp_set(xf_task_t *t, UWORD64 ts)
{
    /* ...technically, wiping out last bit of timestamp is not needed */
    return t->color = ts;
}

/* ...compare two timestamps with respect to wrap-around */
static inline UWORD64 xf_timestamp_before(UWORD64 t0, UWORD64 t1)
{
    /* ...distance between active items is never high */
    return ((WORD64)(t0 - t1) < 0);
}

/*******************************************************************************
 * Entry points
 ******************************************************************************/

/* ...place message into scheduler queue */
extern void xf_sched_put(xf_sched_t *sched, xf_task_t *t, UWORD64 dts);

/* ...get first item from the scheduler */
extern xf_task_t * xf_sched_get(xf_sched_t *sched);

/* ...cancel task execution */
extern UWORD32 xf_sched_cancel(xf_sched_t *sched, xf_task_t *t);

/* ...initialize scheduler */
extern void xf_sched_init(xf_sched_t *sched);

/* ...reinitialize scheduler lock*/
extern void xf_sched_preempt_reinit(xf_sched_t *sched);

/* ...deinitialize scheduler */
extern void xf_sched_deinit(xf_sched_t *sched);

#endif  /* __XF_SCHED_H */
