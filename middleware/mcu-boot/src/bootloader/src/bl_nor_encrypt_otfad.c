/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#define THIS_IS_LOCAL_MODULE_FOR_G_REDUNDANT_OFFSET // This shall be defined before inclusions

#include "bootloader/bl_nor_encrypt.h"
#include "bootloader_config.h"
#include <stdint.h>
#include <stdbool.h>
#include "dcp/fsl_dcp.h"
#include "trng/fsl_trng.h"
#include "crc/crc32.h"

/*******************************************************************************************************************************
 * Type & constant definitions
 *******************************************************************************************************************************/
typedef struct
{
    uint32_t startaddr;
    uint32_t endaddr;
} otfad_context_loc_t;

#define OTFAD_MAX_CONTEXT_COUNT (4)
#define OTFAD_MASK_KEYSRC_HIGH (0x1U)
typedef struct
{
    union
    {
        struct
        {
            uint32_t redundant_offset_256k : 8; //!< Redundant image offset in 256K, 0 for normal image
            uint32_t context_count : 4;         //!< Actual context count of the option
            uint32_t keysrc_high : 4; //!< 0 for SNVS[127:0] as KEK, 1 for SNVS[255:128] as KEK. Undefined otherwise.
            uint32_t reserved : 12;
            uint32_t tag : 4; //!< Tag, fixed to 0x0E for encryption
        } B;
        uint32_t U;
    } option;

    otfad_context_loc_t context[OTFAD_MAX_CONTEXT_COUNT];
} otfad_nor_encrypt_option_t;

#define AES_128_BLOCK_BYTE_SIZE (16)
#define OTFAD_CTX_KEY_BYTE_SIZE (AES_128_BLOCK_BYTE_SIZE)
#define OTFAD_CTX_CTR_BYTE_SIZE (8)

typedef struct
{
    uint8_t key[OTFAD_CTX_KEY_BYTE_SIZE]; // OTFAD context key
    uint8_t ctr[OTFAD_CTX_CTR_BYTE_SIZE]; // OTFAD context initial counter
    uint32_t startaddr;                   // OTFAD context start address
    uint32_t endaddr;                     // OTFAD context end address (last byte address *within* the context)
    uint32_t zero_fill;                   // Padding
    uint32_t key_blob_crc32;              // CRC checksum
    uint8_t expanded_wrap_data[8];        // Space for key wrap
    uint8_t unused_filler[16];
} otfad_keyblob_t;

typedef struct
{
    uint8_t key[OTFAD_CTX_KEY_BYTE_SIZE];
    uint8_t ctr[OTFAD_CTX_CTR_BYTE_SIZE];
    uint32_t startaddr;
    uint32_t endaddr;
} otfad_context_t;

#define ALIGN_256K (256 * 1024)
#define ALIGN_1K (1024)

/*******************************************************************************************************************************
 * Global vars
 *******************************************************************************************************************************/
static otfad_keyblob_t g_otfad_key_blob[OTFAD_MAX_CONTEXT_COUNT];
static uint32_t g_otfad_context_count = 0;
static otfad_context_t g_otfad_context[OTFAD_MAX_CONTEXT_COUNT];

uint32_t g_redundant_offset; // For external read only.
                             // Must be declared as a const in bootloader_config.h

/*******************************************************************************************************************************
 * Prototypes of private functions
 *******************************************************************************************************************************/
static status_t otfad_encrypt_keyblob_keysrc_snvs(otfad_keyblob_t *unencrypted_otfad_keyblob, bool keysrc_high);

/*******************************************************************************************************************************
 * Exported functions
 *******************************************************************************************************************************/
