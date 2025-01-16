/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
#include "fsl_xecc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);
extern status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src);
extern status_t flexspi_nor_flash_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t size);
extern status_t flexspi_nor_flash_read(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t size);
extern status_t flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId);
extern status_t flexspi_nor_enable_quad_mode(FLEXSPI_Type *base);
extern status_t flexspi_nor_erase_chip(FLEXSPI_Type *base);
extern void flexspi_nor_flash_init(FLEXSPI_Type *base);
extern void flexspi_nor_xecc_init(FLEXSPI_Type *flexspiBase, XECC_Type *xeccBase, const xecc_config_t *config);
extern void flexspi_nor_AHB_write_4bytes(FLEXSPI_Type *base, uint32_t address, uint8_t *buffer);
extern void flexspi_nor_AHB_read_4bytes(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool s_xecc_single_error = false;
static uint8_t s_nor_program_buffer[256];
static uint8_t s_nor_read_buffer[256];

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_XECC_IRQ_HANDLER(void)
{
    uint32_t intStatus;

    intStatus = XECC_GetStatusFlags(EXAMPLE_XECC);
    XECC_ClearStatusFlags(EXAMPLE_XECC, intStatus);

    if ((intStatus & (uint32_t)kXECC_SingleErrorInterruptFlag) != 0x00U)
    {
        s_xecc_single_error = true;
    }
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t singleErrorData;
    uint32_t correctedData;
    uint32_t uncorrectedData;
    uint32_t bitPostion;
    uint32_t spiAddress;
    status_t status;
    uint8_t vendorID;
    uint8_t errorFlag = 0x00U;
    xecc_config_t config;
    xecc_single_error_info_t info;

    BOARD_InitHardware();

    XECC_Deinit(EXAMPLE_XECC);

    flexspi_nor_flash_init(EXAMPLE_FLEXSPI);

    PRINTF("\r\nXECC single error example started!\r\n");

    /* Get vendor ID. */
    status = flexspi_nor_get_vendor_id(EXAMPLE_FLEXSPI, &vendorID);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("Vendor ID: 0x%x\r\n", vendorID);

#if !(defined(XIP_EXTERNAL_FLASH))
    /* Erase whole chip . */
    PRINTF("Erasing whole chip over FlexSPI...\r\n");

    status = flexspi_nor_erase_chip(EXAMPLE_FLEXSPI);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("Erase finished !\r\n");
#endif

#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    volatile bool ICacheEnableFlag = false;
    /* Disable I cache. */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
        ICacheEnableFlag = true;
    }
#endif /* __ICACHE_PRESENT */
#elif (defined __CORTEX_M) && (__CORTEX_M == 4U)
    L1CACHE_DisableCodeCache();
    L1CACHE_DisableSystemCache();
#endif
    /* Enter quad mode. */
    status = flexspi_nor_enable_quad_mode(EXAMPLE_FLEXSPI);
#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    if (ICacheEnableFlag)
    {
        /* Enable I cache. */
        SCB_EnableICache();
    }
#endif /* __ICACHE_PRESENT */
#elif (defined __CORTEX_M) && (__CORTEX_M == 4U)
    L1CACHE_EnableCodeCache();
    L1CACHE_EnableSystemCache();
#endif

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Erase sectors. */
    PRINTF("Erasing Serial NOR FLASH over FlexSPI...\r\n");
#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    /* Disable I cache. */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
        ICacheEnableFlag = true;
    }
#endif /* __ICACHE_PRESENT */
#elif (defined __CORTEX_M) && (__CORTEX_M == 4U)
    L1CACHE_DisableCodeCache();
    L1CACHE_DisableSystemCache();
#endif
    status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, EXAMPLE_SECTOR * SECTOR_SIZE);
#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    if (ICacheEnableFlag)
    {
        /* Enable I cache. */
        SCB_EnableICache();
    }
#endif /* __ICACHE_PRESENT */
#elif (defined __CORTEX_M) && (__CORTEX_M == 4U)
    L1CACHE_EnableCodeCache();
    L1CACHE_EnableSystemCache();
