/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdbool.h>
#include <stdint.h>

#include "bootloader_common.h"
#include "fsl_edgelock.h"
#include "keyblob.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The Keysel to DEK. */
#define KEY_BLOB_KEYSEL_DEK (0x8U)
/*! @brief The key blob block size: DEK key configuration. */
#define KEY_BLOB_DEK_KEY_DATA_SIZE (BL_FEATURE_KEYBLOB_DEK_KEY_DATA_SIZE)
/*! @brief The key blob export maximal size. */
#define KEY_BLOB_DEK_EXPORT_MAX_SIZE (BL_FEATURE_KEYBLOB_DEK_BLOB_EXPORT_SIZE)
/*! @brief The Key identifier for DEK. */
#define KEY_BLOB_DEK_IDENTIFIER (0)

#define KEY_BLOB_DEK_BLOCK_HEADER_SIZE (sizeof(generate_keyblob_block_t))

/*! @brief The Keysel to OTFAD1. */
#define KEY_BLOB_KEYSEL_OTFAD1 (0x100U)
/*! @brief The Keysel to OTFAD2. */
#define KEY_BLOB_KEYSEL_OTFAD2 (0x200U)
/*! @brief The key blob block size: OTFAD key configuration. */
#define KEY_BLOB_OTFAD_KEY_DATA_SIZE (BL_FEATURE_KEYBLOB_OTFAD_KEY_DATA_SIZE)
/*! @brief The key blob export size. */
#define KEY_BLOB_OTFAD_EXPORT_SIZE (BL_FEATURE_KEYBLOB_OTFAD_BLOB_EXPORT_SIZE)
/*! @brief Number of keys for each OTFAD configuration. */
#define KEY_BLOB_OTFAD_KEY_NUM (BL_FEATURE_KEYBLOB_OTFAD_KEY_NUM)
/*! @brief The Key identifier for OTFAD. */
#define KEY_BLOB_OTFAD_IDENTIFIER(otfad, keyIndex) ((otfad) | (keyIndex))

#define KEY_BLOB_OTFAD_BLOCK_HEADER_SIZE (sizeof(generate_keyblob_block_t))

/*! @brief The format of the generate keyblob input block. */
typedef struct _generate_keyblob_block_
{
    uint8_t version; /* Must be 0x00*/
    uint16_t length; /* Total length in bytes of the header and its payload. */
    uint8_t tag;     /* Indicates the type of the input data, 0x81: input data is a private key. */
    uint8_t flags;   /* Blob type, 0: reserved, 1: DEK, 2: OTFAD key configuration, 3-7: reserved. */
    uint8_t keySize; /* size of the key contained in the blob, 0x10 - 128 bit AES/SM4 key, 0x18: 192 bit AES key, 0x20:
                        256 bit AES key, 0x28: 40 bytes of OTFAD key struct configuration. */
    uint8_t algorithm; /* 0x03: AES CBC, 0x04: AES CTR, 0x2B: SM4 CBC(128 bits only). */
    uint8_t reserved;  /* Unused. */
    uint8_t key[];     /* The plaintet for input of the blob generation service or as encrypted blob. */
} __attribute__((packed)) generate_keyblob_block_t;

/*! @brief The Keysel to IEE. */
#define KEY_BLOB_KEYSEL_IEE (0x10U)
/*! @brief The key blob header size, used only by ROM. */
#define KEY_BLOB_IEE_BLOCK_HEADER_SIZE (sizeof(iee_keyblob_block_t))
/*! @brief The key blob block size: IEE key configuration. */
#define KEY_BLOB_IEE_KEY_DATA_SIZE (BL_FEATURE_KEYBLOB_IEE_KEY_DATA_SIZE - KEY_BLOB_IEE_BLOCK_HEADER_SIZE)
/*! @brief The key blob export size. */
#define KEY_BLOB_IEE_EXPORT_SIZE (BL_FEATURE_KEYBLOB_IEE_BLOB_EXPORT_SIZE - KEY_BLOB_IEE_BLOCK_HEADER_SIZE)
/*! @brief Also be used as IEE key Identifier. */
#define KEY_BLOB_TAG_IEE (0x49454542U) //"IEEB"
/*! @brief The Key identifier for IEE. */
#define KEY_BLOB_IEE_IDENTIFIER (KEY_BLOB_TAG_IEE)

