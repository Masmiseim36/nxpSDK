/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/*!=================================================================================================
\file       ip_if_6lo.c
\brief      This is a private source file for the 6LowPan Media interface.
==================================================================================================*/
#include "app_stack_config.h"
#include "stack_config.h"

#include "ip.h"
#include "ip6.h"
#include "ip_if_management.h"
#include "MemManager.h"
#include "FunctionLib.h"
#include "nd.h"
#include "sixlowpan.h"

#if IP_IP6_ENABLE
/*==================================================================================================
Private macros
==================================================================================================*/

#define MTU_6LOWPAN  1280
#define SLP_REASSEMBLE_THRESHOLD 160
/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/

void IP_6LowPanRecv(adpdDataInd_t * pAdpdDataInd);
void IP_6LowPanDataCnf(adpdDataCnf_t* pAdpdDataCnf);

uint32_t IP_6LowPanOpen(ifHandle_t ifHandle);
uint32_t IP_6LowPanClose(ifHandle_t ifHandle);
uint32_t IP_6LowPanSend(ipPktInfo_t* pIpPktInfo);
uint32_t IP_6LowPanGetIID(ifHandle_t ifHandle,llAddr_t* macAddr,ipAddr_t* ipAddr);
uint32_t IP_6LowPanJoin(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol);
uint32_t IP_6LowPanLeave(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol);

void IP_6LowPanMergeNwkBuff(nwkBuffer_t* pNwkBuff, nwkBuffer_t* pOutNwkBuff);

/*==================================================================================================
Private global variables declarations
==================================================================================================*/

static const mediaIfStruct_t g6LowPanMediaIf =
{
    IP_6LowPanOpen,
    IP_6LowPanClose,
    NULL,
    NULL,
    IP_6LowPanSend,
    IP_6LowPanGetIID,
    IP_6LowPanJoin,
    IP_6LowPanLeave
};

const mediaIfStruct_t* g6LowPanMediaIfPtr = (mediaIfStruct_t*) &g6LowPanMediaIf;

static const adpdCb_t mAdpCallbacks = {IP_6LowPanDataCnf,IP_6LowPanRecv};
static uint32_t mNsduHandle = 0;
/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Private functions
==================================================================================================*/

void IP_6LowPanMergeNwkBuff
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
            pNwkBuff->freeBuffer = TRUE;
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
    pOutNwkBuff->freeBuffer = pNwkBuff->freeBuffer;
    MEM_BufferFree(pNwkBuff);

}
/*!*************************************************************************************************
\fn  void IP_6LowPanRecv(ipPktInfo_t* pIpPktInfo)

\brief  Services an 6LowPan packet

\param [in]  pParam    the received data indication

\retval      none
***************************************************************************************************/
void IP_6LowPanRecv
(
    adpdDataInd_t * pAdpdDataInd
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
            pIpPktInfo->ipPktOptions.lqi = pAdpdDataInd->linkQualityIndicator;
            pIpPktInfo->ipPktOptions.ifHandle = (void*)ifHandle;
            pIpPktInfo->ipPktOptions.security = pAdpdDataInd->securityLevel;
            pIpPktInfo->ipPktOptions.srcPanId = pAdpdDataInd->srcPanId;
            pIpPktInfo->ipPktOptions.destPanId = pAdpdDataInd->srcPanId;
            pIpPktInfo->ipPktOptions.channel = pAdpdDataInd->channel;
            pIpPktInfo->ipPktOptions.macSecKeyIdMode = pAdpdDataInd->keyIdMode;
            FLib_MemCpy((void*)&pIpPktInfo->ipPktOptions.srcLlInfo,
                        (void*)&pAdpdDataInd->srcAddrInfo,sizeof(llAddr_t));
            IP_Receive(pIpPktInfo,gIpProtv6_c);
        }
        else
        {
            NWKU_FreeIpPktInfo(&pIpPktInfo);
            MEM_BufferFree(pAdpdDataInd->pNsdu);
            ///TBD: send out of memory event
        }
    }
    else
    {
        ///TBD: send out of memory event
        MEM_BufferFree(pAdpdDataInd->pNsdu);
    }
}

/*!*************************************************************************************************
\fn  IP_6LowPanDataCnf(adpdDataCnf_t* pParam)

\brief  Handles ADP Data Confirm callback from 6LowPan

\param [in]  pParam    the received data confirm

\retval      none
***************************************************************************************************/
void IP_6LowPanDataCnf(adpdDataCnf_t* pAdpdDataCnf)
{
#if IP_IF_STATS_ENABLE
    if(gAdpStatusSuccess_c != pAdpdDataCnf->status )
    {
        ifHandle_t ifHandle = IP_IF_GetIfHandle((ipIfUniqueId_t)pAdpdDataCnf->ifInstanceId);
        IPIF_STATS_ENABLED(ifHandle->stats.commonStats.txErrors++);
    }
#endif
}
/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn uint32_t IP_6LowPanOpen(ifHandle ifHandle)
\brief  Registers IP with the 6LowPan layer

