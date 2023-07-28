/**
 * \file config.h
 *
 * \brief Configuration options (set of defines)
 *
 *  This set of compile-time options may be used to enable
 *  or disable features selectively, and reduce the global
 *  memory footprint.
 */
/*
 *  Copyright The Mbed TLS Contributors
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
 */

#ifndef MBEDCRYPTO_PSATEST_CONFIG_H
#define MBEDCRYPTO_PSATEST_CONFIG_H


/**
 * \name SECTION: mbed TLS feature support
 *
 * This section sets support for features that are or are not needed
 * within the modules that are enabled.
 * \{
 */


/**
 * \def MBEDTLS_NO_PLATFORM_ENTROPY
 *
 * Do not use built-in platform entropy functions.
 * This is useful if your platform does not support
 * standards like the /dev/urandom or Windows CryptoAPI.
 *
 * Uncomment this macro to disable the built-in platform entropy functions.
 */
#define MBEDTLS_NO_PLATFORM_ENTROPY

/**
 * \def MBEDTLS_PSA_CRYPTO_CONFIG
 *
 * This setting allows support for cryptographic mechanisms through the PSA
 * API to be configured separately from support through the mbedtls API.
 *
 * When this option is disabled, the PSA API exposes the cryptographic
 * mechanisms that can be implemented on top of the `mbedtls_xxx` API
 * configured with `MBEDTLS_XXX` symbols.
 *
 * When this option is enabled, the PSA API exposes the cryptographic
 * mechanisms requested by the `PSA_WANT_XXX` symbols defined in
 * include/psa/crypto_config.h. The corresponding `MBEDTLS_XXX` settings are
 * automatically enabled if required (i.e. if no PSA driver provides the
 * mechanism). You may still freely enable additional `MBEDTLS_XXX` symbols
 * in mbedtls_config.h.
 *
 * If the symbol #MBEDTLS_PSA_CRYPTO_CONFIG_FILE is defined, it specifies
 * an alternative header to include instead of include/psa/crypto_config.h.
 *
 * This feature is still experimental and is not ready for production since
 * it is not completed.
 */
#define MBEDTLS_PSA_CRYPTO_CONFIG

/* \} name SECTION: mbed TLS feature support */

/**
 * \name SECTION: mbed TLS modules
 *
 * This section enables or disables entire modules in mbed TLS
 * \{
 */


/**
 * \def MBEDTLS_CIPHER_C
 *
 * Enable the generic cipher layer.
 *
 * Module:  library/cipher.c
 *
 * Uncomment to enable generic cipher wrappers.
 */
#define MBEDTLS_CIPHER_C

/**
 * \def MBEDTLS_CTR_DRBG_C
 *
 * Enable the CTR_DRBG AES-based random generator.
 * The CTR_DRBG generator uses AES-256 by default.
 * To use AES-128 instead, enable MBEDTLS_CTR_DRBG_USE_128_BIT_KEY below.
 *
 * Module:  library/ctr_drbg.c
 * Caller:
 *
 * Requires: MBEDTLS_AES_C
 *
 * This module provides the CTR_DRBG AES random number generator.
 */
//#define MBEDTLS_CTR_DRBG_C

/**
 * \def MBEDTLS_ENTROPY_C
 *
 * Enable the platform-specific entropy code.
 *
 * Module:  library/entropy.c
 * Caller:
 *
 * Requires: MBEDTLS_SHA512_C or MBEDTLS_SHA256_C
 *
 * This module provides a generic entropy pool
 */
#define MBEDTLS_ENTROPY_C

/**
 * \def MBEDTLS_PLATFORM_C
 *
 * Enable the platform abstraction layer that allows you to re-assign
 * functions like calloc(), free(), snprintf(), printf(), fprintf(), exit().
 *
 * Enabling MBEDTLS_PLATFORM_C enables to use of MBEDTLS_PLATFORM_XXX_ALT
 * or MBEDTLS_PLATFORM_XXX_MACRO directives, allowing the functions mentioned
 * above to be specified at runtime or compile time respectively.
 *
 * \note This abstraction layer must be enabled on Windows (including MSYS2)
 * as other module rely on it for a fixed snprintf implementation.
 *
 * Module:  library/platform.c
 * Caller:  Most other .c files
 *
 * This module enables abstraction of common (libc) functions.
 */
#define MBEDTLS_PLATFORM_C


/**
 * \def MBEDTLS_PSA_CRYPTO_C
 *
 * Enable the Platform Security Architecture cryptography API.
 *
 * Module:  library/psa_crypto.c
 *
 * Requires: MBEDTLS_CTR_DRBG_C, MBEDTLS_ENTROPY_C
 *
 */
#define MBEDTLS_PSA_CRYPTO_C


/**
 * \name SECTION: Module configuration options
 *
 * This section allows for the setting of module specific sizes and
 * configuration options. The default values are already present in the
 * relevant header files and should suffice for the regular use cases.
 *
 * Our advice is to enable options and change their values here
 * only if you have a good reason and know the consequences.
 *
 * Please check the respective header file for documentation on these
 * parameters (to prevent duplicate documentation).
 * \{
 */

/* ECP options */
#define MBEDTLS_ECP_FIXED_POINT_OPTIM        0 /**< Disable fixed-point speed-up, to save ROM footprint */

/* \} name SECTION: Customisation configuration options */

#if CRYPTO_NV_SEED
#include "tfm_mbedcrypto_config_extra_nv_seed.h"
#endif /* CRYPTO_NV_SEED */

#if !defined(CRYPTO_HW_ACCELERATOR) && defined(MBEDTLS_ENTROPY_NV_SEED)
#include "mbedtls_entropy_nv_seed_config.h"
#endif

#ifdef CRYPTO_HW_ACCELERATOR
#include "mbedtls_accelerator_config.h"
#endif


#endif /* MBEDCRYPTO_PSATEST_CONFIG_H */
