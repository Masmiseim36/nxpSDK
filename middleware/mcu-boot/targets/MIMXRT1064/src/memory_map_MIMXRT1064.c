/*
* Copyright 2017-2018 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "bootloader/bl_context.h"
#include "memory/memory.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Memory map for RT105x.
//!
//! This map is not const because it is updated at runtime with the actual sizes of
//! flash and RAM for the chip we're running on.
//! @note Do not change the index of Flash, SRAM, or QSPI (see memory.h).
memory_map_entry_t g_memoryMap[] = {
    // ITCM SRAM(128KB)
    { 0x00000000, 0x0001ffff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },
    // DTCM SRAM(128KB)
    { 0x20000000, 0x2001ffff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },
    // OCRAM (256KB)
    { 0x20200000, 0x2023ffff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },
#if BL_FEATURE_FLEXSPI_NOR_MODULE
    // FlexSPI AMBA memory
    { 0x60000000, 0x6fffffff, kMemoryNotExecutable | kMemoryType_FLASH, &g_flexspiMemoryInterface },
#endif // #if BL_FEATURE_FLEXSPI_NOR_MODULE
#if BL_FEATURE_SEMC_NOR_MODULE
    // SEMC memory
    { 0x90000000, 0x1000000, kMemoryNotExecutable | kMemoryType_FLASH, &g_semcNorMemoryInterface },
#endif // #if BL_FEATURE_SEMC_NOR_MODULE
    // AIPS peripherals
    { 0x40000000, 0x403fffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface },
    // Terminator
    { 0 }
};

external_memory_map_entry_t g_externalMemoryMap[] = {
#if BL_FEATURE_SPINAND_MODULE
    { kMemorySpiNand, 0, 0x10000, 2048, &g_spiNandMemoryInterface }, // SPI NAND memory
#endif                                                               // BL_FEATURE_SPINAND_MODULE
#if BL_FEATURE_SPI_NOR_EEPROM_MODULE
    { kMemorySpiNorEeprom, 0, 0x10000, 256, &g_spiNorEepromMemoryInterface }, // Serial NOR/EEPROM memory
#endif                                                                        // BL_FEATURE_SPI_NOR_EEPROM_MODULE
#if BL_FEATURE_SD_MODULE
    { kMemorySDCard, 0, 0, 512, &g_sdMemoryInterface }, // SD card memory
#endif
#if BL_FEATURE_MMC_MODULE
    { kMemoryMMCCard, 0, 0, 512, &g_mmcMemoryInterface }, // MMC card memory
#endif
#if BL_FEATURE_SEMC_NAND_MODULE
    { kMemorySemcNand, 0, 0x10000, 2048, &g_semcNandMemoryInterface }, // SEMC NAND memory
#endif
    { 0 } // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
