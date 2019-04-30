/*
 * Copyright (c) 2017, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <FreeRTOS.h>
#include <semphr.h>

#include "srtm_sem.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
srtm_sem_t SRTM_Sem_Create(uint32_t maxCount, uint32_t initCount)
{
    return xSemaphoreCreateCounting(maxCount, initCount);
}

void SRTM_Sem_Destroy(srtm_sem_t sem)
{
    assert(sem);

    vSemaphoreDelete(sem);
}

srtm_status_t SRTM_Sem_Post(srtm_sem_t sem)
{
    portBASE_TYPE taskToWake = pdFALSE;

    if (__get_IPSR())
    {
        if (xSemaphoreGiveFromISR(sem, &taskToWake) == pdPASS)
        {
            portYIELD_FROM_ISR(taskToWake);
            return SRTM_Status_Success;
        }
    }
    else
    {
        if (xSemaphoreGive(sem) == pdTRUE)
        {
            return SRTM_Status_Success;
        }
    }

    return SRTM_Status_Error;
}

srtm_status_t SRTM_Sem_Wait(srtm_sem_t sem, uint32_t timeout)
{
    uint32_t ticks;

    if (timeout == SRTM_WAIT_FOR_EVER)
    {
        ticks = portMAX_DELAY;
    }
    else if (timeout == SRTM_NO_WAIT)
    {
        ticks = 0U;
    }
    else
    {
        ticks = ((uint32_t)configTICK_RATE_HZ * (uint32_t)(timeout - 1U)) / 1000U + 1U;
    }

    if (xSemaphoreTake(sem, ticks) == pdFALSE)
    {
        return SRTM_Status_Timeout;
    }

    return SRTM_Status_Success;
}
