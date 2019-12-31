/*! *********************************************************************************
* \addtogroup Glucose Service
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
#include "MemManager.h"
#include "TimersManager.h"

#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"

#include "current_time_interface.h"
#include "glucose_interface.h"
/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
#define gGls_MeasurementMaxLen 18
#define gGls_MeasurementContextMaxLen 17

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
/*! Glucose Service - Subscribed Client */
static deviceId_t mGls_ClientDeviceId = gInvalidDeviceId_c;

static glsConfig_t      *mpServiceConfig;
static glsProcedure_t   mProcedure;
static uint16_t         mHandle;
static uint8_t          mMeasurementIndex;

static tmrTimerID_t     mReportTimerId;
/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
********************************************************************************** */
static bleResult_t Gls_UpdateGlucoseMeasCharacteristic
(
 uint16_t handle,
 glsFullMeasurement_t *pMeasurement
);

static bleResult_t Gls_UpdateGlucoseMeasContextCharacteristic
(
 uint16_t handle,
 glsFullMeasurement_t *pMeasurement
);

static void Gls_SendNotification(uint16_t handle);


static bleResult_t Gls_SetGlucoseFeature
(
    uint16_t            handle,
    glsFeatureFlags_t   feature
);

static void Gls_SendProcedureResponse
(
     glsConfig_t *pServiceConfig,
     gattServerAttributeWrittenEvent_t* pEvent
);

static bleResult_t Gls_StoreGlucoseMeasurement
(
     glsUserData_t *pUserData,
     glsFullMeasurement_t *pMeasurement
);

static glsRspCodeValue_t Gls_ValidateProcedure(glsProcedure_t* pProcedure, uint8_t procDataLength);
static void Gls_ExecuteProcedure(glsConfig_t *pServiceConfig, glsProcedure_t* pProcedure, uint16_t handle);
static bool_t Gls_MatchMeasurement(glsFullMeasurement_t *pMeasurement, glsProcedure_t* pProcedure);
static void ReportTimerCallback(void * pParam);
/***********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

bleResult_t Gls_Start (glsConfig_t *pServiceConfig)
{
    uint8_t i;

    if(NULL != pServiceConfig->pUserData)
    {
        pServiceConfig->pUserData->lastSeqNumber = 0x0000;

        for (i = 0; i < gGls_MaxNumOfStoredMeasurements_c; i++)
        {
          pServiceConfig->pUserData->pStoredMeasurements[i].seqNumber = 0xFFFF;
        }
    }

    mReportTimerId = TMR_AllocateTimer();

    return Gls_SetGlucoseFeature(pServiceConfig->serviceHandle, pServiceConfig->glsfeatureFlags);
}

bleResult_t Gls_Stop (glsConfig_t *pServiceConfig)
{
    return Gls_Unsubscribe(pServiceConfig);
}

bleResult_t Gls_Subscribe(deviceId_t clientdeviceId)
{
    mGls_ClientDeviceId = clientdeviceId;
    return gBleSuccess_c;
}

bleResult_t Gls_Unsubscribe(glsConfig_t *pServiceConfig)
{
    mGls_ClientDeviceId = gInvalidDeviceId_c;

    pServiceConfig->procInProgress = FALSE;
    return gBleSuccess_c;
}

bleResult_t Gls_RecordGlucoseMeasurement (glsConfig_t *pServiceConfig, glsFullMeasurement_t *pMeasurement)
{
    union
    {
        uint16_t*    uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;

    uint16_t  handle;
    bleResult_t result;
    uint16_t uuid = gBleSig_GlucoseMeasurement_d;

    /* Glucose Measurement */
    /* Get handle of characteristic */
    uuidVars.uuidTemp = &uuid;
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
                gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

    if (result == gBleSuccess_c)
    {
        /* Store measurements */
        (void)Gls_StoreGlucoseMeasurement(pServiceConfig->pUserData, pMeasurement);

        /* Update characteristic value and send notification */
        if (gBleSuccess_c == Gls_UpdateGlucoseMeasCharacteristic(handle, pMeasurement))
        {
            Gls_SendNotification(handle);
        }

        if(0U != (pMeasurement->flags & gGls_ContextInfoFollows_c))
        {
            uuid = gBleSig_GlucoseMeasurementContext_d;

            /* Get handle of characteristic */
            result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
                        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

            if (result == gBleSuccess_c)
            {
                /* Update characteristic value and send notification */
                if (gBleSuccess_c == Gls_UpdateGlucoseMeasContextCharacteristic(handle, pMeasurement))
                {
                    Gls_SendNotification(handle);
                }
            }
        }
    }

    return result;
}

