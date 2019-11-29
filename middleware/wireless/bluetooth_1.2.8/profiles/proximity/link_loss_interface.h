/*! *********************************************************************************
* \addtogroup Link Loss Service
* @{
********************************************************************************** */
/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _LINK_LOSS_INTERFACE_H_
#define _LINK_LOSS_INTERFACE_H_

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

typedef uint8_t llsAlertLevel_t;

/*! Link Loss Service - Alert Level Values */
typedef enum
{
    gLls_NoAlert_c      = 0x00,
    gLls_MildAlert_c    = 0x01,
    gLls_HighAlert_c    = 0x02
} llsAlertLevel_tag;

/*! Link Loss Service - Configuration */
typedef struct llsConfig_tag
{
    uint16_t            serviceHandle;
    llsAlertLevel_t     initialAlertLevel;
} llsConfig_t;
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
* \brief        Starts Link Loss Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lls_Start (llsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Link Loss Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lls_Stop (llsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Link Loss service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lls_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes the GATT client from the Link Loss service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lls_Unsubscribe();

/*!**********************************************************************************
* \brief        Gets the alert level value on a specified service.
*
* \param[in]    serviceHandle   Service handle.
* \param[out]   pOutAlertLevel	Pointer to store the alert level value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lls_GetAlertLevel(uint16_t serviceHandle, llsAlertLevel_t *pOutAlertLevel);

/*!**********************************************************************************
* \brief        Sets the alert level value on a specified service.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    alertLevel		Alert level value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lls_SetAlertLevel(uint16_t serviceHandle, llsAlertLevel_t alertLevel);


#ifdef __cplusplus
}
#endif 

#endif /* _LINK_LOSS_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
