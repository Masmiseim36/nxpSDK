/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "sd_memory.h"
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

#if BL_FEATURE_SD_MODULE
/*******************************************************************************
 * Definitons
 ******************************************************************************/
/*! @brief The common value */
enum
{
    kSDCardErasedPattern0 = 0x00000000, /*!< The default value of SD memory bits. */
    kSDCardErasedPattern1 = 0xFFFFFFFF, /*!< The default value of SD memory bits when DATA_STAT_AFTER_ERASE is set */
    kSDStartAddress = 0x0,              /*!< The default start address of SD Card. */
#if HOST_DMA_BUFFER_ADDR_ALIGN == USDHC_ADMA1_ADDRESS_ALIGN /*!< ADMA1 is not really supported yet */
    kSDBufferBlockCount = 8,                                /*!< The SD memory buffer size in block count. */
#elif HOST_DMA_BUFFER_ADDR_ALIGN == USDHC_ADMA2_ADDRESS_ALIGN
    kSDBufferBlockCount = 1, /*!< The SD memory buffer size in block count. */
#else
#error Invalid USDHC DMA selection.
#endif
    kSDBufferSizeInBytes = FSL_SDMMC_DEFAULT_BLOCK_SIZE * kSDBufferBlockCount, /*!< The SD memory buffer size. */

    kSDConfigTag = 0xD,                          /*!< The SD config block tag. */
    kSDSize_Max4GB_InKbytes = (4 * 1024 * 1024), /* 4GB space in KBytes */
};

/*! @brief Configuration structure used for SD memory. */
typedef struct _sd_config
{
    union
    {
        struct
        {
            uint32_t rsv0 : 8;
            uint32_t bus_width : 1;
            uint32_t rsv1 : 3;
            uint32_t timing_interface : 3;
            uint32_t rsv2 : 4;
            uint32_t enablePowerCycle : 1;
            uint32_t powerUpTime : 1;
            uint32_t rsv3 : 2;
            uint32_t powerPolarity : 1;
            uint32_t powerDownTime : 2;
            uint32_t rsv4 : 2;
            uint32_t tag : 4;
        } B;
        uint32_t U;
    } word0;
} sd_config_t;

/*! @brief Context structure used for SD memory. */
typedef struct _sd_mem_context
{
    sd_card_t sd;
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
} sd_mem_context_t;

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
static status_t sd_mem_load_buffer(uint32_t blockAddr);
/*!
 * @brief Flush one block data to SD
 * @retval kStatus_SDMMC_CardNotSupport Card not support.
 * @retval kStatus_SDMMC_NotSupportYet Not support now.
 * @retval kStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t sd_mem_flush_buffer(void);
/*!
 * @brief Check if the specific memory is erased or not.
 *
 * @retval true Erased.
 * @retval false Not erased.
 */
static bool is_erased_memory(uint32_t blockAddr, uint32_t blockCount, uint32_t erasedPattern);
/*!
 * @brief Check if SD card is initialized successfully or not.
 *
 * @retval true Initialized.
 * @retval false Un-initialized.
 */
static bool is_sd_configured(void);
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

#if BL_FEATURE_GEN_KEYBLOB
/*!
 * @brief Check and update keyblob info as needed.
 */
static status_t check_update_keyblob_info(void *config);
#endif // BL_FEATURE_GEN_KEYBLOB

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* @brief Context variable used for SD memory */
static sd_mem_context_t s_sdContext = { 0 };

/* @brief Buffer used for SD memory */
SDK_ALIGN(static uint8_t s_sd_mem_readBuffer[kSDBufferSizeInBytes], HOST_DMA_BUFFER_ADDR_ALIGN);
SDK_ALIGN(static uint8_t s_sd_mem_writeBuffer[kSDBufferSizeInBytes], HOST_DMA_BUFFER_ADDR_ALIGN);

