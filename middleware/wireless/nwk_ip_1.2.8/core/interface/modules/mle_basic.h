/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _MLE_BASIC_H
#define _MLE_BASIC_H

/*!
\file       mle_basic.h
\brief      This is a header file for the Mesh Link Establishment module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

/* Network Includes */
#include "network_utils.h"

#include "mle.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#define MLE_CHALLENGE_LEN 8    /*!< MLE challenge length */

/*==================================================================================================
Public type definitions
==================================================================================================*/
/*! MLE Command Type enumeration */
typedef enum mleCmdType_tag
{
    gMleCmdLinkReq_c            = 0U,    /*!< MLE Link Request */
    gMleCmdLinkAccept_c         = 1U,    /*!< MLE Link Accept */
    gMleCmdLinkAcceptAndReq_c   = 2U,    /*!< MLE Link Accept and Request */
    gMleCmdLinkReject_c         = 3U,    /*!< MLE Link Reject */
    gMleCmdAdvertisement_c      = 4U,    /*!< MLE Advertisement */
    gMleCmdUpdate_c             = 5U,    /*!< MLE Update */
    gMleCmdUpdateReq_c          = 6U,    /*!< MLE Update Request */
} mleCmdType_e;

/*! MLE TLV Type enumeration */
typedef enum mleTlvType_tag
{
    gMleTlvSrcAddr_c            = 0U,    /*!< Source Address TLV */
    gMleTlvMode_c               = 1U,    /*!< Mode TLV */
    gMleTlvTimeout_c            = 2U,    /*!< Timeout TLV */
    gMleTlvChallenge_c          = 3U,    /*!< Challenge TLV */
    gMleTlvResponse_c           = 4U,    /*!< Response TLV */
    gMleTlvLlayerFrameCtr_c     = 5U,    /*!< Link Layer Frame Counter TLV */
#if !STACK_THREAD
    gMleTlvLinkQuality_c        = 6U,    /*!< Link Quality TLV */
    gMleTlvNetworkParam_c       = 7U,    /*!< Network Parameters TLV */
#endif
    gMleTlvMleFrameCtr_c        = 8U,    /*!< MLE Frame Counter TLV */
} mleTlvType_e;

/*! Generic TLV format */
typedef struct mleGeneric_Tlv_tag
{
    uint8_t tlvType;    /*!< Type */
    uint8_t length;     /*!< Length */
    uint8_t pData[];    /*!< Value */
}mleGeneric_Tlv_t;

/**********************/
/* Source Address TLV */
/**********************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   TLV Type    |    Length     |             EUI...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

***********************/

/*! Source Address TLV - Tx Interface structure */
typedef struct mleTlvSrcAddr_tag
{
    mleTlvType_t tlvType;    /*!< Type */
    llAddr_t address;        /*!< Link layer address */
} mleTlvSrcAddr_t;

/*! Source Address TLV - Over the Air mapping structure */
typedef struct mleOtaTlvSrcAddr_tag
{
    uint8_t tlvType;    /*!< Type */
    uint8_t length;     /*!< Length */
    uint8_t eui[2];     /*!< EUI */
} mleOtaTlvSrcAddr_t;

/************/
/* Mode TLV */
/************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   TLV Type    |    Length     |   Capab Info  |S|N|  Reserved |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*************/

/*! Device Capability Information type */
typedef uint8_t devCapabilityInfo_t;

/*! Mode Flags type */
typedef uint8_t modeFlags_t;

/*! Mode Flags bit masks */
typedef enum modeFlags_tag
{
    gModeFlagN_c                    = 0x01U,    /*!< Full Network Data flag */
    gModeFlagD_c                    = 0x02U,    /*!< Device Type FFD(1) or RFD(0) flag */
    gModeFlagS_c                    = 0x04U,    /*!< Secure Data Requests flag */
    gModeFlagR_c                    = 0x08U,    /*!< Receiver On When Idle flag */
    gModeFlagW_c                    = 0x10U     /*!< Wake-on-radio flag */
} modeFlags_e;

