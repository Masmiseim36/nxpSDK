/*! *********************************************************************************
* \addtogroup BLE PROFILES
* @{
********************************************************************************** */
/*!
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _REFERENCE_TIME_UPDATE_INTERFACE_H_
#define _REFERENCE_TIME_UPDATE_INTERFACE_H_

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

/***********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef enum
{
    gRtus_GetReferenceUpdate_c          = 0x01,
    gRtus_CancelReferenceUpdate_c       = 0x02
} timeUpdateCommand_tag;

typedef uint8_t timeUpdateCommand_t;

typedef enum
{
    gRtusIdle_c             = 0x00,
    gRtusUpdatePending_c    = 0x01
} currentState_tag;

typedef uint8_t currentState_t;

typedef enum
{
    gRtusSuccessful_c                   = 0x00,
    gRtusCanceled_c                     = 0x01,
    gRtusNoConnToReference_c            = 0x02,
    gRtusReferenceResponseError_c       = 0x02,
    gRtusTimeout_c                      = 0x04,
    gRtusUpdateNotAttemptedAfterRst_c   = 0x05
} result_tag;

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
bleResult_t Rtus_Unsubscribe();

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

#endif /* _REFERENCE_TIME_UPDATE_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
