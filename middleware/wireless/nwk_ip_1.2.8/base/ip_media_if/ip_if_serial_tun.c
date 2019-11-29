/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/*!=================================================================================================
\file       ip_if_serial_tun.c
\brief      This is a private source file for the Serial TUN interface.

==================================================================================================*/

#include "ip.h"
#include "ip6.h"
#include "ip_if_management.h"
#include "serial_tun_driver.h"
#include "MemManager.h"
#include "FunctionLib.h"
#include "nd.h"
#include "dhcp_client.h"

#include "FsciInterface.h"
#include "thci.h"
#include "app_serial_tun.h"


/*==================================================================================================
Private macros
==================================================================================================*/
#ifndef IP_DISABLE_INTERFACE_FUNCTIONALITY
    #define IP_DISABLE_INTERFACE_FUNCTIONALITY 0
#endif

#define SERIAL_TUN_MTU (1500U)
#define ENET_HEADER_SIZE 14U

#define IP6_MULTICAST_TO_ENET_ADDR(ip6,eth_addr)  \
        (eth_addr)[0] = 0x33,             \
        (eth_addr)[1] = 0x33,             \
        (eth_addr)[2] = ip6->addr8[12],   \
        (eth_addr)[3] = ip6->addr8[13],   \
        (eth_addr)[4] = ip6->addr8[14],   \
        (eth_addr)[5] = ip6->addr8[15]

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
uint32_t IP_SerialTunOpen(ifHandle_t ifHandle);
uint32_t IP_SerialTunClose(ifHandle_t ifHandle);
uint32_t IP_SerialTunSend6(ipPktInfo_t *pIpPktInfo);
uint32_t IP_SerialTunOpen6(ifHandle_t ifHandle);
uint32_t IP_SerialTunGetIID(ifHandle_t ifHandle, llAddr_t *macAddr, ipAddr_t *ipAddr);

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
static ifHandle_t mIpSerialtunInterfaceHandle;

static const mediaIfStruct_t gVirtualTunMediaIf =
{
    IP_SerialTunOpen,
    IP_SerialTunClose,
    NULL,
    NULL,
#if IP_IP6_ENABLE
    IP_SerialTunSend6,
    IP_SerialTunGetIID,
#else
    NULL,
    NULL,
#endif
    NULL,
    NULL
};
const mediaIfStruct_t *gVirtualTunMediaIfPtr = (mediaIfStruct_t *) &gVirtualTunMediaIf;


/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Private functions
==================================================================================================*/
/*!*************************************************************************************************
\fn  void IP_serialtunRecv(ipPktInfo_t* pIpPktInfo)

\brief  Sends a packet from SERIAL_TUN interface to IP.

\param [in]  pIpPktInfo    the received packet
\param [in]  size          the size of the received packet

\retval      none
***************************************************************************************************/
void IP_serialtunRecv
(
    uint8_t *pInData,
    uint32_t size
)
{
    ipPktInfo_t *pIpPktInfo;


    enetHdr_t enetpkt;
    uint16_t protocol;

    /* check if interface is initialized and packet has correct size */
    if ((NULL != mIpSerialtunInterfaceHandle) && (size > ENET_HEADER_SIZE))
    {
        FLib_MemCpy(&enetpkt, pInData, ENET_HEADER_SIZE);
        protocol = ntohas(enetpkt.type);

        /* Make sure IP packet is aligned */
        size -= ENET_HEADER_SIZE;
        FLib_MemInPlaceCpy(pInData, (uint8_t *) pInData + ENET_HEADER_SIZE, size);

        IPIF_STATS_ENABLED((*mIpSerialtunInterfaceHandle)->stats.commonStats.rxTotal++);
        IPIF_STATS_ENABLED((*mIpSerialtunInterfaceHandle)->stats.rxOctets += size);

        pIpPktInfo = NWKU_CreateIpPktInfo();

        if (NULL != pIpPktInfo)
        {
            /* create NWK Buffer and keep a pointer to the allocated buffer */
            pIpPktInfo->pNwkBuff = NWKU_CreateNwkBuffer(0);

            if (NULL != pIpPktInfo->pNwkBuff)
            {
                pIpPktInfo->ipPktOptions.ifHandle = mIpSerialtunInterfaceHandle;
                pIpPktInfo->pNwkBuff->pData = pInData;
                pIpPktInfo->pNwkBuff->size = size;
                pIpPktInfo->pNextProt = pInData;
                pIpPktInfo->prot.nextProtLen = size;
                pIpPktInfo->ipPktOptions.srcLlInfo.addrSize = gLlayerAddrEui48_c;
                FLib_MemCpy(pIpPktInfo->ipPktOptions.srcLlInfo.eui,
                            enetpkt.source, gLlayerAddrEui48_c);
                IP_Receive(pIpPktInfo, protocol);
            }
            else
            {
                NWKU_FreeIpPktInfo(&pIpPktInfo);
                ///TBD: send out of memory event
            }
        }
        else
        {
            ///TBD: send out of memory event
        }
    }
    else
    {
        MEM_BufferFree(pInData);
    }
}

