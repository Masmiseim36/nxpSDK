/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_sdmmc_osa.h"

/*******************************************************************************
 * Definitons
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
/*!
 * brief Initialize OSA.
 */
void SDMMC_OSAInit(void)
{
    /* Intentional empty */
}

/*!
 * brief OSA Create event.
 * param event handle.
 * retval kStatus_Fail or kStatus_Success.
 */
status_t SDMMC_OSAEventCreate(void *eventHandle)
{
    assert(eventHandle != NULL);

    return OSA_EventCreate(eventHandle, true);
}

/*!
 * brief Wait event.
 *
 * param eventHandle The event type
 * param eventType Timeout time in milliseconds.
 * param timeoutMilliseconds timeout value in ms.
 * param event event flags.
 * retval kStatus_Fail or kStatus_Success.
 */
status_t SDMMC_OSAEventWait(void *eventHandle, uint32_t eventType, uint32_t timeoutMilliseconds, uint32_t *event)
{
    assert(eventHandle != NULL);

    osa_status_t status = KOSA_StatusError;

    while (1)
    {
        status = OSA_EventWait(eventHandle, eventType, 0, timeoutMilliseconds, event);
        if ((KOSA_StatusSuccess == status) || (KOSA_StatusTimeout == status))
        {
            break;
        }
    }

    if (KOSA_StatusSuccess == status)
    {
        return kStatus_Success;
    }

    return kStatus_Fail;
}

/*!
 * brief set event.
 * param event event handle.
 * param eventType The event type
 * retval kStatus_Fail or kStatus_Success.
 */
status_t SDMMC_OSAEventSet(void *eventHandle, uint32_t eventType)
{
    assert(eventHandle != NULL);

    OSA_EventSet(eventHandle, eventType);

    return kStatus_Success;
}

/*!
 * brief Get event flag.
 * param eventHandle event handle.
 * param eventType The event type
 * param flag pointer to store event value.
 * retval kStatus_Fail or kStatus_Success.
 */
status_t SDMMC_OSAEventGet(void *eventHandle, uint32_t eventType, uint32_t *flag)
{
    assert(eventHandle != NULL);
    assert(flag != NULL);

    OSA_EventGet(eventHandle, eventType, flag);

    return kStatus_Success;
}

/*!
 * brief clear event flag.
 * param eventHandle event handle.
 * param eventType The event type
 * retval kStatus_Fail or kStatus_Success.
 */
status_t SDMMC_OSAEventClear(void *eventHandle, uint32_t eventType)
{
    assert(eventHandle != NULL);

    OSA_EventClear(eventHandle, eventType);

    return kStatus_Success;
}

/*!
 * brief Delete event.
 * param event The event handle.
 */
status_t SDMMC_OSAEventDestroy(void *eventHandle)
{
    OSA_EventDestroy(eventHandle);

    return kStatus_Success;
}

/*!
 * brief sdmmc delay.
 * param milliseconds time to delay
 */
void SDMMC_OSADelay(uint32_t milliseconds)
{
#if (defined FSL_OSA_BM_TIMER_CONFIG) && (FSL_OSA_BM_TIMER_CONFIG == FSL_OSA_BM_TIMER_NONE)
    SDK_DelayAtLeastUs(milliseconds * 1000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#else
    OSA_TimeDelay(milliseconds);
#endif
}
