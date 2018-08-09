/*! *********************************************************************************
* \defgroup Pulse Oximeter Service
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

#ifndef _PULSE_OXIMETER_INTERFACE_H_
#define _PULSE_OXIMETER_INTERFACE_H_

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

/*! Pulse Oximeter Service - Maximum number of stored measurements */
#ifndef gPlx_MaxNumOfStoredMeasurements_c
#define gPlx_MaxNumOfStoredMeasurements_c       30
#endif

/*! Pulse Oximeter Service - Default Init structures */
#define gPlx_DefaultSupportedFeatures           ( gPlx_MeasurementStatusSupported_c             |\
                                                gPlx_DeviceAndSensorStatusSupported_c           |\
                                                gPlx_MeasurementStorageSupported_c              |\
                                                gPlx_TimestampSupported_c                       |\
                                                gPlx_SpO2PRFastSupported_c                      |\
                                                gPlx_SpO2PRSlowSupported_c                      |\
                                                gPlx_PulseAmplitudeIndexSupported_c )
#define gPlx_DefaultMeasStatusSupport           ( gPlx_MeasurementOngoingSupported_c            |\
                                                gPlx_EarlyEstimatedDataSupported_c              |\
                                                gPlx_ValidatedDataSupported_c                   |\
                                                gPlx_FullyQualifiedDataSupported_c              |\
                                                gPlx_DataFromMeasurementsStorageSupported_c     |\
                                                gPlx_DataForDemonstrationSupported_c            |\
                                                gPlx_DataForTestingSupported_c                  |\
                                                gPlx_CalibrationOngoingSupported_c              |\
                                                gPlx_MeasurementUnavailableSupported_c          |\
                                                gPlx_QuestionableMeasurementDetectedSupported_c |\
                                                gPlx_InvalidMeasurementDetectedSupported_c )
#define gPlx_DefaultDevAndSensStatusSupport     ( gPlx_ExtDisplayUpdateOngoingSupported_c       |\
                                                gPlx_EquipmentMalfunctionDetSupported_c         |\
                                                gPlx_SignalProcIrregDetectedSupported_c         |\
                                                gPlx_InadequateSignalDetectedSupported_c        |\
                                                gPlx_PoorSignalDetectedSupported_c              |\
                                                gPlx_LowPerfusionDetectedSupported_c            |\
                                                gPlx_LowPerfusionDetectedSupported_c            |\
                                                gPlx_ErraticSignalDetectedSupported_c           |\
                                                gPlx_NonpulsatileSignalDetSupported_c           |\
                                                gPlx_QuestionablePulseDetSupported_c            |\
                                                gPlx_SignalAnalysisOngoingSupported_c           |\
                                                gPlx_SensorInterferenceDetSupported_c           |\
                                                gPlx_SensorUnconnectedSupported_c               |\
                                                gPlx_UnknownSensorConnectedSupported_c          |\
                                                gPlx_SensorDisplacedSupported_c                 |\
                                                gPlx_SensorMalfunctioningSupported_c            |\
                                                gPlx_SensorDisconnectedSupported_c )

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Pulse Oximeter Service - Feature Flags */
typedef uint16_t plxSupportedFeatures_t;

/*! Pulse Oximeter Service - Feature Flags */
typedef enum
{
    gPlx_NoFeature_c                            = 0x00,
    gPlx_MeasurementStatusSupported_c           = BIT0,
    gPlx_DeviceAndSensorStatusSupported_c       = BIT1,
    gPlx_MeasurementStorageSupported_c          = BIT2,
    gPlx_TimestampSupported_c                   = BIT3,
    gPlx_SpO2PRFastSupported_c                  = BIT4,
    gPlx_SpO2PRSlowSupported_c                  = BIT5,
    gPlx_PulseAmplitudeIndexSupported_c         = BIT6,
    gPlx_MultipleBondSupported_c                = BIT7
} plxSupportedFeatures_tag;

/*! Pulse Oximeter Service - Measurement */
typedef struct SpO2PRMeasurement_tag
{
    uint16_t SpO2;
    uint16_t PulseRate;
} SpO2PRMeasurement_t;

/*! Pulse Oximeter Service - Measurement Status Support */
typedef uint16_t plxMeasStatusSupport_t;

