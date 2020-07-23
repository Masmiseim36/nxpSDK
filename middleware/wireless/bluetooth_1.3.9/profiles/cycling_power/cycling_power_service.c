/*! *********************************************************************************
* \addtogroup Cycling Power Service
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

#include "cycling_power_interface.h"

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
#define Cps_SuppportMultipleSensorLoc(serviceCfg)\
        (((serviceCfg)->cpsFeatures & gCps_MultipleSensorLocationsSupported_c) != 0U)

#define Cps_BothTorqueAndForce(measurementFlags)\
        ((((measurementFlags) & gCps_ExtremeForceMagnitudesPresent_c) != 0U) &&\
         (((measurementFlags) & gCps_ExtremeTorqueMagnitudesPresent_c) != 0U))

#define Cps_BothTorqueAndForceVector(vectorFlags)\
        ((((vectorFlags) & gCps_VectorInstantForceMagArrayPresent_c) != 0U) &&\
         (((vectorFlags) & gCps_VectorInstantTorqueMagArrayPresent_c) != 0U))

#define Cps_TorqueOrForceVector(vectorFlags)\
        ((vectorFlags) & (gCps_VectorInstantForceMagArrayPresent_c | gCps_VectorInstantTorqueMagArrayPresent_c))
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
/*! Cycling Power Service - Subscribed Client */
static deviceId_t mCps_ClientDeviceId = gInvalidDeviceId_c;

static const  ctsDateTime_t mFactoryCalibrationDate = {gCps_FactoryCalibrationDate_c};
/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
********************************************************************************** */
static bleResult_t Cps_UpdateMeasurementCharacteristic
(
    uint16_t handle,
    cpsMeasurement_t *pMeasurement
);

static bleResult_t Cps_UpdatePowerVectorCharacteristic
(
    uint16_t handle,
    cpsPowerVector_t *pPowerVector
);

static void Cps_SendNotification
(
     uint16_t handle
);

static bleResult_t Cps_SetFeatures
(
    uint16_t        handle,
    cpsFeature_t    features
);

static void Cps_SendProcedureResponse
(
     cpsConfig_t *pServiceConfig,
     gattServerAttributeWrittenEvent_t* pEvent
);

static bool_t Cps_ValidateSensorLocation
(
    cpsSensorLocation_t sensorLoc,
    const cpsSensorLocation_t *aSensorLoc,
    uint8_t count
);

#if gCyclPwrs_SendSccdWritenStatus_d
static void Cps_PowerMeasurementSccdWritten
(
cpsConfig_t *pServiceConfig,
gattServerAttributeWrittenEvent_t *pEvent
);
#endif
/***********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

bleResult_t Cps_Start (cpsConfig_t *pServiceConfig)
{
    bleResult_t result = gBleSuccess_c;
    uint16_t    handle;
    uint16_t    uuid = gBleSig_CpFeature_d;
    union
    {
        uint16_t*   uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;
    uuidVars.uuidTemp = &uuid;

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

    if (result == gBleSuccess_c)
    {
        result = Cps_SetFeatures(handle, pServiceConfig->cpsFeatures);
    }

    if (result == gBleSuccess_c)
    {
        /* Get handle of characteristic */
        result = Cps_SetSensorLocation(pServiceConfig->serviceHandle, pServiceConfig->sensorLocation);
    }

    return result;

}

bleResult_t Cps_Stop (cpsConfig_t *pServiceConfig)
{
    return Cps_Unsubscribe();
}

bleResult_t Cps_Subscribe(deviceId_t clientdeviceId)
{
    mCps_ClientDeviceId = clientdeviceId;

    return gBleSuccess_c;
}

