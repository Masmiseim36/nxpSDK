/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
* All rights reserved.
 *
 * \file
 *
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _PWR_CONFIGURATION_H_
#define _PWR_CONFIGURATION_H_

/*****************************************************************************
 *                               INCLUDED HEADERS                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the headers that this module needs to include.    *
 * Note that it is not a good practice to include header files into header   *
 * files, so use this section only if there is no other better solution.     *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#include "TimersManager.h"
#include "DCDC.h"

/************************************************************************************
 *************************************************************************************
 * Module configuration constants
 *************************************************************************************
 ************************************************************************************/

/*********************/
/* LVD configuration */
/*********************/

/*-----------------------------------------------------------------------------/
 * The use of Low Voltage detection has the following possibilities:
 *   0: Don't use Low voltage detection at all
 *   1: Use polled => Check made each time the function is called.
 *   2: A minutes software timer used for handling when to poll LVD, according
 *      to the cPWR_LVD_Ticks constant
 *   3: LVDRE  are set to hold MCU in reset while VLVDL  condition is detected
 * PEX Settings: - in modes 0,1,2 the property CPU->Internal peripherals->Power management controller->LVDreset must be Disabled
 *               - in mode 3 the property CPU->Internal peripherals->Power management controller->LVDreset must be Enabled
 * The the propery refers to LVDRE bit in the PMC_LVDSC1 register which is a write once bit, so it cannot be modified after that.
 */

#ifndef cPWR_LVD_Enable
#define cPWR_LVD_Enable                  0
#endif

/*-----------------------------------------------------------------------------
 * How often to check the LVD level when cPWR_LVD_Enable == 2
 * This is the number of minutes before voltage is checked (Consumes
 * current and time)
 */
#ifndef cPWR_LVD_Ticks
#define cPWR_LVD_Ticks                   60
#endif

/*-----------------------------------------------------------------------------
 * To enable/disable all of the code in this PWR/PWRLib files.
 *   TRUE =  1: Use PowerDown functions (Normal)
 *   FALSE = 0: Don't use PowerDown. Will cut variables and code out. But
 *              functions still exist. Useful for debugging and test purposes
 */
#ifndef cPWR_UsePowerDownMode
#define cPWR_UsePowerDownMode            FALSE
#endif

/* This define enables BLE_LL DSM functionalities. Functions from BLE stack are called, so the BLE stack must be
 * linked in the project if you set it TRUE. It must be TRUE to use modes 1 and 2.
 */
#ifndef cPWR_BLE_LL_Enable
#define cPWR_BLE_LL_Enable               FALSE
#endif

/* This define enables Z_LL DSM functionalities. No ZLL DSM functionalities are currently implemented,
 * so using it doesn't change  a thing
 */
#ifndef cPWR_Z_LL_Enable
#define cPWR_Z_LL_Enable                 FALSE
#endif

/* This define enables ANT_LL DSM functionalities. No ANT DSM functionalities are currently implemented,
 * so using it doesn't change  a thing
 */
#ifndef cPWR_ANT_LL_Enable
#define cPWR_ANT_LL_Enable               FALSE
#endif

/* This define enables GENFSK_LL DSM functionalities. */
#ifndef cPWR_GENFSK_LL_Enable
#define cPWR_GENFSK_LL_Enable           FALSE
#endif

/* This define enables some legacy ZigBee stack related checks before entering low power. It is provided to allow
 *a hypothetical demand to get the legacy ZigBee stack running on KW41.
 */
#ifndef cPWR_Zigbee_Enable
#define cPWR_Zigbee_Enable               FALSE
#endif

/*-----------------------------------------------------------------------------
 * Enable/disable the TMR table check for the first low power timer programmed to expire.
 *   TRUE =  1: Check the TMR table for the first LowPower timer to expire
 *   FALSE = 0: Don't check the TMR table
 */
#ifndef cPWR_CheckLowPowerTimers
#define cPWR_CheckLowPowerTimers         FALSE
#endif

/*-----------------------------------------------------------------------------
 * Enable/Disable dedicated stack for the warmboot sequence (LP mode 8)
 */
#ifndef USE_WARMBOOT_SP
#define USE_WARMBOOT_SP 1
#endif

/*-----------------------------------------------------------------------------
 * The way that DeepSleep mode are handled. Following possibilities exist:
 *******************************************************************************/
