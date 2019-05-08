/*
** ###################################################################
**     Processors:          MKL82Z128VLH7
**                          MKL82Z128VLK7
**                          MKL82Z128VLL7
**                          MKL82Z128VMC7
**                          MKL82Z128VMP7
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    KL82P121M72SF0RM, Rev.1.2 September 2015
**     Version:             rev. 1.5, 2015-09-24
**     Build:               b151023
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright (c) 2015 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2015-04-18)
**         Initial version.
**     - rev. 1.1 (2015-05-04)
**         Update SIM, EVMSIM, QuadSPI, and I2C based on Rev0 document.
**     - rev. 1.2 (2015-08-11)
**         Correct clock configuration.
**     - rev. 1.3 (2015-08-20)
**         Align with RM Rev.1.
**     - rev. 1.4 (2015-08-28)
**         Update LPUART to add FIFO.
**     - rev. 1.5 (2015-09-24)
**         Update to align with RM Rev.1.2.
**
** ###################################################################
*/

/*!
 * @file MKL82Z7
 * @version 1.5
 * @date 2015-09-24
 * @brief Device specific configuration file for MKL82Z7 (header file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#ifndef _SYSTEM_MKL82Z7_H_
#define _SYSTEM_MKL82Z7_H_ /**< Symbol preventing repeated inclusion */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef DISABLE_WDOG
#define DISABLE_WDOG 1
#endif

/* MCG mode constants */

#define MCG_MODE_FEI 0U
#define MCG_MODE_FBI 1U
#define MCG_MODE_BLPI 2U
#define MCG_MODE_FEE 3U
#define MCG_MODE_FBE 4U
#define MCG_MODE_BLPE 5U
#define MCG_MODE_PBE 6U
#define MCG_MODE_PEE 7U

/* Predefined clock setups
   0 ... Default  part configuration
         Multipurpose Clock Generator (MCG) in FEI mode.
         Reference clock source for MCG module: Slow internal reference clock
         Core clock = 20.97152MHz
         Bus clock  = 20.97152MHz
   1 ... Maximal speed configuration
         Multipurpose Clock Generator (MCG) in PEE mode.
         Reference clock source for MCG module: System oscillator 0 reference clock
         Core clock = 96MHz
         Bus clock  = 24MHz
   2 ... Chip internally clocked, ready for Very Low Power Run mode.
         Multipurpose Clock Generator (MCG) in BLPI mode.
         Reference clock source for MCG module: Fast internal reference clock
         Core clock = 4MHz
         Bus clock  = 4MHz
   3 ... Chip externally clocked, ready for Very Low Power Run mode.
         Multipurpose Clock Generator (MCG) in BLPE mode.
         Reference clock source for MCG module: System oscillator 0 reference clock
         Core clock = 4MHz
         Bus clock  = 4MHz
   4 ... USB clock setup
         Multipurpose Clock Generator (MCG) in PEE mode.
         Reference clock source for MCG module: System oscillator 0 reference clock
         Core clock = 96MHz
         Bus clock  = 24MHz
   5 ... Maximum achievable clock frequency configuration in RUN mode
         Multipurpose Clock Generator (MCG) in PEE mode.
         Reference clock source for MCG module: System oscillator 0 reference clock
         Core clock = 72MHz
         Bus clock  = 24MHz
*/

/* Define clock source values */

#define CPU_XTAL_CLK_HZ \
    12000000U /* Value of the external crystal or oscillator clock frequency of the system oscillator (OSC) in Hz */
#define CPU_XTAL32k_CLK_HZ 32768U  /* Value of the external 32k crystal or oscillator clock frequency of the RTC in Hz \
                                      */
#define CPU_INT_SLOW_CLK_HZ 32768U /* Value of the slow internal oscillator clock frequency in Hz */
#define CPU_INT_FAST_CLK_HZ 4000000U /* Value of the fast internal oscillator clock frequency in Hz */
#define CPU_INT_IRC_CLK_HZ 48000000U /* Value of the 48M internal oscillator clock frequency in Hz */

