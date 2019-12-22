/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "stdbool.h"
#include "timer.h"

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