/*! @brief The format of the IEE keyblob input block. */
typedef struct _iee_keyblob_block_
{
    uint32_t tag;         /* Must be 0x49454542U. */
    uint32_t regionStart; /* Start address of IEE region0. */
    uint32_t regionEnd;   /* End address of IEE region0. */
    uint32_t xorCheck;    /* Checksum of header. */
    uint8_t keyBlob[];    /* The key blob block, point to generate_keyblob_block_t. */
} iee_keyblob_block_t;

/*******************************************************************************
 * Prototype
 ******************************************************************************/
/*!
 * @brief Generate DEK key blob.
 *
 * @param base Edgelock Message Unit base.
 * @param keyData Address where the input key data can be found.
 * @param keySize Size of keyData.
 * @param keySel Index of DEK
 * @param keyBlob Address where the key blob can be stored.
 * @param keyBlobSize available key blob data in bytes.
 * @return 0 for success, otherwise return error code.
 */
static status_t EDGELOCK_GenerateDekKeyblob(SxMU_Type *base,
                                            const uint8_t *keyData,
                                            const uint32_t keySize,
                                            const int32_t keySel,
                                            uint8_t *keyBlob,
                                            uint32_t *keyBlobSize);

/*!
 * @brief Generate DUK key blob.
 *
 * @param base Edgelock Message Unit base.
 * @param keyData Address where the input key data can be found.
 * @param keySize Size of keyData.
 * @param otfad Index of OTFAD, 1 or 2.
 * @param keyBlob Address where the key blob can be stored.
 * @param keyBlobSize available key blob data in bytes.
 * @return 0 for success, otherwise return error code.
 */
static status_t EDGELOCK_GenerateDukKeyblob(SxMU_Type *base,
                                            const uint8_t *keyData,
                                            const uint32_t keySize,
                                            const int32_t keySel,
                                            uint8_t *keyBlob,
                                            uint32_t *keyBlobSize);

/*!
 * @brief Start RNG and check it status.
 *
 * @param base Edgelock Message Unit base.
 * @return 0 for success, otherwise return error code.
 */
static status_t EDGELOCK_EnableRNG(SxMU_Type *base);

/*!
 * @brief Generate IEE key blob.
 *
 * @param base Edgelock Message Unit base.
 * @param keyData Address where the input key data can be found.
 * @param keySize Size of keyData.
 * @param keySel Index of IEE.
 * @param keyBlob Address where the key blob can be stored.
 * @param keyBlobSize available key blob data in bytes.
 * @return 0 for success, otherwise return error code.
 */
static status_t EDGELOCK_GenerateIEEKeyblob(SxMU_Type *base,
                                            const uint8_t *keyData,
                                            const uint32_t keySize,
                                            const int32_t keySel,
                                            uint8_t *keyBlob,
                                            uint32_t *keyBlobSize);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Codes
 ******************************************************************************/
/*!
 * @brief Generate DEK key blob.
 *
 * @param base Edgelock Message Unit base.
 * @param keyData Address where the input key data can be found.
 * @param keySize Size of keyData.
 * @param keySel Index of DEK
 * @param keyBlob Address where the key blob can be stored.
 * @param keyBlobSize available key blob data in bytes.
 * @return 0 for success, otherwise return error code.
 */
