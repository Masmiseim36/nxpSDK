/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       http_server.c
\brief      This is a public source file for the HTTP Server module. It contains
            the implementation of the interface functions.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "http_cfg.h"
#include "http.h"
#include "sockets.h"
#include "ip.h"
#include "ip_if_management.h"
#include "session.h"

#include <string.h>
#include <stdlib.h>

#include "FunctionLib.h"

#if HTTP_ENABLED
/*==================================================================================================
Private macros
==================================================================================================*/


/*==================================================================================================
Private type definitions
==================================================================================================*/

typedef struct httpSockInfo_tag
{
    ipAddr_t remoteIpAddr;
    httpQuery_t* pQuery[HTTP_QUERY_SIZE];
    int32_t  sockFd;
    char* pAbsPath;
    uint32_t absPathLen;
    bool_t bHostIncludedInUri;
    bool_t bCloseConnection;
    uint16_t remotePort;
    uint8_t nbQuery;
    uint8_t versionIndex;
}httpSockInfo_t;

typedef struct httpServerCfg_tag
{
    int32_t sockFd;
    taskMsgQueue_t acceptMsgQueue;
    uint32_t nbClientSock;
    httpSockInfo_t* apClientSockTbl[HTTP_SERVER_CONNECTION_TABLE_SIZE];
    char* pHostName;
    const uriTableEntry_t* pUriTbl[HTTP_URI_SIZE];
    uint32_t uriTblLen;
    uint16_t hostPort;
    bool_t   bUseSecurity;
}httpServerCfg_t;

typedef enum
{
    mHttpHost_c = 0U,
    mHttpUserAgent_c,
    mHttpAccept_c,
    mHttpConnection_c
}httpRequestIndex_t; /* Request index in mapHttpRequestHeaderList */

/*==================================================================================================
Private prototypes
==================================================================================================*/
static uint32_t HTTP_GetFreeEntryClientSocketTbl(void);
static uint32_t HTTP_GetEntryClientSocketTbl(int32_t sockFd);
static void HTTP_ServerDataIndHandler(void *sockFd);
static void HTTP_ServerProcessDataInd(uint32_t sockIndex, char* pHttpDataInd);
static httpStatus_t HTTP_ParseRequestLine(uint32_t sockIndex, char* pHttpDataInd,
                                                     uint32_t requestLineLength);
static void HTTP_SendResponse(uint32_t sockIndex, httpStatus_t status,
                              char* pResult, const uriTableEntry_t* pUriTableEntry);
static void HTTP_ProcessConnectionHeader(uint32_t sockIndex, char* pConnectionValue);
static httpStatus_t HTTP_CheckHostname(httpString_t hostname);

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
static httpServerCfg_t* mpHttpServerCfg = NULL;

/* Supported Schemes */
static const char* aHttpScheme[]=
{
    {"/",},
    {"http://"},
    {"https://"}
};

const char* mpHttpServer = {"Server:"};

// TBD: to be configured by the user
const char* mpHttpServerToken = {"Freescale IP server"};

const char* mapHttpRequestHeaderList[] =
{
    {"Host:"},      /* mHttpHost_c */
    {"User-Agent:"},
    {"Accept:"},
    {"Connection:"}
};
OSA_TASK_PROTO(HTTP_AcceptTask);
/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern taskMsgQueue_t mThreadStackMsgQueue;

