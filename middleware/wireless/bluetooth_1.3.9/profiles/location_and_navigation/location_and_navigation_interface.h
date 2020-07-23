/*! *********************************************************************************
* \defgroup Location and Navigation Service
* @{
*********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2019, Freescale Semiconductor, Inc.
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef LOCATION_AND_NAGIATION_INTERFACE_H
#define LOCATION_AND_NAGIATION_INTERFACE_H

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
#define Lns_AllFeatures (gLns_InstantaneousSpeedSupported_c | gLns_TotalDistanceSupported_c |\
                        gLns_LocationSupported_c | gLns_ElevationSupported_c | gLns_HeadingSupported_c |\
                        gLns_RollingTimeSupported_c | gLns_UtcTimeSupported_c | gLns_RemainingDistanceSupported_c |\
                        gLns_RemainingVerticalDistanceSupported_c | gLns_EstmatedTimeOfArrivalSupported_c |\
                        gLns_NumberOfBeaconsInSolutionSupported_c | gLns_NumberOfBeaconsInViewSupported_c |\
                        gLns_TimeToFirstFixSupported_c | gLns_EstimatedHorizontalPositionErrorSupported_c |\
                        gLns_EstimatedVerticalPositionErrorSupported_c | gLns_HorizontalDilutionOfPrecisionSupported_c |\
                        gLns_VerticalDilutionOfPrecisionSupported_c | gLns_LocationAndSpeedCharContentMaskingSupported_c |\
                        gLns_FixRateSettingSupported_c | gLns_ElevationSettingSupported_c | gLns_PositionStatusSupported_c )

#define Lns_MandatoryFeatures    0x00

/* lnsFlags_t */
#define gLns_NoFlags                        0x00U
#define gLns_InstantaneousSpeedPresent_c    BIT0
#define gLns_TotalDistancePresent_c         BIT1
#define gLns_LocationPresent_c              BIT2
#define gLns_ElevationPresent_c             BIT3
#define gLns_HeadingPresent_c               BIT4
#define gLns_RollingTimePresent_c           BIT5
#define gLns_UtcTimePresent_c               BIT6

/* lnsNavigationFlags_t */
#define gLns_RemainingDistancePresent_c            BIT0
#define gLns_RemainingVerticalDistancePresent_c    BIT1
#define gLns_EstimatedTimeOfArrivalPresent_c       BIT2
#define gLns_WaypointReached_c                     BIT7
#define gLns_DestinationReached_c                  BIT8

/* lnsRspValue_t */
#define gErrCodeNoError_c                   0x01U
#define gErrCodeLnsOpCodeNotSupported_c     0x02U
#define gErrCodeLnsInvalidParameter_c       0x03U
#define gErrCodeLnsOperationFailed_c        0x04U

/* lnsOpCode_t */
#define gLns_SetCumulativeValue_c           0x01U
#define gLns_MaskLocAndSpeedContent_c       0x02U
#define gLns_NavigationControl_c            0x03U
#define gLns_RequestNumberOfRoutes_c        0x04U
#define gLns_RequestNameOfRoutes_c          0x05U
#define gLns_SelectRoute_c                  0x06U
#define gLns_SetFixRate_c                   0x07U
#define gLns_SetElevation_c                 0x08U
#define gLns_ResponseCode_c                 0x20U

/* lnsNavigationControl_t */
#define gLnsStopNavigation_c                0x00U
#define gLnsStartNavigationFirstWaypoint_c  0x01U
#define gLnsPauseNavigation_c               0x02U
#define gLnsContinueNavigation_c            0x03U
#define gLnsSkipWaypoint_c                  0x04U
#define gLnsNearestWaypoint_c               0x05U

/* lnsPositionQualityFlags_t */
#define gLns_NoOfBeaconsInSolutionPresent_c  BIT0
#define gLns_NoOfBeaconsInViewPresent_c      BIT1
#define gLns_TimeToFirstFixPresent_c         BIT2
#define gLns_EHPEPresent_c                   BIT3
#define gLns_EVPEPresent_c                   BIT4
#define gLns_HDOPPresent_c                   BIT5
#define gLns_VDOPPresent_c                   BIT6

