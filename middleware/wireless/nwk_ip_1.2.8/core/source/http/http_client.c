/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
 
/*!=================================================================================================
\file       http_client.c
\brief      This is a public source file for the HTTP Client module. It contains
            the implementation of the interface functions.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "network_utils.h"
#include "http.h"
#include "http_cfg.h"

#include "sockets.h"
#include "session.h"

#include "FunctionLib.h"

#include "TimersManager.h"

#include <string.h>
#include <stdlib.h>

#if HTTP_ENABLED
/*==================================================================================================
Private macros
==================================================================================================*/
#define HTTP_VERSION_INDEX 0U

/*==================================================================================================
Private type definitions
==================================================================================================*/

typedef struct httpClientCfg_tag
{
    int32_t sockFd;
    pResponseCallback responseCallback;
    taskMsgQueue_t* pTaskMsgQueue;
    tmrTimerID_t responseTimer;
    bool_t bCloseConnection;
}httpClientCfg_t;

/*==================================================================================================
Private prototypes
==================================================================================================*/
static void HTTP_AddQuery(char** ppCrtPos, httpQuery_t* pHttpQuery, uint32_t nbHttpQuery);
static uint32_t HTTP_ComputeQueryTotalLen(httpQuery_t* pHttpQuery, uint32_t nbHttpQuery);
static void HTTP_ClientReceiveInd(void* sockFd);
static void HTTP_ClientProcessDataInd(char* pHttpDataInd, uint32_t httpDataLen);
static void HTTP_ResponseTimeoutCb(void * sockFd);
static void HTTP_ResponseTimeout(void * sockFd);

/*==================================================================================================
Private global variables declarations
==================================================================================================*/


/*==================================================================================================
Public global variables declarations
==================================================================================================*/

httpClientCfg_t* mpHttpClientCfg = NULL;

