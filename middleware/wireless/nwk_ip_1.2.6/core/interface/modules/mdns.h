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

#ifndef _MDNS_H
#define _MDNS_H
/*!
\file       mdns.h
\brief      This is a public header file for the mDNS module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "sockets.h"
#include "ip.h"
#include "network_utils.h"
#include "TimersManager.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#ifndef MDNS_ENABLED
    #define MDNS_ENABLED                0
#endif

#define     MDNS_CACHE_FLUSH_MASK       (0x8000U)
#define     MDNS_UNICAST_RESPONSE_MASK  (MDNS_CACHE_FLUSH_MASK)
#define     MDNS_FIRST_ANNOUNCING_STEP  (4U)
#define     MDNS_MAX_TXT_OPTIONS        (12U)
#define     MDNS_MAX_TXT_OPTION_LEN     (40U)

/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum
{
    mMdnsProbingOK_c            = 0,
    mMdnsProbingTimerInvalid_c  = 1,
    mMdnsProbingNoMemory_c      = 2

} MdnsProbingResponse_t;

typedef struct mDnsServiceTxtOption_tag
{
    uint8_t length;
    uint8_t optionValue[MDNS_MAX_TXT_OPTION_LEN];
} mDnsServiceTxtOption_t;

typedef struct mDnsService_tag
{
    uint8_t *hostName;          /*! host name */
    uint8_t *name;              /*! host name */
    uint8_t *target;            /*! target */
    mDnsServiceTxtOption_t txtInfo[MDNS_MAX_TXT_OPTIONS];           /*! service information */
    uint8_t *domain;            /*! usually local */
    uint8_t *protocolType;      /*! e.g.: _http._tcp */
    uint8_t *fullName;          /*! host._http._tcp.local */
    ipAddr_t address;           /*! ip address of the service */
    uint32_t ip4Addr;           /*! ipv4 address of the service */
    uint32_t portNo;            /*! service port */
    bool_t isAdvertised;

} mDnsService_t;

typedef struct mDnsProbingStruct_tag
{
    mDnsService_t *newService;
    uint8_t *pData;
    tmrTimerID_t probingTmrId;
    uint8_t counter;
    uint16_t dataLength;
} mDnsProbingStruct_t;

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
\fn     uint8_t MDNS_ProbeNewService(mDnsService_t *newService)
\brief  This function starts probing a new mDNS service.

\param  [in]  newService  Pointer to the new service.

\retval       uint8_t     Probing result
***************************************************************************************************/
uint8_t MDNS_ProbeNewService(mDnsService_t *newService);

/*!*************************************************************************************************
\fn     void MDNS_JoinIf(ifHandle_t ifHandle, uint8_t addrFamily)
\brief  This function creates the socket for mDNS module, depending on the IP address family.

\param  [in]  ifHandle   Pointer to IP interface
\param  [in]  addrFamily  IP address family: AF_INET or AF_INET6
***************************************************************************************************/
void MDNS_JoinIf(ifHandle_t ifHandle, uint8_t addrFamily);

/*!*************************************************************************************************
\fn     void MDNS_Init(taskMsgQueue_t *pTaskMsgQueue)
\brief  This function initializes the mDNS module.

\param  [in]  pTaskMsgQueue  Pointer to task queue
***************************************************************************************************/
void MDNS_Init(taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn    void MDNS_AddService(mDnsService_t *pService)
\brief  This function adds a service name in mDNS service table.

\param [in]   pService             pointer to service.

\return       void
***************************************************************************************************/
void MDNS_AddService(mDnsService_t *pService);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _MDNS_H */
