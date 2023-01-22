/*
 * Copyright 2019-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBEDCRYPTO_MCUX_HW_CONFIG_H
#define MBEDCRYPTO_MCUX_HW_CONFIG_H

/* Crypto HW acceleration */
#ifndef MBEDCRYPTO_MCUX_CRYPTO_HW_ACCELERATOR
#define MBEDCRYPTO_MCUX_CRYPTO_HW_ACCELERATOR   1 /* if 1 = enable HW acceleration, 0 = disable HW acceleration, use SW only */ 
#endif

#if MBEDCRYPTO_MCUX_CRYPTO_HW_ACCELERATOR

#include "fsl_device_registers.h"

/* ======== Define features per module ================================== */ 

/* Enable HASHCRYPT use in library if there is HASHCRYPT on chip. */
#if defined(FSL_FEATURE_SOC_HASHCRYPT_COUNT) && (FSL_FEATURE_SOC_HASHCRYPT_COUNT > 0)
    #include "fsl_hashcrypt.h"

    #ifndef MBEDTLS_MCUX_HASHCRYPT_AES
        #define MBEDTLS_MCUX_HASHCRYPT_AES     (1)     /* Enable use of HASHCRYPT AES.*/
    #endif

    #ifndef MBEDTLS_MCUX_HASHCRYPT_SHA        
        #define MBEDTLS_MCUX_HASHCRYPT_SHA     (1)    /* Enable use of HASHCRYPT SHA */
    #endif
    
    #if (MBEDTLS_MCUX_HASHCRYPT_SHA == 1)
    /* Hashcrypt without context switch is not able to calculate SHA in parallel with AES.
     * HW acceleration of SHA is disabled by default in MbedTLS integration.
     * HW acceleration of SHA is enabled on chip with context switch.
     */
        #if defined(FSL_FEATURE_HASHCRYPT_HAS_RELOAD_FEATURE)
        #define MBEDTLS_MCUX_HASHCRYPT_SHA1   /* Enable use of HASHCRYPT SHA1.*/
        #define MBEDTLS_MCUX_HASHCRYPT_SHA256 /* Enable use of HASHCRYPT SHA256.*/
        #endif
    #endif
#endif /* FSL_FEATURE_SOC_HASHCRYPT_COUNT */

/* Enable CASPER use in library if there is CASPER on chip. */
#if defined(FSL_FEATURE_SOC_CASPER_COUNT) && (FSL_FEATURE_SOC_CASPER_COUNT > 0)
    #include "fsl_casper.h"

	#ifndef MBEDTLS_MCUX_CASPER_PKHA
		#define MBEDTLS_MCUX_CASPER_PKHA    (1) /* Enable use of CASPER PKHA.*/
		#define MBEDTLS_MCUX_PKHA_INT_MAX_BYTES (512)
	#endif

	/* CASPER hw acceleration supports limited number of curves (SECP256R1, SECP384R1, SECP521R1) */
	#if (!(defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED) || \
			defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED) || \
			defined(MBEDTLS_ECP_DP_SECP192K1_ENABLED) || \
			defined(MBEDTLS_ECP_DP_SECP224K1_ENABLED) || \
			defined(MBEDTLS_ECP_DP_SECP256K1_ENABLED) || \
			defined(MBEDTLS_ECP_DP_BP256R1_ENABLED) || \
			defined(MBEDTLS_ECP_DP_BP384R1_ENABLED) || \
			defined(MBEDTLS_ECP_DP_BP512R1_ENABLED) || \
			defined(MBEDTLS_ECP_DP_CURVE25519_ENABLED) || \
			defined(MBEDTLS_ECP_DP_CURVE448_ENABLED) ) ) /* Check driver support */

        #ifndef MBEDTLS_MCUX_CASPER_ECC         
            #define MBEDTLS_MCUX_CASPER_ECC     (1)         /* Enable use of CASPER ECC */
        #endif

    #endif

#endif /* FSL_FEATURE_SOC_CASPER_COUNT */