/*==================================================================================================
Public functions
==================================================================================================*/
extern void HTTP_Uint32ToAscii(uint32_t value, char* pAsciiValue, uint8_t* pAsciiValueLen);
extern void HTTP_AddHeader(char** ppCrtPos, const char* headerType, const char* headerValue);

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
bool_t HTTP_StartServer
(
    const uriTableEntry_t * pHttpUriTable,
    uint32_t httpUriTableSize,
    uint16_t serverPort,
    bool_t bUseSecurity,
    httpServerType_t serverType,
    char* pHostName
)
{
    bool_t bRetValue = FALSE;
    int32_t status = 0U; /* Success status */
    uint32_t resourceIndex;
    uint32_t hostNameLen = strlen((char*)pHostName);
    sockaddrStorage_t* pLocalAddr = NULL;

    /* Initialize session task */
    Session_Init();

    /* Parameter validation */
    if((NULL == mpHttpServerCfg) && pHttpUriTable && pHostName &&
       (httpUriTableSize > 0U) && (httpUriTableSize <= HTTP_URI_SIZE))
    {
        mpHttpServerCfg = (httpServerCfg_t*)NWKU_MEM_BufferAllocForever(sizeof(httpServerCfg_t));

        FLib_MemSet(mpHttpServerCfg, 0U, sizeof(httpServerCfg_t));

        mpHttpServerCfg->hostPort = serverPort;

        mpHttpServerCfg->pHostName = NWKU_MEM_BufferAllocForever(hostNameLen);
        FLib_MemCpy(mpHttpServerCfg->pHostName, pHostName, hostNameLen);

        for(resourceIndex = 0U; resourceIndex < httpUriTableSize; resourceIndex ++)
        {
            mpHttpServerCfg->pUriTbl[resourceIndex] = &pHttpUriTable[resourceIndex];
        }

        mpHttpServerCfg->uriTblLen = httpUriTableSize;

        pLocalAddr = (sockaddrStorage_t*)NWKU_MEM_BufferAllocForever(sizeof(sockaddrStorage_t));

        /* Populate pLocalAddr */
        if((gHttpServerTypeIpv6Only_c == serverType) ||
           (gHttpServerTypeDualIpv6Ipv4_c == serverType))
        {
            /* IPv6 only of Dual IPv6 - IPv4 server */
            pLocalAddr->ss_family = AF_INET6;
            ((sockaddrIn6_t*)pLocalAddr)->sin6_port = serverPort;
            IP_AddrCopy((ipAddr_t*)pLocalAddr->ss_addr, &in6addr_any);
        }
        else
        {
            /* IPv4 only */
            pLocalAddr->ss_family = AF_INET;
            ((sockaddrIn_t*)pLocalAddr)->sin_port = serverPort;
            IP_AddrCopy((ipAddr_t*)pLocalAddr->ss_addr, &inaddr_any);
        }

        /* Create socket */
        mpHttpServerCfg->sockFd = socket(pLocalAddr->ss_family, SOCK_STREAM, IPPROTO_TCP);

        if(mpHttpServerCfg->sockFd > 0U)
        {
            if(gHttpServerTypeDualIpv6Ipv4_c == serverType)
            {
                /* Create seperate socket for IPv4 and IPv6 */
            }

            /* Bind the socket to local information */
            status = bind(mpHttpServerCfg->sockFd, pLocalAddr, sizeof(sockaddrStorage_t));

            if(0U == status)
            {
                status = listen(mpHttpServerCfg->sockFd, HTTP_SERVER_CONNECTION_TABLE_SIZE);

                if(0U == status)
                {
                    /* Create HTTP Accept task */
                    ListInit(&mpHttpServerCfg->acceptMsgQueue.msgQueue,HTTP_ACCEPT_TASK_MSG_QUEUE_SIZE);

                    mpHttpServerCfg->acceptMsgQueue.taskId = OSA_TaskCreate(OSA_TASK(HTTP_AcceptTask), (osaTaskParam_t)NULL);

                    bRetValue = TRUE;
                }
                else
                {
                    closesock(mpHttpServerCfg->sockFd);

                    MEM_BufferFree(mpHttpServerCfg);
                    mpHttpServerCfg = NULL;
                }
            }
            else
            {
                closesock(mpHttpServerCfg->sockFd);

                MEM_BufferFree(mpHttpServerCfg);
                mpHttpServerCfg = NULL;
            }
        }
        else
        {
            /* Socket can't be open */

            MEM_BufferFree(mpHttpServerCfg);
            mpHttpServerCfg = NULL;
        }

        MEM_BufferFree(pLocalAddr);

    }
    return bRetValue;
}

