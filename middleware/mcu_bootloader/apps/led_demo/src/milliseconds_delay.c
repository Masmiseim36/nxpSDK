/*
 * Copyright (c) 2013 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "milliseconds_delay.h"

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

static volatile uint32_t s_milliseconds_left = 0;

static inline uint32_t milliseconds_left_get(void)
{
    return s_milliseconds_left;
}

static inline void milliseconds_left_set(uint32_t ms)
{
    s_milliseconds_left = ms;
}

static inline void milliseconds_left_decrement(void)
{
    --s_milliseconds_left;
}

void milliseconds_delay_init(void)
{
    SysTick_Config(SystemCoreClock >> 10);
}

void milliseconds_delay(uint32_t ms)
{
    milliseconds_left_set(ms);
    while (milliseconds_left_get())
        ;
}

void SysTick_Handler(void)
{
    if (milliseconds_left_get())
    {
        milliseconds_left_decrement();
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
