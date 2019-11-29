/*! *********************************************************************************
 * \addtogroup NWK_IP
 * @{
 ********************************************************************************** */
/*!
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* rief     This file is a source file for nwk_ip globals declaration.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Framework */
#include "EmbeddedTypes.h"

#include "stack_config.h"

 /* 6Lo */
#include "sixlowpan_tbl.h"
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

/* UDP */
#include "udp_cfg.h"
#include "udp.h"

/* Sockets */
#include "sockets.h"
#include "sockets_cfg.h"
#include "session.h"

/* CoAP */
#include "coap.h"
#include "coap_cfg.h"

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

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
* 6Lo
************************************************************************************/
taskMsgQueue_t   mSlwpMsgQueue = {{0}};
taskMsgQueue_t*  pSlwpMsgQueue = NULL;

const uint32_t sixlowpanInstancesNb = SLWPCFG_INSTANCES_NB;

sloBleStruct_t  *apSloBleStruct = NULL;

/*! Context Table */
ndContextEntry_t* aContextTable[SLWPCFG_RFC6282_CONTEXT_TABLE_SIZE] = {NULL};

/*! Context Table Size */
const uint32_t slwpContextTableSize = SLWPCFG_RFC6282_CONTEXT_TABLE_SIZE;

/************************************************************************************
* IP
************************************************************************************/

taskMsgQueue_t mIpMsgQueue = {{0}};
taskMsgQueue_t *pIpMsgQueue = NULL;

#if IP_IP6_ROUTING_ENABLE
/*! Routing Table */
ip6RoutingTblEntry_t *aIp6RoutingTable[IP_IP6_ROUTING_TBL_SIZE+1] = {NULL};
const uint32_t ip6RoutingTableSize = IP_IP6_ROUTING_TBL_SIZE;
#endif

/*! Interface Table */
ipIfStruct_t  aIfTable[IP_IF_NB + 1] = {0};
ipIfStruct_t *aInterfaceTable[IP_IF_NB + 1] = {NULL};
const uint32_t interfaceTableSize = IP_IF_NB;

/*! IPv6 Global Address Table */
ip6IfAddrData_t* aGlobalAddrTable6[IP_IF_IP6_ADDR_NB+1] = {NULL};
const uint32_t globalAddrTable6Size = IP_IF_IP6_ADDR_NB;

/*! Multicast Address Table */
#if IP_IP6_ENABLE
ip6MulticastAddrData_t *aMulticastAddrTable[IP_IF_IP6_MULTICAST_ADDR_NB] = {NULL};
const uint32_t multicastAddrTableSize = IP_IF_IP6_MULTICAST_ADDR_NB;
#endif

/*! Transport Services List */
ipTransportServiceStruct_t *aTransportServiceList[IP_TRANSPORT_SERVICE_NB] = {NULL};
const uint32_t transportServiceListSize = IP_TRANSPORT_SERVICE_NB;

/*! IP Reassembly Queue Size */
const uint32_t ipReassemblyQueueSize = IP_IP_REASSEMBLY_QUEUE_SIZE;

/************************************************************************************
* ICMP
************************************************************************************/
extern const icmpProtMsgTypeHandler_t gaIcmpProtMsgTypeHandlerTbl6[];
extern const uint8_t gIcmpProtMsgTypeHandlerTbl6Size;

extern const icmpProtMsgTypeHandler_t gaUdpMsgTypeHandlerTbl6[];
extern const uint8_t gUdpMsgTypeHandlerTbl6Size;

extern const icmpProtMsgTypeHandler_t gaShellProtMsgTypeHandlerTbl6[];
extern const uint8_t gShellProtMsgTypeHandlerTbl6Size;

#if ND_ENABLED
    extern const icmpProtMsgTypeHandler_t gaNdIcmpProtMsgTypeHandlerTbl6[];
    extern const uint8_t gNdIcmpProtMsgTypeHandlerTbl6Size;
#endif

#if PMTU_REGISTER_ICMP_PKT_TOO_BIG_HANDLER ||  PMTU_REGISTER_TCP_PKT_TOO_BIG_HANDLER || PMTU_REGISTER_UDP_PKT_TOO_BIG_HANDLER
   extern const icmpProtMsgTypeHandler_t gaPmtuIcmpProtMsgTypeHandlerTbl6[];
   extern const uint8_t gPmtuIcmpProtMsgTypeHandlerTbl6Size;
#endif

