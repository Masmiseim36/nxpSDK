/*! *********************************************************************************
* \defgroup Blood Pressure Service
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

#ifndef _BLOOD_PRESSURE_INTERFACE_H_
#define _BLOOD_PRESSURE_INTERFACE_H_

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ieee11073.h"
#include "current_time_interface.h"
/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/*! Blood Pressure Service - Enable Stored Measurements */
#ifndef gBps_EnableStoredMeasurements_d
#define gBps_EnableStoredMeasurements_d TRUE
#endif

/*! Blood Pressure Service - Enable Multi-User Support */
#ifndef gBps_EnableMultiUserSupport_d
#define gBps_EnableMultiUserSupport_d TRUE
#endif

/*! Blood Pressure Service - Maximum number of user supported */
#ifndef gBps_MaxNumOfUsers_c
#define gBps_MaxNumOfUsers_c                    2
#endif

/*! Blood Pressure Service - Maximum number of stored measurements */
#ifndef gBps_MaxNumOfStoredMeasurements_c
#define gBps_MaxNumOfStoredMeasurements_c       2
#endif

/*! Blood Pressure Service - Unknown user */
#define gBps_UnknownUser_c      0xFF

/*! Blood Pressure Service - Default Init structure */
#define gBps_InitDefaultConfig      gBps_BodyMoveDetectionSupported_c          |\
                                    gBps_CuffFitDetectionSupported_c           |\
                                    gBps_IrregularPulseDetectionSupported_c    |\
                                    gBps_PulseRateRangeDetectionSupported_c    |\
                                    gBps_MeasurePositionDetectionSupported_c

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Blood Pressure Service - Flags */
typedef uint8_t bpsFlags_t;

/*! Blood Pressure Service - Blood Pressure Units */
typedef enum
{
    gBps_UnitInMmHg_c = 0x00,
    gBps_UnitInkPa_c = BIT0
} bpsUnits_t;

/*! Blood Pressure Service - Time Stamp */
typedef enum
{
    gBps_TimeStampNotPresent_c  = 0x00,
    gBps_TimeStampPresent_c     = BIT1,
} bpsFlagsTimeStamp_tag;

/*! Blood Pressure Service - Pulse Rate */
typedef enum
{
    gBps_PulseRateNotPresent_c  = 0x00,
    gBps_PulseRatePresent_c     = BIT2
} bpsFlagsPulseRate_tag;

/*! Blood Pressure Service - User ID */
typedef enum
{
    gBps_UserIdNotPresent_c  = 0x00,
    gBps_UserIdPresent_c     = BIT3
} bpsFlagsUserId_tag;

/*! Blood Pressure Service - Measurement Status */
typedef enum
{
    gBps_MeasurementStatusNotPresent_c  = 0x00,
    gBps_MeasurementStatusPresent_c     = BIT4
} bpsFlagsMeasurementStatus_tag;

/*! Blood Pressure Service - Measure Status Flags */
typedef uint16_t bpsMeasureStatusFlags_t;

/*! Blood Pressure Service - BodyMovement Detection */
typedef enum
{
    gBps_NoBodyMove_c               = 0x00,
    gBps_BodyMoveDuringDetection_c  = BIT0
} bpsBodyMoveDetection_t;

/*! Blood Pressure Service - Cuff Fit Detect */
typedef enum
{
    gBps_CuffFitsProperly_c = 0x00,
    gBps_CuffTooLoose_c     = BIT1
} bpsCuffFitDetect_t;

/*! Blood Pressure Service - Irregular Pulse Detection */
typedef enum
{
    gBps_NoIrregularPulse_c = 0x00,
    gBps_IrregularPulse_c   = BIT2
} bpsIrregularPulseDetect_t;

/*! Blood Pressure Service - Pulse Rate Range Detection */
typedef enum
{
    gBps_PulseRateWithinRange_c         = 0x00,
    gBps_PulseRateExceedsUpperLimit_c   = BIT3,
    gBps_PulseRateLessThanLowerLimit_c  = BIT3 | BIT4
} bpsIrregularPulseRateRange_t;

