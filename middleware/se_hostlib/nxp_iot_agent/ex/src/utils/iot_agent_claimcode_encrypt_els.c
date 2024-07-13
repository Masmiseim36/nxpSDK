/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "fsl_common.h"
#include "iot_agent_claimcode_encrypt.h"

#include "fsl_device_registers.h"
#include "nxp_iot_agent_status.h"
#include "mcuxClEls_KeyManagement.h"
#include "mcuxClEls_Rng.h"
#include "mcuxClAes.h"
#include "mcuxClEls_Ecc.h"
#include "mcuxClEls_Kdf.h"
#include "mcuxClEls_Cipher.h"
#include "mcuxClEls_Cmac.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#ifdef __ZEPHYR__
#include <zephyr/drivers/hwinfo.h>
#include <stdio.h>
#define LOG printf
#else
#include <fsl_silicon_id.h>
#include <fsl_debug_console.h>
#define LOG PRINTF
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define UID_FUSE_IDX   46U
#define UID_SIZE       16U
#define IV_SIZE        16U
#define AES_BLOCK_SIZE 16U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static const mcuxClEls_KeyProp_t keypair_prop = {
    .bits = 
    {
        .upprot_priv    = MCUXCLELS_KEYPROPERTY_PRIVILEGED_TRUE,
        .upprot_sec     = MCUXCLELS_KEYPROPERTY_SECURE_TRUE,
        .ksize          = MCUXCLELS_KEYPROPERTY_KEY_SIZE_256,
    }
};

static const mcuxClEls_KeyProp_t shared_secret_prop = {
    .bits =
        {
            .upprot_priv = MCUXCLELS_KEYPROPERTY_PRIVILEGED_TRUE,
            .upprot_sec  = MCUXCLELS_KEYPROPERTY_SECURE_TRUE,
            .uckdf       = MCUXCLELS_KEYPROPERTY_CKDF_TRUE,
            .ksize       = MCUXCLELS_KEYPROPERTY_KEY_SIZE_128,
        },
};

static const mcuxClEls_KeyProp_t enc_key_prop = {
    .bits =
        {
            .upprot_priv = MCUXCLELS_KEYPROPERTY_PRIVILEGED_TRUE,
            .upprot_sec  = MCUXCLELS_KEYPROPERTY_SECURE_TRUE,
            .uaes        = MCUXCLELS_KEYPROPERTY_AES_TRUE,
            .ksize       = MCUXCLELS_KEYPROPERTY_KEY_SIZE_128,
            .kactv       = MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE,
        },
};

