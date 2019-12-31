/*! *********************************************************************************
* \addtogroup Pulse Oximeter Service
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
#include "TimersManager.h"

#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"

#include "current_time_interface.h"
#include "pulse_oximeter_interface.h"
/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
#define gPlx_MaxCharValueLen    21
#define gPlx_FeaturesMaxLen     7

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
/*! Pulse Oximeter Service - Subscribed Client */
static deviceId_t mPlx_ClientDeviceId = gInvalidDeviceId_c;

static plxConfig_t      *mpServiceConfig;
static plxProcedure_t   mProcedure;
static uint16_t         mHandle;
static uint8_t          mMeasurementIndex;

static tmrTimerID_t     mReportTimerId;
/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
********************************************************************************** */
static bleResult_t Plx_UpdateContMeasCharacteristic
(
 uint16_t handle,
 plxContinuousMeasurement_t *pMeasurement
);

static bleResult_t Plx_UpdateSpotCheckCharacteristic
(
 uint16_t handle,
 plxSpotCheckMeasurement_t *pMeasurement
);

static void Plx_SendNotification
(
  uint16_t handle
);

static void Plx_SendIndication
(
  uint16_t handle
);

static void Plx_SendProcedureResponse
(
    plxConfig_t *pServiceConfig,
    gattServerAttributeWrittenEvent_t* pEvent
);

static bleResult_t Plx_StoreMeasurement
(
    plxUserData_t *pUserData,
    plxSpotCheckMeasurement_t *pMeasurement
);

static bleResult_t Plx_SetPulseOximeterFeature
(
    uint16_t            handle,
    plxFeatures_t       features
);

static plxRspCodeValue_t Plx_ValidateProcedure(plxProcedure_t* pProcedure, uint8_t procDataLength);
static void Plx_ExecuteProcedure(plxConfig_t *pServiceConfig, plxProcedure_t* pProcedure, uint16_t handle);
static void Plx_DeleteStoredRecords(plxConfig_t *pServiceConfig, uint16_t handle, bool_t responseNeeded);
static void ReportTimerCallback(void * pParam);
/***********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

bleResult_t Plx_Start (plxConfig_t *pServiceConfig)
{
    bleResult_t result;
    uint16_t  handle;
    bleUuid_t uuidPlxFeature = Uuid16(gBleSig_PulseOximeterFeature_d);
    uint8_t i;

    if(pServiceConfig->pUserData != NULL)
    {
        pServiceConfig->pUserData->lastSeqNumber = 0x00;

        for (i = 0; i < gPlx_MaxNumOfStoredMeasurements_c; i++)
        {
          pServiceConfig->pUserData->pStoredMeasurements[i].seqNumber = 0xFF;
        }
    }

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuidPlxFeature, &handle);

    if (result == gBleSuccess_c)
    {
        result = Plx_SetPulseOximeterFeature(handle, pServiceConfig->plxFeatures);
    }

    return result;
}

bleResult_t Plx_Stop (plxConfig_t *pServiceConfig)
{
    return Plx_Unsubscribe(pServiceConfig);
}

bleResult_t Plx_Subscribe(deviceId_t clientdeviceId)
{
    mPlx_ClientDeviceId = clientdeviceId;
    return gBleSuccess_c;
}

bleResult_t Plx_Unsubscribe(plxConfig_t *pServiceConfig)
{
    mPlx_ClientDeviceId = gInvalidDeviceId_c;

    pServiceConfig->procInProgress = gProcNotInProgress;
    return gBleSuccess_c;
}

bleResult_t Plx_RecordContinuousMeasurement (uint16_t serviceHandle, plxContinuousMeasurement_t *pMeasurement)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_PlxContMeasurement_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value and send indication */
        if (gBleSuccess_c == Plx_UpdateContMeasCharacteristic(handle, pMeasurement))
        {
            Plx_SendNotification(handle);
        }
    }

    return result;
}

