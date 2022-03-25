/*! *********************************************************************************
 * \addtogroup Heart Rate Profile
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
#define Hrs_GetHeartRateFormat(hr) ((hr) < 0x00FFU) ? gHrs_8BitHeartRateFormat_c : gHrs_16BitHeartRateFormat_c
#define Hrs_MaxNotificationSize 22U

/*! Heart Rate Service - Maximum Energy Expended Value */
#define gHrs_MaxEnergyExpendedValue_c                   0xFFFFU
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
static deviceId_t mHrs_SubscribedClientId;


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
    union
    {
        uint16_t               functionReturnValue;
        bleResult_t            resultTemp;
    }bleResultVars;

    uint16_t  hValueHrMeasurement;
    uint16_t  hValueBodyLocation;
    bleResult_t result;
    bleUuid_t uuidHrm = Uuid16(gBleSig_HrMeasurement_d);
    bleUuid_t uuidBodyLoc = Uuid16(gBleSig_BodySensorLocation_d);
    uint8_t flags = 0;

    /* Get handle or Heart Rate Measurement characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuidHrm, &hValueHrMeasurement);

    bleResultVars.functionReturnValue  = (uint16_t)result;
    bleResultVars.functionReturnValue |= (uint16_t)GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
                                                                                      gBleUuidType16_c, &uuidBodyLoc, &hValueBodyLocation);
    result = bleResultVars.resultTemp;

    if (result == gBleSuccess_c)
    {
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

        (void)Hrs_SetHrmFlags(hValueHrMeasurement, flags);

        (void)Hrs_SetBodyLocation(hValueBodyLocation, pServiceConfig->bodySensorLocation);

        mHrs_SubscribedClientId = gInvalidDeviceId_c;

        if (pServiceConfig->pUserData != NULL)
        {
#if gHrs_EnableRRIntervalMeasurements_d
            if (pServiceConfig->pUserData->pStoredRrIntervals != NULL)
            {
                FLib_MemSet(pServiceConfig->pUserData->pStoredRrIntervals, 0x00, gHrs_NumOfRRIntervalsRecorded_c * sizeof(uint16_t));
                pServiceConfig->pUserData->rrCursor = 0;
                pServiceConfig->pUserData->cRrIntervals = 0;
            }
#endif

            Hrs_ResetExpendedEnergy(pServiceConfig->pUserData);
        }
    }

    return result;
}

bleResult_t Hrs_Stop (hrsConfig_t *pServiceConfig)
{
    (void)Hrs_Unsubscribe();
    return gBleSuccess_c;
}

bleResult_t Hrs_Subscribe(deviceId_t clientDeviceId)
{
    mHrs_SubscribedClientId = clientDeviceId;
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

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value and send notification */
        if (gBleSuccess_c == Hrs_UpdateHrmCharacteristic(hValueHrMeasurement, heartRate, pHrsUserData))
        {
            Hrs_SendNotifications(hValueHrMeasurement);
        }
    }

    return result;
}

bleResult_t Hrs_SetContactStatus (uint16_t serviceHandle, bool_t isDetected)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_HrMeasurement_d);
    uint8_t flags = 0U;

    /* Get handle of Heart Rate Measurement characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result == gBleSuccess_c)
    {
        (void)Hrs_GetHrmFlags(handle, &flags);

        if (isDetected)
        {
            flags |= gHrs_SensorContactDetected_c;
        }
        else
        {
            flags &= (uint8_t)(~gHrs_SensorContactDetected_c);
        }

        result = Hrs_SetHrmFlags(handle, flags);
    }

    return result;
}

bleResult_t Hrs_RecordRRInterval (hrsUserData_t *pHrsUserData, uint16_t rrInterval)
{
    bleResult_t result = gBleSuccess_c;

#if gHrs_EnableRRIntervalMeasurements_d
    /* Store interval */
    if (pHrsUserData->pStoredRrIntervals == NULL)
    {
        result = gBleUnexpectedError_c;
    }
    else
    {
        pHrsUserData->pStoredRrIntervals[pHrsUserData->rrCursor] = rrInterval;

        /* Update cursor and count */
        pHrsUserData->rrCursor = (pHrsUserData->rrCursor + 1U) % gHrs_NumOfRRIntervalsRecorded_c;
        pHrsUserData->cRrIntervals = MIN(pHrsUserData->cRrIntervals + 1U, gHrs_NumOfRRIntervalsRecorded_c);
    }
