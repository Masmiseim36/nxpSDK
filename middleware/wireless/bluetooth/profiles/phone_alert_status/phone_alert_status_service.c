/*! *********************************************************************************
 * \addtogroup Phone Alert Server Service
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
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
#include "EmbeddedTypes.h"

#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"

#include "phone_alert_status_interface.h"

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

/*! Phone Alert Status Service - Subscribed Client */
static deviceId_t mPass_SubscribedClientId = gInvalidDeviceId_c;

/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

static bleResult_t Pass_GetAlertStatus(uint16_t serviceHandle, passAlertStatus_t* pAsValue);
static bleResult_t Pass_GetRingerSetting(uint16_t serviceHandle, passRingerSetting_t* pRsValue);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

bleResult_t Pass_Start(void)
{

    mPass_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}


bleResult_t Pass_Stop(void)
{
    return gBleSuccess_c;
}


bleResult_t Pass_Subscribe(deviceId_t clientdeviceId)
{
    mPass_SubscribedClientId = clientdeviceId;

    return gBleSuccess_c;
}


bleResult_t Pass_Unsubscribe(void)
{
    mPass_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}


bleResult_t Pass_RecordAlertStatus(uint16_t serviceHandle, passAlertStatus_t asValue)
{
    uint16_t    asUuid  = gBleSig_AlertStatus_d;
    uint16_t    asHandle;
    uint16_t    asCccdHandle;
    bool_t      bAsNotificationActive;
    bleResult_t result;
    
    if(gPass_AlertStatusMaxValue_c < asValue)
    {
        return gBleInvalidParameter_c;
    }
    
    /* Get handle of characteristic */
    result  = GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType16_c, (bleUuid_t*)&asUuid, &asHandle);
    
    if(gBleSuccess_c != result)
    {
        return result;
    }
    
    /* Write new characteristic value */
    result = GattDb_WriteAttribute(asHandle, sizeof(passAlertStatus_t), &asValue);
    
    if(gBleSuccess_c != result)
    {
        return result;
    }
    
    /* Get handle of CCCD */
    result = GattDb_FindCccdHandleForCharValueHandle(asHandle, &asCccdHandle);
    
    if(gBleSuccess_c != result)
    {
        return result;
    }
    
    /* Check Notification Status for the subscribed client */
    result = Gap_CheckNotificationStatus(mPass_SubscribedClientId, asCccdHandle, &bAsNotificationActive);
    
    if(gBleSuccess_c != result)
    {
        return result;
    }
    
    if(TRUE == bAsNotificationActive)
    {
        /* Notify new value to client */
        result = GattServer_SendInstantValueNotification(mPass_SubscribedClientId, asHandle, sizeof(passAlertStatus_t), (uint8_t*)asValue);
    }

    return result;
}


bleResult_t Pass_RecordRingerSetting(uint16_t serviceHandle, passRingerSetting_t rsValue)
{
    uint16_t    rsUuid  = gBleSig_RingerSetting_d;
    uint16_t    rsHandle;
    uint16_t    rsCccdHandle;
    bool_t      bRsNotificationActive;
    /* Get handle of characteristic */
    bleResult_t result  = GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType16_c, (bleUuid_t*)&rsUuid, &rsHandle);
    
    if(gBleSuccess_c != result)
    {
        return result;
    }
    
    /* Write new characteristic value */
    result = GattDb_WriteAttribute(rsHandle, sizeof(passRingerSetting_t), &rsValue);
    
    if(gBleSuccess_c != result)
    {
        return result;
    }
    
    /* Get handle of CCCD */
    result = GattDb_FindCccdHandleForCharValueHandle(rsHandle, &rsCccdHandle);
    
    if(gBleSuccess_c != result)
    {
        return result;
    }
    
    /* Check Notification Status for the subscribed client */
    result = Gap_CheckNotificationStatus(mPass_SubscribedClientId, rsCccdHandle, &bRsNotificationActive);
    
    if(gBleSuccess_c != result)
    {
        return result;
    }
    
    if(TRUE == bRsNotificationActive)
    {
        /* Notify new value to client */
        result = GattServer_SendInstantValueNotification(mPass_SubscribedClientId, rsHandle, sizeof(passRingerSetting_t), (uint8_t*)rsValue);
    }

    return result;
}