\param [in]  ifHandle    interface handle

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_6LowPanOpen
(
    ifHandle_t ifHandle
)
{
    uint32_t  error = gIpInvalidParameterError_c;
    slwpStruct_t* slpHandle = (slwpStruct_t *)ifHandle->ifDriverHandle;

    ifHandle->ifMetric = gIp6LowPanMetric_c;

    /* open interface if ipVersion6 is 1 and if ID si slp. Ignore ipVersion4 value */
    if((1 == ifHandle->ipVersion6) &&
        ((gIpIfSlp0_c == ifHandle->ifUniqueId)||(gIpIfSlp1_c == ifHandle->ifUniqueId)))
    {
        ifHandle->ifMtu = MTU_6LOWPAN;
        ifHandle->ifDevAddrTbl[0].addrSize = gLlayerAddrEui64_c;

        /* add extended address */
        uint64_t tempAddr = slpHandle->pMacAbsReq->GetExtendedAddress(slpHandle->macInstanceId);
        htonall(ifHandle->ifDevAddrTbl[0].eui,tempAddr);

        /* register Media interface callbacks */
        SWLP_RegisterAdpdCallbacks((instanceId_t)ifHandle->ifDriverHandle,
                                   (adpdCb_t*)&mAdpCallbacks);

        /* register private data from data indication and confirm callback */
        SWLP_RegisterIfInstance((instanceId_t)ifHandle->ifDriverHandle,
                                (instanceId_t)ifHandle->ifUniqueId);

        /* RFC4861 6.3.3: The host joins the all-nodes multicast address on all multicast capable 
          interfaces. */
        IP_IF_AddMulticastGroup6(ifHandle->ifUniqueId, (ipAddr_t *)&in6addr_linklocal_allnodes);

        /* Link-Local Address Generation/Auto configuration.
         * It comprises of '1111111010' as the first ten bits followed by 54 zeroes
         * and a 64 bit interface identifier.
         * For all autoconfiguration types, a link-local address is always configured.*/
        error = IP_IF_BindAddr6(ifHandle->ifUniqueId, NULL, ip6AddrTypeAutoconfigurable_c,
                                IP6_ADDRESS_LIFETIME_INFINITE, 64U);

        if(error != gIpOk_c)
        {
            IP_6LowPanClose(ifHandle);
        }
    }
    return error;
}
/*!*************************************************************************************************
\fn uint32_t IP_6LowPanClose(ifHandle_t ifHandle)
\brief  Unregisters IP with the 6LowPan layer

\param [in]  ifHandle  interface handle

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_6LowPanClose
(
    ifHandle_t ifHandle
)
{
#if IP_DISABLE_INTERFACE_FUNCTIONALITY
    uint32_t i;

#if ND_ENABLED
    /* Relaese ND for this interface. */
    ND_Close(pIfHandle);
#endif /* ND_ENABLED */

    /* Unbind all addresses.  */
    for(i = 0; i < IP_IF_IP6_ADDR_NB; i++)
    {
        ip6IfAddrData_t* addr = IP_IF_GetAddrByNr6(i);
        if(NULL != addr)
        {
            IP_IF_UnbindAddr6(ifHandle->ifUniqueId, &addr->ip6Addr);
        }
    }

    /* Leaves the all-nodes multicast address. */
    IP_IF_Leave(ifHandle->ifUniqueId, &in6addr_linklocal_allnodes);

    return gIpOk_c;
#else
    return gIpOk_c;
