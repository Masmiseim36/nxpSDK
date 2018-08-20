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
/*!=================================================================================================
\file       ip_if_6lo.c
\brief      This is a private source file for the 6LoBle Media interface.
==================================================================================================*/
#include "stack_config.h"

#include "ip.h"
#include "ip6.h"
#include "ip_if_management.h"
#include "MemManager.h"
#include "FunctionLib.h"
#include "nd.h"
#include "sixloble_interface.h"

#if IP_IP6_ENABLE
/*==================================================================================================
Private macros
==================================================================================================*/

#define MTU_6LOBLE  1280
#define SLP_REASSEMBLE_THRESHOLD 160
/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/

void IP_6LoBleRecv(adpdBleDataInd_t * pAdpdDataInd);

uint32_t IP_6LoBleOpen(ifHandle_t ifHandle);
uint32_t IP_6LoBleClose(ifHandle_t ifHandle);
uint32_t IP_6LoBleSend(ipPktInfo_t* pIpPktInfo);
uint32_t IP_6LoBleGetIID(ifHandle_t ifHandle,llAddr_t* macAddr,ipAddr_t* ipAddr);
uint32_t IP_6LoBleJoin(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol);
uint32_t IP_6LoBleLeave(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol);

void IP_6LoBleMergeNwkBuff(nwkBuffer_t* pNwkBuff, nwkBuffer_t* pOutNwkBuff);

/*==================================================================================================
Private global variables declarations
==================================================================================================*/

const char* mIfName = "6LoBLE";

static const mediaIfStruct_t g6LoBleMediaIf =
{
    IP_6LoBleOpen,
    IP_6LoBleClose,
    NULL,
    NULL,
    IP_6LoBleSend,
    IP_6LoBleGetIID,
    IP_6LoBleJoin,
    IP_6LoBleLeave
};

const mediaIfStruct_t* g6LoBleMediaIfPtr = (mediaIfStruct_t*) &g6LoBleMediaIf;

static const adpdBleCb_t mAdpCallbacks = {IP_6LoBleRecv};
static uint32_t mNsduHandle = 0;
/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Private functions
==================================================================================================*/

