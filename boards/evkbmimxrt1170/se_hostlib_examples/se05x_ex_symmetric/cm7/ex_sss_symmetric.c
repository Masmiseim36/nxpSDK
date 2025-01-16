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

#ifndef SIMW_DEMO_ENABLE__DEMO_EX_SYMMETRIC
#include "UWBIOT_APP_BUILD.h"
#endif

#if defined(SIMW_DEMO_ENABLE__DEMO_EX_SYMMETRIC)
/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */
#define KEY_BIT_LEN 128
/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

static ex_sss_boot_ctx_t gex_sss_symmetric_boot_ctx;

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_symmetric_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 1
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

/* ************************************************************************** */
/* Include "main()" with the platform specific startup code for Plug & Trust  */
/* MW examples which will call ex_sss_entry()                                 */
/* ************************************************************************** */
#include <ex_sss_main_inc.h>

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status = kStatus_SSS_Success;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    /* clang-format off */
    uint8_t srcData[16] = { 0x48 ,0x45 ,0x4c ,0x4c ,0x4f ,0x48 ,0x45 ,0x4c ,0x4c ,0x4f ,0x48 ,0x45 ,0x4c ,0x4c ,0x4f ,0x31 }; /*HELLOHELLOHELLO1*/
    uint8_t keystring[KEY_BIT_LEN / 8] = { 0x48 ,0x45 ,0x4c ,0x4c ,0x4f ,0x48 ,0x45 ,0x4c ,0x4c ,0x4f ,0x48 ,0x45 ,0x4c ,0x4c ,0x4f ,0x31 }; /*HELLOHELLOHELLO1*/
    uint8_t expectedData[16] = { 0x32, 0xA6, 0x04, 0x88, 0xC5, 0xB3, 0xFF, 0x40, 0x50, 0xAF, 0x56, 0xA5, 0x68, 0xAE, 0xD1, 0x05};
    uint8_t destData[16] = {0,};
    size_t destDataLen = sizeof(destData);
    uint8_t iv[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    size_t ivlen = sizeof(iv);
    uint32_t keyId = MAKE_TEST_ID(__LINE__);
    sss_key_part_t keyPart;
    sss_cipher_type_t cipherType;
    size_t keyByteLenMax = KEY_BIT_LEN/8;
    sss_object_t key = { 0 };
    sss_symmetric_t ctx_symm_encrypt = { 0 };
    sss_symmetric_t ctx_symm_decrypt = { 0 };

    /* clang-format on */

    algorithm  = kAlgorithm_SSS_AES_CBC;
    keyPart    = kSSS_KeyPart_Default;
    cipherType = kSSS_CipherType_AES;
    mode       = kMode_SSS_Encrypt;

    LOG_I("Running AES symmetric Example ex_sss_symmetric.c");
    /* doc:start ex_sss_symmetric-allocate-key */
    /* Pre-requisite for encryption Part*/
    status = sss_key_object_init(&key, &pCtx->ks);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    status =
        sss_key_object_allocate_handle(&key, keyId, keyPart, cipherType, keyByteLenMax, kKeyObject_Mode_Persistent);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    status = sss_key_store_set_key(&pCtx->ks, &key, keystring, sizeof(keystring), sizeof(keystring) * 8, NULL, 0);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);
    /* doc:end ex_sss_symmetric-allocate-key */

    /* doc:start ex_sss_symmetric-encrypt */
    status = sss_symmetric_context_init(&ctx_symm_encrypt, &pCtx->session, &key, algorithm, mode);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    LOG_I("Do Encryption");
    LOG_MAU8_I("iv", iv, ivlen);
    LOG_MAU8_I("srcData", srcData, ivlen);
    /*Do Encryption*/
    status = sss_cipher_one_go(&ctx_symm_encrypt, iv, ivlen, srcData, destData, destDataLen);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);
    /* doc:end ex_sss_symmetric-encrypt */

    if (0 != memcmp(destData, expectedData, sizeof(expectedData))) {
        status = kStatus_SSS_Fail;
    }
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    LOG_I("Encryption successful !!!");
    LOG_MAU8_I("encrypted data", destData, destDataLen);

    algorithm  = kAlgorithm_SSS_AES_CBC;
    keyPart    = kSSS_KeyPart_Default;
    cipherType = kSSS_CipherType_AES;
    mode       = kMode_SSS_Decrypt;

    /* doc:start ex_sss_symmetric-decrypt */
    status = sss_symmetric_context_init(&ctx_symm_decrypt, &pCtx->session, &key, algorithm, mode);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    LOG_I("Do Decryption");
    LOG_MAU8_I("iv", iv, ivlen);
    LOG_MAU8_I("Encrypted data", destData, destDataLen);
    status = sss_cipher_one_go(&ctx_symm_decrypt, iv, ivlen, destData, srcData, destDataLen);
    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);
    /* doc:end ex_sss_symmetric-decrypt */

    if (0 != memcmp(destData, expectedData, sizeof(expectedData))) {
        status = kStatus_SSS_Fail;
    }

    ENSURE_OR_GO_CLEANUP(status == kStatus_SSS_Success);

    LOG_I("Decryption successful !!!");
    LOG_MAU8_I("decrypted data", srcData, destDataLen);

cleanup:
    if (kStatus_SSS_Success == status) {
        LOG_I("ex_sss_symmetric Example Success !!!...");
    }
    else {
        LOG_E("ex_sss_symmetric Example Failed !!!...");
    }
    if (ctx_symm_encrypt.session != NULL) {
        sss_symmetric_context_free(&ctx_symm_encrypt);
    }
    if (ctx_symm_decrypt.session != NULL) {
        sss_symmetric_context_free(&ctx_symm_decrypt);
    }
    sss_key_object_free(&key);
    return status;
}

#endif // SIMW_DEMO_ENABLE__DEMO_EX_SYMMETRIC
