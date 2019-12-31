/*! *********************************************************************************
 * \addtogroup GATT
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef ATT_ERRORS_H
#define ATT_ERRORS_H

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! ATT error codes */
typedef enum
{
    gAttErrCodeNoError_c                        = 0x00U,
    gAttErrCodeInvalidHandle_c                  = 0x01U,
    gAttErrCodeReadNotPermitted_c               = 0x02U,
    gAttErrCodeWriteNotPermitted_c              = 0x03U,
    gAttErrCodeInvalidPdu_c                     = 0x04U,
    gAttErrCodeInsufficientAuthentication_c     = 0x05U,
    gAttErrCodeRequestNotSupported_c            = 0x06U,
    gAttErrCodeInvalidOffset_c                  = 0x07U,
    gAttErrCodeInsufficientAuthorization_c      = 0x08U,
    gAttErrCodePrepareQueueFull_c               = 0x09U,
    gAttErrCodeAttributeNotFound_c              = 0x0AU,
    gAttErrCodeAttributeNotLong_c               = 0x0BU,
    gAttErrCodeInsufficientEncryptionKeySize_c  = 0x0CU,
    gAttErrCodeInvalidAttributeValueLength_c    = 0x0DU,
    gAttErrCodeUnlikelyError_c                  = 0x0EU,
    gAttErrCodeInsufficientEncryption_c         = 0x0FU,
    gAttErrCodeUnsupportedGroupType_c           = 0x10U,
    gAttErrCodeInsufficientResources_c          = 0x11U,
    /* Reserved Error Opcodes                   =    0x12 - 0x7F */
    /* Application Error Opcodes                =    0x80 - 0x9F */
    /* Reserved Error Opcodes                   =    0xA0 - 0xDF */
    /* Common Profile And Service Error Opcodes =    0xE0 - 0xFF */
    /*     Reserved for Future Use Opcodes      =    0xE0 - 0xFB */
    gAttErrCodeWriteRequestRejected_c           = 0xFCU,
    gAttErrCodeCccdImproperlyConfigured_c       = 0xFDU,
    gAttErrCodeProcedureAlreadyInProgress_c     = 0xFEU,
    gAttErrCodeOutOfRange_c                     = 0xFFU,
} attErrorCode_t;

#endif /* ATT_ERRORS_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
