/*
** ###################################################################
**     Processors:          MKL17Z128VFM4
**                          MKL17Z128VFT4
**                          MKL17Z128VLH4
**                          MKL17Z128VMP4
**                          MKL17Z256VFM4
**                          MKL17Z256VFT4
**                          MKL17Z256VLH4
**                          MKL17Z256VMP4
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          MCUXpresso Compiler
**
**     Reference manual:    KL17P64M48SF6RM, Rev.3, Aug 2014
**     Version:             rev. 1.5, 2016-02-02
**     Build:               b180801
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2018 NXP
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2014-03-27)
**         Initial version.
**     - rev. 1.1 (2014-05-26)
**         I2S registers TCR2/RCR2 and others were changed.
**         FLEXIO register FLEXIO_VERID has now bitfields: FEATURE, MINOR, MAJOR.
**         Names of the bitfields of the FLEXIO_SHIFTBUF have been changed to the appropriate register name e.g.: FLEXIO_SHIFTBUFBBS_SHIFTBUFBBS.
**         Peripheral_BASES macros has been changed to Peripheral_BASE_PTRS, e.g.: ADC_BASES to ADC_BASE_PTRS.
**         Clock configuration for high range external oscillator has been added.
**         RFSYS module access has been added.
**     - rev. 1.2 (2014-07-10)
**         GPIO - Renamed modules PTA,PTB,PTC,PTD,PTE to GPIOA,GPIOB,GPIOC,GPIOD,GPIOE.
**         UART0 - UART0 module renamed to UART2.
**         I2S - removed MDR register.
**     - rev. 1.3 (2014-08-21)
**         UART2 - Removed ED register.
**         UART2 - Removed MODEM register.
**         UART2 - Removed IR register.
**         UART2 - Removed PFIFO register.
**         UART2 - Removed CFIFO register.
**         UART2 - Removed SFIFO register.
**         UART2 - Removed TWFIFO register.
**         UART2 - Removed TCFIFO register.
**         UART2 - Removed RWFIFO register.
**         UART2 - Removed RCFIFO register.
**         SIM - Changed bitfield value MCGIRCLK to LIRC_CLK of bitfield CLKOUTSEL in SOPT2 register.
**         SIM - Removed bitfield DIEID in SDID register.
**     - rev. 1.4 (2015-07-29)
**         Correction of backward compatibility.
**     - rev. 1.5 (2016-02-02)
**         FGPIO - Add FGPIO registers.
**
** ###################################################################
*/

/*!
 * @file MKL17Z4
 * @version 1.5
 * @date 2016-02-02
 * @brief Device specific configuration file for MKL17Z4 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "fsl_device_registers.h"



/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit (void) {

#if (ACK_ISOLATION)
  if((PMC->REGSC &  PMC_REGSC_ACKISO_MASK) != 0U) {
    PMC->REGSC |= PMC_REGSC_ACKISO_MASK; /* VLLSx recovery */
  }
#endif

#if (DISABLE_WDOG)
  /* SIM->COPC: ?=0,COPCLKSEL=0,COPDBGEN=0,COPSTPEN=0,COPT=0,COPCLKS=0,COPW=0 */
  SIM->COPC = (uint32_t)0x00u;
#endif /* (DISABLE_WDOG) */

  SystemInitHook();
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void) {

  uint32_t MCGOUTClock;                                 /* Variable to store output clock frequency of the MCG module */
  uint16_t Divider;

  if ((MCG->S & MCG_S_CLKST_MASK) == 0x00U) {
    /* High internal reference clock is selected */
    MCGOUTClock = CPU_INT_FAST_CLK_HZ;                                  /* Fast internal reference clock selected */
  } else if ((MCG->S & MCG_S_CLKST_MASK) == 0x04U) {
    /* Internal reference clock is selected */
    Divider = (uint16_t)(0x01LU << ((MCG->SC & MCG_SC_FCRDIV_MASK) >> MCG_SC_FCRDIV_SHIFT));
    MCGOUTClock = (uint32_t) (CPU_INT_SLOW_CLK_HZ / Divider);           /* Slow internal reference clock 8MHz selected */
  } else if ((MCG->S & MCG_S_CLKST_MASK) == 0x08U) {
    /* External reference clock is selected */
    MCGOUTClock = CPU_XTAL_CLK_HZ;
  } else {
    /* Reserved value */
    return;
  } /* (!((MCG->S & MCG_S_CLKST_MASK) == 0x08U)) */
  SystemCoreClock = (MCGOUTClock / (0x01U + ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT)));

}

/* ----------------------------------------------------------------------------
   -- SystemInitHook()
   ---------------------------------------------------------------------------- */

__attribute__ ((weak)) void SystemInitHook (void) {
  /* Void implementation of the weak function. */
}
