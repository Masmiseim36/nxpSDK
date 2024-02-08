/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"

#if defined(MBEDTLS_SHA256_C)

#include "mbedtls/sha256.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#if defined(MBEDTLS_SHA256_ALT)
#include "sha256_alt.h"
#include "ele_crypto.h"
#include "ele_mbedtls.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#endif

#define SHA256_VALIDATE_RET(cond)                           \
    MBEDTLS_INTERNAL_VALIDATE_RET(cond, MBEDTLS_ERR_SHA256_BAD_INPUT_DATA)
#define SHA256_VALIDATE(cond)  MBEDTLS_INTERNAL_VALIDATE(cond)


void mbedtls_sha256_init(mbedtls_sha256_context *ctx)
{
    SHA256_VALIDATE(ctx != NULL);

    memset(ctx, 0, sizeof(mbedtls_sha256_context));
}

void mbedtls_sha256_free(mbedtls_sha256_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    mbedtls_platform_zeroize(ctx, sizeof(mbedtls_sha256_context));
}

void mbedtls_sha256_clone(mbedtls_sha256_context *dst,
                          const mbedtls_sha256_context *src)
{
    SHA256_VALIDATE(dst != NULL);
    SHA256_VALIDATE(src != NULL);

    memcpy(dst, src, sizeof(*dst));
}

/*
 * SHA-256 context setup
 */
int mbedtls_sha256_starts_ret(mbedtls_sha256_context *ctx, int is224)
{
    SHA256_VALIDATE_RET(ctx != NULL);
    SHA256_VALIDATE_RET(is224 == 0 || is224 == 1);
    int ret = MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED;

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    if (ELE_Hash_Init(S3MU, &ctx->ele_ctx, is224 ? kELE_Sha224 : kELE_Sha256) != kStatus_Success) {
        ret = MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED;
    } else {
        ret = 0;
        ctx->is224 = is224;
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif
    return ret;
}

/*
 * SHA-256 process buffer
 */
int mbedtls_sha256_update_ret(mbedtls_sha256_context *ctx,
                              const unsigned char *input,
                              size_t ilen)
{
    int ret = MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED;

    SHA256_VALIDATE_RET(ctx != NULL);
    SHA256_VALIDATE_RET(ilen == 0 || input != NULL);

    if (ilen == 0) {
        return 0;
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    if (ELE_Hash_Update(S3MU, &ctx->ele_ctx, ctx->is224 ? kELE_Sha224 : kELE_Sha256,
                        (const uint8_t *) input, ilen) != kStatus_Success) {
        ret = MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED;
    } else {
        ret = 0;
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif
    return ret;
}

/*
 * SHA-256 final digest
 */
int mbedtls_sha256_finish_ret(mbedtls_sha256_context *ctx,
                              unsigned char output[32])
{
    int ret = MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED;
    uint32_t out_length = 0u;

    SHA256_VALIDATE_RET(ctx != NULL);
    SHA256_VALIDATE_RET((unsigned char *) output != NULL);

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    if (ELE_Hash_Finish(S3MU, &ctx->ele_ctx, ctx->is224 ? kELE_Sha224 : kELE_Sha256,
                        output, 32u, &out_length, NULL, 0u) != kStatus_Success) {
        ret = MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED;
    } else {
        ret = 0;
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif
    return ret;
}

int mbedtls_internal_sha256_process(mbedtls_sha256_context *ctx, const unsigned char data[64])
{
    int ret = MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED;

    SHA256_VALIDATE_RET(ctx != NULL);
    SHA256_VALIDATE_RET((const unsigned char *) data != NULL);

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    if (ELE_Hash_Update(S3MU, &ctx->ele_ctx, ctx->is224 ? kELE_Sha224 : kELE_Sha256,
                        (const uint8_t *) data, 64) != kStatus_Success) {
        ret = MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED;
    } else {
        ret = 0;
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif
    return ret;
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha256_process(mbedtls_sha256_context *ctx, const unsigned char data[64])
{
    (void) mbedtls_internal_sha256_process(ctx, data);
}
#endif

#endif /* MBEDTLS_SHA256_ALT */
#endif /* MBEDTLS_SHA256_C */
