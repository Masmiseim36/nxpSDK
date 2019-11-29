/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
\file       app_serial_tun.c
\brief      This is a public source file for the Serial Tunnel interface.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "app_stack_config.h"
#include "stack_config.h"
/* Ip */
#include "ip_if_management.h"
#include "ip.h"
#include "ip6.h"
#include "ip_if_serial_tun.h"
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

#if SERIAL_TUN_IF
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

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
#if BR_ROUTER_MODE
static CONST ndPib_t mSerialtunRouterNdPibCfg = {ND6_ROUTER_CONFIGURATION};
#else
static CONST ndPib_t mSerialtunRouterNdPibCfg = {ND6_HOST_CONFIGURATION};
#endif

static const externalIfConfig_t mSerialtunCfg = {SERIAL_TUN_ROUTER_CONFIGURATION};

static ifHandle_t mIfHandleSerialtun = NULL;
static taskMsgQueue_t *mpSerialTunTaskMsgQueue = NULL;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn     void Serialtun_Start(instanceId_t thrInstanceId, uint8_t* pTaskMsgQueue)
\brief  This is a public function used to start the Serialtun Interface.
\return void
***************************************************************************************************/
void Serialtun_Start
(
    uint8_t *pTaskMsgQueue,
    instanceId_t thrInstanceID
)
{
    uint32_t error;

    if (NULL == mpSerialTunTaskMsgQueue)
    {
        mpSerialTunTaskMsgQueue = (taskMsgQueue_t*)&pTaskMsgQueue;

        /* Initialize Serialtun interface */
        VIRTUAL_TUN_initialize((uint8_t *)mSerialtunCfg.macAddress.eui);

        error = IP_IF_Add(mSerialtunCfg.ifUniqueId, NULL, (mediaIfStruct_t *)gVirtualTunMediaIfPtr, gIpProtv6_c);
        mIfHandleSerialtun = IP_IF_GetIfHandle(mSerialtunCfg.ifUniqueId);

        if (mSerialtunCfg.pNdPib && (NULL != mIfHandleSerialtun) && (gIpOk_c == error))
        {
            /* assign routing function to interface */
            mIfHandleSerialtun->ip6If.ip6UnicastForward = IP6_UnicastForward;
            mIfHandleSerialtun->ip6If.ip6McastForward = IP6_MulticastForward;
#if MLD_ENABLED
            MLD_IF_Enable(gIpIfSerialTun_c);
#endif
            /* open the ND instance */
            ND_Open(mIfHandleSerialtun->ifUniqueId, mSerialtunCfg.pNdPib);
        }
    }
}
#endif /* SERIAL_TUN_IF */