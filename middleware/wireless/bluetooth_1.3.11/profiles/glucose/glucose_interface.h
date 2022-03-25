/*! *********************************************************************************
* \defgroup Glucose Service
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

#ifndef GLUCOSE_INTERFACE_H
#define GLUCOSE_INTERFACE_H

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

/*! Glucose Service - Maximum number of stored measurements */
#ifndef gGls_MaxNumOfStoredMeasurements_c
#define gGls_MaxNumOfStoredMeasurements_c       3U
#endif


/*! Glucose Service - All Features Macro */
#define Gls_AllFeatures     (gGls_LowBatteryDetectionSupported_c | gGls_SensorMalfunctionDetectionSupported_c |\
                            gGls_SensorSampleSizeSupported_c | gGls_SensorStripInsertErrDetectionSupported_c |\
                            gGls_SensorStripTypeErrDetectionSupported_c | gGls_SensorResultHighLowDetectionSupported_c |\
                            gGls_SensorTempHighLowDetectionSupported_c | gGls_SensorReadInterruptDetectionSupported_c |\
                            gGls_SensorGeneralDeviceFaultSupported_c | gGls_SensorTimeFaultSupported_c |\
                            gGls_SensorMultipleBondSupported_c)

/* glsOpCode_t */
#define gGls_Reserved_c                         0x00U
#define gGls_ReportStoredRecords_c              0x01U
#define gGls_DeleteStoredRecords_c              0x02U
#define gGls_AbortOperation_c                   0x03U
#define gGls_ReportNumOfStoredRecords_c         0x04U
#define gGls_NumOfStoredRecordsRsp_c            0x05U
#define gGls_RspCode_c                          0x06U

/* glsRspCodeValue_t */
#define gGls_RspReserved_c                      0x00U
#define gGls_RspSuccess_c                       0x01U
#define gGls_RspOpCodeNotSupported_c            0x02U
#define gGls_RspInvalidOperator_c               0x03U
#define gGls_RspOperatorNotSupported_c          0x04U
#define gGls_RspInvalidOperand_c                0x05U
#define gGls_RspNoRecordsFound_c                0x06U
#define gGls_RspAbortUnsuccessful_c             0X07U
#define gGls_RspProcNotCompleted_c              0x08U
#define gGls_RspOperandNotSupported_c           0x09U

/* glsFlags_t */
#define gGls_NoFlags                            0x00U
#define gGls_TimeOffsetPresent_c                BIT0
#define gGls_ConcTypeSampleLocPresent_c         BIT1
#define gGls_SensorStatusPresent_c              BIT3
#define gGls_ContextInfoFollows_c               BIT4

/* glsOperator_t */
#define gGls_Null_c                             0x00U
#define gGls_AllRecords_c                       0x01U
#define gGls_LessThanOrEqualTo_c                0x02U
#define gGls_GreaterThanOrEqualTo_c             0x03U
#define gGls_WithinRangeOf_c                    0x04U
#define gGls_FirstRecord_c                      0x05U
#define gGls_LastRecord_c                       0x06U

/* glsContextFlags_t */
#define gGls_NoContextFlags_c                   0x00U
#define gGls_CarbPresent_c                      BIT0
#define gGls_MealPresent_c                      BIT1
#define gGls_TesterHealthPresent_c              BIT2
#define gGls_ExerciseDurationIntesityPresent_c  BIT3
#define gGls_MedicationPresent_c                BIT4
#define gGls_HbA1cPresent_c                     BIT6
#define gGls_ExtendedFlagsPresent_c             BIT7

/* glsFilterTypeValue_t */
#define gGls_FtvReserved_c                      0x00U
#define gGls_FtvSeqNumber_c                     0x01U
#define gGls_FtvUserFacingTime_c                0x02U

/* For MISRA compliance */
#define gGls_NoAnnce_c                          0x00U
#define gGls_DevBatteryLow_c                    BIT0
#define gGls_SensorFault_c                      BIT1
#define gGls_InsuffSample_c                     BIT2
#define gGls_StripInsertErr_c                   BIT3
#define gGls_StripTypeErr_c                     BIT4
#define gGls_HigherResult_c                     BIT5
#define gGls_LowerResult_c                      BIT6
#define gGls_TempTooHigh_c                      BIT7
#define gGls_TempTooLow_c                       BIT8
#define gGls_ReadInterrupt_c                    BIT9
#define gGls_GeneralFault_c                     BIT10
#define gGls_TimeFault_c                        BIT11

/* glsGlucConcUnits_t */
#define gGls_UnitInKgpL_c                       0x00U
#define gGls_UnitInMolpL_c                      BIT2

/* glsCarbId_t */
#define gGls_CarbIdReserved_c                   0x00U
#define gGls_CarbIdBreakfast_c                  0x01U
#define gGls_CarbIdLunch_c                      0x02U
#define gGls_CarbIdDinner_c                     0x03U
#define gGls_CarbIdSnack_c                      0x04U
#define gGls_CarbIdDrink_c                      0x05U
#define gGls_CarbIdSupper_c                     0x06U
#define gGls_CarbIdBrunch_c                     0x07U

