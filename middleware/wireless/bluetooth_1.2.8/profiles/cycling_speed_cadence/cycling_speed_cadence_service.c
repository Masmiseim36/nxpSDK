/*! *********************************************************************************
* \addtogroup Cycling Speed Cadence Service
* @{
 ********************************************************************************** */
/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
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
#include "Messaging.h"

#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"

#include "cycling_speed_cadence_interface.h"

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
#define Cscs_SuppportMultipleSensorLoc(serviceCfg)\
        (serviceCfg->cscsFeatures & gCscs_MultipleSensorLocationsSupported_c)

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
/*! Cycling Speed Cadence Service - Subscribed Client */
static deviceId_t mCscs_ClientDeviceId = gInvalidDeviceId_c;

/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
********************************************************************************** */
static bleResult_t Cscs_UpdateMeasurementCharacteristic
(
 uint16_t handle,
 cscsMeasurement_t *pMeasurement
);

static void Cscs_SendMeasurementNotification
(
  uint16_t handle
);

static bleResult_t Cscs_SetFeatures
(
    uint16_t        handle,
    cscsFeature_t   features
);

static void Cscs_SendProcedureResponse
(
     cscsConfig_t *pServiceConfig,
     gattServerAttributeWrittenEvent_t* pEvent
);

static bool_t Cscs_ValidateSensorLocation
(
     cscsSensorLocation_t sensorLoc,
     cscsSensorLocation_t* aSensorLoc,
     uint8_t count
);

/***********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

bleResult_t Cscs_Start (cscsConfig_t *pServiceConfig)
{
    bleResult_t result;
    uint16_t  handle;
    bleUuid_t uuid = Uuid16(gBleSig_CscFeature_d);

    pServiceConfig->procInProgress = FALSE;
		
    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    result = Cscs_SetFeatures(handle, pServiceConfig->cscsFeatures);

    if (result != gBleSuccess_c)
        return result;

    /* Get handle of characteristic */
    result = Cscs_SetSensorLocation(pServiceConfig->serviceHandle, pServiceConfig->sensorLocation);

    if (result != gBleSuccess_c)
        return result;

    return gBleSuccess_c;

}

bleResult_t Cscs_Stop (cscsConfig_t *pServiceConfig)
{
    return Cscs_Unsubscribe(mCscs_ClientDeviceId);
}

bleResult_t Cscs_Subscribe(deviceId_t clientdeviceId)
{
    mCscs_ClientDeviceId = clientdeviceId;

    return gBleSuccess_c;
}

bleResult_t Cscs_Unsubscribe()
{
    mCscs_ClientDeviceId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Cscs_RecordMeasurement(uint16_t serviceHandle, cscsMeasurement_t *pMeasurement)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_CscMeasurement_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Update characteristic value and send indication */
    if (!Cscs_UpdateMeasurementCharacteristic(handle, pMeasurement))
    {
        Cscs_SendMeasurementNotification(handle);
    }
    return gBleSuccess_c;
}

bleResult_t Cscs_SetSensorLocation(uint16_t serviceHandle, cscsSensorLocation_t sensorLocation)
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
    return GattDb_WriteAttribute(handle, sizeof(cscsSensorLocation_t), &sensorLocation);
}



void Cscs_ControlPointHandler (cscsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    bool_t  fIndicationActive = FALSE;
    uint16_t  handleCccd;
   
    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(pEvent->handle, &handleCccd) != gBleSuccess_c)
        return;

    /* Check if indications are properly configured */
    Gap_CheckIndicationStatus(mCscs_ClientDeviceId, handleCccd, &fIndicationActive);

    if(!fIndicationActive)
    {
        GattServer_SendAttributeWrittenStatus(mCscs_ClientDeviceId, pEvent->handle,
                                                 gAttErrCodeCccdImproperlyConfigured_c);
        return;
    }

    /* Check if another procedure is in progress */
    if (pServiceConfig->procInProgress)
    {
        GattServer_SendAttributeWrittenStatus(mCscs_ClientDeviceId, pEvent->handle,
                                                 gAttErrCodeProcedureAlreadyInProgress_c);
        return;
    }

    pServiceConfig->procInProgress = TRUE;

    /* Procedure received successfully */
    GattServer_SendAttributeWrittenStatus(mCscs_ClientDeviceId, pEvent->handle,
                                                     gAttErrCodeNoError_c);

    Cscs_SendProcedureResponse(pServiceConfig, pEvent);
}