void IP_6LoBleMergeNwkBuff
(
    nwkBuffer_t* pNwkBuff,
    nwkBuffer_t* pOutNwkBuff
)
{
    nwkBuffer_t* pTemp1NwkBuff = pNwkBuff;
    nwkBuffer_t* pTemp2NwkBuff;
    uint32_t nwkBuffCount = 2;
    uint32_t nwkHdrSize;
    uint8_t* pData;

    while(NULL != pTemp1NwkBuff->next->next)
    {
        nwkBuffCount++;
        pTemp1NwkBuff = pTemp1NwkBuff->next;
    }

    if(nwkBuffCount > 2)
    {
        /* save pointer to last nwk buffer - contains app data */
        pTemp2NwkBuff = pTemp1NwkBuff->next;
        /* make pointer to last nwk buffer null and try to create a single network buffer for all
           the data except the app payload */
        pTemp1NwkBuff->next = NULL;

        nwkHdrSize = NWKU_NwkBufferTotalSize(pNwkBuff);
        pData = NWKU_NwkBufferToRegularBuffer(pNwkBuff, NULL);

        if(NULL != pData)
        {
            NWKU_FreeAllNwkBuffers(&pNwkBuff->next);
            MEM_BufferFree(pNwkBuff->pData);
            pNwkBuff->pData = pData;
            pNwkBuff->size = nwkHdrSize;
            pNwkBuff->next = pTemp2NwkBuff;
        }
        else
        {
            /* restore original buffer configuration */
            pTemp1NwkBuff->next = pTemp2NwkBuff;
        }
    }

    pOutNwkBuff->pData = pNwkBuff->pData;
    pOutNwkBuff->size = pNwkBuff->size;
    pOutNwkBuff->next = pNwkBuff->next;
    MEM_BufferFree(pNwkBuff);

}
/*!*************************************************************************************************
\fn  void IP_6LoBleRecv(ipPktInfo_t* pIpPktInfo)

\brief  Services an 6LoBle packet

\param [in]  pParam    the received data indication

\retval      none
***************************************************************************************************/
void IP_6LoBleRecv
(
    adpdBleDataInd_t * pAdpdDataInd
)
{
    ipPktInfo_t* pIpPktInfo;
    ifHandle_t ifHandle = IP_IF_GetIfHandle((ipIfUniqueId_t)pAdpdDataInd->ifInstanceId);
    
    IPIF_STATS_ENABLED(ifHandle->stats.commonStats.rxTotal++);
    IPIF_STATS_ENABLED(ifHandle->stats.rxOctets += pAdpdDataInd->nsduLength);

    pIpPktInfo = NWKU_CreateIpPktInfo();
    if(NULL != pIpPktInfo)
    {
        pIpPktInfo->pNwkBuff = NWKU_CreateNwkBuffer(0);

        if(NULL != pIpPktInfo->pNwkBuff)
        {
            pIpPktInfo->pNwkBuff->size = pAdpdDataInd->nsduLength;
            pIpPktInfo->pNwkBuff->pData = pAdpdDataInd->pNsdu;
            pIpPktInfo->pNextProt = pAdpdDataInd->pNsdu;
            pIpPktInfo->ipPktOptions.ifHandle = (void*)ifHandle;
            FLib_MemCpy((void*)&pIpPktInfo->ipPktOptions.srcLlInfo,
                        (void*)&pAdpdDataInd->srcAddrInfo,sizeof(llAddr_t));
            IP_Receive(pIpPktInfo,gIpProtv6_c);
        }
        else
        {
            NWKU_FreeIpPktInfo(&pIpPktInfo);
            MEM_BufferFree(pAdpdDataInd->pNsdu);
            ///TODO: send out of memory event
        }
    }
    else
    {
        ///TODO: send out of memory event
        MEM_BufferFree(pAdpdDataInd->pNsdu);
    }
}

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn uint32_t IP_6LoBleOpen(ifHandle_t ifHandle)
\brief  Registers IP with the 6LoBle layer

\param [in]  ifHandle    interface handle

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_6LoBleOpen
(
    ifHandle_t ifHandle
)
{
    uint32_t  error = gIpInvalidParameterError_c;
    sloBleStruct_t* slpHandle = *(sloBleStruct_t **)ifHandle->ifDriverHandle;

    /* add interface name */

    ifHandle->ifMetric = gIp6LowPanMetric_c;

    /* open interface if ipVersion6 is 1 and if ID si slp. Ignore ipVersion4 value */
    if((1 == ifHandle->ipVersion6) &&
        ((gIpIfBle0_c == ifHandle->ifUniqueId)||(gIpIfBle1_c == ifHandle->ifUniqueId)))
    {
        ifHandle->ifMtu = MTU_6LOBLE;
        ifHandle->ifDevAddrTbl[0].addrSize = gLlayerAddrEui48_c;
        ifHandle->ifDevAddrTbl[1].addrSize = gLlayerAddrEui64_c;

        /* add extended address */
        slpHandle->pBleAbsReq->GetBleAddress((void*)&ifHandle->ifDevAddrTbl[0].eui);
        
        SixLoBle_GetIIDFromBleAddr((void*)&ifHandle->ifDevAddrTbl[0].eui,
                                   ifHandle->ifDevAddrTbl[1].eui);

        /* register Media interface callbacks */
        SixLoBle_RegisterAdpdCallbacks((instanceId_t)ifHandle->ifDriverHandle,
                                   (adpdBleCb_t*)&mAdpCallbacks);

        /* register private data from data indication and confirm callback */
        SixLoBle_RegisterIfInstance((instanceId_t)ifHandle->ifDriverHandle,
                                (instanceId_t)ifHandle->ifUniqueId);
        
        /* RFC4861 6.3.3: The host joins the all-nodes multicast address on all multicast capable interfaces. */
        IP_IF_AddMulticastGroup6(ifHandle->ifUniqueId, (ipAddr_t *)&in6addr_linklocal_allnodes);

        /* Link-Local Address Generation/Auto configuration.
         * It comprises of '1111111010' as the first ten bits followed by 54 zeroes
         * and a 64 bit interface identifier.
         * For all autoconfiguration types, a link-local address is always configured.*/
        error = IP_IF_BindAddr6(ifHandle->ifUniqueId, NULL, ip6AddrTypeAutoconfigurable_c,
                                IP6_ADDRESS_LIFETIME_INFINITE, 64U);

        if(error != gIpOk_c)
        {
            IP_6LoBleClose(ifHandle);
        }
    }
    return error;
}
/*!*************************************************************************************************
\fn uint32_t IP_6LoBleClose(ifHandle_t ifHandle)
\brief  Unregisters IP with the 6LoBle layer

\param [in]  ifHandle  interface handle

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_6LoBleClose
(
    ifHandle_t ifHandle
)
{
#if IP_DISABLE_INTERFACE_FUNCTIONALITY
    uint32_t i;

#if ND_ENABLED
    /* Relaese ND for this interface. */
    ND_Close(ifHandle);
