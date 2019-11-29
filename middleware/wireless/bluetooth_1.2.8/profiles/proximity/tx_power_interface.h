/*! *********************************************************************************
* \addtogroup Tx Power Service
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

#ifndef _TX_POWER_INTERFACE_H_
#define _TX_POWER_INTERFACE_H_

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
typedef int8_t pwrLvlDb_t;

/*! Tx Power Service - Configuration */
typedef struct txsConfig_tag
{
    uint16_t       serviceHandle;
    pwrLvlDb_t     initialTxPowerLevel;
} txsConfig_t;

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
* \brief        Starts Tx Power Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Txs_Start (txsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Tx Power Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Txs_Stop (txsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Tx Power service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Txs_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes the GATT client from the Tx Power service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Txs_Unsubscribe();

/*!**********************************************************************************
* \brief        Gets the TX power level value on a specified service.
*
* \param[in]    serviceHandle   	Service handle.
* \param[out]   pOutTxPowerLevel	Pointer to store the tx power value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Txs_GetTxPowerLevel(uint16_t serviceHandle, pwrLvlDb_t *pOutTxPowerLevel);

/*!**********************************************************************************
* \brief        Sets the tx power value on a specified service.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    txPowerLevel	Tx power value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Txs_SetTxPowerLevel(uint16_t serviceHandle, pwrLvlDb_t txPowerLevel);

#ifdef __cplusplus
}
#endif 

#endif /* _TX_POWER_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