bleResult_t Pass_ControlPointHandler(uint16_t serviceHandle, gattServerAttributeWrittenEvent_t *pEvent, bool_t* pbOutRingerOn)
{
    passRingerControlPoint_t rcpValue     = pEvent->aValue[0];
    bool_t                   bUpdateState = FALSE;
    passRingerSetting_t      rsValue;
    /* Get current ringer setting */
    bleResult_t              result       = Pass_GetRingerSetting(serviceHandle, &rsValue);
    
    if(gBleSuccess_c != result)
    {
        return result;
    }
    
    switch(rcpValue)
    {
        case gPass_SilentMode_c:
        {
            if(gPass_RingerNormal_c == rsValue)
            {
                /* Go to silent mode */
                rsValue      = gPass_RingerSilent_c;
                bUpdateState = TRUE;
            }
        }
        break;

        case gPass_MuteOnce_c:
        {
            /* Go to silent mode without updating the state */
            rsValue = gPass_RingerSilent_c;
        }
        break;

        case gPass_CancelSilentMode_c:
        {
            if(gPass_RingerSilent_c == rsValue)
            {
                /* Go to normal mode */
                rsValue      = gPass_RingerNormal_c;
                bUpdateState = TRUE;
            }
        }
        break;

        default:
        {
        }
        break;
    }
    
    if(TRUE == bUpdateState)
    {
        passAlertStatus_t asValue;
        
        /* Get current alert status */
        result = Pass_GetAlertStatus(serviceHandle, &asValue);
    
        if(gBleSuccess_c != result)
        {
            return result;
        }
        
        if(gPass_RingerNormal_c == rsValue)
        {
            asValue |= gPass_RingerStateActive_c;
        }
        else
        {
            asValue &= ~gPass_RingerStateActive_c;
        }
        
        /* Record new alert status */
        result = Pass_RecordAlertStatus(serviceHandle, asValue);
   
        if(gBleSuccess_c != result)
        {
            return result;
        }
        
        /* Record new ringer setting */
        result = Pass_RecordRingerSetting(serviceHandle, rsValue);
    }
    
    if(gBleSuccess_c == result)
    {
        *pbOutRingerOn = (gPass_RingerNormal_c == rsValue) ? TRUE : FALSE;
    }
    
    return result;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

static bleResult_t Pass_GetAlertStatus(uint16_t serviceHandle, passAlertStatus_t* pAsValue)
{
    uint16_t    asUuid  = gBleSig_AlertStatus_d;
    uint16_t    asHandle;
    uint8_t     asValueLength;
    /* Get handle of characteristic */
    bleResult_t result  = GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType16_c, (bleUuid_t*)&asUuid, &asHandle);
    
    if(gBleSuccess_c != result)
    {
        return result;
    }
    
    /* Read characteristic value */
    return GattDb_ReadAttribute(asHandle, sizeof(passAlertStatus_t), &asValueLength, pAsValue);
}


static bleResult_t Pass_GetRingerSetting(uint16_t serviceHandle, passRingerSetting_t* pRsValue)
{
    uint16_t    rsUuid  = gBleSig_RingerSetting_d;
    uint16_t    rsHandle;
    uint8_t     rsValueLength;
    /* Get handle of characteristic */
    bleResult_t result  = GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType16_c, (bleUuid_t*)&rsUuid, &rsHandle);
    
    if(gBleSuccess_c != result)
    {
        return result;
    }
    
    /* Read characteristic value */
    return GattDb_ReadAttribute(rsHandle, sizeof(passRingerSetting_t), &rsValueLength, pRsValue);
}

/*! *********************************************************************************
* @}
********************************************************************************** */
