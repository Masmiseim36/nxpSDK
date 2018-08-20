/*! *********************************************************************************
* \addtogroup Health Thermometer Service
* @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
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

#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"

#include "current_time_interface.h"
#include "health_thermometer_interface.h"
/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

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
/*! Health Thermometer Service - Subscribed Client */
deviceId_t mHts_ClientDeviceId = gInvalidDeviceId_c;

/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
********************************************************************************** */
static bleResult_t Hts_UpdateTempMeasurementCharacteristic
(
 uint16_t handle,
 htsMeasurement_t *pMeasurement
);

static void Hts_SendTempMeasurementIndication
(
  uint16_t handle
);

static void Hts_SendIntermediateTempNotification
(
  uint16_t handle
);

static bleResult_t Hts_SetTemperatureType
(
 uint16_t serviceHandle,
 htsTempType_t tempType
);

/***********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

bleResult_t Hts_Start (htsConfig_t *pServiceConfig)
{
    bleResult_t result;

    mHts_ClientDeviceId = gInvalidDeviceId_c;
      
    /* Set temperature type characteristic */
    result = Hts_SetTemperatureType(pServiceConfig->serviceHandle, pServiceConfig->tempType);

    /* Set measurement interval */
    result = Hts_SetMeasurementInterval(pServiceConfig, pServiceConfig->measurementInterval);
    return result;
}

bleResult_t Hts_Stop (htsConfig_t *pServiceConfig)
{    
    return Hts_Unsubscribe();
}

bleResult_t Hts_Subscribe(deviceId_t clientdeviceId)
{
    mHts_ClientDeviceId = clientdeviceId;
    return gBleSuccess_c;
}

bleResult_t Hts_Unsubscribe()
{
    mHts_ClientDeviceId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Hts_RecordTemperatureMeasurement (uint16_t serviceHandle, htsMeasurement_t *pMeasurement)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = {.uuid16 = gBleSig_TemperatureMeasurement_d};

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Update characteristic value and send indication */
    if (!Hts_UpdateTempMeasurementCharacteristic(handle, pMeasurement))
    {
        Hts_SendTempMeasurementIndication(handle);
    }
    return gBleSuccess_c;
}

bleResult_t Hts_RecordIntermediateTemperature(uint16_t serviceHandle, htsMeasurement_t *pMeasurement)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = {.uuid16 = gBleSig_IntermediateTemperature_d};

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Update characteristic value and send indication */
    if (!Hts_UpdateTempMeasurementCharacteristic(handle, pMeasurement))
    {
        Hts_SendIntermediateTempNotification(handle);
    }
    return gBleSuccess_c;
}

bleResult_t Hts_SetMeasurementInterval(htsConfig_t *pServiceConfig, uint16_t measurementInterval)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = {.uuid16 = gBleSig_MeasurementInterval_d};

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Update characteristic value */
    result = GattDb_WriteAttribute(handle, sizeof(uint16_t), (uint8_t*)&measurementInterval);

    if (result != gBleSuccess_c)
        return result;

    /* Indicate the current value of the characteristic the next time there is a connection */
    if (mHts_ClientDeviceId != gInvalidDeviceId_c)
    {
        pServiceConfig->pUserData->measIntervalIndPending = TRUE;
        return gBleSuccess_c;
    }

    Hts_SendMeasurementIntervalIndication(handle);

    return gBleSuccess_c;
}

bleResult_t Hts_StoreTemperatureMeasurement(htsUserData_t *pUserData, htsMeasurement_t *pMeasurement)
{
#if gHts_EnableStoredMeasurements_d
    /* Store measurement */
    FLib_MemCpy(&pUserData->pStoredMeasurements[pUserData->measurementCursor],
                pMeasurement,
                sizeof(htsMeasurement_t));

    /* Update cursor and count */
    pUserData->measurementCursor = (pUserData->measurementCursor + 1) % gHts_MaxNumOfStoredMeasurements_c;
    pUserData->cMeasurements = MIN(pUserData->cMeasurements + 1, gHts_MaxNumOfStoredMeasurements_c);
#endif
    return gBleSuccess_c;
}

void Hts_SendStoredTemperatureMeasurement(htsConfig_t *pServiceConfig)
{
#if gHts_EnableStoredMeasurements_d
    uint8_t oldestMeasurementIdx;

    if (pServiceConfig->pUserData->cMeasurements == 0)
      return;

    oldestMeasurementIdx = (pServiceConfig->pUserData->measurementCursor + (gHts_MaxNumOfStoredMeasurements_c + pServiceConfig->pUserData->cMeasurements)) % gHts_MaxNumOfStoredMeasurements_c;

    Hts_RecordTemperatureMeasurement(pServiceConfig->serviceHandle,
                                       &pServiceConfig->pUserData->pStoredMeasurements[oldestMeasurementIdx]);

    /* Update measurement count */
    pServiceConfig->pUserData->cMeasurements -= 1;
#endif
}

