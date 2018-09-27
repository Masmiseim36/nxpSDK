/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "bootloader_common.h"
#include "bootloader/bootloader.h"
#include "memory/memory.h"
#include "normal_memory.h"
#include "semc_nor_memory.h"
#include "semc_nor/semc_nor_flash.h"
#include "bootloader/bl_context.h"
#include "microseconds/microseconds.h"
#include "utilities/fsl_rtos_abstraction.h"
#include "utilities/fsl_assert.h"
#include <string.h>
#if BL_FEATURE_GEN_KEYBLOB
#include "bootloader/bl_keyblob.h"
#endif // BL_FEATURE_GEN_KEYBLOB

#if BL_FEATURE_SEMC_NOR_MODULE
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief SEMC NOR memory feature information
//!
//! An instance is maintained in this file, will is used to keep key information for write and flush
//! operations.
typedef struct _semc_nor_mem_context
{
    bool isConfigured; //!< The state which indicates whether SEMC NOR block is successfully
    semc_nor_config_t norConfig;

    // About Program Buffer
    uint32_t progStartAddress;
    uint32_t cachedBytesInProgBuf;
    uint8_t programBuffer[SEMC_NOR_BUFFER_PROG_CACHE_SIZE];

#if BL_FEATURE_GEN_KEYBLOB
    bool has_keyblob;
    uint32_t keyblob_offset;
#endif // BL_FEATURE_GEN_KEYBLOB
} semc_nor_mem_context_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

//! @brief Adjust the memory info got from CFI
static void semc_nor_adjust_memory_info_from_cfi(void);

//! @brief Update memory map of semc NOR Flash
static status_t semc_nor_update_memory_map(void);

#if BL_FEATURE_GEN_KEYBLOB
static status_t check_update_keyblob_info(void *config);
#endif // BL_FEATURE_GEN_KEYBLOB

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Context of SEMC parallel nor.
static semc_nor_mem_context_t s_semcNorContext = {
    .isConfigured = false,
    .cachedBytesInProgBuf = 0,
};
//! @brief Interface to semc nor memory operations
const memory_region_interface_t g_semcNorMemoryInterface = {
    .init = semc_nor_mem_init,
    .read = semc_nor_mem_read,
    .write = semc_nor_mem_write,
#if !BL_MIN_PROFILE
    .fill = semc_nor_mem_fill,
#endif // #if !BL_MIN_PROFILE
    .flush = semc_nor_mem_flush,
    .erase = semc_nor_mem_erase,
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

#if BL_FEATURE_GEN_KEYBLOB
status_t check_update_keyblob_info(void *config)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((config == NULL) || (s_semcNorContext.isConfigured == false))
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
                s_semcNorContext.has_keyblob = false;
                break;
            }
            s_semcNorContext.keyblob_offset = keyblob_info->keyblob_offset;
            s_semcNorContext.has_keyblob = true;
        }
        else if (keyblob_info_type == kKeyBlobInfoType_Program)
        {
            if (!s_semcNorContext.has_keyblob)
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
            uint32_t page_size;
            status = semc_nor_get_property(kSemcNorProperty_StartAddress, &image_start);
            if (status != kStatus_Success)
            {
                break;
            }
            status = semc_nor_get_property(kSemcNorProperty_TotalFlashSize, &image_max_size);
            if (status != kStatus_Success)
            {
                break;
            }
            status = semc_nor_get_property(kSemcNorProperty_PageSize, &page_size);
            if (status != kStatus_Success)
            {
                break;
            }

            uint32_t keyblob_offset = s_semcNorContext.keyblob_offset;
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
            if (keyblob_addr & (page_size - 1))
            {
                break;
            }

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
            if (semc_nor_flash_verify_erase(keyblob_addr, page_size) != kStatus_Success)
            {
                status = kStatusMemoryCumulativeWrite;
                break;
            }
#endif
            status = semc_nor_mem_write(keyblob_addr, keyblob_size, keyblob_buffer);
            if (status != kStatus_Success)
            {
                break;
            }
        }
    } while (0);

    return status;
}
#endif // #if BL_FEATURE_GEN_KEYBLOB