void Gls_ControlPointHandler (glsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    union
    {
        uint8_t*            aValueTemp;
        glsProcedure_t*     glsProcedureTemp;
    }glsProcedureVars;

    bool_t  fIndicationActive = FALSE;
    uint16_t  handleCccd;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(pEvent->handle, &handleCccd) == gBleSuccess_c)
    {
        /* Check if indications are properly configured */
        (void)Gap_CheckIndicationStatus(mGls_ClientDeviceId, handleCccd, &fIndicationActive);

        if(!fIndicationActive)
        {
            (void)GattServer_SendAttributeWrittenStatus(mGls_ClientDeviceId, pEvent->handle,
                                                     gAttErrCodeGlsCccdImproperlyConfigured_c);
        }
        else
        {
            /* Check if another procedure is in progress and we do not abort */
            glsProcedureVars.aValueTemp = pEvent->aValue;

            if (pServiceConfig->procInProgress &&
                (glsProcedureVars.glsProcedureTemp)->opCode != gGls_AbortOperation_c)
            {
                (void)GattServer_SendAttributeWrittenStatus(mGls_ClientDeviceId, pEvent->handle,
                                                         gAttErrCodeGlsProcedureAlreadyInProgress_c);
            }
            else
            {
                pServiceConfig->procInProgress = TRUE;

                /* Procedure received successfully */
                (void)GattServer_SendAttributeWrittenStatus(mGls_ClientDeviceId, pEvent->handle,
                                                                 (uint8_t)gAttErrCodeNoError_c);

                Gls_SendProcedureResponse(pServiceConfig, pEvent);
            }
        }
    }
}

/***********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
static bleResult_t Gls_UpdateGlucoseMeasCharacteristic
(
 uint16_t handle,
 glsFullMeasurement_t *pMeasurement
)
{
    uint8_t charValue[gGls_MeasurementMaxLen];
    uint8_t index = 0;

    /* Add flags */
    charValue[0] = pMeasurement->flags;

    /* Seq Number */
    FLib_MemCopy16Unaligned(&charValue[1], pMeasurement->seqNumber);

    FLib_MemCpy(&charValue[3], &pMeasurement->dateTime, sizeof(ctsDateTime_t));

    index = 3U + sizeof(ctsDateTime_t);

    if (0U != (pMeasurement->flags & gGls_TimeOffsetPresent_c))
    {
        FLib_MemCopy16Unaligned(&charValue[index], (uint16_t)pMeasurement->timeOffset);
        index += sizeof(int16_t);
    }

    FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->glucCon);
    index += sizeof(ieee11073_16BitFloat_t);

    if (0U != (pMeasurement->flags & gGls_ConcTypeSampleLocPresent_c))
    {
        charValue[index] = pMeasurement->typeAndSampleLoc.type +
                        (pMeasurement->typeAndSampleLoc.sampleLoc << 4);
        index += 1U;
    }

    if (0U != (pMeasurement->flags & gGls_SensorStatusPresent_c))
    {
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->sensorStatusAnnce);
        index += sizeof(int16_t);
    }

    return GattDb_WriteAttribute(handle, index, &charValue[0]);
}

