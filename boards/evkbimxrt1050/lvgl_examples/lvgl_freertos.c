/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "lvgl_freertos.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl_demo_utils.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if (INCLUDE_xTaskGetIdleTaskHandle == 1)
#define OS_IsInIdleTask (xTaskGetCurrentTaskHandle() == xTaskGetIdleTaskHandle())
#elif (INCLUDE_xTaskGetCurrentTaskHandle == 1)
#define OS_IsInIdleTask (strcmp(pcTaskGetName(xTaskGetCurrentTaskHandle()), "IDLE") == 0)
#else
#error Can check whether current task is idle task, check the FreeRTOS_Config.h
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if DEMO_MEASURE_IDLE_TIME
static volatile uint32_t idle_start;
static volatile uint64_t idle_cnt; /* Count of CPU cycles spent in idle task. */
#endif

/*******************************************************************************
 * Prototype
 ******************************************************************************/
#if DEMO_MEASURE_IDLE_TIME
static void traceIdleTaskSwitchedIn(void);
static void traceIdleTaskSwitchedOut(void);
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

#if DEMO_MEASURE_IDLE_TIME
static void traceIdleTaskSwitchedIn(void)
{
    idle_start = MSDK_GetCpuCycleCount();
}

static void traceIdleTaskSwitchedOut(void)
{
    uint32_t idle_stop = MSDK_GetCpuCycleCount();

    /* If there is no overflow, simply count the delta time. */
    if (idle_stop >= idle_start)
    {
        idle_cnt += (idle_stop - idle_start);
    }
    else
    {
        /*
         * We assume that overflow will not happen more that once as the
         * idle task switch out shall happen in 4.3sec (in case of 1GHz CPU).
         */
        idle_cnt += ((uint64_t)UINT32_MAX + 1ULL + idle_stop - idle_start);
    }
}
#endif

uint64_t getIdleTaskTime(void)
{
#if DEMO_MEASURE_IDLE_TIME
    /* Convert count to microsecond. */
    return (idle_cnt * 1000000U) / DEMO_GetCpuClockFreq();
#else
    return 0ULL;
#endif
}

void resetIdleTaskTime(void)
{
#if DEMO_MEASURE_IDLE_TIME
    idle_cnt = 0;
#endif
}

void traceTaskSwitchedOut(void)
{
#if DEMO_MEASURE_IDLE_TIME
    if (OS_IsInIdleTask)
    {
        traceIdleTaskSwitchedOut();
    }
#endif
}

void traceTaskSwitchedIn(void)
{
#if DEMO_MEASURE_IDLE_TIME
    if (OS_IsInIdleTask)
    {
        traceIdleTaskSwitchedIn();
    }
#endif
}
