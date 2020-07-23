/*! *********************************************************************************
 * \addtogroup Temperature Custom Profile
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
/*!**********************************************************************************
* \brief        Starts Temperature Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_Start(tmsConfig_t *pServiceConfig)
{
    mTms_SubscribedClientId = gInvalidDeviceId_c;

    /* Set the initial value of the temperature characteristic */
    return Tms_RecordTemperatureMeasurement(pServiceConfig->serviceHandle,
                                            pServiceConfig->initialTemperature);
}

/*!**********************************************************************************
* \brief        Stops Temperature Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_Stop(tmsConfig_t *pServiceConfig)
{
    /* Stop functionality by unsubscribing */
    return Tms_Unsubscribe();
}

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Temperature service
*
* \param[in]    pClient  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_Subscribe(deviceId_t clientDeviceId)
{
    /* Subscribe by saving the client ID */
    mTms_SubscribedClientId = clientDeviceId;
    return gBleSuccess_c;
}

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Temperature service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_Unsubscribe(void)
{
    /* Unsubscribe by invalidating the client ID */
    mTms_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

/*!**********************************************************************************
* \brief        Records Temperature measurement on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    value           Temperature Value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_RecordTemperatureMeasurement(uint16_t serviceHandle, int16_t temperature)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_Temperature_d);

    /* Get handle of Temperature characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
             gBleUuidType16_c, &uuid, &handle);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value */
        result = GattDb_WriteAttribute(handle, sizeof(uint16_t), (uint8_t *)&temperature);

        if (result == gBleSuccess_c)
        {
            /* Notify the temperature value */
            Hts_SendTemperatureMeasurementNotification(handle);
        }
    }

    return result;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
/*!**********************************************************************************
* \brief        Notify temperature measurement values.
*
* \param[in]    handle          Service handle.
************************************************************************************/
static void Hts_SendTemperatureMeasurementNotification
(
    uint16_t handle
)
{
    uint16_t  hCccd;
    bool_t isNotificationActive;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) == gBleSuccess_c)
    {
        /* Check if notifications are active and notify */
        if (gBleSuccess_c == Gap_CheckNotificationStatus
            (mTms_SubscribedClientId, hCccd, &isNotificationActive) &&
            TRUE == isNotificationActive)
        {
            (void)GattServer_SendNotification(mTms_SubscribedClientId, handle);
        }
    }
}

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
