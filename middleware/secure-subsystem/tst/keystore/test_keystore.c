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

/* clang-format off */
#define ECC_PUB_KEY_PEM_MBEDTLS                                        \
    "-----BEGIN PUBLIC KEY-----"                                       \
    "\n"                                                               \
    "MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEMju1aHgcXrpZ8JKGBjIhX8rG7h7y" \
    "JulnoCHWJJmkG/XSDOgx1H/qvZfLMsH8SEqeJkmWpkp5bWrRsBGPwYkbgw=="     \
    "\n"                                                               \
    "-----END PUBLIC KEY-----"

#define ECC_PUB_KEY_PUB_PEM 0x04, 0x32, 0x3b, 0xb5, 0x68, 0x78, \
  0x1c, 0x5e, 0xba, 0x59, 0xf0, 0x92, 0x86, 0x06, 0x32, 0x21, 0x5f, 0xca, 0xc6, 0xee, 0x1e, 0xf2, \
  0x26, 0xe9, 0x67, 0xa0, 0x21, 0xd6, 0x24, 0x99, 0xa4, 0x1b, 0xf5, 0xd2, 0x0c, 0xe8, 0x31, 0xd4, \
  0x7f, 0xea, 0xbd, 0x97, 0xcb, 0x32, 0xc1, 0xfc, 0x48, 0x4a, 0x9e, 0x26, 0x49, 0x96, 0xa6, 0x4a, \
  0x79, 0x6d, 0x6a, 0xd1, 0xb0, 0x11, 0x8f, 0xc1, 0x89, 0x1b, 0x83

#define ECC_PUB_KEY_REF_DER_MBEDTLS { \
  0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a, \
  0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, ECC_PUB_KEY_PUB_PEM }

#if SSS_HAVE_SSS == 1 && SSS_HAVE_MBEDTLS == 1
#   define ECC_PUB_KEY ECC_PUB_KEY_PEM_MBEDTLS
#   define ECC_PUB_KEY_REF ECC_PUB_KEY_REF_DER_MBEDTLS
#elif SSS_HAVE_SSCP
#   define ECC_PUB_KEY         { ECC_PUB_KEY_PUB_PEM }
#   define ECC_PUB_KEY_REF     { ECC_PUB_KEY_PUB_PEM }
#endif


#define ECC_KEY_PAIR_REF { \
0x30, 0x81, 0x87, 0x02, 0x01, 0x00, 0x30, 0x13, \
0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, \
0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, \
0x03, 0x01, 0x07, 0x04, 0x6D, 0x30, 0x6B, 0x02, \
0x01, 0x01, 0x04, 0x20,    \
/*Private Key */ \
0x44, 0x2C, 0x92, 0x88, \
0xA4, 0xBA, 0x2A, 0x7C, 0xB6, 0x01, 0x4C, 0x36, \
0x4A, 0x35, 0xA7, 0x6B, 0xFF, 0x24, 0xC7, 0xE6, \
0x74, 0xAE, 0xDB, 0x56, 0xA9, 0x7B, 0x3D, 0x1B, \
0xDD, 0xD0, 0xD5, 0x94, \
/*Private Key End*/ \
0xA1, 0x44, 0x03,   \
0x42,   \
0x00,   \
/*Public Key */ \
0x04, 0x8D, 0xBE, 0x66, 0x75, 0xA7, 0xED,   \
0xAD, 0xDE, 0xAF, 0xDF, 0x1C, 0xC2, 0xBB, 0x7C, \
0xF5, 0x42, 0x57, 0x51, 0x6F, 0x34, 0x63, 0x8F, \
0x39, 0x25, 0x9D, 0x3A, 0x0E, 0x36, 0x3E, 0x6B, \
0xA6, 0x69, 0x83, 0x8E, 0x0D, 0xD0, 0xF0, 0x6B, \
0xBA, 0x43, 0x70, 0x73, 0x96, 0x06, 0x38, 0x7D, \
0xB7, 0xB5, 0xB5, 0x59, 0x63, 0xA2, 0x26, 0xE5, \
0x19, 0xC6, 0x35, 0x88, 0x3E, 0x29, 0x62, 0xF3, \
0x44, 0xA4  \
/*Public Key End*/  \
}

