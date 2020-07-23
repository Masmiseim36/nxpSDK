/*! *********************************************************************************
* \addtogroup Cycling Speed Cadence Service
* @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
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

#include "cycling_speed_cadence_interface.h"

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
#define Cscs_SuppportMultipleSensorLoc(serviceCfg)\
        ((serviceCfg)->cscsFeatures & gCscs_MultipleSensorLocationsSupported_c)

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

    if (result == gBleSuccess_c)
    {
        result = Cscs_SetFeatures(handle, pServiceConfig->cscsFeatures);

        if (result == gBleSuccess_c)
        {
            /* Get handle of characteristic */
            result = Cscs_SetSensorLocation(pServiceConfig->serviceHandle, pServiceConfig->sensorLocation);
        }
    }

    return result;
}

bleResult_t Cscs_Stop (cscsConfig_t *pServiceConfig)
{
    return Cscs_Unsubscribe();
}

bleResult_t Cscs_Subscribe(deviceId_t clientdeviceId)
{
    mCscs_ClientDeviceId = clientdeviceId;

    return gBleSuccess_c;
}

bleResult_t Cscs_Unsubscribe(void)
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

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value and send indication */
        if (gBleSuccess_c == Cscs_UpdateMeasurementCharacteristic(handle, pMeasurement))
        {
            Cscs_SendMeasurementNotification(handle);
        }
    }

    return result;
}

bleResult_t Cscs_SetSensorLocation(uint16_t serviceHandle, cscsSensorLocation_t sensorLocation)
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
        result = GattDb_WriteAttribute(handle, sizeof(cscsSensorLocation_t), &sensorLocation);
    }

    return result;
}



void Cscs_ControlPointHandler (cscsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    bool_t  fIndicationActive = FALSE;
    uint16_t  handleCccd;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(pEvent->handle, &handleCccd) == gBleSuccess_c)
    {
        /* Check if indications are properly configured */
        (void)Gap_CheckIndicationStatus(mCscs_ClientDeviceId, handleCccd, &fIndicationActive);

        if(!fIndicationActive)
        {
            (void)GattServer_SendAttributeWrittenStatus(mCscs_ClientDeviceId, pEvent->handle,
                                                     gAttAppErrCodeCccdImproperlyConfigured_c);
        }
        else
        {
            /* Check if another procedure is in progress */
            if (pServiceConfig->procInProgress)
            {
                (void)GattServer_SendAttributeWrittenStatus(mCscs_ClientDeviceId, pEvent->handle,
                                                         gAttAppErrCodeProcedureAlreadyInProgress_c);
            }
            else
            {
                pServiceConfig->procInProgress = TRUE;

                /* Procedure received successfully */
                (void)GattServer_SendAttributeWrittenStatus(mCscs_ClientDeviceId, pEvent->handle,
                                                                 (uint8_t)gAttErrCodeNoError_c);

                Cscs_SendProcedureResponse(pServiceConfig, pEvent);
            }
        }
    }
}

bleResult_t Cscs_FinishSensorCalibration(cscsConfig_t *pServiceConfig, bool_t wasSuccessful)
{
    uint16_t    handle;
    bleResult_t result;
    union
    {
        uint16_t*   uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;
    uint16_t    uuid = gBleSig_ScControlPoint_d;
    uuidVars.uuidTemp = &uuid;

    uint8_t data[3] = {gCscs_RspCode_c, gCscs_StartSensorCalibration_c,
                        gCscs_Success_c};

    if (!wasSuccessful)
    {
        data[2] = gCscs_OperationFailed_c;
    }

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

    if (result == gBleSuccess_c)
    {
        /* Write response in characteristic */
        (void)GattDb_WriteAttribute(handle, (uint16_t)sizeof(data), data);

        pServiceConfig->calibrationInProgress = FALSE;

        /* Indicate value to client */
        result = GattServer_SendIndication(mCscs_ClientDeviceId, handle);
    }

    return result;
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
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) == gBleSuccess_c)
    {
        if (gBleSuccess_c == Gap_CheckNotificationStatus
            (mCscs_ClientDeviceId, hCccd, &isNotificationActive) &&
            TRUE == isNotificationActive)
        {
            (void)GattServer_SendNotification(mCscs_ClientDeviceId, handle);
        }
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
    union
    {
        uint8_t*          aValueTemp;
        cscsProcedure_t*  cscsProcedureTemp;
    }cscsProcedureVars;
    cscsProcedureVars.aValueTemp = pEvent->aValue;

    cscsProcedure_t*   pResponse;
    cscsProcedure_t*   pProcedure = cscsProcedureVars.cscsProcedureTemp;
    bool_t             earlyReturn = FALSE;

    /* Allocate buffer for biggest response */
    pResponse = MSG_Alloc((uint32_t)rspSize + (uint32_t)pServiceConfig->pUserData->cNumOfSupportedSensorLocations);

    if (pResponse != NULL)
    {
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
                if(0U == Cscs_SuppportMultipleSensorLoc(pServiceConfig))
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
                    (void)Cscs_SetSensorLocation(pServiceConfig->serviceHandle, pProcedure->procedureData.updateSensorLocation);
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
                            (uint32_t)pServiceConfig->pUserData->cNumOfSupportedSensorLocations * sizeof(cscsSensorLocation_t));
                rspSize += pServiceConfig->pUserData->cNumOfSupportedSensorLocations;
                break;
            }

            case gCscs_StartSensorCalibration_c:
            {
                /* Offset Compensation is started */
                pServiceConfig->calibrationInProgress = TRUE;
                earlyReturn = TRUE;
                break;
            }

            default:
            {
                pResponse->procedureData.response.rspValue = gCscs_OpCodeNotSupported_c;
                break;
            }
        }

        if (!earlyReturn)
        {
            /* Write response in characteristic */
            (void)GattDb_WriteAttribute(pEvent->handle, rspSize, (uint8_t*) pResponse);

            /* Indicate value to client */
            (void)GattServer_SendIndication(mCscs_ClientDeviceId, pEvent->handle);
        }

        /* Free buffer */
        (void)MSG_Free(pResponse);
    }
}

static bool_t Cscs_ValidateSensorLocation(cscsSensorLocation_t sensorLoc, cscsSensorLocation_t *aSensorLoc, uint8_t count)
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