bool bl_nor_encrypt_region_info_valid(void *arg)
{
    if (!arg)
    {
        return false;
    }
    otfad_nor_encrypt_option_t *enc_option = (otfad_nor_encrypt_option_t *)arg;
    if (kNorEncyptOption_Tag != enc_option->option.B.tag)
    {
        return false;
    }
    if ((~OTFAD_MASK_KEYSRC_HIGH) & enc_option->option.B.keysrc_high)
    {
        return false;
    }
    if ((enc_option->option.B.context_count > OTFAD_MAX_CONTEXT_COUNT) || (0 == enc_option->option.B.context_count))
    {
        return false;
    }
    for (int index = 0; index < (enc_option->option.B.context_count); index++)
    {
        if ((enc_option->context[index].startaddr & (ALIGN_1K - 1)) ||
            ((enc_option->context[index].endaddr + 1) & (ALIGN_1K - 1)))
        {
            return false;
        }
    }
    return true;
}

status_t bl_nor_encrypt_init(void *arg)
{
    if (!bl_nor_encrypt_region_info_valid(arg))
    {
        return kStatus_InvalidArgument;
    }
    otfad_nor_encrypt_option_t *enc_option = (otfad_nor_encrypt_option_t *)arg;

    // reset otfad configurations
    memset((void *)g_otfad_key_blob, 0, sizeof(g_otfad_key_blob));
    memset((void *)g_otfad_context, 0, sizeof(g_otfad_context));
    g_otfad_context_count = 0;
    g_redundant_offset = 0;

    // fill in new configurations
    trng_config_t trng_config; // no initialization, saving space
    status_t status = TRNG_GetDefaultConfig(&trng_config);
    if (kStatus_Success != status)
    {
        return status;
    }
    trng_config.sampleMode = kTRNG_SampleModeVonNeumann;
    status = TRNG_Init(TRNG, &trng_config);
    if (kStatus_Success != status)
    {
        return status;
    }
    for (int index = 0; index < enc_option->option.B.context_count; index++)
    {
        status = TRNG_GetRandomData(TRNG, &(g_otfad_key_blob[index].key), OTFAD_CTX_KEY_BYTE_SIZE);
        if (kStatus_Success != status)
        {
            return status;
        }
        status = TRNG_GetRandomData(TRNG, &(g_otfad_key_blob[index].ctr), OTFAD_CTX_CTR_BYTE_SIZE);
        if (kStatus_Success != status)
        {
            return status;
        }
        g_otfad_key_blob[index].startaddr = enc_option->context[index].startaddr;
        g_otfad_key_blob[index].endaddr = enc_option->context[index].endaddr;
        // store vital info for data encryption
        memcpy((void *)&(g_otfad_context[index].key), (const void *)&(g_otfad_key_blob[index].key),
               OTFAD_CTX_KEY_BYTE_SIZE);
        memcpy((void *)&(g_otfad_context[index].ctr), (const void *)&(g_otfad_key_blob[index].ctr),
               OTFAD_CTX_CTR_BYTE_SIZE);
        g_otfad_context[index].startaddr = g_otfad_key_blob[index].startaddr;
        g_otfad_context[index].endaddr = g_otfad_key_blob[index].endaddr;
    }
    // store vital info for data encryption
    g_otfad_context_count = enc_option->option.B.context_count;
    g_redundant_offset = enc_option->option.B.redundant_offset_256k * ALIGN_256K;
    // encrypt keyblob
    return otfad_encrypt_keyblob_keysrc_snvs(g_otfad_key_blob, enc_option->option.B.keysrc_high);
}

bool bl_nor_encrypt_has_encrypted_region(void)
{
    return g_otfad_context_count ? true : false;
}

// index: 0 for normal config block, 1 for redundant block
status_t bl_nor_encrypt_get_config_block(uint32_t index, uint32_t *start, uint32_t *bytes)
{
    if ((!start) || (!bytes))
    {
        return kStatus_InvalidArgument;
    }

    if ((!g_redundant_offset) == index) // Binarize g_redundant_offset
    {
        return kStatus_InvalidArgument;
    }

    (*start) = (uint32_t)&g_otfad_key_blob;
    (*bytes) = sizeof(g_otfad_key_blob);

    return kStatus_Success;
}

