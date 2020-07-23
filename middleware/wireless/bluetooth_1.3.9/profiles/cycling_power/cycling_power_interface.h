/*! *********************************************************************************
* \defgroup Cycling Power Service
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

#ifndef CYCLING_POWER_INTERFACE_H
#define CYCLING_POWER_INTERFACE_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "current_time_interface.h"
/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/*! Cycling Power Service - Enable Measurement Broadcast */
#ifndef gCps_EnableMeasurementBroadcast_d
#define gCps_EnableMeasurementBroadcast_d FALSE
#endif

/*! Cycling Power Service - Default Factory Calibration Date */
#ifndef gCps_FactoryCalibrationDate_c
#define gCps_FactoryCalibrationDate_c   2015, 6, 5, 0, 0, 0
#endif

/*! Cycling Power Service - Default Sampling Rate */
#ifndef gCps_SamplingRate_c
#define gCps_SamplingRate_c             12
#endif

/*! Cycling Power Service - Default Crank Length - 172.5 mm */
#ifndef gCps_CrankLength_c
#define gCps_CrankLength_c             345      
#endif

/*! Cycling Power Service - Default Chain Length - in mm */
#ifndef gCps_ChainLength_c
#define gCps_ChainLength_c             1473
#endif

/*! Cycling Power Service - Default Chain Weight - in grams */
#ifndef gCps_ChainWeight_c
#define gCps_ChainWeight_c             304
#endif

/*! Cycling Power Service - Default Span Length - in mm */
#ifndef gCps_SpanLength_c
#define gCps_SpanLength_c               420
#endif

/*! Cycling Power Service - Send Server Characteristic Config Descriptor Status */
#ifndef gCyclPwrs_SendSccdWritenStatus_d
#define gCyclPwrs_SendSccdWritenStatus_d FALSE
#endif

/*! Cycling Power Service - All Power Features Macro */
#define Cps_AllFeatures     (gCps_PedalPowerBalanceSupported_c | gCps_AccumulatedTorqueSupported_c |\
                            gCps_WheelRevolutionDataSupported_c | gCps_CrankRevolutionDataSupported_c |\
                            gCps_ExtremeMagnitudesSupported_c | gCps_ExtremeAnglesSupported_c |\
                            gCps_TopBottomDeadSpotAnglesSupported_c | gCps_AccumulatedEnergySupported_c |\
                            gCps_OffsetCompensationIndicatorSupported_c | gCps_OffsetCompensatioSupported_c |\
                            gCps_CharContentMaskingSupported_c | gCps_MultipleSensorLocationsSupported_c |\
                            gCps_CrankLengthAdjustmentSupported_c | gCps_ChainLengthAdjustmentSupported_c |\
                            gCps_ChainWeightAdjustmentSupported_c | gCps_SpanLengthAdjustmentSupported_c |\
                            gCps_SensorMeasurementContextSupported_c | gCps_InstantMeasurementDirectionSupported_c |\
                            gCps_FactoryCalibrationDateSupported_c)

/* cpsOpCode_t */
#define gCps_SetCummulativeValue_c          0x01U
#define gCps_UpdateSensorLocation_c         0x02U
#define gCps_ReqSupportedSensorLoc_c        0x03U
#define gCps_SetCrankLength_c               0x04U
#define gCps_RequestCrankLength_c           0x05U
#define gCps_SetChainLength_c               0x06U
#define gCps_RequestChainLength_c           0x07U
#define gCps_SetChainWeight_c               0x08U
#define gCps_RequestChainWeight_c           0x09U
#define gCps_SetSpanLength_c                0x0AU
#define gCps_RequestSpanLength_c            0x0BU
#define gCps_StartOffsetCompensation_c      0x0CU
#define gCps_MaskCpsContent_c               0x0DU
#define gCps_ReqSamplingRate_c              0x0EU
#define gCps_ReqFactoryCalibrationDate_c    0x0FU
#define gCps_RspCode_c                      0x20U

/* cpsRspValue_t */
#define gCps_Success_c                      0x01U
#define gCps_OpCodeNotSupported_c           0x02U
#define gCps_InvalidParameter_c             0x03U
#define gCps_OperationFailed_c              0x04U

