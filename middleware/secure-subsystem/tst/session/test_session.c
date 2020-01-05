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

#define CHECK_SIZE(hw_type, base_type)          \
    printf("%s = %u\n", #hw_type, hw_type);     \
    printf("%s = %u\n", #base_type, base_type); \
    TEST_ASSERT_LESS_OR_EQUAL_MESSAGE(base_type, hw_type, #hw_type " <= " #base_type)

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

TEST_GROUP(Session);

TEST_SETUP(Session)
{
}

TEST_TEAR_DOWN(Session)
{
}

TEST(Session, DoOpenSession_mbedTLS)
{
#if SSS_HAVE_MBEDTLS
#if SSS_HAVE_SSS == 1
    sss_mbedtls_session_t mySession;
#else
    sss_session_t mySession;
#endif
    sss_status_t status;
    status = sss_open_session(&mySession, kType_SSS_mbedTLS, 0, 0, NULL);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "sss_open_session should pass");
    sss_close_session(&mySession);
#else
    TEST_IGNORE_MESSAGE("Compiled without mbed TLS");
#endif
}

TEST(Session, DoOpenSession_a71ch_Socket)
{
#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR
    sss_session_t mySession;
    sss_status_t status;

    status =
        sss_open_session(&mySession, kType_SSS_SE_A71CH, 0, SSS_SSCP_CONNECTIONTYPE_SOCKET, gszA71SocketPortDefault);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "A71CH connection should pass with socket.");

    sss_close_session(&mySession);
#else
    TEST_IGNORE_MESSAGE("Compiled without A71CH");
#endif
}

TEST(Session, DoOpenSession_a71ch_VCOM)
{
#if SSS_HAVE_A71CH
    sss_session_t mySession;
    sss_status_t status = status =
        sss_open_session(&mySession, kType_SSS_SE_A71CH, 0, SSS_SSCP_CONNECTIONTYPE_VCOM, gszA71COMPortDefault);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "A71CH connection should pass with vcom.");
    sss_close_session(&mySession);
#else
    TEST_IGNORE_MESSAGE("Compiled without A71CH");
#endif
}

TEST(Session, DoOpenSession_a71ch_NULL)
{
#if SSS_HAVE_A71CH
    sss_session_t mySession;
    sss_status_t status;
    status = sss_open_session(&mySession, kType_SSS_SE_A71CH, 0, 0, NULL);
    TEST_ASSERT_T(kStatus_SSS_InvalidArgument == status);
    sss_close_session(&mySession);
#else
    TEST_IGNORE_MESSAGE("Compiled without A71CH");
#endif
}

TEST(Session, DoTEE_a71ch_VCOM)
{
#if SSS_HAVE_A71CH
    sss_session_t mySession;
    sss_key_store_t keyStore;
    sss_status_t status;
    const sss_type_t sub_system = kType_SSS_SE_A71CH;
    sss_object_t keyObj;
    status = sss_open_session(&mySession, sub_system, 0, SSS_SSCP_CONNECTIONTYPE_VCOM, "COM9");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(kStatus_SSS_Success, status, "A71CH connection should pass with vcom.");

    sss_key_store_context_init(&keyStore, &mySession);
    sss_key_object_init(&keyObj, &keyStore);
    keyObj.objectType = kSSS_KeyType_ECC_Pair;
    sss_key_store_generate_key(&keyStore, &keyObj, 256, NULL);

    sss_close_session(&mySession);
#else
    TEST_IGNORE_MESSAGE("Compiled without A71CH");
#endif
}

TEST(Session, CheckSizes_Session_t)
{
#if SSS_HAVE_SSCP
    CHECK_SIZE(sizeof(sss_sscp_session_t), sizeof(sss_session_t));
    CHECK_SIZE(sizeof(sss_sscp_object_t), sizeof(sss_object_t));
    CHECK_SIZE(sizeof(sss_sscp_key_store_t), sizeof(sss_key_store_t));
    CHECK_SIZE(sizeof(sss_sscp_asymmetric_t), sizeof(sss_asymmetric_t));
    CHECK_SIZE(sizeof(sss_sscp_symmetric_t), sizeof(sss_symmetric_t));
#endif
#if SSS_HAVE_MBEDTLS
    CHECK_SIZE(sizeof(sss_mbedtls_session_t), sizeof(sss_session_t));
    CHECK_SIZE(sizeof(sss_mbedtls_object_t), sizeof(sss_object_t));
    CHECK_SIZE(sizeof(sss_mbedtls_key_store_t), sizeof(sss_key_store_t));
    CHECK_SIZE(sizeof(sss_mbedtls_asymmetric_t), sizeof(sss_asymmetric_t));
    CHECK_SIZE(sizeof(sss_mbedtls_symmetric_t), sizeof(sss_symmetric_t));
#endif
#if SSS_HAVE_OPENSSL
    CHECK_SIZE(sizeof(sss_openssl_session_t), sizeof(sss_session_t));
    CHECK_SIZE(sizeof(sss_openssl_object_t), sizeof(sss_object_t));
    CHECK_SIZE(sizeof(sss_openssl_key_store_t), sizeof(sss_key_store_t));
    CHECK_SIZE(sizeof(sss_openssl_asymmetric_t), sizeof(sss_asymmetric_t));
    CHECK_SIZE(sizeof(sss_openssl_symmetric_t), sizeof(sss_symmetric_t));
#endif
}

TEST_GROUP_RUNNER(Session)
{
    /* TODO : Purnank These test cases should be run based on
     *        compile or run time options.
     */
    RUN_TEST_CASE(Session, CheckSizes_Session_t);
    RUN_TEST_CASE(Session, DoOpenSession_mbedTLS);
    RUN_TEST_CASE(Session, DoOpenSession_a71ch_Socket);
    RUN_TEST_CASE(Session, DoOpenSession_a71ch_VCOM);
    RUN_TEST_CASE(Session, DoOpenSession_a71ch_NULL);
}

int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, runAllTests);
}

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */

static void runAllTests(void)
{
    RUN_TEST_GROUP(Session);
}
