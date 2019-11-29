/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _MLE_SECURITY_H
#define _MLE_SECURITY_H
/*!=================================================================================================
\file       mle_security.h
\brief      This is a header file for the Mesh Link Establishment security module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

/* Network Includes */
#include "network_utils.h"

#include "mle_cfg.h"
#include "mle.h"

/*! extern variable declarations */
extern uint32_t            mMleOutgoingSecFrameCounter;
extern uint32_t            mMacOutgoingSecFrameCounter;
extern const uint32_t      mMleNvmSaveThreshold;

/*==================================================================================================
Public macros
==================================================================================================*/
#if MLE_SECURITY_ENABLED
/*! Maximum number of MLE key descriptor table entries.
    The last two key entries are used for Mesh Cop KEK key and Discovery Key. */
#ifndef MLE_KEY_DESCRIPTOR_TABLE_SIZE
#define MLE_KEY_DESCRIPTOR_TABLE_SIZE     (gNumKeyTableEntries_c - 2)
#endif

/*! Macro for MLE security level */
#define MLE_SECURITY_LEVEL                          5

/*! Macro for MLE key identifier mode 1 */
#define MLE_KEY_ID_MODE1                             1
/*! Macro for MLE key identifier mode 2 */
#define MLE_KEY_ID_MODE2                             2
#endif /* MLE_SECURITY_ENABLED */

/*! Macro for getting MLE key entry by index */
#define MLE_GetKeyDescEntryByIndex(keyIndex, bAcceptFreeEntry) MLE_GetKeyDescEntryExt(keyIndex, 0, bAcceptFreeEntry)
/*! Macro for getting MLE key entry by sequence */
#define MLE_GetKeyDescEntryBySeq(keySeqNum, bAcceptFreeEntry) MLE_GetKeyDescEntryExt(0, keySeqNum, bAcceptFreeEntry)
/*==================================================================================================
Public type definitions
==================================================================================================*/
#if MLE_SECURITY_ENABLED
/*! MLE key descriptor structure */
typedef struct mleKeyDescriptor_tag
{
    uint8_t     key[16];         /*!< MLE key */
    uint32_t    keySource;       /*!< MLE key source */
    uint8_t     keyIndex;        /*!< MLE key index */
    bool_t      bInUse;          /*!< Flag that indicates if the entry is used or not */
}mleKeyDescriptor_t;
#endif /* MLE_SECURITY_ENABLED */

/*! Prototype of the function called by MLE to obtain extended address of a neighbor identified by IP address */
typedef bool_t (*mleNeighborGetExtendedAddr_t)(ipAddr_t *pIpAddr, uint8_t *pExtendedAddress);

/*! Prototype of the function called by MLE to obtain frameCounter of a neighbor identified by IP address */
typedef bool_t (*mleNeighborGetFrameCounter_t)(ipAddr_t *pIpAddr, uint32_t *pFrameCounter, uint8_t keyIndex);

/*! Prototype of the function called by MLE to update frameCounter of a neighbor identified by IP address */
typedef bool_t (*mleNeighborSetFrameCounter_t)(ipAddr_t *pIpAddr, uint32_t newFrameCounter, uint8_t keyIndex);

/*! MLE neighbor functions structure */
typedef struct mleNeighborFunctions_tag
{
    mleNeighborGetExtendedAddr_t    getExtendedAddress;     /*!< Get extended address */
    mleNeighborGetFrameCounter_t    getFrameCounter;        /*!< Get frame counter */
    mleNeighborSetFrameCounter_t    setFrameCounter;        /*!< Set frame counter */
}mleNeighborFunctions_t;

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
\fn     void MLE_RegisterNeighborFunctions(mleNeighborFunctions_t *pMleNeighborFunctions)
\brief  Public interface function for MLE security module. This function sets the callbacks used
        by MLE security to access a neighbor table from the upper layer.

