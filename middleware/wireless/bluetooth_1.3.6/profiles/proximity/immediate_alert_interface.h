/*! *********************************************************************************
* \addtogroup Immediate Alert Service
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

#ifndef IMMEDIATE_ALERT_INTERFACE_H
#define IMMEDIATE_ALERT_INTERFACE_H

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

typedef uint8_t iasAlertLevel_t;

/*! Immediate Alert Service - Alert Level Values */
typedef enum
{
    gIas_NoAlert_c      = 0x00,
    gIas_MildAlert_c    = 0x01,
    gIas_HighAlert_c    = 0x02
} iasAlertLevel_tag;

/*! Immediate Alert Service - Configuration */
typedef struct iasConfig_tag
{
    uint16_t            serviceHandle;
    iasAlertLevel_t     initialAlertLevel;	
} iasConfig_t;

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
* \brief        Starts Immediate Alert Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ias_Start (iasConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Immediate Alert Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ias_Stop (iasConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Immediate Alert service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ias_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes the GATT client from the Immediate Alert service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ias_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Gets the alert level value on a specified service.
*
* \param[in]    serviceHandle   Service handle.
* \param[out]   pOutAlertLevel	Pointer to store the alert level value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ias_GetAlertLevel(uint16_t serviceHandle, iasAlertLevel_t *pOutAlertLevel);

/*!**********************************************************************************
* \brief        Sets the alert level value on a specified service.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    alertLevel		Alert level value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ias_SetAlertLevel(uint16_t serviceHandle, iasAlertLevel_t alertLevel);


#ifdef __cplusplus
}
#endif 

#endif /* IMMEDIATE_ALERT_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
