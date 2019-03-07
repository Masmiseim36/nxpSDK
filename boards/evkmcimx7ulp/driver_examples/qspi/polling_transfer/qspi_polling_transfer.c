/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "fsl_qspi.h"
#include "fsl_debug_console.h"

#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_QSPI QuadSPI0
#define QSPI_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Qspi)
#define FLASH_PAGE_SIZE 256U
#define FLASH_SECTORE_SIZE 4096U
#define FLASH_SIZE 0x00800000U
#define FLASH_ENABLE_QUAD_CMD 0x40U
#define QSPI_ERASE_ADDR_OFFSET 0x40000U
#define DUMMY_MASTER (0xE)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

uint32_t lut[FSL_FEATURE_QSPI_LUT_DEPTH] =
    {/* Seq0 :Quad Read */
         /* CMD:        0xEB - Quad Read, Single pad */
         /* ADDR:       0x18 - 24bit address, Quad pads */
         /* DUMMY:      0x06 - 6 clock cyles, Quad pads */
         /* READ:       0x80 - Read 128 bytes, Quad pads */
         /* JUMP_ON_CS: 0 */
         [0] = 0x0A1804EB, [1] = 0x1E800E06, [2] = 0x2400,

         /* Seq1: Write Enable */
         /* CMD:      0x06 - Write Enable, Single pad */
         [4] = 0x406,

         /* Seq2: Erase All */
         /* CMD:    0x60 - Erase All chip, Single pad */
         [8] = 0x460,

         /* Seq3: Read Status */
         /* CMD:    0x05 - Read Status, single pad */
         /* READ:   0x01 - Read 1 byte */
         [12] = 0x1c010405,

         /* Seq4: Page Program */
         /* CMD:    0x02 - Page Program, Single pad */
         /* ADDR:   0x18 - 24bit address, Single pad */
         /* WRITE:  0x80 - Write 128 bytes at one pass, Single pad */
         [16] = 0x08180402, [17] = 0x2080,

         /* Seq5: Write Register */
         /* CMD:    0x01 - Write Status Register, single pad */
         /* WRITE:  0x01 - Write 1 byte of data, single pad */
         [20] = 0x20010401,

         /* Seq6: Read Config Register */
         /* CMD:  0x05 - Read Config register, single pad */
         /* READ: 0x01 - Read 1 byte */
         [24] = 0x1c010405,

         /* Seq7: Erase Sector */
         /* CMD:  0x20 - Sector Erase, single pad */
         /* ADDR: 0x18 - 24 bit address, single pad */
         [28] = 0x08180420,

         /* Seq8: Dummy */
         /* CMD:    0xFF - Dummy command, used to force SPI flash to exit continuous read mode */
         [32] = 0x4FF,

         /* Seq9: Fast Single read */
         /* CMD:        0x0B - Fast Read, Single Pad */
         /* ADDR:       0x18 - 24bit address, Single Pad */
         /* DUMMY:      0x08 - 8 clock cyles, Single Pad */
         /* READ:       0x80 - Read 128 bytes, Single Pad */
         /* JUMP_ON_CS: 0 */
         [36] = 0x0818040B, [37] = 0x1C800C08, [38] = 0x2400,

         /* Seq10: Fast Dual read */
         /* CMD:        0x3B - Dual Read, Single Pad */
         /* ADDR:       0x18 - 24bit address, Single Pad */
         /* DUMMY:      0x08 - 8 clock cyles, Single Pad */
         /* READ:       0x80 - Read 128 bytes, Dual pads */
         /* JUMP_ON_CS: 0 */
         [40] = 0x0818043B, [41] = 0x1D800C08, [42] = 0x2400,

         /* Match MISRA rule */
         [63] = 0};

qspi_flash_config_t single_config = {.flashA1Size = FLASH_SIZE, /* 4MB */
                                     .flashA2Size = 0,
#if defined(FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE) && (FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE)
                                     .flashB1Size = FLASH_SIZE,
                                     .flashB2Size = 0,
#endif
#if !defined(FSL_FEATURE_QSPI_HAS_NO_TDH) || (!FSL_FEATURE_QSPI_HAS_NO_TDH)
                                     .dataHoldTime = 0,
#endif
                                     .CSHoldTime = 0,
                                     .CSSetupTime = 0,
                                     .cloumnspace = 0,
                                     .dataLearnValue = 0,
                                     .endian = kQSPI_64LittleEndian,
                                     .enableWordAddress = false};