/* @brief Interface to spi nand memory operations */
const external_memory_region_interface_t g_sdMemoryInterface = {
    .init = sd_mem_init,
    .read = sd_mem_read,
    .write = sd_mem_write,
    .erase = sd_mem_erase,
    .config = sd_mem_config,
    .flush = sd_mem_flush,
    .finalize = sd_mem_finalize,
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
        if ((config == NULL) || (s_sdContext.isConfigured == false))
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
                s_sdContext.has_keyblob = false;
                break;
            }
            s_sdContext.keyblob_offset = keyblob_info->keyblob_offset;
            s_sdContext.has_keyblob = true;
        }
        else if (keyblob_info_type == kKeyBlobInfoType_Program)
        {
            if (!s_sdContext.has_keyblob)
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
            status = sd_get_property(kExternalMemoryPropertyTag_StartAddress, &image_start);
            if (status != kStatus_Success)
            {
                break;
            }
            status = sd_get_property(kExternalMemoryPropertyTag_MemorySizeInKbytes, &image_max_size);
            if (status != kStatus_Success)
            {
                break;
            }
            if (image_max_size > kSDSize_Max4GB_InKbytes)
            {
                image_max_size = 0xFFFFFFFFu;
            }
            status = sd_get_property(kExternalMemoryPropertyTag_BlockSize, &block_size);
            if (status != kStatus_Success)
            {
                break;
            }

            uint32_t keyblob_offset = s_sdContext.keyblob_offset;
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
                                  s_sdContext.sd.scr.flags & kSD_ScrDataStatusAfterErase ? kSDCardErasedPattern1 :
                                                                                           kSDCardErasedPattern0))
            {
                status = kStatusMemoryCumulativeWrite;
                break;
            }
#endif
            status = sd_mem_write(keyblob_addr, keyblob_size, keyblob_buffer);
            if (status != kStatus_Success)
            {
                break;
            }

            status = sd_mem_flush();
            if (status != kStatus_Success)
            {
                break;
            }
        }
    } while (0);

    return status;
}
#endif // #if BL_FEATURE_GEN_KEYBLOB

status_t sd_mem_init(void)
{
    status_t status = kStatus_Success;
    sd_card_t *card = &s_sdContext.sd;
    // Init the basic variable, fill in the uSDHC base address and current frequency.
    card->host.base = BOARD_SD_HOST_BASEADDR;
    card->host.sourceClock_Hz = SD_HOST_CLK_FREQ;

    status = get_sd_default_configuration(card);
    if (status != kStatus_Success)
    {
        // get the default configuration failed. Must init the mmc by configure memory command.
        return status;
    }

    status = SD_BL_Init(card);
    if (status == kStatus_Success)
    {
        // Note: blockSize doesn't need to be checked here.
        // Driver uses multi-block read/write.
        // Then the block size will always be 512 bytes during read/write operation.
        // if ((card->blockSize > kSDBufferSizeInBytes) || ((kSDBufferSizeInBytes % card->blockSize) != 0))
        //{
        //    return kStatusMemoryNotSupported;
        //}

        // Update external map entry info.
        uint32_t index;
        status = find_external_map_index(kMemorySDCard, &index);
        if (status != kStatus_Success)
        {
            return status;
        }
        g_externalMemoryMap[index].basicUnitSize = card->blockSize;
        g_externalMemoryMap[index].basicUnitCount = card->blockCount;

        // Once initialization is succeed, SD card is accessable.
        s_sdContext.isConfigured = true;
    }
    else
    {
        s_sdContext.isConfigured = false;
    }

    return status;
}

