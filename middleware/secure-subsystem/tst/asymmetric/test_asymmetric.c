/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <sss_tst.h>

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */
#define HKDF_MAX_SALT 32
#define SYM_KEY_MAX 16
#define DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_INFO 192

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

/* clang-format off */
const uint8_t otherPubKey[] = {
    0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2A, 0x86,
    0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A,
    0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07, 0x03,
    0x42, 0x00, 0x04, 0xED, 0xA7, 0xE9, 0x0B, 0xF9,
    0x20, 0xCF, 0xFB, 0x9D, 0xF6, 0xDB, 0xCE, 0xF7,
    0x20, 0xE1, 0x23, 0x8B, 0x3C, 0xEE, 0x84, 0x86,
    0xD2, 0x50, 0xE4, 0xDF, 0x30, 0x11, 0x50, 0x1A,
    0x15, 0x08, 0xA6, 0x2E, 0xD7, 0x49, 0x52, 0x78,
    0x63, 0x6E, 0x61, 0xE8, 0x5F, 0xED, 0xB0, 0x6D,
    0x87, 0x92, 0x0A, 0x04, 0x19, 0x14, 0xFE, 0x76,
    0x63, 0x55, 0xDF, 0xBD, 0x68, 0x61, 0x59, 0x31,
    0x8E, 0x68, 0x7C
};

const uint8_t keyPairData[] = { 0x30, 0x81, 0x87, 0x02, 0x01, 0x00, 0x30, 0x13,
                                0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02,
                                0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D,
                                0x03, 0x01, 0x07, 0x04, 0x6D, 0x30, 0x6B, 0x02,
                                0x01, 0x01, 0x04, 0x20, 0x78, 0xE5, 0x20, 0x6A,
                                0x08, 0xED, 0xD2, 0x52, 0x36, 0x33, 0x8A, 0x24,
                                0x84, 0xE4, 0x2F, 0x1F, 0x7D, 0x1F, 0x6D, 0x94,
                                0x37, 0xA9, 0x95, 0x86, 0xDA, 0xFC, 0xD2, 0x23,
                                0x6F, 0xA2, 0x87, 0x35, 0xA1, 0x44, 0x03, 0x42,
                                0x00, 0x04, 0xED, 0xA7, 0xE9, 0x0B, 0xF9, 0x20,
                                0xCF, 0xFB, 0x9D, 0xF6, 0xDB, 0xCE, 0xF7, 0x20,
                                0xE1, 0x23, 0x8B, 0x3C, 0xEE, 0x84, 0x86, 0xD2,
                                0x50, 0xE4, 0xDF, 0x30, 0x11, 0x50, 0x1A, 0x15,
                                0x08, 0xA6, 0x2E, 0xD7, 0x49, 0x52, 0x78, 0x63,
                                0x6E, 0x61, 0xE8, 0x5F, 0xED, 0xB0, 0x6D, 0x87,
                                0x92, 0x0A, 0x04, 0x19, 0x14, 0xFE, 0x76, 0x63,
                                0x55, 0xDF, 0xBD, 0x68, 0x61, 0x59, 0x31, 0x8E,
                                0x68, 0x7C };

const uint8_t extPubKeyData[] = {
                                0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2A, 0x86,
                                0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A,
                                0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07, 0x03,
                                0x42, 0x00, 0x04, 0xED, 0xA7, 0xE9, 0x0B, 0xF9,
                                0x20, 0xCF, 0xFB, 0x9D, 0xF6, 0xDB, 0xCE, 0xF7,
                                0x20, 0xE1, 0x23, 0x8B, 0x3C, 0xEE, 0x84, 0x86,
                                0xD2, 0x50, 0xE4, 0xDF, 0x30, 0x11, 0x50, 0x1A,
                                0x15, 0x08, 0xA6, 0x2E, 0xD7, 0x49, 0x52, 0x78,
                                0x63, 0x6E, 0x61, 0xE8, 0x5F, 0xED, 0xB0, 0x6D,
                                0x87, 0x92, 0x0A, 0x04, 0x19, 0x14, 0xFE, 0x76,
                                0x63, 0x55, 0xDF, 0xBD, 0x68, 0x61, 0x59, 0x31,
                                0x8E, 0x68, 0x7C
};
/* clang-format on */
sss_object_t extPubkey;
sss_object_t derivedKey;
sss_object_t keyPair;
/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

static void runAllTests(void);
static void initialise_allocate_key_object(
    sss_object_t *derivedKey, uint32_t keyId, sss_key_type_t keyType, size_t keyByteLenMax, uint32_t options);

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

TEST_GROUP(asymmetric);

TEST_SETUP(asymmetric)
{
    DoFixtureSetupIfNeeded(false);
    DoFactoryReset();
    DoCommonSetUp();
}