#define PWR_LOWPOWER_INVALID_MODE               0U

 /*******************************************************************************
 *   1: MCU/Radio low power modes: This is a BLE DSM specific low power mode. It requires cPWR_BLE_LL_Enable TRUE.
 *        MCU in LLS3 mode.
 *        BLE_LL in DSM.
 *      Wakeup sources:
 *        GPIO (push button) interrupt using LLWU module(SW2(PTA18),SW3(PTA19)).
 *        BLE_LL wake up interrupt(BLE_LL reference clock reaches wake up instance register)  using LLWU module.
 *          - BTE_LL wakeup timeout: controlled by the BLE stack(SoC must be awake before next BLE action).
 *          - BTE_LL reference clock source:   32Khz oscillator
 *          - BTE_LL reference clock resolution:     625us
 *        LPTMR wakeup timeout: cPWR_DeepSleepDurationMs by default. Use PWR_SetDeepSleepTimeInMs
 *                                  to change it at run time. Maximum timeout is 65535000 ms (18.2 h).
 *          - LPTMR clock source:   32Khz oscillator
 *          - LPTMR resolution:     modified at run time to meet timeout value.
 *      RAM retention:
 *          - 16k Ram retention (0x20000000- 0x20003fff)
 *          - 32k Ram retention (0x20000000- 0x20007fff)
 *          - Full Ram retention in VLLS3
 *******************************************************************************/
#define PWR_LOWPOWER_LLS2_3_LL_DSM_MODE          1U

/*******************************************************************************
 *   2/3: MCU/Radio low power modes: This is a non DSM general purpose low power mode.
 *        MCU in LLS3 mode.
 *        BLE_LL must be idle or cPWR_BLE_LL_Enable FALSE.
 *        Z_LL must be idle or cPWR_Z_LL_Enable FALSE.
 *        ANT_LL must be idle or cPWR_ANT_LL_Enable FALSE.
 *        GEN_LL must be idle or cPWR_GENFSK_LL_Enable FALSE.
 *      Wakeup sources:
 *        GPIO (push button) interrupt using LLWU module(SW2(PTA18),SW3(PTA19)).
 *        DCDC PowerSwitch - available in buck mode only.
 *        LPTMR interrupt using LLWU module
 *          - LPTMR wakeup timeout: cPWR_DeepSleepDurationMs by default. Use PWR_SetDeepSleepTimeInMs
 *                                  to change it at run time. Maximum timeout is 65535000 ms (18.2 h).
 *          - LPTMR clock source:   32Khz oscillator
 *          - LPTMR resolution:     modified at run time to meet timeout value.
 *      RAM retention:
 *          - 16k Ram retention (0x20000000- 0x20003fff)
 *          - 32k Ram retention (0x20000000- 0x20007fff)
 *          - Full Ram retention in VLLS3
 *******************************************************************************/
#define PWR_LOWPOWER_LLS2_3_NO_DSM_MODE         3U

/*******************************************************************************
 *   4: MCU/Radio low power modes: This is a non DSM general purpose low power mode.
 *        BLE_LL must be idle or cPWR_BLE_LL_Enable FALSE.
 *        Z_LL must be idle or cPWR_Z_LL_Enable FALSE.
 *        ANT_LL must be idle or cPWR_ANT_LL_Enable FALSE.
 *        GEN_LL must be idle or cPWR_GENFSK_LL_Enable FALSE.
 *        MCU in VLLS0/1 mode(VLLS0 if DCDC bypassed/ VLLS1 otherwise ).
 *      Wakeup sources:
 *        GPIO (push button) interrupt using LLWU module(SW2(PTA18),SW3(PTA19)).
 *        DCDC PowerSwitch - available in buck mode only.
 *******************************************************************************/
#define PWR_LOWPOWER_VLLS0_1_PSWITCH_MODE       4U

 /*******************************************************************************
 *   5: MCU/Radio low power modes: This is a non DSM general purpose low power mode.
 *        MCU in VLLS2 (16k Ram retention (0x20000000- 0x20003fff)).
 *        BLE_LL must be idle or cPWR_BLE_LL_Enable FALSE.
 *        Z_LL must be idle or cPWR_Z_LL_Enable FALSE.
 *        ANT_LL must be idle or cPWR_ANT_LL_Enable FALSE.
 *        GEN_LL must be idle or cPWR_GENFSK_LL_Enable FALSE.
 *      Wakeup sources:
 *        GPIO (push button) interrupt using LLWU module(SW2(PTA18),SW3(PTA19)).
 *        DCDC PowerSwitch - available in buck mode only.
 *******************************************************************************/
#define PWR_LOWPOWER_VLLS2_3_NO_DSM_MODE       5U

