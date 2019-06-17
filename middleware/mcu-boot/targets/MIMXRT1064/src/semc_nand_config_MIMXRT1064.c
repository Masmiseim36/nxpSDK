/*
* Copyright 2017-2018 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "semc/fsl_semc.h"
#include "semc_nand/semc_nand_flash.h"
#include "fusemap.h"
#include "utilities/fsl_assert.h"
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// 0 - RT1050 A0
// 1 - RT1050 A1
#define SEMC_NAND_FUSE_MAP_VERSION (0)

#define SEMC_NAND_TYPICAL_PAGES_IN_BLOCK (64U)

/* Fuse: ONFI flag */
enum
{
    kFuseSemcNand_OnfiCompliant_Yes = 0U,
    kFuseSemcNand_OnfiCompliant_No = 1U,
};

/* Fuse: ECC type */
enum
{
#if (SEMC_NAND_FUSE_MAP_VERSION == 0)
    kFuseSemcNand_EccType_Sw = 0U,
    kFuseSemcNand_EccType_Hw = 1U,
#elif(SEMC_NAND_FUSE_MAP_VERSION == 1)
    kFuseSemcNand_EccType_Hw = 0U,
    kFuseSemcNand_EccType_Sw = 1U,
#endif
};

/* Fuse: DQS mode */
enum
{
    kFuseSemcNand_DqsPadMode_Disabled = 0U,
    kFuseSemcNand_DqsPadMode_Enabled = 1U,
};

/* Fuse: Pcs selection */
enum
{
    kFuseSemcNand_PcsSelection_CSX0 = 0U,
    kFuseSemcNand_PcsSelection_CSX1 = 1U,
    kFuseSemcNand_PcsSelection_CSX2 = 2U,
    kFuseSemcNand_PcsSelection_CSX3 = 3U,
    // kFuseSemcNand_PcsSelection_A8  = 4-7U,
};

/* Fuse: I/O Port Size */
enum
{
    kFuseSemcNand_IoPortSize_8bit = 0U,
    kFuseSemcNand_IoPortSize_16bit = 1U,
};

/* Fuse: EDO mode */
enum
{
    kFuseSemcNand_EdoMode_Dis = 0U,
    kFuseSemcNand_EdoMode_En = 1U,
};

/* Fuse: RDY# Polarity */
enum
{
#if (SEMC_NAND_FUSE_MAP_VERSION == 0)
    kFuseSemcNand_RdyPolarity_High = 0U,
    kFuseSemcNand_RdyPolarity_Low = 1U,
#elif(SEMC_NAND_FUSE_MAP_VERSION == 1)
    kFuseSemcNand_RdyPolarity_Low = 0U,
    kFuseSemcNand_RdyPolarity_High = 1U,
#endif
};

/* Fuse: Ready Check Type */
enum
{
#if (SEMC_NAND_FUSE_MAP_VERSION == 0)
    kFuseSemcNand_ReadyCheckType_RB = 0U,
    kFuseSemcNand_ReadyCheckType_SR = 1U,
#elif(SEMC_NAND_FUSE_MAP_VERSION == 1)
    kFuseSemcNand_ReadyCheckType_SR = 0U,
    kFuseSemcNand_ReadyCheckType_RB = 1U,
#endif
};

/* Fuse: Clock frequency */
enum
{
    kFuseSemcNand_ClkFreq_2ndMax = 0U,
    kFuseSemcNand_ClkFreq_Max = 1U,
};

/* Fuse: Row Column address mode */
enum
{
    kFuseSemcNand_RowColAddrMode_5byte_CA2RA3 = 0U,
    kFuseSemcNand_RowColAddrMode_5byte_CA2RA3_plus = 1U,
    kFuseSemcNand_RowColAddrMode_4byte_CA2RA2 = 2U,
    kFuseSemcNand_RowColAddrMode_3byte_CA2RA1 = 3U,
    kFuseSemcNand_RowColAddrMode_4byte_CA1RA3 = 4U,
    kFuseSemcNand_RowColAddrMode_4byte_CA1RA3_plus = 5U,
    kFuseSemcNand_RowColAddrMode_3byte_CA1RA2 = 6U,
    kFuseSemcNand_RowColAddrMode_2byte_CA1RA1 = 7U,
};

