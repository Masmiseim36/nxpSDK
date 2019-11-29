/*! *********************************************************************************
* \defgroup Health Thermometer Service
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

#ifndef _HEALTH_THERMOMETER_INTERFACE_H_
#define _HEALTH_THERMOMETER_INTERFACE_H_

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
#define gHts_EnableStoredMeasurements_d TRUE
#endif

/*! Health Temperature Service - Maximum number of stored measurements */
#ifndef gHts_MaxNumOfStoredMeasurements_c
#define gHts_MaxNumOfStoredMeasurements_c TRUE
#endif

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Health Thermometer Service - Flags */
typedef uint8_t htsFlags_t;

/*! Health Thermometer Service - Health Thermometer Units */
typedef enum
{
    gHts_UnitInCelsius_c = 0x00,
    gHts_UnitInFahrenheit_c = BIT0
} htsUnits_t;

/*! Health Thermometer Service - Time Stamp Bit */
typedef enum
{
    gHts_TimeStampNotPresent_c  = 0x00,
    gHts_TimeStampPresent_c     = BIT1,
} htsFlagsTimeStamp_tag;

/*! Health Thermometer Service - Temperature Type Bit */
typedef enum
{
    gHts_TempTypeNotPresent_c  = 0x00,
    gHts_TempTypePresent_c     = BIT2
} htsFlagsTempType_tag;

typedef uint8_t htsTempType_t;

/*! Health Thermometer Service - Temperature Type */
typedef enum
{
    gHts_NoTempType_c   = 0x00,
    gHts_Armpit_c       = 0x01,
    gHts_Body_c         = 0x02,
    gHts_Ear_c          = 0x03,
    gHts_Finger_c       = 0x04,
    gHts_GastroInt_c    = 0x05,
    gHts_Mouth_c        = 0x06,
    gHts_Rectum_c       = 0x07,
    gHts_Toe_c          = 0x08,
    gHts_Tympanum_c     = 0x09
} htsTempType_tag;

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
    gAttErrCodeOutOfRange_c = 0x80
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
bleResult_t Hts_Unsubscribe();

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

#endif /* _HEALTH_THERMOMETER_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
