/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
\file       ip_globals.c
\brief      This is the source file that contains parameters for the IP module that can be
            configured by the application 
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "stack_config.h"
#include "ip.h"
#include "ip4.h"
#include "ip6.h"
#include "ip_cfg.h"
#include "ip_if_management.h"
#include "app_thread_config.h"
/*==================================================================================================
Private macros
==================================================================================================*/


/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/

/*==================================================================================================
Private global variables declarations
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
/*! IP Task Message Queue (Default use Thread Task) */
taskMsgQueue_t mIpMsgQueue = {0};
/*! Pointer to IP Task Message Queue */
taskMsgQueue_t *pIpMsgQueue = NULL;

#if TCP_ENABLED
/*! TCP Task Message Queue (Default use Thread Task) */
taskMsgQueue_t mTcpMsgQueue = {0};
/*! Pointer to TCP Task Message Queue */
taskMsgQueue_t *pTcpMsgQueue = NULL;
#endif

#if IP_IP6_ROUTING_ENABLE
/*! Routing Table */
ip6RoutingTblEntry_t *aIp6RoutingTable[IP_IP6_ROUTING_TBL_SIZE+1] = {NULL};
const uint32_t ip6RoutingTableSize = IP_IP6_ROUTING_TBL_SIZE;
#endif

#if IP_IP4_ROUTING_ENABLE
/*! Routing Table */
ip4RoutingTblEntry_t aIp4RoutingTable[IP_IP4_ROUTING_TBL_SIZE] = {0};
const uint32_t ip4RoutingTableSize = IP_IP4_ROUTING_TBL_SIZE;
#endif

#if IP_IP6_MCAST_FW_ENABLE
/* Multicast forwarding table */
ip6McastFwTblEntry_t aIp6McastFwTable[IP_IP6_MCASTFW_TBL_SIZE] = {0};
const uint32_t ip6McastFwTableSize = IP_IP6_MCASTFW_TBL_SIZE;
#endif

#if IP_IP6_FIREWALL_ENABLE
/*! Firewall Table */
ip6FirewallTblEntry_t *aIp6FirewallTbl[IP_IP6_FIREWALL_TBL_SIZE] = {NULL};
const uint32_t ip6FirewallTableSize = IP_IP6_FIREWALL_TBL_SIZE;
#endif

/*! Interface Table */
ipIfStruct_t  aIfTable[IP_IF_NB] = {0};
ipIfStruct_t *aInterfaceTable[IP_IF_NB] = {NULL};
const uint32_t interfaceTableSize = IP_IF_NB;

/*! MLD Enable Interface Table */
uint8_t mldEnabledIfTable[IP_IF_NB] = {0};
const uint32_t mldEnabledIfTableSize = IP_IF_NB;

/*! IPv6 Global Address Table */
ip6IfAddrData_t* aGlobalAddrTable6[IP_IF_IP6_ADDR_NB+1] = {NULL};
const uint32_t globalAddrTable6Size = IP_IF_IP6_ADDR_NB;

/*! Multicast Address Table */
#if IP_IP6_ENABLE
ip6MulticastAddrData_t *aMulticastAddrTable[IP_IF_IP6_MULTICAST_ADDR_NB] = {NULL};
const uint32_t multicastAddrTableSize = IP_IF_IP6_MULTICAST_ADDR_NB;
#endif

/*! IPv4 Global Address Table */
#if IP_IP4_ENABLE
ip4IfAddrData_t* aGlobalAddrTable4[IP_IF_IP4_ADDR_NB] = {NULL};
const uint32_t globalAddrTable4Size = IP_IF_IP4_ADDR_NB;
#endif

/*! Transport Services List */
ipTransportServiceStruct_t *aTransportServiceList[IP_TRANSPORT_SERVICE_NB] = {NULL};
const uint32_t transportServiceListSize = IP_TRANSPORT_SERVICE_NB;

/*! IP Reassembly Queue Size */
const uint32_t ipReassemblyQueueSize = IP_IP_REASSEMBLY_QUEUE_SIZE;

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/

