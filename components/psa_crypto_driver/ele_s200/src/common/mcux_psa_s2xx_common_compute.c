/*
 * Copyright 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/** \file mcux_psa_s2xx_common_compute.c
 *
 * This file contains the implementation of the entry points for the
 * driver-specific functions for crypto operations
 *
 */

#include "mcux_psa_s2xx_common_compute.h"

/*  AEAD  */

psa_status_t ele_s2xx_common_aead(const uint8_t *nonce, size_t nonce_length,
                                  const uint8_t *additional_data, size_t additional_data_length,
                                  const uint8_t *input, size_t input_size,
                                  uint8_t *output,
                                  uint8_t *tag, size_t *tag_length,
                                  sss_mode_t mode, sss_sscp_object_t *sssKey, sss_algorithm_t ele_alg)
{
    sss_sscp_aead_t ctx = {0};

    if ((sss_sscp_aead_context_init(&ctx, &g_ele_ctx.sssSession, sssKey, ele_alg, mode)) !=
        kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* RUN AEAD */
    if ((sss_sscp_aead_one_go(&ctx, input, output, input_size, (uint8_t *)nonce, nonce_length,
                              additional_data, additional_data_length, tag, tag_length)) !=
        kStatus_SSS_Success)
    {
        (void)sss_sscp_aead_context_free(&ctx);
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        if (kMode_SSS_Decrypt == mode)
        {
            /* If AEAD decrypt failed in this case we cannot differentiate between root cause
             * It may be due to some sanity check, but most likely due to tag mismatch between actual and expected value
             * So treat all fails in this case as signature mismatch */
            return PSA_ERROR_INVALID_SIGNATURE;
        }
        return PSA_ERROR_GENERIC_ERROR;
    };

    /* Free contexts */
    if (sss_sscp_aead_context_free(&ctx) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}

/*  MAC  */

psa_status_t ele_s2xx_common_mac(const uint8_t *input, size_t input_length,
                                 uint8_t *mac, size_t mac_size, size_t *mac_length,
                                 sss_sscp_object_t *sssKey, sss_algorithm_t ele_alg)
{
    sss_sscp_mac_t ctx = {0};

    /* Init context for MAC*/
    if ((sss_sscp_mac_context_init(&ctx, &g_ele_ctx.sssSession, sssKey, ele_alg, kMode_SSS_Mac)) !=
        kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }
    /* Call MAC one go*/
    if ((sss_sscp_mac_one_go(&ctx, (const uint8_t *)input, input_length, (uint8_t *)mac, &mac_size)) !=
        kStatus_SSS_Success)
    {
        (void)sss_sscp_mac_context_free(&ctx);
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    *mac_length = mac_size;

    /* Free context */
    if (sss_sscp_mac_context_free(&ctx) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}

/*  CIPHER  */

psa_status_t ele_s2xx_common_cipher(sss_sscp_object_t *sssKey,
                                    const uint8_t *iv, size_t iv_length,
                                    const uint8_t *input, uint8_t *output,
                                    size_t input_length,
                                    sss_algorithm_t ele_algo, sss_mode_t mode)
{
    sss_sscp_symmetric_t ctx = {0};

    /* Init symmetric context */
    if (sss_sscp_symmetric_context_init(&ctx, &g_ele_ctx.sssSession,
                                        sssKey, ele_algo,
                                        mode) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Run encryption */
    if (sss_sscp_cipher_one_go(&ctx, (uint8_t *)iv,
                               iv_length, input,
                               output, input_length) != kStatus_SSS_Success)
    {
        (void)sss_sscp_symmetric_context_free(&ctx);
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Clean up */
    if (sss_sscp_symmetric_context_free(&ctx) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}

/*  ASYMMETRIC  */

psa_status_t ele_s2xx_common_sign_digest(uint8_t *digest, size_t digest_len,
                                         uint8_t *signature, size_t *signature_len,
                                         sss_sscp_object_t *sssKey, sss_algorithm_t ele_alg)
{
    sss_sscp_asymmetric_t ctx = {0u};

    /* Initialize asymmetric context for signing */
    if (sss_sscp_asymmetric_context_init(&ctx,  &g_ele_ctx.sssSession,
                                         sssKey, ele_alg, kMode_SSS_Sign) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Sign message digest */
    if (sss_sscp_asymmetric_sign_digest(&ctx, digest, digest_len,
                                        signature, signature_len) != kStatus_SSS_Success)
    {
        (void)sss_sscp_asymmetric_context_free(&ctx);
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Clean up */
    if (sss_sscp_asymmetric_context_free(&ctx) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}

psa_status_t ele_s2xx_common_verify_digest(uint8_t *digest, size_t digest_len,
                                           uint8_t *signature, size_t signature_len,
                                           sss_sscp_object_t *sssKey, sss_algorithm_t ele_alg)
{
    sss_sscp_asymmetric_t ctx = {0u};

    /* Initialize asymmetric context for signing */
    if (sss_sscp_asymmetric_context_init(&ctx,  &g_ele_ctx.sssSession,
                                         sssKey, ele_alg, kMode_SSS_Verify) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Verify message digest */
    if (sss_sscp_asymmetric_verify_digest(&ctx, digest, digest_len,
                                          signature, signature_len) != kStatus_SSS_Success)
    {
        (void)sss_sscp_asymmetric_context_free(&ctx);
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);

        /* We do not have return code granularity for differentiating
         * generic errors vs signature verification errors.
         * We will assume the more likely situation of a failure at this point,
         * which is signature verification failure. */
        return PSA_ERROR_INVALID_SIGNATURE;
    }

    /* Clean up */
    if (sss_sscp_asymmetric_context_free(&ctx) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}

/*  KEY AGREEMENT  */

psa_status_t ele_s2xx_common_key_agreement(sss_sscp_object_t *sssKey,
                                           sss_sscp_object_t *sssKey_peer,
                                           sss_sscp_object_t *sssKey_shared,
                                           uint8_t *shared_secret,
                                           size_t shared_secret_size,
                                           size_t *shared_secret_length)
{
    sss_sscp_derive_key_t ctx   = {0};
    size_t shared_secret_bitlen = 0u;

    if (sss_sscp_derive_key_context_init(&ctx,  &g_ele_ctx.sssSession, sssKey,
                                         kAlgorithm_SSS_ECDH, kMode_SSS_ComputeSharedSecret) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (sss_sscp_asymmetric_dh_derive_key(&ctx, sssKey_peer, sssKey_shared) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        (void)sss_sscp_derive_key_context_free(&ctx);
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Use the length parameter as an in/out argument and retrieve the key */
    *shared_secret_length = shared_secret_size;
    if (sss_sscp_key_store_get_key(&g_ele_ctx.keyStore, sssKey_shared,
                                   shared_secret, shared_secret_length,
                                   &shared_secret_bitlen, kSSS_KeyPart_Default) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        (void)sss_sscp_derive_key_context_free(&ctx);
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (sss_sscp_derive_key_context_free(&ctx) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}
