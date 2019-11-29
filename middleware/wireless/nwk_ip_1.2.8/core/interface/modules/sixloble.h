/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _SIXLOBLE_H
#define _SIXLOBLE_H
/*!=================================================================================================
\file       sixloble.h
\brief      This is a header file for the 6LoBLE module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"

#include "TimersManager.h"

#include "stack_config.h"

#include "network_utils.h"
#include "ble_abs_types.h"
#include "sixloble_interface.h"
#include "sixlowpan.h"
#include "sixlowpan_ib.h"

/*==================================================================================================
Public macros
==================================================================================================*/



/*==================================================================================================
Public type definitions
==================================================================================================*/

typedef struct sloBlePktInfo_tag
{
    ipAddr_t                ipDstAddr;              /*!< Destination (Next Hop) IP Address
                                                      If Not in6addr_any then the IP stack provides the next hop IP address.
                                                      If in6addr_any then 6LoWPAN computes the next hop address from the IP header.*/
    instanceId_t            instanceId;            /* 6LoWPAN instance Id */                                              
    bleDataReq_t *          pBleDataReq;           /* Pointer to BleDataReq structure that will be passed to MAC */
    compressionInfo_t       compressInfo;           /* Compression information */
    transmissionType_t      transmissionType;       /* Unicast / Multicast / Broadcast */
    adpPacketType_t         packetType;             /* Type of Packet: IPv6, LBP, Mesh Routing, etc */    
    uint16_t                adpPayloadSize;
    nwkBuffer_t             adpPayload;             /* Original ADP Payload from upper layers
                                                       (IPv6, LBP, MESH) */
    nwkBuffer_t             compressedHeader;       /*  Compressed Header */
} sloBlePktInfo_t;


/*==================================================================================================
Public global variables declarations
==================================================================================================*/

extern taskMsgQueue_t  mSlwpMsgQueue;
extern taskMsgQueue_t* pSlwpMsgQueue;

extern sloBleStruct_t * mpSloBleStruct;
/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void SixLoBle_HandleAdpdDataReq(void * pPayload)
\brief  Function to handle the ADPD Data Request message.

\param  [in]    pPayload       Pointer to the ADPD Data Request structure.
***************************************************************************************************/
void SixLoBle_HandleAdpdDataReq(void * pPayload);

/*!*************************************************************************************************
\fn     void SixLoBle_HandleBleDataInd(void *pPayload)
\brief  Function used to handle the received BLE Data Indication.

\param  [in]    pPayload    Pointer to the received BLE Data Indication.
***************************************************************************************************/
void SixLoBle_HandleBleDataInd(void * pPayload);

/*!*************************************************************************************************
\fn     void SixLoBle_FreePktInfo(slwpPktInfo_t * pSlwpPktInfo)
\brief  Function used to free the 6LoWPAN packet information.

\param  [in]    pSlwpPktInfo      Pointer to the 6Lo packet information structure.
***************************************************************************************************/
void SixLoBle_FreePktInfo(sloBlePktInfo_t *pSloPktInfo);

/*!*************************************************************************************************
\fn     void SixLoBle_DropPacket(bleDataInd_t * pBleDataInd)
\brief  Function used to free the MCPS Data Indication.

\param  [in]    pBleDataInd      Pointer to the MCPS Data Indication structure.
***************************************************************************************************/
void SixLoBle_DropPacket(bleDataInd_t *pBleDataInd);

/*!*************************************************************************************************
\fn     void SixLoBle_GenerateAdpdDataInd(macAbsBleDataInd_t * pBleDataInd)
\brief  Function used to generate a ADPD Data Indication from a received MCPS Data Indication.

\param  [in]    pBleDataInd  Pointer to the MCPS Data Indication (MUST NOT be NULL)
***************************************************************************************************/
void SixLoBle_GenerateAdpdDataInd(bleDataInd_t * pBleDataInd);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/


#endif  /*_SIXLOBLE_H */
