/*
 * Copyright 2014-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "flexspi_nand_flash.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define NAND_CMD_LUT_FOR_IP_CMD 1 //!< 1 Dedicated LUT Sequence IP for NAND IP Command

enum
{
    kSerialNand_EccCheckMask = 0x30,   // ECC Mask in status register
    kSerialNand_EccFailureMask = 0x20, // ECC failure Mask in status register

    kSerialNandStatus_BusyOffset = 0,
    kSerialNandStatus_EraseFailureOffset = 2,
    kSerialNandStatus_ProgramFailureOffset = 3,

    kSerialNandMaxAhbReadSize = kFlexSpi_AhbMemoryMaxSizeMB,
};

////////////////////////////////////////////////////////////////////////////////
// Local variables
////////////////////////////////////////////////////////////////////////////////
#ifdef FLEXSPI_AMBA_BASE_ADDS
static const uint32_t kFlexSpiAmbaBase[] = FLEXSPI_AMBA_BASE_ADDS;
#else
#if FSL_FEATURE_SOC_FLEXSPI_COUNT > 1
#if defined(FlexSPI0_AMBA_BASE)
static const uint32_t kFlexSpiAmbaBase[] = { FlexSPI0_AMBA_BASE, FlexSPI1_AMBA_BASE };
#elif defined(FlexSPI_AMBA_BASE)
static const uint32_t kFlexSpiAmbaBase[] = { 0, FlexSPI_AMBA_BASE, FlexSPI2_AMBA_BASE };
#else
static const uint32_t kFlexSpiAmbaBase[] = { 0, FlexSPI1_AMBA_BASE, FlexSPI2_AMBA_BASE };
#endif // #if defined(FlexSPI0_AMBA_BASE)
#else
#if defined(FlexSPI0_AMBA_BASE)
static const uint32_t kFlexSpiAmbaBase[] = { FlexSPI0_AMBA_BASE };
#else
static const uint32_t kFlexSpiAmbaBase[] = { FlexSPI_AMBA_BASE };
#endif // #if defined(FlexSPI0_AMBA_BASE)
#endif // #if FSL_FEATURE_SOC_FLEXSPI_COUNT > 1
#endif // #ifdef FLEXSPI_AMBA_BASE_ADDS

////////////////////////////////////////////////////////////////////////////////
// Local prototypes
////////////////////////////////////////////////////////////////////////////////
// NAND Check ECC status
static status_t flexspi_nand_check_ecc_status(uint32_t instance,
                                              flexspi_nand_config_t *config,
                                              uint32_t baseAddress,
                                              bool *isPassed);
// NAND Write Enable
static status_t flexspi_nand_write_enable(uint32_t instance, flexspi_nand_config_t *config, uint32_t baseAddress);

// NAND Check ID
static status_t flexspi_nand_verify_id(uint32_t instance, serial_nand_config_option_t *option);

// Check Error status
static status_t flexspi_nand_check_error_status(uint32_t instance, flexspi_nand_config_t *config, uint32_t baseAddress);

////////////////////////////////////////////////////////////////////////////////
// Codes
////////////////////////////////////////////////////////////////////////////////

status_t flexspi_nand_check_ecc_status(uint32_t instance,
                                       flexspi_nand_config_t *config,
                                       uint32_t baseAddress,
                                       bool *isCheckPassed)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((config == NULL) || (isCheckPassed == NULL))
        {
            break;
        }
        flexspi_xfer_t flashXfer;
        uint32_t eccStatus;
        uint32_t eccCheckMask, eccFailureMask;
        flashXfer.operation = kFlexSpiOperation_Read;
        flashXfer.seqNum = 1;
        flashXfer.seqId = NAND_CMD_LUT_SEQ_IDX_READECCSTAT;
        flashXfer.isParallelModeEnable = false;

        flashXfer.baseAddress = baseAddress;
        if (config->memConfig.lutCustomSeqEnable)
        {
            flashXfer.seqId = config->memConfig.lutCustomSeq[NAND_CMD_INDEX_READECCSTAT].seqId;
            flashXfer.seqNum = config->memConfig.lutCustomSeq[NAND_CMD_INDEX_READECCSTAT].seqNum;
        }
        flashXfer.rxBuffer = &eccStatus;
        flashXfer.rxSize = 4;

        *isCheckPassed = false;
        flexspi_update_lut(instance, NAND_CMD_LUT_FOR_IP_CMD, &config->memConfig.lookupTable[4 * flashXfer.seqId],
                           flashXfer.seqNum);
        flashXfer.seqId = NAND_CMD_LUT_FOR_IP_CMD;
        status = flexspi_command_xfer(instance, &flashXfer);
        if (status == kStatus_Success)
        {
            if (config->eccCheckCustomEnable)
            {
                eccCheckMask = config->eccStatusMask;
                eccFailureMask = config->eccFailureMask;
            }
            else
            {
                eccCheckMask = kSerialNand_EccCheckMask;
                eccFailureMask = kSerialNand_EccFailureMask;
            }
            if ((eccStatus & eccCheckMask) != eccFailureMask)
            {
                *isCheckPassed = true;
            }
        }
    } while (0);

    return status;
}

status_t flexspi_nand_check_error_status(uint32_t instance, flexspi_nand_config_t *config, uint32_t baseAddress)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if (config == NULL)
        {
            break;
        }
        flexspi_xfer_t flashXfer;
        uint32_t statusRegister = 0;
        flashXfer.operation = kFlexSpiOperation_Read;
        flashXfer.seqNum = 1;
        flashXfer.seqId = NAND_CMD_LUT_SEQ_IDX_READSTATUS;
        flashXfer.isParallelModeEnable = false;

        flashXfer.baseAddress = baseAddress;
        flashXfer.rxBuffer = &statusRegister;
        flashXfer.rxSize = 4;

        flexspi_update_lut(instance, NAND_CMD_LUT_FOR_IP_CMD, &config->memConfig.lookupTable[4 * flashXfer.seqId],
                           flashXfer.seqNum);
        flashXfer.seqId = NAND_CMD_LUT_FOR_IP_CMD;
        status = flexspi_command_xfer(instance, &flashXfer);
        if (status != kStatus_Success)
        {
            break;
        }

        if (statusRegister & (1 << kSerialNandStatus_EraseFailureOffset))
        {
            status = kStatus_FlexSPINAND_EraseBlockFail;
            break;
        }

        if (statusRegister & (1 << kSerialNandStatus_ProgramFailureOffset))
        {
            status = kStatus_FlexSPINAND_PageExecuteFail;
            break;
        }

        status = kStatus_Success;

    } while (0);

    return status;
}

status_t flexspi_nand_write_enable(uint32_t instance, flexspi_nand_config_t *config, uint32_t baseAddress)
{
    return flexspi_device_write_enable(instance, &config->memConfig, false, baseAddress);
}

status_t flexspi_nand_init(uint32_t instance, flexspi_nand_config_t *config)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if (config == NULL)
        {
            break;
        }
        status = flexspi_init(instance, &config->memConfig);
        if (status != kStatus_Success)
        {
            break;
        }
        // Configure Lookup table for read
        flexspi_update_lut(instance, 0, config->memConfig.lookupTable, 1);
    } while (0);

    return status;
}

status_t flexspi_nand_read_page(
    uint32_t instance, flexspi_nand_config_t *config, uint32_t pageId, uint32_t *buffer, uint32_t length)
{
    status_t status = kStatus_InvalidArgument;
    do
    {
        if ((config == NULL) || (buffer == NULL) || (length < 1))
        {
            break;
        }
        uint32_t baseAddress;
        uint32_t readAddress;
        flexspi_xfer_t flashXfer;
        bool isEccCheckPass;
        flexspi_mem_config_t *flexspiConfig = &config->memConfig;
        baseAddress = pageId * config->pageTotalSize;
        flashXfer.baseAddress = baseAddress;
        flashXfer.seqId = NAND_CMD_LUT_SEQ_IDX_READPAGE;
        flashXfer.seqNum = 1;
        if (flexspiConfig->lutCustomSeqEnable && flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_READPAGE].seqNum)
        {
            flashXfer.seqId = flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_READPAGE].seqId;
            flashXfer.seqNum = flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_READPAGE].seqNum;
        }
        flashXfer.isParallelModeEnable = false;
        flashXfer.operation = kFlexSpiOperation_Command;
        flexspi_update_lut(instance, NAND_CMD_LUT_FOR_IP_CMD, &config->memConfig.lookupTable[4 * flashXfer.seqId],
                           flashXfer.seqNum);
        flashXfer.seqId = NAND_CMD_LUT_FOR_IP_CMD;
        status = flexspi_command_xfer(instance, &flashXfer);
        if (status != kStatus_Success)
        {
            status = kStatus_FlexSPINAND_ReadPageFail;
            break;
        }

        if (config->bypassReadStatus)
        {
            // Delay several microseconds
            flexspi_sw_delay_us(config->readPageTimeUs);
        }
        else
        {
            status = flexspi_device_wait_busy(instance, flexspiConfig, false, baseAddress);
            if (status != kStatus_Success)
            {
                break;
            }
        }

        flexspi_clear_cache(instance);

        if (!config->bypassEccRead)
        {
            status = flexspi_nand_check_ecc_status(instance, config, baseAddress, &isEccCheckPass);
            if (status != kStatus_Success)
            {
                break;
            }

            if (!isEccCheckPass)
            {
                status = kStatus_FlexSPINAND_EccCheckFail;
                break;
            }
        }
        bool need_switch_read_lut = false;
        // Check if the page is in odd blocks if there are multiple planes in the Serial NAND device
        if (config->hasMultiPlanes)
        {
            // Odd blocks, in the plane2
            if ((pageId / config->pagesPerBlock) & 1)
            {
                // Update Read Cache command for page read in Odd blocks
                need_switch_read_lut = true;
                flexspi_update_lut(instance, NAND_CMD_LUT_SEQ_IDX_READCACHE,
                                   &config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READCACHE_ODD], 1);
            }
        }

        // Maximum AHB access size is less than kSerialNandMaxAhbReadSize
        /* if (baseAddress < kSerialNandMaxAhbReadSize)
        {
            readAddress = baseAddress + kFlexSpiAmbaBase[instance];
            memcpy(buffer, (uint32_t *)readAddress, length);
        }
        else */
        {
            flashXfer.baseAddress = baseAddress;
            flashXfer.isParallelModeEnable = false;
            flashXfer.operation = kFlexSpiOperation_Read;
            flashXfer.rxBuffer = buffer;
            flashXfer.rxSize = length;
            flashXfer.seqId = NAND_CMD_LUT_SEQ_IDX_READCACHE;
            flashXfer.seqNum = 1;
            status = flexspi_command_xfer(instance, &flashXfer);
            if (status != kStatus_Success)
            {
                break;
            }
        }

        if (need_switch_read_lut)
        {
            // Restore Read Cache command for page read in Even blocks
            flexspi_update_lut(instance, NAND_CMD_LUT_SEQ_IDX_READCACHE,
                               &config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READCACHE], 1);
        }

    } while (0);

    return status;
}

