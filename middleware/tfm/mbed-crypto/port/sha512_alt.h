/*
 * Copyright 2021 - 2022 NXP
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
/* ELS */
#if defined(MBEDTLS_MCUX_ELS_SHA512) && MBEDTLS_MCUX_ELS_SHA512

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
    uint32_t context[MCUXCLHASH_CONTEXT_SIZE/sizeof(uint32_t)];
}
mbedtls_sha512_context;

#endif 

#ifdef __cplusplus
}
#endif

#endif /* sha512_alt.h */
