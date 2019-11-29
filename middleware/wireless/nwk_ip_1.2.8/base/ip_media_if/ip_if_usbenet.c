/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/*!=================================================================================================
 \file       ip_if_usbenet.c
 \brief      This is a private source file for the virtual Ethernet over USB CDC interface (RNDIS)
 ==================================================================================================*/
#include "EmbeddedTypes.h"
#include "MemManager.h"
#include "FunctionLib.h"
#include "Panic.h"
#include "ip_if_management.h"
#include "network_utils.h"

#if USB_RNDIS_IF

#include "rndis.h"
#if gFsciIncluded_c
#include "rndis_fsci.h"
#endif

#include "ip.h"
#include "ip6.h"
#include "ip4.h"
#include "arp.h"
#include "ip_if_management.h"
#include "ip_if_usbenet.h"
#include "dhcp_client.h"
#include "thread_stack_config.h"
#include "thread_app_callbacks.h"
#include "thread_network.h"
#include "app_init.h"

#if MLD_ENABLED
#include "mld.h"
#endif

#include "FsciInterface.h"
#if gFsciIncluded_c
    extern gFsciStatus_t FSCI_ProcessRxPkt(clientPacket_t *pPacket, uint32_t thciInterface);
#endif

#if IP_IP4_ENABLE || IP_IP6_ENABLE

/*==================================================================================================
 Private macros
 ==================================================================================================*/

#define IP4_BROADCAST_TO_USBENET_ADDR(eth_addr) \
        (eth_addr)[0] = 0xFF, \
        (eth_addr)[1] = 0xFF, \
        (eth_addr)[2] = 0xFF, \
        (eth_addr)[3] = 0xFF, \
        (eth_addr)[4] = 0xFF, \
        (eth_addr)[5] = 0xFF

#define IP4_MULTICAST_TO_USBENET_ADDR(ip,eth_addr)  \
        (eth_addr)[0] = 0x01, \
        (eth_addr)[1] = 0x00, \
        (eth_addr)[2] = 0x5E, \
        (eth_addr)[3] = ((ip) >> 16) & 0x7F, \
        (eth_addr)[4] = ((ip) >>  8) & 0xFF, \
        (eth_addr)[5] =  (ip)        & 0xFF

#define IP6_MULTICAST_TO_USBENET_ADDR(ip6,eth_addr)  \
        (eth_addr)[0] = 0x33,             \
        (eth_addr)[1] = 0x33,             \
        (eth_addr)[2] = ip6->addr8[12],   \
        (eth_addr)[3] = ip6->addr8[13],   \
        (eth_addr)[4] = ip6->addr8[14],   \
        (eth_addr)[5] = ip6->addr8[15]

#define USBENET_HEADER_SIZE  14U

#define USBENET_TYPE_UNICAST                    0x01
#define USBENET_TYPE_BROADCAST                  0x02
#define USBENET_TYPE_MULTICAST                  0x04

#ifndef USBENET_MAC_ADDR
    #define USBENET_INTERNAL_MAC_ADDR 0x00, 0x60, 0x37, 0x00, 0xFA, 0x5E
#endif
#ifndef USBENET_MAC_ADDR
    #define USBENET_RNDIS_MAC_ADDR    0x00, 0x60, 0x37, 0x00, 0xFA, 0x5D
#endif

#ifndef USBENET_MAX_MTU
    #define    USBENET_MAX_MTU 1500
#endif

#ifndef USBENET_GROUPADDR_MAX_SIZE
    #define USBENET_GROUPADDR_MAX_SIZE 4
#endif

#ifndef gRDIS_FSCI_Interface_c
    #define gRDIS_FSCI_Interface_c 0
#endif

#ifndef gFsciInProt_c
    #define gFsciInProt_c  0x88B5
#endif
/*==================================================================================================
Private type definitions
==================================================================================================*/
typedef uint8_t   usbenet_address_t[6];

typedef struct usbenetHdr_tag
{
    uint8_t    dest[6];
    uint8_t    source[6];
    uint8_t    type[2];
} usbenetHdr_t;

/*==================================================================================================
Private prototypes
==================================================================================================*/

