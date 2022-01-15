/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "flexspi_flash.h"
#include "flexspi_flash_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 *****************************************************************************/
// flexspi_device_config_t deviceconfig = {
//    .flexspiRootClk       = 12000000,
//    .flashSize            = FLASH_SIZE,
//    .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
//    .CSInterval           = 2,
//    .CSHoldTime           = 3,
//    .CSSetupTime          = 3,
//    .dataValidTime        = 0,
//    .columnspace          = 0,
//    .enableWordAddress    = 0,
//    .AWRSeqIndex          = 0,
//    .AWRSeqNumber         = 0,
//    .ARDSeqIndex          = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD,
//    .ARDSeqNumber         = 1,
//    .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
//    .AHBWriteWaitInterval = 0,
//};
//
// static const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
//    /* Normal read mode -SDR */
//    [4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x03, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
//    [4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL + 1] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
//
//    /* Fast read mode - SDR */
//    [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x0B, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
//    [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST + 1] = FLEXSPI_LUT_SEQ(
//        kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x08, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),
//
//    /* Fast read quad mode - SDR */
//    [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x18),
//    [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD + 1] = FLEXSPI_LUT_SEQ(
//        kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x06, kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04),
//
//    /* Read extend parameters */
//    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x81, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),
//
//    /* Write Enable */
//    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
//
//    /* Erase Sector  */
//    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xD7, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
//
//    /* Page Program - single mode */
//    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
//    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE + 1] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
//
//    /* Page Program - quad mode */
//    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x32, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
//    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD + 1] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
//
//    /* Read ID */
//    [4 * NOR_CMD_LUT_SEQ_IDX_READID] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x9F, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),
//
//    /* Enable Quad mode */
//    [4 * NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04),
//
//    /* Enter QPI mode */
//    [4 * NOR_CMD_LUT_SEQ_IDX_ENTERQPI] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x35, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
//
//    /* Exit QPI mode */
//    [4 * NOR_CMD_LUT_SEQ_IDX_EXITQPI] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, 0xF5, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
//
//    /* Read status register */
//    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUSREG] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),
//
//    /* Erase whole chip */
//    [4 * NOR_CMD_LUT_SEQ_IDX_ERASECHIP] =
//        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xC7, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
//};

extern const uint32_t customLUT[CUSTOM_LUT_LENGTH];
/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(ISSI_AT25SFxxxA) || defined(ISSI_IS25LPxxxA) || defined(ISSI_IS25WPxxxA) || defined(WINBOND_W25QxxxJV)
status_t flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write enable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;

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
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_READSTATUSREG;
    flashXfer.data          = &readValue;
    flashXfer.dataSize      = 1;

    do
    {
        status = FLEXSPI_TransferBlocking(base, &flashXfer);

        if (status != kStatus_Success)
        {
            return status;
        }
        if (FLASH_BUSY_STATUS_POL)
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = true;
            }
            else
            {
                isBusy = false;
            }
        }
        else
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = false;
            }
            else
            {
                isBusy = true;
            }
        }

    } while (isBusy);

    return status;
}

status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write enable */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(base);

    return status;
}

status_t flexspi_nor_flash_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write enable */
    status = flexspi_nor_write_enable(base, dstAddr);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = dstAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD;
    flashXfer.data          = (uint32_t *)src;
    flashXfer.dataSize      = length;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    /* Do software reset. */
#if defined(FSL_FEATURE_SOC_OTFAD_COUNT) && defined(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK)
    base->AHBCR |= FLEXSPI_AHBCR_CLRAHBRXBUF_MASK;
    base->AHBCR &= ~(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK);
#else
    FLEXSPI_SoftwareReset(base);
#endif

    return status;
}

status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write enable */
    status = flexspi_nor_write_enable(base, dstAddr);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = dstAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD;
    flashXfer.data          = (uint32_t *)src;
    flashXfer.dataSize      = FLASH_PAGE_SIZE;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    /* Do software reset. */
#if defined(FSL_FEATURE_SOC_OTFAD_COUNT) && defined(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK)
    base->AHBCR |= FLEXSPI_AHBCR_CLRAHBRXBUF_MASK;
    base->AHBCR &= ~(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK);
#else
    FLEXSPI_SoftwareReset(base);
#endif

    return status;
}

status_t flexspi_nor_erase_chip(FLEXSPI_Type *base)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write enable */
    status = flexspi_nor_write_enable(base, 0);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = 0;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_ERASECHIP;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    return status;
}
#elif defined(Macronix_MX25UM51345G) || defined(Macronix_MX25UM51345G_2nd)
status_t flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr, bool enableOctal)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write neable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    if (enableOctal)
    {
        flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI;
    }
    else
    {
        flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;
    }

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t flexspi_nor_wait_bus_busy(FLEXSPI_Type *base, bool enableOctal)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = 0;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    if (enableOctal)
    {
        flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI;
    }
    else
    {
        flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READSTATUS;
    }

    flashXfer.data     = &readValue;
    flashXfer.dataSize = 1;

    do
    {
        status = FLEXSPI_TransferBlocking(base, &flashXfer);

        if (status != kStatus_Success)
        {
            return status;
        }
        if (FLASH_BUSY_STATUS_POL)
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = true;
            }
            else
            {
                isBusy = false;
            }
        }
        else
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = false;
            }
            else
            {
                isBusy = true;
            }
        }

    } while (isBusy);

    return status;
}