/* cpsMaskContent_t */
#define gCps_LeaveAsDefault_c               0x0000U
#define gCps_TurnOffPedalPowerBalance_c     BIT0
#define gCps_TurnOffAccumulatedTorque_c     BIT1
#define gCps_TurnOffWheelRevData_c          BIT2
#define gCps_TurnOffCrankRevData_c          BIT3
#define gCps_TurnOffExtremeMagnitudes_c     BIT4
#define gCps_TurnOffExtremeAngles_c         BIT5
#define gCps_TurnOffTopDeadSpotAngle_c      BIT6
#define gCps_TurnOffBottomDeadSpotAngle_c   BIT7
#define gCps_TurnOffAccumulatedEnergy_c     BIT8
#define gCps_MaskAll_c                      0x01FFU
#define gCps_Reserved_c                     0xFE00U

/* cpsFeature_t */
#define gCps_NoFeature_c                            0x00U
#define gCps_PedalPowerBalanceSupported_c           BIT0
#define gCps_AccumulatedTorqueSupported_c           BIT1
#define gCps_WheelRevolutionDataSupported_c         BIT2
#define gCps_CrankRevolutionDataSupported_c         BIT3
#define gCps_ExtremeMagnitudesSupported_c           BIT4
#define gCps_ExtremeAnglesSupported_c               BIT5
#define gCps_TopBottomDeadSpotAnglesSupported_c     BIT6
#define gCps_AccumulatedEnergySupported_c           BIT7
#define gCps_OffsetCompensationIndicatorSupported_c BIT8
#define gCps_OffsetCompensatioSupported_c           BIT9
#define gCps_CharContentMaskingSupported_c          BIT10
#define gCps_MultipleSensorLocationsSupported_c     BIT11
#define gCps_CrankLengthAdjustmentSupported_c       BIT12
#define gCps_ChainLengthAdjustmentSupported_c       BIT13
#define gCps_ChainWeightAdjustmentSupported_c       BIT14
#define gCps_SpanLengthAdjustmentSupported_c        BIT15
#define gCps_SensorMeasurementContextSupported_c    BIT16
#define gCps_InstantMeasurementDirectionSupported_c BIT17
#define gCps_FactoryCalibrationDateSupported_c      BIT18

/* cpsPowerVectorFlags_t */
#define gCps_VectorNoFlags_c                        0x00U
#define gCps_VectorCrankRevDataPresent_c            BIT0
#define gCps_VectorFirstCrankMeasAnglePresent_c     BIT1
#define gCps_VectorInstantForceMagArrayPresent_c    BIT2
#define gCps_VectorInstantTorqueMagArrayPresent_c   BIT3

/* cpsMeasurementFlags_t */
#define gCps_NoFlags_c                              0x00U
#define gCps_PedalPowerBalancePresent_c             BIT0
#define gCps_AccumulatedTorquePresent_c             BIT2
#define gCps_WheelRevolutionDataPresent_c           BIT4
#define gCps_CrankRevolutionDataPresent_c           BIT5
#define gCps_ExtremeForceMagnitudesPresent_c        BIT6
#define gCps_ExtremeTorqueMagnitudesPresent_c       BIT7
#define gCps_ExtremeAnglesPresent_c                 BIT8
#define gCps_TopDeadSpotAnglesPresent_c             BIT9
#define gCps_BotomDeadSpotAnglesPresent_c           BIT10
#define gCps_AccumulatedEnergyPresent_c             BIT11
#define gCps_OffsetCompensationIndicator_c          BIT12

/* cpsInstantMeasDirection_t */
#define gCps_DirectionUnknown_c                     0x00U
#define gCps_DirectionTangential_c                  BIT4
#define gCps_DirectionRadial_c                      BIT5
#define gCps_DirectionLateral_c                     BIT4 | BIT5

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Cycling Power Service - Measurement Flags */
typedef uint16_t cpsMeasurementFlags_t;

