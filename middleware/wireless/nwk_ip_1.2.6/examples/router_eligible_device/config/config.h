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

#ifndef _THREAD_ROUTER_ELIGIBLE_DEVICE_CONFIG_H_
#define _THREAD_ROUTER_ELIGIBLE_DEVICE_CONFIG_H_

/*!=================================================================================================
\file       config.h
\brief      This is the header file for the configuration of the Thread router eligible device demo
            application.
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
#define SHELL_DUT_COMMISSIONER    0

/* Enable CoAP Observe Server */
#define COAP_OBSERVE_SERVER       0
#endif

/* Enable application that retrieves IPv6 addresses of connected devices */
#define GETIPv6ADDRESSES_APP      1

#define APP_NAME_c                "Router Application Demo"

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

/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/

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

#endif /* _THREAD_ROUTER_ELIGIBLE_DEVICE_CONFIG_H_ */
