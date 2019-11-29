/*! *********************************************************************************
* \addtogroup Blood Pressure Service
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

#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"

#include "blood_pressure_interface.h"
/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
#define gBps_BpmMaxCharValueLen 19

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
/*! Blood Pressure Service - Subscribed Client */
deviceId_t mBps_ClientDeviceId = gInvalidDeviceId_c;

/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
********************************************************************************** */
static bleResult_t Hrs_UpdateBpmCharacteristic
(
 uint16_t handle,
 bpsMeasurement_t *pMeasurement
);

static bleResult_t Hrs_UpdateIntCuffPressureCharacteristic
(
 uint16_t handle,
 cuffPressureMeasurement_t *pMeasurement
);

static void Bps_SendCuffPressureNotification
(
 uint16_t handle
);

static void Bps_SendBpmIndication
(
 uint16_t handle
);

static bleResult_t Bps_SetBloodPressureFeature
(
    uint16_t            handle,
    bpsFeatureFlags_t   feature
);
/***********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

bleResult_t Bps_Start (bpsConfig_t *pServiceConfig)
{
    uint16_t  hValueBpFeature;
    bleUuid_t uuidBpFeature = Uuid16(gBleSig_BloodPressureFeature_d);
    bleResult_t result;

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuidBpFeature, &hValueBpFeature);

    if (result != gBleSuccess_c)
        return result;

    return Bps_SetBloodPressureFeature(hValueBpFeature, pServiceConfig->bpsfeatureFlags);

}

bleResult_t Bps_Stop (bpsConfig_t *pServiceConfig)
{    
    return Bps_Unsubscribe();
}

bleResult_t Bps_Subscribe(deviceId_t clientdeviceId)
{
    mBps_ClientDeviceId = clientdeviceId;
    return gBleSuccess_c;
}

bleResult_t Bps_Unsubscribe()
{
    mBps_ClientDeviceId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Bps_RecordBloodPressureMeasurement (uint16_t serviceHandle, bpsMeasurement_t *pMeasurement)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_BpMeasurement_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Update characteristic value and send indication */
    if (!Hrs_UpdateBpmCharacteristic(handle, pMeasurement))
    {
        Bps_SendBpmIndication(handle);
    }
    return gBleSuccess_c;
}

bleResult_t Bps_RecordCuffPressureMeasurement (uint16_t serviceHandle, cuffPressureMeasurement_t *pMeasurement)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_IntermediateCuffPressure_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &handle);
    
    if (result != gBleSuccess_c)
        return result;    
		
    /* Update characteristic value and send indication */
    if (!Hrs_UpdateIntCuffPressureCharacteristic(handle, pMeasurement))
    {
        Bps_SendCuffPressureNotification(handle);
    }
    return gBleSuccess_c;
}

bleResult_t Bps_StoreBloodPressureMeasurement(bpsUserData_t *pUserData, bpsMeasurement_t *pMeasurement)
{
#if gBps_EnableStoredMeasurements_d
    /* Store measurement */
    FLib_MemCpy(&pUserData->pStoredMeasurements[pUserData->measurementCursor],
                pMeasurement,
                sizeof(bpsMeasurement_t));

    /* Update cursor and count */
    pUserData->measurementCursor = (pUserData->measurementCursor + 1) % gBps_MaxNumOfStoredMeasurements_c;
    pUserData->cMeasurements = MIN(pUserData->cMeasurements + 1, gBps_MaxNumOfStoredMeasurements_c);
#endif
    return gBleSuccess_c;  
}

void Bps_SendStoredBloodPressureMeasurement(bpsConfig_t *pServiceConfig)
{
#if gBps_EnableStoredMeasurements_d
    uint8_t oldestMeasurementIdx;
    
    if (pServiceConfig->pUserData->cMeasurements == 0)
      return;

    oldestMeasurementIdx = (pServiceConfig->pUserData->measurementCursor + (gBps_MaxNumOfStoredMeasurements_c + pServiceConfig->pUserData->cMeasurements)) % gBps_MaxNumOfStoredMeasurements_c;

    Bps_RecordBloodPressureMeasurement(pServiceConfig->serviceHandle, 
                                       &pServiceConfig->pUserData->pStoredMeasurements[oldestMeasurementIdx]);
    
    /* Update measurement count */
    pServiceConfig->pUserData->cMeasurements -= 1;
#endif
}