static bleResult_t Gls_UpdateGlucoseMeasContextCharacteristic
(
 uint16_t handle,
 glsFullMeasurement_t *pMeasurement
)
{
    uint8_t charValue[gGls_MeasurementContextMaxLen];
    uint8_t index = 0;

    /* Add flags */
    charValue[0] = pMeasurement->contextFlags;

    /* Seq Number */
    FLib_MemCopy16Unaligned(&charValue[1], pMeasurement->seqNumber);

    index = 3U;

    if (0U != (pMeasurement->contextFlags & gGls_ExtendedFlagsPresent_c))
    {
        charValue[index] = pMeasurement->extendedFlags;
        index += 1U;
    }

    if (0U != (pMeasurement->contextFlags & gGls_CarbPresent_c))
    {
        charValue[index] = pMeasurement->carbId;
        index += 1U;
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->carbUnits);
        index += sizeof(ieee11073_16BitFloat_t);
    }

    if (0U != (pMeasurement->contextFlags & gGls_MealPresent_c))
    {
        charValue[index] = pMeasurement->meal;
        index += 1U;
    }

    if (0U != (pMeasurement->contextFlags & gGls_TesterHealthPresent_c))
    {
        charValue[index] = pMeasurement->testerHealth.tester +
                        (pMeasurement->testerHealth.health << 4);
        index += 1U;
    }

    if (0U != (pMeasurement->contextFlags & gGls_ExerciseDurationIntesityPresent_c))
    {
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->exerciseDuration);
        index += sizeof(int16_t);
        charValue[index] = pMeasurement->exerciseIntensity;
        index += 1U;
    }

    if (0U != (pMeasurement->contextFlags & gGls_MedicationPresent_c))
    {
        charValue[index] = pMeasurement->medicationId;
        index += 1U;
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->medicationUnits);
        index += sizeof(ieee11073_16BitFloat_t);
    }

    if (0U != (pMeasurement->contextFlags & gGls_HbA1cPresent_c))
    {
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->hbA1c);
        index += sizeof(ieee11073_16BitFloat_t);
    }

    return GattDb_WriteAttribute(handle, index, &charValue[0]);
}

static void Gls_SendNotification
(
  uint16_t handle
)
{
    uint16_t  hCccd;
    bool_t isNotificationActive;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) == gBleSuccess_c)
    {
        if (mGls_ClientDeviceId != gInvalidDeviceId_c)
        {
            if (gBleSuccess_c == Gap_CheckNotificationStatus
                (mGls_ClientDeviceId, hCccd, &isNotificationActive) &&
                TRUE == isNotificationActive)
            {
                (void)GattServer_SendNotification(mGls_ClientDeviceId, handle);
            }
        }
    }
}