/*! Mode TLV - Tx Interface structure */
typedef struct mleTlvMode_tag
{
    mleTlvType_t tlvType;                       /*!< Type */
    uint8_t modeFlags;                          /*!< Mode flags: S (Secure Data Requests)
                                                                 N (Network Data) */
} mleTlvMode_t;

/*! Mode TLV - Over the Air mapping structure */
typedef struct mleOtaTlvMode_tag
{
    uint8_t tlvType;                            /*!< Type */
    uint8_t length;                             /*!< Length */
    uint8_t modeFlags;                          /*!< Mode flags: S (Secure Data Requests)
                                                                 N (Network Data) */
} mleOtaTlvMode_t;

/***************/
/* Timeout TLV */
/***************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   TLV Type    |    Length     |             Timeout           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Timeout             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

****************/

/*! Timeout TLV - Tx Interface structure */
typedef struct mleTlvTimeout_tag
{
    mleTlvType_t tlvType;    /*!< Type */
    uint8_t      length;     /*!< Length */
    uint32_t     timeout;    /*!< Timeout */
} mleTlvTimeout_t;

/*! Timeout TLV - Over the Air mapping structure */
typedef struct mleOtaTlvTimeout_tag
{
    uint8_t tlvType;        /*!< Type */
    uint8_t length;         /*!< Length */
    uint8_t timeout[4];     /*!< Timeout */
} mleOtaTlvTimeout_t;

/*****************/
/* Challenge TLV */
/*****************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   TLV Type    |    Length     |           Challenge ...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

******************/

/*! Challenge TLV - Tx Interface structure*/
typedef struct mleTlvChallenge_tag
{
    mleTlvType_t tlvType;                    /*!< Type */
    uint8_t  challenge[MLE_CHALLENGE_LEN];   /*!< Challenge array */
    uint8_t  size;                           /*!< Size */
} mleTlvChallenge_t;

/*! Challenge TLV - Over the Air mapping structure */
typedef struct mleOtaTlvChallenge_tag
{
    uint8_t tlvType;                         /*!< Type */
    uint8_t length;                          /*!< Length */
    uint8_t challenge[MLE_CHALLENGE_LEN];    /*!< Challenge array */
} mleOtaTlvChallenge_t;

/****************/
/* Response TLV */
/****************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   TLV Type    |    Length     |           Response ...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*****************/

/*! Response TLV - Tx Interface structure */
typedef struct mleTlvResponse_tag
{
    mleTlvType_t tlvType;    /*!< Type */
    uint8_t response[16];    /*!< Response array */
    uint8_t size;            /*!< Size */
} mleTlvResponse_t;

/*! Response TLV - Over the Air mapping structure */
typedef struct mleOtaTlvResponse_tag
{
    uint8_t tlvType;                        /*!< Type */
    uint8_t length;                         /*!< Length */
    uint8_t response[MLE_CHALLENGE_LEN];    /*!< Response array */
} mleOtaTlvResponse_t;

/*********************/
/* Frame Counter TLV */
/*********************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  TLV Type 5   |    Length 1   |          Frame Counter        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|        Frame Counter          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

**********************/

/*! Link Layer Frame Counter TLV - Tx Interface structure */
typedef struct mleTlvLlFrameCtr_tag
{
    mleTlvType_t tlvType;     /*!< Type */
    uint32_t frameCounter;    /*!< Frame counter */
} mleTlvLlFrameCtr_t;

/*! Link Layer Frame Counter TLV - Over the Air mapping structure */
typedef struct mleOtaTlvLlFrameCtr_tag
{
    uint8_t tlvType;            /*!< Type */
    uint8_t length;             /*!< Length */
    uint8_t frameCounter[4];    /*!< Frame counter array */
} mleOtaTlvLlFrameCtr_t;

/********************/
/* Link Quality TLV */
/********************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  TLV Type 6   |    Length 1   |C| Res | Size  | Neighbor Data...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*********************/

