/*
 * Copyright 2018,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <ex_sss.h>
#include <ex_sss_boot.h>
#include <fsl_sss_se05x_apis.h>
#include <nxEnsure.h>
#include <nxLog_App.h>

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */
#define EC_KEY_BIT_LEN 256
/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

static ex_sss_boot_ctx_t gex_sss_rsa_boot_ctx;

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_rsa_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 1
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

#include <ex_sss_main_inc.h>

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status;
    uint8_t digest[32] = {0};
    size_t digestLen;
    uint8_t signature[512] = {0};
    size_t signatureLen;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    size_t val = 0;
    /* asymmetric Sign */
    algorithm = kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA512;
    mode = kMode_SSS_Sign;
    digestLen = sizeof(digest);
    signatureLen = sizeof(signature);

    sss_object_t key;
    sss_asymmetric_t ctx_asymm = {0};

    LOG_I("Running RSA Example ex_sss_rsa.c");

    for (; val < sizeof(digest); ++val)
        digest[val] = (uint8_t)val;

    size_t keylen = 4096 / 8;
    uint32_t keyId = MAKE_TEST_ID(__LINE__);

    status = sss_key_object_init(&key, &pCtx->ks);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

#if SSS_HAVE_FIPS
    ENSURE_OR_GO_CLEANUP(status != kStatus_SSS_Success);
    status = kStatus_SSS_Success;
    goto cleanup;
#else

    status = sss_key_object_allocate_handle(&key,
        keyId,
        kSSS_KeyPart_Pair,
        kSSS_CipherType_RSA,
        keylen,
        kKeyObject_Mode_Persistent);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    status = sss_key_store_erase_key(&pCtx->ks, &key);
    LOG_I(
        "Delete key succeeds only if key exists, ignore error message if any");
    status = sss_key_store_generate_key(&pCtx->ks, &key, keylen * 8, 0);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    status = sss_asymmetric_context_init(
        &ctx_asymm, &pCtx->session, &key, algorithm, mode);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    LOG_I("Do Signing");
    LOG_MAU8_I("digest", digest, digestLen);

    status = sss_asymmetric_sign_digest(
        &ctx_asymm, digest, digestLen, signature, &signatureLen);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);
    LOG_I("Signing successful !!!");
    LOG_MAU8_I("signature", signature, signatureLen);

    LOG_I("Do Verification");
    status = sss_asymmetric_verify_digest(
        &ctx_asymm, digest, digestLen, signature, signatureLen);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);
    LOG_I("Verification successful !!!");
#endif
cleanup:
    if (kStatus_SSS_Success == status) {
        LOG_I("ex_sss_RSA Example Success !!!...");
    }
    else {
        LOG_E("ex_sss_RSA Example Failed !!!...");
    }
    if (ctx_asymm.session != NULL)
        sss_asymmetric_context_free(&ctx_asymm);

    return status;
}