static bleResult_t Gls_SetGlucoseFeature
(
    uint16_t            handle,
    glsFeatureFlags_t   feature
)
{
    union
    {
        uint16_t*   uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;

    uint16_t  hValueGlFeature;
    uint16_t uuid = gBleSig_GlucoseFeature_d;
    bleResult_t result;

      /* Get handle of characteristic */
    uuidVars.uuidTemp = &uuid;
    result = GattDb_FindCharValueHandleInService(handle,
                gBleUuidType16_c, uuidVars.bleUuidTemp, &hValueGlFeature);

    if (result == gBleSuccess_c)
    {
        result = GattDb_WriteAttribute(hValueGlFeature, sizeof(glsFeatureFlags_t), (uint8_t*)&feature);
    }

    return result;
}

static void Gls_SendStoredGlucoseMeasurement(glsConfig_t *pServiceConfig, uint8_t index)
{
    union
    {
        uint16_t*    uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;

    uint16_t  handle;
    bleResult_t result;
    uint16_t uuid = gBleSig_GlucoseMeasurement_d;
    glsFullMeasurement_t *pMeasurement = pServiceConfig->pUserData->pStoredMeasurements + index;

    /* Glucose Measurement */
    /* Get handle of characteristic */
    uuidVars.uuidTemp = &uuid;
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
                gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value and send notification */
        if (gBleSuccess_c == Gls_UpdateGlucoseMeasCharacteristic(handle, pMeasurement))
        {
            Gls_SendNotification(handle);
        }

        if(0U != (pMeasurement->flags & gGls_ContextInfoFollows_c))
        {
            uuid = gBleSig_GlucoseMeasurementContext_d;

            /* Get handle of characteristic */
            result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
                        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

            if (result == gBleSuccess_c)
            {
                /* Update characteristic value and send notification */
                if (gBleSuccess_c == Gls_UpdateGlucoseMeasContextCharacteristic(handle, pMeasurement))
                {
                    Gls_SendNotification(handle);
                }
            }
        }
    }
}

static void Gls_SendProcedureResponse
(
     glsConfig_t *pServiceConfig,
     gattServerAttributeWrittenEvent_t* pEvent
)
{
    union
    {
        uint8_t*        aValueTemp;
        glsProcedure_t* pProcedureTemp;
    }procedureVars;
    procedureVars.aValueTemp = pEvent->aValue;

    glsProcedure_t      response;
    glsProcedure_t*     pProcedure = procedureVars.pProcedureTemp;
    uint8_t             procDataLength = (uint8_t)(pEvent->cValueLength - sizeof(glsOpCode_t) - sizeof(glsOperator_t));

    response.opCode = gGls_RspCode_c;
    response.glsOperator = gGls_Null_c;
    response.operand.responseCode.reqOpCode = pProcedure->opCode;
    response.operand.responseCode.rspCodeValue = Gls_ValidateProcedure(pProcedure, procDataLength);

    if (gGls_RspSuccess_c == response.operand.responseCode.rspCodeValue)
    {
        Gls_ExecuteProcedure(pServiceConfig, pProcedure, pEvent->handle);
    }
    else
    {
        /* Write response in characteristic */
        (void)GattDb_WriteAttribute(pEvent->handle, sizeof(glsProcedure_t) - 2U, (uint8_t*) &response);

        /* Indicate value to client */
        (void)GattServer_SendIndication(mGls_ClientDeviceId, pEvent->handle);
    }
}

static bool_t Gls_CheckFilterCondition(glsProcedure_t *pProcedure, glsFullMeasurement_t *pMeasurement)
{
    bool_t result = FALSE;

    if (pProcedure->glsOperator == gGls_AllRecords_c)
    {
        result = TRUE;
    }
    else
    {
        if ((pProcedure->glsOperator == gGls_GreaterThanOrEqualTo_c)
            && (pProcedure->operand.filter.filter == gGls_FtvSeqNumber_c))
        {
            if (pMeasurement->seqNumber >= pProcedure->operand.filter.filterParameter)
            {
                result = TRUE;
            }
        }
    }

    return result;
}

static void Gls_ReportStoredRecords(glsConfig_t *pServiceConfig, glsProcedure_t* pProcedure, uint16_t handle)
{
    glsFullMeasurement_t *pMeasurement;
    glsProcedure_t      response;
    uint8_t             index;
    bool_t isIndicationActive;
    uint16_t  hCccd;
    bool_t earlyReturn = FALSE;

    if (pServiceConfig->procInProgress)
    {
        while ( mMeasurementIndex < gGls_MaxNumOfStoredMeasurements_c)
        {
            index = (pServiceConfig->pUserData->measurementCursor + mMeasurementIndex) % gGls_MaxNumOfStoredMeasurements_c;
            pMeasurement = pServiceConfig->pUserData->pStoredMeasurements + index;

            if (pMeasurement->seqNumber == 0xFFFFU)
            {
                mMeasurementIndex++;
                continue;
            }

            if (Gls_CheckFilterCondition(pProcedure, pMeasurement))
            {
                Gls_SendStoredGlucoseMeasurement(pServiceConfig, index);
                mMeasurementIndex++;
                    /* Start advertising timer */
                (void)TMR_StartLowPowerTimer(mReportTimerId, gTmrLowPowerSecondTimer_c,
                           500, ReportTimerCallback, NULL);
                earlyReturn = TRUE;
                break;
            }

            mMeasurementIndex++;
        }

        if (!earlyReturn)
        {
            /* Get handle of CCCD */
            if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) == gBleSuccess_c)
            {
                response.opCode = gGls_RspCode_c;
                response.glsOperator = gGls_Null_c;
                response.operand.responseCode.reqOpCode = gGls_ReportStoredRecords_c;
                response.operand.responseCode.rspCodeValue =
                  (pServiceConfig->pUserData->cMeasurements == 0U) ?
                    gGls_RspNoRecordsFound_c : gGls_RspSuccess_c;

                /* Indicate value to client */
                if (gBleSuccess_c == Gap_CheckIndicationStatus
                    (mGls_ClientDeviceId, hCccd, &isIndicationActive) &&
                    TRUE == isIndicationActive)
                {
                    (void)GattServer_SendInstantValueIndication(mGls_ClientDeviceId, handle,
                                sizeof(glsProcedure_t) - 2U, (uint8_t*) &response);
                }
            }
        }
    }
}


