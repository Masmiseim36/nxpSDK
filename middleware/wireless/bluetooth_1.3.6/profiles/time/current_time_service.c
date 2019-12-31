/*! *********************************************************************************
* \addtogroup BLE PROFILES
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/***********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"
#include "current_time_interface.h"

/***********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
/*! Current Time Service - Subscribed Client*/
static deviceId_t mCts_SubscribedClientId;

typedef union
{
    uint8_t* aValueTemp;
    ctsExactTime256_t* exactTimeTemp;
}exactTimeInfo_t;
/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
********************************************************************************** */
static uint8_t getMonthDays (unitMonth_t month, uint8_t isLeapYear);

/***********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
bleResult_t Cts_Start (ctsConfig_t *pServiceConfig)
{
    /* Record initial time and info */
    (void)Cts_RecordCurrentTime(pServiceConfig);
    (void)Cts_RecordLocalTimeInfo(pServiceConfig);
    (void)Cts_RecordReferenceTimeInfo(pServiceConfig);

    return gBleSuccess_c;
}

bleResult_t Cts_Stop (ctsConfig_t *pServiceConfig)
{
    mCts_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Cts_Subscribe(deviceId_t clientDeviceId)
{
    mCts_SubscribedClientId = clientDeviceId;
    return gBleSuccess_c;
}

bleResult_t Cts_Unsubscribe(void)
{
    mCts_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Cts_RecordCurrentTime (ctsConfig_t *pServiceConfig)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_CurrentTime_d);
    ctsMeasurement_t ctsMeasurement;

    /* Get handle of  characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result == gBleSuccess_c)
    {
        ctsMeasurement.exactTime.ctsDayDateTime = Cts_EpochToDayDateTime(pServiceConfig->localTime);
        ctsMeasurement.exactTime.fractions = 0U;
        ctsMeasurement.adjustReason = pServiceConfig->adjustReason;

        /* Update characteristic value and send notification */
        result = GattDb_WriteAttribute(handle, sizeof(ctsMeasurement_t), (uint8_t*)&ctsMeasurement);

        if (result == gBleSuccess_c)
        {
            if (ctsMeasurement.adjustReason != gCts_UnknownReason)
            {
                if(ctsMeasurement.adjustReason != gCts_ExternalRefUpdate)
                {
                    Cts_SendNotifications(handle);
                    pServiceConfig->referenceChanged = FALSE;
                }
                else
                {
                    if (((pServiceConfig->lastTime > pServiceConfig->localTime) && (pServiceConfig->lastTime - pServiceConfig->localTime > 60U)) ||
                        ((pServiceConfig->localTime > pServiceConfig->lastTime) && (pServiceConfig->localTime - pServiceConfig->lastTime > 60U)))
                    {
                        Cts_SendNotifications(handle);
                        pServiceConfig->referenceChanged = FALSE;
                    }
                    else
                    {
                        pServiceConfig->referenceChanged = TRUE;
                    }
                }
            }

            pServiceConfig->lastTime = pServiceConfig->localTime;
        }
    }

    return result;
}

bleResult_t Cts_RecordLocalTimeInfo (ctsConfig_t *pServiceConfig)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_LocalTimeInformation_d);

    /* Get handle of  characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value */
        result = GattDb_WriteAttribute(handle, sizeof(ctsLocalTimeInfo_t), (uint8_t*)&pServiceConfig->localTimeInfo);
    }

    return result;
}

bleResult_t Cts_RecordReferenceTimeInfo (ctsConfig_t *pServiceConfig)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_ReferenceTimeInformation_d);

    /* Get handle of  characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value */
        result = GattDb_WriteAttribute(handle, sizeof(ctsReferenceTimeInfo_t), (uint8_t*)&pServiceConfig->referenceTimeInfo);
    }

    return result;
}

void Cts_SendNotifications (uint16_t handle)
{
    uint16_t  handleCccd;
    bool_t isNotifActive;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &handleCccd) == gBleSuccess_c)
    {
        if (gBleSuccess_c == Gap_CheckNotificationStatus
            (mCts_SubscribedClientId, handleCccd, &isNotifActive) &&
            TRUE == isNotifActive)
        {
            (void)GattServer_SendNotification(mCts_SubscribedClientId, handle);
        }
    }
}

