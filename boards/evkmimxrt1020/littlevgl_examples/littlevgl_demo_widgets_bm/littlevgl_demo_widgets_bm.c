/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "littlevgl_support.h"
#include "board.h"
#include "lvgl.h"
#include "lv_examples/src/lv_demo_widgets/lv_demo_widgets.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LPSPI_MASTER_DMA_BASEADDR     DMA0
#define LPSPI_MASTER_DMA_MUX_BASEADDR DMAMUX

/* 1 ms per tick. */
#ifndef LVGL_TICK_MS
#define LVGL_TICK_MS 1U
#endif

/* lv_task_handler is called every 5-tick. */
#ifndef LVGL_TASK_PERIOD_TICK
#define LVGL_TASK_PERIOD_TICK 5U
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint32_t s_tick        = 0U;
static volatile bool s_lvglTaskPending = false;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_SetupTick(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* DMA Mux init and EDMA init */
    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(LPSPI_MASTER_DMA_BASEADDR, &edmaConfig);
#if (defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT)
    DMAMUX_Init(LPSPI_MASTER_DMA_MUX_BASEADDR);
#endif

    PRINTF("littlevgl bare metal widgets demo\r\n");

    DEMO_SetupTick();

    lv_port_pre_init();
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    lv_demo_widgets();

    for (;;)
    {
        while (!s_lvglTaskPending)
        {
        }
        s_lvglTaskPending = false;

        lv_task_handler();
    }
}

static void DEMO_SetupTick(void)
{
    if (0 != SysTick_Config(SystemCoreClock / (LVGL_TICK_MS * 1000U)))
    {
        PRINTF("Tick initialization failed\r\n");
        while (1)
            ;
    }
}

void SysTick_Handler(void)
{
    s_tick++;
    lv_tick_inc(LVGL_TICK_MS);

    if ((s_tick % LVGL_TASK_PERIOD_TICK) == 0U)
    {
        s_lvglTaskPending = true;
    }
}
