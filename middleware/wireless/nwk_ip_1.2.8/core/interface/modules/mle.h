/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _MLE_H
#define _MLE_H

/*!
\file       mle.h
\brief      This is a header file for the Mesh Link Establishment module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "thread_types.h"

/* Network Includes */
#include "network_utils.h"
#include "sockets.h"

/*==================================================================================================
Public macros
==================================================================================================*/
/* The TX options */
#define MLE_TXOPTIONS_NO_SECURITY   0x00    /*!< No security */
#define MLE_TXOPTIONS_SECURED       0x01    /*!< Secured */
#define MLE_TXOPTIONS_USE_TEMP_KEY  0x02    /*!< Use temporary key */

#define MLE_MSG_HOP_LIMIT           255    /*!< MLE message hop limit */

#define MLE_CMD_TYPE_SIZE           1      /*!< MLE command type size */

#define MLE_TLV_HEADER_SIZE         2      /*!< MLE TLV header size: type + length */

/*! Macro used for extracting the MLE command type from the beginning of a MLE message */
#define MLE_CMD_TYPE(p)             (*((uint8_t *)(p)))

/*==================================================================================================
Public type definitions
==================================================================================================*/
/*! MLE Key Id mode type */
typedef enum
{
    gMleKeyIdMode0_c       = 0x00,    /*!< MLE key ID mode 0 */
    gMleKeyIdMode1_c       = 0x01,    /*!< MLE key ID mode 1 */
    gMleKeyIdMode2_c       = 0x02,    /*!< MLE key ID mode 2 */
    gMleKeyIdMode3_c       = 0x03     /*!< MLE key ID mode 3 */
} mleKeyIdModeType_t;

/*! MLE MAC security flags: keyIDMode and SecurityLevel */
typedef enum mleMacSecFlags_tag
{
    gMleMacSecUnsecured_c    = BSDS_SET_TX_SEC_FLAGS(0, 0), /*!< Default value: unsecured */
    gMleMacSecMode1Level5_c  = BSDS_SET_TX_SEC_FLAGS(1, 5), /*!< MacSec mode 1 level 5  */
    gMleMacSecMode0Level5_c  = BSDS_SET_TX_SEC_FLAGS(0, 5), /*!< MacSec mode 0 level 5  */
    gMleMacSecMode2Level5_c  = BSDS_SET_TX_SEC_FLAGS(2, 5)  /*!< MacSec mode 2 level 5  */
}mleMacSecFlags_t; /*!< MAC security flags */

/*! Structure which contains details about the received MLE packet */
typedef struct mleCallbackParams_tag
{
    ipAddr_t senderIp;           /*!< Sender IP address */
    uint8_t *pMleCommand;        /*!< Pointer to MLE command */
    void *pReceivedBuffer;        /*!< Pointer to received buffer */
    uint32_t rxkeySeqNum;        /*!< Received key sequence number */
    uint16_t tlvDataSize;        /*!< TLV data size */
    uint16_t macSrcPanId;        /*!< Source MAC PAN ID */
    ipIfUniqueId_t ifUniqueId;   /*!< Interface unique ID */
    bool_t secured;              /*!< Flag marking if the request was is secured */
    bool_t mCastReq;             /*!< Flag marking if the request was a multicast packet */
    uint8_t macSecKeyIdMode;     /*!< MacSec key ID mode */
    uint8_t channel;             /*!< Channel */
    uint8_t linkMargin;          /*!< Link margin */
    uint8_t lqi;                 /*!< Link quality indication */
} mleCallbackParams_t;

/*! Function pointer type declaration for the function which is to be registered as a callback */
typedef void (*mleCallback_t)(void *pParams);

/*! MLE Command Type type */
typedef uint8_t mleCmdType_t;

/*! MLE TLV Type type */
typedef uint8_t mleTlvType_t;

/**************/
/* TLV Header */
/**************
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |    Type       |    Length     |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

***************/

/*! TLV Header */
typedef struct tlvHdr_tag
{
    uint8_t type;     /*!< Type */
    uint8_t length;   /*!< Length */
} tlvHeader_t;

