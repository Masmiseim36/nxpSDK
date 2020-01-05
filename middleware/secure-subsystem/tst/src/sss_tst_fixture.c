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

#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR
#include <HLSEAPI.h>
#include <fsl_sscp_a71ch.h>
#if AX_EMBEDDED
#include <app_boot.h>
#endif /* AX_EMBEDDED */
#endif /* SSS_HAVE_A71CH / EAR */

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */

#define TEST_ROOT_FOLDER "."

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

const char *gszA71COMPortDefault = NULL; //"COM9";
const char *gszA71SocketPortDefault = "127.0.0.1:8050";

testCtx_t gtCtx = {0};

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

static sss_status_t SessionOpen(void);
static void SessionClose(void);
static void test_assert_all_strutures_zero();
const char *file_name = __FILE__;

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

void DoFactoryReset()
{
#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR
    printf("\r\nDebug Reset A71CH\r\n");
    HLSE_DbgReset();
#endif
#if SSS_HAVE_MBEDTLS
#ifdef MBEDTLS_FS_IO
    ks_mbedtls_fat_remove(TEST_ROOT_FOLDER);
#endif
#endif
}

void DoFixtureSetupIfNeeded(bool withFactoryReset)
{
    sss_status_t status = kStatus_SSS_Fail;
    if (!gtCtx.fixture_setup_done)
    {
        printf("\r\n%s:\r\n", Unity.TestFile);
        printf("Compiled on:" __DATE__ " " __TIME__ "\r\n");
        status = SessionOpen();
        if (withFactoryReset && (status == kStatus_SSS_Success))
        {
            DoFactoryReset();
        }
        gtCtx.fixture_setup_done = TRUE;
    }
    TEST_ASSERT_T(gtCtx.fixture_setup_done == TRUE);
}

void DoCommonSetUp()
{
    sss_status_t status;
    if (gtCtx.ks.session == NULL)
    {
        status = sss_key_store_context_init(&gtCtx.ks, &gtCtx.session);
        TEST_ASSERT_TRUE_MESSAGE(status == kStatus_SSS_Success, "sss_key_store_context_init");
    }
    if (gtCtx.key.keyStore == NULL)
    {
        status = sss_key_object_init(&gtCtx.key, &gtCtx.ks);
        TEST_ASSERT_TRUE_MESSAGE(status == kStatus_SSS_Success, "sss_key_object_init");
    }

    status = sss_key_store_allocate(&gtCtx.ks, __LINE__);
    TEST_ASSERT_TRUE_MESSAGE(status == kStatus_SSS_Success, "sss_key_store_allocate");
}
void DoCommonTearDown(void)
{
    Unity.TestFile = file_name;
    if (gtCtx.asymm.keyObject)
    {
        sss_asymmetric_context_free(&gtCtx.asymm);
    }
    if (gtCtx.key.keyStore != NULL)
    {
        sss_key_object_free(&gtCtx.key);
    }
    if (gtCtx.ks.session != NULL)
    {
        sss_key_store_context_free(&gtCtx.ks);
    }
}

void DoFixtureTearDownIfNeeded()
{
    if (gtCtx.fixture_setup_done)
    {
        SessionClose();
        gtCtx.fixture_setup_done = false;
        printf("\r\nFinished %s:\r\n", Unity.TestFile);
        printf("Compiled on:" __DATE__ " " __TIME__ "\r\n");
    }
    test_assert_all_strutures_zero();
}

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */
static sss_status_t SessionOpen(void)
{
    sss_status_t status = kStatus_SSS_Success;
#if AX_EMBEDDED
    app_boot_Init();
#endif

    do
    {
#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR
#if RJCT_VCOM
        if (NULL != gszA71COMPortDefault)
        {
            status = sss_open_session(&gtCtx.session, kType_SSS_SE_A71CH, 0, SSS_SSCP_CONNECTIONTYPE_VCOM,
                                      gszA71COMPortDefault);
            break;
        }
#endif
#if defined(RJCT_SOCKET) || defined(RJCT_JRCP)
        if (NULL != gszA71SocketPortDefault)
        {
            status = sss_open_session(&gtCtx.session, kType_SSS_SE_A71CH, 0, SSS_SSCP_CONNECTIONTYPE_SOCKET,
                                      gszA71SocketPortDefault);
            break;
        }
#endif
#if defined(I2C)
        status = sss_open_session(&gtCtx.session, kType_SSS_SE_A71CH, 0, SSS_SSCP_CONNECTIONTYPE_SCI2C, NULL);
        break;
#endif
#elif SSS_HAVE_MBEDTLS
        status = sss_open_session(&gtCtx.session, kType_SSS_mbedTLS, 0, 0, TEST_ROOT_FOLDER);
#endif
    } while (0);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "Open Session Failed!!!");

    status = sss_key_store_context_init(&gtCtx.ks, &gtCtx.session);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_store_context_init Failed!!!");

