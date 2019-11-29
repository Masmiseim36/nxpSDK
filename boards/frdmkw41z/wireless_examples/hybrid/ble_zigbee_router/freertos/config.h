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
#define gTotalHeapSize_c (16*1024)

/*! Main Task stack size */
#define gMainThreadStackSize_c 4500

/*! Enable wdog from ksdk */
#define DISABLE_WDOG 0

#define gMWS_Enabled_d 1

#define PoolsDetails_c \
              _block_size_  16      _number_of_blocks_  4   _pool_id_(0)  _eol_  \
              _block_size_  68      _number_of_blocks_  4   _pool_id_(0)  _eol_  \
              _block_size_  180     _number_of_blocks_  15  _pool_id_(0)  _eol_  \
              _block_size_  260     _number_of_blocks_  6   _pool_id_(0)  _eol_  \
              _block_size_  512     _number_of_blocks_  2   _pool_id_(0)  _eol_

/*! *********************************************************************************
 * 	BLE Stack Configuration
 ********************************************************************************** */
 /* Defines Controller task stack size */
#define gControllerTaskStackSize_c 2600
#define gControllerTaskPriority_c  0

/* Defines Host task stack size */
#define gHost_TaskStackSize_c 2600

/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d   0

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d   0

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d   0

#define gPasskeyValue_c                999999

/* Profile Parameters */
#define gScanningTime_c         3   /* 3 s*/

#if (gAppUseBonding_d) && (!gAppUsePairing_d)
  #error "Enable pairing to make use of bonding"
#endif

#define BD_ADDR             0x0A,0x01,0x00,0x37,0x60,0x00

 /* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d                    0

#define gMaxServicesCount_d 10

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
#define APP_SERIAL_INTERFACE_TYPE (gSerialMgrCustom_c)
#define gSerialMgrUseCustomInterface_c      (1)

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