/*==================================================================================================
Private functions
==================================================================================================*/
/*!*************************************************************************************************
\private
\fn     static void HTTP_AcceptTask(void const *argument)

\brief  HTTP Accept task

\param [in]   argument    task private data

\param [out]  none

\retval       none
***************************************************************************************************/
void HTTP_AcceptTask
(
    osaTaskParam_t argument
)
{
    int32_t remoteSock;
    sockaddrStorage_t remoteAddr;
    uint32_t sockIndex;

    /* sin_port and sin6_port are on the same location in sockaddrStorage_t.
       We can use sockaddrIn6_t both for IPv6 and IPv4 */
    sockaddrIn6_t* pSockaddrIn6;

    while (1)
    {
        remoteSock = accept(mpHttpServerCfg->sockFd, &remoteAddr, sizeof(sockaddrStorage_t));

        if(remoteSock > 0)
        {
            if(mpHttpServerCfg->nbClientSock <= HTTP_SERVER_CONNECTION_TABLE_SIZE)
            {
               /* Add new client info in socket table */
                sockIndex = HTTP_GetFreeEntryClientSocketTbl();

                /* No need to check sockIndex because we know there are free entries in
                   mpHttpServerCfg->apClientSockTbl */

                mpHttpServerCfg->apClientSockTbl[sockIndex] = NWKU_MEM_BufferAllocForever(sizeof(httpSockInfo_t));

                if(mpHttpServerCfg->apClientSockTbl[sockIndex])
                {
                    /* Update client socket descriptor table */
                    mpHttpServerCfg->apClientSockTbl[sockIndex]->sockFd = remoteSock;

                    IP_AddrCopy(&mpHttpServerCfg->apClientSockTbl[sockIndex]->remoteIpAddr,
                                (ipAddr_t*)remoteAddr.ss_addr);

                    pSockaddrIn6 = (sockaddrIn6_t*)&remoteAddr;
                    mpHttpServerCfg->apClientSockTbl[sockIndex]->remotePort = pSockaddrIn6->sin6_port;

                    mpHttpServerCfg->nbClientSock ++;

                    Session_RegisterCb(remoteSock, HTTP_ServerDataIndHandler, &mThreadStackMsgQueue, mSessCbData_c);
                }
                //break; /* TBD HTTP: do not break if we can accept more clients */
                //while(1);
            }
            else
            {
                /* TBD nothing to do */
            }
        }

        /* For BareMetal break the while(1) after 1 run */
        if (gUseRtos_c == 0)
        {
            break;
        }

    }
}

/*!*************************************************************************************************
\private
\fn     static uint32_t HTTP_GetFreeEntryClientSocketTbl(void)

\brief  This function searches the first free entry in mpHttpServerCfg->apClientSockTbl

\param [in]   none

\param [out]  none

\retval       index of the free entry
***************************************************************************************************/
static uint32_t HTTP_GetFreeEntryClientSocketTbl
(
    void
)
{
    uint32_t iCount;

    for(iCount = 0U; iCount < HTTP_SERVER_CONNECTION_TABLE_SIZE; iCount ++)
    {
        if(NULL == mpHttpServerCfg->apClientSockTbl[iCount])
        {
            break;
        }
    }

    return iCount;
}

/*!*************************************************************************************************
\private
\fn     static uint32_t HTTP_GetEntryClientSocketTbl(int32_t sockFd)

\brief  This function searches a socket in mpHttpServerCfg->apClientSockTbl

\param [in]   sockFd   the file descriptor of the socket

\param [out]  none

\retval       index of the socket entry
***************************************************************************************************/
static uint32_t HTTP_GetEntryClientSocketTbl
(
    int32_t sockFd
)
{
    uint32_t iCount;

    for(iCount = 0U; iCount < HTTP_SERVER_CONNECTION_TABLE_SIZE; iCount ++)
    {
        if((mpHttpServerCfg->apClientSockTbl[iCount]) &&
           (mpHttpServerCfg->apClientSockTbl[iCount]->sockFd == sockFd))
        {
            break;
        }
    }

    return iCount;
}

/*!*************************************************************************************************
\private
\fn     static void HTTP_ServerDataIndHandler(void *sockFd)

\brief  This function is called when HTTP data is received in TCP

\param [in]   sockFd  the file descriptor of the socket on which data is received

\param [out]  none

\retval       none
***************************************************************************************************/
static void HTTP_ServerDataIndHandler
(
    void *pPacket
)
{
    sessionPacket_t *pSessionPacket = (sessionPacket_t*)pPacket;
    uint32_t clientSockIndex;

    /* Search sockFd in mpHttpServerCfg->apClientSockTbl */
    clientSockIndex = HTTP_GetEntryClientSocketTbl((int32_t)pSessionPacket->sockFd);

    if(clientSockIndex < HTTP_SERVER_CONNECTION_TABLE_SIZE)
    {
        /* Entry found */
        if(pSessionPacket->dataLen == 0)
        {
            /* Connection closed */
            mpHttpServerCfg->nbClientSock --;
            MEM_BufferFree(mpHttpServerCfg->apClientSockTbl[clientSockIndex]);
            mpHttpServerCfg->apClientSockTbl[clientSockIndex] = NULL;
        }
        else
        {
            /* Save '\0' inside the array in order to use string functions */
            pSessionPacket->pData[pSessionPacket->dataLen] = '\0';
            HTTP_ServerProcessDataInd(clientSockIndex, (char*)pSessionPacket->pData);
        }
    }

    MEM_BufferFree(pSessionPacket->pData);
    MEM_BufferFree(pSessionPacket);
}