bleResult_t Plx_RecordSpotCheckMeasurement (plxConfig_t *pServiceConfig, plxSpotCheckMeasurement_t *pMeasurement)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_PlxSCMeasurement_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result == gBleSuccess_c)
    {
        /* Store measurements */
        (void)Plx_StoreMeasurement(pServiceConfig->pUserData, pMeasurement);

        /* Update characteristic value and send indication */
        if (gBleSuccess_c == Plx_UpdateSpotCheckCharacteristic(handle, pMeasurement))
        {
            Plx_SendIndication(handle);
        }
    }

    return result;
}

void Plx_ControlPointHandler (plxConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    union
    {
        uint8_t*         aValueTemp;
        plxProcedure_t*  plxProcedureTemp;
    }plxProcedureVars;

    bool_t  fIndicationActive = FALSE;
    uint16_t  handleCccd;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(pEvent->handle, &handleCccd) == gBleSuccess_c)
    {
        /* Check if indications are properly configured */
        (void)Gap_CheckIndicationStatus(mPlx_ClientDeviceId, handleCccd, &fIndicationActive);

        if(!fIndicationActive)
        {
            (void)GattServer_SendAttributeWrittenStatus(mPlx_ClientDeviceId, pEvent->handle,
                                                     gAttErrCodePlxCccdImproperlyConfigured_c);
        }
        else
        {
            /* Check if another procedure is in progress and we do not abort */
            plxProcedureVars.aValueTemp = pEvent->aValue;

            if ((0U != pServiceConfig->procInProgress) &&
                ((plxProcedureVars.plxProcedureTemp)->opCode != gPlx_AbortOperation_c))
            {
                (void)GattServer_SendAttributeWrittenStatus(mPlx_ClientDeviceId, pEvent->handle,
                                                         gAttErrCodePlxProcedureAlreadyInProgress_c);
            }
            else
            {
                pServiceConfig->procInProgress = gProcInProgress;

                /* Procedure received successfully */
                (void)GattServer_SendAttributeWrittenStatus(mPlx_ClientDeviceId, pEvent->handle,
                                                                 (uint8_t)gAttErrCodeNoError_c);

                Plx_SendProcedureResponse(pServiceConfig, pEvent);
            }
        }
    }
}

/***********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
static bleResult_t Plx_UpdateContMeasCharacteristic
(
 uint16_t handle,
 plxContinuousMeasurement_t *pMeasurement
)
{
    uint8_t charValue[gPlx_MaxCharValueLen];
    uint8_t index = 0;

    /* Add flags */
    charValue[index] = pMeasurement->flags;
    index += 1U;

    /* Add measurements */
    FLib_MemCpy(&charValue[index], &pMeasurement->SpO2PRNormal, sizeof(SpO2PRMeasurement_t));
    index += sizeof(SpO2PRMeasurement_t);

    /* Add fast measurements */
    if (0U != (pMeasurement->flags & gPlx_SpO2PRFastPresent_c))
    {
        FLib_MemCpy(&charValue[index], &pMeasurement->SpO2PRFast, sizeof(SpO2PRMeasurement_t));
        index += sizeof(SpO2PRMeasurement_t);
    }

    /* Add slow measurements */
    if (0U != (pMeasurement->flags & gPlx_SpO2PRSlowPresent_c))
    {
        FLib_MemCpy(&charValue[index], &pMeasurement->SpO2PRSlow, sizeof(SpO2PRMeasurement_t));
        index += sizeof(SpO2PRMeasurement_t);
    }

    /* Add measurement status */
    if (0U != (pMeasurement->flags & gPlx_MeasurementStatusPresent_c))
    {
        FLib_MemCpy(&charValue[index], &pMeasurement->measurementStatus, sizeof(plxMeasStatus_t));
        index += sizeof(plxMeasStatus_t);
    }

    /* Add device status */
    if (0U != (pMeasurement->flags & gPlx_DeviceAndSensorStatusPresent_c))
    {
        FLib_MemCpy(&charValue[index], &pMeasurement->deviceAndSensorStatus, sizeof(plxDeviceAndSensorStatus_t));
        index += sizeof(plxDeviceAndSensorStatus_t);
    }

    /* Add pulse amplitude index */
    if (0U != (pMeasurement->flags & gPlx_PulseAmplitudeIndexPresent_c))
    {
        FLib_MemCpy(&charValue[index], &pMeasurement->pulseAmplitudeIndex, sizeof(uint16_t));
        index += sizeof(uint16_t);
    }

    return GattDb_WriteAttribute(handle, index, &charValue[0]);
}

