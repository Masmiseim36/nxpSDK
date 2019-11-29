/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       app_border_router.c
\brief      This is a public source file for the Ethernet interface.

\=================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "EmbeddedTypes.h"

#include "app_stack_config.h"
#include "stack_config.h"

#include "app_border_router.h"

#include "app_ethernet.h"
#include "app_ethernet_usb.h"
#include "app_serial_tun.h"
#include "mdns.h"
#include "dns_client.h"
#include "ip6.h"
#include "dhcp_client.h"
#include "dhcp_server.h"
#include "dhcp6_client.h"
#include "dhcp6_server.h"
#include "string.h"
#if IP_IP4_ENABLE
    #include "ip4.h"
#endif
#if DNS_ENABLED
  #include "thread_dns.h"
#endif

#include "thread_utils.h"
#include "thread_border_router.h"

/*==================================================================================================
Private macros
==================================================================================================*/
#if MDNS_ENABLED

/* A complete Service Instance Name has the following recommended format:
   VendorName ProductName._meshcop._udp_.local. */
#define MDNS_SERVICE_VENDOR_NAME    "NXP"
#define MDNS_SERVICE_PRODUCT_NAME   "Kinetis BR"
#define MDNS_SERVICE_INSTANCE_NAME  MDNS_SERVICE_VENDOR_NAME " " MDNS_SERVICE_PRODUCT_NAME
#define MDNS_SERVICE_NAME           "_meshcop"
#define MDNS_SERVICE_PROTOCOL       "_udp"
#define MDNS_SERVICE_DOMAIN         "local"
#define MDNS_SERVICE_FULL_NAME      MDNS_SERVICE_NAME "." MDNS_SERVICE_PROTOCOL "." MDNS_SERVICE_DOMAIN

#define STATE_BITMAP      uint32_t
#define TWOBITS_MASK      0x0003
#define THREEBITS_MASK    0x0007

#define CONNECTION_MODE_TYPE            0
#define THREAD_INTERFACE_STATUS_TYPE    1
#define AVAILABILITY_TYPE               2

#define BR_AgentCheck_ConnectionMode(var)            (var & THREEBITS_MASK)
#define BR_AgentCheck_ThreadInterfaceStatus(var)     ((var >> 3) & TWOBITS_MASK)
#define BR_AgentCheck_Availability(var)              ((var >> 5) & TWOBITS_MASK)

#define BR_AgentSet_ConnectionMode(var, val)         if(val < 4)                           \
                                                     {                                     \
                                                         var &= (~THREEBITS_MASK);         \
                                                         var |= val;                       \
                                                     }

#define BR_AgentSet_ThreadInterfaceStatus(var, val)  if(val < 3)                            \
                                                     {                                      \
                                                         var &= ((~(TWOBITS_MASK << 3)));  \
                                                         var |= (val<<3);                   \
                                                     }

#define BR_AgentSet_Availability(var, val)           if(val < 2)                            \
                                                     {                                      \
                                                          var &= ((~(TWOBITS_MASK << 5))); \
                                                          var |= (val<<5);                  \
                                                     }
#endif  /* MDNS_ENABLED */

/*==================================================================================================
Private type definitions
==================================================================================================*/


/*==================================================================================================
Private prototypes
==================================================================================================*/

static void BR_AddExternaIfPrefix(uint32_t thrInstanceId);
#if SERIAL_TUN_IF || ENET_IF || USB_RNDIS_IF
static void BR_ConfigureMode(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID,
                                      ipIfUniqueId_t ifId);
#endif
static bool_t BR_ConfigNwkData(uint32_t thrInstanceId);

#if BR_ROUTER_MODE
static void BR_PrefixSetup(instanceId_t thrInstanceID);
static void BR_ConfigureRouterMode(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID,
                                               ipIfUniqueId_t ifId);
#endif
#if BR_HOST_MODE
static void BR_ConfigureHostMode(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID,
                                            ipIfUniqueId_t ifId);
static void BR_ConfigureHostMode(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID,
                                            ipIfUniqueId_t ifId);
static void BR_ConfigureHostNormalMode(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID,
                                                     ipIfUniqueId_t ifId);
static void BR_ConfigureHostLinuxMode(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID,
                                                    ipIfUniqueId_t ifId);
static void BR_StartDhcpClient(taskMsgQueue_t *pTaskMsgQueue, ipIfUniqueId_t ifId,
                                         uint32_t clientMode);
static void BR_SetAddr6(ipIfUniqueId_t ipIfUniqueId, ipAddr_t *pIpAddr, void *pData);
static void BR_DhcpNdProxyCb(ipAddr_t* pIpAddr,  dhcp6AddrState_t addrState, void* pData);

#if IP_IP4_ENABLE && !SERIAL_TUN_IF
static void BR_SetAddr4(ipIfUniqueId_t ipIfUniqueId, uint32_t *pIpAddr,  void *pData);
#endif /* IP_IP4_ENABLE */
#endif /* BR_HOST_MODE */