uint32_t IP_UsbEnetOpen(ifHandle_t ifHandle);
uint32_t IP_UsbEnetClose(ifHandle_t ifHandle);
uint32_t IP_UsbEnetSendCommon(ipPktInfo_t *pIpPktInfo, llAddr_t *pLlAddr, uint16_t protocol);
uint32_t IP_UsbEnetJoin(ifHandle_t ifHandle, ipAddr_t *pIpAddr, uint16_t protocol);
uint32_t IP_UsbEnetLeave(ifHandle_t ifHandle, ipAddr_t *pIpAddr, uint16_t protocol);

#if IP_IP4_ENABLE
    uint32_t IP_UsbEnetOpen4(ifHandle_t ifHandle);
    uint32_t IP_UsbEnetSend4(ipPktInfo_t *pIpPktInfo);
    uint32_t IP_UsbEnetSendArp(ipPktInfo_t *pIpPktInfo, llAddr_t *pLlAddr);
#endif

#if IP_IP6_ENABLE
    uint32_t IP_UsbEnetSend6(ipPktInfo_t *pIpPktInfo);
    uint32_t IP_UsbEnetOpen6(ifHandle_t ifHandle);
    uint32_t IP_UsbEnetGetIID(ifHandle_t ifHandle, llAddr_t *macAddr, ipAddr_t *ipAddr);
#endif

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
static bool_t enableIPV6Rx = FALSE;
static bool_t enableIPV4Rx = FALSE;
static bool_t enableARPRx = FALSE;

static bool_t bUsbEnable = FALSE;
static bool_t bUsbInit   = FALSE;

/*==================================================================================================
 Public global variables declarations
 ==================================================================================================*/
ifHandle_t ifHandleUsbEnet = NULL;

const mediaIfStruct_t gUsbEnetMediaIf =
{
    IP_UsbEnetOpen,
    IP_UsbEnetClose,
#if IP_IP4_ENABLE
    IP_UsbEnetSend4,
    IP_UsbEnetSendArp,
#else
    NULL,
    NULL,
#endif
#if IP_IP6_ENABLE
    IP_UsbEnetSend6,
    IP_UsbEnetGetIID,
#else
    NULL,
    NULL,
#endif
    IP_UsbEnetJoin,
    IP_UsbEnetLeave
};

const mediaIfStruct_t *gUsbEnetMediaIfPtr = (mediaIfStruct_t *) &gUsbEnetMediaIf;

usbenet_address_t usbenet_internal_mac_addr = { USBENET_INTERNAL_MAC_ADDR };
usbenet_address_t usbenet_rndis_mac_addr = { USBENET_RNDIS_MAC_ADDR };
usbenet_address_t usbenetGroupAddrTable[USBENET_GROUPADDR_MAX_SIZE] = { 0 };
/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn  void IP_UsbEnetGetMacAddr(uint8_t *pMacAddr, uint8_t addrLen, bool_t internalAddr)

\brief  Function used to retrieve USB ENET MAC addresses

\param [out] pMacAddr      Pointer to the location where the address will be put
\param [in]  addrLen       Length of the address
\param [in]  internalAddr  TRUE: internal demo mac address
                           FALSE: rndis demo mac address

\retval      none
***************************************************************************************************/
void IP_UsbEnetGetMacAddr
(
    uint8_t *pMacAddr, 
    uint8_t addrLen, 
    bool_t internalAddr
)
{
    if (internalAddr == TRUE)
    {
        FLib_MemCpy(pMacAddr, usbenet_internal_mac_addr, sizeof(usbenet_address_t));
    }
    else
    {
        FLib_MemCpy(pMacAddr, usbenet_rndis_mac_addr, sizeof(usbenet_address_t));
    }
}

