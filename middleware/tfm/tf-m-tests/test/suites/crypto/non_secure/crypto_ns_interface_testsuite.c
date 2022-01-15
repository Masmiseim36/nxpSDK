/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_framework_helpers.h"
#include "tfm_api.h"
#include "../crypto_tests_common.h"

/* List of tests */
static void tfm_crypto_test_1001(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_ALG_CBC
static void tfm_crypto_test_1002(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CBC */
#ifdef TFM_CRYPTO_TEST_ALG_CFB
static void tfm_crypto_test_1003(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CFB */
#ifdef TFM_CRYPTO_TEST_ALG_CTR
static void tfm_crypto_test_1005(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CTR */
static void tfm_crypto_test_1007(struct test_result_t *ret);
static void tfm_crypto_test_1008(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_ALG_CFB
static void tfm_crypto_test_1009(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CFB */
static void tfm_crypto_test_1010(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_ALG_SHA_224 //NXP
static void tfm_crypto_test_1011(struct test_result_t *ret);
#endif
static void tfm_crypto_test_1012(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_ALG_SHA_512
static void tfm_crypto_test_1013(struct test_result_t *ret);
static void tfm_crypto_test_1014(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_SHA_512 */
static void tfm_crypto_test_1019(struct test_result_t *ret);
static void tfm_crypto_test_1020(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_ALG_SHA_512
static void tfm_crypto_test_1021(struct test_result_t *ret);
static void tfm_crypto_test_1022(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_SHA_512 */
#ifdef TFM_CRYPTO_TEST_ALG_SHA_224 //NXP
static void tfm_crypto_test_1024(struct test_result_t *ret);
#endif
#ifdef TFM_CRYPTO_TEST_ALG_CCM
static void tfm_crypto_test_1030(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CCM */
#ifdef TFM_CRYPTO_TEST_ALG_GCM
static void tfm_crypto_test_1031(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_GCM */
static void tfm_crypto_test_1032(struct test_result_t *ret);
static void tfm_crypto_test_1033(struct test_result_t *ret);
static void tfm_crypto_test_1034(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_ALG_CCM
static void tfm_crypto_test_1035(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ALG_CCM */
static void tfm_crypto_test_1036(struct test_result_t *ret);
static void tfm_crypto_test_1037(struct test_result_t *ret);
#ifdef TFM_CRYPTO_TEST_HKDF
static void tfm_crypto_test_1038(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_HKDF */
#ifdef TFM_CRYPTO_TEST_ECDH
static void tfm_crypto_test_1039(struct test_result_t *ret);
#endif /* TFM_CRYPTO_TEST_ECDH */

static struct test_t crypto_tests[] = {
    {&tfm_crypto_test_1001, "TFM_NS_CRYPTO_TEST_1001",
     "Non Secure Key management interface", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_ALG_CBC
    {&tfm_crypto_test_1002, "TFM_NS_CRYPTO_TEST_1002",
     "Non Secure Symmetric encryption (AES-128-CBC) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_CBC */
#ifdef TFM_CRYPTO_TEST_ALG_CFB
    {&tfm_crypto_test_1003, "TFM_NS_CRYPTO_TEST_1003",
     "Non Secure Symmetric encryption (AES-128-CFB) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_CFB */
#ifdef TFM_CRYPTO_TEST_ALG_CTR
    {&tfm_crypto_test_1005, "TFM_NS_CRYPTO_TEST_1005",
     "Non Secure Symmetric encryption (AES-128-CTR) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_CTR */
    {&tfm_crypto_test_1007, "TFM_NS_CRYPTO_TEST_1007",
     "Non Secure Symmetric encryption invalid cipher", {TEST_PASSED} },
    {&tfm_crypto_test_1008, "TFM_NS_CRYPTO_TEST_1008",
     "Non Secure Symmetric encryption invalid cipher (AES-152)", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_ALG_CFB
    {&tfm_crypto_test_1009, "TFM_NS_CRYPTO_TEST_1009",
     "Non Secure Symmetric encryption invalid cipher (HMAC-128-CFB)", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_CFB */
    {&tfm_crypto_test_1010, "TFM_NS_CRYPTO_TEST_1010",
     "Non Secure Unsupported Hash (SHA-1) interface", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_ALG_SHA_224 //NXP
    {&tfm_crypto_test_1011, "TFM_NS_CRYPTO_TEST_1011",
     "Non Secure Hash (SHA-224) interface", {TEST_PASSED} },
#endif
    {&tfm_crypto_test_1012, "TFM_NS_CRYPTO_TEST_1012",
     "Non Secure Hash (SHA-256) interface", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_ALG_SHA_512
    {&tfm_crypto_test_1013, "TFM_NS_CRYPTO_TEST_1013",
     "Non Secure Hash (SHA-384) interface", {TEST_PASSED} },
    {&tfm_crypto_test_1014, "TFM_NS_CRYPTO_TEST_1014",
     "Non Secure Hash (SHA-512) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_SHA_512 */
    {&tfm_crypto_test_1019, "TFM_NS_CRYPTO_TEST_1019",
     "Non Secure Unsupported HMAC (SHA-1) interface", {TEST_PASSED} },
    {&tfm_crypto_test_1020, "TFM_NS_CRYPTO_TEST_1020",
     "Non Secure HMAC (SHA-256) interface", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_ALG_SHA_512
    {&tfm_crypto_test_1021, "TFM_NS_CRYPTO_TEST_1021",
     "Non Secure HMAC (SHA-384) interface", {TEST_PASSED} },
    {&tfm_crypto_test_1022, "TFM_NS_CRYPTO_TEST_1022",
     "Non Secure HMAC (SHA-512) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_SHA_512 */
#ifdef TFM_CRYPTO_TEST_ALG_SHA_224 //NXP
    {&tfm_crypto_test_1024, "TFM_NS_CRYPTO_TEST_1024",
     "Non Secure HMAC with long key (SHA-224) interface", {TEST_PASSED} },
#endif
#ifdef TFM_CRYPTO_TEST_ALG_CCM
    {&tfm_crypto_test_1030, "TFM_NS_CRYPTO_TEST_1030",
     "Non Secure AEAD (AES-128-CCM) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_CCM */
#ifdef TFM_CRYPTO_TEST_ALG_GCM
    {&tfm_crypto_test_1031, "TFM_NS_CRYPTO_TEST_1031",
     "Non Secure AEAD (AES-128-GCM) interface", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_GCM */
    {&tfm_crypto_test_1032, "TFM_NS_CRYPTO_TEST_1032",
     "Non Secure key policy interface", {TEST_PASSED} },
    {&tfm_crypto_test_1033, "TFM_NS_CRYPTO_TEST_1033",
     "Non Secure key policy check permissions", {TEST_PASSED} },
    {&tfm_crypto_test_1034, "TFM_NS_CRYPTO_TEST_1034",
     "Non Secure persistent key interface", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_ALG_CCM
    {&tfm_crypto_test_1035, "TFM_NS_CRYPTO_TEST_1035",
     "Non Secure AEAD interface with truncated auth tag (AES-128-CCM-8)",
     {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ALG_CCM */
    {&tfm_crypto_test_1036, "TFM_NS_CRYPTO_TEST_1036",
     "Non Secure TLS 1.2 PRF key derivation", {TEST_PASSED} },
    {&tfm_crypto_test_1037, "TFM_NS_CRYPTO_TEST_1037",
     "Non Secure TLS-1.2 PSK-to-MasterSecret key derivation", {TEST_PASSED} },
#ifdef TFM_CRYPTO_TEST_HKDF
    {&tfm_crypto_test_1038, "TFM_NS_CRYPTO_TEST_1038",
     "Non Secure HKDF key derivation", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_HKDF */
#ifdef TFM_CRYPTO_TEST_ECDH
    {&tfm_crypto_test_1039, "TFM_NS_CRYPTO_TEST_1039",
     "Non Secure ECDH key agreement", {TEST_PASSED} },
#endif /* TFM_CRYPTO_TEST_ECDH */
};

void register_testsuite_ns_crypto_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size = (sizeof(crypto_tests) / sizeof(crypto_tests[0]));

    set_testsuite("Crypto non-secure interface test (TFM_NS_CRYPTO_TEST_1XXX)",
                  crypto_tests, list_size, p_test_suite);
}

/**
 * \brief Non-Secure interface test for Crypto
 *
 * \details The scope of this set of tests is to functionally verify
 *          the interfaces specified by psa/crypto.h are working
 *          as expected. This is not meant to cover all possible
 *          scenarios and corner cases.
 *
 */
static void tfm_crypto_test_1001(struct test_result_t *ret)
{
    psa_key_interface_test(PSA_KEY_TYPE_AES, ret);
}

#ifdef TFM_CRYPTO_TEST_ALG_CBC
static void tfm_crypto_test_1002(struct test_result_t *ret)
{
    psa_cipher_test(PSA_KEY_TYPE_AES, PSA_ALG_CBC_NO_PADDING, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_CBC */

#ifdef TFM_CRYPTO_TEST_ALG_CFB
static void tfm_crypto_test_1003(struct test_result_t *ret)
{
    psa_cipher_test(PSA_KEY_TYPE_AES, PSA_ALG_CFB, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_CFB */

#ifdef TFM_CRYPTO_TEST_ALG_CTR
static void tfm_crypto_test_1005(struct test_result_t *ret)
{
    psa_cipher_test(PSA_KEY_TYPE_AES, PSA_ALG_CTR, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_CTR */

static void tfm_crypto_test_1007(struct test_result_t *ret)
{
    psa_invalid_cipher_test(PSA_KEY_TYPE_AES, PSA_ALG_HMAC(PSA_ALG_SHA_256),
                            16, ret);
}

static void tfm_crypto_test_1008(struct test_result_t *ret)
{
    psa_invalid_key_length_test(ret);
}

#ifdef TFM_CRYPTO_TEST_ALG_CFB
static void tfm_crypto_test_1009(struct test_result_t *ret)
{
    /* HMAC is not a block cipher */
    psa_invalid_cipher_test(PSA_KEY_TYPE_HMAC, PSA_ALG_CFB, 16, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_CFB */

static void tfm_crypto_test_1010(struct test_result_t *ret)
{
    psa_unsupported_hash_test(PSA_ALG_SHA_1, ret);
}

#ifdef TFM_CRYPTO_TEST_ALG_SHA_224 //NXP
static void tfm_crypto_test_1011(struct test_result_t *ret)
{
    psa_hash_test(PSA_ALG_SHA_224, ret);
}
#endif

static void tfm_crypto_test_1012(struct test_result_t *ret)
{
    psa_hash_test(PSA_ALG_SHA_256, ret);
}

#ifdef TFM_CRYPTO_TEST_ALG_SHA_512
static void tfm_crypto_test_1013(struct test_result_t *ret)
{
    psa_hash_test(PSA_ALG_SHA_384, ret);
}

static void tfm_crypto_test_1014(struct test_result_t *ret)
{
    psa_hash_test(PSA_ALG_SHA_512, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_SHA_512 */

static void tfm_crypto_test_1019(struct test_result_t *ret)
{
    psa_unsupported_mac_test(PSA_KEY_TYPE_HMAC, PSA_ALG_HMAC(PSA_ALG_SHA_1),
                             ret);
}

static void tfm_crypto_test_1020(struct test_result_t *ret)
{
    psa_mac_test(PSA_ALG_HMAC(PSA_ALG_SHA_256), 0, ret);
}

#ifdef TFM_CRYPTO_TEST_ALG_SHA_512
static void tfm_crypto_test_1021(struct test_result_t *ret)
{
    psa_mac_test(PSA_ALG_HMAC(PSA_ALG_SHA_384), 0, ret);
}

static void tfm_crypto_test_1022(struct test_result_t *ret)
{
    psa_mac_test(PSA_ALG_HMAC(PSA_ALG_SHA_512), 0, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_SHA_512 */

#ifdef TFM_CRYPTO_TEST_ALG_SHA_224 //NXP
static void tfm_crypto_test_1024(struct test_result_t *ret)
{
    psa_mac_test(PSA_ALG_HMAC(PSA_ALG_SHA_224), 1, ret);
}
#endif

#ifdef TFM_CRYPTO_TEST_ALG_CCM
static void tfm_crypto_test_1030(struct test_result_t *ret)
{
    psa_aead_test(PSA_KEY_TYPE_AES, PSA_ALG_CCM, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_CCM */

#ifdef TFM_CRYPTO_TEST_ALG_GCM
static void tfm_crypto_test_1031(struct test_result_t *ret)
{
    psa_aead_test(PSA_KEY_TYPE_AES, PSA_ALG_GCM, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_GCM */

static void tfm_crypto_test_1032(struct test_result_t *ret)
{
    psa_policy_key_interface_test(ret);
}

static void tfm_crypto_test_1033(struct test_result_t *ret)
{
    psa_policy_invalid_policy_usage_test(ret);
}

static void tfm_crypto_test_1034(struct test_result_t *ret)
{
    psa_persistent_key_test(1, ret);
}

#ifdef TFM_CRYPTO_TEST_ALG_CCM
static void tfm_crypto_test_1035(struct test_result_t *ret)
{
    psa_algorithm_t alg = PSA_ALG_AEAD_WITH_SHORTENED_TAG(PSA_ALG_CCM,
                                                       TRUNCATED_AUTH_TAG_LEN);

    psa_aead_test(PSA_KEY_TYPE_AES, alg, ret);
}
#endif /* TFM_CRYPTO_TEST_ALG_GCM */

static void tfm_crypto_test_1036(struct test_result_t *ret)
{
    psa_key_derivation_test(PSA_ALG_TLS12_PRF(PSA_ALG_SHA_256), ret);
}

static void tfm_crypto_test_1037(struct test_result_t *ret)
{
    psa_key_derivation_test(PSA_ALG_TLS12_PSK_TO_MS(PSA_ALG_SHA_256), ret);
}

#ifdef TFM_CRYPTO_TEST_HKDF
static void tfm_crypto_test_1038(struct test_result_t *ret)
{
    psa_key_derivation_test(PSA_ALG_HKDF(PSA_ALG_SHA_256), ret);
}
#endif /* TFM_CRYPTO_TEST_HKDF */

#ifdef TFM_CRYPTO_TEST_ECDH
static void tfm_crypto_test_1039(struct test_result_t *ret)
{
    psa_key_agreement_test(PSA_ALG_ECDH, ret);
}
#endif /* TFM_CRYPTO_TEST_ECDH */
