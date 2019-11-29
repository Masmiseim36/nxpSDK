/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/*!
**  @addtogroup thread_app_config
**  @{
*/
#ifndef _APP_FRAMEWORK_CONFIG_H
#define _APP_FRAMEWORK_CONFIG_H

/*!=================================================================================================
\file       app_framework_config.h
\brief      This file is a for framework configuration of all thread demo applications.
            If it is required to configure just one application use the appllication config. file.
            Ex: for thread router application use thread_router_config.h
==================================================================================================*/

/*!=================================================================================================
       CONFIG FRAMEWORK
==================================================================================================*/

#ifdef CR_INTEGER_PRINTF
    #undef CR_INTEGER_PRINTF
#endif

/*! Use panic */
#ifndef gUsePanic_c
    #define gUsePanic_c 0
#endif

/*!=================================================================================================
* MEMORY MANAGER
==================================================================================================*/

/*! The APP memory pool ID */
#ifndef AppPoolId_d
    #define AppPoolId_d 0
#endif
/*! The THread memory pool ID */
#ifndef ThrPoolId_d
    #define ThrPoolId_d 1
#endif


/*! Thread stack pools configuration */
#ifndef ThreadPoolsDetails_c
    #define ThreadPoolsDetails_c\
              _block_size_  16      _number_of_blocks_  34  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  68      _number_of_blocks_  16 _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  160     _number_of_blocks_  16  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  260     _number_of_blocks_  8  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  540     _number_of_blocks_  5  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  800     _number_of_blocks_  3   _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  1000    _number_of_blocks_  2   _pool_id_(ThrPoolId_d)  _eol_  /* RLY_RX for DTLS handshake */\
              _block_size_  1300    _number_of_blocks_  2   _pool_id_(ThrPoolId_d)  _eol_
#endif


#if gFsciIncluded_c
  /*! Application pools configuration */
  #ifndef AppPoolsDetails_c
      #define AppPoolsDetails_c\
                _block_size_  16      _number_of_blocks_  4   _pool_id_(AppPoolId_d)  _eol_  \
                _block_size_  68      _number_of_blocks_  4   _pool_id_(AppPoolId_d)  _eol_  \
                _block_size_  160     _number_of_blocks_  2   _pool_id_(AppPoolId_d)  _eol_  \
                _block_size_  260     _number_of_blocks_  2   _pool_id_(AppPoolId_d)  _eol_  \
                _block_size_  800     _number_of_blocks_  2   _pool_id_(AppPoolId_d)  _eol_
  #endif
#endif

  /*! Application pools configuration */
#ifndef AppPoolsDetails_c
    #define AppPoolsDetails_c\
              _block_size_  16      _number_of_blocks_  4   _pool_id_(AppPoolId_d)  _eol_  \
              _block_size_  68      _number_of_blocks_  4   _pool_id_(AppPoolId_d)  _eol_  \
              _block_size_  160     _number_of_blocks_  2   _pool_id_(AppPoolId_d)  _eol_  \
              _block_size_  260     _number_of_blocks_  2   _pool_id_(AppPoolId_d)  _eol_
#endif

/*! All used memory pools */
#ifndef PoolsDetails_c
#define PoolsDetails_c \
         AppPoolsDetails_c     \
         ThreadPoolsDetails_c  \
         _block_size_  0     _number_of_blocks_     0 _pool_id_(0xFF) _eol_
#endif
/*!=================================================================================================
  SHELL Configuration
==================================================================================================*/
/*! Use SHELL printf */
#ifndef SHELL_USE_PRINTF
    #define SHELL_USE_PRINTF     1
#endif
/*! The maximum Shell buffer size*/
#ifndef SHELL_CB_SIZE
#if SHELL_DUT_COMMISSIONER
    #define SHELL_CB_SIZE       250
#else
    #define SHELL_CB_SIZE       125