/*!*************************************************************************************************
\fn  void IP_UsbEnetRecv(uint8_t *pInData, uint16_t size)

\brief  Services an Ethernet packet

\param [in]  pInData       Pointer to received buffer
\param [in]  size          Length of received buffer

\retval      none
***************************************************************************************************/
void IP_UsbEnetRecv
(
    void *pInData,
    uint16_t size
)
{
    ipPktInfo_t *pIpPktInfo;
    usbenetHdr_t usbenetpkt;
    uint16_t protocol;

    /* Extract EtherType */
    FLib_MemCpy(&usbenetpkt, pInData, USBENET_HEADER_SIZE);
    protocol = ntohas(usbenetpkt.type);

#if gFsciIncluded_c

    if ((gFsciInProt_c == protocol) &&
            (FLib_MemCmp(usbenet_internal_mac_addr, usbenetpkt.dest, sizeof(usbenet_address_t))))
    {
        size -= USBENET_HEADER_SIZE;
        FLib_MemInPlaceCpy(pInData, (uint8_t *)pInData + USBENET_HEADER_SIZE, size);
        (void)FSCI_ProcessRxPkt((clientPacket_t *)pInData, 0);
        return;
    }

#endif

    /* Check if interface was enabled */
    if ((NULL == ifHandleUsbEnet) || (FALSE == bUsbEnable))
    {
        MEM_BufferFree(pInData);
        return;
    }

    size -= USBENET_HEADER_SIZE;
    FLib_MemInPlaceCpy(pInData, (uint8_t *) pInData + USBENET_HEADER_SIZE, size);

    pIpPktInfo = NWKU_CreateIpPktInfo();

    if (NULL != pIpPktInfo)
    {
        /* Create NWK Buffer but do not allocate memory for it */
        pIpPktInfo->pNwkBuff = NWKU_CreateNwkBuffer(0);

        if (NULL != pIpPktInfo->pNwkBuff)
        {
            pIpPktInfo->ipPktOptions.ifHandle = ifHandleUsbEnet;
            pIpPktInfo->pNwkBuff->pData = pInData;
            pIpPktInfo->pNwkBuff->size = size;
            pIpPktInfo->pNwkBuff->freeBuffer = 1;

            pIpPktInfo->pNextProt = pInData;
            pIpPktInfo->ipPktOptions.srcLlInfo.addrSize = gLlayerAddrEui48_c;
            FLib_MemCpy(pIpPktInfo->ipPktOptions.srcLlInfo.eui,
                        usbenetpkt.source, gLlayerAddrEui48_c);

            IP_Receive(pIpPktInfo, protocol);
        }
        else
        {
            MEM_BufferFree(pInData);
            NWKU_FreeIpPktInfo(&pIpPktInfo);
        }
    }
    else
    {
        MEM_BufferFree(pInData);
    }
}

#if IP_IP6_ENABLE
/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetOpen6(ifHandle_t ifHandle)
\brief  Enable Ipv6 on the provided interface

\param [in]  pIfHandle      interface handle

\retval      uint32_t       error
***************************************************************************************************/
uint32_t IP_UsbEnetOpen6
(
    ifHandle_t ifHandle
)
{
    uint32_t error = gIpOk_c;
    rndisStatus_t rndisStatus = gRndis_Success_c;

    ifHandleUsbEnet = ifHandle;

    if (gRndis_Success_c == rndisStatus)
    {
        rndisStatus = RNDIS_EnableIPv6(TRUE);

        if (gRndis_Success_c != rndisStatus)
        {
            panic(0, (uint32_t) IP_UsbEnetOpen6, 0, 0);
        }
        else
        {
            enableIPV6Rx = TRUE;
        }
    }

    /* RFC4861 6.3.3: The host joins the all-nodes multicast address on all
    multicastcapable interfaces. */
    IP_IF_AddMulticastGroup6(ifHandle->ifUniqueId, (ipAddr_t *)&in6addr_linklocal_allnodes);

    /* Link-Local Address Generation/Auto configuration.
     * It comprises of '1111111010' as the first ten bits followed by 54 zeroes
     * and a 64 bit interface identifier.
     * For all autoconfiguration types, a link-local address is always configured.*/
    error = IP_IF_BindAddr6(ifHandle->ifUniqueId, NULL, ip6AddrTypeAutoconfigurable_c,
                            IP6_ADDRESS_LIFETIME_INFINITE, 64U);

    if (gRndis_Success_c != rndisStatus)
    {
        error = gIpGeneralError_c;
    }
    else
    {
        IP_UsbEnetEnable();
    }
    return error;

}
#endif
#if IP_IP4_ENABLE
/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetOpen4(ifHandle_t ifHandle)
\brief  Enable Ipv4 on the provided interface

