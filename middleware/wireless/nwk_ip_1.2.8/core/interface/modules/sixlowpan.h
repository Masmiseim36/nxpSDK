/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

 /*! *********************************************************************************
 * \defgroup SIXLOWPAN Interface
 * @{
 ***********************************************************************************/
#ifndef _SIXLOWPAN_H
#define _SIXLOWPAN_H

/*!
\file       sixlowpan.h
\brief      This is a header file for the 6LoWPAN module. 
*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"

#include "TimersManager.h"

#include "stack_config.h"

#include "network_utils.h"
#include "sixlowpan_interface.h"
#include "sixlowpan_ib.h"


/*==================================================================================================
Public macros
==================================================================================================*/

/*==================================================================================================
Public type definitions
==================================================================================================*/

typedef enum
{
    gAdpTransmissionTypeUnicast_c,                  /*!< Unicast trasmission */
    gAdpTransmissionTypeMulticast_c,                /*!< Multicast transmission */
    gAdpTransmissionTypeBroadcast_c                 /*!< Broadcast transmission */
} transmissionType_t;

typedef struct fragmentInfo_tag
{
    uint32_t        datagramOffsetInBytes;
    uint16_t        fragNumber;                     /* Fragment number. Starts from 1. */
    uint16_t        datagramTag;                    /* Datagram TAG */
} fragmentInfo_t;

typedef struct compressionInfo_tag
{
    uint8_t *           pUncompData;                /* Pointer to remaining Uncompressed Data
                                                       from AdpPayload */
    nwkBuffer_t*        pUncompDataNwkBuff;         /* Pointer to remaining Uncompressed Data NWK 
                                                       buffer from AdpPayload */
    uint8_t *           pCompHeadPos;               /* Pointer to compression header last position */
    uint32_t            uncompDataOffset;           /* Offset from the beginning of the packet of 
                                                       the uncompressed data */                                
} compressionInfo_t;

typedef struct dispatchInfo_tag
{
    uint8_t type;
    uint8_t command;
} dispatchInfo_t;

typedef struct slwpPktInfo_tag
{
    ipAddr_t                ipDstAddr;              /*!< Destination (Next Hop) IP Address
                                                      If Not in6addr_any then the IP stack provides the next hop IP address.
                                                      If in6addr_any then 6LoWPAN computes the next hop address from the IP header.*/
    instanceId_t            instanceId;             /* 6LoWPAN instance Id */
    macAbsMcpsDataReq_t *   pMcpsDataReq;           /* Pointer to McpsDataReq structure that will be passed to MAC */
    compressionInfo_t       compressInfo;           /* Compression information */
    fragmentInfo_t          fragInfo;               /* Fragment information */
    dispatchInfo_t          dispatchInfo;           /* Dispatch information */ /* TBD: maybe remove it? */
    transmissionType_t      transmissionType;       /* Unicast / Multicast / Broadcast */
    adpPacketType_t         packetType;             /* Type of Packet: IPv6, LBP, Mesh Routing, etc */
    uint32_t                txTimestamp;            /* Timestamp of MAC transmission */
    uint16_t                adpPayloadSize;
    nwkBuffer_t             adpPayload;             /* Original ADP Payload from upper layers
                                                       (IPv6, LBP, MESH) */
    nwkBuffer_t             meshBCastHeader;        /* Mesh & Broadcast Header */
    nwkBuffer_t             compressedHeader;       /* Compressed Header */
    uint8_t                 meshHops;               /* hop limit for mesh header if 0 take from Ib */
    uint8_t                 nsduHandle;             /* nsduHandle for the ADP packet */
    uint8_t                 prevMsduHandle;         /* previous msdu handle for indirect fragmented packets  */
    uint8_t                 retriesNb;              /* number of 6LoWPAN retries performed so far */
    uint8_t                 channel;                /* Channel on which the frame will be transmited. Ignore if set to 0. */
    adpQos_t                pktPriority;            /* Packet priority */
    uint8_t                 meshBCastHdrPay[20];    /* RFC4944_MAX_MESH_HEADER_SIZE + RFC4944_BROACAST_HEADER_SIZE*/
} slwpPktInfo_t;

