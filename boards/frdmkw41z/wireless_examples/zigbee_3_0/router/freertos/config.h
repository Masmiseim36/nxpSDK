/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _ZIGBEE_BDB_ROUTER_H_
#define _ZIGBEE_BDB_ROUTER_H_

/*!=================================================================================================
\file       config.h
\brief      This is the header file for the configuration of the Zigbee BDB Router demo
            application.
==================================================================================================*/

/*==================================================================================================
CONFIG APPLICATION
==================================================================================================*/
/*! Zigbee stack debug config */
#ifdef DEBUG
    #define DBG_ENABLE
    /*Enable serial debug log */
    #define JENNIC_DEBUG_ENABLE
#endif

#define asm __asm

/*! FreeRTOS heap size */
#define gTotalHeapSize_c 11000

/*! Main Task stack size */
#define gMainThreadStackSize_c 4500

/*! Enable wdog from ksdk */
#define DISABLE_WDOG 0

/*! OTA Update support */
//#define BUILD_OTA

#ifdef BUILD_OTA
#define gKBD_TsiElectdCount_c 0
#define gKBD_KeysCount_c 2
#endif

#define BDB_SET_DEFAULT_TC_POLICY TRUE
#define BDB_JOIN_USES_INSTALL_CODE_KEY TRUE
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

/*! Aplication PDM ID */
#define PDM_APP_ID  PDM_APP_BDB_ZR_ID 

/*! Zigbee stack debug config */
#ifdef DEBUG
    #define DEBUG_APP
    #define DEBUG_BDB
    #define DEBUG_ZCL
    #define DEBUG_APP_BDB
    #define DEBUG_APP_EVENT
    #define DEBUG_LIGHT_NODE
#endif
/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/

/* None */

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

#endif /* _ZIGBEE_BDB_ROUTER_H_ */