#if MDNS_ENABLED
static void BR_AgentSet(uint32_t *var, uint32_t val, uint8_t opType);
static void BR_MDNSStart(void);
#endif

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
#if MDNS_ENABLED
static mDnsService_t mBrdnsService = {.hostName      = (uint8_t *)MDNS_SERVICE_INSTANCE_NAME,
                                      .name          = (uint8_t *)MDNS_SERVICE_NAME,
                                      .target        = (uint8_t *)MDNS_SERVICE_INSTANCE_NAME,
                                      .domain        = (uint8_t *)MDNS_SERVICE_DOMAIN,
                                      .protocolType  = (uint8_t *)MDNS_SERVICE_PROTOCOL,
                                      .fullName      = (uint8_t *)MDNS_SERVICE_FULL_NAME,
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
#endif

#if ND_OPT_DNS_RCV_ENABLED
    static bool_t mThrDNSServicesRegistered = FALSE;
#endif

#if BR_HOST_MODE

#if SERIAL_TUN_IF
    static brHostMode_t brHostMode = gBrLinuxHostMode_c;
#else
    static brHostMode_t brHostMode = gBrNormalHostMode_c;
#endif /* SERIAL_TUN_IF */

static bool_t mBrIsGlobalThrPrefixConfig = FALSE;
static brConnnectionMode_t mBrConnMode = gBrConDhcpPd_c;

#else
static bool_t mBrIsGlobalThrPrefixConfig = TRUE;
static brConnnectionMode_t mBrConnMode = gBrConNdRouter_c;
#endif /* BR_HOST_MODE */

static uint32_t mBrThrInstanceId = 0;
ifHandle_t mBrExtIfHandle = NULL;

#if MDNS_ENABLED
static uint32_t mBrIp4Address = 0;
#endif
/*==================================================================================================
Public global variables declarations
==================================================================================================*/
taskMsgQueue_t *gpBrTaskMsgQueue = NULL;
extern taskMsgQueue_t *mpAppThreadMsgQueue;
extern taskMsgQueue_t * pIpMsgQueue;

extern const thrActiveAttr_t mThrDefaultActiveAttr;
/*==================================================================================================
Public functions
==================================================================================================*/

/*!*************************************************************************************************
\fn     void BR_Start(uint8_t  *pTaskMsgQueue, instanceId_t thrInstanceID)
\brief  This function will initiate the Ethernet or USB Ethernet or Seriat Tun components.

\param  [in] pTaskMsgQueue   task message queue to be used by border router services
\param  [in] thrInstanceId   instance ID of the Thread interface
***************************************************************************************************/
void BR_Start
(
    uint8_t *pTaskMsgQueue,
    instanceId_t thrInstanceID
)
{
#if SERIAL_TUN_IF || ENET_IF || USB_RNDIS_IF
    ipIfUniqueId_t ifId;
#endif

    pTaskMsgQueue = (uint8_t *)pIpMsgQueue;

    gpBrTaskMsgQueue = (taskMsgQueue_t *)pTaskMsgQueue;
    mBrThrInstanceId = thrInstanceID;

#if SERIAL_TUN_IF
    Serialtun_Start(pTaskMsgQueue, thrInstanceID);
    ifId = gIpIfSerialTun_c;
#endif

#if ENET_IF
    IFENET_Start(pTaskMsgQueue, thrInstanceID);

    ifId = gIpIfEth0_c;
#endif

#if USB_RNDIS_IF
    IFUSBENET_Start((taskMsgQueue_t*)pTaskMsgQueue, thrInstanceID);
    ifId = gIpIfUsbRndis_c;
#endif

#if SERIAL_TUN_IF || ENET_IF || USB_RNDIS_IF
    THR_SetBorderRouterIf(thrInstanceID, ifId);
    BR_ConfigureMode((taskMsgQueue_t*)pTaskMsgQueue, thrInstanceID, ifId);

    mBrExtIfHandle = IP_IF_GetIfHandle(ifId);
#endif
}

/*!*************************************************************************************************
\fn     void BR_ThreadStartedCb(instanceId_t thrInstanceID)
\brief  This is a callback function for the Thread network connected event

\param  [in] thrInstanceId   instance ID of the Thread interface
***************************************************************************************************/
void BR_ThreadStartedCb
(
    instanceId_t thrInstanceID
)
{

    bool_t brPrefixSync = FALSE;


    brPrefixSync |= BR_ConfigNwkData(thrInstanceID);

#if MDNS_ENABLED
    BR_MDNSStart();
#endif

    /* Distribute Network Data */
    if (brPrefixSync == TRUE)
    {
        THR_BrPrefixAttrSync(thrInstanceID);
    }

}

/*!*************************************************************************************************
\fn     BR_RaReceived(uint8_t *pEvent)
\brief  This function is a callback for ND Router Advertisement packet received on the external
        interface

\param  [in] pEvent     pointer to the event structure
***************************************************************************************************/
void BR_RaReceived
(
    uint8_t *pEvent
)
{
    evmParams_t *pEvmParam = (evmParams_t*)pEvent;

#if BR_HOST_MODE

    thrPrefixAttr_t externalIfPrefix;
    ipPrefix_t *pPrefix;
    thrPrefixAttr_t externalPrefix = {0};
    ndCfg_t **ppNdCfg = NULL;

    THR_GetAttr_BrExternalIfPrefix(mBrThrInstanceId, &externalIfPrefix);

    if ((pEvmParam) && (pEvmParam->pBuff) && (mBrExtIfHandle))
    {
        pPrefix = (ipPrefix_t *)pEvmParam->pBuff;

        if (0 == externalIfPrefix.prefixLenBits)
        {
            /* Store the external IF prefix*/
            externalPrefix.prefixLenBits = pPrefix->prefixLen;
            FLib_MemCpy(&externalPrefix.prefix.addr8[0], pPrefix->aPrefix, pPrefix->prefixLen >> 3);
            THR_SetAttr(mBrThrInstanceId, gNwkAttrId_BrExternalIfPrefix_c, 0, sizeof(thrPrefixAttr_t),
                        (uint8_t*)&externalPrefix);

            ppNdCfg = ND_GetCfg(mBrExtIfHandle->ifUniqueId);

            if (ppNdCfg && *ppNdCfg)
            {
                if ((*ppNdCfg)->bManagedConfig)
                {
                    BR_StartDhcpClient(gpBrTaskMsgQueue,mBrExtIfHandle->ifUniqueId, gDhcp6AddressAssignment_c);
                }
                else if ((*ppNdCfg)->bOtherConfig)
                {
                    BR_StartDhcpClient(gpBrTaskMsgQueue,mBrExtIfHandle->ifUniqueId, gDhcp6DnsAddrAssign_c);
                }
            }
        }
#if 0
        ndNeighborEntry_t **ppNdNeighborEntry = ND_GetDefaultRouter(mBrExtIfHandle);
        if ((NULL != ppNdNeighborEntry) &&
             (NULL != *ppNdNeighborEntry) &&
             (0 != (*ppNdNeighborEntry)->routerExpireTimestampSec))
        {
            bool_t bDefRoute = TRUE;

            /* default router is available -> make BR also a default router for Thread */
            THR_SetAttr(mBrThrInstanceId, gNwkAttrId_BrDefaultRouteOnMeshPrefix_c, 0,
                        sizeof(bool_t), (uint8_t*)&bDefRoute);
        }
#endif
    }

#endif /* BR_HOST_MODE */

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
\fn     BR_RaRouteInfoReceived(uint8_t *pEvent)
\brief  This function is called when a Route Information option is present in an ND Router
        Advertisement packet.

\param  [in] pEvent     pointer to the event structure
***************************************************************************************************/
void BR_RaRouteInfoReceived
(
    uint8_t *pEvent
)
{
    evmParams_t *pEvmParam = (evmParams_t*)pEvent;

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
            THR_BrPrefixAttrRemoveEntry(mBrThrInstanceId, prefixSet.thrBrPrefixLength, prefixSet.thrBrPrefixValue);
        }
        else
        {
            FLib_MemCpy(prefixSet.thrBrExtRouteLifetime, pCurrOption->lifetime, sizeof(pCurrOption->lifetime));
            THR_BR_FLAGS_SET(prefixSet.thrBrExtRouteFlags, pCurrOption->prf >> 3,
                             THR_BR_EXT_ROUTE_FLAGS_R_PREF_MASK, THR_BR_EXT_ROUTE_FLAGS_R_PREF_OFFSET);
            prefixSet.thrBrPrefixAdvertised = FALSE;
            prefixSet.thrBrExtRouteAdvertised = TRUE;

            THR_BrPrefixAttrAddEntry(mBrThrInstanceId, &prefixSet);
        }

        FLib_MemSet(&prefixSet, 0, sizeof(thrOtaBrPrefixSet_t));
    }

    THR_BrPrefixAttrSync(mBrThrInstanceId);

    MEM_BufferFree(pEvmParam->pBuff);
    MEM_BufferFree(pEvmParam);
}

