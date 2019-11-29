/*! *********************************************************************************
* \addtogroup Glucose Service
* @{
 ********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
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
deviceId_t mGls_ClientDeviceId = gInvalidDeviceId_c;

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
    
    if(pServiceConfig->pUserData)
    {
        pServiceConfig->pUserData->lastSeqNumber = 0x0000;
    }
    
    for (i = 0; i < gGls_MaxNumOfStoredMeasurements_c; i++)
    {
        pServiceConfig->pUserData->pStoredMeasurements[i].seqNumber = 0xFFFF;
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
    uint16_t  handle;
    bleResult_t result;
    uint16_t uuid = gBleSig_GlucoseMeasurement_d;

    /* Glucose Measurement */
    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
                gBleUuidType16_c, (bleUuid_t*)&uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Store measurements */
    Gls_StoreGlucoseMeasurement(pServiceConfig->pUserData, pMeasurement);

    /* Update characteristic value and send notification */
    if (!Gls_UpdateGlucoseMeasCharacteristic(handle, pMeasurement))
    {
        Gls_SendNotification(handle);
    }

    if(pMeasurement->flags & gGls_ContextInfoFollows_c)
    {
        uuid = gBleSig_GlucoseMeasurementContext_d;

        /* Get handle of characteristic */
        result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
                    gBleUuidType16_c, (bleUuid_t*)&uuid, &handle);

        if (result != gBleSuccess_c)
            return result;

        /* Update characteristic value and send notification */
        if (!Gls_UpdateGlucoseMeasContextCharacteristic(handle, pMeasurement))
        {
            Gls_SendNotification(handle);
        }
    }
    return gBleSuccess_c;
}

void Gls_ControlPointHandler (glsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    bool_t  fIndicationActive = FALSE;
    uint16_t  handleCccd;
   
    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(pEvent->handle, &handleCccd) != gBleSuccess_c)
        return;

    /* Check if indications are properly configured */
    Gap_CheckIndicationStatus(mGls_ClientDeviceId, handleCccd, &fIndicationActive);

    if(!fIndicationActive)
    {
        GattServer_SendAttributeWrittenStatus(mGls_ClientDeviceId, pEvent->handle,
                                                 gAttErrCodeGlsCccdImproperlyConfigured_c);
        return;
    }

    /* Check if another procedure is in progress and we do not abort */
    if (pServiceConfig->procInProgress &&
        ((glsProcedure_t*)pEvent->aValue)->opCode != gGls_AbortOperation_c)
    {
        GattServer_SendAttributeWrittenStatus(mGls_ClientDeviceId, pEvent->handle,
                                                 gAttErrCodeGlsProcedureAlreadyInProgress_c);
        return;
    }

    pServiceConfig->procInProgress = TRUE;

    /* Procedure received successfully */
    GattServer_SendAttributeWrittenStatus(mGls_ClientDeviceId, pEvent->handle,
                                                     gAttErrCodeNoError_c);

    Gls_SendProcedureResponse(pServiceConfig, pEvent);
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

    index = 3 + sizeof(ctsDateTime_t);

    if (pMeasurement->flags & gGls_TimeOffsetPresent_c)
    {
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->timeOffset);
        index += sizeof(int16_t);
    }

    FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->glucCon);
    index += sizeof(ieee11073_16BitFloat_t);

    if (pMeasurement->flags & gGls_ConcTypeSampleLocPresent_c)
    {
        charValue[index] = pMeasurement->typeAndSampleLoc.type +
                        (pMeasurement->typeAndSampleLoc.sampleLoc << 4);
        index += 1;
    }

    if (pMeasurement->flags & gGls_SensorStatusPresent_c)
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

    index = 3;

    if (pMeasurement->contextFlags & gGls_ExtendedFlagsPresent_c)
    {
        charValue[index] = pMeasurement->extendedFlags;
        index += 1;
    }

    if (pMeasurement->contextFlags & gGls_CarbPresent_c)
    {
        charValue[index] = pMeasurement->carbId;
        index += 1;
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->carbUnits);
        index += sizeof(ieee11073_16BitFloat_t);
    }

    if (pMeasurement->contextFlags & gGls_MealPresent_c)
    {
        charValue[index] = pMeasurement->meal;
        index += 1;
    }

    if (pMeasurement->contextFlags & gGls_TesterHealthPresent_c)
    {
        charValue[index] = pMeasurement->testerHealth.tester +
                        (pMeasurement->testerHealth.health << 4);
        index += 1;
    }

    if (pMeasurement->contextFlags & gGls_ExerciseDurationIntesityPresent_c)
    {
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->exerciseDuration);
        index += sizeof(int16_t);
        charValue[index] = pMeasurement->exerciseIntensity;
        index += 1;
    }

    if (pMeasurement->contextFlags & gGls_MedicationPresent_c)
    {
        charValue[index] = pMeasurement->medicationId;
        index += 1;
        FLib_MemCopy16Unaligned(&charValue[index], pMeasurement->medicationUnits);
        index += sizeof(ieee11073_16BitFloat_t);
    }

    if (pMeasurement->contextFlags & gGls_HbA1cPresent_c)
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
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) != gBleSuccess_c)
        return;
    
    if (mGls_ClientDeviceId == gInvalidDeviceId_c)
      return;

    if (gBleSuccess_c == Gap_CheckNotificationStatus
        (mGls_ClientDeviceId, hCccd, &isNotificationActive) &&
        TRUE == isNotificationActive)
    {
        GattServer_SendNotification(mGls_ClientDeviceId, handle);
    }
}

