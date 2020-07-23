/*! *********************************************************************************
* \defgroup Cycling Speed Cadence Service
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

#ifndef CYCLING_SPEED_CADENCE_INTERFACE_H
#define CYCLING_SPEED_CADENCE_INTERFACE_H

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

/* cscsRspValue_t */
#define gCscs_Success_c                   0x01U
#define gCscs_OpCodeNotSupported_c        0x02U
#define gCscs_InvalidParameter_c          0x03U
#define gCscs_OperationFailed_c           0x04U

/* cscsOpCode_t */
#define gCscs_SetCummulativeValue_c       0x01U
#define gCscs_StartSensorCalibration_c    0x02U
#define gCscs_UpdateSensorLocation_c      0x03U
#define gCscs_ReqSupportedSensorLoc_c     0x04U
#define gCscs_RspCode_c                   0x10U

/* cscsFlagsPulseRate_t */
#define gCscs_CrankRevDataNotPresent_c    0x00U
#define gCscs_CrankRevDataPresent_c       BIT1

/* cscsFlagsTimeStamp_t */
#define gCscs_WheelRevDataNotPresent_c    0x00U
#define gCscs_WheelRevDataPresent_c       BIT0

/* cscsFeature_t */
#define gCscs_NoFeature_c                           0x00U
#define gCscs_WheelRevDataSupported_c               BIT0
#define gCscs_CrankRevDataSupported_c               BIT1
#define gCscs_MultipleSensorLocationsSupported_c    BIT2

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Cycling Speed Cadence Service - Measurement Flags */
typedef uint8_t cscsFlags_t;

/*! Cycling Speed Cadence Service - Wheel Revolution Data Present */
typedef uint8_t cscsFlagsTimeStamp_t;

/*! Cycling Speed Cadence Service - Crank Revolution Data Present */
typedef uint8_t cscsFlagsPulseRate_t;

/*! Cycling Speed Cadence Service - Measurement */
typedef struct cscsMeasurement_tag
{
    uint32_t    cumulativeWheelRevs;
    uint16_t    lastWheelEventTime;
    uint16_t    cumulativeCrankRevs;
    uint16_t    lastCrankEventTime;
    bool_t      wheelRevDataPresent;
    bool_t      crankRevDataPresent;
}cscsMeasurement_t;

/*! Cycling Speed Cadence Service - Feature */
typedef uint16_t cscsFeature_t;

typedef uint8_t cscsSensorLocation_t;

/*! Cycling Speed Cadence Service - Feature */
typedef enum
{
    gCscs_Other_c           = 0x00,
    gCscs_TopOfShoe_c       = 0x01,
    gCscs_InShoe_c          = 0x02,
    gCscs_Hip_c             = 0x03,
    gCscs_FrontWheel_c      = 0x04,
    gCscs_LeftCrank_c       = 0x05,
    gCscs_RightCrank_c      = 0x06,
    gCscs_LeftPedal_c       = 0x07,
    gCscs_RightPedal_c      = 0x08,
    gCscs_FrontHub_c        = 0x09,
    gCscs_RearDropout_c     = 0x0A,
    gCscs_Chainstay_c       = 0x0B,
    gCscs_RearWheel_c       = 0x0C,
    gCscs_RearHub_c         = 0x0D,
    gCscs_Chest_c           = 0x0E
} cscsSensorLocation_tag;

/*! Cycling Speed Cadence Service - SC Op Code */
typedef uint8_t cscsOpCode_t;

/*! Cycling Speed Cadence Service - Response Value */
typedef uint8_t cscsRspValue_t;

typedef PACKED_STRUCT cscsResponse_tag{
    cscsOpCode_t    reqOpCode;
    cscsRspValue_t  rspValue;
}cscsResponse_t;

/*! Cycling Speed Cadence Service - SC Procedure */
typedef PACKED_STRUCT cscsProcedure_tag {
    cscsOpCode_t    opCode;
    PACKED_UNION {
        /* gCscs_SetCummulativeValue_c */
        uint32_t                setCummulativeValue;
        /* gCscs_UpdateSensorCalibration_c */
        cscsSensorLocation_t    updateSensorLocation;
        /* gCscs_RspCode_c */
        cscsResponse_t          response;
    } procedureData;
} cscsProcedure_t;

/*! Cycling Speed Cadence - User Data */
typedef struct cscsUserData_tag
{
    uint32_t                cumulativeWheelRevs;
    cscsSensorLocation_t    *pSupportedSensorLocations;
    uint16_t                cumulativeCrankRevs;
    uint8_t                 cNumOfSupportedSensorLocations;
}cscsUserData_t;

/*! Cycling Speed Cadence Service - Configuration */
typedef struct cscsConfig_tag
{
  uint16_t              serviceHandle;           
  cscsFeature_t         cscsFeatures;
  cscsUserData_t        *pUserData;
  cscsSensorLocation_t  sensorLocation;
  bool_t                procInProgress;
  bool_t                calibrationInProgress;
} cscsConfig_t;

/*! Heart Rate Service - ATT Error Codes */
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
* \brief        Starts Cycling Speed Cadence Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cscs_Start (cscsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Cycling Speed Cadence Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cscs_Stop (cscsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Cycling Speed Cadence service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cscs_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Cycling Speed Cadence service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cscs_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Records Cycling Speed Cadence measurement on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    pMeasurement    Pointer to Cycling Speed Cadence Measurement structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cscs_RecordMeasurement(uint16_t serviceHandle, cscsMeasurement_t *pMeasurement);

/*!**********************************************************************************
* \brief        Sets the sensor location on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    sensorLocation  Sensor location value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cscs_SetSensorLocation(uint16_t serviceHandle, cscsSensorLocation_t sensorLocation);

/*!**********************************************************************************
* \brief        Handles write operations on the SC Control point.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    pEvent          Pointer to the GATT Server event.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
void Cscs_ControlPointHandler (cscsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

/*!**********************************************************************************
* \brief        Handles write operations on the SC Control point.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    wasSuccessful   TRUE if calibration was successful, FALSE otherwise.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cscs_FinishSensorCalibration(cscsConfig_t *pServiceConfig, bool_t wasSuccessful);

#ifdef __cplusplus
}
#endif 

#endif /* CYCLING_SPEED_CADENCE_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