#endif
}
/*!*************************************************************************************************
\fn uint32_t IP_6LowPanSend(ipPktInfo_t* pIpPktInfo)
\brief  Sends an IPv6 packet.

\param [in]  pIpPktInfo    the packet to send

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_6LowPanSend
(
    ipPktInfo_t* pIpPktInfo
)
{
#if ND_ENABLED
    ndNeighborEntry_t **ppNd6NeighborEntry = NULL;
#endif
    ifHandle_t ifPtr = ((ifHandle_t)pIpPktInfo->ipPktOptions.ifHandle);
    adpdDataReq_t dataReq;
    bool_t destFound  = TRUE;

    IPIF_STATS_ENABLED(ifPtr->stats.commonStats.txTotal++);
    IPIF_STATS_ENABLED(ifPtr->stats.txOctets += dataReq.nsduLength);

#if ND_ENABLED
    if((!IP6_IsMulticastAddr(pIpPktInfo->pIpDstAddr)) ||
        (!IP6_IsLinkLocalAddr(pIpPktInfo->pIpDstAddr)))
    {
        if (ifPtr->ip6If.ip6IsAddrOnLink)
        {
            if(FALSE == ifPtr->ip6If.ip6IsAddrOnLink(pIpPktInfo->pIpDstAddr, ifPtr))
            {
                /* Find the Default Router Address */
                ppNd6NeighborEntry = ND_GetDefaultRouter(ifPtr);
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
                //TBD: Change to have only one exit point
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
                    dataReq.pNsduNwkBuff.freeBuffer = TRUE;
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
                IP_6LowPanMergeNwkBuff(pIpPktInfo->pNwkBuff, &dataReq.pNsduNwkBuff);
            }
        }
        else
        {
            dataReq.pNsduNwkBuff.pData = pIpPktInfo->pNwkBuff->pData;
            dataReq.pNsduNwkBuff.size = pIpPktInfo->pNwkBuff->size;
            dataReq.pNsduNwkBuff.next = pIpPktInfo->pNwkBuff->next;
            dataReq.pNsduNwkBuff.freeBuffer = pIpPktInfo->pNwkBuff->freeBuffer;
            MEM_BufferFree(pIpPktInfo->pNwkBuff);
        }

        /* Prevent the network buffer data to be freed */
        pIpPktInfo->pNwkBuff = NULL;

        /* set the data request paremeters */
        dataReq.securityLevel = pIpPktInfo->ipPktOptions.security;
        dataReq.macSecKeyIdMode = pIpPktInfo->ipPktOptions.macSecKeyIdMode;
        dataReq.nsduHandle = mNsduHandle;
        dataReq.discoverRoute = FALSE;
        if(pIpPktInfo->ipPktOptions.qos > 0)
        {
            dataReq.qualityOfService = gAdpQoSHighPriority_c;
        }
        else
        {
            dataReq.qualityOfService = gAdpQoSNormalPriority_c;
        }
        dataReq.slwpInstanceId = (instanceId_t)ifPtr->ifDriverHandle;
        dataReq.channel = pIpPktInfo->ipPktOptions.channel;
        dataReq.destPanId = pIpPktInfo->ipPktOptions.destPanId;
        IP_AddrCopy(&dataReq.ipDstAddr, pIpPktInfo->pIpDstAddr);
        /* free the pIpPktInfo */
        NWKU_FreeIpPktInfo(&pIpPktInfo);

        /* increment NSD handle for next packet */
        mNsduHandle++;

        /* send to 6LowPAN */
        SLWP_DataRequest(&dataReq);
    }
    else
    {
        NWKU_FreeIpPktInfo(&pIpPktInfo);
    }

    return gIpOk_c;
}

/*!*************************************************************************************************
\fn uint32_t IP_6LowPanGetIID(ifHandle_t ifHandle)
\brief  Gets the Interface identifier

\param [in]  pIfHandle  interface handle
\param [in]  macAddr    link layer address
\param [out] ipAddr     IP address to store the IID in

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_6LowPanGetIID
(
    ifHandle_t ifHandle,
    llAddr_t* macAddr,
    ipAddr_t* ipAddr
)
{
    slwpStruct_t* slpHandle = (slwpStruct_t *)(ifHandle->ifDriverHandle);
    adpIb_t * pAdpIb = slpHandle->pAdpIb;
    uint16_t panId = 0;

    if(gAdpIIDWithPanId_c == pAdpIb->adpIIDType)
    {
        panId = slpHandle->pMacAbsReq->GetPANId(slpHandle->macInstanceId);
    }
    NWKU_GetIIDFromLLADDR(macAddr,panId,&ipAddr->addr8[8]);

    return gIpOk_c;

}
/*!*************************************************************************************************
\fn uint32_t IP_6LowPanJoin(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol)
\brief  Joins an IPv6 multicast group.

\param [in]  ifHandle  the packet to send
\param [in]  pIpAddr    the multicast group
\param [in]  protocol   the protocol for the multicast group(IPv4 or IPv6)

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_6LowPanJoin
(
    ifHandle_t ifHandle,
    ipAddr_t* pIpAddr,
    uint16_t protocol
)
{
    return gIpOk_c;
}
/*!*************************************************************************************************
\fn uint32_t IP_6LowPanLeave(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol)
\brief  Leaves an IPv6 multicast group.

\param [in]  pIfHandle  the packet to send
\param [in]  pIpAddr    the multicast group
\param [in]  protocol   the protocol for the multicast group(IPv4 or IPv6)

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_6LowPanLeave
(
    ifHandle_t ifHandle,
    ipAddr_t* pIpAddr,
    uint16_t protocol
)
{

    return gIpOk_c;
}

#endif
