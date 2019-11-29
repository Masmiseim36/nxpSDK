/*! *********************************************************************************
* \addtogroup BLE PROFILES
* @{
********************************************************************************** */
/*!
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _CURRENT_TIME_INTERFACE_H_
#define _CURRENT_TIME_INTERFACE_H_

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "gatt_interface.h"

/***********************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/
#define gCts_InitTime (1468917748U)
#define gCts_InitLocalTimeInfo {UTCp0200, 0U}
#define gCts_InitReferenceTimeInfo {gCtsManual, gCtsAccuracyUnknown, 0U, 0U}

/***********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef enum
{
    gUnit_MonthIsNotKnown_c = 0x00,
    gUnit_January_c,
    gUnit_February_c,
    gUnit_March_c,
    gUnit_April_c,
    gUnit_May_c,
    gUnit_June_c,
    gUnit_July_c,
    gUnit_August_c,
    gUnit_September_c,
    gUnit_October_c,
    gUnit_November_c,
    gUnit_December_c
}unitMonth_tag;

typedef uint8_t unitMonth_t;

typedef PACKED_STRUCT dateTime_tag
{
    uint16_t        year;
    unitMonth_t     month;
    uint8_t         day;
    uint8_t         hours;
    uint8_t         minutes;
    uint8_t         seconds;
} ctsDateTime_t;

typedef enum
{
    gCts_DayIsNotKnown_c = 0x00,
    gCts_Monday_c,
    gCts_Tuesday_c,
    gCts_Wednesday_c,
    gCts_Thursday_c,
    gCts_Friday_c,
    gCts_Saturday_c,
    gCts_Sunday_c
} ctsDayOfWeek_tag;

typedef uint8_t ctsDayOfWeek_t;

typedef struct ctsDayDateTime_tag
{
    ctsDateTime_t       dateTime;
    ctsDayOfWeek_t      dayOfWeek;
} ctsDayDateTime_t;

typedef uint8_t ctsFractions256_t;

typedef struct ctsExactTime256_tag
{
    ctsDayDateTime_t    ctsDayDateTime;
    ctsFractions256_t   fractions;
} ctsExactTime256_t;

typedef enum
{
    gCts_UnknownReason          = 0x00,
    gCts_ManualUpdate           = BIT0,
    gCts_ExternalRefUpdate      = BIT1,
    gCts_ChangeOfTimeZone       = BIT2,
    gCts_ChangeOfDST            = BIT3,
} ctsAdjustReason_tag;

typedef uint8_t ctsAdjustReason_t;

typedef PACKED_STRUCT ctsMeasurement_tag
{
    ctsExactTime256_t   exactTime;
    ctsAdjustReason_t   adjustReason;
} ctsMeasurement_t;

typedef enum
{
    UTCm1200 = -48,    UTCm1100 = -44,    UTCm1000 = -40,    UTCm0930 = -38,
    UTCm0900 = -36,    UTCm0800 = -32,    UTCm0700 = -28,    UTCm0600 = -24,
    UTCm0500 = -20,    UTCm0430 = -18,    UTCm0400 = -16,    UTCm0330 = -14,
    UTCm0300 = -12,    UTCm0200 = -8,     UTCm0100 = -4,     UTCp0000 = 00,
    UTCp0100 = 4,      UTCp0200 = 8,      UTCp0300 = 12,     UTCp0330 = 14,
    UTCp0400 = 16,     UTCp0430 = 18,     UTCp0500 = 20,     UTCp0530 = 22,
    UTCp0545 = 23,     UTCp0600 = 24,     UTCp0630 = 26,     UTCp0700 = 28,
    UTCp0800 = 32,     UTCp0845 = 35,     UTCp0900 = 36,     UTCp0930 = 38,
    UTCp1000 = 40,     UTCp1030 = 42,     UTCp1100 = 44,     UTCp1130 = 46,
    UTCp1200 = 48,     UTCp1245 = 51,     UTCp1300 = 52,     UTCp1400 = 56,
    gCtsOffsetNotKnown_c = -128
} ctsTimeZone_tag;

typedef uint8_t ctsTimeZone_t;

typedef enum
{
    gCtsStandardTime_c          = 0U,
    gCtsHalfHourDaylightTime_c  = BIT1,
    gCtsDaylightTime_c          = BIT2,
    gCtsDoubleDaylightTime_c    = BIT3,
    gCtsDSTNotKnown_c           = 255U
} ctsDSTOffset_tag;

typedef uint8_t ctsDSTOffset_t;

typedef PACKED_STRUCT ctsLocalTimeInfo_tag
{
    ctsTimeZone_t ctsTimeZone;
    ctsDSTOffset_t ctsDSTOffset;
} ctsLocalTimeInfo_t;

typedef enum
{
    gCtsUnknown          = 0U,
    gCtsNetworkTimeProto = 1U,
    gCtsGPS              = 2U,
    gCtsRadioTimeSignal  = 3U,
    gCtsManual           = 4U,
    gCtsAtomicClock      = 5U,
    gCtsCellularNetwork  = 6U
} ctsTimeSource_tag;

typedef uint8_t ctsTimeSource_t;

typedef enum
{
    gCtsAccuracyOutOfRange = 254U,
    gCtsAccuracyUnknown    = 255U
} ctsAccuracy_tag;

typedef uint8_t ctsAccuracy_t;

typedef struct ctsReferenceTimeInfo_tag
{
    ctsTimeSource_t     source;
    ctsAccuracy_t       accuracy;
    uint8_t             daysSinceUpdate;
    uint8_t             hoursSinceUpdate;
} ctsReferenceTimeInfo_t;

/*! Current Time Service - Configuration */
typedef struct ctsConfig_tag
{
    uint16_t                    serviceHandle;
    uint32_t    	        localTime;
    uint32_t    	        lastTime;
    ctsAdjustReason_t           adjustReason;
    ctsLocalTimeInfo_t          localTimeInfo;
    ctsReferenceTimeInfo_t      referenceTimeInfo;
    bool_t                      referenceChanged;
} ctsConfig_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*!**********************************************************************************
* \brief        Starts Current Time Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cts_Start(ctsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Current Time Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cts_Stop(ctsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Current Time Service
*
* \param[in]    pClient  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cts_Subscribe(deviceId_t clientDeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Current Time Service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cts_Unsubscribe();

/*!**********************************************************************************
* \brief        Records current time on a specified service handle.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cts_RecordCurrentTime (ctsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Records local time information on a specified service handle.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cts_RecordLocalTimeInfo (ctsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Records reference time information on a specified service handle.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cts_RecordReferenceTimeInfo (ctsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Handles write operations on a specified characteristic.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    pEvent          Pointer to the GATT Server event.
*
************************************************************************************/
void Cts_CurrentTimeWrittenHandler (ctsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

void Cts_SendNotifications(uint16_t handle);

/*!**********************************************************************************
* \brief        Handles write operations on a specified characteristic.
*
* \param[in]    epochTime       Time in UNIX format (seconds since 1970).
*
* \return       Time in ctsDayDateTime_t format.
************************************************************************************/
ctsDayDateTime_t Cts_EpochToDayDateTime (uint32_t epochTime);

/*!**********************************************************************************
* \brief        Handles write operations on a specified characteristic.
*
* \param[in]    dayDateTime     Time in ctsDayDateTime_t format.
*
* \return       Time in UNIX format (seconds since 1970).
************************************************************************************/
uint32_t Cts_DayDateTimeToEpochTime (ctsDayDateTime_t dayDateTime);

#ifdef __cplusplus
}
#endif 

#endif /* _CURRENT_TIME_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
