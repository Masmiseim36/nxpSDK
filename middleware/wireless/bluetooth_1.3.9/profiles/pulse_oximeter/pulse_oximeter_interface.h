/*! *********************************************************************************
* \defgroup Pulse Oximeter Service
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

#ifndef PULSE_OXIMETER_INTERFACE_H
#define PULSE_OXIMETER_INTERFACE_H

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
#define gPlx_MaxNumOfStoredMeasurements_c       30U
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

/* plxRspCodeValue_t */
#define gPlx_RspReserved_c                          0x00U
#define gPlx_RspSuccess_c                           0x01U
#define gPlx_RspOpCodeNotSupported_c                0x02U
#define gPlx_RspInvalidOperator_c                   0x03U
#define gPlx_RspOperatorNotSupported_c              0x04U
#define gPlx_RspInvalidOperand_c                    0x05U
#define gPlx_RspNoRecordsFound_c                    0x06U
#define gPlx_RspAbortUnsuccessful_c                 0x07U
#define gPlx_RspProcNotCompleted_c                  0x08U
#define gPlx_RspOperandNotSupported_c               0x09U

/* plxOperator_t */
#define gPlx_Null_c                                 0x00U
#define gPlx_AllRecords_c                           0x01U

/*! plxOpCode_t */
#define gPlx_Reserved_c                             0x00U
#define gPlx_ReportStoredRecords_c                  0x01U
#define gPlx_DeleteStoredRecords_c                  0x02U
#define gPlx_AbortOperation_c                       0x03U
#define gPlx_ReportNumOfStoredRecords_c             0x04U
#define gPlx_NumOfStoredRecordsRsp_c                0x05U
#define gPlx_RspCode_c                              0x06U

/* plxProcStatus_t */
#define gProcNotInProgress                          0x00U
#define gProcInProgress                             0x01U
#define gProcSendingRecords                         0x02U
#define gProcWaitingForConfirm                      0x03U

/* plxSupportedFeatures_t */
#define gPlx_NoFeature_c                            0x00U
#define gPlx_MeasurementStatusSupported_c           BIT0
#define gPlx_DeviceAndSensorStatusSupported_c       BIT1
#define gPlx_MeasurementStorageSupported_c          BIT2
#define gPlx_TimestampSupported_c                   BIT3
#define gPlx_SpO2PRFastSupported_c                  BIT4
#define gPlx_SpO2PRSlowSupported_c                  BIT5
#define gPlx_PulseAmplitudeIndexSupported_c         BIT6
#define gPlx_MultipleBondSupported_c                BIT7

/* plxSpotCheckMeasurementFlags_t */
#define gPlx_SCTimestampPresent_c                   BIT0
#define gPlx_SCMeasurementStatusPresent_c           BIT1
#define gPlx_SCDeviceAndSensorStatusPresent_c       BIT2
#define gPlx_SCPulseAmplitudeIndexPresent_c         BIT3
#define gPlx_SCDeviceClockNotSet                    BIT4

/* plxContinuousMeasurementFlags_t */
#define gPlx_NoFlags                                0x00U
#define gPlx_SpO2PRFastPresent_c                    BIT0
#define gPlx_SpO2PRSlowPresent_c                    BIT1
#define gPlx_MeasurementStatusPresent_c             BIT2
#define gPlx_DeviceAndSensorStatusPresent_c         BIT3
#define gPlx_PulseAmplitudeIndexPresent_c           BIT4

/* plxDeviceAndSensorStatus_t */
#define gPlx_ExtDisplayUpdateOngoing                BIT0
#define gPlx_EquipmentMalfunctionDetected           BIT1
#define gPlx_SignalProcIrregDetected                BIT2
#define gPlx_InadequateSignalDetected               BIT3
#define gPlx_PoorSignalDetected                     BIT4
#define gPlx_LowPerfusionDetected                   BIT5
#define gPlx_ErraticSignalDetected                  BIT6
#define gPlx_NonpulsatileSignalDetected             BIT7
#define gPlx_QuestionablePulseDetected              BIT8
#define gPlx_SignalAnalysisOngoing                  BIT9
#define gPlx_SensorInterferenceDetected             BIT10
#define gPlx_SensorUnconnected                      BIT11
#define gPlx_UnknownSensorConnected                 BIT12
#define gPlx_SensorDisplaced                        BIT13
#define gPlx_SensorMalfunctioning                   BIT14
#define gPlx_SensorDisconnected                     BIT15

/* plxMeasStatus_t */
#define gPlx_MeasurementOngoing                     BIT5
#define gPlx_EarlyEstimatedData                     BIT6
#define gPlx_ValidatedData                          BIT7
#define gPlx_FullyQualifiedData                     BIT8
#define gPlx_DataFromMeasurementsStorage            BIT9
#define gPlx_DataForDemonstration                   BIT10
#define gPlx_DataForTesting                         BIT11
#define gPlx_CalibrationOngoing                     BIT12
#define gPlx_MeasurementUnavailable                 BIT13
#define gPlx_QuestionableMeasurementDetected        BIT14
#define gPlx_InvalidMeasurementDetected             BIT15

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Pulse Oximeter Service - Feature Flags */
typedef uint16_t plxSupportedFeatures_t;

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
} plxDevAndSensStatusFeaturesSupport_tag;

/*! Pulse Oximeter Service - Features */
typedef struct plxFeatures_tag
{
    plxSupportedFeatures_t              supportedFeatures;
    plxMeasStatusSupport_t              measStatusSupport;
    plxDevAndSensStatusSupport_t        devAndSensStatusSupport;
} plxFeatures_t;

/*! Pulse Oximeter Service - Continuous Measurement Flags */
typedef uint8_t plxSpotCheckMeasurementFlags_t;

/*! Pulse Oximeter Service - Measurement Status */
typedef uint16_t plxMeasStatus_t;

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
typedef uint8_t plxContinuousMeasurementFlags_t;

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
    PACKED_UNION {
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

#endif /* PULSE_OXIMETER_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