/*! Cycling Power Service - Pedal Power Balance Reference Flags */
typedef enum
{
    gCps_PedalPowerBalanceRefUnknown_c  = 0x00,
    gCps_PedalPowerBalanceRefLeft_c  = BIT1
} cpsPedalPowerBalanceRef_tag;

/*! Cycling Power Service - Accumulated Torque Source Flags */
typedef enum
{
    gCps_AccumulatedTorqueSourceWheel_c  = 0x00,
    gCps_AccumulatedTorqueSourceCrank_c  = BIT3
} cpsAccumulatedTorqueSource_tag;

typedef struct cpsExtremeTorqueMagnitudePair_tag
{
    int16_t     maxTorqueMagnitude;     /* C4 */
    int16_t     minTorqueMagnitude;     /* C4 */
}cpsExtremeTorqueMagnitudePair_t;

typedef struct cpsExtremeForceMagnitudePair_tag
{
    int16_t     maxForceMagnitude;      /* C3 */
    int16_t     minForceMagnitude;      /* C3 */
}cpsExtremeForceMagnitudePair_t;

typedef struct cpsAnglePair_tag
{
    int32_t     maxAngle   :12;      /* C5 */
    int32_t     minAngle   :12;      /* C5 */
    int32_t     reserved   :8; 
}cpsAnglePair_t;

/*! Cycling Power Service - Measurement */
typedef struct cpsMeasurement_tag
{
    cpsMeasurementFlags_t  flags;
    int16_t     instantaneousPower;
    uint8_t     pedalPowerBalance;      /* O */
    uint16_t    accumulatedTorque;      /* O */
    uint32_t    cumulativeWheelRevs;    /* C1 */
    uint16_t    lastWheelEventTime;     /* C1 */
    uint16_t    cumulativeCrankRevs;    /* C2 */
    uint16_t    lastCrankEventTime;     /* C2 */
    union
    {
        cpsExtremeForceMagnitudePair_t  forceMagnitude; /* C3 */
        cpsExtremeTorqueMagnitudePair_t torqueMagnitude; /* C4 */
    }magnitude;
    cpsAnglePair_t    extremeAngles;               /* C5 */
    uint16_t    topDeadSpotAngle;       /* O */
    uint16_t    bottomDeadSpotAngle;    /* O */
    uint16_t    accumulatedEnergy;      /* O */

}cpsMeasurement_t;

/*! Cycling Power Service - Feature */
typedef uint32_t cpsFeature_t;

typedef uint8_t cpsSensorLocation_t;

/*! Cycling Power Service - Sensor Location */
typedef enum
{
    gCps_Other_c           = 0x00,
    gCps_TopOfShoe_c       = 0x01,
    gCps_InShoe_c          = 0x02,
    gCps_Hip_c             = 0x03,
    gCps_FrontWheel_c      = 0x04,
    gCps_LeftCrank_c       = 0x05,
    gCps_RightCrank_c      = 0x06,
    gCps_LeftPedal_c       = 0x07,
    gCps_RightPedal_c      = 0x08,
    gCps_FrontHub_c        = 0x09,
    gCps_RearDropout_c     = 0x0A,
    gCps_Chainstay_c       = 0x0B,
    gCps_RearWheel_c       = 0x0C,
    gCps_RearHub_c         = 0x0D,
    gCps_Chest_c           = 0x0E
} cpsSensorLocation_tag;

/*! Cycling Power Service - Procedures Op Codes */
typedef uint8_t cpsOpCode_t;

/*! Cycling Power Service - Response Value */
typedef uint8_t cpsRspValue_t;

/*! Cycling Power Service - Mask Cycling Power Measurement Characteristic Content */
typedef uint16_t cpsMaskContent_t;

typedef PACKED_STRUCT cpsResponse_tag{
    cpsOpCode_t    reqOpCode;
    cpsRspValue_t  rspValue;
    PACKED_UNION {
        cpsSensorLocation_t sensorLocation[1];
        uint16_t            crankLength;
        uint16_t            chainLength;
        uint16_t            chainWeight;
        uint16_t            spanLength;
        cpsMaskContent_t    contentMask;
        uint8_t             samplingRate;
        int16_t             rawForce;
        ctsDateTime_t       factoryCalibrationDate;
    } rspData;
}cpsResponse_t;

