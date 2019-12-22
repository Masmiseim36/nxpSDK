/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "mmc_memory.h"
#include "bootloader/bootloader.h"
#include "bootloader_common.h"
#include "fsl_card.h"
#include "fsl_clock.h"
#include "fsl_device_registers.h"
#include "fsl_host.h"
#include "memory/memory.h"
#include "property/property.h"

#if BL_FEATURE_GEN_KEYBLOB
#include "bootloader/bl_keyblob.h"
#endif // BL_FEATURE_GEN_KEYBLOB

#if BL_FEATURE_MMC_MODULE
/*******************************************************************************
 * Definitons
 ******************************************************************************/
/*! @brief The common value */
enum
{
    kMMCCardErasedPattern0 = 0x00000000, /*!< The default value of MMC memory bits. */
    kMMCCardErasedPattern1 = 0xFFFFFFFF, /*!< The default value of MMC memory bits when ERASED_MEM_CONT is set */
    kMMCStartAddress = 0x0,              /*!< The default start address of MMC Card. */
#if HOST_DMA_BUFFER_ADDR_ALIGN == USDHC_ADMA1_ADDRESS_ALIGN /*!< ADMA1 is not really supported yet */
    kMMCBufferBlockCount = 8,                               /*!< The MMC memory buffer size in block count. */
#elif HOST_DMA_BUFFER_ADDR_ALIGN == USDHC_ADMA2_ADDRESS_ALIGN
    kMMCBufferBlockCount = 1, /*!< The MMC memory buffer size in block count. */
#else
#error Invalid USDHC DMA selection.
#endif
    kMMCBufferSizeInBytes = FSL_SDMMC_DEFAULT_BLOCK_SIZE * kMMCBufferBlockCount, /*!< The MMC memory buffer size. */

    kMMCSize_Max4GB_InKbytes = (4u * 1024 * 1024),

    kMMCConfigTag = 0xC, /*!< The MMC config block tag. */
};

/*! @brief Configuration structure used for MMC memory. */
typedef struct _mmc_config
{
    union
    {
        struct
        {
            uint32_t boot_config_enable : 1;
            uint32_t rsv0 : 1;
            uint32_t boot_ack : 1;
            uint32_t reset_boot_bus_conditions : 1;
            uint32_t boot_mode : 2;
            uint32_t rsv1 : 2;
            uint32_t bus_width : 4;
            uint32_t timing_interface : 4;
            uint32_t boot_bus_width : 2;
            uint32_t rsv2 : 2;
            uint32_t boot_partition_enable : 3;
            uint32_t rsv3 : 1;
            uint32_t partition_access : 3;
            uint32_t rsv4 : 1;
            uint32_t tag : 4;
        } B;
        uint32_t U;
    } word0;

    union
    {
        struct
        {
            uint32_t rsv_perm_config_enable : 2;
            uint32_t rsv0 : 14;
            uint32_t rsv_perm_boot_config_prot : 2;
            uint32_t enable1V8 : 1;
            uint32_t enablePowerCycle : 1;
            uint32_t powerUpTime : 1;
            uint32_t rsv1 : 2;
            uint32_t powerPolarity : 1;
            uint32_t powerDownTime : 2;
            uint32_t rsv2 : 2;
            uint32_t rsv_driver_strength : 4;
        } B;
        uint32_t U;
    } word1;
} mmc_config_t;

/*! @brief Context structure used for MMC memory. */
typedef struct _mmc_mem_context
{
    mmc_card_t mmc;
    bool isConfigured;
    bool isReadBufferValid;
    uint32_t readBufferBlockAddr;
    bool isWriteBufferValid;
    uint32_t writeBufferOffset;
    uint32_t writeBufferBlockAddr;
#if BL_FEATURE_GEN_KEYBLOB
    bool has_keyblob;
    uint32_t keyblob_offset;
#endif // BL_FEATURE_GEN_KEYBLOB
} mmc_mem_context_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Load one block data to the read buffer.
 *
 * @param blockAddr The block address to read.
 * @retval kStatus_SDMMC_CardNotSupport Card not support.
 * @retval kStatus_SDMMC_NotSupportYet Not support now.
 * @retval kStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t mmc_mem_load_buffer(uint32_t blockAddr);
