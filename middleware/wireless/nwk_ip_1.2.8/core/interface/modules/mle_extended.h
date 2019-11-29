/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _MLE_EXTENDED_H
#define _MLE_EXTENDED_H

/*!
\file       mle_extended.h
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

/*==================================================================================================
Public macros
==================================================================================================*/

#define TLV_ROUTE_ROUTER_OUT_MASK       0xC0    /*!< TLV router out mask */

#define TLV_ROUTE_ROUTER_OUT_SHIFT      6       /*!< TLV router out shift */

#define TLV_ROUTE_ROUTER_IN_MASK        0x30    /*!< TLV router in mask */

#define TLV_ROUTE_ROUTER_IN_SHIFT       4       /*!< TLV router in shift */

#define TLV_ROUTE_ROUTER_ROUTE_MASK     0xF     /*!< TLV router route mask */

#define TLV_ADDR_REG_FULL_ADDR_BIT_MASK 0x80    /*!< TLV address registration full address bit mask */

#define TLV_ADDR_REG_CTXT_ID_MASK       0x0F    /*!< TLV address registration context ID mask */

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! MLE Extended Command Types Enumeration */
typedef enum mleCmdExtType_tag
{
    gMleCmdDataReq_c                    = 7U,     /*!< Data request */
    gMleCmdDataRes_c                    = 8U,     /*!< Data response */
    gMleCmdParentReq_c                  = 9U,     /*!< Parent request */
    gMleCmdParentRes_c                  = 10U,    /*!< Parent response */
    gMleCmdChildIdReq_c                 = 11U,    /*!< Child ID request */
    gMleCmdChildIdRes_c                 = 12U,    /*!< Child ID response */
    gMleCmdChildUpdateReq_c             = 13U,    /*!< Child Update request */
    gMleCmdChildUpdateRes_c             = 14U,    /*!< Child Update response */
    gMleCmdAnnounce_c                   = 15U,    /*!< MLE Announce */
    gMleCmdDiscoveryReq_c               = 16U,    /*!< Discovery request */
    gMleCmdDiscoveryResp_c              = 17U,    /*!< Discovery response */
} mleCmdExtType_e;

/*! MLE Extended TLV Types Enumeration */
typedef enum melTlvExtType_tag
{
    gMleTlvServer_c                     = 6U,     /*!< Server TLV */
    gMleTlvRoute_c                      = 9U,     /*!< Route TLV */
    gMleTlvAddress16_c                  = 10U,    /*!< Address16 TLV (16-bit MAC address) */
    gMleTlvLeaderData_c                 = 11U,    /*!< Leader Data TLV */
    gMleTlvNetworkData_c                = 12U,    /*!< Network Data TLV */
    gMleTlvTlvRequest_c                 = 13U,    /*!< TLV Request TLV */
    gMleTlvScanMask_c                   = 14U,    /*!< Scan Mask TLV */
    gMleTlvConnectivity_c               = 15U,    /*!< Connectivity TLV */
    gMleTlvLinkMargin_c                 = 16U,    /*!< Link Margin TLV */
    gMleTlvStatus_c                     = 17U,    /*!< Status TLV */
    gMleTlvVersion_c                    = 18U,    /*!< Version TLV */
    gMleTlvAddrReg_c                    = 19U,    /*!< Address registration TLV */
    gMleTlvChannel_c                    = 20U,    /*!< Channel TLV */
    gMleTlvPanId_c                      = 21U,    /*!< PAN Id TLV */
    gMleTlvActiveTimestamp_c            = 22U,    /*!< Active Timestamp TLV */
    gMleTlvPendingTimestamp_c           = 23U,    /*!< Pending Timestamp TLV */
    gMleTlvActiveOperationalDataset_c   = 24U,    /*!< Active Operational Dataset TLV */
    gMleTlvPendingOperationalDataset_c  = 25U,    /*!< Pending Operational Dataset TLV */
    gMleTlvDiscovery_c                  = 26U,    /*!< Discovery TLV */
} mleTlvExtType_e;

/*! MLE Extended TLV Type*/
typedef uint8_t mleTlvExtType_t;

/*! Generic extended TLV format */
typedef struct mleGeneric_ExtTlv_tag
{
    uint8_t reserved1;           /*!< Reserved. Value = 0xFF */
    uint8_t tlvType;             /*!< Type */
    uint8_t reserved2;           /*!< Reserved. Value = 0xFF */
    uint8_t length[2];           /*!< Length */
    uint8_t pData[];             /*!< Value */
}mleGeneric_ExtTlv_t;

/******************/
/* Address 16 TLV */
/******************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   TLV Type    |    Length     |           Address             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*******************/