#endif /* ND_ENABLED */

    /* Unbind all addresses.  */
    for(i = 0; i < IP_IF_IP6_ADDR_NB; i++)
    {
        ip6IfAddrData_t* addr = IP_IF_GetAddrByNr6(i);
        if(NULL != addr)
        {
            IP_IF_UnbindAddr6(ifHandle, &addr->ip6Addr);
        }
    }

    /* Leaves the all-nodes multicast address. */
    IP_IF_Leave(ifHandle, &in6addr_linklocal_allnodes);

    return gIpOk_c;
#else
    return gIpOk_c;
#endif
}
/*!*************************************************************************************************
\fn uint32_t IP_6LoBleSend(ipPktInfo_t* pIpPktInfo)
\brief  Sends an IPv6 packet.

\param [in]  pIpPktInfo    the packet to send

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_6LoBleSend
(
    ipPktInfo_t* pIpPktInfo
)
{
#if ND_ENABLED
    ndNeighborEntry_t **ppNd6NeighborEntry = NULL;
#endif
    ifHandle_t ifPtr = ((ifHandle_t)pIpPktInfo->ipPktOptions.ifHandle);
    adpdBleDataReq_t dataReq;
    bool_t destFound  = TRUE;

    IPIF_STATS_ENABLED(ifPtr->stats.commonStats.txTotal++);
    IPIF_STATS_ENABLED(ifPtr->stats.txOctets += dataReq.nsduLength);

#if ND_ENABLED
     if((!IP6_IsMulticastAddr(pIpPktInfo->pIpDstAddr) && 
        (!IP6_IsLinkLocalAddr(pIpPktInfo->pIpDstAddr)))) 
    {
        if (ifPtr->ip6If.ip6IsAddrOnLink)
        {
            if(FALSE == ifPtr->ip6If.ip6IsAddrOnLink(pIpPktInfo->pIpDstAddr, pIpPktInfo->ipPktOptions.ifHandle))
            {
                /* Find the Default Router Address */
                ppNd6NeighborEntry = ND_GetDefaultRouter(pIpPktInfo->ipPktOptions.ifHandle);
                if(NULL != ppNd6NeighborEntry)
                {
                    IP_AddrCopy(&dataReq.ipDstAddr,&(*ppNd6NeighborEntry)->ipAddr);
                }
                else
                {
                    destFound = FALSE;
                }
            }
        }
        else
        {
            /* No way to determine if address is on link or not */
            destFound = FALSE;
        }
    }
