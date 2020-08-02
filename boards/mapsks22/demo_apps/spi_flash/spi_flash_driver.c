/*
 * The Clear BSD License
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#include <string.h>
#include "spi_flash_driver.h"
#include "fsl_gpio.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_dspi.h"

static struct spi_flash gSpiFlash;
extern dspi_master_handle_t g_m_handle;
extern volatile bool isDspiTransferCompleted;

void spi_cs_activate(void)
{
    GPIO_PinWrite(BOARD_SPI_FLASH_CS_GPIO, BOARD_SPI_FLASH_CS_GPIO_PIN, 0);
}

void spi_cs_deactivate(void)
{
    GPIO_PinWrite(BOARD_SPI_FLASH_CS_GPIO, BOARD_SPI_FLASH_CS_GPIO_PIN, 1);
}

void spi_flash_addr2cmd(uint32_t addr, uint8_t *cmd)
{
    cmd[1] = addr >> 16;
    cmd[2] = addr >> 8;
    cmd[3] = addr >> 0;
}

uint32_t spi_flash_rw(uint8_t *cmd, uint32_t cmd_len, uint8_t *data_out, uint8_t *data_in, uint32_t data_len)
{
    spi_cs_activate();

    dspi_transfer_t masterXfer;

    masterXfer.txData = cmd;
    masterXfer.rxData = NULL;
    masterXfer.dataSize = cmd_len;
    masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0;

    isDspiTransferCompleted = false;
    DSPI_MasterTransferNonBlocking(EXAMPLE_DSPI_MASTER_BASE, &g_m_handle, &masterXfer);

    /* Wait transfer all data. */
    while (!isDspiTransferCompleted)
    {
    }

    if (data_len != 0)
    {
        masterXfer.txData = data_out;
        masterXfer.rxData = data_in;
        masterXfer.dataSize = data_len;
        masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0;

        isDspiTransferCompleted = false;
        if (DSPI_MasterTransferNonBlocking(EXAMPLE_DSPI_MASTER_BASE, &g_m_handle, &masterXfer))
        {
            PRINTF("\r\nSF:xfer failed\r\n");
        }
        /* Wait transfer all data. */
        while (!isDspiTransferCompleted)
        {
        }
    }

    spi_cs_deactivate();

    return 0;
}

uint32_t spi_flash_chk_status(uint32_t timeout, uint8_t cmd, uint8_t poll_bit)
{
    uint32_t i = 0;
    uint8_t status;

    spi_cs_activate();
    dspi_transfer_t masterXfer;

    masterXfer.txData = &cmd;
    masterXfer.rxData = NULL;
    masterXfer.dataSize = 1;
    masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

    isDspiTransferCompleted = false;
    DSPI_MasterTransferNonBlocking(EXAMPLE_DSPI_MASTER_BASE, &g_m_handle, &masterXfer);

    /* Wait transfer all data. */
    while (!isDspiTransferCompleted)
    {
    }

    for (i = 0; i < timeout; ++i)
    {
        masterXfer.txData = NULL;
        masterXfer.rxData = &status;
        masterXfer.dataSize = 1;
        masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0;

        isDspiTransferCompleted = false;
        DSPI_MasterTransferNonBlocking(EXAMPLE_DSPI_MASTER_BASE, &g_m_handle, &masterXfer);

        /* Wait transfer all data. */
        while (!isDspiTransferCompleted)
        {
        }

        if ((status & poll_bit) == 0)
        {
            break;
        }
    }

    spi_cs_deactivate();

    if (i == timeout)
    {
        PRINTF("\r\nSF: time out!\n");
        return 1;
    }

    return 0;
}

uint32_t spi_flash_enable_write(uint8_t is_enabled)
{
    uint8_t cmd;

    cmd = is_enabled ? CMD_WRITE_ENABLE : CMD_WRITE_DISABLE;

    return spi_flash_rw(&cmd, 1, NULL, NULL, 0);
}

uint32_t spi_flash_write_page(uint8_t *buf, uint32_t page_offset, uint32_t byte_offset, uint32_t len)
{
    uint8_t cmd[4] = {0};

    cmd[0] = CMD_PAGE_PROGRAM;
    cmd[1] = page_offset >> 8;
    cmd[2] = page_offset;
    cmd[3] = byte_offset;

    /* Each write need to enable write */
    if (spi_flash_enable_write(1))
    {
        PRINTF("\r\nSF: enabling write failed\n");
        return 1;
    }

    if (spi_flash_rw(cmd, 4, buf, NULL, len))
    {
        PRINTF("\r\nSF: write failed\n");
        return 1;
    }

    if (spi_flash_chk_status(SPI_FLASH_TIMEOUT, CMD_READ_STATUS, STATUS_BUSY))
    {
        PRINTF("\r\nSF: check status failed\n");
        return 1;
    }

    return 0;
}

uint32_t spi_flash_write(uint32_t offset, uint32_t len, void *buf)
{
    uint32_t page_offset = 0, byte_offset = 0, page_size = 0;
    uint32_t data_chunk_len = 0, data_transferred = 0;
    uint32_t ret = 0;
    struct spi_flash *flash = &gSpiFlash;

    page_size = flash->page_size;
    page_offset = offset / page_size;
    byte_offset = offset % page_size;

    while (data_transferred < len)
    {
        /* First and last sector might be unaligned to page_size,
           So transfer unaligned sector first. */
        data_chunk_len = min(len - data_transferred, page_size - byte_offset);

        ret = spi_flash_write_page(((uint8_t *)buf + data_transferred), page_offset, byte_offset, data_chunk_len);
        if (1 == ret)
        {
            break;
        }

        byte_offset += data_chunk_len;
        if (byte_offset == page_size)
        {
            page_offset++;
            byte_offset = 0;
        }
        data_transferred += data_chunk_len;
    }

    if (ret)
    {
        PRINTF("\r\nSF: program failed!\r\n");
    }
    else
    {
        PRINTF("\r\nSF: program success!\r\n");
    }

    return ret;
}

uint32_t spi_flash_read(uint32_t offset, uint32_t data_len, void *data)
{
    uint8_t cmd[5];

    cmd[0] = CMD_READ_ARRAY_FAST;
    spi_flash_addr2cmd(offset, cmd);
    cmd[4] = 0x00;

    return spi_flash_rw(cmd, sizeof(cmd), NULL, data, data_len);
}

uint32_t spi_flash_erase_block(uint32_t offset, uint32_t blkSize)
{
    uint32_t ret;
    uint8_t cmd[4];

    if (offset % blkSize)
    {
        PRINTF("\r\nSF: Erase offset or length is not multiple of erase size\n");
        return 1;
    }

    switch (blkSize)
    {
        case ERASE_4K_SIZE:
            cmd[0] = CMD_ERASE_4K;
            break;
        case ERASE_32K_SIZE:
            cmd[0] = CMD_ERASE_32K;
            break;
        case ERASE_64K_SIZE:
            cmd[0] = CMD_ERASE_64K;
            break;
        default:
            PRINTF("\r\nSF: Unreconized block size\r\n");
            return 2;
    }

    spi_flash_addr2cmd(offset, cmd);

    ret = spi_flash_enable_write(1);
    if (ret)
    {
        return 1;
    }

    ret = spi_flash_rw(cmd, sizeof(cmd), NULL, NULL, 0);
    if (ret)
    {
        return 1;
    }

    ret = spi_flash_chk_status(SPI_FLASH_TIMEOUT, CMD_READ_STATUS, STATUS_BUSY);
    if (ret)
    {
        return 1;
    }

    PRINTF("\r\nSF: Block successfully erased!\r\n");

    return 0;
}