#if ND_OPT_DNS_RCV_ENABLED
/*!*************************************************************************************************
\fn     BR_RaDnsInfoReceived(void *pEvent)
\brief  This function is called when a RDNSS or DNSSL option is present in an ND Router
        Advertisement packet.

\param  [in] pEvent     pointer to the event structure
***************************************************************************************************/
void BR_RaDnsInfoReceived
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
        THR_BrPrefixAttrSync(mBrThrInstanceId);
    }

    MEM_BufferFree(pEvmParam);
}
#endif /* ND_OPT_DNS_RCV_ENABLED */
/*==================================================================================================
Private functions
==================================================================================================*/
#if SERIAL_TUN_IF || ENET_IF || USB_RNDIS_IF
/*!*************************************************************************************************
\fn     static void BR_ConfigureMode(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID,
        ipIfUniqueId_t ifId)
\brief  Configures Border Router mode(Host or Router)

\param  [in] pTaskMsgQueue  task message queue to be used by border router services
\param  [in] thrInstanceId  instance ID of the Thread interface
\param  [in] ifId           the unique ID of the BR interface
***************************************************************************************************/
static void BR_ConfigureMode
(
    taskMsgQueue_t *pTaskMsgQueue,
    instanceId_t thrInstanceID,
    ipIfUniqueId_t ifId
)
{

#if BR_ROUTER_MODE
    BR_PrefixSetup(thrInstanceID);
    BR_ConfigureRouterMode(pTaskMsgQueue,thrInstanceID,ifId);
#endif

#if BR_HOST_MODE
    BR_ConfigureHostMode(pTaskMsgQueue,thrInstanceID,ifId);
#endif
}
#endif /* SERIAL_TUN_IF || ENET_IF || USB_RNDIS_IF */

#if BR_ROUTER_MODE
/*!*************************************************************************************************
\fn     static void BR_PrefixSetup(instanceId_t thrInstanceID)
\brief  For ND Router mode this function configures the IPv6 prefix to be used on the LAN interface
        based on the ULA prefix of the Thread network present in Thread attributes

\param  [in] thrInstanceId   thread interface ID
***************************************************************************************************/
static void BR_PrefixSetup
(
    instanceId_t thrInstanceID
)
{
    /* On ND ROUTER use case, if the External IF Prefix is not set (all bytes are zeros),
     * generate the External Prefix using the first 6 bytes from the global on-mesh prefix,
     * concatenated with a random subnet mask (byte 6 and 7) */
    thrPrefixAttr_t externalIfPrefix;
    THR_GetAttr_BrExternalIfPrefix(thrInstanceID, &externalIfPrefix);

    if(IP_IsAddrEqual(&externalIfPrefix.prefix, &in6addr_any))
    {
        thrPrefixAttr_t externalPrefix;
        thrPrefixAttr_t onMeshPrefix;
        uint8_t* pSubNet = &externalPrefix.prefix.addr8[6];

        THR_GetAttr_BrGlobalOnMeshPrefix(thrInstanceID, &onMeshPrefix);
        FLib_MemSet(&externalPrefix, 0, sizeof(externalPrefix));
        externalPrefix.prefixLenBits = 64;
        /* Get the first 6 bytes from the global on-mesh prefix */
        FLib_MemCpy(&externalPrefix.prefix.addr8[0], onMeshPrefix.prefix.addr8, 6);

        /* Generate the random subnet mask. Make sure that is not zero, all ffs or is not
         * the same as the subnet mask of the global on-mesh prefix */
        do
        {
            NWKU_GenRand(pSubNet, 2);
        }while( (ntohas(pSubNet) == 0) ||
                (ntohas(pSubNet) == THR_ALL_FFs16) ||
                 FLib_MemCmp(pSubNet, &onMeshPrefix.prefix.addr8[6] , 2));
        /* Set the attribute */
        THR_SetAttr(thrInstanceID, gNwkAttrId_BrExternalIfPrefix_c, 0,
                    sizeof(thrPrefixAttr_t), (uint8_t*)&externalPrefix);
    }
}

