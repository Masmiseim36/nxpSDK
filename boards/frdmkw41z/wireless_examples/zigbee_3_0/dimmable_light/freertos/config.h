/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _ZIGBEE_DIMMABLE_LIGHT_CONFIG_H_
#define _ZIGBEE_DIMMABLE_LIGHT_CONFIG_H_

/*!=================================================================================================
\file       config.h
\brief      This is the header file for the configuration of the Zigbee Dimmable Light demo
            application.
==================================================================================================*/

/*==================================================================================================
CONFIG APPLICATION
==================================================================================================*/
/*! DIMMABLE LIGHT application */ 
#define DIMMABLELIGHT
#define DimmableLight

#define MONO_WITH_LEVEL

/*! Zigbee stack debug config */
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

/************************************************************************************
*
*       CONFIG STACK
*
************************************************************************************/
/*! Enable little endian build option */
#define LITTLE_ENDIAN_PROCESSOR

/*! Enable Network Steering Capability */
#define BDB_SUPPORT_NWK_STEERING

/*! Enable Support for Network Formation Capability */
#define BDB_SUPPORT_NWK_FORMATION

/*! Enable find and bind target Capability */
#define BDB_SUPPORT_FIND_AND_BIND_TARGET

#define BDB_SUPPORT_TOUCHLINK
#define BDB_COMMISSIONING_MODE 0x0F

/*! Aplication PDM ID */
#define PDM_APP_ID  PDM_APP_LIGHT_ID 

/*! Zigbee stack debug config */
#ifdef DEBUG
    #define DEBUG_APP
    #define DEBUG_BDB
    #define DEBUG_ZCL
    #define DEBUG_APP_BDB
    #define DEBUG_APP_EVENT
    #define DEBUG_LIGHT_NODE
#endif

/*! Green Power support */
#define CLD_GREENPOWER
#define GP_COMBO_BASIC_DEVICE
#define ZPS_GREENPOWER

/*! OTA Update support */
//#define BUILD_OTA

#ifdef BUILD_OTA
#define gKBD_TsiElectdCount_c 0
#define gKBD_KeysCount_c 2
#define gOtaVerifyWrite_d 1
#endif

#define BDB_SET_DEFAULT_TC_POLICY TRUE
#define BDB_JOIN_USES_INSTALL_CODE_KEY TRUE
/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/
/*! Enable RGB led support in the LED module*/
#define gLedRgbEnabled_d    1

#ifdef CLD_GREENPOWER
#define gTmrTaskStackSize_c 1024
#endif

#define gEepromType_d gEepromDevice_AT45DB041E_c
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

#endif /* _ZIGBEE_DIMMABLE_LIGHT_CONFIG_H_ */
