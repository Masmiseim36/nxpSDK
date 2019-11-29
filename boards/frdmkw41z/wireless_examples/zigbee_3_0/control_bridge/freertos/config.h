/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _ZIGBEE_CONTROL_BRIDGE_CONFIG_H_
#define _ZIGBEE_CONTROL_BRIDGE_CONFIG_H_

/*!=================================================================================================
\file       config.h
\brief      This is the header file for the configuration of the Zigbee Control bridge demo
            application.
==================================================================================================*/

/*==================================================================================================
CONFIG APPLICATION
==================================================================================================*/

#ifdef DEBUG
    #define DBG_ENABLE
#endif

#define asm __asm

/*! FreeRTOS heap size */
#define gTotalHeapSize_c 11000

/*! Main Task stack size */
#define gMainThreadStackSize_c 4500

/*! Enable wdog from ksdk */
#define DISABLE_WDOG 0

//#define BUILD_OTA

#ifdef BUILD_OTA
#define gKBD_TsiElectdCount_c 0
#define gKBD_KeysCount_c 2
#define gSerialMgrRxBufSize_c   512
#define OTA_MAX_SIMULTANEOUS_CLIENTS 5
#endif

/************************************************************************************
*
*       CONFIG STACK
*
************************************************************************************/
/*! Enable little endian build option */
#define LITTLE_ENDIAN_PROCESSOR

/*! Enable Network Steering Capability */
#define BDB_SUPPORT_NWK_STEERING

#define BDB_SUPPORT_TOUCHLINK
#define BDB_COMMISSIONING_MODE 0x0F

/*! Enable Network Formation Capability */
#define BDB_SUPPORT_NWK_FORMATION

/*! Aplication PDM ID */
#define PDM_APP_ID  PDM_APP_CONTROLBRIDGE_ID

/*! Supports both Router and Coordinator device */
#define FULL_FUNC_DEVICE

#define BDB_SET_DEFAULT_TC_POLICY TRUE
#define BDB_JOIN_USES_INSTALL_CODE_KEY TRUE
/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/
/*! UART Baud Rate */
#define APP_SERIAL_INTERFACE_SPEED    115200

/*! Number of timers needed by the protocol stack */
#define gTmrStackTimers_c       20

#define gEepromType_d gEepromDevice_AT45DB041E_c  /* gEepromDevice_AT45DB041E_c */

/************************************************************************************
*
*       CONFIG Mac/Phy
*
************************************************************************************/

/*! Enable MKW41Z512VHT4 NeighborTable for Child Aging */
#define gPhyUseNeighborTable_d 1

/************************************************************************************
*
*      COMMON MODULES CONFIGURATIONS FILES
*
************************************************************************************/
/*! Common framework configuration of all zigbee demo applications */
#include "app_framework_config.h"

/*! Common mac/phy configuration of all zigbee demo applications */
#include "app_mac_config.h"

/************************************************************************************
*
*      CHECKS
*
************************************************************************************/

#endif /* _ZIGBEE_CONTROL_BRIDGE_CONFIG_H_ */
