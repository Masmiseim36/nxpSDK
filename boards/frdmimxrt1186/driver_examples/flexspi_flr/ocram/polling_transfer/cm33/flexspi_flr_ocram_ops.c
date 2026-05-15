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

uint8_t get_lut_seqIndex(uint8_t cmd_type)
{
    if (cmd_type == (uint8_t)READ_REG_STATUS)
    {
        return FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(EXAMPLE_FLEXSPI_SLV_MODE);
    }
    else if (cmd_type == (uint8_t)READ_MEMORY)
    {
        return FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(EXAMPLE_FLEXSPI_SLV_MODE);
    }
    else if (cmd_type == (uint8_t)SEND_MAILBOX)
    {
        return FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(EXAMPLE_FLEXSPI_SLV_MODE);
    }
    else if (cmd_type == (uint8_t)WRITE_MEMORY)
    {
        return FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(EXAMPLE_FLEXSPI_SLV_MODE);
    }
    else
    {
        (void)PRINTF("[Error] %s: %s: %s\r\n", __FILE__, __FUNCTION__, __LINE__);
        return 0xFF;
    }
}

status_t flexspi_ocram_send_mailbox(FLEXSPI_Type *base, uint8_t index, uint32_t value)
{
    flexspi_transfer_t flashXfer;

    /* Send a mailbox. */
    flashXfer.deviceAddress = 0x40U + (uint32_t)index * 4U;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = get_lut_seqIndex(SEND_MAILBOX);
    flashXfer.data          = &value;
    flashXfer.dataSize      = 4;

    return FLEXSPI_TransferBlocking(base, &flashXfer);
}

status_t flexspi_ocram_get_mailbox(FLEXSPI_Type *base, uint8_t index, uint32_t *value)
{
    flexspi_transfer_t flashXfer;

    /* Get mailbox content. */
    flashXfer.deviceAddress = 0x40U + (uint32_t)index * 4U;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = get_lut_seqIndex(READ_REG_STATUS);
    flashXfer.data          = value;
    flashXfer.dataSize      = 4;

    return FLEXSPI_TransferBlocking(base, &flashXfer);
}

status_t flexspi_ocram_status_get(FLEXSPI_Type *base, uint32_t *value)
{
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = 0x30;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = get_lut_seqIndex(READ_REG_STATUS);
    flashXfer.data          = value;
    flashXfer.dataSize      = 4;

    return FLEXSPI_TransferBlocking(base, &flashXfer);
}

status_t flexspi_ocram_read_memory(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length)
{
    status_t status;
    flexspi_transfer_t flashXfer;

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_cache_status_t cacheStatus = {0};
    flexspi_disable_cache(&cacheStatus);
#endif

    /* Prepare page program command. */
    flashXfer.deviceAddress = dstAddr;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = get_lut_seqIndex(READ_MEMORY);
    flashXfer.data          = (uint32_t *)(uint32_t)src;
    flashXfer.dataSize      = length;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Do software reset or clear AHB buffer directly. */
#if defined(FSL_FEATURE_SOC_OTFAD_COUNT) && defined(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK) && \
    defined(FLEXSPI_AHBCR_CLRAHBTXBUF_MASK)
    base->AHBCR |= FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK;
    base->AHBCR &= ~(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK);
#else
    FLEXSPI_SoftwareReset(base);
#endif

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_enable_cache(cacheStatus);
#endif

    return status;
}

status_t flexspi_ocram_write_memory(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length)
{
    status_t status;
    flexspi_transfer_t flashXfer;

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_cache_status_t cacheStatus = {0};
    flexspi_disable_cache(&cacheStatus);
#endif

    /* Prepare page program command */
    flashXfer.deviceAddress = dstAddr;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = get_lut_seqIndex(WRITE_MEMORY);
    flashXfer.data          = (uint32_t *)(uint32_t)src;
    flashXfer.dataSize      = length;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Do software reset or clear AHB buffer directly. */
#if defined(FSL_FEATURE_SOC_OTFAD_COUNT) && defined(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK) && \
    defined(FLEXSPI_AHBCR_CLRAHBTXBUF_MASK)
    base->AHBCR |= FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK;
    base->AHBCR &= ~(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK);
#else
    FLEXSPI_SoftwareReset(base);
#endif

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_enable_cache(cacheStatus);
#endif

    return status;
}

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

    /* Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus. */
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
