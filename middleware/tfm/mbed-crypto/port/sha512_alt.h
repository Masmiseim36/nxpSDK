/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#ifndef MBEDTLS_SHA512_ALT_H
#define MBEDTLS_SHA512_ALT_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_MCUX_CSS_SHA512) && MBEDTLS_MCUX_CSS_SHA512 /* CSS */

#include <mcuxClHash.h>

/**
 * \brief          The SHA-512 context structure.
 *
 *                 The structure is used both for SHA-384 and for SHA-512
 *                 checksum calculations. The choice between these two is
 *                 made in the call to mbedtls_sha512_starts_ret().
 */
typedef struct mbedtls_sha512_context
{
    mcuxClHash_Context_t context;
}
mbedtls_sha512_context;

#endif 

#ifdef __cplusplus
}
#endif

#endif /* sha512_alt.h */
