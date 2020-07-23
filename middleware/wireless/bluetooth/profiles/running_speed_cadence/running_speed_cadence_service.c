/*! *********************************************************************************
* \addtogroup Running Speed Cadence Service
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
        ((serviceCfg)->rscsFeatures & gRscs_MultipleSensorSupported_c)

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

    if (result == gBleSuccess_c)
    {
        result = Rscs_SetFeatures(handle, pServiceConfig->rscsFeatures);

        if (result == gBleSuccess_c)
        {
            /* Get handle of characteristic */
            result = Rscs_SetSensorLocation(pServiceConfig->serviceHandle, pServiceConfig->sensorLocation);
        }
    }

    return result;
}

bleResult_t Rscs_Stop (rscsConfig_t *pServiceConfig)
{
    return Rscs_Unsubscribe();
}

bleResult_t Rscs_Subscribe(deviceId_t clientdeviceId)
{
    mRscs_ClientDeviceId = clientdeviceId;

    return gBleSuccess_c;
}

bleResult_t Rscs_Unsubscribe(void)
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

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value and send indication */
        if (gBleSuccess_c == Rscs_UpdateMeasurementCharacteristic(handle, pMeasurement))
        {
            Rscs_SendMeasurementNotification(handle);
        }
    }

    return result;
}

bleResult_t Rscs_SetSensorLocation(uint16_t serviceHandle, rscsSensorLocation_t sensorLocation)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_SensorLocation_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result == gBleSuccess_c)
    {
        /* Write attribute value*/
        result = GattDb_WriteAttribute(handle, sizeof(rscsSensorLocation_t), &sensorLocation);
    }

    return result;
}

void Rscs_ControlPointHandler (rscsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    bool_t  fIndicationActive = FALSE;
    uint16_t  handleCccd;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(pEvent->handle, &handleCccd) == gBleSuccess_c)
    {
        /* Check if indications are properly configured */
        (void)Gap_CheckIndicationStatus(mRscs_ClientDeviceId, handleCccd, &fIndicationActive);

        if(!fIndicationActive)
        {
            (void)GattServer_SendAttributeWrittenStatus(mRscs_ClientDeviceId, pEvent->handle,
                                                     gAttAppErrCodeCccdImproperlyConfigured_c);
        }
        else
        {
            /* Check if another procedure is in progress */
            if (pServiceConfig->procInProgress)
            {
                (void)GattServer_SendAttributeWrittenStatus(mRscs_ClientDeviceId, pEvent->handle,
                                                         gAttAppErrCodeProcedureAlreadyInProgress_c);
            }
            else
            {
                pServiceConfig->procInProgress = TRUE;

                /* Procedure received successfully */
                (void)GattServer_SendAttributeWrittenStatus(mRscs_ClientDeviceId, pEvent->handle,
                                                                 (uint8_t)gAttErrCodeNoError_c);

                Rscs_SendProcedureResponse(pServiceConfig, pEvent);
            }
        }
    }
}

bleResult_t Rscs_FinishSensorCalibration(rscsConfig_t *pServiceConfig, bool_t wasSuccessful)
{
    union
    {
        uint16_t*    uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;

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
    uuidVars.uuidTemp = &uuid;
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

    if (result == gBleSuccess_c)
    {
        /* Write response in characteristic */
        (void)GattDb_WriteAttribute(handle, (uint16_t)sizeof(data), data);

        pServiceConfig->calibrationInProgress = FALSE;

        /* Indicate value to client */
        result = GattServer_SendIndication(mRscs_ClientDeviceId, handle);
    }

    return result;
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
    uint8_t index = 2U * sizeof(uint8_t) + sizeof(uint16_t);

    charValue[0] = pMeasurement->flags;
    FLib_MemCopy16Unaligned(&charValue[1], pMeasurement->instantSpeed);
    charValue[3] = pMeasurement->instantCadence;

    if (0U != (pMeasurement->flags & gRscs_InstantStrideLengthPresent_c))
    {
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->instantStrideLength);
        index += sizeof(uint16_t);
    }

    if (0U != (pMeasurement->flags & gRscs_TotalDistancePresent_c))
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
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) == gBleSuccess_c)
    {
        if (gBleSuccess_c == Gap_CheckNotificationStatus
            (mRscs_ClientDeviceId, hCccd, &isNotificationActive) &&
            TRUE == isNotificationActive)
        {
            (void)GattServer_SendNotification(mRscs_ClientDeviceId, handle);
        }
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
    union
    {
        uint8_t*            aValueTemp;
        rscsProcedure_t*    rscsProcedureTemp;
    }rscsProcedureVars;
    bool_t earlyExit = FALSE;

    uint8_t rspSize = 3;
    rscsProcedure_t*   pResponse;
    rscsProcedureVars.aValueTemp = pEvent->aValue;
    rscsProcedure_t*   pProcedure = rscsProcedureVars.rscsProcedureTemp;

    /* Allocate buffer for biggest response */
    pResponse = MSG_Alloc((uint32_t)rspSize + (uint32_t)pServiceConfig->pUserData->cNumOfSupportedSensorLocations);

    if (pResponse != NULL)
    {
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
                if(0U == Rscs_SuppportMultipleSensorLoc(pServiceConfig))
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
                    (void)Rscs_SetSensorLocation(pServiceConfig->serviceHandle, pProcedure->procedureData.updateSensorLocation);
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
                            (uint32_t)pServiceConfig->pUserData->cNumOfSupportedSensorLocations * sizeof(rscsSensorLocation_t));
                rspSize += pServiceConfig->pUserData->cNumOfSupportedSensorLocations;
                break;
            }

            case gRscs_StartSensorCalibration_c:
            {
                /* Offset Compensation is started */
                pServiceConfig->calibrationInProgress = TRUE;
                earlyExit = TRUE;
                break;
            }

            default:
            {
                pResponse->procedureData.response.rspValue = gRscs_OpCodeNotSupported_c;
                break;
            }
        }

        if (earlyExit == FALSE)
        {
            /* Write response in characteristic */
            (void)GattDb_WriteAttribute(pEvent->handle, rspSize, (uint8_t*) pResponse);

            /* Indicate value to client */
            (void)GattServer_SendIndication(mRscs_ClientDeviceId, pEvent->handle);
        }

        /* Free buffer */
        (void)MSG_Free(pResponse);
    }
}

static bool_t Rscs_ValidateSensorLocation(rscsSensorLocation_t sensorLoc, rscsSensorLocation_t *aSensorLoc, uint8_t count)
{
    bool_t result = FALSE;

    for (uint32_t i = 0; i < (uint32_t)count; i++)
    {
        if (aSensorLoc[i] == sensorLoc)
        {
            result = TRUE;
            break;
        }
    }

    return result;
}

/*! *********************************************************************************
* @}
********************************************************************************** */
