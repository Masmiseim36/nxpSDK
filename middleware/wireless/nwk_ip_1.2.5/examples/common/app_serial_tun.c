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
\file       app_serial_tun.c
\brief      This is a public source file for the Serial Tunnel interface.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "app_stack_config.h"
/* Ip */
#include "ip_if_management.h"
#include "ip.h"
#include "ip6.h"
#include "ip_if_serial_tun.h"
#include "mdns.h"
#include "mld.h"
/* ND */
#include "nd.h"
#include "nd_events.h"
#include "nd_tables.h"
/* utils*/
#include "nvm_adapter.h"
#include "serial_tun_driver.h"
#include "thread_utils.h"
#include "app_serial_tun.h"
#include "thread_meshcop.h"
/* DHCPv6 */
#include "dhcp6_client.h"

#if THR_SERIAL_TUN_ROUTER
/*==================================================================================================
Private macros
==================================================================================================*/
/* ND ROUTER CONFIGURATION */
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

/* ND HOST CONFIGURATION */
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

/* Serialtun interface config  */
#define SERIAL_TUN_ROUTER_CONFIGURATION \
        .ifUniqueId                             = gIpIfSerialTun_c, \
        /* Serialtun */ \
        .macAddress.eui                         = {0x00, 0x60, 0x37, 0x00, 0xFA, 0x5C}, \
        .macAddress.addrSize                    = gLlayerAddrEui48_c, \
        /* ND */ \
        .pNdPib                                 = (ndPib_t*)&mSerialtunRouterNdPibCfg, \

/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/
static void Serialtun_ConfigNwkData(uint32_t thrInstanceId);

#if (!THR_SERIAL_TUN_ENABLE_ND_HOST && !THR_SERIAL_TUN_ENABLE_ND_ROUTER) || !THR_SERIAL_TUN_USE_DHCP6_ADDR
    static void SerialTun_StaticConfig(instanceId_t thrInstanceID);
#endif

#if THR_SERIAL_TUN_ENABLE_ND_HOST
    static void SerialTun_LinkChanged(uint8_t linkState);

    #if THR_SERIAL_TUN_USE_DHCP6_ADDR
        static void SerialTun_SetAddr(ipIfUniqueId_t ipIfUniqueId, ipAddr_t *pIpAddr, void *pData);
    #endif
#endif

#if THR_SERIAL_TUN_ENABLE_ND_HOST || THR_SERIAL_TUN_ENABLE_ND_ROUTER
    #if MDNS_ENABLED
        static void SerialTun_MDNSStart(void);
    #endif
#endif

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
#if THR_SERIAL_TUN_ENABLE_ND_HOST || THR_SERIAL_TUN_ENABLE_ND_ROUTER
static CONST ndPib_t mSerialtunRouterNdPibCfg = {ND6_ROUTER_CONFIGURATION};
#else
static CONST ndPib_t mSerialtunRouterNdPibCfg = {ND6_HOST_CONFIGURATION};
#endif

static const externalIfConfig_t mSerialtunCfg = {SERIAL_TUN_ROUTER_CONFIGURATION};
static ifHandle_t mIfHandleSerialtun = NULL;

#if THR_SERIAL_TUN_ENABLE_ND_HOST || THR_SERIAL_TUN_ENABLE_ND_ROUTER
    static uint32_t mThrInstanceId = 0;
#endif

#if THR_SERIAL_TUN_ENABLE_ND_HOST
    #if THR_SERIAL_TUN_USE_DHCP6_ADDR
        static dhcp6ClientStartParams_t mDhcp6ClientStartParams;
    #endif
#endif