/*! Pulse Oximeter Service - Measurement Status Support */
typedef enum
{
    gPlx_NoSupport_c                                    = 0x00,
    gPlx_MeasurementOngoingSupported_c                  = BIT5,
    gPlx_EarlyEstimatedDataSupported_c                  = BIT6,
    gPlx_ValidatedDataSupported_c                       = BIT7,
    gPlx_FullyQualifiedDataSupported_c                  = BIT8,
    gPlx_DataFromMeasurementsStorageSupported_c         = BIT9,
    gPlx_DataForDemonstrationSupported_c                = BIT10,
    gPlx_DataForTestingSupported_c                      = BIT11,
    gPlx_CalibrationOngoingSupported_c                  = BIT12,
    gPlx_MeasurementUnavailableSupported_c              = BIT13,
    gPlx_QuestionableMeasurementDetectedSupported_c     = BIT14,
    gPlx_InvalidMeasurementDetectedSupported_c          = BIT15,
} plxMeasStatusSupport_tag;

/*! Pulse Oximeter Service - Device and Sensor Status Support */
typedef PACKED_STRUCT plxDevAndSensStatusSupport_tag
{
    uint16_t    flags;
    uint8_t     rfu;
}plxDevAndSensStatusSupport_t;

/*! Pulse Oximeter Service - Device and Sensor Status Support */
typedef enum
{
    gPlx_ExtDisplayUpdateOngoingSupported_c     = BIT0,
    gPlx_EquipmentMalfunctionDetSupported_c     = BIT1,
    gPlx_SignalProcIrregDetectedSupported_c     = BIT2,
    gPlx_InadequateSignalDetectedSupported_c    = BIT3,
    gPlx_PoorSignalDetectedSupported_c          = BIT4,
    gPlx_LowPerfusionDetectedSupported_c        = BIT5,
    gPlx_ErraticSignalDetectedSupported_c       = BIT6,
    gPlx_NonpulsatileSignalDetSupported_c       = BIT7,
    gPlx_QuestionablePulseDetSupported_c        = BIT8,
    gPlx_SignalAnalysisOngoingSupported_c       = BIT9,
    gPlx_SensorInterferenceDetSupported_c       = BIT10,
    gPlx_SensorUnconnectedSupported_c           = BIT11,
    gPlx_UnknownSensorConnectedSupported_c      = BIT12,
    gPlx_SensorDisplacedSupported_c             = BIT13,
    gPlx_SensorMalfunctioningSupported_c        = BIT14,
    gPlx_SensorDisconnectedSupported_c          = BIT15,
} plxDevAndSensStatusSupport_tag;

/*! Pulse Oximeter Service - Features */
typedef struct plxFeatures_tag
{
    plxSupportedFeatures_t              supportedFeatures;
    plxMeasStatusSupport_t              measStatusSupport;
    plxDevAndSensStatusSupport_t        devAndSensStatusSupport;
} plxFeatures_t;

/*! Pulse Oximeter Service - Spot-Check Measurement Flags */
typedef enum
{
    gPlx_SCTimestampPresent_c                   = BIT0,
    gPlx_SCMeasurementStatusPresent_c           = BIT1,
    gPlx_SCDeviceAndSensorStatusPresent_c       = BIT2,
    gPlx_SCPulseAmplitudeIndexPresent_c         = BIT3,
    gPlx_SCDeviceClockNotSet                    = BIT4
} plxSpotCheckMeasurementFlags_tag;

/*! Pulse Oximeter Service - Continuous Measurement Flags */
typedef uint8_t plxSpotCheckMeasurementFlags_t;

/*! Pulse Oximeter Service - Measurement Status */
typedef uint16_t plxMeasStatus_t;

/*! Pulse Oximeter Service - Measurement Status */
typedef enum
{
    gPlx_MeasurementOngoing                     = BIT5,
    gPlx_EarlyEstimatedData                     = BIT6,
    gPlx_ValidatedData                          = BIT7,
    gPlx_FullyQualifiedData                     = BIT8,
    gPlx_DataFromMeasurementsStorage            = BIT9,
    gPlx_DataForDemonstration                   = BIT10,
    gPlx_DataForTesting                         = BIT11,
    gPlx_CalibrationOngoing                     = BIT12,
    gPlx_MeasurementUnavailable                 = BIT13,
    gPlx_QuestionableMeasurementDetected        = BIT14,
    gPlx_InvalidMeasurementDetected             = BIT15,
} plxMeasStatus_tag;

