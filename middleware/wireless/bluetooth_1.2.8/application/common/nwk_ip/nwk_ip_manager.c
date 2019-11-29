/*! *********************************************************************************
* \addtogroup IPv6 Node
* @{
********************************************************************************** */
/*!
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* rief 	This file is the source file for the Netwrok IP Manager
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/* Framework */
#include "EmbeddedTypes.h"

/* BLE */
#include "ble_abs_types.h"

/* 6Lo */
#include "sixlowpan_tbl.h"
#include "ip_if_6LoBle.h"
#include "sixlowpan_cfg.h"
#include "sixloble_interface.h"

/* ND */
#include "nd.h"
#include "nd_cfg.h"
#include "nd_sixlowpan.h"

/* IP */ 
#include "ip.h"
#include "ip6.h"
#include "ip_cfg.h"
#include "ip_if_management.h"

/* ICMP */
#include "icmp.h"
#include "icmp6.h"

/* UDP */
#if UDP_ECHO_PROTOCOL
#include "app_echo_udp.h"
#endif

/* Sockets */
#if SOCK_DEMO
#include "app_sockets.h"
#endif

#include "app_coap.h"

#include "network_utils.h"
#include "nwk_ip_manager.h"
#include "nwk_ip_config.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

nwkInstance_t mNwkStackInstance;

ndPib_t aNdPib = {ND_PIB};

ipAddr_t ipGlobalPrefix = {GLOBAL_PREFIX_INIT};
uint8_t  ipGlobalPrefixLen = GLOBAL_PREFIX_INIT_LEN;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static ipAddr_t* NwkIpManager_NodeGetDefaultRoute
(
    ipPktInfo_t* pIpPktInfo,
    uint8_t direction
);

static bool_t NwkIpManager_ResolveAddr
(
    ipPktInfo_t* pIpPktInfo
);
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

nwkInstance_t* gpNwkStackInstance = NULL;
extern taskMsgQueue_t*  pNwkIpMsgQueue;
extern bleAbsRequests_t* BleApp_RegisterBleCallbacks(instanceId_t instanceId);
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
/* Dummy function required by nwk ip lib */
void EVM_EventNotify
(
    uint32_t code,
    void *pEventData,
    uint16_t dataSize,
    instanceId_t instanceId
)
{

}

nwkStatus_t NwkIpManager_Init(void)
{
    nwkStatus_t status = gNwkStatusFail_c;
    bleAbsRequests_t * pBleAbsReq = NULL;
    instanceId_t sixLoInstanceId = gInvalidInstanceId_c;
    
    /******/
    /* 6Lo */
    /******/    

    /* Initialize 6LoWPAN interface */
    sixLoInstanceId = SixLoBle_Init(gAdpHcRFC6282_c);

    if(gInvalidInstanceId_c != sixLoInstanceId)
    {
        mNwkStackInstance.slwpInstId = sixLoInstanceId;
        gpNwkStackInstance = &mNwkStackInstance;
        
        pBleAbsReq = BleApp_RegisterBleCallbacks(sixLoInstanceId);
        
        SixLoBle_RegisterBleInstance(sixLoInstanceId, pBleAbsReq);
        
        /* Update Network Stack Instance */
        gpNwkStackInstance->pBleAbsReq = pBleAbsReq;
        
        status = gNwkStatusSuccess_c;

		/* Add interface to IP */
		IP_IF_Add(gIpIfBle0_c, (void*)sixLoInstanceId,
						  (mediaIfStruct_t *)g6LoBleMediaIfPtr, gIpProtv6_c);
		mNwkStackInstance.ifHandle = IP_IF_GetIfHandle(gIpIfBle0_c);
		mNwkStackInstance.ifUniqueId = gIpIfBle0_c;

		/******/
		/* IP */
		/******/
    	/* Install on link address verification function */
#if ND_ENABLED    
    	mNwkStackInstance.ifHandle->ip6If.ip6IsAddrOnLink = ND_IsAddrOnlink;
    	mNwkStackInstance.ifHandle->ip6If.ip6ResolveUnicastAddr = NwkIpManager_ResolveAddr;
#endif    

#if UDP_ECHO_PROTOCOL    
    	ECHO_ProtocolInit(pNwkIpMsgQueue);
#endif    
    
#if SOCK_DEMO
    	APP_InitUserSockets(pNwkIpMsgQueue);
#endif    
    
    	APP_InitCoap();
    }
    return status;
}
    