#if THR_SERIAL_TUN_ENABLE_ND_HOST || THR_SERIAL_TUN_ENABLE_ND_ROUTER
    #if MDNS_ENABLED
        static mDnsService_t mdnsService = {.hostName      = (uint8_t *)"BorderRouter THREAD",
                                            .name          = (uint8_t *)"_meshcop",
                                            .target        = (uint8_t *)"BorderRouter",
                                            .domain        = (uint8_t *)"local",
                                            .protocolType  = (uint8_t *)"_udp",
                                            .fullName      = (uint8_t *)"_meshcop._udp.local",
                                            .ip4Addr       = 0,
                                            .portNo        = 49191
                                           };

        static char *mDnsRecordVersion               = {"rv="};                   //record_version
        static char *mDnsThreadVersion               = {"tv="};                   //thread_version
        static char *mDnsStateBitmap                 = {"sb="};                   //state_bitmap
        static char *mDnsNwkName                     = {"nn="};                   //network_name
        static char *mDnsXPanID                      = {"xp="};                   //xpanid
        static char *mDnsVendorName                  = {"vn="};                   //vendor_name
        static char *mDnsModelName                   = {"mn="};                   //model_name
        static char *mDnsActiveTimestamp             = {"at="};                   //active_timestamp
        static char *mDnsPartitionId                 = {"pt="};                   //partition_id
        static char *mDnsVendorData                  = {"vd="};                   //vendor_data
        static char *mDnsVendorOui                   = {"vo="};                   //vendor_oui
    #endif /* MDNS_ENABLED */
#endif

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
taskMsgQueue_t *gpSerialTunTaskMsgQueue = NULL;

#if THR_SERIAL_TUN_ENABLE_ND_HOST || THR_SERIAL_TUN_ENABLE_ND_ROUTER
    #if MDNS_ENABLED
        extern const ipAddr_t mdnsIp6Addr;
    #endif
#endif

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn     void Serialtun_Start(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID)
\brief  This is a public function used to start the Serialtun interface.

\param  [in] pTaskMsgQueue      pointer to the message queue of the application task
\param  [in] thrInstanceID      Thread instance ID

\return void
***************************************************************************************************/
void Serialtun_Start
(
    taskMsgQueue_t *pTaskMsgQueue,
    instanceId_t thrInstanceID
)
{
    uint32_t error;

    if (NULL == gpSerialTunTaskMsgQueue)
    {
        gpSerialTunTaskMsgQueue = pTaskMsgQueue;

        /* Initialize Serialtun interface */
        VIRTUAL_TUN_initialize((uint8_t *)mSerialtunCfg.macAddress.eui);

        error = IP_IF_Add(gIpIfSerialTun_c, NULL, (mediaIfStruct_t *)gVirtualTunMediaIfPtr, gIpProtv6_c);
        mIfHandleSerialtun = IP_IF_GetIfHandle(gIpIfSerialTun_c);

        if (mSerialtunCfg.pNdPib && (NULL != mIfHandleSerialtun) && (gIpOk_c == error))
        {
            /* assign routing function to interface */
            mIfHandleSerialtun->ip6If.ip6UnicastForward = IP6_Forward;
            mIfHandleSerialtun->ip6If.ip6McastForward = IP6_MulticastForward;

#if THR_SERIAL_TUN_ENABLE_ND_HOST
            {

#if MLD_ENABLED
                MLD_IF_Enable(gIpIfSerialTun_c);
#endif
                /* start ND */
                ND_Open(mIfHandleSerialtun, mSerialtunCfg.pNdPib);

#if THR_SERIAL_TUN_USE_DHCP6_ADDR
                /* initialize a DHCPv6-PD client on the interface */
                mDhcp6ClientStartParams.clientStartMode = gDhcp6Pd_c;
                mDhcp6ClientStartParams.ipIfId = mSerialtunCfg.ifUniqueId;
                mDhcp6ClientStartParams.deviceType = DHCP6_HW_TYPE_EUI64;
                mDhcp6ClientStartParams.relayAddr = NULL;
                mDhcp6ClientStartParams.pPrefix = NULL;
                DHCP6_Client_Init(gpSerialTunTaskMsgQueue);
                DHCP6_Client_Start(&mDhcp6ClientStartParams);
                DHCP6_Client_RegisterSetAddr(SerialTun_SetAddr);
#else
                SerialTun_StaticConfig(thrInstanceID);
#endif

                /* Set callback for actions to perform on link change indications */
                IP_SerialTun_RegisterLinkChangedCb(SerialTun_LinkChanged);
            }

#elif THR_SERIAL_TUN_ENABLE_ND_ROUTER
            {
                thrPrefixAttr_t *pExternalIfPrefix = &THR_GetAttr_BrExternalIfPrefix(thrInstanceID);
                thrPrefixAttr_t globalOnMeshPrefix = THR_GetAttr_BrGlobalOnMeshPrefix(thrInstanceID);

#if MLD_ENABLED
                MLD_IF_Enable(gIpIfSerialTun_c);
#endif
                /* start ND */
                ND_Open(mIfHandleSerialtun, mSerialtunCfg.pNdPib);

                /* set Router Lifetime to 0 seconds - the border router is not a default router */
                ND_DefaultRouterConfig(mIfHandleSerialtun, ND_ROUTER_LIFETIME_ZERO, gNdRoutePrefMedFlag_c);

                /* configure external global pPrefix */
                ND_PrefixListAdd(mIfHandleSerialtun, &pExternalIfPrefix->prefix,
                                 pExternalIfPrefix->prefixLenBits,
                                 IP6_ADDRESS_LIFETIME_INFINITE, IP6_ADDRESS_LIFETIME_INFINITE, 1, 1);

                /* set on-mesh route to advertise */
                ND_RouteInfoSet(&(globalOnMeshPrefix.prefix), globalOnMeshPrefix.prefixLenBits,
                                gNdRoutePrefMedFlag_c, ND_LIFETIME_INFINITE);

                /* configure IP address based on global pPrefix */
                IP_IF_BindAddr6(gIpIfSerialTun_c, &pExternalIfPrefix->prefix,
                                ip6AddrTypeAutoconfigurable_c, IP6_ADDRESS_LIFETIME_INFINITE,
                                pExternalIfPrefix->prefixLenBits);
            }

#else       /* static configuration for TUN mode */
            {
                SerialTun_StaticConfig(thrInstanceID);
            }
#endif

        }
    }
}