TEST_TEAR_DOWN(asymmetric)
{
    DoCommonTearDown();
    if (extPubkey.keyStore != NULL)
        sss_key_object_free(&extPubkey);
    if (derivedKey.keyStore != NULL)
        sss_key_object_free(&derivedKey);
    if (keyPair.keyStore != NULL)
        sss_key_object_free(&keyPair);
}

TEST(asymmetric, Encrypt)
{
    sss_status_t status;
    const uint8_t srcData[128] = {0};
    size_t srcLen;
    uint8_t destData[128] = {0};
    size_t destLen;
    srcLen = sizeof(srcData);
    destLen = sizeof(destData);
    /* asymmetric Encrypt */

    RUN_TEST_SCENARIO(asymmetric, _generate_rsa);
    status = sss_asymmetric_encrypt(&gtCtx.asymm, srcData, srcLen, destData, &destLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Encryption Failed!!!");
}

TEST(asymmetric, Decrypt)
{
    sss_status_t status;
    const uint8_t srcData[128] = {0};
    size_t srcLen;
    uint8_t destData[128] = {0};
    size_t destLen;
    srcLen = sizeof(srcData);
    destLen = sizeof(destData);
    /* asymmetric Decrypt */

    RUN_TEST_SCENARIO(asymmetric, _generate_rsa);
    status = sss_asymmetric_encrypt(&gtCtx.asymm, srcData, srcLen, destData, &destLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Encryption Failed!!!");
    status = sss_asymmetric_decrypt(&gtCtx.asymm, srcData, srcLen, destData, &destLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Decryption Failed!!!");
}

TEST(asymmetric, _generate_ecc)
{
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    uint32_t keyId = __LINE__;
    size_t keylen = 256 / 8;
    /* asymmetric Sign */
    algorithm = kAlgorithm_SSS_SHA256;
    mode = kMode_SSS_Sign;
    status =
        sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_ECC_Pair, keylen, kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "key_object_allocate_handle Failed!!!");

    status = sss_asymmetric_context_init(&gtCtx.asymm, &gtCtx.session, &gtCtx.key, algorithm, mode);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Asymmetric Init Context Failed!!!");

    status = sss_key_store_generate_key(&gtCtx.ks, &gtCtx.key, keylen * 8, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Key Store Generate Key Failed!!!");
}

TEST(asymmetric, _generate_ecc1)
{
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    uint32_t keyId = __LINE__;
    size_t keylen = 256 / 8;
    /* asymmetric Sign */
    algorithm = kAlgorithm_SSS_SHA256;
    mode = kMode_SSS_Sign;
    status =
        sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_ECC_Pair, keylen, kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "key_object_allocate_handle Failed!!!");

    status = sss_asymmetric_context_init(&gtCtx.asymm, &gtCtx.session, &gtCtx.key, algorithm, mode);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Asymmetric Init Context Failed!!!");

    status = sss_key_store_generate_key(&gtCtx.ks, &gtCtx.key, keylen * 8, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Key Store Generate Key Failed!!!");
}

TEST(asymmetric, _generate_ecc_verify)
{
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    uint32_t keyId = __LINE__;
    size_t keylen = 256 / 8;
    /* asymmetric Sign */
    algorithm = kAlgorithm_SSS_SHA256;
    mode = kMode_SSS_Sign;
    status =
        sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_ECC_Pair, keylen, kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "key_object_allocate_handle Failed!!!");

    status = sss_asymmetric_context_init(&gtCtx.asymm, &gtCtx.session, &gtCtx.key, algorithm, mode);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Asymmetric Init Context Failed!!!");

    status = sss_key_store_generate_key(&gtCtx.ks, &gtCtx.key, keylen * 8, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Key Store Generate Key Failed!!!");
}

TEST(asymmetric, _generate_rsa)
{
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    uint32_t keyId = __LINE__;
    size_t keylen = 1024 / 8;
    /* asymmetric Sign */
    algorithm = kAlgorithm_SSS_SHA256;
    mode = kMode_SSS_Sign;
    status =
        sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_RSA_Pair, keylen, kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "key_object_allocate_handle Failed!!!");

    status = sss_asymmetric_context_init(&gtCtx.asymm, &gtCtx.session, &gtCtx.key, algorithm, mode);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Asymmetric Init Context Failed!!!");

    status = sss_key_store_generate_key(&gtCtx.ks, &gtCtx.key, keylen * 8, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Key Store Generate Key Failed!!!");
}

TEST(asymmetric, Sign_ecc)
{
    sss_status_t status;
    uint8_t digest[32] = {0};
    size_t digestLen;
    uint8_t signature[256] = {0};
    size_t signatureLen;
    digestLen = sizeof(digest);
    signatureLen = sizeof(signature);
    RUN_TEST_SCENARIO(asymmetric, _generate_ecc1);

    status = sss_asymmetric_sign_digest(&gtCtx.asymm, digest, digestLen, signature, &signatureLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Signing Failed!!!");
}

TEST(asymmetric, Sign_rsa)
{
    sss_status_t status;
    uint8_t digest[32] = {0};
    size_t digestLen;
    uint8_t signature[256] = {0};
    size_t signatureLen;
    digestLen = sizeof(digest);
    signatureLen = sizeof(signature);
    RUN_TEST_SCENARIO(asymmetric, _generate_rsa);

    status = sss_asymmetric_sign_digest(&gtCtx.asymm, digest, digestLen, signature, &signatureLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Signing Failed!!!");
}

TEST(asymmetric, Verify_ecc)
{
    sss_status_t status;
    uint8_t digest[32] = {0};
    size_t digestLen;
    uint8_t signature[256] = {0};
    size_t signatureLen;
    digestLen = sizeof(digest);
    signatureLen = sizeof(signature);
    uint8_t pbKey[112];
    size_t pbKeyBitLen = sizeof(pbKey) * 8;
    sss_asymmetric_t asymVerifyCtx;

    RUN_TEST_SCENARIO(asymmetric, _generate_ecc);

    status = sss_asymmetric_sign_digest(&gtCtx.asymm, digest, digestLen, signature, &signatureLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Signing Failed!!!");

    status = sss_key_store_get_key(&gtCtx.ks, &gtCtx.key, pbKey, &pbKeyBitLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Get Public Key Failed!!!");

    status = sss_key_object_init(&extPubkey, &gtCtx.ks);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set Public Key Failed!!!");

    status = sss_key_object_allocate_handle(&extPubkey, __LINE__, kSSS_KeyType_ECC_Public, 256 / 8,
                                            kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set Public Key Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &extPubkey, pbKey, pbKeyBitLen, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set Public Key Failed!!!");

    sss_asymmetric_context_init(&asymVerifyCtx, &gtCtx.session, &extPubkey, kAlgorithm_SSS_SHA256, kMode_SSS_Verify);

    status = sss_asymmetric_verify_digest(&asymVerifyCtx, digest, digestLen, signature, signatureLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Verification Failed!!!");
    sss_asymmetric_context_free(&asymVerifyCtx);
}

TEST(asymmetric, Sign_Verify_Ecc)
{
    sss_status_t status;
    uint8_t digest[32] = "Hello World";
    size_t digestLen;
    uint8_t signature[256] = {0};
    size_t signatureLen;
    digestLen = sizeof(digest);
    signatureLen = sizeof(signature);
    sss_asymmetric_t asymVerifyCtx;

    /*Signing Part*/
    status = sss_key_object_init(&keyPair, &gtCtx.ks);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set Public Key Failed!!!");

    status = sss_key_object_allocate_handle(&keyPair, __LINE__, kSSS_KeyType_ECC_Pair, sizeof(keyPairData),
                                            kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set Public Key Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &keyPair, keyPairData, sizeof(keyPairData) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set Public Key Failed!!!");

    sss_asymmetric_context_init(&gtCtx.asymm, &gtCtx.session, &keyPair, kAlgorithm_SSS_SHA256, kMode_SSS_Sign);

    status = sss_asymmetric_sign_digest(&gtCtx.asymm, digest, digestLen, signature, &signatureLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Signing Failed!!!");

    /*Verification Part*/
    status = sss_key_object_init(&extPubkey, &gtCtx.ks);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set Public Key Failed!!!");

    status = sss_key_object_allocate_handle(&extPubkey, __LINE__, kSSS_KeyType_ECC_Public, sizeof(extPubKeyData),
                                            kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set Public Key Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &extPubkey, extPubKeyData, sizeof(extPubKeyData) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set Public Key Failed!!!");

    sss_asymmetric_context_init(&asymVerifyCtx, &gtCtx.session, &extPubkey, kAlgorithm_SSS_SHA256, kMode_SSS_Verify);

    status = sss_asymmetric_verify_digest(&asymVerifyCtx, digest, digestLen, signature, signatureLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Verification Failed!!!");
    sss_asymmetric_context_free(&asymVerifyCtx);
}

TEST(asymmetric, Verify_rsa)
{
    sss_status_t status;
    uint8_t digest[32] = {0};
    size_t digestLen;
    uint8_t signature[256] = {0};
    size_t signatureLen;
    digestLen = sizeof(digest);
    signatureLen = sizeof(signature);

    RUN_TEST_SCENARIO(asymmetric, _generate_rsa);

    status = sss_asymmetric_sign_digest(&gtCtx.asymm, digest, digestLen, signature, &signatureLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_asymmetric_sign_digest");

    status = sss_asymmetric_verify_digest(&gtCtx.asymm, digest, digestLen, signature, signatureLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Verification Failed!!!");
}

TEST(asymmetric, Derive_ECDH_PreShrdKey)
{
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    /* asymmetric Compute Shared Secret */
    algorithm = kAlgorithm_SSS_ECDH;
    mode = kMode_SSS_ComputeSharedSecret;
    size_t keyByteLen = 32;

    RUN_TEST_SCENARIO(asymmetric, _generate_ecc);
    status = sss_derive_key_context_init(&gtCtx.derv, &gtCtx.session, &gtCtx.key, algorithm, mode);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "derive key context init Failed!!!");

    initialise_allocate_key_object(&extPubkey, __LINE__, kSSS_KeyType_ECC_Public, keyByteLen,
                                   kKeyObject_Mode_Persistent);
    initialise_allocate_key_object(&derivedKey, __LINE__, kSSS_KeyType_AES, keyByteLen, kKeyObject_Mode_Transient);

    status = sss_key_store_set_key(&gtCtx.ks, &extPubkey, otherPubKey, sizeof(otherPubKey) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_store_set_key Failed");

    status = sss_asymmetric_dh_derive_key(&gtCtx.derv, &extPubkey, &derivedKey);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_asymmetric_dh_derive_key Failed!!!");
}

TEST(asymmetric, HKDF_Secret_Key)
{
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    /* HKDF Extract and Expand*/
    algorithm = kAlgorithm_SSS_HMAC_SHA256;
    mode = kMode_SSS_ComputeSharedSecret;
    uint32_t keyId = __LINE__;

    /* clang-format off */
    const uint8_t aesRef[SYM_KEY_MAX] = {0xDB, 0xFE, 0xE9, 0xE3, 0xB2, 0x76, 0x15, 0x4D, 0x67, 0xF9, 0xD8, 0x4C, 0xB9, 0x35, 0x54, 0x56};

    static uint8_t salt[HKDF_MAX_SALT] = {0xAA, 0x1A, 0x2A, 0xE3, 0xB2, 0x76, 0x15, 0x4D, 0x67, 0xF9, 0xD8, 0x4C, 0xB9, 0x35, 0x54, 0x56, 0xBB, 0x1B, 0x2B, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    /* clang-format on */

    uint8_t info[DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_INFO];
    uint8_t infoLen = sizeof(info);
    for (int i = 0; i < DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_INFO; i++)
    {
        info[i] = (uint8_t)i;
    }

    initialise_allocate_key_object(&gtCtx.key, keyId, kSSS_KeyType_AES, SYM_KEY_MAX, kKeyObject_Mode_Persistent);

    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, aesRef, SYM_KEY_MAX * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_store_set_key Failed");

    initialise_allocate_key_object(&derivedKey, __LINE__, kSSS_KeyType_AES, 255, kKeyObject_Mode_Transient);

    status = sss_derive_key_context_init(&gtCtx.derv, &gtCtx.session, &gtCtx.key, algorithm, mode);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "derive key context init Failed!!!");

    status = sss_derive_key(&gtCtx.derv, salt, HKDF_MAX_SALT, info, infoLen, &derivedKey);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "HKDF Failed!!!");
}

static void initialise_allocate_key_object(
    sss_object_t *derivedKey, uint32_t keyId, sss_key_type_t keyType, size_t keyByteLenMax, uint32_t options)
{
    sss_status_t status;
    status = sss_key_object_init(derivedKey, &gtCtx.ks);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_init for derived key Failed");

    status = sss_key_object_allocate_handle(derivedKey, keyId, keyType, keyByteLenMax, options);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_allocate_handle Failed");
}

TEST_GROUP_RUNNER(asymmetric)
{
    //    RUN_TEST_CASE(asymmetric, Encrypt);
    //    RUN_TEST_CASE(asymmetric, Decrypt);
    RUN_TEST_CASE(asymmetric, Sign_ecc);
    RUN_TEST_CASE(asymmetric, Verify_ecc);
    RUN_TEST_CASE(asymmetric, Sign_Verify_Ecc);
#if (!SSS_HAVE_A71CH)
    RUN_TEST_CASE(asymmetric, Sign_rsa);
    RUN_TEST_CASE(asymmetric, Verify_rsa);
#endif
    RUN_TEST_CASE(asymmetric, Derive_ECDH_PreShrdKey);
    RUN_TEST_CASE(asymmetric, HKDF_Secret_Key);
}

int main(int argc, const char *argv[])
{
    int ret = UnityMain(argc, argv, runAllTests);
    DoFixtureTearDownIfNeeded();
    return ret;
}

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */

static void runAllTests(void)
{
    RUN_TEST_GROUP(asymmetric);
}
