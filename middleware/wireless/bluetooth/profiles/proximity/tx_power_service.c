/*! *********************************************************************************
* \addtogroup Tx Power Service
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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"

#include "tx_power_interface.h"
/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
/*! Tx Power Service - Subscribed Client*/
static deviceId_t mTxs_SubscribedClientId;

/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
bleResult_t Txs_Start(txsConfig_t *pServiceConfig)
{
    (void)Txs_SetTxPowerLevel(pServiceConfig->serviceHandle, pServiceConfig->initialTxPowerLevel);

    mTxs_SubscribedClientId = gInvalidDeviceId_c;

    return gBleSuccess_c;
}

bleResult_t Txs_Stop(txsConfig_t *pServiceConfig)
{
    return gBleSuccess_c;
}

bleResult_t Txs_Subscribe(deviceId_t clientdeviceId)
{
    if (0U == mTxs_SubscribedClientId)
    {
        mTxs_SubscribedClientId = clientdeviceId;
    }

    return gBleSuccess_c;
}

bleResult_t Txs_Unsubscribe(void)
{
    mTxs_SubscribedClientId = gInvalidDeviceId_c;

    return gBleSuccess_c;
}

bleResult_t Txs_GetTxPowerLevel(uint16_t serviceHandle, pwrLvlDb_t *pOutTxPowerLevel)
{
    uint16_t  hTxPowerLevel;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_TxPower_d);
    uint16_t outLen = 0;

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &hTxPowerLevel);

    if (result == gBleSuccess_c)
    {
        result = GattDb_ReadAttribute(hTxPowerLevel, sizeof(pwrLvlDb_t), (uint8_t*) pOutTxPowerLevel, &outLen);
    }

    return result;
}

bleResult_t Txs_SetTxPowerLevel(uint16_t serviceHandle, pwrLvlDb_t txPowerLevel)
{
    uint16_t  hTxPowerLevel;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_TxPower_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &hTxPowerLevel);

    if (result == gBleSuccess_c)
    {
        /* Write attribute value*/
        result = GattDb_WriteAttribute(hTxPowerLevel, sizeof(pwrLvlDb_t), (uint8_t*)&txPowerLevel);
    }

    return result;
}
/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/


/*! *********************************************************************************
* @}
********************************************************************************** */
