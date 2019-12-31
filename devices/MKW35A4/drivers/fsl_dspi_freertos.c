/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_dspi_freertos.h"

static void DSPI_RTOS_Callback(SPI_Type *base, dspi_master_handle_t *drv_handle, status_t status, void *userData)
{
    dspi_rtos_handle_t *handle = (dspi_rtos_handle_t *)userData;
    BaseType_t reschedule;

    xSemaphoreGiveFromISR(handle->event, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

status_t DSPI_RTOS_Init(dspi_rtos_handle_t *handle,
                        SPI_Type *base,
                        const dspi_master_config_t *masterConfig,
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

    memset(handle, 0, sizeof(dspi_rtos_handle_t));

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
    handle->event = xSemaphoreCreateBinaryStatic(&handle->semaphoreBuffer);
#else
    handle->event = xSemaphoreCreateBinary();
#endif
    if (handle->event == NULL)
    {
        vSemaphoreDelete(handle->mutex);
        return kStatus_Fail;
    }

    handle->base = base;

    DSPI_MasterInit(handle->base, masterConfig, srcClock_Hz);
    DSPI_MasterTransferCreateHandle(handle->base, &handle->drv_handle, DSPI_RTOS_Callback, (void *)handle);

    return kStatus_Success;
}

status_t DSPI_RTOS_Deinit(dspi_rtos_handle_t *handle)
{
    DSPI_Deinit(handle->base);
    vSemaphoreDelete(handle->event);
    vSemaphoreDelete(handle->mutex);

    return kStatus_Success;
}

status_t DSPI_RTOS_Transfer(dspi_rtos_handle_t *handle, dspi_transfer_t *transfer)
{
    status_t status;

    /* Lock resource mutex */
    if (xSemaphoreTake(handle->mutex, portMAX_DELAY) != pdTRUE)
    {
        return kStatus_DSPI_Busy;
    }

    status = DSPI_MasterTransferNonBlocking(handle->base, &handle->drv_handle, transfer);
    if (status != kStatus_Success)
    {
        xSemaphoreGive(handle->mutex);
        return status;
    }

    /* Wait for transfer to finish */
    xSemaphoreTake(handle->event, portMAX_DELAY);

    /* Unlock resource mutex */
    xSemaphoreGive(handle->mutex);

    /* Return status captured by callback function */
    return handle->async_status;
}