static bleResult_t Gls_SetGlucoseFeature
(
    uint16_t            handle,
    glsFeatureFlags_t   feature
)
{
    uint16_t  hValueGlFeature;
    uint16_t uuid = gBleSig_GlucoseFeature_d;
    bleResult_t result;
  
      /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(handle,
                gBleUuidType16_c, (bleUuid_t*)&uuid, &hValueGlFeature);

    if (result != gBleSuccess_c)
        return result;
    
    return GattDb_WriteAttribute(hValueGlFeature, sizeof(glsFeatureFlags_t), (uint8_t*)&feature);
}

static void Gls_SendStoredGlucoseMeasurement(glsConfig_t *pServiceConfig, uint8_t index)
{
    uint16_t  handle;
    bleResult_t result;
    uint16_t uuid = gBleSig_GlucoseMeasurement_d;
    glsFullMeasurement_t *pMeasurement = pServiceConfig->pUserData->pStoredMeasurements + index;
      
    /* Glucose Measurement */
    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
                gBleUuidType16_c, (bleUuid_t*)&uuid, &handle);

    if (result != gBleSuccess_c)
        return;

    /* Update characteristic value and send notification */
    if (!Gls_UpdateGlucoseMeasCharacteristic(handle, pMeasurement))
    {
        Gls_SendNotification(handle);
    }

    if(pMeasurement->flags & gGls_ContextInfoFollows_c)
    {
        uuid = gBleSig_GlucoseMeasurementContext_d;

        /* Get handle of characteristic */
        result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
                    gBleUuidType16_c, (bleUuid_t*)&uuid, &handle);

        if (result != gBleSuccess_c)
            return;

        /* Update characteristic value and send notification */
        if (!Gls_UpdateGlucoseMeasContextCharacteristic(handle, pMeasurement))
        {
            Gls_SendNotification(handle);
        }
    }  
}

static void Gls_SendProcedureResponse
(
     glsConfig_t *pServiceConfig,
     gattServerAttributeWrittenEvent_t* pEvent
)
{
    glsProcedure_t      response;
    glsProcedure_t*     pProcedure = (glsProcedure_t*)pEvent->aValue;
    uint8_t             procDataLength = pEvent->cValueLength - sizeof(glsOpCode_t) - sizeof(glsOperator_t);

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
        GattDb_WriteAttribute(pEvent->handle, sizeof(glsProcedure_t) - 2, (uint8_t*) &response);

        /* Indicate value to client */
        GattServer_SendIndication(mGls_ClientDeviceId, pEvent->handle);
    }
}

static bool_t Gls_CheckFilterCondition(glsProcedure_t* pProcedure, glsFullMeasurement_t *pMeasurement)
{
  if ( pProcedure->glsOperator == gGls_AllRecords_c )
  {
      return TRUE;
  }
  
  if(( pProcedure->glsOperator == gGls_GreaterThanOrEqualTo_c )
      &&( pProcedure->operand.filter.filter == gGls_FtvSeqNumber_c ))
  {
      if (pMeasurement->seqNumber >= pProcedure->operand.filter.filterParameter)
      {
        return TRUE;
      }
  }

  return FALSE;
}

