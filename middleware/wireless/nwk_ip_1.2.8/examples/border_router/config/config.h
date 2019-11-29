/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _THREAD_BORDER_ROUTER_CONFIG_H_
#define _THREAD_BORDER_ROUTER_CONFIG_H_

/*!=================================================================================================
\file       config.h
\brief      This is the header file for the configuration of the Thread border router demo
            application.
==================================================================================================*/

/*==================================================================================================
CONFIG APPLICATION
==================================================================================================*/

#define USE_TEMPERATURE_SENSOR                      1

#if (defined(TWR_KW24) || defined(FRDM_KW24) || defined(FRDM_K64F))
    #define THREAD_USE_SHELL                        1
    #define THREAD_USE_THCI                         0
    #define gFsciIncluded_c                         0
#elif defined(USB_KW24)
    #define THREAD_USE_SHELL                        0
    #define THREAD_USE_THCI                         1
    #define gFsciIncluded_c                         1
#else
    #define THREAD_USE_SHELL                        0
    #define THREAD_USE_THCI                         1
    #define gFsciIncluded_c                         1
#endif

#if defined(USB_KW24)
    #define THCI_USB_ENABLE                         1
    #define THCI_USBENET_ENABLE                     1
#elif (defined(USB_KW41))
    #define THCI_SPI_ENABLE                         1
    #define THCI_USBENET_ENABLE                     1
#else
    #define THCI_UART_ENABLE                        1
#endif

#if THREAD_USE_SHELL
    /* Enable CoAP Observe Client */
    #define COAP_OBSERVE_CLIENT                     0

    /* Enable CoAP Observe Server */
    #define COAP_OBSERVE_SERVER                     0
#endif

#define APP_NAME_c                                  "Border Router Application Demo"
/************************************************************************************
*
*       CONFIG STACK
*
************************************************************************************/
#define STACK_THREAD                                1
#define THREAD_BORDER_ROUTER_CONFIG                 1

/* Thread Router which can start network and/or become its Leader and/or act as Active Router.
    THCI enabled by default */
#define THR_DEFAULT_CAN_CREATE_NEW_NETWORK          1
#define THR_DEFAULT_CAN_BECOME_ACTIVE_ROUTER        1
#define THR_DEFAULT_IS_FULL_END_DEVICE              1
#define THR_DEFAULT_IS_POLLING_END_DEVICE           0
#define THR_ENABLE_EVENT_MONITORING                 1
#define SERIAL_TUN_IF                               0
#define THR_ENABLE_MGMT_DIAGNOSTICS                 1
#define THR_USE_DHCP6_SERVER                        1

#define BSDS_MAX_SOCKETS                            20
#define MAX_UDP_CONNECTIONS                         20
#define IP_IP6_MCASTFW_TBL_SIZE                     75

#if defined(CPU_MKW24D512VHA5) || defined(USB_KW41)
    #define USB_RNDIS_IF                            1
#elif defined(FRDM_K64F)
    #define ENET_IF                                 1
    #define NAT64_ENABLED                           0
#endif

#if (ENET_IF || USB_RNDIS_IF)
    #define BR_ROUTER_MODE                          0
    #define BR_HOST_MODE                            1
#endif


/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/

#define gKeyBoardSupported_d 1

#if defined(FRDM_K64F)

/*! Thread stack pools configuration */
#ifndef ThreadPoolsDetails_c
#define ThreadPoolsDetails_c\
              _block_size_  16    _number_of_blocks_    32 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  68    _number_of_blocks_    22 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  160   _number_of_blocks_    16 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  260   _number_of_blocks_    16 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  512    _number_of_blocks_   6   _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  800   _number_of_blocks_    6 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  1600  _number_of_blocks_    2 _pool_id_(ThrPoolId_d)    _eol_
#endif
/*! Application pools configuration */
#ifndef AppPoolsDetails_c
#define AppPoolsDetails_c\
              _block_size_  16    _number_of_blocks_     4 _pool_id_(AppPoolId_d)    _eol_  \
              _block_size_  68    _number_of_blocks_     4 _pool_id_(AppPoolId_d)    _eol_  \
              _block_size_  160   _number_of_blocks_     6 _pool_id_(AppPoolId_d)    _eol_  \
              _block_size_  260   _number_of_blocks_     6 _pool_id_(AppPoolId_d)    _eol_  \
              _block_size_  800   _number_of_blocks_     2 _pool_id_(AppPoolId_d)    _eol_  \
              _block_size_  1600  _number_of_blocks_     2 _pool_id_(AppPoolId_d)    _eol_  \
              _block_size_  6096  _number_of_blocks_     1 _pool_id_(AppPoolId_d)    _eol_  \
              _block_size_  12176 _number_of_blocks_     1 _pool_id_(AppPoolId_d)    _eol_
