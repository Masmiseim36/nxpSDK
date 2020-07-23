/*! *********************************************************************************
* \defgroup HTTP Proxy Service
* @{
*********************************************************************************** */
/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _HTTP_PROXY_INTERFACE_H_
#define _HTTP_PROXY_INTERFACE_H_

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
typedef enum
{
    gHps_State_Idle_c = 0x00,
    gHps_State_Busy_c,
} hpsState_tag;

typedef uint8_t hpsState_t;

typedef enum
{
    gHps_HTTP_GET_Req_c = 0x01,
    gHps_HTTP_HEAD_Req_c,
    gHps_HTTP_POST_Req_c,
    gHps_HTTP_PUT_Req_c,
    gHps_HTTP_DELETE_Req_c,
    gHps_HTTPS_GET_Req_c,
    gHps_HTTPS_HEAD_Req_c,
    gHps_HTTPS_POST_Req_c,
    gHps_HTTPS_PUT_Req_c,
    gHps_HTTPS_DELETE_Req_c,
    gHps_HTTP_Req_Cancel_c,
    
} hpsHttpCommand_tag;

typedef uint8_t hpsHttpCommand_t;

typedef uint16_t hpsStatusCode_t;

typedef enum
{
    gHps_HeadersReceived_c      = 0x01,
    gHps_HeadersTruncated_c     = 0x02,
    gHps_BodyReceived_c         = 0x04,
    gHps_BodyTruncated_c        = 0x08,
} hpsDataStatus_tag;

typedef uint8_t hpsDataStatus_t;

typedef struct hpsHttpStatusCode_tag
{
    hpsStatusCode_t     statusCode;
    hpsDataStatus_t     dataStatus;
} hpsHttpStatusCode_t;

typedef enum
{
    gHps_UriSet         = 0x01,
    gHps_HeadersSet     = 0x02,
    gHps_BodySet        = 0x04
} hpsFlags_tag;

typedef uint8_t hpsFlags_t;

typedef enum
{
    gHpsErrCodeInvalidRequest_c                 = 0x81,
    gHpsErrCodeCccdImproperlyConfigured_c       = 0xFD,
    gHpsErrCodeProcedureAlreadyInProgress_c     = 0xFE,
} attErrorCode_tag;

/*! Pulse Oximeter Service - Configuration */
typedef struct hpsConfig_tag
{
    uint16_t            serviceHandle;
    uint8_t             uriLength;
    uint8_t*            uri;
    uint16_t            httpHeadersLength;
    uint8_t*            httpHeaders;
    uint16_t            httpBodyLength;
    uint8_t*            httpBody;
    hpsHttpStatusCode_t httpStatusCode;
    hpsHttpCommand_t    command;
    hpsState_t          state;
    hpsFlags_t          flags;
} hpsConfig_t;

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
* \brief        Starts HTTP Proxy Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hps_Start (hpsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops HTTP Proxy Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hps_Stop (hpsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the HTTP Proxy service
*
* \param[in]    clientdeviceId  Client Id in Device DB
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hps_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the HTTP Proxy service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hps_Unsubscribe();

/*!**********************************************************************************
* \brief        Handles write operations on the URI characteristic.
*
* \param[in]    pServiceConfig	Pointer to structure that contains server 
*                               configuration information.
*
* \param[in]    pEvent			Pointer to the GATT Server event.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t     Hps_URIWrittenHandler(hpsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

/*!**********************************************************************************
* \brief        Handles write operations on the HTTP Headers characteristic.
*
* \param[in]    pServiceConfig	Pointer to structure that contains server 
*                               configuration information.
*
* \param[in]    pEvent			Pointer to the GATT Server event.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t     Hps_HTTPHeadersWrittenHandler(hpsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

/*!**********************************************************************************
* \brief        Handles write operations on the HTTP Body characteristic.
*
* \param[in]    pServiceConfig	Pointer to structure that contains server 
*                               configuration information.
*
* \param[in]    pEvent			Pointer to the GATT Server event.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t     Hps_HTTPBodyWrittenHandler(hpsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

/*!**********************************************************************************
* \brief        Handles write operations on the HTTP CP characteristic.
*
* \param[in]    pServiceConfig	Pointer to structure that contains server 
*                               configuration information.
*
* \param[in]    pEvent			Pointer to the GATT Server event.
*
* \return       gHpsSuccess_c or error.
************************************************************************************/
uint16_t        Hps_HTTPControlPointHandler(hpsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

/*!**********************************************************************************
* \brief        Updates the HTTP Headers characteristic.
*
* \param[in]    pServiceConfig	Pointer to structure that contains server 
*                               configuration information.
*
* \param[in]    pEvent			Pointer to the GATT Server event.
*
* \return       gHpsSuccess_c or error.
************************************************************************************/
bleResult_t     Hps_UpdateHttpHeadersChar (hpsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Updates the HTTP Body characteristic.
*
* \param[in]    pServiceConfig	Pointer to structure that contains server 
*                               configuration information.
*
* \param[in]    pEvent			Pointer to the GATT Server event.
*
* \return       gHpsSuccess_c or error.
************************************************************************************/
bleResult_t     Hps_UpdateHttpBodyChar (hpsConfig_t *pServiceConfig);

#ifdef __cplusplus
}
#endif 

#endif /* _HTTP_PROXY_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */