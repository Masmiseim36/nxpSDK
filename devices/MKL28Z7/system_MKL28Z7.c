/*
** ###################################################################
**     Processors:          MKL28Z512VDC7
**                          MKL28Z512VLL7
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          MCUXpresso Compiler
**
**     Reference manual:    MKL28ZRM, Rev. 2, March, 2016
**     Version:             rev. 1.12, 2015-06-03
**     Build:               b171226
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2014-12-29)
**         Initial version.
**     - rev. 1.1 (2015-03-19)
**         Make array for registers in XRDC, PCC and TRGMUX
**     - rev. 1.2 (2015-03-20)
**         Add vector table for intmux0
**         Add IRQS for SCG, RCM, DAC, TSI
**     - rev. 1.3 (2015-03-26)
**         Update FGPIO and GPIO
**         Update IRQ for LPIT
**     - rev. 1.4 (2015-04-08)
**         Group channel registers for LPIT
**     - rev. 1.5 (2015-04-15)
**         Group channel registers for INTMUX
**     - rev. 1.6 (2015-05-06)
**         Correct FOPT reset value
**         Correct vector table size
**         Remove parts with 256K flash size
**     - rev. 1.7 (2015-05-08)
**         Reduce register for XRDC
**     - rev. 1.8 (2015-05-12)
**         Add CMP
**         DMAMUX channel count to 8
**         Add PIDR for GPIO/FGPIO
**         Rename PIT to LPIT in PCC
**         SCG, USB register update
**         SIM, TRGMUX1, TRNG, TSTMR0/1 base address update
**         Add KEY related macros for WDOG
**     - rev. 1.9 (2015-05-16)
**         Add IRQS
**     - rev. 1.10 (2015-05-30)
**         Remove MTB1, MCM1 and MMDVSQ1
**     - rev. 1.11 (2015-06-01)
**         Remove registers for modules which are not available on single in PCC and TRGMUX
**     - rev. 1.12 (2015-06-03)
**         Bit 31 of TRGMUX changed to RW from RO
**
** ###################################################################
*/

/*!
 * @file MKL28Z7
 * @version 1.12
 * @date 2015-06-03
 * @brief Device specific configuration file for MKL28Z7 (implementation file)
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

#if (DISABLE_WDOG)
  WDOG0->CNT = WDOG_UPDATE_KEY;
  WDOG0->TOVAL = 0xFFFF;
  WDOG0->CS = (uint32_t) ((WDOG0->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;
#endif /* (DISABLE_WDOG) */

  SystemInitHook();
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void) {

  uint32_t SCGOUTClock;                                 /* Variable to store output clock frequency of the SCG module */
  uint16_t Divider, prediv, multi;
  Divider = ((SCG->CSR & SCG_CSR_DIVCORE_MASK) >> SCG_CSR_DIVCORE_SHIFT) + 1;

  switch ((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT) {
    case 0x1:
      /* System OSC */
      SCGOUTClock = CPU_XTAL_CLK_HZ;
      break;
    case 0x2:
      /* Slow IRC */
      SCGOUTClock = (((SCG->SIRCCFG & SCG_SIRCCFG_RANGE_MASK) >> SCG_SIRCCFG_RANGE_SHIFT) ? 8000000 : 2000000);
      break;
    case 0x3:
      /* Fast IRC */
      SCGOUTClock = 48000000 + ((SCG->FIRCCFG & SCG_FIRCCFG_RANGE_MASK) >> SCG_FIRCCFG_RANGE_SHIFT) * 4000000;
      break;
    case 0x6:
      /* System PLL */
      if ((SCG->SPLLCFG & SCG_SPLLCFG_SOURCE_MASK) >> SCG_SPLLCFG_SOURCE_SHIFT) {
        SCGOUTClock = 48000000 + ((SCG->FIRCCFG & SCG_FIRCCFG_RANGE_MASK) >> SCG_FIRCCFG_RANGE_SHIFT) * 4000000;
      }
      else {
        SCGOUTClock = CPU_XTAL_CLK_HZ;
      }
      prediv = ((SCG->SPLLCFG & SCG_SPLLCFG_PREDIV_MASK) >> SCG_SPLLCFG_PREDIV_SHIFT) + 1;
      multi = ((SCG->SPLLCFG & SCG_SPLLCFG_MULT_MASK) >> SCG_SPLLCFG_MULT_SHIFT) + 16;
      SCGOUTClock = SCGOUTClock * multi / (prediv * 2);
      break;
    default:
      return;
  }
  SystemCoreClock = (SCGOUTClock / Divider);

}

/* ----------------------------------------------------------------------------
   -- SystemInitHook()
   ---------------------------------------------------------------------------- */

__attribute__ ((weak)) void SystemInitHook (void) {
  /* Void implementation of the weak function. */
}
