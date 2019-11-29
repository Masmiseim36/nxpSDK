/*! *********************************************************************************
 * \addtogroup Immediate Alert Service
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

#include "immediate_alert_interface.h"
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
/*! Immediate Alert Service - Subscribed Client*/
static deviceId_t mIas_SubscribedClientId = gInvalidDeviceId_c;

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
bleResult_t Ias_Start(iasConfig_t *pServiceConfig)
{
    Ias_SetAlertLevel(pServiceConfig->serviceHandle, pServiceConfig->initialAlertLevel);

    mIas_SubscribedClientId = gInvalidDeviceId_c;

    return gBleSuccess_c;
}

bleResult_t Ias_Stop(iasConfig_t *pServiceConfig)
{
    return gBleSuccess_c;
}

bleResult_t Ias_Subscribe(deviceId_t clientdeviceId)
{
    if (!mIas_SubscribedClientId)
        mIas_SubscribedClientId = clientdeviceId;

    return gBleSuccess_c;
}

bleResult_t Ias_Unsubscribe(void)
{
    mIas_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Ias_GetAlertLevel(uint16_t serviceHandle, iasAlertLevel_t *pOutAlertLevel)
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

    return GattDb_ReadAttribute(hAlertLevel, sizeof(iasAlertLevel_t), pOutAlertLevel, &outLen);
}

bleResult_t Ias_SetAlertLevel(uint16_t serviceHandle, iasAlertLevel_t alertLevel)
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
    return GattDb_WriteAttribute(hAlertLevel, sizeof(iasAlertLevel_t), &alertLevel);
}
/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/


/*! *********************************************************************************
* @}
********************************************************************************** */
