/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "el2go_blob_test_executor_psa.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __ZEPHYR__
#include <zephyr/kernel.h>
#endif

#ifdef CONFIG_LARGE_BLOBS_ENABLED
#define SKIP_LARGE_BLOBS 0
#else
#define SKIP_LARGE_BLOBS 1
#endif

#define CHECK_BLOB(...)                                                           \
    if (blob_size == 1)                                                           \
    {                                                                             \
        TEST_SKIP("Placeholder blob");                                            \
        return;                                                                   \
    }                                                                             \
    else if (SKIP_LARGE_BLOBS && key_bits > 2783U * 8U)                           \
    {                                                                             \
        TEST_SKIP("Key in blob larger than 2783 bytes");                          \
        return;                                                                   \
    }                                                                             \
    else if ((key_type == PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1) ||    \
              key_type == PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1)) && \
             (key_bits == 224 || key_bits == 225))                                \
    {                                                                             \
        TEST_SKIP("secp224k1 is broken in mbedtls PSA");                          \
        return;                                                                   \
    }

// Constants

// https://arm-software.github.io/psa-api/crypto/1.1/api/ops/sign.html#c.PSA_ALG_RSA_PKCS1V15_SIGN_RAW
// https://datatracker.ietf.org/doc/html/rfc8017.html#appendix-A.2.4
static const uint8_t rsa_digest_info_sha_1[]       = {0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e,
                                                      0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14};
static const uint8_t rsa_digest_info_sha_224[]     = {0x30, 0x2d, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                                      0x65, 0x03, 0x04, 0x02, 0x04, 0x05, 0x00, 0x04, 0x1c};
static const uint8_t rsa_digest_info_sha_512_224[] = {0x30, 0x2d, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                                      0x65, 0x03, 0x04, 0x02, 0x05, 0x05, 0x00, 0x04, 0x1c};
static const uint8_t rsa_digest_info_sha_256[]     = {0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                                      0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20};
static const uint8_t rsa_digest_info_sha_512_256[] = {0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                                      0x65, 0x03, 0x04, 0x02, 0x06, 0x05, 0x00, 0x04, 0x20};
static const uint8_t rsa_digest_info_sha_384[]     = {0x30, 0x41, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                                      0x65, 0x03, 0x04, 0x02, 0x02, 0x05, 0x00, 0x04, 0x30};
static const uint8_t rsa_digest_info_sha_512[]     = {0x30, 0x51, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                                      0x65, 0x03, 0x04, 0x02, 0x03, 0x05, 0x00, 0x04, 0x40};

static const psa_algorithm_t any_hash_algs[] = {PSA_ALG_SHA_1,   PSA_ALG_SHA_224,     PSA_ALG_SHA_512_224,
                                                PSA_ALG_SHA_256, PSA_ALG_SHA_512_256, PSA_ALG_SHA_384,
                                                PSA_ALG_SHA_512};

// Helper functions

static bool is_internal(psa_key_location_t key_location)
{
    return key_location == PSA_KEY_LOCATION_S50_BLOB_STORAGE;
}

static const char *get_hash_name(psa_algorithm_t hash_alg)
{
    switch (hash_alg)
    {
        case PSA_ALG_SHA_1:
            return "SHA_1";
        case PSA_ALG_SHA_224:
            return "SHA_224";
        case PSA_ALG_SHA_512_224:
            return "SHA_512_224";
        case PSA_ALG_SHA_256:
            return "SHA_256";
        case PSA_ALG_SHA_512_256:
            return "SHA_512_256";
        case PSA_ALG_SHA_384:
            return "SHA_384";
        case PSA_ALG_SHA_512:
            return "SHA_512";
        default:
            return NULL;
    }
}

static const uint8_t *get_rsa_digest_info(psa_algorithm_t hash_alg)
{
    switch (hash_alg)
    {
        case PSA_ALG_SHA_1:
            return rsa_digest_info_sha_1;
        case PSA_ALG_SHA_224:
            return rsa_digest_info_sha_224;
        case PSA_ALG_SHA_512_224:
            return rsa_digest_info_sha_512_224;
        case PSA_ALG_SHA_256:
            return rsa_digest_info_sha_256;
        case PSA_ALG_SHA_512_256:
            return rsa_digest_info_sha_512_256;
        case PSA_ALG_SHA_384:
            return rsa_digest_info_sha_384;
        case PSA_ALG_SHA_512:
            return rsa_digest_info_sha_512;
        default:
            return NULL;
    }
}