static status_t EDGELOCK_GenerateDekKeyblob(SxMU_Type *base,
                                            const uint8_t *keyData,
                                            const uint32_t keySize,
                                            const int32_t keySel,
                                            uint8_t *keyBlob,
                                            uint32_t *keyBlobSize)
{
    status_t status;

    SDK_ALIGN(static uint8_t s_keyDataBlock[KEY_BLOB_DEK_KEY_DATA_SIZE], KEY_BLOB_ALIGN_SIZE);
    SDK_ALIGN(static uint8_t s_keyBlobBlock[KEY_BLOB_DEK_EXPORT_MAX_SIZE], KEY_BLOB_ALIGN_SIZE);

    if ((keySel != KEY_BLOB_KEYSEL_DEK) || (keySize > KEY_BLOB_DEK_KEY_DATA_SIZE) || (!keyData) || (!keyBlob) ||
        (!keyBlobSize))
    {
        return kStatus_InvalidArgument;
    }

    *keyBlobSize = 0;

    memcpy(s_keyDataBlock, keyData, sizeof(s_keyDataBlock));
    memset(s_keyBlobBlock, 0, sizeof(s_keyBlobBlock));

    /* Check the key block header. */
    generate_keyblob_block_t *pDataBlock = (generate_keyblob_block_t *)s_keyDataBlock;
    if ((pDataBlock->version != 0U) || (pDataBlock->length > sizeof(s_keyDataBlock)) ||
        (pDataBlock->tag != KEY_BLOB_TAG_PRIVATEKEY) || (pDataBlock->flags != KEY_BLOB_FLAGS_DEK) ||
        (pDataBlock->keySize != (keySize - KEY_BLOB_DEK_BLOCK_HEADER_SIZE)) ||
        ((pDataBlock->algorithm != KEY_BLOB_ALGORITHM_AESCBC) && (pDataBlock->algorithm != KEY_BLOB_ALGORITHM_SM4CBC)))
    {
        return kStatus_NoData;
    }

    status = EDGELOCK_EnableRNG(base);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = EDGELOCK_GenerateKeyblob(base, KEY_BLOB_DEK_IDENTIFIER, (uint32_t)s_keyDataBlock, (uint32_t)s_keyBlobBlock,
                                      sizeof(s_keyBlobBlock));
    if (status == kStatus_Success)
    {
        generate_keyblob_block_t *pBlobBlock = (generate_keyblob_block_t *)s_keyBlobBlock;
        /* Remove the header */
        *keyBlobSize = ((pBlobBlock->length > KEY_BLOB_DEK_BLOCK_HEADER_SIZE) &&
                        (pBlobBlock->length < KEY_BLOB_DEK_EXPORT_MAX_SIZE)) ?
                           (pBlobBlock->length) :
                           (KEY_BLOB_DEK_EXPORT_MAX_SIZE);
        memcpy(&keyBlob[0], pBlobBlock, *keyBlobSize);
    }
    else
    {
        *keyBlobSize = 0;
        return status;
    }

    return status;
}

/*!
 * @brief Generate DUK key blob.
 *
 * @param base Edgelock Message Unit base.
 * @param keyData Address where the input key data can be found.
 * @param keySize Size of keyData.
 * @param keySel Index of OTFAD, 1 or 2.
 * @param keyBlob Address where the key blob can be stored.
 * @param keyBlobSize available key blob data in bytes.
 * @return 0 for success, otherwise return error code.
 */
