/*! *********************************************************************************
* \defgroup Battery Service
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the Battery Service
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef BATTERY_INTERFACE_H
#define BATTERY_INTERFACE_H

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

/*! Battery Service - Configuration */
typedef struct basConfig_tag
{
    uint16_t    serviceHandle;
    uint8_t	    batteryLevel;
    bool_t*     aValidSubscriberList;
    uint8_t     validSubscriberListSize;
} basConfig_t;

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
* \brief        Starts Battery Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Bas_Start(basConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Battery Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Bas_Stop(basConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Battery service
*
* \param[in]    pServiceConfig  Pointer to service configuration structure
* \param[in]    pClient  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Bas_Subscribe(basConfig_t* pServiceConfig, deviceId_t clientDeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Battery service
*
* \param[in]    pServiceConfig  Pointer to service configuration structure
* \param[in]    pClient  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Bas_Unsubscribe(basConfig_t* pServiceConfig, deviceId_t clientDeviceId);

/*!**********************************************************************************
* \brief        Records battery measurement on a specified service handle.
*
* \param[in]    pServiceConfig  pointer to service configuration structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Bas_RecordBatteryMeasurement (basConfig_t* pServiceConfig);

#ifdef __cplusplus
}
#endif

#endif /* BATTERY_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
