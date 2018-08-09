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
\file       app_ethernet.c
\brief      This is a public source file for the Ethernet interface.

==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "app_stack_config.h"
#include "stack_config.h"

#include "EmbeddedTypes.h"
#include "stdio.h"
#include "string.h"

#include "stack_manager_if.h"

#include "ip_if_management.h"
#include "ip.h"
#include "ip4.h"
#include "ip6.h"
#include "nd.h"
#include "nd_events.h"
#include "dhcp_client.h"
#include "dhcp_server.h"
#include "dhcp6_client.h"
#include "mdns.h"
#include "mld.h"

#include "nvm_adapter.h"

#include "event_manager.h"

#include "enet_driver.h"

#include "thread_utils.h"
#if ND_OPT_DNS_RCV_ENABLED
    #include "thread_dns.h"
#endif

#include "ip_if_enet.h"

#include "app_ethernet.h"

/*==================================================================================================
Private macros
==================================================================================================*/
#if (ENET_ROUTER || ENET_HOST)
/* ND ROUTER CONFIGURATIONS */
#define ND6_ROUTER_CONFIGURATION  \
        /* Supported features */ \
        .devType                                = gNd6DevRouter_c, \
        .messages                               = ND_PIB_MSG_RA_RCV_MASK \
                                                 | ND_PIB_MSG_RS_RCV_MASK, \
        .options                                = 0U, \
        .behaviors                              = ND_PIB_SEND_PERIODIC_RA_MASK, \
        /* Variables Defaults */ \
        .variablesDefaults.hopLimit             = 255U, \
        .variablesDefaults.linkMTU              = 1280U, \
        .variablesDefaults.regLifetime          = 0U, \
        /* Constants */ \
        .constants.maxInitialRtrAdvInterval     = 16U,      /*!< sec */ \
        .constants.maxInitialRtrAdv             = 3U, \
        .constants.maxFinalRtrAdv               = 3U, \
        .constants.minDelayBetweenRas           = 3U,       /*!< sec */ \
        .constants.maxRaDelayTime               = 500U,     /*!< ms */ \
        .constants.maxRtrSolicitationDelay      = 1U,       /*!< sec */ \
        .constants.rtrSolicitationInterval      = 4U,       /*!< sec */ \
        .constants.maxRtrSolicitations          = 3U, \
        .constants.maxMulticastSolicit          = 3U, \
        .constants.maxUnicastSolicit            = 3U, \
        .constants.maxAnycastDelayTime          = 1U,       /*!< sec */ \
        .constants.maxNeighborAdvertisement     = 3U, \
        .constants.reachableTime                = 30U,      /*!< sec */ \
        .constants.retransTime                  = 1U,       /*!< sec */ \
        .constants.delayFirstProbeTime          = 5U,       /*!< sec */ \
        .constants.minRandomFactor              = 500U,     /*!< ms */ \
        .constants.maxRandomFactor              = 1500U,    /*!< ms */ \
        .constants.minContextChangeDelay        = 0U,       /*!< sec */ \
        .constants.tentativeNceLifetime         = 0U,       /*!< sec */ \
        .constants.multihopHopLimit             = 0U, \
        .constants.maxRtrSolicitationInterval   = 0U        /*!< sec */

/* ENET interface config  */
#define ETH_ROUTER_CONFIGURATION \
        .ifUniqueId                             = gIpIfEth0_c, \
        /* Ethernet / Virtual Enet */ \
        .macAddress.eui                         = {0x00, 0x60, 0x37, 0x00, 0xFA, 0x5D}, \
        .macAddress.addrSize                    = gLlayerAddrEui48_c, \
        /* ND */ \
        .pNdPib                                 = (ndPib_t*)&mEthRouterNdPibCfg, \


/* ND HOST CONFIGURATIONS */
#define ND6_HOST_CONFIGURATION  \
        /* Supported features */ \
        .devType                                = gNd6DevHost_c, \
        .messages                               = ND_PIB_MSG_RA_RCV_MASK, \
        .options                                = 0U, \
        .behaviors                              = 0U, \
        /* Variables Defaults */ \
        .variablesDefaults.hopLimit             = 255U, \
        .variablesDefaults.linkMTU              = 1280U, \
        .variablesDefaults.regLifetime          = 0U, \
        /* Constants */ \
        .constants.maxInitialRtrAdvInterval     = 16U,      /*!< sec */ \
        .constants.maxInitialRtrAdv             = 3U, \
        .constants.maxFinalRtrAdv               = 3U, \
        .constants.minDelayBetweenRas           = 3U,       /*!< sec */ \
        .constants.maxRaDelayTime               = 500U,     /*!< ms */ \
        .constants.maxRtrSolicitationDelay      = 1U,       /*!< sec */ \
        .constants.rtrSolicitationInterval      = 4U,       /*!< sec */ \
        .constants.maxRtrSolicitations          = 3U, \
        .constants.maxMulticastSolicit          = 3U, \
        .constants.maxUnicastSolicit            = 3U, \
        .constants.maxAnycastDelayTime          = 1U,       /*!< sec */ \
        .constants.maxNeighborAdvertisement     = 3U, \
        .constants.reachableTime                = 30U,      /*!< sec */ \
        .constants.retransTime                  = 1U,       /*!< sec */ \
        .constants.delayFirstProbeTime          = 5U,       /*!< sec */ \
        .constants.minRandomFactor              = 500U,     /*!< ms */ \
        .constants.maxRandomFactor              = 1500U,    /*!< ms */ \
        .constants.minContextChangeDelay        = 0U,       /*!< sec */ \
        .constants.tentativeNceLifetime         = 0U,       /*!< sec */ \
        .constants.multihopHopLimit             = 0U, \
        .constants.maxRtrSolicitationInterval   = 0U        /*!< sec */

#define ETH_HOST_CONFIGURATION \
        .ifUniqueId                             = gIpIfEth0_c, \
        /* Ethernet / Virtual Enet */ \
        .macAddress.eui                         = {0x00, 0x60, 0x37, 0xFF, 0xFF, 0xFF}, \
        .macAddress.addrSize                    = gLlayerAddrEui48_c, \
        /* ND */ \
        .pNdPib                                 = (ndPib_t*)&mStaticNdHostPibCfg, \

static uint32_t ip4Address = 0;

#if MDNS_ENABLED
static mDnsService_t mdnsService = {.hostName      = (uint8_t *)"BorderRouter THREAD",
                                    .name          = (uint8_t *)"_meshcop",
                                    .target        = (uint8_t *)"BorderRouter",
                                    .domain        = (uint8_t *)"local",
                                    .protocolType  = (uint8_t *)"_udp",
                                    .portNo        = 49191
                                   };

static char *mDnsRecordVersion               = {"rv="};                   //record_version
static char *mDnsThreadVersion               = {"tv="};                   //thread_version
static char *mDnsStateBitmap                 = {"sb="};                   //state_bitmap
static char *mDnsNwkName                     = {"nn="};                   //network_name
static char *mDnsXPanID                      = {"xp="};                   //xpanid
static char *mDnsVendorName                  = {"vn="};                   //vendor_name
static char *mDnsModelName                   = {"vm="};                   //model_name
static char *mDnsActiveTimestamp             = {"at="};                   //active_timestamp
static char *mDnsPartitionId                 = {"pt="};                   //partition_id
static char *mDnsVendorData                  = {"vd="};                   //vendor_data
static char *mDnsVendorOui                   = {"vo="};                   //vendor_oui

static char *mDnsBrIpv6Addr                  = {"br_ipv6="};
#endif
#endif
/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/
#if ENET_HOST
    static void NET_SetAddr(ipIfUniqueId_t ipIfUniqueId, ipAddr_t *pIpAddr, void *pData);
    static void NET_SetAddr4(ipIfUniqueId_t ipIfUniqueId, uint32_t *pIpAddr, void *pData);
#endif
#if (ENET_ROUTER || ENET_HOST)
    static void NET_BrAddExternaIfPrefix(uint32_t thrInstanceId);
    #if MDNS_ENABLED
        static void NET_MDNSStart(void);
    #endif
#endif
/*==================================================================================================
Private global variables declarations
==================================================================================================*/
#if ENET_ROUTER
static const ndPib_t mEthRouterNdPibCfg         = {ND6_ROUTER_CONFIGURATION};
static const externalIfConfig_t mEnetCfg        = {ETH_ROUTER_CONFIGURATION};
static bool_t mIsGlobalOnMeshPrefixConfigured   = TRUE;
#elif ENET_HOST
static const ndPib_t mStaticNdHostPibCfg        = {ND6_HOST_CONFIGURATION};
static const externalIfConfig_t mEnetCfg        = {ETH_HOST_CONFIGURATION};
static bool_t mIsGlobalOnMeshPrefixConfigured   = FALSE;
#endif

#if ND_OPT_DNS_RCV_ENABLED
    static bool_t mThrDNSServicesRegistered = FALSE;
#endif

#if (ENET_ROUTER || ENET_HOST)
static uint32_t mThrInstanceId = 0;
static ifHandle_t mIfHandleEnet = NULL;
#if ENET_HOST
    static dhcp6ClientStartParams_t mDhcp6ClientStartParams;
#endif
/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern const mediaIfStruct_t *gEnetMediaIfPtr;
taskMsgQueue_t *gpEthTaskMsgQueue = NULL;
/*==================================================================================================
Public functions
==================================================================================================*/

/*!*************************************************************************************************
\fn     void IFENET_Start()
\brief  This is a public function used to start the Ethernet IP Media Interface.
\return void
***************************************************************************************************/
void IFENET_Start
(
    taskMsgQueue_t *pTaskMsgQueue,
    instanceId_t thrInstanceID
)
{
    if (NULL == gpEthTaskMsgQueue)
    {
        uint16_t protocols = gIpProtv6_c;
#if IP_IP4_ENABLE
        protocols |= gIpProtv4_c;
#endif
        void *enetIfPtr = NULL;
        uint32_t error;
        gpEthTaskMsgQueue = pTaskMsgQueue;

        /* Initialize Ethernet interface */
        ENET_Initialize((uint8_t *)mEnetCfg.macAddress.eui, &enetIfPtr, pTaskMsgQueue);
        error = IP_IF_Add(gIpIfEth0_c, enetIfPtr, (mediaIfStruct_t *)gEnetMediaIfPtr, protocols);
        mIfHandleEnet = IP_IF_GetIfHandle(gIpIfEth0_c);

        if (mEnetCfg.pNdPib && (gIpOk_c == error) && (NULL != mIfHandleEnet))
        {
            /* assign routing function to interface */
            mIfHandleEnet->ip6If.ip6UnicastForward = IP6_Forward;
            mIfHandleEnet->ip6If.ip6McastForward = IP6_MulticastForward;

#if MLD_ENABLED
            MLD_IF_Enable(gIpIfEth0_c);
#endif

            /* start ND */
            ND_Open(mIfHandleEnet, mEnetCfg.pNdPib);

            /* Save thread instance */
            mThrInstanceId = thrInstanceID;

#if ENET_ROUTER

            if (mEnetCfg.pNdPib->devType == gNd6DevRouter_c)
            {
                thrPrefixAttr_t *pExternalIfPrefix = &THR_GetAttr_BrExternalIfPrefix(thrInstanceID);
                thrPrefixAttr_t globalOnMeshPrefix = THR_GetAttr_BrGlobalOnMeshPrefix(thrInstanceID);

                /* set Router Lifetime to 0 seconds - the border router is not a default router */
                ND_DefaultRouterConfig(mIfHandleEnet, ND_ROUTER_LIFETIME_ZERO, gNdRoutePrefMedFlag_c);

                /* configure external global pPrefix */
                ND_PrefixListAdd(mIfHandleEnet, &pExternalIfPrefix->prefix,
                                 pExternalIfPrefix->prefixLenBits,
                                 IP6_ADDRESS_LIFETIME_INFINITE, IP6_ADDRESS_LIFETIME_INFINITE, 1, 1);

                /* set on-mesh route to advertise */
                ND_RouteInfoSet(globalOnMeshPrefix.prefix, globalOnMeshPrefix.prefixLenBits,
                                gNdRoutePrefMedFlag_c, ND_LIFETIME_INFINITE);

                /* configure IP address based on global pPrefix */
                IP_IF_BindAddr6(gIpIfEth0_c, &pExternalIfPrefix->prefix,
                                ip6AddrTypeAutoconfigurable_c, IP6_ADDRESS_LIFETIME_INFINITE,
                                pExternalIfPrefix->prefixLenBits);

                /* set static route for the enet prefix */
                //IP6_SetStaticRoute(&pExternalIfPrefix->prefix, pExternalIfPrefix->prefixLenBits,
                //             IP6_STATIC_ROUTE_DEFAULT_METRIC,(ipAddr_t*)&in6addr_any,gIpIfEth0_c);
            }

#if DHCP4_SERVER_ENABLED && IP_IP4_ENABLE
            {
                uint32_t serverIpAddr = IPV4_ADDR(192, 168, 1, 1);
                uint32_t startIpAddr = IPV4_ADDR(192, 168, 1, 2);
                uint32_t endIpAddr = IPV4_ADDR(192, 168, 1, 10);
                DHCP4_Server_Init(pTaskMsgQueue);
                DHCP4_Server_Start(serverIpAddr, startIpAddr, endIpAddr, gIpIfEth0_c);
            }
#endif /* DHCP v4 SERVER */
#endif

#if ENET_HOST

            if (mEnetCfg.pNdPib->devType == gNd6DevHost_c)
            {
                /****************************************/
                /* Start DHCP Client for Global Address */
                /****************************************/
                DHCP6_Client_Init(gpEthTaskMsgQueue);

                mDhcp6ClientStartParams.clientStartMode = gDhcp6Pd_c;
                mDhcp6ClientStartParams.ipIfId = mEnetCfg.ifUniqueId;
                mDhcp6ClientStartParams.deviceType = DHCP6_HW_TYPE_EUI64;
                mDhcp6ClientStartParams.relayAddr = NULL;
                mDhcp6ClientStartParams.pPrefix = NULL;
                DHCP6_Client_Start(&mDhcp6ClientStartParams);
                DHCP6_Client_RegisterSetAddr(NET_SetAddr);

                DHCP4_Client_Init(gpEthTaskMsgQueue);
                DHCP4_Client_Start(gIpIfEth0_c, NULL);
                DHCP4_Client_RegisterSetAddr(NET_SetAddr4);
            }

#endif
        }

        //MDNS_Init(gpEthTaskMsgQueue);
        //MDNS_JoinIf(mIfHandleEnet,AF_INET6);

    }
}

/*!*************************************************************************************************
\fn     void IFENET_ThreadStarted(uint32_t thrInstanceId)
\brief  This is a public function which handles the steps that should be done after a device with
        Ethernet functionality transitions to Thread Leader.

\param  [in] thrInstanceId  instance ID of the Thread interface

\return void
***************************************************************************************************/
void IFENET_ThreadStarted
(
    uint32_t thrInstanceId
)
{
    if ((mIsGlobalOnMeshPrefixConfigured == TRUE) &&
         (THR_GetAttr_IsDevConnected(thrInstanceId)))
    {
        thrOtaBrPrefixSet_t prefixSet;
        thrPrefixAttr_t *pOnMeshPrefix = &THR_GetAttr_BrGlobalOnMeshPrefix(thrInstanceId);
        bool_t  brPrefixSync = FALSE;

        /* Uncomment for multicast address */
        //ipAddr_t mCastGroup = MCAST_3EAD_ADDRESS;

        /* Add the global on-mesh prefix */
        if (pOnMeshPrefix->prefixLenBits)
        {
            FLib_MemSet(&prefixSet, 0, sizeof(prefixSet));
            prefixSet.thrBrPrefixLength = pOnMeshPrefix->prefixLenBits;
            FLib_MemCpy(prefixSet.thrBrPrefixValue,
                        pOnMeshPrefix->prefix.addr8,
                        pOnMeshPrefix->prefixLenBits >> 3);
            prefixSet.thrBrPrefixFlags[1] = THR_BR_PREFIX_FLAGS_P_SLAAC_MASK;
            prefixSet.thrBrPrefixFlags[1] |= THR_BR_PREFIX_FLAGS_P_ON_MESH_MASK;

            if (THR_GetAttr_BrDefaultRoute(thrInstanceId))
            {
                prefixSet.thrBrPrefixFlags[1] |= THR_BR_PREFIX_FLAGS_P_DEFAULT_MASK;
            }

            htonal(prefixSet.thrBrPrefixLifetime, THR_ALL_FFs32);
            htonal(prefixSet.thrBrExtRouteLifetime, THR_ALL_FFs32);
            prefixSet.thrBrPrefixAdvertised = TRUE;
            prefixSet.thrBrExtRouteAdvertised = FALSE;
            THR_BrPrefixAttrAddEntry(thrInstanceId, &prefixSet);
            brPrefixSync = TRUE;
        }

#if ENET_ROUTER
        /* For ND_ROUTER, the external prefix is already known */
        {
            thrPrefixAttr_t *pExternalIfPrefix = &THR_GetAttr_BrExternalIfPrefix(thrInstanceId);

            if (pExternalIfPrefix->prefixLenBits)
            {
                /* Add External Route prefix */
                NET_BrAddExternaIfPrefix(thrInstanceId);
                brPrefixSync = TRUE;
            }
        }
#endif

        /* Distribute Network Data */
        if (brPrefixSync == TRUE)
        {
            THR_BrPrefixAttrSync(thrInstanceId);
        }

        /* Uncomment to add forwarding rule */
        //IP6_SetStaticRoute(&mCastGroup, 128, IP6_STATIC_ROUTE_DEFAULT_METRIC, (ipAddr_t *)&in6addr_any, gIpIfSlp0_c);
    }

#if MDNS_ENABLED

    if (THR_GetAttr_IsDevConnected(thrInstanceId))
    {
        if (ip4Address != 0)
        {
            NET_MDNSStart();
        }
    }

#endif
}

/*!*************************************************************************************************
\fn    void NET_AgentSet()
\brief  This function is used to set the Boarder Agent State Bitmap.

\param  [in] var      pointer to the bitmap
\param  [in] val      value to be set in bitmap
\param  [in] opType   type of value to be set used to be mapped correctly in bitmap

\return void
***************************************************************************************************/
void NET_AgentSet(uint32_t *var, uint32_t val, uint8_t opType)
{
    if (var != NULL)
    {
        switch (opType)
        {
            case CONNECTION_MODE_TYPE:
                NET_AgentSet_ConnectionMode(*var, val)
                break;

            case THREAD_INTERFACE_STATUS_TYPE:
                NET_AgentSet_ThreadInterfaceStatus(*var, val)
                break;

            case AVAILABILITY_TYPE:
                NET_AgentSet_Availability(*var, val)
                break;

            default:
                break;
        }
    }
}
/*!*************************************************************************************************
\fn     NET_RaReceived
\brief  This is a private function which is called when the router allocates a prefix for us(in an
        ND Router Advertisement packet)

\param  [in] pEvent     pointer to the event structure

\return void
***************************************************************************************************/
void NET_RaReceived
(
    void *pEvent
)
{
    evmParams_t *pEvmParam = pEvent;

#if ENET_HOST
    thrPrefixAttr_t *pExternalIfPrefix = &THR_GetAttr_BrExternalIfPrefix(mThrInstanceId);
    ipPrefix_t *pPrefix = (ipPrefix_t *)pEvmParam->pBuff;

    if ((pEvmParam) && (pEvmParam->pBuff) && (mIfHandleEnet) &&
         (!NWKU_CmpAddrPrefix6(pPrefix->aPrefix, pExternalIfPrefix->prefix.addr8,pPrefix->prefixLen)))
    {
        thrPrefixAttr_t externalPrefix;
        ndCfg_t **ppNdCfg = NULL;

        FLib_MemSet(&externalPrefix, 0, sizeof(externalPrefix));
        /* Store the external IF prefix*/
        externalPrefix.prefixLenBits = pPrefix->prefixLen;
        FLib_MemCpy(&externalPrefix.prefix.addr8[0],
                    pPrefix->aPrefix,
                    pPrefix->prefixLen >> 3);
        THR_SetAttr(mThrInstanceId, gNwkAttrId_BrExternalIfPrefix_c, 0,
                    sizeof(thrPrefixAttr_t), &externalPrefix);

        /* Propagate the external prefix */
        NET_BrAddExternaIfPrefix(mThrInstanceId);
        THR_BrPrefixAttrSync(mThrInstanceId);

        /* If an RA w/ the M flag set was received, solicit an address by DHCPv6-PD */
        ppNdCfg = ND_GetCfg(mIfHandleEnet);

        if (ppNdCfg && *ppNdCfg && (*ppNdCfg)->bManagedConfig)
        {
            DHCP6_Client_SolicitAddress(&mDhcp6ClientStartParams);
        }
    }

#endif /* ENET_HOST */

    if (pEvmParam)
    {
        if (pEvmParam->pBuff)
        {
            MEM_BufferFree(pEvmParam->pBuff);
        }

        MEM_BufferFree(pEvmParam);
    }
}


