/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void flexspi_hyper_flash_init(void);
extern status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);
extern status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t address, const uint32_t *src);
extern status_t flexspi_nor_hyperflash_cfi(FLEXSPI_Type *base);
extern status_t flexspi_nor_flash_erase_chip(FLEXSPI_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint8_t s_hyperflash_program_buffer[FLASH_PAGE_SIZE];
static uint8_t s_hyperflash_read_buffer[FLASH_PAGE_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    uint32_t i      = 0;
    status_t status = kStatus_Fail;
    bool errorFlag  = false;

    BOARD_InitHardware();

    PRINTF("FLEXSPI hyperflash example started!\r\n");

    flexspi_hyper_flash_init();

    status = flexspi_nor_hyperflash_cfi(EXAMPLE_FLEXSPI);
    /* Get vendor ID. */
    if (status != kStatus_Success)
    {
        return status;
    }
    else
    {
        PRINTF("Found the HyperFlash by CFI\r\n");
    }

#if !(defined(XIP_EXTERNAL_FLASH))
    /* Erase whole chip . */
    PRINTF("Erasing whole chip over FlexSPI...\r\n");

    status = flexspi_nor_flash_erase_chip(EXAMPLE_FLEXSPI);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("Erase finished !\r\n");
#endif

    for (uint32_t j = 0x00U; j < (((FLASH_SIZE * 1024U) / SECTOR_SIZE) - EXAMPLE_SECTOR); j++)
    {
        PRINTF("loop nummber: %d\r\n", j);

        /* Erase sector. */
        status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, (EXAMPLE_SECTOR + j) * SECTOR_SIZE);

        if (status != kStatus_Success)
        {
            PRINTF("Erase sector failure !\r\n");
        }

        memset(s_hyperflash_program_buffer, 0xFF, sizeof(s_hyperflash_program_buffer));

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
        DCACHE_InvalidateByRange(EXAMPLE_FLEXSPI_AMBA_BASE + (EXAMPLE_SECTOR + j) * SECTOR_SIZE, FLASH_PAGE_SIZE);
#endif

        memcpy(s_hyperflash_read_buffer, (void *)(FlexSPI_AMBA_BASE + (EXAMPLE_SECTOR + j) * SECTOR_SIZE),
               sizeof(s_hyperflash_read_buffer));

        if (memcmp(s_hyperflash_program_buffer, s_hyperflash_read_buffer, sizeof(s_hyperflash_program_buffer)))
        {
            PRINTF("Erase data -  read out data value incorrect !\r\n ");
        }

        for (i = 0; i < sizeof(s_hyperflash_program_buffer); i++)
        {
            s_hyperflash_program_buffer[i] = i & 0xFFU;
        }

        for (uint32_t k = 0x00U; k < (SECTOR_SIZE / FLASH_PAGE_SIZE); k++)
        {
            status = flexspi_nor_flash_page_program(EXAMPLE_FLEXSPI,
                                                    (EXAMPLE_SECTOR + j) * SECTOR_SIZE + k * FLASH_PAGE_SIZE,
                                                    (void *)s_hyperflash_program_buffer);

            if (status != kStatus_Success)
            {
                PRINTF("Page program failure !\r\n");
            }

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
            DCACHE_InvalidateByRange(
                EXAMPLE_FLEXSPI_AMBA_BASE + (EXAMPLE_SECTOR + j) * SECTOR_SIZE + k * FLASH_PAGE_SIZE, FLASH_PAGE_SIZE);
#endif

            memcpy(s_hyperflash_read_buffer,
                   (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + (EXAMPLE_SECTOR + j) * SECTOR_SIZE + k * FLASH_PAGE_SIZE),
                   sizeof(s_hyperflash_read_buffer));

            if (memcmp(s_hyperflash_read_buffer, s_hyperflash_program_buffer, sizeof(s_hyperflash_program_buffer)) != 0)
            {
                errorFlag = true;
                PRINTF("Page read failure !\r\n");
            }
        }
    }

    if (errorFlag)
    {
        PRINTF("FLEXSPI hyperflash example failed. \r\n ");
    }
    else
    {
        PRINTF("FLEXSPI hyperflash example successfully. \r\n");
    }

    while (1)
    {
    }
}
