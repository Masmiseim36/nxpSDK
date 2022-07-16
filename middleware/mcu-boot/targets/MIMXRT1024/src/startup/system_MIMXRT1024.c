/*
** ###################################################################
**     Processors:          MIMXRT1024CAG4A
**                          MIMXRT1024DAG5A
**
**     Compilers:           Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          Keil ARM C/C++ Compiler
**                          MCUXpresso Compiler
**
**     Reference manual:    IMXRT1024RM Rev.0, 09/2020 | IMXRT102xSRM Rev.0
**     Version:             rev. 0.1, 2020-01-15
**     Build:               b200509
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright 2020 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 0.1 (2020-01-15)
**         Initial version.
**
** ###################################################################
*/

/*!
 * @file MIMXRT1024
 * @version 1.0
 * @date 090520
 * @brief Device specific configuration file for MIMXRT1024 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>

#include "fsl_device_registers.h"

/* ----------------------------------------------------------------------------
   -- Disable RTWDOG
   ---------------------------------------------------------------------------- */
void RTWDOG_disable(void)
{
    // Leave reset value of timout
    RTWDOG->TOVAL |= 0;
    // Set WDOG3_CS[UPDATE] to 1
    RTWDOG->CS = ((RTWDOG->CS) & (~RTWDOG_CS_UPDATE_MASK)) | RTWDOG_CS_UPDATE(1);
    // Unlock WDOG3
    RTWDOG->CNT = RTWDOG_UPDATE_KEY;
    // Check unlock statuc
    while (!(RTWDOG->CS & RTWDOG_CS_ULK_MASK))
    {
    }

    RTWDOG->CS &= ~RTWDOG_CS_EN_MASK;
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
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10, CP11 Full Access */
#endif                                                 /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

    /* Disable SysTick in case it is enabled in ROM bootloader */
    if ((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) != 0U)
    {
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    }

    /* Watchdog disable */

#if (DISABLE_WDOG)
    if (RTWDOG->CS & RTWDOG_CS_EN_MASK)
    {
        RTWDOG_disable();
    }
#endif /* (DISABLE_WDOG) */

    /* Disable SysTick in case it is enabled in ROM bootloader */
    if ((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) != 0U)
    {
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    }
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
