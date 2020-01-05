/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "fsl_debug_console.h"
#include "littlevgl_support.h"
#include "board.h"
#include "lv_examples/terminal/terminal.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
SemaphoreHandle_t lvglMutex = NULL;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void AppTask(void *param);
static void TermTask(void *param);

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t DSPI0_GetFreq(void)
{
    return CLOCK_GetBusClkFreq();
}

uint32_t I2C0_GetFreq(void)
{
    return CLOCK_GetFreq(I2C0_CLK_SRC);
}

uint32_t I2C1_GetFreq(void)
{
    return CLOCK_GetFreq(I2C1_CLK_SRC);
}
static void AppTask(void *param)
{
    lvglMutex = xSemaphoreCreateRecursiveMutex();
    if (NULL == lvglMutex)
    {
        PRINTF("Error while create the LittlevGL mutex\r\n");
        while (1)
            ;
    }

    lv_port_pre_init();
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    terminal_create();

    for (;;)
    {
        /* Must use mutex for LittlevGL api. */
        xSemaphoreTakeRecursive(lvglMutex, portMAX_DELAY);

        lv_tick_inc(10);
        lv_task_handler();

        xSemaphoreGiveRecursive(lvglMutex);

        vTaskDelay(10);
    }
}

static void TermTask(void *param)
{
    char ch[2] = {'\0', '\0'};

    while (1)
    {
        ch[0] = GETCHAR();

        PUTCHAR(ch[0]);

        /* Must use mutex for LittlevGL api. */
        xSemaphoreTakeRecursive(lvglMutex, portMAX_DELAY);

        terminal_add(ch);

        xSemaphoreGiveRecursive(lvglMutex);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BaseType_t stat;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    stat = xTaskCreate(AppTask, "littlevgl", configMINIMAL_STACK_SIZE + 800, NULL, tskIDLE_PRIORITY + 3, NULL);

    if (pdPASS != stat)
    {
        PRINTF("Failed to create littlevgl task");
        while (1)
            ;
    }

    stat = xTaskCreate(TermTask, "terminal", configMINIMAL_STACK_SIZE + 200, NULL, tskIDLE_PRIORITY + 2, NULL);

    if (pdPASS != stat)
    {
        PRINTF("Failed to create terminal task");
        while (1)
            ;
    }

    vTaskStartScheduler();

    for (;;)
    {
    } /* should never get here */
}

/*!
 * @brief Malloc failed hook.
 */
void vApplicationMallocFailedHook(void)
{
    for (;;)
        ;
}

/*!
 * @brief Stack overflow hook.
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
    (void)pcTaskName;
    (void)xTask;

    for (;;)
        ;
}
