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
#define gAppMaxConnections_c           8

/*! Number of credit-based channels supported */
#define gL2caMaxLeCbChannels_c         2

/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d   0

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d   0

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d   0

#define gPasskeyValue_c                999999

#if (gAppUseBonding_d) && (!gAppUsePairing_d)
  #error "Enable pairing to make use of bonding"
#endif

/* Enable Generic FSK protocol in XCVR */
#define XCVR_GENFSK_ENABLED     1

/* The GenFSK coexistence is controlled by the application */
#define gGENFSK_MwsControl_c gGENFSK_AppMwsControl_c

/*! *********************************************************************************
 *  Framework Configuration
 ********************************************************************************** */
/* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d                    0

/* Defines Rx Buffer Size for Serial Manager */
#define gSerialMgrRxBufSize_c   1000

/* Defines Tx Queue Size for Serial Manager */
#define gSerialMgrTxQueueSize_c 30

/* Defines Size for Serial Manager Task*/
#define gSerialTaskStackSize_c  500

/* Defines Size for TMR Task*/
#define gTmrTaskStackSize_c  600

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define AppPoolsDetails_c \
         _block_size_  32  _number_of_blocks_    30 _eol_  \
         _block_size_  64  _number_of_blocks_     5 _eol_  \
         _block_size_ 128  _number_of_blocks_     5 _eol_  \
         _block_size_ 268  _number_of_blocks_    25 _eol_  \
         _block_size_ 512  _number_of_blocks_     2 _eol_

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

/* Enable/Disable Low Power Timer */
#define gTMR_EnableLowPowerTimers       0

/*! *********************************************************************************
 *  RTOS Configuration
 ********************************************************************************** */
/* Defines the RTOS used */
#define FSL_RTOS_FREE_RTOS      1

/* Defines number of OS events used */
#define osNumberOfEvents        6

/* Defines main task stack size */
#define gMainThreadStackSize_c  900
           
/* Defines total heap size used by the OS */
#define gTotalHeapSize_c        9000
           
/*! *********************************************************************************
 *  BLE Stack Configuration
 ********************************************************************************** */
#define gMaxServicesCount_d         10

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

#define gMWS_Enabled_d 1

#endif /* APP_PREINCLUDE_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