/* clang-format on */

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

static void runAllTests(void);

uint32_t change(uint32_t keyId);

uint32_t change(uint32_t keyId)
{
    static i = 0;
    i++;
    return (keyId + i);
}

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

/* Normal key store tests - do not need factory reset */
TEST_GROUP(KeyStore);

TEST_SETUP(KeyStore)
{
    DoFixtureSetupIfNeeded(true);
    DoCommonSetUp();
}

TEST_TEAR_DOWN(KeyStore)
{
    DoCommonTearDown();
}

/* Needs Factory Reset */
TEST_GROUP(KeyStoreFRst);

TEST_TEAR_DOWN(KeyStoreFRst)
{
    DoCommonTearDown();
}

TEST_SETUP(KeyStoreFRst)
{
    DoFixtureSetupIfNeeded(true);
    DoFactoryReset();
    DoCommonSetUp();
}

TEST(KeyStore, _deAllocateKeyStore)
{
    sss_key_store_context_free(&gtCtx.ks);
}

TEST(KeyStore, KS_AllocateKeyStore)
{
    sss_status_t status;
    /* Allocate key */
    uint32_t myKeyStoreId = __LINE__;
    RUN_TEST_SCENARIO(KeyStore, _deAllocateKeyStore);
    sss_key_store_context_init(&gtCtx.ks, &gtCtx.session);
    status = sss_key_store_allocate(&gtCtx.ks, myKeyStoreId);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_store_allocate Failed.");
}

TEST(KeyStore, KS_GenerateECC256Key)
{
    sss_status_t status;
    uint32_t keyId = __LINE__;
    sss_key_type_t keyType = kSSS_KeyType_ECC_Pair;
    size_t keylen = 256 / 8;
    /* Generate ECC keys */
    RUN_TEST_SCENARIO(KeyStore, KS_AllocateKeyStore);

    keyId = change(keyId);

    status = sss_key_object_allocate_handle(&gtCtx.key, keyId, keyType, 256 / 8, kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Allocate kSSS_KeyType_ECC_Pair Failed!!!");

    status = sss_key_store_generate_key(&gtCtx.ks, &gtCtx.key, keylen * 8, NULL);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "ECC Key Generation Failed!!!");
}

TEST(KeyStore, KS_StoreCertificate)
{
    sss_status_t status;
    uint8_t myCert[100] = "SOME RANDOM (INVALID The quick brown fox...) TEXT INSIDE CERTIFICATE";
    uint32_t keyId = __LINE__;
    keyId = change(keyId);
    /* Store Certificate */
    RUN_TEST_SCENARIO(KeyStore, KS_AllocateKeyStore);

    status = sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_Certificate, sizeof(myCert),
                                            kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_allocate_handle Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, myCert, sizeof(myCert) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Store Certificate Failed!!!");
}

TEST(KeyStore, KS_SetSymmKey)
{
    sss_status_t status;
    uint8_t myAes[16] = {0};
    uint32_t keyId = __LINE__;
    keyId = change(keyId);
    /* Store Certificate */
    RUN_TEST_SCENARIO(KeyStore, KS_AllocateKeyStore);

    status =
        sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_AES, sizeof(myAes), kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_allocate_handle Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, myAes, sizeof(myAes) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set AES Key Failed!!!");
}

TEST(KeyStore, KS_StoreCertificate_Get)
{
    sss_status_t status;
    uint8_t myCert[100] = "SOME RANDOM (INVALID The quick brown fox...) TEXT INSIDE CERTIFICATE";
    uint32_t keyId = __LINE__;
    keyId = change(keyId);
    /* Store Certificate */
    RUN_TEST_SCENARIO(KeyStore, KS_AllocateKeyStore);

    status = sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_Certificate, sizeof(myCert),
                                            kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_allocate_handle Failed!!!");
    // TODO change it to get
    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, myCert, sizeof(myCert) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Store Certificate Failed!!!");
}