/* lnsFeatures_t */
#define gLns_NoFeature_c                                    0x00U
#define gLns_InstantaneousSpeedSupported_c                  BIT0
#define gLns_TotalDistanceSupported_c                       BIT1
#define gLns_LocationSupported_c                            BIT2
#define gLns_ElevationSupported_c                           BIT3
#define gLns_HeadingSupported_c                             BIT4
#define gLns_RollingTimeSupported_c                         BIT5
#define gLns_UtcTimeSupported_c                             BIT6
#define gLns_RemainingDistanceSupported_c                   BIT7
#define gLns_RemainingVerticalDistanceSupported_c           BIT8
#define gLns_EstmatedTimeOfArrivalSupported_c               BIT9
#define gLns_NumberOfBeaconsInSolutionSupported_c           BIT10
#define gLns_NumberOfBeaconsInViewSupported_c               BIT11
#define gLns_TimeToFirstFixSupported_c                      BIT12
#define gLns_EstimatedHorizontalPositionErrorSupported_c    BIT13
#define gLns_EstimatedVerticalPositionErrorSupported_c      BIT14
#define gLns_HorizontalDilutionOfPrecisionSupported_c       BIT15
#define gLns_VerticalDilutionOfPrecisionSupported_c         BIT16
#define gLns_LocationAndSpeedCharContentMaskingSupported_c  BIT17
#define gLns_FixRateSettingSupported_c                      BIT18
#define gLns_ElevationSettingSupported_c                    BIT19
#define gLns_PositionStatusSupported_c                      BIT20

/* lnsPositionStatus_t */
#define gLns_NoPosition_c                                   0x00U
#define gLns_PositionOk_c                                   BIT7
#define gLns_EstimatedPosition_c                            BIT8
#define gLns_LastKnownPosition_c                            BIT7 | BIT8

/* lnsHeadingSource_t */
#define gLns_HeadingBasedOnMovement_c                       0x00U
#define gLns_HeadingBasedOnMagneticCompass_c                BIT12

/* lnsLocAndSpeedMask_t */
#define gLnsInstantaneousSpeedOff_c                         BIT0
#define gLnsTotalDistanceOff_c                              BIT1
#define gLnsLocationOff_c                                   BIT2
#define gLnsElevationOff_c                                  BIT3
#define gLnsHeadingOff_c                                    BIT4
#define gLnsRollingTimeOff_c                                BIT5
#define gLnsUtcTimeOff_c                                    BIT6

/* lnsElevationSource_t */
#define gLns_PositioningSystem_c                            0x00U
#define gLns_BarometricAirPressure_c                        BIT10
#define gLns_DatabaseService_c                              BIT11
#define gLns_OtherElevationSource_c                         BIT10 | BIT11

/* lnsSpeedAndDistanceFormat_t */
#define gLns_SpeedAndDistance2D_c                           0x00U
#define gLns_SpeedAndDistance3D_c                           BIT9

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef uint8_t lnsFlags_t;

typedef uint32_t lnsFeatures_t;
typedef uint16_t lnsPositionStatus_t;
typedef uint16_t lnsElevationSource_t;
typedef uint16_t lnsSpeedAndDistanceFormat_t;

typedef uint16_t lnsHeadingSource_t;

typedef uint16_t lnsLocAndSpeedFlags_t;

typedef PACKED_STRUCT lnsLocAndSpeed_tag
{
    lnsLocAndSpeedFlags_t       lnsLocAndSpeedFlags;
    uint16_t                    lnsInstantaneousSpeed;
    uint32_t                    lnsTotalDistance;
    int32_t                     lnsLatitude;
    int32_t                     lnsLongitude;
    int32_t                     lnsElevation;
    uint16_t                    lnsHeading;
    uint8_t                     lnsRollingTime;
    uint32_t                    lnsUtcTime;
} lnsLocAndSpeed_t;

typedef uint16_t lnsPositionQualityFlags_t;

