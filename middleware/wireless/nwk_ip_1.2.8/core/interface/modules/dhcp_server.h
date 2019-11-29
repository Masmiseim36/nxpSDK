/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
void DHCP4_Server_Init(uint8_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn     void DHCP_Server_Service(void* pInData)
\brief  This function parses in incoming DHCP message.

\param  [in]  pInData  Pointer to the incoming DHCP packet.
***************************************************************************************************/
void DHCP_Server_Service(uint8_t* pInData);

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
