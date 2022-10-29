/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "lvgl_support.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "lvgl.h"
#include "lv_examples/src/lv_demo_widgets/lv_demo_widgets.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
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
#if LV_USE_LOG
static void print_cb(lv_log_level_t level, const char *file, uint32_t line, const char *func, const char *buf);
#endif

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
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("lvgl bare metal widgets demo\r\n");

    DEMO_SetupTick();

#if LV_USE_LOG
    lv_log_register_print_cb(print_cb);
#endif

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

#if LV_USE_LOG
static void print_cb(lv_log_level_t level, const char *file, uint32_t line, const char *func, const char *buf)
{
    /*Use only the file name not the path*/
    size_t p;

    for (p = strlen(file); p > 0; p--)
    {
        if (file[p] == '/' || file[p] == '\\')
        {
            p++; /*Skip the slash*/
            break;
        }
    }

    static const char *lvl_prefix[] = {"Trace", "Info", "Warn", "Error", "User"};

    PRINTF("\r%s: %s \t(%s #%d %s())\n", lvl_prefix[level], buf, &file[p], line, func);
}
#endif
