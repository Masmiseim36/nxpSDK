/*! *********************************************************************************
* \addtogroup HTTP Proxy Service
* @{
 ********************************************************************************** */
/*! *********************************************************************************
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
#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"
#include "MemManager.h"

#include "http_proxy_interface.h"

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
#define MAX_URI_LENGTH 512
#define MAX_HEADER_LENGTH 512
#define MAX_BODY_LENGTH 512
/***********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */

/***********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
/*! HTTP Proxy Service - Subscribed Client */
static deviceId_t mHps_SubscribedClientId = gInvalidDeviceId_c;

/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
********************************************************************************** */
static void Hps_SendStatusNotification (hpsConfig_t *pServiceConfig);

/***********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
bleResult_t Hps_Start (hpsConfig_t *pServiceConfig)
{
    mHps_SubscribedClientId 	= gInvalidDeviceId_c;
    pServiceConfig->uri         = (uint8_t*) MEM_BufferAlloc(MAX_URI_LENGTH);
    pServiceConfig->httpHeaders = (uint8_t*) MEM_BufferAlloc(MAX_HEADER_LENGTH);
    pServiceConfig->httpBody    = (uint8_t*) MEM_BufferAlloc(MAX_BODY_LENGTH);
    return gBleSuccess_c;
}

bleResult_t Hps_Stop (hpsConfig_t *pServiceConfig)
{    
    return Hps_Unsubscribe();
}

bleResult_t Hps_Subscribe(deviceId_t clientdeviceId)
{
    mHps_SubscribedClientId = clientdeviceId;
    return gBleSuccess_c;
}

bleResult_t Hps_Unsubscribe()
{
    mHps_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Hps_URIWrittenHandler(hpsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_URI_d);
	
    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);
	
    if (result != gBleSuccess_c)
        return result;
	
    /* Save URI and its length */
    pServiceConfig->uri         = pEvent->aValue;
    pServiceConfig->uriLength   = pEvent->cValueLength;
	
    /* Update characteristic value */
    result = GattDb_WriteAttribute(handle, pServiceConfig->uriLength, pServiceConfig->uri);

    if (result != gBleSuccess_c)
        return result;
    
    /* Update the flags */
    pServiceConfig->flags |= gHps_UriSet;
    
    /* Send attribute writtent status */
    result = GattServer_SendAttributeWrittenStatus(mHps_SubscribedClientId, pEvent->handle, gAttErrCodeNoError_c);
    
    return result;
}

bleResult_t Hps_HTTPHeadersWrittenHandler(hpsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_HTTP_Headers_d);
	
    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);
	
    if (result != gBleSuccess_c)
        return result;
	
    /* Save headers and the length */
    pServiceConfig->httpHeaders         = pEvent->aValue;
    pServiceConfig->httpHeadersLength   = pEvent->cValueLength;
	
    /* Update characteristic value */
    result = GattDb_WriteAttribute(handle, pServiceConfig->httpHeadersLength, pServiceConfig->httpHeaders);

    if (result != gBleSuccess_c)
        return result;
    
    /* Update the flags */
    pServiceConfig->flags |= gHps_HeadersSet;
    
    /* Send attribute writtent status */
    result = GattServer_SendAttributeWrittenStatus(mHps_SubscribedClientId, pEvent->handle, gAttErrCodeNoError_c);
    
    return result;
}

bleResult_t Hps_HTTPBodyWrittenHandler(hpsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_HTTP_EntityBody_d);
	
    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);
	
    if (result != gBleSuccess_c)
        return result;
	
    /* Save HTTP body and its length */
    pServiceConfig->httpBody            = pEvent->aValue;
    pServiceConfig->httpBodyLength      = pEvent->cValueLength;
	
    /* Update characteristic value */
    result = GattDb_WriteAttribute(handle, pServiceConfig->httpBodyLength, pServiceConfig->httpBody);

    if (result != gBleSuccess_c)
        return result;
    
    /* Update the flags */
    pServiceConfig->flags |= gHps_BodySet;
    
    /* Send attribute writtent status */
    result = GattServer_SendAttributeWrittenStatus(mHps_SubscribedClientId, pEvent->handle, gAttErrCodeNoError_c);
    
    return result;
}