#endif
#endif
/*! The maximum number of SHELL commands*/
#ifndef SHELL_MAX_COMMANDS
    #define SHELL_MAX_COMMANDS  28
#endif
/*! The maximum ARGS for SHELL */
#ifndef SHELL_MAX_ARGS
#if SHELL_DUT_COMMISSIONER
    #define SHELL_MAX_ARGS      25
#else
    #define SHELL_MAX_ARGS      20
#endif
#endif

/* config if logo is displayed */
#ifndef SHELL_USE_LOGO
    #define SHELL_USE_LOGO      0
#endif
/*!=================================================================================================
  Led configuration
==================================================================================================*/
#if defined(FRDM_KW24) || defined(FRDM_KW41Z)
    #define BOARD_USE_PWM_FOR_RGB_LED 1
#endif
/*!=================================================================================================
  NVM configuration
==================================================================================================*/
/*! Enable/disable NV storage module */
#ifndef gNvStorageIncluded_d
    #define gNvStorageIncluded_d        1
#endif
/*! Enables/disables fragmented saves/restores (see NVM_Interface.h)*/
#ifndef gNvFragmentation_Enabled_d
    #define gNvFragmentation_Enabled_d  1
#endif
/*! Enables/disables the extended feature set of the NVM module (see NVM_Interface.h) */
#ifndef gNvUseExtendedFeatureSet_d
    #define gNvUseExtendedFeatureSet_d  1
#endif
/*! Enable/disable NVM Unmirrored Feature Set (see NVM module) */
#ifndef gUnmirroredFeatureSet_d
    #define gUnmirroredFeatureSet_d     1
#endif
/*! Enable/disable interrupts when the NV FTFL controller executes a command sequence */
#ifndef gNvDisableIntCmdSeq_c
    #define gNvDisableIntCmdSeq_c       1
#endif
/*! Enable NVM FSCI monitoring */
#ifndef gNvmEnableFSCIMonitoring_c
    #define gNvmEnableFSCIMonitoring_c      (0)
#endif
/*! Indexes of the elements that needs to be merged */
#ifndef gNvRecordsCopiedBufferSize_c
    #define gNvRecordsCopiedBufferSize_c    128
#endif

/*! NVM table size */
#ifndef gNvTableEntriesCountMax_c
    #define gNvTableEntriesCountMax_c       38
#endif

/*! NVM pool id for unmirrored datasets */
#ifndef gNvmMemPoolId_c
    #define gNvmMemPoolId_c                 ThrPoolId_d
#endif

/*! Default minimum-timer-ticks-between-dataset-saves, in seconds */
#ifndef gNvMinimumTicksBetweenSaves_c
    #define gNvMinimumTicksBetweenSaves_c   2
#endif
/*!=================================================================================================
  SERIAL MANAGER configuration
==================================================================================================*/
/*! Use USB for serial communication */
#ifndef gSerialMgrUseUSB_c
  #define gSerialMgrUseUSB_c     0
#endif
/*! Use UART for serial communication */
#ifndef gSerialMgrUseUart_c
    #define gSerialMgrUseUart_c    1
#endif
/*! Serial task stack size */
#ifndef gSerialTaskStackSize_c
    /* key exchange require 300 bytes stack */
    #define gSerialTaskStackSize_c 800
#endif

#ifndef gVirtualCOM_USBTaskPriority_d
  #define gVirtualCOM_USBTaskPriority_d   0
#endif

#ifndef gSerialMgrRxBufSize_c
    #define gSerialMgrRxBufSize_c      (64)
#endif

#ifndef gVirtualCOMPort_DiscardTxOnCOMClose_d
  #define gVirtualCOMPort_DiscardTxOnCOMClose_d    1
#endif
/*!=================================================================================================
  FSCI configuration
==================================================================================================*/
/*! Enable FCSI */
#ifndef gFsciIncluded_c
    #define gFsciIncluded_c         0