/*! Address 16 TLV - Tx Interface structure */
typedef struct mleTlvAddress16_tag
{
    mleTlvExtType_t tlvType;    /*!< Type */
    uint16_t address;           /*!< Address */
} mleTlvAddress16_t;

/*! Address 16 TLV - Over the Air mapping structure */
typedef struct mleOtaTlvAddress16_tag
{
    uint8_t type;            /*!< Type */
    uint8_t length;          /*!< Length */
    uint8_t shortAddr[2];    /*!< Address array */
} mleOtaTlvAddress16_t;

/*******************/
/* Leader Data TLV */
/*******************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|    TLV Type   |     Length    |          Instance ID
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                                |   Weighting   | Data Version  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Stable Data Ver|   Leader ID   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

********************/

/*! Leader Data TLV - Tx Interface structure */
typedef struct mleTlvLeaderData_tag
{
    mleTlvExtType_t tlvType;
    uint32_t instanceId;                /*!< Network Segment Identifier */
    uint8_t  weighting;                 /*!< Weighting value for the network fragment */
    uint8_t  dataVersion;               /*!< Version of the Network Data */
    uint8_t  stableDataVersion;         /*!< Stable Version of the Network Data */
    uint8_t  leaderId;                  /*!< Network Leader Router ID */
} mleTlvLeaderData_t;


/****************/
/* NWK Data TLV */
/****************

 0                   1                   2
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   TLV Type    |    Length     |    NWK DATA
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

****************/

/*! NWK Data TLV - Over the Air mapping structure */
typedef struct mleOtaTlvNwkData_tag
{
    uint8_t type;      /*!< Type */
    uint8_t length;    /*!< Length */
    uint8_t data[];    /*!< Data array */
} mleOtaTlvNwkData_t;

/*******************/
/* Tlv Request TLV */
/*******************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   TLV Type    |    Length     |    TLV ID 0   |    TLV ID 1   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

********************/

/*! TLV Request TLV - Tx Interface structure */
typedef struct mleTlvTlvRequest_tag
{
    mleTlvExtType_t tlvType;    /*!< Type */
    uint32_t nbTlvs;            /*!< number of TLVs */
    uint8_t* pTlvReqIds;        /*!< Pointer to requested TLV IDs */
} mleTlvTlvRequest_t;

/*! TLV Request TLV - Over the Air mapping structure */
typedef struct mleOtaTlvTlvRequest_tag
{
    uint8_t type;              /*!< Type */
    uint8_t length;            /*!< Length */
    uint8_t aTlvReqIds[];      /*!< Requested TLV IDs array */
} mleOtaTlvTlvRequest_t;

/*****************/
/* Scan Mask TLV */
/*****************

 0                   1                   2
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   TLV Type    |     Length    |R|E|  Reserved |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

******************/

/*! Scan Mask Flags bit masks */
typedef enum mleTlvScanMaskFlags_tag
{
    gMleTlvScanMaskFlagRouter_c = 0x80U,            /*!< Active Routers flag */
    gMleTlvScanMaskFlagEligibleRouter_c = 0x40U,    /*!< Router Capable End Devices flag */
} mleTlvScanMaskFlags_e;

/*! Scan Mask TLV - Tx Interface structure */
typedef struct mleTlvScanMask_tag
{
    mleTlvExtType_t tlvType;    /*!< Type */
    uint8_t scanMask;           /*!< Scan mask */
} mleTlvScanMask_t;

/*! Scan Mask TLV - Over the Air mapping structure */
typedef struct mleOtaTlvScanMask_tag
{
    uint8_t type;        /*!< Type */
    uint8_t length;      /*!< Length */
    uint8_t scanMask;    /*!< Scan mask */
} mleOtaTlvScanMask_t;

/********************/
/* Connectivity TLV */
/********************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|P P r r r r r r| Link Quality 3| Link Quality 2| LinkQuality 1 |  Leader Cost  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  ID Sequence  | Active Routers|          Sed Buff Size        |Sed Dgram Count|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*********************/

/*! Connectivity TLV - Tx Interface structure */
typedef struct mleTlvConectivity_tag
{
    uint8_t priority;          /*!< Priority */
    uint8_t cLinkQuality3;     /*!< Number of connected children with link quality 3 */
    uint8_t cLinkQuality2;     /*!< Number of connected children with link quality 2 */
    uint8_t cLinkQuality1;     /*!< Number of connected children with link quality 1 */
    uint8_t leaderCost;        /*!< Leader cost */
    uint8_t idSequence;        /*!< Id sequence */
    uint8_t activeRouters;     /*!< Active routers */
    uint8_t sedBuffSize[2];    /*!< SED buffer size */
    uint8_t sedDgramCount;     /*!< SED datagram count */
} mleTlvConnectivity_t;