/* Fuse: Column Address Width */
enum
{
    kFuseSemcNand_ColumnAddressWidth_12bits = 0U,
    kFuseSemcNand_ColumnAddressWidth_9bits = 1U,
    kFuseSemcNand_ColumnAddressWidth_10bits = 2U,
    kFuseSemcNand_ColumnAddressWidth_11bits = 3U,
    kFuseSemcNand_ColumnAddressWidth_13bits = 4U,
    kFuseSemcNand_ColumnAddressWidth_14bits = 5U,
    kFuseSemcNand_ColumnAddressWidth_15bits = 6U,
    kFuseSemcNand_ColumnAddressWidth_16bits = 7U,
};

/* Fuse: Status Command Type */
enum
{
    kFuseSemcNand_StatusCmdType_Common = 0U,
    kFuseSemcNand_StatusCmdType_Enhanced = 1U,
};

/* Fuse: Pages in block */
enum
{
    kFuseSemcNand_PagesInBlock_128 = 0U,
    kFuseSemcNand_PagesInBlock_8 = 1U,
    kFuseSemcNand_PagesInBlock_16 = 2U,
    kFuseSemcNand_PagesInBlock_32 = 3U,
    kFuseSemcNand_PagesInBlock_64 = 4U,
    kFuseSemcNand_PagesInBlock_256 = 5U,
    kFuseSemcNand_PagesInBlock_512 = 6U,
    kFuseSemcNand_PagesInBlock_1024 = 7U,
};

/* Fuse: Device ECC status */
enum
{
    kFuseSemcNand_DeviceEccStatus_On = 0U,
    kFuseSemcNand_DeviceEccStatus_Off = 1U,
};

/* Fuse: ONFI Timing mode */
enum
{
    kFuseSemcNand_OnfiTimingMode_0_10MHz = 0U,
    kFuseSemcNand_OnfiTimingMode_1_20MHz = 1U,
    kFuseSemcNand_OnfiTimingMode_2_28MHz = 2U,
    kFuseSemcNand_OnfiTimingMode_3_33MHz = 3U,
    kFuseSemcNand_OnfiTimingMode_4_40MHz = 4U,
    kFuseSemcNand_OnfiTimingMode_5_50MHz = 5U,
    // kFuseSemcNand_OnfiTimingMode_Fastest = 6-7U,
};

/* Fuse: Memory Access command */
enum
{
    kFuseSemcNand_AccessCommand_AXI = 0U,
    kFuseSemcNand_AccessCommand_IPG = 1U,
};

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

status_t semc_nand_get_fcb_search_cfg(uint32_t *searchCount, uint32_t *searchStride)
{
    // Calculate Search count for FCB
    *searchCount = 1 << (ROM_OCOTP_SEMC_NAND_SEARCH_COUNT_VALUE());
    // Calculate Search Stride in terms of pages for FCB
    if (ROM_OCOTP_SEMC_NAND_SEARCH_STRIDE_VALUE())
    {
        *searchStride = 1 << (ROM_OCOTP_SEMC_NAND_SEARCH_STRIDE_VALUE());
    }
    else
    {
        *searchStride = SEMC_NAND_TYPICAL_PAGES_IN_BLOCK;
    }

    return kStatus_Success;
}

