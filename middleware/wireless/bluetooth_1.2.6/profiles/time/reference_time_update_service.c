/*! *********************************************************************************
* \addtogroup BLE PROFILES
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
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