/*!*************************************************************************************************
\fn     void Serialtun_ThreadStarted(uint32_t thrInstanceId)
\brief  This is a public function that handles the steps that should be done after the Thread
        stack is started.

\param  [in] thrInstanceId      Thread instance ID

\return void
***************************************************************************************************/
void Serialtun_ThreadStarted
(
    uint32_t thrInstanceId
)
{
    if (THR_GetAttr_IsDevConnected(thrInstanceId))
    {
        ipAddr_t ipAddr = THR_SERIAL_TUN_DEFAULT_ADDRESS;

        /* Uncomment for multicast address */
        //ipAddr_t mCastGroup = MCAST_3EAD_ADDRESS;

#if THR_SERIAL_TUN_USE_DHCP6_ADDR
        DHCP6_Client_SolicitAddress(&mDhcp6ClientStartParams);
#else
        SerialTun_StaticConfig(thrInstanceId);
#endif

#if THR_SERIAL_TUN_ENABLE_ND_HOST || THR_SERIAL_TUN_ENABLE_ND_ROUTER
        ip6IfAddrData_t *result = IP_IF_GetAddrByIf6(gIpIfSerialTun_c, 0, FALSE);
        ipAddr = result ? result->ip6Addr : in6addr_any;
#endif
        /* Register BR address for external commissioning */
        MESHCOP_RegisterBrServerAddr6(thrInstanceId, gIpIfSerialTun_c, &ipAddr);

        /* Uncomment to add forwarding rule */
        //IP6_SetStaticRoute(&mCastGroup, 128, IP6_STATIC_ROUTE_DEFAULT_METRIC, (ipAddr_t *)&in6addr_any, gIpIfSlp0_c);

        THR_SetBorderRouterIf(thrInstanceId, gIpIfSerialTun_c);

#if THR_SERIAL_TUN_ENABLE_ND_HOST || THR_SERIAL_TUN_ENABLE_ND_ROUTER
#if MDNS_ENABLED
        SerialTun_MDNSStart();
#endif
#endif
    }
}