#endif

    return result;
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
            retStatus = (uint8_t)gAttErrCodeNoError_c;
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
    uint8_t characteristic[Hrs_MaxNotificationSize] = {0};
    uint8_t index = 0;
    uint8_t iRRIntervalsCount;

    /* Add flags */
    (void)Hrs_GetHrmFlags(handle, &characteristic[0]);
    index++;

    /* Add HR Value*/
    if ((Hrs_GetHeartRateFormat(heartRate) & gHrs_16BitHeartRateFormat_c) != 0U)
    {
        characteristic[0] |= gHrs_16BitHeartRateFormat_c;
        FLib_MemCpy(&characteristic[index], &heartRate, sizeof(uint16_t));
        index++;
    }
    else
    {
        characteristic[0] &= (uint8_t)(~gHrs_16BitHeartRateFormat_c);
        characteristic[index] = (uint8_t) heartRate;
    }
    index++;

    /* Add EE */
    if ((characteristic[0] & gHrs_EnergyExpendedEnabled_c) != 0U)
    {
        FLib_MemCpy(&characteristic[index], &pHrsUserData->expendedEnergy, sizeof(uint16_t));
        index += sizeof(uint16_t);
    }

#if gHrs_EnableRRIntervalMeasurements_d

    /* Add RR Intervals */
    if (pHrsUserData->cRrIntervals > 0U)
    {
        iRRIntervalsCount = MIN(
            (Hrs_MaxNotificationSize - index) / sizeof(uint16_t),
            pHrsUserData->cRrIntervals);

        characteristic[0] |= gHrs_RrIntervalsEnabled_c;

        if (pHrsUserData->rrCursor + iRRIntervalsCount > gHrs_NumOfRRIntervalsRecorded_c)
        {
            FLib_MemCpy(&characteristic[index],
                &pHrsUserData->pStoredRrIntervals[pHrsUserData->rrCursor],
                ((uint32_t)gHrs_NumOfRRIntervalsRecorded_c - pHrsUserData->rrCursor)
                    * sizeof(uint16_t));

            index += (gHrs_NumOfRRIntervalsRecorded_c - pHrsUserData->rrCursor) * sizeof(uint16_t);

            FLib_MemCpy(&characteristic[index], &pHrsUserData->pStoredRrIntervals[0],
                ((uint32_t)iRRIntervalsCount + pHrsUserData->rrCursor
                    - gHrs_NumOfRRIntervalsRecorded_c) * sizeof(uint16_t));

            index += (iRRIntervalsCount + pHrsUserData->rrCursor
                    - gHrs_NumOfRRIntervalsRecorded_c) * sizeof(uint16_t);
        }
        else
        {
            FLib_MemCpy(&characteristic[index],
                &pHrsUserData->pStoredRrIntervals[0],
                (uint32_t)iRRIntervalsCount * sizeof(uint16_t));
            index += iRRIntervalsCount * sizeof(uint16_t);
        }

        /* Update circular buffer */
        pHrsUserData->rrCursor = (uint8_t)((pHrsUserData->rrCursor + iRRIntervalsCount)
            % gHrs_NumOfRRIntervalsRecorded_c);
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
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccdHrMeasurement) == gBleSuccess_c)
    {
        if (mHrs_SubscribedClientId != gInvalidDeviceId_c)
        {
            if (gBleSuccess_c == Gap_CheckNotificationStatus
                (mHrs_SubscribedClientId, hCccdHrMeasurement, &isNotifActive) &&
                TRUE == isNotifActive)
            {
                (void)GattServer_SendNotification(mHrs_SubscribedClientId, handle);
            }
        }
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