/*! Connectivity TLV - Over the Air mapping structure */
typedef struct mleOtaTlvConectivity_tag
{
    uint8_t type;             /*!< Type */
    uint8_t length;           /*!< Length */
    uint8_t priority;         /*!< Priority */
    uint8_t cLinkQuality3;    /*!< Number of connected children with link quality 3 */
    uint8_t cLinkQuality2;    /*!< Number of connected children with link quality 2 */
    uint8_t cLinkQuality1;    /*!< Number of connected children with link quality 1 */
    uint8_t leaderCost;       /*!< Leader cost */
    uint8_t idSequence;       /*!< Id sequence */
    uint8_t activeRouters;    /*!< Active routers */
    uint8_t sedBuffSize[2];   /*!< SED buffer size */
    uint8_t sedDgramCount;    /*!< SED datagram count */
} mleOtaTlvConnectivity_t;

/*******************/
/* Link Margin TLV */
/*******************

 0                   1                   2
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   TLV Type    |    Length     |  Link Margin  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*************/

/*! Link Margin TLV - Tx Interface structure */
typedef struct mleTlvLinkMargin_tag
{
    mleTlvExtType_t tlvType;    /*!< Type */
    uint8_t linkMargin;         /*!< Link Margin */
} mleTlvLinkMargin_t;

/*! Link Margin TLV - Over the Air mapping structure */
typedef struct mleOtaTlvLinkMargin_tag
{
    uint8_t type;             /*!< Type */
    uint8_t length;           /*!< Length */
    uint8_t linkMargin;       /*!< Link Margin */
} mleOtaTlvLinkMargin_t;

/**************/
/* Status TLV */
/**************

 0                   1                   2
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   TLV Type    |    Length     |     Status    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*************/

/*! Status TLV values */
typedef enum mleTlvStatusValues_tag
{
    gStatusTlvValueError_c    = 1,    /*!< Error */
    gStatusTlvValueDAD_c      = 2,    /*!< Duplicate Address Detected */
} mleTlvStatusValues_t;

/*! Status TLV - Tx Interface structure */
typedef struct mleTlvStatus_tag
{
    mleTlvExtType_t tlvType;    /*!< Type */
    uint8_t status;             /*!< Status */
} mleTlvStatus_t;

/*! Status TLV - Over the Air mapping structure */
typedef struct mleOtaTlvStatus_tag
{
    uint8_t type;      /*!< Type */
    uint8_t length;    /*!< Length */
    uint8_t status;    /*!< Status */
} mleOtaTlvStatus_t;

/***************/
/* Version TLV */
/***************

 0                   1
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Version              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*************/

/*! Version TLV - Tx Interface structure */
typedef struct mleTlvVersion_tag
{
    mleTlvExtType_t tlvType;    /*!< Type */
    uint16_t version;           /*!< Version */
} mleTlvVersion_t;

/*! Version TLV - Over the Air mapping structure */
typedef struct mleOtaTlvVersion_tag
{
    uint8_t type;          /*!< Type */
    uint8_t length;        /*!< Length */
    uint8_t version[2];    /*!< Version array */
} mleOtaTlvVersion_t;

/************************/
/* Address registration */
/************************

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|0|  Reserved   |               IPv6 address [16 bytes] ...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|1| Res |  CID  |               IID [8 bytes] ...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*************/

/*! Address Registration TLV - Tx Interface structure */
typedef struct mleTlvAddrReg_tag
{
    mleTlvExtType_t tlvType;      /*!< Type */
    bool_t bFullAddress;          /*!< Is full address flag */
    uint8_t cid;                  /*!< Context ID */
    uint8_t *pAddress;            /*!< Pointer to address */
} mleTlvAddrReg_t;

/*! Address Registration TLV - Over the Air mapping structure */
typedef struct mleOtaTlvAddrReg_tag
{
    uint8_t type;          /*!< Type */
    uint8_t length;        /*!< Length */
    uint8_t addrOpt;       /*!< Address option */
    uint8_t aAddress[];    /*!< Address array */
} mleOtaTlvAddrReg_t;

/*! Address Registration Information structure */
typedef struct mleOtaAddrRegInfo_tag
{
    uint8_t addrOpt;       /*!< Address option */
    uint8_t aAddress[];    /*!< Address array */
} mleOtaAddrRegInfo_t;

/*****************/
/* Hold Time TLV */
/*****************

 0                   1
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Hold Time            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

******************/

/*! Hold Time TLV - Tx Interface structure */
typedef struct mleTlvHoldTime_tag
{
    mleTlvExtType_t tlvType;    /*!< Type */
    uint16_t holdTime;          /*!< Hold time */
} mleTlvHoldTime_t;