/* RTC oscillator setting */
/* RTC_CR: SC2P=0,SC4P=0,SC8P=0,SC16P=0,CLKO=1,OSCE=1,WPS=0,UM=0,SUP=0,WPE=0,SWR=0 */
#define SYSTEM_RTC_CR_VALUE 0x0300U /* RTC_CR */

/* Low power mode enable */
/* SMC_PMPROT: AHSRUN=1,AVLP=1,ALLS=1,AVLLS=1 */
#define SYSTEM_SMC_PMPROT_VALUE 0xAAU /* SMC_PMPROT */

/* Internal reference clock trim */
/* #undef SLOW_TRIM_ADDRESS */      /* Slow oscillator not trimmed. Commented out for MISRA compliance. */
/* #undef SLOW_FINE_TRIM_ADDRESS */ /* Slow oscillator not trimmed. Commented out for MISRA compliance. */
/* #undef FAST_TRIM_ADDRESS */      /* Fast oscillator not trimmed. Commented out for MISRA compliance. */
/* #undef FAST_FINE_TRIM_ADDRESS */ /* Fast oscillator not trimmed. Commented out for MISRA compliance. */

#ifdef CLOCK_SETUP
#if (CLOCK_SETUP == 0)
#define DEFAULT_SYSTEM_CLOCK 20971520U /* Default System clock value */
#define MCG_MODE MCG_MODE_FEI          /* Clock generator mode */
/* MCG_C1: CLKS=0,FRDIV=0,IREFS=1,IRCLKEN=1,IREFSTEN=0 */
#define SYSTEM_MCG_C1_VALUE 0x06U /* MCG_C1 */
/* MCG_C2: LOCRE0=0,FCFTRIM=0,RANGE=2,HGO=0,EREFS=1,LP=0,IRCS=0 */
#define SYSTEM_MCG_C2_VALUE 0x24U /* MCG_C2 */
/* MCG_C4: DMX32=0,DRST_DRS=0,FCTRIM=0,SCFTRIM=0 */
#define SYSTEM_MCG_C4_VALUE 0x00U /* MCG_C4 */
/* MCG_SC: ATME=0,ATMS=0,ATMF=0,FLTPRSRV=0,FCRDIV=0,LOCS0=0 */
#define SYSTEM_MCG_SC_VALUE 0x00U /* MCG_SC */
/* MCG_C5: PLLCLKEN=0,PLLSTEN=0,PRDIV=0 */
#define SYSTEM_MCG_C5_VALUE 0x00U /* MCG_C5 */
/* MCG_C6: LOLIE0=0,PLLS=0,CME0=0,VDIV=0 */
#define SYSTEM_MCG_C6_VALUE 0x00U /* MCG_C6 */
/* MCG_C7: OSCSEL=0 */
#define SYSTEM_MCG_C7_VALUE 0x00U /* MCG_C7 */
/* OSC_CR: ERCLKEN=1,EREFSTEN=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
#define SYSTEM_OSC_CR_VALUE 0x80U /* OSC_CR */
/* SMC_PMCTRL: RUNM=0,STOPA=0,STOPM=0 */
#define SYSTEM_SMC_PMCTRL_VALUE 0x00U /* SMC_PMCTRL */
/* SIM_CLKDIV1: OUTDIV1=0,OUTDIV2=0,OUTDIV4=1,OUTDIV5=1 */
#define SYSTEM_SIM_CLKDIV1_VALUE 0x00011000U /* SIM_CLKDIV1 */
/* SIM_CLKDIV3: PLLFLLDIV=0,PLLFLLFRAC=0 */
#define SYSTEM_SIM_CLKDIV3_VALUE 0x00U /* SIM_CLKDIV3 */
/* SIM_SOPT1: USBREGEN=0,USBSSTBY=0,USBVSTBY=0,OSC32KSEL=2,RAMSIZE=0 */
#define SYSTEM_SIM_SOPT1_VALUE 0x00080000U /* SIM_SOPT1 */
                                           /* SIM_SOPT2:
                                            * EMVSIMSRC=0,SDHCSRC=0,LPUARTSRC=0,TPMSRC=1,FLEXIOSRC=0,USBSRC=0,PLLFLLSEL=0,TRACECLKSEL=0,FBSL=0,CLKOUTSEL=0,RTCCLKOUTSEL=0
                                            */
