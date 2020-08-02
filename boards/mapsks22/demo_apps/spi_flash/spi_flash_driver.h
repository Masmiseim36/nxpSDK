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
#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_DSPI_MASTER_BASE (SPI0)
#define SPI_FLASH_WINBOND_VENDER_ID (0x51) /* WINBOND vendor id: 0xEF */
#define IDCODE_LEN (0x3)
#define BOARD_SPI_FLASH_CS_GPIO GPIOC
#define BOARD_SPI_FLASH_CS_GPIO_PIN 4U

/* flash capacity configuration */
#define FLASH_PAGE_SIZE 256U             /* 256B page size */
#define FLASH_SECTOR_SIZE (256U * 16U)   /* 4K sector size */
#define FLASH_TOTAL_SIZE (1024U * 1024U) /* 1MB total size */

#define ROUND(a, b) (((a) + (b)) & ~((b)-1))

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifdef SFDEBUG
#define SF_DEBUG(str) printf(str)
#else
#define SF_DEBUG(str)
#endif

#define SPI_FLASH_TIMEOUT (200000)

/* flash commands */
#define CMD_READ_ID 0x9f
#define CMD_READ_ARRAY_SLOW 0x03
#define CMD_READ_ARRAY_FAST 0x0b
#define CMD_WRITE_STATUS 0x01
#define CMD_PAGE_PROGRAM 0x02
#define CMD_WRITE_DISABLE 0x04
#define CMD_READ_STATUS 0x05
#define CMD_WRITE_ENABLE 0x06
#define CMD_ERASE_4K 0x20
#define CMD_ERASE_32K 0x52
#define CMD_ERASE_64K 0xd8
#define CMD_ERASE_CHIP 0xc7

/* flash status */
#define STATUS_BUSY 0x01

/* Erase block size */
#define ERASE_4K_SIZE (4096)
#define ERASE_32K_SIZE (32768)
#define ERASE_64K_SIZE (65536)

struct spi_flash
{
    uint32_t size;
    uint32_t page_size;
    uint32_t sector_size;
};

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif
void spi_cs_activate(void);
void spi_cs_deactivate(void);
void spi_flash_addr2cmd(uint32_t addr, uint8_t *cmd);
uint32_t spi_flash_rw(uint8_t *cmd, uint32_t cmd_len, uint8_t *data_out, uint8_t *data_in, uint32_t data_len);
uint32_t spi_flash_enable_write(uint8_t is_enabled);
uint32_t spi_flash_write_page(uint8_t *buf, uint32_t page_offset, uint32_t byte_offset, uint32_t len);
uint32_t spi_flash_chk_status(uint32_t timeout, uint8_t cmd, uint8_t poll_bit);
uint32_t spi_flash_write(uint32_t offset, uint32_t len, void *buf);
uint32_t spi_flash_read(uint32_t offset, uint32_t data_len, void *data);
uint32_t spi_flash_erase_block(uint32_t offset, uint32_t blkSize);
uint32_t spi_flash_erase_sector(uint32_t offset, uint32_t len);
uint32_t spi_flash_erase_all(void);
uint32_t spi_flash_write_status(uint8_t sts_reg);
uint32_t spi_flash_readid(uint8_t *vendorId, uint8_t devId[]);
uint32_t spi_flash_init_winbond(uint32_t pageSize, uint32_t sectorSize, uint32_t sectorNum);
void spi_flash_drv_init(uint32_t bus, uint32_t cs);
#if defined(__cplusplus)
}
#endif
#endif /* _SPI_FLASH_H_ */
