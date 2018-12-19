/*
* Copyright 2017-2018 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "flexspi/fsl_flexspi.h"
#include "flexspi_nand/flexspi_nand_flash.h"
#include "fusemap.h"
#include "utilities/fsl_assert.h"
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
/*
 *  Fuse definition for safe boot frequency
 */
enum
{
    kFlexSpi_Nand_50MHz = 0,
    kFlexSpi_Nand_30MHz = 1,
    kFlexSpi_Nand_20MHz = 2,
    kFlexSpi_Nand_10MHz = 3,
};

/*
 * Fuse definition for CS interval between two commands
 */
enum
{
    kFlexSpi_Nand_Cs_Interval_100Ns = 0,
    kFlexSpi_Nand_Cs_Interval_200Ns = 1,
    kFlexSpi_Nand_Cs_Interval_400Ns = 2,
    kFlexSpi_Nand_Cs_Interval_50Ns = 3,
};

/*
 * Fuse definition for the Column address width
 */
enum
{
    kFlexSpi_Nand_Cs_Column_Addr_Width_12Bit = 0,
    kFlexSpi_Nand_Cs_Column_Addr_Width_13Bit = 1,
    kFlexSpi_Nand_Cs_Column_Addr_Width_14Bit = 2,
    kFlexSpi_Nand_Cs_Column_Addr_Width_15Bit = 3,
};

/*
 *  Common Serial NAND commands
 */
enum
{
    kSerialNAND_Command_PageRead = 0x13,
    kSerialNAND_Command_ReadCache = 0x03,
    kSerialNAND_Command_WriteEnable = 0x06,
    kSerialNAND_Command_ProgramLoad = 0x02,
    kSerialNAND_Command_ProtramExecute = 0x10,
    kSerialNAND_Command_ReadStatus = 0x0F,
};

enum
{
    kPrimaryBootDevice_SerialNAND = 1,
};

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
// Default Read Status command(Get Feature)
static const uint32_t s_read_status_lut[4] = {
    FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x0F, CMD_SDR, FLEXSPI_1PAD, 0xC0),
    FLEXSPI_LUT_SEQ(READ_SDR, FLEXSPI_1PAD, 0x01, STOP, FLEXSPI_1PAD, 0), 0, 0
};

// Default Read ECC Status
static const uint32_t s_read_ecc_status_lut[4] = {
    FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x0F, CMD_SDR, FLEXSPI_1PAD, 0xC0),
    FLEXSPI_LUT_SEQ(READ_SDR, FLEXSPI_1PAD, 0x01, STOP, FLEXSPI_1PAD, 0), 0, 0
};
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
status_t flexspi_nand_get_fcb_search_cfg(uint32_t *searchCount, uint32_t *searchStride)
{
    // Calculate Search count for FCB
    *searchCount = 1 << (ROM_OCOTP_BOOT_SEARCH_COUNT_VALUE() + 1);
    // Calculate Search Stride in terms of pages for FCB
    *searchStride = 1 << (ROM_OCOTP_BOOT_SEARCH_STRIDE_VALUE() + 6);
    return kStatus_Success;
}

