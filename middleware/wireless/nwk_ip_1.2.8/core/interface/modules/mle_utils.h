/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _MLE_UTILS_H
#define _MLE_UTILS_H

/*!
\file       mle_utils.h
\brief      This is a header file for the Mesh Link Establishment module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "thread_types.h"
#include "mle.h"
/* Network Includes */
#include "network_utils.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*==================================================================================================
Public type definitions
==================================================================================================*/
/*! Function pointer type declaration for MLE processing functions */
typedef uint32_t (*mleTlvRcvFct_t)(uint8_t *pTlv, list_t *pTlvList);

/*! Function pointer type declaration for MLE processing functions */
typedef nwkStatus_t (*mleTlvAddFct_t)(void *pParams, void * pTlvBuffer, uint16_t *pTlvSize);

/*! Structure which holds the add/receive function pointers for a TLV */
typedef struct mleTlvHandlers_tag
{
    uint32_t        tlvType;        /*!< TLV type */
    mleTlvAddFct_t  pfMleTlvAdd;    /*!< Pointer to MLE TLV add function */
    mleTlvRcvFct_t  pfMleTlvRcv;    /*!< Pointer to MLE TLV receive function */
} mleTlvHandlers_t;

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
\fn     uint8_t *MLE_TlvHeaderAdd(mleTlvType_t tlvType, uint32_t tlvLength, uint8_t *pTlvBufferPos)
\brief  Function that adds a TLV header.

\param  [in]    tlvType          TLV type
\param  [in]    tlvLength        TLV length
\param  [in]    pTlvBufferPos    Pointer to the TLV buffer position

\return         uint8_t *        Pointer to the position in packet after header addition
***************************************************************************************************/
uint8_t *MLE_TlvHeaderAdd(mleTlvType_t tlvType, uint32_t tlvLength, uint8_t *pTlvBufferPos);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvListAdd(list_t *pTlvList, mleTlvType_t tlvType, uint8_t tlvLength,
                                   uint8_t *pTlvValue)
\brief  Function that creates and populates a TLV packet.

\param  [in]    pTlvList        List of TLV to be added
\param  [in]    tlvType         TLV type
\param  [in]    tlvLength       TLV length
\param  [in]    pTlvValue       TLV value

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvListAdd(list_t *pTlvList, mleTlvType_t tlvType, uint8_t tlvLength,
                           uint8_t *pTlvValue);

/*!*************************************************************************************************
\fn     uint32_t MLE_TlvListGetSize(mleSendParams_t *pMleSendParams)
\brief  Function that returns the TLV List size.

\param  [in]    pMleSendParams   Pointer to TLV structure

\return         uint32_t         Length of the TLV list
***************************************************************************************************/
uint32_t MLE_TlvListGetSize(mleSendParams_t *pMleSendParams);

/*!*************************************************************************************************
\fn     void MLE_TlvListCreatePacket(mleSendParams_t *pMleSendParams, uint8_t *pMsg)
\brief  Function that populates the MLE packet with the requested TLVs.

\param  [in]    pMleSendParams   Pointer to TLV structure
\param  [in]    pMsg             Pointer to MLE message
***************************************************************************************************/
void MLE_TlvListCreatePacket(mleSendParams_t *pMleSendParams, uint8_t *pMsg);

/*!*************************************************************************************************
\fn     void MLE_TlvListFree(list_t *pTlvList)
\brief  Function that frees the TLV list.

\param  [in]    pTlvList   Pointer to the TLV list
***************************************************************************************************/
void MLE_TlvListFree(list_t *pTlvList);

/*!*************************************************************************************************
\fn     mleTlvHandlers_t *MLE_SearchTlvHandler(uint8_t tlvType)
\brief  Function that searches for a specified TLV.

\param  [in]    tlvType            The specified TLV type

\return         mleTlvHandlers_t   Pointer to returned TLV handler
***************************************************************************************************/
mleTlvHandlers_t *MLE_SearchTlvHandler(uint8_t tlvType);

#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /* _MLE_UTILS_H */
