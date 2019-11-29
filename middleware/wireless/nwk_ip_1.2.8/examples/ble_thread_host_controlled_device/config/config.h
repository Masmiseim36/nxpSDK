/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _THREAD_HOST_CONTROLLED_DEVICE_HYBRID_CONFIG_H_
#define _THREAD_HOST_CONTROLLED_DEVICE_HYBRID_CONFIG_H_

/*!=================================================================================================
\file       thread_host_controlled_device_hybrid_config.h
\brief      This is the header file for the configuration of the thread host control device with bluetooth
            demo application.
==================================================================================================*/

/************************************************************************************
*
*       CONFIG APPLICATION
*
************************************************************************************/
#define USE_TEMPERATURE_SENSOR                      1

#define THREAD_USE_SHELL                            0
#define THREAD_USE_THCI                             1

#define gFsciIncluded_c                             1

#if gUSBKW24D512Dongle
#define THCI_USB_ENABLE                             1
#define THR_SOFTWARE_RESET_ENABLE                   0
#else
#define THCI_UART_ENABLE                            1
#endif
#define THCI_SPI_ENABLE                             0

/* One interface enable for THCI */
#if ((THCI_USB_ENABLE + THCI_UART_ENABLE + THCI_SPI_ENABLE) > 1)
    #warning use just one interface for thci!
#endif

#if THREAD_USE_SHELL
/* Enable CoAP Observe Client */
#define COAP_OBSERVE_CLIENT                         0

/* Enable CoAP Observe Server */
#define COAP_OBSERVE_SERVER                         0
#endif

#define APP_NAME_c                                  "Hybrid Thread Host Controlled Device with BLE BlackBox Demo"

#define gHybridApp_d                                1
/************************************************************************************
*
*       CONFIG STACK
*
************************************************************************************/
#define STACK_THREAD                                1
#define THREAD_BORDER_ROUTER_CONFIG                 1

/* Thread Router which can start network and/or become its Leader and/or act as Active Router.
    FSCI enabled by default */
#define THR_DEFAULT_CAN_CREATE_NEW_NETWORK          1
#define THR_DEFAULT_CAN_BECOME_ACTIVE_ROUTER        1
#define THR_DEFAULT_IS_FULL_END_DEVICE              1
#define THR_DEFAULT_IS_POLLING_END_DEVICE           0
#define THR_ENABLE_EVENT_MONITORING                 1
#define THR_ENABLE_MGMT_DIAGNOSTICS                 1
#define THR_USE_DHCP6_SERVER                        1
#define SERIAL_TUN_IF                               0

#if SERIAL_TUN_IF
    #define BR_ROUTER_MODE                          0
    #define BR_HOST_MODE                            1
    #define IP_IF_IP6_ADDR_NB                       12
#endif

/* Reduce code size when Host control device is not a Border router */
#if !BR_HOST_MODE && !BR_ROUTER_MODE
    #define ND_ENABLED                              0
    #define MLD_ENABLED                             0
#endif

/*! *********************************************************************************
 *     SOCKETS
************************************************************************************/

/*! The maximum number of sockets that can be opened at one time. MUST be corelated to MAX_UDP_CONNECTIONS */
#ifndef BSDS_MAX_SOCKETS
    #define BSDS_MAX_SOCKETS                        12
#endif

/*! *********************************************************************************
 *     UDP
************************************************************************************/

/*! The maximum number of UDP connections that can be opened at one time. MUST not be greater than BSDS_MAX_SOCKETS */
#ifndef MAX_UDP_CONNECTIONS
    #define MAX_UDP_CONNECTIONS                     12
#endif

/*! *********************************************************************************
 *     RTOS Configuration
 ********************************************************************************** */

/* Defines number of OS events used */
#define osNumberOfEvents                            8
#define osNumberOfMutexes                           3
#define osNumberOfSemaphores                        1

/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/

#define gKeyBoardSupported_d                        1
#define gFsciMaxPayloadLen_c                        1300

/*! Thread stack pools configuration */
#ifndef ThreadPoolsDetails_c
    #define ThreadPoolsDetails_c\
              _block_size_  16    _number_of_blocks_    32 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  68    _number_of_blocks_    22 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  160   _number_of_blocks_    16 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  260   _number_of_blocks_    16 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  540    _number_of_blocks_   6   _pool_id_(ThrPoolId_d)  _eol_  \
              _block_size_  800   _number_of_blocks_    6 _pool_id_(ThrPoolId_d)    _eol_  \
              _block_size_  1300  _number_of_blocks_    2 _pool_id_(ThrPoolId_d)    _eol_
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

#if THCI_SPI_ENABLE
    #define gSerialMgrRxBufSize_c                   (768)  /* required for large ICMP ECHO_REQUESTs (e.g. 1200) */
    #define gSerialMgrUseUart_c                     (0)
    #define gSerialMgrUseSPI_c                      (1)
    #define gSerialMgrUseFSCIHdr_c                  (1)
    #define gSerialMgrSlaveDapTxLogicOne_c          (0)
    #define gFsciTxAck_c                            (0)
    #define gFsciRxAck_c                            (0)
    #define gNvPendingSavesQueueSize_c              (64)
#endif

/*! FREE_RTOS HEAP SIZE (used for rtos tasks stacks and rtos objects)*/
#ifdef FSL_RTOS_FREE_RTOS
    #ifdef CPU_MKW24D512VHA5
        #define gTotalHeapSize_c                    0x4400
    #elif  defined(CPU_MK64FN1M0VMD12)
        #define gTotalHeapSize_c                    0x8000
    #else
        #define gTotalHeapSize_c                    0x4400
    #endif
#endif

#ifndef gFsciBleBBox_d
    #define gFsciBleBBox_d                          1
#endif

#ifdef FSL_RTOS_FREE_RTOS
    #define USB_STACK_FREERTOS                      1
#endif

#define gFsciMaxInterfaces_c                        2

#define gFsciMaxVirtualInterfaces_c                 2

#define gSerialTaskStackSize_c 1200
              
#if defined(FRDM_KW41Z)
    #define gLedRgbEnabled_d                        1
    #define gLEDsOnTargetBoardCnt_c                 4
    #define gKBD_TsiElectdCount_c                   2
    #define gKBD_KeysCount_c                        4
#elif defined(USB_KW41)
    #define gLedRgbEnabled_d                        0
    #define gKBD_KeysCount_c                        1
    #define gLEDsOnTargetBoardCnt_c                 2
#endif

#define gNvTableEntriesCountMax_c                   50
/*! *********************************************************************************
 *     CONFIG BLE Stack
********************************************************************************** */
 /* Defines Controller task stack size */
#define gControllerTaskStackSize_c                  2600
#define gControllerTaskPriority_c                   0

/* Defines Host task stack size */
#define gHost_TaskStackSize_c                       2600

#define gUseHciTransport_d                          0

#define gGattDbDynamic_d                            1

#define gMWS_Enabled_d                              1
/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d                            1

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d                            1

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d                            0

#define gPasskeyValue_c                             999999

#if (gAppUseBonding_d) && (!gAppUsePairing_d)
  #error "Enable pairing to make use of bonding"
#endif

 /* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d                                0
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

#endif /* _THREAD_HOST_CONTROLLED_DEVICE_HYBRID_CONFIG_H_ */
