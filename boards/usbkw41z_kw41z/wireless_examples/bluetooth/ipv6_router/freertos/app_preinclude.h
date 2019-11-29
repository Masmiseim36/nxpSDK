/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* This file is the app configuration file which is pre included.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

#undef CR_INTEGER_PRINTF

/*! *********************************************************************************
 * 	Board Configuration
 ********************************************************************************** */
 /* Defines the number of available keys for the keyboard module */
#define gKBD_KeysCount_c        1

/* Specifies the number of physical LEDs on the target board */
#define gLEDsOnTargetBoardCnt_c 2


/*! *********************************************************************************
 * 	App Configuration
 ********************************************************************************** */
 /*! Number of connections supported by the application */
#define gAppMaxConnections_c   2

 /*! Enable/disable use of bonding capability */
#define gAppUseBonding_d       0

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d       0

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d       0

#define gPasskeyValue_c        999999

#if (gAppUseBonding_d) && (!gAppUsePairing_d)
  #error "Enable pairing to make use of bonding"
#endif

/*! *********************************************************************************
 * 	Framework Configuration
 ********************************************************************************** */
/* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d                    0

/* Defines Rx Buffer Size for Serial Manager */
#define gSerialMgrRxBufSize_c   32

/* Defines Tx Queue Size for Serial Manager */
#define gSerialMgrTxQueueSize_c 5

/* Defines Size for Serial Manager Task*/
#define gSerialTaskStackSize_c  1100

/* Defines Size for TMR Task*/
#define gTmrTaskStackSize_c     500

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define AppPoolsDetails_c \
         _block_size_  32  _number_of_blocks_    9 _eol_  \
         _block_size_  64  _number_of_blocks_    9 _eol_  \
         _block_size_ 128  _number_of_blocks_    4 _eol_  \
         _block_size_ 512  _number_of_blocks_    4 _eol_  \
         _block_size_ 1036  _number_of_blocks_   4 _eol_

/* Defines number of timers needed by the application */
#define gTmrApplicationTimers_c         8

/* Defines number of timers needed by the protocol stack */
#define gTmrStackTimers_c               5

/* Set this define TRUE if the PIT frequency is an integer number of MHZ */
#define gTMR_PIT_FreqMultipleOfMHZ_d    0

/* Enables / Disables the precision timers platform component */
#define gTimestamp_Enabled_d            0

/* Enable/Disable Low Power Timer */
#define gTMR_EnableLowPowerTimers       0
           
/* Use Shell printf */           
#define SHELL_USE_PRINTF                1           

/* Maximum number of shell commands */                      
#define SHELL_MAX_COMMANDS              10     

/* Maximum number of arguments for a shell command */
#define SHELL_MAX_ARGS                  10    
           
/*! *********************************************************************************
 * 	RTOS Configuration
 ********************************************************************************** */
/* Defines the RTOS used */
#define FSL_RTOS_FREE_RTOS              1

/* Defines number of OS events used */
#define osNumberOfEvents        7

/* Defines main task stack size */
#define gMainThreadStackSize_c  1024

/* Define L2CAP RTOS Task Size */
#define gTotalHeapSize_c        13000

/*! *********************************************************************************
 * 	BLE Stack Configuration
 ********************************************************************************** */

            
/*! *********************************************************************************
 * 	IPv6 Configuration
 ********************************************************************************** */           
/* Define usage of BLE configuration for IP stack*/           
#define STACK_BLE 1

/* Define usage of ECHO UDP protocol */           
#define UDP_ECHO_PROTOCOL       1

/* Define usage of socket demo */           
#define SOCK_DEMO               1 

/*! *********************************************************************************
 * 	BLE Application Configuration
 ********************************************************************************** */
#define gMaxServicesCount_d     1           
            
/*! *********************************************************************************
 * 	NVM Module Configuration - gAppUseNvm_d shall be defined aboved as 1 or 0
 ********************************************************************************** */    
             
#if gAppUseNvm_d
    #define gNvmMemPoolId_c 1
    /* Defines NVM pools by block size and number of blocks. Must be aligned to 4 bytes.*/
    #define NvmPoolsDetails_c \
         _block_size_ 32   _number_of_blocks_    20 _pool_id_(1) _eol_ \
         _block_size_ 60   _number_of_blocks_    10 _pool_id_(1) _eol_ \
         _block_size_ 80   _number_of_blocks_    10 _pool_id_(1) _eol_ \
         _block_size_ 100  _number_of_blocks_    2 _pool_id_(1) _eol_
             
    /* configure NVM module */
    #define  gNvStorageIncluded_d                (1)
    #define  gNvFragmentation_Enabled_d          (1)
    #define  gUnmirroredFeatureSet_d             (1)
    #define  gNvRecordsCopiedBufferSize_c        (128)
#endif

/*! *********************************************************************************
 * 	Memory Pools Configuration
 ********************************************************************************** */ 
             
/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.
 * DO NOT MODIFY THIS DIRECTLY. CONFIGURE AppPoolsDetails_c
 * If gMaxBondedDevices_c increases, adjust NvmPoolsDetails_c
*/
#if gAppUseNvm_d
    #define PoolsDetails_c \
         AppPoolsDetails_c \
         NvmPoolsDetails_c
#else
    #define PoolsDetails_c \
         AppPoolsDetails_c
#endif

#endif /* _APP_PREINCLUDE_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