static psa_algorithm_t get_usage_key_alg(psa_algorithm_t key_alg,
                                         size_t key_bits,
                                         psa_algorithm_t hash_alg,
                                         bool internal)
{
    if (PSA_ALG_IS_ECDSA(key_alg))
    {
        if ((key_bits >= 384 && hash_alg == PSA_ALG_SHA_1) ||
            (key_bits >= 512 && (hash_alg == PSA_ALG_SHA_224 || hash_alg == PSA_ALG_SHA_512_224)) ||
            (internal && hash_alg != PSA_ALG_SHA_256))
        {
            return PSA_ALG_NONE;
        }
        if (key_alg == PSA_ALG_ECDSA_ANY)
        {
            return PSA_ALG_ECDSA_ANY;
        }
        else
        {
            return PSA_ALG_ECDSA(hash_alg);
        }
    }
    else if (PSA_ALG_IS_RSA_PKCS1V15_SIGN(key_alg))
    {
#ifdef CONFIG_RUN_VERIFIED_ONLY
        if (hash_alg == PSA_ALG_SHA_1 || hash_alg == PSA_ALG_SHA_512_224 || hash_alg == PSA_ALG_SHA_512_256)
        {
            return PSA_ALG_NONE;
        }
#endif
        if (key_alg == PSA_ALG_RSA_PKCS1V15_SIGN_RAW)
        {
            return PSA_ALG_RSA_PKCS1V15_SIGN_RAW;
        }
        else
        {
            return PSA_ALG_RSA_PKCS1V15_SIGN(hash_alg);
        }
    }
    else if (PSA_ALG_IS_RSA_PSS(key_alg))
    {
#ifdef CONFIG_RUN_VERIFIED_ONLY
        if (hash_alg == PSA_ALG_SHA_1 || hash_alg == PSA_ALG_SHA_512_224 || hash_alg == PSA_ALG_SHA_512_256)
        {
            return PSA_ALG_NONE;
        }
#endif
        if (key_bits <= 1024 && hash_alg == PSA_ALG_SHA_512)
        {
            return PSA_ALG_NONE;
        }
        return PSA_ALG_RSA_PSS(hash_alg);
    }

    return PSA_ALG_NONE;
}

// Cipher functions