/*******************************************************************************
 *   6: MCU/Radio low power modes:
 *        MCU in STOP.
 *      Wakeup sources:
 *        GPIO (push button) interrupt using LLWU module(SW2(PTA18),SW3(PTA19)).
 *        DCDC PowerSwitch - available in buck mode only.
 *        LPTMR wakeup timeout: cPWR_DeepSleepDurationMs by default. Use PWR_SetDeepSleepTimeInMs
 *                                  to change it at run time. Maximum timeout is 65535000 ms (18.2 h).
 *          - LPTMR clock source:   32Khz oscillator
 *          - LPTMR resolution:     modified at run time to meet timeout value.
 *        Any Radio interrupt
 *        UART
 *******************************************************************************/
#define PWR_LOWPOWER_MCU_STOP_MODE             6U

 /*******************************************************************************
 *   7: MCU/Radio low power modes: This is a GENFSK DSM specific low power mode. It requires cPWR_GENFSK_LL_Enable TRUE.
 *        MCU in LLS3 mode.
 *        GENFSK_LL in DSM.
 *      Wakeup sources:
 *        GPIO (push button) interrupt using LLWU module(SW2(PTA18),SW3(PTA19)).
 *        GENFSK_LL wake up interrupt(GENFSK_LL reference clock reaches wake up instance register) using LLWU module.
 *          - GENFSK_LL reference clock source:   32Khz oscillator
 *          - GENFSK_LL reference clock resolution:     ~30.5us
 *        LPTMR wakeup timeout: cPWR_DeepSleepDurationMs by default. Use PWR_SetDeepSleepTimeInMs
 *                                  to change it at run time. Maximum timeout is 65535000 ms (18.2 h).
 *          - LPTMR clock source:   32Khz oscillator
 *          - LPTMR resolution:     modified at run time to meet timeout value.
 *******************************************************************************
 *******************************************************************************
 *   8: MCU/Radio low power modes: This is a BLE DSM specific low power mode. It requires cPWR_BLE_LL_Enable TRUE.
 *        MCU in VLLS2/3 mode.
 *        BLE_LL in DSM.
 *      Wakeup sources:
 *        GPIO (push button) interrupt using LLWU module(SW2(PTA18),SW3(PTA19)).
 *        BLE_LL wake up interrupt(BLE_LL reference clock reaches wake up instance register)  using LLWU module.
 *          - BTE_LL wakeup timeout: controlled by the BLE stack(SoC must be awake before next BLE action).
 *          - BTE_LL reference clock source:   32Khz oscillator
 *          - BTE_LL reference clock resolution:     625us
 *        LPTMR wakeup timeout: cPWR_DeepSleepDurationMs by default. Use PWR_SetDeepSleepTimeInMs
 *                                  to change it at run time. Maximum timeout is 65535000 ms (18.2 h).
 *          - LPTMR clock source:   32Khz oscillator
 *          - LPTMR resolution:     modified at run time to meet timeout value.
 *      RAM retention:
 *          - 16k Ram retention (0x20000000- 0x20003fff)
 *          - 32k Ram retention (0x20000000- 0x20007fff)
 *          - Full Ram retention in VLLS3
 *******************************************************************************/
#define PWR_LOWPOWER_VLLS2_3_LL_DSM3_MODE      8U

/*******************************************************************************
 *   9: MCU/Radio low power modes: This is a BLE DSM specific low power mode. It requires cPWR_BLE_LL_Enable TRUE.
 *        MCU in VLLS2/3 mode.
 *        BLE_LL in Idle.
 *      Wakeup sources:
 *        GPIO (push button) interrupt using LLWU module(SW2(PTA18),SW3(PTA19)).
 *        LPTMR wakeup timeout: cPWR_DeepSleepDurationMs by default. Use PWR_SetDeepSleepTimeInMs
 *                                  to change it at run time. Maximum timeout is 65535000 ms (18.2 h).
 *          - LPTMR clock source:   32Khz oscillator
 *          - LPTMR resolution:     modified at run time to meet timeout value.
 *      RAM retention:
 *          - 16k Ram retention (0x20000000- 0x20003fff)
 *          - 32k Ram retention (0x20000000- 0x20007fff)
 *          - Full Ram retention in VLLS3
 *******************************************************************************/
#define PWR_LOWPOWER_VLLS2_3_LL_RESET_ADV_MODE      9U

#define PWR_LOWPOWER_MODE_MAX                       PWR_LOWPOWER_VLLS2_3_LL_RESET_ADV_MODE

#ifndef cPWR_DeepSleepMode
#define cPWR_DeepSleepMode                     1
#endif

#ifndef cPWR_EnableDeepSleepMode_1
#define cPWR_EnableDeepSleepMode_1             1
#endif

