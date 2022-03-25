/*! *********************************************************************************
* \addtogroup HID Service
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
#include "hid_interface.h"
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
/*! HID Service - Subscribed Client*/
static deviceId_t mHid_SubscribedClientId;
/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

static void Hid_SendReportNotifications(uint16_t handle);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
bleResult_t Hid_Start(hidConfig_t *pServiceConfig)
{
    mHid_SubscribedClientId = gInvalidDeviceId_c;
    (void)Hid_SetProtocolMode(pServiceConfig->serviceHandle, pServiceConfig->protocolMode);

    return gBleSuccess_c;
}

bleResult_t Hid_Stop(hidConfig_t *pServiceConfig)
{
    return Hid_Unsubscribe();
}

bleResult_t Hid_Subscribe(deviceId_t clientDeviceId)
{
    if (mHid_SubscribedClientId == gInvalidDeviceId_c)
    {
        mHid_SubscribedClientId = clientDeviceId;
    }

    return gBleSuccess_c;
}

bleResult_t Hid_Unsubscribe(void)
{
    mHid_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Hid_SetProtocolMode(uint16_t serviceHandle, hidProtocolMode_t protocolMode)
{
    uint16_t  hProtocolMode;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_ProtocolMode_d);

    /* Get characteristic handle */
    result = GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType16_c, &uuid, &hProtocolMode);

    if (result == gBleSuccess_c)
    {
        /* Write attribute value */
        result = GattDb_WriteAttribute(hProtocolMode, sizeof(hidProtocolMode_t), &protocolMode);
    }

    return result;
}

bleResult_t Hid_GetProtocolMode(uint16_t serviceHandle, hidProtocolMode_t *pOutProtocolMode)
{
    uint16_t  hProtocolMode;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_ProtocolMode_d);
    uint16_t outLen;

    /* Get characteristic handle */
    result = GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType16_c, &uuid, &hProtocolMode);

    if (result == gBleSuccess_c)
    {
        /* Read attribute value */
        result = GattDb_ReadAttribute(hProtocolMode, sizeof(hidProtocolMode_t), pOutProtocolMode, &outLen);
    }

    return result;
}

bleResult_t Hid_SendInputReport(uint16_t serviceHandle, uint16_t reportlen, void* pInReport)
{
    uint16_t  hReport;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_Report_d);

    /* Get characteristic handle */
    result = GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType16_c, &uuid, &hReport);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value and send notification */
        result = GattDb_WriteAttribute(hReport, reportlen, pInReport);

        if (result == gBleSuccess_c)
        {
            Hid_SendReportNotifications(hReport);
        }
    }

    return result;
}

bleResult_t Hid_SendBootMouseInputReport(uint16_t serviceHandle, uint16_t reportlen, void* pInReport)
{
    uint16_t  hReport;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_BootMouseInputReport_d);

    /* Get characteristic handle */
    result = GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType16_c, &uuid, &hReport);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value and send notification */
        result = GattDb_WriteAttribute(hReport, reportlen, pInReport);

        if (result == gBleSuccess_c)
        {
            Hid_SendReportNotifications(hReport);
        }
    }

    return result;
}


uint8_t Hid_ControlPointHandler(uint16_t serviceHandle, hidControlPointValues_t value)
{
    uint8_t retStatus = (uint8_t)gAttErrCodeNoError_c;

    switch (value)
    {
        case gHid_Suspend_c:
            break;

        case gHid_ExitSuspend_c:
            break;

        default:
            retStatus = (uint8_t)gAttErrCodeRequestNotSupported_c;
            break;
    }

    return retStatus;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static void Hid_SendReportNotifications
(
    uint16_t handle
)
{
    uint16_t  hCccd;
    bool_t isNotifActive;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) == gBleSuccess_c)
    {
        if (gBleSuccess_c == Gap_CheckNotificationStatus
            (mHid_SubscribedClientId, hCccd, &isNotifActive) &&
            TRUE == isNotifActive)
        {
            (void)GattServer_SendNotification(mHid_SubscribedClientId, handle);
        }
    }
}
/*! *********************************************************************************
* @}
********************************************************************************** */
