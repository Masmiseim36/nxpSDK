/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "fsl_device_registers.h"
#include "dcp/fsl_dcp.h"
#include "trng/fsl_trng.h"
#include "bootloader/bl_keyblob.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
//!@brief KeyBlob Header definition
typedef struct hab_hdr
{
    uint8_t tag;    //!< KeyBlob block tag, fixed to 0x81
    uint8_t len[2]; //!<  KeyBlob length, length = len[0] * 256 + len[1]
    uint8_t par;    //!< Version, 0x42 or 0x43.
} hab_hdr_t;

//!@brief Parameters for KeyBlob Block
enum
{
    kKeyblob_Tag = 0x81,
    kKeyBlob_Version = 0x43,
    kKeyBlob_BKSize = 16,
    kKeyBlob_DekSize = 16,
    kKeyBlob_MacSize = 16,
};

//!@brief AEAD Header definition
typedef struct aead_hdr
{
    uint8_t mode;      //!< KeyBlob authentication mode
    uint8_t alg;       //!< Keyblob encryption/decryption algorithm
    uint8_t mac_bytes; //!< MAC bytes
    uint8_t aad_bytes; //!< AEAD bytes
} aead_hdr_t;

//!@brief Paremeter values for AEAD Header
enum
{
    kAEAD_Mode = 0x66,     //!< CCM mode
    kAEAD_Alg = 0x55,      //!< AES algorithm
    kAEAD_MacBytes = 0x10, //!< MAC size is fixed to 16
    kAEAD_AadBytes = 0,    //!< AAD bytes is fiexed to 0
};

//!@brief KeyBlob Block definition
typedef struct
{
    hab_hdr_t hdr;                                     //!< KeyBlob header
    aead_hdr_t aead;                                   //!< AEAD heder
    uint32_t bk[kKeyBlob_BKSize / sizeof(uint32_t)];   //!< Blob Key field
    uint32_t dek[kKeyBlob_DekSize / sizeof(uint32_t)]; //!< DEK field
    uint32_t mac[kKeyBlob_MacSize / sizeof(uint32_t)]; //!< Mac Field
} keyblob_block_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Codes
////////////////////////////////////////////////////////////////////////////////
int32_t generate_key_blob(uint32_t *key_addr, uint8_t *key_blob_addr)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((key_addr == NULL) || (key_blob_addr == NULL))
        {
            break;
        }

        keyblob_block_t *keyblob = (keyblob_block_t *)key_blob_addr;

        memset(keyblob, 0, sizeof(keyblob_block_t));
        // Prepare Header
        keyblob->hdr.len[1] = sizeof(keyblob_block_t);
        keyblob->hdr.par = kKeyBlob_Version;
        keyblob->hdr.tag = kKeyblob_Tag;
        // Prepare AEAD
        keyblob->aead.mode = kAEAD_Mode;
        keyblob->aead.alg = kAEAD_Alg;
        keyblob->aead.mac_bytes = kAEAD_MacBytes;
        keyblob->aead.aad_bytes = kAEAD_AadBytes;

        // Generate Ramdom BK
        trng_config_t trng_config;
        status = TRNG_GetDefaultConfig(&trng_config);
        if (status != kStatus_Success)
        {
            break;
        }
        // Set sample mode of the TRNG ring oscillator to Von Neumann, for better random data.
        trng_config.sampleMode = kTRNG_SampleModeVonNeumann;
        status = TRNG_Init(TRNG, &trng_config);
        if (status != kStatus_Success)
        {
            break;
        }
        status = TRNG_GetRandomData(TRNG, &keyblob->bk, kKeyBlob_BKSize);
        if (status != kStatus_Success)
        {
            break;
        }

        // Encrypt DEK using BK
        uint8_t nonce[13];
        dcp_alg_ctx_t dcp_ctx;
        aes_key_sel_t key_sel;
        memset(nonce, 0, sizeof(nonce));
        dcp_aes_init(&dcp_ctx);
        key_sel.key = (uint8_t *)&keyblob->bk[0];
        dcp_aes_set_key(&dcp_ctx, key_sel, 128);
        dcp_aes_ccm_encrypt_and_mac(&dcp_ctx, kKeyBlob_DekSize, nonce, sizeof(nonce), NULL, kAEAD_AadBytes,
                                    (const uint8_t *)key_addr, (uint8_t *)&keyblob->dek[0], (uint8_t *)&keyblob->mac[0],
                                    kKeyBlob_MacSize);

        // Encrypt BK using key derived from OTPMK
        key_sel.option = OTPMK_SNVS_HIGH_FLAG_LE;
        dcp_aes_set_key(&dcp_ctx, key_sel, 128);
        dcp_aes_ecb_crypt(&dcp_ctx, kAesMode_Encrypt, (uint8_t *)&keyblob->bk[0], (uint8_t *)&keyblob->bk[0],
                          kKeyBlob_BKSize);

        status = kStatus_Success;

    } while (0);

    return status;
}