uint16_t Hps_HTTPControlPointHandler(hpsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    bleResult_t result = gBleSuccess_c;
    uint16_t  handle;
    uint16_t  hCccd;
    bool_t isNotifActive;
    bleUuid_t uuid = Uuid16(gBleSig_HTTP_StatusCode_d);
    
    /* Save the command */
    pServiceConfig->command = pEvent->aValue[0];
    
    /* If one of the characteristic isn't written then send Invalid Request error */
    if(pServiceConfig->flags != (gHps_UriSet | gHps_HeadersSet | gHps_BodySet))
    {
        result = GattServer_SendAttributeWrittenStatus(mHps_SubscribedClientId, pEvent->handle, gHpsErrCodeInvalidRequest_c);
        return 0x01;
    }
    else 
    {
	/* Verify if the command received is valid - if not, send Invalid Request error */
        if((pServiceConfig->command > gHps_HTTP_Req_Cancel_c) || 
           (pServiceConfig->command == 0x00))
        {
            result = GattServer_SendAttributeWrittenStatus(mHps_SubscribedClientId, pEvent->handle, gHpsErrCodeInvalidRequest_c);
            return 0x01;
        }
        else
        {
            /* Get handle of characteristic */
            result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
                gBleUuidType16_c, &uuid, &handle);

            /* Test if client is valid */
            if (mHps_SubscribedClientId == gInvalidDeviceId_c)
              return 0x01;

            /* Get CCCD handle */
            if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) != gBleSuccess_c)
              return 0x01;

            /* Check if notifications are on - if not, send CCCD Improperly Configured error */
            if (!(gBleSuccess_c == Gap_CheckNotificationStatus
              (mHps_SubscribedClientId, hCccd, &isNotifActive) &&
              TRUE == isNotifActive))
            {
                GattServer_SendAttributeWrittenStatus(mHps_SubscribedClientId, pEvent->handle, gHpsErrCodeCccdImproperlyConfigured_c);
            }
            else
            {
		/* Check if there is already a procedure running */
                if(pServiceConfig->state == gHps_State_Busy_c)
                {
                    result = GattServer_SendAttributeWrittenStatus(mHps_SubscribedClientId, pEvent->handle, gHpsErrCodeProcedureAlreadyInProgress_c);
                }
                else
                {
                    result = GattServer_SendAttributeWrittenStatus(mHps_SubscribedClientId, pEvent->handle, gAttErrCodeNoError_c);
                }
            }
        }
    }
    return result;
}

bleResult_t Hps_UpdateHttpHeadersChar (hpsConfig_t *pServiceConfig)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_HTTP_Headers_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);
    
    if (result != gBleSuccess_c)
        return result;
    
    /* Update characteristic value */
    if((pServiceConfig->httpHeadersLength) > 512)
    {
        result = GattDb_WriteAttribute(handle, 512, pServiceConfig->httpHeaders);
        pServiceConfig->httpStatusCode.dataStatus |= gHps_HeadersTruncated_c;
    }
    else
    {
        result = GattDb_WriteAttribute(handle, pServiceConfig->httpHeadersLength, pServiceConfig->httpHeaders);
        pServiceConfig->httpStatusCode.dataStatus &= ~gHps_HeadersTruncated_c;
    }
    
    /* Update status flags and send the notification */
    pServiceConfig->httpStatusCode.dataStatus |= gHps_HeadersReceived_c;
    
    Hps_SendStatusNotification(pServiceConfig);
    
    return result;
}

bleResult_t Hps_UpdateHttpBodyChar (hpsConfig_t *pServiceConfig)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_HTTP_EntityBody_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);
    
    if (result != gBleSuccess_c)
        return result;
    
    /* Update characteristic value */
    if((pServiceConfig->httpBodyLength) > 512)
    {
        result = GattDb_WriteAttribute(handle, 512, pServiceConfig->httpBody);
        pServiceConfig->httpStatusCode.dataStatus |= gHps_BodyTruncated_c;
    }
    else
    {
        result = GattDb_WriteAttribute(handle, pServiceConfig->httpBodyLength, pServiceConfig->httpBody);
        pServiceConfig->httpStatusCode.dataStatus &= ~gHps_BodyTruncated_c;
    }
    
    /* Update status flags and send the notification */
    pServiceConfig->httpStatusCode.dataStatus |= gHps_BodyReceived_c;
    
    Hps_SendStatusNotification(pServiceConfig);
    
    return result;
}

/***********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
static void Hps_SendStatusNotification (hpsConfig_t *pServiceConfig)
{
    bleResult_t result;
    uint16_t  handle;
    uint16_t  hCccd;
    bool_t isNotifActive;
    bleUuid_t uuid = Uuid16(gBleSig_HTTP_StatusCode_d);
    
    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
        gBleUuidType16_c, &uuid, &handle);
    
    /* Update status code characteristic */
    result = GattDb_WriteAttribute(handle, sizeof(hpsHttpStatusCode_t)-1, (uint8_t*)&pServiceConfig->httpStatusCode);
    
    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) != gBleSuccess_c)
        return;

    if (mHps_SubscribedClientId == gInvalidDeviceId_c)
      return;

    /* Check if notifications are enabled and send the notification */
    if (gBleSuccess_c == Gap_CheckNotificationStatus
        (mHps_SubscribedClientId, hCccd, &isNotifActive) &&
        TRUE == isNotifActive)
    {
        GattServer_SendNotification(mHps_SubscribedClientId, handle);
    }

}


/*! *********************************************************************************
* @}
********************************************************************************** */