nwkStatus_t NwkIpManager_StartRouter(void)
{
    nwkStatus_t status = gNwkStatusSuccess_c;
    ifHandle_t  ifHandleSlp = mNwkStackInstance.ifHandle;
    
   ifHandleSlp->ip6If.ip6UnicastForward = IP6_UnicastForward;
    
#if ND_ENABLED     

    sloBleStruct_t * pSloStruct = *(sloBleStruct_t **)mNwkStackInstance.slwpInstId;
    
    pSloStruct->deviceType = gSloDevBRouter_c;
    
    /******/
    /* ND */
    /******/
    ND_Open(mNwkStackInstance.ifUniqueId, &aNdPib);

    /* set Router lifetime */
    ND_DefaultRouterConfig(ifHandleSlp->ifUniqueId, ND_LIFETIME_INFINITE_4_BYTES, gNdRoutePrefMedFlag_c);

    /* configure global pPrefix */
    ND_PrefixListAdd(ifHandleSlp->ifUniqueId, &ipGlobalPrefix, ipGlobalPrefixLen,
        IP6_ADDRESS_LIFETIME_INFINITE, IP6_ADDRESS_LIFETIME_INFINITE, 1, 1);

    /* configure IP address based on global pPrefix */
    IP_IF_BindAddr6(ifHandleSlp->ifUniqueId, &ipGlobalPrefix,
        ip6AddrTypeAutoconfigurable_c, IP6_ADDRESS_LIFETIME_INFINITE,ipGlobalPrefixLen);

    RFC6282_ContextTableAdd(mNwkStackInstance.ifUniqueId, (ipAddr_t*)&ipGlobalPrefix,
                           ipGlobalPrefixLen, 0, 1,ND_LIFETIME_INFINITE_2_BYTES);
#endif
    
    return status;    
}

nwkStatus_t NwkIpManager_StartNode(void)
{
    nwkStatus_t status = gNwkStatusSuccess_c;
    ifHandle_t  ifHandleSlp = mNwkStackInstance.ifHandle;    
    
   ifHandleSlp->ip6If.ip6UnicastForward = NwkIpManager_NodeGetDefaultRoute;
    
#if ND_ENABLED   

    /******/
    /* ND */
    /******/
    
    sloBleStruct_t * pSloStruct = *(sloBleStruct_t **)mNwkStackInstance.slwpInstId;
    
    pSloStruct->deviceType = gSloDevBRouter_c;    

    ND_Open(ifHandleSlp->ifUniqueId, &aNdPib);

    /* set registration lifetime for the node */
    ND_RegistrationLifetimeSet(ifHandleSlp, 9000U);
#endif    
    
    return status;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

static ipAddr_t* NwkIpManager_NodeGetDefaultRoute
(
    ipPktInfo_t* pIpPktInfo,
    uint8_t direction
)
{
#if ND_ENABLED
    ndNeighborEntry_t **ppNd6NeighborEntry = NULL;
#endif

    ipAddr_t* pRetAddr = NULL;

    /* check to see if address is not link local or multicast */
    if((!IP6_IsLinkLocalAddr(pIpPktInfo->pIpDstAddr)) &&
      (!IP6_IsMulticastAddr(pIpPktInfo->pIpDstAddr)))
    {
        /* TX */
        if(gIpForwardTx_c == direction)
        {
#if ND_ENABLED          
           /* Find the Default Router Address */
            ppNd6NeighborEntry = ND_GetDefaultRouter(pIpPktInfo->ipPktOptions.ifHandle);
            if(NULL != ppNd6NeighborEntry)
            {
                pRetAddr = &(*ppNd6NeighborEntry)->ipAddr;
            }
#endif            
        }
        else
        {
            /* free the packet */
            NWKU_FreeIpPktInfo(&pIpPktInfo);
        }
    }
    else
    {
        /* RX */
        if(gIpForwardRx_c == direction)
        {
            /* free the packet */
            NWKU_FreeIpPktInfo(&pIpPktInfo);
        }
        /* TX */
        else
        {
            pRetAddr = pIpPktInfo->pIpDstAddr;
        }
    }
    return pRetAddr;
}

static bool_t NwkIpManager_ResolveAddr
(
    ipPktInfo_t* pIpPktInfo
)
{
    bool_t sendPkt = TRUE;
    ndNeighborEntry_t** ppNeighborEntry = NULL;
    ifHandle_t ifHandle = pIpPktInfo->ipPktOptions.ifHandle;

    if((!IP6_IsLinkLocalAddr(pIpPktInfo->pIpDstAddr)) &&
        (!IP6_IsMulticastAddr(pIpPktInfo->pIpDstAddr)))
    {      
        ppNeighborEntry = ND_NeighborCacheGet(ifHandle, pIpPktInfo->pIpDstAddr);
        
        if(NULL == ppNeighborEntry)
        {
            if (TRUE == IP_IF_IsMyAddr(ifHandle->ifUniqueId, pIpPktInfo->pIpSrcAddr))
            {
                /* drop the packet */
                NWKU_FreeIpPktInfo(&pIpPktInfo);
                sendPkt = FALSE;
            }
            else
            {
                sendPkt = FALSE;

                /* modify to source of the original packet */
                IP_AddrCopy(pIpPktInfo->pIpSrcAddr, &ipGlobalPrefix);
                SixLoBle_GetIIDFromBleAddr(pIpPktInfo->ipPktOptions.srcLlInfo.eui, &pIpPktInfo->pIpSrcAddr->addr8[8]);

                /* the packet was forwarded to us but we are not the final destination nor the
                destination is a child of ours -send ICMP "address unreachable" message with
                the "no route to destination"*/
                ICMP_SendError(&pIpPktInfo, gIcmp6TypeDestinationUnreachable_c,gImcp6CodeNoRoute_c, 0);
            }
        }
        else
        {
            SixLoBle_GetIIDFromBleAddr((*ppNeighborEntry)->llAddrInf.eui ,&pIpPktInfo->pIpDstAddr->addr8[8]);
        }
    }

    return sendPkt;
}

/*! *********************************************************************************
* @}
********************************************************************************** */