typedef struct slwpStruct_tag
{
    instanceId_t        macInstanceId;
    instanceId_t        ifInstanceId;
    macAbsRequests_t *  pMacAbsReq;
    adpIb_t *           pAdpIb;
    adpdCb_t *          pAdpdCb;
    adpmCb_t *          pAdpmCb;
    getRfdType_t        pfGetRfdType;
    getNextReducedDev_t pfGetNextReducedEndDev;
    checkRfdMcast_t  pfCheckRfdMcast;
    checkAddrCompression  pfCheckAddressCompression;
#if (SLWPCFG_LBP_ENABLED)
    lbpCb_t *           pLbpCb;
#endif
} slwpStruct_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

extern taskMsgQueue_t  mSlwpMsgQueue;
extern taskMsgQueue_t* pSlwpMsgQueue;

extern slwpStruct_t * mpSlwpStruct[];


/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void SLWP_Task(void const *pArgument)
\brief  6LoWPAN layer task

\param  [in]    pArgument    Task private data
***************************************************************************************************/
void SLWP_Task(osaTaskParam_t argument);

/*!*************************************************************************************************
\fn     void SLWP_TaskMsgQueueInit(void)
\brief  6LoWPAN layer task message queue initializer.

\param  [in]    pArgument    Task private data
***************************************************************************************************/
taskMsgQueue_t* SLWP_TaskMsgQueueInit(void);

/*!*************************************************************************************************
\fn     void SLWP_HandleAdpdDataReq(uint8_t * pPayload)
\brief  Function to handle the ADPD Data Request message.

\param  [in]    pPayload       Pointer to the ADPD Data Request structure.
***************************************************************************************************/
void SLWP_HandleAdpdDataReq(uint8_t * pPayload);

/*!*************************************************************************************************
\fn     void SLWP_HandleMcpsDataCnf(uint8_t *pPayload)
\brief  Function used to handle the received MCPS Data Confirmation.

\param  [in]    pPayload    Pointer to the received MCPS Data Confirmation.
***************************************************************************************************/
void SLWP_HandleMcpsDataCnf(uint8_t * pPayload);

/*!*************************************************************************************************
\fn     void SLWP_HandleMcpsDataInd(uint8_t *pPayload)
\brief  Function used to handle the received MCPS Data Indication.

\param  [in]    pPayload    Pointer to the received MCPS Data Indication.
***************************************************************************************************/
void SLWP_HandleMcpsDataInd(uint8_t * pPayload);

/*!*************************************************************************************************
\fn     void SWLP_HandleMlmePollIndCB(void * pPayload)
\brief  Function used to handle the received Poll Indication.

\param  [in]    pPayload    Pointer to the received Poll Indication.
***************************************************************************************************/
void SWLP_HandleMlmePollIndCB(void * pPayload);

/*!*************************************************************************************************
\fn     void SLWP_FreePktInfo(slwpPktInfo_t * pSlwpPktInfo)
\brief  Function used to free the 6LoWPAN packet information.

\param  [in]    pSlwpPktInfo      Pointer to the 6LoWPAN packet information structure.
***************************************************************************************************/
void SLWP_FreePktInfo(slwpPktInfo_t * pSlwpPktInfo);

/*!*************************************************************************************************
\fn     void SLWP_ForwardPacket(macAbsMcpsDataInd_t * pMcpsDataInd, llAddr_t nextHopAddr)
\brief  Function used to forward a received MCPS Data Indication.

\param  [in]    pMcpsDataInd    Pointer to the received MAC Data Indication.
\param  [in]    nextHopAddr     Link-Layer address of the next hop.
***************************************************************************************************/
void SLWP_ForwardPacket(macAbsMcpsDataInd_t * pMcpsDataInd, llAddr_t nextHopAddr);

/*!*************************************************************************************************
\fn     void SLWP_DropPacket(macAbsMcpsDataInd_t * pMcpsDataInd)
\brief  Function used to free the MCPS Data Indication.

\param  [in]    pMcpsDataInd      Pointer to the MCPS Data Indication structure.
***************************************************************************************************/
void SLWP_DropPacket(macAbsMcpsDataInd_t * pMcpsDataInd);

/*!*************************************************************************************************
\fn     void SLWP_GenerateAdpdDataInd(macAbsMcpsDataInd_t * pMcpsDataInd)
\brief  Function used to generate a ADPD Data Indication from a received MCPS Data Indication.

\param  [in]    pMcpsDataInd  Pointer to the MCPS Data Indication (MUST NOT be NULL)
***************************************************************************************************/
void SLWP_GenerateAdpdDataInd(macAbsMcpsDataInd_t * pMcpsDataInd);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/


#endif  /*_SIXLOWPAN_H */
