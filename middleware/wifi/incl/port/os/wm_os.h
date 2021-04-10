/*
 *  Copyright 2008-2020 NXP
 *
 *  NXP CONFIDENTIAL
 *  The source code contained or described herein and all documents related to
 *  the source code ("Materials") are owned by NXP, its
 *  suppliers and/or its licensors. Title to the Materials remains with NXP,
 *  its suppliers and/or its licensors. The Materials contain
 *  trade secrets and proprietary and confidential information of NXP, its
 *  suppliers and/or its licensors. The Materials are protected by worldwide copyright
 *  and trade secret laws and treaty provisions. No part of the Materials may be
 *  used, copied, reproduced, modified, published, uploaded, posted,
 *  transmitted, distributed, or disclosed in any way without NXP's prior
 *  express written permission.
 *
 *  No license under any patent, copyright, trade secret or other intellectual
 *  property right is granted to or conferred upon you by disclosure or delivery
 *  of the Materials, either expressly, by implication, inducement, estoppel or
 *  otherwise. Any license under such intellectual property rights must be
 *  express and approved by NXP in writing.
 *
 */

/*! \file wm_os.h
 *  \brief OS Abstraction Layer
 *
 * The OS abstraction layer provides wrapper APIs over some of the
 * commonly used OS primitives. Since the behaviour and semantics of the various
 * OSes differs widely, some abstraction APIs require a specific handling as
 * listed below.
 *
 * \section wm_os_usage Usage
 *
 * The OS abstraction layer provides the following types of primitives:
 *
 * - Thread: Create or delete a thread using os_thread_create() or
 *    os_thread_delete(). Block a thread using os_thread_sleep(). Complete a
 *    thread's execution using os_thread_self_complete().
 * - Message Queue: Create or delete a message queue using os_queue_create() or
 *    os_queue_delete(). Send a message using os_queue_send() and received a
 *    message using os_queue_recv().
 * - Mutex: Create or delete a mutex using os_mutex_create() or
 *    os_mutex_delete(). Acquire a mutex using os_mutex_get() and release it
 *    using os_mutex_put().
 * - Semaphores: Create or delete a semaphore using os_semaphore_create() /
 *    os_semaphore_create_counting() or os_semaphore_delete. Acquire a semaphore
 *    using os_semaphore_get() and release it using os_semaphore_put().
 * - Timers: Create or delete a timer using os_timer_create() or
 *    os_timer_delete(). Change the timer using os_timer_change(). Activate or
 *    de-activate the timer using os_timer_activate() or
 *    os_timer_deactivate(). Reset a timer using os_timer_reset().
 * - Dynamic Memory Allocation: Dynamically allocate memory using
 *    os_mem_alloc(), os_mem_calloc() or os_mem_realloc() and free it using
 *    os_mem_free().
 *
 */

#ifndef _WM_OS_H_
#define _WM_OS_H_

#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "portmacro.h"

#if defined(CPU_MIMXRT1062DVL6A)
#include "clock_config.h"
#endif

#include <wmerrno.h>
#include <wm_utils.h>

#ifdef CONFIG_OS_DEBUG
#define os_dprintf(...) ll_log("[OS]" __VA_ARGS__)
#else
#define os_dprintf(...)
#endif

#define is_isr_context() (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) //(xPortIsInsideInterrupt())

/* System clock frequency. */
extern uint32_t SystemCoreClock;
/* the OS timer register is loaded with CNTMAX */
#define CNTMAX                 ((SystemCoreClock / configTICK_RATE_HZ) - 1UL)
#define CPU_CLOCK_TICKSPERUSEC (SystemCoreClock / 1000000U)
#define USECSPERTICK           (1000000U / configTICK_RATE_HZ)

/**
 * Get the current value of free running microsecond counter
 *
 * \note This will wraparound after CNTMAX and the caller is expected to
 * take care of this.
 *
 * @return The current value of microsecond counter.
 */
#if 0
static inline uint32_t os_get_usec_counter()
{
	return (CNTMAX - SysTick->VAL) / CPU_CLOCK_TICKSPERUSEC;
}
#endif

/** Force a context switch */
#define os_thread_relinquish() taskYIELD()

/** Get current OS tick counter value
 *
 * \return 32 bit value of ticks since boot-up
 */
static inline unsigned os_ticks_get()
{
    if (is_isr_context())
        return xTaskGetTickCountFromISR();
    else
        return xTaskGetTickCount();
}

#if 0
/** Get (wraparound safe) current OS tick counter.
 *
 * Returns a 64 bit unsigned integer. To give a rough idea,
 * for an OS tick period of 1 mS it takes thousands of years
 * before the counter value returned by this API to wrap around.
 * Thus, users of this API can ignore the wrap around problem.
 *
 * \return 64 bit value of ticks since boot-up
 */
static inline unsigned long long os_total_ticks_get()
{
    if (is_isr_context())
        return xTaskGetTotalTickCountFromISR();
    else
        return xTaskGetTotalTickCount();
}
#endif

/** Get ticks to next thread wakeup */
#define os_ticks_to_unblock() xTaskGetUnblockTime()

/**
 * Returns time in micro-secs since bootup
 *
 * @note The value returned will wrap around after sometime and caller is
 * expected to guard itself against this.
 *
 * @return Time in micro-secs since bootup
 */
unsigned int os_get_timestamp(void);

/*** Thread Management ***/
typedef void *os_thread_arg_t;

/**
 * Structure to be used during call to the function
 * os_thread_create(). Please use the macro \ref os_thread_stack_define
 * instead of using this structure directly.
 */
typedef struct os_thread_stack
{
    /** Total stack size */
    int size;
} os_thread_stack_t;

/**
 * Helper macro to define the stack size (in bytes) before a new thread is
 * created using the function os_thread_create().
 */
#define os_thread_stack_define(stackname, stacksize) \
    os_thread_stack_t stackname = {(stacksize) / (sizeof(portSTACK_TYPE))}

typedef xTaskHandle os_thread_t;

static inline const char *get_current_taskname()
{
    os_thread_t *handle = (os_thread_t *)xTaskGetCurrentTaskHandle();
    if (handle)
        return pcTaskGetTaskName(*handle);
    else
        return "Unknown";
}

/** Create new thread
 *
 * This function starts a new thread.  The new thread starts execution by
 * invoking main_func(). The parameter arg is passed as the sole argument of
 * main_func().
 *
 * After finishing execution, the new thread should either call:
 * - os_thread_self_complete() to suspend itself OR
 * - os_thread_delete() to delete itself
 *
 * Failing to do this and just returning from main_func() will result in
 * undefined behavior.
 *
 *
 * @param[out] thandle Pointer to a thread handle
 * @param[in] name Name of the new thread. A copy of this string will be
 * made by the OS for itself. The maximum name length is defined by the
 * macro configMAX_TASK_NAME_LEN in FreeRTOS header file . Any name length
 * above it will be truncated.
 * @param[in] main_func Function pointer to new thread function
 * @param[in] arg The sole argument passed to main_func()
 * @param[in] stack A pointer to initialized object of type \ref
 * os_thread_stack_t. The object should be created and initialized using
 * os_thread_stack_define().
 * @param[in] prio The priority of the new thread. One value among
 * OS_PRIO_0, OS_PRIO_1, OS_PRIO_2, OS_PRIO_3 and OS_PRIO_4 should be
 * passed. OS_PRIO_0 represents the highest priority and OS_PRIO_4
 * represents the lowest priority.
 *
 * @return WM_SUCCESS if thread was created successfully
 * @return -WM_FAIL if thread creation failed
 */
