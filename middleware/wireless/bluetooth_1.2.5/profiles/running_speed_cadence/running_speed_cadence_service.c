/*! *********************************************************************************
* \addtogroup Running Speed Cadence Service
* @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "FunctionLib.h"
#include "Messaging.h"

#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"

#include "running_speed_cadence_interface.h"

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
#define Rscs_SuppportMultipleSensorLoc(serviceCfg)\
        (serviceCfg->rscsFeatures & gRscs_MultipleSensorSupported_c)

/***********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */

/***********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
/*! Running Speed Cadence Service - Subscribed Client */
static deviceId_t mRscs_ClientDeviceId = gInvalidDeviceId_c;

/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
********************************************************************************** */
static bleResult_t Rscs_UpdateMeasurementCharacteristic
(
 uint16_t handle,
 rscsMeasurement_t *pMeasurement
);

static void Rscs_SendMeasurementNotification
(
  uint16_t handle
);

static bleResult_t Rscs_SetFeatures
(
    uint16_t        handle,
    rscsFeature_t   features
);

static void Rscs_SendProcedureResponse
(
     rscsConfig_t *pServiceConfig,
     gattServerAttributeWrittenEvent_t* pEvent
);

static bool_t Rscs_ValidateSensorLocation
(
     rscsSensorLocation_t sensorLoc,
     rscsSensorLocation_t* aSensorLoc,
     uint8_t count
);

/***********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

bleResult_t Rscs_Start (rscsConfig_t *pServiceConfig)
{
    bleResult_t result;
    uint16_t  handle;
    bleUuid_t uuid = Uuid16(gBleSig_RscFeature_d);

    pServiceConfig->procInProgress = FALSE;
		
    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    result = Rscs_SetFeatures(handle, pServiceConfig->rscsFeatures);

    if (result != gBleSuccess_c)
        return result;

    /* Get handle of characteristic */
    result = Rscs_SetSensorLocation(pServiceConfig->serviceHandle, pServiceConfig->sensorLocation);

    if (result != gBleSuccess_c)
        return result;

    return gBleSuccess_c;

}

bleResult_t Rscs_Stop (rscsConfig_t *pServiceConfig)
{
    return Rscs_Unsubscribe(mRscs_ClientDeviceId);
}

bleResult_t Rscs_Subscribe(deviceId_t clientdeviceId)
{
    mRscs_ClientDeviceId = clientdeviceId;

    return gBleSuccess_c;
}

bleResult_t Rscs_Unsubscribe()
{
    mRscs_ClientDeviceId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Rscs_RecordMeasurement(uint16_t serviceHandle, rscsMeasurement_t *pMeasurement)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_RscMeasurement_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Update characteristic value and send indication */
    if (!Rscs_UpdateMeasurementCharacteristic(handle, pMeasurement))
    {
        Rscs_SendMeasurementNotification(handle);
    }
    return gBleSuccess_c;
}

bleResult_t Rscs_SetSensorLocation(uint16_t serviceHandle, rscsSensorLocation_t sensorLocation)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_SensorLocation_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Write attribute value*/
    return GattDb_WriteAttribute(handle, sizeof(rscsSensorLocation_t), &sensorLocation);
}



void Rscs_ControlPointHandler (rscsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    bool_t  fIndicationActive = FALSE;
    uint16_t  handleCccd;
   
    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(pEvent->handle, &handleCccd) != gBleSuccess_c)
        return;

    /* Check if indications are properly configured */
    Gap_CheckIndicationStatus(mRscs_ClientDeviceId, handleCccd, &fIndicationActive);

    if(!fIndicationActive)
    {
        GattServer_SendAttributeWrittenStatus(mRscs_ClientDeviceId, pEvent->handle,
                                                 gAttErrCodeCccdImproperlyConfigured_c);
        return;
    }

    /* Check if another procedure is in progress */
    if (pServiceConfig->procInProgress)
    {
        GattServer_SendAttributeWrittenStatus(mRscs_ClientDeviceId, pEvent->handle,
                                                 gAttErrCodeProcedureAlreadyInProgress_c);
        return;
    }

    pServiceConfig->procInProgress = TRUE;

    /* Procedure received successfully */
    GattServer_SendAttributeWrittenStatus(mRscs_ClientDeviceId, pEvent->handle,
                                                     gAttErrCodeNoError_c);

    Rscs_SendProcedureResponse(pServiceConfig, pEvent);
}

