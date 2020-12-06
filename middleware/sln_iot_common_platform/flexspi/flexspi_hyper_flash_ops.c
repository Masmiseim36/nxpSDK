/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_flexspi.h"
#include "board.h"
#include "clock_config.h"
#include "flexspi_hyper_flash_ops.h"
#include "fsl_common.h"
#include "pin_mux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t flexspi_nor_hyperbus_read(FLEXSPI_Type *base, uint32_t addr, uint32_t *buffer, uint32_t bytes)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    flashXfer.deviceAddress = addr * 2;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA;
    flashXfer.data          = buffer;
    flashXfer.dataSize      = bytes;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    return status;
}

status_t flexspi_nor_hyperbus_write(FLEXSPI_Type *base, uint32_t addr, uint32_t *buffer, uint32_t bytes)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    flashXfer.deviceAddress = addr * 2;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA;
    flashXfer.data          = buffer;
    flashXfer.dataSize      = bytes;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    return status;
}

status_t flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write neable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 2;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t flexspi_nor_wait_bus_busy(FLEXSPI_Type *base)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = 0;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 2;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS;
    flashXfer.data          = &readValue;
    flashXfer.dataSize      = 2;

    do
    {
        status = FLEXSPI_TransferBlocking(base, &flashXfer);

        if (status != kStatus_Success)
        {
            return status;
        }
        if (readValue & 0x8000)
        {
            isBusy = false;
        }
        else
        {
            isBusy = true;
        }

        if (readValue & 0x3200)
        {
            status = kStatus_Fail;
            break;
        }

    } while (isBusy);

    return status;
}

status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    status = flexspi_nor_write_enable(base, address);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 4;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    return status;
}

static status_t flexspi_nor_flash_page_program_with_buffer_seq2(FLEXSPI_Type *base,
                                                                uint32_t address,
                                                                const uint32_t *src)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Prepare page program command */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITE_BUF_SEQ2;
    flashXfer.dataSize      = 0;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    return status;
}

static status_t flexspi_nor_flash_page_program_with_buffer_seq3(FLEXSPI_Type *base,
                                                                uint32_t address,
                                                                const uint32_t *src)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Prepare page program command */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITE_BUF_SEQ3;
    flashXfer.dataSize      = 0;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    return status;
}

static status_t flexspi_nor_flash_page_program_with_buffer_seq4(FLEXSPI_Type *base,
                                                                uint32_t address,
                                                                const uint32_t *src)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Prepare page program command */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITE_BUF_SEQ4;
    flashXfer.data          = (uint32_t *)src;
    flashXfer.dataSize      = 2;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    return status;
}

static status_t flexspi_nor_flash_page_program_with_buffer_seq5(FLEXSPI_Type *base,
                                                                uint32_t address,
                                                                const uint32_t *src)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Prepare page program command */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITE_BUF_SEQ5;
    flashXfer.dataSize      = 0;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    return status;
}

