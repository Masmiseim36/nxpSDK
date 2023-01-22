/*
* Copyright (c) 2015-2022 Cadence Design Systems Inc.
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
 * osal-timer.h
 *
 * OS absraction layer (minimalistic) for XOS
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#ifndef _OSAL_TIMER_H
#define _OSAL_TIMER_H

#ifdef __TOOLS_RF2__
#include <xtensa/xos/xos.h>
#else   // #ifdef __TOOLS_RF2__
#include <xtensa/xos.h>
#endif  // #ifdef __TOOLS_RF2__
#include <stddef.h>

/*******************************************************************************
 * Timer support
 ******************************************************************************/

typedef void xf_timer_fn_t(void *arg);
typedef struct xf_timer {
    XosTimer timer;
    xf_timer_fn_t *fn;
    void *arg;
    int autoreload;
    struct xf_timer *next;
    int active;
} xf_timer_t;

static inline int __xf_timer_init(xf_timer_t *timer, xf_timer_fn_t *fn,
                                  void *arg, int autoreload)
{
    timer->fn = fn;
    timer->arg = arg;
    timer->autoreload = autoreload;
    timer->next = NULL;
    timer->active = 0;
    xos_timer_init(&timer->timer);
    return 0;
}

static inline unsigned long __xf_timer_ratio_to_period(unsigned long numerator,
                                                       unsigned long denominator)
{
    return xos_usecs_to_cycles(numerator * 1000000ull / denominator);
}

int __xf_timer_start(xf_timer_t *timer, unsigned long period);
int __xf_timer_stop(xf_timer_t *timer);

static inline int __xf_timer_destroy(xf_timer_t *timer)
{
    return 0;
}

#if 0
static inline void __xf_sleep(unsigned long period)
{
    xos_thread_sleep(period);
}
#endif

#endif