TEST(KeyStore, KS_StoreCertificate_Save)
{
    sss_status_t status;
    uint8_t myCert[100] = "SOME RANDOM (INVALID The quick brown fox...) TEXT INSIDE CERTIFICATE";
    /* Store Certificate */
    RUN_TEST_SCENARIO(KeyStore, KS_AllocateKeyStore);
    uint32_t keyId = __LINE__;
    keyId = change(keyId);
    status = sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_Certificate, sizeof(myCert),
                                            kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_allocate_handle Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, myCert, sizeof(myCert) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Store Certificate Failed!!!");
}

TEST(KeyStore, KS_SaveKeyStore)
{
    sss_status_t status;
    RUN_TEST_SCENARIO(KeyStore, KS_StoreCertificate_Save);
    status = sss_key_store_save(&gtCtx.ks);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Save KeyStore Failed!!!");
}

TEST(KeyStore, KS_LoadKeyStore)
{
    sss_status_t status;
    /* First save key store */
    RUN_TEST_SCENARIO(KeyStore, KS_SaveKeyStore);
    /* Then load key store */
    status = sss_key_store_load(&gtCtx.ks);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Load KeyStore Failed!!!");
}

TEST(KeyStore, KS_SetECCPublicKey)
{
    sss_status_t status;
    uint32_t keyId = __LINE__;
    /* clang-format off */
    uint8_t setKey[] = ECC_PUB_KEY_REF_DER_MBEDTLS;
    /* clang-format on */

    keyId = change(keyId);

    RUN_TEST_SCENARIO(KeyStore, KS_AllocateKeyStore);

    status = sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_ECC_Public, sizeof(setKey),
                                            kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Allocate Key handle Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, setKey, sizeof(setKey) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set ECC PublicKey Failed!!!");
}

TEST(KeyStore, KS_SetECCKeyPair)
{
    sss_status_t status;
    uint32_t keyId = __LINE__;
    /* clang-format off */
    uint8_t setKey[] = ECC_KEY_PAIR_REF;
    /* clang-format on */

    RUN_TEST_SCENARIO(KeyStore, KS_AllocateKeyStore);

    keyId = change(keyId);
    status = sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_ECC_Pair, sizeof(setKey),
                                            kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Allocate Key Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, setKey, sizeof(setKey) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set ECC KeyPair Failed!!!");
}

TEST(KeyStore, KS_SetECCPublicKey_Set)
{
    sss_status_t status;
    uint32_t keyId = __LINE__;
    /* clang-format off */
    uint8_t setKey[] = ECC_PUB_KEY;
    /* clang-format on */

    RUN_TEST_SCENARIO(KeyStore, KS_AllocateKeyStore);

    keyId = change(keyId);
    status = sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_ECC_Public, sizeof(setKey),
                                            kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Allocate Key Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, setKey, sizeof(setKey) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set ECC PublicKey Failed!!!");
}

TEST(KeyStore, KS_StorePublicKey)
{
    sss_status_t status;
    uint8_t myKey[256] = "SOME INVALID (PUB) KEY";
    uint32_t keyId = __LINE__;
    keyId = change(keyId);
    /* Store Certificate */
    RUN_TEST_SCENARIO(KeyStore, KS_AllocateKeyStore);
    status = sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_ECC_Public, sizeof(myKey) * 8,
                                            kKeyObject_Mode_Persistent);
    TEST_ASSERT_T(status == kStatus_SSS_Success);
    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, myKey, sizeof(myKey) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Store Public Key Failed!!!");
}

