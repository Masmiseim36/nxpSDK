/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "main.h"
#include "timer.h"

volatile uint16_t cycle_counter;

#if defined(CPU_MKW24D512VHA5) || defined(MCU_MK21DX256) || defined(CPU_MKW41Z512VHT4) || defined(CPU_MKW36Z512VHT4)
    #define full_range 356 /* (0xFFFFFF / 48000000 * 1000) */
#elif defined(MCU_MKW40Z160)
    #define full_range 524 /* (0xFFFFFF / 32000000 * 1000) */
#elif defined(CPU_MK64FN1M0VMD12)
    #define full_range 139 /* (0xFFFFFF / 120000000 * 1000) */
#elif defined(CPU_QN9080C)
    #define full_range 1048 /* (0xFFFFFF / 16000000 * 1000) */
#else
    #define full_range 800 /* (0xFFFFFF / 20971520 * 1000) */
#endif

void StartTimer()
{
    /* make sure we dont count while before resetting the values */
    StopTimer();
    /* set maximum range */
    SysTick->LOAD = 0xFFFFFF;
    /* reset cycle counter */
    cycle_counter = 0;
    /* reset counter */
    SysTick->VAL = 0;
    /* enable the module and start counting */
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}
void StopTimer()
{
    /* disable the module */
    SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
}
/* returns elapsed miliseconds */
uint32_t GetElapsedTime()
{
    uint32_t temp;

    temp = ((10000 - ((unsigned long long)SysTick->VAL * 10000 / 0xFFFFFF)) / (10000 / full_range));

    return cycle_counter * full_range + temp;
}
void SysTickHandler(void)
{
    cycle_counter++;
}