#define SYSTEM_SIM_SOPT2_VALUE 0x01000000U /* SIM_SOPT2 */
#elif(CLOCK_SETUP == 1)
#define DEFAULT_SYSTEM_CLOCK 96000000U /* Default System clock value */
#define MCG_MODE MCG_MODE_PEE     /* Clock generator mode */
/* MCG_C1: CLKS=0,FRDIV=4,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
#define SYSTEM_MCG_C1_VALUE 0x22U /* MCG_C1 */
/* MCG_C2: LOCRE0=0,FCFTRIM=0,RANGE=2,HGO=0,EREFS=1,LP=0,IRCS=0 */
#define SYSTEM_MCG_C2_VALUE 0x24U /* MCG_C2 */
/* MCG_C4: DMX32=0,DRST_DRS=0,FCTRIM=0,SCFTRIM=0 */
#define SYSTEM_MCG_C4_VALUE 0x00U /* MCG_C4 */
/* MCG_SC: ATME=0,ATMS=0,ATMF=0,FLTPRSRV=0,FCRDIV=0,LOCS0=0 */
#define SYSTEM_MCG_SC_VALUE 0x00U /* MCG_SC */
/* MCG_C5: PLLCLKEN=0,PLLSTEN=0,PRDIV=0 */
#define SYSTEM_MCG_C5_VALUE 0x00U /* MCG_C5 */
/* MCG_C6: LOLIE0=0,PLLS=1,CME0=0,VDIV=0 */
#define SYSTEM_MCG_C6_VALUE 0x40U /* MCG_C6 */
/* MCG_C7: OSCSEL=0 */
#define SYSTEM_MCG_C7_VALUE 0x00U /* MCG_C7 */
/* OSC_CR: ERCLKEN=1,EREFSTEN=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
#define SYSTEM_OSC_CR_VALUE 0x80U /* OSC_CR */
/* SMC_PMCTRL: RUNM=3,STOPA=0,STOPM=0 */
#define SYSTEM_SMC_PMCTRL_VALUE 0x60U /* SMC_PMCTRL */
/* SIM_CLKDIV1: OUTDIV1=0,OUTDIV2=3,OUTDIV4=3,OUTDIV5=0 */
#define SYSTEM_SIM_CLKDIV1_VALUE 0x03030000U /* SIM_CLKDIV1 */
/* SIM_CLKDIV3: PLLFLLDIV=0,PLLFLLFRAC=0 */
#define SYSTEM_SIM_CLKDIV3_VALUE 0x00U /* SIM_CLKDIV3 */
/* SIM_SOPT1: USBREGEN=0,USBSSTBY=0,USBVSTBY=0,OSC32KSEL=2,RAMSIZE=0 */
#define SYSTEM_SIM_SOPT1_VALUE 0x00080000U /* SIM_SOPT1 */
/* SIM_SOPT2:
 * EMVSIMSRC=0,SDHCSRC=0,LPUARTSRC=0,TPMSRC=1,FLEXIOSRC=0,USBSRC=0,PLLFLLSEL=1,TRACECLKSEL=0,FBSL=0,CLKOUTSEL=0,RTCCLKOUTSEL=0
 */
