/*
* The Clear BSD License
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

#ifndef _ZIGBEE_LIGHT_SENSOR_CONFIG_H_
#define _ZIGBEE_LIGHT_SENSOR_CONFIG_H_

/*!=================================================================================================
\file       config.h
\brief      This is the header file for the configuration of the Zigbee Light Sensor demo
            application.
==================================================================================================*/

/*==================================================================================================
CONFIG APPLICATION
==================================================================================================*/
/*! Zigbee stack debug config */
#ifdef DEBUG
#define DBG_ENABLE
    //#define DEBUG_APP
    //#define DEBUG_BDB
    //#define DEBUG_ZCL
    //#define DEBUG_APP_BDB
    //#define DEBUG_APP_EVENT
    //#define DEBUG_LIGHT_NODE
#endif

/*!   TRUE =  1: Use PowerDown functions (Normal)
      FALSE = 0: Don't use PowerDown. Useful for debugging and test purposes
 */
#define cPWR_UsePowerDownMode 1

/*!  If TRUE - do not use  PWM_FOR_RGB_LED */
#define gLpmIncluded_d        cPWR_UsePowerDownMode

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
#define PDM_APP_ID  PDM_ID_APP_ZED 

/*! The default value for sleepy end device (SED) polling interval */
#ifndef ZB_POLLING_INTERVAL_MS
    #define ZB_POLLING_INTERVAL_MS                  3000     /* Milliseconds */
#endif

#define BDB_SET_DEFAULT_TC_POLICY TRUE
#define BDB_JOIN_USES_INSTALL_CODE_KEY TRUE
/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/
/*! Number of timers needed by the protocol stack */
#define gTmrStackTimers_c            13
#define cPWR_CheckLowPowerTimers     1

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

#endif /* _ZIGBEE_LIGHT_SENSOR_CONFIG_H_ */
