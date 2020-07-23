/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "memory/memory.h"
#include "flexNVM_memory.h"
#include "normal_memory.h"
#include "fsl_flash.h"
#include "bootloader/bl_context.h"
#include "fsl_device_registers.h"
#include "utilities/fsl_rtos_abstraction.h"
#include "utilities/fsl_assert.h"
#include <string.h>

#if defined(BL_FEATURE_SUPPORT_DFLASH) && BL_FEATURE_SUPPORT_DFLASH

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
enum _flashNVM_memory_constants
{
    kFlashMemory_ErasedValue = ~0
};

#if defined(BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED) && BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED
//! @brief FLASH section program memory context
//!
//! An instance is maintained in this file, will is used to keep key information for write and flush
//! operatations.
typedef struct _flashNVM_section_program_info
{
    uint32_t startAddress;                      //!< This address is used to record the address which is used
                                                //!< to write the whole section into flash memory
    uint32_t storedBytes;                       //!< A variable which is used to indicate if the buffer is full.
    uint8_t buffer[0x400U]; //!< A buffer which is used to buffer a full section of data
    //uint8_t buffer[kFLASH_AccelerationRamSize]; //!< A buffer which is used to buffer a full section of data
} flashNVM_section_program_info_t;

static flashNVM_section_program_info_t s_dflash_section_program_info;
#endif

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief flash memory array.
#if defined(BL_FEATURE_SUPPORT_DFLASH) && BL_FEATURE_SUPPORT_DFLASH
flexnvm_config_t g_dflashState;
#endif 

//ftfx_cache_config_t g_flashcacheState;

//! @brief Interface to dflash memory operations.
const memory_region_interface_t g_dFlashMemoryInterface = {
    .init = &flexNVM_mem_init,
    .read = &flexNVM_mem_read,
    .write = &flexNVM_mem_write,
#if !(defined(BL_FEATURE_MIN_PROFILE) && BL_FEATURE_MIN_PROFILE) || \
    defined(BL_FEATURE_FILL_MEMORY) && BL_FEATURE_FILL_MEMORY
    .fill = &flexNVM_mem_fill,
#endif // !BL_FEATURE_MIN_PROFILE
#if defined(BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED) && BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED
    .flush = flexNVM_mem_flush,
#else
    .flush = (void *)0u,
#endif
    .erase = flexNVM_mem_erase,
};

#if defined(BL_TARGET_FLASH) && BL_TARGET_FLASH
static uint32_t s_regPrimask = 0U;
#endif

#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
//! @brief It is used for indicating if an XA controlled region is unlocked to program state
bool isDFlashRegionUnlocked = false;
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
status_t flexNVM_check_access_before_programming(uint32_t address, uint32_t length, bool *verifyWrites);

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
status_t flexNVM_mem_init(void)
{
    if (g_bootloaderContext.dflashDriverInterface != (void *)0u)
    {
        // Update address range of flash
        memory_map_entry_t *map;
        uint32_t tmp;

        union
        {
            memory_map_entry_t const *address;
            memory_map_entry_t *pMap;
        } map_ptr;

        map_ptr.address = &g_bootloaderContext.memoryMap[(uint32_t)kIndexDFlashArray];

        map = map_ptr.pMap;
            
        (void)g_bootloaderContext.dflashDriverInterface->flash_get_property(g_bootloaderContext.dFlashState,
                                                                      kFLEXNVM_PropertyDflashBlockBaseAddr,
                                                                      &map->startAddress);     

        (void)g_bootloaderContext.dflashDriverInterface->flash_get_property(g_bootloaderContext.dFlashState,
                                                                      kFLEXNVM_PropertyDflashTotalSize,
                                                                      &tmp); 
            
        map->endAddress = map->startAddress + tmp - 1u;

#if defined(BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED) && BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED
        s_dflash_section_program_info.storedBytes = 0u;
#endif
    }

    return (int32_t)kStatus_Success;
}

// See flash_memory.h for documentation on this function.
status_t flexNVM_mem_read(uint32_t address, uint32_t length, uint8_t *buffer)
{
    status_t status = (int32_t)kStatus_Success;

    if (g_bootloaderContext.dflashDriverInterface == (void *)0u)
    {
        status = (int32_t)kStatusMemoryRangeInvalid;
    }
    else
    {
#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
        uint32_t alignedAddress = ALIGN_DOWN(address, g_bootloaderContext.dFlashState->ftfxConfig.flashDesc.accessSegmentMem.size);
        uint32_t updatedLength = address - alignedAddress + length;
        uint32_t alignedLength = ALIGN_UP(updatedLength, g_bootloaderContext.dFlashState->ftfxConfig.flashDesc.accessSegmentMem.size);
    
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL

        status = normal_mem_read(address, length, buffer);
    }

    return status;
}

