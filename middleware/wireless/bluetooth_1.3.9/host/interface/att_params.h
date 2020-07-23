/*! *********************************************************************************
* \addtogroup ATT
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef ATT_PARAMS_H
#define ATT_PARAMS_H

#include "ble_constants.h"
#include "att_types.h"

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Error Response Parameters */
typedef struct
{
    attOpcode_t    requestOpcodeInError;
    uint16_t       attributeHandleInError;
    attErrorCode_t errorCode;
} attErrorResponseParams_t;

/*! Exchange MTU Request Parameters */
typedef struct
{
    uint16_t clientRxMtu;
} attExchangeMtuRequestParams_t;

/*! Exchange MTU Response Parameters */
typedef struct
{
    uint16_t serverRxMtu;
} attExchangeMtuResponseParams_t;

/*! Find Information Request Parameters */
typedef struct
{
    uint16_t startingHandle;
    uint16_t endingHandle;
} attFindInformationRequestParams_t;

typedef struct
{
    uint16_t handle;
    uint16_t uuid16;
} attFindInfoHandleUuid16Pair_t;
typedef struct
{
    uint16_t handle;
    uint8_t  uuid128[16];
} attFindInfoHandleUuid128Pair_t;

/*! Find Information Response Parameters */
typedef struct
{
    attUuidFormat_t format;
    uint16_t         handleUuidPairCount;
    union
    {
        attFindInfoHandleUuid16Pair_t  handleUuid16[(gAttMaxDataSize_d(gAttMaxMtu_c) - 1U)/4U]; /* 4 = 2+2 bytes per handle-UUID pair */
        attFindInfoHandleUuid128Pair_t handleUuid128[(gAttMaxDataSize_d(gAttMaxMtu_c) - 1U)/18U]; /* 18 = 2+16 bytes per handle-UUID pair */
    }               informationData;
} attFindInformationResponseParams_t;

/*! Find By Type Value Request Parameters */
typedef struct
{
    uint16_t startingHandle;
    uint16_t endingHandle;
    uint16_t attributeType; /* 16 bit UUID */
    uint8_t  attributeValue[gAttMaxDataSize_d(gAttMaxMtu_c) - 6U]; /* 6 = 4 for handles + 2 for type */
    uint16_t attributeLength;
} attFindByTypeValueRequestParams_t;

typedef attFindInformationRequestParams_t attFindByTypeValueHandleGroupPair_t;

/*! Find By Type Value Response - Parameters */
typedef struct
{
    attFindByTypeValueHandleGroupPair_t handleGroup[gAttMaxDataSize_d(gAttMaxMtu_c)/4U]; /* 4 bytes for starting and ending handle */
    uint16_t                            groupCount;
} attFindByTypeValueResponseParams_t;

/*! Read By Type Request Parameters */
typedef struct
{
    uint16_t        startingHandle;
    uint16_t        endingHandle;
    attUuidFormat_t format;
    union
    {
        uint16_t uuid16;
        uint8_t  uuid128[16];
    }               attributeType;
} attReadByTypeRequestParams_t;

/*! Read By Type Response Parameters */
typedef struct
{
    uint8_t  length; /* Fixed length of each handle+value pair in the Attribute Data List */
    uint8_t  attributeDataList[gAttMaxDataSize_d(gAttMaxMtu_c) - 1U]; /* 1 byte for length */
    uint16_t attributeDataListLength;
} attReadByTypeResponseParams_t;

/*! Read Request Parameters */
typedef struct
{
    uint16_t attributeHandle;
} attReadRequestParams_t;

/*! Read Response Parameters */
typedef struct
{
    uint8_t  attributeValue[gAttMaxDataSize_d(gAttMaxMtu_c)];
    uint16_t attributeLength;
} attReadResponseParams_t;

/*! Read Blob Request Parameters */
typedef struct
{
    uint16_t attributeHandle;
    uint16_t valueOffset;
} attReadBlobRequestParams_t;

/*! Read Blob Response Parameters */
typedef attReadResponseParams_t attReadBlobResponseParams_t;

/*! Read Multiple Request Parameters */
typedef struct
{
    uint16_t listOfHandles[gAttMaxDataSize_d(gAttMaxMtu_c) / 2U]; /* 2 bytes for each handle */
    uint16_t handleCount;
} attReadMultipleRequestParams_t;

/*! Read Multiple Response Parameters */
typedef struct
{
    uint8_t  listOfValues[gAttMaxDataSize_d(gAttMaxMtu_c)];
    uint16_t listLength;
} attReadMultipleResponseParams_t;

/*! Read By Group Type Request Parameters */
typedef attReadByTypeRequestParams_t attReadByGroupTypeRequestParams_t;

/*! Read By Group Type Response Parameters */
typedef attReadByTypeResponseParams_t attReadByGroupTypeResponseParams_t;
/* "length" = fixed length of each Handle+End Group Handle+Value in the Attribute Data List */

/*! Write Request and Write Command Parameters */
typedef struct
{
    uint16_t  attributeHandle;
    uint16_t  attributeLength;
    uint8_t   attributeValue[gAttMaxWriteDataSize_d(gAttMaxMtu_c)];
} attWriteRequestAndCommandParams_t;

/*! Write Request and Write Command Parameters for variable value length*/
typedef struct
{
    uint16_t  attributeHandle;
    uint16_t  attributeLength;
    uint8_t   attributeValue[1];
} attVarWriteRequestAndCommandParams_t;

/*! Signed Write Command Parameters */
typedef struct
{
    uint16_t attributeHandle;
    uint16_t attributeLength;
    /* Reserved space for the authorized signature generation procedure. Value starts at gAttSignedHdrSize_d */
    uint8_t  signedValue[gAttSignedHdrSize_d + gAttMaxSignedWriteDataSize_d + gAttAuthSignatureCtrSize_d];
    uint8_t  authenticationSignature[gAttAuthSignatureSize_d];
} attSignedWriteCommandParams_t;

/*! Prepare Write Request and Prepare Write Response Parameters */
typedef struct
{
    uint16_t attributeHandle;
    uint16_t valueOffset;
    uint8_t  attributeValue[gAttMaxPrepWriteDataSize_d(gAttMaxMtu_c)];
    uint16_t attributeLength;
} attPrepareWriteRequestResponseParams_t;

/*! Prepare Queue Item internal structure */
typedef struct
{
    uint16_t attributeHandle;
    uint16_t valueOffset;
    uint16_t attributeLength;
    uint8_t  attributeValue[1];
} attPrepareQueueItem;

/*! Execute Write Request Parameters */
typedef struct
{
    attExecuteWriteRequestFlags_t flags;
} attExecuteWriteRequestParams_t;

/*! Handle Value Notification and Handle Value Indication Parameters */
typedef attWriteRequestAndCommandParams_t attHandleValueNotificationIndicationParams_t;

/*! Handle Value Notification and Handle Value Indication Parameters for variable value length*/
typedef struct
{
    uint16_t  attributeHandle;
    uint16_t  attributeLength;
    uint8_t   attributeValue[1];
} attVarHandleValueNotificationIndicationParams_t;

typedef struct
{
    attOpcode_t attInvalidPduOpCode;
} attInvalidPduIndicationParams_t;

#endif /* ATT_PARAMS_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
