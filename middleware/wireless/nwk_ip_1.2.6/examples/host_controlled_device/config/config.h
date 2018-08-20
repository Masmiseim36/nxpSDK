/*!
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

#ifndef _THREAD_HOST_CONTROLLED_DEVICE_CONFIG_H_
#define _THREAD_HOST_CONTROLLED_DEVICE_CONFIG_H_

/*!=================================================================================================
\file       config.h
\brief      This is the header file for the configuration of the Thread host controlled device demo
            application.
==================================================================================================*/

/************************************************************************************
*
*       CONFIG APPLICATION
*
************************************************************************************/
#define USE_TEMPERATURE_SENSOR                          1

#define THREAD_USE_SHELL                                0
#define THREAD_USE_THCI                                 1

#define gFsciIncluded_c                                 1

#if USB_KW24
#define THCI_USB_ENABLE                                 1
#define THR_SOFTWARE_RESET_ENABLE                       0
#else
#define THCI_UART_ENABLE                                1
#endif
#define THCI_SPI_ENABLE                                 0

/*! Number of timers */
#ifndef gTmrStackTimers_c
    #define gTmrStackTimers_c                           50
#endif

/* One interface enable for THCI */
#if ((THCI_USB_ENABLE + THCI_UART_ENABLE + THCI_SPI_ENABLE) > 1)
    #warning use just one interface for thci!
#endif

#if THREAD_USE_SHELL
/* Enable CoAP Observe Client */
#define COAP_OBSERVE_CLIENT                             0

/* Enable CoAP Observe Server */
#define COAP_OBSERVE_SERVER                             0
#endif

#define APP_NAME_c                                      "Host Controlled Device Demo"

#define THR_USE_DHCP6_SERVER                            TRUE
#define THR_USE_DHCP6_CLIENT                            TRUE
#define MAC_FILTERING_TABLE_SIZE                        35
#define THR_KEY_SWITCH_GUARD_TIME_HOURS                 0

#ifdef R41Z_DK07
    #undef THCI_UART_ENABLE
    #undef THCI_SPI_ENABLE
    #define THCI_UART_ENABLE         0
    #define THCI_SPI_ENABLE          1
#endif

/************************************************************************************
*
*       CONFIG STACK
*
************************************************************************************/
#define STACK_THREAD                                    1
#define THREAD_BORDER_ROUTER_CONFIG                     1


#define IP_IP4_ENABLE                                   1
#define DHCP4_CLIENT_ENABLED                            1
#define ND_ENABLED                                      1

/* Thread Router which can start network and/or become its Leader and/or act as Active Router.
    FSCI enabled by default */
#define THR_DEFAULT_CAN_CREATE_NEW_NETWORK              1
#define THR_DEFAULT_CAN_BECOME_ACTIVE_ROUTER            1
#define THR_DEFAULT_IS_FULL_END_DEVICE                  1
#define THR_DEFAULT_IS_POLLING_END_DEVICE               0
#define THR_DEFAULT_IS_BORDER_ROUTER                    1
#define THR_ENABLE_EVENT_MONITORING                     1
#define THR_ENABLE_MGMT_DIAGNOSTICS                     1
#define THR_SERIAL_TUN_ROUTER                           0

#if THR_SERIAL_TUN_ROUTER
#define IP_IF_NB                                        2
#define IP_IF_IP6_ADDR_NB                               12
#endif
/*!=================================================================================================
   SOCKETS
==================================================================================================*/
/*! The maximum number of sockets that can be opened at one time. MUST be correlated to
MAX_UDP_CONNECTIONS */
#ifndef BSDS_MAX_SOCKETS
    #define BSDS_MAX_SOCKETS                            24
#endif

/*!=================================================================================================
   UDP
==================================================================================================*/

/*! The maximum number of UDP connections that can be opened at one time. MUST not be greater
than BSDS_MAX_SOCKETS */
#ifndef MAX_UDP_CONNECTIONS
    #define MAX_UDP_CONNECTIONS                         24
#endif


/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/
#define gFsciMaxPayloadLen_c 1300

