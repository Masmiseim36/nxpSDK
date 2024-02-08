/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBEDTLS_SHA512_ALT_H
#define MBEDTLS_SHA512_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_SHA512_ALT)

#include "ele_crypto.h"

/**
 * \brief          The SHA-512 context structure.
 *
 *                 The structure is used both for SHA-256 and for SHA-224
 *                 checksum calculations. The choice between these two is
 *                 made in the call to mbedtls_sha256_starts_ret().
 */
/*! @brief ELE HASH Context structure */
typedef struct _mbedtls_sha512_context {
    ele_hash_ctx_t ele_ctx;     /*!< storage */
    int is384;                  /*!< Determines which function to use:
                                     0: Use SHA-512, or 1: Use SHA-384. */
} mbedtls_sha512_context;

#endif /* MBEDTLS_SHA512_ALT */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SHA512_ALT_H */