static void Gls_ReportStoredRecords(glsConfig_t *pServiceConfig, glsProcedure_t* pProcedure, uint16_t handle)
{
    glsFullMeasurement_t *pMeasurement;
    glsProcedure_t      response;
    uint8_t             index;
    bool_t isIndicationActive;
    uint16_t  hCccd;
    
    if (!pServiceConfig->procInProgress)
    {
      return;
    }
    
    while ( mMeasurementIndex < gGls_MaxNumOfStoredMeasurements_c)
    {
        index = (pServiceConfig->pUserData->measurementCursor + mMeasurementIndex) % gGls_MaxNumOfStoredMeasurements_c;
        pMeasurement = pServiceConfig->pUserData->pStoredMeasurements + index;
        
        if (pMeasurement->seqNumber == 0xFFFF)
        {
            mMeasurementIndex++;
            continue;
        }
        
        if (Gls_CheckFilterCondition(pProcedure, pMeasurement))
        {
            Gls_SendStoredGlucoseMeasurement(pServiceConfig, index);
            mMeasurementIndex++;
                /* Start advertising timer */
            TMR_StartLowPowerTimer(mReportTimerId, gTmrLowPowerSecondTimer_c,
                       500, ReportTimerCallback, NULL);
            return;
        }
        
        mMeasurementIndex++;
    }
    
    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) != gBleSuccess_c)
        return;
    
    response.opCode = gGls_RspCode_c;
    response.glsOperator = gGls_Null_c;
    response.operand.responseCode.reqOpCode = gGls_ReportStoredRecords_c;
    response.operand.responseCode.rspCodeValue = 
      (pServiceConfig->pUserData->cMeasurements == 0) ? 
        gGls_RspNoRecordsFound_c : gGls_RspSuccess_c;
        
    /* Indicate value to client */     
    if (gBleSuccess_c == Gap_CheckIndicationStatus
        (mGls_ClientDeviceId, hCccd, &isIndicationActive) &&
        TRUE == isIndicationActive)
    {
        GattServer_SendInstantValueIndication(mGls_ClientDeviceId, handle,
                    sizeof(glsProcedure_t) - 2, (uint8_t*) &response);
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
    GattDb_WriteAttribute(handle, sizeof(glsProcedure_t) - 2, (uint8_t*) &response);

    /* Indicate value to client */
    GattServer_SendIndication(mGls_ClientDeviceId, handle);    
}

static bool_t Gls_MatchMeasurement(glsFullMeasurement_t *pMeasurement, glsProcedure_t* pProcedure)
{
    switch(pProcedure->glsOperator)
    {
        case gGls_AllRecords_c:
        {
            return TRUE;
            break;
        }
        case gGls_GreaterThanOrEqualTo_c:
        {
            switch(pProcedure->operand.filter.filter)
            {
                case gGls_FtvSeqNumber_c:
                {
                    return (pMeasurement->seqNumber >= pProcedure->operand.filter.filterParameter);
                    break;
                }
                case gGls_FtvUserFacingTime_c:
                default:
                {
                    return FALSE;
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
            return FALSE;
        }
        break;  
    }
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
    GattDb_WriteAttribute(handle, sizeof(glsProcedure_t) - 2, (uint8_t*) &response);

    /* Indicate value to client */
    GattServer_SendIndication(mGls_ClientDeviceId, handle);
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
            break;
        }
    }
}

static glsRspCodeValue_t Gls_ValidateProcedure(glsProcedure_t* pProcedure, uint8_t procDataLength)
{

    if( pProcedure->glsOperator > gGls_LastRecord_c )
    {
        return gGls_RspOperatorNotSupported_c;
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
                    if (procDataLength != 0)
                    {
                        return gGls_RspInvalidOperand_c;
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
                                return gGls_RspInvalidOperand_c;
                            }
                            break;
                        }
                        case gGls_FtvUserFacingTime_c:
                        default:
                        {
                            return gGls_RspOperandNotSupported_c;
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
                    return gGls_RspInvalidOperator_c;
                }
                break;

            }
            break;
        }
        case gGls_AbortOperation_c:
        {
            if( pProcedure->glsOperator != gGls_Null_c )
            {
                return gGls_RspInvalidOperator_c;
            }

            if ( procDataLength != 0 )
            {
                return gGls_RspInvalidOperand_c;
            }
            break;
        }
        case gGls_DeleteStoredRecords_c:
        default:
        {
            return gGls_RspOpCodeNotSupported_c;
        }
    }

    return gGls_RspSuccess_c;
}

static bleResult_t Gls_StoreGlucoseMeasurement(glsUserData_t *pUserData, glsFullMeasurement_t *pMeasurement)
{
    if (pUserData->pStoredMeasurements == NULL)
    {
        return gBleOutOfMemory_c;
    }

    /* Store measurement */
    FLib_MemCpy(&pUserData->pStoredMeasurements[pUserData->measurementCursor],
                pMeasurement,
                sizeof(glsFullMeasurement_t));

    /* Update cursor and count */
    pUserData->measurementCursor = (pUserData->measurementCursor + 1) % gGls_MaxNumOfStoredMeasurements_c;
    pUserData->cMeasurements = MIN(pUserData->cMeasurements + 1, gGls_MaxNumOfStoredMeasurements_c);
    return gBleSuccess_c;
}

static void ReportTimerCallback(void * pParam)
{
    Gls_ReportStoredRecords(mpServiceConfig, &mProcedure, mHandle);
}

/*! *********************************************************************************
* @}
********************************************************************************** */
