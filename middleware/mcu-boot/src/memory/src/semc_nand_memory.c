/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bootloader.h"
#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "memory/memory.h"
#include "semc_nand/semc_nand_flash.h"
#include "semc_nand_memory.h"
#include <string.h>
#include "bootloader/bl_context.h"
#include "crc/crc32.h"
#include "utilities/fsl_assert.h"
#include "utilities/fsl_rtos_abstraction.h"
#if BL_FEATURE_GEN_KEYBLOB
#include "bootloader/bl_keyblob.h"
#endif // BL_FEATURE_GEN_KEYBLOB

#if BL_FEATURE_SEMC_NAND_MODULE
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

enum
{
    kSemcNandMemInfo_StartAddress = 0,
};

typedef enum _semc_bcb_type
{
    kSemcNandBootControlBlockType_FCB = 0U,
    kSemcNandBootControlBlockType_DBBT = 1U,
} semc_bcb_type_t;

//! @brief SEMC NAND memory feature inforamation
//!
//! An instance is maintained in this file, will is used to keep key information for write and flush
//! operatations.
typedef struct _semc_nand_mem_context
{
    bool isConfigured;
    bool needToUpdateDbbt;

    // About pageReadBuffer
    uint8_t readPageBuffer[SEMC_NAND_PHYSICAL_PAGE_SIZE_MAX];
    bool hasValidReadPageData;
    uint32_t readPageIndex;

    // About pageWriteBuffer
    uint8_t writePageBuffer[SEMC_NAND_PHYSICAL_PAGE_SIZE_MAX];
    bool hasValidWritePageData;
    uint32_t cachedBytesToWrite;
    uint32_t writePageIndex;

    uint32_t skippedBlockCount;

#if BL_FEATURE_GEN_KEYBLOB
    bool has_keyblob;
    uint32_t keyblob_offset; //!< Key blob offset in application image
#endif                       // BL_FEATURE_GEN_KEYBLOB
} semc_nand_mem_context_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

// Release readPageBuffer for verification use
static void semc_nand_release_page_read_buffer_for_verify(void);

// Load one page from NAND Device to readPageBuffer
static status_t semc_nand_load_one_page_to_buffer(uint32_t pageIndex);

// Flush one page from writePageBuffer to NAND device
static status_t semc_nand_flush_one_page_from_buffer(void);

// Backup all flushed pages from current bad block to next good NAND block
static status_t semc_nand_backup_flushed_pages_from_bad_block(uint32_t srcPageIndex, uint32_t destBlockIndex);

/* Write and verify SEMC NAND memory */
static status_t semc_nand_write_verify_memory(uint32_t address, uint32_t length, uint8_t *buffer);

/* Erase and verify SEMC NAND block */
static status_t semc_nand_erase_verify_block(uint32_t blockIndex);

/* Validate the BCB data readback from SEMC NAND */
static bool semc_nand_validate_bcb(semc_bcb_type_t bcbType, uint32_t *nandBcb);

/* Search BCB(FCB,DBBT) data from Parallel NAND via SEMC */
static status_t semc_nand_search_bcb(semc_bcb_type_t bcbType,
                                     uint32_t startPage,
                                     uint32_t readCount,
                                     uint32_t searchStride);

/* Update BCB(FCB,DBBT) data to Parallel NAND via SEMC */
static status_t semc_nand_update_bcb(semc_bcb_type_t bcbType);

// Determine if a page is in bad block
static bool is_semc_nand_page_in_bad_block(uint32_t pageIndex);

// Calculate crc32 for nand data
static uint32_t semc_nand_calculate_crc32(uint8_t *start, uint32_t length);

// Determine if SEMC NAND is configured
static bool is_semc_nand_configured(void);

// Create empty NAND fcb data
static status_t semc_nand_creat_empty_fcb(void);

// Create empty NAND dbbt data
static status_t semc_nand_creat_empty_dbbt(void);

// Update external memory map for SEMC NAND
static status_t semc_nand_update_memory_map(void);

// Record discovered bad block to DBBT and mark DBBT staus that need to be updated
static status_t semc_nand_record_bad_block(uint32_t blockIndex);

// Skip bad blocks before read page
static void semc_nand_skip_bad_blocks(uint32_t *pageIndex);

// Determine if block to read has been read once
static bool has_semc_nand_block_been_read_once(uint32_t blockIndex);

// Determine if page to read is cached in readPageBuffer
static bool is_semc_nand_read_page_cached(uint32_t pageIndex);

// Determine if block to read has been written once
static bool has_semc_nand_block_been_written_once(uint32_t blockIndex);

// Determine if page to write is cached in writePageBuffer
static bool is_semc_nand_write_page_cached(uint32_t pageIndex);

