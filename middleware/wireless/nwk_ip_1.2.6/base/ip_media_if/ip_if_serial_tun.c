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
#include "ip_if_serial_tun.h"

#include "FsciInterface.h"
#include "thci.h"
#include "app_serial_tun.h"


/*==================================================================================================
Private macros
==================================================================================================*/
#ifndef IP_DISABLE_INTERFACE_FUNCTIONALITY
    #define IP_DISABLE_INTERFACE_FUNCTIONALITY 0
#endif

#define SERIAL_TUN_MTU      (1500U)
#define ENET_HEADER_SIZE    14U
#define SERIAL_TAP_ROUTER   (THR_SERIAL_TUN_ENABLE_ND_HOST || THR_SERIAL_TUN_ENABLE_ND_ROUTER)

#define IP6_MULTICAST_TO_ENET_ADDR(ip6,eth_addr)  \
        (eth_addr)[0] = 0x33,             \
        (eth_addr)[1] = 0x33,             \
        (eth_addr)[2] = ip6->addr8[12],   \
        (eth_addr)[3] = ip6->addr8[13],   \
        (eth_addr)[4] = ip6->addr8[14],   \
        (eth_addr)[5] = ip6->addr8[15]

#define MAC_ADDR_SIZE 6
#define MULTICAST_GROUPS_TABLE_SIZE 10

/*==================================================================================================
Private type definitions
==================================================================================================*/
typedef struct enetHdr_tag
{
    uint8_t    dest[6];
    uint8_t    source[6];
    uint8_t    type[2];
} enetHdr_t;

typedef struct macAddr_tag { uint8_t data[6]; } macAddr_t;

/*==================================================================================================
Private prototypes
==================================================================================================*/
uint32_t IP_SerialTunOpen(ifHandle_t ifHandle);
uint32_t IP_SerialTunClose(ifHandle_t ifHandle);
uint32_t IP_SerialTunSend6(ipPktInfo_t *pIpPktInfo);
uint32_t IP_SerialTunOpen6(ifHandle_t ifHandle);
uint32_t IP_SerialTunGetIID(ifHandle_t ifHandle, llAddr_t *macAddr, ipAddr_t *ipAddr);
uint32_t IP_SerialTunJoin(ifHandle_t ifHandle, ipAddr_t *pIpAddr, uint16_t protocol);
uint32_t IP_SerialTunLeave(ifHandle_t ifHandle, ipAddr_t *pIpAddr, uint16_t protocol);

#if SERIAL_TAP_ROUTER
    static bool_t IP_SerialTunHeaderPassesFiltering(void *pEthernetHeader);
#endif

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
    IP_SerialTunJoin,
    IP_SerialTunLeave
};
const mediaIfStruct_t *gVirtualTunMediaIfPtr = (mediaIfStruct_t *) &gVirtualTunMediaIf;

static macAddr_t maIPv6MCastMacAddrTable[MULTICAST_GROUPS_TABLE_SIZE] = { 0 };
static ipIfSerialTunLinkStateChangedCb_t mpfSerialTunLinkStateChanged = NULL;

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

#if SERIAL_TAP_ROUTER
    enetHdr_t enetHdr;
    uint16_t protocol;

    FLib_MemCpy(&enetHdr, pInData, ENET_HEADER_SIZE);
    protocol = ntohas(enetHdr.type);

    /* check if mac address is allowed */
    if (!IP_SerialTunHeaderPassesFiltering(&enetHdr))
    {
        MEM_BufferFree(pInData);
        return;
    }

    /* Make sure IP packet is aligned */
    size -= ENET_HEADER_SIZE;
    FLib_MemInPlaceCpy(pInData, (uint8_t *) pInData + ENET_HEADER_SIZE, size);
