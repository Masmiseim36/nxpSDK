/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "trcBase.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_SYSVIEW_APP_NAME "FRDM-K32L3A6 System Viewer Demo"
#define APP_SYSVIEW_DEVICE_NAME "Cortex-M0"
#define APP_SYSVIEW_RAM_BASE (0x09000000)

/* Task priorities. */
#define FIRST_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define SECOND_TASK_PRIORITY (configMAX_PRIORITIES - 2)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void first_task(void *pvParameters);
static void second_task(void *pvParameters);

#define APP_DELAY 100

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    vTraceInitTraceData();

    if (!uiTraceStart())
    {
        vTraceConsoleMessage("Could not start recorder!");
    }

    if (xTaskCreate(first_task, "first_task", configMINIMAL_STACK_SIZE, NULL, FIRST_TASK_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    while (1)
        ;
}

/*!
 * @brief First task, higher priority.
 */
static void first_task(void *pvParameters)
{
    traceLabel consoleLabel;
    consoleLabel = xTraceOpenLabel("Messages");

    if (xTaskCreate(second_task, "second_task", configMINIMAL_STACK_SIZE, NULL, SECOND_TASK_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        vTaskSuspend(NULL);
    }

    PRINTF("Please fill Percepio \"Memory Region\" options: \r\n");
    PRINTF("Start address: 0x%x \r\n", RecorderDataPtr);
    PRINTF("Bytes to read: 0x%x \r\n", sizeof(*RecorderDataPtr));

    /* dummy code, print counter and delay */
    for (int counter = 0;; counter++)
    {
        vTracePrintF(consoleLabel, "first task counter: %d \n", counter++);
        vTaskDelay(APP_DELAY);
    }
}

/*!
 * @brief Second task, lower priority.
 */
static void second_task(void *pvParameters)
{
    traceLabel consoleLabel;

    consoleLabel = xTraceOpenLabel("Messages");
    while (1)
    {
        /* dummy code, notify HOST when int16 underflow */
        for (uint16_t j = (uint16_t)-1;; j--)
        {
            if (j == 0)
            {
                vTracePrintF(consoleLabel, "second task int underflow");
            }
            __NOP();
        }
    }
}
