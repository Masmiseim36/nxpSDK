/*! *********************************************************************************
 * \addtogroup Temperature Custom Profile
 * @{
 ********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
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
#include "gatt_server_interface.h"
#include "gap_interface.h"
#include "temperature_interface.h"
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

/*! Temperature Service - Subscribed Client*/
static deviceId_t mTms_SubscribedClientId;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void Hts_SendTemperatureMeasurementNotification
(
    uint16_t handle
);
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
bleResult_t Tms_Start (tmsConfig_t *pServiceConfig)
{    
    mTms_SubscribedClientId = gInvalidDeviceId_c;
    
    return Tms_RecordTemperatureMeasurement (pServiceConfig->serviceHandle, 
                                             pServiceConfig->initialTemperature);
}

bleResult_t Tms_Stop (tmsConfig_t *pServiceConfig)
{
    return Tms_Unsubscribe();
}

bleResult_t Tms_Subscribe(deviceId_t deviceId)
{
    mTms_SubscribedClientId = deviceId;

    return gBleSuccess_c;
}

bleResult_t Tms_Unsubscribe()
{
	mTms_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Tms_RecordTemperatureMeasurement (uint16_t serviceHandle, int16_t temperature)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_Temperature_d);

    /* Get handle of Temperature characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Update characteristic value */
    result = GattDb_WriteAttribute(handle, sizeof(uint16_t), (uint8_t*)&temperature);

    if (result != gBleSuccess_c)
        return result;

    Hts_SendTemperatureMeasurementNotification(handle);

    return gBleSuccess_c;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

static void Hts_SendTemperatureMeasurementNotification
(
  uint16_t handle
)
{
    uint16_t  hCccd;
    bool_t isNotificationActive;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) != gBleSuccess_c)
        return;

    if (gBleSuccess_c == Gap_CheckNotificationStatus
        (mTms_SubscribedClientId, hCccd, &isNotificationActive) &&
        TRUE == isNotificationActive)
    {
        GattServer_SendNotification(mTms_SubscribedClientId, handle);
    }
}

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
