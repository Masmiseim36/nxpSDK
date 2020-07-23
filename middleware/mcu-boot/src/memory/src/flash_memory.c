/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "memory/memory.h"
#include "flash_memory.h"
#include "normal_memory.h"
#include "fsl_flash.h"
#include "bootloader/bl_context.h"
#include "fsl_device_registers.h"
#include "utilities/fsl_rtos_abstraction.h"
#include "utilities/fsl_assert.h"
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// If the bootloader is running from flash, then we need to make sure that all
// interrupts are disabled during the execution of a flash operation, so that
// no code is unexpectedly run from flash (which would cause a hard fault).
//
// If we're running from ROM or RAM, then we neither need to nor want to disable
// interrupts during flash operations.
#if !(defined(BL_TARGET_FLASH) && BL_TARGET_FLASH)
#define flash_lock_release() (void)sizeof(0)
#define flash_lock_acquire() (void)sizeof(0)
#endif // BL_TARGET_FLASH

//! @brief Flash Memory constants.
enum _flash_memory_constants
{
    kFlashMemory_ErasedValue = ~0
};

#if defined(BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED) && BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED
//! @brief FLASH section program memory context
//!
//! An instance is maintained in this file, will is used to keep key information for write and flush
//! operatations.
typedef struct _flash_section_program_info
{
    uint32_t startAddress;                      //!< This address is used to record the address which is used
                                                //!< to write the whole section into flash memory
    uint32_t storedBytes;                       //!< A variable which is used to indicate if the buffer is full.
    uint8_t buffer[0x400U]; //!< A buffer which is used to buffer a full section of data
    //uint8_t buffer[kFLASH_AccelerationRamSize]; //!< A buffer which is used to buffer a full section of data
} flash_section_program_info_t;

static flash_section_program_info_t s_flash_section_program_info;
#endif

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief flash memory config block.
flash_config_t g_flashState;
ftfx_cache_config_t g_flashcacheState;

//! @brief Interface to main flash memory operations.
const memory_region_interface_t g_flashMemoryInterface = {
    .init = &flash_mem_init,
    .read = &flash_mem_read,
    .write = &flash_mem_write,
#if !(defined(BL_FEATURE_MIN_PROFILE) && BL_FEATURE_MIN_PROFILE) || \
    defined(BL_FEATURE_FILL_MEMORY) && BL_FEATURE_FILL_MEMORY
    .fill = &flash_mem_fill,
#endif // !BL_FEATURE_MIN_PROFILE
#if defined(BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED) && BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED
    .flush = flash_mem_flush,
#else
    .flush = (void *)0u,
#endif
    .erase = flash_mem_erase,
};

#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
//! @brief It is used for indicating if an XA controlled region is unlocked to program state
bool isFlashRegionUnlocked = (_Bool)false;
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL

#if defined(BL_TARGET_FLASH) && BL_TARGET_FLASH
static uint32_t s_regPrimask = 0U;
#endif

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
//! @brief check if a flash region is in an XA controlled region or contains an XA controlled region.
//         and try to open flash program state by calling verify_erase_all command if needed.
#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
status_t flash_preprocess_execute_only_region(uint32_t address,
                                              uint32_t length,
                                              flash_xacc_state_t *state);
#endif

status_t flash_check_access_before_programming(uint32_t address, uint32_t length, bool *verifyWrites);

static uint32_t flash_get_instance(uint32_t address, uint32_t length);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

#if defined(BL_TARGET_FLASH) && BL_TARGET_FLASH
static void flash_lock_acquire(void)
{
    // Ensure that the program operation cannots be interrupted.
    s_regPrimask = __get_PRIMASK();
    __disable_irq();
}

static void flash_lock_release(void)
{
    // Release lock after the write operation completes.
    __set_PRIMASK(s_regPrimask);
}
#endif

