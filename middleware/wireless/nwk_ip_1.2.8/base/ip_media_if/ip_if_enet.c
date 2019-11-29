/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/*!=================================================================================================
\file       ip_if_enet.c
\brief      This is a private source file for the Enet interface.

==================================================================================================*/

#include "ip.h"
#include "ip6.h"
#include "ip4.h"
#include "arp.h"
#include "ip_if_management.h"
#include "enet_driver.h"
#include "MemManager.h"
#include "FunctionLib.h"
#include "nd.h"
#include "dhcp_client.h"
#include "ip_if_enet.h"

#if IP_IP4_ENABLE || IP_IP6_ENABLE

/*==================================================================================================
Private macros
==================================================================================================*/
#ifndef IP_DISCARD_SELF_BCASTS
    #define IP_DISCARD_SELF_BCASTS 0
#endif

#define IP4_BROADCAST_TO_ENET_ADDR(eth_addr) \
        (eth_addr)[0] = 0xFF, \
        (eth_addr)[1] = 0xFF, \
        (eth_addr)[2] = 0xFF, \
        (eth_addr)[3] = 0xFF, \
        (eth_addr)[4] = 0xFF, \
        (eth_addr)[5] = 0xFF

#define IP4_MULTICAST_TO_ENET_ADDR(ip,eth_addr)  \
        (eth_addr)[0] = 0x01, \
        (eth_addr)[1] = 0x00, \
        (eth_addr)[2] = 0x5E, \
        (eth_addr)[3] = ((ip) >> 16) & 0x7F, \
        (eth_addr)[4] = ((ip) >>  8) & 0xFF, \
        (eth_addr)[5] =  (ip)        & 0xFF

#define IP6_MULTICAST_TO_ENET_ADDR(ip6,eth_addr)  \
        (eth_addr)[0] = 0x33,             \
        (eth_addr)[1] = 0x33,             \
        (eth_addr)[2] = ip6->addr8[12],   \
        (eth_addr)[3] = ip6->addr8[13],   \
        (eth_addr)[4] = ip6->addr8[14],   \
        (eth_addr)[5] = ip6->addr8[15]

#define ENET_HEADER_SIZE  14U

#define ENET_TYPE_UNICAST                    0x01
#define ENET_TYPE_BROADCAST                  0x02
#define ENET_TYPE_MULTICAST                  0x04

/*==================================================================================================
Private type definitions
==================================================================================================*/

typedef struct enetHdr_tag
{
    uint8_t    dest[6];
    uint8_t    source[6];
    uint8_t    type[2];
} enetHdr_t;

/*==================================================================================================
Private prototypes
==================================================================================================*/


uint32_t IP_EnetOpen(ifHandle_t ifHandle);
uint32_t IP_EnetClose(ifHandle_t ifHandle);
uint32_t IP_EnetSendCommon(ipPktInfo_t *pIpPktInfo, llAddr_t *pLlAddr, uint16_t protocol);
void IP_EnetRecv(void *privateData, uint8_t *pInData, uint32_t size);
uint32_t IP_EnetJoin(ifHandle_t ifHandle, ipAddr_t *pIpAddr, uint16_t protocol);
uint32_t IP_EnetLeave(ifHandle_t ifHandle, ipAddr_t *pIpAddr, uint16_t protocol);

#if IP_IP4_ENABLE
    uint32_t IP_EnetOpen4(ifHandle_t ifHandle);
    uint32_t IP_EnetSend4(ipPktInfo_t *pIpPktInfo);
    uint32_t IP_EnetSendArp(ipPktInfo_t *pIpPktInfo, llAddr_t *pLlAddr);
#endif

#if IP_IP6_ENABLE
    uint32_t IP_EnetSend6(ipPktInfo_t *pIpPktInfo);
    uint32_t IP_EnetOpen6(ifHandle_t ifHandle);
    uint32_t IP_EnetGetIID(ifHandle_t ifHandle, llAddr_t *macAddr, ipAddr_t *ipAddr);
