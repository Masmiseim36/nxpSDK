/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "littlevgl_support.h"
#include "board.h"
#include "lv_examples/demo/demo.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void AppTask(void *param)
{
    lv_port_pre_init();
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    demo_create();

    for (;;)
    {
        lv_tick_inc(5);
        lv_task_handler();
        vTaskDelay(5);
    }
}

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

    stat = xTaskCreate(AppTask, "littlevgl", configMINIMAL_STACK_SIZE + 800, NULL, tskIDLE_PRIORITY + 2, NULL);

    if (pdPASS != stat)
    {
        PRINTF("Failed to create littlevgl task");
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