#define SYSTEM_SIM_SOPT2_VALUE 0x01010000U /* SIM_SOPT2 */
#elif(CLOCK_SETUP == 2)
#define DEFAULT_SYSTEM_CLOCK 4000000U /* Default System clock value */
#define MCG_MODE MCG_MODE_BLPI    /* Clock generator mode */
/* MCG_C1: CLKS=1,FRDIV=0,IREFS=1,IRCLKEN=1,IREFSTEN=0 */
#define SYSTEM_MCG_C1_VALUE 0x46U /* MCG_C1 */
/* MCG_C2: LOCRE0=0,FCFTRIM=0,RANGE=2,HGO=0,EREFS=1,LP=1,IRCS=1 */
#define SYSTEM_MCG_C2_VALUE 0x27U /* MCG_C2 */
/* MCG_C4: DMX32=0,DRST_DRS=0,FCTRIM=0,SCFTRIM=0 */
#define SYSTEM_MCG_C4_VALUE 0x00U /* MCG_C4 */
/* MCG_SC: ATME=0,ATMS=0,ATMF=0,FLTPRSRV=0,FCRDIV=0,LOCS0=0 */
#define SYSTEM_MCG_SC_VALUE 0x00U /* MCG_SC */
/* MCG_C5: PLLCLKEN=0,PLLSTEN=0,PRDIV=0 */
#define SYSTEM_MCG_C5_VALUE 0x00U /* MCG_C5 */
/* MCG_C6: LOLIE0=0,PLLS=0,CME0=0,VDIV=0 */
#define SYSTEM_MCG_C6_VALUE 0x00U /* MCG_C6 */
/* MCG_C7: OSCSEL=0 */
#define SYSTEM_MCG_C7_VALUE 0x00U /* MCG_C7 */
/* OSC_CR: ERCLKEN=1,EREFSTEN=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
#define SYSTEM_OSC_CR_VALUE 0x80U /* OSC_CR */
/* SMC_PMCTRL: RUNM=0,STOPA=0,STOPM=0 */
#define SYSTEM_SMC_PMCTRL_VALUE 0x00U /* SMC_PMCTRL */
/* SIM_CLKDIV1: OUTDIV1=0,OUTDIV2=3,OUTDIV4=4,OUTDIV5=0 */
#define SYSTEM_SIM_CLKDIV1_VALUE 0x03040000U /* SIM_CLKDIV1 */
/* SIM_CLKDIV3: PLLFLLDIV=0,PLLFLLFRAC=0 */
#define SYSTEM_SIM_CLKDIV3_VALUE 0x00U /* SIM_CLKDIV3 */
/* SIM_SOPT1: USBREGEN=0,USBSSTBY=0,USBVSTBY=0,OSC32KSEL=2,RAMSIZE=0 */
#define SYSTEM_SIM_SOPT1_VALUE 0x00080000U /* SIM_SOPT1 */
/* SIM_SOPT2:
 * EMVSIMSRC=0,SDHCSRC=0,LPUARTSRC=0,TPMSRC=1,FLEXIOSRC=0,USBSRC=0,PLLFLLSEL=3,TRACECLKSEL=0,FBSL=0,CLKOUTSEL=0,RTCCLKOUTSEL=0
 */
#define SYSTEM_SIM_SOPT2_VALUE 0x01030000U /* SIM_SOPT2 */
#elif(CLOCK_SETUP == 3)
#define DEFAULT_SYSTEM_CLOCK 4000000U /* Default System clock value */
#define MCG_MODE MCG_MODE_BLPE    /* Clock generator mode */
/* MCG_C1: CLKS=2,FRDIV=4,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
#define SYSTEM_MCG_C1_VALUE 0xA2U /* MCG_C1 */
/* MCG_C2: LOCRE0=0,FCFTRIM=0,RANGE=2,HGO=0,EREFS=1,LP=1,IRCS=1 */
#define SYSTEM_MCG_C2_VALUE 0x27U /* MCG_C2 */
/* MCG_C4: DMX32=0,DRST_DRS=0,FCTRIM=0,SCFTRIM=0 */
#define SYSTEM_MCG_C4_VALUE 0x00U /* MCG_C4 */
/* MCG_SC: ATME=0,ATMS=0,ATMF=0,FLTPRSRV=0,FCRDIV=1,LOCS0=0 */
#define SYSTEM_MCG_SC_VALUE 0x02U /* MCG_SC */
/* MCG_C5: PLLCLKEN=0,PLLSTEN=0,PRDIV=0 */
#define SYSTEM_MCG_C5_VALUE 0x00U /* MCG_C5 */
/* MCG_C6: LOLIE0=0,PLLS=0,CME0=0,VDIV=0 */
#define SYSTEM_MCG_C6_VALUE 0x00U /* MCG_C6 */
/* MCG_C7: OSCSEL=0 */
#define SYSTEM_MCG_C7_VALUE 0x00U /* MCG_C7 */
/* OSC_CR: ERCLKEN=1,EREFSTEN=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
#define SYSTEM_OSC_CR_VALUE 0x80U /* OSC_CR */
/* SMC_PMCTRL: RUNM=0,STOPA=0,STOPM=0 */
#define SYSTEM_SMC_PMCTRL_VALUE 0x00U /* SMC_PMCTRL */
/* SIM_CLKDIV1: OUTDIV1=0,OUTDIV2=3,OUTDIV4=3,OUTDIV5=0x0B */
#define SYSTEM_SIM_CLKDIV1_VALUE 0x03030000U /* SIM_CLKDIV1 */
/* SIM_CLKDIV3: PLLFLLDIV=0,PLLFLLFRAC=0 */
#define SYSTEM_SIM_CLKDIV3_VALUE 0x00U /* SIM_CLKDIV3 */
/* SIM_SOPT1: USBREGEN=0,USBSSTBY=0,USBVSTBY=0,OSC32KSEL=2,RAMSIZE=0 */
#define SYSTEM_SIM_SOPT1_VALUE 0x00080000U /* SIM_SOPT1 */
/* SIM_SOPT2:
 * EMVSIMSRC=0,SDHCSRC=0,LPUARTSRC=0,TPMSRC=1,FLEXIOSRC=0,USBSRC=0,PLLFLLSEL=3,TRACECLKSEL=0,FBSL=0,CLKOUTSEL=0,RTCCLKOUTSEL=0
 */
