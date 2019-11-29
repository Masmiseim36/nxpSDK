/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _DHCP6_SERVER_H
#define _DHCP6_SERVER_H

/*!
\file       dhcp6_server.h
\brief      This is a public header file for the DHCPv6 Server module. 
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include  "dhcp6.h"
#include  "ip.h"

#if IP_IP6_ENABLE
/*==================================================================================================
Public macros
==================================================================================================*/
#define DHCP6_DUID_LL_LEN          10U

/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum
{
    gDhcp6AssignStateOffered_c,
    gDhcp6AssignStateAssigned_c
} dhcp6AssignState_t;

typedef enum
{
    gDhcp6AddrAssigned_c,
    gDhcp6AddrDeleted_c    
}dhcp6AddrState_t;

typedef struct dhcp6ServerBindingTbl_tag
{
    ipAddr_t ipAddr[DHCP6_MAX_IA_ADDRESSES];
    ipIfUniqueId_t ipIfId;
    uint32_t iaId;
    uint32_t leaseTime;
    uint32_t timestamp;
    uint8_t clientID[DHCP6_MAX_DEVICEID_LEN];
    uint32_t clientMode;
}dhcp6ServerBindingTbl_t;

typedef struct dhcp6ServerStartParams_tag
{
    ipIfUniqueId_t ipIfId;
    dhcp6ServerIpCfg_t** paServerIpCfg;
    ipAddr_t *paDnsCfg;
    uint16_t dnsServersNb;
    uint16_t serverDeviceType;
}dhcp6ServerStartParams_t;

/*! DHCPv6 Server Services Callbacks */
/*!< This defines the function pointers used by DHCPv6 Server module*/
typedef struct dhcp6ServerCallbacks_tag
{
    /*! Pointer to function for the DHCPv6 Server module init */
    void (*pfServerInit)(taskMsgQueue_t *pTaskMsgQueue);    
    /*! Pointer to function for the DHCPv6 Server module start */
    nwkStatus_t (*pfServerStart)(dhcp6ServerStartParams_t *pDhcp6StartParams);    
    /*! Pointer to function for the DHCPv6 Server module stop */
    void (*pfServerStop)(ipIfUniqueId_t ipIfId, ipAddr_t* pPrefix, uint8_t prefixLen);    
    /*! Pointer to function for the DHCPv6 Server  module to assign new address to server, due to DAD failure */
    void (*pfServerAssignNewAddr) (ipAddr_t* pOldServerAddr, ipAddr_t* pNewServerAddr);
} dhcp6ServerCallbacks_t;/*!< Structure for holding pointer to functions used for DHCPv6 modules */

typedef void (*dhcp6ServerAddrAssignedCb_t) (ipAddr_t* pIpAddr, dhcp6AddrState_t addrState, void* pData);
/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern dhcp6ServerCallbacks_t gDhcp6ServerCallbacks;


