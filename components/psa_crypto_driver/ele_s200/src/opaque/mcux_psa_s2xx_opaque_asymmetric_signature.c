/*
 * Copyright 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/** \file mcux_psa_s2xx_opaque_asymmetric_signature.c
 *
 * This file contains the implementation of the entry points associated to the
 * asymmetric signature capability as described by the PSA Cryptoprocessor
 * Driver interface specification
 *
 */
#include "mcux_psa_s2xx_common_init.h"
#include "mcux_psa_s2xx_opaque_asymmetric_signature.h"
#include "mcux_psa_s2xx_key_locations.h"
#include "mcux_psa_s2xx_common_key_management.h"
#include "mcux_psa_s2xx_hash.h"
#include "mcux_psa_s2xx_common_compute.h"

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

static psa_status_t key_management(const psa_key_attributes_t *attributes,
                                   const uint8_t *key_buffer,
                                   size_t key_buffer_size,
                                   sss_sscp_object_t *sssKey)
{
    psa_status_t psa_status = PSA_ERROR_CORRUPTION_DETECTED;

    /* Validate if the key is a blob */
    psa_status = ele_s2xx_validate_blob_attributes(attributes, key_buffer, key_buffer_size);
    if (PSA_SUCCESS != psa_status)
    {
        return psa_status;
    }

    /* Import the key */
    psa_status = ele_s2xx_import_key(attributes, key_buffer, key_buffer_size, sssKey);
    if (PSA_SUCCESS != psa_status)
    {
        return psa_status;
    }

    return PSA_SUCCESS;
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

static psa_status_t validate_key_bitlen_for_message_sign(const psa_key_attributes_t *attributes,
                                                         psa_algorithm_t alg)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    if (true == PSA_ALG_IS_ECDSA(alg))
    {
        /* We will be pre-hashing the message for ECDSA, so we know that the hash length
         * will be PSA_HASH_LENGTH(PSA_ALG_SIGN_GET_HASH(alg))
         */
        status = validate_key_bitlen_for_hash_sign(attributes, alg, PSA_HASH_LENGTH(PSA_ALG_SIGN_GET_HASH(alg)));
    }
    else if ((PSA_ALG_PURE_EDDSA == alg) && (psa_get_key_bits(attributes) != ED25519_BITLEN))
    {
        /* S200 supports only 255 bitlen for EdDSA (Ed25519) */
        status = PSA_ERROR_NOT_SUPPORTED;
    }
    else
    {
        status = PSA_ERROR_NOT_SUPPORTED;
    }

    return status;
}

psa_status_t ele_s2xx_opaque_sign_hash(const psa_key_attributes_t *attributes,
                                       const uint8_t *key_buffer, size_t key_buffer_size,
                                       psa_algorithm_t alg,
                                       const uint8_t *hash, size_t hash_length,
                                       uint8_t *signature, size_t signature_size,
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

    if (mcux_mutex_lock(&ele_hwcrypto_mutex))
    {
        return PSA_ERROR_COMMUNICATION_FAILURE;
    }

    status = key_management(attributes, key_buffer, key_buffer_size, &sssKey);
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
    if (mcux_mutex_unlock(&ele_hwcrypto_mutex))
    {
        return PSA_ERROR_BAD_STATE;
    }

    return status;
}

psa_status_t ele_s2xx_opaque_verify_hash(const psa_key_attributes_t *attributes,
                                         const uint8_t *key_buffer, size_t key_buffer_size,
                                         psa_algorithm_t alg,
                                         const uint8_t *hash, size_t hash_length,
                                         const uint8_t *signature, size_t signature_length)
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

    if (mcux_mutex_lock(&ele_hwcrypto_mutex))
    {
        return PSA_ERROR_COMMUNICATION_FAILURE;
    }

    status = key_management(attributes, key_buffer, key_buffer_size, &sssKey);
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
    if (mcux_mutex_unlock(&ele_hwcrypto_mutex))
    {
        return PSA_ERROR_BAD_STATE;
    }

    return status;
}

