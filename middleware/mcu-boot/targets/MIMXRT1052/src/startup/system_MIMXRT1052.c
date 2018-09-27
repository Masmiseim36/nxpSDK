/*
** ###################################################################
**     Processors:          MCIMXRM2CVL05AA
**                          MCIMXRM2DVL06AA
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          MCUXpresso Compiler
**
**     Reference manual:    i.MX 6RT for ROM
**     Version:             rev. 0.1, 2017-01-10
**     Build:               b170308
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright (c) 2015 Freescale Semiconductor, Inc.
**     Copyright 2016-2018 NXP
**     All rights reserved.
**     
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 0.1 (2017-01-10)
**         Initial version.
**
** ###################################################################
*/

/*!
 * @file MCIMXRM2
 * @version 0.1
 * @date 2017-01-10
 * @brief Device specific configuration file for MCIMXRM2 (implementation file)
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

/* Watchdog disable */

#if (DISABLE_WDOG)
    if (RTWDOG->CS & RTWDOG_CS_EN_MASK)
    {
        RTWDOG_disable();
    }
#endif /* (DISABLE_WDOG) */
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate(void)
{
}
