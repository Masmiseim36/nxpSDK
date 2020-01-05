/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef SECUREIOTMW_SSS_TST_INC_SSS_TST_H_
#define SECUREIOTMW_SSS_TST_INC_SSS_TST_H_

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <unity_fixture.h>
#include "fsl_sss_api.h"
#include "fsl_sss_ftr.h"

#if SSS_HAVE_MBEDTLS
#include "fsl_sss_mbedtls_apis.h"
#include "mbedtls/version.h"
#endif

#if SSS_HAVE_OPENSSL
#include "openssl/opensslv.h"
#endif

#if SSS_HAVE_SSCP
#include "fsl_sss_sscp_types.h"
#include "sm_types.h"
#endif

/* ************************************************************************** */
/* Defines                                                                    */
/* ************************************************************************** */

#define TEST_ASSERT_T(condition) TEST_ASSERT_MESSAGE(condition, #condition " : Failed")

#define RUN_TEST_SCENARIO(group, name)   \
    /* (forward) declare */              \
    void TEST_##group##_##name##_(void); \
    /* call */                           \
    TEST_##group##_##name##_()

/* ************************************************************************** */
/* Structrues and Typedefs                                                    */
/* ************************************************************************** */

typedef struct testCtx_t
{
    uint8_t fixture_setup_done;
    sss_session_t session;
    sss_key_store_t ks;
    sss_object_t key;
    sss_asymmetric_t asymm;
    sss_symmetric_t symm;
    sss_derive_key_t derv;
#if SSS_HAVE_SSCP
    sscp_context_t sscp;
#endif
} testCtx_t;

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

extern const char *gszA71COMPortDefault;
extern const char *gszA71SocketPortDefault;

extern testCtx_t gtCtx;

/* ************************************************************************** */
/* Functions                                                                  */
/* ************************************************************************** */

void DoFixtureSetupIfNeeded(bool withFactoryReset);
void DoCommonSetUp(void);
void DoCommonTearDown(void);
void DoFixtureTearDownIfNeeded(void);
void DoFactoryReset(void);

#endif /* SECUREIOTMW_SSS_TST_INC_SSS_TST_H_ */