#if SSS_HAVE_A71CH
    {
        sss_sscp_session_t *sscp_session = (sss_sscp_session_t *)&gtCtx.session;
        sss_a71ch_key_store_t *a71ch_keystore = (sss_a71ch_key_store_t *)&gtCtx.ks;
        sscp_a71ch_context_t *sscp_context = (sscp_a71ch_context_t *)&gtCtx.sscp;
        if (sscp_session->sscp_context == NULL)
        {
            status = sscp_a71ch_init(sscp_context, a71ch_keystore);
            TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sscp_a71ch_init");
        }
    }
#endif

    status = sss_key_object_init(&gtCtx.key, &gtCtx.ks);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_key_object_init Failed!!!");
    return status;
}

static void SessionClose(void)
{
#if SSS_HAVE_A71CH
    {
        sscp_a71ch_context_t *sscp_context = (sscp_a71ch_context_t *)&gtCtx.sscp;
        if (sscp_context->invoke != NULL)
        {
            sscp_a71ch_free(sscp_context);
        }
    }
#endif
    if (gtCtx.key.keyStore != NULL)
    {
        sss_key_object_free(&gtCtx.key);
    }

    TEST_ASSERT_T(gtCtx.key.keyStore == NULL);
    if (gtCtx.ks.session != NULL)
    {
        sss_key_store_context_free(&gtCtx.ks);
    }
    TEST_ASSERT_T(gtCtx.ks.session == NULL);
    if (gtCtx.session.subsystem != kType_SSS_SubSystem_NONE)
    {
        sss_close_session(&gtCtx.session);
    }
    TEST_ASSERT_T(gtCtx.session.subsystem == kType_SSS_SubSystem_NONE);
}

/* Someone missed to either to real de-init or de-init with wrong API Done */
static void test_assert_all_strutures_zero()
{
    testCtx_t local = {0};

    TEST_ASSERT_EACH_EQUAL_MEMORY((UNITY_INTERNAL_PTR)&local.session, (UNITY_INTERNAL_PTR)&gtCtx.session,
                                  sizeof(local.session), 1);
    TEST_ASSERT_EACH_EQUAL_MEMORY((UNITY_INTERNAL_PTR)&local.ks, (UNITY_INTERNAL_PTR)&gtCtx.ks, sizeof(local.ks), 1);
    TEST_ASSERT_EACH_EQUAL_MEMORY((UNITY_INTERNAL_PTR)&local.key, (UNITY_INTERNAL_PTR)&gtCtx.key, sizeof(local.key), 1);
    TEST_ASSERT_EACH_EQUAL_MEMORY((UNITY_INTERNAL_PTR)&local.asymm, (UNITY_INTERNAL_PTR)&gtCtx.asymm,
                                  sizeof(local.asymm), 1);
    TEST_ASSERT_EACH_EQUAL_MEMORY((UNITY_INTERNAL_PTR)&local.symm, (UNITY_INTERNAL_PTR)&gtCtx.symm, sizeof(local.symm),
                                  1);
// TEST_ASSERT_EACH_EQUAL_MEMORY((UNITY_INTERNAL_PTR)&local.derv,
//    (UNITY_INTERNAL_PTR)&gtCtx.derv,
//    sizeof(local.derv),
//    1);
#if SSS_HAVE_SSCP
    TEST_ASSERT_EACH_EQUAL_MEMORY((UNITY_INTERNAL_PTR)&local.sscp, (UNITY_INTERNAL_PTR)&gtCtx.sscp, sizeof(local.sscp),
                                  1);
#endif
}