/*!*************************************************************************************************
\fn     NET_RaRouteInfoReceived
\brief  This is a private function which is called when a Route Information option is present in an
        ND Router Advertisement packet.

\return void
***************************************************************************************************/
void NET_RaRouteInfoReceived
(
    void *pEvent
)
{
    evmParams_t *pEvmParam = pEvent;
    ndOptionRouteInfo_t *pNdRouteInfoOption = (ndOptionRouteInfo_t *)(pEvmParam->pBuff);
    ndOptionRouteInfo_t *pCurrOption = NULL;
    uint8_t iCount, noOptions = pEvmParam->buffSize / sizeof(ndOptionRouteInfo_t);
    thrOtaBrPrefixSet_t prefixSet = { 0 };

    for (iCount = 0; iCount < noOptions; iCount++)
    {
        pCurrOption = pNdRouteInfoOption + iCount;

        prefixSet.thrBrPrefixLength = pCurrOption->prefixLength;
        FLib_MemCpy(prefixSet.thrBrPrefixValue, pCurrOption->prefix.addr8, pCurrOption->prefixLength >> 3);

        /* If the received route's lifetime is zero, the route is removed from the Routing Table */
        if (ntohal(pCurrOption->lifetime) == ND_LIFETIME_ZERO)
        {
            THR_BrPrefixAttrRemoveEntry(mThrInstanceId, prefixSet.thrBrPrefixLength, prefixSet.thrBrPrefixValue);
        }
        else
        {
            FLib_MemCpy(prefixSet.thrBrExtRouteLifetime, pCurrOption->lifetime, sizeof(pCurrOption->lifetime));
            THR_BR_FLAGS_SET(prefixSet.thrBrExtRouteFlags, pCurrOption->prf >> 3,
                             THR_BR_EXT_ROUTE_FLAGS_R_PREF_MASK, THR_BR_EXT_ROUTE_FLAGS_R_PREF_OFFSET);
            prefixSet.thrBrPrefixAdvertised = FALSE;
            prefixSet.thrBrExtRouteAdvertised = TRUE;

            THR_BrPrefixAttrAddEntry(mThrInstanceId, &prefixSet);
        }

        FLib_MemSet(&prefixSet, 0, sizeof(thrOtaBrPrefixSet_t));
    }

    THR_BrPrefixAttrSync(mThrInstanceId);

    MEM_BufferFree(pEvmParam->pBuff);
    MEM_BufferFree(pEvmParam);
}


#if ND_OPT_DNS_RCV_ENABLED
/*!*************************************************************************************************
\fn     NET_RaDnsInfoReceived
\brief  This is a private function which is called when a RDNSS or DNSSL option is present in an
        ND Router Advertisement packet.

\return void
***************************************************************************************************/
void NET_RaDnsInfoReceived
(
    void *pEvent
)
{
    evmParams_t *pEvmParam = pEvent;
    uint8_t flag = *pEvmParam->pBuff;

    uint8_t iCount;
    bool_t attrSync = FALSE;

    if (!mThrDNSServicesRegistered)
    {
        mThrDNSServicesRegistered = TRUE;
        THR_DNS_BRInit();
    }

    extern const uint8_t gThrServerDataBrSetTblSize;
    extern borderRouterSet_t gaThrServerDataBrSetTbl[];

    /* Set Border Router flag P_nd_dns for all prefixes. */
    for (iCount = 0; iCount < gThrServerDataBrSetTblSize; iCount++)
    {
        borderRouterSet_t *pBrSet = &gaThrServerDataBrSetTbl[iCount];

        if (THR_ALL_FFs8 != pBrSet->brPrefixIndex)
        {
            if (flag != THR_BR_FLAGS_GET(pBrSet->brPrefixFlags[1],
                                         THR_BR_PREFIX_FLAGS_P_ND_DNS_MASK,
                                         THR_BR_PREFIX_FLAGS_P_ND_DNS_OFFSET))
            {
                THR_BR_FLAGS_SET(pBrSet->brPrefixFlags[1], flag,
                                 THR_BR_PREFIX_FLAGS_P_ND_DNS_MASK, THR_BR_PREFIX_FLAGS_P_ND_DNS_OFFSET);
                pBrSet->bAdvertised = FALSE;
                attrSync = TRUE;
            }
        }
    }

    if (attrSync)
    {
        THR_BrPrefixAttrSync(mThrInstanceId);
    }

    MEM_BufferFree(pEvmParam);
}
#endif /* ND_OPT_DNS_RCV_ENABLED */