/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t buff[64]; /* Test data */
#if !defined(FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL) || (!FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL)
static bool isDivNeedRestore = false;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void QSPI_ClockUpdate(QuadSPI_Type *base)
{
}

/* Check if serial flash erase or program finished. */
void check_if_finished(void)
{
    uint32_t val = 0;
    /* Check WIP bit */
    do
    {
        while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
        {
        }
        QSPI_ClearFifo(EXAMPLE_QSPI, kQSPI_RxFifo);
        QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 12U);
        while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
        {
        }
        val = EXAMPLE_QSPI->RBDR[0];
        /* Clear ARDB area */
        QSPI_ClearErrorFlag(EXAMPLE_QSPI, kQSPI_RxBufferDrain);
    } while (val & 0x1);
}

/* Write enable command */
void cmd_write_enable(void)
{
    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 4U);
}

#if defined(FLASH_ENABLE_QUAD_CMD)
/* Enable Quad mode */
void enable_quad_mode(void)
{
    uint32_t val[4] = {FLASH_ENABLE_QUAD_CMD, 0, 0, 0};

    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, FSL_FEATURE_QSPI_AMBA_BASE);

    /* Clear Tx FIFO */
    QSPI_ClearFifo(EXAMPLE_QSPI, kQSPI_TxFifo);

    /* Write enable */
    cmd_write_enable();

    /* Write data into TX FIFO, needs to write at least 16 bytes of data */
    QSPI_WriteBlocking(EXAMPLE_QSPI, val, 16U);

    /* Set seq id, write register */
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 20);

    /* Wait until finished */
    check_if_finished();
}
#endif

#if defined(FLASH_ENABLE_OCTAL_CMD)
/* Enable Quad DDR mode */
void enable_octal_mode(void)
{
    uint32_t val[4] = {FLASH_ENABLE_OCTAL_CMD, 0, 0, 0};

    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, FSL_FEATURE_QSPI_AMBA_BASE);

    /* Clear Tx FIFO */
    QSPI_ClearFifo(EXAMPLE_QSPI, kQSPI_TxFifo);

    /* Write enable */
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 32U);

    /* Write data into TX FIFO, needs to write at least 16 bytes of data */
    QSPI_WriteBlocking(EXAMPLE_QSPI, val, 16U);

    /* Set seq id, write register */
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 20);

    /* Wait until finished */
    check_if_finished();
}
#endif

/*Erase sector */
void erase_sector(uint32_t addr)
{
    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    QSPI_ClearFifo(EXAMPLE_QSPI, kQSPI_TxFifo);
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, addr);
    cmd_write_enable();
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 28U);
    check_if_finished();
}

/* Erase all command */
void erase_all(void)
{
    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, FSL_FEATURE_QSPI_AMBA_BASE);
    /* Write enable*/
    cmd_write_enable();
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 8U);
    check_if_finished();
}

/* Program page into serial flash using QSPI polling way */
void program_page(uint32_t dest_addr, uint32_t *src_addr)
{
    uint32_t leftLongWords = 0;

    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }
    QSPI_ClearFifo(EXAMPLE_QSPI, kQSPI_TxFifo);

    QSPI_SetIPCommandAddress(EXAMPLE_QSPI, dest_addr);
    cmd_write_enable();
    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & kQSPI_Busy)
    {
    }

    /* First write some data into TXFIFO to prevent from underrun */
    QSPI_WriteBlocking(EXAMPLE_QSPI, src_addr, FSL_FEATURE_QSPI_TXFIFO_DEPTH * 4);
    src_addr += FSL_FEATURE_QSPI_TXFIFO_DEPTH;

    /* Start the program */
    QSPI_SetIPCommandSize(EXAMPLE_QSPI, FLASH_PAGE_SIZE);
    QSPI_ExecuteIPCommand(EXAMPLE_QSPI, 16U);

    leftLongWords = FLASH_PAGE_SIZE - 16 * sizeof(uint32_t);
    QSPI_WriteBlocking(EXAMPLE_QSPI, src_addr, leftLongWords);

    /* Wait until flash finished program */
    check_if_finished();
    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & (kQSPI_Busy | kQSPI_IPAccess))
    {
    }
    QSPI_SoftwareReset(EXAMPLE_QSPI);
    while (QSPI_GetStatusFlags(EXAMPLE_QSPI) & (kQSPI_Busy | kQSPI_IPAccess))
    {
    }
}