#if !(defined(BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED) && BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED)
// See flash_memory.h for documentation on this function.
status_t flexNVM_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{  
    status_t status = (int32_t)kStatus_Success;

    // Note: the check for "length != 0" and "range not in reserved region" is done in mem_write().
    assert(length);
    assert(buffer);

    if (g_bootloaderContext.dflashDriverInterface == (void *)0u)
    {
        status = (int32_t)kStatusMemoryRangeInvalid;
    }    
    else
    {
        uint32_t alignedLength;
        uint32_t extraBytes;
        uint32_t extraData[2];
        uint32_t alignmentBaseUnit = g_bootloaderContext.dFlashState->ftfxConfig.opsConfig.addrAligment.blockWriteUnitSize;

        assert(sizeof(extraData) >= sizeof(uint8_t) * alignmentBaseUnit);

        bool verifyWrites;
        status = flexNVM_check_access_before_programming(address, length, &verifyWrites);
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
                status = g_bootloaderContext.dflashDriverInterface->flash_program(g_bootloaderContext.dFlashState, address,
                                                                                 (uint8_t *)buffer, alignedLength);
            }
            if ((status == (int32_t)kStatus_Success) && extraBytes != 0u)
            {
                // Program trailing word.
                status = g_bootloaderContext.dflashDriverInterface->flash_program(
                    g_bootloaderContext.dFlashState, address + alignedLength, (uint8_t *)extraData,
                    alignmentBaseUnit);
            }
            (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, false);
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
                        status = g_bootloaderContext.dflashDriverInterface->flash_verify_program(
                            g_bootloaderContext.dFlashState, address, alignedLength, (uint8_t *)buffer,
                            (ftfx_margin_value_t)g_bootloaderContext.propertyInterface->store->flashReadMargin, &failedAddress,
                            &failedData);
                    }
                    if ((status == (int32_t)kStatus_Success) && extraBytes != 0u)
                    {
                        status = g_bootloaderContext.flashDriverInterface->flash_verify_program(
                            g_bootloaderContext.dFlashState, address + alignedLength, sizeof(extraData), (uint8_t *)extraData,
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
    }

    return status;
}

// See flash_memory.h for documentation on this function.
status_t flexNVM_mem_fill(uint32_t address, uint32_t length, uint32_t pattern)
{ 
    status_t status = (int32_t)kStatus_Success;
    // Note: the check for "length != 0"
    assert(length);
    if (g_bootloaderContext.dflashDriverInterface == (void *)0u)
    {
        status = (int32_t)kStatusMemoryRangeInvalid;
    }
    else
    {
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

            status = flexNVM_mem_write(address, bytes, (uint8_t *)patternBuffer);
            if (status != (int32_t)kStatus_Success)
            {
                break;
            }

            address += bytes;
            length -= bytes;
        }
    }

    return status;
}

#else // BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED

// See flash_memory.h for documentation on this function.
status_t flexNVM_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    status_t status = (int32_t)kStatus_Success;
    // Note: the check for "length != 0" and "range not in reserved region" is done in mem_write().
    assert(length);
    assert(buffer);
    if (g_bootloaderContext.dflashDriverInterface == (void *)0u)
    {
        status = (int32_t)kStatusMemoryRangeInvalid;
    }
    else
    {
        uint32_t alignmentBaseUnit = g_bootloaderContext.dFlashState->ftfxConfig.opsConfig.addrAligment.blockWriteUnitSize;

        while (length != 0u)
        {
            // Set start address when storing first byte into section program buffer
            if ((s_dflash_section_program_info.storedBytes == 0u) &&
                (s_dflash_section_program_info.startAddress == 0u))
            {
                // Check address alignment
                if ((address & (alignmentBaseUnit - 1u)) != 0u)
                {
                    status = (int32_t)kStatus_FTFx_AlignmentError;
                    break;
                }
                s_dflash_section_program_info.startAddress = address;
            }
            else
            {
                // Start section programming operation when meet discontinuous address
                if ((s_dflash_section_program_info.startAddress + s_dflash_section_program_info.storedBytes) != address)
                {
                    // flush cached data into target memory,
                    status = flexNVM_mem_flush();
                    if (status != (int32_t)kStatus_Success)
                    {
                        break;
                    }
                    continue;
                }
            }

            uint32_t storeBytes = 0u;
            // Check to see if section program buffer will be filled with current data packet
            if ((s_dflash_section_program_info.storedBytes + length) <= sizeof(s_dflash_section_program_info.buffer))
            {
                storeBytes = length;
            }
            else
            {
                storeBytes = sizeof(s_dflash_section_program_info.buffer) - s_dflash_section_program_info.storedBytes;
            }

            // Copy data to section program buffer
            if (buffer != &s_dflash_section_program_info.buffer[s_dflash_section_program_info.storedBytes] && 
                storeBytes > 0u)
            {
                (void)memcpy(&s_dflash_section_program_info.buffer[s_dflash_section_program_info.storedBytes], buffer, storeBytes);
            }

            s_dflash_section_program_info.storedBytes += storeBytes;
            buffer += storeBytes;
            address += storeBytes;
            length -= storeBytes;

            // Start section programming operation when section program buffer is full
            if (s_dflash_section_program_info.storedBytes == sizeof(s_dflash_section_program_info.buffer))
            {
                // flush cached data into target memory,
                status = flexNVM_mem_flush();
                if (status != (int32_t)kStatus_Success)
                {
                    break;
                }
            }
        }
    }
    return status;
}

// See flash_memory.h for documentation on this function.
status_t flexNVM_mem_fill(uint32_t address, uint32_t length, uint32_t pattern)
{
    // Note: the check for "length != 0" and "range not in reserved region" is done in mem_fill().
    assert(length);
    status_t status = (int32_t)kStatus_Success;
    if (g_bootloaderContext.dflashDriverInterface == (void *)0u)
    {
        status = (int32_t)kStatusMemoryRangeInvalid;
    }
    else
    {
        // Pre-fill section program buffer with pattern
        union
        {
            uint8_t *address;
            uint32_t *pAddress;
        } buffer_ptr;

        buffer_ptr.address = s_dflash_section_program_info.buffer;

        uint32_t *buffer = buffer_ptr.pAddress;
        uint32_t maxPatterns = sizeof(s_dflash_section_program_info.buffer) >> 2u;
        for (uint32_t i = 0u; i < maxPatterns; i++)
        {
            *buffer++ = pattern;
        }

        while (length != 0u)
        {
            uint32_t bytes;

            s_dflash_section_program_info.storedBytes = 0u;

            // Check to see if remaining address range can hold whole section program buffer
            if (length < sizeof(s_dflash_section_program_info.buffer))
            {
                bytes = length;
            }
            else
            {
                bytes = sizeof(s_dflash_section_program_info.buffer);
            }

            // flush cached data into target memory,
            status = flexNVM_mem_write(address, bytes, s_dflash_section_program_info.buffer);
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
            status = flexNVM_mem_flush();
        }
    }
    return status;
}

