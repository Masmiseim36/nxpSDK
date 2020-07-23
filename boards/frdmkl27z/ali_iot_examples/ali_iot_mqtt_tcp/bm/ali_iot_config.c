/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "ali_iot_config.h"
#include "timer.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint32_t timerTicks = 0U;
TIMER_HANDLE_DEFINE(s_timer);
/*******************************************************************************
 * Code
 ******************************************************************************/
void timer_callback(void *param)
{
    timerTicks++;
}

void ali_iot_timer_init(uint32_t timerInstance, uint32_t sourceClock)
{
    hal_timer_config_t s_timerConfig;

    s_timerConfig.timeout     = 1000U;
    s_timerConfig.srcClock_Hz = sourceClock;
    s_timerConfig.instance    = timerInstance;
    HAL_TimerInit(&s_timer, &s_timerConfig);
    HAL_TimerInstallCallback(&s_timer, timer_callback, NULL);
    /* Start counting */
    HAL_TimerEnable(&s_timer);
}

uint32_t ali_iot_timer_get_ticks(void)
{
    return timerTicks;
}
