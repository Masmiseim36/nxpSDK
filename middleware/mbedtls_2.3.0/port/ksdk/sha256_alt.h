/**
 * \file mbedtls_sha256.h
 *
 * \brief SHA-224 and SHA-256 cryptographic hash function
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_SHA256_ALT_H
#define MBEDTLS_SHA256_ALT_H

// Regular implementation
//

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_FREESCALE_LTC_SHA256)

/**
 * \brief          SHA-256 context structure
 */
#define mbedtls_sha256_context ltc_hash_ctx_t

#elif defined(MBEDTLS_FREESCALE_LPC_SHA256)

/**
 * \brief          SHA-256 context structure
 */
#define mbedtls_sha256_context sha_ctx_t

#endif /* MBEDTLS_FREESCALE_LPC_SHA256 */

/**
 * \brief          Initialize SHA-256 context
 *
 * \param ctx      SHA-256 context to be initialized
 */
void mbedtls_sha256_init(mbedtls_sha256_context *ctx);

/**
 * \brief          Clear SHA-256 context
 *
 * \param ctx      SHA-256 context to be cleared
 */
void mbedtls_sha256_free(mbedtls_sha256_context *ctx);

/**
 * \brief          Clone (the state of) a SHA-256 context
 *
 * \param dst      The destination context
 * \param src      The context to be cloned
 */
void mbedtls_sha256_clone(mbedtls_sha256_context *dst, const mbedtls_sha256_context *src);

/**
 * \brief          SHA-256 context setup
 *
 * \param ctx      context to be initialized
 * \param is224    0 = use SHA256, 1 = use SHA224
 */
void mbedtls_sha256_starts(mbedtls_sha256_context *ctx, int is224);

/**
 * \brief          SHA-256 process buffer
 *
 * \param ctx      SHA-256 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void mbedtls_sha256_update(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen);

/**
 * \brief          SHA-256 final digest
 *
 * \param ctx      SHA-256 context
 * \param output   SHA-224/256 checksum result
 */
void mbedtls_sha256_finish(mbedtls_sha256_context *ctx, unsigned char output[32]);

/* Internal use */
void mbedtls_sha256_process(mbedtls_sha256_context *ctx, const unsigned char data[64]);

#ifdef __cplusplus
}
#endif

#endif /* sha256_alt.h */