/* Enable CSS */
#ifdef CSS
    #ifndef MBEDTLS_MCUX_CSS_AES
        #define MBEDTLS_MCUX_CSS_AES    (1)     /* Enable use of CSS AES.*/
    #endif

    #ifndef MBEDTLS_MCUX_CSS_SHA256
        #define MBEDTLS_MCUX_CSS_SHA256 (1)     /* Enable use of CSS SHA256.*/
    #endif

    #ifndef MBEDTLS_MCUX_CSS_SHA512
        #define MBEDTLS_MCUX_CSS_SHA512 (1)     /* Enable use of CSS SHA512.*/
    #endif
#endif /* CSS */

/* Enable PKC */
#ifdef PKC
    #ifndef MBEDTLS_MCUX_PKC
        #define MBEDTLS_MCUX_PKC        (1)     /* Enable use of PKC. */
    #endif
    
    #ifndef MBEDTLS_MCUX_PKC_ECDH
        #define MBEDTLS_MCUX_PKC_ECDH   (1)     /* Enable use of PKC ECDH.*/
    #endif
    
    #ifndef MBEDTLS_MCUX_PKC_ECDSA
        #define MBEDTLS_MCUX_PKC_ECDSA  (1)     /* Enable use of PKC ECDSA.*/
    #endif
    
    #ifndef MBEDTLS_MCUX_PKC_RSA
        #define MBEDTLS_MCUX_PKC_RSA    (1)     /* Enable use of PKC RSA.*/
    #endif
#endif /* PKC */

/* Entropy */
#if (defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)) || \
    (defined(FSL_FEATURE_SOC_RNG_COUNT) && (FSL_FEATURE_SOC_RNG_COUNT > 0)) || \
    (defined(FSL_FEATURE_SOC_LPC_RNG_COUNT) && (FSL_FEATURE_SOC_LPC_RNG_COUNT > 0)) || \
    (defined(FSL_FEATURE_SOC_LPC_RNG1_COUNT) && (FSL_FEATURE_SOC_LPC_RNG1_COUNT > 0)) || \
    (defined(CSS))

    #ifndef MBEDTLS_MCUX_ENTROPY  
        #define MBEDTLS_MCUX_ENTROPY    (1)
    #endif
 #endif

/* ======== Define ALT functions ====================================== */

#if defined(MBEDTLS_MCUX_HASHCRYPT_AES) && MBEDTLS_MCUX_HASHCRYPT_AES
    #define MBEDTLS_AES_ALT
    #define MBEDTLS_AES_SETKEY_ENC_ALT
    #define MBEDTLS_AES_SETKEY_DEC_ALT
    #define MBEDTLS_AES_ENCRYPT_ALT
    #define MBEDTLS_AES_DECRYPT_ALT
    #define MBEDTLS_AES_CRYPT_CBC_ALT
    #define MBEDTLS_AES_CRYPT_CTR_ALT
#endif
#if defined(MBEDTLS_MCUX_HASHCRYPT_SHA1)
    #define MBEDTLS_SHA1_ALT
#endif
#if defined(MBEDTLS_MCUX_HASHCRYPT_SHA256)
    #define MBEDTLS_SHA256_ALT
#endif

/*
 * LPC SHA module does not support SHA-224.
 *
 * Since mbed TLS does not provide separate APIs for SHA-224 and SHA-256
 * and SHA-224 is not widely used, this implementation provides HW accelerated SHA-256 only
 * and SHA-224 is not available at all (calls will fail).
 *
 * To use SHA-224 on LPC, do not define MBEDTLS_SHA256_ALT and both SHA-224 and SHA-256 will use
 * original mbed TLS software implementation.
 */
#if defined(MBEDTLS_MCUX_HASHCRYPT_SHA256) 
    #define MBEDTLS_SHA256_ALT_NO_224
#endif

#if defined(MBEDTLS_MCUX_CASPER_PKHA) && (MBEDTLS_MCUX_CASPER_PKHA == 1)
    #define MBEDTLS_RSA_PUBLIC_ALT
