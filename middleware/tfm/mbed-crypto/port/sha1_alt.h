/*
 * Copyright 2019 - 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef MBEDTLS_SHA1_ALT_H
#define MBEDTLS_SHA1_ALT_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_FREESCALE_LTC_SHA1)

/**
 * \brief          SHA-1 context structure
 */
#define mbedtls_sha1_context ltc_hash_ctx_t

#elif defined(MBEDTLS_FREESCALE_LPC_SHA1)

/**
 * \brief          SHA-1 context structure
 */
#define mbedtls_sha1_context sha_ctx_t

#elif defined(MBEDTLS_FREESCALE_CAAM_SHA1)

/**
 * \brief          SHA-1 context structure
 */
#define mbedtls_sha1_context caam_hash_ctx_t

#elif defined(MBEDTLS_FREESCALE_CAU3_SHA1)

/**
 * \brief          SHA-1 context structure
 */
#define mbedtls_sha1_context cau3_hash_ctx_t

#elif defined(MBEDTLS_FREESCALE_DCP_SHA1)

/**
 * \brief          SHA-1 context structure
 */
#define mbedtls_sha1_context dcp_hash_ctx_t

#elif defined(MBEDTLS_MCUX_HASHCRYPT_SHA1)

/**
 * \brief          SHA-1 context structure
 */
#define mbedtls_sha1_context hashcrypt_hash_ctx_t

#endif /* MBEDTLS_FREESCALE_LTC_SHA1 */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SHA1_ALT_H */
