/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
\file       app_ethernet.c
\brief      This is a public source file for the Ethernet interface.
*/

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
#include "mld.h"

#include "nvm_adapter.h"

#include "event_manager.h"

#include "enet_driver.h"

#include "thread_utils.h"
#include "ip_if_enet.h"

#include "app_ethernet.h"

/*==================================================================================================
Private macros
==================================================================================================*/
#if ENET_IF
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
        .pNdPib                                 = (ndPib_t*)&mEthRouterNdPibCfg


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
        .pNdPib                                 = (ndPib_t*)&mStaticNdHostPibCfg

/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
#if BR_ROUTER_MODE
static const ndPib_t mEthRouterNdPibCfg         = {ND6_ROUTER_CONFIGURATION};
static const externalIfConfig_t mEnetCfg        = {ETH_ROUTER_CONFIGURATION};
#elif BR_HOST_MODE
static const ndPib_t mStaticNdHostPibCfg        = {ND6_HOST_CONFIGURATION};
static const externalIfConfig_t mEnetCfg        = {ETH_HOST_CONFIGURATION};
#endif

static taskMsgQueue_t *mpEthTaskMsgQueue = NULL;
/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern const mediaIfStruct_t *gEnetMediaIfPtr;

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
    ifHandle_t mIfHandleEnet;

    if (NULL == mpEthTaskMsgQueue)
    {
        uint16_t protocols = gIpProtv6_c;
#if IP_IP4_ENABLE
        protocols |= gIpProtv4_c;
#endif
        void *enetIfPtr = NULL;
        uint32_t error;
        mpEthTaskMsgQueue = pTaskMsgQueue;

        /* Initialize Ethernet interface */
        ENET_Initialize((uint8_t *)mEnetCfg.macAddress.eui, &enetIfPtr, pTaskMsgQueue);
        error = IP_IF_Add(mEnetCfg.ifUniqueId, enetIfPtr, (mediaIfStruct_t *)gEnetMediaIfPtr, protocols);
        mIfHandleEnet = IP_IF_GetIfHandle(mEnetCfg.ifUniqueId);

        if (mEnetCfg.pNdPib && (gIpOk_c == error) && (NULL != mIfHandleEnet))
        {
            /* assign routing function to interface */
            mIfHandleEnet->ip6If.ip6UnicastForward = IP6_UnicastForward;
            mIfHandleEnet->ip6If.ip6McastForward = IP6_MulticastForward;
#if IP_IP4_ENABLE && IP_IP4_ROUTING_ENABLE
            mIfHandleEnet->ip4If.ip4Forward = IP4_UnicastForward;
#endif

#if MLD_ENABLED
            MLD_IF_Enable(mEnetCfg.ifUniqueId);
#endif

            /* start ND */
            ND_Open(mIfHandleEnet->ifUniqueId, mEnetCfg.pNdPib);
        }
    }
}
#endif /* ENET_IF*/

