/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/*!=================================================================================================
\file       ip_if_loopback.c
\brief      This is a private source file for the Loopback Media interface.
==================================================================================================*/
#include "app_stack_config.h"
#include "stack_config.h"

#include "ip.h"
#include "ip6.h"
#include "ip_if_management.h"

/*==================================================================================================
Private macros
==================================================================================================*/
#ifndef LOOPBACK_MAX_MTU
    #define    LOOPBACK_MAX_MTU 1500
#endif

/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/

uint32_t IP_LoopbackOpen(ifHandle_t ifHandle);
#if IP_DISABLE_INTERFACE_FUNCTIONALITY
uint32_t IP_LoopbackClose(ifHandle_t ifHandle);
#endif /* IP_DISABLE_INTERFACE_FUNCTIONALITY */
uint32_t IP_LoopbackSend(ipPktInfo_t* pIpPktInfo);
uint32_t IP_LoopbackArpSend(ipPktInfo_t* pIpPktInfo, llAddr_t* pLlAddr);
uint32_t IP_LoopbackGetIID(ifHandle_t ifHandle,llAddr_t* macAddr,ipAddr_t* ipAddr);
uint32_t IP_LoopbackJoin(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol);
uint32_t IP_LoopbackLeave(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol);


/*==================================================================================================
Private global variables declarations
==================================================================================================*/

static const mediaIfStruct_t gLoopbackMediaIf =
{
    IP_LoopbackOpen,
#if IP_DISABLE_INTERFACE_FUNCTIONALITY
    IP_LoopbackClose,
#else
    NULL,
#endif /* IP_DISABLE_INTERFACE_FUNCTIONALITY */
    IP_LoopbackSend,
    IP_LoopbackArpSend,
    IP_LoopbackSend,
    IP_LoopbackGetIID,
    IP_LoopbackJoin,
    IP_LoopbackLeave
};

const mediaIfStruct_t* gLoopbackMediaIfPtr = (mediaIfStruct_t*) &gLoopbackMediaIf;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn uint32_t IP_LoopbackOpen(ifHandle ifHandle)
\brief  Registers IP with the Loopback layer

\param [in]  ifHandle    interface handle

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_LoopbackOpen
(
    ifHandle_t ifHandle
)
{
    uint32_t  error = gIpOk_c;

    ifHandle->ifMetric = gIpLoopbackMetric_c;

    /* Open interface */
    if(gIpIfLoopback_c == ifHandle->ifUniqueId)
    {
        uint64_t tempAddr = 1;

        ifHandle->ifMtu = LOOPBACK_MAX_MTU;
        ifHandle->ifDevAddrTbl[0].addrSize = gLlayerAddrEui64_c;

        /* add extended address */
        htonall(ifHandle->ifDevAddrTbl[0].eui, tempAddr);
    }

    return error;
}

#if IP_DISABLE_INTERFACE_FUNCTIONALITY
/*!*************************************************************************************************
\fn uint32_t IP_LoopbackClose(ifHandle_t ifHandle)
\brief  Unregisters IP with the Loopback layer

\param [in]  ifHandle  interface handle

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_LoopbackClose
(
    ifHandle_t ifHandle
)
{
    return gIpOk_c;
}
#endif /* IP_DISABLE_INTERFACE_FUNCTIONALITY */

/*!*************************************************************************************************
\fn uint32_t IP_LoopbackArpSend(ipPktInfo_t* pIpPktInfo, llAddr_t* pLlAddr)
\brief  Dummy function for sending ARP packets using loopback interface

\param [in]  pIpPktInfo    the packet to send
\param [in]  pLlAddr       link layer address

\retval      uint32_t      error
***************************************************************************************************/
uint32_t IP_LoopbackArpSend
(
    ipPktInfo_t* pIpPktInfo,
    llAddr_t* pLlAddr
)
{
    (void)pIpPktInfo;
    (void)pLlAddr;

    return 0;
}

/*!*************************************************************************************************
\fn uint32_t IP_LoopbackSend(ipPktInfo_t* pIpPktInfo)
\brief  Sends an IPv6 packet.

\param [in]  pIpPktInfo    the packet to send

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_LoopbackSend
(
    ipPktInfo_t* pIpPktInfo
)
{
    uint8_t *pData;
    uint32_t dataLen;
    uint32_t protocol = 0x0000; /* Undefined */

    IPIF_STATS_ENABLED(ifPtr->stats.commonStats.txTotal++);
    IPIF_STATS_ENABLED(ifPtr->stats.txOctets += dataReq.nsduLength);

    pData = NWKU_NwkBufferToRegularBuffer(pIpPktInfo->pNwkBuff, NULL);
    dataLen = NWKU_NwkBufferTotalSize(pIpPktInfo->pNwkBuff);
    NWKU_FreeAllNwkBuffers(&pIpPktInfo->pNwkBuff);

    pIpPktInfo->pNwkBuff = NWKU_CreateNwkBuffer(0);
    pIpPktInfo->pNwkBuff->pData = pData;
    pIpPktInfo->pNwkBuff->size = dataLen;
    pIpPktInfo->pNwkBuff->freeBuffer = 1;
    
    pIpPktInfo->pNextProt = pData;

    /* Pass the packet back to IP */
    if(IP_IsAddrIPv4(pIpPktInfo->pIpDstAddr))
    {
        protocol = gIpProtv4_c;
    }
    else if(IP_IsAddrIPv6(pIpPktInfo->pIpDstAddr))
    {
        protocol = gIpProtv6_c;
    }

    IP_Receive(pIpPktInfo, protocol);

    return gIpOk_c;
}

/*!*************************************************************************************************
\fn uint32_t IP_LoopbackGetIID(ifHandle_t ifHandle)
\brief  Gets the Interface identifier

\param [in]  pIfHandle  interface handle
\param [in]  macAddr    link layer address
\param [out] ipAddr     IP address to store the IID in

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_LoopbackGetIID
(
    ifHandle_t ifHandle,
    llAddr_t* macAddr,
    ipAddr_t* ipAddr
)
{
    uint16_t panId = 0;

    NWKU_GetIIDFromLLADDR(macAddr,panId,&ipAddr->addr8[8]);

    return gIpOk_c;

}
/*!*************************************************************************************************
\fn uint32_t IP_LoopbackJoin(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol)
\brief  Joins an IPv6 multicast group.

\param [in]  ifHandle  the packet to send
\param [in]  pIpAddr    the multicast group
\param [in]  protocol   the protocol for the multicast group(IPv4 or IPv6)

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_LoopbackJoin
(
    ifHandle_t ifHandle,
    ipAddr_t* pIpAddr,
    uint16_t protocol
)
{
    return gIpOk_c;
}
/*!*************************************************************************************************
\fn uint32_t IP_LoopbackLeave(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol)
\brief  Leaves an IPv6 multicast group.

\param [in]  pIfHandle  the packet to send
\param [in]  pIpAddr    the multicast group
\param [in]  protocol   the protocol for the multicast group(IPv4 or IPv6)

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_LoopbackLeave
(
    ifHandle_t ifHandle,
    ipAddr_t* pIpAddr,
    uint16_t protocol
)
{
    return gIpOk_c;
}

