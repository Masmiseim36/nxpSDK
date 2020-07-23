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

#ifndef ATT_TYPES_H
#define ATT_TYPES_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "att_errors.h"

/************************************************************************************
*************************************************************************************
* Public constants and macros
*************************************************************************************
************************************************************************************/

#define gAttOpcodeSize_d      (1U)
#define gAttHandleSize_d      (2U)
#define gAttOffsetSize_d      (2U)

/* Macros used to compute maximum data size for various ATT payloads */
#define gAttMaxDataSize_d(mtu)          ((mtu) - (gAttOpcodeSize_d)) /* 22 */
#define gAttMaxReadDataSize_d(mtu)      (gAttMaxDataSize_d(mtu)) /* 22 */
#define gAttMaxWriteDataSize_d(mtu)     ((gAttMaxDataSize_d(mtu)) - (gAttHandleSize_d)) /* 20 */
#define gAttMaxNotifIndDataSize_d(mtu)  (gAttMaxWriteDataSize_d(mtu)) /* 20 */
#define gAttMaxPrepWriteDataSize_d(mtu) ((gAttMaxWriteDataSize_d(mtu)) - (gAttOffsetSize_d)) /* 18 */

#define gAttOpcodeCmdFlagPos_d       (6U)
#define gAttOpcodeCmdFlag_d          (1U << gAttOpcodeCmdFlagPos_d)

#define gAttAuthSignatureCtrSize_d   (4U)
#define gAttAuthSignatureSize_d     (12U)

#define gAttSignedHdrSize_d             (gAttOpcodeSize_d + gAttHandleSize_d)
#define gAttMaxSignedWriteDataSize_d    (gAttMaxWriteDataSize_d(gAttMaxMtu_c) - gAttAuthSignatureSize_d)

#define gAttTimeoutSeconds_c        (30U)

/* Based on the fact that corresponding request and response opcodes are consecutive */
#define responseOpcodeOfRequestOpcode(opcode) ((uint8_t)(opcode) + 1U)
#define requestOpcodeOfResponseOpcode(opcode) ((uint8_t)(opcode) - 1U)

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef enum
{
    /* Error Handling */
    gAttOpcodeErrorResponse_c                = 0x01U,
    /* MTU Exchange */
    gAttOpcodeExchangeMtuRequest_c           = 0x02U,
    gAttOpcodeExchangeMtuResponse_c          = 0x03U,
    /* Find Information */
    gAttOpcodeFindInformationRequest_c       = 0x04U,
    gAttOpcodeFindInformationResponse_c      = 0x05U,
    gAttOpcodeFindByTypeValueRequest_c       = 0x06U,
    gAttOpcodeFindByTypeValueResponse_c      = 0x07U,
    /* Reading Attributes */
    gAttOpcodeReadByTypeRequest_c            = 0x08U,
    gAttOpcodeReadByTypeResponse_c           = 0x09U,
    gAttOpcodeReadRequest_c                  = 0x0AU,
    gAttOpcodeReadResponse_c                 = 0x0BU,
    gAttOpcodeReadBlobRequest_c              = 0x0CU,
    gAttOpcodeReadBlobResponse_c             = 0x0DU,
    gAttOpcodeReadMultipleRequest_c          = 0x0EU,
    gAttOpcodeReadMultipleResponse_c         = 0x0FU,
    gAttOpcodeReadByGroupTypeRequest_c       = 0x10U,
    gAttOpcodeReadByGroupTypeResponse_c      = 0x11U,
    /* Writing Attributes */
    gAttOpcodeWriteRequest_c                 = 0x12U,
    gAttOpcodeWriteResponse_c                = 0x13U,
    gAttOpcodeWriteCommand_c                 = 0x52U,
    gAttOpcodeSignedWriteCommand_c           = 0xD2U,
    /* Queued Writes */
    gAttOpcodePrepareWriteRequest_c          = 0x16U,
    gAttOpcodePrepareWriteResponse_c         = 0x17U,
    gAttOpcodeExecuteWriteRequest_c          = 0x18U,
    gAttOpcodeExecuteWriteResponse_c         = 0x19U,
    /* Server Initiated */
    gAttOpcodeHandleValueNotification_c      = 0x1BU,
    gAttOpcodeHandleValueIndication_c        = 0x1DU,
    gAttOpcodeHandleValueConfirmation_c      = 0x1EU,

    gAttInvalidPduReceived_c                 = 0xDEU, /* internal purposes */
    gAttTimeoutOpcode_c                      = 0xDFU, /* internal purposes */
    gAttLastOpcode_c                         = 0xE0U  /* testing purposes */
} attOpcode_t;

typedef enum
{
    gAttUuid16BitFormat_c         = 0x01U,
    gAttUuid128BitFormat_c        = 0x02U,
} attUuidFormat_t;

typedef enum
{
    gAttExecuteWriteRequestCancel               = 0x00U,
    gAttExecuteWriteRequestImmediately          = 0x01U,
} attExecuteWriteRequestFlags_t;

#endif /* ATT_TYPES_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
