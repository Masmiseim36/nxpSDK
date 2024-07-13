/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "el2go_blob_test.h"
#include "el2go_blob_test_executor_psa.h"
#include "el2go_blob_test_suite_internal.h"

/* AES */

static void el2go_blob_test_internal_1000(struct test_result_t *ret)
{
    psa_blob_cipher_test(PSA_KEY_TYPE_AES, 128, PSA_ALG_CTR, PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000000,
                         INTERNAL_AES128_CIPHER_CTR, sizeof(INTERNAL_AES128_CIPHER_CTR), ret);
}

static void el2go_blob_test_internal_1001(struct test_result_t *ret)
{
    psa_blob_cipher_test(PSA_KEY_TYPE_AES, 128, PSA_ALG_CBC_NO_PADDING, PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000001,
                         INTERNAL_AES128_CIPHER_CBC, sizeof(INTERNAL_AES128_CIPHER_CBC), ret);
}

static void el2go_blob_test_internal_1002(struct test_result_t *ret)
{
    psa_blob_cipher_test(PSA_KEY_TYPE_AES, 128, PSA_ALG_ECB_NO_PADDING, PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000002,
                         INTERNAL_AES128_CIPHER_ECB, sizeof(INTERNAL_AES128_CIPHER_ECB), ret);
}

static void el2go_blob_test_internal_1003(struct test_result_t *ret)
{
    psa_blob_aead_test(PSA_KEY_TYPE_AES, 128, PSA_ALG_GCM, PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000003,
                       INTERNAL_AES128_AEAD_GCM, sizeof(INTERNAL_AES128_AEAD_GCM), ret);
}

static void el2go_blob_test_internal_1004(struct test_result_t *ret)
{
    psa_blob_mac_test(PSA_KEY_TYPE_AES, 128, PSA_ALG_CMAC, PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000004,
                      INTERNAL_AES128_MAC_CMAC, sizeof(INTERNAL_AES128_MAC_CMAC), ret);
}

static void el2go_blob_test_internal_1010(struct test_result_t *ret)
{
    psa_blob_cipher_test(PSA_KEY_TYPE_AES, 256, PSA_ALG_CTR, PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000010,
                         INTERNAL_AES256_CIPHER_CTR, sizeof(INTERNAL_AES256_CIPHER_CTR), ret);
}

static void el2go_blob_test_internal_1011(struct test_result_t *ret)
{
    psa_blob_cipher_test(PSA_KEY_TYPE_AES, 256, PSA_ALG_CBC_NO_PADDING, PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000011,
                         INTERNAL_AES256_CIPHER_CBC, sizeof(INTERNAL_AES256_CIPHER_CBC), ret);
}

static void el2go_blob_test_internal_1012(struct test_result_t *ret)
{
    psa_blob_cipher_test(PSA_KEY_TYPE_AES, 256, PSA_ALG_ECB_NO_PADDING, PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000012,
                         INTERNAL_AES256_CIPHER_ECB, sizeof(INTERNAL_AES256_CIPHER_ECB), ret);
}

static void el2go_blob_test_internal_1013(struct test_result_t *ret)
{
    psa_blob_aead_test(PSA_KEY_TYPE_AES, 256, PSA_ALG_GCM, PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000013,
                       INTERNAL_AES256_AEAD_GCM, sizeof(INTERNAL_AES256_AEAD_GCM), ret);
}

static void el2go_blob_test_internal_1014(struct test_result_t *ret)
{
    psa_blob_mac_test(PSA_KEY_TYPE_AES, 256, PSA_ALG_CMAC, PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000014,
                      INTERNAL_AES256_MAC_CMAC, sizeof(INTERNAL_AES256_MAC_CMAC), ret);
}

/* NISTP */

static void el2go_blob_test_internal_1020(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 256, PSA_ALG_ECDSA(PSA_ALG_SHA_256),
                            PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000020, INTERNAL_NISTP256_SIGVERMSG_ECDSASHA256,
                            sizeof(INTERNAL_NISTP256_SIGVERMSG_ECDSASHA256), ret);
}

static void el2go_blob_test_internal_1021(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 256, PSA_ALG_ECDSA(PSA_ALG_SHA_256),
                             PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000021, INTERNAL_NISTP256_SIGVERHASH_ECDSASHA256,
                             sizeof(INTERNAL_NISTP256_SIGVERHASH_ECDSASHA256), ret);
}

