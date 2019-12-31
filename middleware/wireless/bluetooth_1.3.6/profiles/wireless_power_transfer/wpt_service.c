/*! *********************************************************************************
 * \addtogroup WPT Custom Profile
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"
#include "FunctionLib.h"
#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"
#include "wpt_interface.h"

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
#define UUID128(name, ...) extern uint8_t name[];
#include "gatt_uuid128.h"
#undef UUID128
#else
#include "gatt_db_handles.h"
#endif
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

/*! WPT Service - Subscribed Client*/
static deviceId_t mWpt_SubscribedPtuDevice;
static uint16_t mWpt_ServiceHandle = 0xFFFF;
/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static bleResult_t Wpt_SendAlertNotification
(
    uint16_t handle
);
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
bleResult_t Wpt_Start
(
 uint16_t serviceHandle,
 wptPruStaticParamCharacteristic_t*  pPruStaticParameters
)
{
    union
    {
        uint8_t*     uuidTemp;
        bleUuid_t*   bleUuidTemp;
    }uuidVars;

    bleResult_t result;
    uint16_t handle;

    mWpt_SubscribedPtuDevice = gInvalidDeviceId_c;

    mWpt_ServiceHandle = serviceHandle;

    uuidVars.uuidTemp = uuid_characteristic_pru_static;
    bleUuid_t* pUuid = uuidVars.bleUuidTemp;

    /* Get handle of PRU Static characteristic */
    result = GattDb_FindCharValueHandleInService(mWpt_ServiceHandle,
                                                 gBleUuidType128_c, pUuid, &handle);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value */
        result = GattDb_WriteAttribute(handle,
                                       20,
                                       (uint8_t*)pPruStaticParameters);
    }

    return result;
}

bleResult_t Wpt_Stop(void)
{
    return Wpt_Unsubscribe();
}

bleResult_t Wpt_Subscribe(deviceId_t deviceId)
{
    mWpt_SubscribedPtuDevice = deviceId;

    return gBleSuccess_c;
}

bleResult_t Wpt_Unsubscribe(void)
{
    union
    {
        uint8_t            *uuid_characteristicTemp;
        bleUuid_t          *bleUuid_characteristicTemp;
    } bleUuid_characteristicVars;

    uint8_t i = 0;
    uint16_t handle;
    uint8_t maResetValue[20] = {0};

    bleUuid_t *paUuid[4] = { NULL };
    bleUuid_characteristicVars.uuid_characteristicTemp = uuid_characteristic_pru_dynamic;
    paUuid[0] = bleUuid_characteristicVars.bleUuid_characteristicTemp;
    bleUuid_characteristicVars.uuid_characteristicTemp = uuid_characteristic_ptu_static;
    paUuid[1] = bleUuid_characteristicVars.bleUuid_characteristicTemp;
    bleUuid_characteristicVars.uuid_characteristicTemp = uuid_characteristic_pru_control;
    paUuid[2] = bleUuid_characteristicVars.bleUuid_characteristicTemp;
    bleUuid_characteristicVars.uuid_characteristicTemp = uuid_characteristic_pru_alert;
    paUuid[3] = bleUuid_characteristicVars.bleUuid_characteristicTemp;

    uint16_t maHandleSizes[4] = {20, 17, 5, 1};
    bleResult_t result = gBleSuccess_c;

    for (i = 0; i < NumberOfElements(maHandleSizes); ++i)
    {
        /* Get handle of the characteristics */
        result = GattDb_FindCharValueHandleInService(mWpt_ServiceHandle,
                 gBleUuidType128_c, paUuid[i], &handle);

        if (result != gBleSuccess_c)
        {
            break;
        }

        result = GattDb_WriteAttribute(handle, maHandleSizes[i], maResetValue);

        if (gBleSuccess_c != result)
        {
            break;
        }
    }

    mWpt_SubscribedPtuDevice = gInvalidDeviceId_c;

    return result;
}

