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
#include "fsl_cache.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Program data buffer should be 4-bytes alignment, which can avoid busfault due to this memory region is configured as
   Device Memory by MPU. */
SDK_ALIGN(static uint8_t s_nor_program_buffer[256], 4);
static uint8_t s_nor_read_buffer[256];

extern status_t xspi_nor_flash_erase_sector(XSPI_Type *base, uint32_t address);
extern status_t xspi_nor_flash_program(XSPI_Type *base, uint32_t dstAddr, uint32_t *src, uint32_t length);
extern status_t xspi_nor_get_vendor_id(XSPI_Type *base, uint8_t *vendorId);
extern status_t xspi_nor_get_sfdp(XSPI_Type *base, uint32_t *sfdp);
extern status_t xspi_nor_erase_chip(XSPI_Type *base);
extern void xspi_nor_flash_init(XSPI_Type *base);
extern status_t xspi_nor_enable_octal_mode(XSPI_Type *base);
/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    uint32_t i = 0;
    status_t status;
    uint8_t vendorID   = 0;
    uint32_t data_size = sizeof(s_nor_program_buffer);

    BOARD_InitHardware();
    xspi_nor_flash_init(EXAMPLE_XSPI);

    PRINTF("\r\nXSPI example started!\r\n");

    /* Adesto's octal flash has an limitation for read ID, which is that the Read Manufacturer and Device ID command is
     * limited to a maximum clock frequency of fCLK. */
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
    status = xspi_nor_flash_erase_sector(EXAMPLE_XSPI, EXAMPLE_SECTOR * SECTOR_SIZE);
    if (status != kStatus_Success)
    {
        PRINTF("Erase sector failure !\r\n");
        return -1;
    }

    memset(s_nor_program_buffer, 0xFFU, sizeof(s_nor_program_buffer));

#if defined(DEMO_INVALIDATE_CACHES)
    DEMO_INVALIDATE_CACHES;
#endif /*  defined(DEMO_INVALIDATE_CACHES) */
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

    for (i = 0; i < 0xFFU; i++)
    {
        s_nor_program_buffer[i] = i;
    }

    status =
        xspi_nor_flash_program(EXAMPLE_XSPI, EXAMPLE_SECTOR * SECTOR_SIZE, (void *)s_nor_program_buffer, data_size);
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
        PRINTF("Program data - successfully. \r\n");
    }

    while (1)
    {
    }
}
