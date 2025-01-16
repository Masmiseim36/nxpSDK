/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBEDTLS_USER_CONFIG_H
#define MBEDTLS_USER_CONFIG_H

#define MBEDTLS_PLATFORM_MEMORY
#define MBEDTLS_PLATFORM_STD_CALLOC pvPortCalloc
#define MBEDTLS_PLATFORM_STD_FREE   vPortFree

#undef MBEDTLS_DEBUG

#undef MBEDTLS_SSL_MAX_CONTENT_LEN
#define MBEDTLS_SSL_MAX_CONTENT_LEN (1024 * 16)

#undef MBEDTLS_FREESCALE_FREERTOS_CALLOC_ALT

// on some RT targets, mainly RT10xx, the following is needed to specify the ciphersuit manually
// to avoid incompatible setup that is not supported by HW acceleration

#if defined(MBEDTLS_FREESCALE_DCP_AES) && defined(MBEDTLS_AES_ALT_NO_256)
#define MBEDTLS_SSL_CIPHERSUITES                                                                    \
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256, MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256, \
        MBEDTLS_TLS_RSA_WITH_AES_128_GCM_SHA256
#endif

#endif /* MBEDTLS_USER_CONFIG_H */
