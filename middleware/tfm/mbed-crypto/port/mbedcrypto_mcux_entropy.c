/*
 * Copyright 2019 - 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mbedtls/build_info.h"

#if defined(MBEDTLS_MCUX_ENTROPY) && (MBEDTLS_MCUX_ENTROPY == 1)

#include "fsl_common.h"

#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
#include "fsl_trng.h"
#elif defined(FSL_FEATURE_SOC_RNG_COUNT) && (FSL_FEATURE_SOC_RNG_COUNT > 0)
#include "fsl_rnga.h"
#elif defined(FSL_FEATURE_SOC_LPC_RNG_COUNT) && (FSL_FEATURE_SOC_LPC_RNG_COUNT > 0)
#include "fsl_rng.h"
#elif defined(FSL_FEATURE_SOC_LPC_RNG1_COUNT) && (FSL_FEATURE_SOC_LPC_RNG1_COUNT > 0)
#include "fsl_rng.h"
#elif defined(CSS)
#include "mcuxClCss.h"               /* Interface to the entire nxpClCss component */
extern status_t  mbecrypto_mcux_css_init(void);
#endif

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
#elif defined(CSS)
#if 0 /* Simple initialization */
    if( MCUX_CSSL_FP_RESULT(mcuxClCss_Enable_Async()) == MCUXCLCSS_STATUS_OK_WAIT) /* Enable the CSSv2. */
    {
        mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR); /* Wait for the nxpClCss_Enable_Async operation to complete.*/
    }
#else
    mbecrypto_mcux_css_init();
#endif
#endif
}

/* Entropy poll callback for a hardware source */
int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
#ifdef TRNG_IGNORE /* Used only for testing. Ignore TRNG in case of a HW Error.*/
    *olen = len;
    return kStatus_Success;
#else
    static bool rng_init_is_done = false;
    status_t result = kStatus_Success;
    
    if(rng_init_is_done == false)
    {
        mbedtls_mcux_rng_init();
        rng_init_is_done = true;
    }

#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
#ifndef TRNG0
#define TRNG0 TRNG
#endif
    result = TRNG_GetRandomData(TRNG0, output, len);
#elif defined(FSL_FEATURE_SOC_RNG_COUNT) && (FSL_FEATURE_SOC_RNG_COUNT > 0)
    result = RNGA_GetRandomData(RNG, (void *)output, len);
#elif defined(FSL_FEATURE_SOC_CAAM_COUNT) && (FSL_FEATURE_SOC_CAAM_COUNT > 0) && defined(CRYPTO_USE_DRIVER_CAAM)
    result = CAAM_RNG_GetRandomData(CAAM_INSTANCE, &s_caamHandle, kCAAM_RngStateHandle0, output, len, kCAAM_RngDataAny,
                                    NULL);
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
#elif defined(CSS)

    uint8_t rn[MCUXCLCSS_RNG_DRBG_TEST_EXTRACT_OUTPUT_MIN_SIZE];
    size_t  length = len;

    while (length > 0)
    {
        /* Get random word. */
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClCss_Rng_DrbgRequest_Async(rn, sizeof(rn))); 
        if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_Rng_DrbgRequest_Async) != token) || (MCUXCLCSS_STATUS_OK_WAIT != result))
        {
            break;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();
        
        /* Wait for operation to finish */
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_WaitForOperation) != token) || (MCUXCLCSS_STATUS_OK != result))
        {
            break;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();

        if (length >= sizeof(rn))
        {
            memcpy(output, rn, sizeof(rn));
            length -= sizeof(rn);
            output += sizeof(rn);
        }
        else
        {
            memcpy(output, rn, length);
            output += length;
            length = 0U;
        }
    }

    if(length == 0)
    {
        result = kStatus_Success;
    }
    else
    {
        result = kStatus_Fail;
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

