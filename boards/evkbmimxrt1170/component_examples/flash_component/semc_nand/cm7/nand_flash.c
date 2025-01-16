/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_nand_flash.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * variables
 ******************************************************************************/
uint8_t mem_writeBuffer[FLASH_PAGE_SIZE];
uint8_t mem_readBuffer[FLASH_PAGE_SIZE] = {0};
nand_handle_t nandHandle;
extern nand_config_t nandConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*Error trap function*/
void ErrorTrap(void)
{
    PRINTF("\n\rError occurred. Please check the configurations.\n\r");
    while (1)
    {
        ;
    }
}

int main(void)
{
    status_t status;

    /* Hardware Initialization */
    BOARD_InitHardware();

    PRINTF("\r\n***NAND Flash Component Demo Start!***\r\n");

    PRINTF("\r\n***NAND Flash Initialization Start!***\r\n");
    status = Nand_Flash_Init(&nandConfig, &nandHandle);
    if (status != kStatus_Success)
    {
        PRINTF("\r\n***NAND Flash Initialization Failed!***\r\n");
        ErrorTrap();
    }
    PRINTF("\r\n***NAND Flash Initialization Success!***\r\n");

    /* Erase Block */
    PRINTF("\r\n***NAND Flash Erase The First Block Start!***\r\n");
    status = Nand_Flash_Erase_Block(&nandHandle, 0);
    if (status != kStatus_Success)
    {
        PRINTF("\r\n***NAND Flash Erase block Failed!***\r\n");
        ErrorTrap();
    }

    /* Read and check if it is blank. */
    PRINTF("\r\n***NAND Flash Erase Check Start!***\r\n");
    for (uint32_t pageIndex = 0; pageIndex < nandHandle.pagesInBlock; pageIndex++)
    {
#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
        DCACHE_CleanInvalidateByRange(EXAMPLE_SEMC_NAND_AXI_START_ADDRESS, nandHandle.bytesInPageDataArea);
#endif

        status = Nand_Flash_Read_Page(&nandHandle, pageIndex, mem_readBuffer, nandHandle.bytesInPageDataArea);
        if (status != kStatus_Success)
        {
            PRINTF("\r\n***NAND Flash Read Page Failed!***\r\n");
            ErrorTrap();
        }

        for (uint32_t bytesIndex = 0; bytesIndex < nandHandle.bytesInPageDataArea; bytesIndex++)
        {
            if (mem_readBuffer[bytesIndex] != 0xFF)
            {
                PRINTF("\r\n***NAND Flash Erase block Check Failed!***\r\n");
                ErrorTrap();
            }
        }

        PRINTF("\r\n***NAND Flash Erase block Success!***\r\n");

        /* Program the page data. */
        PRINTF("\r\n***NAND Flash Page Program Start!***\r\n");
        /* Initialize the write buffers. */
        memset(mem_writeBuffer, 0xaa, sizeof(mem_writeBuffer));
        status = Nand_Flash_Page_Program(&nandHandle, pageIndex, mem_writeBuffer, nandHandle.bytesInPageDataArea);
        if (status != kStatus_Success)
        {
            PRINTF("\r\n***NAND Flash Page Program Failed!***\r\n");
            ErrorTrap();
        }

        /* Read page data and check if the data read is equal to the data programed. */
        PRINTF("\r\n***NAND Flash Page Read Start!***\r\n");

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
        DCACHE_CleanInvalidateByRange(EXAMPLE_SEMC_NAND_AXI_START_ADDRESS, nandHandle.bytesInPageDataArea);
#endif

        status = Nand_Flash_Read_Page(&nandHandle, pageIndex, mem_readBuffer, nandHandle.bytesInPageDataArea);
        if (status != kStatus_Success)
        {
            PRINTF("\r\n***NAND Flash Page Read Failed!***\r\n");
            ErrorTrap();
        }

        if (memcmp(mem_writeBuffer, mem_readBuffer, nandHandle.bytesInPageDataArea) != 0)
        {
            PRINTF("\r\n***NAND Flash Page Read Failed!***\r\n");
            ErrorTrap();
        }
    }

    PRINTF("\r\n***NAND Flash Page Read/Write Success!***\r\n");
    while (1)
    {
    }
}
