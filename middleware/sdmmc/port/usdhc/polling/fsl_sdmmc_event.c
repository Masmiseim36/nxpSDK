/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_sdmmc_event.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get event instance.
 * @param eventType The event type
 * @return The event instance's pointer.
 */
static volatile uint32_t *SDMMCEVENT_GetInstance(sdmmc_event_t eventType);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Card detect event. */
static volatile uint32_t g_eventCardDetect;

/*! @brief transfer complete event. */
static volatile uint32_t g_eventTransferComplete;

/*! @brief Time variable unites as milliseconds. */
volatile uint32_t g_eventTimeMilliseconds;

/*******************************************************************************
 * Code
 ******************************************************************************/
void SysTick_Handler(void)
{
#ifdef __CA7_REV
    SystemClearSystickFlag();
#endif
    g_eventTimeMilliseconds++;
}

void SDMMCEVENT_InitTimer(void)
{
#ifdef __CA7_REV
    /* special for i.mx6ul */
    SystemSetupSystick(1000U, (void *)SysTick_Handler, 32U);
    SystemClearSystickFlag();
#elif defined MIMXRT1176_cm7_SERIES || MIMXRT1176_cm4_SERIES
#if __CORTEX_M == 7
    SysTick_Config(CLOCK_GetM7Freq() / 1000); // CLOCK_GetRootClockFreq(kCLOCK_Root_M7_Systick) / 1000U);
#else
    SysTick_Config(CLOCK_GetM4Freq() / 1000U);
#endif
#else
    /* Set systick reload value to generate 1ms interrupt */
    SysTick_Config(CLOCK_GetFreq(kCLOCK_CoreSysClk) / 1000U);
#endif
}

static volatile uint32_t *SDMMCEVENT_GetInstance(sdmmc_event_t eventType)
{
    volatile uint32_t *event;

    switch (eventType)
    {
        case kSDMMCEVENT_TransferComplete:
            event = &g_eventTransferComplete;
            break;
        case kSDMMCEVENT_CardDetect:
            event = &g_eventCardDetect;
            break;
        default:
            event = NULL;
            break;
    }

    return event;
}

bool SDMMCEVENT_Create(sdmmc_event_t eventType)
{
    volatile uint32_t *event = SDMMCEVENT_GetInstance(eventType);

    if (event)
    {
        *event = 0;
        return true;
    }
    else
    {
        return false;
    }
}

bool SDMMCEVENT_Wait(sdmmc_event_t eventType, uint32_t timeoutMilliseconds)
{
    uint32_t startTime;
    uint32_t elapsedTime;

    volatile uint32_t *event = SDMMCEVENT_GetInstance(eventType);

    if (timeoutMilliseconds && event)
    {
        startTime = g_eventTimeMilliseconds;
        do
        {
            elapsedTime = (g_eventTimeMilliseconds - startTime);
        } while ((*event == 0U) && (elapsedTime < timeoutMilliseconds));
        *event = 0U;

        return ((elapsedTime < timeoutMilliseconds) ? true : false);
    }
    else
    {
        return false;
    }
}

bool SDMMCEVENT_Notify(sdmmc_event_t eventType)
{
    volatile uint32_t *event = SDMMCEVENT_GetInstance(eventType);

    if (event)
    {
        *event = 1U;
        return true;
    }
    else
    {
        return false;
    }
}

void SDMMCEVENT_Delete(sdmmc_event_t eventType)
{
    volatile uint32_t *event = SDMMCEVENT_GetInstance(eventType);

    if (event)
    {
        *event = 0U;
    }
}

void SDMMCEVENT_Delay(uint32_t milliseconds)
{
    uint32_t startTime = g_eventTimeMilliseconds;
    uint32_t periodTime = 0;
    while (periodTime < milliseconds)
    {
        periodTime = g_eventTimeMilliseconds - startTime;
    }
}
