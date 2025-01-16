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
static volatile bool s_xecc_multi_error = false;
static volatile bool s_bus_fault_flag   = false;
static uint8_t s_nor_program_buffer[256];
static uint8_t s_nor_read_buffer[256];

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_XECC_BUSFAULT_HANDLER(void)
{
    /* BusFault is active and XECC multiple error interrupt is pending. */
    s_bus_fault_flag = true;
}

void EXAMPLE_XECC_IRQ_HANDLER(void)
{
    uint32_t intStatus;

    intStatus = XECC_GetStatusFlags(EXAMPLE_XECC);
    XECC_ClearStatusFlags(EXAMPLE_XECC, intStatus);

    if ((intStatus & (uint32_t)kXECC_MultiErrorInterruptFlag) != 0x00U)
    {
        s_xecc_multi_error = true;
    }

    /* Avoid to loop between multiple error interrupt and BusFault infinitely*/
    XECC_Deinit(EXAMPLE_XECC);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t multiErrorData;
    uint32_t uncorrectedData;
    uint32_t spiAddress;
    status_t status;
    uint8_t vendorID;
    uint8_t errorFlag = 0x00U;
    xecc_config_t config;
    xecc_multi_error_info_t info;

    BOARD_InitHardware();

    XECC_Deinit(EXAMPLE_XECC);

    flexspi_nor_flash_init(EXAMPLE_FLEXSPI);

    PRINTF("\r\nXECC multiple error example started!\r\n");

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

    memset(s_nor_program_buffer, (int32_t)0xFFU, sizeof(s_nor_program_buffer));
    memcpy(s_nor_read_buffer, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE),
           sizeof(s_nor_read_buffer));

    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, sizeof(s_nor_program_buffer)) != 0x00U)
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
    EnableIRQ(EXAMPLE_XECC_IRQ);
    /*
     * When multiple bits error is occurred for read operation on xecc block, the xecc will generate
     * a fatal interrupt if xecc interrupt enable is active. At the same time, xecc will also output
     * an read response error signal on axi bus to the core. If the BusFault interrupt is enabled in NVIC,
     * then the core will also receive an bus fault interrupt (interrupt_number=5), which is more higher priority
     * than xecc fatal error interrupt. The core will response the BusFault interrupt firstly, then the xecc interrupt
     * ISR.
     */
    SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
    /* Enable XECC single error interrupt */
    XECC_EnableInterrupts(EXAMPLE_XECC, kXECC_MultiErrorInterruptStatusEnable);

    /* Multiple error injection, this operation should precede AHB write access */
    multiErrorData = 0x03;
    XECC_ErrorInjection(EXAMPLE_XECC, multiErrorData, 0);

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

    /* Uncorrected data by XECC through AHB access */
    flexspi_nor_AHB_read_4bytes(EXAMPLE_FLEXSPI, spiAddress, &uncorrectedData);

    /* Waiting for single error interrupt */
    while (!s_xecc_multi_error)
    {
    }

    if (s_bus_fault_flag)
    {
        PRINTF("First level BusFault interrupt finished.\r\n");
    }
    else
    {
        PRINTF("First level BusFault interrupt failed.\r\n");
    }

    /* Verify single error with injection operation. */
    (void)XECC_GetMultiErrorInfo(EXAMPLE_XECC, &info);

    if ((spiAddress == info.multiErrorAddress) && (uncorrectedData == info.multiErrorData))
    {
        PRINTF("Uncorrecdted read data: 0x%x \r\n", uncorrectedData);
        PRINTF("Multiple error address: 0x%x \r\n", info.multiErrorAddress);
        PRINTF("Multiple error read data: 0x%x \r\n", info.multiErrorData);
        PRINTF("Multiple error ECC code: 0x%x \r\n", info.multiErrorEccCode);
        PRINTF("Multiple error bit field: 0x%x \r\n", info.multiErrorBitField);
    }
    else
    {
        errorFlag++;
    }

    if (errorFlag != 0x00U)
    {
        PRINTF("XECC multiple error example failed. \r\n");
    }
    else
    {
        PRINTF("XECC multiple error example successfully. \r\n");
    }

    while (1)
    {
    }
}
