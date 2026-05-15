/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* PSA crypto configuration options */
/* NXP: This file is modification of psa/crypto_config.h */
#if defined(MBEDTLS_PSA_CRYPTO_CONFIG)
/**
 * When #MBEDTLS_PSA_CRYPTO_CONFIG is enabled in mbedtls_config.h,
 * this file determines which cryptographic mechanisms are enabled
 * through the PSA Cryptography API (\c psa_xxx() functions).
 *
 * To enable a cryptographic mechanism, uncomment the definition of
 * the corresponding \c PSA_WANT_xxx preprocessor symbol.
 * To disable a cryptographic mechanism, comment out the definition of
 * the corresponding \c PSA_WANT_xxx preprocessor symbol.
 * The names of cryptographic mechanisms correspond to values
 * defined in psa/crypto_values.h, with the prefix \c PSA_WANT_ instead
 * of \c PSA_.
 *
 * Note that many cryptographic mechanisms involve two symbols: one for
 * the key type (\c PSA_WANT_KEY_TYPE_xxx) and one for the algorithm
 * (\c PSA_WANT_ALG_xxx). Mechanisms with additional parameters may involve
 * additional symbols.
 */
#else
/**
 * When \c MBEDTLS_PSA_CRYPTO_CONFIG is disabled in mbedtls_config.h,
 * this file is not used, and cryptographic mechanisms are supported
 * through the PSA API if and only if they are supported through the
 * mbedtls_xxx API.
 */
#endif
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

#ifndef MCUBOOT_PSA_CRYPTO_CONFIG_H
#define MCUBOOT_PSA_CRYPTO_CONFIG_H

#include "sblconfig.h"
#include "mcux_config.h"

#ifdef CONFIG_BOOT_USE_PSA_CRYPTO

#define PSA_WANT_ALG_SHA_256                      1
#define PSA_WANT_ALG_ECB_NO_PADDING               1
#define PSA_WANT_KEY_TYPE_AES                     

/* Currently encrypted images are not supported in MCUboot 2.2.0 */
//#define PSA_WANT_ALG_CTR                        1
//#define PSA_WANT_ALG_ECDH                       1

/* EC256 */
//#define PSA_WANT_ALG_DETERMINISTIC_ECDSA          1 //eventually enable for low quality source of entropy
#define PSA_WANT_ALG_ECDSA                         1
#define PSA_WANT_ECC_SECP_R1_256                   1
#define PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY           1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_BASIC       1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_IMPORT      1

/* Remove built-in implementations from the build  */

#define MBEDTLS_PSA_ACCEL_ALG_SHA_256              1
/* 
 * Currently it is not possible to remove EC-256 built-in implementations even 
 * partially, for more information please see mbedtls3x/docs/driver-only-builds.md 
 */
//#define MBEDTLS_PSA_ACCEL_ALG_DETERMINISTIC_ECDSA 1
//#define MBEDTLS_PSA_ACCEL_ALG_ECDSA               1
//#define MBEDTLS_PSA_ACCEL_ECC_SECP_R1_256         1
//#define MBEDTLS_PSA_ACCEL_ECC_PUBLIC_KEY          1
//#define MBEDTLS_PSA_ACCEL_ECC_KEY_PAIR_BASIC      1
//#define MBEDTLS_PSA_ACCEL_ECC_KEY_PAIR_IMPORT     1
//#define MBEDTLS_PSA_ACCEL_ECC_KEY_PAIR_EXPORT     1
//#define MBEDTLS_PSA_ACCEL_ECC_KEY_PAIR_GENERATE   1

#endif /* CONFIG_BOOT_USE_PSA_CRYPTO */

#endif /* MCUBOOT_PSA_CRYPTO_CONFIG_H */
