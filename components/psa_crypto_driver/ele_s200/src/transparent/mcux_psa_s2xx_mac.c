/*
 * Copyright 2024-2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/** \file mcux_psa_s2xx_mac.c
 *
 * This file contains the implementation of the entry points associated to the
 * mac capability (single-part and multipart) as described by the PSA
 * Cryptoprocessor Driver interface specification
 *
 */

#include "mcux_psa_s2xx_mac.h"
#include "mcux_psa_s2xx_common_compute.h"

/*
 * Entry points for MAC computation and verification as described by the PSA
 *  Cryptoprocessor Driver interface specification
 */

/* Convert PSA Algorithm to ELE Algorithm, CMAC or HMAC */
static psa_status_t ele_psa_mac_alg_to_ele_mac_alg(psa_algorithm_t alg, sss_algorithm_t *ele_alg)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

#if defined(PSA_WANT_ALG_CMAC)
    if (PSA_ALG_FULL_LENGTH_MAC(alg) == PSA_ALG_CMAC)
    {
        *ele_alg = kAlgorithm_SSS_CMAC_AES;
        status = PSA_SUCCESS;
    }
    else
#endif /* PSA_WANT_ALG_CMAC */
#if defined(PSA_WANT_ALG_HMAC)
        if (true == PSA_ALG_IS_HMAC(alg))
    {
        status = PSA_SUCCESS;
        switch (PSA_ALG_HMAC_GET_HASH(alg))
        {
#if defined(PSA_WANT_ALG_SHA_256)
            case PSA_ALG_SHA_256:
                *ele_alg = kAlgorithm_SSS_HMAC_SHA256;
                break;
#endif /* PSA_WANT_ALG_SHA_256 */
#if defined(ELE_FEATURE_EXTENDED_HMAC)
#if defined(PSA_WANT_ALG_SHA_1)
            case PSA_ALG_SHA_1:
                *ele_alg = kAlgorithm_SSS_HMAC_SHA1;
                break;
#endif /* PSA_WANT_ALG_SHA_1 */
#if defined(PSA_WANT_ALG_SHA_224)
            case PSA_ALG_SHA_224:
                *ele_alg = kAlgorithm_SSS_HMAC_SHA224;
                break;
#endif /* PSA_WANT_ALG_SHA_224 */
#if defined(PSA_WANT_ALG_SHA_384)
            case PSA_ALG_SHA_384:
                *ele_alg = kAlgorithm_SSS_HMAC_SHA384;
                break;
#endif /* PSA_WANT_ALG_SHA_384 */
#if defined(PSA_WANT_ALG_SHA_512)
            case PSA_ALG_SHA_512:
                *ele_alg = kAlgorithm_SSS_HMAC_SHA512;
                break;
#endif /* PSA_WANT_ALG_SHA_512 */
#endif /* ELE_FEATURE_EXTENDED_HMAC */
            default:
                status = PSA_ERROR_NOT_SUPPORTED;
                break;
        }
    }
    else
#endif /* PSA_WANT_ALG_HMAC */
    {
        status = PSA_ERROR_NOT_SUPPORTED;
    }

    return status;
}

/**
 *  Set MAC key which is placed in keybuffer into keyslot inside ELE
 */
static status_t set_mac_key(sss_sscp_object_t *sssKey, const uint8_t *key_buffer, size_t key_bits, sss_algorithm_t mode)
{
    sss_cipher_type_t type = 0u;

    /* Validation and enum conversion was done before calling this function,
     * so if mode is not CMAC, then it must be HMAC and nothing else.
     */
    if (mode == kAlgorithm_SSS_CMAC_AES)
    {
        type = kSSS_CipherType_CMAC;
    }
    else
    {
        type = kSSS_CipherType_HMAC;
    }
    size_t key_bytes = key_bits >> 3u;

    if ((sss_sscp_key_object_init(sssKey, &g_ele_ctx.keyStore)) != kStatus_SSS_Success)
    {
        return kStatus_Fail;
    }

    if ((sss_sscp_key_object_allocate_handle(sssKey, 1u, /* key id */
                                             kSSS_KeyPart_Default, type, key_bytes, kSSS_KeyProp_CryptoAlgo_MAC)) !=
        kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return kStatus_Fail;
    }

    if ((sss_sscp_key_store_set_key(&g_ele_ctx.keyStore, sssKey, key_buffer, key_bytes, key_bits,
                                    kSSS_KeyPart_Default)) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return kStatus_Fail;
    }

    return kStatus_Success;
}