/*==================================================================================================
Public functions
==================================================================================================*/
extern void HTTP_Uint32ToAscii(uint32_t value, char* pAsciiValue, uint8_t* pAsciiValueLen);
extern void HTTP_AddHeader(char** ppCrtPos, const char* headerType, const char* headerValue);

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
bool_t HTTP_StartClient
(
    ipAddr_t* pServerAddr,
    uint16_t serverPort,
    bool_t bUseSecurity,
    pResponseCallback responseCallback,
    taskMsgQueue_t *pTaskMsgQueue
)
{
    bool_t bRetValue = FALSE;
    sockaddrStorage_t* pRemSockAddr = NULL;
    uint32_t sockAddrStorageLen = sizeof(sockaddrStorage_t);

    if(pServerAddr && pTaskMsgQueue && (NULL == mpHttpClientCfg))
    {
        /* Allocate memory for mpHttpClientCfg  */
        mpHttpClientCfg = (httpClientCfg_t*)NWKU_MEM_BufferAlloc(sizeof(httpClientCfg_t));

        FLib_MemSet(mpHttpClientCfg, 0U, sizeof(httpClientCfg_t));

        mpHttpClientCfg->pTaskMsgQueue = pTaskMsgQueue;

        /* Allocate timer for waiting for a response */
        mpHttpClientCfg->responseTimer = TMR_AllocateTimer();

        if(gTmrInvalidTimerID_c != mpHttpClientCfg->responseTimer)
        {
            mpHttpClientCfg->responseCallback = responseCallback;

            pRemSockAddr = (sockaddrStorage_t*)NWKU_MEM_BufferAlloc(sockAddrStorageLen);
            FLib_MemSet(pRemSockAddr, 0U, sockAddrStorageLen);

            if(IP_IsAddrIPv4(pServerAddr))
            {
                /* IPv4 socket */
                pRemSockAddr->ss_family = AF_INET;
                ((sockaddrIn_t*)pRemSockAddr)->sin_port = serverPort;
            }
            else
            {
                /* IPv6 socket */
                pRemSockAddr->ss_family = AF_INET6;
                ((sockaddrIn6_t*)pRemSockAddr)->sin6_port = serverPort;
            }

            FLib_MemCpy(pRemSockAddr->ss_addr, pServerAddr, sizeof(ipAddr_t));

            /* Create socket */
            mpHttpClientCfg->sockFd = socket(pRemSockAddr->ss_family, SOCK_STREAM, IPPROTO_TCP);

            if(mpHttpClientCfg->sockFd)
            {
                if(0U == connect(mpHttpClientCfg->sockFd, pRemSockAddr, sockAddrStorageLen))
                {
                    bRetValue = TRUE;

                    Session_RegisterCb(mpHttpClientCfg->sockFd, HTTP_ClientReceiveInd,
                                       mpHttpClientCfg->pTaskMsgQueue, mSessCbData_c);
                }
            }

            MEM_BufferFree(pRemSockAddr);

            if(FALSE == bRetValue)
            {
                MEM_BufferFree(mpHttpClientCfg);
            }
        }
        else
        {
            MEM_BufferFree(mpHttpClientCfg);
            mpHttpClientCfg = NULL;
        }
    }
    return bRetValue;
}

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
bool_t HTTP_SendRequest
(
    httpMethod_t method,
    char* pRequestUri,
    char* pHostName,
    httpQuery_t* pHttpQuery,
    uint32_t nbHttpQuery
)
{
    bool_t bRetValue = FALSE;
    char* pHttpRequest = NULL;
    char* pCrtPos = NULL;
    uint32_t len = strlen(gapHttpMethods[method]);
    uint32_t txFlags = MSG_DONTWAIT;
    uint32_t queryTotalLen;
    char aItoaQueryLen[4];
    uint8_t itoaQueryLen;
    uint32_t requestUriLen = strlen((char*)pRequestUri);
    uint32_t hostNameLen = strlen((char*)pHostName);

    /* Parameter validation */
    if(mpHttpClientCfg && pRequestUri && pHostName)
    {
        // TODO: check how much of the buffer was set

        /* Allocate memory for HTTP request */
        pHttpRequest = NWKU_MEM_BufferAlloc(HTTP_MAX_RX_BUFFER_SIZE);

        pCrtPos = pHttpRequest;

        /* Add method to request */
        FLib_MemCpy(pCrtPos, (void*)gapHttpMethods[method], len);

        pCrtPos += len;

        /* Add request URI to request */
        FLib_MemCpy(pCrtPos, pRequestUri, requestUriLen);

        pCrtPos += requestUriLen;

        /* Check if queries are added to GET request */
        if(pHttpQuery && (gHttpMethodGet_c == method))
        {
            /* Add '?' to request */
            *pCrtPos = '?';
            pCrtPos ++;

            HTTP_AddQuery(&pCrtPos, pHttpQuery, nbHttpQuery);
        }

        /* Add ' ' before HTTP version */
        *pCrtPos = ' ';
        pCrtPos ++;

        len = strlen(gapHttpVersion[HTTP_VERSION_INDEX]);

        /* Add HTTP/1.1 version */
        FLib_MemCpy(pCrtPos, (void*)gapHttpVersion[HTTP_VERSION_INDEX], len);

        pCrtPos += len;

        /* Add \r\n after request line */
        len = strlen(gpHttpNewLine);
        FLib_MemCpy(pCrtPos, (void*)gpHttpNewLine, len);

        pCrtPos += len;

        /* Add Host header */
        HTTP_AddHeader(&pCrtPos, gpHttpHost, (const char*)pHostName);

        /* Add User agent header */
        HTTP_AddHeader(&pCrtPos, gpHttpUserAgent,
                       (const char*)aHttpUserAgentInfo.pstringValue);

#if HTTP_DISABLE_PERSISTENT_CONNECTIONS

        /* Add Connection header */
        HTTP_AddHeader(&pCrtPos, gpHttpConnection, gpHttpConnectionClose);

        mpHttpClientCfg->bCloseConnection = TRUE;

#endif /* HTTP_DISABLE_PERSISTENT_CONNECTIONS */

        // TODO: accept more than 1 entry in header
        /* Add Accept header */
        HTTP_AddHeader(&pCrtPos, gpHttpAccept, gapHttpAcceptMediaRange[0]);

        /* Add \r\n before inserting the message body */
        len = strlen(gpHttpNewLine);
        FLib_MemCpy(pCrtPos, (void*)gpHttpNewLine, len);

        pCrtPos += len;

        /* Check if queries are added to POST request */
        if(pHttpQuery && (gHttpMethodPost_c == method))
        {
            // TODO: add content and content length headers

            /* Compute query length */
            queryTotalLen = HTTP_ComputeQueryTotalLen(pHttpQuery, nbHttpQuery);

            /* Convert query length from uint32_t to ascii */
            HTTP_Uint32ToAscii(queryTotalLen, aItoaQueryLen, &itoaQueryLen);

            /* Add Content-Length header */
            HTTP_AddHeader(&pCrtPos, (const char*)aHttpContentLength.pstringValue,
                           (const char*)aItoaQueryLen);

            // TODO: accept more than 1 entry in header
            /* Add Content-Type header */
            HTTP_AddHeader(&pCrtPos, (const char*)aHttpContentType.pstringValue,
                           (const char*)aHttpContentTypeRange[0].pstringValue);

            HTTP_AddQuery(&pCrtPos, pHttpQuery, nbHttpQuery);
        }

        // TODO: txFlags

        if(-1 == send(mpHttpClientCfg->sockFd, pHttpRequest, pCrtPos - pHttpRequest, txFlags))
        {
            /* Socket was closed, free the data */

            TMR_FreeTimer(mpHttpClientCfg->responseTimer);
            MEM_BufferFree(mpHttpClientCfg);

            mpHttpClientCfg = NULL;
        }
        else
        {
            /* Start timer to wait the reply */
            TMR_StartSingleShotTimer(mpHttpClientCfg->responseTimer, HTTP_CLIENT_TIMEOUT_INTERVAL,
            HTTP_ResponseTimeoutCb, (void*)mpHttpClientCfg->sockFd);

            bRetValue = TRUE;
        }

        MEM_BufferFree(pHttpRequest);
    }
    return bRetValue;
}