/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
    extern "C" {
#endif
/*!*************************************************************************************************
\fn     void DHCP6_Server_Init(taskMsgQueue_t *pTaskMsgQueue)
\brief  This function initializes the DHCPv6 Server module.

\param  [in]  pTaskMsgQueue  Pointer to the task message queue.
***************************************************************************************************/
void DHCP6_Server_Init(taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn     nwkStatus_t DHCP6_Server_Start(dhcp6ServerStartParams_t *pDhcp6StartParams)
\brief  This function starts the DHCPv6 Server module.

\param  [in]  pDhcp6StartParams  Pointer to start parameter structure, that contains:
                                   ifHandle         Double pointer to the interface.
                                   paServerIpCfg    Pointer to server's configuration structure.
                                   paDnsCfg         Pointer to DNS servers list.
                                   serverDeviceType The type of device.

\retval       status             TRUE -- The ip address of the DHCP server and the ip address pool
                                         were successfully set
                                 FALSE - An error occurred and the ip addresses were not set.
***************************************************************************************************/
nwkStatus_t DHCP6_Server_Start(dhcp6ServerStartParams_t *pDhcp6StartParams);

/*!*************************************************************************************************
\fn     dhcp6ServerBindingTbl_t * DHCP6_Server_GetClientByIdx(ifHandle_t* pIfHandle, uint32_t index)
\brief  Returns a client from the client table situated at "index" position.

\param  [in]  pIfHandle  Double pointer to the interface.
\param  [in]  index      Index

\return       pDhcp6ClientEntry  Returns the client entry.
****************************************************************************************************/
dhcp6ServerBindingTbl_t * DHCP6_Server_GetClientByIdx(ifHandle_t* pIfHandle, uint32_t index);

/*!*************************************************************************************************
\fn     DHCP6_Server_Stop(ipIfUniqueId_t ipIfId, ipAddr_t* pPrefix, uint8_t prefixLen)
\brief  This function stops the DHCPv6 Server module.

\param  [in]    ipIfId        IP interface identifier
\param  [in]    pPrefix       Pointer to the prefix for which the DHCPv6 will be stopped. 
\param  [in]    prefixLen     Length of the prefix

\retval         none
***************************************************************************************************/
void DHCP6_Server_Stop(ipIfUniqueId_t ipIfId, ipAddr_t* pPrefix, uint8_t prefixLen);

/*!*************************************************************************************************
\fn     void DHCP6_Server_AssignNewAddr(ipAddr_t* pOldServerAddr)
\brief  This function assigns a new address for the DHCPv6 Server, when DAD fails for the current address.

\param  [in]    pOldServerAddr      The address of the DHCPv6 server for which DAD failed.
\param  [out]   pNewServerAddr      The new address of the DHCPv6 server.
\retval         none            
***************************************************************************************************/
void DHCP6_Server_AssignNewAddr(ipAddr_t* pOldServerAddr, ipAddr_t* pNewServerAddr);

/*!*************************************************************************************************
\fn     void DHCP6_Server_RegisterRelayAg(dhcp6RelayAgCb_t pfRelayAgCb, ifHandle_t* pIfPtr)
\brief  This function registers the Relay Agent callback.

\param  [in]  pfRelayAgCb  Pointer to the RelayAgent callback.
\param  [in]  pIfPtr       Double pointer to the interface.
***************************************************************************************************/
void DHCP6_Server_RegisterRelayAg(dhcp6RelayAgCb_t pfRelayAgCb, ifHandle_t* pIfPtr);

/*!*************************************************************************************************
\fn     void DHCP6_Server_Send(dhcp6ClientTempBindingTbl_t * pServerTempTbl, dhcp6Packet_t *pDhcp6Pkt,
                               uint32_t dhcp6MsgSize)
\brief  This function sends a message to lower layers.

\param  [in]  pServerTempTbl  Pointer to client's structure
\param  [in]  pDhcp6Pkt       Pointer the message to be sent
\param  [in]  dhcp6MsgSize    The size of the message
***************************************************************************************************/
void DHCP6_Server_Send(dhcp6ClientTempBindingTbl_t * pServerTempTbl,
                       dhcp6Packet_t *pDhcp6Pkt, uint32_t dhcp6MsgSize);

/*!*************************************************************************************************
\fn     void DHCP6_Server_AddOptionClientID(uint8_t ** pDhcp6Pkt, dhcp6ClientTempBindingTbl_t * pServerTempTbl,
                                            uint32_t* dhcp6MsgSize)
\brief  This function adds the "ClientID" option.

\param  [in]  pDhcp6Pkt       Pointer to the end of the DHC packet.
\param  [in]  pServerTempTbl  Pointer to the server's table.
\param  [in]  dhcp6MsgSize    The size of the message.
***************************************************************************************************/
void DHCP6_Server_AddOptionClientID(uint8_t ** pDhcp6Pkt, dhcp6ClientTempBindingTbl_t * pServerTempTbl,
                                    uint32_t* dhcp6MsgSize);

/*!*************************************************************************************************
\fn     void DHCP6_Server_AddOptionServerID(uint8_t ** pDhcp6Pkt, dhcp6ClientTempBindingTbl_t * pServerTempTbl,
                                            uint32_t* dhcp6MsgSize)
\brief  This function adds the "ServerID" option.

\param  [in]  pDhcp6Pkt       Pointer to the end of the DHC packet.
\param  [in]  pServerTempTbl  Pointer to the server's table.
\param  [in]  dhcp6MsgSize    The size of the message.
***************************************************************************************************/
void DHCP6_Server_AddOptionServerID(uint8_t ** pDhcp6Pkt, dhcp6ServerCfg_t* pServerStruct,
                                    uint32_t* dhcp6MsgSize);
/*!*************************************************************************************************
\fn     void DHCP6_Server_RegisterCallbacks(dhcp6ServerCallbacks_t pfDhcp6ServerCbs)
\brief    This function sets the pointer to DHCP6 Server API function

\param [in]   pfDhcp6ServerCbs    Pointer to structure of function pointers.

\retval       none
***************************************************************************************************/
void DHCP6_Server_RegisterCallbacks(dhcp6ServerCallbacks_t* pfDhcp6ServerCbs);

/*!*************************************************************************************************
\fn     void DHCP6_Server_RegisterAddrAssignCb(dhcp6ServerAddrAssignedCb_t pAddrAssignCb)
\brief    This function sets the pointer to pAddrAssignCb function, that will be called after the client
            has received a valid address from server

\param [in]   pAddrAssignCb    Function pointer

\retval       none
***************************************************************************************************/
void DHCP6_Server_RegisterAddrAssignCb(dhcp6ServerAddrAssignedCb_t pAddrAssignCb);

/*!*************************************************************************************************
\fn     bool_t DHCP6_Server_IsClient(ipAddr_t *pTargetAddr)
\brief  This function returns TRUE if IP Address is assigned to one of server's clients

\param [in]   pTargetAddr    Pointer to IPv6 address

\retval       bool_t         TRUE if address exists in DHCP binding table, FALSE otherwise
***************************************************************************************************/
bool_t DHCP6_Server_IsClient(ipAddr_t *pTargetAddr);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _DHCP6_SERVER_H */
#endif
