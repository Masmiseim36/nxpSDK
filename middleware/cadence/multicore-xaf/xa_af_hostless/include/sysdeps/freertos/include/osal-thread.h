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
 * OS absraction layer (minimalistic) for FreeRTOS
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/

#ifndef _OSAL_THREAD_H
#define _OSAL_THREAD_H

#include <FreeRTOS.h>
#include <event_groups.h>
#include <semphr.h>
#include <stdlib.h>
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
typedef SemaphoreHandle_t xf_lock_t;

/* ...lock initialization */
static inline void __xf_lock_init(xf_lock_t *lock)
{
    *lock = xSemaphoreCreateMutex();
}

/* ...lock deletion */
static inline void __xf_lock_destroy(xf_lock_t *lock)
{
    vSemaphoreDelete(*lock);
}

/* ...lock acquisition */
static inline void __xf_lock(xf_lock_t *lock)
{
    if (xSemaphoreTake(*lock, portMAX_DELAY) != pdTRUE) {
        /* abort */
    }
}

/* ...lock release */
static inline void __xf_unlock(xf_lock_t *lock)
{
    xSemaphoreGive(*lock);
}

/*******************************************************************************
 * Event support
 ******************************************************************************/

typedef EventGroupHandle_t xf_event_t;

static inline void __xf_event_init(xf_event_t *event, uint32_t mask)
{
    *event = xEventGroupCreate();
}

static inline void __xf_event_destroy(xf_event_t *event)
{
    vEventGroupDelete(*event);
}

static inline uint32_t __xf_event_get(xf_event_t *event)
{
    return xEventGroupGetBits(*event);
}

static inline void __xf_event_set(xf_event_t *event, uint32_t mask)
{
    xEventGroupSetBits(*event, mask);
}

static inline void __xf_event_set_isr(xf_event_t *event, uint32_t mask)
{
    //xEventGroupSetBitsFromISR(*event, mask, NULL);
    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;
    if(xEventGroupSetBitsFromISR(*event, mask, &xHigherPriorityTaskWoken) != pdFAIL)
    {
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}

static inline void __xf_event_clear(xf_event_t *event, uint32_t mask)
{
    xEventGroupClearBits(*event, mask);
}

static inline void __xf_event_wait_any(xf_event_t *event, uint32_t mask)
{
    xEventGroupWaitBits(*event, mask, pdFALSE, pdFALSE, portMAX_DELAY);
}

static inline void __xf_event_wait_all(xf_event_t *event, uint32_t mask)
{
    xEventGroupWaitBits(*event, mask, pdFALSE, pdTRUE, portMAX_DELAY);
}

/*******************************************************************************
 * Thread support
 ******************************************************************************/

/* ...thread handle definition */

typedef void *xf_entry_t(void *);

typedef struct   {
    xf_entry_t *f;
    void *oarg;
    SemaphoreHandle_t done;
}__xf_thread_parameters;

typedef struct {
    TaskHandle_t task;
    __xf_thread_parameters tp;
} xf_thread_t;


static inline void __xf_freertos_thread_wrapper(void *arg)
{
    __xf_thread_parameters * tp_ptr = (__xf_thread_parameters *)arg;
    tp_ptr->f(tp_ptr->oarg);
    xSemaphoreGive(tp_ptr->done);
    vTaskSuspend(NULL);
}

static inline int __xf_thread_init(xf_thread_t *thread)
{

    thread->task = NULL;
    return 0;
}

/* ...thread creation */
static inline int __xf_thread_create(xf_thread_t *thread, xf_entry_t *f,
                                     void *arg, const char *name, void * stack,
                                     unsigned int stack_size, int priority)
{
    BaseType_t r;
    __xf_thread_parameters *tp_ptr = &thread->tp;
    tp_ptr->done = xSemaphoreCreateCounting(1, 0);

    tp_ptr->f = f;
    tp_ptr->oarg = arg;

    r = xTaskCreate(__xf_freertos_thread_wrapper, name,
                    (uint16_t)(stack_size / sizeof(StackType_t)), tp_ptr,
                    priority, &thread->task);

    return r == pdPASS ? 0 : -r;
}

static inline void __xf_thread_yield(void)
{
    taskYIELD();
}

static inline int __xf_thread_cancel(xf_thread_t *thread)
{
    vTaskSuspend(thread->task);
    xSemaphoreGive(thread->tp.done);
    return 0;
}

/* TENA-2117*/
static inline int __xf_thread_join(xf_thread_t *thread, int32_t * p_exitcode)
{
    if (thread->task != NULL)
    {
        xSemaphoreTake(thread->tp.done, portMAX_DELAY);
        if (p_exitcode)
            *p_exitcode = 0;
    }
    return 0;
}

/* ...terminate thread operation */
static inline int __xf_thread_destroy(xf_thread_t *thread)
{
    if (thread->task != NULL)
    {
        vTaskDelete(thread->task);
        vSemaphoreDelete(thread->tp.done);
        thread->task = NULL;
    }
    return 0;
}

static inline const char *__xf_thread_name(xf_thread_t *thread)
{
    return pcTaskGetName(thread ? thread->task : NULL);
}

/* ... Put thread to sleep for at least the specified number of msec */
static inline int32_t __xf_thread_sleep_msec(uint64_t msecs)
{
    int32_t    r = 0;

    /* Convert milliseconds to ticks */
    const TickType_t xDelay = pdMS_TO_TICKS(msecs);
    vTaskDelay(xDelay);

    /* ...return final status */
    return r;
}

/* ... state of the thread */
#define XF_THREAD_STATE_INVALID	(eInvalid)
#define XF_THREAD_STATE_READY	(eReady)
#define XF_THREAD_STATE_RUNNING	(eRunning)
#define XF_THREAD_STATE_BLOCKED	(eBlocked)
#define XF_THREAD_STATE_EXITED	(eDeleted)

static inline int32_t __xf_thread_get_state (xf_thread_t *thread)
{
    int32_t    r = 0;

    if ( thread->task == NULL )
        return XF_THREAD_STATE_INVALID;

    r = (int) eTaskGetState(thread->task);

    if (r == eSuspended) r = XF_THREAD_STATE_EXITED;

    /* ...return final status */
    return r;
}

static inline void __xf_disable_preemption(void)
{
    taskENTER_CRITICAL();
}

static inline void __xf_enable_preemption(void)
{
    taskEXIT_CRITICAL();
}

static inline int32_t __xf_thread_get_priority (xf_thread_t *thread)
{
    return ((int)uxTaskPriorityGet(thread ? thread->task : NULL));
}

static inline int32_t __xf_thread_set_priority (xf_thread_t *thread, int32_t priority)
{
    int r = 0;
    vTaskPrioritySet(thread ? thread->task : NULL, priority);

    return r;
}

#endif
