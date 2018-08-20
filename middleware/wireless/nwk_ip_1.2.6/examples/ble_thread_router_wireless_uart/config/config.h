/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
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

#ifndef _THREAD_ROUTER_BLUETOOTH_DEVICE_CONFIG_H_
#define _THREAD_ROUTER_BLUETOOTH_DEVICE_CONFIG_H_

/*!=================================================================================================
\file       thread_ble_dual_mode_config.h
\brief      This is the header file for the configuration of the thread router demo
            application with ble in dual mode.
==================================================================================================*/

/*==================================================================================================
CONFIG APPLICATION
==================================================================================================*/

#define SOCK_DEMO                 0

#define USE_TEMPERATURE_SENSOR    1

#define THREAD_USE_SHELL          1
#define THREAD_USE_THCI           0

#if THREAD_USE_SHELL
/* Enable CoAP Observe Client */
#define COAP_OBSERVE_CLIENT       0

/* Enable CoAP Observe Server */
#define COAP_OBSERVE_SERVER       0
#endif

/* Enable application that retrieves IPv6 addresses of connected devices */
#define GETIPv6ADDRESSES_APP      1

#define APP_NAME_c                "Thread Router Bluetooth wireless Uart Hybrid Application Demo"

#define gHybridApp_d              1
/************************************************************************************
*
*       CONFIG STACK
*
************************************************************************************/

#define STACK_THREAD                            1
#define THREAD_ROUTER_CONFIG                    1

/* Thread Router which can start network and/or become its Leader and/or act as Active Router */
/* Shell enabled by default. */

#define THR_DEFAULT_CAN_CREATE_NEW_NETWORK      1
#define THR_DEFAULT_CAN_BECOME_ACTIVE_ROUTER    1
#define THR_DEFAULT_IS_FULL_END_DEVICE          1
#define THR_DEFAULT_IS_POLLING_END_DEVICE       0

#define THR_ENABLE_EVENT_MONITORING             1
#define THR_ENABLE_MGMT_DIAGNOSTICS             1

/*! *********************************************************************************
 *     RTOS Configuration
 ********************************************************************************** */

/* Defines number of OS events used */
#define osNumberOfEvents        8

#define osNumberOfMutexes       3
#define osNumberOfSemaphores    1

/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/

#ifndef ThreadPoolsDetails_c
    #define ThreadPoolsDetails_c\
              _block_size_  16      _number_of_blocks_  34  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  68      _number_of_blocks_  16 _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  160     _number_of_blocks_  16  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  260     _number_of_blocks_  6  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  540     _number_of_blocks_  5  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  800     _number_of_blocks_  2   _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  1300    _number_of_blocks_  2   _pool_id_(ThrPoolId_d)  _eol_
#endif



/*! Application pools configuration */
#ifndef AppPoolsDetails_c
      #define AppPoolsDetails_c\
                _block_size_  16      _number_of_blocks_  12   _pool_id_(AppPoolId_d)  _eol_  \
                _block_size_  68      _number_of_blocks_  10   _pool_id_(AppPoolId_d)  _eol_  \
                _block_size_  160     _number_of_blocks_  10   _pool_id_(AppPoolId_d)  _eol_  \
                _block_size_  512     _number_of_blocks_  10   _pool_id_(AppPoolId_d)  _eol_  \
                _block_size_  800     _number_of_blocks_  6   _pool_id_(AppPoolId_d)  _eol_
  #endif


#define gKeyBoardSupported_d 1
#if THREAD_USE_THCI
  #define gFsciIncluded_c 1
#endif
/*! FREE_RTOS HEAP SIZE (used for rtos tasks stacks and rtos objects)*/
#ifdef FSL_RTOS_FREE_RTOS
    #ifdef CPU_MKW24D512VHA5
        #define gTotalHeapSize_c 0x4400
    #elif  defined(CPU_MK64FN1M0VMD12)
        #define gTotalHeapSize_c 0x8000
    #else
        #define gTotalHeapSize_c 0x4400
    #endif
#endif

/*! Number of timers */
#ifndef gTmrStackTimers_c
    #define gTmrStackTimers_c   26
#endif

#define APP_SERIAL_INTERFACE_TYPE (gSerialMgrCustom_c)
#define gSerialMgrUseCustomInterface_c      (1)
#define gSerialMgrUseUart_c                 (1)

/*! Enable shell on uart serial */
#define ENABLE_SHELL_SERIAL_IF 1
#if  ENABLE_SHELL_SERIAL_IF
    #define gSerialManagerMaxInterfaces_c       (2)
#endif

#define gSerialMgrRxBufSize_c      (250)
#if defined(FRDM_KW41Z)
    #define gLedRgbEnabled_d         1
    #define gLEDsOnTargetBoardCnt_c  4
    #define gKBD_TsiElectdCount_c    2
    #define gKBD_KeysCount_c         4
#elif defined(USB_KW41)
    #define gLedRgbEnabled_d        0
    #define gKBD_KeysCount_c        1
    #define gLEDsOnTargetBoardCnt_c 2
#endif

/*! NVM table size */
#define gNvTableEntriesCountMax_c       50
/*! *********************************************************************************
 *     CONFIG BLE Stack
********************************************************************************** */
 /* Defines Controller task stack size */
#define gControllerTaskStackSize_c 2600
#define gControllerTaskPriority_c  0

/* Defines Host task stack size */
#define gHost_TaskStackSize_c 2600

#define gUseHciTransport_d  0

#define gMWS_Enabled_d 1

/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d   1

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d   1

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d   0

#define gPasskeyValue_c                999999

#if (gAppUseBonding_d) && (!gAppUsePairing_d)
  #error "Enable pairing to make use of bonding"
#endif

#define gMaxServicesCount_d		10
                  
 /* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d                    1
/************************************************************************************
*
*       CONFIG Mac/Phy
*
************************************************************************************/
#if defined(CPU_MKW24D512VHA5)||defined(CPU_MK64FN1M0VMD12)
    #define gMacFeatureSet_d gMacFeatureSet_THR_M4_d
#else
    #define gMacFeatureSet_d gMacFeatureSet_THR_M0_d
#endif
/************************************************************************************
*
*      COMMON MODULES CONFIGURATIONS FILES
*
************************************************************************************/
#include "app_config.h"
#include "app_stack_config.h"
#include "app_framework_config.h"
#include "app_mac_config.h"

#endif /* _THREAD_ROUTER_BLUETOOTH_DEVICE_CONFIG_H_ */