/*!
 * @brief Flush one block data to MMC
 * @retval kStatus_SDMMC_CardNotSupport Card not support.
 * @retval kStatus_SDMMC_NotSupportYet Not support now.
 * @retval kStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t mmc_mem_flush_buffer(void);
/*!
 * @brief Check if the specific memory is erased or not.
 *
 * @retval true Erased.
 * @retval false Not erased.
 */
static bool is_erased_memory(uint32_t blockAddr, uint32_t blockCount, uint32_t erasedPattern);
/*!
 * @brief Check if MMC card is initialized successfully or not.
 *
 * @retval true Initialized.
 * @retval false Un-initialized.
 */
static bool is_mmc_configured(void);
/*!
 * @brief Check if the block to read is already cached in the buffer.
 *
 * @retval true Cached.
 * @retval false Not cached.
 */
static bool is_read_block_cached(uint32_t blockAddr);
/*!
 * @brief Check if the block to write is already cached in the buffer.
 *
 * @retval true Cached.
 * @retval false Not cached.
 */
static bool is_write_block_cached(uint32_t blockAddr);

/*!
 * @brief Get current selected partition's block count.
 *
 * @retval kStatus_InvalidArgument Invalid partition number.
 * @retval kStatus_Success Operate successfully.
 */
static status_t get_current_block_count(mmc_card_t *card, uint32_t *partitionBlocks);

#if BL_FEATURE_GEN_KEYBLOB
static status_t check_update_keyblob_info(void *config);
#endif // BL_FEATURE_GEN_KEYBLOB

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* @brief Context variable used for MMC memory */
static mmc_mem_context_t s_mmcContext = { 0 };

/* @brief Buffer used for MMC memory */
SDK_ALIGN(static uint8_t s_mmc_mem_readBuffer[kMMCBufferSizeInBytes], HOST_DMA_BUFFER_ADDR_ALIGN);
SDK_ALIGN(static uint8_t s_mmc_mem_writeBuffer[kMMCBufferSizeInBytes], HOST_DMA_BUFFER_ADDR_ALIGN);

/* @brief Interface to spi nand memory operations */
const external_memory_region_interface_t g_mmcMemoryInterface = {
    .init = mmc_mem_init,
    .read = mmc_mem_read,
    .write = mmc_mem_write,
    .erase = mmc_mem_erase,
    .config = mmc_mem_config,
    .flush = mmc_mem_flush,
    .finalize = mmc_mem_finalize,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
#if BL_FEATURE_GEN_KEYBLOB
status_t check_update_keyblob_info(void *config)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((config == NULL) || (s_mmcContext.isConfigured == false))
        {
            break;
        }

        // Try to read Key blob info based on config
        keyblob_info_t *keyblob_info = (keyblob_info_t *)config;
        if (keyblob_info->option.B.tag != kKeyBlobInfoOption_Tag)
        {
            break;
        }

        int32_t keyblob_info_type = keyblob_info->option.B.type;
        if ((keyblob_info_type != kKeyBlobInfoType_Program) && (keyblob_info_type != kKeyBlobInfoType_Update))
        {
            break;
        }

        if (keyblob_info_type == kKeyBlobInfoType_Update)
        {
            status = keyblob_update(keyblob_info);
            if (status != kStatus_Success)
            {
                s_mmcContext.has_keyblob = false;
                break;
            }
            s_mmcContext.keyblob_offset = keyblob_info->keyblob_offset;
            s_mmcContext.has_keyblob = true;
        }
        else if (keyblob_info_type == kKeyBlobInfoType_Program)
        {
            if (!s_mmcContext.has_keyblob)
            {
                break;
            }
            uint32_t index = keyblob_info->option.B.image_index;
            if (index != 0)
            {
                break;
            }

            uint32_t image_start = 0;
            uint32_t image_max_size = 0;
            uint32_t block_size;
            status = mmc_get_property(kExternalMemoryPropertyTag_StartAddress, &image_start);
            if (status != kStatus_Success)
            {
                break;
            }
            status = mmc_get_property(kExternalMemoryPropertyTag_MemorySizeInKbytes, &image_max_size);
            if (status != kStatus_Success)
            {
                break;
            }
            if (image_max_size > kMMCSize_Max4GB_InKbytes)
            {
                image_max_size = 0xFFFFFFFFu;
            }
            status = mmc_get_property(kExternalMemoryPropertyTag_BlockSize, &block_size);
            if (status != kStatus_Success)
            {
                break;
            }

            uint32_t keyblob_offset = s_mmcContext.keyblob_offset;
            uint32_t keyblob_addr = image_start + keyblob_offset;
            uint8_t *keyblob_buffer;
            uint32_t keyblob_size;
            status = keyblob_get(&keyblob_buffer, &keyblob_size);
            if (status != kStatus_Success)
            {
                break;
            }

            // Check key blob address range
            if ((keyblob_size + keyblob_offset) > image_max_size)
            {
                break;
            }

            // Invalid key blob address, key blob must be page size aligned.
            if (keyblob_addr & (block_size - 1))
            {
                break;
            }

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
            if (!is_erased_memory(keyblob_addr / block_size,
                                  keyblob_size / block_size + (keyblob_size % block_size ? 1 : 0),
                                  (s_mmcContext.mmc.extendedCsd.eraseMemoryContent == 0x1) ? kMMCCardErasedPattern1 :
                                                                                             kMMCCardErasedPattern0))
            {
                status = kStatusMemoryCumulativeWrite;
                break;
            }
#endif
            status = mmc_mem_write(keyblob_addr, keyblob_size, keyblob_buffer);
            if (status != kStatus_Success)
            {
                break;
            }

            status = mmc_mem_flush();
            if (status != kStatus_Success)
            {
                break;
            }
        }
    } while (0);

    return status;
}
#endif // #if BL_FEATURE_GEN_KEYBLOB