#ifndef cPWR_EnableDeepSleepMode_2
#define cPWR_EnableDeepSleepMode_2             0
#endif

#ifndef cPWR_EnableDeepSleepMode_3
#define cPWR_EnableDeepSleepMode_3             1
#endif

#ifndef cPWR_EnableDeepSleepMode_4
#define cPWR_EnableDeepSleepMode_4             1
#endif

#ifndef cPWR_EnableDeepSleepMode_5
#define cPWR_EnableDeepSleepMode_5             1
#endif

#ifndef cPWR_EnableDeepSleepMode_6
#define cPWR_EnableDeepSleepMode_6             1
#endif

#ifndef cPWR_EnableDeepSleepMode_7
#define cPWR_EnableDeepSleepMode_7             0
#endif

#ifndef cPWR_EnableDeepSleepMode_8
#define cPWR_EnableDeepSleepMode_8             1
#endif

#ifndef cPWR_EnableDeepSleepMode_9
#define cPWR_EnableDeepSleepMode_9             0
#endif

/*-----------------------------------------------------------------------------
 * The deep sleep duration in ms.
 */
#ifndef cPWR_DeepSleepDurationMs
#define cPWR_DeepSleepDurationMs               3000
#endif

/*-----------------------------------------------------------------------------
 * BLE LL DEEP SLEEP MODE DEFINES
 */
/* Number of slots(625us) before the wake up instant before which the hardware needs to exit from deep sleep mode. */
#ifndef cPWR_BLE_LL_OffsetToWakeupInstant
#define cPWR_BLE_LL_OffsetToWakeupInstant      (3U)
#endif

/* Oscillator stabilization/startup delay. This is in X.Y for-mat where X is in terms of number of BT slots (625 us) and Y is in terms of number of clock periods of 16KHz clock input, required for RF oscillator to stabilize the clock output to the controller on its output pin, after os-cillator is turned ON. In this period the clock is assumed to be unstable, and so the controller does not turn on the clock to internal logic till this period is over. This means, the wake up from deep sleep mode must ac-count for this delay before the wakeup instant.
 * Osc_startup_delay[7:5] is number of slots(625us)
 * Osc_startup_delay[4:0 is number of clock periods of 16KHz clock
 * (Warning: Max. value of Osc_startup_delay [4:0] sup-ported is 9. Therefore do not program value greater than 9)
 */
#ifndef cPWR_BLE_LL_OscStartupDelayVlls
#define cPWR_BLE_LL_OscStartupDelayVlls        (0x23U)
#endif

#ifndef cPWR_BLE_LL_OscStartupDelayLls
#define cPWR_BLE_LL_OscStartupDelayLls         (0x23U)
#endif


/* Oscillator stabilization/startup delay is reduced when accuracy is not critical. Applied while advertising
 * This is in X.Y for-mat where X is in terms of number of BT slots (625 us) and Y is in terms of number of clock periods of 16KHz clock input, required for RF oscillator to stabilize the clock output to the controller on its output pin, after os-cillator is turned ON. In this period the clock is assumed to be unstable, and so the controller does not turn on the clock to internal logic till this period is over. This means, the wake up from deep sleep mode must ac-count for this delay before the wakeup instant.
 * Osc_startup_delay[7:5] is number of slots(625us)
 * Osc_startup_delay[4:0 is number of clock periods of 16KHz clock
 * (Warning: Max. value of Osc_startup_delay [4:0] sup-ported is 9. Therefore do not program value greater than 9)
 */
#ifndef cPWR_BLE_LL_OscStartupDelayVllsAdv
#define cPWR_BLE_LL_OscStartupDelayVllsAdv    (0x21U)
#endif

#ifndef cPWR_BLE_LL_OscStartupDelayLlsAdv
#define cPWR_BLE_LL_OscStartupDelayLlsAdv     (0x21U)
#endif

#ifndef cPWR_POR_DisabledInVLLS0
#define cPWR_POR_DisabledInVLLS0              (1)
#endif

#ifndef cPWR_PwrUseBlpeMode
#define cPWR_PwrUseBlpeMode                    0U
#endif

#ifndef cPWR_UseDefaultClockInLLS
#define cPWR_UseDefaultClockInLLS              0U
#endif

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
#ifndef PWRLIB_RAM_SAVE_COUNT_DOWN
#define PWRLIB_RAM_SAVE_COUNT_DOWN             1u
#endif

#ifndef PWRLib_LPTMR_KEEP_RUNNING
#define PWRLib_LPTMR_KEEP_RUNNING
#endif