\param  [in]        pMleNeighborFunctions   Pointer to the function pointers structure
***************************************************************************************************/
void MLE_RegisterNeighborFunctions(mleNeighborFunctions_t *pMleNeighborFunctions);

/*!*************************************************************************************************
\fn     uint8_t MLE_CreateKeyDesc(uint8_t *pKey, uint8_t keySeqNum)
\brief  Public interface function for MLE security module. This function creates a key descriptor.

\param  [in]        pKey                Pointer to the MLE key
\param  [in]        keySeqNum           Key sequence number

\return             uint8_t             Index in the mMleKeyDescriptorTable of the Key descriptor
                                        created, 0xFF if key descriptor was not created
***************************************************************************************************/
uint8_t MLE_CreateKeyDesc(uint8_t *pKey, uint32_t keySeqNum);

/*!*************************************************************************************************
\fn     void MLE_GetFrameCounter(instanceId_t instanceId)
\brief  Gets the MLE active frame counter.

\param  [in]    instanceId  Instance id.

\return         uint32_t    MLE Frame Counter
***************************************************************************************************/
uint32_t MLE_GetFrameCounter(instanceId_t instanceId);

/*!*************************************************************************************************
\fn     uint8_t MLE_GetKeyDescIndex(uint8_t keyIndex)
\brief  Public interface function for MLE security module. This function returns the index
        in the mMleKeyDescriptorTable of the key descriptor with the given key index.

\param  [in]        keyIndex        Key index associated with the MLE key

\return             uint8_t         Index in the mMleKeyDescriptorTable of the Key descriptor,
                                    gThrStatus_EntryNotFound_c if key descriptor not found
***************************************************************************************************/
uint8_t MLE_GetKeyDescIndex(uint8_t keyIndex);

/*!*************************************************************************************************
\fn     void MLE_GetActiveKeyIndex(void)
\brief  Gets the MLE active key index.

\return         uint8_t     MLE Active Key Index, 0 otherwise
***************************************************************************************************/
uint8_t MLE_GetActiveKeyIndex(void);

/*!*************************************************************************************************
\fn     bool_t MLE_ActivateKeyDesc(uint32_t keySeqNum)
\brief  Public interface function for MLE security module. This function activates a key descriptor.

\param  [in]        keySeqNum     Key sequence number

\return             bool_t        TRUE if key descriptor was activated, FALSE otherwise
***************************************************************************************************/
bool_t MLE_ActivateKeyDesc(uint32_t keySeqNum);

/*!*************************************************************************************************
\fn     uint8_t *MLE_MsgAlloc(uint8_t txOptions, mleKeyIdModeType_t keyIdMode, uint16_t *pMsgSize,
                              uint16_t tlvsSize, uint8_t **ppTLVs)
\brief  Public interface function for MLE security module. This function allocates buffer for a
        MLE message.

\param  [in]        txOptions       The MLE TX options
\param  [in]        keyIdMode       Security Key Id mode
\param  [out]       pMsgSize        Pointer to the MLE message size
\param  [in]        tlvsSize        The TLVs size
\param  [out]       ppTLVs          Double pointer to the address where TLVs starts

\return             uint8_t *       Pointer to the new allocated buffer for MLE message
***************************************************************************************************/
uint8_t* MLE_MsgAlloc(uint8_t txOptions, mleKeyIdModeType_t keyIdMode, uint16_t *pMsgSize,
                      uint16_t tlvsSize, uint8_t **ppTLVs);

