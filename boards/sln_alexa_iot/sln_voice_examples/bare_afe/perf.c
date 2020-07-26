/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"

#include "fsl_gpt.h"

/* Output of vTaskGetRunTimeStats requires ~40 bytes per task */
static char perfBuffer[20 * 40];

void PERF_InitTimer(void)
{
    gpt_config_t gpt;

    CLOCK_SetMux(kCLOCK_PerclkMux, 0);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0);

    GPT_GetDefaultConfig(&gpt);
    GPT_Init(GPT2, &gpt);

    /* Divide by 1000: yields about 100 counts per millisecond. */
    GPT_SetClockDivider(GPT2, 1000);

    GPT_StartTimer(GPT2);
}

uint32_t PERF_GetTimer(void)
{
    return GPT_GetCurrentTimerCount(GPT2);
}

void PERF_PrintCPU(void)
{
    vTaskGetRunTimeStats((char *)&perfBuffer);
    configPRINTF(("stats:\r\n%s\r\n", perfBuffer));
}

void PERF_PrintHeap(void)
{
    configPRINTF(("  min heap remaining: %d\r\n", xPortGetMinimumEverFreeHeapSize()));
}

void PERF_PrintStack(TaskHandle_t task_id)
{
    configPRINTF(("  stack high water mark: %d\r\n", uxTaskGetStackHighWaterMark(task_id)));
}