typedef struct lnsPositionQuality_tag
{
    lnsPositionQualityFlags_t   lnsPositionQualityFlags;
    uint8_t                     lnsNumberOfBeaconsInSolution;
    uint8_t                     lnsNumberOfBeaconsInView;
    uint16_t                    lnsTimeToFirstFix;
    uint32_t                    lnsEHPE;
    uint32_t                    lnsEVPE;
    uint8_t                     lnsHDOP;
    uint8_t                     lnsVDOP;
} lnsPositionQuality_t;

typedef enum
{
    gNavigation_NoPosition_c            = 0x00,
    gNavigation_PositionOk_c            = BIT3,
    gNavigation_EstimatedPosition_c     = BIT4,
    gNavigation_LastKnownPosition_c     = BIT3 | BIT4
} lnsNavigationPositionStatus_tag;

typedef enum
{
    gNavigation_HeadingBasedOnMovement_c        = 0x00,
    gNavigation_HeadingBasedOnMagneticCompass_c = BIT5
} lnsNavigationHeadingSource_tag;

typedef enum
{
    gLns_ToWaypoint_c           = 0x00,
    gLns_ToDestination_c        = BIT6
} lnsNavigationIndicatorType_tag;

typedef uint16_t lnsNavigationFlags_t;

typedef struct lnsNavigation_tag
{
    lnsNavigationFlags_t        lnsNavigationFlags;
    uint16_t                    lnsBearing;
    uint16_t                    lnsHeading;
    uint32_t                    lnsRemainingDistance;
    int32_t                     lnsRemainingVerticalDistance;
    uint32_t                    lnsETA;
} lnsNavigation_t;

typedef uint16_t lnsLocAndSpeedMask_t;

typedef struct lnsUserData_tag
{
    lnsLocAndSpeed_t            lnsLocAndSpeed;
    lnsPositionQuality_t        lnsPositionQuality;
    lnsNavigation_t             lnsNavigation;
    lnsLocAndSpeedMask_t        lnsMask;
} lnsUserData_t;

/*! Location and Navigation Service - Procedures Op Codes */
typedef uint8_t lnsOpCode_t;

typedef uint8_t lnsNavigationControl_t;

/*! Location and Navigation Service - ATT Error Codes */
typedef uint8_t lnsRspValue_t;

typedef struct lnsResponse_tag{
    lnsOpCode_t    reqOpCode;
    lnsRspValue_t  rspValue;
    union {
        uint16_t    lnsNumberOfRoutes;
        uint8_t     lnsNameOfRoute[4];
        uint16_t    lnsSelectedRoute;
    } rspData;
}lnsResponse_t;

/*! Location and Navigation Service - SC Procedure */
typedef PACKED_STRUCT lnsProcedure_tag {
    lnsOpCode_t    opCode;
    union
    {
        uint32_t        lnsTotalDistance;
        uint16_t        lnsMaskLocAndSpeedCharContent;
        uint8_t         lnsNavigationControl;
        uint16_t        lnsRouteNumber;
//        uint16_t        lns
        uint16_t        lnsSelectRoute;
        uint8_t         lnsFixRate;
        int32_t         lnsElevation;
        lnsResponse_t   response;
    } procedureData;
} lnsProcedure_t;

/*! Location and Navigation Service - Configuration */
typedef struct lnsConfig_tag
{
    uint16_t                    serviceHandle;
    lnsFeatures_t               lnsFeatures;
    lnsUserData_t               *pUserData;
} lnsConfig_t;

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
* \brief        Starts Location and Navigation Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lns_Start (lnsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Location and Navigation Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lns_Stop (lnsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Location and Navigation service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lns_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Location and Navigation service
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lns_Unsubscribe(lnsConfig_t *pServiceConfig);

bleResult_t Lns_RecordLocationAndSpeedCharacteristic(uint16_t serviceHandle, lnsLocAndSpeed_t *pLocAndSpeed);
bleResult_t Lns_RecordNavigationCharacteristic(uint16_t serviceHandle, lnsNavigation_t *pNavigation);
bleResult_t Lns_RecordPositionQualityCharacteristic(uint16_t serviceHandle, lnsPositionQuality_t *pPositionQuality);
void Lns_ControlPointHandler (lnsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

#ifdef __cplusplus
}
#endif

#endif /* LOCATION_AND_NAGIATION_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
