/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "stdbool.h"
#include "timer.h"

#if defined(__ICCARM__) || defined(__ARMCC_VERSION)
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

void InitTimer (void) {
  uint32_t prioritygroup = 0x00U;
  SysTick_Config(CLOCK_GetFreq(kCLOCK_CoreSysClk) / (SYSTICK_PRESCALE * 1000U));
  prioritygroup = NVIC_GetPriorityGrouping();
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(prioritygroup, TICK_PRIORITY, 0U));
}

int GetTimeInUS(void) {
  int us = ((SystemCoreClock / 1000) - SysTick->VAL) / (SystemCoreClock / 1000000);
  us += msTicks * 1000;
  return us;
}

#if defined(__ARMCC_VERSION)

clock_t clock ()
{
  return ((uint64_t)GetTimeInUS() * CLOCKS_PER_SEC) / 1000000;
}

#elif defined(__ICCARM__)

int timespec_get(struct timespec* ts, int base)
{
  int us = GetTimeInUS();
  ts->tv_sec = us / 1000000;
  ts->tv_nsec = (us % 1000000) * 1000;
  return TIME_UTC ;
}

#else

int gettimeofday (struct timeval *__restrict __p,void *__restrict __tz){
  int us = GetTimeInUS();
  __p->tv_sec = us / 1000000;
  __p->tv_usec = us % 1000000;
  return 0;
}

#endif