static status_t EDGELOCK_GenerateDukKeyblob(SxMU_Type *base,
                                            const uint8_t *keyData,
                                            const uint32_t keySize,
                                            const int32_t keySel,
                                            uint8_t *keyBlob,
                                            uint32_t *keyBlobSize)
{
    status_t status;

    SDK_ALIGN(static uint8_t s_keyDataBlock[KEY_BLOB_OTFAD_KEY_DATA_SIZE], KEY_BLOB_ALIGN_SIZE);
    SDK_ALIGN(static uint8_t s_keyBlobBlock[(KEY_BLOB_OTFAD_BLOCK_HEADER_SIZE + KEY_BLOB_OTFAD_EXPORT_SIZE)],
              KEY_BLOB_ALIGN_SIZE);

    if (((keySel != KEY_BLOB_KEYSEL_OTFAD1) && (keySel != KEY_BLOB_KEYSEL_OTFAD2)) ||
        (keySize != (KEY_BLOB_OTFAD_KEY_DATA_SIZE * KEY_BLOB_OTFAD_KEY_NUM)) || (!keyData) || (!keyBlob) ||
        (!keyBlobSize))
    {
        return kStatus_InvalidArgument;
    }

    *keyBlobSize = 0;

    for (uint32_t i = 0; i < KEY_BLOB_OTFAD_KEY_NUM; i++)
    {
        memcpy(s_keyDataBlock, &keyData[i * sizeof(s_keyDataBlock)], sizeof(s_keyDataBlock));
        memset(s_keyBlobBlock, 0, sizeof(s_keyBlobBlock));

        /* Check the key block header. */
        generate_keyblob_block_t *pDataBlock = (generate_keyblob_block_t *)s_keyDataBlock;
        if ((pDataBlock->version != 0U) || (pDataBlock->length != sizeof(s_keyDataBlock)) ||
            (pDataBlock->tag != KEY_BLOB_TAG_PRIVATEKEY) || (pDataBlock->flags != KEY_BLOB_FLAGS_OTFAD) ||
            (pDataBlock->keySize != (sizeof(s_keyDataBlock) - KEY_BLOB_OTFAD_BLOCK_HEADER_SIZE)) ||
            (pDataBlock->algorithm != KEY_BLOB_ALGORITHM_AESCTR))
        {
            return kStatus_NoData;
        }

        status = EDGELOCK_GenerateKeyblob(base, KEY_BLOB_OTFAD_IDENTIFIER(keySel, i), (uint32_t)s_keyDataBlock,
                                          (uint32_t)s_keyBlobBlock, sizeof(s_keyBlobBlock));
        if (status == kStatus_Success)
        {
            generate_keyblob_block_t *pBlobBlock = (generate_keyblob_block_t *)s_keyBlobBlock;
            /* Remove the header */
            *keyBlobSize += KEY_BLOB_OTFAD_EXPORT_SIZE;
            memcpy(&keyBlob[i * KEY_BLOB_OTFAD_EXPORT_SIZE], pBlobBlock->key, KEY_BLOB_OTFAD_EXPORT_SIZE);
        }
        else
        {
            *keyBlobSize = 0;
            return status;
        }
    }

    return status;
}

/*!
 * @brief Start RNG and check it status.
 *
 * @param base Edgelock Message Unit base.
 * @return 0 for success, otherwise return error code.
 */
static status_t EDGELOCK_EnableRNG(SxMU_Type *base)
{
    status_t status = kStatus_Fail;

    status = EDGELOCK_StartRNG(base);
    if (status == kStatus_Success)
    {
        uint8_t trngState = 0, csalState = 0;
        while (1)
        {
            status = EDGELOCK_GetTRNGState(base, &trngState, &csalState);
            if (status != kStatus_Success)
            {
                break;
            }

            if ((trngState == TRNG_STATE_ERROR) || (csalState == CSAL_STATE_ERROR))
            {
                status = kStatus_Fail;
                break;
            }

            if (csalState == CSAL_STATE_COMPLETED)
            {
                status = kStatus_Success;
                break;
            }
        }
    }

    return status;
}

/*!
 * @brief Generate IEE key blob.
 *
 * @param base Edgelock Message Unit base.
 * @param keyData Address where the input key data can be found.
 * @param keySize Size of keyData.
 * @param keySel Index of IEE.
 * @param keyBlob Address where the key blob can be stored.
 * @param keyBlobSize available key blob data in bytes.
 * @return 0 for success, otherwise return error code.
 */