#if BL_FEATURE_GEN_KEYBLOB
static status_t check_update_keyblob_info(void *config);
#endif // BL_FEATURE_GEN_KEYBLOB

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief FCB of SEMC parallel nand.
static semc_nand_fcb_t s_semcNandFcb;
//! @brief DBBT of SEMC parallel nand.
static semc_nand_dbbt_t s_semcNandDbbt;
//! @brief context of SEMC parallel nand.
static semc_nand_mem_context_t s_semcNandContext = {
    .isConfigured = false,
    .needToUpdateDbbt = false,
    .hasValidReadPageData = false,
    .hasValidWritePageData = false,
    .skippedBlockCount = 0,
};
//! @brief Interface to semc nand memory operations
const external_memory_region_interface_t g_semcNandMemoryInterface = {
    .init = semc_nand_mem_init,
    .read = semc_nand_mem_read,
    .write = semc_nand_mem_write,
    .erase = semc_nand_mem_erase,
    .config = semc_nand_mem_config,
    .flush = semc_nand_mem_flush,
    .finalize = semc_nand_mem_finalize,
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

//! @brief Initialize SEMC NAND memory
status_t semc_nand_mem_init(void)
{
    status_t status;
    uint32_t searchCount, searchStride;

    // Init device only if current boot device is SEMC NAND
    if (get_primary_boot_device() != kBootDevice_SemcNAND)
    {
        return kStatus_Fail;
    }

    // A default FCB with fingerprint and version should be created
    semc_nand_creat_empty_fcb();

    // Get default config block from efuse.
    status = semc_nand_get_default_config_block(&s_semcNandFcb.nandConfig);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Init SEMC peripheral to enable FCB read.
    status = semc_nand_flash_init(&s_semcNandFcb.nandConfig);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Get FCB search parameters from efuse
    semc_nand_get_fcb_search_cfg(&searchCount, &searchStride);

    // Search FCB from NAND device
    status = semc_nand_search_bcb(kSemcNandBootControlBlockType_FCB, 0, searchCount, searchStride);
    if (status != kStatus_Success)
    {
        return status;
    }
    else
    {
        // Find FCB and use it
        memcpy(&s_semcNandFcb, &s_semcNandContext.readPageBuffer, sizeof(semc_nand_fcb_t));
    }

    // Re-Init SEMC peripheral using new FCB found from Device.
    status = semc_nand_flash_init(&s_semcNandFcb.nandConfig);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Search DBBT from NAND device
    status = semc_nand_search_bcb(kSemcNandBootControlBlockType_DBBT, s_semcNandFcb.DBBTSerachAreaStartPage,
                                  searchCount, searchStride);
    if (status != kStatus_Success)
    {
        // Do not create a new DBBT during init.
        // There is a risk, that the old DBBT is crashed.
        // User needs to re-configure.
        return status;
    }
    else
    {
        // Find DBBT and use it
        memcpy(&s_semcNandDbbt, &s_semcNandContext.readPageBuffer, sizeof(semc_nand_dbbt_t));
    }

    // All initialization steps are success. SEMC NAND can be accessable.
    s_semcNandContext.isConfigured = true;

    // Update external map entry info.
    status = semc_nand_update_memory_map();

    return status;
}

#if BL_FEATURE_GEN_KEYBLOB
status_t check_update_keyblob_info(void *config)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((config == NULL) || (s_semcNandContext.isConfigured == false))
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
                s_semcNandContext.has_keyblob = false;
                break;
            }
            s_semcNandContext.keyblob_offset = keyblob_info->keyblob_offset;
            s_semcNandContext.has_keyblob = true;
        }
        else if (keyblob_info_type == kKeyBlobInfoType_Program)
        {
            if (!s_semcNandContext.has_keyblob)
            {
                break;
            }
            uint32_t index = keyblob_info->option.B.image_index;
            if (index >= SEMC_NAND_FW_MAX_NUM)
            {
                status = kStatus_InvalidArgument;
                break;
            }

            uint32_t image_page_start = s_semcNandFcb.firmwareTable[index].startPage;
            uint32_t image_max_page_size = s_semcNandFcb.firmwareTable[index].pagesInFirmware;

            uint32_t page_size = s_semcNandFcb.nandConfig.bytesInPageDataArea;
            uint32_t keyblob_offset = s_semcNandContext.keyblob_offset;
            uint32_t keyblob_addr = image_page_start * page_size + keyblob_offset;
            uint8_t *keyblob_buffer;
            uint32_t keyblob_size;
            status = keyblob_get(&keyblob_buffer, &keyblob_size);
            if (status != kStatus_Success)
            {
                break;
            }

            // Check key blob address range
            if ((keyblob_size + keyblob_offset) / page_size > image_max_page_size)
            {
                status = kStatus_InvalidArgument;
                break;
            }

            // Invalid key blob address, key blob must be page size aligned.
            if (keyblob_addr & (page_size - 1))
            {
                status = kStatus_InvalidArgument;
                break;
            }

            uint32_t keyblob_page_index = keyblob_addr / page_size;
#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
            if (semc_nand_flash_verify_erase(&s_semcNandFcb.nandConfig, keyblob_page_index, 1) != kStatus_Success)
            {
                status = kStatusMemoryCumulativeWrite;
                break;
            }
#endif
            status = semc_nand_mem_write(keyblob_addr, keyblob_size, keyblob_buffer);
            if (status != kStatus_Success)
            {
                break;
            }

            status = semc_nand_mem_flush();
        }
    } while (0);

    return status;
}
#endif // #if BL_FEATURE_GEN_KEYBLOB