/*!*************************************************************************************************
\fn     static void BR_ConfigureRouterMode(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID,
        ipIfUniqueId_t ifId)
\brief  Configures Border Router in Router mode

\param  [in] pTaskMsgQueue  task message queue to be used by border router services
\param  [in] thrInstanceId  instance ID of the Thread interface
\param  [in] ifId           the unique ID of the BR interface
***************************************************************************************************/
static void BR_ConfigureRouterMode
(
    taskMsgQueue_t *pTaskMsgQueue,
    instanceId_t thrInstanceID,
    ipIfUniqueId_t ifId
)
{
    thrPrefixAttr_t externalIfPrefix;
    thrPrefixAttr_t globalOnMeshPrefix;

    THR_GetAttr_BrExternalIfPrefix(thrInstanceID, &externalIfPrefix);
    THR_GetAttr_BrGlobalOnMeshPrefix(thrInstanceID, &globalOnMeshPrefix);

    /* set Router Lifetime to 0 seconds - the border router is not a default router */
    ND_DefaultRouterConfig(ifId, ND_ROUTER_LIFETIME_ZERO, gNdRoutePrefMedFlag_c);

    /* configure external global pPrefix */
    ND_PrefixListAdd(ifId, &externalIfPrefix.prefix,
                     externalIfPrefix.prefixLenBits,
                     IP6_ADDRESS_LIFETIME_INFINITE, IP6_ADDRESS_LIFETIME_INFINITE, 1, 1);

    /* set on-mesh route to advertise */
    ND_RouteInfoSet(&globalOnMeshPrefix.prefix, globalOnMeshPrefix.prefixLenBits,
                    gNdRoutePrefMedFlag_c, ND_LIFETIME_INFINITE);


    /* configure IP address based on global pPrefix */
    IP_IF_BindAddr6(ifId, &externalIfPrefix.prefix,
                    ip6AddrTypeAutoconfigurable_c, IP6_ADDRESS_LIFETIME_INFINITE,
                    externalIfPrefix.prefixLenBits);

#if DHCP4_SERVER_ENABLED && !SERIAL_TUN_IF
    uint32_t serverIpAddr = IPV4_ADDR(192, 168, 1, 1);
    uint32_t startIpAddr = IPV4_ADDR(192, 168, 1, 2);
    uint32_t endIpAddr = IPV4_ADDR(192, 168, 1, 10);
    DHCP4_Server_Init((uint8_t*)pTaskMsgQueue);
    DHCP4_Server_Start(serverIpAddr, startIpAddr, endIpAddr, ifId);
#endif /* DHCP v4 SERVER */

}
#endif /* BR_ROUTER_MODE */

#if BR_HOST_MODE
/*!*************************************************************************************************
\fn     static void BR_ConfigureHostMode(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID,
        ipIfUniqueId_t ifId)
\brief  Configures Border Router in Host mode

\param  [in] pTaskMsgQueue  task message queue to be used by border router services
\param  [in] thrInstanceId  instance ID of the Thread interface
\param  [in] ifId           the unique ID of the BR interface
***************************************************************************************************/
static void BR_ConfigureHostMode
(
    taskMsgQueue_t *pTaskMsgQueue,
    instanceId_t thrInstanceID,
    ipIfUniqueId_t ifId
)
{

    switch (brHostMode)
    {
        case gBrNormalHostMode_c:
            BR_ConfigureHostNormalMode(pTaskMsgQueue, thrInstanceID, ifId);
            break;
        case gBrLinuxHostMode_c:
            BR_ConfigureHostLinuxMode(pTaskMsgQueue, thrInstanceID, ifId);
            break;
        default:
            break;
    }
}

/*!*************************************************************************************************
\fn     static void BR_ConfigureHostNormalMode(taskMsgQueue_t *pTaskMsgQueue,
        instanceId_t thrInstanceID, ipIfUniqueId_t ifId)
\brief  Configures Border Router in classic ND Host mode

\param  [in] pTaskMsgQueue  task message queue to be used by border router services
\param  [in] thrInstanceId  instance ID of the Thread interface
\param  [in] ifId           the unique ID of the BR interface
***************************************************************************************************/
static void BR_ConfigureHostNormalMode
(
    taskMsgQueue_t *pTaskMsgQueue,
    instanceId_t thrInstanceID,
    ipIfUniqueId_t ifId
)
{

    BR_StartDhcpClient(pTaskMsgQueue,ifId, gDhcp6Pd_c);
    mBrConnMode = gBrConDhcpPd_c;

#if DHCP4_CLIENT_ENABLED && !SERIAL_TUN_IF
    DHCP4_Client_Init(gpBrTaskMsgQueue);
    DHCP4_Client_Start(ifId, NULL);
    DHCP4_Client_RegisterSetAddr(BR_SetAddr4);
#endif

}

/*!*************************************************************************************************
\fn     static void BR_ConfigureHostLinuxMode(taskMsgQueue_t *pTaskMsgQueue,
        instanceId_t thrInstanceID, ipIfUniqueId_t ifId)
\brief  Configures Border Router in Host mode that is connected to a Linux Ip stack

\param  [in] pTaskMsgQueue  task message queue to be used by border router services
\param  [in] thrInstanceId  instance ID of the Thread interface
\param  [in] ifId           the unique ID of the BR interface
***************************************************************************************************/
static void BR_ConfigureHostLinuxMode
(
    taskMsgQueue_t *pTaskMsgQueue,
    instanceId_t thrInstanceID,
    ipIfUniqueId_t ifId
)
{
    /* initialize a DHCPv6-PD client on the interface */
    BR_StartDhcpClient(pTaskMsgQueue,ifId, gDhcp6Pd_c);
}