psa_status_t ele_s2xx_opaque_sign_message(const psa_key_attributes_t *attributes,
                                          const uint8_t *key_buffer, size_t key_buffer_size,
                                          psa_algorithm_t alg,
                                          const uint8_t *input, size_t input_length,
                                          uint8_t *signature, size_t signature_size,
                                          size_t *signature_length)
{
    psa_status_t status                            = PSA_ERROR_CORRUPTION_DETECTED;
    sss_sscp_object_t sssKey                       = {0};
    sss_algorithm_t ele_alg                        = {0};
    uint8_t hash[PSA_HASH_LENGTH(PSA_ALG_SHA_512)] = {0u};
    size_t hash_length                             = 0u;

    /* Convert PSA_ALG_* to ELE value and validate supported alg */
    status = ele_s2xx_psa_2_ele_asym_alg(attributes, alg, &ele_alg);
    if (PSA_SUCCESS != status)
    {
        return status;
    }

    /* Check if we were given one of the supported algs (EdDSA and ECDSA on S200) and
     * check if key is supported for use with alg
     */
    status = validate_key_bitlen_for_message_sign(attributes, alg);
    if (PSA_SUCCESS != status)
    {
        return status;
    }

    if (NULL == key_buffer || 0u == key_buffer_size)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (NULL == input || 0u == input_length)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (NULL == signature || NULL == signature_length)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Pre-hash for ECDSA */
    if (true == PSA_ALG_IS_ECDSA(alg))
    {
        status = ele_s2xx_transparent_hash_compute(PSA_ALG_SIGN_GET_HASH(alg),
                                                   input, input_length,
                                                   hash, sizeof(hash),
                                                   &hash_length);
        if (PSA_SUCCESS != status)
        {
            return status;
        }

        /* Update the inputs for the call to sign */
        input        = hash;
        input_length = hash_length;
    }

    if (mcux_mutex_lock(&ele_hwcrypto_mutex))
    {
        return PSA_ERROR_COMMUNICATION_FAILURE;
    }

    status = key_management(attributes, key_buffer, key_buffer_size, &sssKey);
    if (PSA_SUCCESS != status)
    {
        goto exit;
    }

    *signature_length = signature_size;
    status = ele_s2xx_common_sign_digest((uint8_t *)input, input_length, signature, signature_length, &sssKey, ele_alg);
    if (PSA_SUCCESS != status)
    {
        goto exit;
    }

exit:
    if (mcux_mutex_unlock(&ele_hwcrypto_mutex))
    {
        return PSA_ERROR_BAD_STATE;
    }

    return status;
}

psa_status_t ele_s2xx_opaque_verify_message(const psa_key_attributes_t *attributes,
                                            const uint8_t *key_buffer, size_t key_buffer_size,
                                            psa_algorithm_t alg,
                                            const uint8_t *input, size_t input_length,
                                            const uint8_t *signature, size_t signature_length)
{
    psa_status_t status                            = PSA_ERROR_CORRUPTION_DETECTED;
    sss_sscp_object_t sssKey                       = {0};
    sss_algorithm_t ele_alg                        = {0};
    uint8_t hash[PSA_HASH_LENGTH(PSA_ALG_SHA_512)] = {0u};
    size_t hash_length                             = 0u;

    /* Convert PSA_ALG_* to ELE value and validate supported alg */
    status = ele_s2xx_psa_2_ele_asym_alg(attributes, alg, &ele_alg);
    if (PSA_SUCCESS != status)
    {
        return status;
    }

    /* Check if we were given one of the supported algs (EdDSA and ECDSA on S200) and
     * check if key is supported for use with alg
     */
    status = validate_key_bitlen_for_message_sign(attributes, alg);
    if (PSA_SUCCESS != status)
    {
        return status;
    }

    if (NULL == key_buffer || 0u == key_buffer_size)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (NULL == input || 0u == input_length)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (NULL == signature || 0u == signature_length)
    {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Pre-hash for ECDSA */
    if (true == PSA_ALG_IS_ECDSA(alg))
    {
        status = ele_s2xx_transparent_hash_compute(PSA_ALG_SIGN_GET_HASH(alg),
                                                   input, input_length,
                                                   hash, sizeof(hash),
                                                   &hash_length);
        if (PSA_SUCCESS != status)
        {
            return status;
        }

        /* Update the inputs for the call to sign */
        input        = hash;
        input_length = hash_length;
    }

    if (mcux_mutex_lock(&ele_hwcrypto_mutex))
    {
        return PSA_ERROR_COMMUNICATION_FAILURE;
    }

    status = key_management(attributes, key_buffer, key_buffer_size, &sssKey);
    if (PSA_SUCCESS != status)
    {
        goto exit;
    }

    status = ele_s2xx_common_verify_digest((uint8_t *)input, input_length, (uint8_t *)signature, signature_length, &sssKey, ele_alg);
    if (PSA_SUCCESS != status)
    {
        goto exit;
    }

exit:
    if (mcux_mutex_unlock(&ele_hwcrypto_mutex))
    {
        return PSA_ERROR_BAD_STATE;
    }

    return status;
}
