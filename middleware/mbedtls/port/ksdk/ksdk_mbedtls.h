/*
 * Copyright 2017, 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef KSDK_MBEDTLS_H
#define KSDK_MBEDTLS_H

#if defined(MBEDTLS_THREADING_C)
/* Threading mutex implementations for mbedTLS. */
#include "mbedtls/threading.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

int fsl_mbedtls_printf(const char *fmt_s, ...);
status_t CRYPTO_InitHardware(void);

#if defined(MBEDTLS_THREADING_C)

/* MUTEXES FOR HW Modules*/

#if defined(FSL_FEATURE_SOC_HASHCRYPT_COUNT) && (FSL_FEATURE_SOC_HASHCRYPT_COUNT > 0)
/* MUTEX for Hashcrypt crypto module */
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_hashcrypt_mutex;
#endif /* (FSL_FEATURE_SOC_HASHCRYPT_COUNT) && (FSL_FEATURE_SOC_HASHCRYPT_COUNT > 0 */

#if defined(FSL_FEATURE_SOC_CASPER_COUNT) && (FSL_FEATURE_SOC_CASPER_COUNT > 0)
/* MUTEX for CASPER crypto module */
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_casper_mutex;
#endif /* (FSL_FEATURE_SOC_CASPER_COUNT) && (FSL_FEATURE_SOC_CASPER_COUNT > 0) */

#if defined(FSL_FEATURE_SOC_DCP_COUNT) && (FSL_FEATURE_SOC_DCP_COUNT > 0)
/* MUTEX for DCP crypto module */
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_dcp_mutex;
#endif /* (FSL_FEATURE_SOC_DCP_COUNT) && (FSL_FEATURE_SOC_DCP_COUNT > 0) */

#if defined(FSL_FEATURE_SOC_CAAM_COUNT) && (FSL_FEATURE_SOC_CAAM_COUNT > 0)
/* MUTEX for CAAM crypto module */
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_caam_mutex;
#endif /* (FSL_FEATURE_SOC_CAAM_COUNT) && (FSL_FEATURE_SOC_CAAM_COUNT > 0) */

#if defined(FSL_FEATURE_SOC_LTC_COUNT) && (FSL_FEATURE_SOC_LTC_COUNT > 0)
/* MUTEX for LTC crypto module */
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_ltc_mutex;
#endif /* (FSL_FEATURE_SOC_LTC_COUNT) && (FSL_FEATURE_SOC_LTC_COUNT > 0) */

#if defined(FSL_FEATURE_SOC_SHA_COUNT) && (FSL_FEATURE_SOC_SHA_COUNT > 0)
/* MUTEX for SHA crypto module */
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_sha_mutex;
#endif /* (FSL_FEATURE_SOC_SHA_COUNT) && (FSL_FEATURE_SOC_SHA_COUNT > 0) */

#if defined(FSL_FEATURE_SOC_AES_COUNT) && (FSL_FEATURE_SOC_AES_COUNT > 0)
/* MUTEX for AES crypto module */
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_aes_mutex;
#endif /* (FSL_FEATURE_SOC_AES_COUNT) && (FSL_FEATURE_SOC_AES_COUNT > 0) */

#if defined(FSL_FEATURE_SOC_MMCAU_COUNT) && (FSL_FEATURE_SOC_MMCAU_COUNT > 0)
/* MUTEX for MMCAU crypto module */
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_mmcau_mutex;
#endif /* (FSL_FEATURE_SOC_MMCAU_COUNT) && (FSL_FEATURE_SOC_MMCAU_COUNT > 0) */

#if defined(FSL_FEATURE_SOC_CAU3_COUNT) && (FSL_FEATURE_SOC_CAU3_COUNT > 0)
/* MUTEX for CAU3 crypto module */
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_cau3_mutex;
#endif

#endif /* defined(MBEDTLS_THREADING_C) */

#ifdef __cplusplus
}
#endif

#endif /* KSDK_MBEDTLS_H */
