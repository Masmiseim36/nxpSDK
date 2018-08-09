/*! *********************************************************************************
 * \addtogroup Heart Rate Profile
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
#include "heart_rate_interface.h"
/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
#define Hrs_GetHeartRateFormat(hr) (hr < 0x00FF) ? gHrs_8BitHeartRateFormat_c : gHrs_16BitHeartRateFormat_c
#define Hrs_MaxNotificationSize 22

/*! Heart Rate Service - Maximum Energy Expended Value */
#define gHrs_MaxEnergyExpendedValue_c                   0xFFFF
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/*! Heart Rate Service - Subscribed Client*/
deviceId_t mHrs_SubscribedClientId;


/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static bleResult_t Hrs_UpdateHrmCharacteristic
(
 uint16_t handle,
 uint16_t heartRate,
 hrsUserData_t *pHrsUserData
);

static void Hrs_SendNotifications
(
 uint16_t handle
);

static bleResult_t Hrs_GetHrmFlags
(
    uint16_t handle,
    uint8_t *pOutFlags
);

static bleResult_t Hrs_SetHrmFlags
(
    uint16_t handle,
    uint8_t  flags
);

static bleResult_t Hrs_SetBodyLocation
(
    uint16_t            handle,
    hrsBodySensorLoc_t  location
);
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

bleResult_t Hrs_Start (hrsConfig_t *pServiceConfig)
{
    uint16_t  hValueHrMeasurement;
    uint16_t  hValueBodyLocation;
    bleResult_t result;
    bleUuid_t uuidHrm = Uuid16(gBleSig_HrMeasurement_d);
    bleUuid_t uuidBodyLoc = Uuid16(gBleSig_BodySensorLocation_d);
    uint8_t flags = 0;

    /* Get handle or Heart Rate Measurement characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuidHrm, &hValueHrMeasurement);

    result |= GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuidBodyLoc, &hValueBodyLocation);

    if (result != gBleSuccess_c)
    {
        return result;
    }
    
    if (pServiceConfig->energyExpandedEnabled)
    {
        flags |= gHrs_EnergyExpendedEnabled_c;
    }
    
    if (pServiceConfig->sensorContactSupported)
    {       
        flags |= gHrs_SensorContactSupported_c;
    }
    
    if (pServiceConfig->sensorContactDetected)
    {       
        flags |= gHrs_SensorContactDetected_c;
    }
    
    Hrs_SetHrmFlags(hValueHrMeasurement, flags);
    
    Hrs_SetBodyLocation(hValueBodyLocation, pServiceConfig->bodySensorLocation);
    
    mHrs_SubscribedClientId = gInvalidDeviceId_c;

#if gHrs_EnableRRIntervalMeasurements_d
    if (pServiceConfig->pUserData && pServiceConfig->pUserData->pStoredRrIntervals)
    {
        FLib_MemSet(pServiceConfig->pUserData->pStoredRrIntervals, 0x00, gHrs_NumOfRRIntervalsRecorded_c * sizeof(uint16_t));
        pServiceConfig->pUserData->rrCursor = 0;
        pServiceConfig->pUserData->cRrIntervals = 0;
    }
#endif

    Hrs_ResetExpendedEnergy(pServiceConfig->pUserData);

    return gBleSuccess_c;
}

bleResult_t Hrs_Stop (hrsConfig_t *pServiceConfig)
{
    Hrs_Unsubscribe();
    return gBleSuccess_c;
}

bleResult_t Hrs_Subscribe(deviceId_t deviceId)
{
    mHrs_SubscribedClientId = deviceId;
    return gBleSuccess_c;
}

bleResult_t Hrs_Unsubscribe(void)
{
    mHrs_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Hrs_RecordHeartRateMeasurement (uint16_t serviceHandle, uint16_t heartRate, hrsUserData_t *pHrsUserData)
{
    uint16_t  hValueHrMeasurement;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_HrMeasurement_d);

    /* Get handle of Heart Rate Measurement characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &hValueHrMeasurement);

    if (result != gBleSuccess_c)
        return result;

    /* Update characteristic value and send notification */
    if (!Hrs_UpdateHrmCharacteristic(hValueHrMeasurement, heartRate, pHrsUserData))
    {
        Hrs_SendNotifications(hValueHrMeasurement);
    }
    return gBleSuccess_c;
}

bleResult_t Hrs_SetContactStatus (uint16_t serviceHandle, bool_t isDetected)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_HrMeasurement_d);
    uint8_t flags;

    /* Get handle of Heart Rate Measurement characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;
    
    Hrs_GetHrmFlags(handle, &flags);
    
    if (isDetected)
    {       
        flags |= gHrs_SensorContactDetected_c;
    }
    else
    {      
        flags &= ~gHrs_SensorContactDetected_c;
    }    
    
    return Hrs_SetHrmFlags(handle, flags);
}

bleResult_t Hrs_RecordRRInterval (hrsUserData_t *pHrsUserData, uint16_t rrInterval)
{

#if gHrs_EnableRRIntervalMeasurements_d  
    /* Store interval */
    if (!pHrsUserData->pStoredRrIntervals)
    {
        return gBleUnexpectedError_c;
    }

    pHrsUserData->pStoredRrIntervals[pHrsUserData->rrCursor] = rrInterval;

    /* Update cursor and count */
    pHrsUserData->rrCursor = (pHrsUserData->rrCursor + 1) % gHrs_NumOfRRIntervalsRecorded_c;
    pHrsUserData->cRrIntervals = MIN(pHrsUserData->cRrIntervals + 1, gHrs_NumOfRRIntervalsRecorded_c);
#endif
    return gBleSuccess_c;
}

