/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _DHCP6_RELAY_AGENT_H
#define _DHCP6_RELAY_AGENT_H

/*!=================================================================================================
\file       dhcp6_relay_agent.h
\brief      This is a header file for the dhcp6_relay_agent module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include  "dhcp6.h" 

#include  "FunctionLib.h" 
#include  "sockets.h"
#include  "session.h"
#include  "panic.h"
#include  "ip_if_management.h"  
                    

/*==================================================================================================
Public macros
==================================================================================================*/

/*==================================================================================================
Public type definitions
==================================================================================================*/

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
\fn     void DHCP6_RelayAg_Service(uint8_t msgType, void* info, dhcp6Packet_t * pRxDhcp6Pkt,
                                   uint32_t dhcp6PktSize)
\brief  This function parses a received DHCPv6 packet.

\param  [in]  msgType       The type of the incoming message.
\param  [in]  info          Pointer to the data structure.
\param  [in]  pRxDhcp6Pkt   Pointer to the incoming packet.
\param  [in]  dhcp6PktSize  The size of the packet.
***************************************************************************************************/
void DHCP6_RelayAg_Service(uint8_t msgType, void* info, dhcp6Packet_t * pRxDhcp6Pkt,
                           uint32_t dhcp6PktSize);

/*!*************************************************************************************************
\fn     void DHCP6_RelayAg_Init(ifHandle_t* pIfPtr)                                                   
\brief  This function initializes the DHCPv6 Relay Agent on a given interface.

\param  [in]  pIfPtr  Double pointer to the interface.
***************************************************************************************************/
void DHCP6_RelayAg_Init(ifHandle_t* pIfPtr);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _DHCP6_RELAY_AGENT_H */