#define SYSTEM_SIM_SOPT2_VALUE 0x01030000U /* SIM_SOPT2 */
#elif(CLOCK_SETUP == 4)
#define DEFAULT_SYSTEM_CLOCK 96000000U /* Default System clock value */
#define MCG_MODE MCG_MODE_PEE     /* Clock generator mode */
/* MCG_C1: CLKS=0,FRDIV=4,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
#define SYSTEM_MCG_C1_VALUE 0x22U /* MCG_C1 */
/* MCG_C2: LOCRE0=0,FCFTRIM=0,RANGE=2,HGO=0,EREFS=1,LP=0,IRCS=0 */
#define SYSTEM_MCG_C2_VALUE 0x24U /* MCG_C2 */
/* MCG_C4: DMX32=0,DRST_DRS=0,FCTRIM=0,SCFTRIM=0 */
#define SYSTEM_MCG_C4_VALUE 0x00U /* MCG_C4 */
/* MCG_SC: ATME=0,ATMS=0,ATMF=0,FLTPRSRV=0,FCRDIV=0,LOCS0=0 */
#define SYSTEM_MCG_SC_VALUE 0x00U /* MCG_SC */
/* MCG_C5: PLLCLKEN=0,PLLSTEN=0,PRDIV=0 */
#define SYSTEM_MCG_C5_VALUE 0x00U /* MCG_C5 */
/* MCG_C6: LOLIE0=0,PLLS=1,CME0=0,VDIV=0 */
#define SYSTEM_MCG_C6_VALUE 0x40U /* MCG_C6 */
/* MCG_C7: OSCSEL=0 */
#define SYSTEM_MCG_C7_VALUE 0x00U /* MCG_C7 */
/* OSC_CR: ERCLKEN=1,EREFSTEN=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
#define SYSTEM_OSC_CR_VALUE 0x80U /* OSC_CR */
/* SMC_PMCTRL: RUNM=3,STOPA=0,STOPM=0 */
#define SYSTEM_SMC_PMCTRL_VALUE 0x60U /* SMC_PMCTRL */
/* SIM_CLKDIV1: OUTDIV1=0,OUTDIV2=3,OUTDIV4=3,OUTDIV5=0 */
#define SYSTEM_SIM_CLKDIV1_VALUE 0x03030000U /* SIM_CLKDIV1 */
/* SIM_CLKDIV2: USBDIV=3,USBFRAC=0 */
#define SYSTEM_SIM_CLKDIV2_VALUE 0x03U       /* SIM_CLKDIV2 */
/* SIM_CLKDIV3: PLLFLLDIV=0,PLLFLLFRAC=0 */
#define SYSTEM_SIM_CLKDIV3_VALUE 0x00U /* SIM_CLKDIV3 */
/* SIM_SOPT1: USBREGEN=0,USBSSTBY=0,USBVSTBY=0,OSC32KSEL=2,RAMSIZE=0 */
#define SYSTEM_SIM_SOPT1_VALUE 0x00080000U /* SIM_SOPT1 */
/* SIM_SOPT2:
 * EMVSIMSRC=0,SDHCSRC=0,LPUARTSRC=0,TPMSRC=1,FLEXIOSRC=0,USBSRC=0,PLLFLLSEL=1,TRACECLKSEL=0,FBSL=0,CLKOUTSEL=0,RTCCLKOUTSEL=0
 */