\param [in]  pIfHandle  interface handle

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_UsbEnetOpen4
(
    ifHandle_t ifHandle
)
{
    uint32_t error = gIpGeneralError_c;
    rndisStatus_t rndisStatus = gRndis_Success_c;
    ifHandleUsbEnet  = ifHandle;

    if (gRndis_Success_c == rndisStatus)
    {
        rndisStatus = RNDIS_EnableIPv4(TRUE);

        if (gRndis_Success_c != rndisStatus)
        {
            panic(0, (uint32_t)IP_UsbEnetOpen4, 0, 0);
        }
        else
        {
            enableIPV4Rx = TRUE;
            enableARPRx = TRUE;
        }
    }

    if (gRndis_Success_c == rndisStatus)
    {
        error = ARP_Open(ifHandle);
    }
    else
    {
        error = gIpGeneralError_c;
    }
    if (gRndis_Success_c == rndisStatus)
    {
        IP_UsbEnetEnable();
    }
    return error;
}
#endif

/*!*************************************************************************************************
\fn uint32_t USBENET_send(void* usbenetHandle,ipPktInfo_t* packet,uint16_t protocol, uint8_t* dest,
                       uint32_t  flags)
\brief  Sends a packet.

\param [in]  usbenetHandle    handle to Ethernet driver
\param [in]  inData        the packet to send
\param [in]  protocol      the protocol to send
\param [in]  dest          the destination Ethernet address
\param [in]  flags         optional flags, zero = default

\retval       uint32_t     USBENET_OK
                           error code
***************************************************************************************************/
uint32_t USBENET_send
(
    void *usbenetHandle,
    ipPktInfo_t *packet,
    uint16_t protocol,
    uint8_t  *dest,
    uint32_t  flags
)
{
    uint8_t headerLen;

    uint32_t returnStatus = 0;
    rndisStatus_t rndisStatus = gRndis_Success_c;
    nwkBuffer_t *pNwkBuffer = packet->pNwkBuff;
    usbenetHdr_t *packetPtr;
    uint8_t *frame;
    uint16_t frameLen;
    uint16_t prot;
    uint8_t *pMACData;

    /* Default frame header size*/
    headerLen = sizeof(usbenetHdr_t);
    frameLen = NWKU_NwkBufferTotalSize(packet->pNwkBuff);

    frame = NWKU_MEM_BufferAlloc(frameLen + headerLen);

    if (NULL == frame)
    {
        NWKU_FreeIpPktInfo(&packet);
        return gIpGeneralError_c;
    }

    /* Add MAC hardware address */
    packetPtr = (usbenetHdr_t *) frame;
    FLib_MemCpy(packetPtr->dest, dest, 6);
    FLib_MemCpy(packetPtr->source, usbenet_internal_mac_addr,
                sizeof(usbenet_address_t));
    prot = htons(protocol);
    FLib_MemCpy(packetPtr->type, &prot, 2);

    pMACData = NWKU_NwkBufferToRegularBuffer(pNwkBuffer, frame + headerLen);
    /* Free the PCB buffer*/
    NWKU_FreeIpPktInfo(&packet);

    if (NULL == pMACData)
    {
        MEM_BufferFree(frame);
        return gIpGeneralError_c;
    }

    /* Send packet to the device*/
    rndisStatus = RNDIS_Send(frame, frameLen + headerLen);

    if (rndisStatus != gRndis_Success_c)
    {
        panic(0, (uint32_t) USBENET_send, 0, 0);
        returnStatus = gIpGeneralError_c;
    }

    return returnStatus;
}

/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetSendCommon(ipPktInfo_t* pIpPktInfo,llAddr_t* pLlAddr,uin16_t protocol)
\brief  Sends an IP packet.

\param [in]  pIpPktInfo    the packet to send
\param [in]  pLlAddr       the link layer address to send the packet
\param [in]  protocol      the protocol to send

\retval      uint32_t      error
***************************************************************************************************/
uint32_t IP_UsbEnetSendCommon
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
        error = USBENET_send(ifHandle->ifDriverHandle, pIpPktInfo, protocol, pLlAddr->eui, 0);
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