void semc_nand_get_config_data_from_fuse(semc_nand_config_t *config)
{
    semc_nand_config_t *pNandConfig = config;
    uint32_t fuseValue;

    // Get ONFI flag
    fuseValue = ROM_OCOTP_SEMC_NAND_ONFI_COMPLIANT_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_OnfiCompliant_No:
            pNandConfig->onfiVersion = kNandOnfiVersion_None;
            break;
        case kFuseSemcNand_OnfiCompliant_Yes:
        default:
            pNandConfig->onfiVersion = kNandOnfiVersion_1p0;
            break;
    }

    // Get ECC check type
    fuseValue = ROM_OCOTP_SEMC_NAND_ECC_TYPE_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_EccType_Sw:
            pNandConfig->eccCheckType = kSemcNandEccCheckType_SoftwareECC;
            break;
        case kFuseSemcNand_EccType_Hw:
        default:
            pNandConfig->eccCheckType = kSemcNandEccCheckType_DeviceECC;
            break;
    }

#if (SEMC_NAND_FUSE_MAP_VERSION == 0)
    // Get DQS pad mode
    fuseValue = ROM_OCOTP_SEMC_NAND_DQS_PAD_MODE_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_DqsPadMode_Enabled:
            pNandConfig->memConfig.readStrobeMode = kSemcDqsMode_LoopbackFromDqsPad;
            break;
        case kFuseSemcNand_DqsPadMode_Disabled:
        default:
            pNandConfig->memConfig.readStrobeMode = kSemcDqsMode_LoopbackInternally;
            break;
    }

    // Get PCS selection
    fuseValue = ROM_OCOTP_SEMC_NAND_PCS_SELECTION_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_PcsSelection_CSX0:
            pNandConfig->memConfig.nandMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_CSX0;
            break;
        case kFuseSemcNand_PcsSelection_CSX1:
            pNandConfig->memConfig.nandMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_CSX1;
            break;
        case kFuseSemcNand_PcsSelection_CSX2:
            pNandConfig->memConfig.nandMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_CSX2;
            break;
        case kFuseSemcNand_PcsSelection_CSX3:
            pNandConfig->memConfig.nandMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_CSX3;
            break;
        // case kFuseSemcNand_PcsSelection_A8:
        default:
            pNandConfig->memConfig.nandMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_A8;
            break;
    }
#endif // #if (SEMC_NAND_FUSE_MAP_VERSION == 0)

    // Get I/O Port Size
    fuseValue = ROM_OCOTP_SEMC_NAND_IO_PORT_WIDTH_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_IoPortSize_16bit:
            pNandConfig->memConfig.nandMemConfig.ioPortWidth = 16;
            break;
        case kFuseSemcNand_IoPortSize_8bit:
        default:
            pNandConfig->memConfig.nandMemConfig.ioPortWidth = 8;
            break;
    }

    // Get EDO mode
    fuseValue = ROM_OCOTP_SEMC_NAND_EDO_MODE_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_EdoMode_En:
            pNandConfig->memConfig.nandMemConfig.edoMode = kSemcNandEdoMode_Enabled;
            break;
        case kFuseSemcNand_EdoMode_Dis:
        default:
            pNandConfig->memConfig.nandMemConfig.edoMode = kSemcNandEdoMode_Disabled;
            break;
    }

    // Get RDY Polarity
    fuseValue = ROM_OCOTP_SEMC_NAND_RDY_POLARITY_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_RdyPolarity_High:
            pNandConfig->memConfig.nandMemConfig.rdyPortPolarity = kSemcPortPloarity_HighActive;
            break;
        case kFuseSemcNand_RdyPolarity_Low:
        default:
            pNandConfig->memConfig.nandMemConfig.rdyPortPolarity = kSemcPortPloarity_LowActive;
            break;
    }

    // Get Ready check type
    fuseValue = ROM_OCOTP_SEMC_NAND_READY_CHECK_TYPE_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_ReadyCheckType_RB:
            pNandConfig->readyCheckOption = kSemcNandReadyCheckOption_RB;
            break;
        case kFuseSemcNand_ReadyCheckType_SR:
        default:
            pNandConfig->readyCheckOption = kSemcNandReadyCheckOption_SR;
            break;
    }