#endif
    if (status != kStatus_Success)
    {
        PRINTF("Erase sector failure !\r\n");
        return -1;
    }

    (void)memset(s_nor_program_buffer, (int32_t)0xFFU, sizeof(s_nor_program_buffer));
    memcpy(s_nor_read_buffer, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE),
           sizeof(s_nor_read_buffer));

    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, sizeof(s_nor_program_buffer)))
    {
        PRINTF("Erase data -  read out data value incorrect !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("Erase data - successfully. \r\n");
    }

    /* Get default configuration */
    XECC_GetDefaultConfig(&config);
    /* Enable ECC function for write and read */
    config.enableXECC     = true;
    config.enableWriteECC = true;
    config.enableReadECC  = true;

    /* Set ECC regions */
    config.Region0BaseAddress = EXAMPLE_FLEXSPI_AMBA_BASE + (EXAMPLE_SECTOR + 0U) * SECTOR_SIZE;
    config.Region0EndAddress  = EXAMPLE_FLEXSPI_AMBA_BASE + (EXAMPLE_SECTOR + 1U) * SECTOR_SIZE;
    config.Region1BaseAddress = EXAMPLE_FLEXSPI_AMBA_BASE + (EXAMPLE_SECTOR + 1U) * SECTOR_SIZE;
    config.Region1EndAddress  = EXAMPLE_FLEXSPI_AMBA_BASE + (EXAMPLE_SECTOR + 2U) * SECTOR_SIZE;
    config.Region2BaseAddress = EXAMPLE_FLEXSPI_AMBA_BASE + (EXAMPLE_SECTOR + 2U) * SECTOR_SIZE;
    config.Region2EndAddress  = EXAMPLE_FLEXSPI_AMBA_BASE + (EXAMPLE_SECTOR + 3U) * SECTOR_SIZE;
    config.Region3BaseAddress = EXAMPLE_FLEXSPI_AMBA_BASE + (EXAMPLE_SECTOR + 3U) * SECTOR_SIZE;
    config.Region3EndAddress  = EXAMPLE_FLEXSPI_AMBA_BASE + (EXAMPLE_SECTOR + 4U) * SECTOR_SIZE;

    /* Initialize XECC */
    flexspi_nor_xecc_init(EXAMPLE_FLEXSPI, EXAMPLE_XECC, &config);
    /* Enable IRQ */
    (void)EnableIRQ(EXAMPLE_XECC_IRQ);
    /* Enable XECC single error interrupt */
    XECC_EnableInterrupts(EXAMPLE_XECC, kXECC_SingleErrorInterruptEnable);

    /* Single error injection, this operation should precede AHB write access */
    singleErrorData = 0x04;
    XECC_ErrorInjection(EXAMPLE_XECC, singleErrorData, 0);

    s_nor_program_buffer[0] = 0xAA;
    s_nor_program_buffer[1] = 0xBB;
    s_nor_program_buffer[2] = 0xCC;
    s_nor_program_buffer[3] = 0xDD;

    spiAddress = EXAMPLE_FLEXSPI_AMBA_BASE + (EXAMPLE_SECTOR + 0U) * SECTOR_SIZE;

#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    /* Disable I cache. */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
        ICacheEnableFlag = true;
    }
#endif /* __ICACHE_PRESENT */
#elif (defined __CORTEX_M) && (__CORTEX_M == 4U)
    L1CACHE_DisableCodeCache();
    L1CACHE_DisableSystemCache();
#endif
    flexspi_nor_AHB_write_4bytes(EXAMPLE_FLEXSPI, spiAddress, s_nor_program_buffer);
#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    if (ICacheEnableFlag)
    {
        /* Enable I cache. */
        SCB_EnableICache();
    }
#endif /* __ICACHE_PRESENT */
#elif (defined __CORTEX_M) && (__CORTEX_M == 4U)
    L1CACHE_EnableCodeCache();
    L1CACHE_EnableSystemCache();
#endif

    DCACHE_CleanInvalidateByRange(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE, FLASH_PAGE_SIZE);

    (void)flexspi_nor_flash_read(EXAMPLE_FLEXSPI, EXAMPLE_SECTOR * SECTOR_SIZE, (void *)s_nor_read_buffer,
                                 sizeof(s_nor_read_buffer));

    /* Uncorrected data bypass XECC through IP access */
    uncorrectedData = (uint32_t)s_nor_read_buffer[0] << 0 | (uint32_t)s_nor_read_buffer[1] << 8 |
                      (uint32_t)s_nor_read_buffer[2] << 16 | (uint32_t)s_nor_read_buffer[3] << 24;

    DCACHE_CleanInvalidateByRange(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE, FLASH_PAGE_SIZE);

    /* Corrected data by XECC through AHB access */
    flexspi_nor_AHB_read_4bytes(EXAMPLE_FLEXSPI, spiAddress, &correctedData);

    /* Waiting for single error interrupt */
    while (!s_xecc_single_error)
    {
    }

    /* Calculate single error bit position */
    bitPostion = correctedData ^ uncorrectedData;

    /* Verify single error with injection operation. */
    XECC_GetSingleErrorInfo(EXAMPLE_XECC, &info);

    if ((spiAddress == info.singleErrorAddress) && (uncorrectedData == info.singleErrorData) &&
        (bitPostion == info.singleErrorBitPos))
    {
        PRINTF("Correcdted read data: 0x%x \r\n", correctedData);
        PRINTF("Uncorrecdted read data: 0x%x \r\n", uncorrectedData);
        PRINTF("Single error address: 0x%x \r\n", info.singleErrorAddress);
        PRINTF("Single error read data: 0x%x \r\n", info.singleErrorData);
        PRINTF("Single error ECC code: 0x%x \r\n", info.singleErrorEccCode);
        PRINTF("Single error bit postion: 0x%x \r\n", info.singleErrorBitPos);
        PRINTF("Single error bit field: 0x%x \r\n", info.singleErrorBitField);
    }
    else
    {
        errorFlag++;
    }

    if (errorFlag != 0x00U)
    {
        PRINTF("XECC single error example failed. \r\n");
    }
    else
    {
        PRINTF("XECC single error example successfully. \r\n");
    }

    while (1)
    {
    }
}
