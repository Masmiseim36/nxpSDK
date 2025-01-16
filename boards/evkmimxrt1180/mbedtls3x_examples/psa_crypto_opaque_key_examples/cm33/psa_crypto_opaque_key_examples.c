/*
 *  Example to demonstrate use of opaque keys with PSA API's
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "psa/crypto.h"
#include <string.h>
#include <stdlib.h>
#include "mcux_psa_defines.h"

/* KSDK */
#if defined(MCUX_MBEDTLS)
#include "app.h"
#include "fsl_debug_console.h"
#define printf PRINTF
#else
#include <stdio.h>
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ASSERT(predicate)                                                   \
    do                                                                        \
    {                                                                         \
        if (!(predicate))                                                 \
        {                                                                     \
            printf("\tassertion failed at %s:%d - '%s'\r\n",         \
                   __FILE__, __LINE__, #predicate);                  \
            goto exit;                                                        \
        }                                                                     \
    } while (0)

#define ASSERT_STATUS(actual, expected)                                     \
    do                                                                        \
    {                                                                         \
        if ((actual) != (expected))                                      \
        {                                                                     \
            printf("\tassertion failed at %s:%d - "                  \
                   "actual:%d expected:%d\r\n", __FILE__, __LINE__,  \
                   (psa_status_t) actual, (psa_status_t) expected); \
            goto exit;                                                        \
        }                                                                     \
    } while (0)

#define PSA_ASSERT(status)      ASSERT_STATUS(status, PSA_SUCCESS)

#define PRINT_STATUS(ok)                                    \
    do                                                                        \
    {                                                                         \
        if (ok == 1) {                                              \
            printf(" PASSED\r\n");                                 \
        } else if (ok == 2) {                                                   \
            printf(" NOT SUPPORTED\r\n");                                 \
        } else {                                                   \
            printf(" FAILED\r\n");                                 \
        }								\
    } while (0)

/* The location to be used for generation of keys can come from command line. */
#if !defined(TEST_KEY_LOCATION)
#define TEST_KEY_LOCATION       PSA_KEY_LOCATION_LOCAL_STORAGE
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*
 * Persistent key ID's are in range 1 - 0x3FFFFFFF.
 * For test case, we randomly start from key ID 1.
 */
psa_key_id_t persistent_key_id = 1;
/*******************************************************************************
 * Code
 ******************************************************************************/

static int exercise_single_part_mac(mbedtls_svc_key_id_t key,
                                    psa_key_usage_t usage,
                                    psa_algorithm_t alg)
{
    psa_mac_operation_t operation = PSA_MAC_OPERATION_INIT;
    const unsigned char input[] = "foo";
    unsigned char mac[PSA_MAC_MAX_SIZE] = { 0 };
    size_t mac_length = sizeof(mac);
    psa_status_t status = PSA_ERROR_BAD_STATE;

    /* Convert wildcard algorithm to exercisable algorithm */
    if (alg & PSA_ALG_MAC_AT_LEAST_THIS_LENGTH_FLAG) {
        alg = PSA_ALG_TRUNCATED_MAC(alg, PSA_MAC_TRUNCATED_LENGTH(alg));
    }

    if (usage & PSA_KEY_USAGE_SIGN_HASH) {
        PSA_ASSERT(psa_mac_compute(key, alg,
                                   input, sizeof(input),
                                   mac, sizeof(mac),
                                   &mac_length));
    }

    if (usage & PSA_KEY_USAGE_VERIFY_HASH) {
        psa_status_t verify_status =
            (usage & PSA_KEY_USAGE_SIGN_HASH ?
             PSA_SUCCESS :
             PSA_ERROR_INVALID_SIGNATURE);
        status = psa_mac_verify(key, alg,
                                input, sizeof(input),
                                mac, mac_length);
        ASSERT_STATUS(status, verify_status);
    }

    return 1;

exit:
    psa_mac_abort(&operation);
    return 0;
}

static int exercise_single_part_cipher(mbedtls_svc_key_id_t key,
                                       psa_key_usage_t usage,
                                       psa_algorithm_t alg)
{
    psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    const unsigned char plaintext[16] = "Hello, world...";
    unsigned char ciphertext[32] = "(wabblewebblewibblewobblewubble)";
    size_t ciphertext_length = sizeof(ciphertext);
    unsigned char decrypted[sizeof(ciphertext)];
    size_t part_length;

    PSA_ASSERT(psa_get_key_attributes(key, &attributes));

    if (usage & PSA_KEY_USAGE_ENCRYPT) {
        PSA_ASSERT(psa_cipher_encrypt(key, alg, plaintext, sizeof(plaintext),
                                      ciphertext, sizeof(ciphertext),
                                      &ciphertext_length));
    }

    if (usage & PSA_KEY_USAGE_DECRYPT) {
        psa_status_t status;
        int maybe_invalid_padding = 0;
        if (!(usage & PSA_KEY_USAGE_ENCRYPT)) {
            maybe_invalid_padding = !PSA_ALG_IS_STREAM_CIPHER(alg);
        }
        status = psa_cipher_decrypt(key, alg,
                                    ciphertext, ciphertext_length,
                                    decrypted, sizeof(decrypted),
                                    &part_length);

        /* For a stream cipher, all inputs are valid. For a block cipher,
         * if the input is some arbitrary data rather than an actual
           ciphertext, a padding error is likely.  */
        if (maybe_invalid_padding) {
            if (status == PSA_SUCCESS ||
                status == PSA_ERROR_INVALID_PADDING) {
                return 1;
            }
        } else {
            PSA_ASSERT(status);
        }
    }

    return 1;

exit:
    psa_cipher_abort(&operation);
    psa_reset_key_attributes(&attributes);
    return 0;
}

static int exercise_aead(mbedtls_svc_key_id_t key,
                         psa_key_usage_t usage,
                         psa_algorithm_t alg)
{
    unsigned char nonce[PSA_AEAD_NONCE_MAX_SIZE] = { 0 };
    size_t nonce_length;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_type_t key_type;
    unsigned char plaintext[16] = "Hello, world...";
    unsigned char ciphertext[48] = "(wabblewebblewibblewobblewubble)";
    size_t ciphertext_length = sizeof(ciphertext);
    size_t plaintext_length = sizeof(ciphertext);
    psa_status_t status = PSA_ERROR_BAD_STATE;

    /* Convert wildcard algorithm to exercisable algorithm */
    if (alg & PSA_ALG_AEAD_AT_LEAST_THIS_LENGTH_FLAG) {
        alg = PSA_ALG_AEAD_WITH_SHORTENED_TAG(alg, PSA_ALG_AEAD_GET_TAG_LENGTH(alg));
    }

    status = psa_get_key_attributes(key, &attributes);
    ASSERT_STATUS(status, PSA_SUCCESS);

    key_type = psa_get_key_type(&attributes);
    nonce_length = PSA_AEAD_NONCE_LENGTH(key_type, alg);

    /* For ELE 4XX HSM , for CCM we only support nonce length as 12 */
#if defined(PSA_ELE_S4XX_SD_NVM_MANAGER)
    if (alg == PSA_ALG_CCM) {
        nonce_length = 12u;
    }
#endif

    if (usage & PSA_KEY_USAGE_ENCRYPT) {
        PSA_ASSERT(psa_aead_encrypt(key, alg,
                                    nonce, nonce_length,
                                    NULL, 0,
                                    plaintext, sizeof(plaintext),
                                    ciphertext, sizeof(ciphertext),
                                    &ciphertext_length));
    }

    if (usage & PSA_KEY_USAGE_DECRYPT) {
        psa_status_t verify_status =
            (usage & PSA_KEY_USAGE_ENCRYPT ?
             PSA_SUCCESS :
             PSA_ERROR_INVALID_SIGNATURE);
        status = psa_aead_decrypt(key, alg,
                                  nonce, nonce_length,
                                  NULL, 0,
                                  ciphertext, ciphertext_length,
                                  plaintext, sizeof(plaintext),
                                  &plaintext_length);
        ASSERT_STATUS(status, verify_status);
    }

    return 1;

exit:
    psa_reset_key_attributes(&attributes);
    return 0;
}

static int exercise_export_public_key(mbedtls_svc_key_id_t key)
{
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_type_t public_type;
    uint8_t *exported = NULL;
    size_t exported_size = 0;
    size_t exported_length = 0;
    int ok = 0;

    PSA_ASSERT(psa_get_key_attributes(key, &attributes));
    if (!PSA_KEY_TYPE_IS_ASYMMETRIC(psa_get_key_type(&attributes))) {
        exported_size = PSA_EXPORT_KEY_OUTPUT_SIZE(
            psa_get_key_type(&attributes),
            psa_get_key_bits(&attributes));
        exported = malloc(exported_size);
        if (!exported) {
            return 1;
        }

        ASSERT_STATUS(psa_export_public_key(key, exported,
                                            exported_size, &exported_length),
                      PSA_ERROR_INVALID_ARGUMENT);
        ok = 1;
        goto exit;
    }

    public_type = PSA_KEY_TYPE_PUBLIC_KEY_OF_KEY_PAIR(
        psa_get_key_type(&attributes));
    exported_size = PSA_EXPORT_PUBLIC_KEY_OUTPUT_SIZE(public_type,
                                                      psa_get_key_bits(&attributes));
    exported = malloc(exported_size);
    if (!exported) {
        return 1;
    }

    PSA_ASSERT(psa_export_public_key(key,
                                     exported, exported_size,
                                     &exported_length));

    /* If you have reached here, tests have passed */
    ok = 1;

exit:
    /*
     * Key attributes may have been returned by psa_get_key_attributes()
     * thus reset them as required.
     */
    psa_reset_key_attributes(&attributes);

    free(exported);
    return ok;
}


static int can_sign_or_verify_message(psa_key_usage_t usage,
                                      psa_algorithm_t alg)
{
    /* Sign-the-unspecified-hash algorithms can only be used with
     * {sign,verify}_hash, not with {sign,verify}_message. */
    if (alg == PSA_ALG_ECDSA_ANY || alg == PSA_ALG_RSA_PKCS1V15_SIGN_RAW) {
        return 0;
    }
    return usage & (PSA_KEY_USAGE_SIGN_MESSAGE |
                    PSA_KEY_USAGE_VERIFY_MESSAGE);
}

int exercise_signature(mbedtls_svc_key_id_t key,
                              psa_key_usage_t usage,
                              psa_algorithm_t alg)
{
    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;

    if (usage & (PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH) &&
        PSA_ALG_IS_SIGN_HASH(alg)) {
        unsigned char payload[PSA_HASH_MAX_SIZE] = { 1 };
        size_t payload_length = 16;
        unsigned char signature[PSA_SIGNATURE_MAX_SIZE] = { 0 };
        size_t signature_length = sizeof(signature);
        psa_algorithm_t hash_alg = PSA_ALG_SIGN_GET_HASH(alg);

        /* If the policy allows signing with any hash, just pick one. */
        if (PSA_ALG_IS_SIGN_HASH(alg) && hash_alg == PSA_ALG_ANY_HASH) {
            printf("No hash algorithm for hash-and-sign testing");
            return PSA_ERROR_NOT_SUPPORTED;
        }

        /* Some algorithms require the payload to have the size of
         * the hash encoded in the algorithm. Use this input size
         * even for algorithms that allow other input sizes. */
        if (hash_alg != 0) {
            payload_length = PSA_HASH_LENGTH(hash_alg);
        }

        if (usage & PSA_KEY_USAGE_SIGN_HASH) {
            status = psa_sign_hash(key, alg,
                                   payload, payload_length,
                                   signature, sizeof(signature),
                                   &signature_length);
            ASSERT_STATUS(status, PSA_SUCCESS);
        }

        if (usage & PSA_KEY_USAGE_VERIFY_HASH) {
            psa_status_t verify_status =
                (usage & PSA_KEY_USAGE_SIGN_HASH ?
                 PSA_SUCCESS :
                 PSA_ERROR_INVALID_SIGNATURE);
            status = psa_verify_hash(key, alg,
                                     payload, payload_length,
                                     signature, signature_length);

            ASSERT_STATUS(status, verify_status);
        }
    }

    if (can_sign_or_verify_message(usage, alg)) {
        unsigned char message[256] = "Hello, world...";
        unsigned char signature[PSA_SIGNATURE_MAX_SIZE] = { 0 };
        size_t message_length = 16;
        size_t signature_length = sizeof(signature);

        if (usage & PSA_KEY_USAGE_SIGN_MESSAGE) {
            status = psa_sign_message(key, alg,
                                      message, message_length,
                                      signature, sizeof(signature),
                                      &signature_length);
            ASSERT_STATUS(status, PSA_SUCCESS);

        }

        if (usage & PSA_KEY_USAGE_VERIFY_MESSAGE) {
            psa_status_t verify_status =
                (usage & PSA_KEY_USAGE_SIGN_MESSAGE ?
                 PSA_SUCCESS :
                 PSA_ERROR_INVALID_SIGNATURE);
            status = psa_verify_message(key, alg,
                                        message, message_length,
                                        signature, signature_length);
            ASSERT_STATUS(status, verify_status);
        }
    }

    return 1;
exit:
    return 0;
}

static int exercise_asymmetric_encryption(mbedtls_svc_key_id_t key,
                                          psa_key_usage_t usage,
                                          psa_algorithm_t alg)
{
    unsigned char plaintext[256] = "Hello, world...";
    unsigned char ciphertext[256] = "(wabblewebblewibblewobblewubble)";
    size_t ciphertext_length = sizeof(ciphertext);
    size_t plaintext_length = 16;
    psa_status_t status = PSA_SUCCESS;

    if (usage & PSA_KEY_USAGE_ENCRYPT) {
        status = psa_asymmetric_encrypt(key, alg,
                                        plaintext, plaintext_length,
                                        NULL, 0,
                                        ciphertext, sizeof(ciphertext),
                                        &ciphertext_length);
        ASSERT_STATUS(status, PSA_SUCCESS);
    }

    if (usage & PSA_KEY_USAGE_DECRYPT) {
        status =
            psa_asymmetric_decrypt(key, alg,
                                   ciphertext, ciphertext_length,
                                   NULL, 0,
                                   plaintext, sizeof(plaintext),
                                   &plaintext_length);
        ASSERT_STATUS(status, PSA_SUCCESS);
    }

    /* SUCCESS */
    return 1;
exit:
    /* Failure */
    return 0;
}

int generate_and_test_key(uint16_t key_type_arg, size_t bits_arg,
                           uint32_t usage_arg, uint32_t alg_arg,
                           uint32_t key_lifetime,
                           uint32_t key_location)
{
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key_id = 0;
    psa_key_lifetime_t lifetime =
        PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION( \
            key_lifetime, key_location);
    psa_algorithm_t alg = alg_arg;
    psa_key_usage_t usage = usage_arg;
    psa_key_type_t key_type = key_type_arg;
    size_t bits = bits_arg;
    psa_status_t status;
    int ok = 0;

    /* For persistent keys, we need to choose and pass the ID */
    if (key_lifetime == PSA_KEY_LIFETIME_PERSISTENT) {
        psa_set_key_id(&attributes, persistent_key_id);
        persistent_key_id++;
    }

    psa_set_key_usage_flags(&attributes, usage);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, key_type);
    psa_set_key_bits(&attributes, bits);
    psa_set_key_lifetime(&attributes, lifetime);

    status = psa_generate_key(&attributes, &key_id);

    if (status == PSA_SUCCESS) {
        psa_reset_key_attributes(&attributes);

        /* Get the attributes of generated key and check if they match */
        PSA_ASSERT(psa_get_key_attributes(key_id, &attributes));

        /* Based on the algo set with the key, do the crypot operation and test it */
        /* The code below is borrowed from tests/psa_exercise_key.c */
        if (PSA_ALG_IS_MAC(alg)) {
            ok = exercise_single_part_mac(key_id, usage, alg);
        } else if (PSA_ALG_IS_CIPHER(alg)) {
            ok = exercise_single_part_cipher(key_id, usage, alg);
        } else if (PSA_ALG_IS_AEAD(alg)) {
            ok = exercise_aead(key_id, usage, alg);
        } else if (PSA_ALG_IS_SIGN(alg)) {
            ok = exercise_signature(key_id, usage, alg);
            if (ok) {
                ok = exercise_export_public_key(key_id);
            }
        } else if (PSA_ALG_IS_ASYMMETRIC_ENCRYPTION(alg)) {
            ok = exercise_asymmetric_encryption(key_id, usage, alg);
            if (ok) {
                ok = exercise_export_public_key(key_id);
            }
        }
    } else if (status == PSA_ERROR_NOT_SUPPORTED) {
        ok = 2;
    }

exit:
    psa_reset_key_attributes(&attributes);
    psa_destroy_key(key_id);

    return ok;
}