/*!*************************************************************************************************
\private
\fn     static void HTTP_ServerProcessDataInd(uint32_t sockIndex, char* pHttpDataInd)

\brief  This function processes a received HTTP request

\param [in]   sockIndex      index of the socket on which data is received
\param [in]   pHttpDataInd   pointer to data

\param [out]  none

\retval       none
***************************************************************************************************/
static void HTTP_ServerProcessDataInd
(
    uint32_t sockIndex,
    char* pHttpDataInd
)
{
    httpStatus_t status = gHttp_ServerRes200_Ok_c;
    char* pSearchedChar;
    uint32_t requestLineLength;
    char* pResult = NULL;
    uriTableEntry_t* pUriTableEntry = NULL;
    char* pCrtPos = pHttpDataInd;
    uint32_t headerIndex;
    //uint32_t headerValueIndex;
    uint32_t resourceIndex;
    char* pAbsPath;
    int sockHow = 0U;
    httpString_t hostname;

    /* Check if CRLF is received (Mandatory HTTP/1.1 : Method SP Request-URI SP HTTP-Version CRLF) */

    /* Searching the first '\n' character is enough */
    pSearchedChar = strchr((const char*)pCrtPos, '\n');

    if(pSearchedChar)
    {
        requestLineLength = pSearchedChar - pCrtPos;

        /* Replace '\n' with '\0' */
        *pSearchedChar = '\0';

        status = HTTP_ParseRequestLine(sockIndex, pCrtPos, requestLineLength);

        if(gHttp_ServerRes200_Ok_c ==  status)
        {
            /* Continue with header processing */
            mpHttpServerCfg->apClientSockTbl[sockIndex]->bCloseConnection = FALSE;

            pCrtPos = pSearchedChar + 1;

            /* Determine where the first header ends */
            pSearchedChar = strchr((const char*)pCrtPos, '\n');

            /* Start processing the header(s) */
            while(pSearchedChar &&
                  //(pSearchedChar < (pHttpDataInd + httpDataLen)) &&
                  (gHttp_ServerRes200_Ok_c ==  status))
            {
                /* Replace '\n' with '\0' for string processing */
                *pSearchedChar = '\0';

                /* Jump over additional white spaces */
                while((*pCrtPos == ' ') &&
                      (*pCrtPos != '\0'))
                {
                    pCrtPos ++;
                }

                /* Check request header */
                for(headerIndex = 0U;
                    headerIndex < NumberOfElements(mapHttpRequestHeaderList) ;
                    headerIndex ++)
                {
                    if(FLib_MemCmp((void*)mapHttpRequestHeaderList[headerIndex],
                                   pCrtPos,
                                   strlen(mapHttpRequestHeaderList[headerIndex])))
                    {
                        pCrtPos += strlen(mapHttpRequestHeaderList[headerIndex]);

                        /* Jump over additional white spaces */
                        while((*pCrtPos == ' ') &&
                              (*pCrtPos != '\0'))
                        {
                            pCrtPos ++;
                        }

                        switch(headerIndex)
                        {
                            case mHttpHost_c:

                                if(FALSE == mpHttpServerCfg->apClientSockTbl[sockIndex]->bHostIncludedInUri)
                                {
                                    hostname.pstringValue = (uint8_t*)pCrtPos;

                                    status = HTTP_CheckHostname(hostname);
                                    mpHttpServerCfg->apClientSockTbl[sockIndex]->bHostIncludedInUri
                                    = TRUE;
                                }
                                break;

                            case mHttpConnection_c:

                                HTTP_ProcessConnectionHeader(sockIndex, pCrtPos);
                                break;

                            case mHttpAccept_c:

                                // TBD: process accept
                                break;

                            default:
                                break;
                        }

                        break;


                    }
                }

                pSearchedChar = strchr((const char*)pCrtPos, '\0');
                pCrtPos = pSearchedChar + 1;

                /* Restart header search */
                pSearchedChar = strchr((const char*)pCrtPos, '\n');

            }

            if(gHttp_ServerRes200_Ok_c ==  status)
            {
                /* Search resource in URI table */
                pAbsPath = mpHttpServerCfg->apClientSockTbl[sockIndex]->pAbsPath;
                for(resourceIndex = 0; ((resourceIndex < mpHttpServerCfg->uriTblLen) && (gHttp_ServerRes200_Ok_c ==  status)); resourceIndex ++)
                {
                    if(0U == strncmp((const char *)pAbsPath,
                                     (const char *)mpHttpServerCfg->pUriTbl[resourceIndex]->pRequestUri,
                                     mpHttpServerCfg->apClientSockTbl[sockIndex]->absPathLen))
                    {
                        /* Resource found */
                        pUriTableEntry = (uriTableEntry_t*)mpHttpServerCfg->pUriTbl[resourceIndex];

                        /* Check if query was present in the request */
                        if((mpHttpServerCfg->apClientSockTbl[sockIndex]->nbQuery) &&
                           (mpHttpServerCfg->pUriTbl[resourceIndex]->pQueryFunction))
                        {
                            /* Call query callback */
                            status = mpHttpServerCfg->pUriTbl[resourceIndex]->pQueryFunction(
                                           mpHttpServerCfg->apClientSockTbl[sockIndex]->nbQuery,
                                           *mpHttpServerCfg->apClientSockTbl[sockIndex]->pQuery);
                            // TBD: resulted URI (pResultUriContent)
                        }
                        else
                        {
                            pResult = mpHttpServerCfg->pUriTbl[resourceIndex]->pUriContent;
                        }

                        /* Check for dynamic data */
                        if(mpHttpServerCfg->pUriTbl[resourceIndex]->pHttpScriptFunction)
                        {
                            pResult = mpHttpServerCfg->pUriTbl[resourceIndex]->pHttpScriptFunction(
                                mpHttpServerCfg->pUriTbl[resourceIndex]->pUriContent);
                        }

                        // TBD: do not return any message body when HEAD is received
                    }
                    else
                    {
                        /* Resource not found */
                        status = gHttp_ServerRes404_NotFound_c;
                    }
                }
            }
        }
    }
    else
    {
        /* Wait until the mandatory part of the HTTP request header is received */
        // TBD: wait for more data

        status = gHttp_ServerRes413_RequestEntityTooLarge_c;
    }

    HTTP_SendResponse(sockIndex, status, pResult, pUriTableEntry);

    if(mpHttpServerCfg->apClientSockTbl[sockIndex] &&
       mpHttpServerCfg->apClientSockTbl[sockIndex]->bCloseConnection)
    {
        // TBD: set sockHow
        /* Close the connection after sending the data */
        shutdown(mpHttpServerCfg->apClientSockTbl[sockIndex]->sockFd, sockHow);

        /* TBD close here will trigger a reset -> need to update code with event indication to
           properly close connection */
        closesock(mpHttpServerCfg->apClientSockTbl[sockIndex]->sockFd);

        /* Free socket related info */
        MEM_BufferFree(mpHttpServerCfg->apClientSockTbl[sockIndex]);
        mpHttpServerCfg->apClientSockTbl[sockIndex] = NULL;

        mpHttpServerCfg->nbClientSock --;
    }
}