/*!*************************************************************************************************
\fn     static void BR_StartDhcpClient(taskMsgQueue_t *pTaskMsgQueue, ipIfUniqueId_t ifId,
        uint32_t clientMode)
\brief  Configures DHCPv6 Client for Prefix Delegation mode and starts solicit process on lan if

\param  [in] pTaskMsgQueue  task message queue to be used by border router services
\param  [in] ifId           the unique ID of the BR interface
\param  [in] clientMode     DHCP Client mode(Addr Assign, PD or DNS)
***************************************************************************************************/
static void BR_StartDhcpClient
(
    taskMsgQueue_t *pTaskMsgQueue,
    ipIfUniqueId_t ifId,
    uint32_t clientMode
)
{
    dhcp6ClientStartParams_t mDhcp6ClientStartParams;

    /* Start DHCP Client for Global Address */
    DHCP6_Client_Init(pTaskMsgQueue);

    mDhcp6ClientStartParams.clientStartMode = clientMode;
    mDhcp6ClientStartParams.ipIfId = ifId;
    mDhcp6ClientStartParams.deviceType = DHCP6_HW_TYPE_EUI64;
    mDhcp6ClientStartParams.relayAddr = NULL;
    mDhcp6ClientStartParams.pPrefix = NULL;
    DHCP6_Client_Start(&mDhcp6ClientStartParams);

    if (gDhcp6DnsAddrAssign_c != clientMode)
    {
        DHCP6_Client_RegisterSetAddr(BR_SetAddr6);
    }

    DHCP6_Client_SolicitAddress(&mDhcp6ClientStartParams);
}

/*!*************************************************************************************************
\fn     static void BR_SetAddr6(ipIfUniqueId_t ipIfUniqueId, ipAddr_t *pIpAddr, void *pData)
\brief  This function is called when the router allocates a prefix for us(in response to a DHCP
        Prefix Delegation request).

\param  [in] ipIfUniqueId   the unique ID of the interface
\param  [in] pIpAddr        pointer to the IP prefix assigned by the router (:: in case of none)
\param  [in] pData          pointer to a variable containing the length of the prefix (NULL in
                            case of no prefix)
***************************************************************************************************/
static void BR_SetAddr6
(
    ipIfUniqueId_t ipIfUniqueId,
    ipAddr_t *pIpAddr,
    void *pData
)
{
    thrPrefixAttr_t globalOnMeshPrefix = {0};
    thrPrefixAttr_t externalIfPrefix = {0};
    bool_t bNwkDataSync = FALSE;
    bool_t bDefRoute = FALSE;

    /* Router could assign a prefix */
    if (pData && pIpAddr)
    {
        if(!IP_IsAddrEqual(pIpAddr, &in6addr_any))
        {
            uint8_t prefixLen = *(uint8_t *)pData;

            /* If we received a prefix length smaller than 64 we expand it to 64 for Thread assignment
             * as we don't need to create any new sub-networks inside Thread */
            if (prefixLen < 64)
            {
                prefixLen = 64;
            }

            if(64 == prefixLen)
            {
                mBrIsGlobalThrPrefixConfig = TRUE;
                FLib_MemSet(&globalOnMeshPrefix, 0, sizeof(globalOnMeshPrefix));
                globalOnMeshPrefix.prefixLenBits = prefixLen;
                FLib_MemCpy(&globalOnMeshPrefix.prefix.addr8[0], pIpAddr->addr8,prefixLen >> 3);
                THR_SetAttr(mBrThrInstanceId, gNwkAttrId_BrGlobalOnMeshPrefix_c, 0, sizeof(thrPrefixAttr_t),
                            (uint8_t*)&globalOnMeshPrefix);

                bNwkDataSync |= BR_ConfigNwkData(mBrThrInstanceId);
            }
            else if(128 == prefixLen)
            {
                /* full address assigned -> bind received address */
                IP_IF_BindAddr6(ipIfUniqueId, pIpAddr, ip6AddrTypeManual_c,
                                IP6_ADDRESS_LIFETIME_INFINITE, 64U);
            }

        }
    }
    else
    {
        /* did not get any prefix from DHCP PD */
        THR_GetAttr_BrExternalIfPrefix(mBrThrInstanceId, &externalIfPrefix);

        mBrIsGlobalThrPrefixConfig = TRUE;
        if (externalIfPrefix.prefixLenBits > 0)
        {
             /* prefix from RA available -> use ND proxy */
             mBrConnMode = gBrConNdProxy_c;

             ND_SetProxyCb(mBrExtIfHandle->ifUniqueId, THR_BR_Dhcp6NdProxyCb);
             DHCP6_Server_RegisterAddrAssignCb(BR_DhcpNdProxyCb);

             bDefRoute = TRUE;
             THR_SetAttr(mBrThrInstanceId, gNwkAttrId_BrDefaultRouteOnMeshPrefix_c, 0, sizeof(bool_t),
                         (uint8_t*)&bDefRoute);

             /* DHCPv6 server is started by NWK configuration */
             bNwkDataSync |= BR_ConfigNwkData(mBrThrInstanceId);
        }
    }

    if (TRUE == bNwkDataSync)
    {
        THR_BrPrefixAttrSync(mBrThrInstanceId);
    }
}
#if IP_IP4_ENABLE && !SERIAL_TUN_IF
/*!*************************************************************************************************
\fn     static void BR_SetAddr4(ipIfUniqueId_t ipIfUniqueId, uint32_t *pIpAddr, void *pData)
\brief  This function is called when the DHCPv4 server allocates an IPv4 address in response to a
        DHCP solicit message

\param  [in] pIfHandle  pointer to the interface handle
\param  [in] pIpAddr    pointer to the IP prefix assigned by the router(:: in case of none)
\param  [in] pData      pointer to a variable containing the length of the prefix(NULL in case of no
                        prefix)
***************************************************************************************************/
static void BR_SetAddr4
(
    ipIfUniqueId_t ipIfUniqueId,
    uint32_t *pIpAddr,
    void *pData
)
{

#if MDNS_ENABLED
    mBrIp4Address = *pIpAddr;

    if (THR_GetAttr_IsDevConnected(mBrThrInstanceId))
    {
        BR_MDNSStart();
    }

#endif

}
#endif /* IP_IP4_ENABLE */
#endif /* BR_HOST_MODE */