#endif

#endif /* defined(FRDM_K64F) */

#if defined(USB_KW41)

/*! Thread stack pools configuration */
#ifndef ThreadPoolsDetails_c
#define ThreadPoolsDetails_c\
              _block_size_  16    _number_of_blocks_    32 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  68    _number_of_blocks_    22 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  160   _number_of_blocks_    16 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  260   _number_of_blocks_    16 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  512    _number_of_blocks_   6   _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  800   _number_of_blocks_    6 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  1600  _number_of_blocks_    2 _pool_id_(ThrPoolId_d)    _eol_
#endif
/*! Application pools configuration */
#ifndef AppPoolsDetails_c
#define AppPoolsDetails_c\
              _block_size_  16    _number_of_blocks_     4 _pool_id_(AppPoolId_d)    _eol_  \
              _block_size_  68    _number_of_blocks_     4 _pool_id_(AppPoolId_d)    _eol_  \
              _block_size_  160   _number_of_blocks_     6 _pool_id_(AppPoolId_d)    _eol_  \
              _block_size_  260   _number_of_blocks_     6 _pool_id_(AppPoolId_d)    _eol_  \
              _block_size_  800   _number_of_blocks_     2 _pool_id_(AppPoolId_d)    _eol_  \
              _block_size_  1600   _number_of_blocks_    2 _pool_id_(AppPoolId_d)    _eol_
#endif

#endif /* defined(USB_KW41) */

/*! FREE_RTOS HEAP SIZE (used for rtos tasks stacks and rtos objects)*/
#ifdef FSL_RTOS_FREE_RTOS
    #ifdef CPU_MKW24D512VHA5
        #define gTotalHeapSize_c                    0x4400
    #elif  defined(FRDM_K64F)
        #define gTotalHeapSize_c                    0x8000
    #else
        #define gTotalHeapSize_c                    0x4400
    #endif
#endif

#define gMainThreadStackSize_c                      2200

#ifdef FSL_RTOS_FREE_RTOS
    #define USB_STACK_FREERTOS                      1
#endif

#if !defined(FRDM_K64F) && !defined(USB_KW41)
    #define gSerialMgrUseUSB_VNIC_c                 1
#endif

/*! Number of timers */
#ifndef gTmrStackTimers_c
    #define gTmrStackTimers_c                       30
#endif

#ifndef DTLS_MAX_CONTEXTS
    #define DTLS_MAX_CONTEXTS                       3
#endif

#ifdef USB_KW41

    #define osNumberOfMutexes                       3

    #define gFsciHostSupport_c                      1
    #define gFsciHost_Rndis_c                       1
    #define gFsciIncluded_c                         1
    #define gFsciMaxPayloadLen_c                    1600
    #define gSerialMgrRxBufSize_c                   1800
    #define gFsciLenHas2Bytes_c                     1
    #define gFsciTxAck_c                            0
    #define gFsciRxAck_c                            0
    #define gFsciRxTimeout_c                        0
    #define gFsciRxAckTimeoutUseTmr_c               0

    #define gSerialMgrUseSPI_c                      (1)
#endif


#ifdef FRDM_KW24
    #define gLedRgbEnabled_d                        1
    #define gLEDsOnTargetBoardCnt_c                 4
    #define gKBD_KeysCount_c                        4
    #define gSerialManagerMaxInterfaces_c           2
#elif defined(USB_KW24)
    #define gLEDsOnTargetBoardCnt_c                 2
    #define gKBD_KeysCount_c                        1
    #define gSerialMgrUseUSB_VNIC_c                 1
    #define gSerialMgrUseUart_c                     0
#elif defined(FRDM_K64F)
    #define gLedRgbEnabled_d                        1
    #define gLEDsOnTargetBoardCnt_c                 4
    #define gKBD_KeysCount_c                        4
#elif defined(USB_KW41)
    #define gLedRgbEnabled_d                        0
    #define gKBD_KeysCount_c                        1
    #define gLEDsOnTargetBoardCnt_c                 2

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
*       INTERFACE NUMBER CALCULATION - DO NOT MODIFY
*
************************************************************************************/

#ifndef SERIAL_TUN_IF
    #define SERIAL_TUN_IF                           0
#endif

#ifndef ENET_IF
    #define ENET_IF                                 0
#endif

#ifndef USB_RNDIS_IF
    #define USB_RNDIS_IF                            0
#endif

/* Thread and Loopback interface are always present */
#define IP_IF_NB                                    2 + SERIAL_TUN_IF + ENET_IF + USB_RNDIS_IF

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

#endif /* _THREAD_BORDER_ROUTER_CONFIG_H_ */
