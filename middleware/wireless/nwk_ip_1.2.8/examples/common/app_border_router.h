/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_BORDER_ROUTER_H
#define _APP_BORDER_ROUTER_H
/*!=================================================================================================
\file       app_border_router.h
\brief      This is a header file for the echo protocol application.

==================================================================================================*/

/*==================================================================================================
 Include Files
 ==================================================================================================*/

#include "EmbeddedTypes.h"
#include "thread_utils.h"

/*==================================================================================================
 Public macros
 ==================================================================================================*/

/*==================================================================================================
 Public type definitions
 ==================================================================================================*/

/*! Border router HOST Mode configuration */
typedef enum
{
    gBrNormalHostMode_c = 0x00,       /*!< border router normal host mode */
    gBrLinuxHostMode_c,               /*!< border router Linux host mode */
    gBrLinuxGwMode_c,                 /*!< border router Linux gateway mode */
}brHostMode_t;


/*! Border router connection mode */
typedef enum
{
    gBrConDhcpPd_c = 0x00,         /*!< border router has Prefix from DHCP-PD */
    gBrConNdProxy_c,               /*!< border router shares prefix with LAN in ND-PROXY mode */
    gBrConNdRouter_c               /*!< border router provides all the prefix information from user config */
}brConnnectionMode_t;
/* Connection mode */
typedef enum NetAgent_ConnectionMode_tag
{
    NetAgent_DtlsNotAllowed_c      = 0,
    NetAgent_DtlsUserAllowed_c     = 1,
    NetAgent_DtlsAllowed_c         = 2,
    NetAgent_DtlsVendorAllowed_c   = 3,
    NetAgent_ConnectionModeLast_c  = 4
    /* values 4-7 - reserved */
}NetAgentConnectionMode_t;

/* Thread Interface Status */
typedef enum brAgThrInterfaceStatus_tag
{
    NetAgent_ThrNotActive_c              = 0,
    NetAgent_ThrInitializedNotActive_c   = 1,
    NetAgent_ThrInitializedActive_c      = 2,
    NetAgent_ThrInterfaceStatusLast_c    = 3
    /* value 3 - reserved */
}NetAgentThrInterfaceStatus_t;

/* Availability */
typedef enum brAgAvailability_tag
{
    NetAgent_InfrequentAvailability_c    = 0,
    NetAgent_HighAvailability_c          = 1,
    NetAgent_AvailabilityLast_c          = 2
    /* values 2-3 - reserved */
}NetAgentAvailability_t;

/*==================================================================================================
 Public global variables declarations
 ==================================================================================================*/
extern taskMsgQueue_t *gpEthTaskMsgQueue;

/*==================================================================================================
 Public function prototypes
 ==================================================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif

/*!*************************************************************************************************
\fn     void BR_Start(uint8_t  *pTaskMsgQueue, instanceId_t thrInstanceID)
\brief  This function will initiate the Ethernet or USB Ethernet or Seriat Tun components.

\param  [in] pTaskMsgQueue   task message queue to be used by border router services
\param  [in] thrInstanceId   instance ID of the Thread interface
***************************************************************************************************/
void BR_Start(uint8_t *pTaskMsgQueue, instanceId_t thrInstanceID);

/*!*************************************************************************************************
\fn     void BR_ThreadStartedCb(instanceId_t thrInstanceID)
\brief  This is a callback function for the Thread network connected event

\param  [in] thrInstanceId   instance ID of the Thread interface
***************************************************************************************************/
void BR_ThreadStartedCb(instanceId_t thrInstanceID);

/*!*************************************************************************************************
\fn     BR_RaRouteInfoReceived(uint8_t *pEvent)
\brief  This function is called when a Route Information option is present in an ND Router
        Advertisement packet.

\param  [in] pEvent     pointer to the event structure
***************************************************************************************************/
void BR_RaRouteInfoReceived(uint8_t *pEvent);

/*!*************************************************************************************************
\fn     BR_RaReceived(uint8_t *pEvent)
\brief  This function is a callback for ND Router Advertisement packet received on the external
        interface

\param  [in] pEvent     pointer to the event structure
***************************************************************************************************/
void BR_RaReceived(uint8_t *pEvent);

#if ND_OPT_DNS_RCV_ENABLED
/*!*************************************************************************************************
\fn     BR_RaDnsInfoReceived(void *pEvent)
\brief  This function is called when a RDNSS or DNSSL option is present in an ND Router
        Advertisement packet.

\param  [in] pEvent     pointer to the event structure
***************************************************************************************************/
void BR_RaDnsInfoReceived(void *pEvent);
#endif

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _APP_APP_BORDER_ROUTER_H */