static bleResult_t Plx_UpdateSpotCheckCharacteristic
(
 uint16_t handle,
 plxSpotCheckMeasurement_t *pMeasurement
)
{
    uint8_t charValue[gPlx_MaxCharValueLen];
    uint8_t index = 0;

    /* Add flags */
    charValue[index] = pMeasurement->flags;
    index += 1U;

    /* Add measurements */
    FLib_MemCpy(&charValue[index], &pMeasurement->SpO2PRNormal, sizeof(SpO2PRMeasurement_t));
    index += sizeof(SpO2PRMeasurement_t);

    /* Add timestamp */
    if (0U != (pMeasurement->flags & gPlx_SCTimestampPresent_c))
    {
        FLib_MemCpy(&charValue[index], &pMeasurement->timeStamp, (sizeof(ctsDateTime_t)) );
        index += sizeof(ctsDateTime_t);
    }

    /* Add measurement status */
    if (0U != (pMeasurement->flags & gPlx_SCMeasurementStatusPresent_c))
    {
        FLib_MemCpy(&charValue[index], &pMeasurement->measurementStatus, sizeof(plxMeasStatus_t));
        index += sizeof(plxMeasStatus_t);
    }

    /* Add device status */
    if (0U != (pMeasurement->flags & gPlx_SCDeviceAndSensorStatusPresent_c))
    {
        FLib_MemCpy(&charValue[index], &pMeasurement->deviceAndSensorStatus, sizeof(plxDeviceAndSensorStatus_t));
        index += sizeof(plxDeviceAndSensorStatus_t);
    }

    /* Add pulse amplitude index */
    if (0U != (pMeasurement->flags & gPlx_SCPulseAmplitudeIndexPresent_c))
    {
        FLib_MemCpy(&charValue[index], &pMeasurement->pulseAmplitudeIndex, sizeof(uint16_t));
        index += sizeof(uint16_t);
    }

    bleResult_t result = GattDb_WriteAttribute(handle, index, &charValue[0]);
    return result;
}

static void Plx_SendNotification
(
  uint16_t handle
)
{
    uint16_t  hCccd;
    bool_t isNotifActive;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) == gBleSuccess_c)
    {
        if (mPlx_ClientDeviceId != gInvalidDeviceId_c)
        {
            if (gBleSuccess_c == Gap_CheckNotificationStatus
                (mPlx_ClientDeviceId, hCccd, &isNotifActive) &&
                TRUE == isNotifActive)
            {
                (void)GattServer_SendNotification(mPlx_ClientDeviceId, handle);
            }
        }
    }
}

static void Plx_SendIndication
(
  uint16_t handle
)
{
    uint16_t  hCccd;
    bool_t isIndicationActive;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) == gBleSuccess_c)
    {
        if (mPlx_ClientDeviceId != gInvalidDeviceId_c)
        {
            if (gBleSuccess_c == Gap_CheckIndicationStatus
                (mPlx_ClientDeviceId, hCccd, &isIndicationActive) &&
                TRUE == isIndicationActive)
            {
                (void)GattServer_SendIndication(mPlx_ClientDeviceId, handle);
            }
        }
    }
}