// See flash_memory.h for documentation on this function.
status_t flash_mem_init(void)
{
    // Update address range of flash
    memory_map_entry_t *map;
    uint32_t tmp;

    union
    {
        memory_map_entry_t const *map;
        memory_map_entry_t *pMap;
    } map_ptr;

    map_ptr.map = &g_bootloaderContext.memoryMap[(uint32_t)kIndexFlashArray];
    map = map_ptr.pMap;

    (void)g_bootloaderContext.flashDriverInterface->flash_get_property(
        g_bootloaderContext.allFlashState, kFLASH_PropertyPflash0BlockBaseAddr, &map->startAddress);
    (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                 kFLASH_PropertyPflash0TotalSize, &tmp);
    map->endAddress = map->startAddress + tmp - 1u;

#if defined(BL_HAS_SECONDARY_INTERNAL_FLASH) && BL_HAS_SECONDARY_INTERNAL_FLASH
    map = (memory_map_entry_t *)&g_bootloaderContext.memoryMap[(uint32_t)kIndexSenondaryFlashArray];
    (void)g_bootloaderContext.flashDriverInterface->flash_get_property(
        g_bootloaderContext.allFlashState, kFLASH_PropertyPflash1BlockBaseAddr, &map->startAddress);
    (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                 kFLASH_PropertyPflash1TotalSize, &tmp);
    map->endAddress = map->startAddress + tmp - 1u;
#endif

#if defined(BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED) && BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED
    s_flash_section_program_info.storedBytes = 0u;
#endif

    return (int32_t)kStatus_Success;
}

// See flash_memory.h for documentation on this function.
status_t flash_mem_read(uint32_t address, uint32_t length, uint8_t *buffer)
{
    status_t status = (int32_t)kStatus_Success;

#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    flash_xacc_state_t access_state;
    uint32_t flashMemoryIndex = flash_get_instance(address, length);
    uint32_t alignedAddress = ALIGN_DOWN(address, g_bootloaderContext.allFlashState->ftfxConfig[flashMemoryIndex].flashDesc.accessSegmentMem.size);
    uint32_t updatedLength = address - alignedAddress + length;
    uint32_t alignedLength = ALIGN_UP(updatedLength, g_bootloaderContext.allFlashState->ftfxConfig[flashMemoryIndex].flashDesc.accessSegmentMem.size);
    status = g_bootloaderContext.flashDriverInterface->flash_is_execute_only(
        g_bootloaderContext.allFlashState, alignedAddress, alignedLength, &access_state);
    if (status == (int32_t)kStatus_Success)
    {
        if (access_state != kFLASH_AccessStateUnLimited)
        {
            status = (int32_t)kStatus_FTFx_RegionExecuteOnly;
        }
    }
    if (status == (int32_t)kStatus_Success)
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    {
        status = normal_mem_read(address, length, buffer);
    }

    return status;
}

#if !(defined(BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED) && BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED)
// See flash_memory.h for documentation on this function.
status_t flash_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    // Note: the check for "length != 0" and "range not in reserved region" is done in mem_write().
    assert(length);
    assert(buffer);

    status_t status = (int32_t)kStatus_Success;
    uint32_t alignedLength;
    uint32_t extraBytes;
    uint32_t extraData[2];
    uint32_t flashMemoryIndex = flash_get_instance(address, length);
    uint32_t alignmentBaseUnit = g_bootloaderContext.allFlashState->ftfxConfig[flashMemoryIndex].opsConfig.addrAligment.blockWriteUnitSize;

    assert(sizeof(extraData) >= sizeof(uint8_t) * alignmentBaseUnit);

    bool verifyWrites;
    status = flash_check_access_before_programming(address, length, &verifyWrites);
    if (status == (int32_t)kStatus_Success)
    {
        // Align length to whole words.
        alignedLength = ALIGN_DOWN(length, sizeof(uint8_t) * alignmentBaseUnit);
        extraBytes = length - alignedLength;
        assert(extraBytes < sizeof(uint8_t) * alignmentBaseUnit);

        // Pre-fill word buffer with flash erased value.
        extraData[0] = (uint32_t)kFlashMemory_ErasedValue;
        extraData[1] = (uint32_t)kFlashMemory_ErasedValue;
        if (extraBytes != 0u)
        {
            // Copy extra bytes to word buffer.
            (void)memcpy((uint8_t *)extraData, &buffer[alignedLength], extraBytes);
        }

        flash_lock_acquire();
        (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, true);
        // Program whole words from the user's buffer.
        if (alignedLength != 0u)
        {
            status = g_bootloaderContext.flashDriverInterface->flash_program(g_bootloaderContext.allFlashState, address,
                                                                             (uint8_t *)buffer, alignedLength);
        }
        if ((status == (int32_t)kStatus_Success) && extraBytes != 0u)
        {
            // Program trailing word.
            status = g_bootloaderContext.flashDriverInterface->flash_program(
                g_bootloaderContext.allFlashState, address + alignedLength, (uint8_t *)extraData,
                alignmentBaseUnit);
        }
        (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, (_Bool)false);
        flash_lock_release();
        if (status == (int32_t)kStatus_Success)
        {
#if !(defined(BL_FEATURE_FLASH_VERIFY_DISABLE) && BL_FEATURE_FLASH_VERIFY_DISABLE)
            if (verifyWrites)
            {
                uint32_t failedAddress;
                uint32_t failedData;

                flash_lock_acquire();
                if (alignedLength != 0u)
                {
                    status = g_bootloaderContext.flashDriverInterface->flash_verify_program(
                        g_bootloaderContext.allFlashState, address, alignedLength, (uint8_t *)buffer,
                        (ftfx_margin_value_t)g_bootloaderContext.propertyInterface->store->flashReadMargin, &failedAddress,
                        &failedData);
                }
                if ((status == (int32_t)kStatus_Success) && extraBytes != 0u)
                {
                    status = g_bootloaderContext.flashDriverInterface->flash_verify_program(
                        g_bootloaderContext.allFlashState, address + alignedLength, sizeof(extraData), (uint8_t *)extraData,
                        (ftfx_margin_value_t)g_bootloaderContext.propertyInterface->store->flashReadMargin, &failedAddress,
                        &failedData);
                }
                flash_lock_release();
                if (status != (int32_t)kStatus_Success)
                {
                    debug_printf("Error: flash verify failed at address: 0x%x\r\n", failedAddress);
                }
            }
#endif // !BL_FEATURE_FLASH_VERIFY_DISABLE
        }
    }

    return status;
}