bleResult_t Cps_Unsubscribe(void)
{
    mCps_ClientDeviceId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Cps_RecordMeasurement(uint16_t serviceHandle, cpsMeasurement_t *pMeasurement)
{
    uint16_t    handle;
    bleResult_t result;
    uint16_t    uuid = gBleSig_CpMeasurement_d;
    union
    {
        uint16_t*   uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;
    uuidVars.uuidTemp = &uuid;

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value and send indication */
        if (gBleSuccess_c == Cps_UpdateMeasurementCharacteristic(handle, pMeasurement))
        {
            Cps_SendNotification(handle);
        }
    }

    return result;
}

bleResult_t Cps_RecordPowerVector(uint16_t serviceHandle, cpsPowerVector_t *pPowerVector)
{
    uint16_t    handle;
    bleResult_t result;
    uint16_t    uuid = gBleSig_CpVector_d;
    union
    {
        uint16_t*   uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;
    uuidVars.uuidTemp = &uuid;

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value and send indication */
        if (gBleSuccess_c == Cps_UpdatePowerVectorCharacteristic(handle, pPowerVector))
        {
            Cps_SendNotification(handle);
        }
    }

    return result;
}

bleResult_t Cps_SetSensorLocation(uint16_t serviceHandle, cpsSensorLocation_t sensorLocation)
{
    uint16_t    handle;
    bleResult_t result;
    uint16_t    uuid = gBleSig_SensorLocation_d;
    union
    {
        uint16_t*   uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;
    uuidVars.uuidTemp = &uuid;

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

    if (result == gBleSuccess_c)
    {
        /* Write attribute value */
        result = GattDb_WriteAttribute(handle, sizeof(cpsSensorLocation_t), &sensorLocation);
    }

    return result;
}

void Cps_ControlPointHandler (cpsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    bool_t  fIndicationActive = FALSE;
    uint16_t  handleCccd;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(pEvent->handle, &handleCccd) == gBleSuccess_c)
    {
        /* Check if indications are properly configured */
        (void)Gap_CheckIndicationStatus(mCps_ClientDeviceId, handleCccd, &fIndicationActive);

        if(!fIndicationActive)
        {
            (void)GattServer_SendAttributeWrittenStatus(mCps_ClientDeviceId, pEvent->handle,
                                                     (uint8_t)gAttErrCodeCccdImproperlyConfigured_c);
        }
        else
        {
            /* Check if another procedure is in progress */
            if (pServiceConfig->procInProgress)
            {
                (void)GattServer_SendAttributeWrittenStatus(mCps_ClientDeviceId, pEvent->handle,
                                                         (uint8_t)gAttErrCodeProcedureAlreadyInProgress_c);
            }
            else
            {
                pServiceConfig->procInProgress = TRUE;

                /* Procedure received successfully */
                (void)GattServer_SendAttributeWrittenStatus(mCps_ClientDeviceId, pEvent->handle,
                                                                 (uint8_t)gAttErrCodeNoError_c);

                Cps_SendProcedureResponse(pServiceConfig, pEvent);
            }
        }
    }
}

bleResult_t Cps_FinishOffsetCompensation(cpsConfig_t *pServiceConfig, uint16_t rawForceTorque)
{
    uint16_t    handle;
    bleResult_t result;
    uint16_t    uuid = gBleSig_CpControlPoint_d;
    union
    {
        uint16_t*   uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;
    uuidVars.uuidTemp = &uuid;

    uint8_t data[5] = {gCps_RspCode_c, gCps_StartOffsetCompensation_c,
                        gCps_Success_c, 0x00, 0x00};

    data[3] = (uint8_t)(rawForceTorque & 0xFFU);
    data[4] = (uint8_t)(rawForceTorque >> 8U);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

    if (result == gBleSuccess_c)
    {
        /* Write response in characteristic */
        (void)GattDb_WriteAttribute(handle, (uint16_t)sizeof(data), data);

        pServiceConfig->offsetCompensationOngoing = FALSE;

        /* Indicate value to client */
        result = GattServer_SendIndication(mCps_ClientDeviceId, handle);
    }

    return result;
}

#if gCyclPwrs_SendSccdWritenStatus_d
static void Cps_PowerMeasurementSccdWritten (cpsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    bool_t  fIndicationActive = FALSE;
    uint16_t  handleCccd;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(pEvent->handle, &handleCccd) == gBleSuccess_c)
    {
        /* Check if indications are properly configured */
        (void)Gap_CheckIndicationStatus(mCps_ClientDeviceId, handleCccd, &fIndicationActive);

        if(!fIndicationActive)
        {
            (void)GattServer_SendAttributeWrittenStatus(mCps_ClientDeviceId, pEvent->handle,
                                                     (uint8_t)gAttErrCodeCccdImproperlyConfigured_c);
        }
        else
        {
            /* Check if another procedure is in progress */
            if (pServiceConfig->procInProgress)
            {
                (void)GattServer_SendAttributeWrittenStatus(mCps_ClientDeviceId, pEvent->handle,
                                                         (uint8_t)gAttErrCodeProcedureAlreadyInProgress_c);
            }
            else
            {
                /* Procedure received successfully */
                (void)GattServer_SendAttributeWrittenStatus(mCps_ClientDeviceId, pEvent->handle,
                                                                 (uint8_t)gAttErrCodeNoError_c);

                Cps_SendProcedureResponse(pServiceConfig, pEvent);
            }
        }
    }
}
#endif

/***********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static bleResult_t Cps_UpdateMeasurementCharacteristic
(
    uint16_t handle,
    cpsMeasurement_t *pMeasurement
)
{
    bleResult_t result;
    uint8_t charValue[31];
    uint8_t index = 0;

    if (Cps_BothTorqueAndForce(pMeasurement->flags))
    {
        result = gBleInvalidParameter_c;
    }
    else
    {
        /* Copy flags and instantaneous power (4 bytes) */
        FLib_MemCpy(&charValue[index], &pMeasurement->flags, 2U*sizeof(uint16_t));
        index += 2U * sizeof(uint16_t);

        if (0U != (pMeasurement->flags & gCps_PedalPowerBalancePresent_c))
        {
            charValue[index] = pMeasurement->pedalPowerBalance;
            index += 1U;
        }

        if (0U != (pMeasurement->flags & gCps_AccumulatedTorquePresent_c))
        {
            FLib_MemCpy(&charValue[index], &pMeasurement->accumulatedTorque, sizeof(uint16_t));
            index += sizeof(uint16_t);
        }

        if (0U != (pMeasurement->flags & gCps_WheelRevolutionDataPresent_c))
        {
            FLib_MemCpy(&charValue[index], &pMeasurement->cumulativeWheelRevs,
                sizeof(uint16_t) + sizeof(uint32_t));
            index += (sizeof(uint16_t) + sizeof(uint32_t));
        }

        if (0U != (pMeasurement->flags & gCps_CrankRevolutionDataPresent_c))
        {
            FLib_MemCpy(&charValue[index], &pMeasurement->cumulativeCrankRevs,
                        2U * sizeof(uint16_t));
            index += (2U * sizeof(uint16_t));
        }

        if ((0U != (pMeasurement->flags & gCps_ExtremeForceMagnitudesPresent_c)) ||
            (0U != (pMeasurement->flags & gCps_ExtremeTorqueMagnitudesPresent_c)))
        {
            FLib_MemCpy(&charValue[index], &pMeasurement->magnitude, 2U * sizeof(uint16_t));
            index += (2U * sizeof(uint16_t));
        }

        if (0U != (pMeasurement->flags & gCps_ExtremeAnglesPresent_c))
        {
            FLib_MemCpy(&charValue[index], &pMeasurement->extremeAngles, 3U * sizeof(uint8_t));
            index += (3U * sizeof(uint8_t));
        }

        if (0U != (pMeasurement->flags & gCps_TopDeadSpotAnglesPresent_c))
        {
            FLib_MemCpy(&charValue[index], &pMeasurement->topDeadSpotAngle, sizeof(uint16_t));
            index += sizeof(uint16_t);
        }

        if (0U != (pMeasurement->flags & gCps_BotomDeadSpotAnglesPresent_c))
        {
            FLib_MemCpy(&charValue[index], &pMeasurement->bottomDeadSpotAngle, sizeof(uint16_t));
            index += sizeof(uint16_t);
        }

        if (0U != (pMeasurement->flags & gCps_AccumulatedEnergyPresent_c))
        {
            FLib_MemCpy(&charValue[index], &pMeasurement->accumulatedEnergy, sizeof(uint16_t));
            index += sizeof(uint16_t);
        }

        result = GattDb_WriteAttribute(handle, index, &charValue[0]);
    }

    return result;
}