/*!*************************************************************************************************
\fn     BR_ConfigNwkData
\brief  This is a private function which handles configuration of Network Data information for Thread

\param  [in] thrInstanceId  instance ID of the Thread interface

\return bool_t   An attribute sync is required if TRUE
***************************************************************************************************/
static bool_t BR_ConfigNwkData
(
    uint32_t thrInstanceId
)
{
    bool_t brPrefixSync = FALSE;
    thrOtaBrPrefixSet_t prefixSet = {0};
    thrPrefixAttr_t onMeshPrefix = {0};
    thrPrefixAttr_t externalIfPrefix = {0};

    bool_t bAddExtRoute = TRUE;
    ipAddr_t dns6Addr = {0};

    THR_GetAttr_BrGlobalOnMeshPrefix(thrInstanceId, &onMeshPrefix);
    THR_GetAttr_BrExternalIfPrefix(thrInstanceId, &externalIfPrefix);

    if ((mBrIsGlobalThrPrefixConfig == TRUE) && (THR_GetAttr_IsDevConnected(thrInstanceId)))
    {
        /* Add the global on-mesh prefix */
        if (onMeshPrefix.prefixLenBits)
        {
            FLib_MemSet(&prefixSet, 0, sizeof(prefixSet));

            switch (mBrConnMode)
            {
                case gBrConDhcpPd_c:
                case gBrConNdRouter_c:

                    prefixSet.thrBrPrefixLength = onMeshPrefix.prefixLenBits;
                    prefixSet.thrBrPrefixFlags[1] = THR_BR_PREFIX_FLAGS_P_SLAAC_MASK;
                    prefixSet.thrBrPrefixFlags[1] |= THR_BR_PREFIX_FLAGS_P_ON_MESH_MASK;
                    FLib_MemCpy(prefixSet.thrBrPrefixValue, onMeshPrefix.prefix.addr8,
                                onMeshPrefix.prefixLenBits >> 3);
                    break;

                case gBrConNdProxy_c:

                    prefixSet.thrBrPrefixLength = externalIfPrefix.prefixLenBits;
                    prefixSet.thrBrPrefixFlags[1] = THR_BR_PREFIX_FLAGS_P_DHCP_MASK;
                    FLib_MemCpy(prefixSet.thrBrPrefixValue, externalIfPrefix.prefix.addr8,
                                externalIfPrefix.prefixLenBits >> 3);

                    /* prevent external prefix to be added also as external route for ND Proxy mode */
                    bAddExtRoute = FALSE;

                    break;

                default:
                    break;
            }

            if (THR_GetAttr_BrDefaultRoute(thrInstanceId))
            {
                prefixSet.thrBrPrefixFlags[1] |= THR_BR_PREFIX_FLAGS_P_DEFAULT_MASK;
            }

            if (TRUE == mThrDNSServicesRegistered)
            {
                prefixSet.thrBrPrefixFlags[0] |=THR_BR_PREFIX_FLAGS_P_ND_DNS_MASK;
            }
            else
            {

#if DNS_ENABLED
                {
                    IP_AddrCopy(&dns6Addr, DNS_GetDnsServerIp6Addr());
                    THR_AddDnsServer(thrInstanceId, &dns6Addr);
                }
#endif

                if (FALSE == IP_IsAddrEqual(&dns6Addr, (ipAddr_t*)&in6addr_any))
                {
                    prefixSet.thrBrPrefixFlags[1] |= THR_BR_PREFIX_FLAGS_P_CONFIGURE_MASK;
                }
            }

            htonal(prefixSet.thrBrPrefixLifetime, THR_ALL_FFs32);
            htonal(prefixSet.thrBrExtRouteLifetime, THR_ALL_FFs32);
            prefixSet.thrBrPrefixAdvertised = TRUE;
            prefixSet.thrBrExtRouteAdvertised = FALSE;
            THR_BrPrefixAttrAddEntry(thrInstanceId, &prefixSet);
            brPrefixSync = TRUE;
        }

        if ((externalIfPrefix.prefixLenBits) && (TRUE == bAddExtRoute))
        {
            /* Add External Route prefix */
            BR_AddExternaIfPrefix(thrInstanceId);
            brPrefixSync = TRUE;
        }
    }
    return brPrefixSync;

}

/*!*************************************************************************************************
\fn     static void BR_AddExternaIfPrefix(uint32_t thrInstanceId)
\brief  This function is called when we want to add an external route prefix to the network data

\param  [in] thrInstanceId   thread interface ID
***************************************************************************************************/
static void BR_AddExternaIfPrefix
(
    uint32_t thrInstanceId
)
{
    thrOtaBrPrefixSet_t prefixSet;
    thrPrefixAttr_t externalIfPrefix;

    THR_GetAttr_BrExternalIfPrefix(thrInstanceId, &externalIfPrefix);

    if (externalIfPrefix.prefixLenBits)
    {
        /* Add External Route prefix */
        FLib_MemSet(&prefixSet, 0, sizeof(prefixSet));

        prefixSet.thrBrPrefixLength = externalIfPrefix.prefixLenBits;
        FLib_MemCpy(prefixSet.thrBrPrefixValue, externalIfPrefix.prefix.addr8, externalIfPrefix.prefixLenBits >> 3);
        htonal(prefixSet.thrBrPrefixLifetime, THR_ALL_FFs32);
        htonal(prefixSet.thrBrExtRouteLifetime, THR_ALL_FFs32);
        prefixSet.thrBrPrefixAdvertised = FALSE;
        prefixSet.thrBrExtRouteAdvertised = TRUE;

        THR_BrPrefixAttrAddEntry(thrInstanceId, &prefixSet);
    }
}

#if BR_HOST_MODE
/*!*************************************************************************************************
\fn     static void BR_DhcpNdProxyCb(ipAddr_t* pIpAddr, dhcp6AddrState_t addrState, void* pData)
\brief  This function is called when DHCPv6 Server assigns/deletes an IP address

\param  [in] pIpAddr     IPv6 address assigned/deleted
\param  [in] addrState   address state - gDhcp6AddrAssigned_c or gDhcp6AddrDeleted_c
\param  [in] pData       callback private data
***************************************************************************************************/
static void BR_DhcpNdProxyCb
(
    ipAddr_t* pIpAddr,
    dhcp6AddrState_t addrState,
    void* pData
)
{

    ipAddr_t solicitedNodeAddr;
    IP6_GetSolicitedMcastAddr(pIpAddr, &solicitedNodeAddr);

    if(gDhcp6AddrAssigned_c == addrState)
    {
        IP_IF_Join(mBrExtIfHandle->ifUniqueId, &solicitedNodeAddr);
    }
    else
    {
        /* gDhcp6AddrDeleted_c */
        IP_IF_Leave(mBrExtIfHandle->ifUniqueId, &solicitedNodeAddr);
    }
}
#endif /* BR_HOST_MODE */