/*==================================================================================================
Private functions
==================================================================================================*/
#if ENET_HOST
/*!*************************************************************************************************
\private
\fn     NET_SetAddr
\brief  This is a private function Which is called when the router allocates a prefix for us(as
        response to DHCP Prefix Delegation request)

\param  [in] pIfHandle  pointer to the interface handle
\param  [in] pIpAddr    pointer to the IP prefix assigned by the router(:: in case of none)
\param  [in] pData      pointer to a variable containing the length of the prefix(NULL in case of no
                        prefix)

\return void
***************************************************************************************************/
static void NET_SetAddr
(
    ipIfUniqueId_t ipIfUniqueId,
    ipAddr_t *pIpAddr,
    void *pData
)
{
    /* Router could assign a prefix */
    if (pData && (!IP_IsAddrEqual(pIpAddr, &in6addr_any)))
    {
        uint8_t prefixLen = *(uint8_t *)pData;
        thrPrefixAttr_t globalOnMeshPrefix;

        /* If we received a prefix length smaller than 64 we expand it to 64 for Thread assignment
         * as we don't need to create any new sub-networks inside Thread */
        if (prefixLen < 64)
        {
            prefixLen = 64;
        }

        FLib_MemSet(&globalOnMeshPrefix, 0, sizeof(globalOnMeshPrefix));
        globalOnMeshPrefix.prefixLenBits = prefixLen;
        FLib_MemCpy(&globalOnMeshPrefix.prefix.addr8[0],
                    pIpAddr->addr8,
                    prefixLen >> 3);
        THR_SetAttr(mThrInstanceId, gNwkAttrId_BrGlobalOnMeshPrefix_c, 0,
                    sizeof(thrPrefixAttr_t), &globalOnMeshPrefix);

        mIsGlobalOnMeshPrefixConfigured = TRUE;
        IFENET_ThreadStarted(mThrInstanceId);
    }
}

/*!*************************************************************************************************
\private
\fn     NET_SetAddr
\brief  This is a private function Which is called when the router allocates a prefix for us(as
        response to DHCP Prefix Delegation request)

\param  [in] pIfHandle  pointer to the interface handle
\param  [in] pIpAddr    pointer to the IP prefix assigned by the router(:: in case of none)
\param  [in] pData      pointer to a variable containing the length of the prefix(NULL in case of no
                        prefix)

\return void
***************************************************************************************************/
static void NET_SetAddr4
(
    ipIfUniqueId_t ipIfUniqueId,
    uint32_t *pIpAddr,
    void *pData
)
{
    ip4Address = *pIpAddr;
#if MDNS_ENABLED

    if (THR_GetAttr_IsDevConnected(mThrInstanceId))
    {
        NET_MDNSStart();
    }

#endif
}
#endif /* ENET_HOST */

