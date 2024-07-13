/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef __EL2GO_BLOB_TEST_EXECUTOR_PSA_H__
#define __EL2GO_BLOB_TEST_EXECUTOR_PSA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "el2go_blob_test.h"
#include "psa/crypto.h"

#define TEST_FAIL_PSA_INDENT(msg, indentation)                      \
    LOG_SET_COLOR(RED);                                             \
    LOG("%*s%s returned %d\r\n", indentation, "", msg, psa_status); \
    TEST_FAIL(NULL)

#define TEST_FAIL_PSA(msg) TEST_FAIL_PSA_INDENT(msg, TEST);

#define PSA_KEY_LOCATION_S50_ENC_STORAGE_DATA 0xE08101U
#define PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY  0xE00101U
#define PSA_KEY_LOCATION_S50_BLOB_STORAGE     0xE00001U

/**
 * \brief Run cipher tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_cipher_test(psa_key_type_t key_type,
                          size_t key_bits,
                          const psa_algorithm_t key_alg,
                          psa_key_location_t key_location,
                          size_t key_id,
                          const uint8_t *blob,
                          size_t blob_size,
                          struct test_result_t *result);

/**
 * \brief Run encypt tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_encrypt_test(psa_key_type_t key_type,
                           size_t key_bits,
                           const psa_algorithm_t key_alg,
                           psa_key_location_t key_location,
                           size_t key_id,
                           const uint8_t *blob,
                           size_t blob_size,
                           struct test_result_t *result);

/**
 * \brief Run decrypt tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_decrypt_test(psa_key_type_t key_type,
                           size_t key_bits,
                           const psa_algorithm_t key_alg,
                           psa_key_location_t key_location,
                           size_t key_id,
                           const uint8_t *blob,
                           size_t blob_size,
                           struct test_result_t *result);

/**
 * \brief Run AEAD tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_aead_test(psa_key_type_t key_type,
                        size_t key_bits,
                        const psa_algorithm_t key_alg,
                        psa_key_location_t key_location,
                        size_t key_id,
                        const uint8_t *blob,
                        size_t blob_size,
                        struct test_result_t *result);

/**
 * \brief Run AEAD tests with different blobs (only encryption)
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_aead_encrypt_test(psa_key_type_t key_type,
                                size_t key_bits,
                                const psa_algorithm_t key_alg,
                                psa_key_location_t key_location,
                                size_t key_id,
                                const uint8_t *blob,
                                size_t blob_size,
                                struct test_result_t *result);

/**
 * \brief Run AEAD tests with different blobs (only encryption)
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_aead_decrypt_test(psa_key_type_t key_type,
                                size_t key_bits,
                                const psa_algorithm_t key_alg,
                                psa_key_location_t key_location,
                                size_t key_id,
                                const uint8_t *blob,
                                size_t blob_size,
                                struct test_result_t *result);

/**
 * \brief Run MAC tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_mac_test(psa_key_type_t key_type,
                       size_t key_bits,
                       const psa_algorithm_t key_alg,
                       psa_key_location_t key_location,
                       size_t key_id,
                       const uint8_t *blob,
                       size_t blob_size,
                       struct test_result_t *result);

/**
 * \brief Run MAC compute tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_mac_compute_test(psa_key_type_t key_type,
                               size_t key_bits,
                               const psa_algorithm_t key_alg,
                               psa_key_location_t key_location,
                               size_t key_id,
                               const uint8_t *blob,
                               size_t blob_size,
                               struct test_result_t *result);

/**
 * \brief Run MAC verify tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_mac_verify_test(psa_key_type_t key_type,
                              size_t key_bits,
                              const psa_algorithm_t key_alg,
                              psa_key_location_t key_location,
                              size_t key_id,
                              const uint8_t *blob,
                              size_t blob_size,
                              struct test_result_t *result);

/**
 * \brief Run Sign & Verify Message tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_sigvermsg_test(psa_key_type_t key_type,
                             size_t key_bits,
                             const psa_algorithm_t key_alg,
                             psa_key_location_t key_location,
                             size_t key_id,
                             const uint8_t *blob,
                             size_t blob_size,
                             struct test_result_t *result);

/**
 * \brief Run Sign Message tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_sigmsg_test(psa_key_type_t key_type,
                          size_t key_bits,
                          const psa_algorithm_t key_alg,
                          psa_key_location_t key_location,
                          size_t key_id,
                          const uint8_t *blob,
                          size_t blob_size,
                          struct test_result_t *result);

/**
 * \brief Run Verify Message tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_vermsg_test(psa_key_type_t key_type,
                          size_t key_bits,
                          const psa_algorithm_t key_alg,
                          psa_key_location_t key_location,
                          size_t key_id,
                          const uint8_t *blob,
                          size_t blob_size,
                          struct test_result_t *result);

/**
 * \brief Run Sign & Verify Hash tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_sigverhash_test(psa_key_type_t key_type,
                              size_t key_bits,
                              const psa_algorithm_t key_alg,
                              psa_key_location_t key_location,
                              size_t key_id,
                              const uint8_t *blob,
                              size_t blob_size,
                              struct test_result_t *result);

/**
 * \brief Run Sign Hash tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_sighash_test(psa_key_type_t key_type,
                           size_t key_bits,
                           const psa_algorithm_t key_alg,
                           psa_key_location_t key_location,
                           size_t key_id,
                           const uint8_t *blob,
                           size_t blob_size,
                           struct test_result_t *result);

/**
 * \brief Run Verify Hash tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_verhash_test(psa_key_type_t key_type,
                           size_t key_bits,
                           const psa_algorithm_t key_alg,
                           psa_key_location_t key_location,
                           size_t key_id,
                           const uint8_t *blob,
                           size_t blob_size,
                           struct test_result_t *result);

/**
 * \brief Run key export tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_export_test(psa_key_type_t key_type,
                          size_t key_bits,
                          const psa_algorithm_t key_alg,
                          psa_key_location_t key_location,
                          size_t key_id,
                          const uint8_t *blob,
                          size_t blob_size,
                          struct test_result_t *result);

/**
 * \brief Run KDF tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_kdf_test(psa_key_type_t key_type,
                       size_t key_bits,
                       const psa_algorithm_t key_alg,
                       psa_key_location_t key_location,
                       size_t key_id,
                       const uint8_t *blob,
                       size_t blob_size,
                       struct test_result_t *result);

/**
 * \brief Run key exchange tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_keyexch_test(psa_key_type_t key_type,
                           size_t key_bits,
                           const psa_algorithm_t key_alg,
                           psa_key_location_t key_location,
                           size_t key_id,
                           const uint8_t *blob,
                           size_t blob_size,
                           struct test_result_t *result);

/**
 * \brief Run asymetric encryption/decryption tests with different blobs
 *
 * \param[in]  key_type     PSA key type
 * \param[in]  key_bits     Key size in bits
 * \param[in]  key_alg      PSA algorithm
 * \param[in]  key_location PSA location for the blob
 * \param[in]  key_id       Key ID
 * \param[in]  blob         Key blob from EL2GO
 * \param[in]  blob_size    Size of key blob from EL2GO
 * \param[out] result       Test result
 *
 */
void psa_blob_crypt_test(psa_key_type_t key_type,
                         size_t key_bits,
                         const psa_algorithm_t key_alg,
                         psa_key_location_t key_location,
                         size_t key_id,
                         const uint8_t *blob,
                         size_t blob_size,
                         struct test_result_t *result);

#ifdef __cplusplus
}
#endif

#endif /* __EL2GO_BLOB_TEST_EXECUTOR_PSA_H__ */
