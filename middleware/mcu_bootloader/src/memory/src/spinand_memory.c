/*
 * Copyright (c) 2014-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "spinand_memory.h"
#include "bootloader.h"
#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "memory.h"
#if BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
#include "flexspi_nand_flash.h"
#endif // BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
#include <string.h>
#include "bl_context.h"
#include "crc32.h"
#include "fsl_assert.h"
#include "fsl_rtos_abstraction.h"
#if BL_FEATURE_GEN_KEYBLOB
#include "bl_keyblob.h"
#endif // BL_FEATURE_GEN_KEYBLOB

#if BL_FEATURE_SPINAND_MODULE

#ifndef SPINAND_INSTANCE
#if defined(BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
#define SPINAND_INSTANCE spinand_get_instance()
#else
#define SPINAND_INSTANCE BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE
#endif // BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
#endif

#ifndef SPINAND_ERASE_VERIFY
#define SPINAND_ERASE_VERIFY BL_FEATURE_SPINAND_MODULE_ERASE_VERIFY
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

enum
{
    kFlashDefaultPattern = 0xFF,
};

enum
{
    kSPINANDStartAddress = 0,
};

enum
{
    kNandAddressType_ByteAddress = 0,
    kNandAddressType_BlockAddress = 1,
};

//! @brief SPI NAND memory feature inforamation
//!
//! An instance is maintained in this file, will is used to keep key information for write and flush
//! operatations.
typedef struct _spinand_mem_context
{
    bool isConfigured;
    bool needToUpdateDbbt;
    uint32_t nandAddressType;
    uint32_t startBlockId;
    bool readwriteInProgress;
    uint32_t readBuffer[kSpiNandMemory_MaxPageSize / sizeof(uint32_t)];
    bool isReadBufferValid;
    uint32_t readBufferPageAddr;
    uint32_t writeBuffer[kSpiNandMemory_MaxPageSize / sizeof(uint32_t)];
    bool isWriteBufferValid;
    uint32_t writeBufferOffset;
    uint32_t writeBufferPageAddr;
    uint32_t skippedBlockCount;
#if BL_FEATURE_GEN_KEYBLOB
    bool has_keyblob;
    uint32_t keyblob_offset; //!< Key blob offset in application image
#endif                       // BL_FEATURE_GEN_KEYBLOB
#if defined(BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
    uint32_t instance;
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
} spinand_mem_context_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
static status_t spinand_mem_load_buffer(uint32_t pageAddr);

static status_t spinand_mem_flush_buffer(void);

static status_t spinand_mem_block_backup(uint32_t srcPageAddr, uint32_t destBlockAddr);

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
static bool is_erased_memory(uint32_t pageAddr, uint32_t pageCount);
#endif

static status_t spinand_mem_load_fcb(uint32_t searchCount, uint32_t searchStride);

static status_t spinand_mem_update_fcb(spinand_fcb_t *config);

static status_t spinand_mem_load_dbbt(spinand_fcb_t *config);

static status_t spinand_mem_update_dbbt(spinand_fcb_t *config);

static status_t spinand_memory_read(uint32_t pageAddr, uint32_t length, uint8_t *buffer);

static status_t spinand_memory_write_and_verify(uint32_t pageAddr, uint32_t length, uint8_t *buffer);

static status_t spinand_memory_erase_and_verify(uint32_t blockAddr);

static status_t spinand_memory_spi_init(flexspi_nand_config_t *config);

static status_t spinand_memory_read_page(uint32_t pageAddr, uint32_t length, uint8_t *buffer);

static status_t spinand_memory_program_page(uint32_t pageAddr, uint32_t length, uint8_t *buffer);

static status_t spinand_memory_erase_block(uint32_t blockAddr);

static bool is_block_crc32_check_pass(const uint32_t *start, uint32_t length);

static uint32_t calculate_crc32(const uint32_t *start, uint32_t length);

static bool is_spinand_configured(void);

static status_t spinand_mem_creat_empty_dbbt(void);

static bool is_bad_block_in_dbbt(uint32_t blockAddr);

static status_t bad_block_discovered(uint32_t blockAddr);

static status_t skip_bad_blocks(uint32_t *pageAddr);

static bool need_to_check_dbbt_before_read(uint32_t blockAddr);

static bool need_to_check_dbbt_before_write(uint32_t blockAddr);

static bool is_read_page_cached(uint32_t pageAddr);

static bool is_write_page_cached(uint32_t pageAddr);

static bool is_nand_address_type_valid(uint32_t type);

static status_t nand_generate_fcb(spinand_fcb_t *fcb, spinand_img_option_t *option);

static status_t check_update_keyblob_info(void *config);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static spinand_fcb_t s_spinandFcb;

static spinand_dbbt_t s_spinandDbbt;

static spinand_mem_context_t s_spinandContext = {
    .isConfigured = false,
    .needToUpdateDbbt = false,
    .skippedBlockCount = 0,
    .isReadBufferValid = false,
    .isWriteBufferValid = false,
 #if defined(BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
    .instance = BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE,
#endif // BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
};

//! @brief Interface to spi nand memory operations
const external_memory_region_interface_t g_spiNandMemoryInterface = {
    .init = spinand_mem_init,
    .read = spinand_mem_read,
    .write = spinand_mem_write,
    .erase = spinand_mem_erase,
    .config = spinand_mem_config,
    .flush = spinand_mem_flush,
    .finalize = spinand_mem_finalize,
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

//! @brief Get the instance of current flexspi nand
static uint32_t spinand_get_instance()
{
#if defined(BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
    return s_spinandContext.instance;
#else
    return SPINAND_INSTANCE;
#endif // BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
}

#if defined(BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
static bool spinand_instance_available(uint32_t inst)
{
    if ((inst >= 1) && (inst <= BL_FEATURE_SPINAND_MODULE_MAX_PERIPHERAL_INSTANCE))
    {
        return true;
    }
    else
    {
        return false;
    }
}
#endif // BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL

bool is_nand_address_type_valid(uint32_t type)
{
    bool is_valid = false;
    if ((type == kNandAddressType_ByteAddress) || (type == kNandAddressType_BlockAddress))
    {
        is_valid = true;
    }

    return is_valid;
}

// See qspi_memory.h for documentation on this function.
status_t spinand_mem_init(void)
{
    if (get_primary_boot_device() != kBootDevice_FlexSpiNAND)
    {
        return kStatus_Fail;
    }
    status_t status;

#if BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
    // Load default config block from efuse.
    status = flexspi_nand_get_default_cfg_blk(&s_spinandFcb.config);
    if (status != kStatus_Success)
    {
        return status;
    }
#else
#error "Unknown default config"
#endif // BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
    // Init SPI peripheral to enable fcb read.
    status = spinand_memory_spi_init(&s_spinandFcb.config);
    if (status != kStatus_Success)
    {
        return status;
    }

    uint32_t searchCount, searchStride;
#if BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
    flexspi_nand_get_fcb_search_cfg(&searchCount, &searchStride);
#else
#error "Unknown default fcb search config."
#endif
    status = spinand_mem_load_fcb(searchCount, searchStride);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = spinand_memory_spi_init(&s_spinandFcb.config);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = spinand_mem_load_dbbt(&s_spinandFcb);
    if (status != kStatus_Success)
    {
        // Do not create a new DBBT during init.
        // There is a risk, that the old DBBT is crashed.
        // User needs to re-configure.
        return status;
    }

    // All initialization steps are success. SPI NAND can be accessable.
    s_spinandContext.isConfigured = true;

    // Update external map entry info.
    uint32_t index;
    status = find_external_map_index(kMemorySpiNand, &index);
    if (status != kStatus_Success)
    {
        return status;
    }
    g_externalMemoryMap[index].basicUnitSize = s_spinandFcb.config.pageDataSize;
    g_externalMemoryMap[index].basicUnitCount = s_spinandFcb.config.blocksPerDevice * s_spinandFcb.config.pagesPerBlock;

    return status;
}

#if BL_FEATURE_GEN_KEYBLOB
status_t check_update_keyblob_info(void *config)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((config == NULL) || (s_spinandContext.isConfigured == false))
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
                s_spinandContext.has_keyblob = false;
                break;
            }
            s_spinandContext.keyblob_offset = keyblob_info->keyblob_offset;
            s_spinandContext.has_keyblob = true;
        }
        else if (keyblob_info_type == kKeyBlobInfoType_Program)
        {
            if (!s_spinandContext.has_keyblob)
            {
                break;
            }
            uint32_t index = keyblob_info->option.B.image_index;
            if (index >= kSpiNandMemory_MaxFirmware)
            {
                status = kStatus_InvalidArgument;
                break;
            }

            uint32_t image_page_start = s_spinandFcb.firmwareTable[index].startPage;
            uint32_t image_max_page_size = s_spinandFcb.firmwareTable[index].pagesInFirmware;

            uint32_t page_size = s_spinandFcb.config.pageDataSize;
            uint32_t keyblob_offset = s_spinandContext.keyblob_offset;
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
                status = kStatusMemoryRangeInvalid;
                break;
            }

            // Invalid key blob address, key blob must be page size aligned.
            if (keyblob_addr & (page_size - 1))
            {
                status = kStatusMemoryAlignmentError;
                break;
            }

            uint32_t keyblob_page_addr = keyblob_addr / page_size;
#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
            if (!is_erased_memory(keyblob_page_addr, 1))
            {
                status = kStatusMemoryCumulativeWrite;
                break;
            }
#endif
            status = spinand_memory_write_and_verify(keyblob_page_addr, keyblob_size, keyblob_buffer);
            if (status != kStatus_Success)
            {
                break;
            }

            status = spinand_mem_flush();
        }
    } while (0);

    return status;
}
#endif // #if BL_FEATURE_GEN_KEYBLOB

status_t nand_generate_fcb(spinand_fcb_t *fcb, spinand_img_option_t *option)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((fcb == NULL) || (option == NULL))
        {
            break;
        }

        if ((option->option0.B.size < kNandImgOption_MinSize) || (option->option0.B.size > kNandImgOption_MaxSize))
        {
            break;
        }

        memset(fcb, 0, sizeof(spinand_fcb_t));

        // Check wether NAND option is in valid memory range
        if ((!is_valid_application_location(option->nand_option_addr)) ||
            (!is_valid_application_location(option->nand_option_addr + sizeof(serial_nand_config_option_t))))
        {
            break;
        }

        serial_nand_config_option_t *nand_option = (serial_nand_config_option_t *)option->nand_option_addr;

        status = flexspi_nand_get_config(SPINAND_INSTANCE, &fcb->config, nand_option);
        if (status != kStatus_Success)
        {
            break;
        }

        uint32_t address_type = option->option0.B.address_type;
        if (is_nand_address_type_valid(address_type))
        {
            s_spinandContext.nandAddressType = address_type;
        }
        else
        {
            s_spinandContext.nandAddressType = kNandAddressType_ByteAddress;
        }

        uint32_t image_copies = option->option0.B.size - 2;
        if (image_copies > kSpiNandMemory_MaxFirmware)
        {
            break;
        }
        uint32_t search_stride = option->option0.B.search_stride;
        uint32_t search_count = option->option0.B.search_count;

        switch (search_stride)
        {
            case kSerialNandSearchStridePages_64:
                search_stride = 64;
                break;
            case kSerialNandSearchStridePages_128:
                search_stride = 128;
                break;
            case kSerialNandSearchStridePages_256:
                search_stride = 256;
                break;
            case kSerialNandSearchStridePages_32:
                search_stride = 32;
                break;
            default:
                break;
        }

        if (search_stride < 32)
        {
            break;
        }

        if ((search_count < 1) || (search_count > 4))
        {
            break;
        }

        fcb->searchStride = search_stride;
        fcb->searchCount = search_count;
        fcb->fingerprint = kTag_SPINAND_FCB;
        fcb->version = kTag_SPINAND_FCB_Version;
        fcb->DBBTSerachAreaStartPage = search_count * search_stride;
        fcb->firmwareCopies = image_copies;

        uint32_t image_starting_page;
        uint32_t image_page_count;
        uint32_t min_allowed_image_start_block;
        for (uint32_t i = 0; i < image_copies; i++)
        {
            // Do parameter check here
            if (i == 0)
            {
                // Image must start after FCB + DBBT region
                min_allowed_image_start_block = (search_stride / fcb->config.pagesPerBlock) * search_count * 2;
            }
            else
            {
                min_allowed_image_start_block =
                    option->image_info[i - 1].block_id + option->image_info[i - 1].block_count;
            }

            if (option->image_info[i].block_id < min_allowed_image_start_block)
            {
                break;
            }

            image_starting_page = option->image_info[i].block_id * fcb->config.pagesPerBlock;
            image_page_count = option->image_info[i].block_count * fcb->config.pagesPerBlock;
            fcb->firmwareTable[i].pagesInFirmware = image_page_count;
            fcb->firmwareTable[i].startPage = image_starting_page;
        }

        // Calculate CRC
        fcb->crcChecksum = calculate_crc32(&fcb->fingerprint, sizeof(*fcb) - sizeof(fcb->crcChecksum));

        status = kStatus_Success;

    } while (0);

    return status;
}

status_t spinand_mem_config(uint32_t *config)
{
    status_t status = kStatus_InvalidArgument;

    bool isNandConfig = false;

    do
    {
        uint32_t startAddr = (uint32_t)config;
        uint32_t endAddr = startAddr + sizeof(spinand_fcb_t) - 1;
        // Should check the config is in valid internal space.
        if ((!is_valid_application_location(startAddr)) || (!is_valid_application_location(endAddr)))
        {
            break;
        }

        // Try to get FCB based on full FCCB
        const spinand_fcb_t *firmwareConfig = (const spinand_fcb_t *)config;
        // Try to get FCB based on an option
        spinand_img_option_t *img_option = (spinand_img_option_t *)config;

#if BL_FEATURE_GEN_KEYBLOB
        keyblob_info_t *keyblob_info = (keyblob_info_t *)config;
#endif // BL_FEATURE_GEN_KEYBLOB
#if defined(BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
        if (MAGIC_NUMBER_SPINAND_PRECFG == img_option->option0.P.magic)
        {
            if (!spinand_instance_available(img_option->option0.P.cf9_field))
            {
                status = kStatus_InvalidArgument;
                break;
            }

            if (img_option->option0.P.cf9_field != s_spinandContext.instance)
            {
                s_spinandContext.instance = img_option->option0.P.cf9_field;
            }
            status = kStatus_Success;
            break;
        }
        else if (img_option->option0.B.tag == kNandImgOption_Tag)
#else
        if (img_option->option0.B.tag == kNandImgOption_Tag)
#endif // BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
        {
            status = nand_generate_fcb(&s_spinandFcb, img_option);
            if (status != kStatus_Success)
            {
                break;
            }

            // First, mark SPI NAND as not configured.
            s_spinandContext.isConfigured = false;
            isNandConfig = true;
        }
        else if (firmwareConfig->fingerprint == kTag_SPINAND_FCB)
        {
            // CRC check to ensure the FCB is valid.
            if (!is_block_crc32_check_pass(config, sizeof(spinand_fcb_t)))
            {
                break;
            }
            // All FCB check is passed. Then start to configure SPI NAND.
            // First, mark SPI NAND as not configured.
            s_spinandContext.isConfigured = false;
            // Over-write FCB.
            memcpy(&s_spinandFcb, firmwareConfig, sizeof(spinand_fcb_t));

            isNandConfig = true;
        }
#if BL_FEATURE_GEN_KEYBLOB
        else if (keyblob_info->option.B.tag == kKeyBlobInfoOption_Tag)
        {
            status = check_update_keyblob_info(config);
            if (status != kStatus_Success)
            {
                break;
            }
            else
            {
                status = kStatus_Success;
            }
        }
#endif
        else
        {
            break;
        }

        if (isNandConfig)
        {
            status = spinand_memory_spi_init(&s_spinandFcb.config);
            if (status != kStatus_Success)
            {
                break;
            }
            // Load DBBT from the address FCB tells.
            status = spinand_mem_load_dbbt(&s_spinandFcb);
            if (status != kStatus_Success)
            {
                // Load failure means a new dbbt need to be created.
                spinand_mem_creat_empty_dbbt();
                // Update new DBBT to SPI NAND.
                status = spinand_mem_update_dbbt(&s_spinandFcb);
                if (status != kStatus_Success)
                {
                    break;
                }
            }
            // Update new FCB to SPI NAND
            status = spinand_mem_update_fcb(&s_spinandFcb);

            if (status == kStatus_Success)
            {
                // All configuration steps are success. SPI NAND can be accessable.
                s_spinandContext.isConfigured = true;

                // Update external map entry info.
                uint32_t index;
                status = find_external_map_index(kMemorySpiNand, &index);
                if (status != kStatus_Success)
                {
                    return status;
                }
                g_externalMemoryMap[index].basicUnitSize = s_spinandFcb.config.pageDataSize;
                g_externalMemoryMap[index].basicUnitCount =
                    s_spinandFcb.config.blocksPerDevice * s_spinandFcb.config.pagesPerBlock;
            }
        }
    } while (0);

    return status;
}

// See qspi_memory.h for documentation on this function.
status_t spinand_mem_read(uint32_t address, uint32_t length, uint8_t *restrict buffer)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if (buffer == NULL)
        {
            break;
        }

        // SPI NAND should be configured before access.
        if (!is_spinand_configured())
        {
            status = kStatusMemoryNotConfigured;
            break;
        }

        uint32_t pageSize = s_spinandFcb.config.pageDataSize;
        uint32_t columnAddr;
        uint32_t pageAddr;

        if (s_spinandContext.nandAddressType == kNandAddressType_BlockAddress)
        {
            // If the Write transfer is not started yet, log the address as block address (index)
            if (!s_spinandContext.readwriteInProgress)
            {
                // Ensure the block address is a valid block address
                if (address >= s_spinandFcb.config.blocksPerDevice)
                {
                    break;
                }
                s_spinandContext.readwriteInProgress = true;
                s_spinandContext.startBlockId = address;
                columnAddr = 0;
                pageAddr = address * s_spinandFcb.config.pagesPerBlock;
            }
            // Otherwise, need to convert the address to actual physical address
            else
            {
                // Actual physical address caluclation formula: blockId * page size * pages per block + address -
                // blockId
                uint32_t actualAddress = s_spinandContext.startBlockId * s_spinandFcb.config.pagesPerBlock *
                                             s_spinandFcb.config.pageDataSize +
                                         (address - s_spinandContext.startBlockId);
                columnAddr = actualAddress % pageSize;
                pageAddr = actualAddress / pageSize;
            }
        }
        else // Address is actual NAND address
        {
            columnAddr = address % pageSize;
            pageAddr = address / pageSize;
        }

        // Skip the skipped blocks during a read operation.
        // No need to change the columnAddr.
        skip_bad_blocks(&pageAddr);

        uint32_t readLength;

        while (length)
        {
            // Check if current page to read is already read to readbuffer.
            // If no, need to read the whole page to buffer.
            if (!is_read_page_cached(pageAddr))
            {
                // Check if the page to read and cached page is in the same block.
                // If no, need to check if the block is a bad block.
                uint32_t blockAddr = pageAddr / s_spinandFcb.config.pagesPerBlock;
                if (need_to_check_dbbt_before_read(blockAddr))
                {
                    // Due to skipping bad blocks, blockAddr might cross the end boundary.
                    // Need to check the range.
                    if (blockAddr >= s_spinandFcb.config.blocksPerDevice)
                    {
                        status = kStatusMemoryRangeInvalid;
                        break;
                    }
                    // Check if reading a bad block.
                    if (is_bad_block_in_dbbt(blockAddr))
                    {
                        // If yes, skip the bad block and read the next block.
                        s_spinandContext.skippedBlockCount++;
                        pageAddr += s_spinandFcb.config.pagesPerBlock;
                        continue;
                    }
                }

                // Good block and not cached, then read the page to buffer.
                status = spinand_mem_load_buffer(pageAddr);
                if (status != kStatus_Success)
                {
                    break;
                }
            }
            // If it is a read accoss the page, divide it into two steps.
            if (columnAddr + length <= pageSize)
            {
                readLength = length;
            }
            else
            {
                readLength = pageSize - columnAddr;
            }
            uint8_t *p_readBuffer_8 = (uint8_t *)s_spinandContext.readBuffer;
            memcpy(buffer, &p_readBuffer_8[columnAddr], readLength);
            length -= readLength;
            buffer += readLength;
            columnAddr += readLength;
            if (columnAddr >= pageSize)
            {
                columnAddr -= pageSize;
                pageAddr++;
            }
            // Mark current loop is successfully executed.
            status = kStatus_Success;
        }

        // Terminate current transfer if errors happen during transfer
        if (status != kStatus_Success)
        {
            s_spinandContext.readwriteInProgress = false;
        }

    } while (0);

    return status;
}

status_t spinand_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if (buffer == NULL)
        {
            break;
        }

        // SPI NAND should be configured before access.
        if (!is_spinand_configured())
        {
            status = kStatusMemoryNotConfigured;
            break;
        }

        uint32_t pageSize = s_spinandFcb.config.pageDataSize;
        uint32_t columnAddr;
        uint32_t pageAddr;
        static uint32_t expectedNextActualAddr;
        if (s_spinandContext.nandAddressType == kNandAddressType_BlockAddress)
        {
            // If the Write transfer is not started yet, log the address as block address (index)
            if (!s_spinandContext.readwriteInProgress)
            {
                // Ensure the block address is a valid block address
                if (address >= s_spinandFcb.config.blocksPerDevice)
                {
                    break;
                }
                s_spinandContext.isWriteBufferValid = false;
                s_spinandContext.readwriteInProgress = true;
                s_spinandContext.startBlockId = address;
                columnAddr = 0;
                pageAddr = address * s_spinandFcb.config.pagesPerBlock;
                expectedNextActualAddr = pageAddr * pageSize + length;
            }
            // Otherwise, need to convert the address to actual physical address
            else
            {
                // Actual physical address caluclation formula: blockId * page size * pages per block + address -
                // blockId
                uint32_t actualAddress = s_spinandContext.startBlockId * s_spinandFcb.config.pagesPerBlock *
                                             s_spinandFcb.config.pageDataSize +
                                         (address - s_spinandContext.startBlockId);
                columnAddr = actualAddress % pageSize;
                pageAddr = actualAddress / pageSize;

                // The address is continuous in a transfer, so once the address is not continuous,
                // Flush data in buffer into SPI NAND and then re-start a new transfer
                if (actualAddress != expectedNextActualAddr)
                {
                    if (s_spinandContext.isWriteBufferValid)
                    {
                        status = spinand_mem_flush_buffer();
                        if (status != kStatus_Success)
                        {
                            // Terminate transfer if error occurs.
                            s_spinandContext.readwriteInProgress = false;
                            break;
                        }
                    }
                    s_spinandContext.readwriteInProgress = true;
                    s_spinandContext.startBlockId = address;
                    columnAddr = 0;
                    pageAddr = s_spinandContext.startBlockId * s_spinandFcb.config.pagesPerBlock;
                    actualAddress = pageAddr * pageSize;
                }
                expectedNextActualAddr = actualAddress + length;
            }
        }
        else // Address is actual NAND address
        {
            columnAddr = address % pageSize;
            pageAddr = address / pageSize;
        }

        // Ensure DBBT and FCB cannot be overwritedn by normal write operation
        if (pageAddr < s_spinandFcb.firmwareTable[0].startPage)
        {
            break;
        }

        // Skip the skipped blocks during a read operation.
        // No need to change the columnAddr.
        skip_bad_blocks(&pageAddr);

        uint32_t writeLength;

        while (length)
        {
            // Check if current page to write is already cached to writebuffer.
            // If no, need to init the writebuffer.
            if (!is_write_page_cached(pageAddr))
            {
                uint32_t blockAddr = pageAddr / s_spinandFcb.config.pagesPerBlock;
                // Check if the page to write and cached page is in the same block.
                // If no, need to check if the block is a bad block.
                if (need_to_check_dbbt_before_write(blockAddr))
                {
                    // Due to skipping bad blocks, blockAddr might cross the end boundary.
                    // Need to check the range.

                    if (blockAddr >= s_spinandFcb.config.blocksPerDevice)
                    {
                        status = kStatusMemoryRangeInvalid;
                        break;
                    }
                    // Check if writting a bad block.
                    if (is_bad_block_in_dbbt(blockAddr))
                    {
                        // If yes, skip the bad block and write to the next block.
                        s_spinandContext.skippedBlockCount++;
                        pageAddr += s_spinandFcb.config.pagesPerBlock;
                        continue;
                    }
                }
                // There is data already cached in the buffer, flush it to SPI NAND.
                if (s_spinandContext.isWriteBufferValid)
                {
                    status = spinand_mem_flush_buffer();
                    if (status != kStatus_Success)
                    {
                        break;
                    }
                }
                // Start a new page write. The address must page size aligned.
                if (columnAddr != 0)
                {
                    status = kStatus_FlexSPINAND_WriteAlignmentError;
                    break;
                }
                s_spinandContext.writeBufferOffset = columnAddr;
                s_spinandContext.writeBufferPageAddr = pageAddr;
                s_spinandContext.isWriteBufferValid = true;
            }

            // If the address is not continuous, start a new page write.
            if (s_spinandContext.writeBufferOffset != columnAddr)
            {
                status = spinand_mem_flush_buffer();
                if (status != kStatus_Success)
                {
                    break;
                }
                continue;
            }

            if (columnAddr + length <= pageSize)
            {
                writeLength = length;
            }
            else
            {
                writeLength = pageSize - columnAddr;
            }
            uint8_t *p_writeBuffer_8 = (uint8_t *)s_spinandContext.writeBuffer;
            memcpy(&p_writeBuffer_8[columnAddr], buffer, writeLength);
            s_spinandContext.writeBufferOffset += writeLength;
            length -= writeLength;
            buffer += writeLength;
            columnAddr += writeLength;
            if (columnAddr >= pageSize)
            {
                columnAddr -= pageSize;
                pageAddr++;
            }
            // Mark current loop is successfully executed.
            status = kStatus_Success;
        }

        // Terminate current transfer if errors happen during transfer
        if (status != kStatus_Success)
        {
            s_spinandContext.readwriteInProgress = false;
        }
    } while (0);

    return status;
}

status_t spinand_mem_flush(void)
{
    status_t status = kStatus_Success;
    // If there still is data in the buffer, then flush them to SPI NAND.
    if (s_spinandContext.isWriteBufferValid)
    {
        status = spinand_mem_flush_buffer();
    }
    return status;
}

// See qspi_memory.h for documentation on this function.
status_t spinand_mem_finalize(void)
{
    status_t status = kStatus_Success;

    // If a bad block is discovered during the operation, then need to update the DBBT to SPI NAND.
    if (s_spinandContext.needToUpdateDbbt)
    {
        status = spinand_mem_update_dbbt(&s_spinandFcb);
    }
    // Mark buffer to invalid.
    s_spinandContext.isWriteBufferValid = false;
    s_spinandContext.isReadBufferValid = false;
    // A read / write operation is finished. Clear the skipped block count.
    s_spinandContext.skippedBlockCount = 0;

    s_spinandContext.readwriteInProgress = false;

    return status;
}

// See qspi_memory.h for documentation on this function.
status_t spinand_mem_erase(uint32_t address, uint32_t length)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        // SPI NAND should be configured before access.
        if (!is_spinand_configured())
        {
            status = kStatusMemoryNotConfigured;
            break;
        }
        // length = 0 means no erase operation will be executed. Just return success.
        if (length == 0)
        {
            status = kStatus_Success;
            break;
        }
        // else means 1 block at lest to be erased.

        uint32_t totalBlocks = s_spinandFcb.config.blocksPerDevice;
        uint32_t startBlockAddr;
        uint32_t blockCount;
        // The address[30:0] is block id if address[31] is 1
        if (s_spinandContext.nandAddressType == kNandAddressType_BlockAddress)
        {
            startBlockAddr = address;
            blockCount = length;
        }
        else // Address is actual NAND address
        {
            startBlockAddr = address / s_spinandFcb.config.pageDataSize / s_spinandFcb.config.pagesPerBlock;
            // Don't get block count from length. Address to address + length might across block boundary.
            blockCount = (address + length - 1) / s_spinandFcb.config.pageDataSize / s_spinandFcb.config.pagesPerBlock -
                         startBlockAddr + 1;
        }

        // Ensure the FCB and DBBT cannot be erased by erase command
        uint32_t minAllowedEraseBlockId = s_spinandFcb.firmwareTable[0].startPage / s_spinandFcb.config.pagesPerBlock;
        if (startBlockAddr < minAllowedEraseBlockId)
        {
            break;
        }

        // Due to bad block is skipped,
        // then also need to check if the block to erase is not cross the memory end.
        while (blockCount && (startBlockAddr < totalBlocks))
        {
            if (!is_bad_block_in_dbbt(startBlockAddr))
            {
                status = spinand_memory_erase_and_verify(startBlockAddr);
                if (status != kStatus_Success)
                {
                    bad_block_discovered(startBlockAddr);
                }
                else
                {
                    // Don't count in the bad blocks.
                    blockCount--;
                }
            }
            startBlockAddr++;
        }

        // If a new bad block is discovered, update DBBT to SPI NAND.
        if (s_spinandContext.needToUpdateDbbt)
        {
            status = spinand_mem_update_dbbt(&s_spinandFcb);
            if (status != kStatus_Success)
            {
                break;
            }
        }
    } while (0);

    return status;
}

// See memory.h for documentation on this function.
status_t spinand_mem_erase_all(void)
{
    // SPI NAND should be configured before access.
    if (!is_spinand_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    uint32_t startBlockAddr = 0;
    uint32_t totalBlocks = s_spinandFcb.config.blocksPerDevice;
    // In case SPI NAND is over 4G, do not call spinand_mem_erase() here.
    while (startBlockAddr < totalBlocks)
    {
        if (!is_bad_block_in_dbbt(startBlockAddr))
        {
            status_t status = spinand_memory_erase_and_verify(startBlockAddr);
            if (status != kStatus_Success)
            {
                return status;
            }
        }
        startBlockAddr++;
    }

    // If a new bad block is discovered, update DBBT to SPI NAND.
    if (s_spinandContext.needToUpdateDbbt)
    {
        status_t status = spinand_mem_update_dbbt(&s_spinandFcb);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    return kStatus_Success;
}

//! @brief Get Property from flexspi driver
status_t spinand_get_property(uint32_t whichProperty, uint32_t *value)
{
    if (value == NULL)
    {
        return kStatus_InvalidArgument;
    }

    switch (whichProperty)
    {
        case kSPINANDProperty_InitStatus:
            *value = is_spinand_configured() ? kStatus_Success : kStatusMemoryNotConfigured;
            break;

        case kSPINANDProperty_StartAddress:
            *value = kSPINANDStartAddress;
            break;

        case kSPINANDrProperty_TotalFlashSizeInKBytes:
        {
            uint32_t totalFlashSizeInKbytes =
                (s_spinandFcb.config.memConfig.sflashA1Size + s_spinandFcb.config.memConfig.sflashA2Size +
                 s_spinandFcb.config.memConfig.sflashB1Size + s_spinandFcb.config.memConfig.sflashB2Size) /
                2;

            *value = totalFlashSizeInKbytes / 1024;
        }
        break;

        case kSPINANDProperty_PageSize:
            *value = s_spinandFcb.config.pageDataSize;
            break;

        case kSPINANDProperty_BlockSize:
            *value = s_spinandFcb.config.pageDataSize * s_spinandFcb.config.pagesPerBlock;
            break;

        case kSPINANDProperty_TotalFlashSize:
        {
            uint32_t totalFlashSize =
                s_spinandFcb.config.memConfig.sflashA1Size + s_spinandFcb.config.memConfig.sflashA2Size +
                s_spinandFcb.config.memConfig.sflashB1Size + s_spinandFcb.config.memConfig.sflashB2Size;

            *value = totalFlashSize;
        }
        break;

        default: // catch inputs that are not recognized
            return kStatus_InvalidArgument;
    }

    return kStatus_Success;
}

static status_t spinand_mem_load_buffer(uint32_t pageAddr)
{
    status_t status;

    s_spinandContext.isReadBufferValid = false; // Mark read buffer invalid.

    // Read the page to read buffer.
    status =
        spinand_memory_read(pageAddr, s_spinandFcb.config.pageDataSize, (uint8_t *)&s_spinandContext.readBuffer[0]);
    if (status == kStatus_Success)
    {
        s_spinandContext.isReadBufferValid = true;
        s_spinandContext.readBufferPageAddr = pageAddr;
    }

    return status;
}

static status_t spinand_mem_flush_buffer(void)
{
    status_t status;

    // Terminate current transfer if the write buffer size is less than page size when the flush API is called
    if (s_spinandContext.writeBufferOffset != s_spinandFcb.config.pageDataSize)
    {
        s_spinandContext.readwriteInProgress = false;
    }

    s_spinandContext.isWriteBufferValid = false;

    uint32_t srcPageAddr = s_spinandContext.writeBufferPageAddr;

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
    if (!is_erased_memory(srcPageAddr, 1))
    {
        return kStatusMemoryCumulativeWrite;
    }
#endif // #if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE

    // Flush the data in the write buffer to SPI NAND
    status =
        spinand_memory_write_and_verify(srcPageAddr, s_spinandContext.writeBufferOffset, (uint8_t*)s_spinandContext.writeBuffer);
    if (status == kStatus_Success)
    {
        // Write success, return.
        return status;
    }
    // else write failed, and need to move data to the next good block.

    uint32_t srcBlockAddr = srcPageAddr / s_spinandFcb.config.pagesPerBlock;

    bad_block_discovered(srcBlockAddr);
    s_spinandContext.skippedBlockCount++;

    uint32_t totalBlocks = s_spinandFcb.config.blocksPerDevice;
    uint32_t destBlockAddr = srcBlockAddr + 1; // First destination block is next block.
    // Should not cross the end boundary.
    while (destBlockAddr < totalBlocks)
    {
        // Check if destination block is a good block.
        if (!is_bad_block_in_dbbt(destBlockAddr))
        {
            // If a good block, try to backup the datas to next block.
            status = spinand_mem_block_backup(srcPageAddr, destBlockAddr);
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
            bad_block_discovered(destBlockAddr);
        }

        // Move to next block.
        destBlockAddr++;
        s_spinandContext.skippedBlockCount++;
    }
    // No erased good block left.
    return kStatusMemoryRangeInvalid;
}

static status_t spinand_mem_block_backup(uint32_t srcPageAddr, uint32_t destBlockAddr)
{
    status_t status = kStatus_Success;

    uint32_t startPageAddr = srcPageAddr - (srcPageAddr % s_spinandFcb.config.pagesPerBlock); // First page to backup.
    uint32_t endPageAddr = srcPageAddr; // The last page to backup. The last page is the page needs to flush.
    uint32_t destPageAddr =
        destBlockAddr * s_spinandFcb.config.pagesPerBlock; // Destination page to store the first page.

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
    // Firstly, need to check if the destination is erased.
    if (!is_erased_memory(destPageAddr, endPageAddr - startPageAddr + 1))
    {
        return kStatusMemoryCumulativeWrite;
    }
#endif // #if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE

    // Move all pages in the block to the good block, except the last page.
    while (startPageAddr < endPageAddr)
    {
        // Read the page needs to backup.
        status = spinand_memory_read(startPageAddr, s_spinandFcb.config.pageDataSize,
                                     (uint8_t *)&s_spinandContext.readBuffer[0]);
        if (status != kStatus_Success)
        {
            // Read failed, skip to next block to execute the backup progress.
            return status;
        }
        // Write the read page to the destination memory.
        status = spinand_memory_write_and_verify(destPageAddr, s_spinandFcb.config.pageDataSize,
                                                 (uint8_t *)&s_spinandContext.readBuffer[0]);
        if (status != kStatus_Success)
        {
            // Write failed, then skip to next block to execute the backup progress.
            return status;
        }
        // Move the source and destination pointer.
        startPageAddr++;
        destPageAddr++;
    }
    // Flush the last page. The data is contained in the write buffer.
    return spinand_memory_write_and_verify(destPageAddr, s_spinandFcb.config.pageDataSize,
                                           (uint8_t*)s_spinandContext.writeBuffer);
}

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
static bool is_erased_memory(uint32_t pageAddr, uint32_t pageCount)
{
    status_t status;
    uint32_t pageSize = s_spinandFcb.config.pageDataSize;
    uint32_t offset, *buffer;
    while (pageCount)
    {
        // Read the page firstly.
        status = spinand_memory_read(pageAddr++, pageSize, (uint8_t *)&s_spinandContext.readBuffer[0]);
        if (status != kStatus_Success)
        {
            // If read failed, return false.
            return false;
        }

        offset = 0;
        buffer = (uint32_t *)s_spinandContext.readBuffer;
        while (offset < pageSize)
        {
            // Check if all 0xFFs
            if (*buffer != 0xffffffff)
            {
                return false;
            }
            buffer++;
            offset += 4;
        }
        pageCount--;
    }
    return true;
}
#endif // #if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE

static status_t spinand_mem_load_fcb(uint32_t searchCount, uint32_t searchStride)
{
    status_t status;

    uint16_t i;
    uint32_t pageAddr;

    for (i = 0, pageAddr = 0; i < searchCount; i++, pageAddr += searchStride)
    {
        // Currently, No DBBT is available. Just read it. CRC check will ensure it is correct.
        // Do not read it to s_spinandFcb directly, it will over-write s_spinandFcb.config
        // Which contains the default config.
        status = spinand_memory_read(pageAddr, sizeof(spinand_fcb_t), (uint8_t *)&s_spinandContext.readBuffer[0]);
        // Read failed, then skip to next position.
        if (status != kStatus_Success)
        {
            continue;
        }
        // else reading FCB is success.
        spinand_fcb_t *config = (spinand_fcb_t *)s_spinandContext.readBuffer;
        // Check the FCB tag and version.
        if ((config->fingerprint != kTag_SPINAND_FCB) || (config->version != kTag_SPINAND_FCB_Version))
        {
            // Invalid tag and version, skip to next position.
            continue;
        }
        // else correct tag and version.

        // CRC check to ensure the FCB is correct.
        if (is_block_crc32_check_pass((uint32_t *)config, sizeof(spinand_fcb_t)))
        {
            // CRC check passed, find the FCB.
            memcpy(&s_spinandFcb, config, sizeof(spinand_fcb_t));
            return kStatus_Success;
        }
        // else CRC check failed and continue.
    }
    return kStatus_Fail;
}

status_t spinand_mem_update_fcb(spinand_fcb_t *config)
{
    // No bad block checking while updating FCB.
    // ECC and CRC will ensure the accuracy.
    assert(config);

    status_t status, returnStatus = kStatus_FlexSPINAND_DbbtUpdateFail;

    uint32_t i;
    uint32_t pageAddr;

    uint32_t searchCount = config->searchCount;
    uint32_t searchStride = config->searchStride;

    uint32_t blockCount = searchStride * searchCount / config->config.pagesPerBlock;
    // Update FCB if the FCB in SPI NAND doesn't match the new one, such design will avoid updating FCB frequently.
    bool fcb_match = true;
    for (i = 0, pageAddr = 0; i < searchCount; i++, pageAddr += searchStride)
    {
        status = spinand_memory_read(pageAddr, sizeof(spinand_fcb_t), (uint8_t *)&s_spinandContext.readBuffer[0]);
        if (status != kStatus_Success)
        {
            fcb_match = false;
            break;
        }
        if (memcmp(s_spinandContext.readBuffer, config, sizeof(spinand_fcb_t)) != 0)
        {
            fcb_match = false;
            break;
        }
    }

    if (!fcb_match)
    {
        // Erase. Stride can be less than 1 block size, then need to erase the whole region firstly.
        for (i = 0; i < blockCount; i++)
        {
            // No need to verify the erase.
            // None-erased bits will cause write failed, and skip to next position.
            spinand_memory_erase_block(i);
        }

        // Write FCB back to SPI NAND.
        for (i = 0, pageAddr = 0; i < searchCount; i++, pageAddr += searchStride)
        {
            status = spinand_memory_write_and_verify(pageAddr, sizeof(spinand_fcb_t), (uint8_t *)config);
            if (status != kStatus_Success)
            {
                // Erase even it might be a bad block, to ensure the data in good bits will not be read when loading
                // DBBT.
                // Only erase and not verify it.
                spinand_memory_erase_block(pageAddr / config->config.pagesPerBlock);
                continue;
            }
            else
            {
                // If there is one updated success, the whole progress is success.
                returnStatus = kStatus_Success;
                // Do not return. Should update FCB to all the positions, in case bad block generated.
                // return kStatus_Success;
            }
        } // for (i = 0, pageAddr = 0; i < searchCount; i++, pageAddr += searchStride)
    }     // if (!fcb_match)
    else
    {
        returnStatus = kStatus_Success;
    }
    return returnStatus;
}

static status_t spinand_mem_load_dbbt(spinand_fcb_t *config)
{
    assert(config);

    status_t status;

    uint16_t i;
    uint32_t pageAddr;

    s_spinandDbbt.fingerprint = 0; // Mark dbbt invalid.

    uint32_t searchCount = config->searchCount;
    uint32_t searchStride = config->searchStride;

    for (i = 0, pageAddr = config->DBBTSerachAreaStartPage; (i < searchCount) && (i < 8); i++, pageAddr += searchStride)
    {
        // Currently, No DBBT is available. Just read it. CRC check will ensure it is correct.
        status = spinand_memory_read(pageAddr, sizeof(spinand_dbbt_t), (uint8_t *)&s_spinandDbbt);
        // Read is failed, then skip to next position.
        if (status != kStatus_Success)
        {
            continue;
        }
        // else reading is success.

        // Check DBBT tag.
        if ((s_spinandDbbt.fingerprint != kTag_SPINAND_DBBT) || (s_spinandDbbt.version != kTag_SPINAND_DBBT_Version))
        {
            // Invalid tag, skip to next position.
            continue;
        }
        // else valid tag.

        // CRC check to ensure the data is correct.
        if (is_block_crc32_check_pass((uint32_t *)&s_spinandDbbt, sizeof(spinand_dbbt_t)))
        {
            // CRC check passed, find the DBBT.
            return kStatus_Success;
        }
        // else means CRC check is failed, continue.
    }
    return kStatus_Fail;
}

status_t spinand_mem_update_dbbt(spinand_fcb_t *config)
{
    // No bad block checking while updating DBBT.
    // ECC and CRC will ensure the accuracy.
    assert(config);

    status_t status, returnStatus = kStatus_FlexSPINAND_DbbtUpdateFail;

    uint16_t i;
    uint32_t pageAddr;

    uint32_t searchCount = config->searchCount;
    uint32_t searchStride = config->searchStride;

    uint32_t blockCount = searchStride * searchCount / config->config.pagesPerBlock;
    uint32_t blockStart = config->DBBTSerachAreaStartPage / config->config.pagesPerBlock;
    // Erase. Stride can be less than 1 block size, then need to erase the whole region firstly.
    for (i = 0; i < blockCount; i++)
    {
        // No need to verify the erase.
        // None-erased bits will cause write failed, and skip to next position.
        spinand_memory_erase_block(blockStart + i);
    }

    // Update CRC value before programming
    s_spinandDbbt.crcChecksum =
        calculate_crc32(&s_spinandDbbt.fingerprint, sizeof(spinand_dbbt_t) - sizeof(s_spinandDbbt.crcChecksum));

    // Write DBBT back to SPI NAND.
    for (i = 0, pageAddr = config->DBBTSerachAreaStartPage; i < searchCount; i++, pageAddr += searchStride)
    {
        status = spinand_memory_write_and_verify(pageAddr, sizeof(spinand_dbbt_t), (uint8_t *)&s_spinandDbbt);
        if (status != kStatus_Success)
        {
            // Erase even it might be a bad block, to ensure the data in good bits will not be read when loading DBBT.
            // Only erase and not verify it.
            spinand_memory_erase_block(pageAddr / config->config.pagesPerBlock);
            continue;
        }
        else
        {
            s_spinandContext.needToUpdateDbbt = false;
            // If there is one updated success, the whole progress is success.
            returnStatus = kStatus_Success;
            // Do not return. Should update DBBT to all the strides, in case bad block generated.
            // return kStatus_Success;
        }
    }
    return returnStatus;
}

static status_t spinand_memory_read(uint32_t pageAddr, uint32_t length, uint8_t *buffer)
{
    assert(buffer);

    status_t status;
    uint32_t size;

    while (length)
    {
        size = MIN(s_spinandFcb.config.pageDataSize, length);
        status = spinand_memory_read_page(pageAddr, size, buffer);
        if (status != kStatus_Success)
        {
            return status;
        }
        buffer += size;
        length -= size;
        pageAddr++;
    }

    return status;
}

static status_t spinand_memory_write_and_verify(uint32_t pageAddr, uint32_t length, uint8_t *buffer)
{
    assert(buffer);

    status_t status;
    uint32_t size;

    while (length)
    {
        size = MIN(s_spinandFcb.config.pageDataSize, length);
        status = spinand_memory_program_page(pageAddr, size, buffer);
        if (status != kStatus_Success)
        {
            return status;
        }

        status = spinand_memory_read_page(pageAddr, size, (uint8_t *)&s_spinandContext.readBuffer[0]);
        if (status != kStatus_Success)
        {
            return status;
        }

        if (memcmp(buffer, s_spinandContext.readBuffer, size))
        {
            return kStatus_Fail;
        }

        buffer += size;
        length -= size;
        pageAddr++;
    }

    return status;
}

status_t spinand_memory_erase_and_verify(uint32_t blockAddr)
{
#if SPI_NAND_ERASE_VERIFY
    status_t status;

    uint32_t pageAddr = blockAddr * s_spinandFcb.config.pagesPerBlock;

    // Erase the block.
    status = spinand_memory_erase_block(blockAddr);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Check if the memory is erased ( All 0xFFs).
    if (is_erased_memory(pageAddr, s_spinandFcb.config.pagesPerBlock))
    {
        return kStatus_Success;
    }
    else
    {
        // If not all 0xFFs, means erase operation is failed.
        return kStatus_FlexSPINAND_EraseBlockFail;
    }
#else
    return spinand_memory_erase_block(blockAddr);
#endif // defined(SPI_NAND_ERASE_VERIFY)
}

status_t spinand_memory_spi_init(flexspi_nand_config_t *config)
{
    assert(config);
#if BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
    return flexspi_nand_init(SPINAND_INSTANCE, config);
#else
#error "Unknown peripheral init function."
#endif
}

static status_t spinand_memory_read_page(uint32_t pageAddr, uint32_t length, uint8_t *buffer)
{
#if BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
    return flexspi_nand_read_page(SPINAND_INSTANCE, &s_spinandFcb.config, pageAddr, (uint32_t *)buffer, length);
#else
#error "Unsupported Peripheral for SPI NAND"
#endif // BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
}

static status_t spinand_memory_program_page(uint32_t pageAddr, uint32_t length, uint8_t *buffer)
{
#if BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
    return flexspi_nand_program_page(SPINAND_INSTANCE, &s_spinandFcb.config, pageAddr, (uint32_t *)buffer, length);
#else
#error "Unsupported Peripheral for SPI NAND"
#endif // BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
}

static status_t spinand_memory_erase_block(uint32_t blockAddr)
{
#if BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
    return flexspi_nand_erase_block(SPINAND_INSTANCE, &s_spinandFcb.config, blockAddr);
#else
#error "Unsupported Peripheral for SPI NAND"
#endif // BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
}

static bool is_block_crc32_check_pass(const uint32_t *start, uint32_t length)
{
#if TEST_SKIP_CRC
    // Note! Only for testing.
    return true;
#else
    // First word is the expected data, the left is the data to calculate.
    return calculate_crc32(&start[1], length - sizeof(uint32_t)) == start[0];
#endif // TEST_SKIP_CRC
}

static uint32_t calculate_crc32(const uint32_t *start, uint32_t length)
{
    uint32_t crc32CalculatedValue;
    crc32_data_t crcInfo;
    crc32_init(&crcInfo);

    crc32_update(&crcInfo, (uint8_t *)start, length);
    crc32_finalize(&crcInfo, &crc32CalculatedValue);

    return crc32CalculatedValue;
}

static bool is_spinand_configured(void)
{
    return s_spinandContext.isConfigured;
}

static status_t spinand_mem_creat_empty_dbbt(void)
{
    // Create empty dbbt;
    // Fill DBBT to all 0xFFs.
    memset(&s_spinandDbbt, kFlashDefaultPattern, sizeof(spinand_dbbt_t));

    s_spinandDbbt.fingerprint = kTag_SPINAND_DBBT;     // DBBT tag.
    s_spinandDbbt.version = kTag_SPINAND_DBBT_Version; // DBBT Version.
    s_spinandDbbt.badBlockNumber = 0;
    s_spinandDbbt.crcChecksum =
        calculate_crc32(&s_spinandDbbt.fingerprint, sizeof(spinand_dbbt_t) - sizeof(s_spinandDbbt.crcChecksum));

    return kStatus_Success;
}

static bool is_bad_block_in_dbbt(uint32_t blockAddr)
{
    uint32_t i;
    // Traversal. No sort when adding items.
    for (i = 0; i < s_spinandDbbt.badBlockNumber; i++)
    {
        if (s_spinandDbbt.badBlockTable[i] == blockAddr)
        {
            return true;
        }
    }
    return false;
}

static status_t bad_block_discovered(uint32_t blockAddr)
{
    if (s_spinandDbbt.badBlockNumber < kSpiNandMemory_MaxDBBTSize)
    {
        s_spinandDbbt.badBlockTable[s_spinandDbbt.badBlockNumber++] = blockAddr;
        s_spinandContext.needToUpdateDbbt = true;
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

static status_t skip_bad_blocks(uint32_t *addr)
{
    *addr += s_spinandContext.skippedBlockCount * s_spinandFcb.config.pagesPerBlock;
    return kStatus_Success;
}

static bool need_to_check_dbbt_before_read(uint32_t blockAddr)
{
    return (!s_spinandContext.isReadBufferValid) ||
           (blockAddr != (s_spinandContext.readBufferPageAddr / s_spinandFcb.config.pagesPerBlock));
}

static bool need_to_check_dbbt_before_write(uint32_t blockAddr)
{
    return (!s_spinandContext.isWriteBufferValid) ||
           (blockAddr != (s_spinandContext.writeBufferPageAddr / s_spinandFcb.config.pagesPerBlock));
}

static bool is_read_page_cached(uint32_t pageAddr)
{
    return (s_spinandContext.readBufferPageAddr == pageAddr) && (s_spinandContext.isReadBufferValid);
}

static bool is_write_page_cached(uint32_t pageAddr)
{
    return (s_spinandContext.writeBufferPageAddr == pageAddr) && (s_spinandContext.isWriteBufferValid);
}

#endif // BL_FEATURE_SPINAND_MODULE

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
