/*
 * Copyright 2017, 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef KSDK_MBEDTLS_H
#define KSDK_MBEDTLS_H

#ifdef __cplusplus
extern "C" {
#endif

int fsl_mbedtls_printf(const char *fmt_s, ...);
status_t CRYPTO_InitHardware(void);

#if defined(MBEDTLS_THREADING_C)

/* MUTEX FOR HW Modules*/
#if defined(FSL_FEATURE_SOC_HASHCRYPT_COUNT) && (FSL_FEATURE_SOC_HASHCRYPT_COUNT > 0)
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_hashcrypt_mutex;
#endif /* (FSL_FEATURE_SOC_HASHCRYPT_COUNT) && (FSL_FEATURE_SOC_HASHCRYPT_COUNT > 0 */

#if defined(FSL_FEATURE_SOC_CASPER_COUNT) && (FSL_FEATURE_SOC_CASPER_COUNT > 0)
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_casper_mutex;
#endif /* (FSL_FEATURE_SOC_CASPER_COUNT) && (FSL_FEATURE_SOC_CASPER_COUNT > 0) */

#endif /* defined(MBEDTLS_THREADING_C) */

#ifdef __cplusplus
}
#endif

#endif /* KSDK_MBEDTLS_H */
