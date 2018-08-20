/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef _DNS_CLIENT_H
#define _DNS_CLIENT_H

/*!
\file       dns_client.h
\brief      This is a public header file for the DNS module. 
*/


/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "dns_utils.h"
#include "sockets.h"
#include "network_utils.h"
#include "GenericList.h"


/*==================================================================================================
Public macros
==================================================================================================*/
#ifndef DNS_ENABLED
    #define DNS_ENABLED                  1
#endif

/*! Description of the purpose or use of this macro, if needed. */
#define DNS_TIMER_PERIOD_MS      1000U                      /*!< Timer interval in milliseconds */

/*==================================================================================================
Public type definitions
==================================================================================================*/

typedef struct addrinfo_tag
{
    uint32_t              ai_flags;
    uint32_t              ai_family;
    uint32_t              ai_socktype;
    uint32_t              ai_protocol;
    socklen_t             ai_addrlen;
    sockaddrStorage_t     *ai_addr;
    uint8_t               *ai_canonname;

} addrinfo_t;

typedef struct hostent_tag
{
    list_t    *pHostAddrList;       /* list of addresses */
    list_t    *pHostAliasesList;         /* alias list */
    uint8_t   *pHostName;            /* official name of host */
    uint8_t    hostAddrType;        /* host address type */
} hostent_t;

typedef void (*appReturnHandler_t)(void* pMsg);

typedef struct dnsRequestEntry_tag
{
    uint32_t expirationTimestamp;
    appReturnHandler_t pfAppReturn;
    taskMsgQueue_t *pTaskMsgQueue;
    uint16_t id;
    uint16_t type;
} dnsRequestEntry_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void DNS_ClientInit(taskMsgQueue_t *pTaskMsgQueue)
\brief  This function initializes the DNS client.

\param  [in]  pTaskMsgQueue  Pointer to message task queue.
***************************************************************************************************/
void DNS_ClientInit(taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn     void DNS_Client_Service(void *pInData)
\brief  This function parses an incoming DNS response.

\param  [in]  pInData  Pointer to the incoming DNS response.
***************************************************************************************************/
void DNS_ClientService(void *pInData);

/*!*************************************************************************************************
\fn     uint8_t DNS_getHostByName(const uint8_t *nodename, const uint8_t type, const uint8_t dnsClass
                                  appReturnHandler_t pfAppReturn, taskMsgQueue_t *pTaskMsgQueue)
\brief  This function makes a DNS request.

\param  [in]  nodename       Pointer to the string .
\param  [in]  type           Type of request.
\param  [in]  dnsClass       Class type of request.
\param  [in]  pfAppReturn    Pointer to the function to be resumed after the response in received
                             or the time is up
\param  [in]  pTaskMsgQueue  Pointer to the message queue of the task where the resume function will
                             be executed

\return       uint8_t        Returns query response.
***************************************************************************************************/
uint8_t DNS_GetHostByName(uint8_t *nodename, const uint8_t type, const uint8_t dnsClass,
                          appReturnHandler_t pfAppReturn, taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn     void DNS_parseTypeAResponse(uint8_t *pPacket, uint16_t offset, hostent_t *pRes
                                    dnsCacheEntry_t *pCacheEntry)
\brief  This function parse an A type answer in the response packet.

\param  [in]  pPacket      Pointer to the dns packet.
\param  [in]  offset       Offset in the dns packet where the answer begins.
\param  [in]  pRes         Pointer to the result structure
\param  [in]  pCacheEntry  Pointer to a new cache entry.
***************************************************************************************************/
void DNS_ParseTypeAResponse(uint8_t *pPacket, uint16_t offset, hostent_t *pRes,
                            dnsCacheEntry_t *pCacheEntry);

/*!*************************************************************************************************
\fn     void DNS_parseTypeAAAAResponse(uint8_t *pPacket, uint16_t offset, hostent_t *pRes
                                       dnsCacheEntry_t *pCacheEntry)
\brief  This function parse an AAAA type answer in the response packet.

\param  [in]  pPacket      Pointer to the dns packet.
\param  [in]  offset       Offset in the dns packet where the answer begins.
\param  [in]  pRes         Pointer to the result structure
\param  [in]  pCacheEntry  Pointer to a new cache entry.
***************************************************************************************************/
void DNS_ParseTypeAAAAResponse(uint8_t *pPacket, uint16_t offset, hostent_t *pRes,
                               dnsCacheEntry_t *pCacheEntry);

/*!*************************************************************************************************
\fn     void DNS_parseTypeA6Response(uint8_t *pPacket, uint16_t offset, hostent_t *pRes
                                     dnsCacheEntry_t *pCacheEntry)
\brief  This function parse an AAAA type answer in the response packet.

\param  [in]  pPacket      Pointer to the dns packet.
\param  [in]  offset       Offset in the dns packet where the answer begins.
\param  [in]  pRes         Pointer to the result structure
\param  [in]  pCacheEntry  Pointer to a new cache entry.
***************************************************************************************************/
void DNS_ParseTypeA6Response(uint8_t *pPacket, uint16_t offset, hostent_t *pRes,
                             dnsCacheEntry_t *pCacheEntry);

/*!*************************************************************************************************
\fn    void DNS_SetDnsServerIp6Addr(ipAddr_t *addr);
\brief This function sets the IPv6 address of the DNS server.
\param [in]  pAddr    Pointer to IPv6 server address.
***************************************************************************************************/
void DNS_SetDnsServerIp6Addr(ipAddr_t *pAddr);

/*!*************************************************************************************************
\fn     void setDnsServerIp6Addr(uint32_t addr)
\brief  This function sets the IPv6 address of the DNS server.

\param  [in]  addr    IPv4 server address.
***************************************************************************************************/
void DNS_SetDnsServerIpAddr(uint32_t addr);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _DNS_CLIENT_H */
