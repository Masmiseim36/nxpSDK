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

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */
uint32_t gkeyId = __LINE__;
/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

static void runAllTests(void);

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

TEST_GROUP(KeyObject);

TEST_SETUP(KeyObject)
{
    DoFixtureSetupIfNeeded(true);
    /* No Common setup here...
     * Because Common setup inits more contexts
     * than needed */
    if (gtCtx.ks.session == NULL)
    {
        sss_key_store_context_init(&gtCtx.ks, &gtCtx.session);
    }
    if (gtCtx.key.keyStore == NULL)
    {
        sss_key_object_init(&gtCtx.key, &gtCtx.ks);
    }
}

TEST_TEAR_DOWN(KeyObject)
{
    DoCommonTearDown();
}

TEST(KeyObject, _deAllocateKeyStore)
{
    sss_key_store_context_free(&gtCtx.ks);
    sss_key_store_context_init(&gtCtx.ks, &gtCtx.session);
}

TEST(KeyObject, KS_AllocateKeyStore)
{
    sss_status_t status;
    /* Allocate key */
    uint32_t myKeyStoreId = __LINE__;
    RUN_TEST_SCENARIO(KeyObject, _deAllocateKeyStore);
    status = sss_key_store_allocate(&gtCtx.ks, myKeyStoreId);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_store_allocate Failed.");
}

TEST(KeyObject, KO_AllocateHandle)
{
    sss_status_t status;
    uint32_t mykeyId = __LINE__;
    size_t keyByteLenMax = 64;
    uint32_t options = kKeyObject_Mode_Persistent;

    RUN_TEST_SCENARIO(KeyObject, KS_AllocateKeyStore);
    /* Allocate Handle */
    status = sss_key_object_allocate_handle(&gtCtx.key, mykeyId, kSSS_KeyType_ECC_Pair, keyByteLenMax, options);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Allocate Handle Failed!!!");
}

TEST(KeyObject, KS_StoreCertificate_Save)
{
    sss_status_t status;
    uint8_t myCert[100] = "SOME RANDOM (INVALID The quick brown fox...) TEXT INSIDE CERTIFICATE";
    /* Store Certificate */
    RUN_TEST_SCENARIO(KeyObject, KS_AllocateKeyStore);

    status = sss_key_object_allocate_handle(&gtCtx.key, gkeyId, kSSS_KeyType_Certificate, sizeof(myCert),
                                            kKeyObject_Mode_Persistent);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_allocate_handle Failed!!!");

    status = sss_key_store_set_key(&gtCtx.ks, &gtCtx.key, myCert, sizeof(myCert) * 8, NULL, 0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Store Certificate Failed!!!");
}

TEST(KeyObject, KS_SaveKeyStore)
{
    sss_status_t status;
    RUN_TEST_SCENARIO(KeyObject, KS_StoreCertificate_Save);
    status = sss_key_store_save(&gtCtx.ks);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Save KeyStore Failed!!!");
}

TEST(KeyObject, KS_LoadKeyStore)
{
    sss_status_t status;
    /* Save key */
    status = sss_key_store_load(&gtCtx.ks);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, " Load KeyStore Failed!!!");
}

TEST(KeyObject, KO_GetHandle)
{
    sss_status_t status;
    /* Get Handle */
    RUN_TEST_SCENARIO(KeyObject, KS_AllocateKeyStore);
    RUN_TEST_SCENARIO(KeyObject, KS_LoadKeyStore);
    status = sss_key_object_get_handle(&gtCtx.key, gkeyId);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Get Handle Failed!!!");
}

TEST(KeyObject, KO_SetUser)
{
    sss_status_t status;
    uint32_t options = 0;
    uint32_t user = 0;
    /* Set User */
    status = sss_key_object_set_user(&gtCtx.key, user, options);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Set User Failed!!!");
}

TEST(KeyObject, KO_SetPurpose)
{
    sss_status_t status;
    uint32_t options = 0;
    sss_mode_t purpose = kMode_SSS_Encrypt;
    /* Set Purpose */
    status = sss_key_object_set_purpose(&gtCtx.key, purpose, options);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Set Purpose Failed!!!");
}

TEST(KeyObject, KO_SetAccess)
{
    sss_status_t status;
    uint32_t options = 0;
    uint32_t access = 0;
    /* Set Access */
    status = sss_key_object_set_access(&gtCtx.key, access, options);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Set Access Failed!!!");
}

TEST(KeyObject, KO_GetUser)
{
    sss_status_t status;
    uint32_t user = 0;
    /* Get User */
    status = sss_key_object_get_user(&gtCtx.key, &user);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Get User Failed!!!");
}

TEST(KeyObject, KO_GetPurpose)
{
    sss_status_t status;
    sss_mode_t purpose = kMode_SSS_Encrypt;
    /* Get Purpose */
    status = sss_key_object_get_purpose(&gtCtx.key, &purpose);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Get Purpose Failed!!!");
}

TEST(KeyObject, KO_GetAccess)
{
    sss_status_t status;
    uint32_t access = 0;
    /* Get Access */
    status = sss_key_object_get_access(&gtCtx.key, &access);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Get Access Failed!!!");
}

TEST_GROUP_RUNNER(KeyObject)
{
    RUN_TEST_CASE(KeyObject, KO_AllocateHandle);
    RUN_TEST_CASE(KeyObject, KS_SaveKeyStore);
    RUN_TEST_CASE(KeyObject, KO_GetHandle);
    // RUN_TEST_CASE(KeyObject, KO_SetUser);
    // RUN_TEST_CASE(KeyObject, KO_SetPurpose);
    // RUN_TEST_CASE(KeyObject, KO_SetAccess);
    // RUN_TEST_CASE(KeyObject, KO_GetUser);
    // RUN_TEST_CASE(KeyObject, KO_GetPurpose);
    // RUN_TEST_CASE(KeyObject, KO_GetAccess);
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
    RUN_TEST_GROUP(KeyObject);
}