/*! Hold Time TLV - Over the Air mapping structure */
typedef struct mleOtaTlvHoldTime_tag
{
    uint8_t type;           /*!< Type */
    uint8_t length;         /*!< Length */
    uint8_t holdTime[2];    /*!< Hold time array */
} mleOtaTlvHoldTime_t;

/*************************/
/* MLE Channel TLV */
/*************************

0                   1                   2
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  ChannelPage  |  Channel                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

**************************/

/*! Channel TLV - Tx interface structure */
typedef struct mleTlvChannel_tag
{
    uint8_t tlvType;        /*!< Type */
    uint8_t channelPage;    /*!< Channel Page */
    uint16_t channel;       /*!< Channel */
} mleTlvChannel_t;

/*! Channel TLV - Over the Air mapping structure */
typedef struct mleOtaTlvChannel_tag
{
    uint8_t tlvType;        /*!< Type */
    uint8_t length;         /*!< Length */
    uint8_t channelPage;    /*!< Channel Page */
    uint8_t channel[2];     /*!< Channel array */
} mleOtaTlvChannel_t;

/*************************/
/* MLE Pan Id TLV */
/*************************

0                   1
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  PAN ID                       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

**************************/

/*! PAN Id TLV - Tx interface structure */
typedef struct mleTlvPanId_tag
{
    uint8_t tlvType;     /*!< Type */
    uint16_t panId;      /*!< PAN Id */
} mleTlvPanId_t;

/*! PAN Id TLV - Over the Air mapping structure */
typedef struct mleOtaTlvPanId_tag
{
    uint8_t tlvType;     /*!< Type */
    uint8_t length;      /*!< Length */
    uint8_t panId[2];    /*!< PAN Id array */
} mleOtaTlvPanId_t;

/*************************/
/* MLE Active Timestamp TLV */
/*************************

0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Timestamp Seconds ...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                                |  Timestamp Ticks            |U|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

**************************/

/*! Active Timestamp TLV - Tx interface structure */
typedef struct mleOtaTlvActiveTimestamp_tag
{
    uint8_t tlvType;        /*!< Type */
    uint8_t length;         /*!< Length */
    uint8_t seconds[6];     /*!< Seconds array */
    uint8_t ticks[2];       /*!< Ticks array */
} mleOtaTlvActiveTimestamp_t;

/*! Pending Timestamp TLV - Tx interface structure */
typedef struct mleOtaTlvPendingTimestamp_tag
{
    uint8_t tlvType;       /*!< Type */
    uint8_t length;        /*!< Length */
    uint8_t seconds[6];    /*!< Seconds array */
    uint8_t ticks[2];      /*!< Ticks array */
} mleOtaTlvPendingTimestamp_t;

/*! Commissioning Dataset TLV - Tx interface structure */
typedef struct meshcopCommissioningDataset_tag
{
    uint8_t tlvType;    /*!< Type */
    uint8_t length;     /*!< Length */
    uint8_t aValue[];   /*!< Value array */
} meshcopCommissioningDataset_t;

/*! Active Operational Dataset TLV - Tx interface structure */
typedef struct mleOtaTlvActiveOperationalDataset_tag
{
    uint8_t tlvType;    /*!< Type */
    uint8_t length;     /*!< Length */
    uint8_t aValue[];   /*!< Value array */
} mleOtaTlvActiveOperationalDataset_t;

/*! Pending Operational Dataset - Tx interface structure */
typedef struct mleOtaTlvPendingOperationalDataset_tag
{
    uint8_t tlvType;    /*!< Type */
    uint8_t length;     /*!< Length */
    uint8_t aValue[];   /*!< Value array */
} mleOtaTlvPendingOperationalDataset_t;

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
\fn     nwkStatus_t MLE_TlvAddress16Add(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the Short Address TLV to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

 \return        nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvAddress16Add(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvTlvRequestAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the TLV Request to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvTlvRequestAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

/*!*************************************************************************************************
\fn     void MLE_TlvScanMaskAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the TLV ScanMask to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvScanMaskAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

#if !THREAD_ED_CONFIG
/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvConnectivityAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the TLV Connectivity to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvConnectivityAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);
#endif

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvRssiAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the TLV RSSI to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvRssiAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvStatusAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the TLV Status to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvStatusAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

/*!*************************************************************************************************
\fn     nwkStatus_t MLE_TlvVersionAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize)
\brief  Function used to add the TLV Version to a TLV buffer.

\param  [in]    pParams         Pointer to the input TLV structure
\param  [in]    pTlvBuffer      Pointer to the buffer that will contain the TLV
\param  [in]    pTlvSize        Pointer to the TLV size

\return         nwkStatus_t     Status of the operation
***************************************************************************************************/
nwkStatus_t MLE_TlvVersionAdd(void *pParams, void *pTlvBuffer, uint16_t *pTlvSize);

#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /* _MLE_EXTENDED_H */