static void Gls_ReportNumOfStoredRecords(glsConfig_t *pServiceConfig, glsProcedure_t* pProcedure, uint16_t handle)
{
    glsProcedure_t      response;
    uint8_t             i;

    response.opCode = gGls_NumOfStoredRecordsRsp_c;
    response.glsOperator = gGls_Null_c;

    if (pProcedure->glsOperator == gGls_AllRecords_c)
    {
        response.operand.numberOfRecords = pServiceConfig->pUserData->cMeasurements;
    }
    else
    {
        response.operand.numberOfRecords = 0;

        for (i = 0; i < gGls_MaxNumOfStoredMeasurements_c; i++)
        {
            if (Gls_MatchMeasurement(&pServiceConfig->pUserData->pStoredMeasurements[i], pProcedure))
            {
                response.operand.numberOfRecords++;
            }
        }
    }

    /* Write response in characteristic */
    (void)GattDb_WriteAttribute(handle, sizeof(glsProcedure_t) - 2U, (uint8_t*) &response);

    /* Indicate value to client */
    (void)GattServer_SendIndication(mGls_ClientDeviceId, handle);
}

static bool_t Gls_MatchMeasurement(glsFullMeasurement_t *pMeasurement, glsProcedure_t* pProcedure)
{
    bool_t result;
    result = FALSE;

    switch(pProcedure->glsOperator)
    {
        case gGls_AllRecords_c:
        {
            result = TRUE;
            break;
        }
        case gGls_GreaterThanOrEqualTo_c:
        {
            switch(pProcedure->operand.filter.filter)
            {
                case gGls_FtvSeqNumber_c:
                {
                    result = (pMeasurement->seqNumber >= pProcedure->operand.filter.filterParameter);
                    break;
                }
                case gGls_FtvUserFacingTime_c:
                    break;
                default:
                {
                    ; /* For MISRA compliance */
                    break;
                }
            }
            break;
        }
        case gGls_FirstRecord_c:
        case gGls_LastRecord_c:
        case gGls_LessThanOrEqualTo_c:
        case gGls_WithinRangeOf_c:
        default:
        {
            ; /* For MISRA compliance */
        }
        break;
    }

    return result;
}

static void Gls_AbortOperation(glsConfig_t *pServiceConfig, uint16_t handle)
{
    glsProcedure_t      response;

    response.opCode = gGls_RspCode_c;
    response.glsOperator = gGls_Null_c;
    response.operand.responseCode.reqOpCode = gGls_AbortOperation_c;
    response.operand.responseCode.rspCodeValue = gGls_RspSuccess_c;

    pServiceConfig->procInProgress = FALSE;

    /* Write response in characteristic */
    (void)GattDb_WriteAttribute(handle, sizeof(glsProcedure_t) - 2U, (uint8_t*) &response);

    /* Indicate value to client */
    (void)GattServer_SendIndication(mGls_ClientDeviceId, handle);
}

