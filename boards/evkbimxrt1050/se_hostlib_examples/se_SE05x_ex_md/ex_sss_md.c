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
#include <string.h>
/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */
#define MD_LEN 1
#define MD_LEN_BYTES 20
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

#include <ex_sss_main_inc.h>

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status = kStatus_SSS_Success;
    sss_algorithm_t algorithm = kAlgorithm_SSS_SHA1;
    sss_mode_t mode = kMode_SSS_Digest;
    uint8_t input[] = "HelloWorld";
    size_t inputLen = strlen((const char *)input);
    uint8_t digest[MD_LEN_BYTES] = {0};
    size_t digestLen = sizeof(digest);
    sss_digest_t ctx_digest = {0};
    /* clang-format off */
    uint8_t expectedVal[MD_LEN_BYTES] = { 0xdb, 0x8a, 0xc1, 0xc2, 0x59, 0xeb, 0x89, 0xd4,
                                          0xa1, 0x31, 0xb2, 0x53, 0xba, 0xcf, 0xca, 0x5f,
                                          0x31, 0x9d, 0x54, 0xf2 };
    /* clang-format on */

    LOG_I("Running Message Digest Example ex_sss_md.c");

    status =
        sss_digest_context_init(&ctx_digest, &pCtx->session, algorithm, mode);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    LOG_I("Do Digest");
    LOG_MAU8_I("input", input, inputLen);
    status =
        sss_digest_one_go(&ctx_digest, input, inputLen, digest, &digestLen);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    if (0 != memcmp(digest, expectedVal, sizeof(MD_LEN_BYTES))) {
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
    if (ctx_digest.session != NULL)
        sss_digest_context_free(&ctx_digest);
    return status;
}