/*! Pulse Oximeter Service - Device and Sensor Status */
typedef PACKED_STRUCT plxDeviceAndSensorStatus_tag
{
    uint16_t    flags;
    uint8_t     rfu;
} plxDeviceAndSensorStatus_t;

/*! Pulse Oximeter Service - Spot-Check Measurement */
typedef struct plxSpotCheckMeasurement_tag
{
    plxSpotCheckMeasurementFlags_t      flags;
    SpO2PRMeasurement_t                 SpO2PRNormal;
    ctsDateTime_t                       timeStamp;
    plxMeasStatus_t                     measurementStatus;
    plxDeviceAndSensorStatus_t          deviceAndSensorStatus;
    uint16_t                            pulseAmplitudeIndex;
    uint8_t                            seqNumber;
}plxSpotCheckMeasurement_t;

/*! Pulse Oximeter Service - User Data */
typedef struct plxUserData_tag
{
    plxSpotCheckMeasurement_t   *pStoredMeasurements;
    uint8_t                     cMeasurements;
    uint8_t                     measurementCursor;
    uint8_t                     lastSeqNumber;    
    uint8_t                     cReportedRecords;
}plxUserData_t;

typedef enum 
{
    gProcNotInProgress  = 0x00U,
    gProcInProgress,
    gProcSendingRecords,
    gProcWaitingForConfirm
} plxProcStatus_tag;
    
typedef uint8_t plxProcStatus_t;

/*! Pulse Oximeter Service - Configuration */
typedef struct plxConfig_tag
{
  uint16_t              serviceHandle;           
  plxFeatures_t         plxFeatures;
  plxUserData_t         *pUserData;
  plxProcStatus_t      procInProgress;
} plxConfig_t;

/*! Pulse Oximeter Service - Continuous Measurement Flags */
typedef enum
{
    gPlx_NoFlags                        = 0x00,
    gPlx_SpO2PRFastPresent_c            = BIT0,
    gPlx_SpO2PRSlowPresent_c            = BIT1,
    gPlx_MeasurementStatusPresent_c     = BIT2,
    gPlx_DeviceAndSensorStatusPresent_c = BIT3,
    gPlx_PulseAmplitudeIndexPresent_c   = BIT4
} plxContinuousMeasurementFlags_tag;

/*! Pulse Oximeter Service - Continuous Measurement Flags */
typedef uint8_t plxContinuousMeasurementFlags_t;

/*! Pulse Oximeter Service - Device and Sensor Status */
typedef enum
{
    gPlx_ExtDisplayUpdateOngoing        = BIT0,
    gPlx_EquipmentMalfunctionDetected   = BIT1,
    gPlx_SignalProcIrregDetected        = BIT2,
    gPlx_InadequateSignalDetected       = BIT3,
    gPlx_PoorSignalDetected             = BIT4,
    gPlx_LowPerfusionDetected           = BIT5,
    gPlx_ErraticSignalDetected          = BIT6,
    gPlx_NonpulsatileSignalDetected     = BIT7,
    gPlx_QuestionablePulseDetected      = BIT8,
    gPlx_SignalAnalysisOngoing          = BIT9,
    gPlx_SensorInterferenceDetected     = BIT10,
    gPlx_SensorUnconnected              = BIT11,
    gPlx_UnknownSensorConnected         = BIT12,
    gPlx_SensorDisplaced                = BIT13,
    gPlx_SensorMalfunctioning           = BIT14,
    gPlx_SensorDisconnected             = BIT15,
} plxDeviceAndSensorStatus_tag;

/*! Pulse Oximeter Service - Continuous Measurement */
typedef struct plxContinuousMeasurement_tag
{
    plxContinuousMeasurementFlags_t     flags;
    SpO2PRMeasurement_t                 SpO2PRNormal;
    SpO2PRMeasurement_t                 SpO2PRFast;
    SpO2PRMeasurement_t                 SpO2PRSlow;
    plxMeasStatus_t                     measurementStatus;
    plxDeviceAndSensorStatus_t          deviceAndSensorStatus;
    uint16_t                            pulseAmplitudeIndex;
}plxContinuousMeasurement_t;