attErrorCode_t Hts_MeasurementIntervalWriting(htsConfig_t *pServiceConfig, uint16_t handle, uint16_t newMeasurementInterval)
{
    bleUuid_t uuid = Uuid16(gBleSig_ValidRangeDescriptor_d);
    uint16_t  descHandle, minValue, maxValue, descValueLength;
    uint8_t aRangeValues[4];
    bleResult_t result;

    /* Find range descriptor*/
    GattDb_FindDescriptorHandleForCharValueHandle(handle,
                                                  gBleUuidType16_c,
                                                  &uuid,
                                                  &descHandle);
    /* Read range descriptor */
    GattDb_ReadAttribute(descHandle, sizeof(aRangeValues), &aRangeValues[0], &descValueLength);

    minValue = Utils_ExtractFourByteValue(&aRangeValues[0]);
    maxValue = Utils_ExtractFourByteValue(&aRangeValues[2]);

    if ((newMeasurementInterval != 0) && 
        ((newMeasurementInterval < minValue) || (newMeasurementInterval > maxValue)))
    {

        return (attErrorCode_t)gAttErrCodeOutOfRange_c;
    }
    
    /* Write new value in database*/
    result = Hts_SetMeasurementInterval(pServiceConfig, newMeasurementInterval);
    if (gGattDbSuccess_c != result)
    {
        return gAttErrCodeUnlikelyError_c;
    }
    return gAttErrCodeNoError_c;
}

void Hts_SendMeasurementIntervalIndication
(
  uint16_t handle
)
{
    uint16_t  hCccd;
    bool_t isIndicationActive;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) != gBleSuccess_c)
        return;

    if (gBleSuccess_c == Gap_CheckIndicationStatus
        (mHts_ClientDeviceId, hCccd, &isIndicationActive) &&
        TRUE == isIndicationActive)
    {
        GattServer_SendIndication(mHts_ClientDeviceId, handle);
    }
}

/***********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static bleResult_t Hts_UpdateTempMeasurementCharacteristic
(
 uint16_t handle,
 htsMeasurement_t *pMeasurement
)
{
    uint8_t charValue[13];
    uint8_t index = 0;

    /* Add flags */
    charValue[0] = pMeasurement->unit;

    index += 1;

    /* Add measurements */
    FLib_MemCpy(&charValue[index], &pMeasurement->temperature, sizeof(ieee11073_32BitFloat_t));
    index += sizeof(ieee11073_32BitFloat_t);

    /* Add time stamp */
    if (pMeasurement->timeStampPresent)
    {
        charValue[0] |= gHts_TimeStampPresent_c;
        FLib_MemCpy(&charValue[index], &pMeasurement->timeStamp, sizeof(ctsDateTime_t));
        index += sizeof(ctsDateTime_t);
    }

    /* Add temperature type */
    if (pMeasurement->tempTypePresent)
    {
        charValue[0] |= gHts_TempTypePresent_c;
        charValue[index] = pMeasurement->tempType;
        index += sizeof(htsTempType_t);
    }

    return GattDb_WriteAttribute(handle, index, &charValue[0]);
}


static void Hts_SendTempMeasurementIndication
(
  uint16_t handle
)
{
    uint16_t  hCccd;
    bool_t isIndicationActive;
   
    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) != gBleSuccess_c)
        return;

    if (gBleSuccess_c == Gap_CheckIndicationStatus
        (mHts_ClientDeviceId, hCccd, &isIndicationActive) &&
        TRUE == isIndicationActive)
    {
        GattServer_SendIndication(mHts_ClientDeviceId, handle);
    }
}

static void Hts_SendIntermediateTempNotification
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
        (mHts_ClientDeviceId, hCccd, &isNotificationActive) &&
        TRUE == isNotificationActive)
    {
        GattServer_SendNotification(mHts_ClientDeviceId, handle);
    }
}

static bleResult_t Hts_SetTemperatureType
(
 uint16_t serviceHandle,
 htsTempType_t tempType
)
{
    uint16_t  hValue;
    bleUuid_t uuid = {.uuid16 = gBleSig_TemperatureType_d};
    bleResult_t result;

    result = GattDb_FindCharValueHandleInService(serviceHandle,
       gBleUuidType16_c, &uuid, &hValue);

    if(result != gBleSuccess_c)
        return result;

    return GattDb_WriteAttribute(hValue, sizeof(htsTempType_t), &tempType);
}


/*! *********************************************************************************
* @}
********************************************************************************** */
