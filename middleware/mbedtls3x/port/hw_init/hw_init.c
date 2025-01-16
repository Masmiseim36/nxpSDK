/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mbedtls/build_info.h"
#include "hw_init.h"

#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
#include "fsl_trng.h"
#elif defined(FSL_FEATURE_SOC_RNG_COUNT) && (FSL_FEATURE_SOC_RNG_COUNT > 0)
#include "fsl_rnga.h"
#elif defined(FSL_FEATURE_SOC_LPC_RNG_COUNT) && (FSL_FEATURE_SOC_LPC_RNG_COUNT > 0)
#include "fsl_rng.h"
#elif defined(FSL_FEATURE_SOC_LPC_RNG1_COUNT) && (FSL_FEATURE_SOC_LPC_RNG1_COUNT > 0)
#include "fsl_rng.h"
#elif defined(FSL_FEATURE_EDGELOCK) && (FSL_FEATURE_EDGELOCK > 0)
#include "sss_crypto.h"
#endif

/******************************************************************************/
/******************** static functions **************************************/
/******************************************************************************/
static void mbedtls_mcux_rng_init(void)
{
#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
#if defined(TRNG)
#define TRNG0 TRNG
#endif
    trng_config_t trngConfig;

    TRNG_GetDefaultConfig(&trngConfig);
    /* Set sample mode of the TRNG ring oscillator to Von Neumann, for better random data.*/
    /* Initialize TRNG */
    TRNG_Init(TRNG0, &trngConfig);
#elif defined(FSL_FEATURE_SOC_RNG_COUNT) && (FSL_FEATURE_SOC_RNG_COUNT > 0)
    RNGA_Init(RNG);
    RNGA_Seed(RNG, SIM->UIDL);
#elif defined(FSL_FEATURE_SOC_LPC_RNG1_COUNT) && (FSL_FEATURE_SOC_LPC_RNG1_COUNT > 0)
    RNG_Init(RNG);
#endif
}

uint32_t g_isCryptoHWInitialized = ELS_PKC_CRYPTOHW_UNINITIALIZED;

/******************************************************************************/
/******************** CRYPTO_InitHardware **************************************/
/******************************************************************************/

/*!
 * @brief Application init for Crypto blocks.
 *
 * This function is provided to be called by MCUXpresso SDK applications.
 * It is used to initialized Crypto Hw and/or Hw entropy modules.
 * Function is expected to be called in single thread context only.
 */
status_t CRYPTO_InitHardware(void)
{
    if (g_isCryptoHWInitialized != ELS_PKC_CRYPTOHW_INITIALIZED) 
    {
      
        /* Current sdk examples using this file, does not have any HW crypto
        module supported that can be initialized. But in future, if any becomes
        availbale, it can be initialized here*/

        /* Initialize the trng*/
        mbedtls_mcux_rng_init();
        
        /* Set the global init flag*/
        g_isCryptoHWInitialized = ELS_PKC_CRYPTOHW_INITIALIZED;
    }
    return kStatus_Success;
}