uint32_t spi_flash_erase_sector(uint32_t offset, uint32_t len)
{
    uint32_t eraseStart, eraseEnd, eraseSize;
    uint32_t ret;
    uint8_t cmd[4];
    struct spi_flash *flash = &gSpiFlash;

    eraseSize = flash->sector_size;
    if ((offset % eraseSize) || (len % eraseSize))
    {
        PRINTF("\r\nSF: Erase offset or length is not multiple of erase size\r\n");
        return 1;
    }

    switch (eraseSize)
    {
        case ERASE_4K_SIZE:
            cmd[0] = CMD_ERASE_4K;
            break;
        case ERASE_32K_SIZE:
            cmd[0] = CMD_ERASE_32K;
            break;
        case ERASE_64K_SIZE:
            cmd[0] = CMD_ERASE_64K;
            break;
        default:
            PRINTF("\r\nSF: Unreconized erase sector size, use 4K for default!\n");
            cmd[0] = CMD_ERASE_4K;
            break;
    }

    eraseStart = offset;
    eraseEnd = eraseStart + len;

    while (offset < eraseEnd)
    {
        spi_flash_addr2cmd(offset, cmd);
        offset += eraseSize;

        ret = spi_flash_enable_write(1);
        if (ret)
        {
            return 1;
        }

        ret = spi_flash_rw(cmd, sizeof(cmd), NULL, NULL, 0);
        if (ret)
        {
            return 1;
        }

        ret = spi_flash_chk_status(SPI_FLASH_TIMEOUT, CMD_READ_STATUS, STATUS_BUSY);
        if (ret)
        {
            return 1;
        }
    }

    PRINTF("\r\nSF: Sector(s) successfully erased!\r\n");

    return 0;
}

uint32_t spi_flash_erase_all(void)
{
    uint32_t ret;
    uint8_t cmd = CMD_ERASE_CHIP;

    ret = spi_flash_enable_write(1);
    if (ret)
    {
        return 1;
    }

    ret = spi_flash_rw(&cmd, 1, NULL, NULL, 0);
    if (ret)
    {
        return 1;
    }

    ret = spi_flash_chk_status(SPI_FLASH_TIMEOUT, CMD_READ_STATUS, STATUS_BUSY);
    if (ret)
    {
        return 1;
    }

    PRINTF("\r\nSF: Chip successfully erased!\r\n");

    return 0;
}

uint32_t spi_flash_write_status(uint8_t sts_reg)
{
    uint8_t cmd;
    uint32_t ret;

    ret = spi_flash_enable_write(1);
    if (ret != 0)
    {
        PRINTF("\r\nSF: enabling write failed\n");
        return ret;
    }

    cmd = CMD_WRITE_STATUS;
    ret = spi_flash_rw(&cmd, 1, &sts_reg, NULL, 1);
    if (ret != 0)
    {
        PRINTF("\r\nSF: fail to write status register\n");
        return ret;
    }

    ret = spi_flash_chk_status(SPI_FLASH_TIMEOUT, CMD_READ_STATUS, STATUS_BUSY);
    if (ret != 0)
    {
        PRINTF("\r\nSF: write status register timed out\n");
        return ret;
    }

    return 0;
}

uint32_t spi_flash_readid(uint8_t *vendorId, uint8_t devId[])
{
    uint32_t ret;
    uint8_t idcode[IDCODE_LEN] = {0};
    uint8_t cmd = CMD_READ_ID;

    /* Send CMD_READ_ID to get flash chip ID codes */
    ret = spi_flash_rw(&cmd, 1, NULL, idcode, sizeof(idcode));
    if (ret)
    {
        return 1;
    }

    *vendorId = idcode[0];
    devId[0] = idcode[1];
    devId[1] = idcode[2];

    return 0;
}

uint32_t spi_flash_init_winbond(uint32_t pageSize, uint32_t sectorSize, uint32_t sectorNum)
{
    struct spi_flash *flash = &gSpiFlash;

    flash->page_size = pageSize;
    flash->sector_size = sectorSize;
    flash->size = flash->sector_size * sectorNum;

    return spi_flash_chk_status(SPI_FLASH_TIMEOUT, CMD_READ_STATUS, STATUS_BUSY);
}

/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