/*!*************************************************************************************************
\fn void USBENET_Initialize(void)
\brief  Initialize RNDIS module and interface

\param [in]  void

\retval      void
***************************************************************************************************/
void USBENET_Initialize
(
    void
)
{
    rndisStatus_t rndisStatus = gRndis_Success_c;
    uint32_t gaUniqueId[4];

    if (FALSE == bUsbInit)
    {
        RNDIS_RegisterRxCallback(IP_UsbEnetRecv);

#if defined(CPU_MKW41Z512VHT4)
        fsciRegisterRndisToHost(gRDIS_FSCI_Interface_c);
        /* Reset rndis (local or bridge) for  synchronization*/
        FSCI_ResetReq(gRDIS_FSCI_Interface_c);
        /* Wait 100 ms */
        OSA_TimeDelay(200);
#endif
        THR_GetUniqueId(gaUniqueId);
        /* Configure rndis demo mac address */
        FLib_MemCpy(&usbenet_rndis_mac_addr[3], (uint8_t *)&gaUniqueId[3], 3);
        /* Configure internal demo mac address */
        FLib_MemCpyReverseOrder(&usbenet_internal_mac_addr[3], (uint8_t *)&gaUniqueId[3], 3);

        /* Set rndis and internal mac adresses and init the rndis module */
        rndisStatus = RNDIS_SetRndisMacAddress(usbenet_rndis_mac_addr);

        if (gRndis_Success_c == rndisStatus)
        {
            rndisStatus = RNDIS_SetDestMacAddress(usbenet_internal_mac_addr);

            if (gRndis_Success_c == rndisStatus)
            {
                rndisStatus = RNDIS_Init();
            }
        }

        if (gRndis_Success_c != rndisStatus)
        {
            APP_CriticalExitCb((uint32_t)USBENET_Initialize, 0);
        }

        bUsbInit = TRUE;
    }
}

/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetOpen(ifHandle_t ifHandle)
\brief  Registers IP and ARP with an Ethernet packet driver

\param [in]  pIfHandle    interface handle

\retval      uint32_t     error
***************************************************************************************************/
uint32_t IP_UsbEnetOpen
(
    ifHandle_t ifHandle
)
{
    uint32_t  ipv4Error = gIpOk_c;
    uint32_t  ipv6Error = gIpOk_c;
    uint32_t error = gIpInvalidParameterError_c;
   
    if (gIpIfUsbRndis_c == ifHandle->ifUniqueId)
    {
        ifHandle->ifMetric = gIpUsbMetric_c;
        ifHandle->ifMtu = USBENET_MAX_MTU;
        ifHandle->ifDevAddrTbl[0].addrSize = gLlayerAddrEui48_c;

        FLib_MemCpy(ifHandle->ifDevAddrTbl[0].eui, usbenet_internal_mac_addr, sizeof(usbenet_address_t));
#if IP_IP4_ENABLE

        if (ifHandle->ipVersion4)
        {
            // ipv4Error = IP_UsbEnetOpen4(pIfHandle);
        }

#endif

#if IP_IP6_ENABLE

        if (ifHandle->ipVersion6)
        {
            ipv6Error = IP_UsbEnetOpen6(ifHandle);
        }

#endif

        if ((ipv4Error != gIpOk_c) || (ipv6Error != gIpOk_c))
        {
            IP_UsbEnetClose(ifHandle);
        }
        else
        {
            error = gIpOk_c;
        }
    }

    return error;
}

/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetClose(ifHandle_t ifHandle)
\brief  Unregisters IP and ARP with an Ethernet packet driver

\param [in]  pIfHandle    interface handle

\retval      uint32_t     error
***************************************************************************************************/
uint32_t IP_UsbEnetClose
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
            if (addr->ipIfId == (*pIfHandle)->ifUniqueId)
            {
                IP_IF_UnbindAddr6(ifHandle->ifUniqueId, &addr->ip6Addr);
            }
        }
    }

    /* Leaves the all-nodes multicast address. */
    IP_IF_Leave(ifHandle->ifUniqueId, &in6addr_linklocal_allnodes);

    /* Deregister IP6. */
    //error = USBENET_close(ifHandle->ifDriverHandle,0);
#if defined(CPU_MKW41Z512VHT4)
    /* Reset rndis (local or bridge) for  synchronization*/
    FSCI_ResetReq(gRDIS_FSCI_Interface_c);
    /* Wait 100 ms */
    OSA_TimeDelay(100);