void Cts_CurrentTimeWrittenHandler (ctsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    uint8_t retStatus =  (uint8_t)gAttErrCodeNoError_c;
    uint8_t reason;
    ctsExactTime256_t exactTime;

    exactTimeInfo_t exactTimeInfo;
    exactTimeInfo.aValueTemp = pEvent->aValue;
    exactTime = *(exactTimeInfo.exactTimeTemp);

    reason = pEvent->aValue[sizeof(ctsExactTime256_t)];

    /* Test if reserved bits are trying to be written */
    if((reason & 0x80U) != 0U)
    {
        /* return error status - couldn't find the codes table for this profile */
        retStatus = 0x80;
    }
    else
    {
        pServiceConfig->localTime = Cts_DayDateTimeToEpochTime(exactTime.ctsDayDateTime);
    }

    (void)GattServer_SendAttributeWrittenStatus(mCts_SubscribedClientId, pEvent->handle, retStatus);
}

ctsDayDateTime_t Cts_EpochToDayDateTime (uint32_t epochTime)
{
    ctsDayDateTime_t tempTime;
    uint16_t dayOfYear;
    uint16_t dayOfMonth;
    uint8_t month;
    uint32_t year;

    tempTime.dateTime.seconds = (uint8_t)(epochTime % 60U);
    epochTime /= 60U;
    tempTime.dateTime.minutes = (uint8_t)(epochTime % 60U);
    epochTime /= 60U;
    tempTime.dateTime.hours = (uint8_t)(epochTime % 24U);
    epochTime /= 24U;
    year = (((epochTime * 4U) + 2U) / 1461U);
    tempTime.dateTime.year = (uint8_t)(year + 1970U);
    dayOfYear = (uint16_t)(epochTime - (((year * 1461U) + 1U) / 4U));

    dayOfMonth = (uint16_t)(dayOfYear + 1U);
    for(month = 1U; month<=12U; month++)
    {
        uint8_t monthDays = (uint8_t)getMonthDays(month, (((year+2U) % 4U > 0U) ? 0U : 1U));
        if(dayOfMonth > monthDays)
        {
            dayOfMonth -= monthDays;
        }
        else
        {
            break;
        }
    }

    tempTime.dateTime.month = month;
    tempTime.dateTime.day = (uint8_t)dayOfMonth;
    tempTime.dayOfWeek = (uint8_t)((dayOfYear + 4U) % 7U);

    return tempTime;
}

uint32_t Cts_DayDateTimeToEpochTime (ctsDayDateTime_t dayDateTime)
{
    uint32_t epochTime;
    uint32_t year;

    year = ((uint32_t)dayDateTime.dateTime.year  - 1970U);
    epochTime = ((year * 1461U) / 4U);

    for(uint8_t month = 1U; month<dayDateTime.dateTime.month; month++)
    {
        epochTime += getMonthDays(month, ((year+2U)%4U > 0U) ? 0U : 1U);
    }

    if(year % 4U > 2U)
    {
        epochTime++;
    }

    epochTime += (uint32_t)dayDateTime.dateTime.day - 1U;

    epochTime *= 24U;
    epochTime += dayDateTime.dateTime.hours;
    epochTime *= 60U;
    epochTime += dayDateTime.dateTime.minutes;
    epochTime *= 60U;
    epochTime += dayDateTime.dateTime.seconds;

    return epochTime;
}

/***********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
static uint8_t getMonthDays (unitMonth_t month, uint8_t isLeapYear)
{
    uint8_t result = 0;
    switch(month)
    {
        case gUnit_January_c: result = 31U; break;
        case gUnit_February_c:
        {
            if(isLeapYear > 0U)
            {
                result = 29U;
            }
            else
            {
                result = 28U;
            }
        }
        break;

        case gUnit_April_c:
        case gUnit_June_c:
        case gUnit_September_c:
        case gUnit_November_c:
            result = 30U;
            break;
        case gUnit_March_c:
        case gUnit_May_c:
        case gUnit_July_c:
        case gUnit_August_c:
        case gUnit_October_c:
        case gUnit_December_c:
            result = 31U;
            break;

        default: result = 0; break;
    }

    return result;
}

/*! *********************************************************************************
* @}
********************************************************************************** */
