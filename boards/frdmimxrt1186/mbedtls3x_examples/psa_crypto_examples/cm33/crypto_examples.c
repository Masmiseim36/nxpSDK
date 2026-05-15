/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */
/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "psa/crypto.h"
#include <string.h>
#include <stdlib.h>

/* NXP change */
#if defined(MCUX_MBEDTLS)
#include "app.h"
#include "fsl_debug_console.h"
#define printf PRINTF
#else
#include <stdio.h>
#endif

#define ASSERT( predicate )                                                   \
    do                                                                        \
    {                                                                         \
        if (!(predicate))                                                     \
        {                                                                     \
            printf("\tassertion failed at %s:%d - '%s'\r\n",                  \
                   __FILE__, __LINE__, #predicate);                           \
            goto exit;                                                        \
        }                                                                     \
    } while (0)

#define ASSERT_STATUS(actual, expected)                                      \
    do                                                                       \
    {                                                                        \
        if ((actual) != (expected))                                          \
        {                                                                    \
            printf("\tassertion failed at %s:%d - "                          \
                   "actual:%d expected:%d\r\n", __FILE__, __LINE__,          \
                   (psa_status_t) actual, (psa_status_t) expected);          \
            goto exit;                                                       \
        }                                                                    \
    } while (0)

#if !defined(MBEDTLS_PSA_CRYPTO_C) || !defined(MBEDTLS_AES_C) || \
    !defined(MBEDTLS_CIPHER_MODE_CBC) || !defined(MBEDTLS_CIPHER_MODE_CTR) || \
    !defined(MBEDTLS_CIPHER_MODE_WITH_PADDING) || \
    defined(MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER)
int main(void)
{
    printf("MBEDTLS_PSA_CRYPTO_C and/or MBEDTLS_AES_C and/or "
           "MBEDTLS_CIPHER_MODE_CBC and/or MBEDTLS_CIPHER_MODE_CTR "
           "and/or MBEDTLS_CIPHER_MODE_WITH_PADDING "
           "not defined and/or MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER"
           " defined.\r\n");
    return 0;
}
#else

static psa_status_t cipher_operation(psa_cipher_operation_t *operation,
                                     const uint8_t *input,
                                     size_t input_size,
                                     size_t part_size,
                                     uint8_t *output,
                                     size_t output_size,
                                     size_t *output_len)
{
    psa_status_t status;
    size_t bytes_to_write = 0, bytes_written = 0, len = 0;

    *output_len = 0;
    while (bytes_written != input_size) {
        bytes_to_write = (input_size - bytes_written > part_size ?
                          part_size :
                          input_size - bytes_written);

        status = psa_cipher_update(operation, input + bytes_written,
                                   bytes_to_write, output + *output_len,
                                   output_size - *output_len, &len);
        ASSERT_STATUS(status, PSA_SUCCESS);

        bytes_written += bytes_to_write;
        *output_len += len;
    }

    status = psa_cipher_finish(operation, output + *output_len,
                               output_size - *output_len, &len);
    ASSERT_STATUS(status, PSA_SUCCESS);
    *output_len += len;

exit:
    return status;
}

static psa_status_t cipher_encrypt(psa_key_id_t key,
                                   psa_algorithm_t alg,
                                   uint8_t *iv,
                                   size_t iv_size,
                                   const uint8_t *input,
                                   size_t input_size,
                                   size_t part_size,
                                   uint8_t *output,
                                   size_t output_size,
                                   size_t *output_len)
{
    psa_status_t status;
    psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;
    size_t iv_len = 0;

    memset(&operation, 0, sizeof(operation));
    status = psa_cipher_encrypt_setup(&operation, key, alg);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = psa_cipher_generate_iv(&operation, iv, iv_size, &iv_len);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = cipher_operation(&operation, input, input_size, part_size,
                              output, output_size, output_len);
    ASSERT_STATUS(status, PSA_SUCCESS);

exit:
    psa_cipher_abort(&operation);
    return status;
}

static psa_status_t cipher_decrypt(psa_key_id_t key,
                                   psa_algorithm_t alg,
                                   const uint8_t *iv,
                                   size_t iv_size,
                                   const uint8_t *input,
                                   size_t input_size,
                                   size_t part_size,
                                   uint8_t *output,
                                   size_t output_size,
                                   size_t *output_len)
{
    psa_status_t status;
    psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;

    memset(&operation, 0, sizeof(operation));
    status = psa_cipher_decrypt_setup(&operation, key, alg);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = psa_cipher_set_iv(&operation, iv, iv_size);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = cipher_operation(&operation, input, input_size, part_size,
                              output, output_size, output_len);
    ASSERT_STATUS(status, PSA_SUCCESS);

exit:
    psa_cipher_abort(&operation);
    return status;
}

static psa_status_t
cipher_example_encrypt_decrypt_aes_cbc_nopad_1_block(void)
{
    enum {
        block_size = PSA_BLOCK_CIPHER_BLOCK_LENGTH(PSA_KEY_TYPE_AES),
        key_bits = 256,
        part_size = block_size,
    };
    const psa_algorithm_t alg = PSA_ALG_CBC_NO_PADDING;

    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key = 0;
    size_t output_len = 0;
    uint8_t iv[block_size];
    uint8_t input[block_size];
    uint8_t encrypt[block_size];
    uint8_t decrypt[block_size];

    status = psa_generate_random(input, sizeof(input));
    ASSERT_STATUS(status, PSA_SUCCESS);

    psa_set_key_usage_flags(&attributes,
                            PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, key_bits);

    status = psa_generate_key(&attributes, &key);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = cipher_encrypt(key, alg, iv, sizeof(iv),
                            input, sizeof(input), part_size,
                            encrypt, sizeof(encrypt), &output_len);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = cipher_decrypt(key, alg, iv, sizeof(iv),
                            encrypt, output_len, part_size,
                            decrypt, sizeof(decrypt), &output_len);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = memcmp(input, decrypt, sizeof(input));
    ASSERT_STATUS(status, PSA_SUCCESS);

exit:
    psa_destroy_key(key);
    return status;
}

static psa_status_t cipher_example_encrypt_decrypt_aes_cbc_pkcs7_multi(void)
{
    enum {
        block_size = PSA_BLOCK_CIPHER_BLOCK_LENGTH(PSA_KEY_TYPE_AES),
        key_bits = 256,
        input_size = 100,
        part_size = 10,
    };

    const psa_algorithm_t alg = PSA_ALG_CBC_PKCS7;

    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key = 0;
    size_t output_len = 0;
    uint8_t iv[block_size], input[input_size],
            encrypt[input_size + block_size], decrypt[input_size + block_size];

    status = psa_generate_random(input, sizeof(input));
    ASSERT_STATUS(status, PSA_SUCCESS);

    psa_set_key_usage_flags(&attributes,
                            PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, key_bits);

    status = psa_generate_key(&attributes, &key);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = cipher_encrypt(key, alg, iv, sizeof(iv),
                            input, sizeof(input), part_size,
                            encrypt, sizeof(encrypt), &output_len);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = cipher_decrypt(key, alg, iv, sizeof(iv),
                            encrypt, output_len, part_size,
                            decrypt, sizeof(decrypt), &output_len);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = memcmp(input, decrypt, sizeof(input));
    ASSERT_STATUS(status, PSA_SUCCESS);

exit:
    psa_destroy_key(key);
    return status;
}

static psa_status_t cipher_example_encrypt_decrypt_aes_ctr_multi(void)
{
    enum {
        block_size = PSA_BLOCK_CIPHER_BLOCK_LENGTH(PSA_KEY_TYPE_AES),
        key_bits = 256,
        input_size = 100,
        part_size = 10,
    };
    const psa_algorithm_t alg = PSA_ALG_CTR;

    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key = 0;
    size_t output_len = 0;
    uint8_t iv[block_size], input[input_size], encrypt[input_size],
            decrypt[input_size];

    status = psa_generate_random(input, sizeof(input));
    ASSERT_STATUS(status, PSA_SUCCESS);

    psa_set_key_usage_flags(&attributes,
                            PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, key_bits);

    status = psa_generate_key(&attributes, &key);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = cipher_encrypt(key, alg, iv, sizeof(iv),
                            input, sizeof(input), part_size,
                            encrypt, sizeof(encrypt), &output_len);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = cipher_decrypt(key, alg, iv, sizeof(iv),
                            encrypt, output_len, part_size,
                            decrypt, sizeof(decrypt), &output_len);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = memcmp(input, decrypt, sizeof(input));
    ASSERT_STATUS(status, PSA_SUCCESS);

exit:
    psa_destroy_key(key);
    return status;
}

static void cipher_examples(void)
{
    psa_status_t status;

    printf("cipher encrypt/decrypt AES CBC no padding:\r\n");
    status = cipher_example_encrypt_decrypt_aes_cbc_nopad_1_block();
    if (status == PSA_SUCCESS) {
        printf("\tsuccess!\r\n");
    }

    printf("cipher encrypt/decrypt AES CBC PKCS7 multipart:\r\n");
    status = cipher_example_encrypt_decrypt_aes_cbc_pkcs7_multi();
    if (status == PSA_SUCCESS) {
        printf("\tsuccess!\r\n");
    }

    printf("cipher encrypt/decrypt AES CTR multipart:\r\n");
    status = cipher_example_encrypt_decrypt_aes_ctr_multi();
    if (status == PSA_SUCCESS) {
        printf("\tsuccess!\r\n");
    }
}

static void hash_example(void)
{
    psa_status_t status;
    psa_algorithm_t alg = PSA_ALG_SHA_256;
    psa_hash_operation_t operation = PSA_HASH_OPERATION_INIT;
    static const uint8_t message[] =
        "Be that word our sign of parting, bird or fiend! I shrieked upstarting"
        "Get thee back into the tempest and the Nights Plutonian shore!"
        "Leave no black plume as a token of that lie thy soul hath spoken!"
        "Leave my loneliness unbroken! quit the bust above my door!"
        "Take thy beak from out my heart, and take thy form from off my door!"
        "Quoth the raven, Nevermore.  ";

    /* Expected SHA-256 for the message. */
    static const unsigned char sha256[] = {0x63, 0x76, 0xea, 0xcc, 0xc9, 0xa2, 0xc0, 0x43, 0xf4, 0xfb, 0x01,
                                           0x34, 0x69, 0xb3, 0x0c, 0xf5, 0x28, 0x63, 0x5c, 0xfa, 0xa5, 0x65,
                                           0x60, 0xef, 0x59, 0x7b, 0xd9, 0x1c, 0xac, 0xaa, 0x31, 0xf7};
    unsigned char computed_hash[PSA_HASH_MAX_SIZE];
    size_t output_hash_len;

    printf("Hash a message SHA-256:\r\n");

    /* Compute hash of message  */
    status = psa_hash_setup(&operation, alg);
    if (status != PSA_SUCCESS) {
        printf("Failed to begin hash operation\n");
        return;
    }
    status = psa_hash_update(&operation, message, sizeof(message) - 1u);
    if (status != PSA_SUCCESS) {
        printf("Failed to update hash operation\n");
        return;
    }
    status = psa_hash_finish(&operation, computed_hash, sizeof(computed_hash),
                             &output_hash_len);

    if (status != PSA_SUCCESS) {
        printf("Failed to finish hash operation\n");
        return;
    }
    
    ASSERT(memcmp(computed_hash, sha256, output_hash_len) == 0);

    printf("\tsuccess!\r\n");

exit:
    /* Clean up hash operation context */
    psa_hash_abort(&operation);
}

static psa_status_t cipher_example_encrypt_decrypt_aes_cbc_onego(bool padding)
{
    enum {
        block_size = PSA_BLOCK_CIPHER_BLOCK_LENGTH(PSA_KEY_TYPE_AES),
        key_bits = 128,
    };
    
    psa_algorithm_t alg = PSA_ALG_NONE;
    
    if( padding ) {
        printf("cipher encrypt/decrypt AES CBC PKCS7 padding one go:\r\n");
        alg = PSA_ALG_CBC_PKCS7;
    }
    else {
        printf("cipher encrypt/decrypt AES CBC no padding one go:\r\n");
        alg = PSA_ALG_CBC_NO_PADDING;
    }
    
    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key = 0;
    size_t output_len = 0;
    uint8_t input[block_size];
    uint8_t encrypt[block_size + 16u + 16u]; /* Add space for IV and padding */
    uint8_t decrypt[block_size + 16u]; /* Add space for padding */

    status = psa_generate_random(input, sizeof(input));
    ASSERT_STATUS(status, PSA_SUCCESS);

    psa_set_key_usage_flags(&attributes,
                            PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, key_bits);

    status = psa_generate_key(&attributes, &key);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = psa_cipher_encrypt(key, alg,
                                input, sizeof(input),
                                encrypt, sizeof(encrypt), &output_len);

    ASSERT_STATUS(status, PSA_SUCCESS);

    status = psa_cipher_decrypt(key, alg,
                                encrypt, output_len,
                                decrypt, sizeof(decrypt), &output_len);
    ASSERT_STATUS(status, PSA_SUCCESS);

    status = memcmp(input, decrypt, sizeof(input));
    ASSERT_STATUS(status, PSA_SUCCESS);

    if (status == 0u){
      printf("\tsuccess!\r\n");
    }

exit:
    psa_destroy_key(key);
    return status;
}
                          
int main(void)
{
#if defined(MCUX_MBEDTLS)
    /* HW init */
    BOARD_InitHardware();
#endif

    printf(" * PSA crypto example * \r\n\r\n");    
    
    ASSERT( psa_crypto_init( ) == PSA_SUCCESS );
    cipher_examples( );
    cipher_example_encrypt_decrypt_aes_cbc_onego( false );
    cipher_example_encrypt_decrypt_aes_cbc_onego( true );
    hash_example( );
exit:
    mbedtls_psa_crypto_free();

    printf("\r\n * Example End * \r\n\r\n");    

    while(1)
    {
        GETCHAR();
    }

    return 0;
}
#endif /* MBEDTLS_PSA_CRYPTO_C && MBEDTLS_AES_C && MBEDTLS_CIPHER_MODE_CBC &&
          MBEDTLS_CIPHER_MODE_CTR && MBEDTLS_CIPHER_MODE_WITH_PADDING */