#define SYSTEM_SIM_SOPT2_VALUE 0x01010000U /* SIM_SOPT2 */
#elif(CLOCK_SETUP == 5)
#define DEFAULT_SYSTEM_CLOCK 72000000U /* Default System clock value */
#define MCG_MODE MCG_MODE_PEE     /* Clock generator mode */
/* MCG_C1: CLKS=0,FRDIV=4,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
#define SYSTEM_MCG_C1_VALUE 0x22U /* MCG_C1 */
/* MCG_C2: LOCRE0=0,FCFTRIM=0,RANGE=2,HGO=0,EREFS=1,LP=0,IRCS=0 */
#define SYSTEM_MCG_C2_VALUE 0x24U /* MCG_C2 */
/* MCG_C4: DMX32=0,DRST_DRS=0,FCTRIM=0,SCFTRIM=0 */
#define SYSTEM_MCG_C4_VALUE 0x00U /* MCG_C4 */
/* MCG_SC: ATME=0,ATMS=0,ATMF=0,FLTPRSRV=0,FCRDIV=0,LOCS0=0 */
#define SYSTEM_MCG_SC_VALUE 0x00U /* MCG_SC */
/* MCG_C5: PLLCLKEN=0,PLLSTEN=0,PRDIV=0 */
#define SYSTEM_MCG_C5_VALUE 0x00U /* MCG_C5 */
/* MCG_C6: LOLIE0=0,PLLS=1,CME0=0,VDIV=8 */
#define SYSTEM_MCG_C6_VALUE 0x48U /* MCG_C6 */
/* MCG_C7: OSCSEL=0 */
#define SYSTEM_MCG_C7_VALUE 0x00U /* MCG_C7 */
/* OSC_CR: ERCLKEN=1,EREFSTEN=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
#define SYSTEM_OSC_CR_VALUE 0x80U /* OSC_CR */
/* SMC_PMCTRL: RUNM=0,STOPA=0,STOPM=0 */
#define SYSTEM_SMC_PMCTRL_VALUE 0x00U /* SMC_PMCTRL */
/* SIM_CLKDIV1: OUTDIV1=1,OUTDIV2=5,OUTDIV4=5,OUTDIV5=1 */
#define SYSTEM_SIM_CLKDIV1_VALUE 0x15051000U /* SIM_CLKDIV1 */
/* SIM_CLKDIV3: PLLFLLDIV=0,PLLFLLFRAC=0 */
#define SYSTEM_SIM_CLKDIV3_VALUE 0x00U /* SIM_CLKDIV3 */
/* SIM_SOPT1: USBREGEN=0,USBSSTBY=0,USBVSTBY=0,OSC32KSEL=2,RAMSIZE=0 */
#define SYSTEM_SIM_SOPT1_VALUE 0x00080000U /* SIM_SOPT1 */
/* SIM_SOPT2:
 * EMVSIMSRC=0,SDHCSRC=0,LPUARTSRC=0,TPMSRC=1,FLEXIOSRC=0,USBSRC=0,PLLFLLSEL=1,TRACECLKSEL=0,FBSL=0,CLKOUTSEL=0,RTCCLKOUTSEL=0
 */
#define SYSTEM_SIM_SOPT2_VALUE 0x01010000U /* SIM_SOPT2 */
#endif
#else
#define DEFAULT_SYSTEM_CLOCK 20971520u
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
void SystemInit(void);

/**
 * @brief Updates the SystemCoreClock variable.
 *
 * It must be called whenever the core clock is changed during program
 * execution. SystemCoreClockUpdate() evaluates the clock register settings and calculates
 * the current core clock.
 */
void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_MKL82Z7_H_ */