// See flash_memory.h for documentation on this function.
status_t flash_mem_fill(uint32_t address, uint32_t length, uint32_t pattern)
{
    // Note: the check for "length != 0"
    assert(length);

    status_t status = (int32_t)kStatus_Success;
    uint32_t patternBuffer[8];

    // Pre-fill pattern buffer with pattern.
    for (uint32_t i = 0u; i < 8u; i++)
    {
        patternBuffer[i] = pattern;
    }

    // Program patterns from the pattern buffer.
    while (length != 0u)
    {
        uint32_t bytes;
        if (length < sizeof(patternBuffer))
        {
            bytes = length;
        }
        else
        {
            bytes = sizeof(patternBuffer);
        }

        status = flash_mem_write(address, bytes, (uint8_t *)patternBuffer);
        if (status != (int32_t)kStatus_Success)
        {
            break;
        }

        address += bytes;
        length -= bytes;
    }

    return status;
}

#else // BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED

// See flash_memory.h for documentation on this function.
status_t flash_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    // Note: the check for "length != 0" and "range not in reserved region" is done in mem_write().
    assert(length);
    assert(buffer);

    status_t status = (int32_t)kStatus_Success;
    uint32_t flashMemoryIndex = flash_get_instance(address, length);
    uint32_t alignmentBaseUnit = g_bootloaderContext.allFlashState->ftfxConfig[flashMemoryIndex].opsConfig.addrAligment.sectionCmd;

    while (length != 0u)
    {
        // Set start address when storing first byte into section program buffer
        if ((s_flash_section_program_info.storedBytes == 0u) &&
            (s_flash_section_program_info.startAddress == 0u))
        {
            // Check address alignment
            if ((address & (alignmentBaseUnit - 1u)) != 0u)
            {
                status = kStatus_FTFx_AlignmentError;
                break;
            }
            s_flash_section_program_info.startAddress = address;
        }
        else
        {
            // Start section programming operation when meet discontinuous address
            if ((s_flash_section_program_info.startAddress + s_flash_section_program_info.storedBytes) != address)
            {
                // flush cached data into target memory,
                status = flash_mem_flush();
                if (status != (int32_t)kStatus_Success)
                {
                    break;
                }
                continue;
            }
        }

        uint32_t storeBytes;
        // Check to see if section program buffer will be filled with current data packet
        if ((s_flash_section_program_info.storedBytes + length) <= sizeof(s_flash_section_program_info.buffer))
        {
            storeBytes = length;
        }
        else
        {
            storeBytes = sizeof(s_flash_section_program_info.buffer) - s_flash_section_program_info.storedBytes;
        }

        // Copy data to section program buffer
        if (buffer != &s_flash_section_program_info.buffer[s_flash_section_program_info.storedBytes])
        {
            union
            {
                uint8_t *bytes;
                void *pBuffer;
            } buffer_ptr;
            union
            {
                const uint8_t *bytes;
                const void *pBuffer;
            } buffer_ptr1;
            buffer_ptr.bytes = &s_flash_section_program_info.buffer[s_flash_section_program_info.storedBytes];
            buffer_ptr1.bytes = buffer;
            if (storeBytes != 0u)
            {
                (void)memcpy(buffer_ptr.pBuffer, buffer_ptr1.pBuffer, storeBytes);
            }
        }

        s_flash_section_program_info.storedBytes += storeBytes;
        buffer += storeBytes;
        address += storeBytes;
        length -= storeBytes;

        // Start section programming operation when section program buffer is full
        if (s_flash_section_program_info.storedBytes == sizeof(s_flash_section_program_info.buffer))
        {
            // flush cached data into target memory,
            status = flash_mem_flush();
            if (status != (int32_t)kStatus_Success)
            {
                break;
            }
        }
    }

    return status;
}