static bleResult_t Plx_SetPulseOximeterFeature
(
    uint16_t            handle,
    plxFeatures_t       features
)
{
    uint8_t charValue[gPlx_FeaturesMaxLen];
    uint8_t index = 0;

    FLib_MemCpy(&charValue[index], &features.supportedFeatures, sizeof(plxSupportedFeatures_t));
    index += sizeof(plxSupportedFeatures_t);

    if(0U != (features.supportedFeatures & gPlx_MeasurementStatusSupported_c))
    {
          FLib_MemCpy(&charValue[index], &features.measStatusSupport, sizeof(plxMeasStatusSupport_t));
          index += sizeof(plxSupportedFeatures_t);
    }

    if(0U != (features.supportedFeatures & gPlx_DeviceAndSensorStatusSupported_c))
    {
          FLib_MemCpy(&charValue[index], (uint8_t*)(&features.devAndSensStatusSupport.flags), sizeof(uint16_t));
          index += sizeof(uint16_t);
          FLib_MemCpy(&charValue[index], &features.devAndSensStatusSupport.rfu, sizeof(uint8_t));
          index += sizeof(uint8_t);
    }

    bleResult_t result = GattDb_WriteAttribute(handle, index, charValue);
    return result;
}

static void Plx_SendStoredMeasurement(plxConfig_t *pServiceConfig, uint8_t index)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_PlxSCMeasurement_d);
    plxSpotCheckMeasurement_t *pMeasurement = pServiceConfig->pUserData->pStoredMeasurements + index;

    /* SpO2PR Measurement */
    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
                gBleUuidType16_c, (bleUuid_t*)&uuid, &handle);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value and send notification */
        if (gBleSuccess_c == Plx_UpdateSpotCheckCharacteristic(handle, pMeasurement))
        {
            Plx_SendIndication(handle);
        }
    }
}

static void Plx_SendProcedureResponse
(
     plxConfig_t *pServiceConfig,
     gattServerAttributeWrittenEvent_t* pEvent
)
{
    union
    {
        uint8_t*            aValueTemp;
        plxProcedure_t*     plxProcedurTemp;
    }plxProcedurVars;
    plxProcedurVars.aValueTemp = pEvent->aValue;
    plxProcedure_t      response;
    plxProcedure_t*     pProcedure = plxProcedurVars.plxProcedurTemp;
    uint8_t             procDataLength = (uint8_t)(pEvent->cValueLength - sizeof(plxOpCode_t) - sizeof(plxOperator_t));

    response.opCode = gPlx_RspCode_c;
    response.plxOperator = gPlx_Null_c;
    response.operand.responseCode.reqOpCode = pProcedure->opCode;
    response.operand.responseCode.rspCodeValue = Plx_ValidateProcedure(pProcedure, procDataLength);

    if (gPlx_RspSuccess_c == response.operand.responseCode.rspCodeValue)
    {
        Plx_ExecuteProcedure(pServiceConfig, pProcedure, pEvent->handle);
    }
    else
    {
        /* Write response in characteristic */
        (void)GattDb_WriteAttribute(pEvent->handle, sizeof(plxProcedure_t), (uint8_t*) &response);

        /* Indicate value to client */
        (void)GattServer_SendIndication(mPlx_ClientDeviceId, pEvent->handle);
    }
}

static void Plx_ReportStoredRecords(plxConfig_t *pServiceConfig, plxProcedure_t* pProcedure, uint16_t handle)
{
    plxSpotCheckMeasurement_t *pMeasurement;
    plxProcedure_t      response;
    uint8_t             index;
    bool_t isIndicationActive;
    uint16_t  hCccd;
    bool_t earlyReturn = FALSE;

    if (0U != pServiceConfig->procInProgress)
    {
        while ( mMeasurementIndex < gPlx_MaxNumOfStoredMeasurements_c)
        {
            index = (pServiceConfig->pUserData->measurementCursor + mMeasurementIndex) % gPlx_MaxNumOfStoredMeasurements_c;
            pMeasurement = pServiceConfig->pUserData->pStoredMeasurements + index;

            if (pMeasurement->seqNumber == 0xFFU)
            {
                mMeasurementIndex++;
                continue;
            }

            Plx_SendStoredMeasurement(pServiceConfig, index);
            mMeasurementIndex++;
            pServiceConfig->pUserData->cReportedRecords++;
                /* Start advertising timer */
            (void)TMR_StartLowPowerTimer(mReportTimerId, gTmrLowPowerSecondTimer_c,
                       500, ReportTimerCallback, NULL);
            earlyReturn = TRUE;
            break;
        }

        if (!earlyReturn)
        {
            /* Get handle of CCCD */
            if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) == gBleSuccess_c)
            {
                response.opCode = gPlx_RspCode_c;
                response.plxOperator = gPlx_Null_c;
                response.operand.responseCode.reqOpCode = gPlx_ReportStoredRecords_c;
                response.operand.responseCode.rspCodeValue =
                  (pServiceConfig->pUserData->cMeasurements == 0U) ?
                    gPlx_RspNoRecordsFound_c : gPlx_RspSuccess_c;

                /* Indicate value to client */
                if (gBleSuccess_c == Gap_CheckIndicationStatus
                    (mPlx_ClientDeviceId, hCccd, &isIndicationActive) &&
                    TRUE == isIndicationActive)
                {
                    (void)GattServer_SendInstantValueIndication(mPlx_ClientDeviceId, handle,
                                sizeof(plxProcedure_t), (uint8_t*) &response);
                }

                Plx_DeleteStoredRecords(pServiceConfig, handle, FALSE);
            }
        }
    }
}

