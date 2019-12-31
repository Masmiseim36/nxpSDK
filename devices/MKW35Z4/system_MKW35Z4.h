/*
** ###################################################################
**     Processor:           MKW35Z512VHT4
**     Compilers:           GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          Keil ARM C/C++ Compiler
**                          MCUXpresso Compiler
**
**     Reference manual:    MKW36A512RM Rev. 7, 09/2019
**     Version:             rev. 1.5, 2019-09-10
**     Build:               b190910
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2019 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2017-04-06)
**         Initial version.
**     - rev. 1.1 (2017-06-06)
**         PIT_TCTRLx[CHN] has been added.
**         RSIM_ANA_TEST register has been added.
**         RTCCR[CLKO] has been removed.
**     - rev. 1.2 (2018-06-13)
**         RSIM_MISC has been added.
**     - rev. 1.3 (2018-07-06)
**         Add FIFO and WATER registers for LPUART.
**     - rev. 1.4 (2019-01-25)
**         Add new part MKW35A512VFT4, MKW36A512VFT4.
**         Add new subset MKW34A which includes part MKW34A512VFT4.
**     - rev. 1.5 (2019-09-10)
**         Remove reisters XACCxx, SACCxx, FACSS, FACSN in FTFE.
**         Remove bitfields BIT1 and BIT3 in MTB.
**         Remove bitfields ALLOW_DFT_RESETS, IPP_IBE_DFT_RESET and RADIO_DFT_RESET_SEL in RSIM.
**
** ###################################################################
*/

/*!
 * @file MKW35Z4
 * @version 1.5
 * @date 2019-09-10
 * @brief Device specific configuration file for MKW35Z4 (header file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#ifndef _SYSTEM_MKW35Z4_H_
#define _SYSTEM_MKW35Z4_H_                       /**< Symbol preventing repeated inclusion */

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

#endif  /* _SYSTEM_MKW35Z4_H_ */