/*!*************************************************************************************************
\fn uint32_t IP_SerialTunOpen6(ifHandle_t ifHandle)
\brief  Enable Ipv6 on the provided interface

\param [in]  pIfHandle    interface handle

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_SerialTunOpen6
(
    ifHandle_t ifHandle
)
{
    uint32_t  error = gIpGeneralError_c;

    /* Link-Local Address Generation/Auto configuration.
     * It comprises of '1111111010' as the first ten bits followed by 54 zeroes
     * and a 64 bit interface identifier.
     * For all autoconfiguration types, a link-local address is always configured.*/
    error = IP_IF_BindAddr6(ifHandle->ifUniqueId, NULL, ip6AddrTypeAutoconfigurable_c,
                            IP6_ADDRESS_LIFETIME_INFINITE, 64U);
    return error;
}

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn uint32_t IP_SerialTunOpen(ifHandle_t ifHandle)
\brief  Registers IP and ARP with an Virtual Ethernet packet driver

\param [in]  pIfHandle    interface handle

\retval      uint32_t     error
***************************************************************************************************/
uint32_t IP_SerialTunOpen
(
    ifHandle_t ifHandle
)
{
    uint32_t  error = gIpInvalidParameterError_c;

    if ((1 == ifHandle->ipVersion6) &&
            (gIpIfSerialTun_c == ifHandle->ifUniqueId))
    {
        ifHandle->ifMetric = gIpTunMetric_c;
        ifHandle->ifMtu = SERIAL_TUN_MTU;
        ifHandle->ifDevAddrTbl[0].addrSize = gLlayerAddrEui48_c;

        VIRTUAL_TUN_get_address(ifHandle->ifDevAddrTbl[0].eui);

        /* Keep mac driver handle for later use(i.e. IP_serialtunRecv function) */
        mIpSerialtunInterfaceHandle = ifHandle;

        error = IP_SerialTunOpen6(ifHandle);

        if (gIpOk_c != error)
        {
            IP_SerialTunClose(ifHandle);
        }
    }

    return error;
}
/*!*************************************************************************************************
\fn uint32_t IP_SerialTunClose(ifHandle_t ifHandle)
\brief  Unregisters IP and ARP with an Virtual Ethernet packet driver

\param [in]  pIfHandle    interface handle

\retval      uint32_t     error
***************************************************************************************************/
uint32_t IP_SerialTunClose
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
            if (addr->ipIfId == ifHandle->ifUniqueId)
            {
                IP_IF_UnbindAddr6(ifHandle->ifUniqueId, &addr->ip6Addr);
            }
        }
    }

    /* Leaves the all-nodes multicast address. */
    //IP_IF_Leave(ifHandle->ifUniqueId, &in6addr_linklocal_allnodes);

    /* Deregister IP6. */
    //error = VIRTUAL_ENET_close();

    return error;