/*==================================================================================================
Private functions
==================================================================================================*/
/*!*************************************************************************************************
\private
\fn     static void HTTP_AddQuery(char** ppCrtPos, httpQuery_t* pHttpQuery, uint32_t nbHttpQuery)

\brief  This function adds queries in HTTP request

\param [in]   ppCrtPos       current position in HTTP request buffer
\param [in]   pHttpQuery     pointer to query
\param [in]   nbHttpQuery    number of queries to be included in th request

\param [out]  ppCrtPos       current position in HTTP request buffer

\retval       none
***************************************************************************************************/
static void HTTP_AddQuery
(
    char** ppCrtPos,
    httpQuery_t* pHttpQuery,
    uint32_t nbHttpQuery
)
{
    uint32_t queryIndex;
    httpQuery_t* pCrtQuery;

    for(queryIndex = 0U; queryIndex < nbHttpQuery; queryIndex++)
    {
        if(queryIndex > 0U)
        {
            /* Add '&' to query */
            *(*ppCrtPos) = '&';
            (*ppCrtPos) ++;
        }

        pCrtQuery = &pHttpQuery[queryIndex];

        /* Add query parameter to request */
        FLib_MemCpy(*ppCrtPos, pCrtQuery->pQueryParam, pCrtQuery->queryParamLen);

        *ppCrtPos += pCrtQuery->queryParamLen;

        /* Add '=' to query */
        *(*ppCrtPos) = '=';
        (*ppCrtPos) ++;

        /* Add query parameter value to request */
        FLib_MemCpy(*ppCrtPos, pCrtQuery->pQueryParamValue, pCrtQuery->queryParamValueLen);

        *ppCrtPos += pCrtQuery->queryParamValueLen;
    }
}

/*!*************************************************************************************************
\private
\fn     static uint32_t HTTP_ComputeQueryTotalLen(httpQuery_t* pHttpQuery, uint32_t nbHttpQuery)

\brief  This function adds queries in HTTP request

\param [in]   pHttpQuery     pointer tp query list
\param [in]   nbHttpQuery    number of queries to be included in th request

\param [out]  none

\retval       total length of the queries included in request
***************************************************************************************************/
static uint32_t HTTP_ComputeQueryTotalLen
(
    httpQuery_t* pHttpQuery,
    uint32_t nbHttpQuery
)
{
    uint32_t queryIndex;
    uint32_t queryTotalLen = 0U;
    httpQuery_t* pCrtQuery = NULL;

    for(queryIndex = 0U; queryIndex < nbHttpQuery; queryIndex++)
    {
        if(queryIndex > 0U)
        {
            /* Add '&' to query */
            queryTotalLen ++;
        }

        pCrtQuery = &pHttpQuery[queryIndex];

        /* Add query parameter to request */
        queryTotalLen += pCrtQuery->queryParamLen;

        /* Add '=' to query */
        queryTotalLen ++;

        /* Add query parameter value to request */
        queryTotalLen += pCrtQuery->queryParamValueLen;
    }

    return queryTotalLen;
}