/*! TLV Extended Header */
typedef struct tlvExtHdr_tag
{
    uint8_t type;          /*!< Type */
    uint8_t extLenMarker;  /*!< Extended length marker */
    uint8_t length[2];     /*!< Length */
} tlvExtHeader_t;

/*! Neighbor Data Record */
typedef struct mleNeighborRecord_tag
{
    uint8_t  incoming;      /*!< Incoming */
    uint8_t  outgoing;      /*!< Outgoing */
    uint8_t  priority;      /*!< Priority */
    uint8_t  incomingIDR;   /*!< Incoming IDR */
    llAddr_t address;       /*!< Address */
} mleNeighborRecord_t;

/*! Network Parameter TLV Type */
typedef uint16_t nwkParamType_t;

/*! Network Parameter Channel TLV */
typedef struct nwkParamChannel_tag
{
    uint32_t channel;    /*!< Channel */
} nwkParamChannel_t;

/*! Network Parameter PAN Id TLV */
typedef struct nwkParamPanID_tag
{
    uint32_t panId;     /*!< PAN Id */
} nwkParamPanID_t;

/*! Network Parameter Permit Joining TLV */
typedef struct nwkParamPermitJoining_tag
{
    uint32_t permitJoining;    /*!< Permit join */
} nwkParamPermitJoining_t;

/*! Network Parameter Route TLV */
typedef struct mleTlvRoute_tag
{
    mleTlvType_t tlvType;              /*!< TLV Type */
    uint16_t idSequence;               /*!< Id sequence */
    uint16_t nbOfRouters;              /*!< Number of routers */
    uint8_t  aNeighborRouteCosts[];    /*!< Neighbor Route costs */
} mleTlvRoute_t;

/*! MLE TLV Server Data structure */
typedef struct mleTlvServer_tag
{
    mleTlvType_t tlvType;    /*!< TLV Type */
    list_t subTlvList;       /*!< Sub-TLV list */
} mleTlvServerData_t;

/*! MLE data select parameters structure */
typedef struct nwkDataSelectParams_tag
{
   bool_t       onlyStable;         /*!< Only stable flag */
   bool_t       hideStableShort;    /*!< Hide stable short flag */
}nwkDataSelectParams_t;

/*! Structure which contains details about the MLE packet */
typedef struct mleTlvInfo_tag
{
   uint64_t      tlvTypeFlags[2]; /*!< TLV type flags */
   uint8_t*      pTlvReq;         /*!< Pointer to TLV request */
   instanceId_t  instanceId;      /*!< Instance Id */
   void*         pRxChallenge;    /*!< Pointer to received challenge */
   uint8_t*      pTxChallenge;    /*!< Pointer to transmitted challenge */
   void*         pThrNeighbor;    /*!< Pointer to neighbor */
   uint8_t       scanMask;        /*!< Scan mask */
   uint8_t       linkMargin;      /*!< Link margin */
   uint8_t       status;          /*!< Status */
   nwkDataSelectParams_t nwkDataSelectParams;
}mleTlvInfo_t;

/*! MLE send parameters */
typedef struct mleSendParams_tag
{
    ipAddr_t *pIpSrcAddr;         /*!< Pointer to source IP address */
    ipAddr_t *pIpDstAddr;         /*!< Pointer to destination IP address */
    uint32_t delayMs;             /*!< time delay in MS when the MLE packet should be sent */
    uint32_t macTxFlags;          /*!< MAC Key ID Mode and Security Level (see mleMacSecFlags_t) */
    uint8_t txOptions;            /*!< - bit0 set means Tx is secured
                                     - bit1 set means Tx uses a temporary key */
    uint8_t hopLimit;             /*!< the number of hops to be set in the IP packet only for this send */
    mleCmdType_t cmdType;         /*!< The MLE command type */
    mleKeyIdModeType_t keyIdMode; /*!< The Key ID mode */
    uint8_t keySeqNum;            /*!<*/
    ipIfUniqueId_t ifUniqueId;    /*!< Interface Unique Identifier */
    mleTlvInfo_t mleTlvInfo;      /*!< TLV info */
}mleSendParams_t;

