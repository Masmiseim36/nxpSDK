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