status_t flexspi_nor_enable_octal_mode(FLEXSPI_Type *base)
{
    flexspi_transfer_t flashXfer;
    status_t status;
    uint32_t writeValue = FLASH_ENABLE_OCTAL_CMD;

    /* Write neable */
    status = flexspi_nor_write_enable(base, 0, false);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Enable quad mode. */
    flashXfer.deviceAddress = 0;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_ENTEROPI;
    flashXfer.data          = &writeValue;
    flashXfer.dataSize      = 1;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base, true);

    return status;
}

status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write neable */
    status = flexspi_nor_write_enable(base, 0, true);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base, true);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(base);

    return status;
}

status_t flexspi_nor_flash_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write neable */
    status = flexspi_nor_write_enable(base, dstAddr, true);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = dstAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM;
    flashXfer.data          = (uint32_t *)src;
    flashXfer.dataSize      = length;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base, true);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(base);

    return status;
}

#elif defined(Cypress_S26KSxxxS)
static status_t flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr)
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

static status_t flexspi_nor_wait_bus_busy(FLEXSPI_Type *base)
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

status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address)
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

    /* Do software reset. */
    FLEXSPI_SoftwareReset(EXAMPLE_FLEXSPI);

    return status;
}

status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t address, const uint32_t *src, size_t len)
{
    status_t status;
    flexspi_transfer_t flashXfer;
    flexspi_clock_set();

    /* Write neable */
    status = flexspi_nor_write_enable(base, address);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 2;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM;
    flashXfer.data          = (uint32_t *)src;
    flashXfer.dataSize      = len /*FLASH_PAGE_SIZE*/;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    flexspi_clock_update();

    return status;
}
#endif /* ISSI_AT25SFxxxA ISSI_IS25LPxxxA ISSI_IS25WPxxxA WINBOND_W25QxxxJV */

status_t sbl_flash_erase(uint32_t address, size_t len)
{
    status_t status;

    if ((address % SECTOR_SIZE) || (len % SECTOR_SIZE))
        return -1;

    for (; len > 0; len -= SECTOR_SIZE)
    {
        /* Erase sectors. */
        status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, address);
        if (status != kStatus_Success)
        {
            return -1;
        }

        address += SECTOR_SIZE;
    }

    return 0;
}

status_t sbl_flash_init(void)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    bool DCacheEnableFlag = false;
    /* Disable D cache. */
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
        DCacheEnableFlag = true;
    }
#endif /* __DCACHE_PRESENT */

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(EXAMPLE_FLEXSPI, 0, &customLUT[0], CUSTOM_LUT_LENGTH);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(EXAMPLE_FLEXSPI);

#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    if (DCacheEnableFlag)
    {
        /* Enable D cache. */
        SCB_EnableDCache();
    }
#endif /* __DCACHE_PRESENT */

    return kStatus_Success;
}

/* Make sure the data address is 4 bytes aligned */
static uint32_t flash_program_buffer[64];
status_t sbl_flash_write(uint32_t dstAddr, const void *src, size_t len)
{
    status_t status;
    uint8_t page_off        = 0;
    const uint8_t *src_addr = (const uint8_t *)src;
    uint16_t write_size     = 0;

    if (((dstAddr + len) & FLEXSPI_BASE_ADDRESS_MASK) > (FLASH_SIZE_KB * 0x400))
        return -1;

    for (; len > 0;)
    {
        page_off = dstAddr % FLASH_PAGE_SIZE;    /* An offset value in a page */
        if ((page_off + len) <= FLASH_PAGE_SIZE) /* Write the last page */
            write_size = len;
        else
            write_size = FLASH_PAGE_SIZE - page_off;

        memcpy(flash_program_buffer, src_addr, write_size);
        status = flexspi_nor_flash_program(EXAMPLE_FLEXSPI, dstAddr, flash_program_buffer, write_size);
        if (status != kStatus_Success)
        {
            return -1;
        }
        src_addr += write_size;
        dstAddr += write_size;
        len -= write_size;
    }

    return 0;
}

status_t sbl_flash_read(uint32_t dstAddr, void *buf, size_t len)
{
    uint32_t addr = dstAddr | EXAMPLE_FLEXSPI_AMBA_BASE;

    //	DCACHE_CleanInvalidateByRange(area->fa_off + off, len);
    memcpy(buf, (void *)addr, len);

    return 0;
}

status_t sbl_flash_read_ipc(uint32_t address, uint8_t *buffer, uint32_t length)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Prepare page program command */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD;
    flashXfer.data          = (uint32_t *)(void *)buffer;
    flashXfer.dataSize      = length;
    status                  = FLEXSPI_TransferBlocking(EXAMPLE_FLEXSPI, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(EXAMPLE_FLEXSPI);

    return status;
}
