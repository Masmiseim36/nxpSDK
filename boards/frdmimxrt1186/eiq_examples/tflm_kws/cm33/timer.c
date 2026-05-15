/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "timer.h"

#include <stdint.h>
#if defined(__ICCARM__) || defined(__ARMCC_VERSION) || defined(__REDLIB__)
#include <time.h>
#else
#include <sys/time.h>
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Timer settings */
#define SYSTICK_PRESCALE 1U
#define TICK_PRIORITY 1U

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint32_t msTicks;

/*******************************************************************************
 * Code
 ******************************************************************************/

void SysTick_Handler(void)
{
    msTicks++;
}

void TIMER_Init(void)
{
    uint32_t priorityGroup = NVIC_GetPriorityGrouping();

#if (defined(CPU_MIMXRT2662AVJ7A) || defined(CPU_MIMXRT2662AVM7A) || defined(CPU_MIMXRT2662CVJ8A) || defined(CPU_MIMXRT2662CVL8A) || defined(CPU_MIMXRT2662CVM8A) || defined(CPU_MIMXRT2662DVJAA) || defined(CPU_MIMXRT2662DVLAA) || defined(CPU_MIMXRT2662DVMAA) || defined(CPU_MIMXRT2662XVJ78A) || defined(CPU_MIMXRT2662XVL8A) || defined(CPU_MIMXRT2662XVM8A))
    SysTick_Config(12000000 / (SYSTICK_PRESCALE * 1000U));
#else
    SysTick_Config(CLOCK_GetFreq(kCLOCK_CoreSysClk) / (SYSTICK_PRESCALE * 1000U));
#endif
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(priorityGroup, TICK_PRIORITY, 0U));
}

int TIMER_GetTimeInUS(void)
{
#if (defined(CPU_MIMXRT2662AVJ7A) || defined(CPU_MIMXRT2662AVM7A) || defined(CPU_MIMXRT2662CVJ8A) || defined(CPU_MIMXRT2662CVL8A) || defined(CPU_MIMXRT2662CVM8A) || defined(CPU_MIMXRT2662DVJAA) || defined(CPU_MIMXRT2662DVLAA) || defined(CPU_MIMXRT2662DVMAA) || defined(CPU_MIMXRT2662XVJ78A) || defined(CPU_MIMXRT2662XVL8A) || defined(CPU_MIMXRT2662XVM8A))
    int us = (12000 - SysTick->VAL) / (12000000 / 1000000);
#else
    int us = ((SystemCoreClock / 1000) - SysTick->VAL) / (SystemCoreClock / 1000000);
#endif
    us += msTicks * 1000;
    return us;
}

#if defined(__ARMCC_VERSION) || defined(__REDLIB__)

clock_t clock ()
{
    return ((uint64_t)TIMER_GetTimeInUS() * CLOCKS_PER_SEC) / 1000000;
}

#elif defined(__ICCARM__)

int timespec_get(struct timespec* ts, int base)
{
    int us = TIMER_GetTimeInUS();
    ts->tv_sec = us / 1000000;
    ts->tv_nsec = (us % 1000000) * 1000;
    return TIME_UTC ;
}

#else

int gettimeofday(struct timeval *__restrict __p,void *__restrict __tz){
    int us = TIMER_GetTimeInUS();
    __p->tv_sec = us / 1000000;
    __p->tv_usec = us % 1000000;
    return 0;
}

#endif