/* glsMeal_t */
#define gGls_MealReserved_c                     0x00U
#define gGls_MealPreprandial_c                  0x01U
#define gGls_MealPostprandial_c                 0x02U
#define gGls_MealFasting_c                      0x03U
#define gGls_MealCasual_c                       0x04U
#define gGls_MealBedtime_c                      0x05U

/* glsTester_t */
#define gGls_TesterReserved_c                   0x00U
#define gGls_TesterSelf_c                       0x01U
#define gGls_TesterProfessional_c               0x02U
#define gGls_TesterLab_c                        0x03U
#define gGls_TesterNotAvailable_c               0x0FU

/* glsHealth_t */
#define gGls_HealthReserved_c                   0x00U
#define gGls_HealthMinorIssues_c                0x01U
#define gGls_HealthMajorIssues_c                0x02U
#define gGls_HealthDuringMenses_c               0x03U
#define gGls_HealthUnderStress_c                0x04U
#define gGls_HealthNoIssues_c                   0x05U
#define gGls_HealthNotAvailable_c               0x0FU

/* glsMedicationId_t */
#define gGls_MedicationIdReserved_c             0x00U
#define gGls_MedicationIdRapidActingInsulin_c   0x01U
#define gGls_MedicationIdShortActingInsulin_c   0x02U
#define gGls_MedicationIdIntermActingInsulin_c  0x03U
#define gGls_MedicationIdLongActingInsulin_c    0x04U
#define gGls_MedicationIdPreMixedInsulin_c      0x05U

/* glsCMedicationUnits_t */
#define gGls_MedicationUnitsInKg_c              0x00U
#define gGls_MedicationUnitsInLitres_c          BIT5

/* glsType_t */
#define gGls_TypeReserved_c                     0x00U
#define gGls_TypeCapilaryWholeBlood_c           0x01U
#define gGls_TypeCapilaryPlasma_c               0x02U
#define gGls_TypeVenousWholeBlood_c             0x03U
#define gGls_TypeVenousPlasma_c                 0x04U
#define gGls_TypeArterialWholeBlood_c           0x05U
#define gGls_TypeArterialPlasma_c               0x06U
#define gGls_TypeUndetWholeBlood_c              0x07U
#define gGls_TypeUndetPlasma_c                  0x08U
#define gGls_TypeIsf_c                          0x09U
#define gGls_TypeControlSolution_c              0x0AU

/* For MISRA compliance */
#define gGls_LocReserved_c                      0x00U
#define gGls_LocFinger_c                        0x01U
#define gGls_LocAltSiteTest_c                   0x02U
#define gGls_LocEarLobe_c                       0x03U
#define gGls_LocControlSolution_c               0x04U
#define gGls_LocNotAvailable_c                  0x0FU

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Glucose Service - Flags */
typedef uint8_t glsFlags_t;

/*! Glucose Service - Glucose Concentration Units */
typedef uint8_t glsGlucConcUnits_t;

/*! Glucose Service - Type and Sample Location */
typedef struct glsTypeSampleLoc_tag
{
    uint8_t     type        : 4;
    uint8_t     sampleLoc   : 4;
}glsTypeSampleLoc_t;

/*! Glucose Service - Type Enum */
typedef uint8_t glsType_t;

/*! Glucose Service - Sample Location Enum */
typedef uint8_t glsSampleLocation_t;

/*! Glucose Service - Sensor Status Annce */
typedef uint16_t glsSensorStatusAnnce_t;

/*! Glucose Service - Measurement */
typedef struct glsMeasurement_tag
{
    glsFlags_t              flags;
    glsTypeSampleLoc_t      typeAndSampleLoc;
    glsSensorStatusAnnce_t  sensorStatusAnnce;
    uint16_t                seqNumber;
    ctsDateTime_t           dateTime;
    int16_t                 timeOffset;
    ieee11073_16BitFloat_t  glucCon;
}glsMeasurement_t;

/*! Glucose Service - Measurement Context Flags */
typedef uint8_t glsContextFlags_t;

/*! Glucose Service - Medication Units */
typedef uint8_t glsCMedicationUnits_t;

/*! Glucose Service - Carbohydrate Id */
typedef uint8_t glsCarbId_t;

/*! Glucose Service - Meal */
typedef uint8_t glsMeal_t;

/*! Glucose Service - Tester and Health */
typedef struct glsTesterHealth_tag
{
    uint8_t     tester  : 4;
    uint8_t     health  : 4;
}glsTesterHealth_t;

typedef uint8_t glsTester_t;

typedef uint8_t glsHealth_t;

/*! Glucose Service - Medication Id */
typedef uint8_t glsMedicationId_t;

/*! Glucose Service - Measurement Context */
typedef struct glsMeasurementContext_tag
{
    glsContextFlags_t       contextFlags;
    uint16_t                seqNumber;    
    uint8_t                 extendedFlags;
    glsCarbId_t             carbId;
    ieee11073_16BitFloat_t  carbUnits;
    glsMeal_t               meal;
    glsTesterHealth_t       testerHealth;
    uint16_t                exerciseDuration;
    uint8_t                 exerciseIntensity;
    glsMedicationId_t       medicationId;
    ieee11073_16BitFloat_t  medicationUnits;
    ieee11073_16BitFloat_t  hbA1c;
}glsMeasurementContext_t;