bleResult_t Rscs_FinishSensorCalibration(rscsConfig_t *pServiceConfig, bool_t wasSuccessful)
{
    uint16_t    handle;
    bleResult_t result;
    uint16_t    uuid = gBleSig_ScControlPoint_d;
    uint8_t data[3] = {gRscs_RspCode_c, gRscs_StartSensorCalibration_c,
                        gRscs_Success_c};
    
    if (!wasSuccessful)
    {
        data[2] = gRscs_OperationFailed_c;
    }

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, (bleUuid_t*)&uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Write response in characteristic */
    GattDb_WriteAttribute(handle, sizeof(data), data);

    pServiceConfig->calibrationInProgress = FALSE;
    
    /* Indicate value to client */
    return GattServer_SendIndication(mRscs_ClientDeviceId, handle);
}                                              
/***********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static bleResult_t Rscs_UpdateMeasurementCharacteristic
(
    uint16_t handle,
    rscsMeasurement_t *pMeasurement
)
{
    uint8_t charValue[10];
    uint8_t index = 2 * sizeof(uint8_t) + sizeof(uint16_t);

    charValue[0] = pMeasurement->flags;
    FLib_MemCopy16Unaligned(&charValue[1], pMeasurement->instantSpeed);
    charValue[3] = pMeasurement->instantCadence;

    if (pMeasurement->flags & gRscs_InstantStrideLengthPresent_c)
    {
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->instantStrideLength);
        index += sizeof(uint16_t);
    }

    if (pMeasurement->flags & gRscs_TotalDistancePresent_c)
    {
        FLib_MemCopy32Unaligned(&charValue[index], pMeasurement->totalDistance);
        index += sizeof(uint32_t);
    }

    return GattDb_WriteAttribute(handle, index, &charValue[0]);
}

static void Rscs_SendMeasurementNotification
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
        (mRscs_ClientDeviceId, hCccd, &isNotificationActive) &&
        TRUE == isNotificationActive)
    {
        GattServer_SendNotification(mRscs_ClientDeviceId, handle);
    }
}

static bleResult_t Rscs_SetFeatures
(
    uint16_t        handle,
    rscsFeature_t   features
)
{
    return GattDb_WriteAttribute(handle, sizeof(rscsFeature_t), (uint8_t*)&features);
}

static void Rscs_SendProcedureResponse
(
     rscsConfig_t *pServiceConfig,
     gattServerAttributeWrittenEvent_t* pEvent
)
{
    uint8_t rspSize = 3;
    rscsProcedure_t*   pResponse;
    rscsProcedure_t*   pProcedure = (rscsProcedure_t*)pEvent->aValue;
    
    /* Allocate buffer for biggest response */
    pResponse = MSG_Alloc(rspSize + pServiceConfig->pUserData->cNumOfSupportedSensorLocations);

    if (pResponse == NULL)
    {
        return;
    }

    pResponse->opCode = gRscs_RspCode_c;
    pResponse->procedureData.response.reqOpCode = pProcedure->opCode;

    switch (pProcedure->opCode)
    {
        case gRscs_SetCummulativeValue_c:
        {
            pServiceConfig->pUserData->totalDistance = pProcedure->procedureData.setCummulativeValue;
            pResponse->procedureData.response.rspValue = gRscs_Success_c;
        }
        break;

        case gRscs_UpdateSensorLocation_c:
        {
            if(!Rscs_SuppportMultipleSensorLoc(pServiceConfig))
            {
                pResponse->procedureData.response.rspValue = gRscs_OpCodeNotSupported_c;
            }
            else if (!Rscs_ValidateSensorLocation(pProcedure->procedureData.updateSensorLocation,
                                                    pServiceConfig->pUserData->pSupportedSensorLocations,
                                                    pServiceConfig->pUserData->cNumOfSupportedSensorLocations))
            {
                pResponse->procedureData.response.rspValue = gRscs_InvalidParameter_c;
            }
            else
            {
                Rscs_SetSensorLocation(pServiceConfig->serviceHandle, pProcedure->procedureData.updateSensorLocation);
                pResponse->procedureData.response.rspValue = gRscs_Success_c;
            }
            break;
        }

        case gRscs_ReqSupportedSensorLoc_c:
        {
            pResponse->procedureData.response.rspValue = gRscs_Success_c;

            /* Copy in supported locations */
            FLib_MemCpy((uint8_t*)pResponse + rspSize,
                        pServiceConfig->pUserData->pSupportedSensorLocations,
                        pServiceConfig->pUserData->cNumOfSupportedSensorLocations * sizeof(rscsSensorLocation_t));
            rspSize += pServiceConfig->pUserData->cNumOfSupportedSensorLocations;
            break;
        }

        case gRscs_StartSensorCalibration_c:
        {
            /* Offset Compensation is started */
            pServiceConfig->calibrationInProgress = TRUE;
            MSG_Free(pResponse);
            return;

        }
		
        default:
        {
            pResponse->procedureData.response.rspValue = gRscs_OpCodeNotSupported_c;
            break;
        }
    }
    /* Write response in characteristic */
    GattDb_WriteAttribute(pEvent->handle, rspSize, (uint8_t*) pResponse);

    /* Free buffer */
    MSG_Free(pResponse);

    /* Indicate value to client */
    GattServer_SendIndication(mRscs_ClientDeviceId, pEvent->handle);
}

static bool_t Rscs_ValidateSensorLocation(rscsSensorLocation_t sensorLoc, rscsSensorLocation_t* aSensorLoc, uint8_t count)
{
    uint8_t i;
    for (i = 0; i < count; i++)
    {
        if(aSensorLoc[i] == sensorLoc)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*! *********************************************************************************
* @}
********************************************************************************** */