void Hrs_ResetExpendedEnergy (hrsUserData_t *pHrsUserData)
{
    pHrsUserData->expendedEnergy = 0;
}

void Hrs_AddExpendedEnergy(hrsUserData_t *pHrsUserData, uint16_t energy)
{
    if ((gHrs_MaxEnergyExpendedValue_c - energy) < pHrsUserData->expendedEnergy)
    {
        pHrsUserData->expendedEnergy = gHrs_MaxEnergyExpendedValue_c;
    }
    else
    {
        pHrsUserData->expendedEnergy += energy;
    }
}

uint8_t Hrs_ControlPointHandler (hrsUserData_t *pHrsUserData, uint8_t value)
{
    uint8_t retStatus;

    switch (value)
    {
        case gHrs_CpResetEnergyExpended_c:
        {
            Hrs_ResetExpendedEnergy(pHrsUserData);
            retStatus = gAttErrCodeNoError_c;
        }
        break;
        
        default:
        {
            retStatus = gAttErrCodeCtrlPointValueNotSupported_c;
        }
        break;
    }
    return retStatus;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static bleResult_t Hrs_UpdateHrmCharacteristic
(
    uint16_t handle,
    uint16_t heartRate,
    hrsUserData_t *pHrsUserData
)
{
    uint8_t characteristic[Hrs_MaxNotificationSize];
    uint8_t index = 0;
    uint8_t iRRIntervalsCount;
    
    /* Add flags */
    Hrs_GetHrmFlags(handle, &characteristic[0]);
    index++;

    /* Add HR Value*/
    if (Hrs_GetHeartRateFormat(heartRate) & gHrs_16BitHeartRateFormat_c)
    {
        characteristic[0] |= gHrs_16BitHeartRateFormat_c;
        FLib_MemCpy(&characteristic[index], &heartRate, sizeof(uint16_t));
        index++;
    }
    else
    {
        characteristic[0] &= ~gHrs_16BitHeartRateFormat_c;
        characteristic[index] = (uint8_t) heartRate;
    }
    index++;

    /* Add EE */
    if (characteristic[0] & gHrs_EnergyExpendedEnabled_c)
    {
        FLib_MemCpy(&characteristic[index], &pHrsUserData->expendedEnergy, sizeof(uint16_t));
        index += sizeof(uint16_t);
    }

#if gHrs_EnableRRIntervalMeasurements_d

    /* Add RR Intervals */
    if (pHrsUserData->cRrIntervals > 0)
    {
        iRRIntervalsCount = MIN(
            (Hrs_MaxNotificationSize - index) / sizeof(uint16_t),
            pHrsUserData->cRrIntervals);
        
        characteristic[0] |= gHrs_RrIntervalsEnabled_c;

        if (pHrsUserData->rrCursor + iRRIntervalsCount > gHrs_NumOfRRIntervalsRecorded_c)
        {
            FLib_MemCpy(&characteristic[index],
                &pHrsUserData->pStoredRrIntervals[pHrsUserData->rrCursor],
                (gHrs_NumOfRRIntervalsRecorded_c - pHrsUserData->rrCursor)
                    * sizeof(uint16_t));
            
            index += (gHrs_NumOfRRIntervalsRecorded_c - pHrsUserData->rrCursor) * sizeof(uint16_t);

            FLib_MemCpy(&characteristic[index], &pHrsUserData->pStoredRrIntervals[0],
                (iRRIntervalsCount + pHrsUserData->rrCursor
                    - gHrs_NumOfRRIntervalsRecorded_c) * sizeof(uint16_t));
            
            index += (iRRIntervalsCount + pHrsUserData->rrCursor
                    - gHrs_NumOfRRIntervalsRecorded_c) * sizeof(uint16_t);            
        }
        else
        {
            FLib_MemCpy(&characteristic[index],
                &pHrsUserData->pStoredRrIntervals[0],
                iRRIntervalsCount * sizeof(uint16_t));
            index += iRRIntervalsCount * sizeof(uint16_t);
        }               

        /* Update circular buffer */
        pHrsUserData->rrCursor = (pHrsUserData->rrCursor + iRRIntervalsCount)
            % gHrs_NumOfRRIntervalsRecorded_c;
        pHrsUserData->cRrIntervals -= iRRIntervalsCount;

    }
#endif  /* gHrs_EnableRRIntervalMeasurements_d */

    return GattDb_WriteAttribute(handle, index, &characteristic[0]);
}

static void Hrs_SendNotifications
(
 uint16_t handle
)
{
    uint16_t  hCccdHrMeasurement;
    bool_t isNotifActive;
   
    /* Get handle of Heart Rate Measurement CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccdHrMeasurement) != gBleSuccess_c)
        return;

    if (mHrs_SubscribedClientId == gInvalidDeviceId_c)
      return;

    if (gBleSuccess_c == Gap_CheckNotificationStatus
        (mHrs_SubscribedClientId, hCccdHrMeasurement, &isNotifActive) &&
        TRUE == isNotifActive)
    {
        GattServer_SendNotification(mHrs_SubscribedClientId, handle);
    }
}

static bleResult_t Hrs_GetHrmFlags
(
    uint16_t handle,
    uint8_t *pOutFlags
)
{
    uint16_t length;
    
    return GattDb_ReadAttribute(handle, sizeof(uint8_t), pOutFlags, &length);
}

static bleResult_t Hrs_SetHrmFlags
(
    uint16_t handle,
    uint8_t  flags
)
{
    return GattDb_WriteAttribute(handle, sizeof(uint8_t), &flags);
}

static bleResult_t Hrs_SetBodyLocation
(
    uint16_t            handle,
    hrsBodySensorLoc_t  location
)
{
    return GattDb_WriteAttribute(handle, sizeof(uint8_t), &location);
}

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
