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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"

#include "link_loss_interface.h"
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
/*! Link Loss Service - Subscribed Client*/
static deviceId_t mLls_SubscribedClientId = gInvalidDeviceId_c;

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
bleResult_t Lls_Start(llsConfig_t *pServiceConfig)
{
    Lls_SetAlertLevel(pServiceConfig->serviceHandle, pServiceConfig->initialAlertLevel);

    mLls_SubscribedClientId = gInvalidDeviceId_c;

    return gBleSuccess_c;
}

bleResult_t Lls_Stop(llsConfig_t *pServiceConfig)
{
    return gBleSuccess_c;
}

bleResult_t Lls_Subscribe(deviceId_t clientdeviceId)
{
    if (!mLls_SubscribedClientId)
        mLls_SubscribedClientId = clientdeviceId;

    return gBleSuccess_c;
}

bleResult_t Lls_Unsubscribe(void)
{
    mLls_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Lls_GetAlertLevel(uint16_t serviceHandle, llsAlertLevel_t *pOutAlertLevel)
{
    uint16_t  hAlertLevel;
    bleResult_t result;
    bleUuid_t uuid =  Uuid16(gBleSig_AlertLevel_d);
    uint16_t outLen = 0;

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &hAlertLevel);

    if (result != gBleSuccess_c)
        return result;

    return GattDb_ReadAttribute(hAlertLevel, sizeof(llsAlertLevel_t), pOutAlertLevel, &outLen);
}

bleResult_t Lls_SetAlertLevel(uint16_t serviceHandle, llsAlertLevel_t alertLevel)
{
    uint16_t  hAlertLevel;
    bleResult_t result;
    bleUuid_t uuid =  Uuid16(gBleSig_AlertLevel_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &hAlertLevel);

    if (result != gBleSuccess_c)
        return result;

    /* Write attribute value*/
    return GattDb_WriteAttribute(hAlertLevel, sizeof(llsAlertLevel_t), &alertLevel);
}
/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/


/*! *********************************************************************************
* @}
********************************************************************************** */
