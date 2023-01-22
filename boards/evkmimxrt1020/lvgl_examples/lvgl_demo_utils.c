/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "lvgl_demo_utils.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t s_cpuClockFreqHz;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize the timer to get microsecond. */
void DEMO_InitUsTimer(void)
{
    s_cpuClockFreqHz = SystemCoreClock;
#if defined(DWT)
    MSDK_EnableCpuCycleCounter();
#endif
}

/* Get current time in microsecond. */
uint32_t DEMO_GetUsTimer(void)
{
#if defined(DWT)
    uint32_t count = MSDK_GetCpuCycleCount();

    /*
     * Because core clock is faster than 1MHz, so after converted to microsecond,
     * the value is always smaller than UINT32_MAX.
     */
    return (uint32_t)COUNT_TO_USEC(count, s_cpuClockFreqHz);
#else
    return 0;
#endif
}

uint32_t DEMO_GetCpuClockFreq(void)
{
    return s_cpuClockFreqHz;
}
