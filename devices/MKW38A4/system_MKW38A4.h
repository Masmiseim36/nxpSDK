/*
** ###################################################################
**     Processor:           MKW38A512VFT4
**     Compilers:           GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          Keil ARM C/C++ Compiler
**                          MCUXpresso Compiler
**
**     Reference manual:    MKW39A512RM Rev. 4, 12/2019
**     Version:             rev. 1.2, 2019-11-15
**     Build:               b200220
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2020 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2018-06-25)
**         Initial version.
**     - rev. 1.1 (2019-08-14)
**         Remove XACC, SACC, FACSN, FACSS related registers in FTFE
**     - rev. 1.2 (2019-11-15)
**         Add register RXDIG_DFT in XCVR_RX_DIG
**
** ###################################################################
*/

/*!
 * @file MKW38A4
 * @version 1.2
 * @date 2019-11-15
 * @brief Device specific configuration file for MKW38A4 (header file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#ifndef _SYSTEM_MKW38A4_H_
#define _SYSTEM_MKW38A4_H_                       /**< Symbol preventing repeated inclusion */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#ifndef DISABLE_WDOG
  #define DISABLE_WDOG                 1
#endif

/* Define clock source values */

#define CPU_XTAL_CLK_HZ                32000000u           /* Value of the external crystal or oscillator clock frequency in Hz */
#define CPU_XTAL32k_CLK_HZ             32768u              /* Value of the external 32k crystal or oscillator clock frequency in Hz */
#define CPU_INT_SLOW_CLK_HZ            32768u              /* Value of the slow internal oscillator clock frequency in Hz  */
#define CPU_INT_FAST_CLK_HZ            4000000u            /* Value of the fast internal oscillator clock frequency in Hz  */

/* RF oscillator setting */
#define SYSTEM_RSIM_CONTROL_VALUE      0xC00100U           /* Enable RF oscillator in Run/Wait mode */

/* Low power mode enable */

/* SMC_PMPROT: ?=0,?=0,AVLP=1,?=0,?=0,?=0,AVLLS=1,?=0 */
#define SYSTEM_SMC_PMPROT_VALUE        (SMC_PMPROT_AVLP_MASK | SMC_PMPROT_ALLS_MASK | SMC_PMPROT_AVLLS_MASK) /* Mask of allowed low power modes used to initialize power modes protection register */

#define DEFAULT_SYSTEM_CLOCK           20971520U           /* Default System clock value */

#ifdef SUPPORT_WARMBOOT
#define DEFAULT_WARMBOOT_SEQUENCE      0x5A5A5A5AU
#endif


#ifdef SUPPORT_WARMBOOT
/**
 * @brief Warm boot sequence
 *
 * The const variable WARMBOOT_SEQUENCE is added to support warm boot features.
 * It is stored to WarmbootConfig section before entering into the mode that
 * support warm boot. And it is used by startup code to check whether it matches what
 * WarmbootConfig stored or not. If it matches, processor will perform a warm
 * boot process.
 */
extern const uint32_t WARMBOOT_SEQUENCE;
#endif

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

#ifdef SUPPORT_WARMBOOT
/**
 * @brief Setup the microcontroller system.
 *
 * Typically this function configures the oscillator (PLL) that is part of the
 * microcontroller device. For systems with variable clock speed it also updates
 * the variable SystemCoreClock. SystemInitWarm is called from startup_device file.
 */
void SystemInitWarm (void);
#endif

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

#ifdef SUPPORT_WARMBOOT
/**
 * @brief SystemInit function hook during warmboot.
 *
 * This weak function allows to call specific initialization code during the
 * SystemInitWarm() execution.This can be used when an application specific code
 * needs to be called as close to the reset entry as possible (for example the
 * Multicore Manager MCMGR_EarlyInit() function call).
 * NOTE: No global r/w variables can be used in this hook function because the
 * initialization of these variables happens after this function.
 */
void SystemInitHookWarm (void);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* _SYSTEM_MKW38A4_H_ */