/*!*************************************************************************************************
\private
\fn     static httpStatus_t HTTP_ParseRequestLine(uint32_t sockIndex, char* pHttpDataInd,
                                                  uint32_t requestLineLength)

\brief  This function processes a request line

\param [in]   sockIndex          index of the socket on which data is received
\param [in]   pHttpDataInd       pointer to request line
\param [in]   requestLineLength  length of the request line

\param [out]  none

\retval       httpStatus_t
***************************************************************************************************/
static httpStatus_t HTTP_ParseRequestLine
(
    uint32_t sockIndex,
    char* pHttpDataInd,
    uint32_t requestLineLength
)
{
    httpStatus_t status = gHttp_ServerRes200_Ok_c;
    uint32_t methodIndex;
    uint32_t schemeIndex;
    char* pSearchedChar;
    httpString_t host;
    uint32_t versionIndex;
    uint32_t nbMethods = NumberOfElements(gapHttpMethods);
    uint32_t nbSchemes = NumberOfElements(aHttpScheme);
    uint32_t nbVersions = NumberOfElements(gapHttpVersion);
    char* pQueryCrt;
    uint32_t queryIndex = 0;
    char* pAbsPath = NULL;
    httpQuery_t* pQuery = NULL;
    uint32_t length;
    char* pCrtPos = pHttpDataInd;
    uint32_t absPathLen = 0U;

    mpHttpServerCfg->apClientSockTbl[sockIndex]->bHostIncludedInUri = FALSE;

    /* Test which the method is present */
    for(methodIndex = 0U; methodIndex < nbMethods; methodIndex ++)
    {
        length = strlen(gapHttpMethods[methodIndex]);
        if(0U == strncmp((const char *)pCrtPos, gapHttpMethods[methodIndex],
                         length))
        {
            break;
        }
    }

    if(methodIndex < nbMethods)
    {
        /* Method implemented */
        pCrtPos += length;

        /* Jump over additional white spaces */
        while((*pCrtPos == ' ') &&
              (*pCrtPos != '\0'))
        {
            pCrtPos ++;
        }

        /* Test which scheme is present */
        for(schemeIndex = 0U; schemeIndex < nbSchemes; schemeIndex ++)
        {
            length = strlen(aHttpScheme[schemeIndex]);
            if(0U == strncmp((const char *)pCrtPos, aHttpScheme[schemeIndex],
                             length))
            {
                break;
            }
        }

        if(schemeIndex < nbSchemes)
        {
            pAbsPath = pCrtPos;

            /* Scheme implemented */
            pCrtPos += length;

            /* Determine the remaining length of the RequestURI */
            pSearchedChar = strchr((const char*)pCrtPos, ' ');

            if(pSearchedChar && (pSearchedChar > pCrtPos))
            {
                /* absPath included in URI */
                if(0 == schemeIndex)
                {
                    /* Host not included in the RequestURI, only abs_path present */
                    /* Host header must be present */
                    absPathLen = 1U;
                }
                else
                {
                    /* Host included in the RequestURI */

                    /* Determine where the abs_path starts */
                    pSearchedChar = strchr((const char*)pCrtPos, '/');

                    /* abs_path starts immediately after '/'  */
                    pAbsPath = pSearchedChar + 1U;

                    host.pstringValue = (uint8_t*)pCrtPos;
                    host.strLength = pSearchedChar - pCrtPos;

                    /* Check if host port is included (search for ':') */
                    pSearchedChar = strchr((const char*)pCrtPos, ':');
                    if(pSearchedChar)
                    {
                        /* Update host length */
                        host.strLength = pSearchedChar - pCrtPos;
                    }

                    status = HTTP_CheckHostname(host);
                }

                /* Search for queries ('?') */
                pSearchedChar = strchr((const char*)pCrtPos, '?');
                if(pSearchedChar)
                {
                    /* Query found */

                    /* Save the length of the URI */
                    // TBD: verify this
                    //pAbsPath->strLength = pSearchedChar - pAbsPath->pstringValue;

                    /* Process query and extract parameters and their values */
                    pQueryCrt = pSearchedChar;

                    while((*pQueryCrt != ' ') &&
                          (*pQueryCrt != '\0') &&
                          (queryIndex < HTTP_QUERY_SIZE))
                    {
                        /* Read query parameter */
                        pSearchedChar = strchr((const char*)(pQueryCrt), '=');

                        if(pSearchedChar)
                        {
                            pQuery = mpHttpServerCfg->apClientSockTbl[sockIndex]->pQuery[queryIndex];

                            pQueryCrt = pSearchedChar + 1U;

                            /* Read query parameter value (this value may be missing) */
                            pSearchedChar = strchr((const char*)pQueryCrt, '&');

                            pQuery->queryParamValueLen = 0U;

                            if(pSearchedChar)
                            {
                                if(pSearchedChar > pQuery->pQueryParam)
                                {
                                    /* Query parameter value present */
                                    pQuery->pQueryParamValue = pQueryCrt;
                                    pQuery->queryParamValueLen = pSearchedChar - pQueryCrt;
                                }
                                else
                                {
                                    /* Query parameter value not present */
                                    pQuery->pQueryParamValue = NULL;
                                }
                            }

                            queryIndex ++;
                        }

                        pQueryCrt = pSearchedChar + 1U;
                    }
                }
                else
                {
                    /* No queries inside the request, search for ' ' */
                    pSearchedChar = strchr((const char*)pCrtPos, ' ');

                    if(pSearchedChar)
                    {
                        absPathLen = pSearchedChar - pAbsPath;

                        pCrtPos = pSearchedChar + 1;
                    }
                }

                mpHttpServerCfg->apClientSockTbl[sockIndex]->nbQuery = queryIndex;
            }
            else
            {
                pAbsPath = (char*)aHttpScheme[schemeIndex];
                absPathLen = 1U;

                /* Jump over additional white spaces */
                while((*pCrtPos == ' ') &&
                      (*pCrtPos != '\0'))
                {
                    pCrtPos ++;
                }
            }

            mpHttpServerCfg->apClientSockTbl[sockIndex]->pAbsPath = pAbsPath;
            mpHttpServerCfg->apClientSockTbl[sockIndex]->absPathLen = absPathLen;

            /* Check HTTP version */
            for(versionIndex = 0U; versionIndex < nbVersions; versionIndex ++)
            {
                length = strlen(gapHttpVersion[versionIndex]);

                if(0U == strncmp(gapHttpVersion[versionIndex], pCrtPos, length))
                {
                    break;
                }
            }

            if(versionIndex < nbVersions)
            {
                pCrtPos += length;

                mpHttpServerCfg->apClientSockTbl[sockIndex]->versionIndex = versionIndex;

                return status;
            }
            else
            {
                return gHttp_ServerRes505_VersionNotSupported_c;
            }
        }
    }

    return status;
}

