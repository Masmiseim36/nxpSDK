/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_sramc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static status_t SRAMC_ReadWrite8Bit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t sram_writeBuffer[BUFFER_SIZE];
static uint32_t sram_readBuffer[BUFFER_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    status_t result = kStatus_Fail;
    /* Hardware initialize. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\nSRAMC Example Start!\r\n");

    result = SRAMC_ReadWrite8Bit();
    
    if (result == kStatus_Success)
    {
        PRINTF("--- SRAM Read/Write Test Passed ---\r\n");
    }
    else
    {
        PRINTF("--- SRAM Read/Write Test Failed ---\r\n");
    }
   
    while (true)
    {
    }
}

static status_t SRAMC_ReadWrite8Bit(void)
{
    sramc_config_t config;
    status_t result = kStatus_Success;
    uint32_t i;
    volatile uint8_t *sram8 = (volatile uint8_t *)EXAMPLE_SRAMC_START_ADDRESS;
    /* Number of 8-bit elements */
    uint32_t datalen8 = BUFFER_SIZE;

    /* Get Default Configuration */
    SRAMC_GetDefaultConfig(&config);
    config.portSize = kSRAMC_PortSize8Bit;
    /* Adjust timing parameters depends on boards wire connection, to give 
     * enough time for flexio data sample.
     */
    config.turnaroundTime    = EXAMPLE_TURN_AROUND_TIME;
    config.addressHoldTime   = EXAMPLE_ADDRESS_HOLD_TIME;
    config.addressSetupTime  = EXAMPLE_ADDRESS_SETUP_TIME;
    config.readEnableLowTime = EXAMPLE_READ_ENABLE_LOW_TIME;
    
    /* Initialize SRAMC */
    SRAMC_Init(EXAMPLE_SRAMC_BASE, &config);

    /* Prepare Write Buffer */
    PRINTF("Preparing write data...\r\n");
    for (i = 0U; i < datalen8; i++)
    {
        sram_writeBuffer[i] = i % 0x100U;
    }

    /* Write to SRAM */
    PRINTF("Writing %d bytes (8-bit access) to SRAM at 0x%X...\r\n", BUFFER_SIZE, (uint32_t)sram8);

    for (i = 0U; i < datalen8; i++)
    {
        sram8[i] = (uint8_t)(sram_writeBuffer[i] & 0xFFU);
    }
    PRINTF("Write complete.\r\n");

    /* Clear Read Buffer */
    memset(sram_readBuffer, 0, BUFFER_SIZE);

    /* Read from SRAM */
    PRINTF("Reading %d bytes (8-bit access) from SRAM at 0x%X...\r\n", BUFFER_SIZE, (uint32_t)sram8);

    for (i = 0U; i < datalen8; i++)
    {
        sram_readBuffer[i] = sram8[i];
    }
    PRINTF("Read complete.\r\n");

    /* Compare Buffers */
    PRINTF("Comparing written and read data...\r\n");
    for (i = 0U; i < datalen8; i++)
    {
        if (sram_writeBuffer[i] != sram_readBuffer[i])
        {
            PRINTF(" Data mismatch at 8-bit index %d (Address 0x%X): Expected 0x%X, Read 0x%X\r\n",
                   i, (uint32_t)&sram8[i], sram_writeBuffer[i], sram_readBuffer[i]);
            result = kStatus_Fail;
            break;
        }
    }

    /* De-initialize SRAMC */
    SRAMC_Deinit(EXAMPLE_SRAMC_BASE);
    PRINTF("SRAMC De-initialized.\r\n");

    return result;
}