// See flash_memory.h for documentation on this function.
status_t flash_mem_fill(uint32_t address, uint32_t length, uint32_t pattern)
{
    // Note: the check for "length != 0" and "range not in reserved region" is done in mem_fill().
    assert(length);

    status_t status = (int32_t)kStatus_Success;

    // Pre-fill section program buffer with pattern

    union
    {
        uint8_t *address;
        uint32_t *buffer;
    } buffer_ptr;

    buffer_ptr.address = s_flash_section_program_info.buffer;
    uint32_t *buffer = buffer_ptr.buffer;

    uint32_t maxPatterns = sizeof(s_flash_section_program_info.buffer) >> 2u;
    for (uint32_t i = 0u; i < maxPatterns; i++)
    {
        *buffer++ = pattern;
    }

    while (length != 0u)
    {
        uint32_t bytes;

        s_flash_section_program_info.storedBytes = 0u;

        // Check to see if remaining address range can hold whole section program buffer
        if (length < sizeof(s_flash_section_program_info.buffer))
        {
            bytes = length;
        }
        else
        {
            bytes = sizeof(s_flash_section_program_info.buffer);
        }

        // flush cached data into target memory,
        status = flash_mem_write(address, bytes, s_flash_section_program_info.buffer);
        if (status != (int32_t)kStatus_Success)
        {
            break;
        }

        address += bytes;
        length -= bytes;
    }

    if (status == (int32_t)kStatus_Success)
    {
        // flush cached data into target memory,
        status = flash_mem_flush();
    }

    return status;
}

// See memory.h for documentation on this function.
status_t flash_mem_flush(void)
{
    status_t status = (int32_t)kStatus_Success;

    if (s_flash_section_program_info.storedBytes != 0u)
    {
        uint32_t address = s_flash_section_program_info.startAddress;
        uint32_t length = s_flash_section_program_info.storedBytes;
        uint32_t flashMemoryIndex = flash_get_instance(address, length);
        uint32_t alignmentBaseUnit = g_bootloaderContext.allFlashState->ftfxConfig[flashMemoryIndex].opsConfig.addrAligment.sectionCmd;

        // Clear related states no matter following operations are executed successfully or not.
        s_flash_section_program_info.startAddress = 0;
        s_flash_section_program_info.storedBytes = 0;

        // Align length to section program unit
        uint32_t alignedLength = ALIGN_UP(length, alignmentBaseUnit);

        bool verifyWrites;
        status = flash_check_access_before_programming(address, length, &verifyWrites);
        if (status == (int32_t)kStatus_Success)
        {
            // Fill unused region with oxFFs.
            assert(alignedLength <= sizeof(s_flash_section_program_info.buffer));
            assert(length <= sizeof(s_flash_section_program_info.buffer));
            if (length < alignedLength)
            {
                (void)memset(&s_flash_section_program_info.buffer[length], 0xFF, alignedLength - length);
            }

            flash_lock_acquire();
            (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, true);
            // Write data of aligned length to flash
            status = FLASH_ProgramSection(g_bootloaderContext.allFlashState, address,
                                          (uint8_t *)s_flash_section_program_info.buffer, alignedLength);
            (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, false);
            flash_lock_release();

            if (status == (int32_t)kStatus_Success)
            {
// Verify wether the data has been programmed to flash successfully.
#if !(defined(BL_FEATURE_FLASH_VERIFY_DISABLE) && BL_FEATURE_FLASH_VERIFY_DISABLE)
                // Verify flash program
                if (verifyWrites)
                {
                    uint32_t failedAddress;
                    uint32_t failedData;

                    flash_lock_acquire();
                    status = g_bootloaderContext.flashDriverInterface->flash_verify_program(
                        g_bootloaderContext.allFlashState, address, alignedLength,
                        (uint8_t *)&s_flash_section_program_info.buffer,
                        (ftfx_margin_value_t)g_bootloaderContext.propertyInterface->store->flashReadMargin, &failedAddress,
                        &failedData);
                    flash_lock_release();
                    if (status != (int32_t)kStatus_Success)
                    {
                        debug_printf("Error: flash verify failed at address: 0x%x\r\n", failedAddress);
                    }
                }
#endif // !BL_FEATURE_FLASH_VERIFY_DISABLE
            }
        }
    }

    return status;
}
#endif // !BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED

// See memory.h for documentation on this function.
status_t flash_mem_erase(uint32_t address, uint32_t length)
{
    status_t status;

    flash_lock_acquire();
    (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, true);
    status = g_bootloaderContext.flashDriverInterface->flash_erase(g_bootloaderContext.allFlashState, address, length,
                                                                   kFLASH_ApiEraseKey);
    (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, false);
    flash_lock_release();

#if !(defined(BL_FEATURE_FLASH_VERIFY_DISABLE) && BL_FEATURE_FLASH_VERIFY_DISABLE)
    if ((status == (int32_t)kStatus_Success) && (g_bootloaderContext.propertyInterface->store->verifyWrites != 0u))
    {
        flash_lock_acquire();
        status = g_bootloaderContext.flashDriverInterface->flash_verify_erase(
            g_bootloaderContext.allFlashState, address, length,
            (ftfx_margin_value_t)g_bootloaderContext.propertyInterface->store->flashReadMargin);
        flash_lock_release();
        if (status != (int32_t)kStatus_Success)
        {
            debug_printf("Error: flash_verify_erase failed\r\n");
        }
    }
#endif // !BL_FEATURE_FLASH_VERIFY_DISABLE

    return status;
}

// See memory.h for documentation on this function.
#if defined(BL_FEATURE_FAC_ERASE) && BL_FEATURE_FAC_ERASE
status_t flash_mem_erase_all(flash_erase_all_option_t eraseOption)
#else
status_t flash_mem_erase_all(void)
#endif
{
    status_t status = (int32_t)kStatus_Success;
    bool isReservedRegionEmpty = true;;

// Decompose the the flash erase all into two region erases.
#if defined(BL_TARGET_FLASH) && BL_TARGET_FLASH
    reserved_region_t *reservedRegion =
        &g_bootloaderContext.propertyInterface->store->reservedRegions[kProperty_FlashReservedRegionIndex];
    const uint32_t eraseSize = g_bootloaderContext.propertyInterface->store->flashSectorSize[kFlashIndex_Main];

#if defined(BL_FEATURE_FAC_ERASE) && BL_FEATURE_FAC_ERASE
    if (eraseOption == kFlashEraseAllOption_ExecuteOnlySegments)
    {
        uint32_t address = ALIGN_DOWN(reservedRegion->startAddress, eraseSize);
        uint32_t length = ALIGN_UP(reservedRegion->endAddress, eraseSize) - address;
        flash_xacc_state_t access_state;

        // If the reserved flash region is in an execute-only protected segment, then FAC erase command is not
        // allowed
        flash_lock_acquire();
        status = flash_preprocess_execute_only_region(address, length, &access_state);
        if (status == (int32_t)kStatus_Success)
        {
            flash_lock_release();
            if (access_state != kFLASH_AccessStateUnLimited)
            {
                status = (int32_t)kStatusMemoryAppOverlapWithExecuteOnlyRegion;
            }
        }
    }
    else if (eraseOption == kFlashEraseAllOption_Blocks)
#endif
    {

        union
        {
            memory_map_entry_t const *map;
            memory_map_entry_t *pMap;
        } map_ptr;

        map_ptr.map = &g_bootloaderContext.memoryMap[(uint32_t)kIndexFlashArray];
        memory_map_entry_t *map = map_ptr.pMap;

        isReservedRegionEmpty =
            (reservedRegion->startAddress == map->startAddress) && (reservedRegion->endAddress == map->startAddress);
        if (!isReservedRegionEmpty)
        {
            // Erase the initial unreserved region, if any.
            if (reservedRegion->startAddress > map->startAddress)
            {
                uint32_t length = ALIGN_DOWN(reservedRegion->startAddress, eraseSize);
                if (length > 0u)
                {
                    status = flash_mem_erase(map->startAddress, length);
                }
            }

            // Erase the final unreserved region, if any.
            if (status == (int32_t)kStatus_Success)
            {
                uint32_t start = ALIGN_UP(reservedRegion->endAddress, eraseSize);
                if (start < map->endAddress)
                {
                    status = flash_mem_erase(start, (map->endAddress + 1u) - start);
                }
            }
        }
    }

#endif // BL_TARGET_FLASH

    // Do full erase and verify.
    if (status == (int32_t)kStatus_Success && isReservedRegionEmpty)
    {
        flash_lock_acquire();
        (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, true);
#if defined(BL_FEATURE_FAC_ERASE) && BL_FEATURE_FAC_ERASE
        if (eraseOption == kFlashEraseAllOption_ExecuteOnlySegments)
        {
            status = g_bootloaderContext.flashDriverInterface->flash_erase_all_execute_only_segments(
                &g_bootloaderContext.allFlashState, kFLASH_ApiEraseKey);
        }
        else if (eraseOption == kFlashEraseAllOption_Blocks)
#endif
        {
            status = g_bootloaderContext.flashDriverInterface->flash_erase_all(g_bootloaderContext.allFlashState,
                                                                               kFLASH_ApiEraseKey);
        }
        (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, false);
        flash_lock_release();

#if !(defined(BL_FEATURE_FLASH_VERIFY_DISABLE) && BL_FEATURE_FLASH_VERIFY_DISABLE)
        if ((status == (int32_t)kStatus_Success) && (g_bootloaderContext.propertyInterface->store->verifyWrites != 0u))
        {
            flash_lock_acquire();
#if defined(BL_FEATURE_FAC_ERASE) && BL_FEATURE_FAC_ERASE
            if (eraseOption == kFlashEraseAllOption_ExecuteOnlySegments)
            {
                status = g_bootloaderContext.flashDriverInterface->flash_verify_erase_all_execute_only_segments(
                    &g_bootloaderContext.allFlashState,
                    (ftfx_margin_value_t)g_bootloaderContext.propertyInterface->store->flashReadMargin);
            }
            else if (eraseOption == kFlashEraseAllOption_Blocks)
#endif
            {
                status = g_bootloaderContext.flashDriverInterface->flash_verify_erase_all(
                    g_bootloaderContext.allFlashState,
                    (ftfx_margin_value_t)g_bootloaderContext.propertyInterface->store->flashReadMargin);
            }
            flash_lock_release();
            if (status != (int32_t)kStatus_Success)
            {
                debug_printf("Error: flash_verify_erase_all/all_execute_only_segments failed\r\n");
            }
        }
#endif // !BL_FEATURE_FLASH_VERIFY_DISABLE

#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
        if (status == (int32_t)kStatus_Success)
        {
            isFlashRegionUnlocked = true;
        }
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    }
    return status;
}