static const uint8_t ckdf_derivation_data_enc[12] = {
    0x43, 0x43, 0x5f, 0x45, 0x4E, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const mcuxClEls_KeyProp_t mac_key_prop = {
    .bits =
        {
            .upprot_priv = MCUXCLELS_KEYPROPERTY_PRIVILEGED_TRUE,
            .upprot_sec  = MCUXCLELS_KEYPROPERTY_SECURE_TRUE,
            .ucmac       = MCUXCLELS_KEYPROPERTY_CMAC_TRUE,
            .ksize       = MCUXCLELS_KEYPROPERTY_KEY_SIZE_128,
            .kactv       = MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE,
        },
};

static const uint8_t ckdf_derivation_data_mac[12] = {
    0x43, 0x43, 0x5f, 0x4d, 0x41, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define PLOG_DEBUG(...)
#define PLOG_INFO(...)  LOG(__VA_ARGS__)
#define PLOG_ERROR(...) LOG(__VA_ARGS__)

static const char nibble_to_char[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};

static void printf_buffer(const char *name, const unsigned char *buffer, size_t size)
{
#define PP_BYTES_PER_LINE (32)
    char line_buffer[PP_BYTES_PER_LINE * 2 + 2];
    const unsigned char *pos = buffer;
    size_t remaining         = size;
    while (remaining > 0)
    {
        size_t block_size = remaining > PP_BYTES_PER_LINE ? PP_BYTES_PER_LINE : remaining;
        uint32_t len      = 0;
        for (size_t i = 0; i < block_size; i++)
        {
            line_buffer[len++] = nibble_to_char[((*pos) & 0xf0) >> 4];
            line_buffer[len++] = nibble_to_char[(*pos++) & 0x0f];
        }
        line_buffer[len++] = '\n';
        line_buffer[len++] = '\0';
        LOG("%s (0x%p): %s", name, pos, line_buffer);
        remaining -= block_size;
    }
}

static size_t ceil_to_aes_blocksize(size_t size)
{
    return ((size + (AES_BLOCK_SIZE - 1)) & (~(AES_BLOCK_SIZE - 1)));
}

static iot_agent_status_t read_uid(uint8_t *uid)
{
    uint32_t uid_len = UID_SIZE;
#ifdef __ZEPHYR__
    hwinfo_get_device_id(uid, uid_len);
#else
    SILICONID_GetID(uid, &uid_len);
#endif
    return IOT_AGENT_SUCCESS;
}

static bool is_active_keyslot(mcuxClEls_KeyIndex_t keyIdx)
{
    mcuxClEls_KeyProp_t key_properties;
    key_properties.word.value = ((const volatile uint32_t *)(&ELS->ELS_KS0))[keyIdx];
    return key_properties.bits.kactv;
}

static inline uint32_t get_required_keyslots(mcuxClEls_KeyProp_t prop)
{
    return prop.bits.ksize == MCUXCLELS_KEYPROPERTY_KEY_SIZE_128 ? 1 : 2;
}

static mcuxClEls_KeyIndex_t get_free_keyslot(uint32_t required_keyslots)
{
    for (mcuxClEls_KeyIndex_t keyIdx = 0; keyIdx <= (MCUXCLELS_KEY_SLOTS - required_keyslots); keyIdx++)
    {
        bool is_valid_keyslot = true;
        for (uint32_t i = 0; i < required_keyslots; i++)
        {
            if (is_active_keyslot(keyIdx + i))
            {
                is_valid_keyslot = false;
                break;
            }
        }

        if (is_valid_keyslot)
        {
            return keyIdx;
        }
    }
    return MCUXCLELS_KEY_SLOTS;
}

static iot_agent_status_t generate_keypair(mcuxClEls_KeyIndex_t *dst_key_index,
                                           uint8_t *public_key,
                                           size_t *public_key_size)
{
    if (*public_key_size < 64)
    {
        PLOG_ERROR("insufficient space for public key");
        return IOT_AGENT_FAILURE;
    }

    mcuxClEls_EccKeyGenOption_t options = {0};
    options.bits.kgsrc                  = MCUXCLELS_ECC_OUTPUTKEY_RANDOM;
    options.bits.kgtypedh               = MCUXCLELS_ECC_OUTPUTKEY_KEYEXCHANGE;

    uint32_t keypair_required_keyslots = get_required_keyslots(keypair_prop);
    *dst_key_index                     = (mcuxClEls_KeyIndex_t)get_free_keyslot(keypair_required_keyslots);

    if (!(*dst_key_index < MCUXCLELS_KEY_SLOTS))
    {
        PLOG_ERROR("no free keyslot found");
        return IOT_AGENT_FAILURE;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClEls_EccKeyGen_Async(options, (mcuxClEls_KeyIndex_t)0U, *dst_key_index, keypair_prop, NULL, public_key));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_EccKeyGen_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        PLOG_ERROR("mcuxClEls_EccKeyGen_Async failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        PLOG_ERROR("mcuxClEls_WaitForOperation failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    *public_key_size = 64;
    return IOT_AGENT_SUCCESS;
}

static iot_agent_status_t perform_key_agreement(mcuxClEls_KeyIndex_t keypair_index,
                                                mcuxClEls_KeyProp_t shared_secret_prop,
                                                mcuxClEls_KeyIndex_t *dst_key_index,
                                                const uint8_t *public_key,
                                                size_t public_key_size)
{
    uint32_t shared_secret_required_keyslots = get_required_keyslots(shared_secret_prop);
    *dst_key_index                           = get_free_keyslot(shared_secret_required_keyslots);

    if (!(*dst_key_index < MCUXCLELS_KEY_SLOTS))
    {
        PLOG_ERROR("no free keyslot found");
        return IOT_AGENT_FAILURE;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token, mcuxClEls_EccKeyExchange_Async(keypair_index, public_key, *dst_key_index, shared_secret_prop));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_EccKeyExchange_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        PLOG_ERROR("mcuxClEls_EccKeyExchange_Async failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        PLOG_ERROR("mcuxClEls_WaitForOperation failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    return IOT_AGENT_SUCCESS;
}

static iot_agent_status_t derive_key(mcuxClEls_KeyIndex_t src_key_index,
                                     mcuxClEls_KeyProp_t key_prop,
                                     const uint8_t *dd,
                                     mcuxClEls_KeyIndex_t *dst_key_index)
{
    uint32_t required_keyslots = get_required_keyslots(key_prop);

    *dst_key_index = get_free_keyslot(required_keyslots);

    if (!(*dst_key_index < MCUXCLELS_KEY_SLOTS))
    {
        PLOG_ERROR("no free keyslot found");
        return IOT_AGENT_FAILURE;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token,
                                     mcuxClEls_Ckdf_Sp800108_Async(src_key_index, *dst_key_index, key_prop, dd));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Ckdf_Sp800108_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        PLOG_ERROR("mcuxClEls_Ckdf_Sp800108_Async failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        PLOG_ERROR("mcuxClEls_WaitForOperation failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();
    return IOT_AGENT_SUCCESS;
}

static iot_agent_status_t delete_key(mcuxClEls_KeyIndex_t key_index)
{
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_KeyDelete_Async(key_index));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_KeyDelete_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        PLOG_ERROR("mcuxClEls_KeyDelete_Async failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        PLOG_ERROR("mcuxClEls_WaitForOperation failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();
    return IOT_AGENT_SUCCESS;
}

static iot_agent_status_t generate_iv(uint8_t *iv)
{
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClCss_Rng_DrbgRequest_Async(iv, IV_SIZE));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_Rng_DrbgRequest_Async) != token) ||
        (MCUXCLCSS_STATUS_OK_WAIT != result))
    {
        PLOG_ERROR("mcuxClCss_Rng_DrbgRequest_Async failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        PLOG_ERROR("mcuxClEls_WaitForOperation failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();
    return IOT_AGENT_SUCCESS;
}

static iot_agent_status_t encrypt_claimcode(mcuxClEls_KeyIndex_t shared_secret_index,
                                            const char *plain_claimcode,
                                            const uint8_t *iv,
                                            uint8_t *encrypted_claimcode)
{
    uint8_t padded_plain_claimcode[0x70] = {0U}; // Max claimcode length is 100 characters.
    size_t plain_claimcode_len           = strlen(plain_claimcode);
    size_t encrypted_claimcode_len       = ceil_to_aes_blocksize(plain_claimcode_len + 1);
    mcuxClEls_KeyIndex_t enc_key_index   = 0U;
    iot_agent_status_t agent_status =
        derive_key(shared_secret_index, enc_key_prop, ckdf_derivation_data_enc, &enc_key_index);
    if (IOT_AGENT_SUCCESS != agent_status)
    {
        PLOG_ERROR("derive_key failed: 0x%08x\n", agent_status);
        return IOT_AGENT_FAILURE;
    }

    mcuxClEls_CipherOption_t cipher_options = {0U};
    cipher_options.bits.cphmde              = MCUXCLELS_CIPHERPARAM_ALGORITHM_AES_CBC;
    cipher_options.bits.dcrpt               = MCUXCLELS_CIPHER_ENCRYPT;
    cipher_options.bits.extkey              = MCUXCLELS_CIPHER_INTERNAL_KEY;

    memcpy(padded_plain_claimcode, plain_claimcode, plain_claimcode_len);
    padded_plain_claimcode[plain_claimcode_len] = 0x80;

    // The ELS will not write to the location of the IV with the given ciper options, therefore it is safe to cast away
    // the const here.
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClEls_Cipher_Async(cipher_options, enc_key_index, NULL, 0, padded_plain_claimcode, encrypted_claimcode_len,
                               (uint8_t *)iv, encrypted_claimcode));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Cipher_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        PLOG_ERROR("mcuxClEls_Cipher_Async failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        PLOG_ERROR("mcuxClEls_WaitForOperation failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    agent_status = delete_key(enc_key_index);
    if (IOT_AGENT_SUCCESS != agent_status)
    {
        PLOG_ERROR("delete_key failed: 0x%08x", agent_status);
        return IOT_AGENT_FAILURE;
    }

    return IOT_AGENT_SUCCESS;
}

static iot_agent_status_t cmac_claimcode(mcuxClEls_KeyIndex_t shared_secret_index,
                                         uint8_t *claimcode_blob,
                                         uint32_t claimcode_blob_length_before_mac)
{
    uint8_t *pos                         = &claimcode_blob[claimcode_blob_length_before_mac];
    uint8_t mac[AES_BLOCK_SIZE]          = {0};
    uint32_t missing_bytes_to_fill_block = AES_BLOCK_SIZE - (claimcode_blob_length_before_mac % AES_BLOCK_SIZE);
    // ELS needs us to pad the message, it does not do that itself :-(
    if (missing_bytes_to_fill_block != 0)
    {
        memset(pos, 0, missing_bytes_to_fill_block);
        *pos = 0x80;
    }

    mcuxClEls_KeyIndex_t mac_key_index = 13U;
    iot_agent_status_t agent_status =
        derive_key(shared_secret_index, mac_key_prop, ckdf_derivation_data_mac, &mac_key_index);
    if (IOT_AGENT_SUCCESS != agent_status)
    {
        PLOG_ERROR("derive_key failed: 0x%08x\n", agent_status);
        return IOT_AGENT_FAILURE;
    }

    mcuxClEls_CmacOption_t cmac_options = {0U};
    cmac_options.bits.initialize        = MCUXCLELS_CMAC_INITIALIZE_ENABLE;
    cmac_options.bits.finalize          = MCUXCLELS_CMAC_FINALIZE_ENABLE;
    cmac_options.bits.extkey            = MCUXCLELS_CMAC_EXTERNAL_KEY_DISABLE;

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token,
                                     mcuxClEls_Cmac_Async(cmac_options, mac_key_index, NULL, 0, claimcode_blob,
                                                          claimcode_blob_length_before_mac, mac));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Cmac_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        PLOG_ERROR("mcuxClEls_Cmac_Async failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        PLOG_ERROR("mcuxClEls_WaitForOperation failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    agent_status = delete_key(mac_key_index);
    if (IOT_AGENT_SUCCESS != agent_status)
    {
        PLOG_ERROR("delete_key failed: 0x%08x", agent_status);
        return IOT_AGENT_FAILURE;
    }

    memcpy(pos, mac, sizeof(mac));
    return IOT_AGENT_SUCCESS;
}

iot_agent_status_t iot_agent_claimcode_encrypt(const char *plain_claimcode,
                                               const uint8_t *el2go_public_key,
                                               size_t el2go_public_key_size,
                                               uint8_t *claimcode_blob,
                                               size_t *claimcode_blob_size)
{
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    size_t plain_claimcode_len      = strlen(plain_claimcode);
    size_t encrypted_claimcode_len =
        ceil_to_aes_blocksize(plain_claimcode_len + 1 /* padding adds at least one byte */);

    // clang-format off
    size_t claimcode_blob_len = 0
        + 2 + UID_SIZE 
        + 2 + 65               // public key
        + 2 + sizeof(uint32_t) // key properties
        + 2 + sizeof(uint32_t) // key properties
        + 2 + IV_SIZE          // IV
        + 2 + encrypted_claimcode_len
        + 2 + AES_BLOCK_SIZE;              // CMAC
    // clang-format on

    if (*claimcode_blob_size < claimcode_blob_len)
    {
        PLOG_ERROR("claimcode blob buffer too small\n");
        return IOT_AGENT_FAILURE;
    }

    PLOG_INFO("Enabling ELS... ");
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_Enable_Async());

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Enable_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
        PLOG_ERROR("mcuxClEls_Enable_Async failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
        PLOG_ERROR("mcuxClEls_WaitForOperation failed: 0x%08x\n", result);
        return IOT_AGENT_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();
    PLOG_INFO("done\n");

    PLOG_INFO("Generating random ECC keypair... ");
    mcuxClEls_KeyIndex_t keypair_index = MCUXCLELS_KEY_SLOTS;
    uint8_t public_key[64]             = {0u};
    size_t public_key_size             = sizeof(public_key);
    agent_status                       = generate_keypair(&keypair_index, &public_key[0], &public_key_size);
    if (IOT_AGENT_SUCCESS != agent_status)
    {
        PLOG_ERROR("generate_keypair failed: 0x%08x\n", agent_status);
        return IOT_AGENT_FAILURE;
    }
    PLOG_INFO("done\n");

    PLOG_INFO("Calculating shared secret... ");
    mcuxClEls_KeyIndex_t shared_secret_index = MCUXCLELS_KEY_SLOTS;
    agent_status = perform_key_agreement(keypair_index, shared_secret_prop, &shared_secret_index, el2go_public_key,
                                         el2go_public_key_size);
    if (IOT_AGENT_SUCCESS != agent_status)
    {
        PLOG_ERROR("perform_key_agreement failed: 0x%08x\n", agent_status);
        delete_key(keypair_index);
        return IOT_AGENT_FAILURE;
    }
    agent_status = delete_key(keypair_index);
    if (IOT_AGENT_SUCCESS != agent_status)
    {
        PLOG_ERROR("delete_key failed: 0x%08x\n", agent_status);
        delete_key(shared_secret_index);
        return IOT_AGENT_FAILURE;
    }
    PLOG_INFO("done\n");

    PLOG_INFO("Creating claimcode blob... ");
    uint8_t *pos = claimcode_blob;

    *pos++       = 0x41;
    *pos++       = UID_SIZE;
    agent_status = read_uid(pos);
    if (IOT_AGENT_SUCCESS != agent_status)
    {
        PLOG_ERROR("read_uid failed: 0x%08x\n", agent_status);
        delete_key(shared_secret_index);
        return IOT_AGENT_FAILURE;
    }
    pos += UID_SIZE;

    *pos++ = 0x42;
    *pos++ = sizeof(public_key) + 1;
    *pos++ = 0x04; // Indicating uncompressed point format (this is what PSA uses as well).
    memcpy(pos, public_key, sizeof(public_key));
    pos += sizeof(public_key);

    *pos++ = 0x43;
    *pos++ = 4;
    *pos++ = (enc_key_prop.word.value >> 24) & 0xFF;
    *pos++ = (enc_key_prop.word.value >> 16) & 0xFF;
    *pos++ = (enc_key_prop.word.value >> 8) & 0xFF;
    *pos++ = (enc_key_prop.word.value) & 0xFF;

    *pos++ = 0x44;
    *pos++ = 4;
    *pos++ = (mac_key_prop.word.value >> 24) & 0xFF;
    *pos++ = (mac_key_prop.word.value >> 16) & 0xFF;
    *pos++ = (mac_key_prop.word.value >> 8) & 0xFF;
    *pos++ = (mac_key_prop.word.value) & 0xFF;

    *pos++       = 0x45;
    *pos++       = IV_SIZE;
    uint8_t *iv  = pos; // The IV is filled during encryption
    agent_status = generate_iv(iv);
    if (IOT_AGENT_SUCCESS != agent_status)
    {
        PLOG_ERROR("generate_iv failed: 0x%08x\n", agent_status);
        delete_key(shared_secret_index);
        return IOT_AGENT_FAILURE;
    }
    pos += IV_SIZE;

    *pos++                       = 0x46;
    *pos++                       = encrypted_claimcode_len;
    uint8_t *encrypted_claimcode = pos;
    agent_status                 = encrypt_claimcode(shared_secret_index, plain_claimcode, iv, encrypted_claimcode);
    if (IOT_AGENT_SUCCESS != agent_status)
    {
        PLOG_ERROR("encrypt_claimcode failed: 0x%08x\n", agent_status);
        delete_key(shared_secret_index);
        return IOT_AGENT_FAILURE;
    }
    pos += encrypted_claimcode_len;

    *pos++                                  = 0x5e;
    *pos++                                  = AES_BLOCK_SIZE;
    size_t claimcode_blob_length_before_mac = pos - claimcode_blob;
    agent_status = cmac_claimcode(shared_secret_index, claimcode_blob, claimcode_blob_length_before_mac);
    if (IOT_AGENT_SUCCESS != agent_status)
    {
        PLOG_ERROR("cmac_claimcode failed: 0x%08x\n", agent_status);
        delete_key(shared_secret_index);
        return IOT_AGENT_FAILURE;
    }
    pos += AES_BLOCK_SIZE;

    *claimcode_blob_size = pos - claimcode_blob;

    agent_status = delete_key(shared_secret_index);
    if (IOT_AGENT_SUCCESS != agent_status)
    {
        PLOG_ERROR("delete_key failed: 0x%08x\n", agent_status);
        return IOT_AGENT_FAILURE;
    }
    PLOG_INFO("done\n");

    printf_buffer("claimcode", claimcode_blob, *claimcode_blob_size);
    return IOT_AGENT_SUCCESS;
}