/***********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
static bleResult_t Hrs_UpdateBpmCharacteristic
(
 uint16_t handle,
 bpsMeasurement_t *pMeasurement
)
{
    uint8_t charValue[gBps_BpmMaxCharValueLen];
    uint8_t index = 0;

    /* Add flags */
    charValue[0] = ((pMeasurement->unit)?gBps_UnitInkPa_c:gBps_UnitInMmHg_c);

    index += 1;

    /* Add measurements */
    FLib_MemCpy(&charValue[index], &pMeasurement->systolicValue, 3 * sizeof(ieee11073_16BitFloat_t));
    index += 3 * sizeof(ieee11073_16BitFloat_t);

    /* Add time stamp */
    if (pMeasurement->timeStampPresent)
    {
        charValue[0] |= gBps_TimeStampPresent_c;
        FLib_MemCpy(&charValue[index], &pMeasurement->timeStamp, sizeof(ctsDateTime_t));
        index += sizeof(ctsDateTime_t);
    }

    /* Add pulse rate */
    if (pMeasurement->pulseRatePresent)
    {
        charValue[0] |= gBps_PulseRatePresent_c;
        FLib_MemCpy(&charValue[index], &pMeasurement->pulseRate, sizeof(ieee11073_16BitFloat_t));
        index += sizeof(ieee11073_16BitFloat_t);
    }

#if gBps_EnableMultiUserSupport_d    
    /* Add user id */
    if (pMeasurement->userIdPresent)
    {
        charValue[0] |= gBps_UserIdPresent_c;
        charValue[index] = pMeasurement->userId;
        index += sizeof(uint8_t);
    }
#endif
    
    /* Add measurement status */
    if (pMeasurement->measurementStatusPresent)
    {
        charValue[0] |= gBps_MeasurementStatusPresent_c;
        FLib_MemCpy(&charValue[index], &pMeasurement->measurementStatus, sizeof(bpsMeasureStatusFlags_t));
        index += sizeof(bpsMeasureStatusFlags_t);
    }

    return GattDb_WriteAttribute(handle, index, &charValue[0]);
}

static bleResult_t Hrs_UpdateIntCuffPressureCharacteristic
(
 uint16_t handle,
 cuffPressureMeasurement_t *pMeasurement
)
{
    uint8_t charValue[gBps_BpmMaxCharValueLen];
    uint8_t index = 0;

    /* Add flags */
    charValue[0] = ((pMeasurement->unit)?gBps_UnitInkPa_c:gBps_UnitInMmHg_c);

    index += 1;

    /* Add measurements */

    FLib_MemCpy(&charValue[index], &pMeasurement->cuffPressure, sizeof(ieee11073_16BitFloat_t));
    index += sizeof(ieee11073_16BitFloat_t);

    /*TODO need a flib function*/

    *(charValue + index) =  (gIeee11073_16Bit_NaN_c & 0xFF);
    *(charValue + index + 1) =  (gIeee11073_16Bit_NaN_c >> 8) & 0xFF;
    index += sizeof(ieee11073_16BitFloat_t);

    *(charValue + index) =  (gIeee11073_16Bit_NaN_c & 0xFF);
    *(charValue + index + 1) =  (gIeee11073_16Bit_NaN_c >> 8) & 0xFF;
    index += sizeof(ieee11073_16BitFloat_t);

#if gBps_EnableMultiUserSupport_d    
    /* Add user id */
    if (pMeasurement->userIdPresent)
    {
        charValue[0] |= gBps_UserIdPresent_c;
        charValue[index] = pMeasurement->userId;
        index += sizeof(uint8_t);
    }
#endif

    /* Add measurement status */
    if (pMeasurement->measurementStatusPresent)
    {
        charValue[0] |= gBps_MeasurementStatusPresent_c;
        FLib_MemCpy(&charValue[index], &pMeasurement->measurementStatus, sizeof(bpsMeasureStatusFlags_t));
        index += sizeof(bpsMeasureStatusFlags_t);
    }

    return GattDb_WriteAttribute(handle, index, &charValue[0]);
}

static void Bps_SendCuffPressureNotification
(
  uint16_t handle
)
{
    uint16_t  hCccd;
    bool_t isNotifActive;
   
    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) != gBleSuccess_c)
        return;

    if (mBps_ClientDeviceId == gInvalidDeviceId_c)
      return;

    if (gBleSuccess_c == Gap_CheckNotificationStatus
        (mBps_ClientDeviceId, hCccd, &isNotifActive) &&
        TRUE == isNotifActive)
    {
        GattServer_SendNotification(mBps_ClientDeviceId, handle);
    }

}

static void Bps_SendBpmIndication
(
  uint16_t handle
)
{
    uint16_t  hCccd;
    bool_t isIndicationActive;
   
    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) != gBleSuccess_c)
        return;
    
    if (mBps_ClientDeviceId == gInvalidDeviceId_c)
      return;

    if (gBleSuccess_c == Gap_CheckIndicationStatus
        (mBps_ClientDeviceId, hCccd, &isIndicationActive) &&
        TRUE == isIndicationActive)
    {
        GattServer_SendIndication(mBps_ClientDeviceId, handle);
    }
}

static bleResult_t Bps_SetBloodPressureFeature
(
    uint16_t            handle,
    bpsFeatureFlags_t   feature
)
{
    return GattDb_WriteAttribute(handle, sizeof(uint8_t), (uint8_t*)&feature);
}
/*! *********************************************************************************
* @}
********************************************************************************** */