static void Gls_ExecuteProcedure(glsConfig_t *pServiceConfig, glsProcedure_t* pProcedure, uint16_t handle)
{
    switch (pProcedure->opCode)
    {
        case gGls_ReportStoredRecords_c:
        {
            mpServiceConfig = pServiceConfig;
            FLib_MemCpy(&mProcedure, pProcedure, sizeof(glsProcedure_t));
            mHandle = handle;
            mMeasurementIndex = 0;
            Gls_ReportStoredRecords(pServiceConfig, pProcedure, handle);
            break;
        }

        case gGls_ReportNumOfStoredRecords_c:
        {
            Gls_ReportNumOfStoredRecords(pServiceConfig, pProcedure, handle);
            break;
        }
        case gGls_AbortOperation_c:
        {
            Gls_AbortOperation(pServiceConfig, handle);
            break;
        }

        default:
        {
            /* For MISRA compliance */
            break;
        }
    }
}

static glsRspCodeValue_t Gls_ValidateProcedure(glsProcedure_t* pProcedure, uint8_t procDataLength)
{
    glsRspCodeValue_t status = gGls_RspSuccess_c;

    if( pProcedure->glsOperator > gGls_LastRecord_c )
    {
        status = gGls_RspOperatorNotSupported_c;
    }

    switch (pProcedure->opCode)
    {
        case gGls_ReportStoredRecords_c:
        case gGls_ReportNumOfStoredRecords_c:
        {
            switch(pProcedure->glsOperator)
            {
                case gGls_AllRecords_c:
                {
                    if (procDataLength != 0U)
                    {
                        status = gGls_RspInvalidOperand_c;
                    }
                    break;
                }
                case gGls_GreaterThanOrEqualTo_c:
                {
                    switch(pProcedure->operand.filter.filter)
                    {
                        case gGls_FtvSeqNumber_c:
                        {
                            if ( procDataLength != sizeof(glsFilter_t))
                            {
                                status = gGls_RspInvalidOperand_c;
                            }
                            break;
                        }
                        case gGls_FtvUserFacingTime_c:
                            break;
                        default:
                        {
                            status = gGls_RspOperandNotSupported_c;
                            break;
                        }
                    }
                    break;
                }
                case gGls_FirstRecord_c:
                case gGls_LastRecord_c:
                case gGls_LessThanOrEqualTo_c:
                case gGls_WithinRangeOf_c:
                default:
                {
                    status = gGls_RspInvalidOperator_c;
                }
                break;

            }
            break;
        }
        case gGls_AbortOperation_c:
        {
            if( pProcedure->glsOperator != gGls_Null_c )
            {
                status = gGls_RspInvalidOperator_c;
            }

            if ( procDataLength != 0U )
            {
                status = gGls_RspInvalidOperand_c;
            }
            break;
        }
        case gGls_DeleteStoredRecords_c:
            break;
        default:
        {
            status = gGls_RspOpCodeNotSupported_c;
            break;
        }
    }

    return status;
}

static bleResult_t Gls_StoreGlucoseMeasurement(glsUserData_t *pUserData, glsFullMeasurement_t *pMeasurement)
{
    bleResult_t result = gBleOutOfMemory_c;

    if (pUserData->pStoredMeasurements != NULL)
    {
        /* Store measurement */
        FLib_MemCpy(&pUserData->pStoredMeasurements[pUserData->measurementCursor],
                    pMeasurement,
                    sizeof(glsFullMeasurement_t));

        /* Update cursor and count */
        pUserData->measurementCursor = (pUserData->measurementCursor + 1U) % gGls_MaxNumOfStoredMeasurements_c;
        pUserData->cMeasurements = MIN(pUserData->cMeasurements + 1U, gGls_MaxNumOfStoredMeasurements_c);

        result = gBleSuccess_c;
    }

    return result;
}

static void ReportTimerCallback(void * pParam)
{
    Gls_ReportStoredRecords(mpServiceConfig, &mProcedure, mHandle);
}

/*! *********************************************************************************
* @}
********************************************************************************** */