#if THR_SERIAL_TUN_ENABLE_ND_HOST || THR_SERIAL_TUN_ENABLE_ND_ROUTER
/*!*************************************************************************************************
\fn     void Serialtun_RaReceived(void *pEvent)
\brief  This is a function that is called when a Prefix Information Option is present in an
        ND Router Advertisement packet.

\param  [in] pEvent     pointer to the event structure

\return void
***************************************************************************************************/
void Serialtun_RaReceived
(
    void *pEvent
)
{
    evmParams_t *pEvmParam = pEvent;

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
\fn     void Serialtun_RaRouteInfoReceived(void *pEvent)
\brief  This is a function that is called when a Route Information option is present in an
        ND Router Advertisement packet.

\param  [in] pEvent     pointer to the event structure

\return void
***************************************************************************************************/
void Serialtun_RaRouteInfoReceived
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
#endif /* THR_SERIAL_TUN_ENABLE_ND_HOST || THR_SERIAL_TUN_ENABLE_ND_ROUTER */

/*==================================================================================================
Private functions
==================================================================================================*/
#if THR_SERIAL_TUN_ENABLE_ND_HOST
/*!*************************************************************************************************
\private
\fn     static void SerialTun_LinkChanged(uint8_t linkState)
\brief  This is a private function that is called when the host informs about a link change on the
        TAP or uplink interface.

\param  [in] linkState   0: link down, 1: link up

\return void
***************************************************************************************************/
static void SerialTun_LinkChanged
(
    uint8_t linkState
)
{
    if (THR_GetAttr_IsDevConnected(mThrInstanceId))
    {
        if (linkState == gNd6LinkUp)
        {
#if THR_SERIAL_TUN_USE_DHCP6_ADDR
            /* restart DHCPv6-PD client to re-establish connectivity with the delegating router */
            DHCP6_Client_Stop(mDhcp6ClientStartParams.ipIfId,
                              mDhcp6ClientStartParams.clientStartMode,
                              mDhcp6ClientStartParams.pPrefix, 0);
            DHCP6_Client_Init(gpSerialTunTaskMsgQueue);
            DHCP6_Client_Start(&mDhcp6ClientStartParams);
            DHCP6_Client_SolicitAddress(&mDhcp6ClientStartParams);
#endif
        }
        else
        {
            /* Actions for link down event */
        }
    }
}

#if THR_SERIAL_TUN_USE_DHCP6_ADDR
/*!*************************************************************************************************
\private
\fn     static void SerialTun_SetAddr(ipIfUniqueId_t ipIfUniqueId, ipAddr_t *pIpAddr, void *pData)
\brief  This is a private function that is called when the router allocates a prefix for us (as
        response to a DHCPv6 Prefix Delegation request).

\param  [in] ipIfUniqueId   the unique ID of the interface
\param  [in] pIpAddr        pointer to the IP prefix assigned by the router (:: in case of none)
\param  [in] pData          pointer to a variable containing the length of the prefix (NULL in
                            case of no prefix)

\return void
***************************************************************************************************/
static void SerialTun_SetAddr
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
        thrPrefixAttr_t globalOnMeshPrefix = { 0 };
        thrPrefixAttr_t externalPrefix = { 0 };

        /* If we received a prefix length smaller than 64 we expand it to 64 for Thread assignment
         * as we don't need to create any new sub-networks inside Thread */
        if (prefixLen < 64)
        {
            prefixLen = 64;
        }

        globalOnMeshPrefix.prefixLenBits = prefixLen;
        FLib_MemCpy(globalOnMeshPrefix.prefix.addr8, pIpAddr->addr8, prefixLen >> 3);
        THR_SetAttr(mThrInstanceId, gNwkAttrId_BrGlobalOnMeshPrefix_c, 0,
                    sizeof(thrPrefixAttr_t), &globalOnMeshPrefix);

        /* Store the external IF prefix */
        externalPrefix.prefixLenBits = 48;
        FLib_MemCpy(externalPrefix.prefix.addr8, pIpAddr->addr8, externalPrefix.prefixLenBits >> 3);
        THR_SetAttr(mThrInstanceId, gNwkAttrId_BrExternalIfPrefix_c, 0,
                    sizeof(thrPrefixAttr_t), &externalPrefix);

        /* set Router Lifetime to 0 seconds - the border router is not a default router */
        ND_DefaultRouterConfig(mIfHandleSerialtun, ND_ROUTER_LIFETIME_ZERO, gNdRoutePrefMedFlag_c);

        /* set on-mesh route to advertise */
        ND_RouteInfoSet(&globalOnMeshPrefix.prefix, globalOnMeshPrefix.prefixLenBits,
                        gNdRoutePrefMedFlag_c, ND_LIFETIME_INFINITE);

        Serialtun_ConfigNwkData(mThrInstanceId);
    }
}
#endif /* THR_SERIAL_TUN_USE_DHCP6_ADDR */
#endif /* THR_SERIAL_TUN_ENABLE_ND_HOST */

