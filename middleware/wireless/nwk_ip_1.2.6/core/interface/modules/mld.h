/*
* The Clear BSD License
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

#ifndef _MLD_H_
#define _MLD_H_

/*!
\file       mld.h
\brief      This is a header file for the MLD module. It contains the declarations
            of the interface functions.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

/* Framework includes */
#include "FunctionLib.h"

/* Network Includes */
#include "network_utils.h"
#include "ip.h"
#include "ip6.h"
#include "icmp.h"
#include "ip_if_management.h"
#include "mld.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! MLD version1 message types (RFC2710)*/
typedef enum
{
    gMldTypeMulticastListenerQuery_c     = 130U,
    gMldTypeMulticastListenerReport_c    = 131U,
    gMldTypeMulticastListenerDone_c      = 132U,
}mldMsgTypes_e;


/* The MLDv1 message format:

  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |     Code      |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Maximum Response Delay    |          Reserved             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   +                                                               +
   |                                                               |
   +                       Multicast Address                       +
   |                                                               |
   +                                                               +
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

/*! MLD version1 message format (RFC2710)*/
typedef struct mldHeader_tag
{
    uint8_t     type;
    uint8_t     code;
    uint16_t    checksum;
    uint16_t    maxRespDelay;
    uint16_t    reserved;
    ipAddr_t    mCastAddr;
}mldHeader_t;

/*! The IPv6 Hop-by-Hop Option header used to carry the MLD Router Alert option */
typedef struct mldRouterAlertHeader_tag
{
    uint8_t nextHeader;
    uint8_t headerExLen;
    uint8_t routerAlertType;
    uint8_t routerAlertLength;
    uint8_t routerAlertValue[2];
    uint8_t paddingType;
    uint8_t paddingLength;
}mldRouterAlertHeader_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

extern const icmpProtMsgTypeHandler_t gaMldProtMsgTypeHandlerTbl[];
extern const uint8_t gMldProtMsgTypeHandlerTblSize;

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void MLD_ListenerQueryRecv(ipPktInfo_t *pRxIpPktInfo)
\brief  This function handles MLD queries from routers regarding existing IPv6 multicast groups

\param  [in]   pRxIpPktInfo  Pointer to the received IP packet (MLD Listner Query) and additional
                             IP and link layer information.
***************************************************************************************************/
void MLD_ListenerQueryRecv(ipPktInfo_t *pRxIpPktInfo);

/*!*************************************************************************************************
\fn     void MLD_ListenerReportSend(ipAddr_t* pIpMcastAddr, ipAddr_t* pIpDstAddr, ifHandle_t ifHandle)
\brief  This function allocates and populates an MLD Listener Report message which will be sent to IP.

\param  [in]   pIpMcastAddr  Pointer to IPv6 Multicast Address that is advertised
\param  [in]   ifUniqueId    Outgoing interface id for the MLD Listner Report
***************************************************************************************************/
void MLD_ListenerReportSend(ipAddr_t* pIpMcastAddr,  ipIfUniqueId_t ifUniqueId);

/*!*************************************************************************************************
\fn     void MLD_ListenerDoneSend(ipAddr_t* pIpMcastAddr, ipAddr_t* pIpDstAddr, ifHandle_t ifHandle)
\brief  This function allocates and populates an MLD Listener Done message which will be sent to IP.

\param  [in]   pIpMcastAddr  Pointer to IPv6 Multicast Address that is advertised
\param  [in]   ifUniqueId    Outgoing interface id for the MLD Listner Done
***************************************************************************************************/
void MLD_ListenerDoneSend(ipAddr_t* pIpMcastAddr,  ipIfUniqueId_t ifUniqueId);

/*!*************************************************************************************************
\fn     void MLD_EnableGroup(ipAddr_t* pIpMcastAddr, ipIfUniqueId_t ipIfId)
\brief  This function enabled MLD for the given multicast IP address as a consequence of adding
        this multicast address on the interface. As a result it will generate MLD_ListnerReport

\param  [in]   pIpMcastAddr  Pointer to IPv6 Multicast Addres
\param  [in]   ifUniqueId    Interface related to the IPv6 Multicast Address
***************************************************************************************************/
void MLD_EnableGroup(ipAddr_t *pMcastAddr, ipIfUniqueId_t ipIfId);

/*!*************************************************************************************************
\fn     void MLD_DisableGroup(ipAddr_t* pIpMcastAddr, ipIfUniqueId_t ipIfId)
\brief  This function disables MLD for the given multicast IP address as a consequence of removal of
        this multicast address from the interface. As a result it will generate MLD_ListnerDone

\param  [in]   pIpMcastAddr  Pointer to IPv6 Multicast Address
\param  [in]   ifUniqueId    Interface related to the IPv6 Multicast Address
***************************************************************************************************/
void MLD_DisableGroup(ipAddr_t *pMcastAddr, ipIfUniqueId_t ipIfId);

/*!*************************************************************************************************
\fn     void MLD_IF_Enable(IfUniqueId_t ipIfId)
\brief  This function enables MLD for the given interface. As a result it will enable MLD for all
        associated multicast addresses for that interface

\param  [in]   ifUniqueId    Interface to enable MLD on
***************************************************************************************************/
void MLD_IF_Enable(ipIfUniqueId_t ipIfId);

/*!*************************************************************************************************
\fn     void MLD_IF_Disable(IfUniqueId_t ipIfId)
\brief  This function disables MLD for the given interface. As a result it will disable MLD for all
        associated multicast addresses for that interface

\param  [in]   ifUniqueId   Interface to disable MLD on
***************************************************************************************************/
void MLD_IF_Disable(ipIfUniqueId_t ipIfId);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _MLD_H_ */