static void el2go_blob_test_internal_1022(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 256, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_BLOB_STORAGE, 0x3E000022, INTERNAL_NISTP256_SIGVERHASH_ECDSAANY,
                             sizeof(INTERNAL_NISTP256_SIGVERHASH_ECDSAANY), ret);
}

/* HMAC */

static void el2go_blob_test_internal_1030(struct test_result_t *ret)
{
    psa_blob_mac_test(PSA_KEY_TYPE_HMAC, 256, PSA_ALG_HMAC(PSA_ALG_SHA_256), PSA_KEY_LOCATION_S50_BLOB_STORAGE,
                      0x3E000030, INTERNAL_HMAC256_MAC_HMACSHA256, sizeof(INTERNAL_HMAC256_MAC_HMACSHA256), ret);
}

#ifndef CONFIG_RUN_VERIFIED_ONLY
static void el2go_blob_test_internal_1031(struct test_result_t *ret)
{
    psa_blob_kdf_test(PSA_KEY_TYPE_DERIVE, 256, PSA_ALG_HKDF(PSA_ALG_SHA_256), PSA_KEY_LOCATION_S50_BLOB_STORAGE,
                      0x3E000031, INTERNAL_HMAC256_KDF_HKDFSHA256, sizeof(INTERNAL_HMAC256_KDF_HKDFSHA256), ret);
}
#endif

/* TEST SUITE */

static struct test_t blob_internal_tests[] = {
    {&el2go_blob_test_internal_1000, "EL2GO_BLOB_TEST_INTERNAL_1000", "Internal AES128 CIPHER CTR"},
    {&el2go_blob_test_internal_1001, "EL2GO_BLOB_TEST_INTERNAL_1001", "Internal AES128 CIPHER CBC"},
    {&el2go_blob_test_internal_1002, "EL2GO_BLOB_TEST_INTERNAL_1002", "Internal AES128 CIPHER ECB"},
    {&el2go_blob_test_internal_1003, "EL2GO_BLOB_TEST_INTERNAL_1003", "Internal AES128 AEAD GCM"},
    {&el2go_blob_test_internal_1004, "EL2GO_BLOB_TEST_INTERNAL_1004", "Internal AES128 MAC CMAC"},

    {&el2go_blob_test_internal_1010, "EL2GO_BLOB_TEST_INTERNAL_1010", "Internal AES256 CIPHER CTR"},
    {&el2go_blob_test_internal_1011, "EL2GO_BLOB_TEST_INTERNAL_1011", "Internal AES256 CIPHER CBC"},
    {&el2go_blob_test_internal_1012, "EL2GO_BLOB_TEST_INTERNAL_1012", "Internal AES256 CIPHER ECB"},
    {&el2go_blob_test_internal_1013, "EL2GO_BLOB_TEST_INTERNAL_1013", "Internal AES256 AEAD GCM"},
    {&el2go_blob_test_internal_1014, "EL2GO_BLOB_TEST_INTERNAL_1014", "Internal AES256 MAC CMAC"},

    {&el2go_blob_test_internal_1020, "EL2GO_BLOB_TEST_INTERNAL_1020", "Internal NISTP256 SIGVERMSG ECDSASHA256"},
    {&el2go_blob_test_internal_1021, "EL2GO_BLOB_TEST_INTERNAL_1021", "Internal NISTP256 SIGVERHASH ECDSASHA256"},
    {&el2go_blob_test_internal_1022, "EL2GO_BLOB_TEST_INTERNAL_1022", "Internal NISTP256 SIGVERHASH ECDSAANY"},

    {&el2go_blob_test_internal_1030, "EL2GO_BLOB_TEST_INTERNAL_1030", "Internal HMAC256 MAC HMACSHA256"},
#ifndef CONFIG_RUN_VERIFIED_ONLY
    {&el2go_blob_test_internal_1031, "EL2GO_BLOB_TEST_INTERNAL_1031", "Internal HMAC256 KDF HKDFSHA256"},
#endif
};

void testsuite_blob_internal(struct test_suite_t *test_suite)
{
    test_suite->name           = "INTERNAL (EL2GO_BLOB_TEST_INTERNAL_10XX)";
    test_suite->test_list      = blob_internal_tests;
    test_suite->test_list_size = (sizeof(blob_internal_tests) / sizeof(blob_internal_tests[0]));
}
