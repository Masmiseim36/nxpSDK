/*
** ###################################################################
**     Processors:          MKV42F128VLF16
**                          MKV42F128VLH16
**                          MKV42F128VLL16
**                          MKV42F256VLH16
**                          MKV42F256VLL16
**                          MKV42F64VLF16
**                          MKV42F64VLH16
**
**     Compilers:           Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          Keil ARM C/C++ Compiler
**                          MCUXpresso Compiler
**
**     Reference manual:    KV4XP100M168RM Rev 3.2, 09/2015
**     Version:             rev. 1.3, 2015-07-29
**     Build:               b181105
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2018 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 0.1 (2014-01-27)
**         Initial version.
**     - rev. 1.0 (2014-07-29)
**         FPU initialization and VLLSx wake-up recovery added to the system initialization
**     - rev. 1.1 (2014-10-14)
**         Renamed interrupt vector I2C to I2C0 and LPTimer to LPTMR0
**     - rev. 1.2 (2015-02-11)
**         AIPS registers have been added.
**     - rev. 1.3 (2015-07-29)
**         Correction of backward compatibility.
**
** ###################################################################
*/

/*!
 * @file MKV42F16
 * @version 1.3
 * @date 2015-07-29
 * @brief Device specific configuration file for MKV42F16 (header file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#ifndef _SYSTEM_MKV42F16_H_
#define _SYSTEM_MKV42F16_H_                      /**< Symbol preventing repeated inclusion */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#ifndef DISABLE_WDOG
  #define DISABLE_WDOG                 1
#endif

/* Define clock source values */

#define CPU_XTAL_CLK_HZ                8000000UL           /* Value of the external crystal or oscillator clock frequency in Hz */
#define CPU_INT_SLOW_CLK_HZ            32768UL             /* Value of the slow internal oscillator clock frequency in Hz  */
#define CPU_INT_FAST_CLK_HZ            4000000UL           /* Value of the fast internal oscillator clock frequency in Hz  */

/* Low power mode enable */

/* SMC_PMPROT: AHSRUN=1,?=0,AVLP=1,?=0,?=0,?=0,AVLLS=1,?=0 */
#define SYSTEM_SMC_PMPROT_VALUE        SMC_PMPROT_AHSRUN_MASK | SMC_PMPROT_AVLP_MASK | SMC_PMPROT_AVLLS_MASK /* Mask of allowed low power modes used to initialize power modes protection register */

#define DEFAULT_SYSTEM_CLOCK            20971520UL         /* Default System clock value */


/**
 * @brief System clock frequency (core clock)
 *
 * The system clock frequency supplied to the SysTick timer and the processor
 * core clock. This variable can be used by the user application to setup the
 * SysTick timer or configure other parameters. It may also be used by debugger to
 * query the frequency of the debug timer or configure the trace clock speed
 * SystemCoreClock is initialized with a correct predefined value.
 */
extern uint32_t SystemCoreClock;

/**
 * @brief Setup the microcontroller system.
 *
 * Typically this function configures the oscillator (PLL) that is part of the
 * microcontroller device. For systems with variable clock speed it also updates
 * the variable SystemCoreClock. SystemInit is called from startup_device file.
 */
void SystemInit (void);

/**
 * @brief Updates the SystemCoreClock variable.
 *
 * It must be called whenever the core clock is changed during program
 * execution. SystemCoreClockUpdate() evaluates the clock register settings and calculates
 * the current core clock.
 */
void SystemCoreClockUpdate (void);

/**
 * @brief SystemInit function hook.
 *
 * This weak function allows to call specific initialization code during the
 * SystemInit() execution.This can be used when an application specific code needs
 * to be called as close to the reset entry as possible (for example the Multicore
 * Manager MCMGR_EarlyInit() function call).
 * NOTE: No global r/w variables can be used in this hook function because the
 * initialization of these variables happens after this function.
 */
void SystemInitHook (void);

#ifdef __cplusplus
}
#endif

#endif  /* _SYSTEM_MKV42F16_H_ */
