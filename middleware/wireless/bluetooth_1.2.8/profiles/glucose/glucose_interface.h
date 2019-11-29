/*! *********************************************************************************
* \defgroup Glucose Service
* @{
********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _GLUCOSE_INTERFACE_H_
#define _GLUCOSE_INTERFACE_H_

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
#define gGls_MaxNumOfStoredMeasurements_c       3
#endif


/*! Glucose Service - All Features Macro */
#define Gls_AllFeatures     (gGls_LowBatteryDetectionSupported_c | gGls_SensorMalfunctionDetectionSupported_c |\
                            gGls_SensorSampleSizeSupported_c | gGls_SensorStripInsertErrDetectionSupported_c |\
                            gGls_SensorStripTypeErrDetectionSupported_c | gGls_SensorResultHighLowDetectionSupported_c |\
                            gGls_SensorTempHighLowDetectionSupported_c | gGls_SensorReadInterruptDetectionSupported_c |\
                            gGls_SensorGeneralDeviceFaultSupported_c | gGls_SensorTimeFaultSupported_c |\
                            gGls_SensorMultipleBondSupported_c)


/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Glucose Service - Flags */
typedef uint8_t glsFlags_t;

/*! Glucose Service - Flags */
typedef enum
{
    gGls_NoFlags                        = 0x00,
    gGls_TimeOffsetPresent_c            = BIT0,
    gGls_ConcTypeSampleLocPresent_c     = BIT1,
    gGls_SensorStatusPresent_c          = BIT3,
    gGls_ContextInfoFollows_c           = BIT4
} glsFlags_tag;


/*! Glucose Service - Glucose Concentration Units */
typedef enum
{
    gGls_UnitInKgpL_c   = 0x00,
    gGls_UnitInMolpL_c  = BIT2
} glsGlucConcUnits_tag;

/*! Glucose Service - Type and Sample Location */
typedef struct glsTypeSampleLoc_tag
{
    uint8_t     type        : 4;
    uint8_t     sampleLoc   : 4;
}glsTypeSampleLoc_t;

/*! Glucose Service - Type Enum */
typedef enum
{
    gGls_TypeReserved_c             = 0x00,
    gGls_TypeCapilaryWholeBlood_c,
    gGls_TypeCapilaryPlasma_c,
    gGls_TypeVenousWholeBlood_c,
    gGls_TypeVenousPlasma_c,
    gGls_TypeArterialWholeBlood_c,
    gGls_TypeArterialPlasma_c,
    gGls_TypeUndetWholeBlood_c,
    gGls_TypeUndetPlasma_c,
    gGls_TypeIsf_c,
    gGls_TypeControlSolution_c
} glsType_t;

/*! Glucose Service - Sample Location Enum */
typedef enum
{
    gGls_LocReserved_c     = 0x00,
    gGls_LocFinger_c,
    gGls_LocAltSiteTest_c,
    gGls_LocEarLobe_c,
    gGls_LocControlSolution_c,
    gGls_LocNotAvailable_c         = 0x0F
} glsSampleLocation_t;


typedef uint16_t glsSensorStatusAnnce_t;

/*! Glucose Service - Sensor Status Annce */
typedef enum
{
    gGls_NoAnnce_c          = 0x00,
    gGls_DevBatteryLow_c    = BIT0,
    gGls_SensorFault_c      = BIT1,
    gGls_InsuffSample_c     = BIT2,
    gGls_StripInsertErr_c   = BIT3,
    gGls_StripTypeErr_c     = BIT4,
    gGls_HigherResult_c     = BIT5,
    gGls_LowerResult_c      = BIT6,
    gGls_TempTooHigh_c      = BIT7,
    gGls_TempTooLow_c       = BIT8,
    gGls_ReadInterrupt_c    = BIT9,
    gGls_GeneralFault_c     = BIT10,
    gGls_TimeFault_c        = BIT11
} glsSensorStatusAnnce_tag;

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

typedef enum
{
    gGls_NoContextFlags_c                   = 0x00,
    gGls_CarbPresent_c                      = BIT0,
    gGls_MealPresent_c                      = BIT1,
    gGls_TesterHealthPresent_c              = BIT2,
    gGls_ExerciseDurationIntesityPresent_c  = BIT3,
    gGls_MedicationPresent_c                = BIT4,
    gGls_HbA1cPresent_c                     = BIT6,
    gGls_ExtendedFlagsPresent_c             = BIT7
}glsContextFlags_tag;