/*!*************************************************************************************************
\private
\fn     static void HTTP_ClientReceiveInd(void* sockFd)

\brief  This function is called when HTTP data is received in TCP

\param [in]   sockFd  the file descriptor of the socket on which data is received

\param [out]  none

\retval       none
***************************************************************************************************/
static void HTTP_ClientReceiveInd
(
    void* sockFd
)
{
    uint8_t* pHttpDataInd = NULL;
    uint32_t recvFlags = 0U;
    int32_t httpDataIndLen;
    int32_t optLen = 4;

    if(mpHttpClientCfg->sockFd == (int32_t)sockFd)
    {
        /* Received data from server */

        /* Get all pending packets from the RX queue */
        do
        {
            /* Get the size of the received packet. Size should be greater than 0 */
            getsockopt(mpHttpClientCfg->sockFd, SOL_SOCKET, SO_RCVBUF, (uint8_t*)&httpDataIndLen, (int32_t*)&optLen);

            if(!httpDataIndLen)
            {
                break;
            }

            /* Allocate memory for reception. Reserve the last byte for '\0' */
            pHttpDataInd = NWKU_MEM_BufferAlloc(httpDataIndLen);

            // TODO: set recvFlags parameter
            httpDataIndLen = recv(mpHttpClientCfg->sockFd, pHttpDataInd, httpDataIndLen, recvFlags);

            switch(httpDataIndLen)
            {
                case 0U:
                    /* Connection closed */
                    TMR_FreeTimer(mpHttpClientCfg->responseTimer);

                    MEM_BufferFree(mpHttpClientCfg);

                    break;

                case -1:
                    /* Error occured, just free the reception buffer */
                    break;

                default:
                    /* Process received data */

                    /* Save '\0' inside the array in order to use string functions */
                    pHttpDataInd[httpDataIndLen] = '\0';

                    HTTP_ClientProcessDataInd((char*)pHttpDataInd, httpDataIndLen + 1U);

                    // TODO: Is there any more data to receive? call recv again before processing
                    break;
            }

            MEM_BufferFree(pHttpDataInd);
        } while(httpDataIndLen);
    }
}

