/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CRYPTO_TESTS_CHECK_CONFIG_H__
#define __CRYPTO_TESTS_CHECK_CONFIG_H__

#ifdef MBEDTLS_PSA_CRYPTO_CONFIG_FILE
#include MBEDTLS_PSA_CRYPTO_CONFIG_FILE

#if defined(TFM_CRYPTO_TEST_ALG_CBC) && \
    (!defined(PSA_WANT_ALG_CBC_NO_PADDING) || !defined(PSA_WANT_ALG_CBC_PKCS7))
#error "TFM_CRYPTO_TEST_ALG_CBC enables, but CBC mode is not defined!"
#endif

#if defined(TFM_CRYPTO_TEST_ALG_CCM) && !defined(PSA_WANT_ALG_CCM)
#error "TFM_CRYPTO_TEST_ALG_CCM enables, but CCM mode is not defined!"
#endif

#if defined(TFM_CRYPTO_TEST_ALG_CFB) && !defined(PSA_WANT_ALG_CFB)
#error "TFM_CRYPTO_TEST_ALG_CFB enables, but CFB mode is not defined!"
#endif

#if defined(TFM_CRYPTO_TEST_ALG_ECB) && !defined(PSA_WANT_ALG_ECB_NO_PADDING)
#error "TFM_CRYPTO_TEST_ALG_ECB enables, but ECB mode is not defined!"
#endif

#if defined(TFM_CRYPTO_TEST_ALG_CTR) && !defined(PSA_WANT_ALG_CTR)
#error "TFM_CRYPTO_TEST_ALG_CTR enables, but CTR mode is not defined!"
#endif

#if defined(TFM_CRYPTO_TEST_ALG_OFB) && !defined(PSA_WANT_ALG_OFB)
#error "TFM_CRYPTO_TEST_ALG_OFB enables, but OFB mode is not defined!"
#endif

#if defined(TFM_CRYPTO_TEST_ALG_GCM) && !defined(PSA_WANT_ALG_GCM)
#error "TFM_CRYPTO_TEST_ALG_GCM enables, but GCM mode is not defined!"
#endif

#if defined(TFM_CRYPTO_TEST_ALG_SHA_384) && !defined(PSA_WANT_ALG_SHA_384)
#error "TFM_CRYPTO_TEST_ALG_SHA_384 enables, but SHA-384 algorithm is not defined!"
#endif

#if defined(TFM_CRYPTO_TEST_ALG_SHA_512) && !defined(PSA_WANT_ALG_SHA_512)
#error "TFM_CRYPTO_TEST_ALG_SHA_512 enables, but SHA-512 algorithm is not defined!"
#endif

#if defined(TFM_CRYPTO_TEST_HKDF) && !defined(PSA_WANT_ALG_HKDF)
#error "TFM_CRYPTO_TEST_ALG_HKDF enables, but HKDF key derivation algorithm is not defined!"
#endif

#if defined(TFM_CRYPTO_TEST_ECDH) && !defined(PSA_WANT_ALG_ECDH)
#error "TFM_CRYPTO_TEST_ALG_ECDH enables, but ECDH key agreement algorithm is not defined!"
#endif

#if defined(TFM_CRYPTO_TEST_CHACHA20) && !defined(PSA_WANT_KEY_TYPE_CHACHA20)
#error "TFM_CRYPTO_TEST_ALG_CHACHA20 enables, but ChaCha20 stream cipher algorithm is not defined!"
#endif

#if defined(TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305) && \
    !defined(PSA_WANT_ALG_CHACHA20_POLY1305)
#error "TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305 enables, but ChaCha20-Poly1305 AEAD algorithm is not defined!"
#endif

#endif /* MBEDTLS_PSA_CRYPTO_CONFIG_FILE */

#endif /* __CRYPTO_TESTS_CHECK_CONFIG_H__ */
