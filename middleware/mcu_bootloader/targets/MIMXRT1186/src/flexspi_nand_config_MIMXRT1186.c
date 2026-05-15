/*
 * Copyright 2016-2017 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_flexspi.h"
#include "bootloader_common.h"
#include "flexspi_nand_flash.h"
#include "fsl_assert.h"
#include "fsl_device_registers.h"
#include "fusemap.h"
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
/*
 *  Fuse definition for safe boot frequency
 */
enum
{
    kFlexSpi_Nand_Safe_Freq_80MHz = 0,
    kFlexSpi_Nand_Safe_Freq_100MHz,
    kFlexSpi_Nand_Safe_Freq_120MHz,
    kFlexSpi_Nand_Safe_Freq_133MHz,
    kFlexSpi_Nand_Safe_Freq_166MHz,
    kFlexSpi_Nand_Safe_Freq_200MHz,
    kFlexSpi_Nand_Safe_Freq_50MHz,
    kFlexSpi_Nand_Safe_Freq_30MHz,
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

/*
 * Flash Connection
 */
enum
{
    kSerialNandConnection_SinglePortA = 0,
    kSerialNandConnection_SinglePortB = 1,
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
    *searchCount = 1 << (FUSE_XSPI_NAND_SEARCH_CNT_VALUE + 1);
    // Calculate Search Stride in terms of pages for FCB
    *searchStride = 1 << (FUSE_XSPI_NAND_SEARCH_STRIDE_VALUE + 6);
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
    switch (FUSE_XSPI_NAND_SAFE_FREQ_VALUE)
    {
        default:
        case kFlexSpi_Nand_Safe_Freq_80MHz:
            memCfg->serialClkFreq = kFlexSpiSerialClk_80MHz;
            break;
        case kFlexSpi_Nand_Safe_Freq_100MHz:
            memCfg->serialClkFreq = kFlexSpiSerialClk_100MHz;
            break;
        case kFlexSpi_Nand_Safe_Freq_120MHz:
            memCfg->serialClkFreq = kFlexSpiSerialClk_120MHz;
            break;
        case kFlexSpi_Nand_Safe_Freq_133MHz:
            memCfg->serialClkFreq = kFlexSpiSerialClk_133MHz;
            break;
        case kFlexSpi_Nand_Safe_Freq_166MHz:
            memCfg->serialClkFreq = kFlexSpiSerialClk_166MHz;
            break;
        case kFlexSpi_Nand_Safe_Freq_200MHz:
            memCfg->serialClkFreq = kFlexSpiSerialClk_200MHz;
            break;
        case kFlexSpi_Nand_Safe_Freq_50MHz:
            memCfg->serialClkFreq = kFlexSpiSerialClk_50MHz;
            break;
        case kFlexSpi_Nand_Safe_Freq_30MHz:
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
    s_read_from_cache_lut[0] =
        FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, kSerialNAND_Command_ReadCache, CADDR_SDR, FLEXSPI_1PAD, 0x10);
    s_read_from_cache_lut[1] = FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_1PAD, 0x08, READ_SDR, FLEXSPI_1PAD, 0x04);

    // Get LUT for Page Read
    s_page_read_lut[0] =
        FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, kSerialNAND_Command_PageRead, RADDR_SDR, FLEXSPI_1PAD, 0x18);

    // Get Column Address Width
    switch (FUSE_XSPI_NAND_COL_ADDR_WIDTH_VALUE)
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
    nandCfg->bypassReadStatus = false;
    memcpy(&memCfg->lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READSTATUS], s_read_status_lut, sizeof(s_read_status_lut));

    // Get LUT for ECC read if it is not bypassed.
    nandCfg->bypassEccRead = false;
    memcpy(&memCfg->lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READECCSTAT], s_read_ecc_status_lut,
           sizeof(s_read_ecc_status_lut));

    memCfg->busyOffset = 0;

    // Get the command interval
    switch (FUSE_XSPI_NAND_CS_INTERVAL_VALUE)
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

    if (FUSE_XSPI_PIN_GROUP_VALUE)
    {
        memCfg->controllerMiscOption |= FLEXSPI_BITMASK(kFlexSpiMiscOffset_SecondPinMux);
    }

    if (0u == FUSE_XSPI_LOOPBACK_DQS_PIN_SEL_VALUE) // Dummy DQS selected for "loopback from DQS"
    {
        memCfg->controllerMiscOption |= FLEXSPI_BITMASK(kFlexSpiMiscOffset_SecondDqsPinMux);
    }

    if (FUSE_XSPI_PAD_SETTING_OVERRIDE_EN_VALUE)
    {
        flexspi_update_padsetting((FUSE_XSPI_INSTANCE_VALUE + 1), memCfg, FUSE_XSPI_PAD_SETTING_OVERRIDE_VALUE);
    }

    switch (FUSE_XSPI_NAND_CONN_SEL_VALUE)
    {
        default:
        case kSerialNandConnection_SinglePortA:
            memCfg->sflashA1Size = 128UL * 2 * 1024u * 1024u; // Default size: 1Gbit
            break;
        case kSerialNandConnection_SinglePortB:
            memCfg->sflashA1Size = 0;
            memCfg->sflashB1Size = 128UL * 2 * 1024u * 1024u; // Default size: 1Gbit
            break;
    }

    return kStatus_Success;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
