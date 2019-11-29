/*! *********************************************************************************
 * \addtogroup BLE OTAP Custom Profile
 * @{
 ********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* This file contains the source for the BLE OTAP Service/Profile
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "FunctionLib.h"
#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_db_handles.h" // Include this file for the 128 bit characteristic UUIDs. Do not access the handles directlty!
#include "gatt_server_interface.h"
#include "gap_interface.h"
#include "otap_interface.h"
/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/*! OTAP Client Service - Subscribed Client*/
static deviceId_t mOtapCS_SubscribedClientId;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static bleResult_t OtapCS_SendControlPointIndication (uint16_t handle);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
bleResult_t OtapCS_Start (otapClientConfig_t *pServiceConfig)
{
    mOtapCS_SubscribedClientId = gInvalidDeviceId_c;
    
    return gBleSuccess_c;
}

bleResult_t OtapCS_Stop (otapClientConfig_t *pServiceConfig)
{
    return OtapCS_Unsubscribe();
}

bleResult_t OtapCS_Subscribe(deviceId_t deviceId)
{
    mOtapCS_SubscribedClientId = deviceId;

    return gBleSuccess_c;
}

bleResult_t OtapCS_Unsubscribe()
{
	mOtapCS_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t OtapCS_SendCommandToOtapServer (uint16_t serviceHandle,
                                            void* pCommand,
                                            uint16_t cmdLength)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t* pUuid = (bleUuid_t*)&uuid_char_otap_control_point;
    
    /* Get handle of OTAP Control Point characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
                                                 gBleUuidType128_c, pUuid, &handle);
    
    if (result != gBleSuccess_c)
        return result;
    
    /* Write characteristic value */
    result = GattDb_WriteAttribute(handle,
                                   cmdLength,
                                   (uint8_t*)pCommand);

    if (result != gBleSuccess_c)
        return result;
    
    /* Send Command to the OTAP Server via ATT Indication */
    return OtapCS_SendControlPointIndication (handle);
}


/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*!**********************************************************************************
* \brief        Sends an ATT Indications for the specified handle
*               of an OTAP Control Point characteristic.
*
* \param[in]    handle   Characteristic handle.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
static bleResult_t OtapCS_SendControlPointIndication (uint16_t handle)
{
    uint16_t    hCccd;
    bool_t      isIndicationActive;
    bleResult_t result;
    
    /* Get handle of CCCD */
    result = GattDb_FindCccdHandleForCharValueHandle (handle, &hCccd);
    
    if (result != gBleSuccess_c)
    {
        return result;
    }
    
    result = Gap_CheckIndicationStatus (mOtapCS_SubscribedClientId,
                                        hCccd,
                                        &isIndicationActive);
    
    if (result != gBleSuccess_c)
    {
        return result;
    }
    
    if (TRUE == isIndicationActive)
    {
        return GattServer_SendIndication (mOtapCS_SubscribedClientId,
                                          handle);
    }
    else
    {
        return gBleSuccess_c;
    }
}

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