bleResult_t Cscs_FinishSensorCalibration(cscsConfig_t *pServiceConfig, bool_t wasSuccessful)
{
    uint16_t    handle;
    bleResult_t result;
    uint16_t    uuid = gBleSig_ScControlPoint_d;
    uint8_t data[3] = {gCscs_RspCode_c, gCscs_StartSensorCalibration_c,
                        gCscs_Success_c};
    
    if (!wasSuccessful)
    {
        data[2] = gCscs_OperationFailed_c;
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
    return GattServer_SendIndication(mCscs_ClientDeviceId, handle);
}                                              
/***********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static bleResult_t Cscs_UpdateMeasurementCharacteristic
(
 uint16_t handle,
 cscsMeasurement_t *pMeasurement
)
{
    uint8_t charValue[13];
    uint8_t index = 1;

    charValue[0] = 0x00;

    if (pMeasurement->wheelRevDataPresent)
    {
        charValue[0] |= gCscs_WheelRevDataPresent_c;
        FLib_MemCopy32Unaligned(&charValue[index], pMeasurement->cumulativeWheelRevs);
        index += sizeof(uint32_t);
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->lastWheelEventTime);
        index += sizeof(uint16_t);
    }

    if (pMeasurement->crankRevDataPresent)
    {
        charValue[0] |= gCscs_CrankRevDataPresent_c;
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->cumulativeCrankRevs);
        index += sizeof(uint16_t);
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->lastCrankEventTime);
        index += sizeof(uint16_t);
    }

    return GattDb_WriteAttribute(handle, index, &charValue[0]);
}

static void Cscs_SendMeasurementNotification
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
        (mCscs_ClientDeviceId, hCccd, &isNotificationActive) &&
        TRUE == isNotificationActive)
    {
        GattServer_SendNotification(mCscs_ClientDeviceId, handle);
    }
}

static bleResult_t Cscs_SetFeatures
(
    uint16_t        handle,
    cscsFeature_t   features
)
{
    return GattDb_WriteAttribute(handle, sizeof(cscsFeature_t), (uint8_t*)&features);
}

static void Cscs_SendProcedureResponse
(
     cscsConfig_t *pServiceConfig,
     gattServerAttributeWrittenEvent_t* pEvent
)
{
    uint8_t rspSize = 3;
    cscsProcedure_t*   pResponse;
    cscsProcedure_t*   pProcedure = (cscsProcedure_t*)pEvent->aValue;
    
    /* Allocate buffer for biggest response */
    pResponse = MSG_Alloc(rspSize + pServiceConfig->pUserData->cNumOfSupportedSensorLocations);

    if (pResponse == NULL)
    {
        return;
    }

    pResponse->opCode = gCscs_RspCode_c;
    pResponse->procedureData.response.reqOpCode = pProcedure->opCode;

    switch (pProcedure->opCode)
    {
        case gCscs_SetCummulativeValue_c:
        {
            pServiceConfig->pUserData->cumulativeWheelRevs = pProcedure->procedureData.setCummulativeValue;
            pResponse->procedureData.response.rspValue = gCscs_Success_c;
        }
        break;

        case gCscs_UpdateSensorLocation_c:
        {
            if(!Cscs_SuppportMultipleSensorLoc(pServiceConfig))
            {
                pResponse->procedureData.response.rspValue = gCscs_OpCodeNotSupported_c;
            }
            else if (!Cscs_ValidateSensorLocation(pProcedure->procedureData.updateSensorLocation,
                                                    pServiceConfig->pUserData->pSupportedSensorLocations,
                                                    pServiceConfig->pUserData->cNumOfSupportedSensorLocations))
            {
                pResponse->procedureData.response.rspValue = gCscs_InvalidParameter_c;
            }
            else
            {
                Cscs_SetSensorLocation(pServiceConfig->serviceHandle, pProcedure->procedureData.updateSensorLocation);
                pResponse->procedureData.response.rspValue = gCscs_Success_c;
            }
            break;
        }

        case gCscs_ReqSupportedSensorLoc_c:
        {
            pResponse->procedureData.response.rspValue = gCscs_Success_c;

            /* Copy in supported locations */
            FLib_MemCpy((uint8_t*)pResponse + rspSize,
                        pServiceConfig->pUserData->pSupportedSensorLocations,
                        pServiceConfig->pUserData->cNumOfSupportedSensorLocations * sizeof(cscsSensorLocation_t));
            rspSize += pServiceConfig->pUserData->cNumOfSupportedSensorLocations;
            break;
        }

        case gCscs_StartSensorCalibration_c:
        {
            /* Offset Compensation is started */
            pServiceConfig->calibrationInProgress = TRUE;
            MSG_Free(pResponse);
            return;

        }
		
        default:
        {
            pResponse->procedureData.response.rspValue = gCscs_OpCodeNotSupported_c;
            break;
        }
    }
    /* Write response in characteristic */
    GattDb_WriteAttribute(pEvent->handle, rspSize, (uint8_t*) pResponse);

    /* Free buffer */
    MSG_Free(pResponse);

    /* Indicate value to client */
    GattServer_SendIndication(mCscs_ClientDeviceId, pEvent->handle);
}

static bool_t Cscs_ValidateSensorLocation(cscsSensorLocation_t sensorLoc, cscsSensorLocation_t* aSensorLoc, uint8_t count)
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