/* For RSA adding very limited testcases as these are time consuming */
void generate_rsa_keys(psa_key_lifetime_t lifetime)
{
    int ok = 0;

    printf("2048 bit RSA key with sign/verify algo RSA_PKCS1V15(SHA256) :");
    ok = generate_and_test_key(PSA_KEY_TYPE_RSA_KEY_PAIR,
                               2048,
                               PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH,
                               PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256),
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);

    printf("2048 bit RSA key with sign/verify algo RSA_PSS(SHA512) :");
    ok = generate_and_test_key(PSA_KEY_TYPE_RSA_KEY_PAIR,
                               2048,
                               PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH,
                               PSA_ALG_RSA_PSS(PSA_ALG_SHA_512),
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);
}


void generate_mac_keys(psa_key_lifetime_t lifetime)
{
    int ok = 0;

    printf("256 bit PERSISTENT HMAC key with sign/verify algo HMAC(SHA-256):");
    ok = generate_and_test_key(PSA_KEY_TYPE_HMAC,
                               256,
                               PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE,
                               PSA_ALG_HMAC(PSA_ALG_SHA_256),
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);

}

void generate_cipher_keys(psa_key_lifetime_t lifetime)
{
    int ok = 0;

    printf("128 bit AES key with encrypt/decrypt algo ECB_NO_PADDING:");
    ok = generate_and_test_key(PSA_KEY_TYPE_AES,
                               128,
                               PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT,
                               PSA_ALG_ECB_NO_PADDING,
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);


    printf("192 bit AES key with encrypt/decrypt algo CBC_NO_PADDING:");
    ok = generate_and_test_key(PSA_KEY_TYPE_AES,
                               192,
                               PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT,
                               PSA_ALG_CBC_NO_PADDING,
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);

    printf("256 bit AES key with encrypt/decrypt algo CTR:");
    ok = generate_and_test_key(PSA_KEY_TYPE_AES,
                               256,
                               PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT,
                               PSA_ALG_CTR,
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);

    printf("128 bit AES key with encrypt/decrypt algo CCM:");
    ok = generate_and_test_key(PSA_KEY_TYPE_AES,
                               128,
                               PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT,
                               PSA_ALG_CCM,
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);

    printf("256 bit AES key with encrypt/decrypt algo GCM:");
    ok = generate_and_test_key(PSA_KEY_TYPE_AES,
                               256,
                               PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT,
                               PSA_ALG_GCM,
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);


    printf("192 bit AES key with sign/verify algo CMAC :");
    ok = generate_and_test_key(PSA_KEY_TYPE_AES,
                               192,
                               PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE,
                               PSA_ALG_CMAC,
                               lifetime,
                               TEST_KEY_LOCATION);

    PRINT_STATUS(ok);
}

