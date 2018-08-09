/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define APP_SYSVIEW_APP_NAME "FRDMK22F System Viewer Demo"
#define APP_SYSVIEW_DEVICE_NAME "Cortex-M4"
#define APP_SYSVIEW_RAM_BASE (0x1FFF0000)


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
