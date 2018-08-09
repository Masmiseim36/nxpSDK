/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "lfs_support.h"
#include "fsl_debug_console.h"
#include "fsl_flexspi.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern flexspi_device_config_t deviceconfig;
extern const uint32_t customLUT[CUSTOM_LUT_LENGTH];

/*******************************************************************************
 * Code
 ******************************************************************************/

static status_t flexspi_nor_hyperbus_read(FLEXSPI_Type *base, uint32_t addr, uint32_t *buffer, uint32_t bytes)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    flashXfer.deviceAddress = addr * 2;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA;
    flashXfer.data = buffer;
    flashXfer.dataSize = bytes;
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    return status;
}

static status_t flexspi_nor_hyperbus_write(FLEXSPI_Type *base, uint32_t addr, uint32_t *buffer, uint32_t bytes)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    flashXfer.deviceAddress = addr * 2;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Write;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA;
    flashXfer.data = buffer;
    flashXfer.dataSize = bytes;
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    return status;
}

static status_t flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write neable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Command;
    flashXfer.SeqNumber = 2;
    flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

static status_t flexspi_nor_wait_bus_busy(FLEXSPI_Type *base)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 2;
    flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS;
    flashXfer.data = &readValue;
    flashXfer.dataSize = 2;

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

static status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write enable */
    status = flexspi_nor_write_enable(base, address);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Command;
    flashXfer.SeqNumber = 4;
    flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR;
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    return status;
}

static status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t address, const uint32_t *src)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write neable */
    status = flexspi_nor_write_enable(base, address);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Write;
    flashXfer.SeqNumber = 2;
    flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM;
    flashXfer.data = (uint32_t *)src;
    flashXfer.dataSize = FLASH_PAGE_SIZE;
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    return status;
}

static status_t flexspi_nor_hyperflash_cfi(FLEXSPI_Type *base)
{
    /*
     * Read ID-CFI Parameters
     */
    // CFI Entry
    status_t status;
    uint32_t buffer[2];
    uint32_t data[1] = {0x9800};
    status = flexspi_nor_hyperbus_write(base, 0x555, data, 2);
    if (status != kStatus_Success)
    {
        return status;
    }

    PRINTF("Entering the ASO mode\r\n");
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
        PRINTF("Can not found the HyperFlash!\r\n");
        return status;
    }
    // ASO Exit
    data[0] = 0xF000;
    status = flexspi_nor_hyperbus_write(base, 0x0, data, 2);
    if (status != kStatus_Success)
    {
        PRINTF("Can not exit the ASO\r\n");
        return status;
    }

    PRINTF("Found the HyperFlash by CFI\r\n");

    return status;
}

static int lfs_hyperflash_read(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    block += LFS_FIRST_SECTOR;
    memcpy(buffer, (void *)(FlexSPI_AMBA_BASE + block * lfsc->block_size + off), size);
    return LFS_ERR_OK;
}

static int lfs_hyperflash_prog(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    status_t status;

    block += LFS_FIRST_SECTOR;
    status = flexspi_nor_flash_page_program(EXAMPLE_FLEXSPI, block * lfsc->block_size + off, (void *)buffer);
    FLEXSPI_SoftwareReset(EXAMPLE_FLEXSPI);

    if (status == kStatus_Fail)
    {
        return LFS_ERR_CORRUPT;
    }
    else if (status != kStatus_Success)
    {
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

static int lfs_hyperflash_erase(const struct lfs_config *lfsc, lfs_block_t block)
{
    status_t status;

    block += LFS_FIRST_SECTOR;
    status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, block * lfsc->block_size);
    FLEXSPI_SoftwareReset(EXAMPLE_FLEXSPI);

    if (status == kStatus_Fail)
    {
        return LFS_ERR_CORRUPT;
    }
    else if (status != kStatus_Success)
    {
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

static int lfs_hyperflash_sync(const struct lfs_config *lfsc)
{
    return LFS_ERR_OK;
}

static const struct lfs_config lfsc_default = {
    // block device driver context data
    .context = NULL,

    // block device operations
    .read = lfs_hyperflash_read,
    .prog = lfs_hyperflash_prog,
    .erase = lfs_hyperflash_erase,
    .sync = lfs_hyperflash_sync,

    // block device configuration
    .read_size = 16,
    .prog_size = FLASH_PAGE_SIZE,
    .block_size = FLASH_SECTOR_SIZE,
    .block_count = LFS_SECTORS,
    .lookahead = 128,
};

int lfs_get_default_config(struct lfs_config *lfsc)
{
    *lfsc = lfsc_default; /* copy pre-initialized lfs config structure */
    return 0;
}

int lfs_storage_init(const struct lfs_config *lfsc)
{
    flexspi_config_t config;
    status_t status;

    SCB_DisableDCache();
    PRINTF("FLEXSPI hyperflash example started!\r\n");

    /*Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus. */
    config.ahbConfig.enableAHBPrefetch = true;
    /* enable diff clock and DQS */
    config.enableSckBDiffOpt = true;
    config.rxSampleClock = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
    config.enableCombination = true;
    FLEXSPI_Init(EXAMPLE_FLEXSPI, &config);

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(EXAMPLE_FLEXSPI, &deviceconfig, kFLEXSPI_PortA1);

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(EXAMPLE_FLEXSPI, 0, customLUT, CUSTOM_LUT_LENGTH);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(EXAMPLE_FLEXSPI);

    status = flexspi_nor_hyperflash_cfi(EXAMPLE_FLEXSPI);
    /* Get vendor ID. */

    return status;
}
