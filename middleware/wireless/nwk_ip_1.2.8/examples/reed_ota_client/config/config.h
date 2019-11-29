/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _THREAD_ROUTER_ELIGIBLE_DEVICE_OTA_CLIENT_CONFIG_H_
#define _THREAD_ROUTER_ELIGIBLE_DEVICE_OTA_CLIENT_CONFIG_H_

/*!=================================================================================================
\file       config.h
\brief      This is the header file for the configuration of the Thread router eligible device OTA
            client demo application.
==================================================================================================*/

/*==================================================================================================
CONFIG APPLICATION
==================================================================================================*/

#define SOCK_DEMO                                   0

#define USE_TEMPERATURE_SENSOR                      1

#define THREAD_USE_SHELL                            1
#define THREAD_USE_THCI                             0

#if THREAD_USE_SHELL
/* Enable CoAP Observe Client */
#define COAP_OBSERVE_CLIENT                         0
#define SHELL_DUT_COMMISSIONER                      0

/* Enable CoAP Observe Server */
#define COAP_OBSERVE_SERVER                         0
#endif

/* Enable application that retrieves IPv6 addresses of connected devices */
#define GETIPv6ADDRESSES_APP                        1

#define APP_NAME_c                                  "Router Application Demo"

#define gEnableOTAServer_d                          0
#define gEnableOTAClient_d                          1
/* external eeprom type */
/*  FRDM_KW41Z512  gEepromDevice_AT45DB041E_c */
/*  FRDM_KW24D512  gEepromDevice_AT45DB161E_c */
#if (defined(TWR_KW24D512) || defined(FRDM_KW24))
#define gEepromType_d                               gEepromDevice_AT45DB161E_c
#elif defined(FRDM_KW41Z)
#define gEepromType_d                               gEepromDevice_AT45DB041E_c 
#else
#define gEepromType_d                               gEepromDevice_InternalFlash_c
#endif
/************************************************************************************
*
*       CONFIG STACK
*
************************************************************************************/

#define STACK_THREAD                                1
#define THREAD_ROUTER_CONFIG                        1

/* Thread Router which can start network and/or become its Leader and/or act as Active Router */
/* Shell enabled by default. */

#define THR_DEFAULT_CAN_CREATE_NEW_NETWORK          1
#define THR_DEFAULT_CAN_BECOME_ACTIVE_ROUTER        1
#define THR_DEFAULT_IS_FULL_END_DEVICE              1
#define THR_DEFAULT_IS_POLLING_END_DEVICE           0

#define THR_ENABLE_EVENT_MONITORING                 1
#define THR_ENABLE_MGMT_DIAGNOSTICS                 1

#define MPL_SEED_SET_SIZE                           25
#define MPL_BUFFERED_MESSAGE_SET_SIZE               5
#define COAP_MAX_SESSIONS                           10
#define THR_MAX_DEV_ADDR_QUERY_CACHE_ENTRIES        10
/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/

#ifndef ThreadPoolsDetails_c
    #define ThreadPoolsDetails_c\
              _block_size_  16      _number_of_blocks_  50  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  68      _number_of_blocks_  34  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  160     _number_of_blocks_  32  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  260     _number_of_blocks_  12  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  540     _number_of_blocks_  10  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  800     _number_of_blocks_  10  _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  1300    _number_of_blocks_  6   _pool_id_(ThrPoolId_d)  _eol_
#endif

/*! Application pools configuration */
#ifndef AppPoolsDetails_c
    #define AppPoolsDetails_c\
              _block_size_  16      _number_of_blocks_  10  _pool_id_(AppPoolId_d)  _eol_  \
              _block_size_  68      _number_of_blocks_  8   _pool_id_(AppPoolId_d)  _eol_  \
              _block_size_  160     _number_of_blocks_  4   _pool_id_(AppPoolId_d)  _eol_  \
              _block_size_  260     _number_of_blocks_  2   _pool_id_(AppPoolId_d)  _eol_  \
              _block_size_  800     _number_of_blocks_  2   _pool_id_(AppPoolId_d)  _eol_
#endif

#define gKeyBoardSupported_d                        1
#if THREAD_USE_THCI
    #define gFsciIncluded_c                         1
#endif

/*! FREE_RTOS HEAP SIZE (used for rtos tasks stacks and rtos objects)*/
#ifdef FSL_RTOS_FREE_RTOS
    #ifdef CPU_MKW24D512VHA5
        #define gTotalHeapSize_c                    0x4400
    #elif  defined(CPU_MK64FN1M0VMD12)
        #define gTotalHeapSize_c                    0x8000
    #else
        #define gTotalHeapSize_c                    0x4000
    #endif
#endif
#define gMainThreadStackSize_c                      2200

#ifdef FSL_RTOS_FREE_RTOS
    #define USB_STACK_FREERTOS                      1
#endif

#ifdef FRDM_KW24
    #define gLedRgbEnabled_d                        1
    #define gLEDsOnTargetBoardCnt_c                 4
    #define gKBD_KeysCount_c                        4
#elif defined(FRDM_KW41Z)
    #define gLedRgbEnabled_d                        1
    #define gLEDsOnTargetBoardCnt_c                 4
    #define gKBD_TsiElectdCount_c                   0
    #define gKBD_KeysCount_c                        2
#endif
/************************************************************************************
*
*       CONFIG Mac/Phy
*
************************************************************************************/
#if defined(CPU_MKW24D512VHA5)||defined(CPU_MK64FN1M0VMD12)
    #define gMacFeatureSet_d                        gMacFeatureSet_THR_M4_d
#else
    #define gMacFeatureSet_d                        gMacFeatureSet_THR_M0_d
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
#if SERIAL_TUN_IF && (gFsciIncluded_c == 0)
#error "Serial Tun interface must have FSCI enabled"
#endif

#endif /* _THREAD_ROUTER_ELIGIBLE_DEVICE_OTA_CLIENT_CONFIG_H_ */
