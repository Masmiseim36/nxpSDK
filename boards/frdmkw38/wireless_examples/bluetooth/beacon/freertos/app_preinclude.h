/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*!
 * Copyright 2019-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

/*!
 *  Application specific configuration file only
 *  Board Specific Configuration shall be added to board.h file directly such as :
 *  - Number of button on the board,
 *  - Number of LEDs,
 *  - etc...
 */
/*! *********************************************************************************
 *     Board Configuration
 ********************************************************************************** */
 /* Defines the number of required keys for the keyboard module */
#define gKBD_KeysCount_c        2

/* Specifies the number of required LEDs for the LED module */
#define gLEDsOnTargetBoardCnt_c 4

/* Specifies if the LED operation is inverted. LED On = GPIO Set */
#define gLED_InvertedMode_d     1

/*! *********************************************************************************
 * 	App Configuration
 ********************************************************************************** */
/* Enable Extended Advertising */
#define gBeaconAE_c                   1

#if gBeaconAE_c
/* Use very large extended advertising data */
#define gBeaconLargeExtAdvData_c       0

/*! Set the Tx power in dBm */
#define mAdvertisingDefaultTxPower_c    0
#endif /* gBeaconAE_c */

/*! Repeated Attempts - Mitigation for pairing attacks */
#define gRepeatedAttempts_d             0
/*! *********************************************************************************
 *     Framework Configuration
 ********************************************************************************** */
/* Do not modify. Not used for this application */
#define gAppUseNvm_d                    1

/* Defines Num of Serial Manager interfaces */
#define gSerialManagerMaxInterfaces_c   0

/* Defines Size for Timer Task*/
#define gTmrTaskStackSize_c  500

#if (gBeaconAE_c && gBeaconLargeExtAdvData_c)
/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define AppPoolsDetails_c \
         _block_size_  72  _number_of_blocks_    3 _eol_  \
         _block_size_ 128  _number_of_blocks_    2 _eol_  \
         _block_size_ 264  _number_of_blocks_    2 _eol_  \
         _block_size_ 1680  _number_of_blocks_   1 _eol_
#else
/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define AppPoolsDetails_c \
         _block_size_   72  _number_of_blocks_    3 _eol_  \
         _block_size_  256  _number_of_blocks_    2 _eol_
#endif

/* Defines number of timers needed by the application */
#if gRepeatedAttempts_d
#define gTmrApplicationTimers_c         5
#else
#define gTmrApplicationTimers_c         4
#endif

/* Set this define TRUE if the PIT frequency is an integer number of MHZ */
#define gTMR_PIT_FreqMultipleOfMHZ_d    0

/* Enables / Disables the precision timers platform component */
#define gTimestamp_Enabled_d            0

/* Check Low Power Timer */
#define cPWR_CheckLowPowerTimers        1

/* Enable/Disable Low Power Timer */
#define gTMR_EnableLowPowerTimers_d     1

/* Enable/Disable PowerDown functionality in PwrLib */
#define cPWR_UsePowerDownMode           1

/* Enable/Disable BLE Link Layer DSM */
#define cPWR_BLE_LL_Enable              1

/* Default Deep Sleep Mode*/
#define cPWR_DeepSleepMode              5

/* Enable/Disable MCU Sleep During BLE Events */
#define cMCU_SleepDuringBleEvents       1

/* Default deep sleep duration in ms */
#define cPWR_DeepSleepDurationMs        30000

/* Number of slots(625us) before the wake up instant before which the hardware needs to exit from deep sleep mode. */
#if (gBeaconAE_c && gBeaconLargeExtAdvData_c)
/* need 3 more slots to reload large extended advertising data */
#define cPWR_BLE_LL_OffsetToWakeupInstant 6
#else
#define cPWR_BLE_LL_OffsetToWakeupInstant 3
#endif

/* Application Connection sleep mode */
#define gAppDeepSleepMode_c             8

/* Disable unused LowPower modes */
#define cPWR_EnableDeepSleepMode_1      1
#define cPWR_EnableDeepSleepMode_2      0
#define cPWR_EnableDeepSleepMode_3      0
#define cPWR_EnableDeepSleepMode_4      0
#define cPWR_EnableDeepSleepMode_5      1
#define cPWR_EnableDeepSleepMode_7      0
#define cPWR_EnableDeepSleepMode_8      1

/* Enables / Disables MWS coexistence */
#define gMWS_UseCoexistence_d           0

/* Enables / Disables the DCDC platform component */
#define gDCDC_Enabled_d                 1

/*! Default DCDC Mode used by the application gDCDC_Buck_c or gDCDC_Bypass_c */
#define APP_DCDC_MODE                   gDCDC_Buck_c

/* Default DCDC Battery Level Monitor interval */
#define APP_DCDC_VBAT_MONITOR_INTERVAL  600000

/*! Store RNG seed to allow faster wakeup time from RAM OFF
    Seed location in HW parameters */
#define gRngSeedHwParamStorage_d        1

/*! Enable XCVR calibration storage in Flash */
#define gControllerPreserveXcvrDacTrimValue_d     1

/*! Enable the SWD pins to be managed into low-power */
#define gBoard_ManageSwdPinsInLowPower_d    1

/*! *********************************************************************************
 *     RTOS Configuration
 ********************************************************************************** */
/* Defines the RTOS used */
#define FSL_RTOS_FREE_RTOS      1

/* Defines number of OS events used */
#define osNumberOfEvents        5

/* Defines main task stack size */
#define gMainThreadStackSize_c  900

/*! Defines Size for Idle Task  (minimum 572 is required) */
#define gAppIdleTaskStackSize_c         600

/* Defines total heap size used by the OS */
#define gTotalHeapSize_c        8500

/*! *********************************************************************************
 *     BLE Stack Configuration
 ********************************************************************************** */


/*! *********************************************************************************
 *     BLE LL Configuration
 ***********************************************************************************/
/*  ble_ll_config.h file lists the parameters with their default values. User can override
 *    the parameter here by defining the parameter to a user defined value. */
#define gAppExtAdvEnable_d              1
#define gLlMaxUsedAdvSet_c              2
#define gLlUsePeriodicAdvertising_d     1
#if gBeaconLargeExtAdvData_c
#define gLlMaxExtAdvDataLength_c        1650U
#else
#define gLlMaxExtAdvDataLength_c        250U
#endif /* gBeaconLargeExtAdvData_c */

/*
 * Specific configuration of LL pools by block size and number of blocks for this application.
 * Optimized using the MEM_OPTIMIZE_BUFFER_POOL feature in MemManager,
 * we find that the most optimized combination for LL buffers.
 *
 * If LlPoolsDetails_c is not defined, default LL buffer configuration in app_preinclude_common.h
 * will be applied.
 */

/* Include common configuration file and board configuration file */
#include "app_preinclude_common.h"
#endif /* _APP_PREINCLUDE_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
