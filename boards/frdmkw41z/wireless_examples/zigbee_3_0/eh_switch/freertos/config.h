/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _ZIGBEE_EH_SWITCH_CONFIG_H_
#define _ZIGBEE_EH_SWITCH_CONFIG_H_

/*!=================================================================================================
\file       config.h
\brief      This is the header file for the configuration of the Zigbee Color Temperature Light demo
            application.
==================================================================================================*/

/*==================================================================================================
CONFIG APPLICATION
==================================================================================================*/
/* Zigbee stack debug config */
#ifdef DEBUG
#define DBG_ENABLE
    //#define DEBUG_APP
    //#define DEBUG_BDB
    //#define DEBUG_ZCL
    //#define DEBUG_APP_BDB
    //#define DEBUG_APP_EVENT
    //#define DEBUG_LIGHT_NODE
    //#define DBG_DEVICE_SWITCH TRUE
#endif

#define DK4

/*!   TRUE =  1: Use PowerDown functions (Normal)
      FALSE = 0: Don't use PowerDown. Useful for debugging and test purposes
 */
#define cPWR_UsePowerDownMode 0

/*!  If TRUE - do not use  PWM_FOR_RGB_LED */
#define gLpmIncluded_d        cPWR_UsePowerDownMode

#define asm __asm

/*! FreeRTOS heap size */
#define gTotalHeapSize_c 13000

/*! Main Task stack size */
#define gMainThreadStackSize_c 4500

/*! Enable wdog from ksdk */
#define DISABLE_WDOG 0

#define BUTTON_MAP_FRDM_KW41Z
/************************************************************************************
*
*       CONFIG STACK
*
************************************************************************************/
/*! Enable little endian build option */
#define LITTLE_ENDIAN_PROCESSOR

/*! Device type zigbee end device */ 
#define ZBPRO_DEVICE_TYPE_ZED

/*! Enable Network Steering Capability */
#define BDB_SUPPORT_NWK_STEERING

/*! Enable find and bind intitiator Capability */
#define BDB_SUPPORT_FIND_AND_BIND_INITIATOR

/*! Aplication PDM ID */
#define PDM_APP_ID  PDM_ID_APP_SENSOR 

/*! The default value for sleepy end device (SED) polling interval */
#ifndef ZB_POLLING_INTERVAL_MS
    #define ZB_POLLING_INTERVAL_MS                  3000     /* Milliseconds */
#endif

#define SINGLE_CHANNEL 0
/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/
/*! Number of timers needed by the protocol stack */
#define gTmrStackTimers_c       13

/*! Button imput queue */
#define osNumberOfMessageQs 1

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

#endif /* _ZIGBEE_EH_SWITCH_CONFIG_H_ */
