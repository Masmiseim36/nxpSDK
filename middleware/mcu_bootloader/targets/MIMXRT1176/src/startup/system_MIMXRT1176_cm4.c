/*
 * Copyright 2018 NXP
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include "fsl_device_registers.h"

extern void init_interrupts(void);

/* ----------------------------------------------------------------------------
   -- Disable RTWDOG
   ---------------------------------------------------------------------------- */
void RTWDOG_disable(void)
{
    if (RTWDOG3->CS & RTWDOG_CS_EN_MASK)
    {
        RTWDOG3->CNT = 0xD928C520U; /* 0xD928C520U is the update key */
        RTWDOG3->TOVAL = 0xFFFF;
        RTWDOG3->CS = (uint32_t)((RTWDOG3->CS) & ~RTWDOG_CS_EN_MASK) | RTWDOG_CS_UPDATE_MASK;
    }

    if (RTWDOG4->CS & RTWDOG_CS_EN_MASK)
    {
        RTWDOG4->CNT = 0xD928C520U; /* 0xD928C520U is the update key */
        RTWDOG4->TOVAL = 0xFFFF;
        RTWDOG4->CS = (uint32_t)((RTWDOG4->CS) & ~RTWDOG_CS_EN_MASK) | RTWDOG_CS_UPDATE_MASK;
    }
}

/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */
uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit(void)
{
#if (DISABLE_WDOG)
    /* Watchdog disable */
    RTWDOG_disable();
#endif /* (DISABLE_WDOG) */

#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10, CP11 Full Access */
#endif                                                 /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

    init_interrupts();
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate(void) {}

/* ----------------------------------------------------------------------------
   -- SystemInitHook()
   ---------------------------------------------------------------------------- */

__attribute__((weak)) void SystemInitHook(void)
{
    /* Void implementation of the weak function. */
}
