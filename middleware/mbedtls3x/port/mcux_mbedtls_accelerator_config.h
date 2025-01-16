/*
 * Copyright 2019-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MCUX_MBEDTLS_ACCELERATOR_CONFIG_H
#define MCUX_MBEDTLS_ACCELERATOR_CONFIG_H

/* Crypto HW acceleration */
#ifndef MBEDCRYPTO_MCUX_CRYPTO_HW_ACCELERATOR
#define MBEDCRYPTO_MCUX_CRYPTO_HW_ACCELERATOR \
    1 /* if 1 = enable HW acceleration, 0 = disable HW acceleration, use SW only */
#endif

#ifndef PSA_CRYPTO_DRIVER_NXP_MCU
#define PSA_CRYPTO_DRIVER_NXP_MCU
#endif

#if defined(MCUXPRESSO_SDK)

#define MCUX_MBEDTLS

#endif /* MCUXPRESSO_SDK */

#if MBEDCRYPTO_MCUX_CRYPTO_HW_ACCELERATOR

#include "fsl_device_registers.h"

/* ======== Define features per module ================================== */

/* Entropy */
#if (defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)) ||                           \
    (defined(FSL_FEATURE_SOC_RNG_COUNT) && (FSL_FEATURE_SOC_RNG_COUNT > 0)) ||                             \
    (defined(FSL_FEATURE_SOC_LPC_RNG_COUNT) && (FSL_FEATURE_SOC_LPC_RNG_COUNT > 0)) ||                     \
    (defined(FSL_FEATURE_EDGELOCK) && (FSL_FEATURE_EDGELOCK > 0)) ||                     \
    (defined(FSL_FEATURE_SOC_LPC_RNG1_COUNT) && (FSL_FEATURE_SOC_LPC_RNG1_COUNT > 0)) || (defined(ELS)) || \
    (defined(FSL_FEATURE_ELE_S4XX) && (FSL_FEATURE_ELE_S4XX > 0))

#ifndef MBEDTLS_MCUX_ENTROPY
#define MBEDTLS_MCUX_ENTROPY (1)
#endif
#endif

/*
 * For ELE_S4XX, crypto storage can be defined if NVM nased SD manager is present
 * and ITS is implemented over FATFS.
 */
#if (defined(FSL_FEATURE_ELE_S4XX) && defined(PSA_CRYPTO_DRIVER_ELE_S4XX) &&                \
     defined(PSA_ELE_S4XX_SD_NVM_MANAGER) && defined(MBEDTLS_PSA_ITS_FILE_FATFS))
#define MBEDTLS_PSA_CRYPTO_STORAGE_C
#endif

/* ======== Define ALT functions ====================================== */

/* For PSA Crypto drivers entropy is not supported currently so enable hardware alt */
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
#define MBEDTLS_PLATFORM_EXIT_MACRO(x) \
    do                                 \
    {                                  \
    } while (1) /* Default exit macro to use, can be undefined */
#endif

#if !defined(MBEDTLS_PLATFORM_PRINTF_ALT) && !defined(MBEDTLS_PLATFORM_PRINTF_MACRO)
#include "fsl_debug_console.h"
#define MBEDTLS_PLATFORM_PRINTF_MACRO PRINTF /* Default printf macro to use, can be undefined */
#endif

#if !defined(MBEDTLS_PLATFORM_FPRINTF_ALT) && !defined(MBEDTLS_PLATFORM_FPRINTF_MACRO)
#include "fsl_debug_console.h"
#define MBEDTLS_PLATFORM_FPRINTF_MACRO PRINTF /* Default printf macro to use, can be undefined */
#endif

#endif /* MCUX_MBEDTLS_ACCELERATOR_CONFIG_H */