//! @brief Configure SEMC NAND memory
status_t semc_nand_mem_config(uint32_t *config)
{
    status_t status;
    uint32_t startAddr = (uint32_t)config;
    uint32_t endAddr = startAddr + sizeof(semc_nand_fcb_t) - 1;
    // Should check the config is in valid internal space.
    if ((!is_valid_application_location(startAddr)) || (!is_valid_application_location(endAddr)))
    {
        return kStatus_InvalidArgument;
    }

    // Try to get FCB based on an option
    semc_nand_img_option_t *nandImgOption = (semc_nand_img_option_t *)config;
    
#if BL_FEATURE_GEN_KEYBLOB
    keyblob_info_t *keyblob_info = (keyblob_info_t *)config;
#endif // BL_FEATURE_GEN_KEYBLOB

    // Validate user FCB
    if (semc_nand_validate_bcb(kSemcNandBootControlBlockType_FCB, config))
    {
        // All FCB check is passed. Then start to configure SEMC NAND.
        // Mark SEMC NAND as not configured.
        s_semcNandContext.isConfigured = false;

        // Over-write FCB.
        memcpy(&s_semcNandFcb, config, sizeof(semc_nand_fcb_t));

        // Re-Init SEMC peripheral using user FCB.
        status = semc_nand_flash_init(&s_semcNandFcb.nandConfig);
        if (status != kStatus_Success)
        {
            return status;
        }
    }
    else if (nandImgOption->nandOption.option.B.tag == kParallelNandConfigOption_Tag)
    {
        uint32_t minImageBlockIndex;
        uint32_t searchStride;
        uint32_t searchCount =
            (nandImgOption->bcbOption.B.searchCount != 0) ? nandImgOption->bcbOption.B.searchCount : 1;
        uint32_t imageCopies = (nandImgOption->bcbOption.B.imgCopies != 0) ? nandImgOption->bcbOption.B.imgCopies : 1;
        nandImgOption->nandOption.option.B.ioPortDiv8 =
            (nandImgOption->nandOption.option.B.ioPortDiv8 != 0) ? nandImgOption->nandOption.option.B.ioPortDiv8 : 1;
        nandImgOption->bcbOption.B.searchStrideBlk =
            (nandImgOption->bcbOption.B.searchStrideBlk != 0) ? nandImgOption->bcbOption.B.searchStrideBlk : 1;

        // Validate ONFI version
        if (nandImgOption->nandOption.option.B.onfiVersion < kNandOnfiVersion_1p0)
        {
            return kStatus_SemcNAND_OnlySupportOnfiDevice;
        }

        // Validate FCB option
        if (imageCopies > SEMC_NAND_FW_MAX_NUM)
        {
            return kStatus_SemcNAND_MoreThanMaxImageCopy;
        }

        // Mark SEMC NAND as not configured.
        s_semcNandContext.isConfigured = false;

        // A default FCB with fingerprint and version should be created
        semc_nand_creat_empty_fcb();

        // Get default config block.
        status = semc_nand_get_default_config_block(&s_semcNandFcb.nandConfig);
        if (status != kStatus_Success)
        {
            return status;
        }

        // Default FCB(config block) shouldn't depend on eFUSE setting
        s_semcNandFcb.nandConfig.readyCheckOption = kSemcNandReadyCheckOption_SR;
        s_semcNandFcb.nandConfig.memConfig.asyncClkFreq = SEMC_2ND_MAX_CLK_FREQ;
        s_semcNandFcb.nandConfig.statusCommandType = kSemcNandStatusCommandType_Common;
        s_semcNandFcb.nandConfig.memConfig.accessCommandType = kSemcAccessCommandType_IPBUSCMD;
        // s_semcNandFcb.nandConfig.memConfig.nandMemConfig.rdyPortPolarity = kSemcPortPloarity_LowActive;
        // s_semcNandFcb.nandConfig.memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_5byte_CA2RA3;
        // s_semcNandFcb.nandConfig.memConfig.nandMemConfig.columnAddressWidth = 12;
        // s_semcNandFcb.nandConfig.pagesInBlock = 64;

        // Configure nand memory according to Option
        s_semcNandFcb.nandConfig.onfiVersion = nandImgOption->nandOption.option.B.onfiVersion;
        s_semcNandFcb.nandConfig.memConfig.nandMemConfig.edoMode = nandImgOption->nandOption.option.B.edoMode;
        s_semcNandFcb.nandConfig.acTimingTableIndex = nandImgOption->nandOption.option.B.onfiTimingMode + 1;
        s_semcNandFcb.nandConfig.memConfig.nandMemConfig.ioPortWidth =
            nandImgOption->nandOption.option.B.ioPortDiv8 * 8;
        s_semcNandFcb.nandConfig.memConfig.nandMemConfig.cePortOutputSelection =
            nandImgOption->nandOption.option.B.pcsSelection;
        s_semcNandFcb.nandConfig.eccCheckType = nandImgOption->nandOption.option.B.eccType;
        s_semcNandFcb.nandConfig.deviceEccStatus = nandImgOption->nandOption.option.B.eccStatus;

        // Re-Init SEMC peripheral using new FCB.
        status = semc_nand_flash_init(&s_semcNandFcb.nandConfig);
        if (status != kStatus_Success)
        {
            return status;
        }

        searchStride = nandImgOption->bcbOption.B.searchStrideBlk * s_semcNandFcb.nandConfig.pagesInBlock;

        // Image must start after FCB + DBBT region
        minImageBlockIndex = nandImgOption->bcbOption.B.searchStrideBlk * searchCount * 2;

        // Validate image option
        for (uint32_t i = 0; i < imageCopies; i++)
        {
            // Do parameter check here
            if (i != 0)
            {
                minImageBlockIndex =
                    nandImgOption->imageInfo[i - 1].blockIndex + nandImgOption->imageInfo[i - 1].blockCount;
            }

            if (nandImgOption->imageInfo[i].blockIndex < minImageBlockIndex)
            {
                return kStatus_SemcNAND_DisorderedImageCopies;
            }

            s_semcNandFcb.firmwareTable[i].pagesInFirmware =
                nandImgOption->imageInfo[i].blockCount * s_semcNandFcb.nandConfig.pagesInBlock;
            s_semcNandFcb.firmwareTable[i].startPage =
                nandImgOption->imageInfo[i].blockIndex * s_semcNandFcb.nandConfig.pagesInBlock;
        }

        // Configure FCB
        s_semcNandFcb.searchStride = searchStride;
        s_semcNandFcb.searchCount = searchCount;
        s_semcNandFcb.DBBTSerachAreaStartPage = searchCount * searchStride;
        s_semcNandFcb.firmwareCopies = imageCopies;
    }
#if BL_FEATURE_GEN_KEYBLOB
    else if (keyblob_info->option.B.tag == kKeyBlobInfoOption_Tag)
    {
        status = check_update_keyblob_info(config);
        if (status != kStatus_Success)
        {
            return status;
        }
    }
#endif
    else
    {
        return kStatus_SemcNAND_InvalidCfgTag;
    }

    // Load DBBT from device according to user FCB.
    status = semc_nand_search_bcb(kSemcNandBootControlBlockType_DBBT, s_semcNandFcb.DBBTSerachAreaStartPage,
                                  s_semcNandFcb.searchCount, s_semcNandFcb.searchStride);
    if (status != kStatus_Success)
    {
        // Load failure means a new dbbt need to be created.
        semc_nand_creat_empty_dbbt();
        // Update new DBBT to SEMC NAND.
        status = semc_nand_update_bcb(kSemcNandBootControlBlockType_DBBT);
        if (status != kStatus_Success)
        {
            return status;
        }
    }
    else
    {
        // Find DBBT and use it
        memcpy(&s_semcNandDbbt, &s_semcNandContext.readPageBuffer, sizeof(semc_nand_dbbt_t));
    }

    // Update new FCB to SEMC NAND
    status = semc_nand_update_bcb(kSemcNandBootControlBlockType_FCB);
    if (status == kStatus_Success)
    {
        // All configuration steps are success. SEMC NAND can be accessable.
        s_semcNandContext.isConfigured = true;

        // Update external map entry info.
        status = semc_nand_update_memory_map();
    }

    return status;
}