status_t mmc_mem_init(void)
{
    status_t status = kStatus_Success;
    mmc_card_t *card = &s_mmcContext.mmc;
    // Init the basic variable, fill in the uSDHC base address and current frequency.
    card->host.base = BOARD_MMC_HOST_BASEADDR;
    card->host.sourceClock_Hz = MMC_HOST_CLK_FREQ;
    card->hostVoltageWindowVCC = kMMC_VoltageWindows270to360; // Not really used for bootloader.
    card->hostVoltageWindowVCCQ = kMMC_VoltageWindow170to195; // Not really used for bootloader.

    status = get_mmc_default_configuration(card);
    if (status != kStatus_Success)
    {
        // get the default configuration failed. Must init the mmc by configure memory command.
        return status;
    }

    status = MMC_BL_Init(card);
    if (status == kStatus_Success)
    {
        // Update external map entry info.
        uint32_t index;
        status = find_external_map_index(kMemoryMMCCard, &index);
        if (status != kStatus_Success)
        {
            return status;
        }

        uint32_t blockCount;
        status = get_current_block_count(card, &blockCount);
        if (status != kStatus_Success)
        {
            return status;
        }

        g_externalMemoryMap[index].basicUnitSize = card->blockSize;
        g_externalMemoryMap[index].basicUnitCount = blockCount;

        // Once initialization is succeed, MMC card is accessable.
        s_mmcContext.isConfigured = true;
    }
    else
    {
        s_mmcContext.isConfigured = false;
    }

    return status;
}