#if MDNS_ENABLED
/*!*************************************************************************************************
\fn     static void BR_AgentSet(uint32_t *var, uint32_t val, uint8_t opType)
\brief  This function is used to set the Boarder Agent State Bitmap

\param  [in] var      pointer to the bitmap
\param  [in] val      value to be set in bitmap
\param  [in] opType   type of value to be set used to be mapped correctly in bitmap
***************************************************************************************************/
static void BR_AgentSet
(
    uint32_t *var,
    uint32_t val,
    uint8_t opType
)
{
    if (var != NULL)
    {
        switch (opType)
        {
            case CONNECTION_MODE_TYPE:
                BR_AgentSet_ConnectionMode(*var, val)
                break;

            case THREAD_INTERFACE_STATUS_TYPE:
                BR_AgentSet_ThreadInterfaceStatus(*var, val)
                break;

            case AVAILABILITY_TYPE:
                BR_AgentSet_Availability(*var, val)
                break;

            default:
                break;
        }
    }
}
/*!*************************************************************************************************
\fn     static void BR_MDNSStart(void)
\brief  Configures Border Router MDNS server data

\return void
***************************************************************************************************/
static void BR_MDNSStart
(
    void
)
{
    ip6IfAddrData_t *pIpAddrData = NULL;
    STATE_BITMAP brAgentStateBitmap = 0;
    uint32_t strLen = 0;
    uint8_t aStr[64] = {0};
    uint8_t xpanId[8];

    pIpAddrData = IP_IF_GetAddrByIf6(mBrExtIfHandle->ifUniqueId, 0, TRUE);

    //TBD Connection Mode should depend on Security Policy TLV bits
    BR_AgentSet(&brAgentStateBitmap, NetAgent_DtlsUserAllowed_c, CONNECTION_MODE_TYPE);

    if ((FALSE == THR_GetAttr_IsDevConnected(mBrThrInstanceId)) &&
         (THR_GetAttr_DeviceRole(mBrThrInstanceId) == gThrDevRole_Disconnected))
    {
        BR_AgentSet(&brAgentStateBitmap, NetAgent_ThrNotActive_c, THREAD_INTERFACE_STATUS_TYPE);
    }
    else if ((FALSE == THR_GetAttr_IsDevConnected(mBrThrInstanceId)) &&
             (THR_GetAttr_DeviceRole(mBrThrInstanceId) != gThrDevRole_Disconnected))
    {
        BR_AgentSet(&brAgentStateBitmap, NetAgent_ThrInitializedNotActive_c, THREAD_INTERFACE_STATUS_TYPE);
    }
    else if (TRUE == THR_GetAttr_IsDevConnected(mBrThrInstanceId))
    {
        BR_AgentSet(&brAgentStateBitmap, NetAgent_ThrInitializedActive_c, THREAD_INTERFACE_STATUS_TYPE);
    }

    BR_AgentSet(&brAgentStateBitmap, NetAgent_HighAvailability_c, AVAILABILITY_TYPE);

    THR_GetAttr_XPanId(0, xpanId);
    if (ntohall(xpanId) != THR_ALL_FFs64)
    {
        /* Record Version */
        FLib_MemCpy(mBrdnsService.txtInfo[0].optionValue, mDnsRecordVersion, FLib_StrLen(mDnsRecordVersion));
        FLib_MemCpy(mBrdnsService.txtInfo[0].optionValue + FLib_StrLen(mDnsRecordVersion), "1", FLib_StrLen("1"));
        mBrdnsService.txtInfo[0].length = FLib_StrLen(mDnsRecordVersion) + FLib_StrLen("1");

        /* Thread Version */
        FLib_MemCpy(mBrdnsService.txtInfo[1].optionValue, mDnsThreadVersion, FLib_StrLen(mDnsThreadVersion));
        FLib_MemCpy(mBrdnsService.txtInfo[1].optionValue + FLib_StrLen(mDnsThreadVersion), "1.1.0", FLib_StrLen("1.1.0"));
        mBrdnsService.txtInfo[1].length = FLib_StrLen(mDnsThreadVersion) + FLib_StrLen("1.1.0");

        /* State bitmap */
        FLib_MemCpy(mBrdnsService.txtInfo[2].optionValue, mDnsStateBitmap, FLib_StrLen(mDnsStateBitmap));
        FLib_MemCpy(mBrdnsService.txtInfo[2].optionValue + FLib_StrLen(mDnsStateBitmap), (void *)brAgentStateBitmap,
                    sizeof(brAgentStateBitmap));
        mBrdnsService.txtInfo[2].length = FLib_StrLen(mDnsStateBitmap) + sizeof(brAgentStateBitmap);

        /* Network Name */
        if (BR_AgentCheck_ConnectionMode(brAgentStateBitmap) != NetAgent_DtlsNotAllowed_c)
        {
            FLib_MemCpy(mBrdnsService.txtInfo[3].optionValue, mDnsNwkName, FLib_StrLen(mDnsNwkName));

            if ((BR_AgentCheck_ConnectionMode(brAgentStateBitmap) == NetAgent_DtlsUserAllowed_c) &&
                (BR_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrInitializedActive_c))
            {
                THR_GetAttr(mBrThrInstanceId, gNwkAttrId_NwkName_c, 0, &strLen, aStr);
                FLib_MemCpy(mBrdnsService.txtInfo[3].optionValue + FLib_StrLen(mDnsNwkName), aStr, strLen);
                mBrdnsService.txtInfo[3].length = FLib_StrLen(mDnsNwkName) + strLen;
            }
            else if (BR_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrNotActive_c)
            {
                THR_GetAttr(mBrThrInstanceId, gNwkAttrId_ModelName_c, 0, &strLen, aStr);
                /* if Thread Interface is not initialized, the string should reflect the Border Agent product or model name */
                FLib_MemCpy(mBrdnsService.txtInfo[3].optionValue + FLib_StrLen(mDnsModelName), aStr, strLen);
                mBrdnsService.txtInfo[3].length = FLib_StrLen(mDnsModelName) + strLen;
            }
        }

        /* Extended PAN ID */
        if (BR_AgentCheck_ConnectionMode(brAgentStateBitmap) != NetAgent_DtlsNotAllowed_c)
        {
            FLib_MemCpy(mBrdnsService.txtInfo[4].optionValue, mDnsXPanID, FLib_StrLen(mDnsXPanID));

            if ((BR_AgentCheck_ConnectionMode(brAgentStateBitmap) == NetAgent_DtlsUserAllowed_c) &&
                (BR_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrInitializedActive_c))
            {
                NWKU_HexToAscii(xpanId, 8, mBrdnsService.txtInfo[4].optionValue + FLib_StrLen(mDnsXPanID),
                                16, TRUE);
                mBrdnsService.txtInfo[4].length = FLib_StrLen(mDnsXPanID) + 16;
            }
            else if (BR_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrNotActive_c)
            {
                /* field should have the SHA-256 hash value of the factory IEEE EUI-64
                 * of the Thread Interface if Thread Interface Status is not active */
                NWKU_TransformUint64ToArray(mBrdnsService.txtInfo[4].optionValue + FLib_StrLen(mDnsXPanID),
                                            THR_GetAttr_ShaHashAddr(mBrThrInstanceId));
                mBrdnsService.txtInfo[4].length = FLib_StrLen(mDnsXPanID) + 8;
            }
        }

        /* Vendor Name */
        THR_GetAttr(mBrThrInstanceId, gNwkAttrId_VendorName_c, 0, &strLen, aStr);
        FLib_MemCpy(mBrdnsService.txtInfo[5].optionValue, mDnsVendorName, FLib_StrLen(mDnsVendorName));
        FLib_MemCpy(mBrdnsService.txtInfo[5].optionValue  + FLib_StrLen(mDnsVendorName), aStr, strLen);
        mBrdnsService.txtInfo[5].length = FLib_StrLen(mDnsVendorName) + strLen;

        /* Model Name */
        THR_GetAttr(mBrThrInstanceId, gNwkAttrId_ModelName_c, 0, &strLen, aStr);
        FLib_MemCpy(mBrdnsService.txtInfo[6].optionValue, mDnsModelName, FLib_StrLen(mDnsModelName));
        FLib_MemCpy(mBrdnsService.txtInfo[6].optionValue + FLib_StrLen(mDnsModelName), aStr, strLen);
        mBrdnsService.txtInfo[6].length = FLib_StrLen(mDnsModelName) + strLen;

        /* Active Timestamp */
        if (BR_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrInitializedActive_c)
        {
            uint8_t pendingActiveTimestamp[8];
            THR_GetAttr(mBrThrInstanceId, gNwkAttrId_MeshCop_PendingActiveTimestamp_c, 0, NULL, (uint8_t*)&pendingActiveTimestamp);
            FLib_MemCpy(mBrdnsService.txtInfo[7].optionValue, mDnsActiveTimestamp, FLib_StrLen(mDnsActiveTimestamp));
            FLib_MemCpy(mBrdnsService.txtInfo[7].optionValue + FLib_StrLen(mDnsActiveTimestamp), &pendingActiveTimestamp,
                        sizeof(pendingActiveTimestamp));
            mBrdnsService.txtInfo[7].length = FLib_StrLen(mDnsActiveTimestamp) + sizeof(pendingActiveTimestamp);
        }

        /* Partition ID */
        if (BR_AgentCheck_ThreadInterfaceStatus(brAgentStateBitmap) == NetAgent_ThrInitializedActive_c)
        {
            FLib_MemCpy(mBrdnsService.txtInfo[8].optionValue, mDnsPartitionId, FLib_StrLen(mDnsPartitionId));
            NWKU_TransformUint32ToArray(mBrdnsService.txtInfo[8].optionValue + (uint8_t)FLib_StrLen(mDnsPartitionId),
                                        THR_GetAttr_PartionId(mBrThrInstanceId));
            mBrdnsService.txtInfo[8].length = FLib_StrLen(mDnsPartitionId) + sizeof(THR_GetAttr_PartionId(mBrThrInstanceId));
        }

        /* Vendor Data */
        THR_GetAttr(mBrThrInstanceId, gNwkAttrId_VendorData_c, 0, &strLen, aStr);
        FLib_MemCpy(mBrdnsService.txtInfo[9].optionValue, mDnsVendorData, FLib_StrLen(mDnsVendorData));
        FLib_MemCpy(mBrdnsService.txtInfo[9].optionValue  + FLib_StrLen(mDnsVendorData), aStr, strLen);
        mBrdnsService.txtInfo[9].length = FLib_StrLen(mDnsVendorData) + strLen;

        /* Vendor OUI */
        FLib_MemCpy(mBrdnsService.txtInfo[10].optionValue, mDnsVendorOui, FLib_StrLen(mDnsVendorOui));
        FLib_MemCpy(mBrdnsService.txtInfo[10].optionValue  + FLib_StrLen(mDnsVendorOui), mBrExtIfHandle->ifDevAddrTbl[DEFAULT_LLADDR_IDX].eui,
                    3 * sizeof(uint8_t));
        mBrdnsService.txtInfo[10].length = FLib_StrLen(mDnsVendorOui) + 3 * sizeof(uint8_t);

        /* Start mDNS */
        MDNS_Init(gpBrTaskMsgQueue);
        MDNS_JoinIf(mBrExtIfHandle, AF_INET6);

        if (0 != mBrIp4Address)
        {
            MDNS_JoinIf(mBrExtIfHandle, AF_INET);
            mBrdnsService.ip4Addr = mBrIp4Address;
        }

        FLib_MemCpy(&mBrdnsService.address, &pIpAddrData->ip6Addr, sizeof(ipAddr_t));
        MDNS_AddService(&mBrdnsService);
    }
}

#endif