status_t flexspi_nand_get_default_cfg_blk(flexspi_nand_config_t *config)
{
    uint32_t s_read_from_cache_lut[4] = { 0 };
    uint32_t s_page_read_lut[4] = { 0 };
    uint32_t columnAddrWidth = 12;
    uint32_t readCacheCommand = kSerialNAND_Command_ReadCache;
    uint32_t readPageCommand = kSerialNAND_Command_PageRead;
    uint32_t tempVal;

    if (get_primary_boot_device() != kBootDevice_FlexSpiNAND)
    {
        return kStatus_InvalidArgument;
    }

    flexspi_mem_config_t *memCfg = &config->memConfig;
    flexspi_nand_config_t *nandCfg = config;

    memset(memCfg, 0, sizeof(flexspi_mem_config_t));

    memCfg->tag = FLEXSPI_CFG_BLK_TAG;
    memCfg->version = FLEXSPI_CFG_BLK_VERSION;
    memCfg->deviceType = kFlexSpiDeviceType_SerialNAND;

    // Get safe frequency for Serial NAND.
    switch (ROM_OCOTP_SAFE_FREQ_VALUE())
    {
        default:
        case kFlexSpi_Nand_50MHz:
            memCfg->serialClkFreq = kFlexSpiSerialClk_50MHz;
            break;
        case kFlexSpi_Nand_30MHz:
            memCfg->serialClkFreq = kFlexSpiSerialClk_30MHz;
            break;
    }

    // Configure default size of Serial NAND.
    // Configured size = actual size * 2
    memCfg->sflashA1Size = 128UL * 1024 * 1024 * 2; // 128M

    memCfg->csHoldTime = 3;
    memCfg->csSetupTime = 3;
    memCfg->sflashPadType = kSerialFlash_1Pad;
    memCfg->timeoutInMs = 100;

    // Get LUT for Cache read
    if (ROM_OCOTP_SPI_NAND_BOOT_CACHE_RD_CMD_VALUE())
    {
        readCacheCommand = ROM_OCOTP_SPI_NAND_BOOT_CACHE_RD_CMD_VALUE();
    }
    s_read_from_cache_lut[0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, readCacheCommand, CADDR_SDR, FLEXSPI_1PAD, 0x10);
    s_read_from_cache_lut[1] = FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_1PAD, 0x08, READ_SDR, FLEXSPI_1PAD, 0x04);

    // Get LUT for Page Read
    if (ROM_OCOTP_SPI_NAND_BOOT_PAGE_RD_CMD_VALUE())
    {
        readPageCommand = ROM_OCOTP_SPI_NAND_BOOT_PAGE_RD_CMD_VALUE();
    }
    s_page_read_lut[0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, readPageCommand, RADDR_SDR, FLEXSPI_1PAD, 0x18);

    // Get Column Address Width
    switch (ROM_OCOTP_COL_ADDRESS_WIDTH_VALUE())
    {
        default:
        case kFlexSpi_Nand_Cs_Column_Addr_Width_12Bit:
            columnAddrWidth = 12;
            break;
        case kFlexSpi_Nand_Cs_Column_Addr_Width_13Bit:
            columnAddrWidth = 13;
            break;
        case kFlexSpi_Nand_Cs_Column_Addr_Width_14Bit:
            columnAddrWidth = 14;
            break;
        case kFlexSpi_Nand_Cs_Column_Addr_Width_15Bit:
            columnAddrWidth = 15;
            break;
    }

    memCfg->columnAddressWidth = columnAddrWidth;
    nandCfg->pageTotalSize = 1 << columnAddrWidth;
    nandCfg->pageDataSize = 1 << (columnAddrWidth - 1);
    memcpy(memCfg->lookupTable, s_read_from_cache_lut, sizeof(s_read_from_cache_lut));
    memcpy(&memCfg->lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READPAGE], s_page_read_lut, sizeof(s_page_read_lut));

    // Get LUT for Read Status if it is not bypassed.
    if (!ROM_OCOTP_BYPASS_READ_STATUS_VALUE())
    {
        memcpy(&memCfg->lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READSTATUS], s_read_status_lut, sizeof(s_read_status_lut));
    }
    else
    {
        nandCfg->bypassReadStatus = true;
        // Calculate the page read time if the read status command is bypassed.
        tempVal = ROM_OCOTP_BYPASS_READ_STATUS_VALUE();
        if (tempVal == 0)
        {
            nandCfg->readPageTimeUs = 75;
        }
        else
        {
            nandCfg->readPageTimeUs = tempVal * 10;
        }
    }

    // Get LUT for ECC read if it is not bypassed.
    if (!ROM_OCOTP_BYPASS_ECC_READ_VALUE())
    {
        nandCfg->bypassEccRead = false;
        memcpy(&memCfg->lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READECCSTAT], s_read_ecc_status_lut,
               sizeof(s_read_ecc_status_lut));
    }
    else
    {
        nandCfg->bypassEccRead = true;
    }

    if (ROM_OCOTP_SPI_NAND_BOOT_BUSY_BIT_OFFSET_OVERRIDE_VALUE())
    {
        memCfg->busyOffset = ROM_OCOTP_SPI_NAND_BOOT_BUSY_BIT_OFFSET_OVERRIDE_VALUE();
    }

    // Get the command interval
    switch (ROM_OCOTP_CS_INTERVAL_VALUE())
    {
        default:
        case kFlexSpi_Nand_Cs_Interval_100Ns:
            memCfg->commandInterval = 100;
            break;
        case kFlexSpi_Nand_Cs_Interval_200Ns:
            memCfg->commandInterval = 200;
            break;
        case kFlexSpi_Nand_Cs_Interval_400Ns:
            memCfg->commandInterval = 400;
            break;
        case kFlexSpi_Nand_Cs_Interval_50Ns:
            memCfg->commandInterval = 50;
            break;
    }

    return kStatus_Success;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
