/*
 *
 * Copyright 2018,2019 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <ex_sss.h>
#include <ex_sss_boot.h>
#include <fsl_sss_se05x_apis.h>
#include <nxEnsure.h>
#include <nxLog_App.h>
#include <string.h>
/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */
#define MD_LEN_BYTES 32
/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

static ex_sss_boot_ctx_t gex_sss_digest_boot_ctx;

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_digest_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 1
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

/* ************************************************************************** */
/* Include "main()" with the platform specific startup code for Plug & Trust  */
/* MW examples which will call ex_sss_entry()                                 */
/* ************************************************************************** */
#include <ex_sss_main_inc.h>

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status          = kStatus_SSS_Success;
    sss_algorithm_t algorithm    = kAlgorithm_SSS_SHA256;
    sss_mode_t mode              = kMode_SSS_Digest;
    uint8_t input[]              = "HelloWorld";
    size_t inputLen              = strlen((const char *)input);
    uint8_t digest[MD_LEN_BYTES] = {0};
    size_t digestLen             = sizeof(digest);
    sss_digest_t ctx_digest      = {0};
    /* clang-format off */
    uint8_t expectedVal[MD_LEN_BYTES] = { 0x87, 0x2e, 0x4e, 0x50, 0xce, 0x99, 0x90, 0xd8,
                                          0xb0, 0x41, 0x33, 0x0c, 0x47, 0xc9, 0xdd, 0xd1,
                                          0x1b, 0xec, 0x6b, 0x50, 0x3a, 0xe9, 0x38, 0x6a,
                                          0x99, 0xda, 0x85, 0x84, 0xe9, 0xbb, 0x12, 0xc4 };
    /* clang-format on */

    LOG_I("Running Message Digest Example ex_sss_md.c");

    status = sss_digest_context_init(&ctx_digest, &pCtx->session, algorithm, mode);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    LOG_I("Do Digest");
    LOG_MAU8_I("input", input, inputLen);
    status = sss_digest_one_go(&ctx_digest, input, inputLen, digest, &digestLen);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    if (0 != memcmp(digest, expectedVal, MD_LEN_BYTES)) {
        status = kStatus_SSS_Fail;
    }
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);
    LOG_I("Message Digest successful !!!");
    LOG_MAU8_I("digest", digest, digestLen);
cleanup:
    if (kStatus_SSS_Success == status) {
        LOG_I("ex_sss_digest Example Success !!!...");
    }
    else {
        LOG_E("ex_sss_digest Example Failed !!!...");
    }
    if (ctx_digest.session != NULL) {
        sss_digest_context_free(&ctx_digest);
    }
    return status;
}