/*!*************************************************************************************************
\private
\fn     static void HTTP_ClientProcessDataInd(char* pHttpDataInd, uint32_t httpDataLen)

\brief  This function processes a received HTTP response

\param [in]   pHttpDataInd   pointer to data
\param [in]   httpDataLen    length of the data (including string terminator)

\param [out]  none

\retval       none
***************************************************************************************************/
static void HTTP_ClientProcessDataInd
(
    char* pHttpDataInd,
    uint32_t httpDataLen
)
{
    char* pSearchedChar;
    char* pCrtPos = pHttpDataInd;
    uint32_t versionIndex;
    uint32_t nbVersions = NumberOfElements(gapHttpVersion);
    int32_t statusCode;
    uint32_t contentLength = 0U;
    char* pMsgBodyStart = NULL;
    char* pAppDataInd = NULL;
    uint32_t appDataIndLen = 0;
    uint32_t length;
    char* pContentLength = NULL;
    char* pConnection = NULL;
    bool_t bCloseConnection = FALSE;

    /* Check if CRLF is received (HTTP-Version SP Status-Code SP Reason-Phrase CRLF) */

    /* Searching the first '\n' character is enough */
    pSearchedChar = strchr((const char*)pCrtPos, '\n');

    if(pSearchedChar)
    {
        /* Replace '\n' with '\0' */
        *pSearchedChar = '\0';

        /* Check HTTP version */
        for(versionIndex = 0U; versionIndex < nbVersions; versionIndex ++)
        {
            length = strlen(gapHttpVersion[versionIndex]);
            if(0U == strncmp(pCrtPos, gapHttpVersion[versionIndex], length))
            {
                break;
            }
        }

        if(versionIndex < nbVersions)
        {
            /* Known HTTP version */
            pCrtPos += length;

            /* Jump over additional white spaces */
            while((*pCrtPos == ' ') &&
                  (*pCrtPos != '\0'))
            {
                pCrtPos ++;
            }

            /* Check status code */
            statusCode = atoi(pCrtPos);

            switch(statusCode)
            {
                case 200U:
                case 201U:
                case 202U:
                    /* Sucess status, go to the end of status line */
                    pCrtPos += strlen((const char*)pCrtPos) + 1;

                    if((pCrtPos - pHttpDataInd) < httpDataLen)
                    {
                        /* There is more data to be processed */

                        /* Search for Content-Length header */
                        length = strlen((char*)aHttpContentLength.pstringValue);
                        pContentLength = strstr(pCrtPos, (char*)aHttpContentLength.pstringValue);
                        if(pContentLength)
                        {
                            /* Extract content length */
                            contentLength = atoi(pContentLength + length);

                        }

                        /* Search for '\r\n\r\n' to see where the message body starts */
                        pMsgBodyStart = strstr(pCrtPos, gpHttpStrEnd);
                    }
                    break;

                default:
                    /* Other status, free the data and inform the application */
                    break;
            }

            /* Search for Connection header */
            pConnection = strstr(pCrtPos, gpHttpConnection);
            if(pConnection)
            {
                /* Jump over additional white spaces */
                while((*pConnection == ' ') &&
                      (*pConnection != '\0'))
                {
                    pConnection ++;
                }

                /* Extract connection token */
                if(FLib_MemCmp(pConnection, (void*)gpHttpConnectionClose, strlen(gpHttpConnectionClose)))
                {
                    bCloseConnection = TRUE;
                }
            }

            /* Pass the data to application */
            if(mpHttpClientCfg->responseCallback)
            {
                if(pMsgBodyStart)
                {
                    /* Skip over \r\n\r\n */
                    pMsgBodyStart += strlen(gpHttpStrEnd);

                    appDataIndLen = httpDataLen - (pMsgBodyStart - pHttpDataInd);

                    if(contentLength == appDataIndLen)
                    {
                        pAppDataInd = NWKU_MEM_BufferAlloc(appDataIndLen + 1U);

                        FLib_MemCpy(pAppDataInd, pMsgBodyStart, appDataIndLen);

                        pAppDataInd[appDataIndLen] = '\0';

                        /* Call application callback for handling the response */
                        mpHttpClientCfg->responseCallback(statusCode, bCloseConnection, (char*)pAppDataInd);
                    }
                    else
                    {
                        /* Not enought data received */
                    }
                }
            }

            if(bCloseConnection)
            {
                // TODO: close connection
            }
        }
        else
        {
            /* HTTP version not supported */
        }
    }
    else
    {
        /* Wait for more data to come */
        // TODO:
    }
}

/*!*************************************************************************************************
\private
\fn     static void HTTP_ResponseTimeoutCb(void * sockFd)

\brief  This function is called when HTTP_CLIENT_TIMEOUT_INTERVAL interval for waiting the response
        expires

\param [in]   sockFd   the file descriptor of the socket

\param [out]  none

\retval       none
***************************************************************************************************/
static void HTTP_ResponseTimeoutCb

(
    void * sockFd
)
{
    NWKU_SendMsg(HTTP_ResponseTimeout, sockFd, mpHttpClientCfg->pTaskMsgQueue);
}

/*!*************************************************************************************************
\private
\fn     static void HTTP_ResponseTimeout(void * sockFd)

\brief  This function sends an error status code to application when no response is received from
        server

\param [in]   sockFd   the file descriptor of the socket

\param [out]  none

\retval       none
***************************************************************************************************/
static void HTTP_ResponseTimeout

(
    void * sockFd
)
{
    int32_t statusCode = 500U;

    /* No response was received from the server, send status code 500 (Internal Server Error) */
    mpHttpClientCfg->responseCallback(statusCode, FALSE, NULL);
}
#endif /*HTTP_ENABLED */
/*==================================================================================================
Private debug functions
==================================================================================================*/