/*! Link Quality TLV - Tx Interface structure */
typedef struct mleLinkQuality_tag
{
    mleTlvType_t tlvType;
    uint16_t complete;                      /*!< Has a value of 1 if the message includes all neighboring
                                                 routers for which the source has link quality data.
                                                 Otherwise is set to 0 */
    uint16_t size;                          /*!< The size of the type of included LL address minus 1.
                                                 Valid range: 0 - 15 */
    list_t   neighborDataList;              /*!< A list of neighbor records */
} mleLinkQuality_t;

/*! Link Quality TLV - Over the Air mapping structure */
typedef struct mleOtaLinkQuality_tag
{
    uint8_t tlvType;            /*!< Type */
    uint8_t length;             /*!< Length */
    uint8_t completeAndSize;    /*!< Complete and size */
    uint8_t neighborData[];     /*!< Neighbor data array */
} mleOtaLinkQuality_t;

/*************************/
/* Network Parameter TLV */
/*************************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  TLV Type 7   |    Length     | Parameter ID  |     Delay
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                     Delay                     |    Value...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

**************************/

/*! Network Parameter TLV Enumeration */
typedef enum nwkParamType_tag
{
    gNwkParamChannel_c,          /*!< Channel */
    gNwkParamPanId_c,            /*!< PAN Id */
    gNwkParamPermitJoining_c,    /*!< Permit Join */
    gNwkParamBeaconPayload_c     /*!< Beacon payload */
} nwkParamType_e;

/*! Network Parameter TLV - Tx Interface structure */
typedef struct mleTlvNwkParam_tag
{
    mleTlvType_t    tlvType;    /*!< Type */
    uint32_t        delay;      /*!< Delay */
    nwkParamType_t  paramId;    /*!< Parameter Id */
    uint16_t        size;       /*!< Size */
    uint8_t         aParam[];   /*!< Parameter array */
} mleTlvNwkParam_t;

/*! Network Parameter TLV - Over the Air mapping structure */
typedef struct mleOtaTlvNwkParam_tag
{
    uint8_t         tlvType;    /*!< Type */
    uint8_t         length;     /*!< Length */
    uint8_t         paramId;    /*!< Parameter Id */
    uint8_t         delay[4];   /*!< Delay array */
    uint8_t         value[];    /*!< Value array */
} mleOtaTlvNwkParam_t;

/*************************/
/* MLE Frame Counter TLV */
/*************************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   TLV Type    |    Length     |          Frame Counter        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Frame Counter        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

**************************/

/*! Network Parameter MLE Frame Counter TLV - Tx Interface structure */
typedef struct mleTlvMleFrameCtr_tag
{
    mleTlvType_t tlvType;     /*!< Type */
    uint32_t frameCounter;    /*!< Frame counter */
} mleTlvMleFrameCtr_t;

/*! Network Parameter MLE Frame Counter TLV - Over the Air mapping structure */
typedef struct mleOtaTlvMleFrameCtr_tag
{
    uint8_t tlvType;            /*!< Type */
    uint8_t length;             /*!< Length */
    uint8_t frameCounter[4];    /*!< Frame counter array */
} mleOtaTlvMleFrameCtr_t;

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
\fn     nwkStatus_t MLE_TlvSrcAddrAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the Source Address TLV to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV parameters
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvSrcAddrAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvModeAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the Mode TLV to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvModeAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvTimeoutAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the Timeout TLV  to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvTimeoutAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvChallengeAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the Challenge TLV  to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvChallengeAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvResponseAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the Response TLV  to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvResponseAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvLLFrameCtrAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the Link Layer Frame Counter TLV a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvLLFrameCtrAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvMleFrameCtrAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the MLE Frame Counter TLV to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvMleFrameCtrAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

#if !STACK_THREAD
/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvLinkQualityAdd(void *pParams, list_t *pTlvList)
\brief  Function used to add the Link Quality TLV to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure.
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV.
\param  [in]    pTlvSize        Pointer to the TLV size.

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MLE_TlvLinkQualityAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvNwkParamAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the Network Parameter TLV to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure.
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV.
\param  [in]    pTlvSize        Pointer to the TLV size.

\return         nwkStatus_t
***************************************************************************************************/
nwkStatus_t MLE_TlvNwkParamAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);
#endif /* STACK_THREAD */

#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /* _MLE_BASIC_H */
