/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*!
 * Copyright 2020 NXP
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
 *     App Configuration
 ********************************************************************************** */
/*enable/disable memory debug */     
/*#define MEM_TRACKING 1*/
/*#define MEM_STATISTICS 1*/
/*#define MEM_DEBUG 1*/
/*#define MEM_DEBUG_OUT_OF_MEMORY 1*/
/*#define TMR_DEBUG_ALLOC 1*/
/*#define gUsePanic_c 1*/
/*#define gFLib_CheckBufferOverflow_d 1 */     

/*Enable Extended Advertising*/
#define gAppExtAdvEnable_d                      1      
     
/*! Enable/disable printing debug information*/      
#define mAE_PeripheralDebug_c   0     
     
/*define the max number of connections this device is able to handle*/     
#define gAppMaxConnections_c           (2U)     
     
/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d   1

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d   1

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d   0

#define gPasskeyValue_c                999999

/*! Set the Tx power in dBm */
#define mAdvertisingDefaultTxPower_c    0

/*! specifies whether the phy update procedure is going to be initiated in connection or not */
#define gConnInitiatePhyUpdateRequest_c            (0U)

/*! Repeated Attempts - Mitigation for pairing attacks */
#define gRepeatedAttempts_d             0
/*! *********************************************************************************
 *     Framework Configuration
 ********************************************************************************** */
/* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d                    1

/*! Enable Serial Console with external device or host using the LPUART
    Define the Number of Serial Manager interfaces - typical value is 0 or 1
    Lowpower does not support more than 1 serial interface
    Warning   : Memory usage is increased  when set to 0 , roughly :
    1KB of RAM, several KB of Flash (depend of the string size)     */
#define gSerialManagerMaxInterfaces_c   1

/* Prevent the MCU to enter Sleep during serial data transmission */
#define gSerialMgr_DisallowMcuSleep_d   1

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define AppPoolsDetails_c \
         _block_size_  80  _number_of_blocks_    10 _eol_  \
         _block_size_ 280   _number_of_blocks_   4 _eol_  \
         _block_size_ 312  _number_of_blocks_    1 _eol_  \
         _block_size_ 392  _number_of_blocks_    1 _eol_  \
         _block_size_ 1680  _number_of_blocks_   1 _eol_    

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

/*! Enable/Disable Low Power Timer */
#define gTMR_EnableLowPowerTimers_d     1

/* Enable/Disable PowerDown functionality in PwrLib */
#define cPWR_UsePowerDownMode           1

/* Enable/Disable BLE Link Layer DSM */
#define cPWR_BLE_LL_Enable              1

/* Default Deep Sleep Mode*/
#define cPWR_DeepSleepMode              3

/* Enable/Disable MCU Sleep During BLE Events */
#define cMCU_SleepDuringBleEvents       1

/* Default deep sleep duration in ms */
#define cPWR_DeepSleepDurationMs        30000

/* Number of slots(625us) before the wake up instant before which the hardware needs to exit from deep sleep mode. */
#define cPWR_BLE_LL_OffsetToWakeupInstant 3

/* Application Connection sleep mode */
#define gAppDeepSleepMode_c             1

/* Disable unused LowPower modes */
#define cPWR_EnableDeepSleepMode_1      1
#define cPWR_EnableDeepSleepMode_2      0
#define cPWR_EnableDeepSleepMode_3      1
#define cPWR_EnableDeepSleepMode_4      0
#define cPWR_EnableDeepSleepMode_5      0
#define cPWR_EnableDeepSleepMode_7      0
#define cPWR_EnableDeepSleepMode_8      0

/* Enables / Disables MWS coexistence */
#define gMWS_UseCoexistence_d           0

/*! Enables / Disables the DCDC platform component */
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
/* Defines number of OS events used */
#define osNumberOfEvents        5

/*! *********************************************************************************
 *     BLE Stack Configuration
 ********************************************************************************** */


/*! *********************************************************************************
 *     BLE LL Configuration
 ***********************************************************************************/
/*  ble_ll_config.h file lists the parameters with their default values. User can override
 *    the parameter here by defining the parameter to a user defined value. */

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
