/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/** \file mcux_psa_s2xx_asymmetric_signature.c
 *
 * This file contains the implementation of the entry points associated to the
 * asymmetric signature capability as described by the PSA Cryptoprocessor
 * Driver interface specification
 *
 */

#include "mcux_psa_s2xx_init.h"
#include "mcux_psa_s2xx_asymmetric_signature.h"
#include "mcux_psa_s2xx_common_compute.h"

//#include "psa_crypto_driver_wrappers_no_static.h"

/* Public key is double length of private key + 1byte for leading byte (0x04) which is indicating uncompressed format.
 * Support for 521 means we might need two additional bytes per ECC coordinate, hence 132 Bytes in total. */
#define MAX_PUB_KEY_SIZE_IN_BYTES  (132u + 1u)
#define MAX_PAIR_KEY_SIZE_IN_BYTES (MAX_PUB_KEY_SIZE_IN_BYTES + 66u)

#define NISTP521_BITLEN (521u)
#define ED25519_BITLEN  (255u)

static psa_status_t ele_s2xx_psa_2_ele_asym_alg(const psa_key_attributes_t *attributes,
                                                psa_algorithm_t alg,
                                                sss_algorithm_t *ele_alg)
{
    psa_status_t status           = PSA_SUCCESS;
    psa_algorithm_t sign_hash_alg = PSA_ALG_ANY_HASH;

    if (PSA_ALG_PURE_EDDSA == alg)
    {
        *ele_alg = kAlgorithm_SSS_EdDSA_Ed25519;
        status   = PSA_SUCCESS;
    }
    else if (true == PSA_ALG_IS_ECDSA(alg))
    {
        sign_hash_alg = PSA_ALG_SIGN_GET_HASH(alg);
        status        = PSA_SUCCESS;

        switch (sign_hash_alg)
        {
#if defined(PSA_WANT_ALG_SHA_224)
            case PSA_ALG_SHA_224:
                *ele_alg = kAlgorithm_SSS_ECDSA_SHA224;
                break;
#endif /* PSA_WANT_ALG_SHA_224 */
#if defined(PSA_WANT_ALG_SHA_256)
            case PSA_ALG_SHA_256:
                *ele_alg = kAlgorithm_SSS_ECDSA_SHA256;
                break;
#endif /* PSA_WANT_ALG_SHA_256 */
#if defined(PSA_WANT_ALG_SHA_384)
            case PSA_ALG_SHA_384:
                *ele_alg = kAlgorithm_SSS_ECDSA_SHA384;
                break;
#endif /* PSA_WANT_ALG_SHA_384 */
#if defined(PSA_WANT_ALG_SHA_512)
            case PSA_ALG_SHA_512:
                *ele_alg = kAlgorithm_SSS_ECDSA_SHA512;
                break;
#endif /* PSA_WANT_ALG_SHA_512 */
            default:
                status = PSA_ERROR_NOT_SUPPORTED;
                break;
        }
    }
    else
    {
        status = PSA_ERROR_NOT_SUPPORTED;
    }

    return status;
}

