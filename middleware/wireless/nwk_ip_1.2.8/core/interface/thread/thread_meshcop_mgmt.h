/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
**  @addtogroup MCOP
**  @{
*/
#ifndef _THREAD_MESHCOP_MGMT_H
#define _THREAD_MESHCOP_MGMT_H

/*!
\file       thread_meshcop_mgmt.h
\brief      This is a header file for the MESHCOP management module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "ip.h"
#include "sockets.h"
#include "coap.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef struct meshcopHandlers_tag meshcopHandlers_t;

/*!*************************************************************************************************
\brief  Callback used by the application to receive TLVs

\param  [in]    pIdHandlerEntry         Pointer to MESHCOP handler
\param  [in]    pTlvs                   Pointer to TLVs location
\param  [in]    tlvsLen                 TLVs length

\return         NONE
***************************************************************************************************/
typedef void (*meshcopHandlerCb_t)(meshcopHandlers_t *pIdHandlerEntry, uint8_t *pTlvs, uint32_t
                                   tlvsLen);

/*! Structure defining the MESHCOP handler */
struct meshcopHandlers_tag
{
    uint32_t id;                        /*!< Callback Id */
    uint32_t secondId;                  /*!< Optional parameter */
    meshcopHandlerCb_t pfCallback;      /*!< Callback used by the application to receive TLVs */
    bool_t keep;                        /*!< Keep or erase handler after the first execution */
    bool_t used;                        /*!< The status of this entry */
};



/*! Structure defining the parameters of MESHCOP_SendNetworkForm */
typedef struct meshcopNwkFormParams_tag
{
    uint8_t instanceId;                 /*!< Thread instance Id */
    uint8_t networkNameSize;            /*!< Network name length */
    uint8_t masterKeySize;              /*!< Master key length */
    uint8_t pskcSize;                   /*!< PSKC length */
    uint8_t *pNwkName;                  /*!< Pointer to network name */
    uint8_t *pMasterKey;                /*!< Pointer to master key */
    uint8_t *pPskc;                     /*!< Pointer to PSKC */
    meshcopHandlerCb_t pfGetCb;         /*!< Pointer to handler function */
    uint8_t channel;                    /*!< Channel */
} meshcopNwkFormParams_t;

/*! Structure defining the parameters used for management commands */
typedef struct meshcopMgmtParams_tag
{
    instanceId_t thrInstId;             /*!< Thread instance Id */
    uint8_t *pTlvs;                     /*!< Pointer to the TLVs to be sent */
    uint32_t tlvsLength;                /*!< Length of the TLVs buffer */
    meshcopHandlerCb_t pfCb;            /*!< Pointer to the callback */
    uint8_t *pDstIpAddr;                /*!< Pointer to the IP of the destination */
} meshcopMgmtParams_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/
/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_Set(instanceId_t thrInstId, uint8_t *pTlvs, uint32_t tlvsLength,
        meshcopHandlerCb_t pfSetCb)
\brief  Function used to do a ManagementSet.

\param  [in]    thrInstId               Thread instance ID
\param  [in]    pTlvs                   Pointer to the start of the TLVs buffer
\param  [in]    tlvsLen                 Length of the TLVs buffer
\param  [in]    pfSetCb                 Pointer to the function which will be called when the
                                        request is completed
\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MESHCOP_Set(instanceId_t thrInstId, uint8_t *pTlvs, uint32_t tlvsLength,
                        meshcopHandlerCb_t pfSetCb);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_Get(instanceId_t thrInstId, uint8_t *pTlvs, uint32_t tlvsLength,
        meshcopHandlerCb_t pfGetCb)
\brief  Function used to do a management get.
\param  [in]    thrInstId               Thread instance ID
\param  [in]    pTlvs                   Pointer to the list of TLV IDs
\param  [in]    tlvsLen                 Length of the TLV IDs list
\param  [in]    pfGetCb                 Pointer to the function which will be called when the
                                        request is completed
\param  [in]    pNeighborIpAddr         Pointer to the specific neighbor IP address(optional)

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MESHCOP_Get(instanceId_t thrInstId, uint8_t *pTlvs, uint32_t tlvsLength,
                        meshcopHandlerCb_t pfGetCb);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_MgmtSendPanIdQuery(instanceId_t thrInstId, uint32_t channelMask,
                                               uint16_t panId, meshcopHandlerCb_t pfHandlerCb,
                                               ipAddr_t *pIpAddr)
\brief  Request to search for Pan ID conflict.

\param  [in]    thrInstId               Thread instance ID
\param  [in]    channelMask             Mask of channels
\param  [in]    panId                   Pan ID
\param  [in]    pfHandlerCb             Pointer to the function which will be called when the
                                        request is completed
\param  [in]    pIpAddr                 Pointer to the IP address of the node which will search for
                                        Pan ID conflict

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MESHCOP_SendNetworkForm(meshcopNwkFormParams_t* pNwkFormParams);

/*!*************************************************************************************************
\fn     void MESHCOP_SendNetworkLeave(ipAddr_t *pDeviceAddr, meshcopHandlerCb_t pfCb)
\brief  Request to do Network Leave.

\param  [in]    pDeviceAddr             Pointer to the device where to send this command
\param  [in]    pfCb                    Pointer to the callback that will inform the user
***************************************************************************************************/
void MESHCOP_SendNetworkLeave(const ipAddr_t *pDeviceAddr, meshcopHandlerCb_t pfCb);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_MgmtSendPanIdQuery(instanceId_t thrInstId, uint32_t channelMask,
                                               uint16_t panId, meshcopHandlerCb_t pfHandlerCb,
                                               ipAddr_t *pIpAddr)
\brief  Request to search for Pan ID conflict.

\param  [in]    thrInstId               Thread instance ID
\param  [in]    channelMask             Mask of channels
\param  [in]    panId                   Pan ID
\param  [in]    pfHandlerCb             Pointer to the function which will be called when the
                                        request is completed
\param  [in]    pIpAddr                 Pointer to the IP address of the node which will search for
                                        Pan ID conflict

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MESHCOP_MgmtSendPanIdQuery(instanceId_t thrInstId, uint32_t channelMask, uint16_t panId,
                                       meshcopHandlerCb_t pfHandlerCb, const ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_MgmtSendEdScan(instanceId_t thrInstId, uint32_t channelMask,
        uint32_t count, uint32_t period, uint32_t scanDuration, meshcopHandlerCb_t pfHandlerCb,
        ipAddr_t *pIpAddr)
\brief  Request to do ED scan.

\param  [in]    thrInstId               Thread instance ID
\param  [in]    channelMask             Mask of channels
\param  [in]    count                   Count
\param  [in]    period                  Period
\param  [in]    scanDuration            Scan duration
\param  [in]    pfHandlerCb             Pointer to the function which will be called when the
                                        request is completed
\param  [in]    pIpAddr                 Pointer to the IP address of the node which will search for
                                        Pan ID conflict

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MESHCOP_MgmtSendEdScan(instanceId_t thrInstId, uint32_t channelMask, uint32_t count,
                                   uint32_t period, uint32_t scanDuration,
                                   meshcopHandlerCb_t pfHandlerCb, ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_MgmtSendAnnounceBegin(instanceId_t thrInstId,
        uint16_t commissionerSessionId, uint32_t channelMask, uint32_t count, uint16_t period,
        ipAddr_t *pIpAddr)
\brief  Request to send a MGMT_ANNOUNCE_BEGIN.ntf

\param  [in]    thrInstId               Thread instance ID
\param  [in]    commissionerSessionId   Commissioner Session ID
\param  [in]    channelMask             Mask of channels
\param  [in]    count                   Count
\param  [in]    period                  Period
\param  [in]    pIpAddr                 Pointer to the IP address of the node which will begin
                                        sending the MGMT_ANNOUNCE.ntf
\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MESHCOP_MgmtSendAnnounceBegin(instanceId_t thrInstId, uint16_t commissionerSessionId,
                                          uint32_t channelMask, uint32_t count, uint16_t period,
                                          ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_MgmtCommSet meshcopMgmtParams_t *pParams)
\brief  Function used to do send a MGMT_COMMISSIONER_SET packet.

\param  [in]    pParams                 Pointer to the input parameters

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MESHCOP_MgmtCommSet(const meshcopMgmtParams_t *pParams);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_MgmtActiveSet( meshcopMgmtParams_t *pParams)
\brief  Function used to send a MGMT_ACTIVE_SET packet.

\param  [in]    pParams                 Pointer to the input parameters

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MESHCOP_MgmtActiveSet(const meshcopMgmtParams_t *pParams);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_MgmtPendingSet(meshcopMgmtParams_t *pParams)
\brief  Function used to send a MGMT_PENDING_SET packet.

\param  [in]    pParams                 Pointer to the input parameters

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MESHCOP_MgmtPendingSet(const meshcopMgmtParams_t *pParams);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_MgmtCommGet(meshcopMgmtParams_t *pParams)
\brief  Function used to send a MGMT_COMMISSIONER_GET packet.

\param  [in]    pParams                 Pointer to the input parameters

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MESHCOP_MgmtCommGet(const meshcopMgmtParams_t *pParams);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_MgmtActiveGet(meshcopMgmtParams_t *pParams)
\brief  Function used to send a MGMT_ACTIVE_GET packet.

\param  [in]    pParams                 Pointer to the input parameters

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MESHCOP_MgmtActiveGet(const meshcopMgmtParams_t *pParams);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_MgmtPendingGet(meshcopMgmtParams_t *pParams)
\brief  Function used to send a MGMT_PENDING_GET packet.

\param  [in]    pParams                 Pointer to the input parameters

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MESHCOP_MgmtPendingGet(const meshcopMgmtParams_t *pParams);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_SendUdpRxNtf(instanceId_t thrInstId)
\brief  This function is used to send UDP_RX.ntf message to commissioner. Used on Boarder Agent.

\param  [in]    pSrcIpAddr              Pointer to source address of the UDP datagram
\param  [in]    pktLength               Packet length
\param  [in]    srcPort                 Source port
\param  [in]    dstPort                 Destination port
\param  [in]    pPayload                Pointer to encapsulated UDP data

\retval         nwkStatus_t             The status of the call
***************************************************************************************************/
nwkStatus_t MESHCOP_SendUdpRxNtf(ipAddr_t *pSrcIpAddr, uint16_t pktLength, uint16_t srcPort,
                                 uint16_t dstPort, void *pPayload);

/*!*************************************************************************************************
\fn     nwkStatus_t MESHCOP_SendUdpTxNtf(instanceId_t thrInstId)
\brief  This function is used to send UDP_TX.ntf message to border agent. Used on commissioner.

\param  [in]    pDstIpAddr              Pointer to destination address of the UDP datagram
\param  [in]    pktLength               Packet length
\param  [in]    srcPort                 Source port
\param  [in]    dstPort                 Destination port
\param  [in]    pPayload                Pointer to encapsulated UDP data

\retval         nwkStatus_t             The status of the call
***************************************************************************************************/
nwkStatus_t MESHCOP_SendUdpTxNtf(ipAddr_t *pDstIpAddr, uint16_t pktLength, uint16_t srcPort,
                                 uint16_t dstPort, void *pPayload);

#endif /* _THREAD_MESHCOP_MGMT_H */

/*!
** @}
*/