/* Register ICMP receive callbacks */
const icmpMsgTypeHandler_t gaIcmpMsgTypeHandlerTable[]=
{
    /*ICMP*/
	{NULL, (icmpProtMsgTypeHandler_t*)&gaIcmpProtMsgTypeHandlerTbl6,IPPROTO_ICMPV6, (uint8_t*)&gIcmpProtMsgTypeHandlerTbl6Size},

	/*UDP*/
	{NULL, (icmpProtMsgTypeHandler_t*)&gaUdpMsgTypeHandlerTbl6, IPPROTO_UDP , (uint8_t*)&gUdpMsgTypeHandlerTbl6Size},

#if ND_ENABLED
	/*ND*/
    {NULL, (icmpProtMsgTypeHandler_t*)&gaNdIcmpProtMsgTypeHandlerTbl6, IPPROTO_ICMPV6 , (uint8_t*)&gNdIcmpProtMsgTypeHandlerTbl6Size},

   #if PMTU_REGISTER_ICMP_PKT_TOO_BIG_HANDLER
	{NULL, (icmpProtMsgTypeHandler_t*)&gaPmtuIcmpProtMsgTypeHandlerTbl6, IPPROTO_ICMPV6 , (uint8_t*)&gPmtuIcmpProtMsgTypeHandlerTbl6Size},
   #endif

   #if PMTU_REGISTER_TCP_PKT_TOO_BIG_HANDLER
	{NULL, (icmpProtMsgTypeHandler_t*)&gaPmtuIcmpProtMsgTypeHandlerTbl6, IPPROTO_TCP , (uint8_t*)&gPmtuIcmpProtMsgTypeHandlerTbl6Size},
   #endif

   #if PMTU_REGISTER_UDP_PKT_TOO_BIG_HANDLER
	{NULL, (icmpProtMsgTypeHandler_t*)&gaPmtuIcmpProtMsgTypeHandlerTbl6, IPPROTO_UDP , (uint8_t*)&gPmtuIcmpProtMsgTypeHandlerTbl6Size},
   #endif

#endif

	/*SHELL_APP OR THCI*/
	{NULL, (icmpProtMsgTypeHandler_t*)&gaShellProtMsgTypeHandlerTbl6, IPPROTO_ICMPV6, (uint8_t*)&gShellProtMsgTypeHandlerTbl6Size},
  
};

const uint8_t gIcmpMsgTypeHandlerSize = sizeof(gaIcmpMsgTypeHandlerTable)/ sizeof(icmpMsgTypeHandler_t);

/************************************************************************************
* ND
************************************************************************************/

/*! ND Configs Table */
ndCfg_t* aNdCfg[IP_IF_NB] = {NULL};
ndRuntimeVariables_t aNdVariables[IP_IF_NB];
/*! ND Configs Table Size */
const uint32_t ndCfgTblSize = IP_IF_NB;
/*! Neighbor Cache */
ndNeighborEntry_t* aNeighborCache[ND_NEIGHBOR_CACHE_SIZE] = {NULL};
/*! Neighbor Cache Size */
const uint32_t ndNeighborCacheSize = ND_NEIGHBOR_CACHE_SIZE;
/*! Prefix List */
ndPrefixEntry_t* aPrefixList[ND_PREFIX_LIST_SIZE] = {NULL};
/*! Prefix List Size */
const uint32_t ndPrefixListSize = ND_PREFIX_LIST_SIZE;
/*! Route Information List Size */
const uint8_t ndRouteInfoListSize = ND_ROUTE_INFO_LIST_SIZE;
/*! Route Information */
ndOptionRouteInfo_t ndRouteInfoOption = {{ 0 }};
/*! Destination Cache */
ndDstCacheEntry_t* aDestinationCache[ND_DESTINATION_CACHE_SIZE] = {NULL};
/*! Destination Cache Size */
const uint32_t ndDestinationCacheSize = ND_DESTINATION_CACHE_SIZE;

const bool_t ndDADTransmits = ND_DAD_TRANSMITS;
const uint8_t ndLifeTimeGuardInterval = ND_LIFETIME_GUARD_INTERVAL;
const uint8_t ndRegAddrGuardTime = ND_REG_ADDRESS_GUARD_TIME;

/************************************************************************************
* UDP
************************************************************************************/

/*! UDP Connections table */
udpConn_t aUdpConnections[MAX_UDP_CONNECTIONS];
const uint32_t udpConnectionsSize = MAX_UDP_CONNECTIONS;
const uint8_t udpHopDefaultLimit = UDP_DEFAULT_HOP_LIMIT;

/************************************************************************************
* Sockets
************************************************************************************/

/*! The list of used sockets for this device */
const uint32_t  gSocketsSize = BSDS_MAX_SOCKETS;
sock_t          gaStaticUdpSockets[BSDS_MAX_SOCKETS] = {{0}};
sessEnt_t       gaSockEntList[BSDS_MAX_SOCKETS];
const uint16_t  gSocketsSelectMaxFds = BSDS_SELECT_MAX_FDS;

/************************************************************************************
* CoAP
************************************************************************************/
/*! CoAP Sessions table */
const uint32_t gCoapMaxSessions = COAP_MAX_SESSIONS;

/* Table of resources and their callbacks */
coapCallbackStruct_t gCoapResourceCbs[COAP_MAX_CALLBACKS];
const uint32_t gCoapMaxCallbacks = COAP_MAX_CALLBACKS;

/* Table of tokens and their callbacks, for non-piggybacked responses */
coapTokenCbStruct_t gCoapTokenCbs[COAP_MAX_NON_PIGGYBACKED_RSP];
const uint32_t gCoapMaxNonPiggybackedResponses = COAP_MAX_NON_PIGGYBACKED_RSP;


coapInstance_t gCoapInstances[COAP_MAX_INSTANCES];
const uint32_t gCoapMaxInstances = COAP_MAX_INSTANCES;

/* Keep track of the last  msg IDs to avoid duplicates */
uint16_t gaCoapLastMsgIds[COAP_MAX_MSG_IDs] = {0};
const uint32_t  gCoapMaxMsgIds = COAP_MAX_MSG_IDs;

const uint32_t gCoapMaxOptions = COAP_MAX_OPTIONS;
const uint32_t gCoapTokenLen = COAP_TOKEN_LENGTH;

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* @}
********************************************************************************** */