static void Cps_SendNotification
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
            (mCps_ClientDeviceId, hCccd, &isNotificationActive) &&
            TRUE == isNotificationActive)
        {
            (void)GattServer_SendNotification(mCps_ClientDeviceId, handle);
        }
    }
}

static bleResult_t Cps_UpdatePowerVectorCharacteristic
(
    uint16_t handle,
    cpsPowerVector_t *pPowerVector
)
{
    bleResult_t result;
    uint8_t *pCursor = (uint8_t *)pPowerVector + 1;
    uint8_t length;

    if (Cps_BothTorqueAndForceVector(pPowerVector->flags))
    {
        result = gBleInvalidParameter_c;
    }
    else
    {
        /* Do in-place processing */

        /* Flags remain in place */
        pCursor += 1;

        if (0U != (pPowerVector->flags & gCps_VectorCrankRevDataPresent_c))
        {
            /* Crank data remains in place*/
            pCursor += 2U * sizeof(uint16_t);
        }

        if (0U != (pPowerVector->flags & gCps_VectorFirstCrankMeasAnglePresent_c))
        {
            /* Crank data remains in place*/
            pCursor += sizeof(uint16_t);
        }

        if (0U != Cps_TorqueOrForceVector(pPowerVector->flags) && (pPowerVector->arrayLength > 0U))
        {
            FLib_MemInPlaceCpy(pCursor, pPowerVector->instantMagnitudeArray, (uint32_t)pPowerVector->arrayLength * sizeof(uint16_t));
            pCursor += pPowerVector->arrayLength * sizeof(uint16_t);
        }

        length = (uint8_t)((int8_t)(pCursor - (uint8_t *)pPowerVector - 1));

        result = GattDb_WriteAttribute(handle, length, (uint8_t *)pPowerVector + 1);
    }

    return result;
}