bool bl_nor_in_encrypted_region(uint32_t start, uint32_t bytes)
{
    start -= g_redundant_offset; // g_redundant_offset_256k * ALIGN_256K
    for (int index = 0; index < g_otfad_context_count; index++)
    {
        if (g_otfad_context[index].startaddr <= start && g_otfad_context[index].endaddr >= (start + bytes - 1))
        {
            return true;
        }
    }
    return false;
}

status_t bl_nor_encrypt_data(uint32_t addr, uint32_t size, uint32_t *data_start)
{
    if ((0 == size) || (!data_start))
    {
        return kStatus_InvalidArgument;
    }
    if (((AES_128_BLOCK_BYTE_SIZE - 1) & addr) || ((AES_128_BLOCK_BYTE_SIZE - 1) & size)) // Alignment check
    {
        return kStatus_InvalidArgument;
    }

    otfad_context_t *context = NULL;
    addr -= g_redundant_offset; // g_redundant_offset_256k * ALIGN_256K
    for (int index = 0; index < g_otfad_context_count; index++)
    {
        if (g_otfad_context[index].startaddr <= addr && g_otfad_context[index].endaddr >= (addr + size - 1))
        {
            context = &(g_otfad_context[index]);
            break;
        }
    }

    if (!context)
    {
        return kStatus_OutOfRange;
    }

    // Init encryptor
    dcp_alg_ctx_t dcp_ctx; // no initialization, saving space
    if (kAesOperation_Success != dcp_aes_init(&dcp_ctx))
    {
        return kStatus_Fail;
    }

    // Set encryption key
    const aes_key_sel_t key_sel = {.key = context->key };
    dcp_aes_set_key(&dcp_ctx, key_sel, 128);

    // Set nounce
    uint8_t nounce[AES_128_BLOCK_BYTE_SIZE]; // no initialization, saving space
    memcpy((void *)nounce, (const void *)context->ctr, OTFAD_CTX_CTR_BYTE_SIZE);
    *((uint32_t *)(nounce + 8)) = *((const uint32_t *)(context->ctr + 0)) ^ *((const uint32_t *)(context->ctr + 4));
    nounce[12] = ((const uint8_t *)(&addr))[3];
    nounce[13] = ((const uint8_t *)(&addr))[2];
    nounce[14] = ((const uint8_t *)(&addr))[1];
    nounce[15] = ((const uint8_t *)(&addr))[0];

    // Encryption
    uint8_t encrypted_nounce[AES_128_BLOCK_BYTE_SIZE];
    for (size_t offset = 0; offset <= size; offset += AES_128_BLOCK_BYTE_SIZE)
    {
        if (kAesOperation_Success !=
            dcp_aes_ecb_crypt(&dcp_ctx, kAesMode_Encrypt, nounce, encrypted_nounce, AES_128_BLOCK_BYTE_SIZE))
        {
            return kStatus_Fail;
        }

        int32_t byte_index; // Signed type shall be used
        for (byte_index = 0; byte_index < AES_128_BLOCK_BYTE_SIZE; byte_index++)
        {
            ((uint8_t *)data_start)[offset + byte_index] ^= encrypted_nounce[byte_index];
        }

        for (byte_index = AES_128_BLOCK_BYTE_SIZE - 1; byte_index >= 0; byte_index--)
        {
            nounce[byte_index] +=
                AES_128_BLOCK_BYTE_SIZE; // Vital: add 16, which is why dcp_aes_ctr_crypt() is not applied
            if (0 != nounce[byte_index]) // Break if no carry
            {
                break;
            }
        }
    }

    return kStatus_Success;
}