#endif /* ND_ENABLED */

    if(destFound == TRUE)
    {
        if(ifPtr->ip6If.ip6ResolveUnicastAddr)
        {
            if(FALSE == ifPtr->ip6If.ip6ResolveUnicastAddr(pIpPktInfo))
            {
                ///TODO: Change to have only one exit point
                return gIpOk_c;
            }
        }

        /* If we have a fragmented nwk buffer use NWKU_NwkBufferToRegularBuffer to unify the
           fragments in a new buffer if total pkt size is smaller than a threshold */
        if(pIpPktInfo->pNwkBuff->next)
        {
            uint32_t dataSize = NWKU_NwkBufferTotalSize(pIpPktInfo->pNwkBuff);
            if(dataSize <= SLP_REASSEMBLE_THRESHOLD)
            {
                dataReq.pNsduNwkBuff.pData = NWKU_NwkBufferToRegularBuffer(pIpPktInfo->pNwkBuff, NULL);
                if(NULL != dataReq.pNsduNwkBuff.pData)
                {
                    dataReq.pNsduNwkBuff.size = dataSize;
                    dataReq.pNsduNwkBuff.next = NULL;
                    NWKU_FreeAllNwkBuffers(&pIpPktInfo->pNwkBuff);
                }
                else
                {
                    NWKU_FreeIpPktInfo(&pIpPktInfo);
                    return gIpOk_c;
                }
            }
            else
            {
                IP_6LoBleMergeNwkBuff(pIpPktInfo->pNwkBuff, &dataReq.pNsduNwkBuff);
            }
        }
        else
        {
            dataReq.pNsduNwkBuff.pData = pIpPktInfo->pNwkBuff->pData;
            dataReq.pNsduNwkBuff.size = pIpPktInfo->pNwkBuff->size;
            dataReq.pNsduNwkBuff.next = pIpPktInfo->pNwkBuff->next;
            MEM_BufferFree(pIpPktInfo->pNwkBuff);
        }

        /* Prevent the network buffer data to be freed */
        pIpPktInfo->pNwkBuff = NULL;

        /* set the data request paremeters */
        dataReq.nsduHandle = mNsduHandle;
        dataReq.slwpInstanceId = (instanceId_t)ifPtr->ifDriverHandle;
        IP_AddrCopy(&dataReq.ipDstAddr, pIpPktInfo->pIpDstAddr);
        /* free the pIpPktInfo */
        NWKU_FreeIpPktInfo(&pIpPktInfo);

        /* increment NSD handle for next packet */
        mNsduHandle++;

        /* send to 6LowPAN */
        SixLoBle_DataRequest(&dataReq);
    }
    else
    {
        NWKU_FreeIpPktInfo(&pIpPktInfo);
    }

    return gIpOk_c;
}

/*!*************************************************************************************************
\fn uint32_t IP_6LoBleGetIID(ifHandle_t ifHandle)
\brief  Gets the Interface identifier

\param [in]  ifHandle  interface handle
\param [in]  macAddr    link layer address
\param [out] ipAddr     IP address to store the IID in

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_6LoBleGetIID
(
    ifHandle_t ifHandle,
    llAddr_t* macAddr,
    ipAddr_t* ipAddr
)
{
    
    if (macAddr->addrSize == gLlayerAddrEui48_c)
    {
        SixLoBle_GetIIDFromBleAddr(macAddr->eui, &ipAddr->addr8[8]);
        return gIpOk_c;
    }
    else
    {
        return gIpBadAddrError_c;
    }
}
/*!*************************************************************************************************
\fn uint32_t IP_6LoBleJoin(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol)
\brief  Joins an IPv6 multicast group.

\param [in]  ifHandle  the packet to send
\param [in]  pIpAddr    the multicast group
\param [in]  protocol   the protocol for the multicast group(IPv4 or IPv6)

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_6LoBleJoin
(
    ifHandle_t ifHandle,
    ipAddr_t* pIpAddr,
    uint16_t protocol
)
{
    return gIpOk_c;
}
/*!*************************************************************************************************
\fn uint32_t IP_6LoBleLeave(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol)
\brief  Leaves an IPv6 multicast group.

\param [in]  ifHandle  the packet to send
\param [in]  pIpAddr    the multicast group
\param [in]  protocol   the protocol for the multicast group(IPv4 or IPv6)

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_6LoBleLeave
(
    ifHandle_t ifHandle,
    ipAddr_t* pIpAddr,
    uint16_t protocol
)
{

#if IP_DISABLE_INTERFACE_FUNCTIONALITY

#else
    return gIpOk_c;
#endif
}

#endif
