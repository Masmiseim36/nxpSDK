/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

/*
 * Copyright 2022 NXP
 */

#include <osal.h>
#include <assert.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define RT_TASK_PRIORITY     (configMAX_PRIORITIES - 1U)
#define NORMAL_TASK_PRIORITY (configMAX_PRIORITIES - 2U)
#define LOW_TASK_PRIORITY    (1)

#define USECS_PER_SEC  (1000000UL)
#define USECS_PER_TICK (USECS_PER_SEC / configTICK_RATE_HZ)

void udelay(uint32_t us)
{
    TickType_t ticks = (us / USECS_PER_TICK) + 1;
    vTaskDelay(ticks);
}

int gettimeofday(struct timeval *tp, void *tzp)
{
    TickType_t tick = xTaskGetTickCount();
    TickType_t ticks_left;

    assert(tp != NULL);

    tp->tv_sec = tick / configTICK_RATE_HZ;

    ticks_left  = tick % configTICK_RATE_HZ;
    tp->tv_usec = ticks_left * USECS_PER_TICK;
    assert(tp->tv_usec < USECS_PER_SEC);

    return 0;
}

int osal_usleep(uint32 usec)
{
    udelay(usec);
    return 0;
}

ec_timet osal_current_time(void)
{
    struct timeval current_time;
    ec_timet return_value;

    gettimeofday(&current_time, 0);
    return_value.sec  = current_time.tv_sec;
    return_value.usec = current_time.tv_usec;
    return return_value;
}

void osal_timer_start(osal_timert *self, uint32 timeout_usec)
{
    struct timeval start_time;
    struct timeval timeout;
    struct timeval stop_time;

    gettimeofday(&start_time, 0);
    timeout.tv_sec  = timeout_usec / USECS_PER_SEC;
    timeout.tv_usec = timeout_usec % USECS_PER_SEC;
    timeradd(&start_time, &timeout, &stop_time);

    self->stop_time.sec  = stop_time.tv_sec;
    self->stop_time.usec = stop_time.tv_usec;
}

boolean osal_timer_is_expired(osal_timert *self)
{
    struct timeval current_time;
    struct timeval stop_time;

    gettimeofday(&current_time, 0);
    stop_time.tv_sec   = self->stop_time.sec;
    stop_time.tv_usec  = self->stop_time.usec;

    return timercmp(&current_time, &stop_time, >=);
}

#if (configSUPPORT_DYNAMIC_ALLOCATION)

int osal_thread_create(void *thandle, int stacksize, void *func, void *param)
{
    BaseType_t xReturned;

    xReturned = xTaskCreate((TaskFunction_t)func, "worker",
                            (configSTACK_DEPTH_TYPE)stacksize, param,
                            NORMAL_TASK_PRIORITY, thandle);
    if (xReturned != pdPASS)
    {
        return 0;
    }
    return 1;
}

mtx_t osal_mutex_create(void)
{
    SemaphoreHandle_t xSemaphore;

    xSemaphore = xSemaphoreCreateMutex();
    configASSERT(xSemaphore);

    return (mtx_t)xSemaphore;
}

#elif defined(configSUPPORT_STATIC_ALLOCATION)

static StackType_t osalStackBuffer[MAX_TASK][MAX_TASK_STACK];
static StaticTask_t osalTaskBuffer[MAX_TASK];
static int osalTaskIndex = 0;

int osal_thread_create(void *thandle, int stacksize, void *func, void *param)
{
    TaskHandle_t xReturned;

    configASSERT(stacksize <= MAX_TASK_STACK);
    configASSERT(osalTaskIndex < MAX_TASK);

    xReturned = xTaskCreateStatic((TaskFunction_t)func, "worker",
                                  MAX_TASK_STACK, param,
                                  NORMAL_TASK_PRIORITY,
                                  (StackType_t *)&osalStackBuffer[osalTaskIndex],
                                  &osalTaskBuffer[osalTaskIndex]);
    if (xReturned == NULL)
    {
        return 0;
    }

    osalTaskIndex++;

    return 1;
}

static StaticSemaphore_t osalMutexBuffer[MAX_MUTE];
static int osalMuteIndex = 0;

mtx_t osal_mutex_create(void)
{
    SemaphoreHandle_t xSemaphore = NULL;

    configASSERT(osalMuteIndex < MAX_MUTE);
    xSemaphore = xSemaphoreCreateMutexStatic(&osalMutexBuffer[osalMuteIndex]);
    configASSERT(xSemaphore);

    osalMuteIndex++;

    return (mtx_t)xSemaphore;
}

#endif