#if defined(gAppUseBonding_d) && ( gAppUseBonding_d == 1 )
#define gPWR_RamOffRestoreBonding_d            1u
#endif
#endif   // defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)

/*-----------------------------------------------------------------------------*/
#if (cPWR_LVD_Enable > 3)
#error "*** ERROR: Illegal value in cPWR_LVD_Enable"
#endif

#if (cPWR_LVD_Enable == 2)
#if (gTMR_Enabled_d != TRUE)
#error "*** ERROR: Illegal value in cPWR_LVD_Enable"
#endif
#endif

#if (cPWR_UsePowerDownMode > 1)
#error "*** ERROR: Illegal value in cPWR_UsePowerDownMode"
#endif

#if (cPWR_UsePowerDownMode == 1)
#if (!defined(gDCDC_Enabled_d) || (gDCDC_Enabled_d == 0)) && (APP_DCDC_MODE == gDCDC_Buck_c)
#error DCDC module shall be enabled in lowpower in DCDC buck mode
#endif

#if (cPWR_DeepSleepMode > PWR_LOWPOWER_MODE_MAX)
#error "*** ERROR: Illegal value in cPWR_DeepSleepMode"
#endif

#if ((cPWR_EnableDeepSleepMode_1 == 0) && (cPWR_DeepSleepMode == 1))
#error "*** ERROR: Illegal value in cPWR_DeepSleepMode"
#endif
#if ((cPWR_EnableDeepSleepMode_2 == 0) && (cPWR_DeepSleepMode == 2))
#error "*** ERROR: Illegal value in cPWR_DeepSleepMode"
#endif
#if ((cPWR_EnableDeepSleepMode_3 == 0) && (cPWR_DeepSleepMode == 3))
#error "*** ERROR: Illegal value in cPWR_DeepSleepMode"
#endif
#if ((cPWR_EnableDeepSleepMode_4 == 0) && (cPWR_DeepSleepMode == 4))
#error "*** ERROR: Illegal value in cPWR_DeepSleepMode"
#endif
#if ((cPWR_EnableDeepSleepMode_5 == 0) && (cPWR_DeepSleepMode == 5))
#error "*** ERROR: Illegal value in cPWR_DeepSleepMode"
#endif
#if ((cPWR_EnableDeepSleepMode_6 == 0) && (cPWR_DeepSleepMode == 6))
#error "*** ERROR: Illegal value in cPWR_DeepSleepMode"
#endif
#if ((cPWR_EnableDeepSleepMode_7 == 0) && (cPWR_DeepSleepMode == 7))
#error "*** ERROR: Illegal value in cPWR_DeepSleepMode"
#endif
#if ((cPWR_EnableDeepSleepMode_8 == 0) && (cPWR_DeepSleepMode == 8))
#error "*** ERROR: Illegal value in cPWR_DeepSleepMode"
#endif
#if ((cPWR_EnableDeepSleepMode_9 == 0) && (cPWR_DeepSleepMode == 9))
#error "*** ERROR: Illegal value in cPWR_DeepSleepMode"
#endif
#if ( cPWR_EnableDeepSleepMode_8 && !cPWR_EnableDeepSleepMode_1 )
#error "*** cPWR_EnableDeepSleepMode_8 requires cPWR_EnableDeepSleepMode_1"
#endif
#if (!(cPWR_EnableDeepSleepMode_1 || cPWR_EnableDeepSleepMode_2 || cPWR_EnableDeepSleepMode_3 || cPWR_EnableDeepSleepMode_4 || cPWR_EnableDeepSleepMode_5 || cPWR_EnableDeepSleepMode_6 || cPWR_EnableDeepSleepMode_7 || cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_9) && cPWR_UsePowerDownMode)
#error "*** ERROR: cPWR_UsePowerDownMode TRUE requires at least one low power mode to be enabled"
#endif
#endif

#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1)
#if !defined(gTMR_EnableLowPowerTimers_d) || (gTMR_EnableLowPowerTimers_d == 0)
#error lptrm shall be enabled when PWR_EnableRtosSysticks is set to count the number of ticks in lowpower
#endif
#endif

/* Debug MACRO stubbed if not defined */
#ifndef BOARD_DBGLPIOSET
#define BOARD_DBGLPIOSET(__x, __y)
#endif

#ifndef BOARD_DBGSETDBGIOFORLOWPOWER
#define BOARD_DBGSETDBGIOFORLOWPOWER()
#endif

#ifndef PWR_DBG_LOG
#define PWR_DBG_LOG(...)
#endif

#endif /* _PWR_CONFIGURATION_H_ */