//! @brief Initialize SEMC NOR memory
status_t semc_nor_mem_init(void)
{
    status_t status;

    // Init device only if current boot device is SEMC NOR
    if (get_primary_boot_device() != kBootDevice_SemcNOR)
    {
        return kStatus_Fail;
    }

    // Get default config block from efuse.
    status = semc_nor_get_default_config_block(&s_semcNorContext.norConfig);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Init SEMC peripheral
    status = semc_nor_flash_init(&s_semcNorContext.norConfig);
    if (status != kStatus_Success)
    {
        return status;
    }

    // If the memory size got from CFI is larger than which is configured for SEMC AXI region,
    //  then adjust the memory size to align with AXI region.
    semc_nor_adjust_memory_info_from_cfi();

    // All initialization steps are success. SEMC NOR can be accessable.
    if (status == kStatus_Success)
    {
        s_semcNorContext.isConfigured = true;
    }

    return status;
}

//! @brief Configure SEMC NOR memory.
status_t semc_nor_mem_config(uint32_t *config)
{
    status_t status;
    uint32_t startAddr = (uint32_t)config;
    uint32_t endAddr = startAddr + sizeof(semc_nor_config_t) - 1;
    // Should check the config is in valid internal space.
    if ((!is_valid_application_location(startAddr)) || (!is_valid_application_location(endAddr)))
    {
        return kStatus_InvalidArgument;
    }

    // Try to check whether the 'config' variable is an option supported by parallel_nor_config_option_t function
    parallel_nor_config_option_t *norOption = (parallel_nor_config_option_t *)config;
#if BL_FEATURE_GEN_KEYBLOB
    keyblob_info_t *keyblob_info = (keyblob_info_t *)config;
#endif // BL_FEATURE_GEN_KEYBLOB

    const semc_nor_config_t *pNorConfig = (const semc_nor_config_t *)config;
    // Check mem config tag.
    if (pNorConfig->memConfig.tag == kSemcConfigBlockTag)
    {
        // All tag check is passed. Then start to configure Parallel NOR device.
        // Over-write config block.
        memcpy(&s_semcNorContext.norConfig, pNorConfig, sizeof(semc_nor_config_t));
    }
    else if (norOption->option.B.tag == kParallelNorConfigOption_Tag)
    {
        norOption->option.B.dataIoPortDiv8 =
            (norOption->option.B.dataIoPortDiv8 != 0) ? norOption->option.B.dataIoPortDiv8 : 1;

        // Get default config block.
        status = semc_nor_get_default_config_block(&s_semcNorContext.norConfig);
        if (status != kStatus_Success)
        {
            return status;
        }

        // Default config block shouldn't depend on eFUSE setting
        s_semcNorContext.norConfig.memConfig.asyncClkFreq = SEMC_2ND_MAX_CLK_FREQ;
        s_semcNorContext.norConfig.memConfig.norMemConfig.addressPortWidth = kSemcMiscProperty_NOR_BaseFlashAddressPortWidth;

        // Configure nor memory according to Option
        s_semcNorContext.norConfig.deviceCommandSet = norOption->option.B.commandSet;
        s_semcNorContext.norConfig.memConfig.norMemConfig.dataPortWidth = norOption->option.B.dataIoPortDiv8 * 8;
        s_semcNorContext.norConfig.memConfig.norMemConfig.advPortPolarity = norOption->option.B.advPortPolarity;
        s_semcNorContext.norConfig.memConfig.norMemConfig.cePortOutputSelection = norOption->option.B.pcsSelection;
        s_semcNorContext.norConfig.acTimingMode = norOption->option.B.acTimingMode;
        if (s_semcNorContext.norConfig.acTimingMode == kSemcNorAcTimingMode_UserDefined)
        {
            semc_mem_config_t *pMemConfig = &s_semcNorContext.norConfig.memConfig;

            pMemConfig->norMemConfig.ceSetupTime = semc_nor_get_specific_ac_cycles(
                &s_semcNorContext.norConfig, kNorAcCycleSetOption_Min, norOption->acTable.min_tCES_ns);
            pMemConfig->norMemConfig.ceMinHoldTime = semc_nor_get_specific_ac_cycles(
                &s_semcNorContext.norConfig, kNorAcCycleSetOption_Min, norOption->acTable.min_tCEH_ns);
            pMemConfig->norMemConfig.ceMinIntervalTime = semc_nor_get_specific_ac_cycles(
                &s_semcNorContext.norConfig, kNorAcCycleSetOption_Min, norOption->acTable.min_tCEITV_ns);
            pMemConfig->norMemConfig.addressSetupTime = semc_nor_get_specific_ac_cycles(
                &s_semcNorContext.norConfig, kNorAcCycleSetOption_Min, norOption->acTable.min_tAS_ns);
            pMemConfig->norMemConfig.addressHoldTime = semc_nor_get_specific_ac_cycles(
                &s_semcNorContext.norConfig, kNorAcCycleSetOption_Min, norOption->acTable.min_tAH_ns);
            pMemConfig->norMemConfig.asyncOeLowTime = semc_nor_get_specific_ac_cycles(
                &s_semcNorContext.norConfig, kNorAcCycleSetOption_Min, norOption->acTable.min_tREL_ns);
            pMemConfig->norMemConfig.asyncOeHighTime = semc_nor_get_specific_ac_cycles(
                &s_semcNorContext.norConfig, kNorAcCycleSetOption_Max, norOption->acTable.max_tREH_ns);
            pMemConfig->norMemConfig.asyncTurnaroundTime = semc_nor_get_specific_ac_cycles(
                &s_semcNorContext.norConfig, kNorAcCycleSetOption_Min, norOption->acTable.min_tTA_ns);
            pMemConfig->norMemConfig.asyncWeLowTime = semc_nor_get_specific_ac_cycles(
                &s_semcNorContext.norConfig, kNorAcCycleSetOption_Min, norOption->acTable.min_tWEL_ns);
            pMemConfig->norMemConfig.asyncWeHighTime = semc_nor_get_specific_ac_cycles(
                &s_semcNorContext.norConfig, kNorAcCycleSetOption_Min, norOption->acTable.min_tWEH_ns);
            pMemConfig->norMemConfig.asyncAddressToDataHoldTime = semc_nor_get_specific_ac_cycles(
                &s_semcNorContext.norConfig, kNorAcCycleSetOption_Min, norOption->acTable.min_tAWDH_ns);
        }
    }
#if BL_FEATURE_GEN_KEYBLOB
    else if (keyblob_info->option.B.tag == kKeyBlobInfoOption_Tag)
    {
        status = check_update_keyblob_info(config);
        return status;
    }
#endif // BL_FEATURE_GEN_KEYBLOB
    else
    {
        return kStatus_SemcNOR_InvalidCfgTag;
    }

    // Mark Parallel NOR as not configured.
    s_semcNorContext.isConfigured = false;

    status = semc_nor_flash_init(&s_semcNorContext.norConfig);
    if (status != kStatus_Success)
    {
        return status;
    }

    // If the memory size got from CFI is larger than which is configured for SEMC AXI region,
    //  then adjust the memory size to align with AXI region.
    semc_nor_adjust_memory_info_from_cfi();

    status = semc_nor_update_memory_map();
    if (status != kStatus_Success)
    {
        return status;
    }

    s_semcNorContext.isConfigured = true;

    return status;
}