status_t flexspi_nand_erase_block(uint32_t instance, flexspi_nand_config_t *config, uint32_t blockId)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if (config == NULL)
        {
            break;
        }

        uint32_t baseAddress;
        flexspi_xfer_t flashXfer;
        bool isEccCheckPassed = false;
        flexspi_mem_config_t *flexspiConfig = &config->memConfig;

        baseAddress = blockId * config->pagesPerBlock * config->pageTotalSize;

        status = flexspi_nand_write_enable(instance, config, baseAddress);
        if (status != kStatus_Success)
        {
            break;
        }

        flashXfer.operation = kFlexSpiOperation_Command;
        flashXfer.isParallelModeEnable = false;
        flashXfer.seqId = NAND_CMD_LUT_SEQ_IDX_ERASEBLOCK;
        flashXfer.seqNum = 1;
        if (flexspiConfig->lutCustomSeqEnable && flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_ERASEBLOCK].seqNum)
        {
            flashXfer.seqId = flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_ERASEBLOCK].seqId;
            flashXfer.seqNum = flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_ERASEBLOCK].seqNum;
        }
        flashXfer.baseAddress = baseAddress;
        flexspi_update_lut(instance, NAND_CMD_LUT_FOR_IP_CMD, &config->memConfig.lookupTable[4 * flashXfer.seqId],
                           flashXfer.seqNum);
        flashXfer.seqId = NAND_CMD_LUT_FOR_IP_CMD;
        status = flexspi_command_xfer(instance, &flashXfer);
        if (status != kStatus_Success)
        {
            status = kStatus_FlexSPINAND_EraseBlockFail;
            break;
        }

        status = flexspi_device_wait_busy(instance, flexspiConfig, false, baseAddress);
        if (status != kStatus_Success)
        {
            break;
        }

        status = flexspi_nand_check_error_status(instance, config, baseAddress);
        if (status != kStatus_Success)
        {
            break;
        }

        if (!config->bypassEccRead)
        {
            status = flexspi_nand_check_ecc_status(instance, config, baseAddress, &isEccCheckPassed);
            if (status != kStatus_Success)
            {
                break;
            }

            if (!isEccCheckPassed)
            {
                status = kStatus_FlexSPINAND_EccCheckFail;
            }
        }

    } while (0);

    return status;
}

