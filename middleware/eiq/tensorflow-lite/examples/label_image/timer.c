/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile static int s_MSTicks = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

void SysTick_Handler(void)
{
    s_MSTicks++;
}

void InitTimer (void)
{
    SysTick_Config(SystemCoreClock / 1000);
}

int GetTimeInUS()
{
    int us = ((SystemCoreClock / 1000) - SysTick->VAL) / (SystemCoreClock / 1000000);
    us += s_MSTicks * 1000;
    return us;
}

