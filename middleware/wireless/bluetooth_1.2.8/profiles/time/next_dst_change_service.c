/*! *********************************************************************************
* \addtogroup BLE PROFILES
* @{
********************************************************************************** */
/*!
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"
#include "next_dst_change_interface.h"

/***********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
/*! Next DST Change Service - Subscribed Client*/
deviceId_t mNdcs_SubscribedClientId;

/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
********************************************************************************** */

/***********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
bleResult_t Ndcs_Start (ndcsConfig_t *pServiceConfig)
{
    return gBleSuccess_c;
}

bleResult_t Ndcs_Stop (ndcsConfig_t *pServiceConfig)
{
    mNdcs_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Ndcs_Subscribe(deviceId_t clientDeviceId)
{
    mNdcs_SubscribedClientId = clientDeviceId;
    return gBleSuccess_c;
}

bleResult_t Ndcs_Unsubscribe()
{
    mNdcs_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Ndcs_RecordCurrentTime (ndcsConfig_t *pServiceConfig)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_TimeWithDST_d);

    /* Get handle of  characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Update characteristic value */
    result = GattDb_WriteAttribute(handle, sizeof(ndcsTimeWithDst_t), (uint8_t*)(&pServiceConfig->timeWithDst));

    if (result != gBleSuccess_c)
        return result;

    return gBleSuccess_c;
}

/***********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* @}
********************************************************************************** */