status_t flexspi_nand_erase(uint32_t instance, flexspi_nand_config_t *config, uint32_t pageStart, uint32_t pages)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        uint32_t aligned_block_start = pageStart / config->pagesPerBlock;
        uint32_t aligned_block_end = (pageStart + pages + config->pagesPerBlock - 1) / config->pagesPerBlock;

        while (aligned_block_start < aligned_block_end)
        {
            status = flexspi_nand_erase_block(instance, config, aligned_block_start);
            if (status != kStatus_Success)
            {
                return status;
            }
        }

    } while (0);

    return status;
}

status_t flexspi_nand_program_page(
    uint32_t instance, flexspi_nand_config_t *config, uint32_t pageId, uint32_t *src, uint32_t length)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((config == NULL) || (src == NULL) || (length < 1))
        {
            break;
        }

        uint32_t baseAddress;
        flexspi_xfer_t flashXfer;
        bool isEccCheckPassed = false;
        flexspi_mem_config_t *flexspiConfig = &config->memConfig;
        baseAddress = pageId * config->pageTotalSize;

        flexspi_nand_write_enable(instance, config, baseAddress);

        // Send Page load command
        flashXfer.baseAddress = baseAddress;
        flashXfer.isParallelModeEnable = false;
        flashXfer.txBuffer = src;
        flashXfer.txSize = length;

        flashXfer.operation = kFlexSpiOperation_Write;
        flashXfer.seqId = NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD;
        flashXfer.seqNum = 1;
        if (flexspiConfig->lutCustomSeqEnable && flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_PROGRAMLOAD].seqNum)
        {
            flashXfer.seqId = flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_PROGRAMLOAD].seqId;
            flashXfer.seqNum = flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_PROGRAMLOAD].seqNum;
        }

        // If device has multiple planes and current page is in odd block
        if (config->hasMultiPlanes && ((pageId / config->pagesPerBlock) & 1))
        {
            flashXfer.seqId = NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD_ODD;

            if (flexspiConfig->lutCustomSeqEnable && flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_PROGRAMLOAD_ODD].seqNum)
            {
                flashXfer.seqId = flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_PROGRAMLOAD_ODD].seqId;
                flashXfer.seqNum = flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_PROGRAMLOAD_ODD].seqNum;
            }
        }

        flexspi_update_lut(instance, NAND_CMD_LUT_FOR_IP_CMD, &config->memConfig.lookupTable[4 * flashXfer.seqId],
                           flashXfer.seqNum);
        flashXfer.seqId = NAND_CMD_LUT_FOR_IP_CMD;
        status = flexspi_command_xfer(instance, &flashXfer);
        if (status != kStatus_Success)
        {
            status = kStatus_FlexSPINAND_PageLoadFail;
            break;
        }

        // Send Program Execute command
        flashXfer.operation = kFlexSpiOperation_Command;
        flashXfer.seqId = NAND_CMD_LUT_SEQ_IDX_PROGRAMEXECUTE;
        flashXfer.seqNum = 1;
        if (flexspiConfig->lutCustomSeqEnable)
        {
            flashXfer.seqId = flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_PROGRAMEXECUTE].seqId;
            flashXfer.seqNum = flexspiConfig->lutCustomSeq[NAND_CMD_INDEX_PROGRAMEXECUTE].seqNum;
        }
        flexspi_update_lut(instance, NAND_CMD_LUT_FOR_IP_CMD, &config->memConfig.lookupTable[4 * flashXfer.seqId],
                           flashXfer.seqNum);
        flashXfer.seqId = NAND_CMD_LUT_FOR_IP_CMD;
        status = flexspi_command_xfer(instance, &flashXfer);
        if (status != kStatus_Success)
        {
            status = kStatus_FlexSPINAND_PageExecuteFail;
            break;
        }

        // Wait until the program operation is complete.
        status = flexspi_device_wait_busy(instance, flexspiConfig, false, baseAddress);
        if (status != kStatus_Success)
        {
            break;
        }

        status = flexspi_nand_check_error_status(instance, config, baseAddress);
        if (status != kStatus_Success)
        {
            break;
        }

        if (!config->bypassEccRead)
        {
            status = flexspi_nand_check_ecc_status(instance, config, baseAddress, &isEccCheckPassed);
            if (status != kStatus_Success)
            {
                break;
            }

            if (!isEccCheckPassed)
            {
                status = kStatus_FlexSPINAND_EccCheckFail;
            }
        }

    } while (0);

    return status;
}