void psa_blob_cipher_test(psa_key_type_t key_type,
                          size_t key_bits,
                          const psa_algorithm_t key_alg,
                          psa_key_location_t key_location,
                          size_t key_id,
                          const uint8_t *blob,
                          size_t blob_size,
                          struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    uint8_t *ciphertext          = NULL;
    uint8_t *decrypted_plaintext = NULL;

    const uint8_t plaintext[] = "This is the plaintxt to encrypt";
    size_t ciphertext_size    = PSA_CIPHER_ENCRYPT_OUTPUT_SIZE(key_type, key_alg, sizeof(plaintext));
    ciphertext                = malloc(ciphertext_size);
    size_t ciphertext_length  = 0;
    psa_status =
        psa_cipher_encrypt(id, key_alg, plaintext, sizeof(plaintext), ciphertext, ciphertext_size, &ciphertext_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_cipher_encrypt");
        goto cleanup;
    }

    size_t decrypted_plaintext_size   = PSA_CIPHER_DECRYPT_OUTPUT_SIZE(key_type, key_alg, ciphertext_length);
    decrypted_plaintext               = malloc(decrypted_plaintext_size);
    size_t decrypted_plaintext_length = 0;
    psa_status = psa_cipher_decrypt(id, key_alg, ciphertext, ciphertext_length, decrypted_plaintext,
                                    decrypted_plaintext_size, &decrypted_plaintext_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_cipher_decrypt");
        goto cleanup;
    }

    uint32_t comp_result = memcmp(plaintext, decrypted_plaintext, sizeof(plaintext));
    if (comp_result != 0 || sizeof(plaintext) != decrypted_plaintext_length)
    {
        TEST_FAIL("Decrypted data doesn't match with plaintext");
        goto cleanup;
    }

cleanup:
    free(ciphertext);
    free(decrypted_plaintext);

    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

void psa_blob_encrypt_test(psa_key_type_t key_type,
                           size_t key_bits,
                           const psa_algorithm_t key_alg,
                           psa_key_location_t key_location,
                           size_t key_id,
                           const uint8_t *blob,
                           size_t blob_size,
                           struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    uint8_t *ciphertext = NULL;

    const uint8_t plaintext[] = "This is the plaintxt to encrypt";
    size_t ciphertext_size    = PSA_CIPHER_ENCRYPT_OUTPUT_SIZE(key_type, key_alg, sizeof(plaintext));
    ciphertext                = malloc(ciphertext_size);
    size_t ciphertext_length  = 0;
    psa_status =
        psa_cipher_encrypt(id, key_alg, plaintext, sizeof(plaintext), ciphertext, ciphertext_size, &ciphertext_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_cipher_encrypt");
        goto cleanup;
    }

cleanup:
    free(ciphertext);

    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

void psa_blob_decrypt_test(psa_key_type_t key_type,
                           size_t key_bits,
                           const psa_algorithm_t key_alg,
                           psa_key_location_t key_location,
                           size_t key_id,
                           const uint8_t *blob,
                           size_t blob_size,
                           struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    uint8_t *plaintext = NULL;

    const uint8_t ciphertext[] = {0xd4, 0xd7, 0x61, 0x4e, 0x93, 0xb6, 0xe1, 0x8a, 0x5f, 0xe8, 0x06,
                                  0x30, 0x98, 0xfe, 0x41, 0x90, 0xe0, 0x3c, 0xec, 0x2d, 0x51, 0xd7,
                                  0x1d, 0x34, 0x8d, 0xbf, 0x6d, 0xa3, 0x00, 0xe8, 0xfc, 0x61};
    size_t plaintext_size      = sizeof(ciphertext);
    plaintext                  = malloc(plaintext_size);
    size_t plaintext_length    = 0;
    psa_status =
        psa_cipher_decrypt(id, key_alg, ciphertext, sizeof(ciphertext), plaintext, plaintext_size, &plaintext_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_cipher_decrypt");
        goto cleanup;
    }

cleanup:
    free(plaintext);

    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

// AEAD functions

void psa_blob_aead_test(psa_key_type_t key_type,
                        size_t key_bits,
                        const psa_algorithm_t key_alg,
                        psa_key_location_t key_location,
                        size_t key_id,
                        const uint8_t *blob,
                        size_t blob_size,
                        struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    uint8_t *nonce               = NULL;
    uint8_t *ciphertext          = NULL;
    uint8_t *decrypted_plaintext = NULL;

    size_t nonce_length = PSA_AEAD_NONCE_LENGTH(key_type, key_alg);
    nonce               = malloc(nonce_length);
    psa_status          = psa_generate_random(nonce, nonce_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_generate_random");
        goto cleanup;
    }

    const uint8_t plaintext[] = "This is the plaintxt to encrypt";
    size_t ciphertext_size    = PSA_AEAD_ENCRYPT_OUTPUT_SIZE(key_type, key_alg, sizeof(plaintext));
    ciphertext                = malloc(ciphertext_size);
    size_t ciphertext_length  = 0;
    psa_status = psa_aead_encrypt(id, key_alg, nonce, nonce_length, 0, 0, plaintext, sizeof(plaintext), ciphertext,
                                  ciphertext_size, &ciphertext_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_aead_encrypt");
        goto cleanup;
    }

    size_t decrypted_plaintext_size   = PSA_AEAD_DECRYPT_OUTPUT_SIZE(key_type, key_alg, ciphertext_length);
    decrypted_plaintext               = malloc(decrypted_plaintext_size);
    size_t decrypted_plaintext_length = 0;
    psa_status = psa_aead_decrypt(id, key_alg, nonce, nonce_length, 0, 0, ciphertext, ciphertext_length,
                                  decrypted_plaintext, decrypted_plaintext_size, &decrypted_plaintext_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_aead_decrypt");
        goto cleanup;
    }

    uint32_t comp_result = memcmp(plaintext, decrypted_plaintext, sizeof(plaintext));
    if (comp_result != 0 || sizeof(plaintext) != decrypted_plaintext_length)
    {
        TEST_FAIL("Decrypted data doesn't match with plaintext");
        goto cleanup;
    }

cleanup:
    free(nonce);
    free(ciphertext);
    free(decrypted_plaintext);

    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

void psa_blob_aead_encrypt_test(psa_key_type_t key_type,
                                size_t key_bits,
                                const psa_algorithm_t key_alg,
                                psa_key_location_t key_location,
                                size_t key_id,
                                const uint8_t *blob,
                                size_t blob_size,
                                struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    uint8_t *nonce      = NULL;
    uint8_t *ciphertext = NULL;

    size_t nonce_length = PSA_AEAD_NONCE_LENGTH(key_type, key_alg);
    nonce               = malloc(nonce_length);
    psa_status          = psa_generate_random(nonce, nonce_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_generate_random");
        goto cleanup;
    }

    const uint8_t plaintext[] = "This is the plaintxt to encrypt";
    size_t ciphertext_size    = PSA_AEAD_ENCRYPT_OUTPUT_SIZE(key_type, key_alg, sizeof(plaintext));
    ciphertext                = malloc(ciphertext_size);
    size_t ciphertext_length  = 0;
    psa_status = psa_aead_encrypt(id, key_alg, nonce, nonce_length, 0, 0, plaintext, sizeof(plaintext), ciphertext,
                                  ciphertext_size, &ciphertext_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_aead_encrypt");
        goto cleanup;
    }

cleanup:
    free(nonce);
    free(ciphertext);

    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

void psa_blob_aead_decrypt_test(psa_key_type_t key_type,
                                size_t key_bits,
                                const psa_algorithm_t key_alg,
                                psa_key_location_t key_location,
                                size_t key_id,
                                const uint8_t *blob,
                                size_t blob_size,
                                struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    uint8_t *nonce               = NULL;
    uint8_t *decrypted_plaintext = NULL;

    size_t nonce_length = PSA_AEAD_NONCE_LENGTH(key_type, key_alg);
    nonce               = malloc(nonce_length);
    psa_status          = psa_generate_random(nonce, nonce_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_generate_random");
        goto cleanup;
    }

    const uint8_t ciphertext[]        = {0xd4, 0xd7, 0x61, 0x4e, 0x93, 0xb6, 0xe1, 0x8a,
                                         0x5f, 0xe8, 0x06, 0x30, 0x98, 0xfe, 0x41, 0x90};
    size_t ciphertext_length          = sizeof(ciphertext);
    size_t decrypted_plaintext_size   = PSA_AEAD_DECRYPT_OUTPUT_SIZE(key_type, key_alg, ciphertext_length);
    decrypted_plaintext               = malloc(decrypted_plaintext_size);
    size_t decrypted_plaintext_length = 0;
    psa_status = psa_aead_decrypt(id, key_alg, nonce, nonce_length, 0, 0, ciphertext, ciphertext_length,
                                  decrypted_plaintext, decrypted_plaintext_size, &decrypted_plaintext_length);

    if (psa_status == PSA_ERROR_INVALID_SIGNATURE)
    {
        // Decrypt only will always result an invalid ciphertext auth, this case is just a sanity usage check
        psa_status = PSA_SUCCESS;
    }
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_aead_decrypt");
        goto cleanup;
    }

cleanup:
    free(nonce);
    free(decrypted_plaintext);

    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

// MAC functions

void psa_blob_mac_test(psa_key_type_t key_type,
                       size_t key_bits,
                       const psa_algorithm_t key_alg,
                       psa_key_location_t key_location,
                       size_t key_id,
                       const uint8_t *blob,
                       size_t blob_size,
                       struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    uint8_t *mac = NULL;

    const uint8_t message[] = "This is the message to authenticate";
    size_t mac_size         = PSA_MAC_LENGTH(key_type, key_bits, key_alg);
    mac                     = malloc(mac_size);
    size_t mac_length       = 0;
    psa_status              = psa_mac_compute(id, key_alg, message, sizeof(message), mac, mac_size, &mac_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_mac_compute");
        goto cleanup;
    }

    psa_status = psa_mac_verify(id, key_alg, message, sizeof(message), mac, mac_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_mac_verify");
        goto cleanup;
    }

cleanup:
    free(mac);

    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

void psa_blob_mac_compute_test(psa_key_type_t key_type,
                               size_t key_bits,
                               const psa_algorithm_t key_alg,
                               psa_key_location_t key_location,
                               size_t key_id,
                               const uint8_t *blob,
                               size_t blob_size,
                               struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    uint8_t *mac = NULL;

    const uint8_t message[] = "This is the message to authenticate";
    size_t mac_size         = PSA_MAC_LENGTH(key_type, key_bits, key_alg);
    mac                     = malloc(mac_size);
    size_t mac_length       = 0;
    psa_status              = psa_mac_compute(id, key_alg, message, sizeof(message), mac, mac_size, &mac_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_mac_compute");
        goto cleanup;
    }

cleanup:
    free(mac);

    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

void psa_blob_mac_verify_test(psa_key_type_t key_type,
                              size_t key_bits,
                              const psa_algorithm_t key_alg,
                              psa_key_location_t key_location,
                              size_t key_id,
                              const uint8_t *blob,
                              size_t blob_size,
                              struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_VERIFY_MESSAGE);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    uint8_t *mac = NULL;

    const uint8_t message[] = "This is the message to authenticate";
    size_t mac_size         = PSA_MAC_LENGTH(key_type, key_bits, key_alg);
    mac                     = malloc(mac_size);
    // Mock a signature of correct length
    size_t mac_length = mac_size;

    psa_status = psa_mac_verify(id, key_alg, message, sizeof(message), mac, mac_length);

    if (psa_status == PSA_ERROR_INVALID_SIGNATURE)
    {
        // Verify only will always result an invalid signature, this case is just a sanity usage check
        psa_status = PSA_SUCCESS;
    }
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_mac_verify");
        goto cleanup;
    }

cleanup:
    free(mac);

    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

// Sign & verify message functions

static psa_status_t psa_blob_sig_ver_msg(psa_key_type_t key_type,
                                         size_t key_bits,
                                         const psa_algorithm_t key_alg,
                                         size_t id,
                                         bool execute_sign,
                                         bool execute_verify,
                                         enum indentation_t indentation,
                                         struct test_result_t *result)
{
    psa_status_t psa_status;

    const uint8_t message[] = "This is the message to sign";

    size_t signature_size   = PSA_SIGN_OUTPUT_SIZE(key_type, key_bits, key_alg);
    uint8_t *signature      = malloc(signature_size);
    size_t signature_length = 0;

    if (execute_sign)
    {
        psa_status =
            psa_sign_message(id, key_alg, message, sizeof(message), signature, signature_size, &signature_length);
        if (psa_status != PSA_SUCCESS)
        {
            TEST_FAIL_PSA_INDENT("psa_sign_message", indentation);
            goto cleanup;
        }
    }

    if (execute_verify)
    {
        if (!execute_sign)
        {
            // Mock a signature of correct length
            signature_length = signature_size;
        }
        psa_status = psa_verify_message(id, key_alg, message, sizeof(message), signature, signature_length);
        if (!execute_sign && psa_status == PSA_ERROR_INVALID_SIGNATURE)
        {
            // Verify only will always result an invalid signature, this case is just a sanity usage check
            psa_status = PSA_SUCCESS;
        }
        if (psa_status != PSA_SUCCESS)
        {
            TEST_FAIL_PSA_INDENT("psa_verify_message", indentation);
            goto cleanup;
        }
    }

cleanup:
    free(signature);

    return psa_status;
}

static void psa_blob_sig_ver_msg_variations(psa_key_type_t key_type,
                                            size_t key_bits,
                                            const psa_algorithm_t key_alg,
                                            psa_key_location_t key_location,
                                            size_t id,
                                            bool execute_sign,
                                            bool execute_verify,
                                            struct test_result_t *result)
{
    for (size_t i = 0; i < (sizeof(any_hash_algs) / sizeof(psa_algorithm_t)); i++)
    {
        psa_algorithm_t hash_alg      = any_hash_algs[i];
        psa_algorithm_t usage_key_alg = get_usage_key_alg(key_alg, key_bits, hash_alg, is_internal(key_location));
        if (usage_key_alg == PSA_ALG_NONE)
        {
            continue;
        }
        LOG_SET_COLOR(DEFAULT);
        LOG("%*s> Executing variation %s\r\n", TEST, "", get_hash_name(hash_alg));

#ifdef __ZEPHYR__
        int64_t start_time = k_uptime_get();
#else
        uint32_t start_time   = get_uptime_ms();
#endif

        psa_status_t psa_status = psa_blob_sig_ver_msg(key_type, key_bits, usage_key_alg, id, execute_sign,
                                                       execute_verify, VARIATION, result);

#ifdef __ZEPHYR__
        uint32_t elapsed_time = k_uptime_delta(&start_time);
#else
        uint32_t elapsed_time = get_uptime_ms() - start_time;
#endif

        if (psa_status != PSA_SUCCESS)
        {
            LOG_SET_COLOR(RED);
            if (result->message != 0)
            {
                LOG("%*s%s", VARIATION, "", result->message);
                if (result->function != 0)
                {
                    LOG(" (%s:%d)\r\n", result->function, result->line);
                }
            }
            else
            {
                if (result->function != 0)
                {
                    LOG("%*sFailed at %s:%d\r\n", VARIATION, "", result->function, result->line);
                }
            }
            LOG("%*sVariation %s - FAILED (%d ms)\r\n", VARIATION, "", get_hash_name(hash_alg), elapsed_time);
        }
        else
        {
            LOG_SET_COLOR(GREEN);
            LOG("%*sVariation %s - PASSED (%d ms)\r\n", VARIATION, "", get_hash_name(hash_alg), elapsed_time);
        }
    }
}

void psa_blob_sigvermsg_test(psa_key_type_t key_type,
                             size_t key_bits,
                             const psa_algorithm_t key_alg,
                             psa_key_location_t key_location,
                             size_t key_id,
                             const uint8_t *blob,
                             size_t blob_size,
                             struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    psa_algorithm_t hash_alg = PSA_ALG_GET_HASH(key_alg);

    if (hash_alg == PSA_ALG_ANY_HASH)
    {
        psa_blob_sig_ver_msg_variations(key_type, key_bits, key_alg, key_location, id, true, true, result);
        if (result->status == TEST_FAILED)
        {
            result->message  = NULL;
            result->function = NULL;
            goto cleanup;
        }
    }
    else
    {
        psa_status = psa_blob_sig_ver_msg(key_type, key_bits, key_alg, id, true, true, TEST, result);
        if (psa_status != PSA_SUCCESS)
        {
            goto cleanup;
        }
    }

cleanup:
    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

void psa_blob_sigmsg_test(psa_key_type_t key_type,
                          size_t key_bits,
                          const psa_algorithm_t key_alg,
                          psa_key_location_t key_location,
                          size_t key_id,
                          const uint8_t *blob,
                          size_t blob_size,
                          struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    psa_algorithm_t hash_alg = PSA_ALG_GET_HASH(key_alg);

    if (hash_alg == PSA_ALG_ANY_HASH)
    {
        psa_blob_sig_ver_msg_variations(key_type, key_bits, key_alg, key_location, id, true, false, result);
        if (result->status == TEST_FAILED)
        {
            result->message  = NULL;
            result->function = NULL;
            goto cleanup;
        }
    }
    else
    {
        psa_status = psa_blob_sig_ver_msg(key_type, key_bits, key_alg, id, true, false, TEST, result);
        if (psa_status != PSA_SUCCESS)
        {
            goto cleanup;
        }
    }

cleanup:
    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

void psa_blob_vermsg_test(psa_key_type_t key_type,
                          size_t key_bits,
                          const psa_algorithm_t key_alg,
                          psa_key_location_t key_location,
                          size_t key_id,
                          const uint8_t *blob,
                          size_t blob_size,
                          struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_VERIFY_MESSAGE);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    psa_algorithm_t hash_alg = PSA_ALG_GET_HASH(key_alg);

    if (hash_alg == PSA_ALG_ANY_HASH)
    {
        psa_blob_sig_ver_msg_variations(key_type, key_bits, key_alg, key_location, id, false, true, result);
        if (result->status == TEST_FAILED)
        {
            result->message  = NULL;
            result->function = NULL;
            goto cleanup;
        }
    }
    else
    {
        psa_status = psa_blob_sig_ver_msg(key_type, key_bits, key_alg, id, false, true, TEST, result);
        if (psa_status != PSA_SUCCESS)
        {
            goto cleanup;
        }
    }

cleanup:
    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

// Sign & verify hash functions

static psa_status_t psa_blob_sig_ver_hash(psa_key_type_t key_type,
                                          size_t key_bits,
                                          const psa_algorithm_t key_alg,
                                          size_t id,
                                          psa_algorithm_t hash_alg,
                                          bool execute_sign,
                                          bool execute_verify,
                                          enum indentation_t indentation,
                                          struct test_result_t *result)
{
    psa_status_t psa_status;

    uint8_t *hash           = NULL;
    uint8_t *signature      = NULL;
    size_t digest_info_size = 0;
    size_t hash_size        = PSA_HASH_LENGTH(hash_alg);
    size_t hash_length      = 0;
    const uint8_t message[] = "This is the message to sign";

    if (key_alg == PSA_ALG_RSA_PKCS1V15_SIGN_RAW)
    {
        const uint8_t *digest_info = get_rsa_digest_info(hash_alg);
        if (!digest_info)
        {
            TEST_FAIL("No RSA digest info avaliable for specified hash algorithm");
            psa_status = PSA_ERROR_GENERIC_ERROR;
            goto cleanup;
        }
        digest_info_size = sizeof(digest_info);
        hash             = malloc(digest_info_size + hash_size);
        memcpy(hash, digest_info, digest_info_size);
        psa_status =
            psa_hash_compute(hash_alg, message, sizeof(message), (hash + digest_info_size), hash_size, &hash_length);
    }
    else
    {
        hash       = malloc(hash_size);
        psa_status = psa_hash_compute(hash_alg, message, sizeof(message), hash, hash_size, &hash_length);
    }
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA_INDENT("psa_hash_compute", indentation);
        goto cleanup;
    }

    if (hash_size != hash_length)
    {
        TEST_FAIL("Hash has wrong size");
        psa_status = PSA_ERROR_GENERIC_ERROR;
        goto cleanup;
    }

    if (key_alg == PSA_ALG_RSA_PKCS1V15_SIGN_RAW)
    {
        hash_length += digest_info_size;
    }

    size_t signature_size   = PSA_SIGN_OUTPUT_SIZE(key_type, key_bits, key_alg);
    signature               = malloc(signature_size);
    size_t signature_length = 0;

    if (execute_sign)
    {
        psa_status = psa_sign_hash(id, key_alg, hash, hash_length, signature, signature_size, &signature_length);
        if (psa_status != PSA_SUCCESS)
        {
            TEST_FAIL_PSA_INDENT("psa_sign_hash", indentation);
            goto cleanup;
        }
    }

    if (execute_verify)
    {
        if (!execute_sign)
        {
            // Mock a signature of correct length
            signature_length = signature_size;
        }
        psa_status = psa_verify_hash(id, key_alg, hash, hash_length, signature, signature_length);
        if (!execute_sign && psa_status == PSA_ERROR_INVALID_SIGNATURE)
        {
            // Verify only will always result an invalid signature, this case is just a sanity usage check
            psa_status = PSA_SUCCESS;
        }
        if (psa_status != PSA_SUCCESS)
        {
            TEST_FAIL_PSA_INDENT("psa_verify_hash", indentation);
            goto cleanup;
        }
    }

cleanup:
    free(hash);
    free(signature);

    return psa_status;
}

static void psa_blob_sig_ver_hash_variations(psa_key_type_t key_type,
                                             size_t key_bits,
                                             const psa_algorithm_t key_alg,
                                             psa_key_location_t key_location,
                                             size_t id,
                                             bool execute_sign,
                                             bool execute_verify,
                                             struct test_result_t *result)
{
    for (size_t i = 0; i < (sizeof(any_hash_algs) / sizeof(psa_algorithm_t)); i++)
    {
        psa_algorithm_t hash_alg      = any_hash_algs[i];
        psa_algorithm_t usage_key_alg = get_usage_key_alg(key_alg, key_bits, hash_alg, is_internal(key_location));
        if (usage_key_alg == PSA_ALG_NONE)
        {
            continue;
        }
        LOG_SET_COLOR(DEFAULT);
        LOG("%*s> Executing variation %s\r\n", TEST, "", get_hash_name(hash_alg));

#ifdef __ZEPHYR__
        int64_t start_time = k_uptime_get();
#else
        uint32_t start_time   = get_uptime_ms();
#endif

        psa_status_t psa_status = psa_blob_sig_ver_hash(key_type, key_bits, usage_key_alg, id, hash_alg, execute_sign,
                                                        execute_verify, VARIATION, result);

#ifdef __ZEPHYR__
        uint32_t elapsed_time = k_uptime_delta(&start_time);
#else
        uint32_t elapsed_time = get_uptime_ms() - start_time;
#endif

        if (psa_status != PSA_SUCCESS)
        {
            LOG_SET_COLOR(RED);
            if (result->message != 0)
            {
                LOG("%*s%s", VARIATION, "", result->message);
                if (result->function != 0)
                {
                    LOG(" (%s:%d)\r\n", result->function, result->line);
                }
            }
            else
            {
                if (result->function != 0)
                {
                    LOG("%*sFailed at %s:%d\r\n", VARIATION, "", result->function, result->line);
                }
            }
            LOG("%*sVariation %s - FAILED (%d ms)\r\n", VARIATION, "", get_hash_name(hash_alg), elapsed_time);
        }
        else
        {
            LOG_SET_COLOR(GREEN);
            LOG("%*sVariation %s - PASSED (%d ms)\r\n", VARIATION, "", get_hash_name(hash_alg), elapsed_time);
        }
    }
}

void psa_blob_sigverhash_test(psa_key_type_t key_type,
                              size_t key_bits,
                              const psa_algorithm_t key_alg,
                              psa_key_location_t key_location,
                              size_t key_id,
                              const uint8_t *blob,
                              size_t blob_size,
                              struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    psa_algorithm_t hash_alg = PSA_ALG_GET_HASH(key_alg);

    if (hash_alg == PSA_ALG_NONE || hash_alg == PSA_ALG_ANY_HASH)
    {
        psa_blob_sig_ver_hash_variations(key_type, key_bits, key_alg, key_location, id, true, true, result);
        if (result->status == TEST_FAILED)
        {
            result->message  = NULL;
            result->function = NULL;
            goto cleanup;
        }
    }
    else
    {
        psa_status = psa_blob_sig_ver_hash(key_type, key_bits, key_alg, id, hash_alg, true, true, TEST, result);
        if (psa_status != PSA_SUCCESS)
        {
            goto cleanup;
        }
    }

cleanup:
    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

void psa_blob_sighash_test(psa_key_type_t key_type,
                           size_t key_bits,
                           const psa_algorithm_t key_alg,
                           psa_key_location_t key_location,
                           size_t key_id,
                           const uint8_t *blob,
                           size_t blob_size,
                           struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    psa_algorithm_t hash_alg = PSA_ALG_GET_HASH(key_alg);

    if (hash_alg == PSA_ALG_NONE || hash_alg == PSA_ALG_ANY_HASH)
    {
        psa_blob_sig_ver_hash_variations(key_type, key_bits, key_alg, key_location, id, true, false, result);
        if (result->status == TEST_FAILED)
        {
            result->message  = NULL;
            result->function = NULL;
            goto cleanup;
        }
    }
    else
    {
        psa_status = psa_blob_sig_ver_hash(key_type, key_bits, key_alg, id, hash_alg, true, false, TEST, result);
        if (psa_status != PSA_SUCCESS)
        {
            goto cleanup;
        }
    }

cleanup:
    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

void psa_blob_verhash_test(psa_key_type_t key_type,
                           size_t key_bits,
                           const psa_algorithm_t key_alg,
                           psa_key_location_t key_location,
                           size_t key_id,
                           const uint8_t *blob,
                           size_t blob_size,
                           struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_VERIFY_HASH);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    psa_algorithm_t hash_alg = PSA_ALG_GET_HASH(key_alg);

    if (hash_alg == PSA_ALG_NONE || hash_alg == PSA_ALG_ANY_HASH)
    {
        psa_blob_sig_ver_hash_variations(key_type, key_bits, key_alg, key_location, id, false, true, result);
        if (result->status == TEST_FAILED)
        {
            result->message  = NULL;
            result->function = NULL;
            goto cleanup;
        }
    }
    else
    {
        psa_status = psa_blob_sig_ver_hash(key_type, key_bits, key_alg, id, hash_alg, false, true, TEST, result);
        if (psa_status != PSA_SUCCESS)
        {
            goto cleanup;
        }
    }

cleanup:
    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

// Export functions

void psa_blob_export_test(psa_key_type_t key_type,
                          size_t key_bits,
                          const psa_algorithm_t key_alg,
                          psa_key_location_t key_location,
                          size_t key_id,
                          const uint8_t *blob,
                          size_t blob_size,
                          struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    if (!PSA_KEY_TYPE_IS_PUBLIC_KEY(key_type))
    {
        psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_EXPORT);
    }
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    uint8_t *exported_key        = NULL;
    uint8_t *exported_public_key = NULL;

    size_t exported_key_size   = PSA_EXPORT_KEY_OUTPUT_SIZE(key_type, key_bits);
    exported_key               = malloc(exported_key_size);
    size_t exported_key_length = 0;
    psa_status                 = psa_export_key(id, exported_key, exported_key_size, &exported_key_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_export_key");
        goto cleanup;
    }

    if (PSA_KEY_TYPE_IS_UNSTRUCTURED(key_type) && exported_key_size != exported_key_length)
    {
        TEST_FAIL("Exported key has wrong size");
        goto cleanup;
    }

    if (PSA_KEY_TYPE_IS_KEY_PAIR(key_type) || PSA_KEY_TYPE_IS_PUBLIC_KEY(key_type))
    {
        size_t exported_public_key_size   = PSA_EXPORT_PUBLIC_KEY_OUTPUT_SIZE(key_type, key_bits);
        exported_public_key               = malloc(exported_public_key_size);
        size_t exported_public_key_length = 0;
        psa_status =
            psa_export_public_key(id, exported_public_key, exported_public_key_size, &exported_public_key_length);
        if (psa_status != PSA_SUCCESS)
        {
            TEST_FAIL_PSA("psa_export_public_key");
            goto cleanup;
        }

        if (PSA_KEY_TYPE_IS_PUBLIC_KEY(key_type))
        {
            uint32_t comp_result = memcmp(exported_key, exported_public_key, exported_public_key_length);
            if (comp_result != 0 || exported_key_length != exported_public_key_length)
            {
                TEST_FAIL("Public keys do not match");
                goto cleanup;
            }
        }
    }

cleanup:
    free(exported_key);
    free(exported_public_key);

    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

// KDF functions

void psa_blob_kdf_test(psa_key_type_t key_type,
                       size_t key_bits,
                       const psa_algorithm_t key_alg,
                       psa_key_location_t key_location,
                       size_t key_id,
                       const uint8_t *blob,
                       size_t blob_size,
                       struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DERIVE);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    psa_key_derivation_operation_t operation;
    operation = psa_key_derivation_operation_init();

    psa_status = psa_key_derivation_setup(&operation, key_alg);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_key_derivation_setup");
        goto cleanup;
    }

    psa_status = psa_key_derivation_input_key(&operation, PSA_KEY_DERIVATION_INPUT_SECRET, id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_key_derivation_input_key");
        goto abort;
    }

    const uint8_t info[] = "This is my info";
    psa_status = psa_key_derivation_input_bytes(&operation, PSA_KEY_DERIVATION_INPUT_INFO, info, sizeof(info));
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_key_derivation_input_bytes");
        goto abort;
    }

    uint8_t derived_secret[16];
    psa_status = psa_key_derivation_output_bytes(&operation, derived_secret, sizeof(derived_secret));
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_key_derivation_output_bytes");
        goto abort;
    }

abort:
    psa_status = psa_key_derivation_abort(&operation);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_key_derivation_abort");
    }
cleanup:
    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

// Key agreement functions

void psa_blob_keyexch_test(psa_key_type_t key_type,
                           size_t key_bits,
                           const psa_algorithm_t key_alg,
                           psa_key_location_t key_location,
                           size_t key_id,
                           const uint8_t *blob,
                           size_t blob_size,
                           struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DERIVE);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    uint8_t *peer_key            = NULL;
    uint8_t *public_key          = NULL;
    uint8_t *output              = NULL;
    uint8_t *verification_output = NULL;

    psa_set_key_lifetime(&attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT,
                                                                                     PSA_KEY_LOCATION_LOCAL_STORAGE));
    psa_set_key_id(&attributes, key_id + 1);

    psa_key_id_t peer_id = 0U;
    psa_status           = psa_generate_key(&attributes, &peer_id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_generate_key");
        goto cleanup;
    }

    size_t peer_key_size   = PSA_EXPORT_PUBLIC_KEY_OUTPUT_SIZE(key_type, key_bits);
    peer_key               = malloc(peer_key_size);
    size_t peer_key_length = 0;
    psa_status             = psa_export_public_key(peer_id, peer_key, peer_key_size, &peer_key_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_export_public_key");
        goto cleanup;
    }

    size_t output_size   = PSA_RAW_KEY_AGREEMENT_OUTPUT_SIZE(key_type, key_bits);
    output               = malloc(output_size);
    size_t output_length = 0;
    psa_status = psa_raw_key_agreement(key_alg, id, peer_key, peer_key_length, output, output_size, &output_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_raw_key_agreement");
        goto cleanup;
    }

    if (output_size != output_length)
    {
        TEST_FAIL("Shared secret has wrong size");
        goto cleanup;
    }

    public_key               = malloc(peer_key_size);
    size_t public_key_length = 0;
    psa_status               = psa_export_public_key(id, public_key, peer_key_size, &public_key_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_export_public_key");
        goto cleanup;
    }

    verification_output               = malloc(output_size);
    size_t verification_output_length = 0;
    psa_status = psa_raw_key_agreement(key_alg, peer_id, public_key, public_key_length, verification_output,
                                       output_size, &verification_output_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_raw_key_agreement");
        goto cleanup;
    }

    if (output_size != verification_output_length)
    {
        TEST_FAIL("Shared secret has wrong size");
        goto cleanup;
    }

    uint32_t comp_result = memcmp(output, verification_output, verification_output_length);
    if (comp_result != 0 || output_length != verification_output_length)
    {
        TEST_FAIL("Shared secrets do not match");
        goto cleanup;
    }

cleanup:
    free(peer_key);
    free(public_key);
    free(output);
    free(verification_output);

    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }

    psa_status = psa_destroy_key(peer_id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}

// Crypt functions

void psa_blob_crypt_test(psa_key_type_t key_type,
                         size_t key_bits,
                         const psa_algorithm_t key_alg,
                         psa_key_location_t key_location,
                         size_t key_id,
                         const uint8_t *blob,
                         size_t blob_size,
                         struct test_result_t *result)
{
    CHECK_BLOB();

    psa_status_t psa_status;
    psa_status = psa_crypto_init();
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_crypto_init");
        return;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, key_alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, key_location));
    psa_set_key_bits(&attributes, key_bits);
    psa_set_key_id(&attributes, key_id);

    psa_key_id_t id = 0U;
    psa_status      = psa_import_key(&attributes, blob, blob_size, &id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_import_key");
        return;
    }

    uint8_t *ciphertext          = NULL;
    uint8_t *decrypted_plaintext = NULL;

    const uint8_t plaintext[] = "This is the plaintxt to encrypt";
    size_t ciphertext_size    = PSA_ASYMMETRIC_ENCRYPT_OUTPUT_SIZE(key_type, key_bits, key_alg);
    ciphertext                = malloc(ciphertext_size);
    size_t ciphertext_length  = 0;
    psa_status = psa_asymmetric_encrypt(id, key_alg, plaintext, sizeof(plaintext), NULL, 0, ciphertext, ciphertext_size,
                                        &ciphertext_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_asymmetric_encrypt");
        goto cleanup;
    }

    size_t decrypted_plaintext_size   = PSA_ASYMMETRIC_DECRYPT_OUTPUT_SIZE(key_type, key_bits, key_alg);
    decrypted_plaintext               = malloc(decrypted_plaintext_size);
    size_t decrypted_plaintext_length = 0;
    psa_status = psa_asymmetric_decrypt(id, key_alg, ciphertext, ciphertext_length, NULL, 0, decrypted_plaintext,
                                        decrypted_plaintext_size, &decrypted_plaintext_length);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_asymmetric_decrypt");
        goto cleanup;
    }

    uint32_t comp_result = memcmp(plaintext, decrypted_plaintext, sizeof(plaintext));
    if (comp_result != 0 || sizeof(plaintext) != decrypted_plaintext_length)
    {
        TEST_FAIL("Decrypted data doesn't match with plaintext");
        goto cleanup;
    }

cleanup:
    free(ciphertext);
    free(decrypted_plaintext);

    psa_status = psa_destroy_key(id);
    if (psa_status != PSA_SUCCESS)
    {
        TEST_FAIL_PSA("psa_destroy_key");
    }
}