#endif
    return error;
#else
    return 0;
#endif
}
#if IP_IP4_ENABLE
/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetSendArp(ipPktInfo_t* pIpPktInfo, llAddr_t* pLlAddr)
\brief  Sends an ARP packet

\param [in]  pInData    the APR packet to send
\param [in]  pLlAddr    pointer to link layer address to send data

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_UsbEnetSendArp
(
    ipPktInfo_t *pIpPktInfo,
    llAddr_t *pLlAddr
)
{
    return IP_UsbEnetSendCommon(pIpPktInfo, pLlAddr, gIpProtArp_c);
}

/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetSend4(ipPktInfo_t* pIpPktInfo)
\brief  Sends an IPv4 packet.

\param [in]  pIpPktInfo    the packet to send

\retval      uint32_t      error
***************************************************************************************************/
uint32_t IP_UsbEnetSend4
(
    ipPktInfo_t *pIpPktInfo
)
{
    uint32_t error = gIpOk_c;
    llAddr_t usbenetDst;
    usbenetDst.addrSize = gLlayerAddrEui48_c;

    llAddr_t *pUsbEnetDstInf = &usbenetDst;
    IPIF_STATS_ENABLED(ifHandle_t ifHandle = *((ifHandle_t *)pIpPktInfo->ipPktOptions.ifHandle));

    uint32_t ipDst = IP4_AddrToUint32(pIpPktInfo->pIpDstAddr);

    if (ipDst == IP4_ADDR_BROADCAST)
    {
        IPIF_STATS_ENABLED(ifHandle->stats.txBroadcast++);
        IP4_BROADCAST_TO_USBENET_ADDR(usbenetDst.eui);
    }
    else
    {
        if (IP4_MULTICAST(ipDst))
        {
            IPIF_STATS_ENABLED(ifHandle->stats.txMulticast++);
            IP4_MULTICAST_TO_USBENET_ADDR(ipDst, usbenetDst.eui);
        }
        else
        {
            /* resolve unicast address */
            IPIF_STATS_ENABLED(ifHandle->stats.txUnicast++);
            pUsbEnetDstInf = ARP_Resolve(pIpPktInfo);
        }
    }

    error = IP_UsbEnetSendCommon(pIpPktInfo, pUsbEnetDstInf, gIpProtv4_c);

    return error;
}
#endif

#if IP_IP6_ENABLE && ND_ENABLED
/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetSend6(ipPktInfo_t* pIpPktInfo)
\brief  Sends an IPv6 packet.

\param [in]  pIpPktInfo    the packet to send

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_UsbEnetSend6
(
    ipPktInfo_t *pIpPktInfo
)
{
    uint32_t error;
    llAddr_t usbenetDst;
    llAddr_t *pUsbEnetDstInf = NULL;
    ifHandle_t ifHandle = (ifHandle_t)pIpPktInfo->ipPktOptions.ifHandle;
    ndCfg_t **ppNd6Cfg = ND_GetCfg(ifHandle->ifUniqueId);

    if (IP6_IsMulticastAddr(pIpPktInfo->pIpDstAddr))
    {
        IP6_MULTICAST_TO_USBENET_ADDR(pIpPktInfo->pIpDstAddr, usbenetDst.eui);
        pUsbEnetDstInf = &usbenetDst;
        //pUsbEnetDstInf->addrSize = gLlayerAddrEui48_c;

        IPIF_STATS_ENABLED(ifHandle->stats.txMulticast++);
    }
    else
    {
        if ((*ppNd6Cfg)->ndIf->ndResolveDstLinkLayerAddr)
        {
            pUsbEnetDstInf = (*ppNd6Cfg)->ndIf->ndResolveDstLinkLayerAddr(pIpPktInfo);
            IPIF_STATS_ENABLED(ifHandle->stats.txUnicast++);
        }
        else
        {
            IPIF_STATS_ENABLED(ifHandle->stats.commonStats.txDiscarded++);
            NWKU_FreeIpPktInfo(&pIpPktInfo);
        }
    }

    error = IP_UsbEnetSendCommon(pIpPktInfo, pUsbEnetDstInf, gIpProtv6_c);

    return error;
}

