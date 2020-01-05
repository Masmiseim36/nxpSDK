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
#include <string.h>

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */

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

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

TEST_GROUP(SymmTest);

TEST_SETUP(SymmTest)
{
    DoFixtureSetupIfNeeded(true);
    DoCommonSetUp();
}

TEST_TEAR_DOWN(SymmTest)
{
    DoCommonTearDown();
}

TEST(SymmTest, SYMM_AES256_CBC_Encrypt)
{
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    /* clang-format off */
    const uint8_t srcData[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t keystring[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t expectedData[] = {0x8b, 0x52, 0x7a, 0x6a, 0xeb, 0xda, 0xec, 0x9e, 0xae, 0xf8, 0xed, 0xa2, 0xcb, 0x77, 0x83, 0xe5};
    size_t destDataLen = 16;
    /*IV is not required for AES ECB*/
    uint8_t iv[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    size_t ivlen;
    uint8_t destData[32];
    uint32_t keyid = 0xDEADBEEF;
    sss_key_type_t keyType;
    size_t keyByteLenMax = 16;
    /* clang-format on */
    memset(destData, 0x00, 32);
    destDataLen = sizeof(srcData);
    ivlen = sizeof(iv);
    algorithm = kAlgorithm_SSS_AES_CBC;
    keyType = kSSS_KeyType_AES;
    mode = kMode_SSS_Encrypt;

    status = sss_key_object_allocate_handle(&gtCtx.key, keyid, keyType, keyByteLenMax, kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "key_object_allocate_handle Failed!!!");
    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, keystring, sizeof(keystring) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set AES Key Failed!!!");

    status = sss_symmetric_context_init(&gtCtx.symm, &gtCtx.session, &gtCtx.key, algorithm, mode);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Symmetric context init failed!!!");

    status = sss_cipher_one_go(&gtCtx.symm, iv, ivlen, srcData, destData, destDataLen);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Cipher one go failed!!!");

    if (memcmp(destData, expectedData, destDataLen) == 0)
    {
        printf("AES CBC encryption successful \n");
        status = kStatus_SSS_Success;
    }
    else
    {
        printf("AES CBC encryption FAILED \n");
        status = kStatus_SSS_Fail;
    }

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "AES encryption ecb failed   !!!");
}

TEST(SymmTest, SYMM_AES256_CBC_Decrypt)
{
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    /* clang-format off */
    const uint8_t srcData[] = {0x23, 0xf7, 0x10, 0x84, 0x2b, 0x9b, 0xb9, 0xc3, 0x2f, 0x26, 0x64, 0x8c, 0x78, 0x68, 0x7, 0xca};
    uint8_t keystring[] = {0xff, 0xff, 0xff, 0xff, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    uint8_t expectedData[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    size_t destDataLen = 16;
    uint8_t iv[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    size_t ivlen;
    uint8_t destData[32];
    uint32_t keyid = 0xDEADBEEF;
    sss_key_type_t keyType;
    size_t keyByteLenMax = 16;

    /* clang-format on */
    memset(destData, 0x00, 32);
    destDataLen = sizeof(srcData);
    ivlen = sizeof(iv);
    algorithm = kAlgorithm_SSS_AES_CBC;
    keyType = kSSS_KeyType_AES;
    mode = kMode_SSS_Decrypt;

    status = sss_key_object_allocate_handle(&gtCtx.key, keyid, keyType, keyByteLenMax, kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "key_object_allocate_handle Failed!!!");
    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, keystring, sizeof(keystring) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set AES Key Failed!!!");

    status = sss_symmetric_context_init(&gtCtx.symm, &gtCtx.session, &gtCtx.key, algorithm, mode);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Symmetric context init failed!!!");

    status = sss_cipher_one_go(&gtCtx.symm, iv, ivlen, srcData, destData, destDataLen);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Cipher one go failed!!!");

    if (memcmp(destData, expectedData, destDataLen) == 0)
    {
        printf("AES CBC decryption successful \n");
        status = kStatus_SSS_Success;
    }
    else
    {
        printf("AES CBC decryption FAILED \n");
        status = kStatus_SSS_Fail;
    }

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "AES decryption cbc failed   !!!");
}

TEST(SymmTest, SYMM_AES256_ECB_Encrypt)
{
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    /* clang-format off */
    const uint8_t srcData[] = {0xf3, 0x44, 0x81, 0xec, 0x3c, 0xc6, 0x27, 0xba, 0xcd, 0x5d, 0xc3, 0xfb, 0x8, 0xf2, 0x73, 0xe6};
    uint8_t keystring[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t expectedData[] = {0x03, 0x36, 0x76, 0x3e, 0x96, 0x6d, 0x92, 0x59, 0x5a, 0x56, 0x7c, 0xc9, 0xce, 0x53, 0x7f, 0x5e};
    size_t destDataLen = sizeof(expectedData);
    /*IV is not required for AES ECB*/
    uint8_t iv = 0;
    size_t ivlen = 0;
    uint8_t destData[32];
    uint32_t keyid = 0xDEADBEEF;
    sss_key_type_t keyType;
    size_t keyByteLenMax = 16;
    /* clang-format on */

    memset(destData, 0x00, 32);
    destDataLen = sizeof(srcData);
    keyType = kSSS_KeyType_AES;
    algorithm = kAlgorithm_SSS_AES_ECB;
    mode = kMode_SSS_Encrypt;

    status = sss_key_object_allocate_handle(&gtCtx.key, keyid, keyType, keyByteLenMax, kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "key_object_allocate_handle Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, keystring, sizeof(keystring) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set AES Key Failed!!!");

    status = sss_symmetric_context_init(&gtCtx.symm, &gtCtx.session, &gtCtx.key, algorithm, mode);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Symmetric context init failed!!!");

    status = sss_cipher_one_go(&gtCtx.symm, &iv, ivlen, srcData, destData, destDataLen);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Cipher one go failed!!!");

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_cipher_one_go failed.");
    TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expectedData, destData, destDataLen, "AES ECB encryption FAILED");
}

TEST(SymmTest, SYMM_AES256_ECB_Decrypt)
{
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    /* clang-format off */
    const uint8_t srcData[] = {0xdb, 0x4f, 0x1a, 0xa5, 0x30, 0x96, 0x7d, 0x67, 0x32, 0xce, 0x47, 0x15, 0xeb, 0xe, 0xe2, 0x4b};
    uint8_t keystring[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t expectedData[] = {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    size_t destDataLen = sizeof(expectedData);
    uint8_t iv = 0;
    size_t ivlen = 0;
    uint8_t destData[32];
    uint32_t keyid = 0xDEADBEEF;
    sss_key_type_t keyType;
    size_t keyByteLenMax = 16;
    /* clang-format on */

    memset(destData, 0x00, 32);
    destDataLen = sizeof(srcData);

    keyType = kSSS_KeyType_AES;
    algorithm = kAlgorithm_SSS_AES_ECB;
    mode = kMode_SSS_Decrypt;

    status = sss_key_object_allocate_handle(&gtCtx.key, keyid, keyType, keyByteLenMax, kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "key_object_allocate_handle Failed!!!");
    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, keystring, sizeof(keystring) * 8, NULL, 0);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set AES Key Failed!!!");

    status = sss_symmetric_context_init(&gtCtx.symm, &gtCtx.session, &gtCtx.key, algorithm, mode);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Cipher one go failed!!!");

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Symmetric context init failed!!!");

    status = sss_cipher_one_go(&gtCtx.symm, &iv, ivlen, srcData, destData, destDataLen);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "AES decryption ecb failed   !!!");
    if (memcmp(destData, expectedData, destDataLen) == 0)
    {
        printf("AES ECB decryption successful \n");
        status = kStatus_SSS_Success;
    }
    else
    {
        printf("AES ECB decryption FAILED \n");
        status = kStatus_SSS_Fail;
    }
}

TEST(SymmTest, SYMM_AES256_ECB_Encrypt_MultiSteps)
{
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    /* clang-format off */
    uint8_t key[16];
    uint8_t iv[16];
    uint8_t inbuf[64];
    uint8_t encbuf[64];
    uint8_t decbuf[64];
    size_t encbufLen;
    size_t decbuflen;
    size_t inbuflen;
    /*IV is not required for AES ECB*/
    size_t ivlen;
    uint32_t keyid = 0xDEADBEEF;
    sss_key_type_t keyType;
    size_t keyByteLenMax;

    /* clang-format on */
    ivlen = sizeof(iv);
    algorithm = kAlgorithm_SSS_AES_ECB;
    keyType = kSSS_KeyType_AES;
    mode = kMode_SSS_Encrypt;
    inbuflen = sizeof(inbuf);

    encbufLen = 0x0;

    memset(key, 0x2a, sizeof(key));
    memset(iv, 0x00, sizeof(iv));
    memset(inbuf, 0x20, sizeof(inbuf));
    memset(encbuf, 0x00, sizeof(encbuf));
    memset(decbuf, 0x00, sizeof(decbuf));

    keyByteLenMax = sizeof(key);

    status = sss_key_object_allocate_handle(&gtCtx.key, keyid, keyType, keyByteLenMax, kKeyObject_Mode_Persistent);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "key_object_allocate_handle Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, key, sizeof(key) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Set AES Key Failed!!!");

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "failed.");

    gtCtx.symm.keyObject = &gtCtx.key;
    status = sss_symmetric_context_init(&gtCtx.symm, &gtCtx.session, &gtCtx.key, algorithm, mode);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Symmetric context init failed!!!");

    status = sss_cipher_init(&gtCtx.symm, iv, ivlen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "failed.");

    status = sss_cipher_update(&gtCtx.symm, inbuf, inbuflen, encbuf, &encbufLen);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_cipher_Update failed.");

    status = sss_cipher_finish(&gtCtx.symm, encbuf + encbufLen, encbufLen, NULL, 0);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "AES Multi parts encryption cbc failed   !!!");

    mode = kMode_SSS_Decrypt;

    decbuflen = 0x0;
    status = sss_symmetric_context_init(&gtCtx.symm, &gtCtx.session, &gtCtx.key, algorithm, mode);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Symmetric context init failed!!!");

    status = sss_cipher_init(&gtCtx.symm, iv, ivlen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "failed.");

    status = sss_cipher_update(&gtCtx.symm, encbuf, encbufLen, decbuf, &decbuflen);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "failed.");
    status = sss_cipher_finish(&gtCtx.symm, decbuf + decbuflen, decbuflen, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "failed.");

    if (memcmp(inbuf, decbuf, decbuflen) == 0)
    {
        printf("AES ECB Multi parts  encryption decryption successful \n");
        status = kStatus_SSS_Success;
    }
    else
    {
        printf("AES ECB multi parts encryption failed decryption FAILED \n");
        status = kStatus_SSS_Fail;
    }
}

TEST_GROUP_RUNNER(SymmTest)
{
    RUN_TEST_CASE(SymmTest, SYMM_AES256_ECB_Encrypt);
    RUN_TEST_CASE(SymmTest, SYMM_AES256_ECB_Decrypt);
    RUN_TEST_CASE(SymmTest, SYMM_AES256_CBC_Encrypt);
    RUN_TEST_CASE(SymmTest, SYMM_AES256_CBC_Decrypt);
    RUN_TEST_CASE(SymmTest, SYMM_AES256_ECB_Encrypt_MultiSteps);
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
    RUN_TEST_GROUP(SymmTest);
}