/*! Glucose Service - Medication Units */
typedef enum
{
    gGls_MedicationUnitsInKg_c                   = 0x00,
    gGls_MedicationUnitsInLitres_c               = BIT5
}glsCMedicationUnits_tag;

/*! Glucose Service - Carbohydrate Id */
typedef uint8_t glsCarbId_t;

typedef enum
{
    gGls_CarbIdReserved_c = 0x00,
    gGls_CarbIdBreakfast_c,
    gGls_CarbIdLunch_c,
    gGls_CarbIdDinner_c,
    gGls_CarbIdSnack_c,
    gGls_CarbIdDrink_c,
    gGls_CarbIdSupper_c,
    gGls_CarbIdBrunch_c
}glsCarbId_tag;

/*! Glucose Service - Meal */
typedef uint8_t glsMeal_t;

typedef enum
{
    gGls_MealReserved_c = 0x00,
    gGls_MealPreprandial_c,
    gGls_MealPostprandial_c,
    gGls_MealFasting_c,
    gGls_MealCasual_c,
    gGls_MealBedtime_c
}glsMeal_tag;

/*! Glucose Service - Tester and Health */
typedef struct glsTesterHealth_tag
{
    uint8_t     tester  : 4;
    uint8_t     health  : 4;
}glsTesterHealth_t;

typedef enum
{
    gGls_TesterReserved_c = 0x00,
    gGls_TesterSelf_c,
    gGls_TesterProfessional_c,
    gGls_TesterLab_c,
    gGls_TesterNotAvailable_c = 0x0F
}glsTester_tag;

typedef enum
{
    gGls_HealthReserved_c = 0x00,
    gGls_HealthMinorIssues_c,
    gGls_HealthMajorIssues_c,
    gGls_HealthDuringMenses_c,
    gGls_HealthUnderStress_c,
    gGls_HealthNoIssues_c,
    gGls_HealthNotAvailable_c = 0x0F
}glsHealth_tag;

/*! Glucose Service - Medication Id */
typedef uint8_t glsMedicationId_t;

typedef enum
{
    gGls_MedicationIdReserved_c = 0x00,
    gGls_MedicationIdRapidActingInsulin_c,
    gGls_MedicationIdShortActingInsulin_c,
    gGls_MedicationIdIntermActingInsulin_c,
    gGls_MedicationIdLongActingInsulin_c,
    gGls_MedicationIdPreMixedInsulin_c,
}glsMedicationId_tag;

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

typedef enum
{
    gGls_Reserved_c             = 0x00,
    gGls_ReportStoredRecords_c,
    gGls_DeleteStoredRecords_c,
    gGls_AbortOperation_c,
    gGls_ReportNumOfStoredRecords_c,
    gGls_NumOfStoredRecordsRsp_c,
    gGls_RspCode_c
} glsOpCode_tag;

/*! Glucose Service - Operator */
typedef uint8_t glsOperator_t;

typedef enum
{
    gGls_Null_c             = 0x00,
    gGls_AllRecords_c,
    gGls_LessThanOrEqualTo_c,
    gGls_GreaterThanOrEqualTo_c,
    gGls_WithinRangeOf_c,
    gGls_FirstRecord_c,
    gGls_LastRecord_c
} glsOperator_tag;

/*! Glucose Service - Filter Type Value */
typedef uint8_t glsFilterTypeValue_t;

typedef enum
{
    gGls_FtvReserved_c          = 0x00,
    gGls_FtvSeqNumber_c,
    gGls_FtvUserFacingTime_c
} glsFilterTypeValue_tag;

/*! Glucose Service - Response Code */
typedef uint8_t glsRspCodeValue_t;

typedef enum
{
    gGls_RspReserved_c          = 0x00,
    gGls_RspSuccess_c,
    gGls_RspOpCodeNotSupported_c,
    gGls_RspInvalidOperator_c,
    gGls_RspOperatorNotSupported_c,
    gGls_RspInvalidOperand_c,
    gGls_RspNoRecordsFound_c,
    gGls_RspAbortUnsuccessful_c,
    gGls_RspProcNotCompleted_c,
    gGls_RspOperandNotSupported_c
} glsRspCodeValue_tag;

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
    union {
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

#endif /* _GLUCOSE_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