/*! MLE TX options */
typedef struct mleTxOptions_tag
{
    ipAddr_t *pIpSrcAddr;         /*!< Pointer to source IP address */
    ipAddr_t *pIpDstAddr;         /*!< Pointer to destination IP address */
    uint32_t delayMs;             /*!< time delay in MS when the MLE packet should be sent */
    uint32_t macTxFlags;          /*!< MAC Key ID Mode and Security Level (see mleMacSecFlags_t) */
    uint8_t txOptions;            /*!< - bit0 set means Tx is secured
                                     - bit1 set means Tx uses a temporary key */
    uint8_t hopLimit;             /*!< the number of hops to be set in the IP packet only for this send */
    mleCmdType_t cmdType;         /*!< The MLE command type */
    mleKeyIdModeType_t keyIdMode; /*!< The Key ID mode */
    uint8_t keySeqNum;            /*!< Key sequence number */
    ipIfUniqueId_t ifUniqueId;    /*!< Interface Unique Identifier */
}mleTxOptions_t;

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
\fn     void MLE_Init(taskMsgQueue_t *pTaskMsgQueue)
\brief  Interface function for the Mesh Link Establishment module. Initializes the module.

\param  [in]    pTaskMsgQueue       Task message queue
***************************************************************************************************/
void MLE_Init(taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn     MLE_Reset
\brief  Reset (clean up) the MLE module.
***************************************************************************************************/
void MLE_Reset(void);

/*!*************************************************************************************************
\fn     void MLE_RegisterCb(mleCallback_t pfMleCallback)
\brief  Interface function for the Mesh Link Establishment module. It registers a callback function
        which will be called when a MLE packet is received.

\param  [in]    pfMleCallback       Pointer to the function that will be called when a MLE packet
                                    is received
***************************************************************************************************/
void MLE_RegisterCb(mleCallback_t pfMleCallback);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvAdd(mleTlvType_t mleTlvType, void *pParams, mleTlvInfo_t *pMleTlvInfo)
\brief  Interface function for the Mesh Link Establishment module. It adds a TLV to a TLV list.

\param  [in]    mleTlvType_t        TLV type
\param  [in]    pParams             Pointer to the TLV parameters
\param  [in]    pMleTlvInfo         Pointer to the MLE TLV info

\return         nwkStatus_t         Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvAdd(mleTlvType_t mleTlvType, void *pParams, mleTlvInfo_t *pMleTlvInfo);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_Send(mleSendParams_t *pMleSendParams)
\brief  Interface function for the Mesh Link Establishment module. It sends a MLE packet.

\param  [in]    pMleSendParams      Pointer to the MLE_Send parameters

\return         nwkStatus_t         Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_Send(mleSendParams_t *pMleSendParams);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_SendTlvs(mleTxOptions_t *pTxOptions, uint16_t tlvsLength, uint8_t* pTLVs)
\brief  Send TLVs using MLE layer.

\param  [in]    pTxOptions       Pointer to MLE transmission options
\param  [in]    tlvsLength       TLVs Length
\param  [in]    pTLVs            Pointer to TLVs

\return         nwkStatus_t      Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_SendTlvs(mleTxOptions_t *pTxOptions, uint16_t tlvsLength, uint8_t* pTLVs);

/*!*************************************************************************************************
\fn     uint8_t *MLE_TlvGetNext(uint8_t *pStart, uint16_t *pRemainingLength)
\brief  MLE function used to get a pointer to the next TLV in a TLV buffer list.

\param  [in]     pStart              Pointer to the start of the buffer list
\param  [in,out] pRemainingLength    Pointer to the length left to parse from the original buffer

\return          uint8_t*            Pointer to the next TLV
***************************************************************************************************/
uint8_t* MLE_TlvGetNext(uint8_t *pStart, uint16_t *pRemainingLength);

/*!*************************************************************************************************
\fn     int32_t MLE_GetSockFd(void)
\brief  Getter for the socket descriptor used for MLE communication.

\return         int32_t   socket
***************************************************************************************************/
int32_t MLE_GetSockFd(void);

#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /* _PMTU_DISCOVERY_H */