/*! Pulse Oximeter Service - ATT Error Codes */
enum
{
    gAttErrCodePlxProcedureAlreadyInProgress_c = 0xFE,
    gAttErrCodePlxCccdImproperlyConfigured_c   = 0xFD
};

/*! Pulse Oximeter Service - Op Code */
typedef uint8_t plxOpCode_t;

/*! Pulse Oximeter Service - Op Code */
typedef enum
{
    gPlx_Reserved_c             = 0x00,
    gPlx_ReportStoredRecords_c,
    gPlx_DeleteStoredRecords_c,
    gPlx_AbortOperation_c,
    gPlx_ReportNumOfStoredRecords_c,
    gPlx_NumOfStoredRecordsRsp_c,
    gPlx_RspCode_c
} plxOpCode_tag;

typedef enum
{
    gPlx_Null_c             = 0x00,
    gPlx_AllRecords_c,
} plxOperator_tag;

/*! Pulse Oximeter Service - Operator */
typedef uint8_t plxOperator_t;

/*! Pulse Oximeter Service - Filter Type Value */
typedef enum
{
    gPlx_FtvReserved_c          = 0x00,
    gPlx_FtvSeqNumber_c,
    gPlx_FtvUserFacingTime_c
} plxFilterTypeValue_tag;

/*! Pulse Oximeter Service - Filter Type Value */
typedef uint8_t plxFilterTypeValue_t;

/*! Pulse Oximeter Service - Response Code */
typedef enum
{
    gPlx_RspReserved_c          = 0x00,
    gPlx_RspSuccess_c,
    gPlx_RspOpCodeNotSupported_c,
    gPlx_RspInvalidOperator_c,
    gPlx_RspOperatorNotSupported_c,
    gPlx_RspInvalidOperand_c,
    gPlx_RspNoRecordsFound_c,
    gPlx_RspAbortUnsuccessful_c,
    gPlx_RspProcNotCompleted_c,
    gPlx_RspOperandNotSupported_c
} plxRspCodeValue_tag;

/*! Pulse Oximeter Service - Response Code */
typedef uint8_t plxRspCodeValue_t;

/*! Pulse Oximeter Service - Response Code */
typedef PACKED_STRUCT plxResponseCode_tag{
    plxOpCode_t         reqOpCode;
    plxRspCodeValue_t   rspCodeValue;
}plxResponseCode_t;

typedef PACKED_STRUCT plxFilter_tag{
    plxFilterTypeValue_t    filter;
    uint16_t                filterParameter;
}plxFilter_t;

/*! Pulse Oximeter Service - RACP Procedure */
typedef PACKED_STRUCT plxProcedure_tag {
    plxOpCode_t     opCode;
    plxOperator_t   plxOperator;
    union {
        uint16_t                numberOfRecords;
        plxResponseCode_t       responseCode;
    } operand;
} plxProcedure_t;

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
* \brief        Starts Pulse Oximeter Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Plx_Start (plxConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Pulse Oximeter Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Plx_Stop (plxConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Pulse Oximeter service
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Plx_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Pulse Oximeter service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Plx_Unsubscribe(plxConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Records SpO2PR measurement on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    pMeasurement    Pointer to SpO2PR Measurement structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Plx_RecordContinuousMeasurement (uint16_t serviceHandle, plxContinuousMeasurement_t *pMeasurement);

/*!**********************************************************************************
* \brief        Records SpO2PR measurement on a specified service handle.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    pMeasurement    Pointer to SpO2PR Measurement structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Plx_RecordSpotCheckMeasurement (plxConfig_t *pServiceConfig, plxSpotCheckMeasurement_t *pMeasurement);

/*!**********************************************************************************
* \brief        Handles write operations on the Control point.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    pEvent          Pointer to the GATT Server event.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
void Plx_ControlPointHandler (plxConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

#ifdef __cplusplus
}
#endif 

#endif /* _PULSE_OXIMETER_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
