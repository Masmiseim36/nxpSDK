/*
 * Copyright (c) 2014-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "utilities/fsl_assert.h"
#include "fsl_otfad_driver.h"
#include "security/aes128_key_wrap_unwrap.h"
#include "crc/crc32.h"
#include "security/aes_security.h"

#if BL_FEATURE_OTFAD_MODULE

//! @addtogroup otfad
//! @{

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief Constants
enum _constants
{
    kAesKeySizeBytes = 16,                                    //!< Number of bytes in AES-128 key
    kCtrSizeBytes = 8,                                        //!< Number of bytes in Ctr
    kCrc32SizeBytes = 32,                                     //!< Number of bytes covered by CRC32
    kNumKeyBlobs = 4,                                         //!< Number of key blobs
    kKeyBlobSizeBytes = 64,                                   //!< Key blob size in bytes
    kKeyBlobArraySizeBytes = kKeyBlobSizeBytes * kNumKeyBlobs //!< Key blob array size in bytes
};

//! @brief Key Blob format.
typedef struct KeyBlob
{
    uint8_t key[kAesKeySizeBytes]; // 16 bytes, 128-bits, KEY[A15...A00]
    uint8_t ctr[kCtrSizeBytes];    // 8 bytes, 64-bits, CTR[C7...C0]
    uint32_t srtaddr;              // region start, STRADDR[31 - 10]
    uint32_t endaddr;              // region end, ENDADDR[31 - 10] + flags R0,ADE,VLD
                                   // end of 32-byte area covered by CRC
    uint32_t zero_fill;            // zeros
    uint32_t key_blob_crc32;       // crc32 over 1st 32-bytes
                                   // end of 40 byte (5*64-bit) key blob data
    uint8_t expanded_wrap_data[8]; // 8 bytes, used for wrap expanded data
                                   // end of 48 byte (6*64-bit) wrap data
    uint8_t unused_filler[16];     // unused fill to 64 bytes
} keyblob_t;

typedef struct KeyBlobUnwrap
{
    uint8_t ivHeader[8]; // 8 bytes that are added during the wrap for an unwrap error check (ivHeader == wrapIV)
    keyblob_t keyBlob;   // The actual keyblob
} keyblob_data_t;

////////////////////////////////////////////////////////////////////////////////
// Local function prototypes
////////////////////////////////////////////////////////////////////////////////

static void program_contexts(OTFAD_Type *baseAddr, uint8_t *keyBlobs, uint32_t *kek);
static bool validate_crc(const keyblob_t *keyBlob);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Instance base addresses.
const uint32_t g_otfadBaseAddr[] = OTFAD_BASE_ADDRS;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

//! @brief Validate CRC32 in key blob.
//!
//! @param keyBlob Pointer to key blob
//! @return True if CRC matches, False otherwise
bool validate_crc(const keyblob_t *keyBlob)
{
    assert(keyBlob);
    crc32_data_t crc32Data;

    crc32_init(&crc32Data);
    crc32_update(&crc32Data, (const uint8_t *)keyBlob, kCrc32SizeBytes);
    uint32_t hash;
    crc32_finalize(&crc32Data, &hash);
    return hash == keyBlob->key_blob_crc32;
}

//! @brief Program context regiters with information from key blobs.
//!
//! Key blobs are unwrapped first. Each blob is validated with CRC32.
//!
//! @param baseAddr OTFAD module base address
//! @param keyBlobs pointer to array of 4 key blobs
//! @param kek KeK words
static void program_contexts(OTFAD_Type *baseAddr, uint8_t *keyBlobs, uint32_t *kek)
{
    assert(kek);
    assert(keyBlobs);

    uint32_t numContexts = otfad_hal_get_number_of_contexts(baseAddr);
    keyblob_data_t unwrappedKeyBlob;

    for (uint32_t ctx = 0; ctx < numContexts; ++ctx)
    {
        uint32_t status = do_aes128_key_unwrap(keyBlobs, (uint8_t *)&unwrappedKeyBlob, kek);
        if (status == 0)
        {
            if (!validate_crc(&unwrappedKeyBlob.keyBlob))
            {
                // Ignore this key blob and move to the next one.
                keyBlobs += sizeof(keyblob_t);
                continue;
            }

            // Program context registers from unwrapped keyblob.
            const otfad_context_info_t *contextInfo = (const otfad_context_info_t *)&unwrappedKeyBlob.keyBlob;
            otfad_hal_set_context(baseAddr, ctx, contextInfo);
        }

        // Advance to next wrapped key blob.
        keyBlobs += sizeof(keyblob_t);
    }

    // Clear unwrapped key blob memory on stack.
    memset(&unwrappedKeyBlob, 0, sizeof(unwrappedKeyBlob));
}

// See fsl_otfd_driver.h for documentation on this function.
status_t otfad_init(uint32_t instance, const uint8_t *keyBlobs, const otfad_kek_t *otfadKek)
{
    assert(instance < FSL_FEATURE_SOC_OTFAD_COUNT);
    uint32_t baseAddr = g_otfadBaseAddr[instance];
    assert(keyBlobs);
    assert(otfadKek);

    // Make local copies of keyBlobs and Kek.
    static uint8_t blobs[kKeyBlobArraySizeBytes];
    static uint32_t kek[AES_BLOCK_SIZE_WORDS];
    memcpy(blobs, keyBlobs, sizeof(blobs));
    memcpy(kek, otfadKek, sizeof(kek));

    aes_init(kek);

    // Get the operational mode.
    uint32_t opMode = otfad_hal_get_mode((OTFAD_Type *)baseAddr);

    if (opMode == kOtfadMode_Normal)
    {
        // It the module is access restricted it cannot be programmed.
        if (otfad_hal_is_register_access_restricted((OTFAD_Type *)baseAddr))
        {
            return kStatus_OtfadLogicallyDisabled;
        }

        program_contexts((OTFAD_Type *)baseAddr, blobs, kek);

        // Enable decryption.
        otfad_hal_global_enable((OTFAD_Type *)baseAddr);
        if (!otfad_hal_is_enabled((OTFAD_Type *)baseAddr))
        {
            // Could not enable module.
            otfad_hal_restricted_register_access_enable((OTFAD_Type *)baseAddr);
            return kStatus_OtfadLogicallyDisabled;
        }

        // Restrict access to context registers so software can't read clear keys.
        otfad_hal_restricted_register_access_enable((OTFAD_Type *)baseAddr);

        return kStatus_Success;
    }
    else if (opMode == kOtfadMode_SecurityViolation)
    {
        return kStatus_OtfadSecurityViolation;
    }
    else
    {
        return kStatus_OtfadLogicallyDisabled;
    }
}

// See fsl_otfd_driver.h for documentation on this function.
status_t otfad_bypass(uint32_t instance)
{
    assert(instance < FSL_FEATURE_SOC_OTFAD_COUNT);
    uint32_t baseAddr = g_otfadBaseAddr[instance];

    otfad_hal_global_disable((OTFAD_Type *)baseAddr);

    return kStatus_Success;
}

// See fsl_otfd_driver.h for documentation on this function.
status_t otfad_resume(uint32_t instance)
{
    assert(instance < FSL_FEATURE_SOC_OTFAD_COUNT);
    uint32_t baseAddr = g_otfadBaseAddr[instance];

    otfad_hal_global_enable((OTFAD_Type *)baseAddr);

    return kStatus_Success;
}

//! @}

#endif // BL_FEATURE_OTFAD_MODULE

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
