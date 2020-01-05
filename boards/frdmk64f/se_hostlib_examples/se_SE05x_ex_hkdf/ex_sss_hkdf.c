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
#define HKDF_MAX_SALT 32
#define SYM_KEY_MAX 16
#define DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_INFO 192
#define KEY_BIT_LENGTH 256
/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

/* clang-format on */

static ex_sss_boot_ctx_t gex_sss_hkdf_boot_ctx;

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_hkdf_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 1
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

#include <ex_sss_main_inc.h>

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    /* HKDF Extract and Expand*/
    algorithm = kAlgorithm_SSS_HMAC_SHA256;
    mode = kMode_SSS_ComputeSharedSecret;
    uint32_t keyId = MAKE_TEST_ID(__LINE__);
    int i;
    uint32_t hmacKey_len = 18;
    uint16_t deriveDataLen = 128;
    uint8_t hkdfOutput[256] = {0};
    size_t hkdfOutputLen = sizeof(hkdfOutput);
    /* clang-format off */
    const uint8_t hmacRef[18] = { 0xDB, 0xFE, 0xE9, 0xE3, 0xB2, 0x76, 0x15, 0x4D,
        0x67, 0xF9, 0xD8, 0x4C, 0xB9, 0x35, 0x54, 0x56 };
    static uint8_t salt[HKDF_MAX_SALT] = { 0xAA, 0x1A, 0x2A, 0xE3, 0xB2, 0x76, 0x15, 0x4D,
        0x67, 0xF9, 0xD8, 0x4C, 0xB9, 0x35, 0x54, 0x56,
        0xBB, 0x1B, 0x2B, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    /* clang-format on */
    sss_object_t keyObject;
    sss_object_t derived_key;
    sss_derive_key_t ctx_derv = {0};

    LOG_I("Running HMAC Key Derivation Function Example ex_sss_hkdf.c");

    uint8_t info[DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_INFO];
    uint8_t infoLen = sizeof(info);
    for (i = 0; i < DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_INFO; i++) {
        info[i] = (uint8_t)i;
    }

    status = sss_key_object_init(&keyObject, &pCtx->ks);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    status = sss_key_object_allocate_handle(&keyObject,
        keyId,
        kSSS_KeyPart_Default,
        kSSS_CipherType_HMAC,
        KEY_BIT_LENGTH,
        kKeyObject_Mode_Persistent);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    status = sss_key_object_init(&derived_key, &pCtx->ks);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    status = sss_key_object_allocate_handle(&derived_key,
        keyId + 2,
        kSSS_KeyPart_Default,
        kSSS_CipherType_AES,
        KEY_BIT_LENGTH,
        kKeyObject_Mode_Transient);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    status = sss_key_store_set_key(
        &pCtx->ks, &keyObject, hmacRef, hmacKey_len, hmacKey_len * 8, NULL, 0);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    status = sss_derive_key_context_init(
        &ctx_derv, &pCtx->session, &keyObject, algorithm, mode);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    LOG_I("Do Key Derivation");
    LOG_MAU8_I("salt", salt, HKDF_MAX_SALT);
    LOG_MAU8_I("info", info, infoLen);

    status = sss_derive_key_go(&ctx_derv,
        salt,
        HKDF_MAX_SALT,
        info,
        infoLen,
        &derived_key,
        deriveDataLen,
        hkdfOutput,
        &hkdfOutputLen);

#if SSS_HAVE_FIPS
    ENSURE_OR_GO_CLEANUP(status != kStatus_SSS_Success);
    status = kStatus_SSS_Success;
    goto cleanup;
#else
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    LOG_I(" Key Derivation successful !!!");
    LOG_MAU8_I("hkdfOutput", hkdfOutput, hkdfOutputLen);
#endif

cleanup:
    if (kStatus_SSS_Success == status) {
        LOG_I("ex_sss_hkdf Example Success !!!...");
    }
    else {
        LOG_E("ex_sss_hkdf Example Failed !!!...");
    }

    if (ctx_derv.session != NULL)
        sss_derive_key_context_free(&ctx_derv);
    return status;
}