#if (SEMC_NAND_FUSE_MAP_VERSION == 1)
    // Get clk freq
    fuseValue = ROM_OCOTP_SEMC_NAND_CLK_FREQ_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_ClkFreq_Max:
            pNandConfig->memConfig.asyncClkFreq = SEMC_MAX_CLK_FREQ;
            break;
        case kFuseSemcNand_ClkFreq_2ndMax:
        default:
            pNandConfig->memConfig.asyncClkFreq = SEMC_2ND_MAX_CLK_FREQ;
            break;
    }
#endif

    // Get Row-Column address mode
    fuseValue = ROM_OCOTP_SEMC_NAND_ROW_COL_ADDR_MODE_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_RowColAddrMode_4byte_CA2RA2:
            pNandConfig->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_4byte_CA2RA2;
            break;
        case kFuseSemcNand_RowColAddrMode_3byte_CA2RA1:
            pNandConfig->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_3byte_CA2RA1;
            break;
        case kFuseSemcNand_RowColAddrMode_4byte_CA1RA3:
        case kFuseSemcNand_RowColAddrMode_4byte_CA1RA3_plus:
            pNandConfig->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_4byte_CA1RA3;
            break;
        case kFuseSemcNand_RowColAddrMode_3byte_CA1RA2:
            pNandConfig->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_3byte_CA1RA2;
            break;
        case kFuseSemcNand_RowColAddrMode_2byte_CA1RA1:
            pNandConfig->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_2byte_CA1RA1;
            break;
        case kFuseSemcNand_RowColAddrMode_5byte_CA2RA3:
        case kFuseSemcNand_RowColAddrMode_5byte_CA2RA3_plus:
        default:
            pNandConfig->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_5byte_CA2RA3;
            break;
    }

    // Get Column Address Width
    fuseValue = ROM_OCOTP_SEMC_NAND_COL_ADDRESS_WIDTH_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_ColumnAddressWidth_9bits:
            pNandConfig->memConfig.nandMemConfig.columnAddressWidth = 9;
            break;
        case kFuseSemcNand_ColumnAddressWidth_10bits:
            pNandConfig->memConfig.nandMemConfig.columnAddressWidth = 10;
            break;
        case kFuseSemcNand_ColumnAddressWidth_11bits:
            pNandConfig->memConfig.nandMemConfig.columnAddressWidth = 11;
            break;
        case kFuseSemcNand_ColumnAddressWidth_13bits:
            pNandConfig->memConfig.nandMemConfig.columnAddressWidth = 13;
            break;
        case kFuseSemcNand_ColumnAddressWidth_14bits:
            pNandConfig->memConfig.nandMemConfig.columnAddressWidth = 14;
            break;
        case kFuseSemcNand_ColumnAddressWidth_15bits:
            pNandConfig->memConfig.nandMemConfig.columnAddressWidth = 15;
            break;
        case kFuseSemcNand_ColumnAddressWidth_16bits:
            pNandConfig->memConfig.nandMemConfig.columnAddressWidth = 16;
            break;
        case kFuseSemcNand_ColumnAddressWidth_12bits:
        default:
            pNandConfig->memConfig.nandMemConfig.columnAddressWidth = 12;
            break;
    }

    // Get Status command type
    fuseValue = ROM_OCOTP_SEMC_NAND_STATUS_CMD_TYPE_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_StatusCmdType_Enhanced:
            pNandConfig->statusCommandType = kSemcNandStatusCommandType_Enhanced;
            break;
        case kFuseSemcNand_StatusCmdType_Common:
        default:
            pNandConfig->statusCommandType = kSemcNandStatusCommandType_Common;
            break;
    }

#if (SEMC_NAND_FUSE_MAP_VERSION == 1)
    // Get Memory access command type
    fuseValue = ROM_OCOTP_SEMC_NAND_ACCESS_COMMAND_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_AccessCommand_IPG:
            pNandConfig->memConfig.accessCommandType = kSemcAccessCommandType_IPBUSCMD;
            break;
        case kFuseSemcNand_AccessCommand_AXI:
        default:
            pNandConfig->memConfig.accessCommandType = kSemcAccessCommandType_AXI32CMD;
            break;
    }
