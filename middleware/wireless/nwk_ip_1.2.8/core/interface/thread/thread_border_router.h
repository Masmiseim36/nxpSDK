/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
 * \file thread_border_router.h
 * This is a private header file holding border router specific functions.
*/

#ifndef _THREAD_BORDER_ROUTER_H
#define _THREAD_BORDER_ROUTER_H

/*!
 * \file thread_border_router.h
 * This is a private header file holding border router specific functions.
*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

/* Coap */
#include "coap.h"

/* Framework includes */
#include "FunctionLib.h"

/* Thread */
#include "thread_types.h"

/* Network Includes */
#include "network_utils.h"
#include "ip.h"
#include "ip6.h"

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
\fn     uint32_t THR_BR_GetDuaTblSize()
\brief  Returns the size of the Border Router DUA Table

\param [in]    thrInstanceID    Thread instance ID

\return        uint32_t         Size of Border Router DUA Table
***************************************************************************************************/
uint32_t THR_BR_GetDuaTblSize();

/*!*************************************************************************************************
\fn     uint32_t THR_BR_GetMproxyTblSize()
\brief  Returns the size of the Border Router Multicast proxy Table

\return   uint32_t         Size of Multicast proxy Table
***************************************************************************************************/
uint32_t THR_BR_GetMproxyTblSize();

/*!*************************************************************************************************
\fn     thrMcastFwTblEntry_t* THR_BR_GetMproxyTblEntry(uint32_t iCount)
\brief  Returns the entry of index iCount of the Border Router Multicast proxy Table

\param [in] iCount                        Index in Multicast proxy table

\return     thrMcastFwTblEntry_t*  Pointer to Multicast proxy table entry
***************************************************************************************************/
thrMcastFwTblEntry_t *THR_BR_GetMproxyTblEntry(uint32_t iCount);

/*!*************************************************************************************************
\fn    bool_t THR_BR_ConfigBbrServiceData(instanceId_t thrInstId,serviceSet_t *pServiceSet)
\brief This function configures the BBR service TLV in the Nwk Data

\param [in]  thrInstId         Thread instance id
\param [in]  pServiceSet       Pointer to current NWK Data Service Set or NULL

\retval      bool_t            Status - configuration was successful if TRUE
***************************************************************************************************/
bool_t THR_BR_ConfigBbrServiceData(instanceId_t thrInstId, serviceSet_t *pServiceSet);

/*!*************************************************************************************************
\fn    void THR_BR_BbrRegisterDua(instanceId_t thrInstId)
\brief This function configures DUA address on Primary BBR

\param [in]  thrInstId         Thread instance id
***************************************************************************************************/
void THR_BR_BbrRegisterDua(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn    thrStatus_t THR_BR_RegisterBbrCoapServices(instanceId_t thrInstId)
\brief This function enables Multicast Listener Report service on the Backbone Border Router

\param [in]  thrInstId         Thread instance id

\retval      thrStatus_t       Status
***************************************************************************************************/
thrStatus_t THR_BR_RegisterBbrCoapServices(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn    static void THR_BR_MulticastReqMsgCb(coapSessionStatus_t sessionStatus, uint8_t *pData,
       coapSession_t *pSession, uint32_t dataLen)
\brief This function handles MLR.req messages towards the BBR and responds with MLR.rsp

\param [in]    sessionStatus           Coap session status
\param [in]    pData                   Pointer to the payload received
\param [in]    pSession                Pointer to the coap session
\param [in]    dataLen                 Length of the payload received
***************************************************************************************************/
void THR_BR_MulticastReqMsgCb(coapSessionStatus_t sessionStatus, uint8_t *pData,
                              coapSession_t *pSession, uint32_t dataLen);

/*!*************************************************************************************************
\fn    void THR_BR_HandleNdProxyFw(instanceId_t thrInstId, ipPktInfo_t *pIpPktInfo)
\brief This function handles forwarding of ND proxy destination packets to and from the Thread NWK

\param [in]    thrInstId            Thread instance
\param [in]    pIpPktInfo           Ip packet that needs to be forwarded

\retval         TRUE                The packet will not be send now, but when the address query
                                    mechanism will be completed.
\retval         FALSE               The packet is send on the external interface
***************************************************************************************************/
bool_t THR_BR_HandleNdProxyFw(instanceId_t thrInstId, ipPktInfo_t *pIpPktInfo);

/*!*************************************************************************************************
\fn    bool_t THR_BR_BH_NdProxyCb(ifHandle_t ifHandle, ipAdd_t *pTargetAddr, uint8_t naFlags)
\brief This function is a callback for ND Proxy service running on BBR for DUA addresses

\param [in]  ifHandle          Receiving interface handle
\param [in]  pTargetAddr       Target Ip Address from NS packet
\param [in]  naFlags           NA flags that will be used for response if we proxy the pTargetAddr

\retval      bool_t            TRUE if target address is an address we are proxying for
***************************************************************************************************/
bool_t THR_BR_BH_NdProxyCb(ifHandle_t ifHandle, ipAddr_t *pTargetAddr, uint8_t naFlags);

/*!*************************************************************************************************
\fn    void THR_BR_MulticastProxyExpire(instanceId_t thrInstId)
\brief This function handles the expiration of multicast addresses that the PBBR proxies

\param [in]  thrInstId          Thread instance ID
***************************************************************************************************/
void THR_BR_MulticastProxyExpire(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     void THR_BR_UpdateServiceData(instanceId_t thrInstID)
\brief  This function is used to update and resend BBR service data on primary BBR

\param  [in]    thrInstID   Thread instance id.
***************************************************************************************************/
void THR_BR_UpdateServiceData(instanceId_t thrInstID);

/*!*************************************************************************************************
\fn    void THR_BR_ClearPermanentMcastEntries(instanceId_t thrInstId)
\brief This function clears permanent multicast proxy entries(saved in NVM)from Multicast proxy table

\param [in]  thrInstId         Thread instance id
***************************************************************************************************/
void THR_BR_ClearPermanentMcastEntries(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn    thrMcastFwTblEntry_t *THR_BR_ProxyUpdateGroup (instanceId_t thrInstId, ipAddr_t *mcastAddr,
                                               uint32_t timeout);
\brief This function updates the multicast forward table

\param  [in]   thrInstID               Thread instance ID
\param [in]    mcastAddr               Pointer to the updated multicast address
\param [in]    timeout                 Timeout for the multicast group

\retval        thrMcastFwTblEntry_t *  Pointer to thread multicast forward table entry
***************************************************************************************************/
thrMcastFwTblEntry_t *THR_BR_ProxyUpdateGroup (instanceId_t thrInstId, ipAddr_t *mcastAddr,
                                               uint32_t timeout);

/*!*************************************************************************************************
\fn    bool_t THR_BR_Dhcp6NdProxyCb(ifHandle_t ifHandle, ipAdd_t *pTargetAddr, uint8_t naFlags)
\brief This function is a callback for ND Proxy service running on BR for DHCPv6 assigned addresses

\param [in]  ifHandle          Receiving interface handle
\param [in]  pTargetAddr       Target Ip Address from NS packet
\param [in]  naFlags           NA flags that will be used for response if we proxy the pTargetAddr

\retval      bool_t            TRUE if target address is an address we are proxying for
***************************************************************************************************/
bool_t THR_BR_Dhcp6NdProxyCb(ifHandle_t ifHandle, ipAddr_t *pTargetAddr, uint8_t naFlags);

#ifdef __cplusplus
}
#endif

/*================================================================================================*/

#endif  /* _THREAD_BORDER_ROUTER_H */
