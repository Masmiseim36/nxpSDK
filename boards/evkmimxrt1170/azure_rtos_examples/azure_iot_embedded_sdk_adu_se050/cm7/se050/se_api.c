/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ex_sss.h"
#include "ex_sss_boot.h"
#include "fsl_sss_se05x_apis.h"
#include "se_api.h"
#include "fsl_debug_console.h"

#include "nx_crypto.h"

/* user provisioned key ID and device cert ID */
#define DEVICE_ECC_PRIVATE_KEY_ID       0x223344
#define DEVICE_CERTIFCATE_KEY_ID        0x223345

extern sss_status_t se05x_get_certificate(ex_sss_boot_ctx_t *pCtx,
                                          uint32_t certId,
                                          uint8_t *certificate_data,
                                          size_t *certLength);

extern sss_status_t se05x_ecdsa_sign(ex_sss_boot_ctx_t *pCtx,
                                     uint32_t privateKeyId,
                                     uint8_t *inputDigest,
                                     size_t inputDigestLength,
                                     uint8_t *signature,
                                     size_t *signatureLen);

extern sss_status_t se05x_ecdsa_verify(ex_sss_boot_ctx_t *pCtx,
                                       uint32_t privateKeyId,
                                       uint8_t *inputDigest,
                                       size_t inputDigestLength,
                                       uint8_t *signature,
                                       size_t signatureLen);

extern UINT se_memory_pool_init(VOID);

static ex_sss_boot_ctx_t se_ctx;
static TX_SEMAPHORE se_lock;

UINT se_setup(VOID)
{
    tx_semaphore_create(&se_lock, "se_lock", 1);

    se_memory_pool_init();

    return NX_CRYPTO_SUCCESS;
}

UINT se_open(VOID)
{
    ex_sss_boot_ctx_t *pCtx = &se_ctx;
    sss_status_t status;

    tx_semaphore_get(&se_lock, TX_WAIT_FOREVER);

    memset(pCtx, 0, sizeof(ex_sss_boot_ctx_t));

    PRINTF("ex_sss_boot_open()...\r\n");
    status = ex_sss_boot_open(pCtx, NULL);
    if (kStatus_SSS_Success != status)
    {
        PRINTF("ERR: ex_sss_session_open() failed 0x%04x\r\n", status);
        return NX_CRYPTO_NOT_SUCCESSFUL;
    }

    if (kType_SSS_SE_SE05x == pCtx->session.subsystem)
    {
        PRINTF("call ex_sss_key_store_and_object_init()...\r\n");
        status = ex_sss_key_store_and_object_init(pCtx);
        if (kStatus_SSS_Success != status) {
            PRINTF("ERR: ex_sss_key_store_and_object_init() failed 0x%04x\r\n", status);
            return NX_CRYPTO_NOT_SUCCESSFUL;
        }
    }
    else
    {
        PRINTF("ERR: SE05x device is not opened\r\n");
        return NX_CRYPTO_NOT_SUCCESSFUL;
    }

    return NX_CRYPTO_SUCCESS;
}

VOID se_close(VOID)
{
    ex_sss_boot_ctx_t *pCtx = &se_ctx;

    ex_sss_session_close(pCtx);

    memset(pCtx, 0, sizeof(ex_sss_boot_ctx_t));

    tx_semaphore_put(&se_lock);
}

UINT se_device_get_certificate(UCHAR *buf, UINT *size)
{
    ex_sss_boot_ctx_t *pCtx = &se_ctx;
    sss_status_t status;

    PRINTF("call se05x_GetCertificate()...\r\n");
    status = se05x_get_certificate(pCtx,
                                  (uint32_t)DEVICE_CERTIFCATE_KEY_ID,
                                  (uint8_t *)buf,
                                  (size_t *)size);
    if (kStatus_SSS_Success != status)
    {
        PRINTF("ERR: se05x_GetCertificate() failed 0x%04x\r\n", status);
        return NX_CRYPTO_NOT_SUCCESSFUL;
    }

    return NX_CRYPTO_SUCCESS;
}

UINT se_device_ecdsa_sign(UCHAR *hash, UINT hash_length,
                          UCHAR *signature, UINT *signature_length)
{
    ex_sss_boot_ctx_t *pCtx = &se_ctx;
    sss_status_t status;

	status = se05x_ecdsa_sign(pCtx, DEVICE_ECC_PRIVATE_KEY_ID,
                              hash, hash_length,
                              signature, signature_length);
    if (status != kStatus_SSS_Success)
    {
        return NX_NOT_SUCCESSFUL;
    }

    return NX_CRYPTO_SUCCESS;
}

UINT se_device_ecdsa_verify(UCHAR *hash, UINT hash_length,
                            UCHAR *signature, UINT signature_length)
{
    ex_sss_boot_ctx_t *pCtx = &se_ctx;
    sss_status_t status;

    status = se05x_ecdsa_verify(pCtx, DEVICE_ECC_PRIVATE_KEY_ID,
                                hash, hash_length,
                                signature, signature_length);
    if (status != kStatus_SSS_Success)
    {
        return NX_NOT_SUCCESSFUL;
    }

    return NX_CRYPTO_SUCCESS;
}
