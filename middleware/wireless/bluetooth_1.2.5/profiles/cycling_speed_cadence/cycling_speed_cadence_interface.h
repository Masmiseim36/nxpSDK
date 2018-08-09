/*! *********************************************************************************
* \defgroup Cycling Speed Cadence Service
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
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

#ifndef _CYCLING_SPEED_CADENCE_INTERFACE_H_
#define _CYCLING_SPEED_CADENCE_INTERFACE_H_

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

/*! Cycling Speed Cadence Service - Measurement Flags */
typedef uint8_t cscsFlags_t;

/*! Cycling Speed Cadence Service - Wheel Revolution Data Present */
typedef enum
{
    gCscs_WheelRevDataNotPresent_c  = 0x00,
    gCscs_WheelRevDataPresent_c     = BIT0,
} cscsFlagsTimeStamp_tag;

/*! Cycling Speed Cadence Service - Crank Revolution Data Present */
typedef enum
{
    gCscs_CrankRevDataNotPresent_c  = 0x00,
    gCscs_CrankRevDataPresent_c     = BIT1
} cscsFlagsPulseRate_tag;

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

typedef uint16_t cscsFeature_t;

/*! Cycling Speed Cadence Service - Feature */
typedef enum
{
    gCscs_NoFeature_c                           = 0x00,
    gCscs_WheelRevDataSupported_c               = BIT0,
    gCscs_CrankRevDataSupported_c               = BIT1,
    gCscs_MultipleSensorLocationsSupported_c    = BIT2
} cscsFeature_tag;

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

typedef uint8_t cscsOpCode_t;

/*! Cycling Speed Cadence Service - SC Op Code */
typedef enum
{
    gCscs_SetCummulativeValue_c         = 0x01,
    gCscs_StartSensorCalibration_c      = 0x02,
    gCscs_UpdateSensorLocation_c     = 0x03,
    gCscs_ReqSupportedSensorLoc_c       = 0x04,
    gCscs_RspCode_c                     = 0x10
} cscsOpCode_tag;

typedef uint8_t cscsRspValue_t;

/*! Cycling Speed Cadence Service - Response Value */
typedef enum
{
    gCscs_Success_c             = 0x01,
    gCscs_OpCodeNotSupported_c  = 0x02,
    gCscs_InvalidParameter_c    = 0x03,
    gCscs_OperationFailed_c     = 0x04,
}cscsRspValue_tag;

typedef struct cscsResponse_tag{
    cscsOpCode_t    reqOpCode;
    cscsRspValue_t  rspValue;
}cscsResponse_t;

/*! Cycling Speed Cadence Service - SC Procedure */
typedef PACKED_STRUCT cscsProcedure_tag {
    cscsOpCode_t    opCode;
    union {
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
bleResult_t Cscs_Unsubscribe();

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

#endif /* _CYCLING_SPEED_CADENCE_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