/*! Blood Pressure Service - Measurement Position Detection */
typedef enum
{
    gBps_ProperMeasurementPosition_c    = 0x00,
    gBps_ImproperMeasurementPosition_c  = BIT5
} bpsImproperMeasPos_t;

/*! Blood Pressure Service - Feature Flags */
typedef uint16_t bpsFeatureFlags_t;

/*! Blood Pressure Service - Feature Flags */
typedef enum
{
    gBps_NoFeature_c                            = 0x00,
    gBps_BodyMoveDetectionSupported_c           = BIT0,
    gBps_CuffFitDetectionSupported_c            = BIT1,
    gBps_IrregularPulseDetectionSupported_c     = BIT2,
    gBps_PulseRateRangeDetectionSupported_c     = BIT3,
    gBps_MeasurePositionDetectionSupported_c    = BIT4,
    gBps_MultipleBondSupported_c                = BIT5
} bpsFeatureFlags_tag;

/*! Blood Pressure Service - Measurement */
typedef struct bpsMeasurement_tag
{
    bpsUnits_t                  unit;
    bool_t                      timeStampPresent;
    bool_t                      pulseRatePresent;
    bool_t                      userIdPresent;
    bool_t                      measurementStatusPresent;
    ieee11073_16BitFloat_t      systolicValue;
    ieee11073_16BitFloat_t      diastolicValue;
    ieee11073_16BitFloat_t      meanArterialPressure;
    ctsDateTime_t               timeStamp;
    ieee11073_16BitFloat_t      pulseRate;
    uint8_t                     userId;
    bpsMeasureStatusFlags_t     measurementStatus;
}bpsMeasurement_t;

/*! Blood Pressure Service - Measurement */
typedef struct cuffPressureMeasurement_tag
{
    bpsUnits_t                  unit;
    bool_t                      userIdPresent;
    bool_t                      measurementStatusPresent;
    ieee11073_16BitFloat_t      cuffPressure;
    uint8_t                     userId;
    bpsMeasureStatusFlags_t     measurementStatus;
}cuffPressureMeasurement_t;
    
/*! Blood Pressure Service - User Data */
#if gBps_EnableMultiUserSupport_d || gBps_EnableStoredMeasurements_d
typedef struct bpsUserData_tag
{
#if gBps_EnableStoredMeasurements_d 
  bpsMeasurement_t      *pStoredMeasurements;  
  uint8_t               cMeasurements;
  uint8_t               measurementCursor;
#endif    
#if gBps_EnableMultiUserSupport_d
  uint8_t               userId;
#endif  
}bpsUserData_t;
#endif

/*! Blood Pressure Service - Configuration */
typedef struct bpsConfig_tag
{
  uint16_t              serviceHandle;           
  bpsFeatureFlags_t     bpsfeatureFlags;
#if gBps_EnableMultiUserSupport_d || gBps_EnableStoredMeasurements_d
  bpsUserData_t         *pUserData;
#endif
} bpsConfig_t;
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
* \brief        Starts Blood Pressure Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Bps_Start (bpsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Blood Pressure Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Bps_Stop (bpsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Blood Pressure service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Bps_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Blood Pressure service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Bps_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Records blood pressure measurement on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    pMeasurement    Pointer to Blood Pressure Measurement structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Bps_RecordBloodPressureMeasurement (uint16_t serviceHandle, bpsMeasurement_t *pMeasurement);

/*!**********************************************************************************
* \brief        Records cuff pressure on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    pMeasurement    Pointer to Cuff Pressure Measurement structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Bps_RecordCuffPressureMeasurement (uint16_t serviceHandle, cuffPressureMeasurement_t *pMeasurement);

/*!**********************************************************************************
* \brief        Sends stored measurements.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
************************************************************************************/
void Bps_SendStoredBloodPressureMeasurement(bpsConfig_t *pServiceConfig);

#ifdef __cplusplus
}
#endif 

#endif /* _BLOOD_PRESSURE_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