static inline int os_thread_create(os_thread_t *thandle,
                                   const char *name,
                                   void (*main_func)(os_thread_arg_t arg),
                                   void *arg,
                                   os_thread_stack_t *stack,
                                   int prio)
{
    int ret;

    ret = xTaskCreate(main_func, name, stack->size, arg, prio, thandle);

    os_dprintf(
        " Thread Create: ret %d thandle %p"
        " stacksize = %d\r\n",
        ret, thandle ? *thandle : NULL, stack->size);
    return ret == pdPASS ? WM_SUCCESS : -WM_FAIL;
}

static inline os_thread_t os_get_current_task_handle()
{
    return xTaskGetCurrentTaskHandle();
}

/** Terminate a thread
 *
 * This function deletes a thread. The task being deleted will be removed from
 * all ready, blocked, suspended and event lists.
 *
 * @param[in] thandle Pointer to the thread handle of the thread to be
 * deleted. If self deletion is required NULL should be passed.
 *
 * @return WM_SUCCESS if operation success
 * @return -WM_FAIL if operation fails
 */
static inline int os_thread_delete(os_thread_t *thandle)
{
    if (thandle == NULL)
    {
        os_dprintf("OS: Thread Self Delete\r\n");
        vTaskDelete(NULL);
    }
    else
    {
        os_dprintf("OS: Thread Delete: %p\r\n", *thandle);
        vTaskDelete(*thandle);
    }

    *thandle = NULL;

    return WM_SUCCESS;
}

/** Sleep for specified number of OS ticks
 *
 * This function causes the calling thread to sleep and block for the given
 * number of OS ticks. The actual time that the task remains blocked depends on
 * the tick rate. The function os_msec_to_ticks() is provided to convert from
 * real-time to ticks.
 *
 * Any other thread can wake up this task specifically using the API
 * os_thread_wait_abort()
 *
 * @param[in] ticks Number of ticks to sleep
 *
 * @return 0 If slept for given ticks or more
 * @return Positive value if woken up before given ticks.
 * @note The value returned is amount of ticks left before the task was
 * to be originally scheduled to be woken up. So if sleep was for 10 ticks
 * and the task is woken up after 8 ticks then 2 will be returned.
 */
static inline void os_thread_sleep(int ticks)
{
    os_dprintf("OS: Thread Sleep: %d\r\n", ticks);
    vTaskDelay(ticks);
    return;
}

/** Convert milliseconds to OS ticks
 *
 * This function converts the given millisecond value to the number of OS
 * ticks.
 *
 * This is useful as functions like os_thread_sleep() accept only ticks
 * as input.
 *
 * @param[in] msecs Milliseconds
 *
 * @return Number of OS ticks corresponding to msecs
 */
/*! @brief Convert the milliseconds to ticks in FreeRTOS. */

static inline unsigned long os_msec_to_ticks(unsigned long msecs)
{
    return (msecs) / (portTICK_RATE_MS);
}

/** Convert ticks to milliseconds
 *
 * This function converts the given ticks value to milliseconds. This is useful
 * as some functions, like os_ticks_get(), return values in units of OS ticks.
 *
 * @param[in] ticks OS ticks
 *
 * @return Number of milliseconds corresponding to ticks
 */
static inline unsigned long os_ticks_to_msec(unsigned long ticks)
{
    return (ticks) * (portTICK_RATE_MS);
}

/** Suspend the given thread
 *
 * - The function os_thread_self_complete() will \b permanently suspend the
 * given thread. Passing NULL will suspend the current thread. This
 * function never returns.
 * - The thread continues to consume system resources. To delete the thread
 * the function os_thread_delete() needs to be called separately.
 *
 * @param[in] thandle Pointer to thread handle
 */
static inline void os_thread_self_complete(os_thread_t *thandle)
{
    /* Suspend self until someone calls delete. This is required because in
     * freeRTOS, main functions of a thread cannot return.
     */
    if (thandle != NULL)
    {
        os_dprintf("OS: Thread Complete: %p\r\n", *thandle);
        vTaskSuspend(*thandle);
    }
    else
    {
        os_dprintf("OS: Thread Complete: SELF\r\n");
        vTaskSuspend(NULL);
    }

    /*
     * We do not want this function to return ever.
     */
    while (1)
        os_thread_sleep(os_msec_to_ticks(60000));
}

#ifndef CONFIG_WIFI_MAX_PRIO
#error Define CONFIG_WIFI_MAX_PRIO in wifi_config.h
#elif CONFIG_WIFI_MAX_PRIO < 4
#error CONFIG_WIFI_MAX_PRIO must be defined to be greater than or equal to 4
#endif
#define OS_PRIO_0 CONFIG_WIFI_MAX_PRIO /** High **/
#define OS_PRIO_1 (CONFIG_WIFI_MAX_PRIO - 1)
#define OS_PRIO_2 (CONFIG_WIFI_MAX_PRIO - 2)
#define OS_PRIO_3 (CONFIG_WIFI_MAX_PRIO - 3)
#define OS_PRIO_4 (CONFIG_WIFI_MAX_PRIO - 4) /** Low **/

/** Structure used for queue definition */
typedef struct os_queue_pool
{
    /** Size of the queue */
    int size;
} os_queue_pool_t;

/** Define OS Queue pool
 *
 * This macro helps define the name and size of the queue to be created
 * using the function os_queue_create().
 */
#define os_queue_pool_define(poolname, poolsize) os_queue_pool_t poolname = {poolsize};

typedef xQueueHandle os_queue_t;

/** Create an OS queue
 *
 * This function creates a new queue instance. This allocates the storage
 * required by the new queue and returns a handle for the queue.
 *
 * @param[out] qhandle Pointer to the handle of the newly created queue
 * @param[in] name String specifying the name of the queue
 * @param[in] msgsize The number of bytes each item in the queue will
 * require. Items are queued by copy, not by reference, so this is the
 * number of bytes that will be copied for each posted item. Each item on
 * the queue must be the same size.
 * @param[in] poolname The object of the type \ref os_queue_pool_t. The
 * helper macro os_queue_pool_define() helps to define this object.
 *
 * @return WM_SUCCESS if queue creation was successful
 * @return -WM_FAIL if queue creation failed
 */
int os_queue_create(os_queue_t *qhandle, const char *name, int msgsize, os_queue_pool_t *poolname);

/** Wait Forever */
#define OS_WAIT_FOREVER portMAX_DELAY
/** Do Not Wait */
#define OS_NO_WAIT 0