/*!*************************************************************************************************
\private
\fn     static void Serialtun_ConfigNwkData(uint32_t thrInstanceId)
\brief  This is a private function that handles configuration of Network Data information based on
        the configured/acquired on-mesh and external prefixes.

\param  [in] thrInstanceId      Thread instance ID

\return void
***************************************************************************************************/
static void Serialtun_ConfigNwkData
(
    uint32_t thrInstanceId
)
{
    thrOtaBrPrefixSet_t prefixSet;
    thrPrefixAttr_t onMeshPrefix, externalIfPrefix;
    bool_t brPrefixSync = FALSE;

    onMeshPrefix = THR_GetAttr_BrGlobalOnMeshPrefix(thrInstanceId);
    externalIfPrefix = THR_GetAttr_BrExternalIfPrefix(thrInstanceId);

    /* Add the global on-mesh prefix */
    if (onMeshPrefix.prefixLenBits)
    {
        FLib_MemSet(&prefixSet, 0, sizeof(prefixSet));
        prefixSet.thrBrPrefixLength = onMeshPrefix.prefixLenBits;
        FLib_MemCpy(prefixSet.thrBrPrefixValue,
                    onMeshPrefix.prefix.addr8,
                    onMeshPrefix.prefixLenBits >> 3);
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

    if (externalIfPrefix.prefixLenBits)
    {
        /* Add External Route prefix */
        FLib_MemSet(&prefixSet, 0, sizeof(prefixSet));

        prefixSet.thrBrPrefixLength = externalIfPrefix.prefixLenBits;
        FLib_MemCpy(prefixSet.thrBrPrefixValue, externalIfPrefix.prefix.addr8, externalIfPrefix.prefixLenBits >> 3);
        htonal(prefixSet.thrBrPrefixLifetime, THR_ALL_FFs32);
        htonal(prefixSet.thrBrExtRouteLifetime, THR_ALL_FFs32);
        prefixSet.thrBrExtRouteAdvertised = TRUE;
        prefixSet.thrBrPrefixAdvertised = FALSE;
        THR_BrPrefixAttrAddEntry(thrInstanceId, &prefixSet);
        brPrefixSync = TRUE;
    }

    /* Distribute Network Data */
    if (brPrefixSync == TRUE)
    {
        THR_BrPrefixAttrSync(thrInstanceId);
    }
}

#if (!THR_SERIAL_TUN_ENABLE_ND_HOST && !THR_SERIAL_TUN_ENABLE_ND_ROUTER) || !THR_SERIAL_TUN_USE_DHCP6_ADDR
/*!*************************************************************************************************
\private
\fn     static void SerialTun_StaticConfig(instanceId_t thrInstanceID)
\brief  This is a private function that handles configuration of Network Data in TUN mode, when
        ND is disabled and external configuration is not supported.

\param  [in] thrInstanceId      Thread instance ID

\return void
***************************************************************************************************/
static void SerialTun_StaticConfig
(
    instanceId_t thrInstanceID
)
{
    ipAddr_t ipAddr = THR_SERIAL_TUN_DEFAULT_ADDRESS;
    thrPrefixAttr_t externalIfPrefix;

    IP_IF_BindAddr6(gIpIfSerialTun_c, &ipAddr,
                    ip6AddrTypeManual_c, IP6_ADDRESS_LIFETIME_INFINITE, 64);

    /* set the external prefix */
    FLib_MemSet(&externalIfPrefix, 0, sizeof(externalIfPrefix));
    externalIfPrefix.prefixLenBits = 64;
    FLib_MemCpy(&externalIfPrefix.prefix.addr8[0], ipAddr.addr8, 8);
    THR_SetAttr(thrInstanceID, gNwkAttrId_BrExternalIfPrefix_c, 0,
                sizeof(thrPrefixAttr_t), &externalIfPrefix);

    /* set static route for the serial tun */
    IP6_SetStaticRoute(&externalIfPrefix.prefix, externalIfPrefix.prefixLenBits,
                       IP6_STATIC_ROUTE_DEFAULT_METRIC, (ipAddr_t *)&in6addr_any, gIpIfSerialTun_c);

    /* Linux host is a default router for serial serial tun br */
    IP6_SetStaticRoute((ipAddr_t *)&in6addr_any, 0, IP6_STATIC_ROUTE_DEFAULT_METRIC,
                       (ipAddr_t *)&in6addr_any, gIpIfSerialTun_c);

    /* configure the external global prefix as on-link for address resolution */
    ND_PrefixListAdd(mIfHandleSerialtun, &externalIfPrefix.prefix,
                     externalIfPrefix.prefixLenBits, IP6_ADDRESS_LIFETIME_INFINITE,
                     IP6_ADDRESS_LIFETIME_INFINITE, TRUE, FALSE);

    Serialtun_ConfigNwkData(thrInstanceID);
}
#endif /* (!THR_SERIAL_TUN_ENABLE_ND_HOST && !THR_SERIAL_TUN_ENABLE_ND_ROUTER) || !THR_SERIAL_TUN_USE_DHCP6_ADDR */

#if THR_SERIAL_TUN_ENABLE_ND_HOST || THR_SERIAL_TUN_ENABLE_ND_ROUTER
#if MDNS_ENABLED
/*!*************************************************************************************************
\private
\fn    void SerialTun_AgentSet()
\brief  This function is used to set the Border Agent State Bitmap.

\param  [in] var      pointer to the bitmap
\param  [in] val      value to be set in bitmap
\param  [in] opType   type of value to be set used to be mapped correctly in bitmap

\return void
***************************************************************************************************/
static void SerialTun_AgentSet(uint32_t *var, uint32_t val, uint8_t opType)
{
    if (var != NULL)
    {
        switch (opType)
        {
            case CONNECTION_MODE_TYPE:
                SerialTun_AgentSet_ConnectionMode(*var, val)
                break;

            case THREAD_INTERFACE_STATUS_TYPE:
                SerialTun_AgentSet_ThreadInterfaceStatus(*var, val)
                break;

            case AVAILABILITY_TYPE:
                SerialTun_AgentSet_Availability(*var, val)
                break;

            default:
                break;
        }
    }
}

/*!*************************************************************************************************
\private
\fn    void SerialTun_MDNSStart()
\brief  This function is used to advertise a mDNS service for Commissioner Discovery (8.4.1.1.2),
        over IPv6.

\return void
***************************************************************************************************/
static void SerialTun_MDNSStart(void)
{
    ip6IfAddrData_t *pIpAddrData = NULL;
    STATE_BITMAP brAgentStateBitmap = 0;

    pIpAddrData = IP_IF_GetAddrByIf6(mIfHandleSerialtun->ifUniqueId, 0, TRUE);

    //TBD Connection Mode should depend on Security Policy TLV bits
    SerialTun_AgentSet(&brAgentStateBitmap, NetAgent_DtlsUserAllowed_c, CONNECTION_MODE_TYPE);

    if ((FALSE == THR_GetAttr_IsDevConnected(mThrInstanceId)) &&
            (THR_GetAttr_DeviceRole(mThrInstanceId) == gThrDevRole_Disconnected))
    {
        SerialTun_AgentSet(&brAgentStateBitmap, NetAgent_ThrNotActive_c, THREAD_INTERFACE_STATUS_TYPE);
    }
    else if ((FALSE == THR_GetAttr_IsDevConnected(mThrInstanceId)) &&
             (THR_GetAttr_DeviceRole(mThrInstanceId) != gThrDevRole_Disconnected))
    {
        SerialTun_AgentSet(&brAgentStateBitmap, NetAgent_ThrInitializedNotActive_c, THREAD_INTERFACE_STATUS_TYPE);
    }
    else if (TRUE == THR_GetAttr_IsDevConnected(mThrInstanceId))
    {
        SerialTun_AgentSet(&brAgentStateBitmap, NetAgent_ThrInitializedActive_c, THREAD_INTERFACE_STATUS_TYPE);
    }

    SerialTun_AgentSet(&brAgentStateBitmap, NetAgent_HighAvailability_c, AVAILABILITY_TYPE);

    if (ntohall(gpaThrActiveAttr[0]->xPanId) != THR_ALL_FFs64)
    {
        /* Record Version */
        FLib_MemCpy(mdnsService.txtInfo[0].optionValue, mDnsRecordVersion, FLib_StrLen(mDnsRecordVersion));
        FLib_MemCpy(mdnsService.txtInfo[0].optionValue + FLib_StrLen(mDnsRecordVersion), "1", FLib_StrLen("1"));
        mdnsService.txtInfo[0].length = FLib_StrLen(mDnsRecordVersion) + FLib_StrLen("1");

        /* Thread Version */
        FLib_MemCpy(mdnsService.txtInfo[1].optionValue, mDnsThreadVersion, FLib_StrLen(mDnsThreadVersion));
        FLib_MemCpy(mdnsService.txtInfo[1].optionValue + FLib_StrLen(mDnsThreadVersion), "1.1.1", FLib_StrLen("1.1.1"));
        mdnsService.txtInfo[1].length = FLib_StrLen(mDnsThreadVersion) + FLib_StrLen("1.1.1");

        /* State bitmap */
        FLib_MemCpy(mdnsService.txtInfo[2].optionValue, mDnsStateBitmap, FLib_StrLen(mDnsStateBitmap));
        FLib_MemCpy(mdnsService.txtInfo[2].optionValue + FLib_StrLen(mDnsStateBitmap), (void *)brAgentStateBitmap,
                    sizeof(brAgentStateBitmap));
        mdnsService.txtInfo[2].length = FLib_StrLen(mDnsStateBitmap) + sizeof(brAgentStateBitmap);

        /* Network Name */
        if (SerialTun_AgentCheck_ConnectionMode(brAgentStateBitmap) != NetAgent_DtlsNotAllowed_c)
        {
            FLib_MemCpy(mdnsService.txtInfo[3].optionValue, mDnsNwkName, FLib_StrLen(mDnsNwkName));

            if ((SerialTun_AgentCheck_ConnectionMode(brAgentStateBitmap) == NetAgent_DtlsUserAllowed_c) &&
                    (SerialTun_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrInitializedActive_c))
            {
                FLib_MemCpy(mdnsService.txtInfo[3].optionValue + FLib_StrLen(mDnsNwkName), gpaThrActiveAttr[mThrInstanceId]->nwkName.aStr,
                            gpaThrActiveAttr[mThrInstanceId]->nwkName.length);
                mdnsService.txtInfo[3].length = FLib_StrLen(mDnsNwkName) + gpaThrActiveAttr[mThrInstanceId]->nwkName.length;
            }
            else if (SerialTun_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrNotActive_c)
            {
                /* if Thread Interface is not initialized, the string should reflect the Border Agent product or model name */
                FLib_MemCpy(mdnsService.txtInfo[3].optionValue + FLib_StrLen(mDnsModelName), gpaThrStringAttr[mThrInstanceId]->modelName.aStr,
                            gpaThrStringAttr[mThrInstanceId]->modelName.length);
                mdnsService.txtInfo[3].length = FLib_StrLen(mDnsModelName) + gpaThrStringAttr[mThrInstanceId]->modelName.length;
            }
        }

        /* Extended PAN ID */
        if (SerialTun_AgentCheck_ConnectionMode(brAgentStateBitmap) != NetAgent_DtlsNotAllowed_c)
        {
            FLib_MemCpy(mdnsService.txtInfo[4].optionValue, mDnsXPanID, FLib_StrLen(mDnsXPanID));

            if ((SerialTun_AgentCheck_ConnectionMode(brAgentStateBitmap) == NetAgent_DtlsUserAllowed_c) &&
                    (SerialTun_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrInitializedActive_c))
            {
                NWKU_HexToAscii(gpaThrActiveAttr[mThrInstanceId]->xPanId, 8, mdnsService.txtInfo[4].optionValue + FLib_StrLen(mDnsXPanID),
                                16, TRUE);
                mdnsService.txtInfo[4].length = FLib_StrLen(mDnsXPanID) + 16;
            }
            else if (SerialTun_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrNotActive_c)
            {
                /* field should have the SHA-256 hash value of the factory IEEE EUI-64
                 * of the Thread Interface if Thread Interface Status is not active */
                NWKU_TransformUint64ToArray(mdnsService.txtInfo[4].optionValue + FLib_StrLen(mDnsXPanID),
                                            THR_GetAttr_ShaHashAddr(mThrInstanceId));
                mdnsService.txtInfo[4].length = FLib_StrLen(mDnsXPanID) + 8;
            }
        }

        /* Vendor Name */
        FLib_MemCpy(mdnsService.txtInfo[5].optionValue, mDnsVendorName, FLib_StrLen(mDnsVendorName));
        FLib_MemCpy(mdnsService.txtInfo[5].optionValue  + FLib_StrLen(mDnsVendorName), gpaThrStringAttr[mThrInstanceId]->vendorName.aStr,
                    gpaThrStringAttr[mThrInstanceId]->vendorName.length);
        mdnsService.txtInfo[5].length = FLib_StrLen(mDnsVendorName) + gpaThrStringAttr[mThrInstanceId]->vendorName.length;

        /* Model Name */
        FLib_MemCpy(mdnsService.txtInfo[6].optionValue, mDnsModelName, FLib_StrLen(mDnsModelName));
        FLib_MemCpy(mdnsService.txtInfo[6].optionValue + FLib_StrLen(mDnsModelName), gpaThrStringAttr[mThrInstanceId]->modelName.aStr,
                    gpaThrStringAttr[mThrInstanceId]->modelName.length);
        mdnsService.txtInfo[6].length = FLib_StrLen(mDnsModelName) + gpaThrStringAttr[mThrInstanceId]->modelName.length;

        /* Active Timestamp */
        if (SerialTun_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrInitializedActive_c)
        {
            FLib_MemCpy(mdnsService.txtInfo[7].optionValue, mDnsActiveTimestamp, FLib_StrLen(mDnsActiveTimestamp));
            FLib_MemCpy(mdnsService.txtInfo[7].optionValue + FLib_StrLen(mDnsActiveTimestamp), &gpaThrPendingAttr[mThrInstanceId]->activeTimestamp,
                        sizeof(gpaThrPendingAttr[mThrInstanceId]->activeTimestamp));
            mdnsService.txtInfo[7].length = FLib_StrLen(mDnsActiveTimestamp) + sizeof(gpaThrPendingAttr[mThrInstanceId]->activeTimestamp);
        }

        /* Partition ID */
        if (SerialTun_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrInitializedActive_c)
        {
            FLib_MemCpy(mdnsService.txtInfo[8].optionValue, mDnsPartitionId, FLib_StrLen(mDnsPartitionId));
            NWKU_TransformUint32ToArray(mdnsService.txtInfo[8].optionValue + (uint8_t)FLib_StrLen(mDnsPartitionId),
                                        THR_GetAttr_PartionId(mThrInstanceId));
            mdnsService.txtInfo[8].length = FLib_StrLen(mDnsPartitionId) + sizeof(THR_GetAttr_PartionId(mThrInstanceId));
        }

        /* Vendor Data */
        FLib_MemCpy(mdnsService.txtInfo[9].optionValue, mDnsVendorData, FLib_StrLen(mDnsVendorData));
        FLib_MemCpy(mdnsService.txtInfo[9].optionValue  + FLib_StrLen(mDnsVendorData), gpaThrStringAttr[mThrInstanceId]->vendorData.aStr,
                    gpaThrStringAttr[mThrInstanceId]->vendorData.length);
        mdnsService.txtInfo[9].length = FLib_StrLen(mDnsVendorData) + gpaThrStringAttr[mThrInstanceId]->vendorData.length;

        /* Vendor OUI */
        FLib_MemCpy(mdnsService.txtInfo[10].optionValue, mDnsVendorOui, FLib_StrLen(mDnsVendorOui));
        FLib_MemCpy(mdnsService.txtInfo[10].optionValue  + FLib_StrLen(mDnsVendorOui), (uint8_t *)mSerialtunCfg.macAddress.eui, 3 * sizeof(uint8_t));
        mdnsService.txtInfo[10].length = FLib_StrLen(mDnsVendorOui) + 3 * sizeof(uint8_t);

        /* Start mDNS */
        IP_IF_AddMulticastGroup6(gIpIfSerialTun_c, (ipAddr_t *)&mdnsIp6Addr);

        MDNS_Init(gpSerialTunTaskMsgQueue);
        MDNS_JoinIf(mIfHandleSerialtun, AF_INET6);

        FLib_MemCpy(&mdnsService.address, &pIpAddrData->ip6Addr, sizeof(ipAddr_t));
        MDNS_AddService(&mdnsService);
    }
}
#endif /* MDNS_ENABLED */
#endif /* THR_SERIAL_TUN_ENABLE_ND_HOST || THR_SERIAL_TUN_ENABLE_ND_ROUTER */

#endif /* THR_SERIAL_TUN_ROUTER */
