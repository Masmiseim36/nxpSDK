/*
 * Copyright 2019 - 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mbedtls/build_info.h"

#if defined(MBEDTLS_MCUX_ENTROPY) && (MBEDTLS_MCUX_ENTROPY == 1)

#include "fsl_common.h"

    /* This check here is required, because same function name exisit for KWxx platforms,
    that is using same function to do its HW init with same function name.*/
#if (( defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0))      || \
    ( defined(FSL_FEATURE_SOC_RNG_COUNT) && (FSL_FEATURE_SOC_RNG_COUNT > 0))         || \
    ( defined(FSL_FEATURE_SOC_LPC_RNG_COUNT) && (FSL_FEATURE_SOC_LPC_RNG_COUNT > 0)) || \
    ( defined(FSL_FEATURE_SOC_LPC_RNG1_COUNT) && (FSL_FEATURE_SOC_LPC_RNG1_COUNT > 0)))
#include "hw_init.h"
#endif

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

extern uint32_t g_isCryptoHWInitialized;
/* Entropy poll callback for a hardware source */
/* TBD - We should add a mutex when entering this function for threaded implementations */
int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
#ifdef TRNG_IGNORE /* Used only for testing. Ignore TRNG in case of a HW Error.*/
    *olen = len;
    return kStatus_Success;
#else
    status_t result              = kStatus_Success;

    /* This check here is required, because same function name exisit for KWxx platforms,
    that is using same function to do its HW init with same function name.*/
#if (( defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0))      || \
    ( defined(FSL_FEATURE_SOC_RNG_COUNT) && (FSL_FEATURE_SOC_RNG_COUNT > 0))         || \
    ( defined(FSL_FEATURE_SOC_LPC_RNG_COUNT) && (FSL_FEATURE_SOC_LPC_RNG_COUNT > 0)) || \
    ( defined(FSL_FEATURE_SOC_LPC_RNG1_COUNT) && (FSL_FEATURE_SOC_LPC_RNG1_COUNT > 0)))
    /* Temp HW initialization for platforms, which are using psa-apis in examples,
    but does not have the psa-crypto-driver available*/
    if (g_isCryptoHWInitialized == ELS_PKC_CRYPTOHW_UNINITIALIZED)
    {
        CRYPTO_InitHardware();
    }
#endif

#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
#ifndef TRNG0
#define TRNG0 TRNG
#endif
    result = TRNG_GetRandomData(TRNG0, output, len);
#elif defined(FSL_FEATURE_SOC_RNG_COUNT) && (FSL_FEATURE_SOC_RNG_COUNT > 0)
    result = RNGA_GetRandomData(RNG, (void *)output, len);
#elif defined(FSL_FEATURE_SOC_LPC_RNG_COUNT) && (FSL_FEATURE_SOC_LPC_RNG_COUNT > 0)
    uint32_t rn;
    size_t length;
    int i;

    length = len;

    while (length > 0)
    {
        rn = RNG_GetRandomData();

        if (length >= sizeof(uint32_t))
        {
            memcpy(output, &rn, sizeof(uint32_t));
            length -= sizeof(uint32_t);
            output += sizeof(uint32_t);
        }
        else
        {
            memcpy(output, &rn, length);
            output += length;
            length = 0U;
        }

        /* Discard next 32 random words for better entropy */
        for (i = 0; i < 32; i++)
        {
            RNG_GetRandomData();
        }
    }

    result = kStatus_Success;
#elif defined(FSL_FEATURE_SOC_LPC_RNG1_COUNT) && (FSL_FEATURE_SOC_LPC_RNG1_COUNT > 0)
    result = RNG_GetRandomData(RNG, output, len);
    
#elif defined(FSL_FEATURE_EDGELOCK) && (FSL_FEATURE_EDGELOCK > 0)
    sss_sscp_rng_t ctx;

    result = CRYPTO_InitHardware();
    if (result != kStatus_Success) 
    {
      return result;
    }
    
    result = sss_sscp_rng_context_init(&g_sssSession, &ctx, 0u);
    if (result != kStatus_SSS_Success) 
    {
      return result;
    }
    
    result = sss_sscp_rng_get_random(&ctx, output, len);
    if (result != kStatus_SSS_Success) 
    {
      return result;
    }
    
    result = sss_sscp_rng_free(&ctx);
    if (result != kStatus_SSS_Success) 
    {
      return result;
    }
    
    if (result == kStatus_SSS_Success) 
    {
      result = kStatus_Success;
    }
    
#endif

    if (result == kStatus_Success)
    {
        *olen = len;
        return 0;
    }
    else
    {
        return result;
    }

#endif /* TRNG_IGNORE */
}

#endif /* MBEDTLS_MCUX_ENTROPY */
