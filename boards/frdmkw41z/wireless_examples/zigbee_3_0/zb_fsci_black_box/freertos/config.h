/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _ZIGBEE_FSCI_BLACK_BOX_CONFIG_H_
#define _ZIGBEE_FSCI_BLACK_BOX_CONFIG_H_

/*!=================================================================================================
\file       config.h
\brief      This is the header file for the configuration of the Zigbee FSCI Black Box demo
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
/************************************************************************************
*
*       CONFIG STACK
*
************************************************************************************/
/*! Enable little endian build option */
#define LITTLE_ENDIAN_PROCESSOR

/*! Enable Network Steering Capability */
#define BDB_SUPPORT_NWK_STEERING

/*! Enable Network Formation Capability */
#define BDB_SUPPORT_NWK_FORMATION

/*! Aplication PDM ID */
#define PDM_APP_ID  PDM_APP_CONTROLBRIDGE_ID  
/*! Supports both Router and Coordinator device */
#define FULL_FUNC_DEVICE
/*! Enable Enhanced Scene commands */
#define CLD_SCENES_CMD_COPY_SCENE
#define CLD_SCENES_CMD_ENHANCED_VIEW_SCENE
#define CLD_SCENES_CMD_ENHANCED_ADD_SCENE

#define BDB_SET_DEFAULT_TC_POLICY TRUE
#define BDB_JOIN_USES_INSTALL_CODE_KEY TRUE

/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/
/*! UART Baud Rate */
#define APP_SERIAL_INTERFACE_SPEED 115200

/*! Number of timers needed by the protocol stack */
#define gTmrStackTimers_c       20

/* Enable FSCI module */
#define gFsciIncluded_c         1

/*! FSCI has 2 bytes for  payload length */ 
#define gFsciLenHas2Bytes_c     1

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

#endif /* _ZIGBEE_FSCI_BLACK_BOX_CONFIG_H_ */
