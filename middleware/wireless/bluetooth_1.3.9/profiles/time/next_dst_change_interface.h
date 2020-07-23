/*! *********************************************************************************
* \addtogroup BLE PROFILES
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef NEXT_DST_CHANGE_INTERFACE_H
#define NEXT_DST_CHANGE_INTERFACE_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "gatt_interface.h"
#include "current_time_interface.h"

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
    gNdcsStandardTime           = 0U,
    gNdcsHalfDaylightTime       = 2U,
    gNdcsDaylightTime           = 4U,
    gNdcsDoubleDaylightTime     = 8U,
    gNdcsDstUnknown             = 255U
} dstOffset_tag;

typedef uint8_t dstOffset_t;

typedef PACKED_STRUCT ndcsTimeWithDst_tag
{
    ctsDateTime_t  date;
    dstOffset_t offset;
} ndcsTimeWithDst_t;

/*! Next DST Change Service - Configuration */
typedef struct ndcsConfig_tag
{
    uint16_t            serviceHandle;
    ndcsTimeWithDst_t   timeWithDst;
} ndcsConfig_t;

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
* \brief        Starts Next DST Change Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ndcs_Start(ndcsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Next DST Change Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ndcs_Stop(ndcsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Next DST Change Service
*
* \param[in]    pClient  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ndcs_Subscribe(deviceId_t clientDeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Next DST Change Service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ndcs_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Records current time on a specified service handle.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ndcs_RecordCurrentTime (ndcsConfig_t *pServiceConfig);

#ifdef __cplusplus
}
#endif

#endif /* NEXT_DST_CHANGE_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