/*! Glucose Service - Full Measurement with Context */
typedef struct glsFullMeasurement_tag
{
    glsFlags_t              flags;
    glsTypeSampleLoc_t      typeAndSampleLoc;
    glsSensorStatusAnnce_t  sensorStatusAnnce;
    uint16_t                seqNumber;
    ctsDateTime_t           dateTime;
    int16_t                 timeOffset;
    ieee11073_16BitFloat_t  glucCon;
    /* Extended fields */
    glsContextFlags_t       contextFlags;
    uint8_t                 extendedFlags;
    glsCarbId_t             carbId;
    ieee11073_16BitFloat_t  carbUnits;
    glsMeal_t               meal;
    glsTesterHealth_t       testerHealth;
    uint16_t                exerciseDuration;
    uint8_t                 exerciseIntensity;
    glsMedicationId_t       medicationId;
    ieee11073_16BitFloat_t  medicationUnits;
    ieee11073_16BitFloat_t  hbA1c;
}glsFullMeasurement_t;

/*! Glucose Service - Feature Flags */
typedef uint16_t glsFeatureFlags_t;

/*! Glucose Service - Feature Flags */
typedef enum
{
    gGls_NoFeature_c                                = 0x00,
    gGls_LowBatteryDetectionSupported_c             = BIT0,
    gGls_SensorMalfunctionDetectionSupported_c      = BIT1,
    gGls_SensorSampleSizeSupported_c                = BIT2,
    gGls_SensorStripInsertErrDetectionSupported_c   = BIT3,
    gGls_SensorStripTypeErrDetectionSupported_c     = BIT4,
    gGls_SensorResultHighLowDetectionSupported_c    = BIT5,
    gGls_SensorTempHighLowDetectionSupported_c      = BIT6,
    gGls_SensorReadInterruptDetectionSupported_c    = BIT7,
    gGls_SensorGeneralDeviceFaultSupported_c        = BIT8,
    gGls_SensorTimeFaultSupported_c                 = BIT9,
    gGls_SensorMultipleBondSupported_c              = BIT10
} glsFeatureFlags_tag;

/*! Glucose Service - Op Code */
typedef uint8_t glsOpCode_t;

/*! Glucose Service - Operator */
typedef uint8_t glsOperator_t;

/*! Glucose Service - Filter Type Value */
typedef uint8_t glsFilterTypeValue_t;

/*! Glucose Service - Response Code */
typedef uint8_t glsRspCodeValue_t;

/*! Glucose Service - Response Code */
typedef PACKED_STRUCT glsResponseCode_tag{
    glsOpCode_t         reqOpCode;
    glsRspCodeValue_t   rspCodeValue;
}glsResponseCode_t;

typedef PACKED_STRUCT glsFilter_tag{
    glsFilterTypeValue_t    filter;
    uint16_t                filterParameter;
}glsFilter_t;

/*! Glucose Service - RACP Procedure */
typedef PACKED_STRUCT glsProcedure_tag {
    glsOpCode_t     opCode;
    glsOperator_t   glsOperator;
    PACKED_UNION {
        uint16_t                numberOfRecords;
        glsResponseCode_t       responseCode;
        glsFilter_t             filter;
    } operand;
} glsProcedure_t;

/*! Glucose Service - User Data */
typedef struct glsUserData_tag
{
    glsFullMeasurement_t       *pStoredMeasurements;
    uint8_t                     cMeasurements;
    uint8_t                     measurementCursor;
    uint16_t                    lastSeqNumber;    
}glsUserData_t;

/*! Glucose Service - Configuration */
typedef struct glsConfig_tag
{
    uint16_t              serviceHandle;           
    glsFeatureFlags_t     glsfeatureFlags;
    glsUserData_t         *pUserData;
    bool_t                procInProgress;
    bool_t                currentProcedure;
} glsConfig_t;

/*! Glucose Service - ATT Error Codes */
enum
{
    gAttErrCodeGlsProcedureAlreadyInProgress_c = 0x80,
    gAttErrCodeGlsCccdImproperlyConfigured_c   = 0x81
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
* \brief        Starts Glucose Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Gls_Start (glsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Glucose Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Gls_Stop (glsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Glucose service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Gls_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Glucose service
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Gls_Unsubscribe(glsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Records blood pressure measurement on a specified service handle.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    pMeasurement    Pointer to Glucose Full Measurement structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Gls_RecordGlucoseMeasurement (glsConfig_t *pServiceConfig, glsFullMeasurement_t *pMeasurement);

/*!**********************************************************************************
* \brief        Handles write operations on the Control point.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    pEvent          Pointer to the GATT Server event.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
void Gls_ControlPointHandler (glsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

#ifdef __cplusplus
}
#endif 

#endif /* GLUCOSE_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