/*!*************************************************************************************************
\fn     bool_t MLE_SecureTxMessage(uint8_t* pTxMessage, uint32_t txMessageLength, ipAddr_t* pSrcIpAddr,
                                   ipAddr_t* pDstIpAddr,uint8_t* pLocalExtendedAddress, bool_t bSecured)
\brief  Public interface function for MLE security module. This function secures a MLE message to be
        transmitted, only if requested. Otherwise this function does nothing.

\param  [in]        pTxMessage                  Pointer to the MLE message to be transmitted
\param  [in]        txMessageLength             MLE message length
\param  [in]        pSrcIpAddr                  Pointer to the source IP address
\param  [in]        pDstIpAddr                  Pointer to the destination IP address
\param  [in]        pLocalExtendedAddress       Pointer to the local extended address
\param  [in]        txOptions                   Flag that indicates if the MLE message will be secured or not

\return             bool_t                      TRUE if MLE message was successfully secured,
                                                FALSE otherwise
***************************************************************************************************/
bool_t MLE_SecureTxMessage(uint8_t *pTxMessage, uint32_t txMessageLength, ipAddr_t *pSrcIpAddr,
                           ipAddr_t *pDstIpAddr, uint8_t *pLocalExtendedAddress, uint8_t txOptions);

/*!*************************************************************************************************
\fn     bool_t MLE_UnsecureRxMessage(uint8_t *pRxMessage, uint8_t **ppRxData, uint32_t *pRxMessageLength,
                                     ipAddr_t *pSrcIpAddr, ipAddr_t *pDstIpAddr, bool_t *pbSecured,
                                     ipIfUniqueId_t ifUniqueId, uint32_t *pRxKeySeqNb)
\brief  Public interface function for MLE security module. This function unsecures a received MLE message
        only if it is secured.

\param  [in]        pRxMessage               Pointer to the received MLE message
\param  [out]       ppData                   Pointer to the address where starts unsecured MLE data
\param  [in, out]   pRxMessageLength         Pointer to the MLE message size
\param  [in]        pSrcIpAddr               Pointer to the source IP address
\param  [in]        pDstIpAddr               Pointer to the destination IP address
\param  [out]       pbSecured                Pointer to a flag that indicates if the MLE message was
                                             secured or not
\param  [in]        ifUniqueId               Interface unique ID
\param  [out]       pRxKeySeqNb              Pointer to received MLE key sequence number

\return             bool_t                   TRUE if MLE message was successfully unsecured,
                                             FALSE otherwise
***************************************************************************************************/
bool_t MLE_UnsecureRxMessage(uint8_t *pRxMessage, uint8_t **ppRxData, uint32_t *pRxMessageLength,
                             ipAddr_t *pSrcIpAddr, ipAddr_t *pDstIpAddr, bool_t *pbSecured,
                             ipIfUniqueId_t ifUniqueId, uint32_t *pRxKeySeqNb);

/*!*************************************************************************************************
\fn     mleKeyDescriptor_t *MLE_GetKeyDescEntryExt(uint8_t keyIndex, uint32_t keySeqNum,
                                                   bool_t bAcceptFreeEntry)
\brief  Returns a requested key descriptor. If after keyIndex if keyIndex!=0; otherwise search
        key sequence counter.

\param  [in]    keyIndex                Key index of the requested key descriptor
\param  [in]    keySeqNum               Key Sequence counter
\param  [in]    bAcceptFreeEntry        Flag that indicates if the first free entry must be returned
                                        when keyIndex is not found in key descriptor table

\return         mleKeyDescriptor_t *    Pointer to the requested key descriptor
***************************************************************************************************/
mleKeyDescriptor_t* MLE_GetKeyDescEntryExt(uint8_t keyIndex, uint32_t keySeqNum, bool_t  bAcceptFreeEntry);

/*!*************************************************************************************************
\fn     uint8_t MLE_GetHeaderSize(uint8_t txOptions, uint8_t keyIdMode)
\brief  Returns MLE header size (the header contains all the bytes before TLVs).

\param  [in]    txOptions      MLE tx options
\param  [in]    keyIdMode      Key ID mode

\return         uint8_t        MLE header size
***************************************************************************************************/
uint8_t MLE_GetHeaderSize(uint8_t txOptions, uint8_t keyIdMode);

#ifdef __cplusplus
}
#endif

/*================================================================================================*/

#endif  /* _MLE_SECURITY_H */
