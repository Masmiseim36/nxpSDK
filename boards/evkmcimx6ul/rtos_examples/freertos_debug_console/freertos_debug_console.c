/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
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

/*System includes.*/
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "semphr.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_UART_BASEADDR UART1_BASE
#define DEMO_UART_CLK_FREQ BOARD_DebugConsoleSrcFreq()
#define DEMO_IRQn UART1_IRQn
#define DEMO_DEBUG_CONSOLE_DEVICE_TYPE DEBUG_CONSOLE_DEVICE_TYPE_IUART
#define DEMO_DEBUG_UART_BAUDRATE 115200U

#define DEMO_PRINT_TASK1_PRIORITY (configMAX_PRIORITIES - 1U)
#define DEMO_PRINT_TASK2_PRIORITY (configMAX_PRIORITIES - 1U)
#define DEMO_PRINT_TASK3_PRIORITY (configMAX_PRIORITIES - 2U)
#define DEMO_SCANF_TASK_PRIORITY (configMAX_PRIORITIES - 3U)

#define DEMO_TASK1_STACK_SIZE_OFFSET (100U)
#define DEMO_TASK2_STACK_SIZE_OFFSET (100U)
#define DEMO_TASK3_STACK_SIZE_OFFSET (100U)
#define DEMO_SCANF_TASK_STACK_SIZE_OFFSET (100U)


/*******************************************************************************
* Variables
******************************************************************************/
TaskHandle_t printTask1Handle = NULL;
TaskHandle_t printTask2Handle = NULL;
TaskHandle_t printTask3Handle = NULL;
TaskHandle_t scanfTaskHandle = NULL;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Application API */
static void printTask1(void *pvParameters);
static void printTask2(void *pvParameters);
static void printTask3(void *pvParameters);
static void scanfTask(void *pvParameters);
/*******************************************************************************
 * Code
 ******************************************************************************/

/*Provides own implementation of vApplicationIRQHandler() to call SystemIrqHandler()
 *directly without saving the FPU registers on interrupt entry.
 */
void vApplicationIRQHandler(uint32_t ulICCIAR)
{
    SystemIrqHandler(ulICCIAR);
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    /* Install IRQ Handler */
    SystemInitIrqTable();
    /* Set interrupt priority */
    GIC_SetPriority(DEMO_IRQn, 25);

    xTaskCreate(printTask1, "printTask1", configMINIMAL_STACK_SIZE + DEMO_TASK1_STACK_SIZE_OFFSET, NULL,
                DEMO_PRINT_TASK1_PRIORITY, &printTask1Handle);
    xTaskCreate(printTask2, "printTask2", configMINIMAL_STACK_SIZE + DEMO_TASK2_STACK_SIZE_OFFSET, NULL,
                DEMO_PRINT_TASK2_PRIORITY, &printTask2Handle);
    xTaskCreate(printTask3, "printTask3", configMINIMAL_STACK_SIZE + DEMO_TASK3_STACK_SIZE_OFFSET, NULL,
                DEMO_PRINT_TASK3_PRIORITY, &printTask3Handle);
    xTaskCreate(scanfTask, "scanfTask", configMINIMAL_STACK_SIZE + DEMO_SCANF_TASK_STACK_SIZE_OFFSET, NULL,
                DEMO_SCANF_TASK_PRIORITY, &scanfTaskHandle);

    vTaskStartScheduler();
    for (;;)
        ;
}

/*******************************************************************************
 * Application functions
 ******************************************************************************/
/*!
 * @brief print task1 function
 */
static void printTask1(void *pvParameters)
{
    while (1)
    {
        PRINTF("\r\n%s\r\n", "Debug console Demo-print task1.");
        PRINTF("\r\nTask1 priority %d, stack size is %d, current tick count %d\r\n", DEMO_PRINT_TASK1_PRIORITY,
               DEMO_TASK1_STACK_SIZE_OFFSET, xTaskGetTickCount());
        /* delay */
        vTaskDelay(100U);
    }
}

/*!
 * @brief print task2 function
 */
static void printTask2(void *pvParameters)
{
    while (1)
    {
        PRINTF("\r\nDebug console Demo-print task2.\r\n");
        PUTCHAR('c');
        PRINTF("\r\nTask2 priority %d, stack size is %d, current tick count %d\r\n", DEMO_PRINT_TASK2_PRIORITY,
               DEMO_TASK2_STACK_SIZE_OFFSET, xTaskGetTickCount());
        /* delay */
        vTaskDelay(500U);
    }
}

/*!
 * @brief print task3 function
 */
static void printTask3(void *pvParameters)
{
    while (1)
    {
        PRINTF("\r\nDebug console Demo-print task3.\r\n");
        PRINTF("\r\nTask3 priority %d, stack size is %d, current tick count %d\r\n", DEMO_PRINT_TASK3_PRIORITY,
               DEMO_TASK3_STACK_SIZE_OFFSET, xTaskGetTickCount());
        /* delay */
        vTaskDelay(300U);
    }
}

static void scanfTask(void *pvParameters)
{
    char ch;

    PRINTF("\r\nDebug console Demo-scanf task.\r\n");
    PRINTF("\r\n Please Input a char together with press enter: \r\n");
    SCANF("%c", &ch);
    PUTCHAR(ch);
    /* suspend */
    vTaskSuspend(scanfTaskHandle);
}