//! @brief Read SEMC NOR memory.
status_t semc_nor_mem_read(uint32_t address, uint32_t length, uint8_t *buffer)
{
    // SEMC NOR should be configured before access.
    if (!is_semc_nor_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    return normal_mem_read(address, length, buffer);
}

//! @brief Write SEMC NOR memory.
status_t semc_nor_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    status_t status = kStatus_Success;

    // SEMC NOR should be configured before access.
    if (!is_semc_nor_configured())
    {
        return kStatusMemoryNotConfigured;
    }

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
    if (semc_nor_flash_verify_erase(address, length) != kStatus_Success)
    {
        return kStatusMemoryCumulativeWrite;
    }
#endif // BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE

    while (length)
    {
        // Set start address when storing first byte into program buffer
        if ((!s_semcNorContext.cachedBytesInProgBuf) &&
            (!s_semcNorContext.progStartAddress))
        {
            // Check address alignment
            uint32_t alignmentBaseByte = s_semcNorContext.norConfig.memConfig.norMemConfig.dataPortWidth / 8;
            if (address % alignmentBaseByte)
            {
                return kStatusMemoryAlignmentError;
            }
            s_semcNorContext.progStartAddress = address;
        }
        else
        {
            // Start buffer programming operation when meet discontinuous address
            if ((s_semcNorContext.progStartAddress + s_semcNorContext.cachedBytesInProgBuf) != address)
            {
                // flush cached data into target memory,
                status = semc_nor_mem_flush();
                if (status != kStatus_Success)
                {
                    return status;
                }
                continue;
            }
        }

        uint32_t storeBytes;
        // Check to see if program buffer will be filled with current data packet
        if ((s_semcNorContext.cachedBytesInProgBuf + length) <= sizeof(s_semcNorContext.programBuffer))
        {
            storeBytes = length;
        }
        else
        {
            storeBytes = sizeof(s_semcNorContext.programBuffer) - s_semcNorContext.cachedBytesInProgBuf;
        }

        // Copy data to program buffer
        memcpy(&s_semcNorContext.programBuffer[s_semcNorContext.cachedBytesInProgBuf], buffer, storeBytes);

        s_semcNorContext.cachedBytesInProgBuf += storeBytes;
        buffer += storeBytes;
        address += storeBytes;
        length -= storeBytes;

        // Start buffer programming operation when program buffer is full
        if (s_semcNorContext.cachedBytesInProgBuf == sizeof(s_semcNorContext.programBuffer))
        {
            // flush cached data into target memory,
            status = semc_nor_mem_flush();
            if (status != kStatus_Success)
            {
                return status;
            }
        }
    }

    return kStatus_Success;
}