status_t sd_mem_config(uint32_t *config)
{
    status_t status = kStatus_Fail;

    uint32_t startAddr = (uint32_t)config;
    uint32_t endAddr = startAddr + sizeof(sd_config_t) - 1;
    // Should check the config is in valid internal space.
    if ((!is_valid_application_location(startAddr)) || (!is_valid_application_location(endAddr)))
    {
        return kStatus_InvalidArgument;
    }

    const sd_config_t *sdConfig = (const sd_config_t *)config;

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
        if (sdConfig->word0.B.tag != kSDConfigTag)
    {
        return kStatus_InvalidArgument;
    }

    // Clear the Context.
    memset(&s_sdContext, 0, sizeof(sd_mem_context_t));

    sd_card_t *card = &s_sdContext.sd;

    card->host.base = BOARD_SD_HOST_BASEADDR;
    card->host.sourceClock_Hz = SD_HOST_CLK_FREQ;

    card->userConfig.timing = (sd_timing_mode_t)sdConfig->word0.B.timing_interface;
    card->userConfig.busWidth = (sd_data_bus_width_t)sdConfig->word0.B.bus_width;
    card->userConfig.enablePowerCycle = sdConfig->word0.B.enablePowerCycle;
    card->userConfig.powerPolarity = sdConfig->word0.B.powerPolarity;
    switch (sdConfig->word0.B.powerDownTime)
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
    switch (sdConfig->word0.B.powerUpTime)
    {
        default:
        case kSDMMC_PWR_UP_5MS:
            card->userConfig.powerUpDelay_US = 5 * 1000;
            break;
        case kSDMMC_PWR_UP_2D5MS:
            card->userConfig.powerUpDelay_US = 25 * 100;
            break;
    }

    status = SD_BL_Init(card);
    if (status == kStatus_Success)
    {
        // Note: blockSize doesn't need to be checked here.
        // Driver uses multi-block read/write.
        // Then the block size will always be 512 bytes during read/write operation.
        // if ((card->blockSize > kSDBufferSizeInBytes) || ((kSDBufferSizeInBytes % card->blockSize) != 0))
        //{
        //    return kStatusMemoryNotSupported;
        //}

        // Update external map entry info.
        uint32_t index;
        status = find_external_map_index(kMemorySDCard, &index);
        if (status != kStatus_Success)
        {
            return status;
        }
        g_externalMemoryMap[index].basicUnitSize = card->blockSize;
        g_externalMemoryMap[index].basicUnitCount = card->blockCount;

        // Once initialization is succeed, SD card is accessable.
        s_sdContext.isConfigured = true;
    }
    else
    {
        s_sdContext.isConfigured = false;
    }

    return status;
}

status_t sd_mem_read(uint32_t address, uint32_t length, uint8_t *restrict buffer)
{
    assert(buffer);

    status_t status = kStatus_Success;

    // SD should be initialized before access.
    if (!is_sd_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    uint32_t bufferSize = kSDBufferSizeInBytes; // Use CPU reg.
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
            status = sd_mem_load_buffer(blockAddr);
            if (status != kStatus_Success)
            {
                return status;
            }
        }
        else
        {
            // If the block is cached in the buffer, then need to change the buffer offset.
            // Buffer might has multi blocks. And the dest might not locate at the first block.
            bufferOffset = address - s_sdContext.readBufferBlockAddr * FSL_SDMMC_DEFAULT_BLOCK_SIZE;
        }

        // If it is a read accoss the block, divide it into two steps.
        if ((bufferOffset + length) <= bufferSize)
        {
            readLength = length;
        }
        else
        {
            readLength = bufferSize - bufferOffset;
        }
        memcpy(buffer, &s_sd_mem_readBuffer[bufferOffset], readLength);
        length -= readLength;
        buffer += readLength;
        bufferOffset += readLength;
        if (bufferOffset >= bufferSize)
        {
            bufferOffset -= bufferSize;
            blockAddr += kSDBufferBlockCount;
        }
    }

    return kStatus_Success;
}