#if MDNS_ENABLED
static void NET_MDNSStart(void)
{
    char ipAddrString[INET6_ADDRSTRLEN] = {0};
    //char xPanIDString[16] = {0};
    ip6IfAddrData_t *pIpAddrData = NULL;
    ipAddr_t mdnsMcast;
    uint32_t mMdnsIp4Addr = 0xE00000FB;
    STATE_BITMAP brAgentStateBitmap = 0;

    pIpAddrData = IP_IF_GetAddrByIf6(mIfHandleEnet->ifUniqueId, 0, TRUE);
    ntop(AF_INET6, &pIpAddrData->ip6Addr, ipAddrString, INET6_ADDRSTRLEN);

    //TBD Connection Mode should depend on Security Policy TLV bits
    NET_AgentSet(&brAgentStateBitmap, NetAgent_DtlsUserAllowed_c, CONNECTION_MODE_TYPE);

    if ((FALSE == THR_GetAttr_IsDevConnected(mThrInstanceId)) &&
         (THR_GetAttr_DeviceRole(mThrInstanceId) == gThrDevRole_Disconnected))
    {
        NET_AgentSet(&brAgentStateBitmap, NetAgent_ThrNotActive_c, THREAD_INTERFACE_STATUS_TYPE);
    }
    else if ((FALSE == THR_GetAttr_IsDevConnected(mThrInstanceId)) &&
             (THR_GetAttr_DeviceRole(mThrInstanceId) != gThrDevRole_Disconnected))
    {
        NET_AgentSet(&brAgentStateBitmap, NetAgent_ThrInitializedNotActive_c, THREAD_INTERFACE_STATUS_TYPE);
    }
    else if (TRUE == THR_GetAttr_IsDevConnected(mThrInstanceId))
    {
        NET_AgentSet(&brAgentStateBitmap, NetAgent_ThrInitializedActive_c, THREAD_INTERFACE_STATUS_TYPE);
    }

    NET_AgentSet(&brAgentStateBitmap, NetAgent_HighAvailability_c, AVAILABILITY_TYPE);

    if (ntohall(gpaThrActiveAttr[0]->xPanId) != THR_ALL_FFs64)
    {
        /* Record Version */
        FLib_MemCpy(mdnsService.txtInfo[0].optionValue, mDnsRecordVersion, strlen(mDnsRecordVersion));
        FLib_MemCpy(mdnsService.txtInfo[0].optionValue + strlen(mDnsRecordVersion), "1", strlen("1"));
        mdnsService.txtInfo[0].length = strlen(mDnsRecordVersion) + strlen("1");

        /* Thread Version */
        FLib_MemCpy(mdnsService.txtInfo[1].optionValue, mDnsThreadVersion, strlen(mDnsThreadVersion));
        FLib_MemCpy(mdnsService.txtInfo[1].optionValue + strlen(mDnsThreadVersion), "1.1.0", strlen("1.1.0"));
        mdnsService.txtInfo[1].length = strlen(mDnsThreadVersion) + strlen("1.1.0");

        /* State bitmap */
        FLib_MemCpy(mdnsService.txtInfo[2].optionValue, mDnsStateBitmap, strlen(mDnsStateBitmap));
        FLib_MemCpy(mdnsService.txtInfo[2].optionValue + strlen(mDnsStateBitmap), (void *)brAgentStateBitmap,
                    sizeof(brAgentStateBitmap));
        mdnsService.txtInfo[2].length = strlen(mDnsStateBitmap) + sizeof(brAgentStateBitmap);

        /* Network Name */
        if (NET_AgentCheck_ConnectionMode(brAgentStateBitmap) != NetAgent_DtlsNotAllowed_c)
        {
            FLib_MemCpy(mdnsService.txtInfo[3].optionValue, mDnsNwkName, strlen(mDnsNwkName));

            if ((NET_AgentCheck_ConnectionMode(brAgentStateBitmap) == NetAgent_DtlsUserAllowed_c) &&
                (NET_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrInitializedActive_c))
            {
                FLib_MemCpy(mdnsService.txtInfo[3].optionValue + strlen(mDnsNwkName), gpaThrActiveAttr[mThrInstanceId]->nwkName.aStr,
                            gpaThrActiveAttr[mThrInstanceId]->nwkName.length);
                mdnsService.txtInfo[3].length = strlen(mDnsNwkName) + gpaThrActiveAttr[mThrInstanceId]->nwkName.length;
            }
            else if (NET_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrNotActive_c)
            {
                /* if Thread Interface is not initialized, the string should reflect the Border Agent product or model name */
                FLib_MemCpy(mdnsService.txtInfo[3].optionValue + strlen(mDnsModelName), gpaThrStringAttr[mThrInstanceId]->modelName.aStr,
                            gpaThrStringAttr[mThrInstanceId]->modelName.length);
                mdnsService.txtInfo[3].length = strlen(mDnsModelName) + gpaThrStringAttr[mThrInstanceId]->modelName.length;
            }
        }

        /* Extended PAN ID */
        if (NET_AgentCheck_ConnectionMode(brAgentStateBitmap) != NetAgent_DtlsNotAllowed_c)
        {
            FLib_MemCpy(mdnsService.txtInfo[4].optionValue, mDnsXPanID, strlen(mDnsXPanID));

            if ((NET_AgentCheck_ConnectionMode(brAgentStateBitmap) == NetAgent_DtlsUserAllowed_c) &&
                (NET_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrInitializedActive_c))
            {
                NWKU_HexToAscii(gpaThrActiveAttr[mThrInstanceId]->xPanId, 8, mdnsService.txtInfo[4].optionValue + strlen(mDnsXPanID),
                                16, TRUE);
                mdnsService.txtInfo[4].length = strlen(mDnsXPanID) + 16;
            }
            else if (NET_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrNotActive_c)
            {
                /* field should have the SHA-256 hash value of the factory IEEE EUI-64
                 * of the Thread Interface if Thread Interface Status is not active */
                NWKU_TransformUint64ToArray(mdnsService.txtInfo[4].optionValue + strlen(mDnsXPanID), 
                                            THR_GetAttr_ShaHashAddr(mThrInstanceId));
                mdnsService.txtInfo[4].length = strlen(mDnsXPanID) + 8;
            }
        }

        /* Vendor Name */
        FLib_MemCpy(mdnsService.txtInfo[5].optionValue, mDnsVendorName, strlen(mDnsVendorName));
        FLib_MemCpy(mdnsService.txtInfo[5].optionValue  + strlen(mDnsVendorName), gpaThrStringAttr[mThrInstanceId]->vendorName.aStr,
                    gpaThrStringAttr[mThrInstanceId]->vendorName.length);
        mdnsService.txtInfo[5].length = strlen(mDnsVendorName) + gpaThrStringAttr[mThrInstanceId]->vendorName.length;

        /* Model Name */
        FLib_MemCpy(mdnsService.txtInfo[6].optionValue, mDnsModelName, strlen(mDnsModelName));
        FLib_MemCpy(mdnsService.txtInfo[6].optionValue + strlen(mDnsModelName), gpaThrStringAttr[mThrInstanceId]->modelName.aStr,
                    gpaThrStringAttr[mThrInstanceId]->modelName.length);
        mdnsService.txtInfo[6].length = strlen(mDnsModelName) + gpaThrStringAttr[mThrInstanceId]->modelName.length;

        /* Active Timestamp */
        if (NET_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrInitializedActive_c)
        {
            FLib_MemCpy(mdnsService.txtInfo[7].optionValue, mDnsActiveTimestamp, strlen(mDnsActiveTimestamp));
            FLib_MemCpy(mdnsService.txtInfo[7].optionValue + strlen(mDnsActiveTimestamp), &gpaThrPendingAttr[mThrInstanceId]->activeTimestamp,
                        sizeof(gpaThrPendingAttr[mThrInstanceId]->activeTimestamp));
            mdnsService.txtInfo[7].length = strlen(mDnsActiveTimestamp) + sizeof(gpaThrPendingAttr[mThrInstanceId]->activeTimestamp);
        }

        /* Partition ID */
        if (NET_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrInitializedActive_c)
        {
            FLib_MemCpy(mdnsService.txtInfo[8].optionValue, mDnsPartitionId, strlen(mDnsPartitionId));
            NWKU_TransformUint32ToArray(mdnsService.txtInfo[8].optionValue + (uint8_t)strlen(mDnsPartitionId), 
                                        THR_GetAttr_PartionId(mThrInstanceId));
            mdnsService.txtInfo[8].length = strlen(mDnsPartitionId) + sizeof(THR_GetAttr_PartionId(mThrInstanceId));
        }

        /* Vendor Data */
        FLib_MemCpy(mdnsService.txtInfo[9].optionValue, mDnsVendorData, strlen(mDnsVendorData));
        FLib_MemCpy(mdnsService.txtInfo[9].optionValue  + strlen(mDnsVendorData), gpaThrStringAttr[mThrInstanceId]->vendorData.aStr,
                    gpaThrStringAttr[mThrInstanceId]->vendorData.length);
        mdnsService.txtInfo[9].length = strlen(mDnsVendorData) + sizeof(gpaThrStringAttr[mThrInstanceId]->vendorData.length);

        /* Vendor OUI */
        FLib_MemCpy(mdnsService.txtInfo[10].optionValue, mDnsVendorOui, strlen(mDnsVendorOui));
        FLib_MemCpy(mdnsService.txtInfo[10].optionValue  + strlen(mDnsVendorOui), (uint8_t *) mEnetCfg.macAddress.eui, 3 * sizeof(uint8_t));
        mdnsService.txtInfo[10].length = strlen(mDnsVendorOui) + 3 * sizeof(uint8_t);

        /* Border Router IPv6 Address */
        FLib_MemCpy(mdnsService.txtInfo[11].optionValue, mDnsBrIpv6Addr, strlen(mDnsBrIpv6Addr));
        FLib_MemCpy(mdnsService.txtInfo[11].optionValue + strlen(mDnsBrIpv6Addr), ipAddrString, strlen(ipAddrString));
        mdnsService.txtInfo[11].length = strlen(mDnsBrIpv6Addr) + strlen(ipAddrString);

        NWKU_ConvertIp4Addr(mMdnsIp4Addr, &mdnsMcast);
        IP_IF_Join(gIpIfEth0_c, &mdnsMcast);

        /* Start mDNS */
        MDNS_Init(gpEthTaskMsgQueue);
        MDNS_JoinIf(mIfHandleEnet, AF_INET);

        mdnsService.ip4Addr = ip4Address;

        FLib_MemCpy(&mdnsService.address, &pIpAddrData->ip6Addr, sizeof(ipAddr_t));
        MDNS_ProbeNewService(&mdnsService);
    }
}