#endif

    // Get Pages in block
    fuseValue = ROM_OCOTP_SEMC_NAND_PAGES_IN_BLOCK_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNand_PagesInBlock_8:
            pNandConfig->pagesInBlock = 8;
            break;
        case kFuseSemcNand_PagesInBlock_16:
            pNandConfig->pagesInBlock = 16;
            break;
        case kFuseSemcNand_PagesInBlock_32:
            pNandConfig->pagesInBlock = 32;
            break;
        case kFuseSemcNand_PagesInBlock_64:
            pNandConfig->pagesInBlock = 64;
            break;
        case kFuseSemcNand_PagesInBlock_128:
            pNandConfig->pagesInBlock = 128;
            break;
        case kFuseSemcNand_PagesInBlock_512:
            pNandConfig->pagesInBlock = 512;
            break;
        case kFuseSemcNand_PagesInBlock_1024:
            pNandConfig->pagesInBlock = 1024;
            break;
        case kFuseSemcNand_PagesInBlock_256:
        default:
            pNandConfig->pagesInBlock = 256;
            break;
    }

    if (pNandConfig->onfiVersion != kNandOnfiVersion_None)
    {
        // Get device ECC initial status
        fuseValue = ROM_OCOTP_SEMC_NAND_DEVICE_ECC_STATUS_VALUE();
        switch (fuseValue)
        {
            case kFuseSemcNand_DeviceEccStatus_Off:
                pNandConfig->deviceEccStatus = kSemcNandDeviceEccStatus_Disabled;
                break;
            case kFuseSemcNand_DeviceEccStatus_On:
            default:
                pNandConfig->deviceEccStatus = kSemcNandDeviceEccStatus_Enabled;
                break;
        }

#if (SEMC_NAND_FUSE_MAP_VERSION == 1)
        // Get ONFI Timing Mode
        fuseValue = ROM_OCOTP_SEMC_NAND_TIMING_MODE_VALUE();
        switch (fuseValue)
        {
            case kFuseSemcNand_OnfiTimingMode_0_10MHz:
                pNandConfig->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode0_10MHz;
                break;
            case kFuseSemcNand_OnfiTimingMode_1_20MHz:
                pNandConfig->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode1_20MHz;
                break;
            case kFuseSemcNand_OnfiTimingMode_2_28MHz:
                pNandConfig->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode2_28MHz;
                break;
            case kFuseSemcNand_OnfiTimingMode_3_33MHz:
                pNandConfig->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode3_33MHz;
                break;
            case kFuseSemcNand_OnfiTimingMode_4_40MHz:
                pNandConfig->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode4_40MHz;
                break;
            case kFuseSemcNand_OnfiTimingMode_5_50MHz:
                pNandConfig->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode5_50MHz;
                break;
            // case kFuseSemcNand_OnfiTimingMode_Fastest:
            default:
                pNandConfig->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_FastestMode;
                break;
        }
#endif
    }
}

