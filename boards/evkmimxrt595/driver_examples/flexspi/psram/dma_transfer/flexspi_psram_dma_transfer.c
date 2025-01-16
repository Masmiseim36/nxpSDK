/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "fsl_flexspi_dma.h"
#include "app.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*Default flexspi+dma driver uses 32-bit data width configuration for transfer,
this requires data buffer address should be aligned to 32-bit. */
#if defined(__ICCARM__)
#pragma data_alignment = 4
static uint8_t s_hyper_ram_write_buffer[1024];
static uint8_t s_hyper_ram_read_buffer[1024];
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
__attribute__((aligned(4))) static uint8_t s_hyper_ram_write_buffer[1024];
__attribute__((aligned(4))) static uint8_t s_hyper_ram_read_buffer[1024];
#elif defined(__GNUC__)
__attribute__((aligned(4))) static uint8_t s_hyper_ram_write_buffer[1024];
__attribute__((aligned(4))) static uint8_t s_hyper_ram_read_buffer[1024];
#endif

static volatile bool g_completionFlag = false;
extern dma_handle_t dmaTxHandle;
extern dma_handle_t dmaRxHandle;
static flexspi_dma_handle_t flexspiHandle;
/*******************************************************************************
 * Code
 ******************************************************************************/

static void flexspi_callback(FLEXSPI_Type *base, flexspi_dma_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        g_completionFlag = true;
    }
}

status_t flexspi_hyper_ram_dmacommand_write_data(FLEXSPI_Type *base,
                                                 uint32_t address,
                                                 uint32_t *buffer,
                                                 uint32_t length)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write data */
    flashXfer.deviceAddress = address;
    flashXfer.port          = EXAMPLE_FLEXSPI_PORT;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA;
    flashXfer.data          = buffer;
    flashXfer.dataSize      = length;

    status = FLEXSPI_TransferDMA(base, &flexspiHandle, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    /*  Wait for transfer completed. */
    while (!g_completionFlag)
    {
        SDK_DelayAtLeastUs(100, SystemCoreClock);
    }
    g_completionFlag = false;

    return status;
}

status_t flexspi_hyper_ram_dmacommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write data */
    flashXfer.deviceAddress = address;
    flashXfer.port          = EXAMPLE_FLEXSPI_PORT;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = HYPERRAM_CMD_LUT_SEQ_IDX_READDATA;
    flashXfer.data          = buffer;
    flashXfer.dataSize      = length;

    status = FLEXSPI_TransferDMA(base, &flexspiHandle, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    /*  Wait for transfer completed. */
    while (!g_completionFlag)
    {
        SDK_DelayAtLeastUs(100, SystemCoreClock);
    }
    g_completionFlag = false;

    return status;
}

int main(void)
{
    uint32_t i = 0;
    status_t result;

    BOARD_InitHardware();

    /* Create handle for flexspi. */
    FLEXSPI_TransferCreateHandleDMA(EXAMPLE_FLEXSPI, &flexspiHandle, flexspi_callback, NULL, &dmaTxHandle,
                                    &dmaRxHandle);

    PRINTF("FLEXSPI example started!\r\n");

    for (i = 0; i < sizeof(s_hyper_ram_write_buffer); i++)
    {
        s_hyper_ram_write_buffer[i] = i + 1;
    }

    /* IP command write/read, should notice that the start address should be even address and the write address/size
     * should be 1024 aligned.*/
    for (i = 0; i < DRAM_SIZE; i += 1024)
    {
        result = flexspi_hyper_ram_dmacommand_write_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_write_buffer,
                                                         sizeof(s_hyper_ram_write_buffer));
        if (result != kStatus_Success)
        {
            PRINTF("DMA Command Write Failure 0x%x - 0x%x, Failure code : 0x%x!\r\n", i, i + 1023, result);
            return -1;
        }

        result = flexspi_hyper_ram_dmacommand_read_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_read_buffer,
                                                        sizeof(s_hyper_ram_write_buffer));
        if (result != kStatus_Success)
        {
            PRINTF("DMA Command Read Failure 0x%x - 0x%x, Failure code : 0x%x!\r\n", i, i + 1023, result);
            return -1;
        }

        if (memcmp(s_hyper_ram_read_buffer, s_hyper_ram_write_buffer, sizeof(s_hyper_ram_write_buffer)) != 0)
        {
            PRINTF("DMA Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 1023);
            return -1;
        }
    }

    PRINTF("DMA Command Read/Write data succeed at all address range !\r\n");

    while (1)
    {
    }
}