status_t mmc_mem_config(uint32_t *config)
{
    status_t status = kStatus_Fail;

    uint32_t startAddr = (uint32_t)config;
    uint32_t endAddr = startAddr + sizeof(mmc_config_t) - 1;
    // Should check the config is in valid internal space.
    if ((!is_valid_application_location(startAddr)) || (!is_valid_application_location(endAddr)))
    {
        return kStatus_InvalidArgument;
    }

    const mmc_config_t *mmcConfig = (const mmc_config_t *)config;
#if BL_FEATURE_GEN_KEYBLOB
    keyblob_info_t *keyblob_info = (keyblob_info_t *)config;
    if (keyblob_info->option.B.tag == kKeyBlobInfoOption_Tag)
    {
        status = check_update_keyblob_info(config);
        return status;
    }
    else
#endif // BL_FEATURE_GEN_KEYBLOB

        // Check the tag.
        if (mmcConfig->word0.B.tag != kMMCConfigTag)
    {
        return kStatus_InvalidArgument;
    }

    // Clear the Context.
    memset(&s_mmcContext, 0, sizeof(mmc_mem_context_t));

    mmc_card_t *card = &s_mmcContext.mmc;

    card->host.base = BOARD_MMC_HOST_BASEADDR;
    card->host.sourceClock_Hz = MMC_HOST_CLK_FREQ;
    card->hostVoltageWindowVCC = kMMC_VoltageWindows270to360; // Not really used for bootloader.
    card->hostVoltageWindowVCCQ = kMMC_VoltageWindow170to195; // Not really used for bootloader.

    card->userConfig.timing = (mmc_high_speed_timing_t)mmcConfig->word0.B.timing_interface;
    card->userConfig.busWidth = (mmc_data_bus_width_t)mmcConfig->word0.B.bus_width;
    card->userConfig.enablePowerCycle = mmcConfig->word1.B.enablePowerCycle;
    card->userConfig.powerPolarity = mmcConfig->word1.B.powerPolarity;
    switch (mmcConfig->word1.B.powerDownTime)
    {
        default:
        case kSDMMC_PWR_DOWN_20MS:
            card->userConfig.powerDownDelay_US = 20 * 1000;
            break;
        case kSDMMC_PWR_DOWN_10MS:
            card->userConfig.powerDownDelay_US = 10 * 1000;
            break;
        case kSDMMC_PWR_DOWN_5MS:
            card->userConfig.powerDownDelay_US = 5 * 1000;
            break;
        case kSDMMC_PWR_DOWN_2D5MS:
            card->userConfig.powerDownDelay_US = 25 * 100;
            break;
    }
    switch (mmcConfig->word1.B.powerUpTime)
    {
        default:
        case kSDMMC_PWR_UP_5MS:
            card->userConfig.powerUpDelay_US = 5 * 1000;
            break;
        case kSDMMC_PWR_UP_2D5MS:
            card->userConfig.powerUpDelay_US = 25 * 100;
            break;
    }
    card->userConfig.switch1V8 = mmcConfig->word1.B.enable1V8;

    // Do eMMC init firstly.
    status = MMC_BL_Init(card);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Switching to the selected partition.
    status = MMC_SelectPartition(card, (mmc_access_partition_t)(mmcConfig->word0.B.partition_access));
    if (status != kStatus_Success)
    {
        return status;
    }

    // Re-update memory map to selected partition.
    uint32_t blockCount;
    status = get_current_block_count(card, &blockCount);
    if (status != kStatus_Success)
    {
        return status;
    }

    uint32_t index;
    status = find_external_map_index(kMemoryMMCCard, &index);
    if (status != kStatus_Success)
    {
        return status;
    }
    g_externalMemoryMap[index].basicUnitSize = card->blockSize;
    g_externalMemoryMap[index].basicUnitCount = blockCount;

    // Check if boot mode configuration is needed.
    if (mmcConfig->word0.B.boot_config_enable)
    {
        mmc_boot_config_t mmcBootConfig;

        mmcBootConfig.bootBusWidth = (mmc_boot_bus_width_t)mmcConfig->word0.B.boot_bus_width;
        mmcBootConfig.bootMode = (mmc_boot_mode_t)mmcConfig->word0.B.boot_mode;
        mmcBootConfig.bootPartition = (mmc_boot_partition_enable_t)mmcConfig->word0.B.boot_partition_enable;
        mmcBootConfig.enableBootAck = mmcConfig->word0.B.boot_ack == 1 ? true : false;
        mmcBootConfig.retainBootBusWidth = mmcConfig->word0.B.reset_boot_bus_conditions == 1 ? true : false;

        // Do eMMC boot mode configuration.
        status = MMC_SetBootConfig(&s_mmcContext.mmc, &mmcBootConfig);
        if (status != kStatus_Success)
        {
            return status;
        }
    }
#if BL_FEATURE_MMC_MODULE_ENABLE_PERMANENT_CONFIG
// Will be added in the future.
#endif
    s_mmcContext.isConfigured = true;

    return status;
}

