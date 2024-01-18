/*
 * Copyright 2019 - 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBEDTLS_SHA256_ALT_H
#define MBEDTLS_SHA256_ALT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          The SHA-256 context structure.
 *
 *                 The structure is used both for SHA-256 and for SHA-224
 *                 checksum calculations. The choice between these two is
 *                 made in the call to mbedtls_sha256_starts_ret().
 */
#if defined(MBEDTLS_MCUX_HASHCRYPT_SHA256) /* Hashcrypt */

#define mbedtls_sha256_context hashcrypt_hash_ctx_t

/* ELS */
#elif defined(MBEDTLS_MCUX_ELS_SHA256) && MBEDTLS_MCUX_ELS_SHA256

#include <mcuxClHash.h>
typedef struct mbedtls_sha256_context
{
    uint32_t context[MCUXCLHASH_CONTEXT_SIZE/sizeof(uint32_t)];
}
mbedtls_sha256_context;

#endif 

#ifdef __cplusplus
}
#endif

#endif /* sha256_alt.h */