bleResult_t Wpt_RecordPruDynamic
(
 wptPruDynamicParamCharacteristic_t* pPruDynamic
)
{
    union
    {
        uint8_t*    uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;

    bleResult_t result;
    uint16_t handle;

    uuidVars.uuidTemp = uuid_characteristic_pru_dynamic;
    bleUuid_t* pUuid = uuidVars.bleUuidTemp;

    /* Get handle of PRU Dynamic characteristic */
    result = GattDb_FindCharValueHandleInService(mWpt_ServiceHandle,
                                                 gBleUuidType128_c, pUuid, &handle);
    if(result == gBleSuccess_c)
    {
        /* Update characteristic value */
        result = GattDb_WriteAttribute(handle, 20, (uint8_t*)pPruDynamic);
    }

    return result;
}

bleResult_t Wpt_ReadCharacteristic
(
 uint8_t* charUuid128,
 uint8_t* aOutBuffer,
 uint16_t* pOutLen
)
{
    union
    {
        uint8_t*     uuidTemp;
        bleUuid_t*   bleUuidTemp;
    }uuidVars;

    bleResult_t result;
    uint16_t handle;

    uuidVars.uuidTemp = charUuid128;
    bleUuid_t* pUuid = uuidVars.bleUuidTemp;

    /* Get handle of the characteristic */
    result = GattDb_FindCharValueHandleInService(mWpt_ServiceHandle,
        gBleUuidType128_c, pUuid, &handle);

    if(gBleSuccess_c == result)
    {
        result = GattDb_ReadAttribute(handle, 20, aOutBuffer, pOutLen);
    }

    return result;
}

bleResult_t Wpt_RecordPruAlertAndNotifyIfCccdWritten
(
 wptPruAlertCharacteristic_t* pPruAlert
)
{
    union
    {
        uint8_t*     uuidTemp;
        bleUuid_t*   bleUuidTemp;
    }uuidVars;

    bleResult_t result;
    uint16_t handle;

    uuidVars.uuidTemp = uuid_characteristic_pru_alert;
    bleUuid_t* pUuid = uuidVars.bleUuidTemp;

    /* Get handle of PRU Alert characteristic */
    result = GattDb_FindCharValueHandleInService(mWpt_ServiceHandle,
                                                 gBleUuidType128_c, pUuid, &handle);
    if(result == gBleSuccess_c)
    {
        /* Update characteristic value */
        result = GattDb_WriteAttribute(handle, 1, (uint8_t*)pPruAlert);

        if (result == gBleSuccess_c)
        {
            result = Wpt_SendAlertNotification(handle);
        }
    }

    return result;
}

bleResult_t Wpt_PruRecordPtuStaticParamCharacteristic
(
 wptPtuStaticParamCharacteristic_t* pPtuStatic
)
{
    union
    {
        uint8_t*     uuidTemp;
        bleUuid_t*   bleUuidTemp;
    }uuidVars;
    uuidVars.uuidTemp = uuid_characteristic_ptu_static;

    bleResult_t result;
    uint16_t handle;

    bleUuid_t* pUuid = uuidVars.bleUuidTemp;

    /* Get handle of PRU Static characteristic */
    result = GattDb_FindCharValueHandleInService(mWpt_ServiceHandle,
                                                 gBleUuidType128_c, pUuid, &handle);

    if(result == gBleSuccess_c)
    {
        /* Update characteristic value */
        result = GattDb_WriteAttribute(handle, 17, (uint8_t*)pPtuStatic);
    }

    return result;
}

bleResult_t Wpt_PruRecordPruControlCharacteristic
(
 wptPruControlCharacteristic_t* pPruControl
)
{
    union
    {
        uint8_t*    uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;

    bleResult_t result;
    uint16_t handle;

    uuidVars.uuidTemp = uuid_characteristic_pru_control;
    bleUuid_t* pUuid = uuidVars.bleUuidTemp;

    /* Get handle of PRU Control characteristic */
    result = GattDb_FindCharValueHandleInService(mWpt_ServiceHandle,
                                                 gBleUuidType128_c, pUuid, &handle);

    if(result == gBleSuccess_c)
    {
        /* Update characteristic value */
        result = GattDb_WriteAttribute(handle, 5, (uint8_t*)pPruControl);
    }

    return result;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static bleResult_t Wpt_SendAlertNotification
(
 uint16_t handle
)
{
    bleResult_t result;
    uint16_t  hCccd;
    bool_t isNotificationActive;

    /* Get handle of CCCD */
    result = GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd);

    if (gBleSuccess_c == result)
    {
        result = Gap_CheckNotificationStatus(mWpt_SubscribedPtuDevice,
                                             hCccd,
                                             &isNotificationActive);
        if(gBleSuccess_c == result)
        {
            if (TRUE == isNotificationActive)
            {
                result = GattServer_SendNotification(mWpt_SubscribedPtuDevice, handle);
            }
        }
    }

    return result;
}

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
