/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

// This construct is required so the <psa/crypto.h> include works with the TF-M NS interface, 
// while still having it in this "tfm" subfolder so it doesn't interfere with mbedcrypto.
#include <tfm/crypto.h>

// This is needed because of an incompatibility between the TF-M PSA Interface headers based 
// on mbedTLS 3.4.x and mbedTLS 3.6.x, which is used on the TF-M NS side.
#if defined(MBEDTLS_PSA_CRYPTO_CLIENT)
#include <mbedtls/md.h>
#endif // MBEDTLS_PSA_CRYPTO_CLIENT

#if defined(MBEDTLS_PSA_CRYPTO_C)
#ifndef PSA_SET_KEY_ENROLLMENT_CRYPTO_C
#define PSA_SET_KEY_ENROLLMENT_CRYPTO_C
static inline void psa_set_key_enrollment_algorithm(psa_key_attributes_t *attributes, psa_algorithm_t alg2)
{
    psa_set_key_algorithm(attributes, alg2);
}

static inline psa_algorithm_t psa_get_key_enrollment_algorithm(const psa_key_attributes_t *attributes)
{
    return psa_get_key_algorithm(attributes);
}
#endif // PSA_SET_KEY_ENROLLMENT_CRYPTO_C
#endif // MBEDTLS_PSA_CRYPTO_C