/*
 * Copyright 2022 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <FreeRTOS.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "mpp_config.h"
#include "mpp_api_types.h"
#include "hal_freertos.h"
#include "hal_debug.h"

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

uint32_t hal_get_exec_time()
{
    TaskStatus_t taskStatus[HAL_MAX_TASKS] = {0};
    configRUN_TIME_COUNTER_TYPE runtime;
    TaskHandle_t cur_task = xTaskGetCurrentTaskHandle();
    uint32_t runtime_ms, tasks_time = 0;

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