static void Plx_DeleteStoredRecords(plxConfig_t *pServiceConfig, uint16_t handle, bool_t responseNeeded)
{
    plxProcedure_t      response;
    bool_t isIndicationActive;
    uint16_t  hCccd;
    uint8_t i;

    if(responseNeeded)
    {
        response.opCode = gPlx_RspCode_c;
        response.plxOperator = gPlx_Null_c;
        response.operand.responseCode.reqOpCode = gPlx_DeleteStoredRecords_c;
        response.operand.responseCode.rspCodeValue = gPlx_RspSuccess_c;
    }

    if(pServiceConfig->pUserData != NULL)
    {
        pServiceConfig->pUserData->lastSeqNumber = 0x00;

        for (i = 0; i < gPlx_MaxNumOfStoredMeasurements_c; i++)
        {
          pServiceConfig->pUserData->pStoredMeasurements[i].seqNumber = 0xFF;
        }

        /* Update cursor and count */
        pServiceConfig->pUserData->measurementCursor = 0x00;
        pServiceConfig->pUserData->cMeasurements = 0x00;
    }

    if(responseNeeded)
    {
        /* Get handle of CCCD */
        if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) == gBleSuccess_c)
        {
            /* Indicate value to client */
            if (gBleSuccess_c == Gap_CheckIndicationStatus
                (mPlx_ClientDeviceId, hCccd, &isIndicationActive) &&
                TRUE == isIndicationActive)
            {
                (void)GattServer_SendInstantValueIndication(mPlx_ClientDeviceId, handle,
                            sizeof(plxProcedure_t), (uint8_t*) &response);
            }
        }
    }
}

static void Plx_ReportNumOfStoredRecords(plxConfig_t *pServiceConfig, plxProcedure_t* pProcedure, uint16_t handle)
{
    plxProcedure_t      response;

    response.opCode = gPlx_NumOfStoredRecordsRsp_c;
    response.plxOperator = gPlx_Null_c;

    response.operand.numberOfRecords = pServiceConfig->pUserData->cMeasurements;

    /* Write response in characteristic */
    (void)GattDb_WriteAttribute(handle, sizeof(plxProcedure_t), (uint8_t*) &response);

    /* Indicate value to client */
    (void)GattServer_SendIndication(mPlx_ClientDeviceId, handle);
}

static void Plx_AbortOperation(plxConfig_t *pServiceConfig, uint16_t handle)
{
    plxProcedure_t      response;

    response.opCode = gPlx_RspCode_c;
    response.plxOperator = gPlx_Null_c;
    response.operand.responseCode.reqOpCode = gPlx_AbortOperation_c;
    response.operand.responseCode.rspCodeValue = gPlx_RspSuccess_c;

    pServiceConfig->procInProgress = gProcNotInProgress;

    /* Write response in characteristic */
    (void)GattDb_WriteAttribute(handle, sizeof(plxProcedure_t), (uint8_t*) &response);

    /* Indicate value to client */
    (void)GattServer_SendIndication(mPlx_ClientDeviceId, handle);
}