void generate_ecc_keys(psa_key_lifetime_t lifetime)
{
    int ok = 0;

    
    printf("224 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA224):");
    ok = generate_and_test_key(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1),
                               224,
                               PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH,
                               PSA_ALG_ECDSA(PSA_ALG_SHA_224),
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);

    printf("521 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA512):");
    ok = generate_and_test_key(PSA_KEY_TYPE_ECC_KEY_PAIR(
                                   PSA_ECC_FAMILY_SECP_R1),
                               521,
                               PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE,
                               PSA_ALG_ECDSA(PSA_ALG_SHA_512),
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);
    

        printf("256 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA256):");
    ok = generate_and_test_key(PSA_KEY_TYPE_ECC_KEY_PAIR(
                                   PSA_ECC_FAMILY_SECP_R1),
                               256,
                               PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH| PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE,
                               PSA_ALG_ECDSA(PSA_ALG_SHA_256),
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);


    printf("256 ECC_KEY_PAIR(BRAINPOOL_R1) with sign/verify algo ECDSA(SHA256):");
    ok = generate_and_test_key(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1),
                               256,
                               PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH,
                               PSA_ALG_ECDSA(PSA_ALG_SHA_256),
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);

    printf("384 bit ECC_KEY_PAIR(BRAINPOOL_R1) with sign/verify algo ECDSA(SHA384):");
    ok = generate_and_test_key(PSA_KEY_TYPE_ECC_KEY_PAIR(
                                   PSA_ECC_FAMILY_BRAINPOOL_P_R1),
                               384,
                               PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE,
                               PSA_ALG_ECDSA(PSA_ALG_SHA_384),
                               lifetime,
                               TEST_KEY_LOCATION);
    PRINT_STATUS(ok);

}

