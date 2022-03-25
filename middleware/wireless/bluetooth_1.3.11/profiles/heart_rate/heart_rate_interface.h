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
* This file is the interface file for the Heart Rate Service
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef HEART_RATE_INTERFACE_H
#define HEART_RATE_INTERFACE_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/*! Heart Rate Service - Enable RR Interval collection */
#ifndef	gHrs_EnableRRIntervalMeasurements_d
#define gHrs_EnableRRIntervalMeasurements_d     TRUE
#endif

/*! Heart Rate Service - Number of RR Interval values recorded */
#ifndef	gHrs_NumOfRRIntervalsRecorded_c
#define gHrs_NumOfRRIntervalsRecorded_c                  0x02U
#endif

/*! Heart Rate Service - Control Point Reset Energy Expended */
#define gHrs_CpResetEnergyExpended_c                    0x01U

/* hrsFlagsHeartRateFormat_t */
#define gHrs_8BitHeartRateFormat_c                      0x00U
#define gHrs_16BitHeartRateFormat_c                     BIT0

/* hrsFlagsSensorContactDetected_t */
#define gHrs_SensorContactNotDetected_c                 0x00U
#define gHrs_SensorContactDetected_c                    BIT1

/* hrsFlagsSensorContactSupported_t */
#define gHrs_SensorContactNotSupported_c                0x00U
#define gHrs_SensorContactSupported_c                   BIT2

/* hrsFlagsEnergyExpended_t */
#define gHrs_EnergyExpendedDisabled_c                   0x00U
#define gHrs_EnergyExpendedEnabled_c                    BIT3

/* hrsFlagsRRIntervals_t */
#define gHrs_RrIntervalsDisabled_c                      0x00U
#define gHrs_RrIntervalsEnabled_c                       BIT4

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
/*! Heart Rate Service - Heart Rate Format */
typedef uint8_t hrsFlagsHeartRateFormat_t;

/*! Heart Rate Service - Sensor Contact detected status */
typedef uint8_t hrsFlagsSensorContactDetected_t;

/*! Heart Rate Service - Sensor Contact supported status */
typedef uint8_t hrsFlagsSensorContactSupported_t;

/*! Heart Rate Service - Energy expended status */
typedef uint8_t hrsFlagsEnergyExpended_t;

/*! Heart Rate Service - RR Intervals status */
typedef uint8_t hrsFlagsRRIntervals_t;

typedef uint8_t hrsBodySensorLoc_t;

/*! Heart Rate Service - Body Sensor Location */
typedef enum
{
    gHrs_BodySensorLocOther_c = 0x00,
    gHrs_BodySensorLocChest_c = 0x01,
    gHrs_BodySensorLocWrist_c = 0x02,
    gHrs_BodySensorLocFinger_c = 0x03,
    gHrs_BodySensorLocHand_c = 0x04,
    gHrs_BodySensorLocEarLobe_c = 0x05,
    gHrs_BodySensorLocFoot_c = 0x06
} hrsBodySensorLoc_tag;

/*! Heart Rate Service - User Data */
typedef struct hrsUserData_tag
{
#if gHrs_EnableRRIntervalMeasurements_d
  /*! Heart Rate Service - RR Interval Measurements Buffer*/
  uint16_t *pStoredRrIntervals;

  /*! Heart Rate Service - RR Interval Count*/
  uint8_t   cRrIntervals;

  /*! Heart Rate Service - Current Position in RR Interval Measurements Vector*/
  uint8_t   rrCursor;
#endif
  /*! Heart Rate Service - Expended Energy Values */
  uint16_t  expendedEnergy;
}hrsUserData_t;


/*! Heart Rate Service - Configuration */
typedef struct hrsConfig_tag
{
    uint16_t             serviceHandle;
    bool_t               sensorContactSupported;
    bool_t               sensorContactDetected;
    bool_t               energyExpandedEnabled;
    hrsBodySensorLoc_t   bodySensorLocation;
    hrsUserData_t        *pUserData;
} hrsConfig_t;

/*! Heart Rate Service - Error Codes */
enum
{
    gAttErrCodeCtrlPointValueNotSupported_c = 0x80
};


/*! Heart Rate Client - Configuration */
typedef struct hrcConfig_tag
{
    deviceId_t           serverDeviceId;
    uint16_t             hService;
    uint16_t             hHeartRateMeasurementChar;
    uint16_t             hHeartRateMeasurementCccd;
    uint16_t             hSensorLocation;
    uint16_t             hHeartRateControlPoint;
} hrcConfig_t;

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
* \brief        Starts Heart Rate Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hrs_Start(hrsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Heart Rate Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hrs_Stop(hrsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Heart Rate service
*
* \param[in]    clientDeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hrs_Subscribe(deviceId_t clientDeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Heart Rate service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hrs_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Handles command on the Heart Rate Control Point
*
* \param[in]    pHrsUserData    Pointer to user data information structure.
* \param[in]    value           Command Value.
*
* \return       gAttErrCodeNoError_c or error.
*************************************************************************************/
uint8_t Hrs_ControlPointHandler (hrsUserData_t *pHrsUserData, uint8_t value);

/*!**********************************************************************************
* \brief        Records heart rate measurement on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    value           Heart Rate Value.
* \param[in]    pHrsUserData    Pointer to user data information structure.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hrs_RecordHeartRateMeasurement (uint16_t serviceHandle, uint16_t heartRate, hrsUserData_t *pHrsUserData);

/*!**********************************************************************************
* \brief        Sets the contact status on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    isDetected      True if contact detected, False otherwise.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hrs_SetContactStatus (uint16_t serviceHandle, bool_t isDetected);

/*!**********************************************************************************
* \brief        Records RR Intervals.
*
* \param[in]    pHrsUserData    Pointer to user data information structure.
* \param[in]    rrInterval      RR Interval value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hrs_RecordRRInterval (hrsUserData_t *pHrsUserData, uint16_t rrInterval);

/*!**********************************************************************************
* \brief        Resets Expended Energy value.
*
* \param[in]    pHrsUserData    Pointer to user data information structure.
*
************************************************************************************/
void Hrs_ResetExpendedEnergy (hrsUserData_t *pHrsUserData);

/*!**********************************************************************************
* \brief        Adds Expended Energy to total value.
*
* \param[in]    pHrsUserData    Pointer to user data information structure.
* \param[in]    energy      Energy value.
*
************************************************************************************/
void Hrs_AddExpendedEnergy (hrsUserData_t *pHrsUserData, uint16_t energy);

#ifdef __cplusplus
}
#endif

#endif /* HEART_RATE_INTERFACE_H */

/*! **********************************************************************************
 * @}
 ************************************************************************************/