//! @brief Fill SEMC NOR memory with a word pattern.
status_t semc_nor_mem_fill(uint32_t address, uint32_t length, uint32_t pattern)
{
    status_t status;
    uint32_t flashStartAddress;
    uint32_t flashTotalSize;
    uint32_t fillEndAddress = address + length;

    // SEMC NOR should be configured before access.
    if (!is_semc_nor_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    semc_nor_get_property(kSemcNorProperty_StartAddress, &flashStartAddress);
    semc_nor_get_property(kSemcNorProperty_TotalFlashSize, &flashTotalSize);

    // Check the memory range
    if ((address < flashStartAddress) || (fillEndAddress > (flashStartAddress + flashTotalSize)))
    {
        return kStatus_SemcNOR_InvalidMemoryAddress;
    }

    // Note: the check for "length != 0" and "range not in reserved region" is done in mem_fill().
    assert(length);
    // Pre-fill program buffer with pattern
    uint32_t *buffer = (uint32_t *)s_semcNorContext.programBuffer;
    uint32_t maxPatterns = sizeof(s_semcNorContext.programBuffer) >> 2;
    for (uint32_t i = 0; i < maxPatterns; i++)
    {
        *buffer++ = pattern;
    }

    while (length)
    {
        uint32_t bytes;
        s_semcNorContext.cachedBytesInProgBuf = 0;
        // Check to see if remaining address range can hold whole program buffer
        if (length < sizeof(s_semcNorContext.programBuffer))
        {
            bytes = length;
        }
        else
        {
            bytes = sizeof(s_semcNorContext.programBuffer);
        }

        // flush cached data into target memory,
        status = semc_nor_mem_write(address, bytes, s_semcNorContext.programBuffer);
        if (status != kStatus_Success)
        {
            return status;
        }

        address += bytes;
        length -= bytes;
    }

    // flush cached data into target memory,
    status = semc_nor_mem_flush();

    return status;
}

//! @brief Flush cached data into SEMC NOR memory.
status_t semc_nor_mem_flush(void)
{
    status_t status = kStatus_Success;

    if (s_semcNorContext.cachedBytesInProgBuf)
    {
        uint32_t address = s_semcNorContext.progStartAddress;
        uint32_t length = s_semcNorContext.cachedBytesInProgBuf;

        // Clear related states no matter following operations are executed successfully or not.
        s_semcNorContext.progStartAddress = 0;
        s_semcNorContext.cachedBytesInProgBuf = 0;

        // Program data into SEMC NOR memory
        status = semc_nor_flash_page_program(&s_semcNorContext.norConfig, address, s_semcNorContext.programBuffer, length);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    return status;
}

//! @brief Erase SEMC NOR memory
status_t semc_nor_mem_erase(uint32_t address, uint32_t length)
{
    status_t status;
    uint32_t flashStartAddress;
    uint32_t flashBlockSize;
    uint32_t flashTotalSize;
    uint32_t eraseEndAddress = address + length;

    // SEMC NOR should be configured before access.
    if (!is_semc_nor_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    semc_nor_get_property(kSemcNorProperty_StartAddress, &flashStartAddress);
    semc_nor_get_property(kSemcNorProperty_BlockSize, &flashBlockSize);
    semc_nor_get_property(kSemcNorProperty_TotalFlashSize, &flashTotalSize);

    // Check the memory range
    if ((address < flashStartAddress) || (eraseEndAddress > (flashStartAddress + flashTotalSize)))
    {
        return kStatus_SemcNOR_InvalidMemoryAddress;
    }

    // length = 0 means no erase operation will be executed. Just return success.
    if (length == 0)
    {
        return kStatus_Success;
    }

    while (address < eraseEndAddress)
    {
        uint32_t blockIndex = (address - flashStartAddress) / flashBlockSize;
        status = semc_nor_flash_erase_block(&s_semcNorContext.norConfig, blockIndex);
        if (status != kStatus_Success)
        {
            return status;
        }
        address += flashBlockSize;
    }

    return status;
}

//! @brief Erase all SEMC NOR memory
status_t semc_nor_mem_erase_all(void)
{
    uint32_t flashStartAddress;
    uint32_t flashTotalSize;

    semc_nor_get_property(kSemcNorProperty_StartAddress, &flashStartAddress);
    semc_nor_get_property(kSemcNorProperty_TotalFlashSize, &flashTotalSize);

    return semc_nor_mem_erase(flashStartAddress, flashTotalSize);
}

//! @brief Get Property from semc nor driver
status_t semc_nor_get_property(uint32_t whichProperty, uint32_t *value)
{
    if (value == NULL)
    {
        return kStatus_InvalidArgument;
    }

    switch (whichProperty)
    {
        case kSemcNorProperty_InitStatus:
            *value = is_semc_nor_configured() ? kStatus_Success : kStatusMemoryNotConfigured;
            break;

        case kSemcNorProperty_StartAddress:
            *value = s_semcNorContext.norConfig.memConfig.norMemConfig.comMemBaseAddress;
            break;

        case kSemcNorProperty_TotalFlashSizeInKBytes:
            *value = s_semcNorContext.norConfig.blockSizeInBytes * s_semcNorContext.norConfig.blockCount / 1024;
            break;

        case kSemcNorProperty_PageSize:
            *value = s_semcNorContext.norConfig.pageSizeInBytes;
            break;

        case kSemcNorProperty_BlockSize:
            if (s_semcNorContext.norConfig.blockSizeInBytes)
            {
                *value = s_semcNorContext.norConfig.blockSizeInBytes;
            }
            else
            {
                return kStatus_InvalidArgument;
            }
            break;

        case kSemcNorProperty_TotalFlashSize:
            *value = s_semcNorContext.norConfig.blockSizeInBytes * s_semcNorContext.norConfig.blockCount;
            break;

        default: // catch inputs that are not recognized
            return kStatus_InvalidArgument;
    }

    return kStatus_Success;
}

//! @brief Get the status of semc nor configuration
bool is_semc_nor_configured(void)
{
    return s_semcNorContext.isConfigured;
}

//! @brief Adjust the memory info got from CFI
void semc_nor_adjust_memory_info_from_cfi(void)
{
    uint32_t flashBlockSize;
    uint32_t flashTotalSize;

    semc_nor_get_property(kSemcNorProperty_BlockSize, &flashBlockSize);
    semc_nor_get_property(kSemcNorProperty_TotalFlashSize, &flashTotalSize);

    if (flashTotalSize > s_semcNorContext.norConfig.memConfig.norMemConfig.comMemSizeInByte)
    {
        s_semcNorContext.norConfig.blockCount =
            s_semcNorContext.norConfig.memConfig.norMemConfig.comMemSizeInByte / flashBlockSize;
    }
}

//! @brief Update memory map of semc NOR Flash
static status_t semc_nor_update_memory_map(void)
{
    uint32_t flashStartAddress;
    uint32_t flashTotalSize;

    semc_nor_get_property(kSemcNorProperty_StartAddress, &flashStartAddress);
    semc_nor_get_property(kSemcNorProperty_TotalFlashSize, &flashTotalSize);

    g_memoryMap[kIndexSemcNor].startAddress = flashStartAddress;
    g_memoryMap[kIndexSemcNor].endAddress = g_memoryMap[kIndexSemcNor].startAddress + flashTotalSize - 1;
    g_memoryMap[kIndexSemcNor].memoryProperty = (kMemoryIsExecutable | kMemoryType_FLASH);

    return kStatus_Success;
}

#endif // #if BL_FEATURE_SEMC_NOR_MODULE

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
