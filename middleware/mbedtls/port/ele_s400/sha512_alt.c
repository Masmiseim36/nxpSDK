/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"

#if defined(MBEDTLS_SHA512_C)

#include "mbedtls/sha512.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#if defined(MBEDTLS_SHA512_ALT)
#include "sha512_alt.h"
#include "ele_crypto.h"
#include "ele_mbedtls.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#endif

#define SHA512_VALIDATE_RET(cond)                           \
    MBEDTLS_INTERNAL_VALIDATE_RET(cond, MBEDTLS_ERR_SHA512_BAD_INPUT_DATA)
#define SHA512_VALIDATE(cond)  MBEDTLS_INTERNAL_VALIDATE(cond)


void mbedtls_sha512_init(mbedtls_sha512_context *ctx)
{
    SHA512_VALIDATE(ctx != NULL);

    memset(ctx, 0, sizeof(mbedtls_sha512_context));
}

void mbedtls_sha512_free(mbedtls_sha512_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    mbedtls_platform_zeroize(ctx, sizeof(mbedtls_sha512_context));
}

void mbedtls_sha512_clone(mbedtls_sha512_context *dst,
                          const mbedtls_sha512_context *src)
{
    SHA512_VALIDATE(dst != NULL);
    SHA512_VALIDATE(src != NULL);

    memcpy(dst, src, sizeof(*dst));
}

/*
 * SHA-512 context setup
 */
int mbedtls_sha512_starts_ret(mbedtls_sha512_context *ctx, int is384)
{
    SHA512_VALIDATE_RET(ctx != NULL);
    SHA512_VALIDATE_RET(is384 == 0 || is384 == 1);

    int ret = MBEDTLS_ERR_SHA512_HW_ACCEL_FAILED;

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    if (ELE_Hash_Init(S3MU, &ctx->ele_ctx, is384 ? kELE_Sha384 : kELE_Sha512) != kStatus_Success) {
        ret = MBEDTLS_ERR_SHA512_HW_ACCEL_FAILED;
    } else {
        ret = 0;
        ctx->is384 = is384;
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif
    return ret;
}

/*
 * SHA-512 process buffer
 */
int mbedtls_sha512_update_ret(mbedtls_sha512_context *ctx,
                              const unsigned char *input,
                              size_t ilen)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    SHA512_VALIDATE_RET(ctx != NULL);
    SHA512_VALIDATE_RET(ilen == 0 || input != NULL);

    if (ilen == 0) {
        return 0;
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    if (ELE_Hash_Update(S3MU, &ctx->ele_ctx, ctx->is384 ? kELE_Sha384 : kELE_Sha512,
                        (const uint8_t *) input, ilen) != kStatus_Success) {
        ret = MBEDTLS_ERR_SHA512_HW_ACCEL_FAILED;
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
 * SHA-512 final digest
 */
int mbedtls_sha512_finish_ret(mbedtls_sha512_context *ctx,
                              unsigned char output[64])
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    uint32_t out_length = 0u;

    SHA512_VALIDATE_RET(ctx != NULL);
    SHA512_VALIDATE_RET((unsigned char *) output != NULL);

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    if (ELE_Hash_Finish(S3MU, &ctx->ele_ctx, ctx->is384 ? kELE_Sha384 : kELE_Sha512,
                        output, 64u, &out_length, NULL, 0u) != kStatus_Success) {
        ret = MBEDTLS_ERR_SHA512_HW_ACCEL_FAILED;
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

int mbedtls_internal_sha512_process(mbedtls_sha512_context *ctx, const unsigned char data[128])
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    SHA512_VALIDATE_RET(ctx != NULL);
    SHA512_VALIDATE_RET((const unsigned char *) data != NULL);

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    if (ELE_Hash_Update(S3MU, &ctx->ele_ctx, ctx->is384 ? kELE_Sha384 : kELE_Sha512,
                        (const uint8_t *) data, 128) != kStatus_Success) {
        ret = MBEDTLS_ERR_SHA512_HW_ACCEL_FAILED;
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
void mbedtls_sha512_process(mbedtls_sha512_context *ctx, const unsigned char data[128])
{
    (void) mbedtls_internal_sha512_process(ctx, data);
}
#endif

#endif /* MBEDTLS_SHA512_ALT */
#endif /* MBEDTLS_SHA512_C */
