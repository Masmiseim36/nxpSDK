/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _HTTP_H
#define _HTTP_H
/*!=================================================================================================
\file       http.h
\brief      This is a header file for the HTTP module. It contains the declarations of
            the interface functions.
==================================================================================================*/


/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "network_utils.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#define HTTP_MAX_RX_BUFFER_SIZE 1536U

/*==================================================================================================
Public type definitions
==================================================================================================*/

typedef struct httpQuery_tag
{
    char* pQueryParam;
    uint32_t queryParamLen;
    char* pQueryParamValue;
    uint32_t queryParamValueLen;
}httpQuery_t;


/*Server status error response*/
typedef enum httpStatus_tag
{
  gHttp_ServerRes200_Ok_c = 0,
  gHttp_ServerRes201_Created_c,
  gHttp_ServerRes204_NoContent_c,
  gHttp_ServerRes400_BadRequest_c,
  gHttp_ServerRes403_Forbidden_c,
  gHttp_ServerRes404_NotFound_c,
  gHttp_ServerRes405_MethodNotAllowed_c,
  gHttp_ServerRes411_LengthRequired_c,
  gHttp_ServerRes413_RequestEntityTooLarge_c,
  gHttp_ServerRes415_UnsupportedMediaType_c,
  gHttp_ServerRes500_InternalServerError_c,
  gHttp_ServerRes501_NotImplemented_c,
  gHttp_ServerRes505_VersionNotSupported_c

} httpStatus_e;
typedef uint8_t httpStatus_t;

typedef enum
{
    gHttpRequestHeaderMask_c = 1U,
    gHttpResponseHeaderMask_c = 2U
}httpHeaderType_t;

/* Known types of URI */
typedef enum httpMediaType_tag
{
  gHttpUriType_notPresent_c = 0U,
  gHttpUriType_textplain_c,
  gHttpUriType_texthtml_c,
  gHttpUriType_imagegif_c,
  gHttpUriType_NotKnown_c = 0xFF
}httpMediaType_t;

typedef enum
{
    gHttpServerTypeIpv6Only_c = 1U,
    gHttpServerTypeIpv4Only_c = 2U,
    gHttpServerTypeDualIpv6Ipv4_c = 4U
}httpServerType_t;

/* Http strings structure */
typedef struct httpString_tag
{
  uint8_t* pstringValue;
  uint32_t strLength;
} httpString_t;

typedef struct httpQueryString_tag
{
    uint32_t param;
    int32_t  value;
}httpQueryString_t;

typedef httpStatus_t (*pHttpQueryFunction_t)(uint32_t nbQuery, httpQuery_t* pQueryList
// TODO: pointer to resulted URI
);

typedef char* (*pHttpScriptFunction_t)(char *pData);

typedef struct uriTableEntry_tag
{
    uint8_t*              pRequestUri;
    //uint32_t              requestUriLen;
    httpMediaType_t       mediaType;
    void*                 pUriContent;
    //uint32_t              uriContentLen;
    pHttpQueryFunction_t  pQueryFunction; //Query function with query string
    pHttpScriptFunction_t pHttpScriptFunction; // Used for updating dinamic data
}uriTableEntry_t;


typedef struct httpHeader_tag
{
    const char* pHttpHeaderType;
    uint32_t headerTypeNb;
    const char* pHttpHeaderValue;
    uint32_t headerValueNb;
}httpHeader_t;

typedef httpStatus_t (*pResponseCallback)(uint32_t status, bool_t bCloseConnection, char* pMessageBody);

typedef enum
{
    gHttpMethodGet_c = 0U,
    gHttpMethodHead_c,
    gHttpMethodPost_c,
    gHttpMethodPut_c,
    gHttpMethodDelete_c
}httpMethod_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern const httpHeader_t aHttpHeaderList[14];
extern const httpString_t aHttpContentType;
extern const httpString_t      aHttpContentTypeValue[4U];
extern const httpString_t aHttpContentLength;
extern const char* gapHttpMethods[5U];
extern const char* gapHttpVersion[2U];
extern const char* gpHttpNewLine;
extern const char* gpHttpHost;
extern const char* gpHttpUserAgent;
extern const httpString_t aHttpUserAgentInfo;
extern const httpString_t aHttpStatusReason[13];
extern const char* gpHttpStrEnd;
extern const char* gpHttpAccept;
extern const char* gapHttpAcceptMediaRange[1];
extern const char* gpHttpConnection;
extern const httpString_t aHttpContentTypeRange[1];
extern const char* gpHttpConnectionClose;

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     bool_t HTTP_StartServer(const uriTableEntry_t * pHttpUriTable, uint32_t httpUriTableSize,
                                uint16_t serverPort, bool_t bUseSecurity,
                                httpServerType_t serverType, char* pHostName)

\brief  This function starts the HTTP server

\param [in]   pHttpUriTable      pointer to the URI table
\param [in]   httpUriTableSize   number of entries in URI table
\param [in]   serverPort         server port
\param [in]   bUseSecurity       enable/disable HTTPS
\param [in]   serverType         IPv6 only, IPv4 only, dual IPv6 - IPv4
\param [in]   pHostName          server name/alias/address

\retval TRUE    returned when the HTTP Server is started
\retval FALSE   otherwise
***************************************************************************************************/
bool_t HTTP_StartServer(const uriTableEntry_t * pHttpUriTable, uint32_t httpUriTableSize,
                        uint16_t serverPort, bool_t bUseSecurity, httpServerType_t serverType,
                        char* pHostName);

/*!*************************************************************************************************
\fn     bool_t HTTP_StartClient(ipAddr_t* pServerAddr, uint16_t serverPort, bool_t bUseSecurity,
                                pResponseCallback responseCallback, taskMsgQueue_t *pTaskMsgQueue)

\brief  This function starts the HTTP client

\param [in]   pServerAddr        IP address of the server (using IPv6 format even for IPv4 address)
\param [in]   serverPort         server port
\param [in]   bUseSecurity       enable/disable HTTPS
\param [in]   responseCallback   callback used for returning HTTP message body to application
\param [in]   pTaskMsgQueue      pointer to the task message queue

\retval TRUE    returned when the HTTP Client is started
\retval FALSE   otherwise
***************************************************************************************************/
bool_t HTTP_StartClient(ipAddr_t* pServerAddr, uint16_t serverPort, bool_t bUseSecurity,
                        pResponseCallback responseCallback, taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn     bool_t HTTP_SendRequest(httpMethod_t method, char* pRequestUri, char* pHostName,
                                httpQuery_t* pHttpQuery, uint32_t nbHttpQuery)

\brief  This function sends an HTTP request

\param [in]   method        HTTP method
\param [in]   pRequestUri   request URI
\param [in]   pHostName     server name/alias/address
\param [in]   pHttpQuery    HTTP queries added in request
\param [in]   nbHttpQuery   number of HTTP quesries

\retval TRUE    returned after the request is sucessfully sent
\retval FALSE   otherwise
***************************************************************************************************/
bool_t HTTP_SendRequest(httpMethod_t method, char* pRequestUri, char* pHostName,
                        httpQuery_t* pHttpQuery, uint32_t nbHttpQuery);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _HTTP_H */