int main(int argc, char *argv[])
{
    psa_key_lifetime_t lifetime = PSA_KEY_LIFETIME_VOLATILE;
#if defined(MCUX_MBEDTLS)
    /* HW init */
    BOARD_InitHardware();
#endif

    printf("Location is %x\n", TEST_KEY_LOCATION);
    ASSERT(psa_crypto_init() == PSA_SUCCESS);

    printf("\r\nVOLATILE KEYS\r\n");
    printf("\r\nECC keys\r\n");
    generate_ecc_keys(lifetime);

    printf("\r\nAES keys \r\n");
    generate_cipher_keys(lifetime);

    printf("\r\nMAC keys \r\n");
    generate_mac_keys(lifetime);

    printf("\r\nRSA keys \r\n");
    generate_rsa_keys(lifetime);
    
#if defined(MBEDTLS_PSA_CRYPTO_STORAGE_C)
    printf("\r\nPERSISTENT KEYS\r\n");
    
    lifetime = PSA_KEY_LIFETIME_PERSISTENT;
    printf("\r\nECC keys\r\n");
    generate_ecc_keys(lifetime);

    printf("\r\nAES keys \r\n");
    generate_cipher_keys(lifetime);

    printf("\r\nMAC keys \r\n");
    generate_mac_keys(lifetime);

    printf("\r\nRSA keys \r\n");
    generate_rsa_keys(lifetime);
#endif    
           
exit:
    mbedtls_psa_crypto_free();
    return 0;

}