status_t mmc_mem_read(uint32_t address, uint32_t length, uint8_t *restrict buffer)
{
    assert(buffer);

    status_t status = kStatus_Success;

    // MMC should be initialized before access.
    if (!is_mmc_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    uint32_t bufferSize = kMMCBufferSizeInBytes; // Use CPU reg
    uint32_t bufferOffset = address % bufferSize;
    uint32_t blockAddr = address / FSL_SDMMC_DEFAULT_BLOCK_SIZE; // Not use "card->blockSize" here.

    uint32_t readLength;

    while (length)
    {
        // Check if current block to read is already read to readbuffer.
        // If no, need to read the whole block to buffer.
        if (!is_read_block_cached(blockAddr))
        {
            // Read the page to buffer.
            status = mmc_mem_load_buffer(blockAddr);
            if (status != kStatus_Success)
            {
                return status;
            }
        }
        else
        {
            bufferOffset = address - s_mmcContext.readBufferBlockAddr * FSL_SDMMC_DEFAULT_BLOCK_SIZE;
        }

        // If it is a read accoss the block, divide it into two steps.
        if (bufferOffset + length <= bufferSize)
        {
            readLength = length;
        }
        else
        {
            readLength = bufferSize - bufferOffset;
        }
        memcpy(buffer, &s_mmc_mem_readBuffer[bufferOffset], readLength);
        length -= readLength;
        buffer += readLength;
        bufferOffset += readLength;
        if (bufferOffset >= bufferSize)
        {
            bufferOffset -= bufferSize;
            blockAddr += kMMCBufferBlockCount;
        }
    }

    return kStatus_Success;
}
status_t mmc_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    assert(buffer);

    status_t status = kStatus_Success;

    // MMC should be initialized before access.
    if (!is_mmc_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    uint32_t bufferSize = kMMCBufferSizeInBytes; // Use CPU reg
    uint32_t bufferOffset = address % bufferSize;
    uint32_t blockAddr = address / FSL_SDMMC_DEFAULT_BLOCK_SIZE; // Not use "card->blockSize" here.

    uint32_t writeLength;

    while (length)
    {
        // Check if current block to write is already cached to writebuffer.
        // If no, need to init the writebuffer.
        if (!is_write_block_cached(blockAddr))
        {
            // There is data already cached in the buffer, flush it to MMC.
            if (s_mmcContext.isWriteBufferValid)
            {
                status = mmc_mem_flush_buffer();
                if (status != kStatus_Success)
                {
                    return status;
                }
            }
            // Start a new block write. The address must block size aligned.
            if (bufferOffset != 0)
            {
                return kStatusMemoryAlignmentError;
            }
            s_mmcContext.writeBufferOffset = bufferOffset;
            s_mmcContext.writeBufferBlockAddr = blockAddr;
            s_mmcContext.isWriteBufferValid = true;
        }
        else
        {
            bufferOffset = address - s_mmcContext.writeBufferBlockAddr * FSL_SDMMC_DEFAULT_BLOCK_SIZE;
        }

        // If the address is not continuous, start a new block write.
        if (s_mmcContext.writeBufferOffset != bufferOffset)
        {
            status = mmc_mem_flush_buffer();
            if (status != kStatus_Success)
            {
                return status;
            }
            continue;
        }

        if (bufferOffset + length <= bufferSize)
        {
            writeLength = length;
        }
        else
        {
            writeLength = bufferSize - bufferOffset;
        }
        memcpy(&s_mmc_mem_writeBuffer[bufferOffset], buffer, writeLength);
        s_mmcContext.writeBufferOffset += writeLength;
        length -= writeLength;
        buffer += writeLength;
        bufferOffset += writeLength;
        if (bufferOffset >= bufferSize)
        {
            bufferOffset -= bufferSize;
            blockAddr += kMMCBufferBlockCount;
        }
    }

    return kStatus_Success;
}
status_t mmc_mem_flush(void)
{
    status_t status = kStatus_Success;
    // If there still is data in the buffer, then flush them to MMC.
    if (s_mmcContext.isWriteBufferValid)
    {
        status = mmc_mem_flush_buffer();
    }
    return status;
}
status_t mmc_mem_finalize(void)
{
    status_t status = kStatus_Success;

    // Mark buffer to invalid.
    s_mmcContext.isWriteBufferValid = false;
    s_mmcContext.isReadBufferValid = false;

    return status;
}
status_t mmc_mem_erase(uint32_t address, uint32_t length)
{
    status_t status;
    mmc_card_t *card = &s_mmcContext.mmc;

    // MMC should be initialized before access.
    if (!is_mmc_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    if (MMC_CheckReadOnly(card))
    {
        return kStatusMemoryWriteProtected;
    }

    // length = 0 means no erase operation will be executed. Just return success.
    if (length == 0)
    {
        return kStatus_Success;
    }
    // else means 1 block at lest to be erased.

    uint32_t startGroupAddr =
        address / card->blockSize / card->eraseGroupBlocks; // Not use FSL_SDMMC_DEFAULT_BLOCK_SIZE here
    uint32_t endGroupAddr =
        (address + length - 1) / card->blockSize / card->eraseGroupBlocks; // Not use FSL_SDMMC_DEFAULT_BLOCK_SIZE here

    status = MMC_EraseGroups(card, startGroupAddr, endGroupAddr);
    if (status != kStatus_Success)
    {
        return status;
    }
#if 0
    // Indeed, no need to check if the memory is erased. Card will guarantee it.
    if (!is_erased_memory(
            startGroupAddr * card->eraseGroupBlocks, (endGroupAddr - startGroupAddr) * card->eraseGroupBlocks,
            (card->extendedCsd.eraseMemoryContent == 0x1) ? kMMCCardErasedPattern1 : kMMCCardErasedPattern0))
    {
        return kStatusMemoryVerifyFailed;
    }
#endif
    return status;
}
status_t mmc_mem_erase_all(void)
{
    status_t status;
    mmc_card_t *card = &s_mmcContext.mmc;
    // SPI NAND should be configured before access.
    if (!is_mmc_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    if (MMC_CheckReadOnly(card))
    {
        return kStatusMemoryWriteProtected;
    }

    uint32_t blockCount;
    status = get_current_block_count(card, &blockCount);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = MMC_EraseGroups(card, kMMCStartAddress, blockCount / card->eraseGroupBlocks);
    if (status != kStatus_Success)
    {
        return status;
    }
#if 0
    // Indeed, no need to check if the memory is erased. Card will guarantee it.
    if (!is_erased_memory(
            kMMCStartAddress, blockCount * card->blockSize / FSL_SDMMC_DEFAULT_BLOCK_SIZE,
            (card->extendedCsd.eraseMemoryContent == 0x1) ? kMMCCardErasedPattern1 : kMMCCardErasedPattern0))
    {
        return kStatusMemoryVerifyFailed;
    }
#endif
    return status;
}
status_t mmc_get_property(uint32_t whichProperty, uint32_t *value)
{
    mmc_card_t *card = &s_mmcContext.mmc;
    status_t status;
    uint32_t blockCount;

    if (value == NULL)
    {
        return kStatus_InvalidArgument;
    }

    switch (whichProperty)
    {
        case kExternalMemoryPropertyTag_InitStatus:
            *value = is_mmc_configured() ? kStatus_Success : kStatusMemoryNotConfigured;
            break;

        case kExternalMemoryPropertyTag_StartAddress:
            *value = kMMCStartAddress;
            break;

        case kExternalMemoryPropertyTag_MemorySizeInKbytes:
            status = get_current_block_count(card, &blockCount);
            if (status != kStatus_Success)
            {
                return status;
            }
            *value = (uint32_t)((uint64_t)blockCount * card->blockSize / 1024);
            break;

        case kExternalMemoryPropertyTag_BlockSize:
            *value = card->blockSize;
            break;

        default: // catch inputs that are not recognized
            return kStatus_InvalidArgument;
    }

    return kStatus_Success;
}

static status_t mmc_mem_load_buffer(uint32_t blockAddr)
{
    status_t status;

    s_mmcContext.isReadBufferValid = false; // Mark read buffer invalid.

    // Read the page to read buffer.
    status = MMC_ReadBlocks(&s_mmcContext.mmc, s_mmc_mem_readBuffer, blockAddr, kMMCBufferBlockCount);
    if (status == kStatus_Success)
    {
        s_mmcContext.isReadBufferValid = true;
        s_mmcContext.readBufferBlockAddr = blockAddr;
    }
    return status;
}

static status_t mmc_mem_flush_buffer(void)
{
    status_t status = kStatus_Success;
    mmc_card_t *card = &s_mmcContext.mmc;

    if (MMC_CheckReadOnly(card))
    {
        return kStatusMemoryWriteProtected;
    }

    // Fill up the left bytes.
    if (s_mmcContext.writeBufferOffset != kMMCBufferSizeInBytes)
    {
        memset(&s_mmc_mem_writeBuffer[s_mmcContext.writeBufferOffset],
               (card->extendedCsd.eraseMemoryContent == 0x1) ? kMMCCardErasedPattern1 : kMMCCardErasedPattern0,
               kMMCBufferSizeInBytes - s_mmcContext.writeBufferOffset);
    }

    s_mmcContext.isWriteBufferValid = false;

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
    if (!is_erased_memory(
            s_mmcContext.writeBufferBlockAddr, kMMCBufferBlockCount,
            (card->extendedCsd.eraseMemoryContent == 0x1) ? kMMCCardErasedPattern1 : kMMCCardErasedPattern0))
    {
        return kStatusMemoryCumulativeWrite;
    }
#endif // #if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE

    // Flush the data in the write buffer to MMC
    status = MMC_WriteBlocks(card, s_mmc_mem_writeBuffer, s_mmcContext.writeBufferBlockAddr, kMMCBufferBlockCount);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Indeed, no need to check if the memory is written correct or not. Card will guarantee it.
    // But still do it to make sure user's data written without any incorrect.
    status = MMC_ReadBlocks(card, s_mmc_mem_readBuffer, s_mmcContext.writeBufferBlockAddr, kMMCBufferBlockCount);
    if (status != kStatus_Success)
    {
        return status;
    }

    if (memcmp(s_mmc_mem_writeBuffer, s_mmc_mem_readBuffer, kMMCBufferSizeInBytes))
    {
        return kStatusMemoryVerifyFailed;
    }

    return status;
}

static bool is_erased_memory(uint32_t blockAddr, uint32_t blockCount, uint32_t erasedPattern)
{
    status_t status = kStatus_Success;
    mmc_card_t *card = &s_mmcContext.mmc;
    uint32_t offset, *buffer, readblock;
    while (blockCount)
    {
        if (blockCount > kMMCBufferBlockCount)
        {
            readblock = kMMCBufferBlockCount;
        }
        else
        {
            readblock = blockCount;
        }
        // Read the page firstly.
        status = MMC_ReadBlocks(card, s_mmc_mem_readBuffer, blockAddr, readblock);
        if (status != kStatus_Success)
        {
            // If read failed, return false.
            return false;
        }

        offset = 0;
        buffer = (uint32_t *)s_mmc_mem_readBuffer;
        while (offset < (readblock * FSL_SDMMC_DEFAULT_BLOCK_SIZE))
        {
            // Check if all 0xFFs or all 0x00s
            if (*buffer != erasedPattern)
            {
                return false;
            }
            buffer++;
            offset += 4;
        }
        blockCount -= readblock;
    }
    return true;
}

static bool is_mmc_configured(void)
{
    return s_mmcContext.isConfigured;
}

static bool is_read_block_cached(uint32_t blockAddr)
{
    return (s_mmcContext.isReadBufferValid) && (s_mmcContext.readBufferBlockAddr <= blockAddr) &&
           (s_mmcContext.readBufferBlockAddr + kMMCBufferBlockCount > blockAddr);
}

static bool is_write_block_cached(uint32_t blockAddr)
{
    return (s_mmcContext.isWriteBufferValid) && (s_mmcContext.writeBufferBlockAddr <= blockAddr) &&
           (s_mmcContext.writeBufferBlockAddr + kMMCBufferBlockCount > blockAddr);
}

static status_t get_current_block_count(mmc_card_t *card, uint32_t *partitionBlocks)
{
    status_t status = kStatus_Success;
    switch (card->currentPartition)
    {
        case kMMC_AccessPartitionUserAera:
        {
            *partitionBlocks = card->userPartitionBlocks;
            break;
        }
        case kMMC_AccessPartitionBoot1:
        case kMMC_AccessPartitionBoot2:
        {
            /* Boot partition 1 and partition 2 have the same partition size. */
            *partitionBlocks = card->bootPartitionBlocks;
            break;
        }
        case kMMC_AccessGeneralPurposePartition1:
        {
            *partitionBlocks = card->generalPartitionSize[0];
            break;
        }
        case kMMC_AccessGeneralPurposePartition2:
        {
            *partitionBlocks = card->generalPartitionSize[1];
            break;
        }
        case kMMC_AccessGeneralPurposePartition3:
        {
            *partitionBlocks = card->generalPartitionSize[2];
            break;
        }
        case kMMC_AccessGeneralPurposePartition4:
        {
            *partitionBlocks = card->generalPartitionSize[3];
            break;
        }
        default:
            status = kStatus_InvalidArgument;
            break;
    }
    return status;
}
#endif // #if BL_FEATURE_MMC_MODULE