#endif

/*==================================================================================================
Private global variables declarations
==================================================================================================*/

static const mediaIfStruct_t gEnetMediaIf =
{
    IP_EnetOpen,
    IP_EnetClose,
#if IP_IP4_ENABLE
    IP_EnetSend4,
    IP_EnetSendArp,
#else
    NULL,
    NULL,
#endif
#if IP_IP6_ENABLE
    IP_EnetSend6,
    IP_EnetGetIID,
#else
    NULL,
    NULL,
#endif
    IP_EnetJoin,
    IP_EnetLeave
};

const mediaIfStruct_t *gEnetMediaIfPtr = (mediaIfStruct_t *) &gEnetMediaIf;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Private functions
==================================================================================================*/
/*!*************************************************************************************************
\fn  void IP_EnetRecv(void* privateData,uint8_t* pInData, uint32_t size)

\brief  Services an Ethernet packet

\param [in]  pIpPktInfo    the received packet
\param [in]  protocol      the received protocol

\retval      none
***************************************************************************************************/
void IP_EnetRecv
(
    void *privateData,
    uint8_t *pInData,
    uint32_t size
)
{
    ipPktInfo_t *pIpPktInfo;
    enetHdr_t enetpkt;
    uint16_t protocol;

    IPIF_STATS_ENABLED(pIfHandle->stats.commonStats.rxTotal++);
    IPIF_STATS_ENABLED(pIfHandle->stats.rxOctets += size);

    /* Extract EtherType */
    FLib_MemCpy(&enetpkt, pInData, ENET_HEADER_SIZE);
    protocol = ntohas(enetpkt.type);

    /* Make sure IP packet is aligned */
    size -= ENET_HEADER_SIZE;
    FLib_MemInPlaceCpy(pInData, (uint8_t *) pInData + ENET_HEADER_SIZE, size);

    pIpPktInfo = NWKU_CreateIpPktInfo();

    if (NULL != pIpPktInfo)
    {
        /* create NWK Buffer but do not allocate memory for it */
        pIpPktInfo->pNwkBuff = NWKU_CreateNwkBuffer(0);

        if (NULL != pIpPktInfo->pNwkBuff)
        {
            pIpPktInfo->ipPktOptions.ifHandle = privateData;
            pIpPktInfo->pNwkBuff->pData = pInData;
            pIpPktInfo->pNwkBuff->size = size;
            pIpPktInfo->pNwkBuff->freeBuffer = TRUE;

            pIpPktInfo->pNextProt = pInData;
            pIpPktInfo->ipPktOptions.srcLlInfo.addrSize = gLlayerAddrEui48_c;
            FLib_MemCpy(pIpPktInfo->ipPktOptions.srcLlInfo.eui,
                        enetpkt.source, gLlayerAddrEui48_c);

#if IP_DISCARD_SELF_BCASTS

            uint8_t enetType;

            if (enetpkt.dest[0] & 1)
            {
                if ((enetpkt.dest[0] == 0xFF)
                        && (enetpkt.dest[1] == 0xFF)
                        && (enetpkt.dest[2] == 0xFF)
                        && (enetpkt.dest[3] == 0xFF)
                        && (enetpkt.dest[4] == 0xFF)
                        && (enetpkt.dest[5] == 0xFF))
                {
                    IPIF_STATS_ENABLED(pIfHandle->stats.rxBroadcast++);
                    enetType = ENET_TYPE_BROADCAST;
                }
                else
                {
                    IPIF_STATS_ENABLED(pIfHandle->stats.rxMulticast++);
                    enetType = ENET_TYPE_MULTICAST;
                }
            }
            else
            {
                IPIF_STATS_ENABLED(pIfHandle->stats.rxUnicast++);
                enetType = ENET_TYPE_UNICAST;
            }

            /* discard any broadcast pkts that we sent, as they are likely echoes */
            if ((enetType == ENET_TYPE_BROADCAST) &&
                    (FLib_MemCmp(enetpkt.source, pIfHandle->ifDevAddrTbl[0].eui,
                                 pIfHandle->ifDevAddrTbl[0].addrSize) == 0))
            {
                IPIF_STATS_ENABLED(pIfHandle->stats.commonStats.rxDiscarded++);
                NWKU_FreeIpPktInfo(&pIpPktInfo);
                return;
            }

#endif

            IP_Receive(pIpPktInfo, protocol);
        }
        else
        {
            NWKU_FreeIpPktInfo(&pIpPktInfo);
            MEM_BufferFree(pInData);
            ///TODO: send out of memory event
        }
    }
    else
    {
        ///TODO: send out of memory event
        MEM_BufferFree(pInData);
    }
}
#if IP_IP6_ENABLE
/*!*************************************************************************************************
\fn uint32_t IP_EnetOpen6(ifHandle_t ifHandle)
\brief  Enable Ipv6 on the provided interface

\param [in]  ifHandle      interface handle

\retval      uint32_t       error
***************************************************************************************************/
uint32_t IP_EnetOpen6
(
    ifHandle_t ifHandle
)
{
    uint32_t  error = gIpGeneralError_c;

    /* Register IP6 */
    error = ENET_Open(ifHandle->ifDriverHandle, gIpProtv6_c, IP_EnetRecv, (void *)ifHandle);

    if (error == gIpOk_c)
    {
        /* RFC4861 6.3.3: The host joins the all-nodes multicast address on all
           multicastcapable interfaces. */
        IP_IF_AddMulticastGroup6(ifHandle->ifUniqueId, (ipAddr_t *)&in6addr_linklocal_allnodes);

        /* Link-Local Address Generation/Auto configuration.
         * It comprises of '1111111010' as the first ten bits followed by 54 zeroes
         * and a 64 bit interface identifier.
         * For all autoconfiguration types, a link-local address is always configured.*/
        error = IP_IF_BindAddr6(ifHandle->ifUniqueId, NULL, ip6AddrTypeAutoconfigurable_c,
                                IP6_ADDRESS_LIFETIME_INFINITE, 64U);
    }

    return error;
}
#endif
#if IP_IP4_ENABLE
/*!*************************************************************************************************
\fn uint32_t IP_EnetOpen4(ifHandle_t ifHandle)
\brief  Enable Ipv4 on the provided interface

\param [in]  ifHandle  interface handle

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_EnetOpen4
(
    ifHandle_t ifHandle
)
{
    uint32_t  error = gIpGeneralError_c;
    /* Register IP4 */
    error = ENET_Open(ifHandle->ifDriverHandle, gIpProtv4_c, IP_EnetRecv, (void *)ifHandle);

    if (error == gIpOk_c)
    {
        error = ENET_Open(ifHandle->ifDriverHandle, gIpProtArp_c, IP_EnetRecv, (void *)ifHandle);

        if (error == gIpOk_c)
        {
            error = ARP_Open(ifHandle);
        }
    }

    return error;
}
#endif
/*!*************************************************************************************************
\fn uint32_t IP_EnetSendCommon(ipPktInfo_t* pIpPktInfo,llAddr_t* pLlAddr,uin16_t protocol)
\brief  Sends an IP packet.

\param [in]  pIpPktInfo    the packet to send
\param [in]  pLlAddr       the link layer address to send the packet
\param [in]  protocol      the protocol to send

\retval      uint32_t      error
***************************************************************************************************/
uint32_t IP_EnetSendCommon
(
    ipPktInfo_t *pIpPktInfo,
    llAddr_t *pLlAddr,
    uint16_t protocol
)
{
    uint32_t error;
    ifHandle_t ifHandle = (ifHandle_t)pIpPktInfo->ipPktOptions.ifHandle;

    IPIF_STATS_ENABLED(ifHandle->stats.commonStats.txTotal++);
    IPIF_STATS_ENABLED(ifHandle->stats.txOctets += NWKU_NwkBufferTotalSize(pIpPktInfo->pNwkBuff));

    if (pLlAddr)
    {
        error = ENET_Send(ifHandle->ifDriverHandle, pIpPktInfo, protocol, pLlAddr->eui, 0);
    }
    else
    {
        IPIF_STATS_ENABLED(ifHandle->stats.commonStats.txDiscarded++);
        error = gIpGeneralError_c;
    }

    if (error)
    {
        IPIF_STATS_ENABLED(ifHandle->stats.commonStats.txErrors++);
    }

    return error;

}
/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn uint32_t IP_EnetOpen(ifHandle_t ifHandle)
\brief  Registers IP and ARP with an Ethernet packet driver

