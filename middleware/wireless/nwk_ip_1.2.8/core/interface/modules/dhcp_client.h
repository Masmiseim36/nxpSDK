/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _DHCP_CLIENT_H
#define _DHCP_CLIENT_H

/*!
\file       dhcp_client.h
\brief      This is a public header file for the DHCPv4 Client module. 
*/


/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "ip_if_management.h"
#include "dhcp.h"

#if DHCP4_CLIENT_ENABLED && IP_IP4_ENABLE
/*==================================================================================================
Public macros
==================================================================================================*/

/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef void (*addressAssignCb_t)(uint32_t * pIpAddr);

typedef struct dhcpClientData_tag
{
    uint8_t chaddr[8];
    uint32_t xid;                   /*transaction ID of the last sent request message */
    uint32_t ipAddrAssigned;
    uint32_t ipAddrNextServer;
    uint32_t ipAddrOffered;
    uint32_t subnetMask;
    uint32_t timestamp;
    uint32_t serverID;
    uint32_t serverAddr;
    uint32_t leasetime;
    ipIfUniqueId_t ifId;
    uint8_t retries;                /* number of retries of the current request */
    uint8_t state;
    tmrTimerID_t retryTimer;
    void (*addressAssignCb)(uint32_t * pIpAddr);
} dhcpClientData_t;

typedef void (*dhcp4ClientSetAddrCb_t) (ipIfUniqueId_t ifUniqueId,uint32_t* ipAddr, void* pData);

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
\fn     void DHCP4_Client_Init(taskMsgQueue_t *pTaskMsgQueue)
\brief  This function initialize the DHCP Client module.

\param  [in]  pTaskMsgQueue  Pointer to the task message queue.
***************************************************************************************************/
void DHCP4_Client_Init(taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn     void DHCP4_Client_Start(ipIfUniqueId_t ifId, addressAssignCb_t addressAssignCb)
\brief  This function returns the entry from client table parameters.

\param  [in]  ifId             IP interface identifier.
\param  [in]  addressAssignCb  Callback function.It is called when the client binds to an address.
***************************************************************************************************/
void DHCP4_Client_Start(ipIfUniqueId_t ifId,addressAssignCb_t addressAssignCb);

/*!*************************************************************************************************
\fn     void DHCP_Client_Service(uint8_t* pInData)
\brief  This function parses an incoming DHCP packet.

\param  [in]  pInData  Pointer to the incoming DHCP packet.
***************************************************************************************************/
void DHCP_Client_Service(uint8_t* pInData);

/*!*************************************************************************************************
\fn     void DHCP_Client_TimerMng(uint32_t timerInterval)
\brief  This function manages the on timer operations.

\param  [in]  timerInterval  The timer interval in milliseconds.
***************************************************************************************************/
void DHCP_Client_TimerMng(uint32_t timerInterval);

/*!*************************************************************************************************
\fn     void DHCP4_Client_RegisterSetAddr(dhcp4ClientSetAddrCb_t pfDhcp4ClientSetAddr)
\brief  This function sets the pointer to SetAddr function, that will be called after the client
        has received a valid address from server

\param  [in]   pfDhcp6ClientSetAddr  Function pointer
\param  [out]  none
***************************************************************************************************/
void DHCP4_Client_RegisterSetAddr(dhcp4ClientSetAddrCb_t pfDhcp4ClientSetAddr);

#ifdef __cplusplus
}
#endif
#endif
/*================================================================================================*/
#endif  /*_DHCP_CLIENT_H */
