/*
 * Copyright 2022-2024 NXP.
 * All rights reserved.
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <FreeRTOS.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "atomic.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

#include "mpp_config.h"
#include "mpp_api_types.h"
#include "hal_os.h"
#include "hal_freertos.h"
#include "hal_debug.h"

#define CASSERT(predicate, file) _impl_CASSERT_LINE(predicate,__LINE__,file)
#define _impl_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
    typedef char _impl_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];

void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
    HAL_LOGE("Stack overflow detected in task %s\n",pcTaskName);
    while(true);
}

void vApplicationMallocFailedHook()
{
    HAL_LOGE("Malloc failure detected.\n");
    while(true);
}

/**
 * hal mutex handling:
 *  create, lock, unlock and remove
 */

#define HAL_MAX_MUTEX_TIME_MS (HAL_MUTEX_TIMEOUT_MS / portTICK_PERIOD_MS)

int hal_mutex_create (hal_mutex_t *mutex)
{
    volatile int ret = MPP_ERROR;

    do {
        if (mutex == NULL) {
            HAL_LOGE("%s: Invalid mutex pointer\n", __func__);
            ret = MPP_INVALID_MUTEX;
            break;
        }

        *mutex = xSemaphoreCreateMutex();

        if (*mutex == NULL) {
            HAL_LOGE("%s: Failed to create mutex\n", __func__);
            ret = MPP_ERR_ALLOC_MUTEX;
        } else {
            ret = MPP_SUCCESS;
        }

    } while (false);

    return ret;
}

void hal_mutex_remove (hal_mutex_t mutex)
{
    vSemaphoreDelete(mutex);
}

int hal_mutex_lock (hal_mutex_t mutex)
{
    volatile int ret = MPP_ERROR;

    do {
        if (mutex == NULL) {
            HAL_LOGE("%s: Invalid mutex\n", __func__);
            ret = MPP_INVALID_MUTEX;
            break;
        }
        if (xSemaphoreTake (mutex, HAL_MAX_MUTEX_TIME_MS) != pdTRUE) {
            HAL_LOGE("%s: Mutex timed out\n", __func__);
            ret = MPP_MUTEX_TIMEOUT;
            break;
        }
        ret = MPP_SUCCESS;
    } while (false);

    return ret;
}

int hal_mutex_unlock (hal_mutex_t mutex)
{
    volatile int ret = MPP_ERROR;

    do {
        if (mutex == NULL) {
            HAL_LOGE("%s: Invalid mutex\n", __func__);
            ret = MPP_INVALID_MUTEX;
            break;
        }
        if (xSemaphoreGive (mutex) != pdTRUE) {
            HAL_LOGE("%s: Mutex give error\n", __func__);
            ret = MPP_MUTEX_ERROR;
            break;
        }
        ret = MPP_SUCCESS;
    } while (false);

    return ret;
}

/* Warning: FreeRTOS requires enabling configGENERATE_RUN_TIME_STATS */
uint32_t hal_get_exec_time()
{
    TaskStatus_t taskStatus[HAL_MAX_TASKS] = {0};
    configRUN_TIME_COUNTER_TYPE runtime = 0;
    TaskHandle_t cur_task = xTaskGetCurrentTaskHandle();
    uint32_t runtime_ms = 0, tasks_time = 0;

    uxTaskGetSystemState(taskStatus, HAL_MAX_TASKS, &runtime);
    for(int i= 0; i < HAL_MAX_TASKS; i++)
    {
        if (taskStatus[i].xHandle == NULL) break;
        if (taskStatus[i].xHandle == cur_task) continue;
        /* accumulate task exec time in ms to avoid overflow */
        tasks_time += taskStatus[i].ulRunTimeCounter * HAL_EXEC_TIMER_US / 1000;
    }
    /* convert to ms */
    runtime_ms = runtime * HAL_EXEC_TIMER_US / 1000;

    return runtime_ms - tasks_time;
}

void *hal_malloc(uint32_t size)
{
    return pvPortMalloc(size);
}

void hal_free(void *pointer)
{
    vPortFree(pointer);
}