/** Post an item to the back of the queue.
 *
 * This function posts an item to the back of a queue. The item is queued by
 * copy, not by reference. This function can also be called from an interrupt
 * service routine.
 *
 * @param[in] qhandle Pointer to the handle of the queue
 * @param[in] msg A pointer to the item that is to be placed on the
 * queue. The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from msg
 * into the queue storage area.
 * @param[in] wait The maximum amount of time, in OS ticks, the task should
 * block waiting for space to become available on the queue, should it already
 * be full. The function os_msec_to_ticks() can be used to convert from
 * real-time to OS ticks. The special values \ref OS_WAIT_FOREVER and \ref
 * OS_NO_WAIT are provided to respectively wait infinitely or return
 * immediately.
 *
 * @return WM_SUCCESS if send operation was successful
 * @return -WM_E_INVAL if invalid parameters are passed
 * @return -WM_FAIL if send operation failed
 */
static inline int os_queue_send(os_queue_t *qhandle, const void *msg, unsigned long wait)
{
    int ret;
    signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    if (!qhandle || !(*qhandle))
        return -WM_E_INVAL;

    os_dprintf("OS: Queue Send: handle %p, msg %p, wait %d\r\n", *qhandle, msg, wait);

    if (is_isr_context())
    {
        /* This call is from Cortex-M3 handler mode, i.e. exception
         * context, hence use FromISR FreeRTOS APIs.
         */
        ret = xQueueSendToBackFromISR(*qhandle, msg, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    else
        ret = xQueueSendToBack(*qhandle, msg, wait);
    os_dprintf("OS: Queue Send: done\r\n");

    return ret == pdTRUE ? WM_SUCCESS : -WM_FAIL;
}

/** Receive an item from queue
 *
 * This function receives an item from a queue. The item is received by copy so
 * a buffer of adequate size must be provided. The number of bytes copied into
 * the buffer was defined when the queue was created.
 *
 * @param[in] qhandle Pointer to handle of the queue
 * @param[out] msg Pointer to the buffer into which the received item will
 * be copied. The size of the items in the queue was defined when the queue was
 * created. This pointer should point to a buffer as many bytes in size.
 * @param[in] wait The maximum amount of time, in OS ticks, the task should
 * block waiting for messages to arrive on the queue, should it already
 * be empty. The function os_msec_to_ticks() can be used to convert from
 * real-time to OS ticks. The special values \ref OS_WAIT_FOREVER and \ref
 * OS_NO_WAIT are provided to respectively wait infinitely or return
 * immediately.
 *
 * @return WM_SUCCESS if receive operation was successful
 * @return -WM_E_INVAL if invalid parameters are passed
 * @return -WM_FAIL if receive operation failed
 *
 * \note This function must not be used in an interrupt service routine.
 */
static inline int os_queue_recv(os_queue_t *qhandle, void *msg, unsigned long wait)
{
    int ret;
    if (!qhandle || !(*qhandle))
        return -WM_E_INVAL;

    os_dprintf("OS: Queue Receive: handle %p, msg %p, wait %d\r\n", *qhandle, msg, wait);
    ret = xQueueReceive(*qhandle, msg, wait);
    os_dprintf("OS: Queue Receive: done\r\n");
    return ret == pdTRUE ? WM_SUCCESS : -WM_FAIL;
}

/** Delete queue
 *
 * This function deletes a queue. It frees all the memory allocated for storing
 * of items placed on the queue.
 *
 * @param[in] qhandle Pointer to handle of the queue to be deleted.
 *
 * @return Currently always returns WM_SUCCESS
 */
static inline int os_queue_delete(os_queue_t *qhandle)
{
    os_dprintf("OS: Queue Delete: handle %p\r\n", *qhandle);

    vQueueDelete(*qhandle);
    // sem_debug_delete((const xSemaphoreHandle)*qhandle);
    *qhandle = NULL;

    return WM_SUCCESS;
}

/** Return the number of messages stored in queue.
 *
 * @param[in] qhandle Pointer to handle of the queue to be queried.
 *
 * @returns Number of items in the queue
 * @return -WM_E_INVAL if invalid parameters are passed
 */
static inline int os_queue_get_msgs_waiting(os_queue_t *qhandle)
{
    int nmsg = 0;
    if (!qhandle || !(*qhandle))
        return -WM_E_INVAL;
    nmsg = uxQueueMessagesWaiting(*qhandle);
    os_dprintf("OS: Queue Msg Count: handle %p, count %d\r\n", *qhandle, nmsg);
    return nmsg;
}

/* Critical Sections */
static inline unsigned long os_enter_critical_section()
{
    taskENTER_CRITICAL();
    return WM_SUCCESS;
}

static inline void os_exit_critical_section(unsigned long state)
{
    taskEXIT_CRITICAL();
}

/*** Tick function */
#define MAX_CUSTOM_HOOKS 4

extern void (*g_os_tick_hooks[MAX_CUSTOM_HOOKS])(void);
extern void (*g_os_idle_hooks[MAX_CUSTOM_HOOKS])(void);

/** Setup idle function
 *
 * This function sets up a callback function which will be called whenever the
 * system enters the idle thread context.
 *
 *  @param[in] func The callback function
 *
 *  @return WM_SUCCESS on success
 *  @return -WM_FAIL on error
 */
static inline int os_setup_idle_function(void (*func)(void))
{
    unsigned int i;

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
        if (g_os_idle_hooks[i] && g_os_idle_hooks[i] == func)
            return WM_SUCCESS;

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
    {
        if (g_os_idle_hooks[i] == NULL)
        {
            g_os_idle_hooks[i] = func;
            break;
        }
    }

    if (i == MAX_CUSTOM_HOOKS)
        return -WM_FAIL;

    return WM_SUCCESS;
}

/** Setup tick function
 *
 * This function sets up a callback function which will be called on every
 * SysTick interrupt.
 *
 *  @param[in] func The callback function
 *
 *  @return WM_SUCCESS on success
 *  @return -WM_FAIL on error
 */
static inline int os_setup_tick_function(void (*func)(void))
{
    unsigned int i;

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
        if (g_os_tick_hooks[i] && g_os_tick_hooks[i] == func)
            return WM_SUCCESS;

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
    {
        if (g_os_tick_hooks[i] == NULL)
        {
            g_os_tick_hooks[i] = func;
            break;
        }
    }

    if (i == MAX_CUSTOM_HOOKS)
        return -WM_FAIL;

    return WM_SUCCESS;
}

/** Remove idle function
 *
 *  This function removes an idle callback function that was registered
 *  previously using os_setup_idle_function().
 *
 *  @param[in] func The callback function
 *
 *  @return WM_SUCCESS on success
 *  @return -WM_FAIL on error
 */
static inline int os_remove_idle_function(void (*func)(void))
{
    unsigned int i;

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
    {
        if (g_os_idle_hooks[i] == func)
        {
            g_os_idle_hooks[i] = NULL;
            break;
        }
    }

    if (i == MAX_CUSTOM_HOOKS)
        return -WM_FAIL;

    return WM_SUCCESS;
}

/** Remove tick function
 *
 *  This function removes a tick callback function that was registered
 *  previously using os_setup_tick_function().
 *
 *  @param[in] func Callback function
 *  @return WM_SUCCESS on success
 *  @return -WM_FAIL on error
 */
static inline int os_remove_tick_function(void (*func)(void))
{
    unsigned int i;

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
    {
        if (g_os_tick_hooks[i] == func)
        {
            g_os_tick_hooks[i] = NULL;
            break;
        }
    }

    if (i == MAX_CUSTOM_HOOKS)
        return -WM_FAIL;

    return WM_SUCCESS;
}

/*** Mutex ***/
typedef xSemaphoreHandle os_mutex_t;

/** Priority Inheritance Enabled */
#define OS_MUTEX_INHERIT 1
/** Priority Inheritance Disabled */
#define OS_MUTEX_NO_INHERIT 0

static inline int os_mutex_create(os_mutex_t *mhandle, const char *name, int flags) WARN_UNUSED_RET;
/** Create mutex
 *
 * This function creates a mutex.
 *
 * @param [out] mhandle Pointer to a mutex handle
 * @param [in] name Name of the mutex
 * @param [in] flags Priority inheritance selection. Valid options are \ref
 * OS_MUTEX_INHERIT or \ref OS_MUTEX_NO_INHERIT.
 *
 * @note Currently non-inheritance in mutex is not supported.
 *
 * @return WM_SUCCESS on success
 * @return -WM_FAIL on error
 */
static inline int os_mutex_create(os_mutex_t *mhandle, const char *name, int flags)
{
    if (flags == OS_MUTEX_NO_INHERIT)
    {
        *mhandle = NULL;
        os_dprintf("Cannot create mutex for non-inheritance yet \r\n");
        return -WM_FAIL;
    }
    os_dprintf("OS: Mutex Create: name = %s \r\n", name);
    *mhandle = xSemaphoreCreateMutex();
    os_dprintf("OS: Mutex Create: handle = %p\r\n", *mhandle);
    if (*mhandle)
    {
        // sem_debug_add((const xQueueHandle)*mhandle,
        //	      name, 1);
        return WM_SUCCESS;
    }
    else
        return -WM_FAIL;
}

/** Acquire mutex
 *
 * This function acquires a mutex. Only one thread can acquire a mutex at any
 * given time. If already acquired the callers will be blocked for the specified
 * time duration.
 *
 * @param[in] mhandle Pointer to mutex handle
 * @param[in] wait The maximum amount of time, in OS ticks, the task should
 * block waiting for the mutex to be acquired. The function os_msec_to_ticks()
 * can be used to convert from real-time to OS ticks. The special values \ref
 * OS_WAIT_FOREVER and \ref OS_NO_WAIT are provided to respectively wait
 * infinitely or return immediately.
 *
 * @return WM_SUCCESS when mutex is acquired
 * @return -WM_E_INVAL if invalid parameters are passed
 * @return -WM_FAIL on failure
 */
static inline int os_mutex_get(os_mutex_t *mhandle, unsigned long wait)
{
    int ret;
    if (!mhandle || !(*mhandle))
        return -WM_E_INVAL;
    os_dprintf("OS: Mutex Get: handle %p\r\n", *mhandle);
    ret = xSemaphoreTake(*mhandle, wait);
    return ret == pdTRUE ? WM_SUCCESS : -WM_FAIL;
}

/** Release mutex
 *
 * This function releases a mutex previously acquired using os_mutex_get().
 *
 * @note The mutex should be released from the same thread context from which it
 * was acquired. If you wish to acquire and release in different contexts,
 * please use os_semaphore_get() and os_semaphore_put() variants.
 *
 * @param[in] mhandle Pointer to the mutex handle
 *
 * @return WM_SUCCESS when mutex is released
 * @return -WM_E_INVAL if invalid parameters are passed
 * @return -WM_FAIL on failure
 */
static inline int os_mutex_put(os_mutex_t *mhandle)
{
    int ret;

    if (!mhandle || !(*mhandle))
        return -WM_E_INVAL;

    os_dprintf("OS: Mutex Put: %p\r\n", *mhandle);

    ret = xSemaphoreGive(*mhandle);
    return ret == pdTRUE ? WM_SUCCESS : -WM_FAIL;
}

/**
 * Create recursive mutex
 *
 * This function creates a recursive mutex. A mutex used recursively can be
 * 'get' repeatedly by the owner. The mutex doesn't become available again
 * until the owner has called os_recursive_mutex_put() for each
 * successful 'get' request.
 *
 * @note This type of mutex uses a priority inheritance mechanism so a
 * task 'get'ing a mutex MUST ALWAYS 'put' the mutex back once no longer
 * required.
 *
 * @param[out] mhandle Pointer to a mutex handle
 * @param[in] name Name of the mutex as NULL terminated string
 *
 * @return WM_SUCCESS on success
 * @return -WM_E_INVAL on invalid parameter.
 * @return -WM_FAIL on error
 */
static inline int os_recursive_mutex_create(os_mutex_t *mhandle, const char *name)
{
    if (!mhandle)
        return -WM_E_INVAL;

    os_dprintf("OS: Recursive Mutex Create: name = %s \r\n", name);
    *mhandle = xSemaphoreCreateRecursiveMutex();
    os_dprintf("OS: Recursive Mutex Create: handle = %p\r\n", *mhandle);
    if (!*mhandle)
        return -WM_FAIL;

    // sem_debug_add(*mhandle, name, 1);
    return WM_SUCCESS;
}

/**
 * Get recursive mutex
 *
 * This function recursively obtains, or 'get's, a mutex. The mutex must
 * have previously been created using a call to
 * os_recursive_mutex_create().
 *
 * @param[in] mhandle Pointer to mutex handle obtained from
 * os_recursive_mutex_create().
 * @param[in] wait The maximum amount of time, in OS ticks, the task should
 * block waiting for the mutex to be acquired. The function os_msec_to_ticks()
 * can be used to convert from real-time to OS ticks. The special values \ref
 * OS_WAIT_FOREVER and \ref OS_NO_WAIT are provided to respectively wait
 * for portMAX_DELAY (0xffffffff) or return immediately.
 *
 * @return WM_SUCCESS when recursive mutex is acquired
 * @return -WM_FAIL on failure

 */
static inline int os_recursive_mutex_get(os_mutex_t *mhandle, unsigned long wait)
{
    os_dprintf("OS: Recursive Mutex Get: handle %p\r\n", *mhandle);
    int ret = xSemaphoreTakeRecursive(*mhandle, wait);
    return ret == pdTRUE ? WM_SUCCESS : -WM_FAIL;
}

/**
 * Put recursive mutex
 *
 * This function recursively releases, or 'give's, a mutex. The mutex must
 * have previously been created using a call to
 * os_recursive_mutex_create()
 *
 * @param[in] mhandle Pointer to the mutex handle
 *
 * @return WM_SUCCESS when mutex is released
 * @return -WM_FAIL on failure
 */
static inline int os_recursive_mutex_put(os_mutex_t *mhandle)
{
    os_dprintf("OS: Recursive Mutex Put: %p\r\n", *mhandle);
    int ret = xSemaphoreGiveRecursive(*mhandle);
    return ret == pdTRUE ? WM_SUCCESS : -WM_FAIL;
}

/** Delete mutex
 *
 * This function deletes a mutex.
 *
 * @param[in] mhandle Pointer to the mutex handle
 *
 * @note A mutex should not be deleted if other tasks are blocked on it.
 *
 * @return WM_SUCCESS on success
 */
static inline int os_mutex_delete(os_mutex_t *mhandle)
{
    vSemaphoreDelete(*mhandle);
    // sem_debug_delete((const xSemaphoreHandle)*mhandle);
    *mhandle = NULL;
    return WM_SUCCESS;
}

/*** Event Notification ***/

/**
 * Wait for task notification
 *
 * This function waits for task notification from other task or interrupt
 * context. This is similar to binary semaphore, but uses less RAM and much
 * faster than semaphore mechanism
 *
 * @param[in] wait_time Timeout specified in no. of OS ticks
 *
 * @return WM_SUCCESS when notification is successful
 * @return -WM_FAIL on failure or timeout
 */
static inline int os_event_notify_get(unsigned long wait_time)
{
    int ret = ulTaskNotifyTake(pdTRUE, wait_time);
    return ret == pdTRUE ? WM_SUCCESS : -WM_FAIL;
}

/**
 * Give task notification
 *
 * This function gives task notification so that waiting task can be
 * unblocked. This is similar to binary semaphore, but uses less RAM and much
 * faster than semaphore mechanism
 *
 * @param[in] task Task handle to be notified
 *
 * @return WM_SUCCESS when notification is successful
 * @return -WM_FAIL on failure or timeout
 */
static inline int os_event_notify_put(os_thread_t task)
{
    int ret                                       = pdTRUE;
    signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if (!task)
        return -WM_E_INVAL;
    if (is_isr_context())
    {
        /* This call is from Cortex-M3/4 handler mode, i.e. exception
         * context, hence use FromISR FreeRTOS APIs.
         */
        vTaskNotifyGiveFromISR(task, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        ret = xTaskNotifyGive(task);
    }

    return ret == pdTRUE ? WM_SUCCESS : -WM_FAIL;
}

/*** Semaphore ***/

typedef xSemaphoreHandle os_semaphore_t;

static inline int os_semaphore_create(os_semaphore_t *mhandle, const char *name) WARN_UNUSED_RET;

/** Create binary semaphore
 *
 * This function creates a binary semaphore. A binary semaphore can be acquired
 * by only one entity at a given time.
 *
 * @param[out] mhandle Pointer to a semaphore handle
 * @param[in] name Name of the semaphore
 *
 * @return WM_SUCCESS on success
 * @return -WM_FAIL on error
 */
static inline int os_semaphore_create(os_semaphore_t *mhandle, const char *name)
{
    vSemaphoreCreateBinary(*mhandle);
    if (*mhandle)
    {
        // sem_debug_add((const xSemaphoreHandle)*mhandle,
        //	      name, 1);
        return WM_SUCCESS;
    }
    else
        return -WM_FAIL;
}

/** Create counting semaphore
 *
 * This function creates a counting semaphore. A counting semaphore can be
 * acquired 'count' number of times at a given time.
 *
 * @param[out] mhandle Pointer to a semaphore handle
 * @param[in] name Name of the semaphore
 * @param[in] maxcount The maximum count value that can be reached. When
 * the semaphore reaches this value it can no longer be 'put'
 * @param[in] initcount The count value assigned to the semaphore when it
 * is created. For e.g. If '0' is passed, then os_semaphore_get() will
 * block until some other thread does an os_semaphore_put().
 *
 * @return WM_SUCCESS on success
 * @return -WM_FAIL on error
 */

static inline int os_semaphore_create_counting(os_semaphore_t *mhandle,
                                               const char *name,
                                               unsigned long maxcount,
                                               unsigned long initcount)
{
    *mhandle = xSemaphoreCreateCounting(maxcount, initcount);
    if (*mhandle)
    {
        ////sem_debug_add((const xQueueHandle)*mhandle,
        //	      name, 1);
        return WM_SUCCESS;
    }
    else
        return -WM_FAIL;
}
/** Acquire semaphore
 *
 * This function acquires a semaphore. At a given time, a binary semaphore can
 * be acquired only once, while a counting semaphore can be acquired as many as
 * 'count' number of times. Once this condition is reached, the other callers of
 * this function will be blocked for the specified time duration.
 *
 * @param[in] mhandle Pointer to a semaphore handle
 * @param[in] wait The maximum amount of time, in OS ticks, the task should
 * block waiting for the semaphore to be acquired. The function
 * os_msec_to_ticks() can be used to convert from real-time to OS ticks. The
 * special values \ref OS_WAIT_FOREVER and \ref OS_NO_WAIT are provided to
 * respectively wait infinitely or return immediately.
 *
 * @return WM_SUCCESS when semaphore is acquired
 * @return -WM_E_INVAL if invalid parameters are passed
 * @return -WM_FAIL on failure
 */
static inline int os_semaphore_get(os_semaphore_t *mhandle, unsigned long wait)
{
    int ret;
    signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    if (!mhandle || !(*mhandle))
        return -WM_E_INVAL;
    os_dprintf("OS: Semaphore Get: handle %p\r\n", *mhandle);
    if (is_isr_context())
    {
        /* This call is from Cortex-M3 handler mode, i.e. exception
         * context, hence use FromISR FreeRTOS APIs.
         */
        ret = xSemaphoreTakeFromISR(*mhandle, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    else
        ret = xSemaphoreTake(*mhandle, wait);
    return ret == pdTRUE ? WM_SUCCESS : -WM_FAIL;
}

/** Release semaphore
 *
 * This function releases a semaphore previously acquired using
 * os_semaphore_get().
 *
 * @note This function can also be called from interrupt-context.
 *
 * @param[in] mhandle Pointer to a semaphore handle
 *
 * @return WM_SUCCESS when semaphore is released
 * @return -WM_E_INVAL if invalid parameters are passed
 * @return -WM_FAIL on failure
 */
static inline int os_semaphore_put(os_semaphore_t *mhandle)
{
    int ret;
    signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    if (!mhandle || !(*mhandle))
        return -WM_E_INVAL;

    os_dprintf("OS: Semaphore Put: handle %p\r\n", *mhandle);
    if (is_isr_context())
    {
        /* This call is from Cortex-M3 handler mode, i.e. exception
         * context, hence use FromISR FreeRTOS APIs.
         */
        ret = xSemaphoreGiveFromISR(*mhandle, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    else
        ret = xSemaphoreGive(*mhandle);
    return ret == pdTRUE ? WM_SUCCESS : -WM_FAIL;
}

/** Get semaphore count
 *
 * This function returns the current value of a semaphore.
 *
 * @param[in] mhandle Pointer to a semaphore handle
 *
 * @return current value of the semaphore
 */
static inline int os_semaphore_getcount(os_semaphore_t *mhandle)
{
    os_dprintf("OS: Semaphore Get Count: handle %p\r\n", *mhandle);
    return uxQueueMessagesWaiting(*mhandle);
}

/** Delete a semaphore
 *
 * This function deletes the semaphore.
 *
 * @param [in] mhandle Pointer to a semaphore handle
 *
 * @note Do not delete a semaphore that has tasks blocked on it (tasks that are
 * in the Blocked state waiting for the semaphore to become available)
 *
 * @return WM_SUCCESS on success
 */
static inline int os_semaphore_delete(os_semaphore_t *mhandle)
{
    vSemaphoreDelete(*mhandle);
    // sem_debug_delete((const xSemaphoreHandle)*mhandle);
    *mhandle = NULL;
    return WM_SUCCESS;
}

/*
 * Reader Writer Locks
 * This is a generic implementation of reader writer locks
 * which is reader priority.
 * Not only it provides mutual exclusion but also synchronization.
 * Six APIs are exposed to user which include.
 * -# Create a reader writer lock
 * -# Delete a reader writer lock
 * -# Reader lock
 * -# Reader unlock
 * -# Writer lock
 * -# Writer unlock
 * The locking operation is timeout based.
 * Caller can give a timeout from 0 (no wait) to
 * infinite (wait forever)
 */

typedef struct _rw_lock os_rw_lock_t;
/** This is prototype of reader callback */
typedef int (*cb_fn)(os_rw_lock_t *plock, unsigned int wait_time);

struct _rw_lock
{
    /** Mutex for reader mutual exclusion */
    os_mutex_t reader_mutex;
    /** Lock which when held by reader,
     *  writer cannot enter critical section
     */
    os_semaphore_t rw_lock;
    /** Function being called when first reader gets
     *  the lock
     */
    cb_fn reader_cb;
    /** Counter to maintain number of readers
     *  in critical section
     */
    unsigned int reader_count;
};

int os_rwlock_create_with_cb(os_rw_lock_t *lock, const char *mutex_name, const char *lock_name, cb_fn r_fn);

/** Create reader-writer lock
 *
 * This function creates a reader-writer lock.
 *
 * @param[in] lock Pointer to a reader-writer lock handle
 * @param[in] mutex_name Name of the mutex
 * @param[in] lock_name Name of the lock
 *
 * @return WM_SUCCESS on success
 * @return -WM_FAIL on error
 */
int os_rwlock_create(os_rw_lock_t *lock, const char *mutex_name, const char *lock_name);

/** Delete a reader-write lock
 *
 * This function deletes a reader-writer lock.
 *
 * @param[in] lock Pointer to the reader-writer lock handle
 *
 */
void os_rwlock_delete(os_rw_lock_t *lock);

/** Acquire writer lock
 *
 * This function acquires a writer lock. While readers can acquire the lock on a
 * sharing basis, writers acquire the lock in an exclusive manner.
 *
 * @param[in] lock Pointer to the reader-writer lock handle
 * @param[in] wait_time The maximum amount of time, in OS ticks, the task should
 * block waiting for the lock to be acquired. The function os_msec_to_ticks()
 * can be used to convert from real-time to OS ticks. The special values \ref
 * OS_WAIT_FOREVER and \ref OS_NO_WAIT are provided to respectively wait
 * infinitely or return immediately.
 *
 * @return  WM_SUCCESS on success
 * @return  -WM_FAIL on error
 *
 */
int os_rwlock_write_lock(os_rw_lock_t *lock, unsigned int wait_time);

/** Release writer lock
 *
 * This function releases a writer lock previously acquired using
 * os_rwlock_write_lock().
 *
 * @param[in] lock Pointer to the reader-writer lock handle
 */
void os_rwlock_write_unlock(os_rw_lock_t *lock);

/** Acquire reader lock
 *
 * This function acquires a reader lock. While readers can acquire the lock on a
 * sharing basis, writers acquire the lock in an exclusive manner.
 *
 * @param[in] lock pointer to the reader-writer lock handle
 * @param[in] wait_time The maximum amount of time, in OS ticks, the task should
 * block waiting for the lock to be acquired. The function os_msec_to_ticks()
 * can be used to convert from real-time to OS ticks. The special values \ref
 * OS_WAIT_FOREVER and \ref OS_NO_WAIT are provided to respectively wait
 * infinitely or return immediately.
 *
 * @return  WM_SUCCESS on success
 * @return  -WM_FAIL on error
 *
 */
int os_rwlock_read_lock(os_rw_lock_t *lock, unsigned int wait_time);

/** Release reader lock
 *
 * This function releases a reader lock previously acquired using
 * os_rwlock_read_lock().
 *
 * @param[in] lock pointer to the reader-writer lock handle
 *
 * @return WM_SUCCESS if unlock operation successful.
 * @return -WM_FAIL if unlock operation failed.
 */
int os_rwlock_read_unlock(os_rw_lock_t *lock);

/*** Timer Management ***/

typedef xTimerHandle os_timer_t;
typedef os_timer_t os_timer_arg_t;
typedef portTickType os_timer_tick;

/** OS Timer reload Options
 *
 */
typedef enum os_timer_reload
{
    /**
     * Create one shot timer. Timer will be in the dormant state after
     * it expires.
     */
    OS_TIMER_ONE_SHOT,
    /**
     * Create a periodic timer. Timer will auto-reload after it expires.
     */
    OS_TIMER_PERIODIC,
} os_timer_reload_t;

/**
 * OS Timer Activate Options
 */
typedef enum os_timer_activate
{
    /** Start the timer on creation. */
    OS_TIMER_AUTO_ACTIVATE,
    /** Do not start the timer on creation. */
    OS_TIMER_NO_ACTIVATE,
} os_timer_activate_t;

/** Create timer
 *
 * This function creates a timer.
 *
 * @param[out] timer_t Pointer to the timer handle
 * @param[in] name Name of the timer
 * @param[in] ticks Period in ticks
 * @param[in] call_back Timer expire callback function
 * @param[in] cb_arg Timer callback data
 * @param[in] reload Reload Options, valid values include \ref OS_TIMER_ONE_SHOT
 * or \ref OS_TIMER_PERIODIC.
 * @param[in] activate Activate Options, valid values include \ref
 * OS_TIMER_AUTO_ACTIVATE or \ref OS_TIMER_NO_ACTIVATE
 *
 * @return WM_SUCCESS if timer created successfully
 * @return -WM_FAIL if timer creation fails
 */
int os_timer_create(os_timer_t *timer_t,
                    const char *name,
                    os_timer_tick ticks,
                    void (*call_back)(os_timer_arg_t),
                    void *cb_arg,
                    os_timer_reload_t reload,
                    os_timer_activate_t activate);

/** Activate timer
 *
 * This function activates (or starts) a timer that was previously created using
 * os_timer_create(). If the timer had already started and was already in the
 * active state, then this call is equivalent to os_timer_reset().
 *
 * @param[in] timer_t Pointer to a timer handle
 *
 * @return WM_SUCCESS if timer activated successfully
 * @return -WM_E_INVAL if invalid parameters are passed
 * @return -WM_FAIL if timer fails to activate
 */
int os_timer_activate(os_timer_t *timer_t);

/** Change timer period
 *
 * This function changes the period of a timer that was previously created using
 * os_time_create(). This function changes the period of an active or dormant
 * state timer.
 *
 * @param[in] timer_t Pointer to a timer handle
 * @param[in] ntime Time in ticks after which the timer will expire
 * @param[in] block_time  This option is currently not supported
 *
 * @return WM_SUCCESS on success
 * @return -WM_E_INVAL if invalid parameters are passed
 * @return -WM_FAIL on failure
 */
static inline int os_timer_change(os_timer_t *timer_t, os_timer_tick ntime, os_timer_tick block_time)
{
    int ret;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if (!timer_t || !(*timer_t))
        return -WM_E_INVAL;
    if (is_isr_context())
    {
        /* This call is from Cortex-M3 handler mode, i.e. exception
         * context, hence use FromISR FreeRTOS APIs.
         */
        ret = xTimerChangePeriodFromISR(*timer_t, ntime, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    else
        /* Fixme: What should be value of xBlockTime? */
        ret = xTimerChangePeriod(*timer_t, ntime, 100);
    return ret == pdPASS ? WM_SUCCESS : -WM_FAIL;
}

/** Check the timer active state
 *
 * This function checks if the timer is in the active or dormant state. A timer
 * is in the dormant state if (a) it has been created but not started, or (b) it
 * has expired and a one-shot timer.
 *
 * @param [in] timer_t Pointer to a timer handle
 *
 * @return true if timer is active
 * @return false if time is not active
 */
static inline bool os_timer_is_running(os_timer_t *timer_t)
{
    int ret;

    if (!timer_t || !(*timer_t))
        return false;

    ret = xTimerIsTimerActive(*timer_t);
    return ret == pdPASS ? true : false;
}

/**
 * Get the timer context
 *
 * This function helps to retrieve the timer context i.e. 'cb_arg' passed
 * to os_timer_create().
 *
 * @param[in] timer_t Pointer to timer handle. The timer handle is received
 * in the timer callback.
 *
 * @return The timer context i.e. the callback argument passed to
 * os_timer_create().
 */
static inline void *os_timer_get_context(os_timer_t *timer_t)
{
    if (!timer_t || !(*timer_t))
        return (void *)-1;

    return pvTimerGetTimerID(*timer_t);
}

/** Reset timer
 *
 * This function resets a timer that was previously created using using
 * os_timer_create(). If the timer had already been started and was already in
 * the active state, then this call will cause the timer to re-evaluate its
 * expiry time so that it is relative to when os_timer_reset() was called. If
 * the timer was in the dormant state then this call behaves in the same way as
 * os_timer_activate().
 *
 * @param[in] timer_t Pointer to a timer handle
 *
 * @return WM_SUCCESS on success
 * @return -WM_E_INVAL if invalid parameters are passed
 * @return -WM_FAIL on failure
 */
static inline int os_timer_reset(os_timer_t *timer_t)
{
    int ret;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if (!timer_t || !(*timer_t))
        return -WM_E_INVAL;
    /* Note:
     * XTimerStop, seconds argument is xBlockTime which means, the time,
     * in ticks, that the calling task should be held in the Blocked
     * state, until timer command succeeds.
     * We are giving as 0, to be consistent with threadx logic.
     */
    if (is_isr_context())
    {
        /* This call is from Cortex-M3 handler mode, i.e. exception
         * context, hence use FromISR FreeRTOS APIs.
         */
        ret = xTimerResetFromISR(*timer_t, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    else
        ret = xTimerReset(*timer_t, 0);
    return ret == pdPASS ? WM_SUCCESS : -WM_FAIL;
}

/** Deactivate timer
 *
 * This function deactivates (or stops) a timer that was previously started.
 *
 * @param [in] timer_t handle populated by os_timer_create()
 *
 * @return WM_SUCCESS on success
 * @return -WM_E_INVAL if invalid parameters are passed
 * @return -WM_FAIL on failure
 */
static inline int os_timer_deactivate(os_timer_t *timer_t)
{
    int ret;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if (!timer_t || !(*timer_t))
        return -WM_E_INVAL;
    /* Note:
     * XTimerStop, seconds argument is xBlockTime which means, the time,
     * in ticks, that the calling task should be held in the Blocked
     * state, until timer command succeeds.
     * We are giving as 0, to be consistent with threadx logic.
     */
    if (is_isr_context())
    {
        /* This call is from Cortex-M3 handler mode, i.e. exception
         * context, hence use FromISR FreeRTOS APIs.
         */
        ret = xTimerStopFromISR(*timer_t, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    else
        ret = xTimerStop(*timer_t, 0);
    return ret == pdPASS ? WM_SUCCESS : -WM_FAIL;
}

/** Delete timer
 *
 * This function deletes a timer.
 *
 * @param[in] timer_t Pointer to a timer handle
 *
 * @return WM_SUCCESS on success
 * @return -WM_E_INVAL if invalid parameters are passed
 * @return -WM_FAIL on failure
 */
static inline int os_timer_delete(os_timer_t *timer_t)
{
    int ret;

    if (!timer_t || !(*timer_t))
        return -WM_E_INVAL;

    /* Below timer handle invalidation needs to be protected as a context
     * switch may create issues if same handle is used before
     * invalidation.
     */
    int sta = os_enter_critical_section();
    /* Note: Block time is set as 0, thus signifying non-blocking
       API. Can be changed later if required. */
    ret      = xTimerDelete(*timer_t, 0);
    *timer_t = NULL;
    os_exit_critical_section(sta);

    return ret == pdPASS ? WM_SUCCESS : -WM_FAIL;
}

/* OS Memory allocation API's */
#ifndef CONFIG_HEAP_DEBUG

/** Allocate memory
 *
 * This function allocates memory dynamically.
 *
 *  @param[in] size Size of the memory to be allocated
 *
 * @return Pointer to the allocated memory
 * @return NULL if allocation fails
 */
#define os_mem_alloc(size) pvPortMalloc(size)

/** Allocate memory and zero it
 *
 * This function allocates memory dynamically and sets the memory contents to
 * zero.
 *
 * @param[in] size Size of the memory to be allocated
 *
 * @return Pointer to the allocated memory
 * @return NULL if allocation fails
 */
static inline void *os_mem_calloc(size_t size)
{
    void *ptr = pvPortMalloc(size);
    if (ptr)
        (void)memset(ptr, 0x00, size);

    return ptr;
}

/** Reallocate memory
 *
 * This function attempts to resize a previously allocated memory block.
 *
 *  @param[in] old_ptr Pointer to earlier allocated memory
 *  @param[in] new_size The new size
 *
 * @return Pointer to the newly resized memory block
 * @return NULL if reallocation fails
 */
#define os_mem_realloc(old_ptr, new_size) pvPortReAlloc(old_ptr, new_size)
/** Free Memory
 *
 * This function frees dynamically allocated memory using any of the dynamic
 * allocation primitives.
 *
 * @param[in] ptr Pointer to the memory to be freed
 */
#define os_mem_free(ptr) vPortFree(ptr)
#else  /* ! CONFIG_HEAP_DEBUG */
static inline void *os_mem_alloc(size_t size) WARN_UNUSED_RET;
static inline void *os_mem_calloc(size_t size) WARN_UNUSED_RET;
static inline void *os_mem_realloc(void *ptr, size_t size) WARN_UNUSED_RET;

/** This function allocates memory dynamically
 *  @param [in] size Size of memory to be allocated
 *
 *  @return Pointer to the allocated memory
 *  @return NULL if allocation fails
 */
static inline void *os_mem_alloc(size_t size)
{
    void *ptr = pvPortMalloc(size);
    if (ptr)
        (void)PRINTF("MDC:A:%x:%d\r\n", ptr, size);
    return ptr;
}
/** This function allocates memory dynamically and
 *  sets memory content to zero
 *  @param [in] size Size of memory to be allocated
 *
 *  @return Pointer to the allocated memory
 *  @return NULL if allocation fails
 */
static inline void *os_mem_calloc(size_t size)
{
    void *ptr = pvPortMalloc(size);
    if (ptr)
    {
        (void)PRINTF("MDC:A:%x:%d\r\n", ptr, size);
        (void)memset(ptr, 0x00, size);
    }

    return ptr;
}

/**This function attempts to resize the memory block pointed to by
 *  ptr that was previously allocated with a call to os_mem_alloc()
 *  or os_mem_calloc()
 * @param [in] ptr  Pointer to earlier alocated memory
 * @param [in] size New size
 *
 * @return Pointer to the newly resized memory block
 * @return NULL if reallocation fails
 */
static inline void *os_mem_realloc(void *ptr, size_t size)
{
    void *new_ptr = pvPortReAlloc(ptr, size);
    if (new_ptr)
        (void)PRINTF("MDC:R:%x:%x:%d\r\n", ptr, new_ptr, size);

    return new_ptr;
}
/** This function frees dynamically allocated memory
 *  @param [in] ptr Pointer to memory to be freed
 */
static inline void os_mem_free(void *ptr)
{
    vPortFree(ptr);
    (void)PRINTF("MDC:F:%x\r\n", ptr);
}
#endif /* CONFIG_HEAP_DEBUG */

/** This function dumps complete statistics
 *  of the heap memory.
 */
static inline void os_dump_mem_stats(void)
{
    unsigned sta = os_enter_critical_section();
    //#ifdef FREERTOS_ENABLE_MALLOC_STATS
    HeapStats_t HS;
    vPortGetHeapStats(&HS);

    (void)PRINTF("\n\r");
    (void)PRINTF("Heap size ---------------------- : %d\n\r", HS.xAvailableHeapSpaceInBytes);
    (void)PRINTF("Largest Free Block size ---------: %d\n\r", HS.xSizeOfLargestFreeBlockInBytes);
    (void)PRINTF("Smallest Free Block size ------- : %d\n\r", HS.xSizeOfSmallestFreeBlockInBytes);
    (void)PRINTF("Number of Free Blocks ---------- : %d\n\r", HS.xNumberOfFreeBlocks);
    (void)PRINTF("Total successful allocations --- : %d\n\r", HS.xNumberOfSuccessfulAllocations);
    (void)PRINTF("Total successful frees --------- : %d\n\r", HS.xNumberOfSuccessfulFrees);
    (void)PRINTF("Min Free since system boot ----- : %d\n\r", HS.xMinimumEverFreeBytesRemaining);
    //#endif /* FREERTOS_ENABLE_MALLOC_STATS */
    os_exit_critical_section(sta);
}

#if 0
/** This function returns the size of biggest free block
 *  available in heap.
 */
static inline size_t os_mem_get_free_size(void)
{
	unsigned sta = os_enter_critical_section();

	const heapAllocatorInfo_t *hI = getheapAllocInfo();

	os_exit_critical_section(sta);

	return hI->biggestFreeBlockAvailable;
}

/*** OS thread system information ***/

void os_dump_threadinfo(char *name);
void os_thread_stackmark(char *name);

/* Return -WM_FAIL is stats is not enabled */
static inline int os_free_heap_percentage()
{
#ifdef FREERTOS_ENABLE_MALLOC_STATS
	const heapAllocatorInfo_t *hI = getheapAllocInfo();
	int temp = 0;
	temp = (hI->heapSize - hI->freeSize);
	temp = (temp * 100) / hI->heapSize;
	return temp;
#else
	return -WM_FAIL;
#endif /* FREERTOS_ENABLE_MALLOC_STATS */
}

/* Return -WM_FAIL if stats is not enabled */
static inline int os_get_free_size()
{
#ifdef FREERTOS_ENABLE_MALLOC_STATS
	const heapAllocatorInfo_t *hI = getheapAllocInfo();
	return (int)hI->freeSize;
#else
	return -WM_FAIL;
#endif /* FREERTOS_ENABLE_MALLOC_STATS */
}

/* Return -WM_FAIL is stats is not enabled */
static inline int os_get_heap_size()
{
#ifdef FREERTOS_ENABLE_MALLOC_STATS
	const heapAllocatorInfo_t *hI = getheapAllocInfo();
	return (int)hI->heapSize;
#else
	return -WM_FAIL;
#endif /* FREERTOS_ENABLE_MALLOC_STATS */
}

/* This function updates the global tick count
  *  When  MC200 core enters a low power state
  *  system tick counter stops generating interrupt.
  *  Due to this global time count does not get updated.
  *  This API is used to update the value by number
  *  passed as a parameter.
  *  @param [in] ticks : The amount by which global
  *                     tick count needs to be updated
  */
static inline void os_update_tick_count(unsigned long ticks)
{
	xTaskUpdateTickCount(ticks);
}
#endif

typedef unsigned int event_group_handle_t;

typedef enum flag_rtrv_option_t_
{
    EF_AND,
    EF_AND_CLEAR,
    EF_OR,
    EF_OR_CLEAR
} flag_rtrv_option_t;

#define EF_NO_WAIT      0
#define EF_WAIT_FOREVER 0xFFFFFFFFUL
#define EF_NO_EVENTS    0x7

int os_event_flags_create(event_group_handle_t *hnd);
int os_event_flags_get(event_group_handle_t hnd,
                       unsigned requested_flags,
                       flag_rtrv_option_t option,
                       unsigned *actual_flags_ptr,
                       unsigned wait_option);
int os_event_flags_set(event_group_handle_t hnd, unsigned flags_to_set, flag_rtrv_option_t option);

/**** OS init call **********/
WEAK int os_init();

void _os_delay(int cnt);

/**
 * \def os_get_runtime_stats(__buff__)
 *
 * Get ASCII formatted run time statistics
 *
 * Please ensure that your buffer is big enough for the formatted data to
 * fit. Failing to do this may cause memory data corruption.
 */
#define os_get_runtime_stats(__buff__) vTaskGetRunTimeStats(__buff__)

/** Disables all interrupts at NVIC level */
static inline void os_disable_all_interrupts()
{
    taskDISABLE_INTERRUPTS();
}

/** Enable all interrupts at NVIC lebel */
static inline void os_enable_all_interrupts()
{
    taskENABLE_INTERRUPTS();
}

unsigned int os_get_timestamp();

#endif /* ! _WM_OS_H_ */
