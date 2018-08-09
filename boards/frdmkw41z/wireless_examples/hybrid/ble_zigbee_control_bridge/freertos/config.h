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

#ifndef _BLE_CONTROL_BRIDGE_CONFIG_H_
#define _BLE_CONTROL_BRIDGE_CONFIG_H_

/*!=================================================================================================
\file       config.h
\brief      This is the header file for the configuration of the BLE Zigbee Control bridge demo
            application.
==================================================================================================*/

/*==================================================================================================
CONFIG APPLICATION
==================================================================================================*/
#define gBleControlBridge_c

/*! Zigbee stack debug config */
#ifdef DEBUG
    #define DBG_ENABLE
#endif

#define asm __asm

/*! Main Task stack size */
#define gMainThreadStackSize_c 4500

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

/*! Aplication PDM ID */
#define PDM_APP_ID PDM_APP_BLE_CONTROLBRIDGE_ID  


/*! Supports both Router and Coordinator device */
#define FULL_FUNC_DEVICE
/*! *********************************************************************************
 *     CONFIG BLE Stack
********************************************************************************** */
 /* Defines Controller task stack size */
#define gControllerTaskStackSize_c 2600
#define gControllerTaskPriority_c  0

/* Defines Host task stack size */
#define gHost_TaskStackSize_c 2600

#define BD_ADDR             0x0A,0x01,0x00,0x37,0x60,0x00
#define gUseHciTransport_d  0

#define gGattDbDynamic_d            0

/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d   0

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d   0

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d   0

/*! Pass key */
#define gPasskeyValue_c                999999

#if (gAppUseBonding_d) && (!gAppUsePairing_d)
  #error "Enable pairing to make use of bonding"
#endif

 /* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d                    0
/************************************************************************************
*
*       CONFIG FRAMEWORK
*
************************************************************************************/
/* Enable RF multi-protocol */
#define gMWS_Enabled_d    1

/*! UART Baud Rate */
#define APP_SERIAL_INTERFACE_SPEED    115200

/*! Number of timers needed by the protocol stack */
#define gTmrStackTimers_c    24

/*! *********************************************************************************
 *     RTOS Configuration
 ********************************************************************************** */

/* Defines number of OS events used (5,3,1) */
#define osNumberOfEvents        10

#define osNumberOfMutexes       10
#define osNumberOfSemaphores    10

/*! FREE_RTOS HEAP SIZE (used for rtos tasks stacks and rtos objects)*/
#define gTotalHeapSize_c 0x8100

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

#endif /* _BLE_CONTROL_BRIDGE_CONFIG_H_ */