#endif
/*! FCSI packet has two bytes */
#ifndef gFsciLenHas2Bytes_c
    #define gFsciLenHas2Bytes_c  1
#endif
/*! Maximum FCSI packet length  */
#ifndef gFsciMaxPayloadLen_c
    #define gFsciMaxPayloadLen_c 245
#endif
/*!=================================================================================================
  TMR configuration
==================================================================================================*/
/*! Timer task stack size */
#ifndef gTmrTaskStackSize_c
    #define gTmrTaskStackSize_c 600
#endif
/*! Number of timers */
#ifndef gTmrStackTimers_c
    #define gTmrStackTimers_c   25
#endif

#ifdef FRDM_KL46Z
#ifndef gTMR_PIT_Timestamp_Enabled_d
#define gTMR_PIT_Timestamp_Enabled_d    1
#endif
#endif

#if defined(FRDM_KW24)
    #define gStackTimerInstance_c  1
#endif
/*!=================================================================================================
* OSA EXT configuration
==================================================================================================*/
/*! Number of semaphores */
#ifndef osNumberOfSemaphores
    #define osNumberOfSemaphores 1
#endif
/*! Number of mutexes*/
#ifndef osNumberOfMutexes
    #define osNumberOfMutexes    2
#endif
/*! Number of message queues*/
#ifndef osNumberOfMessageQs
    #define osNumberOfMessageQs  0
#endif
/*! Number of messages */
#ifndef osNumberOfMessages
    #define osNumberOfMessages   0
#endif
/*! Number of events */
#ifndef osNumberOfEvents
    #define osNumberOfEvents     5
#endif
/*! Main Thread task priority */
#ifndef gMainThreadPriority_c
    #define gMainThreadPriority_c  (OSA_PRIORITY_IDLE + 2)
#endif
/*! Thread task size */
#ifndef gMainThreadStackSize_c
    #define gMainThreadStackSize_c 1600
#endif

/*!=================================================================================================
* RNG
==================================================================================================*/

#if CPU_MKL46Z256VLL4
    #ifndef gRNG_UsePhyRngForInitialSeed_d
        #define gRNG_UsePhyRngForInitialSeed_d  1
    #endif
#endif

/*!=================================================================================================
* LPM - low power
==================================================================================================*/
#if CPU_MKW41Z512VHT4
   #ifndef cPWR_DeepSleepMode
   #define cPWR_DeepSleepMode                       3
   #endif
#else
    /* By default configure the SED to stay in Low power 3 seconds
    using 15.625ms for a tick (15.625ms*192ticks)*/
    #define cPWR_LPTMRTickTimeSource_ERCLK32K      cLPTMR_PRS_125_div_by_8ms
    #define cPWR_TMRTicks                          192
    #define cPWR_DeepSleepMode                     10
#endif

/* Overwrite the default configuration for KL46Z and USKKW24 */
#if CPU_MKL46Z256VLL4 || USB_KW24
    #undef  cPWR_TMRTicks
    #define cPWR_TMRTicks                           188
    #define cPWR_LPTMRTickTimeSource_LPO_1KHz       cLPTMR_PRS_00016ms
    /* DeepSleepDurationMs  =  188* 16ms = 3000ms */
    /* max DeepSleepDurationMs = 0xFFFF * 16ms = 1048560 ms */

    #undef  cPWR_DeepSleepMode
    #define cPWR_DeepSleepMode                       9
#endif

#if (defined(CPU_MKW41Z512VHT4) && (gLpmIncluded_d == 1))
  #define gTMR_PIT_Timestamp_Enabled_d  0
#endif

/* Enable/disable the TMR table check for the first low power timer programmed to expire */
#ifndef cPWR_CheckLowPowerTimers
#define cPWR_CheckLowPowerTimers                     1
#endif

/*!=================================================================================================
* Board
==================================================================================================*/

#endif /* _APP_FRAMEWORK_CONFIG_H  */
/*!
** @}
*/
