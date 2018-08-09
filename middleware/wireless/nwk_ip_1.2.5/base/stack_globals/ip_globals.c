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

#if IP_IP6_ROUTING_ENABLE
/*! Routing Table */
ip6RoutingTblEntry_t *aIp6RoutingTable[IP_IP6_ROUTING_TBL_SIZE+1] = {NULL};
const uint32_t ip6RoutingTableSize = IP_IP6_ROUTING_TBL_SIZE;
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

#if MLD_ENABLED
/*! MLD Enable Interface Table */
uint8_t mldEnabledIfTable[IP_IF_NB] = {0};
const uint32_t mldEnabledIfTableSize = IP_IF_NB;
#endif

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