TEST(KeyStore, KS_GetECCKeyPair)
{
    sss_status_t status;
    uint8_t myKey[256];
    size_t myKeyLen = sizeof(myKey);
    /* Get ECC Key Pair */
    RUN_TEST_SCENARIO(KeyStore, KS_GenerateECC256Key);
    status = sss_key_store_get_key(&gtCtx.ks, &gtCtx.key, myKey, &myKeyLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Get KeyPair Failed!!!");
}

TEST(KeyStore, KS_GetECCPublicKey)
{
    sss_status_t status;
    uint32_t keyId = __LINE__;
    /* clang-format off */
    uint8_t setKey[] = ECC_PUB_KEY_REF_DER_MBEDTLS;
    /* clang-format on */
    uint8_t getKey[200] = {0};
    size_t getKeyLen = sizeof(getKey) * 8;

    RUN_TEST_SCENARIO(KeyStore, KS_AllocateKeyStore);
    keyId = change(keyId);
    status = sss_key_object_allocate_handle(&gtCtx.key, keyId, kSSS_KeyType_ECC_Public, sizeof(setKey),
                                            kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Allocate Key Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, setKey, sizeof(setKey) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set ECC PublicKey Failed!!!");
    /* Get Public key ECC */
    status = sss_key_store_get_key(&gtCtx.ks, &gtCtx.key, getKey, &getKeyLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Get Public Key Failed!!!");
    TEST_ASSERT_EQUAL_HEX8_ARRAY(setKey, getKey, sizeof(setKey));
}

TEST(KeyStore, KS_GetCertificate)
{
    sss_status_t status;
    uint8_t getCert[100];
    size_t getCertLen = sizeof(getCert);
    /* Get Certificate */
    RUN_TEST_SCENARIO(KeyStore, KS_StoreCertificate_Get);
    status = sss_key_store_get_key(&gtCtx.ks, &gtCtx.key, getCert, &getCertLen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Get Certificate Failed!!!");
}
TEST(KeyStoreFRst, KS_FreezeKeyPair)
{
    sss_status_t status;
#if !SSS_HAVE_SSCP
    TEST_IGNORE_MESSAGE("Only for SSCP Based systems");
#endif
    RUN_TEST_SCENARIO(KeyStore, KS_GenerateECC256Key);
    status = sss_key_store_freeze_key(&gtCtx.ks, &gtCtx.key);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Freeze KeyPair Failed!!!");
}

TEST(KeyStoreFRst, KS_FreezePublicKey)
{
    sss_status_t status;
    RUN_TEST_SCENARIO(KeyStore, KS_SetECCPublicKey);
    status = sss_key_store_freeze_key(&gtCtx.ks, &gtCtx.key);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Freeze Public Key Failed!!!");
}

TEST(KeyStoreFRst, KS_EraseKeyPair)
{
    sss_status_t status;
    RUN_TEST_SCENARIO(KeyStore, KS_GenerateECC256Key);
    status = sss_key_store_erase_key(&gtCtx.ks, &gtCtx.key);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Erase KeyPair Failed!!!");
}

TEST(KeyStoreFRst, KS_ErasePublicKey)
{
    sss_status_t status;
    RUN_TEST_SCENARIO(KeyStore, KS_SetECCPublicKey);
    status = sss_key_store_erase_key(&gtCtx.ks, &gtCtx.key);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Erase Public Key Failed!!!");
}
TEST(KeyStore, KS_Create_Multiple_TransientObjects)
{
    sss_status_t status;
    uint8_t myCert[] = "Empty!";
    sss_object_t key[100];
    int i = 0;
    uint32_t keyId = 1;
    /* Store Certificate */
    RUN_TEST_SCENARIO(KeyStore, KS_AllocateKeyStore);

    for (i = 0; i < ARRAY_SIZE(key); i++)
    {
        sss_key_object_init(&key[i], &gtCtx.ks);
        status = sss_key_object_allocate_handle(&key[i], keyId++, kSSS_KeyType_Certificate, sizeof(myCert),
                                                kKeyObject_Mode_Transient);
        TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_allocate_handle Failed!!!");
        status = sss_key_store_set_key(&gtCtx.ks, &key[i], myCert, sizeof(myCert) * 8, NULL, 0);
        TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Store Certificate Failed!!!");
    }
    for (i = 0; i < ARRAY_SIZE(key); i++)
    {
        sss_key_object_free(&key[i]);
    }
}

TEST(KeyStoreFRst, KS_Create_Multiple_Persistent_Objects)
{
    sss_status_t status;
    uint8_t myCert[] = "Empty!";
    sss_object_t key[KS_N_ENTIRES];
    int i = 0;
    uint32_t keyId = __LINE__;
    uint32_t cnt = 0;
    keyId = change(keyId);

    /* Store Certificate */

    RUN_TEST_SCENARIO(KeyStore, KS_AllocateKeyStore);
    for (i = 0; i < ARRAY_SIZE(key); i++)
    {
        sss_key_object_init(&key[i], &gtCtx.ks);
    }

    for (i = 0; i < KS_N_ECC_KEY_PAIRS; i++)
    {
        status = sss_key_object_allocate_handle(&key[cnt++], keyId++, kSSS_KeyType_ECC_Pair, sizeof(myCert),
                                                kKeyObject_Mode_Persistent);
        TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_allocate_handle Failed!!!");
    }
    for (i = 0; i < KS_N_ECC_PUB_KEYS; i++)
    {
        status = sss_key_object_allocate_handle(&key[cnt++], keyId++, kSSS_KeyType_ECC_Public, sizeof(myCert),
                                                kKeyObject_Mode_Persistent);
        TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_allocate_handle Failed!!!");
    }
    for (i = 0; i < KS_N_AES_KEYS; i++)
    {
        status = sss_key_object_allocate_handle(&key[cnt++], keyId++, kSSS_KeyType_AES, sizeof(myCert),
                                                kKeyObject_Mode_Persistent);
        TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_allocate_handle Failed!!!");
    }
    for (i = 0; i < KS_N_CERTIFCATES; i++)
    {
        status = sss_key_object_allocate_handle(&key[cnt++], keyId++, kSSS_KeyType_Certificate, sizeof(myCert),
                                                kKeyObject_Mode_Persistent);
        TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_allocate_handle Failed!!!");
    }
    for (i = 0; i < ARRAY_SIZE(key); i++)
    {
        sss_key_store_erase_key(&gtCtx.ks, &key[i]);
        sss_key_object_free(&key[i]);
    }
}

TEST_GROUP_RUNNER(KeyStore)
{
    ///*Test case Save KeyStore*/
    // RUN_TEST_CASE(KeyStore, KS_SaveKeyStore);

    /*Test case Load KeyStore*/
    RUN_TEST_CASE(KeyStore, KS_LoadKeyStore);

    /*Test case Generate Key*/
    RUN_TEST_CASE(KeyStore, KS_GenerateECC256Key);

    /*Test case Set Key*/
    RUN_TEST_CASE(KeyStore, KS_SetECCPublicKey);
    RUN_TEST_CASE(KeyStore, KS_StoreCertificate);
    RUN_TEST_CASE(KeyStore, KS_SetSymmKey);

    /*Test case Get Key*/
    RUN_TEST_CASE(KeyStore, KS_GetECCPublicKey);
    RUN_TEST_CASE(KeyStore, KS_GetCertificate);

    /* Test Case to create Multiple Transient and Persistent Keys*/
    RUN_TEST_CASE(KeyStore, KS_Create_Multiple_TransientObjects);
    RUN_TEST_CASE(KeyStore, KS_SetECCKeyPair);
}

TEST_GROUP_RUNNER(KeyStoreFRst)
{
    RUN_TEST_CASE(KeyStoreFRst, KS_Create_Multiple_Persistent_Objects);
#if SSS_HAVE_SSCP

    /*Test case Freeze Key*/
    RUN_TEST_CASE(KeyStoreFRst, KS_FreezeKeyPair);
    RUN_TEST_CASE(KeyStoreFRst, KS_FreezePublicKey);

    /*Test case Erase Key*/
    RUN_TEST_CASE(KeyStoreFRst, KS_EraseKeyPair);
    RUN_TEST_CASE(KeyStoreFRst, KS_ErasePublicKey);

#endif
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
    RUN_TEST_GROUP(KeyStore);
    RUN_TEST_GROUP(KeyStoreFRst);
}
