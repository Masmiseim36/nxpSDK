/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"

#if defined(MBEDTLS_GCM_C)

#include <string.h>

#include "mbedtls/gcm.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/platform.h"
#include "mbedtls/error.h"

#if defined(MBEDTLS_GCM_ONE_GO_ALT)
#include "ccm_alt.h"
#include "aes_alt.h"
#include "ele_crypto.h"
#include "ele_mbedtls.h"

/* Parameter validation macros */
#define GCM_VALIDATE_RET(cond) \
    MBEDTLS_INTERNAL_VALIDATE_RET(cond, MBEDTLS_ERR_GCM_BAD_INPUT)
#define GCM_VALIDATE(cond) \
    MBEDTLS_INTERNAL_VALIDATE(cond)

#if defined(MBEDTLS_GCM_ALT)
/*
 * Initialize a context
 */
void mbedtls_gcm_init(mbedtls_gcm_context *ctx)
{
    GCM_VALIDATE(ctx != NULL);
    memset(ctx, 0, sizeof(mbedtls_gcm_context));
}

/*
 * Free a context
 */
void mbedtls_gcm_free(mbedtls_gcm_context *ctx)
{
    if (ctx == NULL) {
        return;
    }
    mbedtls_platform_zeroize(ctx, sizeof(mbedtls_gcm_context));
}

/*
 * Set key
 */
int mbedtls_gcm_setkey(mbedtls_gcm_context *ctx,
                       mbedtls_cipher_id_t cipher,
                       const unsigned char *key,
                       unsigned int keybits)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    const mbedtls_cipher_info_t *cipher_info;

    GCM_VALIDATE_RET(ctx != NULL);
    GCM_VALIDATE_RET(key != NULL);
    GCM_VALIDATE_RET(keybits == 128 || keybits == 192 || keybits == 256);
    /* Only AES based CCM is supported */
    GCM_VALIDATE_RET(cipher == MBEDTLS_CIPHER_ID_AES);

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
            return MBEDTLS_ERR_GCM_BAD_INPUT;
    }

    memcpy(ctx->key, key, ctx->keySize);

    return 0;
}
#endif /* MBEDTLS_GCM_ALT */

int mbedtls_gcm_crypt_and_tag(mbedtls_gcm_context *ctx,
                              int mode,
                              size_t length,
                              const unsigned char *iv,
                              size_t iv_len,
                              const unsigned char *add,
                              size_t add_len,
                              const unsigned char *input,
                              unsigned char *output,
                              size_t tag_len,
                              unsigned char *tag)
{
    SDK_ALIGN(uint8_t key_tmp[32u], 8u) = { 0u };
    uint32_t key_size = 0u;
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    GCM_VALIDATE_RET(ctx != NULL);
    GCM_VALIDATE_RET(iv != NULL);
    GCM_VALIDATE_RET(add_len == 0 || add != NULL);
    GCM_VALIDATE_RET(length == 0 || input != NULL);
    GCM_VALIDATE_RET(length == 0 || output != NULL);
    GCM_VALIDATE_RET(tag != NULL);

    mbedtls_aes_context *aes = (mbedtls_aes_context *) ctx->cipher_ctx.cipher_ctx;

    memcpy(key_tmp, (void *) aes->key, 32u);
    key_size = aes->keySize;

    ele_generic_aead_t GenericAeadGCM = { 0u };
    GenericAeadGCM.data     = (uint32_t) input;
    GenericAeadGCM.output   = (uint32_t) output;
    GenericAeadGCM.size     = length;
    GenericAeadGCM.key      = (uint32_t) key_tmp;
    GenericAeadGCM.key_size = key_size;
    GenericAeadGCM.iv       = (uint32_t) iv;
    GenericAeadGCM.iv_size  = iv_len;
    GenericAeadGCM.aad      = (uint32_t) add;
    GenericAeadGCM.aad_size = add_len;
    GenericAeadGCM.tag      = (uint32_t) tag;
    GenericAeadGCM.tag_size = tag_len;
    GenericAeadGCM.algo     = kAES_GCM;

    if (mode == MBEDTLS_GCM_ENCRYPT) {
        GenericAeadGCM.mode = kEncrypt;
    } else if (mode == MBEDTLS_GCM_DECRYPT) {
        GenericAeadGCM.mode = kDecrypt;
    } else {
        ret = MBEDTLS_ERR_GCM_BAD_INPUT;
        memset(key_tmp, 0u, sizeof(key_tmp));
        return ret;
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
    goto cleanup;
#endif

    if (ELE_GenericAead(S3MU, &GenericAeadGCM) != kStatus_Success) {
        ret = MBEDTLS_ERR_GCM_HW_ACCEL_FAILED;
        goto clenup;
    } else {
        ret = 0u;
    }

clenup:

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    memset(key_tmp, 0u, sizeof(key_tmp));
    return ret;
}

#endif /* MBEDTLS_GCM_ONE_GO_ALT */
#endif /* MBEDTLS_GCM_C */