static psa_status_t validate_key_bitlen_for_hash_sign(const psa_key_attributes_t *attributes,
                                                      psa_algorithm_t alg,
                                                      size_t hash_length)
{
    size_t hash_alg_bitlen   = PSA_BYTES_TO_BITS(PSA_HASH_LENGTH(PSA_ALG_SIGN_GET_HASH(alg)));
    size_t hash_input_bitlen = PSA_BYTES_TO_BITS(hash_length);
    size_t key_bitlen        = psa_get_key_bits(attributes);

    /* NIST-P 521 can used for signing 512-bit hashes,
     * so we just update the bitlen for the comparison
     */
    if (NISTP521_BITLEN == key_bitlen)
    {
        key_bitlen = PSA_BYTES_TO_BITS(PSA_HASH_LENGTH(PSA_ALG_SHA_512));
    }

    if (key_bitlen != hash_alg_bitlen)
    {
        /* key is not supported for use with alg */
        return PSA_ERROR_NOT_SUPPORTED;
    }

    if (key_bitlen != hash_input_bitlen)
    {
        /* hash_length is not valid for the algorithm and key type */
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return PSA_SUCCESS;
}

static psa_status_t asymmetric_sign_setkey(const psa_key_attributes_t *attributes,
                                           sss_sscp_object_t *sssKey,
                                           const uint8_t *key_buffer,
                                           size_t key_buffer_size,
                                           size_t key_bits)
{
    psa_status_t status           = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_type_t key_type       = psa_get_key_type(attributes);
    sss_key_part_t key_part       = kSSS_KeyPart_NONE;
    sss_cipher_type_t cipher_type = (sss_cipher_type_t) 0;
    size_t allocation_size        = PSA_BITS_TO_BYTES(key_bits);
    uint8_t *key_data             = NULL;
    size_t key_data_size          = 0u;

    /* For exporting the public part of the key */
    uint8_t public_key_data[MAX_PAIR_KEY_SIZE_IN_BYTES] = {0u};
    size_t public_key_data_length                       = 0u;

    if (true == PSA_KEY_TYPE_IS_KEY_PAIR(key_type))
    {
        /* In PSA, an ECC key pair is represented by the secret value,
         * so we need to also export the public part for S2XX and position them
         * correctly in memory [pub_x, pub_y, private] */
        key_part        = kSSS_KeyPart_Pair;
        allocation_size = allocation_size * 3u;

        status = psa_export_public_key(psa_get_key_id(attributes), public_key_data, MAX_PAIR_KEY_SIZE_IN_BYTES, &public_key_data_length);

        if (PSA_SUCCESS != status)
        {
            return status;
        }

        if (0u == public_key_data_length)
        {
            return PSA_ERROR_GENERIC_ERROR;
        }

        key_data      = public_key_data + 1;
        key_data_size = public_key_data_length - 1u;

        (void)memcpy(key_data + key_data_size, key_buffer, PSA_BITS_TO_BYTES(key_bits));
        key_data_size = key_data_size + PSA_BITS_TO_BYTES(key_bits);
    }
    else if (true == PSA_KEY_TYPE_IS_PUBLIC_KEY(key_type))
    {
        /* Set required S2XX flags and skip the first Byte of the ECC public key */
        key_part        = kSSS_KeyPart_Public;
        allocation_size = allocation_size * 2u;

        key_data      = (uint8_t *)key_buffer + 1;
        key_data_size = PSA_BITS_TO_BYTES(key_bits) * 2u;
    }
    else
    {
        /* Private key - no need to do anything special */
        key_part = kSSS_KeyPart_Private;

        key_data      = (uint8_t *)key_buffer;
        key_data_size = key_buffer_size;
    }

    /* Preemptively inflate the allocation size, due to possible additional
     * Bytes required for 521bit public/keypair keys */
    allocation_size = allocation_size + 6u;

    status = PSA_SUCCESS;
    switch (PSA_KEY_TYPE_ECC_GET_FAMILY(key_type))
    {
        case PSA_ECC_FAMILY_SECP_R1:
            cipher_type = kSSS_CipherType_EC_NIST_P;
            break;
        default:
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
    }
    if (PSA_SUCCESS != status)
    {
        return status;
    }

    /* Allocate keyobject and load key */
    if ((sss_sscp_key_object_init(sssKey, &g_ele_ctx.keyStore)) != kStatus_SSS_Success)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if ((sss_sscp_key_object_allocate_handle(sssKey, 1u, /* key id */
                                             key_part, cipher_type, allocation_size,
                                             kSSS_KeyProp_CryptoAlgo_AsymSignVerify)) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    if ((sss_sscp_key_store_set_key(&g_ele_ctx.keyStore, sssKey, key_data, key_data_size, key_bits,
                                    key_part)) != kStatus_SSS_Success)
    {
        (void)sss_sscp_key_object_free(sssKey, kSSS_keyObjFree_KeysStoreDefragment);
        return PSA_ERROR_GENERIC_ERROR;
    }

    return PSA_SUCCESS;
}

psa_status_t ele_s2xx_transparent_sign_hash(const psa_key_attributes_t *attributes,
                                            const uint8_t *key_buffer,
                                            size_t key_buffer_size,
                                            psa_algorithm_t alg,
                                            const uint8_t *hash,
                                            size_t hash_length,
                                            uint8_t *signature,
                                            size_t signature_size,
                                            size_t *signature_length)
{
    psa_status_t status      = PSA_ERROR_CORRUPTION_DETECTED;
    sss_sscp_object_t sssKey = {0};
    sss_algorithm_t ele_alg  = {0};
    size_t output_size       = 0;

    /* Convert PSA_ALG_* to ELE value and validate supported alg */
    status = ele_s2xx_psa_2_ele_asym_alg(attributes, alg, &ele_alg);
    if (PSA_SUCCESS != status)
    {
        return status;
    }

    if (PSA_KEY_TYPE_ECC_GET_FAMILY(psa_get_key_type(attributes)) != PSA_ECC_FAMILY_SECP_R1)
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Hash sign/verify only with ECDSA on S200 */
    if (false == PSA_ALG_IS_ECDSA(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Deterministic ECDSA not supported */
    if (true == PSA_ALG_IS_DETERMINISTIC_ECDSA(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* The given hash length and algorithm must match the ECDSA key length */
    status = validate_key_bitlen_for_hash_sign(attributes, alg, hash_length);
    if (PSA_SUCCESS != status)
    {
        return status;
    }

    if (NULL == key_buffer || 0u == key_buffer_size)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (NULL == hash || 0u == hash_length)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (NULL == signature || NULL == signature_length)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    output_size = PSA_SIGN_OUTPUT_SIZE(psa_get_key_type(attributes), psa_get_key_bits(attributes), alg);
    if (output_size > signature_size)
    {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    if (mcux_mutex_lock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_BAD_STATE;
    }

    status = asymmetric_sign_setkey(attributes, &sssKey, key_buffer, key_buffer_size, psa_get_key_bits(attributes));
    if (PSA_SUCCESS != status)
    {
        goto exit;
    }

    *signature_length = signature_size;
    status = ele_s2xx_common_sign_digest((uint8_t *)hash, hash_length, signature, signature_length, &sssKey, ele_alg);
    if (PSA_SUCCESS != status)
    {
        goto exit;
    }

exit:
    (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);

    if (mcux_mutex_unlock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_BAD_STATE;
    }

    return status;
}

psa_status_t ele_s2xx_transparent_verify_hash(const psa_key_attributes_t *attributes,
                                              const uint8_t *key_buffer,
                                              size_t key_buffer_size,
                                              psa_algorithm_t alg,
                                              const uint8_t *hash,
                                              size_t hash_length,
                                              const uint8_t *signature,
                                              size_t signature_length)
{
    psa_status_t status      = PSA_ERROR_CORRUPTION_DETECTED;
    sss_sscp_object_t sssKey = {0};
    sss_algorithm_t ele_alg  = {0};

    /* Convert PSA_ALG_* to ELE value and validate supported alg */
    status = ele_s2xx_psa_2_ele_asym_alg(attributes, alg, &ele_alg);
    if (PSA_SUCCESS != status)
    {
        return status;
    }

    if (PSA_KEY_TYPE_ECC_GET_FAMILY(psa_get_key_type(attributes)) != PSA_ECC_FAMILY_SECP_R1)
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Hash sign/verify only with ECDSA on S200 */
    if (false == PSA_ALG_IS_ECDSA(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Deterministic ECDSA not supported */
    if (true == PSA_ALG_IS_DETERMINISTIC_ECDSA(alg))
    {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* The given hash length and algorithm must match the ECDSA key length */
    status = validate_key_bitlen_for_hash_sign(attributes, alg, hash_length);
    if (PSA_SUCCESS != status)
    {
        return status;
    }

    if (NULL == key_buffer || 0u == key_buffer_size)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (NULL == hash || 0u == hash_length)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (NULL == signature || 0u == signature_length)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (mcux_mutex_lock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_GENERIC_ERROR;
    }

    status = asymmetric_sign_setkey(attributes, &sssKey, key_buffer, key_buffer_size, psa_get_key_bits(attributes));
    if (PSA_SUCCESS != status)
    {
        goto exit;
    }

    status = ele_s2xx_common_verify_digest((uint8_t *)hash, hash_length, (uint8_t *)signature, signature_length, &sssKey, ele_alg);
    if (PSA_SUCCESS != status)
    {
        goto exit;
    }

exit:
    (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);

    if (mcux_mutex_unlock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_BAD_STATE;
    }

    return status;
}

/** @} */ // end of psa_asym_sign
