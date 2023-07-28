/*
 * Copyright 2018 NXP
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_context.h"
#include "bl_mpu.h"
#include "memory.h"
#include "memory_config.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
// Dummy interface, do nothing, this definition is defined here to satisfy the memory interface check
const memory_region_interface_t k_dummyInterface;
//! @brief Memory map for RT105x.
//!
//! This map is not const because it is updated at runtime with the actual sizes of
//! flash and RAM for the chip we're running on.
//! @note Do not change the index of Flash, SRAM, or QSPI (see memory.h).
memory_map_entry_t g_memoryMap[] = {
    // ITCM SRAM(512KB)
    [kIndexITCM] = {
      .startAddress = M7_ITCM_SRAM_START_ADDRESS,
      .endAddress = 0, /* Flexible size which will be filled during ROM initialization. */
      .memoryProperty = kMemoryIsExecutable | kMemoryType_RAM,
      .memoryId = kMemoryInternal,
      .memoryInterface = &g_normalMemoryInterface
    },

    // DTCM SRAM(512KB)
    [kIndexDTCM] = {
      .startAddress = M7_DTCM_SRAM_START_ADDRESS,
      .endAddress = 0, /* Flexible size which will be filled during ROM initialization. */
      .memoryProperty = kMemoryIsExecutable | kMemoryType_RAM,
      .memoryId = kMemoryInternal,
      .memoryInterface = &g_normalMemoryInterface
    },

    // OCRAM (2048KB)
    [kIndexOCRAM] = {
      .startAddress = OCRAM_START_ADDRESS,
      .endAddress = 0, /* Flexible size which will be filled during ROM initialization. */
      .memoryProperty = kMemoryIsExecutable | kMemoryType_RAM,
      .memoryId = kMemoryInternal,
      .memoryInterface = &g_normalMemoryInterface
    },

    // FlexSPI1 AMBA memory
    [kIndexFlexSpiNor] = {
      .startAddress = FLEXSPI1_AMBA_START_ADDRESS,
      .endAddress = 0, /* Flexible size which will be filled after FlexSPI1 initialization. */
      .memoryProperty = kMemoryNotExecutable | kMemoryType_FLASH,
      .memoryId = kMemoryFlexSpiNor,
      .memoryInterface = &g_flexspiMemoryInterface
    },

    // M4 FlexSPI1 Alias memory. Invalid for M7.
    [kIndexFlexSpiNorAlias] = {
      .startAddress = M4_FLEXSPI1_ALIAS_START_ADDRESS,
      .endAddress =
          0, /* Flexible size which will be filled after FlexSPI1 initialization. Only filled when M4 booting. */
      .memoryProperty = kMemoryNotExecutable | kMemoryType_FLASH,
      .memoryId = kMemoryFlexSpiNor,
      .memoryInterface = &g_flexspiAliasAreaInterface
    },

    // FlexSPI2 AMBA memory
    [kIndexFlexSpiNor2] = {
      .startAddress = FLEXSPI2_AMBA_START_ADDRESS,
      .endAddress = 0, /* Flexible size which will be filled after FlexSPI2 initialization. */
      .memoryProperty = kMemoryNotExecutable | kMemoryType_FLASH,
      .memoryId = kMemoryFlexSpiNor,
      .memoryInterface = &g_flexspiMemoryInterface
    },

    // Terminator
    { 0 }
};

#if BL_FEATURE_EXPAND_MEMORY
external_memory_map_entry_t g_externalMemoryMap[] = {
#if BL_FEATURE_SPINAND_MODULE
    // SPI NAND memory
    { .memoryId = kMemorySpiNand,
      .status = kStatus_Success,
      .basicUnitCount = 0x10000,
      .basicUnitSize = 2048,
      .memoryInterface = &g_spiNandMemoryInterface },
#endif // BL_FEATURE_SPINAND_MODULE
#if BL_FEATURE_SPI_NOR_EEPROM_MODULE
    // Serial NOR/EEPROM memory
    { .memoryId = kMemorySpiNorEeprom,
      .status = kStatus_Success,
      .basicUnitCount = 0x10000,
      .basicUnitSize = 256,
      .memoryInterface = &g_spiNorEepromMemoryInterface },
#endif // BL_FEATURE_SPI_NOR_EEPROM_MODULE
#if BL_FEATURE_SD_MODULE
    // SD card memory
    { .memoryId = kMemorySDCard,
      .status = kStatus_Success,
      .basicUnitCount = 0,
      .basicUnitSize = 512,
      .memoryInterface = &g_sdMemoryInterface },
#endif // BL_FEATURE_SD_MODULE
#if BL_FEATURE_MMC_MODULE
    // MMC card memory
    { .memoryId = kMemoryMMCCard,
      .status = kStatus_Success,
      .basicUnitCount = 0,
      .basicUnitSize = 512,
      .memoryInterface = &g_mmcMemoryInterface },
#endif // BL_FEATURE_MMC_MODULE
#if BL_FEATURE_SEMC_NAND_MODULE
    // SEMC NAND memory
    { .memoryId = kMemorySemcNand,
      .status = kStatus_Success,
      .basicUnitCount = 0x10000,
      .basicUnitSize = 2048,
      .memoryInterface = &g_semcNandMemoryInterface },
#endif    // BL_FEATURE_SEMC_NAND_MODULE
    { 0 } // Terminator
};
#endif // #if BL_FEATURE_EXPAND_MEMORY

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
