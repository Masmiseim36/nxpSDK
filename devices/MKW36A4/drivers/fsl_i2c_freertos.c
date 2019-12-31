/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_i2c_freertos.h"

static void I2C_RTOS_Callback(I2C_Type *base, i2c_master_handle_t *drv_handle, status_t status, void *userData)
{
    i2c_rtos_handle_t *handle = (i2c_rtos_handle_t *)userData;
    BaseType_t reschedule;
    handle->async_status = status;
    xSemaphoreGiveFromISR(handle->semaphore, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

status_t I2C_RTOS_Init(i2c_rtos_handle_t *handle,
                       I2C_Type *base,
                       const i2c_master_config_t *masterConfig,
                       uint32_t srcClock_Hz)
{
    if (handle == NULL)
    {
        return kStatus_InvalidArgument;
    }

    if (base == NULL)
    {
        return kStatus_InvalidArgument;
    }

    memset(handle, 0, sizeof(i2c_rtos_handle_t));
#if (configSUPPORT_STATIC_ALLOCATION == 1)
    handle->mutex = xSemaphoreCreateMutexStatic(&handle->mutexBuffer);
#else
    handle->mutex = xSemaphoreCreateMutex();
#endif
    if (handle->mutex == NULL)
    {
        return kStatus_Fail;
    }
#if (configSUPPORT_STATIC_ALLOCATION == 1)
    handle->semaphore = xSemaphoreCreateBinaryStatic(&handle->semaphoreBuffer);
#else
    handle->semaphore = xSemaphoreCreateBinary();
#endif
    if (handle->semaphore == NULL)
    {
        vSemaphoreDelete(handle->mutex);
        return kStatus_Fail;
    }

    handle->base = base;

    I2C_MasterInit(handle->base, masterConfig, srcClock_Hz);
    I2C_MasterTransferCreateHandle(base, &handle->drv_handle, I2C_RTOS_Callback, (void *)handle);

    return kStatus_Success;
}

status_t I2C_RTOS_Deinit(i2c_rtos_handle_t *handle)
{
    I2C_MasterDeinit(handle->base);

    vSemaphoreDelete(handle->semaphore);
    vSemaphoreDelete(handle->mutex);

    return kStatus_Success;
}

status_t I2C_RTOS_Transfer(i2c_rtos_handle_t *handle, i2c_master_transfer_t *transfer)
{
    status_t status;

    /* Lock resource mutex */
    if (xSemaphoreTake(handle->mutex, portMAX_DELAY) != pdTRUE)
    {
        return kStatus_I2C_Busy;
    }

    status = I2C_MasterTransferNonBlocking(handle->base, &handle->drv_handle, transfer);
    if (status != kStatus_Success)
    {
        xSemaphoreGive(handle->mutex);
        return status;
    }

    /* Wait for transfer to finish */
    xSemaphoreTake(handle->semaphore, portMAX_DELAY);

    /* Unlock resource mutex */
    xSemaphoreGive(handle->mutex);

    /* Return status captured by callback function */
    return handle->async_status;
}