static void Plx_ExecuteProcedure(plxConfig_t *pServiceConfig, plxProcedure_t* pProcedure, uint16_t handle)
{
    switch (pProcedure->opCode)
    {
        case gPlx_ReportStoredRecords_c:
        {
            mpServiceConfig = pServiceConfig;
            FLib_MemCpy(&mProcedure, pProcedure, sizeof(plxProcedure_t));
            mHandle = handle;
            mMeasurementIndex = 0;
            pServiceConfig->pUserData->cReportedRecords = 0;
            pServiceConfig->procInProgress = gProcSendingRecords;
            Plx_ReportStoredRecords(pServiceConfig, pProcedure, handle);
            break;
        }

        case gPlx_ReportNumOfStoredRecords_c:
        {
            Plx_ReportNumOfStoredRecords(pServiceConfig, pProcedure, handle);
            break;
        }
        case gPlx_AbortOperation_c:
        {
            Plx_AbortOperation(pServiceConfig, handle);
            break;
        }

        case gPlx_DeleteStoredRecords_c:
        {
            mMeasurementIndex = 0;
            Plx_DeleteStoredRecords(pServiceConfig, handle, TRUE);
            break;
        }

        default:
        {
            ; /* For MISRA compliance */
            break;
        }
    }
}

static plxRspCodeValue_t Plx_ValidateProcedure(plxProcedure_t* pProcedure, uint8_t procDataLength)
{
    uint8_t retVal = gPlx_RspSuccess_c;

    if( pProcedure->plxOperator > gPlx_AllRecords_c )
    {
        retVal = gPlx_RspOperatorNotSupported_c;
    }
    else
    {
        switch (pProcedure->opCode)
        {
            case gPlx_ReportStoredRecords_c:
            case gPlx_ReportNumOfStoredRecords_c:
            case gPlx_DeleteStoredRecords_c:
            {
                switch(pProcedure->plxOperator)
                {
                    case gPlx_AllRecords_c:
                    {
                        if (procDataLength != 0U)
                        {
                            retVal = gPlx_RspOperandNotSupported_c;
                        }
                        break;
                    }
                    default:
                    {
                        retVal = gPlx_RspInvalidOperator_c;
                    }
                    break;
                }
                break;
            }
            case gPlx_AbortOperation_c:
            {
                if( pProcedure->plxOperator != gPlx_Null_c )
                {
                    retVal = gPlx_RspInvalidOperator_c;
                }
                else if ( procDataLength != 0U )
                {
                    retVal = gPlx_RspOperandNotSupported_c;
                }
                else
                {
                    ; /* For MISRA compliance */
                }
                break;
            }
            default:
            {
                retVal = gPlx_RspOpCodeNotSupported_c;
                break;
            }
        }
    }

    return retVal;
}

static bleResult_t Plx_StoreMeasurement(plxUserData_t *pUserData, plxSpotCheckMeasurement_t *pMeasurement)
{
    bleResult_t result = gBleSuccess_c;

    if (pUserData->pStoredMeasurements == NULL)
    {
        result = gBleOutOfMemory_c;
    }
    else
    {
        /* Store measurement */
        FLib_MemCpy(&pUserData->pStoredMeasurements[pUserData->measurementCursor],
                    pMeasurement,
                    sizeof(plxSpotCheckMeasurement_t));

        /* Update cursor and count */
        pUserData->measurementCursor = (pUserData->measurementCursor + 1U) % gPlx_MaxNumOfStoredMeasurements_c;
        pUserData->cMeasurements = MIN(pUserData->cMeasurements + 1U, gPlx_MaxNumOfStoredMeasurements_c);
    }

    return result;
}

static void ReportTimerCallback(void * pParam)
{
    Plx_ReportStoredRecords(mpServiceConfig, &mProcedure, mHandle);
}

/*! *********************************************************************************
* @}
********************************************************************************** */
