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
