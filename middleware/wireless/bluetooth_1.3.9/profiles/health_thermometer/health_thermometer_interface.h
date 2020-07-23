/*! *********************************************************************************
* \defgroup Health Thermometer Service
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

#ifndef HEALTH_THERMOMETER_INTERFACE_H
#define HEALTH_THERMOMETER_INTERFACE_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ieee11073.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/*! Health Temperature Service - Enable Stored Measurements */
#ifndef gHts_EnableStoredMeasurements_d
#define gHts_EnableStoredMeasurements_d 1U
#endif

/*! Health Temperature Service - Maximum number of stored measurements */
#ifndef gHts_MaxNumOfStoredMeasurements_c
#define gHts_MaxNumOfStoredMeasurements_c 1U
#endif

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
/* htsFlagsTimeStamp_t */
#define gHts_TimeStampNotPresent_c  0x00U
#define gHts_TimeStampPresent_c     BIT1

/* htsUnits_t */
#define gHts_UnitInCelsius_c        0x00U
#define gHts_UnitInFahrenheit_c     BIT0

/* htsFlagsTempType_t */
#define gHts_TempTypeNotPresent_c   0x00U
#define gHts_TempTypePresent_c      BIT2

/* htsTempType_t */
#define gHts_NoTempType_c           0x00U
#define gHts_Armpit_c               0x01U
#define gHts_Body_c                 0x02U
#define gHts_Ear_c                  0x03U
#define gHts_Finger_c               0x04U
#define gHts_GastroInt_c            0x05U
#define gHts_Mouth_c                0x06U
#define gHts_Rectum_c               0x07U
#define gHts_Toe_c                  0x08U
#define gHts_Tympanum_c             0x09U


/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Health Thermometer Service - Flags */
typedef uint8_t htsFlags_t;

/*! Health Thermometer Service - Health Thermometer Units */
typedef uint8_t htsUnits_t;

/*! Health Thermometer Service - Time Stamp Bit */
typedef uint8_t htsFlagsTimeStamp_t;

/*! Health Thermometer Service - Temperature Type Bit */
typedef uint8_t htsFlagsTempType_t;

/*! Health Thermometer Service - Temperature Type */
typedef uint8_t htsTempType_t;

/*! Health Thermometer Service - Measurement */
typedef struct htsMeasurement_tag
{
    ieee11073_32BitFloat_t      temperature;
    ctsDateTime_t               timeStamp;
    htsTempType_t               tempType;
    htsUnits_t                  unit;
    bool_t                      timeStampPresent;
    bool_t                      tempTypePresent;
}htsMeasurement_t;

/*! Health Thermometer Service - User Data */
typedef struct htsUserData_tag
{
    bool_t              measIntervalIndPending;
#if gHts_EnableStoredMeasurements_d
    htsMeasurement_t    *pStoredMeasurements;
    uint8_t             cMeasurements;
    uint8_t             measurementCursor;
#endif
} htsUserData_t;

/*! Health Thermometer Service - Configuration */
typedef struct htsConfig_tag
{
    uint16_t          serviceHandle;
    uint16_t          measurementInterval;
    htsTempType_t     tempType;
    htsUserData_t*    pUserData;
} htsConfig_t;

/*! Health Thermometer Service - Error Codes */
enum
{
    gAttAppErrCodeOutOfRange_c = 0x80
};
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
* \brief        Starts Health Thermometer Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hts_Start (htsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Health Thermometer Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hts_Stop (htsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Health Thermometer service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hts_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Health Thermometer service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hts_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Records temperature measurement on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    pMeasurement    Pointer to Temperature Measurement structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hts_RecordTemperatureMeasurement (uint16_t serviceHandle, htsMeasurement_t *pMeasurement);

/*!**********************************************************************************
* \brief        Records intermediate temperature on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    pMeasurement    Pointer to Temperature Measurement structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hts_RecordIntermediateTemperature (uint16_t serviceHandle, htsMeasurement_t *pMeasurement);

/*!**********************************************************************************
* \brief        Sets measurement interval on a specified service handle.
*
* \param[in]    pServiceConfig         Pointer to service structure
* \param[in]    measurementInterval    Measurement interval value
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hts_SetMeasurementInterval(htsConfig_t *pServiceConfig, uint16_t measurementInterval);

/*!**********************************************************************************
* \brief        Stores temperature measurement for transmitting later.
*
* \param[in]    pUserData              Pointer to service user specific data structure
* \param[in]    measurementInterval    Measurement interval value
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hts_StoreTemperatureMeasurement(htsUserData_t *pUserData, htsMeasurement_t *pMeasurement);

/*!**********************************************************************************
* \brief        Sends stored temperature measurements.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
************************************************************************************/
void Hts_SendStoredTemperatureMeasurement(htsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Checks if the new Measurement Interval value is in range
*
* \param[in]    pServiceConfig  		Pointer to structure that contains server
*                               		configuration information.
* \param[in]    handle                  Characteristic value handle.
* \param[in]    newMeasurementInterval  Proposed value for measurement interval.
*
* \return       gAttErrCodeNoError_c or error.
************************************************************************************/
attErrorCode_t Hts_MeasurementIntervalWriting(htsConfig_t *pServiceConfig, uint16_t handle, uint16_t newMeasurementInterval);

/*!**********************************************************************************
* \brief        Checks if the new Measurement Interval value is in range
*
* \param[in]    handle                  Characteristic value handle.
* \param[in]    newMeasurementInterval  Proposed value for measurement interval.
*
************************************************************************************/
void Hts_SendMeasurementIntervalIndication(uint16_t handle);

#ifdef __cplusplus
}
#endif 

#endif /* HEALTH_THERMOMETER_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