/*!*************************************************************************************************
\private
\fn     static void HTTP_SendResponse(uint32_t sockIndex, httpStatus_t status, char* pResult,
                                      const uriTableEntry_t* pUriTableEntry)

\brief  This function sends an HTTP response

\param [in]   sockIndex       index of the socket on which data is received
\param [in]   status          status to be included in response
\param [in]   pResult         message body to be included in response (NULL is valid)
\param [in]   pUriTableEntry  pointer to the URI

\param [out]  none

\retval       none
***************************************************************************************************/
static void HTTP_SendResponse
(
    uint32_t sockIndex,
    httpStatus_t status,
    char* pResult,
    const uriTableEntry_t* pUriTableEntry
)
{
    char* pHttpResponse = NWKU_MEM_BufferAlloc(HTTP_MAX_RX_BUFFER_SIZE);
    uint32_t txFlags = MSG_DONTWAIT;
    uint32_t versionIndex = mpHttpServerCfg->apClientSockTbl[sockIndex]->versionIndex;
    uint32_t versionLen;
    char aItoaContentLen[4];
    uint8_t itoaContentLen;
    char* pCrtPos = pHttpResponse;
    uint32_t length;
    uint32_t resultSize = 0U;

    if(pResult)
    {
        resultSize = strlen(pResult);
    }

    if(versionIndex >= NumberOfElements(gapHttpVersion))
    {
        /* Use HTTP/1.1 default version for response */
        versionIndex = 0U;
    }

    versionLen = strlen(gapHttpVersion[versionIndex]);

    /* Add HTTP version */
    FLib_MemCpy(pCrtPos, (void*)gapHttpVersion[versionIndex], versionLen);
    pCrtPos += versionLen;

    /* Add SP */
    *pCrtPos = ' ';
    pCrtPos ++;

    /* Add Status-Code SP Reason-Phrase CRLF */
    length = strlen((char*)aHttpStatusReason[status].pstringValue);
    FLib_MemCpy(pCrtPos, aHttpStatusReason[status].pstringValue, length);
    pCrtPos += length;

    /* Always add Server header */
    HTTP_AddHeader(&pCrtPos,
                   (const char*)mpHttpServer,
                   (const char*)mpHttpServerToken);

    if(resultSize)
    {

        /* Always add Content Type header */
        HTTP_AddHeader(&pCrtPos,
                       (const char*)aHttpContentType.pstringValue,
                       (const char*)aHttpContentTypeValue[pUriTableEntry->mediaType].pstringValue);

        /* Convert content length to ASCII */
        HTTP_Uint32ToAscii(resultSize, aItoaContentLen, &itoaContentLen);

        /* Always add Content Length header */
        HTTP_AddHeader(&pCrtPos,
                       (const char*)aHttpContentLength.pstringValue,
                       (const char*)aItoaContentLen);

        /* Add the last '\r\n' */
        length = strlen(gpHttpNewLine);
        FLib_MemCpy(pCrtPos, (void*)gpHttpNewLine, length);
        pCrtPos += length;

        /* Add message body */
        FLib_MemCpy(pCrtPos, pResult, resultSize);

        MEM_BufferFree(pResult);

        pCrtPos += resultSize;
    }
    else
    {
        /* Add the last '\r\n' */
        length = strlen(gpHttpNewLine);
        FLib_MemCpy(pCrtPos, (void*)gpHttpNewLine, length);
        pCrtPos += length;
    }

    /* Send HTTP response to TCP */
    if(-1 == send(mpHttpServerCfg->apClientSockTbl[sockIndex]->sockFd, pHttpResponse, pCrtPos - pHttpResponse, txFlags))
    {
        /* Socket was closed, free the data */

        MEM_BufferFree(mpHttpServerCfg->apClientSockTbl[sockIndex]);

        mpHttpServerCfg->apClientSockTbl[sockIndex] = NULL;

        mpHttpServerCfg->nbClientSock --;
    }

    MEM_BufferFree(pHttpResponse);

}

