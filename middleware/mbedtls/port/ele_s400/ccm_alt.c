/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"

#if defined(MBEDTLS_CCM_C)

#include <string.h>

#include "mbedtls/ccm.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/platform.h"
#include "mbedtls/error.h"

#if defined(MBEDTLS_CCM_ALT)
#include "ccm_alt.h"
#include "ele_crypto.h"
#include "ele_mbedtls.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#endif

#define CCM_VALIDATE_RET(cond) \
    MBEDTLS_INTERNAL_VALIDATE_RET(cond, MBEDTLS_ERR_CCM_BAD_INPUT)
#define CCM_VALIDATE(cond) \
    MBEDTLS_INTERNAL_VALIDATE(cond)

/*
 * Initialize context
 */
void mbedtls_ccm_init(mbedtls_ccm_context *ctx)
{
    CCM_VALIDATE(ctx != NULL);
    memset(ctx, 0, sizeof(mbedtls_ccm_context));
}

/*
 * Free context
 */
void mbedtls_ccm_free(mbedtls_ccm_context *ctx)
{
    if (ctx == NULL) {
        return;
    }
    mbedtls_platform_zeroize(ctx, sizeof(mbedtls_ccm_context));
}

int mbedtls_ccm_setkey(mbedtls_ccm_context *ctx,
                       mbedtls_cipher_id_t cipher,
                       const unsigned char *key,
                       unsigned int keybits)
{
    CCM_VALIDATE_RET(ctx != NULL);
    CCM_VALIDATE_RET(key != NULL);
    /* Only AES based CCM is supported */
    CCM_VALIDATE_RET(cipher == MBEDTLS_CIPHER_ID_AES);

    switch (keybits) {
        case 128:
            ctx->keySize = 16;
            break;
        case 192:
            ctx->keySize = 24;
            break;
        case 256:
            ctx->keySize = 32;
            break;
        default:
            return MBEDTLS_ERR_CCM_BAD_INPUT;
    }

    memcpy(ctx->key, key, ctx->keySize);

    return 0;
}

int mbedtls_ccm_encrypt_and_tag(mbedtls_ccm_context *ctx, size_t length,
                                const unsigned char *iv, size_t iv_len,
                                const unsigned char *add, size_t add_len,
                                const unsigned char *input, unsigned char *output,
                                unsigned char *tag, size_t tag_len)
{
    CCM_VALIDATE_RET(ctx != NULL);
    CCM_VALIDATE_RET(iv != NULL);
    CCM_VALIDATE_RET(add_len == 0 || add != NULL);
    CCM_VALIDATE_RET(length == 0 || input != NULL);
    CCM_VALIDATE_RET(length == 0 || output != NULL);
    CCM_VALIDATE_RET(tag_len == 0 || tag != NULL);
    if (tag_len == 0) {
        return MBEDTLS_ERR_CCM_BAD_INPUT;
    }


    ele_generic_aead_t GenericAeadGCM;
    GenericAeadGCM.data     = (uint32_t) input;
    GenericAeadGCM.output   = (uint32_t) output;
    GenericAeadGCM.size     = length;
    GenericAeadGCM.key      = (uint32_t) ctx->key;
    GenericAeadGCM.key_size = ctx->keySize;
    GenericAeadGCM.iv       = (uint32_t) iv;
    GenericAeadGCM.iv_size  = iv_len;
    GenericAeadGCM.aad      = (uint32_t) add;
    GenericAeadGCM.aad_size = add_len;
    GenericAeadGCM.tag      = (uint32_t) tag;
    GenericAeadGCM.tag_size = tag_len;
    GenericAeadGCM.mode     = kEncrypt;
    GenericAeadGCM.algo     = kAES_CCM;
    int ret = MBEDTLS_ERR_CCM_HW_ACCEL_FAILED;

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    if (ELE_GenericAead(S3MU, &GenericAeadGCM) != kStatus_Success) {
        ret = MBEDTLS_ERR_CCM_HW_ACCEL_FAILED;
        goto exit;
    } else {
        ret = 0;
    }

exit:
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif
    return ret;
}

int mbedtls_ccm_auth_decrypt(mbedtls_ccm_context *ctx, size_t length,
                             const unsigned char *iv, size_t iv_len,
                             const unsigned char *add, size_t add_len,
                             const unsigned char *input, unsigned char *output,
                             const unsigned char *tag, size_t tag_len)
{
    CCM_VALIDATE_RET(ctx != NULL);
    CCM_VALIDATE_RET(iv != NULL);
    CCM_VALIDATE_RET(add_len == 0 || add != NULL);
    CCM_VALIDATE_RET(length == 0 || input != NULL);
    CCM_VALIDATE_RET(length == 0 || output != NULL);
    CCM_VALIDATE_RET(tag_len == 0 || tag != NULL);

    if (tag_len == 0) {
        return MBEDTLS_ERR_CCM_BAD_INPUT;
    }

    ele_generic_aead_t GenericAeadGCM;
    GenericAeadGCM.data     = (uint32_t) input;
    GenericAeadGCM.output   = (uint32_t) output;
    GenericAeadGCM.size     = length;
    GenericAeadGCM.key      = (uint32_t) ctx->key;
    GenericAeadGCM.key_size = ctx->keySize;
    GenericAeadGCM.iv       = (uint32_t) iv;
    GenericAeadGCM.iv_size  = iv_len;
    GenericAeadGCM.aad      = (uint32_t) add;
    GenericAeadGCM.aad_size = add_len;
    GenericAeadGCM.tag      = (uint32_t) tag;
    GenericAeadGCM.tag_size = tag_len;
    GenericAeadGCM.mode     = kDecrypt;
    GenericAeadGCM.algo     = kAES_CCM;
    int ret = MBEDTLS_ERR_CCM_HW_ACCEL_FAILED;

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    if (ELE_GenericAead(S3MU, &GenericAeadGCM) != kStatus_Success) {
        ret = MBEDTLS_ERR_CCM_HW_ACCEL_FAILED;
        goto exit;
    } else {
        ret = 0;
    }

exit:
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif
    return ret;
}

#endif /* MBEDTLS_CCM_ALT */
#endif /* MBEDTLS_CCM_C */
