/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_xspi.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_xspi_edma.h"
#include "fsl_edma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t xspi_nor_flash_erase_sector(XSPI_Type *base, uint32_t address);
status_t xspi_nor_flash_page_program(XSPI_Type *base, uint32_t dstAddr, uint32_t *src);
status_t xspi_nor_read_data(XSPI_Type *base, uint32_t startAddress, uint32_t *buffer, uint32_t length);
extern status_t xspi_nor_get_vendor_id(XSPI_Type *base, uint8_t *vendorId);
extern status_t xspi_nor_get_sfdp(XSPI_Type *base, uint32_t *sfdp);
extern void xspi_nor_flash_init(XSPI_Type *base);
extern void xspi_callback(XSPI_Type *base, xspi_edma_handle_t *handle, status_t status, void *userData);
extern status_t xspi_nor_enable_octal_mode(XSPI_Type *base);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*Default xspi+edma driver uses 32-bit data width configuration for transfer,
this requires data buffer address should be aligned to 32-bit. */
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_nor_program_buffer[256], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_nor_read_buffer[256], 4);
edma_handle_t dmaTxHandle;
edma_handle_t dmaRxHandle;
xspi_edma_handle_t xspiHandle;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i = 0;
    status_t status;
    uint8_t vendorID = 0;
    edma_config_t userConfig;

    BOARD_InitHardware();

    PRINTF("\r\nXSPI edma example started!\r\n");

    /* EDMA init */
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&userConfig);
    /* To unlock TBDR, configure the master ID of EDMA to be the same as the cm33_core0 */
    userConfig.enableMasterIdReplication = true;
    EDMA_Init(EXAMPLE_XSPI_DMA, &userConfig);
    /* To unlock TBDR, configure the master ID of EDMA to be the same as the cm33_core0 */
    EDMA_EnableChannelMasterIDReplication(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL, true);
    /* Create the EDMA channel handles */
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&dmaRxHandle, EXAMPLE_XSPI_DMA, XSPI_RX_DMA_CHANNEL);

#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL, XSPI_TX_DMA_REQUEST_SOURCE);
    EDMA_SetChannelMux(EXAMPLE_XSPI_DMA, XSPI_RX_DMA_CHANNEL, XSPI_RX_DMA_REQUEST_SOURCE);
#endif

    /* Create handle for xspi. */
    XSPI_TransferCreateHandleEDMA(EXAMPLE_XSPI, &xspiHandle, xspi_callback, NULL, &dmaTxHandle, &dmaRxHandle);
    /* XSPI init */
    xspi_nor_flash_init(EXAMPLE_XSPI);

#if defined(FLASH_ADESTO) && FLASH_ADESTO
    /* Get vendor ID. */
    status = xspi_nor_get_vendor_id(EXAMPLE_XSPI, &vendorID);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("Vendor ID: 0x%x\r\n", vendorID);

    /* Enter quad mode. */
    status = xspi_nor_enable_octal_mode(EXAMPLE_XSPI);
    if (status != kStatus_Success)
    {
        return status;
    }
#else
    /* Enter quad mode. */
    status = xspi_nor_enable_octal_mode(EXAMPLE_XSPI);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Get vendor ID. */
    status = xspi_nor_get_vendor_id(EXAMPLE_XSPI, &vendorID);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("Vendor ID: 0x%x\r\n", vendorID);
#endif

    /* Erase sectors. */
    PRINTF("Erasing Serial NOR over XSPI...\r\n");

    /* Disable I cache to avoid cache pre-fatch instruction with branch prediction from flash
       and application operate flash synchronously in multi-tasks. */

    status = xspi_nor_flash_erase_sector(EXAMPLE_XSPI, EXAMPLE_SECTOR * SECTOR_SIZE);

    if (status != kStatus_Success)
    {
        PRINTF("Erase sector failure !\r\n");
        return -1;
    }

    memset(s_nor_program_buffer, 0xFFU, sizeof(s_nor_program_buffer));
    memcpy(s_nor_read_buffer, (void *)(EXAMPLE_XSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE),
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

    for (i = 0; i < FLASH_PAGE_SIZE; i++)
    {
        s_nor_program_buffer[i] = i;
    }
    status = xspi_nor_flash_page_program(EXAMPLE_XSPI, EXAMPLE_SECTOR * SECTOR_SIZE, (void *)s_nor_program_buffer);

    if (status != kStatus_Success)
    {
        PRINTF("Page program failure !\r\n");
        return -1;
    }

#if defined(DEMO_INVALIDATE_CACHES)
    DEMO_INVALIDATE_CACHES;
#endif /*  defined(DEMO_INVALIDATE_CACHES) */
    memcpy(s_nor_read_buffer, (void *)(EXAMPLE_XSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE),
           sizeof(s_nor_read_buffer));

    if (memcmp(s_nor_read_buffer, s_nor_program_buffer, sizeof(s_nor_program_buffer)) != 0)
    {
        PRINTF("Program data -  read out data value incorrect !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("EDMA Program data - successfully. \r\n");
    }

    while (1)
    {
    }
}