status_t flexspi_nand_verify_id(uint32_t instance, serial_nand_config_option_t *option)
{
    status_t status = kStatus_InvalidArgument;
    uint32_t id = 0;

    flexspi_xfer_t flashXfer;
    flashXfer.baseAddress = 0;
    flashXfer.isParallelModeEnable = false;
    flashXfer.operation = kFlexSpiOperation_Read;
    flashXfer.rxBuffer = &id;
    flashXfer.rxSize = 2;
    flashXfer.seqId = NAND_CMD_LUT_FOR_IP_CMD;
    flashXfer.seqNum = 1;

    do
    {
        status = flexspi_command_xfer(instance, &flashXfer);
        if (status != kStatus_Success)
        {
            break;
        }

        uint32_t manufacturer_id = id & 0xFF;

        status = kStatus_FlexSPINAND_NotFound;

        if ((manufacturer_id == 0xFF) || (manufacturer_id == 0x00))
        {
            break;
        }
        if ((option->option0.B.option_size > 0) && (option->option1.B.manufacturer_id > 0) &&
            (manufacturer_id != option->option1.B.manufacturer_id))
        {
            break;
        }
        status = kStatus_Success;

    } while (0);

    return status;
}

status_t flexspi_nand_get_config(uint32_t instance, flexspi_nand_config_t *config, serial_nand_config_option_t *option)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((config == NULL) || (option == NULL))
        {
            break;
        }

        if (option->option0.B.tag != kSerialNandOption_Tag)
        {
            break;
        }

        memset(config, 0, sizeof(flexspi_nand_config_t));

        config->memConfig.tag = FLEXSPI_CFG_BLK_TAG;
        config->memConfig.version = FLEXSPI_CFG_BLK_VERSION;
        config->memConfig.deviceType = kFlexSpiDeviceType_SerialNAND;
        config->memConfig.csHoldTime = 3;
        config->memConfig.csSetupTime = 3;
        config->memConfig.commandInterval = 50;
        config->memConfig.sflashPadType = kSerialFlash_4Pads;
        config->memConfig.readSampleClkSrc = kFlexSPIReadSampleClk_LoopbackInternally;
        config->memConfig.serialClkFreq = kFlexSpiSerialClk_SafeFreq;
        config->memConfig.sflashA1Size = 128U * 1024 * 1024 * 2; // Default size: 1Gbit

        if (option->option0.B.option_size > 0)
        {
            // Switch to second pinmux group
            if (option->option1.B.pinmux_group == 1)
            {
                config->memConfig.controllerMiscOption |= FLEXSPI_BITMASK(kFlexSpiMiscOffset_SecondPinMux);
            }

            if (option->option1.B.flash_connection)
            {
                uint32_t flashConnection = option->option1.B.flash_connection;

                switch (flashConnection)
                {
                    default:
                    case kSerialNandConnection_PortA:
                        // This is default setting, do nothing here
                        break;
                    case kSerialNandConnection_PortB:
                        config->memConfig.sflashA1Size = 0;
                        config->memConfig.sflashB1Size = 128U * 1024 * 1024 * 2; // Default size: 1Gbit;
                        break;
                }
            }
        }

        status = flexspi_nand_init(instance, config);
        if (status != kStatus_Success)
        {
            break;
        }

        status = flexspi_nand_software_reset(instance, config, option);
        if (status != kStatus_Success)
        {
            break;
        }

        // Generate Read ID sequence
        uint32_t readIdSeq[4];
        memset(readIdSeq, 0, sizeof(readIdSeq));
        readIdSeq[0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, kSerialNandCmd_ReadId, CMD_SDR, FLEXSPI_1PAD, 0x0),
        readIdSeq[1] = FLEXSPI_LUT_SEQ(READ_SDR, FLEXSPI_1PAD, 0x02, STOP, FLEXSPI_1PAD, 0);
        flexspi_update_lut(instance, NAND_CMD_LUT_FOR_IP_CMD, readIdSeq, 1);

        // Probe device presence by verifying Manufacturer ID
        status = flexspi_nand_verify_id(instance, option);
        if (status != kStatus_Success)
        {
            break;
        }

        config->memConfig.readSampleClkSrc = kFlexSPIReadSampleClk_LoopbackFromDqsPad;
        config->memConfig.serialClkFreq = option->option0.B.max_freq;

        uint32_t page_size_in_kb = option->option0.B.page_size_in_kb;
        if ((page_size_in_kb != kSerialNandPageSize_2KB) && (page_size_in_kb != kSerialNandPageSize_4KB))
        {
            break;
        }
        config->pageDataSize = 1024UL * page_size_in_kb;
        config->pageTotalSize = 2 * config->pageDataSize;
        uint32_t pages_per_block = option->option0.B.pages_per_block;
        switch (pages_per_block)
        {
            case kSerialNandPagesPerBlock_64:
                pages_per_block = 64;
                break;
            case kSerialNandPagesPerBlock_128:
                pages_per_block = 128;
                break;
            case kSerialNandPagesPerBlock_256:
                pages_per_block = 256;
                break;
            case kSerialNandPagesPerBlock_32:
                pages_per_block = 32;
                break;
            default:
                break;
        }
        if (pages_per_block < 32)
        {
            break;
        }
        config->pagesPerBlock = pages_per_block;

        if (config->memConfig.sflashB1Size > 0)
        {
            config->blocksPerDevice = config->memConfig.sflashB1Size / config->pageTotalSize / config->pagesPerBlock;
        }
        else
        {
            config->blocksPerDevice = config->memConfig.sflashA1Size / config->pageTotalSize / config->pagesPerBlock;
        }

        if (config->pageDataSize == 4096)
        {
            config->memConfig.columnAddressWidth = 13;
        }
        else
        {
            config->memConfig.columnAddressWidth = 12;
        }

        uint32_t flash_size = option->option0.B.flash_size;
        switch (flash_size)
        {
            case 0:
                flash_size = 512U * 1024 * 1024 / 8;
                break;
            case 1:
                flash_size = 1024U * 1024 * 1024 / 8;
                break;
            case 2:
                flash_size = 2048U * 1024 * 1024 / 8;
                break;
            case 4:
                flash_size = 4096U * 1024 * 1024 / 8;
                break;
            case 8:
                flash_size = 8192U * 1024 * 1024 / 8;
                break;
            default:
                break;
        }
        // Invalid Flash size
        if (flash_size < 512)
        {
            break;
        }
        // The size is the actual size * 2
        if (config->memConfig.sflashB1Size > 0)
        {
            config->memConfig.sflashB1Size = flash_size * 2;
        }
        else
        {
            config->memConfig.sflashA1Size = flash_size * 2;
        }

        if (option->option0.B.device_type == kSerialNandCfgOption_DeviceType_Octal)
        {
            config->memConfig.sflashPadType = kSerialFlash_8Pads;
            config->memConfig.readSampleClkSrc = kFlexSPIReadSampleClk_ExternalInputFromDqsPad;

            // Write Enable
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_WRITEENABLE] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x06, STOP, FLEXSPI_8PAD, 0);

            // Read Status
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READSTATUS + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x0F, CMD_SDR, FLEXSPI_8PAD, 0xC0);
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READSTATUS + 1] =
                FLEXSPI_LUT_SEQ(DUMMY_DDR, FLEXSPI_8PAD, 0x0E,READ_DDR, FLEXSPI_8PAD, 0x01);

            // Read Page
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READPAGE + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x13, RADDR_DDR, FLEXSPI_8PAD, 0x10);

            // Page Program Execute
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMEXECUTE] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x10, RADDR_DDR, FLEXSPI_8PAD, 0x10);

            // Erase Block
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_ERASEBLOCK] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0xD8, RADDR_DDR, FLEXSPI_8PAD, 0x10);

            // Read ECC status
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READECCSTAT] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x0F, CMD_SDR, FLEXSPI_8PAD, 0xC0);
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READECCSTAT + 1] =
                FLEXSPI_LUT_SEQ(DUMMY_DDR, FLEXSPI_8PAD, 0x0E,READ_DDR, FLEXSPI_8PAD, 0x01);

            // Read cache 8X
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READCACHE + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x9D, CADDR_DDR, FLEXSPI_8PAD, 0x10);
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READCACHE + 1] =
                FLEXSPI_LUT_SEQ(DUMMY_DDR, FLEXSPI_8PAD, 0x10, READ_DDR, FLEXSPI_8PAD, 0x80);

            // Page Program Load 8x
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0xC2, CADDR_DDR, FLEXSPI_8PAD, 0x10);
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD + 1] =
                FLEXSPI_LUT_SEQ(WRITE_DDR, FLEXSPI_8PAD, 0x40, STOP, FLEXSPI_8PAD, 0);

            // Enable OPI mode
            config->memConfig.deviceModeCfgEnable = true;
            config->memConfig.deviceModeType = kDeviceConfigCmdType_Spi2Xpi;
            config->memConfig.deviceModeArg = 0xE7; // Octal DDR mode
            config->memConfig.deviceModeSeq.seqId = 7;
            config->memConfig.deviceModeSeq.seqNum = 2;
            config->memConfig.waitTimeCfgCommands = 1;

            // Write enable
            config->memConfig.lookupTable[4 * 7 + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x06, STOP, FLEXSPI_1PAD, 0);
            // Write Volatile register
            config->memConfig.lookupTable[4 * 8 + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x81, CMD_SDR, FLEXSPI_1PAD, 0x00);
            config->memConfig.lookupTable[4 * 8 + 1] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x00, CMD_SDR, FLEXSPI_1PAD, 0x00);
            config->memConfig.lookupTable[4 * 8 + 2] =
                FLEXSPI_LUT_SEQ(WRITE_SDR, FLEXSPI_1PAD, 0x1, STOP, FLEXSPI_1PAD, 0);

            // Enable Command Configuration
            config->memConfig.configCmdEnable = true;

            // Disable All protection bits
            config->memConfig.configCmdSeqs[0].seqId = 2;
            config->memConfig.configCmdSeqs[0].seqNum = 1;
            config->memConfig.lookupTable[4 * 2 + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x1F, CMD_SDR, FLEXSPI_8PAD, 0xA0);
            config->memConfig.lookupTable[4 * 2 + 1] =
                FLEXSPI_LUT_SEQ(WRITE_DDR, FLEXSPI_8PAD, 0x01, STOP, FLEXSPI_8PAD, 0);
            config->memConfig.configCmdArgs[0] = 0;

            // Enable ECC, Enable QE, Enable BUF
            // Note: ECC is bit4, shared by all vendors
            //       BUF is bit3, only supported by Winbond
            //       QE  is bit0, only supported by Macronix
            config->memConfig.configCmdSeqs[1].seqId = 6;
            config->memConfig.configCmdSeqs[1].seqNum = 1;
            config->memConfig.lookupTable[4 * 6 + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x1F, CMD_SDR, FLEXSPI_8PAD, 0xB0);
            config->memConfig.lookupTable[4 * 6 + 1] =
                FLEXSPI_LUT_SEQ(WRITE_DDR, FLEXSPI_8PAD, 0x01, STOP, FLEXSPI_8PAD, 0);
            config->memConfig.configCmdArgs[1] = 0x19;
        }
        else
        {
            // Write Enable
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_WRITEENABLE] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x06, STOP, FLEXSPI_1PAD, 0);

            // Read Status
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READSTATUS + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x0F, CMD_SDR, FLEXSPI_1PAD, 0xC0);
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READSTATUS + 1] =
                FLEXSPI_LUT_SEQ(READ_SDR, FLEXSPI_1PAD, 0x01, STOP, FLEXSPI_1PAD, 0);

            // Read Page
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READPAGE + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x13, RADDR_SDR, FLEXSPI_1PAD, 0x18);

            // Page Program Execute
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMEXECUTE] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x10, RADDR_SDR, FLEXSPI_1PAD, 0x18);

            // Erase Block
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_ERASEBLOCK] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xD8, RADDR_SDR, FLEXSPI_1PAD, 0x18);

            // Read ECC status
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READECCSTAT] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x0F, CMD_SDR, FLEXSPI_1PAD, 0xC0);
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READECCSTAT + 1] =
                FLEXSPI_LUT_SEQ(READ_SDR, FLEXSPI_1PAD, 0x01, STOP, FLEXSPI_1PAD, 0);

            // Read cache 4X
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READCACHE + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x6B, CADDR_SDR, FLEXSPI_1PAD, 0x10);
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READCACHE + 1] =
                FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_4PAD, 0x08, READ_SDR, FLEXSPI_4PAD, 0x80);

            // Page Program Load 4x
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x32, CADDR_SDR, FLEXSPI_1PAD, 0x10);
            config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD + 1] =
                FLEXSPI_LUT_SEQ(WRITE_SDR, FLEXSPI_4PAD, 0x40, STOP, FLEXSPI_1PAD, 0);

            config->hasMultiPlanes = false;
            if (option->option0.B.has_multiplanes)
            {
                // Page Program Load Odd 4x
                config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD_ODD + 0] =
                    FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x32, MODE4_SDR, FLEXSPI_1PAD, 0x01);
                config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD_ODD + 1] =
                    FLEXSPI_LUT_SEQ(CADDR_SDR, FLEXSPI_1PAD, 0x0c, WRITE_SDR, FLEXSPI_4PAD, 0x40);
                // Read cache odd 4X
                config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READCACHE_ODD + 0] =
                    FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x6B, MODE4_SDR, FLEXSPI_1PAD, 0x01);
                config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READCACHE_ODD + 1] =
                    FLEXSPI_LUT_SEQ(CADDR_SDR, FLEXSPI_1PAD, 0x0C, DUMMY_SDR, FLEXSPI_4PAD, 0x08);
                config->memConfig.lookupTable[4 * NAND_CMD_LUT_SEQ_IDX_READCACHE_ODD + 2] =
                    FLEXSPI_LUT_SEQ(READ_SDR, FLEXSPI_4PAD, 0x80, STOP, FLEXSPI_1PAD, 0);

                config->hasMultiPlanes = true;
            }

            // Enable Command Configuration
            config->memConfig.configCmdEnable = true;

            // Disable All protection bits
            config->memConfig.configCmdSeqs[0].seqId = 2;
            config->memConfig.configCmdSeqs[0].seqNum = 1;
            config->memConfig.lookupTable[4 * 2 + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x1F, CMD_SDR, FLEXSPI_1PAD, 0xA0);
            config->memConfig.lookupTable[4 * 2 + 1] =
                FLEXSPI_LUT_SEQ(WRITE_SDR, FLEXSPI_1PAD, 0x01, STOP, FLEXSPI_1PAD, 0);
            config->memConfig.configCmdArgs[0] = 0;

            // Enable ECC, Enable QE, Enable BUF
            // Note: ECC is bit4, shared by all vendors
            //       BUF is bit3, only supported by Winbond
            //       QE  is bit0, only supported by Macronix
            config->memConfig.configCmdSeqs[1].seqId = 6;
            config->memConfig.configCmdSeqs[1].seqNum = 1;
            config->memConfig.lookupTable[4 * 6 + 0] =
                FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x1F, CMD_SDR, FLEXSPI_1PAD, 0xB0);
            config->memConfig.lookupTable[4 * 6 + 1] =
                FLEXSPI_LUT_SEQ(WRITE_SDR, FLEXSPI_1PAD, 0x01, STOP, FLEXSPI_1PAD, 0);
            config->memConfig.configCmdArgs[1] = 0x19;
        }

        bool use_default_ecc_parameters = true;

        config->eccCheckCustomEnable = true;
        if (option->option0.B.option_size > 0)
        {
            if (option->option1.B.eccCheckMask && option->option1.B.eccFailureMask)
            {
                config->eccStatusMask = option->option1.B.eccCheckMask & 0xFF;
                config->eccFailureMask = option->option1.B.eccFailureMask & 0xFF;
                use_default_ecc_parameters = false;
            }
        }

        if (use_default_ecc_parameters)
        {
            config->eccStatusMask = kSerialNand_EccCheckMask;
            config->eccFailureMask = kSerialNand_EccFailureMask;
        }

        config->memConfig.deviceType = kFlexSpiDeviceType_SerialNAND;

    } while (0);

    if (status == kStatus_Success)
    {
        flexspi_set_failsafe_setting(&config->memConfig);
    }

    return status;
}

