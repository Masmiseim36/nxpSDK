/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _PMTU_DISCOVERY_H
#define _PMTU_DISCOVERY_H
/*!=================================================================================================
\file       pmtu_discovery.h
\brief      This is a header file for the Neighbor Discovery for IP version 6 (IPv6) module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

/* Network Includes */
#include "ip.h"
#include "ip_if_management.h"
#include "network_utils.h"

/*==================================================================================================
Public macros
==================================================================================================*/


/*! Description of the PMTU_PERIODIC_INCREASE_PMTU_ENABLED configuration option: set to 1 to enable
    the periodical increase of PMTU in the Path MTU Discovery module */
#ifndef PMTU_PERIODIC_INCREASE_PMTU_ENABLED
    #define PMTU_PERIODIC_INCREASE_PMTU_ENABLED    1
#endif

/*! Description of the PMTU_CALLBACKS_ENABLED configuration option: set to 1 to enable
    the possibility of registering callbacks that are called to notify the upper layers */
#ifndef PMTU_CALLBACKS_ENABLED
    #define PMTU_CALLBACKS_ENABLED                 0
#endif

/*! Description of the PMTU_MANAGEMENT_FUNCTIONS_ENABLED configuration option: set to 1 to enable
    the management functions of the Path MTU Discovery */
#ifndef PMTU_MANAGEMENT_FUNCTIONS_ENABLED
    #define PMTU_MANAGEMENT_FUNCTIONS_ENABLED      0
#endif

/*! Description of the PMTU_REGISTER_ICMP_PKT_TOO_BIG_HANDLER configuration option: set to 1 to
    register the Packet Too Big Error Handler for packets send using ICMP */
#ifndef PMTU_REGISTER_ICMP_PKT_TOO_BIG_HANDLER
    #define PMTU_REGISTER_ICMP_PKT_TOO_BIG_HANDLER 1
#endif

/*! Description of the PMTU_REGISTER_TCP_PKT_TOO_BIG_HANDLER configuration option: set to 1 to
    register the Packet Too Big Error Handler for packets send using TCP */
#ifndef PMTU_REGISTER_TCP_PKT_TOO_BIG_HANDLER
    #define PMTU_REGISTER_TCP_PKT_TOO_BIG_HANDLER 0
#endif

/*! Description of the PMTU_REGISTER_UDP_PKT_TOO_BIG_HANDLER configuration option: set to 1 to
    register the Packet Too Big Error Handler for packets send using UDP */
#ifndef PMTU_REGISTER_UDP_PKT_TOO_BIG_HANDLER
    #define PMTU_REGISTER_UDP_PKT_TOO_BIG_HANDLER 0
#endif

/*==================================================================================================
Public type definitions
==================================================================================================*/

typedef void (*pmtuNotifyDroppedPktCb_t)(ipAddr_t *pDestAddr);
typedef void (*pmtuNotifyIncreasePmtuCb_t)(ipAddr_t *pDestAddr, uint32_t newPmtu);

/*==================================================================================================
Public global variables declarations
==================================================================================================*/



/*==================================================================================================
Public function prototypes
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     uint32_t PMTU_GetPmtu(ifHandle_t* ifHandle, ipAddr_t *pSrcIpAddr,ipAddr_t *pDestIpAddr)
\brief  Interface function for the Path MTU discovery module.

\param  [in]    ifHandle        Interface Handle. If the Source Address is unspecified, the ifHandle MUST NOT be NULL.
\param  [in]    pSrcIpAddr      Pointer to the source IP address.
\param  [in]    pDestIpAddr     Pointer to the destination IP address.

\return         uint32_t        Value of the PMTU for the specified path.
***************************************************************************************************/
uint32_t PMTU_GetPmtu(ifHandle_t ifHandle, ipAddr_t *pSrcIpAddr, ipAddr_t *pDestIpAddr);

#if ND_ENABLED

/*!*************************************************************************************************
\fn     void PMTU_Init(void)
\brief  Interface function for the Path MTU discovery module.
***************************************************************************************************/
void PMTU_Init(void);

#if PMTU_MANAGEMENT_FUNCTIONS_ENABLED

/*!*************************************************************************************************
\fn     void PMTU_SetPmtuDiscovery(void)
\brief  Interface function for the Path MTU discovery module.

\param  [in]    pDestIpAddr     Pointer to the destination IP address.
\param  [in]    enabled         TRUE/FALSE - Enables/Disables Path MTU discovery for this path.
***************************************************************************************************/
void PMTU_SetPmtuDiscovery(ipAddr_t *pDestIpAddr, bool_t enabled);

/*!*************************************************************************************************
\fn     void PMTU_SetPmtuIncreaseInterval(uint32_t increaseInterval)
\brief  Interface function for the Path MTU discovery module.

\param  [in]    increaseInterval    Pointer to the destination IP address.
***************************************************************************************************/
void PMTU_SetPmtuIncreaseInterval(uint32_t increaseInterval);

#endif /* PMTU_MANAGEMENT_FUNCTIONS_ENABLED */

#if PMTU_CALLBACKS_ENABLED

/*!*************************************************************************************************
\fn     void PMTU_GetPmtu(void)
\brief  Interface function for the Path MTU discovery module.

\return uint32_t    Value of the PMTU for the specified path.
***************************************************************************************************/
void PMTU_RegisterCallbacks(pmtuNotifyDroppedPktCb_t droppedPktCb, pmtuNotifyIncreasePmtuCb_t increasePmtuCb);

#endif /* PMTU_CALLBACKS_ENABLED */

/*!*************************************************************************************************
\fn     void PMTU_TimerHandler(uint32_t timerPeriodInSec)
\brief  Path MTU Discovery periodical timer handler.

\param  [in]    timerPeriodInSec      The interval the timer is triggered.
***************************************************************************************************/
void PMTU_TimerHandler(uint32_t timerPeriodInSec);

#endif /* ND_ENABLED */

#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /* _PMTU_DISCOVERY_H */

