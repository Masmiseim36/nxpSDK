/*
 * Copyright (c) 2012, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __KEYBLOB_H__
#define __KEYBLOB_H__

#include "fsl_device_registers.h"

//! @name Error codes
//@{
#if !defined(SUCCESS)
#define SUCCESS (0)
#endif

typedef struct
{
    union
    {
        struct
        {
            uint32_t image_index : 4; //!< Key blob for the image specified by image_index, valid only if type = 1
            uint32_t dek_size : 4; //!< Decryption Key size, 0-128bits, 1-192 bits, 2-256 bits, 3-DUK, 4-IEE, valid only
                                   //!< if type = 0
            uint32_t bk_sel : 12;  //!< Blob key source selection, valid only if type = 0
            uint32_t size : 4;     //!< Keyblob Info structure size in longwords, valid only if type = 0
            uint32_t type : 4; //!< Type, 0 - Update key blob info, 1 - Generate and program Keyblob to corresponding
            //! offset for image specified by image_index
            uint32_t tag : 4; //!< Tag, fixed to 0x0F
        } B;
        uint32_t U;
    } option;
    uint32_t dek_addr;       //!< Valid if type = 0
    uint32_t keyblob_offset; //!< Valid if type = 0
} keyblob_info_t;

enum
{
    kKeyBlobInfoOption_Tag = 0x0B, //!< Key blob info tag
    kKeyBlobInfoType_Update = 0,   //!< key blob info type: Update
    kKeyBlobInfoType_Program = 1,  //!< key blob info type: Program
    kKeyBlobInfoSize = 3,          //!< KeyBlob Size in long word when type = kKeyBlobInfoType_Update

    kDekSize_128bits = 0,
    kDekSize_192bits = 1,
    kDekSize_256bits = 2,
    kDekSize_edgelockDUK = 3,
    kDekSize_edgelockIEE = 4,

    kBlobKeySize_128bits = 0,
    kBlobKeySize_192bits = 1,
    kBlobKeySize_256bits = 2,

    kKeyBlob_HeaderSize = 8,
    kKeyBlob_MACSize = 16,

#if defined(BL_FEATURE_KEYBLOB_BLOB_KEY_MAX_SIZE)
    kKeyBlob_BK_MaxSize = BL_FEATURE_KEYBLOB_BLOB_KEY_MAX_SIZE,
#else
    kKeyBlob_BK_MaxSize = 32,
#endif
#if defined(BL_FEATURE_KEYBLOB_DATA_KEY_MAX_SIZE)
    kKeyBlob_DEK_MaxSize = BL_FEATURE_KEYBLOB_DATA_KEY_MAX_SIZE,
#else
    kKeyBlob_DEK_MaxSize = 32,
#endif
#if defined(BL_FEATURE_KEYBLOB_BLOB_MAX_SIZE)
    kKeyBlob_Blob_MaxSize = BL_FEATURE_KEYBLOB_BLOB_MAX_SIZE,
#else
    kKeyBlob_Blob_MaxSize = kKeyBlob_HeaderSize + kKeyBlob_BK_MaxSize + kKeyBlob_DEK_MaxSize + kKeyBlob_MACSize,
#endif
};

typedef struct _keyblob_context
{
    uint32_t dek_size;
    uint32_t bk_sel;
    uint32_t bk_size;
    uint32_t dek[kKeyBlob_DEK_MaxSize / sizeof(uint32_t)];
    uint32_t keyblob[kKeyBlob_Blob_MaxSize / sizeof(uint32_t)];
} keyblob_context_t;

////////////////////////////////////////////////////////////////////////////////
//! @brief Wrapper function for generating blob.
//!
//! @param[in] key_addr  Location address of 128 bit dek key.
//!
//! @return SUCCESS
//! @return ERROR_XXX
////////////////////////////////////////////////////////////////////////////////
status_t generate_key_blob(const uint8_t *pKeyAddr,
                           const uint32_t keySize,
                           const uint32_t keySel,
                           uint8_t *pKeyBlobAddr,
                           uint32_t *pKeyBlobSize);

status_t keyblob_update(keyblob_info_t *key_info);

status_t keyblob_get(uint8_t **keyblob_start, uint32_t *keyblob_size);

#endif /* __KEYBLOB_H__ */
