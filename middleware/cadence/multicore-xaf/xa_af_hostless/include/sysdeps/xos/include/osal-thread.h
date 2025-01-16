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
 * osal-thread.h
 *
 * OS absraction layer (minimalistic) for XOS
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#ifndef _OSAL_THREAD_H
#define _OSAL_THREAD_H

#ifdef __TOOLS_RF2__
#include <xtensa/xos/xos.h>
#else   // #ifdef __TOOLS_RF2__
#include <xtensa/xos.h>
#endif  // #ifdef __TOOLS_RF2__

#include <string.h>

/*******************************************************************************
 * Tracing primitive
 ******************************************************************************/

#define __xf_puts(str)                  \
    puts((str))

/*******************************************************************************
 * Lock operation
 ******************************************************************************/

/* ...lock definition */
typedef XosSem     xf_lock_t;

/* ...lock initialization */
static inline void __xf_lock_init(xf_lock_t *lock)
{
    xos_sem_create(lock, XOS_SEM_WAIT_PRIORITY, 1);
}

/* ...lock deletion */
static inline void __xf_lock_destroy(xf_lock_t *lock)
{
    xos_sem_delete(lock);
}

/* ...lock acquisition */
static inline void __xf_lock(xf_lock_t *lock)
{
    xos_sem_get(lock);
}

/* ...lock release */
static inline void __xf_unlock(xf_lock_t *lock)
{
    xos_sem_put(lock);
}

/*******************************************************************************
 * Event support
 ******************************************************************************/

typedef XosEvent     xf_event_t;

static inline void __xf_event_init(xf_event_t *event, uint32_t mask)
{
    xos_event_create(event, mask, 0);
}

static inline void __xf_event_destroy(xf_event_t *event)
{
    xos_event_delete(event);
}

static inline uint32_t __xf_event_get(xf_event_t *event)
{
    uint32_t rv;

    xos_event_get(event, &rv);
    return rv;
}

static inline void __xf_event_set(xf_event_t *event, uint32_t mask)
{
    xos_event_set(event, mask);
}

static inline void __xf_event_set_isr(xf_event_t *event, uint32_t mask)
{
    xos_event_set(event, mask);
}

static inline void __xf_event_clear(xf_event_t *event, uint32_t mask)
{
    xos_event_clear(event, mask);
}

static inline void __xf_event_wait_any(xf_event_t *event, uint32_t mask)
{
    xos_event_wait_any(event, mask);
}

static inline void __xf_event_wait_all(xf_event_t *event, uint32_t mask)
{
    xos_event_wait_all(event, mask);
}

/*******************************************************************************
 * Thread support
 ******************************************************************************/

/* ...thread handle definition */
typedef XosThread           xf_thread_t;
typedef void *xf_entry_t(void *);


/* This is dummy for xos */
static inline int __xf_thread_init(xf_thread_t *thread)
{
    return 0;
}

/* ...thread creation
 *
 * return: 0 -- OK, negative -- OS-specific error code
 */

static inline int __xf_thread_create(xf_thread_t *thread, xf_entry_t *f,
                                     void *arg, const char *name, void *stack,
                                     unsigned int stack_size, int priority)
{
    return xos_thread_create(thread, 0, ( XosThreadFunc *)f, arg, name, stack, stack_size, (int8_t)priority, 0, 0);
}

static inline void __xf_thread_yield(void)
{
    xos_thread_yield();
}

static inline int __xf_thread_cancel(xf_thread_t *thread)
{
    return xos_thread_abort(thread, 0);
}

/* TENA-2117*/
static inline int __xf_thread_join(xf_thread_t *thread, int32_t * p_exitcode)
{
    int    r;

    r = xos_thread_join(thread, p_exitcode);

    return r;
}

/* ...terminate thread operation */
static inline int __xf_thread_destroy(xf_thread_t *thread)
{
    int    r;

    /* ...wait until thread terminates */
    /* v-tbd - avoid infinite wait for join */
    //xos_thread_join(thread, &r);

    /* ...delete thread, free up TCB, stack */
    r = xos_thread_delete(thread);

    /* ...return final status */
    return r;
}

static inline const char *__xf_thread_name(xf_thread_t *thread)
{
    return xos_thread_get_name(thread ? thread : XOS_THREAD_SELF);
}

/* ... Put calling thread to sleep for at least the specified number of msec */
static inline int32_t __xf_thread_sleep_msec(uint64_t msecs)
{
    int32_t    r;

    r = xos_thread_sleep_msec( msecs );

    /* ...return final status */
    return r;
}

/* ... state of the thread */
#define XF_THREAD_STATE_INVALID (XOS_THREAD_STATE_INVALID)  /* 0 */
#define XF_THREAD_STATE_BLOCKED (XOS_THREAD_STATE_BLOCKED)  /* 1 */
#define XF_THREAD_STATE_READY	(XOS_THREAD_STATE_READY)    /* 2 */
#define XF_THREAD_STATE_RUNNING	(XOS_THREAD_STATE_RUNNING)  /* 3 */
#define XF_THREAD_STATE_EXITED  (XOS_THREAD_STATE_EXITED)   /* 4 */

static inline int32_t __xf_thread_get_state (xf_thread_t *thread)
{
    int32_t    r = 0;

    r = (int)xos_thread_get_state( thread );

    /* ...return final status */
    return r;
}

static inline void __xf_disable_preemption(void)
{
    xos_preemption_disable();
}

static inline void __xf_enable_preemption(void)
{
    xos_preemption_enable();
}

static inline int32_t __xf_thread_get_priority (xf_thread_t *thread)
{
    return ((int) xos_thread_get_priority(thread ? thread : XOS_THREAD_SELF));
}

static inline int32_t __xf_thread_set_priority (xf_thread_t *thread, int32_t priority)
{
    int r = 0;
    xos_thread_set_priority(thread ? thread : XOS_THREAD_SELF, (int8_t)priority);

    return r;
}


#endif
