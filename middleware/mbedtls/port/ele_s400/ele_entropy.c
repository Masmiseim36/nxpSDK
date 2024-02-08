/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif


#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT)

#include "ele_crypto.h"
#include "ele_mbedtls.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#endif

extern ele_ctx_t g_ele_ctx; /* Global context */

/* Entropy poll callback for a hardware source */
int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    status_t result = kStatus_Success;

#if defined(MBEDTLS_THREADING_C)
    if ((result = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return result;
    }
#endif

    result = ELE_RngGetRandom(S3MU, (uint32_t *) output, len);

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif

    if (result == kStatus_Success) {
        *olen = len;
        return 0;
    } else {
        return result;
    }
}

#endif /* MBEDTLS_ENTROPY_HARDWARE_ALT */
