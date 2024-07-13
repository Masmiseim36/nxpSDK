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

/* HW accel on RT1180 currently doesn't work with enabled cache.
   Disabling cache degrades the performance even more */

#undef MBEDTLS_AES_ALT
#undef MBEDTLS_CCM_ALT

#undef MBEDTLS_PKCS1_V15_ALT
#undef MBEDTLS_PKCS1_V21_ALT

#undef MBEDTLS_SHA256_ALT
#undef MBEDTLS_SHA512_ALT

#define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
#define MBEDTLS_ECDH_C
#define MBEDTLS_ECDSA_C
#define MBEDTLS_ECP_C

#endif /* MBEDTLS_USER_CONFIG_H */