void bl_nor_encrypt_region_refresh(uint32_t start, uint32_t bytes)
{
    const uint32_t keyblob_base = BL_FLEXSPI_AMBA_BASE + g_redundant_offset; // g_redundant_offset_256k * ALIGN_256K
    if ((keyblob_base > start - sizeof(g_otfad_key_blob)) && (keyblob_base < start + bytes))
    {
        // reset otfad configurations
        memset((void *)g_otfad_key_blob, 0, sizeof(g_otfad_key_blob));
        memset((void *)g_otfad_context, 0, sizeof(g_otfad_context));
        g_otfad_context_count = 0;
        g_redundant_offset = 0;
    }
}

/*******************************************************************************************************************************
 * Private functions
 *******************************************************************************************************************************/
status_t otfad_encrypt_keyblob_keysrc_snvs(otfad_keyblob_t *unencrypted_otfad_keyblob, bool keysrc_high)
{
#define N 5 // Round num as is defined by RFC3394, for consistency with image_enc

    if (!unencrypted_otfad_keyblob)
    {
        return kStatus_InvalidArgument;
    }

    // RFC3394 key wrap scheme
    uint8_t r[8 * (N + 1)];
    uint8_t aes_block_128b[16];

#define plaintext ((uint8_t *)(unencrypted_otfad_keyblob + context_index))          // for consistency with image_enc
#define wrapped_ciphertext ((uint8_t *)(unencrypted_otfad_keyblob + context_index)) // for consistency with image_enc

    for (int context_index = 0; context_index < OTFAD_MAX_CONTEXT_COUNT; context_index++)
    {
        crc32_data_t crc32Config; // no initialization, saving space
        crc32_init(&crc32Config);
        crc32_update(&crc32Config, plaintext, 32);
        crc32_finalize(&crc32Config, &(unencrypted_otfad_keyblob[context_index].key_blob_crc32));

        dcp_alg_ctx_t ctx;
        if (kAesOperation_Success != dcp_aes_init(&ctx))
        {
            return kStatus_Fail;
        }

        uint8_t a[8] = { 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6 };

        for (int j = 1; j <= N; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                r[8 * j + k] = plaintext[8 * (j - 1) + k];
            }
        }

        // step 2: calculate intermediate values
        // for i = 0 to 5
        //     for j = 1 to n
        //         B = AES(K, A | R[j])
        //         A = MSB(64, B) ^ (n*i)+j
        //         R[j] = LSB(64, B)

        for (int i = 0; i <= 5; i++)
        {
            for (int j = 1; j <= N; j++)
            {
                for (int k = 0; k < 8; k++)
                {
                    aes_block_128b[k] = a[k];
                    aes_block_128b[k + 8] = r[8 * j + k];
                }

                const aes_key_sel_t key_sel = {
                    .option = (OTFAD_MASK_KEYSRC_HIGH & keysrc_high) ?
                                  OTPMK_SNVS_HIGH_FLAG_LE :
                                  OTPMK_SNVS_LOW_FLAG_LE // only keysrc_high's LSB (bit) is concerned
                };
                if (kAesOperation_Success != dcp_aes_set_key(&ctx, key_sel, 128))
                {
                    return kStatus_Fail;
                }
                if (kAesOperation_Success !=
                    dcp_aes_ecb_crypt(&ctx, kAesMode_Encrypt, aes_block_128b, aes_block_128b, 16))
                {
                    return kStatus_Fail;
                }

                for (int k = 0; k < 8; k++)
                {
                    a[k] = aes_block_128b[k];
                    r[8 * j + k] = aes_block_128b[8 + k];
                }
                a[7] ^= ((N * i) + j);
            } // end for (j)
        }     // end for (i)

        // step 3: output the results
        // set C[0] = A
        // for i = 1 to n
        //     C[i] = R[i]

        for (int k = 0; k < 8; k++)
        {
            wrapped_ciphertext[k] = a[k];
        }

        for (int j = 1; j <= N; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                wrapped_ciphertext[8 * j + k] = r[8 * j + k];
            }
        }
    }

    return kStatus_Success;
}
