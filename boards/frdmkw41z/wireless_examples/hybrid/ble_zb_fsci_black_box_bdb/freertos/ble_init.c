/*! *********************************************************************************
 * \addtogroup BLE
 * @{
 ********************************************************************************** */
/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
\file       ble_init.c
*/

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_init.h"
#include "ble_general.h"
#include "ble_host_task_config.h"
#include "ble_controller_task_config.h"
#include "hci_transport.h"
#include "board.h"

#if !gUseHciTransportDownward_d
#include "controller_interface.h"
#include "fsl_xcvr.h"
#include "Flash_Adapter.h"
#endif /* gUseHciTransportDownward_d */

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/
#ifndef cMCU_SleepDuringBleEvents
    #define cMCU_SleepDuringBleEvents    0
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
extern bool_t gEnableSingleAdvertisement;
extern bool_t gMCUSleepDuringBleEvents;

bleResult_t Ble_Initialize
(
    gapGenericCallback_t gapGenericCallback
)
{
        
#if (gUseHciTransportDownward_d == 1)
    
    /* Configure HCI Transport */
    hcitConfigStruct_t hcitConfigStruct = 
    {
        .interfaceType = gHcitInterfaceType_d,
        .interfaceChannel = gHcitInterfaceNumber_d,
        .interfaceBaudrate = gHcitInterfaceSpeed_d,
        .transportInterface =  Ble_HciRecv
    };
      
    /* HCI Transport Init */
    if (gHciSuccess_c != Hcit_Init(&hcitConfigStruct))
    {
        return gHciTransportError_c;
    }
    
    /* BLE Host Tasks Init */
    if (osaStatus_Success != Ble_HostTaskInit())
    {
        return gBleOsError_c;
    }
    
    /* BLE Host Stack Init */
    return Ble_HostInitialize(gapGenericCallback, 
                (hciHostToControllerInterface_t) Hcit_SendPacket);
    
#elif (gUseHciTransportUpward_d == 1)
    
    /* BLE Radio Init */    
    XCVR_Init(BLE_MODE, DR_1MBPS);    
    XCVR_SetXtalTrim( (uint8_t)gHardwareParameters.xtalTrim );

    if (osaStatus_Success != Controller_TaskInit())
    {
        return gBleOsError_c;
    }
    
    gMCUSleepDuringBleEvents = cMCU_SleepDuringBleEvents;        
    
    /* BLE Controller Init */
    if (osaStatus_Success != Controller_Init((gHostHciRecvCallback_t)Hcit_SendPacket))
    {
        return gBleOsError_c;
    }
    
    /* Configure HCI Transport */
    hcitConfigStruct_t hcitConfigStruct = 
    {
        .interfaceType = gHcitInterfaceType_d,
        .interfaceChannel = gHcitInterfaceNumber_d,
        .interfaceBaudrate = gHcitInterfaceSpeed_d,
        .transportInterface =  Hci_SendPacketToController
    };
    
    return Hcit_Init(&hcitConfigStruct);
    
#else    

    /* BLE Radio Init */    
    XCVR_Init(BLE_MODE, DR_1MBPS);    
    XCVR_SetXtalTrim( (uint8_t)gHardwareParameters.xtalTrim );
    
    /* BLE Controller Task Init */
    if (osaStatus_Success != Controller_TaskInit())
    {
        return gBleOsError_c;
    }
    
    gEnableSingleAdvertisement = TRUE;
    gMCUSleepDuringBleEvents = cMCU_SleepDuringBleEvents;
        
    /* BLE Controller Init */
    if (osaStatus_Success != Controller_Init(Ble_HciRecv))
    {
        return gBleOsError_c;
    }

    /* BLE Host Tasks Init */
    if (osaStatus_Success != Ble_HostTaskInit())
    {
        return gBleOsError_c;
    }    
    
    /* BLE Host Stack Init */
    return Ble_HostInitialize(gapGenericCallback, 
                (hciHostToControllerInterface_t) Hci_SendPacketToController);
    
#endif
}


/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/


/*! *********************************************************************************
* @}
********************************************************************************** */
