/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_semc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SEMC_EXAMPLE_DATALEN    (0x1000U)
#define SEMC_EXAMPLE_WRITETIMES (1000U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t BOARD_InitSEMC(void);
static void SEMC_SDRAMReadWrite32Bit(void);
static void SEMC_SDRAMReadWrite16Bit(void);
static void SEMC_SDRAMReadWrite8Bit(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

uint32_t sdram_writeBuffer[SEMC_EXAMPLE_DATALEN];
uint32_t sdram_readBuffer[SEMC_EXAMPLE_DATALEN];

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
#include "fsl_cache.h"
#endif

/*!
 * @brief Main function
 */
int main(void)
{
    /* Hardware initialize. */
    BOARD_InitHardware();

    PRINTF("\r\n SEMC SDRAM Example Start!\r\n");
    if (BOARD_InitSEMC() != kStatus_Success)
    {
        PRINTF("\r\n SEMC SDRAM Init Failed\r\n");
    }
    /* 32Bit data read and write. */
    SEMC_SDRAMReadWrite32Bit();
    /* 16Bit data read and write. */
    SEMC_SDRAMReadWrite16Bit();
    /* 8Bit data read and write. */
    SEMC_SDRAMReadWrite8Bit();

    PRINTF("\r\n SEMC SDRAM Example End.\r\n");
    while (1)
    {
    }
}

void SEMC_SDRAMReadWrite32Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    uint32_t *sdram  = (uint32_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
    bool result      = true;

    PRINTF("\r\n SEMC SDRAM Memory 32 bit Write Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);
    /* Prepare data and write to SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index;
        sdram[index]             = sdram_writeBuffer[index];
    }

    PRINTF("\r\n SEMC SDRAM Read 32 bit Data Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    DCACHE_InvalidateByRange(EXAMPLE_SEMC_START_ADDRESS, 4U * SEMC_EXAMPLE_DATALEN);
#endif

    /* Read data from the SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }

    PRINTF("\r\n SEMC SDRAM 32 bit Data Write and Read Compare Start!\r\n");
    /* Compare the two buffers. */
    while (datalen--)
    {
        if (sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        PRINTF("\r\n SEMC SDRAM 32 bit Data Write and Read Compare Failed!\r\n");
    }
    else
    {
        PRINTF("\r\n SEMC SDRAM 32 bit Data Write and Read Compare Succeed!\r\n");
    }
}

static void SEMC_SDRAMReadWrite16Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    uint16_t *sdram  = (uint16_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
    bool result      = true;

    PRINTF("\r\n SEMC SDRAM Memory 16 bit Write Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);

    memset(sdram_writeBuffer, 0, sizeof(sdram_writeBuffer));
    memset(sdram_readBuffer, 0, sizeof(sdram_readBuffer));

    /* Prepare data and write to SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index % 0xFFFF;
        sdram[index]             = sdram_writeBuffer[index];
    }

    PRINTF("\r\n SEMC SDRAM Read 16 bit Data Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    DCACHE_InvalidateByRange(EXAMPLE_SEMC_START_ADDRESS, 4U * SEMC_EXAMPLE_DATALEN);
#endif

    /* Read data from the SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }

    PRINTF("\r\n SEMC SDRAM 16 bit Data Write and Read Compare Start!\r\n");
    /* Compare the two buffers. */
    while (datalen--)
    {
        if (sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        PRINTF("\r\n SEMC SDRAM 16 bit Data Write and Read Compare Failed!\r\n");
    }
    else
    {
        PRINTF("\r\n SEMC SDRAM 16 bit Data Write and Read Compare Succeed!\r\n");
    }
}

static void SEMC_SDRAMReadWrite8Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    uint8_t *sdram   = (uint8_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
    bool result      = true;

    PRINTF("\r\n SEMC SDRAM Memory 8 bit Write Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);

    memset(sdram_writeBuffer, 0, sizeof(sdram_writeBuffer));
    memset(sdram_readBuffer, 0, sizeof(sdram_readBuffer));

    /* Prepare data and write to SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index % 0x100;
        sdram[index]             = sdram_writeBuffer[index];
    }

    PRINTF("\r\n SEMC SDRAM Read 8 bit Data Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    DCACHE_InvalidateByRange(EXAMPLE_SEMC_START_ADDRESS, 4U * SEMC_EXAMPLE_DATALEN);
#endif

    /* Read data from the SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }

    PRINTF("\r\n SEMC SDRAM 8 bit Data Write and Read Compare Start!\r\n");
    /* Compare the two buffers. */
    while (datalen--)
    {
        if (sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        PRINTF("\r\n SEMC SDRAM 8 bit Data Write and Read Compare Failed!\r\n");
    }
    else
    {
        PRINTF("\r\n SEMC SDRAM 8 bit Data Write and Read Compare Succeed!\r\n");
    }
}