#endif

#if defined(MBEDTLS_MCUX_CASPER_ECC) && (MBEDTLS_MCUX_CASPER_ECC == 1)
    #define MBEDTLS_ECP_MUL_COMB_ALT    /* Alternate implementation of ecp_mul_comb() */
    #define MBEDTLS_ECP_MULADD_ALT      /* Alternate implementation of mbedtls_ecp_muladd() */
#endif

#if defined(MBEDTLS_MCUX_CSS_AES) && MBEDTLS_MCUX_CSS_AES
    #define MBEDTLS_AES_ALT
    #define MBEDTLS_AES_SETKEY_ENC_ALT
    #define MBEDTLS_AES_SETKEY_DEC_ALT
    #define MBEDTLS_AES_ENCRYPT_ALT
    #define MBEDTLS_AES_DECRYPT_ALT
#endif

#if defined(MBEDTLS_MCUX_CSS_SHA256) && MBEDTLS_MCUX_CSS_SHA256
    #define MBEDTLS_SHA256_ALT
#endif

#if defined(MBEDTLS_MCUX_CSS_SHA512) && MBEDTLS_MCUX_CSS_SHA512
    #define MBEDTLS_SHA512_ALT
#endif

#if defined(MBEDTLS_MCUX_PKC) && MBEDTLS_MCUX_PKC && defined(MBEDTLS_MCUX_PKC_ECDH) && MBEDTLS_MCUX_PKC_ECDH
    #define MBEDTLS_ECDH_GEN_PUBLIC_ALT
    #define MBEDTLS_ECDH_COMPUTE_SHARED_ALT
    #define MBEDTLS_ECDH_CANDO_ALT
#endif

#if defined(MBEDTLS_MCUX_PKC) && MBEDTLS_MCUX_PKC && defined(MBEDTLS_MCUX_PKC_ECDSA) && MBEDTLS_MCUX_PKC_ECDSA
    #define MBEDTLS_ECDSA_SIGN_ALT
    #define MBEDTLS_ECDSA_VERIFY_ALT
    #define MBEDTLS_ECDSA_GENKEY_ALT
#endif

#if defined(MBEDTLS_MCUX_PKC) && MBEDTLS_MCUX_PKC && defined(MBEDTLS_MCUX_PKC_RSA) && MBEDTLS_MCUX_PKC_RSA
    #define MBEDTLS_RSA_PUBLIC_ALT
    #define MBEDTLS_RSA_PRIVATE_ALT
#endif

#if defined(MBEDTLS_MCUX_ENTROPY) && (MBEDTLS_MCUX_ENTROPY == 1)
	#ifdef MBEDTLS_ENTROPY_NV_SEED
    #undef MBEDTLS_ENTROPY_NV_SEED
    #endif
	#ifdef MBEDTLS_TEST_NULL_ENTROPY
	#undef MBEDTLS_TEST_NULL_ENTROPY
	#endif
	#ifdef MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
	#undef MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
	#endif

	#define MBEDTLS_ENTROPY_HARDWARE_ALT /* Use own hardware entropy collector */
#endif

#endif /* MBEDCRYPTO_MCUX_CRYPTO_HW_ACCELERATOR */


/* ========= Application-specific paltform macros ======================*/
#if !defined(MBEDTLS_PLATFORM_EXIT_MACRO)
    #define MBEDTLS_PLATFORM_EXIT_MACRO(x)          do{}while(1)    /* Default exit macro to use, can be undefined */
#endif
    
#if !defined(MBEDTLS_PLATFORM_PRINTF_ALT) && !defined(MBEDTLS_PLATFORM_PRINTF_MACRO)
    #include "fsl_debug_console.h"    
    #define MBEDTLS_PLATFORM_PRINTF_MACRO           PRINTF          /* Default printf macro to use, can be undefined */
#endif

#endif /* MBEDCRYPTO_MCUX_HW_CONFIG_H */