status_t sd_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    assert(buffer);

    status_t status = kStatus_Success;

    // SD should be initialized before access.
    if (!is_sd_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    uint32_t bufferSize = kSDBufferSizeInBytes; // Use CPU reg.
    uint32_t bufferOffset = address % bufferSize;
    uint32_t blockAddr = address / FSL_SDMMC_DEFAULT_BLOCK_SIZE; // Not use "card->blockSize" here.

    uint32_t writeLength;

    while (length)
    {
        // Check if current block to write is already cached to writebuffer.
        // If no, need to init the writebuffer.
        if (!is_write_block_cached(blockAddr))
        {
            // There is data already cached in the buffer, flush it to SD.
            if (s_sdContext.isWriteBufferValid)
            {
                status = sd_mem_flush_buffer();
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
            s_sdContext.writeBufferOffset = bufferOffset;
            s_sdContext.writeBufferBlockAddr = blockAddr;
            s_sdContext.isWriteBufferValid = true;
        }
        else
        {
            // If the block is cached in the buffer, then need to change the buffer offset.
            // Buffer might has multi blocks. And the dest might not locate at the first block.
            bufferOffset = address - s_sdContext.writeBufferBlockAddr * FSL_SDMMC_DEFAULT_BLOCK_SIZE;
        }

        // If the address is not continuous, start a new block write.
        if (s_sdContext.writeBufferOffset != bufferOffset)
        {
            status = sd_mem_flush_buffer();
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
        memcpy(&s_sd_mem_writeBuffer[bufferOffset], buffer, writeLength);
        s_sdContext.writeBufferOffset += writeLength;
        length -= writeLength;
        buffer += writeLength;
        bufferOffset += writeLength;
        if (bufferOffset >= bufferSize)
        {
            bufferOffset -= bufferSize;
            blockAddr += kSDBufferBlockCount;
        }
    }

    return kStatus_Success;
}
status_t sd_mem_flush(void)
{
    status_t status = kStatus_Success;
    // If there still is data in the buffer, then flush them to SD.
    if (s_sdContext.isWriteBufferValid)
    {
        status = sd_mem_flush_buffer();
    }
    return status;
}
status_t sd_mem_finalize(void)
{
    // Mark buffer to invalid.
    s_sdContext.isWriteBufferValid = false;
    s_sdContext.isReadBufferValid = false;

    return kStatus_Success;
}
status_t sd_mem_erase(uint32_t address, uint32_t length)
{
    status_t status;
    sd_card_t *card = &s_sdContext.sd;

    // SD should be initialized before access.
    if (!is_sd_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    if (SD_CheckReadOnly(card))
    {
        return kStatusMemoryWriteProtected;
    }

    // length = 0 means no erase operation will be executed. Just return success.
    if (length == 0)
    {
        return kStatus_Success;
    }
    // else means 1 block at lest to be erased.

    uint32_t startBlockAddr = address / FSL_SDMMC_DEFAULT_BLOCK_SIZE; // Not use "card->blockSize" here.
    // Don't get block count from length. Address to address + length might across block boundary.
    uint32_t blockCount = (address + length - 1) / FSL_SDMMC_DEFAULT_BLOCK_SIZE - startBlockAddr + 1;

    status = SD_EraseBlocks(card, startBlockAddr, blockCount);
    if (status != kStatus_Success)
    {
        return status;
    }
#if 0
    // Indeed, no need to check if the memory is erased. Card will guarantee it.
    if (!is_erased_memory(startBlockAddr, blockCount, (card->scr.flags & kSD_ScrDataStatusAfterErase) ?
                                                          kSDCardErasedPattern1 :
                                                          kSDCardErasedPattern0))
    {
        return kStatusMemoryVerifyFailed;
    }
#endif
    return status;
}
status_t sd_mem_erase_all(void)
{
    status_t status;
    sd_card_t *card = &s_sdContext.sd;
    // SD should be configured before access.
    if (!is_sd_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    if (SD_CheckReadOnly(card))
    {
        return kStatusMemoryWriteProtected;
    }
    status =
        SD_EraseBlocks(card, 0, (uint64_t)card->blockCount * (uint64_t)card->blockSize / FSL_SDMMC_DEFAULT_BLOCK_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
#if 0
    // Indeed, no need to check if the memory is erased. Card will guarantee it.
    if (!is_erased_memory(
            0, (uint64_t)card->blockCount * (uint64_t)card->blockSize / FSL_SDMMC_DEFAULT_BLOCK_SIZE,
            (card->scr.flags & kSD_ScrDataStatusAfterErase) ? kSDCardErasedPattern1 : kSDCardErasedPattern0))
    {
        return kStatusMemoryVerifyFailed;
    }
#endif
    return status;
}
status_t sd_get_property(uint32_t whichProperty, uint32_t *value)
{
    sd_card_t *card = &s_sdContext.sd;

    if (value == NULL)
    {
        return kStatus_InvalidArgument;
    }

    switch (whichProperty)
    {
        case kExternalMemoryPropertyTag_InitStatus:
            *value = is_sd_configured() ? kStatus_Success : kStatusMemoryNotConfigured;
            break;

        case kExternalMemoryPropertyTag_StartAddress:
            *value = kSDStartAddress;
            break;

        case kExternalMemoryPropertyTag_MemorySizeInKbytes:
            *value = (uint32_t)((uint64_t)card->blockCount * card->blockSize / 1024);
            break;

        case kExternalMemoryPropertyTag_BlockSize:
            *value = card->blockSize;
            break;

        default: // catch inputs that are not recognized
            return kStatus_InvalidArgument;
    }

    return kStatus_Success;
}

static status_t sd_mem_load_buffer(uint32_t blockAddr)
{
    status_t status;

    s_sdContext.isReadBufferValid = false; // Mark read buffer invalid.

    // Read the page to read buffer.
    status = SD_ReadBlocks(&s_sdContext.sd, s_sd_mem_readBuffer, blockAddr, kSDBufferBlockCount);
    if (status == kStatus_Success)
    {
        s_sdContext.isReadBufferValid = true;
        s_sdContext.readBufferBlockAddr = blockAddr;
    }
    return status;
}

static status_t sd_mem_flush_buffer(void)
{
    status_t status = kStatus_Success;
    sd_card_t *card = &s_sdContext.sd;

    if (SD_CheckReadOnly(card))
    {
        return kStatusMemoryWriteProtected;
    }

    // Fill up the left bytes.
    if (s_sdContext.writeBufferOffset != kSDBufferSizeInBytes)
    {
        memset(&s_sd_mem_writeBuffer[s_sdContext.writeBufferOffset],
               (card->scr.flags & kSD_ScrDataStatusAfterErase) ? kSDCardErasedPattern1 : kSDCardErasedPattern0,
               kSDBufferSizeInBytes - s_sdContext.writeBufferOffset);
    }

    s_sdContext.isWriteBufferValid = false;

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
    if (!is_erased_memory(
            s_sdContext.writeBufferBlockAddr, kSDBufferBlockCount,
            (card->scr.flags & kSD_ScrDataStatusAfterErase) ? kSDCardErasedPattern1 : kSDCardErasedPattern0))
    {
        return kStatusMemoryCumulativeWrite;
    }
#endif // #if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE

    // Flush the data in the write buffer to SD
    status = SD_WriteBlocks(card, s_sd_mem_writeBuffer, s_sdContext.writeBufferBlockAddr, kSDBufferBlockCount);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Indeed, no need to check if the memory is written correct or not. Card will guarantee it.
    // But still do it to make sure user's data written without any incorrect.
    status = SD_ReadBlocks(card, s_sd_mem_readBuffer, s_sdContext.writeBufferBlockAddr, kSDBufferBlockCount);
    if (status != kStatus_Success)
    {
        return status;
    }

    if (memcmp(s_sd_mem_writeBuffer, s_sd_mem_readBuffer, kSDBufferSizeInBytes))
    {
        return kStatusMemoryVerifyFailed;
    }

    return status;
}

static bool is_erased_memory(uint32_t blockAddr, uint32_t blockCount, uint32_t erasedPattern)
{
    status_t status = kStatus_Success;
    sd_card_t *card = &s_sdContext.sd;
    uint32_t offset, *buffer, readblock;
    while (blockCount)
    {
        if (blockCount > kSDBufferBlockCount)
        {
            readblock = kSDBufferBlockCount;
        }
        else
        {
            readblock = blockCount;
        }

        // Read the page firstly.
        status = SD_ReadBlocks(card, s_sd_mem_readBuffer, blockAddr, readblock);
        if (status != kStatus_Success)
        {
            // If read failed, return false.
            return false;
        }

        offset = 0;
        buffer = (uint32_t *)s_sd_mem_readBuffer;
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

static bool is_sd_configured(void)
{
    return s_sdContext.isConfigured;
}

static bool is_read_block_cached(uint32_t blockAddr)
{
    return (s_sdContext.isReadBufferValid) && (s_sdContext.readBufferBlockAddr <= blockAddr) &&
           ((s_sdContext.readBufferBlockAddr + kSDBufferBlockCount) > blockAddr);
}

static bool is_write_block_cached(uint32_t blockAddr)
{
    return (s_sdContext.isWriteBufferValid) && (s_sdContext.writeBufferBlockAddr == blockAddr) &&
           ((s_sdContext.writeBufferBlockAddr + kSDBufferBlockCount) > blockAddr);
}
#endif // #if BL_FEATURE_SD_MODULE
