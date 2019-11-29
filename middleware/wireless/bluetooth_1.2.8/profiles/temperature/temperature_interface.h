/*! *********************************************************************************
 * \addtogroup Temperature Custom Profile
 * @{
 ********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* This file is the interface file for the Temperature Service
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _TEMPERATURE_INTERFACE_H_
#define _TEMPERATURE_INTERFACE_H_

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

/*! Temperature Service - Invalid Value */
#define gTms_InvalidTemperatureValue_c     0x1000

/*! Temperature Service - Maximum Value ( -273.15 C)*/
#define gTms_MinimumTemperatureValue_c     0x954D

/*! Temperature Service - Minimum Value ( -273.15 C)*/
#define gTms_MaximumTemperatureValue_c     0x8FFF

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Temperature Service - Configuration */
typedef struct tmsConfig_tag
{
    uint16_t    serviceHandle;
    int16_t     initialTemperature;        
} tmsConfig_t;

/*! Temperature Client - Configuration */
typedef struct tmcConfig_tag
{
    uint16_t    hService;
    uint16_t    hTemperature;
    uint16_t    hTempCccd; 
    uint16_t    hTempDesc; 
    gattDbCharPresFormat_t  tempFormat;
} tmcConfig_t;

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
* \brief        Starts Temperature Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_Start(tmsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Temperature Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_Stop(tmsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Temperature service
*
* \param[in]    pClient  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_Subscribe(deviceId_t clientDeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Temperature service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_Unsubscribe();

/*!**********************************************************************************
* \brief        Records Temperature measurement on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    value           Temperature Value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_RecordTemperatureMeasurement (uint16_t serviceHandle, int16_t temperature);

#ifdef __cplusplus
}
#endif

#endif /* _TEMPERATURE_INTERFACE_H_ */

/*! **********************************************************************************
 * @}
 ************************************************************************************/