static status_t EDGELOCK_GenerateIEEKeyblob(SxMU_Type *base,
                                            const uint8_t *keyData,
                                            const uint32_t keySize,
                                            const int32_t keySel,
                                            uint8_t *keyBlob,
                                            uint32_t *keyBlobSize)
{
    iee_keyblob_block_t *pKeyData = (iee_keyblob_block_t *)keyData;
    status_t status;

    /* */
    SDK_ALIGN(static uint8_t s_keyDataBlock[KEY_BLOB_IEE_KEY_DATA_SIZE], KEY_BLOB_ALIGN_SIZE);
    SDK_ALIGN(static uint8_t s_keyBlobBlock[KEY_BLOB_IEE_EXPORT_SIZE], KEY_BLOB_ALIGN_SIZE);

    if (((keySel != KEY_BLOB_KEYSEL_IEE) ||
         (keySize != (KEY_BLOB_IEE_KEY_DATA_SIZE + KEY_BLOB_IEE_BLOCK_HEADER_SIZE))) ||
        (!keyData) || (!keyBlob) || (!keyBlobSize) || (pKeyData->tag != KEY_BLOB_TAG_IEE))
    {
        return kStatus_InvalidArgument;
    }

    uint32_t xorCheck = 0U;
    for (uint32_t *pKeyblobWord = (uint32_t *)pKeyData; pKeyblobWord < (uint32_t *)&(pKeyData->xorCheck);
         pKeyblobWord++)
    {
        xorCheck ^= *pKeyblobWord;
    }

    if (((pKeyData->regionStart & (~IEE_APC_REGION0_BOT_ADDR_BOT_ADDR_MASK)) != 0U) ||
        ((pKeyData->regionEnd & (~IEE_APC_REGION0_TOP_ADDR_TOP_ADDR_MASK)) != 0U) || (xorCheck != pKeyData->xorCheck))
    {
        return kStatus_InvalidArgument;
    }

    *keyBlobSize = 0;

    memcpy(s_keyDataBlock, keyData + KEY_BLOB_IEE_BLOCK_HEADER_SIZE, sizeof(s_keyDataBlock));
    memset(s_keyBlobBlock, 0, sizeof(s_keyBlobBlock));

    /* Check the key block header. */
    generate_keyblob_block_t *pDataBlock = (generate_keyblob_block_t *)s_keyDataBlock;
    if ((pDataBlock->version != 0U) || (pDataBlock->length != sizeof(s_keyDataBlock)) ||
        (pDataBlock->tag != KEY_BLOB_TAG_PRIVATEKEY) || (pDataBlock->flags != KEY_BLOB_FLAGS_IEE) ||
        ((pDataBlock->algorithm != KEY_BLOB_ALGORITHM_AESXTS) && (pDataBlock->algorithm != KEY_BLOB_ALGORITHM_AESCTR)))
    {
        return kStatus_NoData;
    }

    status = EDGELOCK_EnableRNG(base);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = EDGELOCK_GenerateKeyblob(base, KEY_BLOB_IEE_IDENTIFIER, (uint32_t)s_keyDataBlock, (uint32_t)s_keyBlobBlock,
                                      sizeof(s_keyBlobBlock));
    if (status == kStatus_Success)
    {
        *keyBlobSize = KEY_BLOB_IEE_EXPORT_SIZE + KEY_BLOB_IEE_BLOCK_HEADER_SIZE;
        memcpy(keyBlob, keyData, KEY_BLOB_IEE_BLOCK_HEADER_SIZE);
        memcpy(&keyBlob[KEY_BLOB_IEE_BLOCK_HEADER_SIZE], s_keyBlobBlock, KEY_BLOB_IEE_EXPORT_SIZE);
    }
    else
    {
        *keyBlobSize = 0;
        return status;
    }

    return status;
}

status_t generate_key_blob(const uint8_t *pKeyAddr,
                           const uint32_t keySize,
                           const uint32_t keySel,
                           uint8_t *pKeyBlobAddr,
                           uint32_t *pKeyBlobSize)
{
    status_t status = kStatus_InvalidArgument;

    switch (keySel)
    {
        case KEY_BLOB_KEYSEL_OTFAD1:
        case KEY_BLOB_KEYSEL_OTFAD2:
            status = EDGELOCK_GenerateDukKeyblob(SxMU, pKeyAddr, keySize, keySel, pKeyBlobAddr, pKeyBlobSize);
            break;
        case KEY_BLOB_KEYSEL_IEE:
            status = EDGELOCK_GenerateIEEKeyblob(SxMU, pKeyAddr, keySize, keySel, pKeyBlobAddr, pKeyBlobSize);
            break;
        case KEY_BLOB_KEYSEL_DEK:
            status = EDGELOCK_GenerateDekKeyblob(SxMU, pKeyAddr, keySize, keySel, pKeyBlobAddr, pKeyBlobSize);
            break;
        default:
            break;
    }

    return status;
}