/* See Table 26-28 from IMXRT1060RM */
status_t flexspi_nor_flash_page_program_with_buffer(FLEXSPI_Type *base, uint32_t address, const uint32_t *src)
{
    status_t status;

    /* Write enable */
    status = flexspi_nor_write_enable(base, address);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* seq 2 */
    status = flexspi_nor_flash_page_program_with_buffer_seq2(base, address, NULL);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* seq 3 */
    status = flexspi_nor_flash_page_program_with_buffer_seq3(base, address, NULL);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* seq 4 in a loop */
    for (int i = 0; i <= 255; i++)
    {
        status =
            flexspi_nor_flash_page_program_with_buffer_seq4(base, address + i * 2, (uint32_t *)((uint16_t *)src + i));
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    /* seq5 - program buffer to flash */
    status = flexspi_nor_flash_page_program_with_buffer_seq5(base, address, NULL);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    return status;
}

status_t flexspi_nor_read_vcr(FLEXSPI_Type *base, uint32_t *vcr)
{
    /* Wait status ready. */
    status_t status;
    flexspi_transfer_t flashXfer;

    status = flexspi_nor_write_enable(base, 0);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = 0;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 2;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_READVCR;
    flashXfer.data          = vcr;
    flashXfer.dataSize      = 2;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    return status;
}

status_t flexspi_nor_write_vcr(FLEXSPI_Type *base, uint32_t *vcr)
{
    status_t status;
    uint8_t data[4] = {0x00, 0x38, 0x00, 0x00};

    status = flexspi_nor_write_enable(base, 0);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_hyperbus_write(base, 0x555, (uint32_t *)data, 2);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_hyperbus_write(base, 0x555, vcr, 2);

    if (status != kStatus_Success)
    {
        return status;
    }

    return status;
}

status_t flexspi_nor_hyperflash_cfi(FLEXSPI_Type *base)
{
    /*
     * Read ID-CFI Parameters
     */
    // CFI Entry
    status_t status;
    uint32_t buffer[2];
    uint32_t cfibuffer[32];
    uint8_t data[4] = {0x00, 0x98, 0x00, 0x00};
    int i;
    status = flexspi_nor_hyperbus_write(base, 0x555, (uint32_t *)data, 2);
    if (status != kStatus_Success)
    {
        return status;
    }

    // PRINTF("Entering the ASO mode\r\n");
    // ID-CFI Read
    // Read Query Unique ASCII String

    status = flexspi_nor_hyperbus_read(base, 0x10, &buffer[0], sizeof(buffer));
    if (status != kStatus_Success)
    {
        return status;
    }

    buffer[1] &= 0xFFFF;
    // Check that the data read out is  unicode "QRY" in big-endian order
    if ((buffer[0] != 0x52005100) || (buffer[1] != 0x5900))
    {
        status = kStatus_Fail;
        // PRINTF("Can not found the HyperFlash!\r\n");
        return status;
    }

    for (i = 0; i < 0x20; i++)
    {
        status = flexspi_nor_hyperbus_read(base, i * 2, &cfibuffer[i], 4);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    // ASO Exit
    data[0] = 0x00;
    data[1] = 0xF0;
    status = flexspi_nor_hyperbus_write(base, 0x0, (uint32_t *)data, 2);
    if (status != kStatus_Success)
    {
        // PRINTF("Can not exit the ASO\r\n");
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    // PRINTF("Found the HyperFlash by CFI\r\n");

    return status;
}

status_t flexspi_nor_hyperflash_id(FLEXSPI_Type *base, uint8_t *pid)
{
    /*
     * Read ID-CFI Parameters
     */
    // CFI Entry
    status_t status;
    uint32_t buffer[2];
    uint32_t cfibuffer[32];
    uint8_t data[4] = {0x00, 0x98, 0x00, 0x00};
    int i;

    *pid = 0;

    status = flexspi_nor_hyperbus_write(base, 0x555, (uint32_t *)data, 2);
    if (status != kStatus_Success)
    {
        return status;
    }

    // PRINTF("Entering the ASO mode\r\n");
    // ID-CFI Read
    // Read Query Unique ASCII String

    status = flexspi_nor_hyperbus_read(base, 0x10, &buffer[0], sizeof(buffer));
    if (status != kStatus_Success)
    {
        return status;
    }

    buffer[1] &= 0xFFFF;
    // Check that the data read out is  unicode "QRY" in big-endian order
    if ((buffer[0] != 0x52005100) || (buffer[1] != 0x5900))
    {
        status = kStatus_Fail;
        // PRINTF("Can not found the HyperFlash!\r\n");
        return status;
    }

    for (i = 0; i < 0x20; i++)
    {
        status = flexspi_nor_hyperbus_read(base, i * 2, &cfibuffer[i], 4);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    // ASO Exit
    data[0] = 0x00;
    data[1] = 0xF0;
    status = flexspi_nor_hyperbus_write(base, 0x0, (uint32_t *)data, 2);
    if (status != kStatus_Success)
    {
        // PRINTF("Can not exit the ASO\r\n");
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    // PRINTF("Found the HyperFlash by CFI\r\n");

    *pid = (uint8_t)(cfibuffer[0] >> 24);

    return status;
}