/*! Cycling Power Service - SC Procedure */
typedef PACKED_STRUCT cpsProcedure_tag {
    cpsOpCode_t    opCode;
    PACKED_UNION {
        uint32_t            cummulativeValue;
        cpsSensorLocation_t sensorLocation;
        uint16_t            crankLength;
        uint16_t            chainLength;
        uint16_t            chainWeight;
        uint16_t            spanLength;
        cpsMaskContent_t    contentMask;
        cpsResponse_t       response;
    } procedureData;
} cpsProcedure_t;

/*! Cycling Power Service - Power Vector Flags */
typedef uint8_t cpsPowerVectorFlags_t;

/*! Cycling Power Service - Instantaneous Measurement Direction */
typedef uint8_t cpsInstantMeasDirection_t;

/*! Cycling Power Service - Power Vector */
typedef struct cpsPowerVector_tag {
    uint8_t                 arrayLength;
    cpsPowerVectorFlags_t   flags;
    uint16_t                crankRevCumulativeRev;  /* C1 */
    uint16_t                crankRevLastEventTime;  /* C1 */
    uint16_t                firstCrankMeasAngle;    /* O*/
    union {
        int16_t             force;    /* C2 */
        int16_t             torque;   /* C2 */
    }instantMagnitudeArray[1];
} cpsPowerVector_t;


/*! Cycling Power - User Data */
typedef struct cpsUserData_tag
{
    uint32_t                cumulativeWheelRevs;
    uint16_t                cumulativeCrankRevs;
    uint16_t                accumulatedEnergy;
    uint16_t                accumulatedTorque;
    uint16_t                crankLength;
    uint16_t                chainLength;
    uint16_t                chainWeight;
    uint16_t                spanLength;
    const cpsSensorLocation_t     *pSupportedSensorLocations;
    uint8_t                 cNumOfSupportedSensorLocations;
}cpsUserData_t;

/*! Cycling Power Service - Configuration */
typedef struct cpsConfig_tag
{
    uint16_t              serviceHandle;
    cpsFeature_t          cpsFeatures;
    cpsMaskContent_t      maskContent;
    cpsUserData_t         *pUserData;
    cpsSensorLocation_t   sensorLocation;
    uint16_t              pendingCccdFlags;
    bool_t                offsetCompensationOngoing;
    bool_t                procInProgress;
} cpsConfig_t;

/*! Cycling Power Service - ATT Error Codes */
enum
{
    gAttErrCodeInappropiateConnParam_c = 0x80
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
* \brief        Starts Cycling Power Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cps_Start (cpsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Cycling Power Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cps_Stop (cpsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Cycling Power service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cps_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Cycling Power service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cps_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Records Cycling Power measurement on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    pMeasurement    Pointer to Cycling Power Measurement structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cps_RecordMeasurement(uint16_t serviceHandle, cpsMeasurement_t *pMeasurement);

/*!**********************************************************************************
* \brief        Records Cycling Power measurement on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    pPowerVector    Pointer to Power Vector structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cps_RecordPowerVector(uint16_t serviceHandle, cpsPowerVector_t *pPowerVector);

/*!**********************************************************************************
* \brief        Sets the sensor location on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    sensorLocation  Sensor location value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cps_SetSensorLocation(uint16_t serviceHandle, cpsSensorLocation_t sensorLocation);

/*!**********************************************************************************
* \brief        Sends the Offset Compensation value and ends the procedure.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    rawForceTorque  Raw Force/Torque value w/ resolution of 1 N resp 1 N/m.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Cps_FinishOffsetCompensation(cpsConfig_t *pServiceConfig, uint16_t rawForceTorque);

/*!**********************************************************************************
* \brief        Handles write operations on the Control point.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    pEvent          Pointer to the GATT Server event.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
void Cps_ControlPointHandler (cpsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

#ifdef __cplusplus
}
#endif 

#endif /* CYCLING_POWER_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