// See memory.h for documentation on this function.
status_t flash_mem_erase_all_unsecure(void)
{
    status_t status;

    flash_lock_acquire();
    (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, true);
    status = g_bootloaderContext.flashDriverInterface->flash_erase_all_unsecure(g_bootloaderContext.allFlashState,
                                                                                kFLASH_ApiEraseKey);
    (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, false);
    flash_lock_release();

    return status;
}

static uint32_t flash_get_instance(uint32_t address, uint32_t length)
{
    uint32_t instance = (uint32_t)kFlashIndex_Main;
    
#if defined(BL_HAS_SECONDARY_INTERNAL_FLASH) && BL_HAS_SECONDARY_INTERNAL_FLASH
    for (uint32_t i = (uint32_t)kIndexFlashArray; i <= (uint32_t)kIndexSenondaryFlashArray; i++)
    {
        memory_map_entry_t *map = (memory_map_entry_t *)&g_bootloaderContext.memoryMap[i];
        if ((address >= map->startAddress) && (address + length <= map->endAddress + 1u))
        {
            instance = i;
            break;
        }
    }
#endif
    return instance;
}

//! @brief check if a flash region is in an XA controlled region or contains an XA controlled region.
//         and try to open flash program state by calling verify_erase_all command if needed.
#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
status_t flash_preprocess_execute_only_region(uint32_t address,
                                              uint32_t length,
                                              flash_xacc_state_t *state)
{
    status_t status = (int32_t)kStatus_Success;
    *state = kFLASH_AccessStateUnLimited;

#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    // If a target flash location is in an execute-only protected segment, these program commands are not
    // allowed unless a Read 1s All Blocks command is executed and returns with a pass code
    flash_xacc_state_t access_state;

    status = g_bootloaderContext.flashDriverInterface->flash_is_execute_only(g_bootloaderContext.allFlashState, address,
                                                                             length, &access_state);
    if (status == (int32_t)kStatus_Success)
    {
        *state = access_state;

        if ((access_state != kFLASH_AccessStateUnLimited) && (!isFlashRegionUnlocked))
        {
#if defined(BL_FEATURE_FAC_ERASE) && BL_FEATURE_FAC_ERASE
            status = g_bootloaderContext.flashDriverInterface->flash_verify_erase_all_execute_only_segments(
                &g_bootloaderContext.allFlashState,
                (ftfx_margin_value_t)g_bootloaderContext.propertyInterface->store->flashReadMargin);
#else
            status = g_bootloaderContext.flashDriverInterface->flash_verify_erase_all(
                g_bootloaderContext.allFlashState,
                (ftfx_margin_value_t)g_bootloaderContext.propertyInterface->store->flashReadMargin);
#endif
            if (status != (int32_t)kStatus_Success)
            {
                status = (int32_t)kStatus_FTFx_RegionExecuteOnly;
            }

            isFlashRegionUnlocked = true;
        }
    }
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    return status;
}
#endif