static void NET_BrAddExternaIfPrefix
(
    uint32_t thrInstanceId
)
{
    thrOtaBrPrefixSet_t prefixSet;
    thrPrefixAttr_t *pExternalIfPrefix = &THR_GetAttr_BrExternalIfPrefix(thrInstanceId);

    if (pExternalIfPrefix->prefixLenBits)
    {
        /* Add External Route prefix */
        FLib_MemSet(&prefixSet, 0, sizeof(prefixSet));

        prefixSet.thrBrPrefixLength = pExternalIfPrefix->prefixLenBits;
        FLib_MemCpy(prefixSet.thrBrPrefixValue, pExternalIfPrefix->prefix.addr8, pExternalIfPrefix->prefixLenBits >> 3);
        htonal(prefixSet.thrBrPrefixLifetime, THR_ALL_FFs32);
        htonal(prefixSet.thrBrExtRouteLifetime, THR_ALL_FFs32);
        prefixSet.thrBrExtRouteAdvertised = TRUE;

        /* Advertise prefix in Border Router TLV as not-on-mesh, BR flags are zeroed */
        prefixSet.thrBrPrefixAdvertised = TRUE;

        THR_BrPrefixAttrAddEntry(thrInstanceId, &prefixSet);
    }
}
#endif
#endif /*(ENET_ROUTER || ENET_HOST)*/