//! @brief Read SEMC NAND memory.
status_t semc_nand_mem_read(uint32_t address, uint32_t length, uint8_t *restrict buffer)
{
    status_t status;
    uint32_t pageSize = s_semcNandFcb.nandConfig.bytesInPageDataArea;
    uint32_t pageColumnAddress = address % pageSize;
    ;
    uint32_t pageIndex = address / pageSize;
    uint32_t readoutBytes;
    // Validate the buffer parameter
    if (buffer == NULL)
    {
        return kStatus_InvalidArgument;
    }

    // SEMC NAND should be configured before access.
    if (!is_semc_nand_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    // Skip the skipped blocks during a read operation.
    // No need to change the pageColumnAddress.
    semc_nand_skip_bad_blocks(&pageIndex);

    while (length)
    {
        // Check if current page to read is already read to readPageBuffer.
        // If no, need to read the whole page to buffer.
        if (!is_semc_nand_read_page_cached(pageIndex))
        {
            // Check if the page to read and cached page is in the same block.
            // If no, need to check if the block is a bad block.
            uint32_t blockIndex = pageIndex / s_semcNandFcb.nandConfig.pagesInBlock;
            if (!has_semc_nand_block_been_read_once(blockIndex))
            {
                // Due to skipping bad blocks, blockIndex might cross the end boundary.
                // Need to check the range.
                if (blockIndex >= s_semcNandFcb.nandConfig.blocksInPlane * s_semcNandFcb.nandConfig.planesInDevice)
                {
                    return kStatusMemoryRangeInvalid;
                }
                // Check if reading a bad block.
                if (is_semc_nand_page_in_bad_block(pageIndex))
                {
                    // If yes, skip the bad block and read the next block.
                    s_semcNandContext.skippedBlockCount++;
                    pageIndex += s_semcNandFcb.nandConfig.pagesInBlock;
                    continue;
                }
            }

            // Good block and not cached, then read the page to buffer.
            status = semc_nand_load_one_page_to_buffer(pageIndex);
            if (status != kStatus_Success)
            {
                break;
            }
        }
        // If it is a read accoss the page, divide it into two steps.
        if (pageColumnAddress + length <= pageSize)
        {
            readoutBytes = length;
        }
        else
        {
            readoutBytes = pageSize - pageColumnAddress;
        }
        memcpy(buffer, &s_semcNandContext.readPageBuffer[pageColumnAddress], readoutBytes);
        length -= readoutBytes;
        buffer += readoutBytes;
        pageColumnAddress += readoutBytes;
        if (pageColumnAddress >= pageSize)
        {
            pageColumnAddress -= pageSize;
            pageIndex++;
        }
        // Mark current loop is successfully executed.
        status = kStatus_Success;
    }

    return status;
}

//! @brief Write SEMC NAND memory.
status_t semc_nand_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    status_t status;
    uint32_t pageSize = s_semcNandFcb.nandConfig.bytesInPageDataArea;
    uint32_t pageColumnAddress = address % pageSize;
    ;
    uint32_t pageIndex = address / pageSize;
    uint32_t writeBytes;
    // Validate the buffer parameter
    if (buffer == NULL)
    {
        return kStatus_InvalidArgument;
    }

    // SEMC NAND should be configured before access.
    if (!is_semc_nand_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    // Ensure DBBT and FCB cannot be overwritedn by normal write operation
    if (pageIndex < s_semcNandFcb.firmwareTable[0].startPage)
    {
        return kStatus_SemcNAND_DisallowOverwriteBcb;
    }

    // Skip the skipped blocks during a read operation.
    // No need to change the columnAddr.
    semc_nand_skip_bad_blocks(&pageIndex);

    while (length)
    {
        // Check if current page to write is already cached to writePagebuffer.
        // If no, need to init the writePagebuffer.
        if (!is_semc_nand_write_page_cached(pageIndex))
        {
            uint32_t blockIndex = pageIndex / s_semcNandFcb.nandConfig.pagesInBlock;
            // Check if the page to write and cached page is in the same block.
            // If no, need to check if the block is a bad block.
            if (!has_semc_nand_block_been_written_once(blockIndex))
            {
                // Due to skipping bad blocks, blockAddr might cross the end boundary.
                // Need to check the range.
                if (blockIndex >= s_semcNandFcb.nandConfig.blocksInPlane * s_semcNandFcb.nandConfig.planesInDevice)
                {
                    return kStatusMemoryRangeInvalid;
                }
                // Check if writting a bad block.
                if (is_semc_nand_page_in_bad_block(pageIndex))
                {
                    // If yes, skip the bad block and write to the next block.
                    s_semcNandContext.skippedBlockCount++;
                    pageIndex += s_semcNandFcb.nandConfig.pagesInBlock;
                    continue;
                }
            }
            // There is data already cached in the buffer, flush it to SEMC NAND.
            if (s_semcNandContext.hasValidWritePageData)
            {
                status = semc_nand_flush_one_page_from_buffer();
                if (status != kStatus_Success)
                {
                    break;
                }
            }
            // Start a new page write. The address must page size aligned.
            if (pageColumnAddress != 0)
            {
                return kStatus_SemcNAND_InvalidMemoryAddress;
            }
            s_semcNandContext.cachedBytesToWrite = pageColumnAddress;
            s_semcNandContext.writePageIndex = pageIndex;
            s_semcNandContext.hasValidWritePageData = true;
        }

        // If the address is not continuous, start a new page write.
        if (s_semcNandContext.cachedBytesToWrite != pageColumnAddress)
        {
            status = semc_nand_flush_one_page_from_buffer();
            if (status != kStatus_Success)
            {
                break;
            }
            continue;
        }

        if (pageColumnAddress + length <= pageSize)
        {
            writeBytes = length;
        }
        else
        {
            writeBytes = pageSize - pageColumnAddress;
        }
        memcpy(&s_semcNandContext.writePageBuffer[pageColumnAddress], buffer, writeBytes);
        s_semcNandContext.cachedBytesToWrite += writeBytes;
        length -= writeBytes;
        buffer += writeBytes;
        pageColumnAddress += writeBytes;
        if (pageColumnAddress >= pageSize)
        {
            pageColumnAddress -= pageSize;
            pageIndex++;
        }
        // Mark current loop is successfully executed.
        status = kStatus_Success;
    }

    return status;
}

//! @brief  Erase SEMC NAND memory
status_t semc_nand_mem_erase(uint32_t address, uint32_t length)
{
    status_t status = kStatus_Fail;
    uint32_t startBlockIndex;
    uint32_t endBlockIndex;
    uint32_t blockCount;
    uint32_t totalBlocks = s_semcNandFcb.nandConfig.planesInDevice * s_semcNandFcb.nandConfig.blocksInPlane;

    // SEMC NAND should be configured before access.
    if (!is_semc_nand_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    // length = 0 means no erase operation will be executed. Just return success.
    if (length == 0)
    {
        return kStatus_Success;
    }

    startBlockIndex = address / s_semcNandFcb.nandConfig.bytesInPageDataArea / s_semcNandFcb.nandConfig.pagesInBlock;
    endBlockIndex =
        (address + length - 1) / s_semcNandFcb.nandConfig.bytesInPageDataArea / s_semcNandFcb.nandConfig.pagesInBlock;
    // Don't get block count from length. Address to address + length might across block boundary.
    blockCount = endBlockIndex - startBlockIndex + 1;

    // Ensure the FCB and DBBT cannot be erased by erase command
    uint32_t minAllowedEraseBlockId = s_semcNandFcb.firmwareTable[0].startPage / s_semcNandFcb.nandConfig.pagesInBlock;
    if (startBlockIndex < minAllowedEraseBlockId)
    {
        return kStatus_SemcNAND_DisallowOverwriteBcb;
    }

    // Due to bad block is skipped,
    // then also need to check if the block to erase is not cross the memory end.
    while (blockCount && (startBlockIndex < totalBlocks))
    {
        if (!is_semc_nand_page_in_bad_block(startBlockIndex * s_semcNandFcb.nandConfig.pagesInBlock))
        {
            status = semc_nand_erase_verify_block(startBlockIndex);
            if (status != kStatus_Success)
            {
                semc_nand_record_bad_block(startBlockIndex);
            }
            else
            {
                // Don't count in the bad blocks.
                blockCount--;
            }
        }
        startBlockIndex++;
    }

    // If a new bad block is discovered, update DBBT to SEMC NAND.
    if (s_semcNandContext.needToUpdateDbbt)
    {
        status = semc_nand_update_bcb(kSemcNandBootControlBlockType_DBBT);
    }

    return status;
}

//! @brief Erase all SEMC NAND memory
status_t semc_nand_mem_erase_all(void)
{
    status_t status;
    uint32_t startBlockIndex = 0;
    uint32_t totalBlocks = s_semcNandFcb.nandConfig.planesInDevice * s_semcNandFcb.nandConfig.blocksInPlane;

    // SEMC NAND should be configured before access.
    if (!is_semc_nand_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    // In case SEMC NAND is over 4G, do not call spinand_mem_erase() here.
    while (startBlockIndex < totalBlocks)
    {
        if (!is_semc_nand_page_in_bad_block(startBlockIndex * s_semcNandFcb.nandConfig.pagesInBlock))
        {
            status = semc_nand_erase_verify_block(startBlockIndex);
            if (status != kStatus_Success)
            {
                semc_nand_record_bad_block(startBlockIndex);
            }
        }
        startBlockIndex++;
    }

    // If a new bad block is discovered, update DBBT to SEMC NAND.
    if (s_semcNandContext.needToUpdateDbbt)
    {
        status = semc_nand_update_bcb(kSemcNandBootControlBlockType_DBBT);
    }

    return status;
}

//! @brief flush cached data to SEMC NAND memory
status_t semc_nand_mem_flush(void)
{
    status_t status = kStatus_Success;
    // If there still is data in the buffer, then flush them to SEMC NAND.
    if (s_semcNandContext.hasValidWritePageData)
    {
        status = semc_nand_flush_one_page_from_buffer();
    }
    return status;
}

//! @brief finalize the read/write operation of SEMC NAND memory
status_t semc_nand_mem_finalize(void)
{
    status_t status = kStatus_Success;

    // If a bad block is discovered during the operation, then need to update the DBBT to SEMC NAND.
    if (s_semcNandContext.needToUpdateDbbt)
    {
        status = semc_nand_update_bcb(kSemcNandBootControlBlockType_DBBT);
    }
    // Mark buffer to invalid.
    s_semcNandContext.hasValidWritePageData = false;
    s_semcNandContext.hasValidReadPageData = false;
    // A read / write operation is finished. Clear the skipped block count.
    s_semcNandContext.skippedBlockCount = 0;

    return status;
}

//! @brief Get Property from semc nand driver
status_t semc_nand_get_property(uint32_t whichProperty, uint32_t *value)
{
    if (value == NULL)
    {
        return kStatus_InvalidArgument;
    }

    switch (whichProperty)
    {
        case kSemcNandProperty_InitStatus:
            *value = is_semc_nand_configured() ? kStatus_Success : kStatusMemoryNotConfigured;
            break;

        case kSemcNandProperty_StartAddress:
            *value = kSemcNandMemInfo_StartAddress;
            break;

        case kSemcNandProperty_TotalFlashSizeInKBytes:
        {
            uint32_t totalFlashSizeInKbytes =
                (s_semcNandFcb.nandConfig.planesInDevice * s_semcNandFcb.nandConfig.blocksInPlane *
                 s_semcNandFcb.nandConfig.pagesInBlock * s_semcNandFcb.nandConfig.bytesInPageDataArea);

            *value = totalFlashSizeInKbytes / 1024;
        }
        break;

        case kSemcNandProperty_PageSize:
            *value = s_semcNandFcb.nandConfig.bytesInPageDataArea;
            break;

        case kSemcNandProperty_BlockSize:
            *value = s_semcNandFcb.nandConfig.pagesInBlock * s_semcNandFcb.nandConfig.bytesInPageDataArea;
            break;

        case kSemcNandProperty_TotalFlashSize:
        {
            uint32_t totalFlashSize =
                (s_semcNandFcb.nandConfig.planesInDevice * s_semcNandFcb.nandConfig.blocksInPlane *
                 s_semcNandFcb.nandConfig.pagesInBlock * s_semcNandFcb.nandConfig.bytesInPageDataArea);

            *value = totalFlashSize;
        }
        break;

        default: // catch inputs that are not recognized
            return kStatus_InvalidArgument;
    }

    return kStatus_Success;
}

// Release readPageBuffer for verification use
static void semc_nand_release_page_read_buffer_for_verify(void)
{
    if (s_semcNandFcb.nandConfig.enableReadbackVerify == kSemcNandReadbackVerifyOption_Enabled)
    {
        s_semcNandContext.hasValidReadPageData = false;
        s_semcNandContext.readPageIndex = 0;
        s_semcNandFcb.nandConfig.readbackPageBufferAddress = (uint32_t)&s_semcNandContext.readPageBuffer[0];
    }
}

// Load one page from NAND device to readPageBuffer
static status_t semc_nand_load_one_page_to_buffer(uint32_t pageIndex)
{
    status_t status;

    s_semcNandContext.hasValidReadPageData = false; // Mark read buffer invalid.

    // Read the page to readPageBuffer.
    status = semc_nand_flash_read_page(&s_semcNandFcb.nandConfig, pageIndex, s_semcNandContext.readPageBuffer,
                                       s_semcNandFcb.nandConfig.bytesInPageDataArea);
    if (status == kStatus_Success)
    {
        s_semcNandContext.hasValidReadPageData = true;
        s_semcNandContext.readPageIndex = pageIndex;
    }

    return status;
}

// Flush one page from writePageBuffer to NAND device
static status_t semc_nand_flush_one_page_from_buffer(void)
{
    status_t status;
    uint32_t srcPageIndex = s_semcNandContext.writePageIndex;
    uint32_t srcBlockIndex = srcPageIndex / s_semcNandFcb.nandConfig.pagesInBlock;
    uint32_t totalBlocks = s_semcNandFcb.nandConfig.planesInDevice * s_semcNandFcb.nandConfig.blocksInPlane;
    uint32_t destBlockIndex;

    s_semcNandContext.hasValidWritePageData = false;

    // Fill up the left bytes.
    if (s_semcNandContext.cachedBytesToWrite != s_semcNandFcb.nandConfig.bytesInPageDataArea)
    {
        memset(&s_semcNandContext.writePageBuffer[s_semcNandContext.cachedBytesToWrite], NAND_MEM_BLANK_VALUE,
               s_semcNandFcb.nandConfig.bytesInPageDataArea - s_semcNandContext.cachedBytesToWrite);
    }

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
    if (semc_nand_flash_verify_erase(&s_semcNandFcb.nandConfig, srcPageIndex, 1) != kStatus_Success)
    {
        return kStatusMemoryCumulativeWrite;
    }
#endif // #if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE

    // Flush the data in the writePagebuffer to SEMC NAND
    status =
        semc_nand_write_verify_memory(srcPageIndex * s_semcNandFcb.nandConfig.bytesInPageDataArea,
                                      s_semcNandFcb.nandConfig.bytesInPageDataArea, s_semcNandContext.writePageBuffer);

    if (status == kStatus_Success)
    {
        // Write success, return.
        return status;
    }
    // else write failed, and need to move data to the next good block.

    semc_nand_record_bad_block(srcBlockIndex);
    s_semcNandContext.skippedBlockCount++;

    // First destination block is next block.
    destBlockIndex = srcBlockIndex + 1;
    // Should not cross the end boundary.
    while (destBlockIndex < totalBlocks)
    {
        uint32_t destPageIndex = destBlockIndex * s_semcNandFcb.nandConfig.pagesInBlock;
        // Check if destination block is a good block.
        if (!is_semc_nand_page_in_bad_block(destPageIndex))
        {
            // If a good block, try to backup the datas to next block.
            status = semc_nand_backup_flushed_pages_from_bad_block(srcPageIndex, destBlockIndex);
#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
            // Return if success or the next good block is not erased.
            if ((status == kStatus_Success) || (status == kStatusMemoryCumulativeWrite))
#else
            if (status == kStatus_Success)
#endif // #if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
            {
                return status;
            }

            // Backup failed means destination block is also a bad block.
            semc_nand_record_bad_block(destBlockIndex);
        }

        // Move to next block.
        destBlockIndex++;
        s_semcNandContext.skippedBlockCount++;
    }

    // No erased good block left.
    return kStatusMemoryRangeInvalid;
}

// Backup all flushed pages from current bad block to next good NAND block
static status_t semc_nand_backup_flushed_pages_from_bad_block(uint32_t srcPageIndex, uint32_t destBlockIndex)
{
    status_t status = kStatus_Success;
    // First page to backup.
    uint32_t startPageIndex = srcPageIndex - (srcPageIndex % s_semcNandFcb.nandConfig.pagesInBlock);
    // The last page to backup. The last page is the page needs to flush.
    uint32_t endPageIndex = srcPageIndex;
    // Destination page to store the first page.
    uint32_t destPageIndex = destBlockIndex * s_semcNandFcb.nandConfig.pagesInBlock;

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
    // Firstly, need to check if the destination is erased.
    if (semc_nand_flash_verify_erase(&s_semcNandFcb.nandConfig, destPageIndex, endPageIndex - startPageIndex + 1) !=
        kStatus_Success)
    {
        return kStatusMemoryCumulativeWrite;
    }
#endif // #if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE

    // Move all pages in the block to the good block, except the last page.
    while (startPageIndex < endPageIndex)
    {
        // Read the page needs to backup.
        status = semc_nand_flash_read_page(&s_semcNandFcb.nandConfig, startPageIndex, s_semcNandContext.readPageBuffer,
                                           s_semcNandFcb.nandConfig.bytesInPageDataArea);
        if (status != kStatus_Success)
        {
            // Read failed, skip to next block to execute the backup progress.
            return status;
        }
        // Write the read page to the destination memory.
        status = semc_nand_write_verify_memory(destPageIndex * s_semcNandFcb.nandConfig.bytesInPageDataArea,
                                               s_semcNandFcb.nandConfig.bytesInPageDataArea,
                                               s_semcNandContext.readPageBuffer);
        if (status != kStatus_Success)
        {
            // Write failed, then skip to next block to execute the backup progress.
            return status;
        }
        // Move the source and destination pointer.
        startPageIndex++;
        destPageIndex++;
    }
    // Flush the last page. The data is contained in the write buffer.
    return semc_nand_write_verify_memory(endPageIndex * s_semcNandFcb.nandConfig.bytesInPageDataArea,
                                         s_semcNandFcb.nandConfig.bytesInPageDataArea,
                                         s_semcNandContext.writePageBuffer);
}

/* Write and verify SEMC NAND memory */
static status_t semc_nand_write_verify_memory(uint32_t address, uint32_t length, uint8_t *buffer)
{
    status_t status;
    uint32_t writePageIndex = address / s_semcNandFcb.nandConfig.bytesInPageDataArea;
    uint8_t *writeMemStart = (uint8_t *)buffer;
    uint8_t *writeMemEnd = writeMemStart + length;
    uint32_t writeMemSize = length;

    semc_nand_release_page_read_buffer_for_verify();

    while (writeMemStart < writeMemEnd)
    {
        uint32_t size = (writeMemSize >= s_semcNandFcb.nandConfig.bytesInPageDataArea) ?
                            s_semcNandFcb.nandConfig.bytesInPageDataArea :
                            writeMemSize;
        status = semc_nand_flash_page_program(&s_semcNandFcb.nandConfig, writePageIndex, writeMemStart, size);
        if (status == kStatus_Success)
        {
            writeMemStart += size;
            writeMemSize -= size;
            writePageIndex++;
        }
        else
        {
            break;
        }
    }

    return status;
}

/* Erase and verify SEMC NAND memory */
static status_t semc_nand_erase_verify_block(uint32_t blockIndex)
{
    status_t status;

    semc_nand_release_page_read_buffer_for_verify();

    status = semc_nand_flash_erase_block(&s_semcNandFcb.nandConfig, blockIndex);

    return status;
}

/* Validate the BCB data readback from SEMC NAND */
static bool semc_nand_validate_bcb(semc_bcb_type_t bcbType, uint32_t *nandBcb)
{
    semc_bcb_header_t *bcbHeader;
    uint32_t bcbTag, bcbVersion;
    uint32_t calculatedCrc;
    uint8_t *calculatedStart;
    uint32_t calculatedSize;

    switch (bcbType)
    {
        case kSemcNandBootControlBlockType_FCB:
        {
            semc_nand_fcb_t *nandFcb = (semc_nand_fcb_t *)nandBcb;
            bcbHeader = &nandFcb->bcbHeader;
            bcbTag = SEMC_NAND_FCB_TAG;
            bcbVersion = SEMC_NAND_FCB_VERSION;
            calculatedStart = (uint8_t *)&nandFcb->bcbHeader.fingerprint;
            calculatedSize = sizeof(semc_nand_fcb_t) - 4;
        }
        break;

        case kSemcNandBootControlBlockType_DBBT:
        default:
        {
            semc_nand_dbbt_t *nandDbbt = (semc_nand_dbbt_t *)nandBcb;
            bcbHeader = &nandDbbt->bcbHeader;
            bcbTag = SEMC_NAND_DBBT_TAG;
            bcbVersion = SEMC_NAND_DBBT_VERSION;
            calculatedStart = (uint8_t *)&nandDbbt->bcbHeader.fingerprint;
            calculatedSize = sizeof(semc_nand_dbbt_t) - 4;
        }
        break;
    }

    // Check the Tag
    if ((bcbHeader->fingerprint != bcbTag) || (bcbHeader->version != bcbVersion))
    {
        return false;
    }

    // Calculate CRC
    calculatedCrc = semc_nand_calculate_crc32(calculatedStart, calculatedSize);

    // Validate CRC
    return (calculatedCrc == bcbHeader->crcChecksum);
}

/* Search BCB(FCB,DBBT) data from Parallel NAND via SEMC */
static status_t semc_nand_search_bcb(semc_bcb_type_t bcbType,
                                     uint32_t startPage,
                                     uint32_t readCount,
                                     uint32_t searchStride)
{
    status_t status;
    uint32_t retryCnt;
    uint32_t bcbPageIndex;
    uint32_t *bcbMemStart = (uint32_t *)s_semcNandContext.readPageBuffer;
    uint32_t bcbMemSize;

    // Cleanup readPageBuffer before BCB search work
    memset(s_semcNandContext.readPageBuffer, 0, sizeof(s_semcNandContext.readPageBuffer));

    switch (bcbType)
    {
        case kSemcNandBootControlBlockType_FCB:
            bcbMemSize = sizeof(semc_nand_fcb_t);
            break;

        case kSemcNandBootControlBlockType_DBBT:
        default:
            bcbMemSize = sizeof(semc_nand_dbbt_t);
            break;
    }

    bcbPageIndex = startPage;
    for (retryCnt = 0; retryCnt < readCount; retryCnt++)
    {
        uint32_t bcbStartPageAddr = bcbPageIndex * s_semcNandFcb.nandConfig.bytesInPageDataArea;
        status = semc_nand_mem_read(bcbStartPageAddr, bcbMemSize, (uint8_t *)bcbMemStart);
        if (status == kStatus_Success)
        {
            if (semc_nand_validate_bcb(bcbType, bcbMemStart))
            {
                return kStatus_Success;
            }
        }

        bcbPageIndex += searchStride;
    }
    return kStatus_Fail;
}

/* Update BCB(FCB,DBBT) data to Parallel NAND via SEMC */
static status_t semc_nand_update_bcb(semc_bcb_type_t bcbType)
{
    // No bad block checking while updating BCB(FCB,DBBT).
    // ECC and CRC will ensure the accuracy.
    status_t status, bcbUpdateStatus;
    uint32_t bcbPageIndex, bcbBlockIndex;
    semc_nand_fcb_t *nandFcb = &s_semcNandFcb;
    uint32_t searchCount = nandFcb->searchCount;
    uint32_t searchStride = nandFcb->searchStride;
    uint32_t blockCount = (searchCount > 1) ? (searchStride * searchCount / nandFcb->nandConfig.pagesInBlock) : 1;
    uint32_t *bcbMemStart;
    uint32_t bcbMemSize;
    bool isBcbNeedToUpdate;

    switch (bcbType)
    {
        case kSemcNandBootControlBlockType_FCB:
        {
            // Update FCB if the FCB in SEMC NAND doesn't match the new one, such design
            // will avoid updating FCB frequently.
            bool isTheSameFcb = true;
            for (uint32_t i = 0, bcbPageIndex = 0; i < searchCount; i++, bcbPageIndex += searchStride)
            {
                uint32_t bcbStartPageAddr = bcbPageIndex * nandFcb->nandConfig.bytesInPageDataArea;
                status =
                    semc_nand_mem_read(bcbStartPageAddr, sizeof(semc_nand_fcb_t), s_semcNandContext.readPageBuffer);
                if (status != kStatus_Success)
                {
                    isTheSameFcb = false;
                    break;
                }
                if (memcmp(s_semcNandContext.readPageBuffer, nandFcb, sizeof(semc_nand_fcb_t)) != 0)
                {
                    isTheSameFcb = false;
                    break;
                }
            }
            isBcbNeedToUpdate = !isTheSameFcb;
            bcbBlockIndex = 0;
            bcbPageIndex = 0;
            bcbUpdateStatus = kStatus_SemcNAND_FailToUpdateFcb;
            bcbMemStart = (uint32_t *)nandFcb;
            bcbMemSize = sizeof(semc_nand_fcb_t);
            // Re-calculate CRC for FCB as it may be changed after calling nand_flash_init (ONFI)
            if (isBcbNeedToUpdate)
            {
                nandFcb->bcbHeader.crcChecksum =
                    semc_nand_calculate_crc32((uint8_t *)&nandFcb->bcbHeader.fingerprint,
                                              sizeof(semc_nand_fcb_t) - sizeof(nandFcb->bcbHeader.crcChecksum));
            }
        }
        break;

        case kSemcNandBootControlBlockType_DBBT:
        default:
        {
            isBcbNeedToUpdate = true;
            bcbBlockIndex = nandFcb->DBBTSerachAreaStartPage / nandFcb->nandConfig.pagesInBlock;
            bcbPageIndex = nandFcb->DBBTSerachAreaStartPage;
            bcbUpdateStatus = kStatus_SemcNAND_FailToUpdateDbbt;
            bcbMemStart = (uint32_t *)&s_semcNandDbbt;
            bcbMemSize = sizeof(semc_nand_dbbt_t);
            s_semcNandDbbt.bcbHeader.crcChecksum =
                semc_nand_calculate_crc32((uint8_t *)&s_semcNandDbbt.bcbHeader.fingerprint,
                                          sizeof(semc_nand_dbbt_t) - sizeof(s_semcNandDbbt.bcbHeader.crcChecksum));
        }
        break;
    }

    if (isBcbNeedToUpdate)
    {
        // Erase. Stride can be less than 1 block size, then need to erase the whole region firstly.
        for (uint32_t i = 0; i < blockCount; i++)
        {
            // No need to verify the erase.
            // None-erased bits will cause write failed, and skip to next position.
            semc_nand_erase_verify_block(bcbBlockIndex + i);
        }

        // Write BCB(FCB,DBBT) back to SEMC NAND.
        for (uint32_t i = 0; i < searchCount; i++, bcbPageIndex += searchStride)
        {
            uint32_t bcbStartPageAddr = bcbPageIndex * s_semcNandFcb.nandConfig.bytesInPageDataArea;
            status = semc_nand_write_verify_memory(bcbStartPageAddr, bcbMemSize, (uint8_t *)bcbMemStart);
            if (status != kStatus_Success)
            {
                // Erase even it might be a bad block, to ensure the data in good bits will not be read when loading
                // BCB(FCB,DBBT).
                // Only erase and not verify it.
                semc_nand_erase_verify_block(bcbPageIndex / nandFcb->nandConfig.pagesInBlock);
                continue;
            }
            else
            {
                if (bcbType == kSemcNandBootControlBlockType_DBBT)
                {
                    s_semcNandContext.needToUpdateDbbt = false;
                }
                // If there is one updated success, the whole progress is success.
                bcbUpdateStatus = kStatus_Success;
                // Do not return. Should update BCB(FCB,DBBT) to all the positions, in case bad block generated.
                // return kStatus_Success;
            }
        }
    }
    else
    {
        bcbUpdateStatus = kStatus_Success;
    }

    return bcbUpdateStatus;
}

// Determine if a page is in bad block
static bool is_semc_nand_page_in_bad_block(uint32_t pageIndex)
{
    uint32_t index = 0;
    uint32_t blockIndex = 0;
    bool isBadBlock = false;

    blockIndex = pageIndex / s_semcNandFcb.nandConfig.pagesInBlock;

    // Search bad block
    for (index = 0; index < s_semcNandDbbt.badBlockNumber; index++)
    {
        if (blockIndex == s_semcNandDbbt.badBlockTable[index])
        {
            isBadBlock = true;
            break;
        }
    }

    return isBadBlock;
}

// Calculate crc32 for nand data
static uint32_t semc_nand_calculate_crc32(uint8_t *start, uint32_t length)
{
    uint32_t crc32CalculatedValue;
    crc32_data_t crcInfo;
    crc32_init(&crcInfo);

    crc32_update(&crcInfo, start, length);
    crc32_finalize(&crcInfo, &crc32CalculatedValue);

    return crc32CalculatedValue;
}

// Determine if SEMC NAND is configured
static bool is_semc_nand_configured(void)
{
    return s_semcNandContext.isConfigured;
}

// Create empty NAND fcb
static status_t semc_nand_creat_empty_fcb(void)
{
    // Create empty fcb;
    // Fill FCB with all 0xFFs.
    memset(&s_semcNandFcb, NAND_MEM_BLANK_VALUE, sizeof(semc_nand_fcb_t));

    s_semcNandFcb.bcbHeader.fingerprint = SEMC_NAND_FCB_TAG; // FCB tag.
    s_semcNandFcb.bcbHeader.version = SEMC_NAND_FCB_VERSION; // FCB Version.

    s_semcNandFcb.bcbHeader.crcChecksum =
        semc_nand_calculate_crc32((uint8_t *)&s_semcNandFcb.bcbHeader.fingerprint,
                                  sizeof(semc_nand_fcb_t) - sizeof(s_semcNandFcb.bcbHeader.crcChecksum));

    return kStatus_Success;
}

// Create empty NAND dbbt
static status_t semc_nand_creat_empty_dbbt(void)
{
    // Create empty dbbt;
    // Fill DBBT with all 0xFFs.
    memset(&s_semcNandDbbt, NAND_MEM_BLANK_VALUE, sizeof(semc_nand_dbbt_t));

    s_semcNandDbbt.bcbHeader.fingerprint = SEMC_NAND_DBBT_TAG; // DBBT tag.
    s_semcNandDbbt.bcbHeader.version = SEMC_NAND_DBBT_VERSION; // DBBT Version.
    s_semcNandDbbt.badBlockNumber = 0;
    s_semcNandDbbt.bcbHeader.crcChecksum =
        semc_nand_calculate_crc32((uint8_t *)&s_semcNandDbbt.bcbHeader.fingerprint,
                                  sizeof(semc_nand_dbbt_t) - sizeof(s_semcNandDbbt.bcbHeader.crcChecksum));

    return kStatus_Success;
}

// Update external memory map for SEMC NAND
static status_t semc_nand_update_memory_map(void)
{
    uint32_t index;
    status_t status = find_external_map_index(kMemorySemcNand, &index);
    if (status != kStatus_Success)
    {
        return status;
    }
    g_externalMemoryMap[index].basicUnitSize = s_semcNandFcb.nandConfig.bytesInPageDataArea;
    g_externalMemoryMap[index].basicUnitCount = s_semcNandFcb.nandConfig.planesInDevice *
                                                s_semcNandFcb.nandConfig.blocksInPlane *
                                                s_semcNandFcb.nandConfig.pagesInBlock;

    return status;
}

// Record discovered bad block to DBBT and mark DBBT staus that need to be updated
static status_t semc_nand_record_bad_block(uint32_t blockIndex)
{
    if (s_semcNandDbbt.badBlockNumber < SEMC_NAND_BAD_BLOCKS_MAX_NUM)
    {
        s_semcNandDbbt.badBlockTable[s_semcNandDbbt.badBlockNumber++] = blockIndex;
        s_semcNandContext.needToUpdateDbbt = true;
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

// Skip bad blocks before read page
static void semc_nand_skip_bad_blocks(uint32_t *pageIndex)
{
    *pageIndex += s_semcNandContext.skippedBlockCount * s_semcNandFcb.nandConfig.pagesInBlock;
}

// Determine if block to read has been read once
static bool has_semc_nand_block_been_read_once(uint32_t blockIndex)
{
    return (s_semcNandContext.hasValidReadPageData) &&
           (blockIndex == (s_semcNandContext.readPageIndex / s_semcNandFcb.nandConfig.pagesInBlock));
}

// Determine if page to read is cached in readPageBuffer
static bool is_semc_nand_read_page_cached(uint32_t pageIndex)
{
    return (s_semcNandContext.readPageIndex == pageIndex) && (s_semcNandContext.hasValidReadPageData);
}

// Determine if block to read has been written once
static bool has_semc_nand_block_been_written_once(uint32_t blockIndex)
{
    return (s_semcNandContext.hasValidWritePageData) &&
           (blockIndex == (s_semcNandContext.writePageIndex / s_semcNandFcb.nandConfig.pagesInBlock));
}

// Determine if page to write is cached in writePageBuffer
static bool is_semc_nand_write_page_cached(uint32_t pageIndex)
{
    return (s_semcNandContext.writePageIndex == pageIndex) && (s_semcNandContext.hasValidWritePageData);
}

#endif // #if BL_FEATURE_SEMC_NAND_MODULE

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
