/*! *********************************************************************************
* \defgroup Running Speed Cadence Service
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#ifndef _RUNNING_SPEED_CADENCE_INTERFACE_H_
#define _RUNNING_SPEED_CADENCE_INTERFACE_H_

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

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Running Speed Cadence Service - Measurement Flags */
typedef uint8_t rscsFlags_t;

/*! Running Speed Cadence Service - Instant Stride Length Present */
typedef enum
{
    gRscs_InstantStrideLengthNotPresent_c  = 0x00,
    gRscs_InstantStrideLengthPresent_c     = BIT0
} rscsFlagsInstantStrideLen_tag;

/*! Running Speed Cadence Service - Total Distance Present */
typedef enum
{
    gRscs_TotalDistanceNotPresent_c  = 0x00,
    gRscs_TotalDistancePresent_c     = BIT1
} rscsFlagsTotalDistance_tag;

/*! Running Speed Cadence Service - Walking/Running Status */
typedef enum
{
    gRscs_WalkingStatus_c   = 0x00,
    gRscs_RunningStatus_c   = BIT2
} rscsFlagsWalkOrRunStatus_tag;

/*! Running Speed Cadence Service - Measurement */
typedef struct rscsMeasurement_tag
{
    rscsFlags_t flags;
    uint16_t    instantSpeed;
    uint8_t     instantCadence;
    uint16_t    instantStrideLength;
    uint32_t    totalDistance;
}rscsMeasurement_t;

typedef uint16_t rscsFeature_t;

/*! Running Speed Cadence Service - Feature */
typedef enum
{
    gRscs_NoFeature_c                       = 0x00,
    gRscs_InstantStrideLengthSupported_c    = BIT0,
    gRscs_TotalDistanceSupported_c          = BIT1,
    gRscs_WalkingOrRunningSupported_c       = BIT2,
    gRscs_SensorCalibrationSupported_c      = BIT3,
    gRscs_MultipleSensorSupported_c         = BIT4
} rscsFeature_tag;

typedef uint8_t rscsSensorLocation_t;

/*! Running Speed Cadence Service - Feature */
typedef enum
{
    gRscs_Other_c           = 0x00,
    gRscs_TopOfShoe_c       = 0x01,
    gRscs_InShoe_c          = 0x02,
    gRscs_Hip_c             = 0x03,
    gRscs_FrontWheel_c      = 0x04,
    gRscs_LeftCrank_c       = 0x05,
    gRscs_RightCrank_c      = 0x06,
    gRscs_LeftPedal_c       = 0x07,
    gRscs_RightPedal_c      = 0x08,
    gRscs_FrontHub_c        = 0x09,
    gRscs_RearDropout_c     = 0x0A,
    gRscs_Chainstay_c       = 0x0B,
    gRscs_RearWheel_c       = 0x0C,
    gRscs_RearHub_c         = 0x0D,
    gRscs_Chest_c           = 0x0E
} rscsSensorLocation_tag;

typedef uint8_t rscsOpCode_t;

/*! Running Speed Cadence Service - SC Op Code */
typedef enum
{
    gRscs_SetCummulativeValue_c         = 0x01,
    gRscs_StartSensorCalibration_c      = 0x02,
    gRscs_UpdateSensorLocation_c        = 0x03,
    gRscs_ReqSupportedSensorLoc_c       = 0x04,
    gRscs_RspCode_c                     = 0x10
} rscsOpCode_tag;

typedef uint8_t rscsRspValue_t;

/*! Running Speed Cadence Service - Response Value */
typedef enum
{
    gRscs_Success_c             = 0x01,
    gRscs_OpCodeNotSupported_c  = 0x02,
    gRscs_InvalidParameter_c    = 0x03,
    gRscs_OperationFailed_c     = 0x04,
}rscsRspValue_tag;

typedef struct rscsResponse_tag{
    rscsOpCode_t    reqOpCode;
    rscsRspValue_t  rspValue;
}rscsResponse_t;

/*! Running Speed Cadence Service - SC Procedure */
typedef PACKED_STRUCT rscsProcedure_tag {
    rscsOpCode_t    opCode;
    union {
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
    gAttErrCodeProcedureAlreadyInProgress_c = 0x80,
    gAttErrCodeCccdImproperlyConfigured_c   = 0x81
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
bleResult_t Rscs_Unsubscribe();

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

#endif /* _RUNNING_SPEED_CADENCE_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
