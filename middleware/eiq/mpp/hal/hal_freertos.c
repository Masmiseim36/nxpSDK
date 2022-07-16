/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <FreeRTOS.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "mpp_config.h"
#include "mpp_api_types.h"
#include "hal_freertos.h"
#include "hal_debug.h"

/**
 * hal mutex handling:
 *  create, lock, unlock and remove
 */

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