status_t semc_nand_get_default_config_block(semc_nand_config_t *config)
{
    semc_nand_config_t *pNandConfig = config;

    // Cleanup config block
    memset(pNandConfig, 0, sizeof(semc_nand_config_t));

    // Set Fuse related config parameters
    // Note: Both ONFI and SPEC device can use ONFI 1.0 timing mode 0
    pNandConfig->onfiVersion = kNandOnfiVersion_1p0;
    pNandConfig->eccCheckType = kSemcNandEccCheckType_DeviceECC;
    pNandConfig->memConfig.nandMemConfig.ioPortWidth = 8;
    pNandConfig->memConfig.nandMemConfig.edoMode = kSemcNandEdoMode_Disabled;
    pNandConfig->memConfig.nandMemConfig.rdyPortPolarity = kSemcPortPloarity_LowActive;
    pNandConfig->readyCheckOption = kSemcNandReadyCheckOption_SR;
    pNandConfig->memConfig.asyncClkFreq = SEMC_2ND_MAX_CLK_FREQ;

    pNandConfig->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_5byte_CA2RA3;
    // Note NA1.0: COL setting should be aligned with device spec for both x8 and x16 device,
    //   it always mean actual page column bit in byte(x8) or word(x16) here. (see Note NA3.0)
    // For example:
    //   MT29F4G08, COL=12, columnAddressWidth=11
    //   MT29F4G16, COL=11, columnAddressWidth=10
    pNandConfig->memConfig.nandMemConfig.columnAddressWidth = 12;
    pNandConfig->statusCommandType = kSemcNandStatusCommandType_Common;
    pNandConfig->pagesInBlock = 64;

    pNandConfig->deviceEccStatus = kSemcNandDeviceEccStatus_Enabled;
    pNandConfig->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode0_10MHz;
    pNandConfig->memConfig.accessCommandType = kSemcAccessCommandType_IPBUSCMD;

    // Get config data from Fuse
    semc_nand_get_config_data_from_fuse(config);

    // Set other config parameters
    pNandConfig->vendorType = kSemcNandVendorType_Micron;
    pNandConfig->cellTechnology = kSemcNandCellTechnology_SLC;
    pNandConfig->enableEccCheck = kSemcNandEccCheckOption_Enabled;
    pNandConfig->swEccBlockBytes = kNandEccBlockSize_512B;
    pNandConfig->readyCheckTimeoutInMs = 1000;
    pNandConfig->readyCheckIntervalInUs = 1;

    // Note NA1.1: since the COL setting of x8 and x16 device is set differently, so the page
    //   unit is differnet here, that's why we have below different calculation.
    //   besides, bytesInPageDataArea is important to following nand address conversion (see Note NA2.x).
    if (pNandConfig->memConfig.nandMemConfig.ioPortWidth == 8)
    {
        pNandConfig->bytesInPageDataArea = 1 << pNandConfig->memConfig.nandMemConfig.columnAddressWidth;
    }
    else if (pNandConfig->memConfig.nandMemConfig.ioPortWidth == 16)
    {
        pNandConfig->bytesInPageDataArea = 1 << (pNandConfig->memConfig.nandMemConfig.columnAddressWidth + 1);
    }
    // Typical usage: 512 Byte page -> 16 Byte spare
    pNandConfig->bytesInPageSpareArea = pNandConfig->bytesInPageDataArea / 32;
    pNandConfig->blocksInPlane = 1024;
    pNandConfig->planesInDevice = 1;
    pNandConfig->enableReadbackVerify = kSemcNandReadbackVerifyOption_Enabled;

    pNandConfig->memConfig.tag = kSemcConfigBlockTag;
    pNandConfig->memConfig.version = SEMC_DRIVER_VERSION;
    pNandConfig->memConfig.deviceMemType = kSemcDeviceMemType_NAND;
    pNandConfig->memConfig.busTimeoutCycles = 255;
    pNandConfig->memConfig.commandExecutionTimeoutCycles = 255;
#if (SEMC_NAND_FUSE_MAP_VERSION == 1)
    pNandConfig->memConfig.readStrobeMode = kSemcDqsMode_LoopbackInternally;
#endif

    pNandConfig->memConfig.nandMemConfig.axiMemBaseAddress = SEMC_MEM_NAND_AXI_BASE;
    pNandConfig->memConfig.nandMemConfig.axiMemSizeInByte = SEMC_MEM_NAND_AXI_SIZE;
    pNandConfig->memConfig.nandMemConfig.ipgMemBaseAddress = SEMC_MEM_NAND_IPG_BASE;
    pNandConfig->memConfig.nandMemConfig.ipgMemSizeInByte = SEMC_MEM_NAND_IPG_SIZE;
#if (SEMC_NAND_FUSE_MAP_VERSION == 1)
    pNandConfig->memConfig.nandMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_CSX0;
#endif
    pNandConfig->memConfig.nandMemConfig.burstLengthInBytes = kSemcMiscProperty_NORNAND_MaxBurstLengthInBytes;

    return kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
