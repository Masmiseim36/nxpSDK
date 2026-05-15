/*
 * Copyright 2023, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "fsl_flexspi_flr.h"
#include "app.h"
#include "fsl_debug_console.h"
#if (defined CACHE_MAINTAIN) && (CACHE_MAINTAIN == 1)
#include "fsl_cache.h"
#endif
#include "fsl_gpt.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 *****************************************************************************/
extern flexspi_device_config_t deviceconfig;
extern const uint32_t customLUT[CUSTOM_LUT_LENGTH];

/*******************************************************************************
 * Code
 ******************************************************************************/
#if (defined CACHE_MAINTAIN) && (CACHE_MAINTAIN == 1)
void flexspi_disable_cache(flexspi_cache_status_t *cacheStatus)
{
#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    /* Disable D cache. */
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
        cacheStatus->DCacheEnableFlag = true;
    }
#endif /* __DCACHE_PRESENT */

#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    /* Disable I cache. */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
        cacheStatus->ICacheEnableFlag = true;
    }
#endif /* __ICACHE_PRESENT */

#elif (defined FSL_FEATURE_SOC_LMEM_COUNT) && (FSL_FEATURE_SOC_LMEM_COUNT != 0U)
    /* Disable code bus cache and system bus cache */
    if (LMEM_PCCCR_ENCACHE_MASK == (LMEM_PCCCR_ENCACHE_MASK & LMEM->PCCCR))
    {
        L1CACHE_DisableCodeCache();
        cacheStatus->codeCacheEnableFlag = true;
    }
    if (LMEM_PSCCR_ENCACHE_MASK == (LMEM_PSCCR_ENCACHE_MASK & LMEM->PSCCR))
    {
        L1CACHE_DisableSystemCache();
        cacheStatus->systemCacheEnableFlag = true;
    }

#elif (defined FSL_FEATURE_SOC_CACHE64_CTRL_COUNT) && (FSL_FEATURE_SOC_CACHE64_CTRL_COUNT != 0U)
    /* Disable cache */
    CACHE64_DisableCache(EXAMPLE_CACHE);
    cacheStatus->CacheEnableFlag = true;
#endif
}

void flexspi_enable_cache(flexspi_cache_status_t cacheStatus)
{
#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    if (cacheStatus.DCacheEnableFlag)
    {
        /* Enable D cache. */
        SCB_EnableDCache();
    }
#endif /* __DCACHE_PRESENT */

#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    if (cacheStatus.ICacheEnableFlag)
    {
        /* Enable I cache. */
        SCB_EnableICache();
    }
#endif /* __ICACHE_PRESENT */

#elif (defined FSL_FEATURE_SOC_LMEM_COUNT) && (FSL_FEATURE_SOC_LMEM_COUNT != 0U)
    if (cacheStatus.codeCacheEnableFlag)
    {
        /* Enable code cache. */
        L1CACHE_EnableCodeCache();
    }

    if (cacheStatus.systemCacheEnableFlag)
    {
        /* Enable system cache. */
        L1CACHE_EnableSystemCache();
    }

#elif (defined FSL_FEATURE_SOC_CACHE64_CTRL_COUNT) && (FSL_FEATURE_SOC_CACHE64_CTRL_COUNT != 0U)
    if (cacheStatus.CacheEnableFlag)
    {
        /* Enable cache. */
        CACHE64_EnableCache(EXAMPLE_CACHE);
    }
#endif
}
#endif

void flexspi_ocram_init(FLEXSPI_Type *base)
{
    flexspi_config_t config;
    /* To store custom's LUT table in local. */
    uint32_t tempLUT[CUSTOM_LUT_LENGTH] = {0x00U};

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_cache_status_t cacheStatus = {0};
    flexspi_disable_cache(&cacheStatus);
#endif

    /* Copy LUT information from flash region into RAM region, because LUT update maybe corrupt read sequence(LUT[0])
     * and load wrong LUT table from FLASH region. */
    (void)memcpy(tempLUT, customLUT, sizeof(tempLUT));

    /*Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /* Set AHB buffer size for reading data through AHB bus. */
    config.ahbConfig.enableAHBPrefetch    = true;
    config.ahbConfig.enableAHBBufferable  = true;
    config.ahbConfig.enableReadAddressOpt = true;
    config.ahbConfig.enableAHBCachable    = true;
    config.rxSampleClock                  = EXAMPLE_FLEXSPI_RX_SAMPLE_CLOCK;
    FLEXSPI_Init(base, &config);

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(base, &deviceconfig, FLASH_PORT);

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(base, 0, tempLUT, CUSTOM_LUT_LENGTH);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(base);

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_enable_cache(cacheStatus);
#endif
}

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
uint32_t gptFreq;
void setupTimerInterrupt(void)
{
    gpt_config_t gptConfig;

    GPT_GetDefaultConfig(&gptConfig);
    /* Initialize GPT module */
    GPT_Init(EXAMPLE_GPT, &gptConfig);

    /* Divide GPT clock source frequency by 3 inside GPT module */
    GPT_SetClockDivider(EXAMPLE_GPT, 3);
    /* Get GPT clock frequency */
    gptFreq = EXAMPLE_GPT_CLK_FREQ / 3U;

    assert(gptFreq < ((UINT32_MAX) / 50U));
    /* Use long restart period in case counter is always over. */
    GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, gptFreq * 50U);

    /* Start Timer */
    GPT_StartTimer(EXAMPLE_GPT);
}