status_t flash_check_access_before_programming(uint32_t address, uint32_t length, bool *verifyWrites)
{
    status_t status = (int32_t)kStatus_Success;
    uint32_t flashMemoryIndex = flash_get_instance(address, length);
    uint32_t alignmentBaseUnit = g_bootloaderContext.allFlashState->ftfxConfig[flashMemoryIndex].opsConfig.addrAligment.blockWriteUnitSize;

#if !(defined(BL_FEATURE_FLASH_VERIFY_DISABLE) && BL_FEATURE_FLASH_VERIFY_DISABLE)
    *verifyWrites = (g_bootloaderContext.propertyInterface->store->verifyWrites != 0u)? true: (_Bool)false;
#endif // BL_FEATURE_FLASH_VERIFY_DISABLE

#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    // If a target flash location is in an execute-only protected segment, these program commands are not
    // allowed unless a Read 1s All Blocks command is executed and returns with a pass code
    flash_xacc_state_t access_state;

    uint32_t actualLength = ALIGN_UP(length, alignmentBaseUnit);
    flash_lock_acquire();
    status = flash_preprocess_execute_only_region(address, actualLength, &access_state);
    if (status == (int32_t)kStatus_Success)
    {
        flash_lock_release();

#if !(defined(BL_FEATURE_FLASH_VERIFY_DISABLE) && BL_FEATURE_FLASH_VERIFY_DISABLE)
        if (access_state != kFLASH_AccessStateUnLimited)
        {
            *verifyWrites = (_Bool)false;
        }
#endif // BL_FEATURE_FLASH_VERIFY_DISABLE
    }
    else
    {
        status = status; \\ doing nothing
    }
    if (status == (int32_t)kStatus_Success)
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    {
// Do cumulative write check
#if defined(BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE) && BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE || \
    defined(FSL_FEATURE_SOC_FTFE_COUNT) && FSL_FEATURE_SOC_FTFE_COUNT
#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
        bool isCumulativeCheckNeeded = true;
        if (access_state != kFLASH_AccessStateUnLimited)
        {
            isCumulativeCheckNeeded = (_Bool)false;
        }
        if (isCumulativeCheckNeeded)
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
        {
            uint32_t actualLength = ALIGN_UP(length, alignmentBaseUnit);
            if (!mem_is_erased(address, actualLength))
            {
                status = (int32_t)kStatusMemoryCumulativeWrite;
            }
        }
#endif // BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE || FSL_FEATURE_SOC_FTFE_COUNT
    }
    return status;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