/* Use QSPI polling way to program serial flash */
void qspi_polling(void)
{
    uint32_t i = 0;
    uint32_t err = 0;
    uint32_t addr = 0;
#if !defined(QSPI_ERASE_ADDR_OFFSET)
    addr = FSL_FEATURE_QSPI_AMBA_BASE;
#else
    addr = FSL_FEATURE_QSPI_AMBA_BASE + QSPI_ERASE_ADDR_OFFSET;
#endif
    erase_sector(addr);
    PRINTF("Erase finished!\r\n");

#if !defined(FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL) || (!FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL)
    /* Reduce frequency while clock divder is less than 2 */
    uint8_t qspiClockDiv = ((EXAMPLE_QSPI->MCR & QuadSPI_MCR_SCLKCFG_MASK) >> QuadSPI_MCR_SCLKCFG_SHIFT) + 1U;
    if (qspiClockDiv == 1U)
    {
        /* Reduce the frequency */
        isDivNeedRestore = true;
        QSPI_Enable(EXAMPLE_QSPI, false);
        EXAMPLE_QSPI->MCR &= ~QuadSPI_MCR_SCLKCFG_MASK;
        EXAMPLE_QSPI->MCR |= QuadSPI_MCR_SCLKCFG(1U);
        QSPI_Enable(EXAMPLE_QSPI, true);
    }
#endif

    /* Program pages in a sector */
    for (i = 0; i < FLASH_SECTORE_SIZE / FLASH_PAGE_SIZE; i++)
    {
        program_page(addr + i * FLASH_PAGE_SIZE, buff);
    }
    PRINTF("Program data finished!\r\n");

#if !defined(FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL) || (!FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL)
    /* Restore the frequency if needed */
    if (isDivNeedRestore)
    {
        QSPI_Enable(EXAMPLE_QSPI, false);
        EXAMPLE_QSPI->MCR &= ~QuadSPI_MCR_SCLKCFG_MASK;
        EXAMPLE_QSPI->MCR |= QuadSPI_MCR_SCLKCFG(0U);
        QSPI_Enable(EXAMPLE_QSPI, true);
    }
#endif

#if defined(FLASH_NEED_DQS)
    /* Re-configure QSPI Serial clock frequency in order to acheive high performance. */
    QSPI_ClockUpdate(EXAMPLE_QSPI);
#endif

    for (i = 0; i < FLASH_SECTORE_SIZE / 4; i++)
    {
        if (((uint32_t *)addr)[i] != buff[i % 64])
        {
            PRINTF("The data in %d is wrong!!\r\n", i);
            PRINTF("The flash value in %d is %d\r\n", i, ((uint32_t *)addr)[i]);
            err++;
        }
    }
    if (err == 0)
    {
        PRINTF("Program through QSPI polling succeed!\r\n");
    }
}

int main(void)
{
    uint32_t i = 0;
    uint32_t clockSourceFreq = 0;
    qspi_config_t config = {0};

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* 115.2MHz / 4 = 28.8MHz */
    CLOCK_SetIpSrcDiv(kCLOCK_Qspi, kCLOCK_IpSrcSystem, 3, 0);

    /*Enable QSPI clock */
    PRINTF("QSPI example started!\r\n");

    /*Get QSPI default settings and configure the qspi */
    QSPI_GetDefaultQspiConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus */
    config.AHBbufferSize[3] = FLASH_PAGE_SIZE;
    clockSourceFreq = QSPI_CLK_FREQ;

    QSPI_Init(EXAMPLE_QSPI, &config, clockSourceFreq);

    /* Copy the LUT table */
    memcpy(single_config.lookuptable, lut, sizeof(uint32_t) * FSL_FEATURE_QSPI_LUT_DEPTH);

#if defined(FLASH_NEED_DQS)
    /* Set DQS config */
    QSPI_SetDqsConfig(EXAMPLE_QSPI, &dqsConfig);
#endif

    /*According to serial flash feature to configure flash settings */
    QSPI_SetFlashConfig(EXAMPLE_QSPI, &single_config);

    /*Initialize data buffer */ for (i = 0; i < 64; i++)
    {
        buff[i] = i;
    }

#if defined(FLASH_ENABLE_QUAD_CMD)
    /* Enable Quad mode for the flash */
    enable_quad_mode();
#endif

#if defined(FLASH_ENABLE_OCTAL_CMD)
    QSPI_EnableDDRMode(EXAMPLE_QSPI, true);
    /* Enable Octal mode for the flash */
    enable_octal_mode();
#endif

    /* Use polling to program flash. */
    qspi_polling();

    while (1)
    {
    }
}