#endif

    /* check if interface is initialized */
    if (NULL != mIpSerialtunInterfaceHandle)
    {
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
#if SERIAL_TAP_ROUTER
                pIpPktInfo->ipPktOptions.srcLlInfo.addrSize = gLlayerAddrEui48_c;
                FLib_MemCpy(pIpPktInfo->ipPktOptions.srcLlInfo.eui,
                            enetHdr.source, gLlayerAddrEui48_c);
                IP_Receive(pIpPktInfo, protocol);
#else
                pIpPktInfo->ipPktOptions.srcLlInfo.addrSize = gLlayerAddrNoAddr_c;
                IP_Receive(pIpPktInfo, gIpProtv6_c);
#endif
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

#if SERIAL_TAP_ROUTER
    /* get destination MAC addr from cache or from NS */
    llAddr_t *pEnetDstInf = NULL, enetDst;
    enetHdr_t enetHeader;

    ndCfg_t **ppNd6Cfg = ND_GetCfg(ifHandle);

    if (ppNd6Cfg == NULL || *ppNd6Cfg == NULL)
    {
        NWKU_FreeIpPktInfo(&pIpPktInfo);
        return 1;
    }

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

#else
    (void)ifHandle;
#endif

    IPIF_STATS_ENABLED(ifHandle->stats.commonStats.txTotal++);
    IPIF_STATS_ENABLED(ifHandle->stats.txOctets += NWKU_NwkBufferTotalSize(pIpPktInfo->pNwkBuff));

    bufferSize = NWKU_NwkBufferTotalSize(pIpPktInfo->pNwkBuff);
#if SERIAL_TAP_ROUTER
    bufferSize += ENET_HEADER_SIZE;
#endif
    pBuff = NWKU_MEM_BufferAlloc(bufferSize);

    if (pBuff)
    {
#if SERIAL_TAP_ROUTER

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

#else
        currentPos = 0;
#endif

        pNwkBuff = pIpPktInfo->pNwkBuff;

        while (pNwkBuff)
        {
            FLib_MemCpy(pBuff + currentPos, pNwkBuff->pData, pNwkBuff->size);

            currentPos += pNwkBuff->size;
            pNwkBuff = pNwkBuff->next;
        }

        FSCI_transmitPayload(gFSCI_IpStackOpGCnf_c, gTHCI_IpStackSerialtunReceive_c, pBuff, bufferSize,
                             FSCI_INTERFACE);
    }

    MEM_BufferFree(pBuff);
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

/*!*************************************************************************************************
\fn     uint32_t IP_SerialTunJoin(ifHandle_t ifHandle, ipAddr_t *pIpAddr, uint16_t protocol)
\brief  Joins an IPv6 multicast group.

\param [in]  pIfHandle  interface handle
\param [in]  pIpAddr    the multicast group
\param [in]  protocol   the protocol for the multicast group, only IPv6 is supported

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_SerialTunJoin
(
    ifHandle_t ifHandle,
    ipAddr_t *pIpAddr,
    uint16_t protocol
)
{
    uint8_t groupAddr[MAC_ADDR_SIZE];
    uint8_t invalidMacAddr[MAC_ADDR_SIZE] = { 0 };
    uint32_t error = gIpNoAddressSpaceError_c;

    if (gIpProtv6_c == protocol)
    {
        IP6_MULTICAST_TO_ENET_ADDR(pIpAddr, groupAddr);

        for (uint8_t i = 0; i < MULTICAST_GROUPS_TABLE_SIZE; i++)
        {
            if (FLib_MemCmp(maIPv6MCastMacAddrTable[i].data, invalidMacAddr, MAC_ADDR_SIZE))
            {
                FLib_MemCpy(maIPv6MCastMacAddrTable[i].data, groupAddr, MAC_ADDR_SIZE);
                error = gIpOk_c;
                break;
            }
            else if (FLib_MemCmp(maIPv6MCastMacAddrTable[i].data, groupAddr, MAC_ADDR_SIZE))
            {
                error = gIpOk_c;
                break;
            }
        }
    }
    else
    {
        error = gIpBadAddrError_c;
    }

    return error;
}

/*!*************************************************************************************************
\fn     uint32_t IP_SerialTunLeave(ifHandle_t ifHandle, ipAddr_t *pIpAddr, uint16_t protocol)
\brief  Leaves an IPv6 multicast group.

\param [in]  pIfHandle  interface handle
\param [in]  pIpAddr    the multicast group
\param [in]  protocol   the protocol for the multicast group, only IPv6 is supported

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_SerialTunLeave
(
    ifHandle_t ifHandle,
    ipAddr_t *pIpAddr,
    uint16_t protocol
)
{
    uint8_t groupAddr[6];
    uint32_t error = gIpInvalidParameterError_c;

    if (gIpProtv6_c == protocol)
    {
        IP6_MULTICAST_TO_ENET_ADDR(pIpAddr, groupAddr);

        for (uint8_t i = 0; i < MULTICAST_GROUPS_TABLE_SIZE; i++)
        {
            if (FLib_MemCmp(maIPv6MCastMacAddrTable[i].data, groupAddr, MAC_ADDR_SIZE))
            {
                FLib_MemSet(maIPv6MCastMacAddrTable[i].data, 0, MAC_ADDR_SIZE);
                error = gIpOk_c;
                break;
            }
        }
    }
    else
    {
        error = gIpBadAddrError_c;
    }

    return error;
}

/*!*************************************************************************************************
\fn     void IP_SerialTun_RegisterLinkChangedCb(
            ipIfSerialTunLinkStateChangedCb_t pfSerialTunLinkStateChanged)
\brief  Called on link change events.

\param [in]   pfSerialTunLinkStateChanged    Function pointer
***************************************************************************************************/
void IP_SerialTun_RegisterLinkChangedCb
(
    ipIfSerialTunLinkStateChangedCb_t pfSerialTunLinkStateChanged
)
{
    mpfSerialTunLinkStateChanged = pfSerialTunLinkStateChanged;
}

/*!*************************************************************************************************
\fn     void IP_SerialTunLinkChanged(ifHandle_t ifHandle, uint8_t newState)
\brief  Take the appropriate actions on link change event.

\param  [in]    ifHandle         pointer to media interface configuration structure
\param  [in]    newState         0: link down, 1: link up
***************************************************************************************************/
void IP_SerialTunLinkChanged
(
    ifHandle_t ifHandle,
    uint8_t newState
)
{
    ndCfg_t **ppNdCfg = ND_GetCfg(ifHandle);
    ndCfg_t *pNdCfg = ppNdCfg ? *ppNdCfg : NULL;

    if (pNdCfg && pNdCfg->ndIf && pNdCfg->ndIf->ndLinkStateChanged)
    {
        pNdCfg->ndIf->ndLinkStateChanged(ifHandle, newState);
    }

    if (mpfSerialTunLinkStateChanged)
    {
        mpfSerialTunLinkStateChanged(newState);
    }
}

#if SERIAL_TAP_ROUTER
/*!*************************************************************************************************
\fn     static bool_t IP_SerialTunHeaderPassesFiltering(void *pEthernetHeader)
\brief  Minimalistic firewall at Ethernet layer.

\param [in]  pEthernetHeader    pointer to the Ethernet header

\retval      bool_t             TRUE if packet shall be passed to upper layers, FALSE otherwise
***************************************************************************************************/
static bool_t IP_SerialTunHeaderPassesFiltering
(
    void *pEthernetHeader
)
{
    uint16_t protocol;
    enetHdr_t *pEnetHdr = pEthernetHeader;
    protocol = (pEnetHdr->type[0] << 8) + pEnetHdr->type[1];

    if (gIpProtv6_c != protocol)
    {
        return FALSE;
    }

    if (FLib_MemCmp(mIpSerialtunInterfaceHandle->ifDevAddrTbl[0].eui, pEnetHdr->dest, MAC_ADDR_SIZE))
    {
        return TRUE;
    }

    /* check for broadcast */
    if ((pEnetHdr->dest[5] == 0xFF) && (pEnetHdr->dest[4] == 0xFF) && (pEnetHdr->dest[3] == 0xFF) &&
            (pEnetHdr->dest[2] == 0xFF) && (pEnetHdr->dest[1] == 0xFF) && (pEnetHdr->dest[0] == 0xFF))
    {
        return TRUE;
    }

    for (uint8_t i = 0; i < MULTICAST_GROUPS_TABLE_SIZE; i++)
    {
        if (FLib_MemCmp(maIPv6MCastMacAddrTable[i].data, pEnetHdr->dest, MAC_ADDR_SIZE))
        {
            return TRUE;
        }
    }

    return FALSE;
}
#endif  /* SERIAL_TAP_ROUTER */