#if (defined(FRDM_K64F) || defined(FRDM_KW41Z) || defined(USB_KW41))
    /*! Thread stack pools configuration */
    #ifndef ThreadPoolsDetails_c
        #define ThreadPoolsDetails_c\
                  _block_size_  16    _number_of_blocks_    32 _pool_id_(ThrPoolId_d)    _eol_  \
                  _block_size_  68    _number_of_blocks_    22 _pool_id_(ThrPoolId_d)    _eol_  \
                  _block_size_  160   _number_of_blocks_    16 _pool_id_(ThrPoolId_d)    _eol_  \
                  _block_size_  260   _number_of_blocks_    16 _pool_id_(ThrPoolId_d)    _eol_  \
                  _block_size_  512    _number_of_blocks_   12   _pool_id_(ThrPoolId_d)  _eol_  \
                  _block_size_  800   _number_of_blocks_    12 _pool_id_(ThrPoolId_d)    _eol_  \
                  _block_size_  1300  _number_of_blocks_    8 _pool_id_(ThrPoolId_d)    _eol_
    #endif
    /*! Application pools configuration */
    #ifndef AppPoolsDetails_c
        #define AppPoolsDetails_c\
                  _block_size_  16    _number_of_blocks_     4 _pool_id_(AppPoolId_d)    _eol_  \
                  _block_size_  68    _number_of_blocks_     4 _pool_id_(AppPoolId_d)    _eol_  \
                  _block_size_  160   _number_of_blocks_     6 _pool_id_(AppPoolId_d)    _eol_  \
                  _block_size_  260   _number_of_blocks_     16 _pool_id_(AppPoolId_d)    _eol_  \
                  _block_size_  1300  _number_of_blocks_     8 _pool_id_(AppPoolId_d)    _eol_
    #endif
#endif

#if THCI_SPI_ENABLE
    #define gSerialMgrRxBufSize_c               (1500)  /* required for large ICMP ECHO_REQUESTs (e.g. 1200) */
    #define gSerialMgrUseUart_c                 (0)
    #define gSerialMgrUseSPI_c                  (1)
    #define gSerialMgrUseFSCIHdr_c              (1)
    #define gSerialMgrSlaveDapTxLogicOne_c      (0)
    #define gFsciTxAck_c                        (0)
    #define gFsciRxAck_c                        (0)
    #define gNvPendingSavesQueueSize_c          (64)
#endif
 
/*! FREE_RTOS HEAP SIZE (used for rtos tasks stacks and rtos objects)*/                      
#ifdef FSL_RTOS_FREE_RTOS
    #ifdef CPU_MKW24D512VHA5
        #define gTotalHeapSize_c 0x4400
    #elif  defined(CPU_MK64FN1M0VMD12)
        #define gTotalHeapSize_c 0x8000
    #else
        #define gTotalHeapSize_c 0x2A00              
    #endif 
#endif
                      
#ifdef FSL_RTOS_FREE_RTOS
    #define USB_STACK_FREERTOS 1                      
#endif
                      
#ifdef FRDM_KW24
    #define gLedRgbEnabled_d         1 
    #define gLEDsOnTargetBoardCnt_c  4
    #define gKBD_KeysCount_c         4
#elif defined(USB_KW24)
    #define gLEDsOnTargetBoardCnt_c  2
    #define gKBD_KeysCount_c         1
    #define gSerialMgrUseUSB_c       1
    #define gSerialMgrUseUart_c      0
#elif defined(FRDM_K64F)
    #define gLedRgbEnabled_d         1
    #define gLEDsOnTargetBoardCnt_c  4
    #define gKBD_KeysCount_c         4
#elif defined(FRDM_KW41Z)
    #define gLedRgbEnabled_d         1
    #define gLEDsOnTargetBoardCnt_c  4
    #define gKBD_TsiElectdCount_c    2
    #define gKBD_KeysCount_c         4
#elif defined(USB_KW41)
    #define gLedRgbEnabled_d         0
    #define gKBD_KeysCount_c         1
    #define gLEDsOnTargetBoardCnt_c  2
#elif defined(R41Z_DK07)
    #define gLEDSupported_d          0
    #define gKeyBoardSupported_d     0
    #define gMWS_FsciEnabled_d       1
    #define gMWS_UseCoexistence_d    1
    #define gLedRgbEnabled_d         0
    #define gLEDsOnTargetBoardCnt_c  0
    #define gKBD_TsiElectdCount_c    0
    #define gKBD_KeysCount_c         0
#endif
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

/************************************************************************************
*
*      CHECKS
*
************************************************************************************/
#if THR_SERIAL_TUN_ROUTER && (gFsciIncluded_c == 0)
#error "Serial Tun Router must have FSCI enabled"
#endif

#endif /* _THREAD_HOST_CONTROLLED_DEVICE_CONFIG_H_ */
