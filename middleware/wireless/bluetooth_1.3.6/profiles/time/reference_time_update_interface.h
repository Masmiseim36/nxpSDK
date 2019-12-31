/*! *********************************************************************************
* \addtogroup BLE PROFILES
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef REFERENCE_TIME_UPDATE_INTERFACE_H
#define REFERENCE_TIME_UPDATE_INTERFACE_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "gatt_interface.h"

/***********************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/
#define gRtus_GetReferenceUpdate_c          0x01U
#define gRtus_CancelReferenceUpdate_c       0x02U

#define gRtusIdle_c                         0x00U
#define gRtusUpdatePending_c                0x01U

#define gRtusSuccessful_c                   0x00U
#define gRtusCanceled_c                     0x01U
#define gRtusNoConnToReference_c            0x02U
#define gRtusReferenceResponseError_c       0x02U
#define gRtusTimeout_c                      0x04U
#define gRtusUpdateNotAttemptedAfterRst_c   0x05U

/***********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef uint8_t timeUpdateCommand_t;
typedef uint8_t currentState_t;
typedef uint8_t result_t;
typedef uint8_t rtusResult_t;

typedef PACKED_STRUCT timeUpdateState_tag
{
    currentState_t      currentState;
    rtusResult_t        result;
} timeUpdateState_t;

/*! Reference Time Update Service - Configuration */
typedef struct rtusConfig_tag
{
    uint16_t            serviceHandle;
    timeUpdateState_t   timeUpdateState;
} rtusConfig_t;

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
* \brief        Starts Reference Time Update Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Rtus_Start(rtusConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Reference Time Update Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Rtus_Stop(rtusConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Reference Time Update Service
*
* \param[in]    pClient  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Rtus_Subscribe(deviceId_t clientDeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Reference Time Update Service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Rtus_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Records time update state on a specified service handle.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Rtus_RecordTimeUpdateState (rtusConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Handles write operations on the RTUS Control Point.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    pEvent          Pointer to the GATT Server event.
*
************************************************************************************/
void Rtus_ControlPointHandler(rtusConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);


#ifdef __cplusplus
}
#endif 

#endif /* REFERENCE_TIME_UPDATE_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