#else
    return 0;
#endif
}

/*!*************************************************************************************************
\fn uint32_t IP_SerialTunSend6(ipPktInfo_t* pIpPktInfo)
\brief  Sends an IPv6 packet via FSCI interface.

\param [in]  pIpPktInfo    the packet to send

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_SerialTunSend6
(
    ipPktInfo_t *pIpPktInfo
)
{
    ifHandle_t ifHandle = ((ifHandle_t)pIpPktInfo->ipPktOptions.ifHandle);
    uint32_t bufferSize;
    uint8_t *pBuff;
    nwkBuffer_t *pNwkBuff;
    uint32_t currentPos;

    /* get destination MAC addr from cache or from NS */
    llAddr_t *pEnetDstInf = NULL, enetDst;
    enetHdr_t enetHeader;

    ndCfg_t **ppNd6Cfg = ND_GetCfg(ifHandle->ifUniqueId);

    if (IP6_IsMulticastAddr(pIpPktInfo->pIpDstAddr))
    {
        IP6_MULTICAST_TO_ENET_ADDR(pIpPktInfo->pIpDstAddr, enetDst.eui);
        pEnetDstInf = &enetDst;
        pEnetDstInf->addrSize = gLlayerAddrEui48_c;
    }
    else
    {
        if ((*ppNd6Cfg)->ndIf->ndResolveDstLinkLayerAddr)
        {
            pEnetDstInf = (*ppNd6Cfg)->ndIf->ndResolveDstLinkLayerAddr(pIpPktInfo);

            /* ndResolveDstLinkLayerAddr may drop the packet */
            if (!pIpPktInfo)
            {
                return 1;
            }
        }
    }

    IPIF_STATS_ENABLED(ifHandle->stats.commonStats.txTotal++);
    IPIF_STATS_ENABLED(ifHandle->stats.txOctets += NWKU_NwkBufferTotalSize(pIpPktInfo->pNwkBuff));

    bufferSize = NWKU_NwkBufferTotalSize(pIpPktInfo->pNwkBuff);
    bufferSize += ENET_HEADER_SIZE;
    pBuff = NWKU_MEM_BufferAlloc(bufferSize);

    if (pBuff)
    {
        if (pEnetDstInf)
        {
            /* add Ethernet header */
            FLib_MemCpy(enetHeader.dest, pEnetDstInf->eui, pEnetDstInf->addrSize);
            VIRTUAL_TUN_get_address(enetHeader.source);
            htonas(enetHeader.type, gIpProtv6_c);
            FLib_MemCpy(pBuff, &enetHeader, ENET_HEADER_SIZE);
            currentPos = ENET_HEADER_SIZE;
        }
        else
        {
            MEM_BufferFree(pBuff);
            /* don't free pIpPktInfo as it's been queued by ND_AddressResolutionStart */
            return 1;
        }

        pNwkBuff = pIpPktInfo->pNwkBuff;

        while (pNwkBuff)
        {
            FLib_MemCpy(pBuff + currentPos, pNwkBuff->pData, pNwkBuff->size);

            currentPos += pNwkBuff->size;
            pNwkBuff = pNwkBuff->next;
        }
#if SERIAL_TUN_IF
        Serialtun_Send(pBuff, bufferSize); /* The buffer is handed to this function */
#endif
    }

    NWKU_FreeIpPktInfo(&pIpPktInfo);

    return 0;
}

/*!*************************************************************************************************
\fn uint32_t IP_SerialTunGetIID(ifHandle_t ifHandle)
\brief  Gets the Interface identifier

\param [in]  pIfHandle  interface handle
\param [in]  macAddr    link layer address
\param [out] ipAddr     IP address to store the IID in

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_SerialTunGetIID
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

