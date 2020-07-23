/*! *********************************************************************************
* \defgroup Running Speed Cadence Service
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
* 
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef RUNNING_SPEED_CADENCE_INTERFACE_H
#define RUNNING_SPEED_CADENCE_INTERFACE_H

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

/* rscsRspValue_t */
#define gRscs_Success_c                         0x01U
#define gRscs_OpCodeNotSupported_c              0x02U
#define gRscs_InvalidParameter_c                0x03U
#define gRscs_OperationFailed_c                 0x04U

/* rscsOpCode_t */
#define gRscs_SetCummulativeValue_c             0x01U
#define gRscs_StartSensorCalibration_c          0x02U
#define gRscs_UpdateSensorLocation_c            0x03U
#define gRscs_ReqSupportedSensorLoc_c           0x04U
#define gRscs_RspCode_c                         0x10U

/* rscsFeature_t */
#define gRscs_NoFeature_c                       0x00U
#define gRscs_InstantStrideLengthSupported_c    BIT0
#define gRscs_TotalDistanceSupported_c          BIT1
#define gRscs_WalkingOrRunningSupported_c       BIT2
#define gRscs_SensorCalibrationSupported_c      BIT3
#define gRscs_MultipleSensorSupported_c         BIT4

/* rscsFlagsTotalDistance_t */
#define gRscs_TotalDistanceNotPresent_c         0x00U
#define gRscs_TotalDistancePresent_c            BIT1

/* rscsFlagsInstantStrideLen_t */
#define gRscs_InstantStrideLengthNotPresent_c   0x00U
#define gRscs_InstantStrideLengthPresent_c      BIT0

/* rscsFlagsWalkOrRunStatus_t */
#define gRscs_WalkingStatus_c                   0x00U
#define gRscs_RunningStatus_c                   BIT2

/* rscsSensorLocation_t */
#define gRscs_Other_c                           0x00U
#define gRscs_TopOfShoe_c                       0x01U
#define gRscs_InShoe_c                          0x02U
#define gRscs_Hip_c                             0x03U
#define gRscs_FrontWheel_c                      0x04U
#define gRscs_LeftCrank_c                       0x05U
#define gRscs_RightCrank_c                      0x06U
#define gRscs_LeftPedal_c                       0x07U
#define gRscs_RightPedal_c                      0x08U
#define gRscs_FrontHub_c                        0x09U
#define gRscs_RearDropout_c                     0x0AU
#define gRscs_Chainstay_c                       0x0BU
#define gRscs_RearWheel_c                       0x0CU
#define gRscs_RearHub_c                         0x0DU
#define gRscs_Chest_c                           0x0EU

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Running Speed Cadence Service - Measurement Flags */
typedef uint8_t rscsFlags_t;

/*! Running Speed Cadence Service - Instant Stride Length Present */
typedef uint8_t rscsFlagsInstantStrideLen_t;

/*! Running Speed Cadence Service - Total Distance Present */
typedef uint8_t rscsFlagsTotalDistance_t;

/*! Running Speed Cadence Service - Walking/Running Status */
typedef uint8_t rscsFlagsWalkOrRunStatus_t;

/*! Running Speed Cadence Service - Measurement */
typedef struct rscsMeasurement_tag
{
    rscsFlags_t flags;
    uint16_t    instantSpeed;
    uint8_t     instantCadence;
    uint16_t    instantStrideLength;
    uint32_t    totalDistance;
}rscsMeasurement_t;

/*! Running Speed Cadence Service - Feature */
typedef uint16_t rscsFeature_t;

/*! Running Speed Cadence Service - Feature */
typedef uint8_t rscsSensorLocation_t;

/*! Running Speed Cadence Service - SC Op Code */
typedef uint8_t rscsOpCode_t;

typedef uint8_t rscsRspValue_t;

typedef PACKED_STRUCT rscsResponse_tag{
    rscsOpCode_t    reqOpCode;
    rscsRspValue_t  rspValue;
}rscsResponse_t;

/*! Running Speed Cadence Service - SC Procedure */
typedef PACKED_STRUCT rscsProcedure_tag {
    rscsOpCode_t    opCode;
    PACKED_UNION {
        /* gRscs_SetCummulativeValue_c */
        uint32_t                setCummulativeValue;
        /* gRscs_UpdateSensorCalibration_c */
        rscsSensorLocation_t    updateSensorLocation;
        /* gRscs_RspCode_c */
        rscsResponse_t          response;
    } procedureData;
} rscsProcedure_t;

/*! Running Speed Cadence - User Data */
typedef struct rscsUserData_tag
{
    uint32_t                totalDistance;
    rscsSensorLocation_t    *pSupportedSensorLocations;
    uint8_t                 cNumOfSupportedSensorLocations;
}rscsUserData_t;

/*! Running Speed Cadence Service - Configuration */
typedef struct rscsConfig_tag
{
  uint16_t              serviceHandle;           
  rscsFeature_t         rscsFeatures;
  rscsUserData_t        *pUserData;
  rscsSensorLocation_t  sensorLocation;
  bool_t                calibrationInProgress;
  bool_t                procInProgress;
} rscsConfig_t;

/*! Running Speed Cadence Service - ATT Error Codes */
enum
{
    gAttAppErrCodeProcedureAlreadyInProgress_c = 0x80,
    gAttAppErrCodeCccdImproperlyConfigured_c   = 0x81
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
* \brief        Starts Running Speed Cadence Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Rscs_Start (rscsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Running Speed Cadence Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Rscs_Stop (rscsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Running Speed Cadence service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Rscs_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Running Speed Cadence service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Rscs_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Records Running Speed Cadence measurement on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    pMeasurement    Pointer to Running Speed Cadence Measurement structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Rscs_RecordMeasurement(uint16_t serviceHandle, rscsMeasurement_t *pMeasurement);

/*!**********************************************************************************
* \brief        Sets the sensor location on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    sensorLocation  Sensor location value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Rscs_SetSensorLocation(uint16_t serviceHandle, rscsSensorLocation_t sensorLocation);

/*!**********************************************************************************
* \brief        Handles write operations on the SC Control point.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    pEvent          Pointer to the GATT Server event.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
void Rscs_ControlPointHandler (rscsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

/*!**********************************************************************************
* \brief        Signals the end of the sensor calibration procedure to the collector.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    wasSuccessful   TRUE if calibration was successful, FALSE otherwise.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Rscs_FinishSensorCalibration(rscsConfig_t *pServiceConfig, bool_t wasSuccessful);

#ifdef __cplusplus
}
#endif 

#endif /* RUNNING_SPEED_CADENCE_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