static bleResult_t Cps_SetFeatures
(
    uint16_t        handle,
    cpsFeature_t    features
)
{
    return GattDb_WriteAttribute(handle, sizeof(cpsFeature_t), (uint8_t*)&features);
}

static void Cps_SendProcedureResponse
(
     cpsConfig_t *pServiceConfig,
     gattServerAttributeWrittenEvent_t* pEvent
)
{
    bool_t earlyReturn = FALSE;
    uint8_t rspSize = 3;
    union
    {
        uint8_t*         aValueTemp;
        cpsProcedure_t*  pProcedureTemp;
    }scProcedureVars;
    scProcedureVars.aValueTemp = pEvent->aValue;

    cpsProcedure_t*   pResponse;
    cpsProcedure_t*   pProcedure = scProcedureVars.pProcedureTemp;
    uint8_t           procDataLength = (uint8_t)pEvent->cValueLength - sizeof(cpsOpCode_t);

    /* Allocate buffer for biggest response */
    pResponse = MSG_Alloc(rspSize +
                      (uint32_t)FLib_GetMax((sizeof(ctsDateTime_t) - 1U),
                      pServiceConfig->pUserData->cNumOfSupportedSensorLocations));

    if (pResponse != NULL)
    {
        pResponse->opCode = gCps_RspCode_c;
        pResponse->procedureData.response.reqOpCode = pProcedure->opCode;

        switch (pProcedure->opCode)
        {
            case gCps_SetCummulativeValue_c:
            {
                if (procDataLength == sizeof(uint32_t))
                {
                    pResponse->procedureData.response.rspValue = gCps_Success_c;
                    pServiceConfig->pUserData->cumulativeWheelRevs = pProcedure->procedureData.cummulativeValue;
                }
                else
                {
                    pResponse->procedureData.response.rspValue = gCps_InvalidParameter_c;
                }
                break;
            }

            case gCps_UpdateSensorLocation_c:
            {
                if(!Cps_SuppportMultipleSensorLoc(pServiceConfig))
                {
                    pResponse->procedureData.response.rspValue = gCps_OpCodeNotSupported_c;
                }
                else if (!Cps_ValidateSensorLocation(pProcedure->procedureData.sensorLocation,
                                                     (const void *)pServiceConfig->pUserData->pSupportedSensorLocations,
                                                     pServiceConfig->pUserData->cNumOfSupportedSensorLocations))
                {
                    pResponse->procedureData.response.rspValue = gCps_InvalidParameter_c;
                }
                else
                {
                    (void)Cps_SetSensorLocation(pServiceConfig->serviceHandle, pProcedure->procedureData.sensorLocation);
                    pResponse->procedureData.response.rspValue = gCps_Success_c;
                }
                break;
            }

            case gCps_ReqSupportedSensorLoc_c:
            {
                pResponse->procedureData.response.rspValue = gCps_Success_c;

                /* Copy in supported locations */
                FLib_MemCpy(pResponse->procedureData.response.rspData.sensorLocation,
                            (const void*)pServiceConfig->pUserData->pSupportedSensorLocations,
                            (uint32_t)pServiceConfig->pUserData->cNumOfSupportedSensorLocations * sizeof(cpsSensorLocation_t));
                rspSize += pServiceConfig->pUserData->cNumOfSupportedSensorLocations;
                break;
            }

            case gCps_SetCrankLength_c:
            {
                if (procDataLength == sizeof(uint16_t))
                {
                    pResponse->procedureData.response.rspValue = gCps_Success_c;
                    pServiceConfig->pUserData->crankLength = pProcedure->procedureData.crankLength;
                }
                else
                {
                    pResponse->procedureData.response.rspValue = gCps_InvalidParameter_c;
                }
                break;
            }

            case gCps_RequestCrankLength_c:
            {
                pResponse->procedureData.response.rspValue = gCps_Success_c;
                pResponse->procedureData.response.rspData.crankLength = pServiceConfig->pUserData->crankLength;
                rspSize += sizeof(uint16_t);
                break;
            }

            case gCps_SetChainLength_c:
            {
                if (procDataLength == sizeof(uint16_t))
                {
                    pResponse->procedureData.response.rspValue = gCps_Success_c;
                    pServiceConfig->pUserData->chainLength = pProcedure->procedureData.chainLength;
                }
                else
                {
                    pResponse->procedureData.response.rspValue = gCps_InvalidParameter_c;
                }
                break;
            }

            case gCps_RequestChainLength_c:
            {
                pResponse->procedureData.response.rspValue = gCps_Success_c;
                pResponse->procedureData.response.rspData.chainLength = pServiceConfig->pUserData->chainLength;
                rspSize += sizeof(uint16_t);
                break;
            }

            case gCps_SetChainWeight_c:
            {
                if (procDataLength == sizeof(uint16_t))
                {
                    pResponse->procedureData.response.rspValue = gCps_Success_c;
                    pServiceConfig->pUserData->chainWeight = pProcedure->procedureData.chainWeight;
                }
                else
                {
                    pResponse->procedureData.response.rspValue = gCps_InvalidParameter_c;
                }
                break;
            }

            case gCps_RequestChainWeight_c:
            {
                pResponse->procedureData.response.rspValue = gCps_Success_c;
                pResponse->procedureData.response.rspData.chainWeight = pServiceConfig->pUserData->chainWeight;
                rspSize += sizeof(uint16_t);
                break;
            }

            case gCps_SetSpanLength_c:
            {
                if (procDataLength == sizeof(uint16_t))
                {
                    pResponse->procedureData.response.rspValue = gCps_Success_c;
                    pServiceConfig->pUserData->spanLength = pProcedure->procedureData.spanLength;
                }
                else
                {
                    pResponse->procedureData.response.rspValue = gCps_InvalidParameter_c;
                }
                break;
            }

            case gCps_RequestSpanLength_c:
            {
                pResponse->procedureData.response.rspValue = gCps_Success_c;
                pResponse->procedureData.response.rspData.spanLength = pServiceConfig->pUserData->spanLength;
                rspSize += sizeof(uint16_t);
                break;
            }

            case gCps_StartOffsetCompensation_c:
            {
                /* Offset Compensation is started */
                pServiceConfig->offsetCompensationOngoing = TRUE;
                earlyReturn = TRUE;
                break;
            }

            case gCps_MaskCpsContent_c:
            {
                if (procDataLength == sizeof(uint16_t) &&
                    (0U == (pProcedure->procedureData.contentMask & gCps_Reserved_c)))
                {
                    pResponse->procedureData.response.rspValue = gCps_Success_c;
                    pServiceConfig->maskContent = pProcedure->procedureData.contentMask;
                }
                else
                {
                    pResponse->procedureData.response.rspValue = gCps_InvalidParameter_c;
                }

                break;
            }

            case gCps_ReqSamplingRate_c:
            {
                pResponse->procedureData.response.rspValue = gCps_Success_c;
                pResponse->procedureData.response.rspData.samplingRate = gCps_SamplingRate_c;
                rspSize += sizeof(uint8_t);
                break;
            }

            case gCps_ReqFactoryCalibrationDate_c:
            {
                pResponse->procedureData.response.rspValue = gCps_Success_c;
                FLib_MemCpy(&pResponse->procedureData.response.rspData.factoryCalibrationDate,
                            (const void*)&mFactoryCalibrationDate,sizeof(ctsDateTime_t));
                rspSize += (sizeof(ctsDateTime_t) - 1U);
                break;
            }

            default:
            {
                pResponse->procedureData.response.rspValue = gCps_OpCodeNotSupported_c;
                break;
            }
        }

        if(earlyReturn == FALSE)
        {
            /* Write response in characteristic */
            (void)GattDb_WriteAttribute(pEvent->handle, rspSize, (uint8_t*) pResponse);

            /* Indicate value to client */
            (void)GattServer_SendIndication(mCps_ClientDeviceId, pEvent->handle);
        }

        /* Free buffer */
        (void)MSG_Free(pResponse);
    }
}

static bool_t Cps_ValidateSensorLocation(cpsSensorLocation_t sensorLoc, const cpsSensorLocation_t *aSensorLoc, uint8_t count)
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
