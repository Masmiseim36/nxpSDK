/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"

#if defined(MBEDTLS_MD_C)

#include "mbedtls/md.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#if defined(MBEDTLS_HMAC_ONE_GO_ALT)
#include "ele_crypto.h"
#include "ele_mbedtls.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#endif

int mbedtls_md_hmac(const mbedtls_md_info_t *md_info,
                    const unsigned char *key, size_t keylen,
                    const unsigned char *input, size_t ilen,
                    unsigned char *output)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    uint32_t out_size = 0;

    if (md_info == NULL) {
        return MBEDTLS_ERR_MD_BAD_INPUT_DATA;
    }

    if (mbedtls_md_get_type(md_info) != MBEDTLS_MD_SHA256) {
        return MBEDTLS_ERR_MD_BAD_INPUT_DATA;
    }

    if (keylen != 32u) {
        return MBEDTLS_ERR_MD_FEATURE_UNAVAILABLE;
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

     if (ELE_GenericHmac(S3MU, (const uint8_t *)input, ilen, output, &out_size,
                         key, keylen, kELE_Hmac256) == kStatus_Success) {
        ret = 0u;
    } else {
        ret = MBEDTLS_ERR_MD_HW_ACCEL_FAILED;
    }

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    return ret;
}

#endif /* MBEDTLS_HMAC_ONE_GO_ALT */
#endif /* MBEDTLS_MD_C */