hal_sema_t hal_sema_create()
{
    /* not implemented */
    return NULL;
}

hal_sema_t hal_sema_create_binary()
{
    return xSemaphoreCreateBinary();
}

bool hal_sema_give(hal_sema_t handle)
{
    SemaphoreHandle_t h = handle;

    BaseType_t ret = xSemaphoreGive(h);
    if (ret == pdFALSE) return false;   /* error */

    return true;
}

bool hal_sema_take(hal_sema_t handle, uint32_t timeout)
{
    SemaphoreHandle_t h = handle;

    BaseType_t ret = xSemaphoreTake(h, timeout);
    if (ret == pdFALSE) return false;   /* error */

    return true;
}

bool hal_sema_give_isr(hal_sema_t handle, long int * const p_higher_prio)
{
    SemaphoreHandle_t h = handle;

    BaseType_t ret = xSemaphoreGiveFromISR(h, p_higher_prio);
    if (ret == pdFALSE) return false;   /* error */

    return true;
}

bool hal_sema_take_isr(hal_sema_t handle, long int * const p_higher_prio)
{
    SemaphoreHandle_t h = handle;

    BaseType_t ret = xSemaphoreTakeFromISR(h, p_higher_prio);
    if (ret == pdFALSE) return false;   /* error */

    return true;
}

void hal_sched_yield(long HigherPriorityTaskWoken)
{
    portYIELD_FROM_ISR(HigherPriorityTaskWoken);
}

uint32_t hal_get_ostick()
{
    return xTaskGetTickCount();
}

uint32_t hal_get_tick_period_ms()
{
    return portTICK_PERIOD_MS;
}

uint32_t hal_get_tick_rate_hz()
{
    return configTICK_RATE_HZ;
}

UBaseType_t uxCriticalSectionType;
void hal_atomic_enter()
{
    uxCriticalSectionType = portSET_INTERRUPT_MASK_FROM_ISR();
}

void hal_atomic_exit()
{
    portCLEAR_INTERRUPT_MASK_FROM_ISR( uxCriticalSectionType );
}

uint32_t hal_tick_to_ms(uint32_t os_tick)
{
    return (os_tick * TICK_PERIOD_MS);
}

int hal_task_create( hal_task_fct_t fct,
                            const char * const name,
                            const uint16_t stackdepth,
                            void * const pparams,
                            uint32_t prio,
                            hal_task_t * const ptask )
{
    BaseType_t ret = pdFAIL;
    ret = xTaskCreate(fct,
                      name,
                      stackdepth,
                      pparams,
                      prio,
                      (TaskHandle_t * const ) ptask);
    if (pdFAIL == ret)
        return MPP_ERROR;
    else
        return MPP_SUCCESS;
}

void hal_task_suspend(hal_task_t task)
{
    vTaskSuspend((TaskHandle_t) task);
}

void hal_task_resume(hal_task_t task)
{
    vTaskResume((TaskHandle_t) task);
}

void hal_task_delay(uint32_t ticks)
{
    vTaskDelay(ticks);
}

hal_event_group_t hal_eventgrp_create()
{
    return (hal_event_group_t) xEventGroupCreate();
}

hal_eventbits_t hal_eventgrp_set_bits( hal_event_group_t eventgrp,
                                    const hal_eventbits_t bitmask )
{
    return xEventGroupSetBits((EventGroupHandle_t) eventgrp, bitmask);
}

hal_eventbits_t hal_eventgrp_wait_bits( hal_event_group_t eventgrp,
                                        const hal_eventbits_t bitmask,
                                        const uint32_t bClearOnExit,
                                        const uint32_t bWaitForAllBits,
                                        uint32_t tickstowait )
{
    return xEventGroupWaitBits( (EventGroupHandle_t) eventgrp,
                                bitmask,
                                bClearOnExit,
                                bWaitForAllBits,
                                tickstowait);
}

int hal_get_max_syscall_prio()
{
    return configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
}

int hal_get_os_max_prio()
{
	return configMAX_PRIORITIES;
}