psa_status_t ele_s2xx_transparent_mac_compute(const psa_key_attributes_t *attributes,
                                              const uint8_t *key_buffer,
                                              size_t key_buffer_size,
                                              psa_algorithm_t alg,
                                              const uint8_t *input,
                                              size_t input_length,
                                              uint8_t *mac,
                                              size_t mac_size,
                                              size_t *mac_length)
{
    psa_status_t status      = PSA_ERROR_CORRUPTION_DETECTED;
    sss_algorithm_t ele_alg  = 0;
    sss_sscp_object_t sssKey = {0};
    size_t key_bits          = psa_get_key_bits(attributes);
    psa_key_type_t key_type  = psa_get_key_type(attributes);

    /* Get Algo for ELE */
    status = ele_psa_mac_alg_to_ele_mac_alg(alg, &ele_alg);
    if (status != PSA_SUCCESS)
    {
        return status;
    }

    mac_size = PSA_MAC_LENGTH(key_type, key_bits, alg);

    if (mcux_mutex_lock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Set Key for MAC */
    if ((set_mac_key(&sssKey, key_buffer, key_bits, ele_alg)) != kStatus_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    status = ele_s2xx_common_mac(input, input_length, mac, mac_size, mac_length, &sssKey, ele_alg);
    if (PSA_SUCCESS != status)
    {
        goto exit;
    }

exit:
    (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);

    if (mcux_mutex_unlock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return status;
}

#if defined(ELE_FEATURE_MAC_MULTIPART)
static psa_status_t mac_common_setup(ele_s2xx_transparent_mac_operation_t *operation,
                                     const psa_key_attributes_t *attributes,
                                     const uint8_t *key_buffer,
                                     size_t key_buffer_size,
                                     psa_algorithm_t alg)
{
    psa_status_t status    = PSA_ERROR_CORRUPTION_DETECTED;
    sss_sscp_mac_t mac_ctx = {0};
    size_t key_size_bits   = psa_get_key_bits(attributes);
    size_t blob_size       = ELE_S2XX_MULTIPART_MAC_BLOB_SIZE;

    /* Workaround : we need 1 Byte allocated for 0-bit HMAC key support */
    size_t key_handle_alloc_size = (0u != key_size_bits)
                                        ? PSA_BITS_TO_BYTES(key_size_bits)
                                        : 1u;

    if (false == PSA_ALG_IS_MAC(alg))
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    status = ele_psa_mac_alg_to_ele_mac_alg(alg, &operation->algorithm);
    if (PSA_SUCCESS != status)
    {
        return status;
    }

    if (mcux_mutex_lock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* This key object is to be freed during mac_*_finish() or mac_abort() */
    if (sss_sscp_key_object_init(&operation->key_object,
                                 &g_ele_ctx.keyStore) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    if (sss_sscp_key_object_allocate_handle(&operation->key_object,
                                            0u, /* Let S200 choose key ID */
                                            kSSS_KeyPart_Default,
                                            kSSS_CipherType_MAC,
                                            key_handle_alloc_size,
                                            kSSS_KeyProp_CryptoAlgo_MAC) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    /* For S200 0-keysize HMAC support, we need to allocate a dummy
     * handle (step above), and NOT set a key.
     */
    if (0u != key_size_bits)
    {
        if (sss_sscp_key_store_set_key(&g_ele_ctx.keyStore,
                                       &operation->key_object,
                                       key_buffer,
                                       key_buffer_size,
                                       key_size_bits,
                                       kSSS_KeyPart_Default) != kStatus_SSS_Success)
        {
            status = PSA_ERROR_GENERIC_ERROR;
            goto exit;
        }
    }

    if (sss_sscp_mac_context_init(&mac_ctx,
                                  &g_ele_ctx.sssSession,
                                  &operation->key_object,
                                  operation->algorithm,
                                  kMode_SSS_Mac) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    if (sss_sscp_mac_init(&mac_ctx) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    if (sss_sscp_mac_export(&mac_ctx,
                            operation->context_blob,
                            &blob_size) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

exit:
    /* Clean the temporary MAC context, but not the key object, even on failure.
     * PSA calls mac_abort() on failure, so deferring the key object
     * cleanup to the mac_abort() call.
     */
    (void)sss_sscp_mac_context_free(&mac_ctx);

    if (mcux_mutex_unlock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return status;
}

psa_status_t ele_s2xx_transparent_mac_sign_setup(ele_s2xx_transparent_mac_operation_t *operation,
                                                 const psa_key_attributes_t *attributes,
                                                 const uint8_t *key_buffer,
                                                 size_t key_buffer_size,
                                                 psa_algorithm_t alg)
{
    /* On S200 MAC sign and verify setup is identical */
    return mac_common_setup(operation,
                            attributes,
                            key_buffer,
                            key_buffer_size,
                            alg);
}

psa_status_t ele_s2xx_transparent_mac_verify_setup(ele_s2xx_transparent_mac_operation_t *operation,
                                                   const psa_key_attributes_t *attributes,
                                                   const uint8_t *key_buffer,
                                                   size_t key_buffer_size,
                                                   psa_algorithm_t alg)
{
    /* On S200 MAC sign and verify setup is identical */
    return mac_common_setup(operation,
                            attributes,
                            key_buffer,
                            key_buffer_size,
                            alg);
}

psa_status_t ele_s2xx_transparent_mac_update(ele_s2xx_transparent_mac_operation_t *operation,
                                             const uint8_t *input,
                                             size_t input_length)
{
    psa_status_t status    = PSA_SUCCESS;
    sss_sscp_mac_t mac_ctx = {0};
    size_t blob_size       = ELE_S2XX_MULTIPART_MAC_BLOB_SIZE;

    if (mcux_mutex_lock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (sss_sscp_mac_context_init(&mac_ctx,
                                  &g_ele_ctx.sssSession,
                                  &operation->key_object,
                                  operation->algorithm,
                                  kMode_SSS_Mac) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    if (sss_sscp_mac_import(&mac_ctx,
                            operation->context_blob,
                            blob_size) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    if (sss_sscp_mac_update(&mac_ctx,
                            input,
                            input_length) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    if (sss_sscp_mac_export(&mac_ctx,
                            operation->context_blob,
                            &blob_size) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

exit:
    /* Clean the temporary MAC context, but not the key object, even on failure.
     * PSA calls mac_abort() on failure, so deferring the key object
     * cleanup to the mac_abort() call.
     */
    (void)sss_sscp_mac_context_free(&mac_ctx);

    if (mcux_mutex_unlock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return status;
}

psa_status_t ele_s2xx_transparent_mac_sign_finish(ele_s2xx_transparent_mac_operation_t *operation,
                                                  uint8_t *mac,
                                                  size_t mac_size,
                                                  size_t *mac_length)
{
    psa_status_t status        = PSA_SUCCESS;
    sss_sscp_mac_t mac_ctx     = {0};
    size_t mac_length_internal = mac_size;

    if (mcux_mutex_lock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (sss_sscp_mac_context_init(&mac_ctx,
                                  &g_ele_ctx.sssSession,
                                  &operation->key_object,
                                  operation->algorithm,
                                  kMode_SSS_Mac) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    if (sss_sscp_mac_import(&mac_ctx,
                            operation->context_blob,
                            ELE_S2XX_MULTIPART_MAC_BLOB_SIZE) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    if (sss_sscp_mac_finish(&mac_ctx,
                            mac,
                            &mac_length_internal) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    if (sss_sscp_mac_context_free(&mac_ctx) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

exit:
    /* Clean the temporary MAC context, but not the key object, even on failure.
     * PSA calls mac_abort() on failure, so deferring the key object
     * cleanup to the mac_abort() call.
     */
    (void)sss_sscp_mac_context_free(&mac_ctx);

    if (mcux_mutex_unlock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (PSA_SUCCESS == status)
    {
        *mac_length = mac_length_internal;
    }

    return status;
}

psa_status_t ele_s2xx_transparent_mac_verify_finish(ele_s2xx_transparent_mac_operation_t *operation,
                                                    const uint8_t *mac,
                                                    size_t mac_length)
{
    psa_status_t status                    = PSA_SUCCESS;
    sss_sscp_mac_t mac_ctx                 = {0};
    uint8_t mac_finished[PSA_MAC_MAX_SIZE] = {0u};
    size_t mac_length_internal             = sizeof(mac_finished);

    if (mcux_mutex_lock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (sss_sscp_mac_context_init(&mac_ctx,
                                  &g_ele_ctx.sssSession,
                                  &operation->key_object,
                                  operation->algorithm,
                                  kMode_SSS_Mac) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    if (sss_sscp_mac_import(&mac_ctx,
                            operation->context_blob,
                            ELE_S2XX_MULTIPART_MAC_BLOB_SIZE) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

    if (sss_sscp_mac_finish(&mac_ctx,
                            mac_finished,
                            &mac_length_internal) != kStatus_SSS_Success)
    {
        status = PSA_ERROR_GENERIC_ERROR;
        goto exit;
    }

exit:
    /* Clean the temporary MAC context, but not the key object, even on failure.
     * PSA calls mac_abort() on failure, so deferring the key object
     * cleanup to the mac_abort() call.
     */
    (void)sss_sscp_mac_context_free(&mac_ctx);

    if (mcux_mutex_unlock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Verify the MAC */
    if (PSA_SUCCESS == status)
    {
        if (memcmp(mac, mac_finished, mac_length) != 0)
        {
            status = PSA_ERROR_INVALID_SIGNATURE;
        }
    }

    return status;
}

psa_status_t ele_s2xx_transparent_mac_abort(ele_s2xx_transparent_mac_operation_t *operation)
{
    if (mcux_mutex_lock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* We have no S200 error code granularity, so we go by best effort and
     * assume success when cleaning up the S200 key object. Failure here would
     * mean that no such key can be free'd in S200, so
     *  - the key object is already free'd,
     *  - the key object has never been created (failure during key creation),
     *  - operation->key_object attribute has been corrupted.
     */
    (void)sss_sscp_key_object_free(&operation->key_object, kSSS_keyObjFree_KeysStoreDefragment);

    /* Clean up the SW-side context */
    (void)memset(operation, 0, sizeof(ele_s2xx_transparent_mac_operation_t));

    if (mcux_mutex_unlock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}
#else /* ELE_FEATURE_MAC_MULTIPART */
psa_status_t ele_s2xx_transparent_mac_sign_setup(ele_s2xx_transparent_mac_operation_t *operation,
                                                 const psa_key_attributes_t *attributes,
                                                 const uint8_t *key_buffer,
                                                 size_t key_buffer_size,
                                                 psa_algorithm_t alg)
{
    (void)operation;
    (void)attributes;
    (void)key_buffer;
    (void)key_buffer_size;
    (void)alg;
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t ele_s2xx_transparent_mac_verify_setup(ele_s2xx_transparent_mac_operation_t *operation,
                                                   const psa_key_attributes_t *attributes,
                                                   const uint8_t *key_buffer,
                                                   size_t key_buffer_size,
                                                   psa_algorithm_t alg)
{
    (void)operation;
    (void)attributes;
    (void)key_buffer;
    (void)key_buffer_size;
    (void)alg;
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t ele_s2xx_transparent_mac_update(ele_s2xx_transparent_mac_operation_t *operation,
                                             const uint8_t *input,
                                             size_t input_length)
{
    (void)operation;
    (void)input;
    (void)input_length;
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t ele_s2xx_transparent_mac_sign_finish(ele_s2xx_transparent_mac_operation_t *operation,
                                                  uint8_t *mac,
                                                  size_t mac_size,
                                                  size_t *mac_length)
{
    (void)operation;
    (void)mac;
    (void)mac_size;
    (void)mac_length;
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t ele_s2xx_transparent_mac_verify_finish(ele_s2xx_transparent_mac_operation_t *operation,
                                                    const uint8_t *mac,
                                                    size_t mac_length)
{
    (void)operation;
    (void)mac;
    (void)mac_length;
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t ele_s2xx_transparent_mac_abort(ele_s2xx_transparent_mac_operation_t *operation)
{
    (void)operation;
    return PSA_ERROR_NOT_SUPPORTED;
}
#endif /* ELE_FEATURE_MAC_MULTIPART */

/** @} */ // end of psa_mac