/*!*************************************************************************************************
\private
\fn     static void HTTP_ProcessConnectionHeader(uint32_t sockIndex, char* pConnectionValue)

\brief  This function processes a HTTP Connection header

\param [in]   sockIndex         index of the socket on which data is received
\param [in]   pConnectionValue  pointer to connection token

\param [out]  none

\retval       none
***************************************************************************************************/
static void HTTP_ProcessConnectionHeader
(
    uint32_t sockIndex,
    char* pConnectionValue
)
{
    if(FLib_MemCmp(pConnectionValue, (void*)gpHttpConnectionClose, strlen(gpHttpConnectionClose)))
    {
         mpHttpServerCfg->apClientSockTbl[sockIndex]->bCloseConnection = TRUE;
    }
}

/*!*************************************************************************************************
\private
\fn     static httpStatus_t HTTP_CheckHostname(httpString_t hostname)

\brief  This function validates the server hostname

\param [in]   hostname received server hostname

\param [out]  none

\retval       httpStatus_t
***************************************************************************************************/
static httpStatus_t HTTP_CheckHostname
(
    httpString_t hostname
)
{
    char* pSearchedChar = NULL;
    httpStatus_t httpStatus = gHttp_ServerRes400_BadRequest_c;
    ipAddr_t hostIpAddr;
    int32_t  serverPort;
    int32_t ptonRes;

    /* Check if '[' is included in host name (server IPv6 address instead of server name) */
    pSearchedChar = strchr((const char*)(hostname.pstringValue), '[');
    if(pSearchedChar)
    {
        /* Server IPv6 address included */

        pSearchedChar = strchr((const char*)(hostname.pstringValue), ']');

        if(pSearchedChar)
        {
            *pSearchedChar = '\0';
        }

        {
            char *pTerminator = strchr((const char*)(hostname.pstringValue + 1), '\r');
            if(pTerminator)
            {
                *pTerminator = 0;
            }
            else
            {
                pTerminator = strchr((const char*)(hostname.pstringValue + 1), '\n');
                if(pTerminator)
                {
                    *pTerminator = 0;
                }
            }
        }
        ptonRes = pton(AF_INET6, (char*)(hostname.pstringValue + 1), &hostIpAddr);

        switch(ptonRes)
        {
            case 1U:
                /* Server IPv6 address */
                if(IP_IF_GetIfByAddr(&hostIpAddr))
                {
                    httpStatus = gHttp_ServerRes200_Ok_c;
                }
                break;

            default:
                break;
        }
    }
    else
    {
        /* Check if '.' is included in host name (server IPv4 address instead of server name) */
        pSearchedChar = strchr((const char*)hostname.pstringValue, '.');

        if(pSearchedChar)
        {
            /* Server IPv4 address, name or alias included */
            {
                char *pTerminator = strchr((const char*)(hostname.pstringValue), '\r');
                if(pTerminator)
                {
                    *pTerminator = 0;
                }
                else
                {
                    pTerminator = strchr((const char*)(hostname.pstringValue), '\n');
                    if(pTerminator)
                    {
                        *pTerminator = 0;
                    }
                }
            }
            ptonRes = pton(AF_INET, (char*)hostname.pstringValue, &hostIpAddr);

            switch(ptonRes)
            {
                case 1U:
                    /* Server IPv4 address, check if address is valid */
                    if(IP_IF_GetIfByAddr(&hostIpAddr))
                    {
                        httpStatus = gHttp_ServerRes200_Ok_c;
                    }
                    break;

                default:
                    /* Server name or alias included */

                    if(FLib_MemCmp(hostname.pstringValue, mpHttpServerCfg->pHostName,
                                   strlen(mpHttpServerCfg->pHostName)))
                    {
                        httpStatus = gHttp_ServerRes200_Ok_c;
                    }
                    break;
            }
        }
        else
        {
            /* Server name or alias included */

            if(FLib_MemCmp(hostname.pstringValue, mpHttpServerCfg->pHostName,
                           strlen(mpHttpServerCfg->pHostName)))
            {
                httpStatus = gHttp_ServerRes200_Ok_c;
            }
        }

        pSearchedChar = (char*)hostname.pstringValue;
    }

    /* Search if port is included */
    if(gHttp_ServerRes200_Ok_c == httpStatus)
    {
        pSearchedChar = strchr((const char*)(pSearchedChar + 1), ':');
        if(pSearchedChar)
        {
            /* Port included */
            serverPort = atoi((const char *)(pSearchedChar + 1));

            if(serverPort == mpHttpServerCfg->hostPort)
            {
                httpStatus = gHttp_ServerRes200_Ok_c;
            }
        }
    }

    return httpStatus;
}
#endif /* HTTP_ENABLED */
/*==================================================================================================
Private debug functions
==================================================================================================*/
