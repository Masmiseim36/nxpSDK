/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the app configuration file which is pre included.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
         _block_size_ 1036  _number_of_blocks_   2 _eol_

/* Defines number of timers needed by the application */
#define gTmrApplicationTimers_c         8

/* Defines number of timers needed by the protocol stack */
#define gTmrStackTimers_c               5

/* Set this define TRUE if the PIT frequency is an integer number of MHZ */
#define gTMR_PIT_FreqMultipleOfMHZ_d    0

/* Enables / Disables the precision timers platform component */
#define gTimestamp_Enabled_d            1

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
#define gMainThreadStackSize_c  1280

/* Define L2CAP RTOS Task Size */
#define gTotalHeapSize_c        15000

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