\param [in]  pIfHandle    interface handle

\retval      uint32_t     error
***************************************************************************************************/
uint32_t IP_EnetOpen
(
    ifHandle_t ifHandle
)
{
    uint32_t  ipv4Error = gIpOk_c;
    uint32_t  ipv6Error = gIpOk_c;
    uint32_t error = gIpInvalidParameterError_c;

    if ((gIpIfEth0_c == ifHandle->ifUniqueId) || (gIpIfEth1_c == ifHandle->ifUniqueId))
    {
        ifHandle->ifMetric = gIpEnetMetric_c;
        ifHandle->ifMtu = ENET_GetMTU(ifHandle->ifDriverHandle);
        ifHandle->ifDevAddrTbl[0].addrSize = gLlayerAddrEui48_c;

        ENET_GetAddress(ifHandle->ifDriverHandle, ifHandle->ifDevAddrTbl[0].eui);

#if IP_IP4_ENABLE

        if (ifHandle->ipVersion4)
        {
            ipv4Error = IP_EnetOpen4(ifHandle);
        }

#endif

#if IP_IP6_ENABLE

        if (ifHandle->ipVersion6)
        {
            ipv6Error = IP_EnetOpen6(ifHandle);
        }

#endif

        if ((ipv4Error != gIpOk_c) || (ipv6Error != gIpOk_c))
        {
            IP_EnetClose(ifHandle);
        }
        else
        {
            error = gIpOk_c;
        }
    }

    return error;
}
/*!*************************************************************************************************
\fn uint32_t IP_EnetClose(ifHandle_t ifHandle)
\brief  Unregisters IP and ARP with an Ethernet packet driver

\param [in]  pIfHandle    interface handle

\retval      uint32_t     error
***************************************************************************************************/
uint32_t IP_EnetClose
(
    ifHandle_t ifHandle
)
{
#if IP_DISABLE_INTERFACE_FUNCTIONALITY
    uint32_t  error, i;

    /* Relaese ND for this interface. */
    ND_Close(pIfHandle);

    /* Unbind all addresses bound to this interface */
    for (i = 0; i < IP_IF_IP6_ADDR_NB; i++)
    {
        ip6IfAddrData_t *addr = IP_IF_GetAddrByNr6(i);

        if (NULL != addr)
        {
            if (addr->ifPtr == pIfHandle)
            {
                IP_IF_UnbindAddr6((*pIfHandle)->ifUniqueId, &addr->ip6Addr);
            }
        }
    }

    /* Leaves the all-nodes multicast address. */
    IP_IF_Leave(pIfHandle, &in6addr_linklocal_allnodes);

    /* Deregister IP6. */
    error = ENET_Close((*pIfHandle)->ifDriverHandle, 0);

    return error;
#else
    return gIpOk_c;
#endif
}
#if IP_IP4_ENABLE
/*!*************************************************************************************************
\fn uint32_t IP_EnetSendArp(ipPktInfo_t* pIpPktInfo, llAddr_t* pLlAddr)
\brief  Sends an ARP packet

\param [in]  pInData    the APR packet to send
\param [in]  pLlAddr    pointer to link layer address to send data

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_EnetSendArp
(
    ipPktInfo_t *pIpPktInfo,
    llAddr_t *pLlAddr
)
{

    return IP_EnetSendCommon(pIpPktInfo, pLlAddr, gIpProtArp_c);

}
/*!*************************************************************************************************
\fn uint32_t IP_EnetSend4(ipPktInfo_t* pIpPktInfo)
\brief  Sends an IPv4 packet.

\param [in]  pIpPktInfo    the packet to send

\retval      uint32_t      error
***************************************************************************************************/
uint32_t IP_EnetSend4
(
    ipPktInfo_t *pIpPktInfo
)
{
    uint32_t error = gIpOk_c;
    llAddr_t enetDst;
    enetDst.addrSize = gLlayerAddrEui48_c;

    llAddr_t *pEnetDstInf = &enetDst;
    IPIF_STATS_ENABLED(ifHandle_t ifHandle = *((ifHandle_t *)pIpPktInfo->ipPktOptions.ifHandle));

    uint32_t ipDst = IP4_AddrToUint32(pIpPktInfo->pIpDstAddr);

    if (ipDst == IP4_ADDR_BROADCAST)
    {
        IPIF_STATS_ENABLED(ifHandle->stats.txBroadcast++);
        IP4_BROADCAST_TO_ENET_ADDR(enetDst.eui);
    }
    else
    {
        if (IP4_MULTICAST(ipDst))
        {
            IPIF_STATS_ENABLED(ifHandle->stats.txMulticast++);
            IP4_MULTICAST_TO_ENET_ADDR(ipDst, enetDst.eui);
        }
        else
        {
            /* resolve unicast address */
            IPIF_STATS_ENABLED(ifHandle->stats.txUnicast++);
            pEnetDstInf = ARP_Resolve(pIpPktInfo);
        }
    }

    error = IP_EnetSendCommon(pIpPktInfo, pEnetDstInf, gIpProtv4_c);

    return error;
}
#endif
#if IP_IP6_ENABLE && ND_ENABLED
/*!*************************************************************************************************
\fn uint32_t IP_EnetSend6(ipPktInfo_t* pIpPktInfo)
\brief  Sends an IPv6 packet.

\param [in]  pIpPktInfo    the packet to send

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_EnetSend6
(
    ipPktInfo_t *pIpPktInfo
)
{
    uint32_t error;
    llAddr_t enetDst;
    llAddr_t *pEnetDstInf = NULL;
    ifHandle_t ifHandle = (ifHandle_t)pIpPktInfo->ipPktOptions.ifHandle;
    ndCfg_t **ppNd6Cfg = ND_GetCfg(ifHandle->ifUniqueId);

    if (IP6_IsMulticastAddr(pIpPktInfo->pIpDstAddr))
    {
        IP6_MULTICAST_TO_ENET_ADDR(pIpPktInfo->pIpDstAddr, enetDst.eui);
        pEnetDstInf = &enetDst;
        //pEnetDstInf->addrSize = gLlayerAddrEui48_c;

        IPIF_STATS_ENABLED(ifHandle->stats.txMulticast++);
    }
    else
    {
        if ((*ppNd6Cfg)->ndIf->ndResolveDstLinkLayerAddr)
        {
            pEnetDstInf = (*ppNd6Cfg)->ndIf->ndResolveDstLinkLayerAddr(pIpPktInfo);
            IPIF_STATS_ENABLED(ifHandle->stats.txUnicast++);
        }
        else
        {
            IPIF_STATS_ENABLED(ifHandle->stats.commonStats.txDiscarded++);
            NWKU_FreeIpPktInfo(&pIpPktInfo);
        }
    }

    error = IP_EnetSendCommon(pIpPktInfo, pEnetDstInf, gIpProtv6_c);

    return error;
}

/*!*************************************************************************************************
\fn uint32_t IP_EnetGetIID(ifHandle_t ifHandle)
\brief  Gets the Interface identifier

\param [in]  ifHandle  interface handle
\param [in]  macAddr    link layer address
\param [out] ipAddr     IP address to store the IID in

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_EnetGetIID
(
    ifHandle_t ifHandle,
    llAddr_t *macAddr,
    ipAddr_t *ipAddr
)
{
    if (6U == macAddr->addrSize) /* IEEE 802.11 48-bit addresses. MAC address for Ethernet */
    {
        /* [RFC2464] The OUI of the Ethernet address (the first three octets) becomes the
           company_id of the EUI-64 (the first three octets).  The fourth and
           fifth octets of the EUI are set to the fixed value FFFE hexadecimal.
           The last three octets of the Ethernet address become the last three
           octets of the EUI-64. */
        FLib_MemCpy(&(ipAddr->addr8[8]), macAddr->eui, 3U);
        ipAddr->addr8[11] = 0xff;
        ipAddr->addr8[12] = 0xfe;
        FLib_MemCpy(&(ipAddr->addr8[13]), &(macAddr->eui[3]),  3U);

    }
    else /* macAddr->devAddrlen == 8U */
    {
        /* IEEE EUI-64 identifier. IEEE 802.15.4 */

        /* In this case, the Interface Identifier is formed
         * from the EUI-64 according to the "IPv6 over Ethernet" specification
         * [RFC2464]. */
        FLib_MemCpy(&(ipAddr->addr8[8]), macAddr->eui, 8U);
    }

    /* The Interface Identifier is then formed from the EUI-64 by
       complementing the "Universal/Local" (U/L) bit, which is the next-to-
       lowest order bit of the first octet of the EUI-64. */
    ipAddr->addr8[8] ^= 0x02;

    return 0;

}
#endif
/*!*************************************************************************************************
\fn uint32_t IP_EnetJoin6(ifHandle_t ifHandle, ipAddr_t* pIpAddr,uint16_t protocol)
\brief  Joins an IP multicast group(IPv4 or IPv6 depending on the protocol parameter).

\param [in]  pIfHandle  interface handle
\param [in]  pIpAddr    the multicast group
\param [in]  protocol   the protocol for the multicast group(IPv4 or IPv6)

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_EnetJoin
(
    ifHandle_t ifHandle,
    ipAddr_t *pIpAddr,
    uint16_t protocol
)
{
    uint8_t enetGroupAddr[6];

    if (gIpProtv4_c == protocol)
    {
        uint32_t ipAddr = IP4_AddrToUint32(pIpAddr);
        IP4_MULTICAST_TO_ENET_ADDR(ipAddr, enetGroupAddr);
    }
    else
    {
        IP6_MULTICAST_TO_ENET_ADDR(pIpAddr, enetGroupAddr);
    }

    return ENET_Join(ifHandle->ifDriverHandle, enetGroupAddr, protocol);
}
/*!*************************************************************************************************
\fn uint32_t IP_EnetLeave6(ifHandle_t* ifHandle, ipAddr_t* pIpAddr,uint16_t protocol)
\brief  Leaves an IP multicast group(IPv4 or IPv6 depending on the protocol parameter).

\param [in]  pIfHandle  interface handle
\param [in]  pIpAddr    the multicast group
\param [in]  protocol   the protocol for the multicast group(IPv4 or IPv6)

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_EnetLeave
(
    ifHandle_t ifHandle,
    ipAddr_t *pIpAddr,
    uint16_t protocol
)
{

    uint8_t enetGroupAddr[6];

    if (gIpProtv4_c == protocol)
    {
        uint32_t ipAddr = IP4_AddrToUint32(pIpAddr);
        IP4_MULTICAST_TO_ENET_ADDR(ipAddr, enetGroupAddr);
    }
    else
    {
        IP6_MULTICAST_TO_ENET_ADDR(pIpAddr, enetGroupAddr);
    }

    return ENET_Leave(ifHandle->ifDriverHandle, enetGroupAddr, protocol);
}

#endif
