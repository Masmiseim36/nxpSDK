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

#ifndef _DHCP_SERVER_H
#define _DHCP_SERVER_H

/*!
\file       dhcp_server.h
\brief      This is a public header file for the DHCPv4 module. 
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "ip.h"
#include "dhcp.h"

#if DHCP4_SERVER_ENABLED && IP_IP4_ENABLE
/*==================================================================================================
Public macros
==================================================================================================*/

/*==================================================================================================
Public type definitions
==================================================================================================*/

typedef enum
{
    gDhcpAssignStateOffered_c,
    gDhcpAssignStateAssigned_c
} dhcpAssignState_t;

typedef struct dhcpServerData_tag
{
    uint8_t assignState;
    uint8_t clientState;
    uint16_t flags;
    uint32_t xid;
    uint32_t giaddr;
    uint32_t leaseTime;
    uint32_t timestamp;
    uint32_t ipAddr;
    llAddr_t clientHwAddr;
    ipIfUniqueId_t ifId;
}dhcpServerData_t;

typedef struct dhcpServerCfgStruct_tag
{
    uint32_t serverIpAddr;
    uint32_t serverSubNetMask;
    uint32_t startIpAddr;
    uint32_t endIpAddr;
    uint32_t lastAssignedIpAddr;
    ipIfUniqueId_t ifId;
}dhcpServerCfgStruct_t;


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
\fn     nwkStatus_t DHCP4_Server_Start(uint32_t serverIpAddr, uint32_t startIpAddr,
                                       uint32_t endIpAddr, ipIfUniqueId_t ifId)
\brief  This function starts the DHCP Server module.

\param  [in]  serverIpAddr  Server's IP address.
\param  [in]  startIpAddr   Start IP address from IP address pool.
\param  [in]  endIpAddr     End IP address from IP address pool.
\param  [in]  ifId          IP interface identifier.

\retval       nwkStatus_t   Status of the operation.
***************************************************************************************************/
nwkStatus_t DHCP4_Server_Start(uint32_t serverIpAddr, uint32_t startIpAddr, uint32_t endIpAddr,
    ipIfUniqueId_t ifId);

/*!*************************************************************************************************
\fn     void DHCP4_Server_Init(taskMsgQueue_t *pTaskMsgQueue)
\brief  This function initializes the DHCP Server module.

\param  [in]  pTaskMsgQueue  Pointer to the task message queue.
***************************************************************************************************/
void DHCP4_Server_Init(taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn     void DHCP_Server_Service(void* pInData)
\brief  This function parses in incoming DHCP message.

\param  [in]  pInData  Pointer to the incoming DHCP packet.
***************************************************************************************************/
void DHCP_Server_Service(void* pInData);

/*!*************************************************************************************************
\fn     void DHCP_Server_TimerMng(uint32_t timerInterval)
\brief  This function manages the on timer operations.

\param  [in]  timerInterval  The timer interval in milliseconds.
***************************************************************************************************/
void DHCP_Server_TimerMng(uint32_t timerInterval);

#ifdef __cplusplus
}
#endif
#endif
/*================================================================================================*/
#endif  /*_DHCP_SERVER_H */
