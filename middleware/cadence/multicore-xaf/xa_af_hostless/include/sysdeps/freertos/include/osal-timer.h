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
 * osal-timer.h
 *
 * OS absraction layer (minimalistic) for FreeRTOS
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/

#ifndef _OSAL_TIMER_H
#define _OSAL_TIMER_H

#include <FreeRTOS.h>
#include <timers.h>

/*******************************************************************************
 * Timer support
 ******************************************************************************/

typedef void xf_timer_fn_t(void *arg);
typedef struct {
    TimerHandle_t timer;
    xf_timer_fn_t *fn;
    void *arg;
} xf_timer_t;

static inline void __xf_freertos_timer_wrapper(TimerHandle_t t)
{
    xf_timer_t *timer = pvTimerGetTimerID(t);
    timer->fn(timer->arg);
}

static inline int __xf_timer_init(xf_timer_t *timer, xf_timer_fn_t *fn,
                                  void *arg, int autoreload)
{
    timer->fn = fn;
    timer->arg = arg;
    timer->timer = xTimerCreate("", 1, autoreload, timer,
                                __xf_freertos_timer_wrapper);
    return timer->timer ? 0 : -1;
}

static inline unsigned long __xf_timer_ratio_to_period(unsigned long numerator,
                                                       unsigned long denominator)
{
    return numerator * 1000ull / portTICK_PERIOD_MS / denominator;
}

static inline int __xf_timer_start(xf_timer_t *timer, unsigned long period)
{
    if (xTimerGetPeriod(timer->timer) != period &&
        xTimerChangePeriod(timer->timer, period, portMAX_DELAY) != pdPASS)
        return -1;
    return xTimerStart(timer->timer, portMAX_DELAY) == pdPASS ? 0 : -1;
}

static inline int __xf_timer_stop(xf_timer_t *timer)
{
    return xTimerStop(timer->timer, portMAX_DELAY) == pdPASS ? 0 : -1;
}

static inline int __xf_timer_destroy(xf_timer_t *timer)
{
    return xTimerDelete(timer->timer, portMAX_DELAY) == pdPASS ? 0 : -1;
}

#if 0
static inline void __xf_sleep(unsigned long period)
{
    vTaskDelay(period);
}
#endif

#endif
