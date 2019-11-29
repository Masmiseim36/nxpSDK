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
#include "reference_time_update_interface.h"

/***********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
/*! Reference Time Update Service - Subscribed Client*/
deviceId_t mRtus_SubscribedClientId;

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
bleResult_t Rtus_Start (rtusConfig_t *pServiceConfig)
{
    return gBleSuccess_c;
}

bleResult_t Rtus_Stop (rtusConfig_t *pServiceConfig)
{
    mRtus_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Rtus_Subscribe(deviceId_t clientDeviceId)
{
    mRtus_SubscribedClientId = clientDeviceId;
    return gBleSuccess_c;
}

bleResult_t Rtus_Unsubscribe()
{
    mRtus_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Rtus_RecordTimeUpdateState (rtusConfig_t *pServiceConfig)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_TimeUpdateState_d);

    /* Get handle of  characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;
    
    /* Update characteristic value */
    result = GattDb_WriteAttribute(handle, sizeof(timeUpdateState_t), (uint8_t*)&pServiceConfig->timeUpdateState);

    if (result != gBleSuccess_c)
        return result;

    return gBleSuccess_c;
}

void Rtus_ControlPointHandler(rtusConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    timeUpdateCommand_t command;
      
    command = pEvent->aValue[0];
    
    switch(command)
    {
        case gRtus_GetReferenceUpdate_c:
        {
            pServiceConfig->timeUpdateState.currentState = gRtusUpdatePending_c;
            Rtus_RecordTimeUpdateState(pServiceConfig);
            break;
        }
        case gRtus_CancelReferenceUpdate_c:
        {
            pServiceConfig->timeUpdateState.currentState = gRtusIdle_c;
            pServiceConfig->timeUpdateState.result = gRtusCanceled_c;
            Rtus_RecordTimeUpdateState(pServiceConfig);
            break;
        }
        default: break;
    }
}

/***********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* @}
********************************************************************************** */