/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetGetIID(ifHandle_t ifHandle)
\brief  Gets the Interface identifier

\param [in]  pIfHandle  interface handle
\param [in]  macAddr    link layer address
\param [out] ipAddr     IP address to store the IID in

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_UsbEnetGetIID
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
\fn uint32_t IP_UsbEnetJoin(ifHandle_t ifHandle, ipAddr_t* pIpAddr, uint16_t protocol)
\brief  Joins an IP multicast group (IPv4 or IPv6 depending on the protocol parameter).

\param [in]  pIfHandle  interface handle
\param [in]  pIpAddr    the multicast group
\param [in]  protocol   the protocol for the multicast group(IPv4 or IPv6)

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_UsbEnetJoin
(
    ifHandle_t ifHandle,
    ipAddr_t *pIpAddr,
    uint16_t protocol
)
{
    uint8_t usbenetGroupAddr[6];
    uint32_t error = gIpOk_c;
    rndisStatus_t rndisStatus = gRndis_Success_c;

    if (gIpProtv4_c == protocol)
    {
        uint32_t ipAddr = IP4_AddrToUint32(pIpAddr);
        IP4_MULTICAST_TO_USBENET_ADDR(ipAddr, usbenetGroupAddr);
        rndisStatus = RNDIS_RegisterMulticastMacAddress(
                          rndisMCastMacAddType_IPv4, TRUE, usbenetGroupAddr);
    }
    else
    {
        IP6_MULTICAST_TO_USBENET_ADDR(pIpAddr, usbenetGroupAddr);
        rndisStatus = RNDIS_RegisterMulticastMacAddress(
                          rndisMCastMacAddType_IPv6, TRUE, usbenetGroupAddr);
    }

    if (gRndis_Success_c != rndisStatus)
    {
        error = gIpGeneralError_c;
    }

    return error;
}
/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetLeave6(ifHandle_t* ifHandle, ipAddr_t* pIpAddr,uint16_t protocol)
\brief  Leaves an IP multicast group(IPv4 or IPv6 depending on the protocol parameter).

\param [in]  pIfHandle  interface handle
\param [in]  pIpAddr    the multicast group
\param [in]  protocol   the protocol for the multicast group(IPv4 or IPv6)

\retval      uint32_t   error
***************************************************************************************************/
uint32_t IP_UsbEnetLeave
(
    ifHandle_t ifHandle,
    ipAddr_t *pIpAddr,
    uint16_t protocol
)
{
    uint8_t usbenetGroupAddr[6];
    uint32_t error = gIpOk_c;
    rndisStatus_t rndisStatus = gRndis_Success_c;
    
    if (gIpProtv4_c == protocol)
    {
        uint32_t ipAddr = IP4_AddrToUint32(pIpAddr);
        IP4_MULTICAST_TO_USBENET_ADDR(ipAddr, usbenetGroupAddr);
        rndisStatus = RNDIS_RegisterMulticastMacAddress(
                          rndisMCastMacAddType_IPv4, FALSE, usbenetGroupAddr);
    }
    else
    {
        IP6_MULTICAST_TO_USBENET_ADDR(pIpAddr, usbenetGroupAddr);
        rndisStatus = RNDIS_RegisterMulticastMacAddress(
                          rndisMCastMacAddType_IPv6, FALSE, usbenetGroupAddr);
    }

    if (gRndis_Success_c != rndisStatus)
    {
        error = gIpGeneralError_c;
    }
    
    return error;
}
/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetDisable(void)
\brief  Disable Usb enet Interface

\param [in]  void

\retval      void
***************************************************************************************************/
void IP_UsbEnetDisable(void)
{
    bUsbEnable = FALSE;
}

/*!*************************************************************************************************
\fn uint32_t IP_UsbEnetEnable(void)
\brief  Enable Usb enet Interface

\param [in]  void

\retval      void
***************************************************************************************************/
void IP_UsbEnetEnable(void)
{
    bUsbEnable = TRUE;
    (void)enableIPV6Rx;
    (void)enableIPV4Rx;
    (void)enableARPRx;
}
#endif
#endif/* USB_RNDIS_IF */

/*==================================================================================================
 Private functions
 ==================================================================================================*/