status_t flexspi_nand_software_reset(uint32_t instance, flexspi_nand_config_t *config, serial_nand_config_option_t *option)
{
    status_t status = kStatus_InvalidArgument;
    flexspi_xfer_t xfer;

    do
    {
        uint32_t lut_seq[8];
        memset(&lut_seq, 0, sizeof(lut_seq));

        xfer.baseAddress = 0;
        xfer.isParallelModeEnable = false;
        xfer.operation = kFlexSpiOperation_Command;
        xfer.seqId = NAND_CMD_LUT_FOR_IP_CMD;

        if (option->option0.B.device_type == kSerialNandCfgOption_DeviceType_Octal)
        {
            // Restore NAND device to the initial state
            lut_seq[0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x66, STOP, FLEXSPI_8PAD, 0);
            lut_seq[4] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x99, STOP, FLEXSPI_8PAD, 0);
            xfer.seqNum = 2;
        }
        else
        {
            lut_seq[0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xFF, STOP, FLEXSPI_1PAD, 0);
            xfer.seqNum = 1;
        }

        flexspi_update_lut(instance, xfer.seqId, &lut_seq[0], xfer.seqNum);
        status = flexspi_command_xfer(instance, &xfer);

        // Delay several ms until device is restored to SPI protocol
        flexspi_sw_delay_us(1000);

    } while (0);

    return status;
}