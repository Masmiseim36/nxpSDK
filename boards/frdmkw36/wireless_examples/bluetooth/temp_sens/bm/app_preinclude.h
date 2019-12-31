/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This file is the app configuration file which is pre included.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


#ifndef APP_PREINCLUDE_H
#define APP_PREINCLUDE_H

/*! *********************************************************************************
 *  Board Configuration
 ********************************************************************************** */
 /* Defines the number of available keys for the keyboard module */
#define gKBD_KeysCount_c        2

/* Specifies the number of physical LEDs on the target board */
#define gLEDsOnTargetBoardCnt_c 4

/* Specifies if the LED operation is inverted. LED On = GPIO Set */
#define gLED_InvertedMode_d     1

/*! *********************************************************************************
 *  App Configuration
 ********************************************************************************** */
 
 /*! Maximum number of connections supported for this application */
#define gAppMaxConnections_c           1

 /*! Maximum number of bonded devices supported for this application */
#define gMaxBondedDevices_c            1

 /*! Maximum number of resolving list entries supported for this application */
#define gMaxResolvingListSize_c        1

/*! Number of credit-based channels supported */
#define gL2caMaxLeCbChannels_c         2
 
/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d                1

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d                1

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d                0

#define gPasskeyValue_c                999999

#if (gAppUseBonding_d) && (!gAppUsePairing_d)
  #error "Enable pairing to make use of bonding"
#endif

/*! *********************************************************************************
 *  Framework Configuration
 ********************************************************************************** */
/* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d                    1

/* Prevent the MCU to enter Sleep during serial data transmission */
#define gSerialMgr_DisallowMcuSleep_d   1

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define AppPoolsDetails_c \
         _block_size_  64  _number_of_blocks_    8 _eol_  \
         _block_size_  88  _number_of_blocks_    3 _eol_  \
         _block_size_ 248  _number_of_blocks_    2 _eol_  \
         _block_size_ 312  _number_of_blocks_    1 _eol_  \
         _block_size_ 392  _number_of_blocks_    1 _eol_

/* Defines number of timers needed by the application */
#define gTmrApplicationTimers_c         5

/* Defines number of timers needed by the protocol stack */
#if defined(gAppMaxConnections_c) && defined(gL2caMaxLeCbChannels_c)
    #define gTmrStackTimers_c (3 + (gAppMaxConnections_c * 2) + gL2caMaxLeCbChannels_c)
#else
    #define gTmrStackTimers_c (7)
#endif

/* Set this define TRUE if the PIT frequency is an integer number of MHZ */
#define gTMR_PIT_FreqMultipleOfMHZ_d    0

/* Enables / Disables the precision timers platform component */
#define gTimestamp_Enabled_d            0

/* Check Low Power Timer */
#define cPWR_CheckLowPowerTimers        1

/* Enable/Disable Low Power Timer */
#define gTMR_EnableLowPowerTimers       1

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
#define cPWR_BLE_LL_OffsetToWakeupInstant 3

/* Application Connection sleep mode */
#define gAppDeepSleepMode_c             8

/* Application RAM usage configuration */
#define cPWR_RamRetentionInVLLS         2 /* 32K */

/* Disable unused LowPower modes */
#define cPWR_EnableDeepSleepMode_1      0
#define cPWR_EnableDeepSleepMode_2      0
#define cPWR_EnableDeepSleepMode_3      0
#define cPWR_EnableDeepSleepMode_4      0
#define cPWR_EnableDeepSleepMode_5      1
#define cPWR_EnableDeepSleepMode_7      0
#define cPWR_EnableDeepSleepMode_8      1

/* Enable RNG seed storage in Flash */
#define gRngSeedStorageAddr_d           ((uint32_t)FREESCALE_PROD_DATA_BASE_ADDR + 1024)

/* Enable XCVR calibration storage in Flash */
#define gPreserveXcvrDacTrimValue_d     1
#define gXcvrDacTrimValueSorageAddr_d   ((uint32_t)FREESCALE_PROD_DATA_BASE_ADDR + 1040)           

/* Enables / Disables MWS coexistence */
#define gMWS_UseCoexistence_d           0

/* Enables / Disables the DCDC platform component */
#define gDCDC_Enabled_d                 1

/* Default DCDC Mode used by the application */
#define APP_DCDC_MODE                   gDCDC_Mode_Buck_c

/* Default DCDC Battery Level Monitor interval */
#define APP_DCDC_VBAT_MONITOR_INTERVAL  600000

/*! *********************************************************************************
 *  RTOS Configuration
 ********************************************************************************** */
/* Defines number of OS events used */
#define osNumberOfEvents        5

/*! *********************************************************************************
 *  BLE Stack Configuration
 ********************************************************************************** */

/* Time between the beginning of two consecutive advertising PDU's */
#define mcAdvertisingPacketInterval_c     0x02    /* 1.25 msec */

/* Offset to the first instant register. */
#define mcOffsetToFirstInstant_c        0x00      /* 625usec */

/*! *********************************************************************************
 *  Auto Configuration
 ********************************************************************************** */
/* Disable LEDs when enabling low power */
#if cPWR_UsePowerDownMode || gMWS_UseCoexistence_d
#define gLEDSupported_d 0
#endif

#if gMWS_UseCoexistence_d
#undef gKBD_KeysCount_c
#define gKBD_KeysCount_c        1
#endif

/*! *********************************************************************************
 *  NVM Module Configuration - gAppUseNvm_d shall be defined above as 1 or 0
 ********************************************************************************** */

#if gAppUseNvm_d
    /* configure NVM module */
    #define  gNvStorageIncluded_d                (1)
    #define  gNvFragmentation_Enabled_d          (1)
    #define  gNvUseFlexNVM_d                     (0)
    #if gNvUseFlexNVM_d
        #define  gUnmirroredFeatureSet_d         (0)
    #else
        #define  gUnmirroredFeatureSet_d         (1)
    #endif
    #define  gNvRecordsCopiedBufferSize_c        (128)
#endif

/*! *********************************************************************************
 *  Memory Pools Configuration
 ********************************************************************************** */

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.
 * DO NOT MODIFY THIS DIRECTLY. CONFIGURE AppPoolsDetails_c
 * If gMaxBondedDevices_c increases, adjust memory pools
*/
#define PoolsDetails_c \
	 AppPoolsDetails_c


#endif /* APP_PREINCLUDE_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