// See memory.h for documentation on this function.
status_t flexNVM_mem_flush(void)
{ 
    status_t status = (int32_t)kStatus_Success;
    if (g_bootloaderContext.dflashDriverInterface == (void *)0u)
    {
        status = (int32_t)kStatusMemoryRangeInvalid;
    }
    else
    {
        if (s_dflash_section_program_info.storedBytes != 0u)
        {
            uint32_t address = s_dflash_section_program_info.startAddress;
            uint32_t length = s_dflash_section_program_info.storedBytes;
            uint32_t alignmentBaseUnit = g_bootloaderContext.dFlashState->ftfxConfig.opsConfig.addrAligment.sectionCmd;

            // Clear related states no matter following operations are executed successfully or not.
            s_dflash_section_program_info.startAddress = 0u;
            s_dflash_section_program_info.storedBytes = 0u;

            // Align length to section program unit
            uint32_t alignedLength = ALIGN_UP(length, alignmentBaseUnit);

            bool verifyWrites = (_Bool)false;
            status = flexNVM_check_access_before_programming(address, length, &verifyWrites);
            if (status == (int32_t)kStatus_Success)
            {
                // Fill unused region with oxFFs.
                assert(alignedLength <= sizeof(s_dflash_section_program_info.buffer));
                assert(length <= sizeof(s_dflash_section_program_info.buffer));
                if (length < alignedLength)
                {
                    (void)memset(&s_dflash_section_program_info.buffer[length], 0xFF, alignedLength - length);
                }

                flash_lock_acquire();
                (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, true);
                // Write data of aligned length to flash
                status = FLEXNVM_DflashProgramSection(g_bootloaderContext.dFlashState, address,
                                              (uint8_t *)s_dflash_section_program_info.buffer, alignedLength);
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
                        status = g_bootloaderContext.dflashDriverInterface->flash_verify_program(
                            g_bootloaderContext.dFlashState, address, alignedLength,
                            (uint8_t *)&s_dflash_section_program_info.buffer,
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
    }
    return status;
}
#endif // !BL_IS_FLASH_SECTION_PROGRAMMING_ENABLED

// See memory.h for documentation on this function.
status_t flexNVM_mem_erase(uint32_t address, uint32_t length)
{  
    status_t status = (int32_t)kStatus_Success;
    if (g_bootloaderContext.dflashDriverInterface == (void *)0u)
    {
        status = (int32_t)kStatusMemoryRangeInvalid;
    }
    else
    {
        flash_lock_acquire();
        (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, true);
        status = g_bootloaderContext.dflashDriverInterface->flash_erase(g_bootloaderContext.dFlashState, address, length,
                                                                       kFLASH_ApiEraseKey);
        (void)FTFx_CACHE_ClearCachePrefetchSpeculation(g_bootloaderContext.allFlashCacheState, false);
        flash_lock_release();

#if !(defined(BL_FEATURE_FLASH_VERIFY_DISABLE) && BL_FEATURE_FLASH_VERIFY_DISABLE)
        if ((status == (int32_t)kStatus_Success) && (g_bootloaderContext.propertyInterface->store->verifyWrites != 0u))
        {
            flash_lock_acquire();
            status = g_bootloaderContext.dflashDriverInterface->flash_verify_erase(
                g_bootloaderContext.dFlashState, address, length,
                (ftfx_margin_value_t)g_bootloaderContext.propertyInterface->store->flashReadMargin);
            flash_lock_release();
            if (status != (int32_t)kStatus_Success)
            {
                debug_printf("Error: flash_verify_erase failed\r\n");
            }
        }
#endif // !BL_FEATURE_FLASH_VERIFY_DISABLE
    }
    return status;
}

// See memory.h for documentation on this function.
status_t flexNVM_mem_erase_all(void)
{
    status_t status = (int32_t)kStatus_Success;
    if (g_bootloaderContext.dflashDriverInterface != (void *)0u)
    {
        union
        {
            memory_map_entry_t const *address;
            memory_map_entry_t *pMap;
        } map_ptr;

        // Erase FlexNVM flash
        map_ptr.address = &g_bootloaderContext.memoryMap[(uint32_t)kIndexDFlashArray];
        memory_map_entry_t *map_dflash = map_ptr.pMap;
        status = flexNVM_mem_erase(map_dflash->startAddress, (map_dflash->endAddress + 1u) - map_dflash->startAddress);
    }
    return status;
}

// See memory.h for documentation on this function.
status_t flexNVM_mem_erase_all_unsecure(void)
{
    return flexNVM_mem_erase_all();
}

status_t flexNVM_check_access_before_programming(uint32_t address, uint32_t length, bool *verifyWrites)
{
    status_t status = (int32_t)kStatus_Success;
    uint32_t alignmentBaseUnit = g_bootloaderContext.dFlashState->ftfxConfig.opsConfig.addrAligment.blockWriteUnitSize;

#if !(defined(BL_FEATURE_FLASH_VERIFY_DISABLE) && BL_FEATURE_FLASH_VERIFY_DISABLE)
    *verifyWrites = (g_bootloaderContext.propertyInterface->store->verifyWrites != 0u)? true : (_Bool)false;
#endif // BL_FEATURE_FLASH_VERIFY_DISABLE

// Do cumulative write check
#if defined(BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE) && BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE || \
    defined(FSL_FEATURE_SOC_FTFE_COUNT) && FSL_FEATURE_SOC_FTFE_COUNT

    uint32_t actualLength = ALIGN_UP(length, alignmentBaseUnit);
    if (!mem_is_erased(address, actualLength))
    {
        status = (int32_t)kStatusMemoryCumulativeWrite;
    }
#endif // BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE || FSL_FEATURE_SOC_FTFE_COUNT

    return status;
}

#endif // BL_FEATURE_SUPPORT_DFLASH
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
