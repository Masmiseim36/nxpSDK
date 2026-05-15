/*
 * Copyright 2018 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader.h"
#include "memory.h"
#include "memory_config.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
//! @brief Memory map
//!
//! This map is not const because it is updated at runtime with the actual sizes of
//! flash and RAM for the chip we're running on.
//! @note Do not change the index of Flash, SRAM, or QSPI (see memory.h).
memory_map_entry_t g_memoryMap[] = {
    // M33 CODE TCM SRAM(256KB)
    { .startAddress = M33_CODE_TCM_START_ADDRESS_IN_M33,
      .endAddress = M33_CODE_TCM_END_ADDRESS_IN_M33,
      .memoryProperty = kMemoryIsExecutable | kMemoryType_RAM,
      .memoryId = kMemoryInternal,
      .memoryInterface = &g_normalMemoryInterface },

    // M33 CODE TCM NS SRAM(256KB)
    { .startAddress = M33_CODE_TCM_NS_START_ADDRESS_IN_M33,
      .endAddress = M33_CODE_TCM_NS_END_ADDRESS_IN_M33,
      .memoryProperty = kMemoryIsExecutable | kMemoryType_RAM,
      .memoryId = kMemoryInternal,
      .memoryInterface = &g_normalMemoryInterface },

    // M33 SYS TCM SRAM(256KB)
    { .startAddress = M33_SYS_TCM_START_ADDRESS_IN_M33,
      .endAddress = M33_SYS_TCM_END_ADDRESS_IN_M33,
      .memoryProperty = kMemoryIsExecutable | kMemoryType_RAM,
      .memoryId = kMemoryInternal,
      .memoryInterface = &g_normalMemoryInterface },

    // M33 SYS TCM NS SRAM(256KB)
    { .startAddress = M33_SYS_TCM_NS_START_ADDRESS_IN_M33,
      .endAddress = M33_SYS_TCM_NS_END_ADDRESS_IN_M33,
      .memoryProperty = kMemoryIsExecutable | kMemoryType_RAM,
      .memoryId = kMemoryInternal,
      .memoryInterface = &g_normalMemoryInterface },

    // M7 TCM SRAM
    { .startAddress = M7_TCM_START_ADDRESS_IN_M33,
      .endAddress = M7_TCM_END_ADDRESS_IN_M33, /* Flexible size which will be filled during ROM initialization. */
      .memoryProperty = kMemoryIsExecutable | kMemoryType_RAM,
      .memoryId = kMemoryInternal,
      .memoryInterface = &g_normalMemoryInterface },

    // M7 TCM NS SRAM
    { .startAddress = M7_TCM_NS_START_ADDRESS_IN_M33,
      .endAddress = M7_TCM_NS_END_ADDRESS_IN_M33, /* Flexible size which will be filled during ROM initialization. */
      .memoryProperty = kMemoryIsExecutable | kMemoryType_RAM,
      .memoryId = kMemoryInternal,
      .memoryInterface = &g_normalMemoryInterface },

    // OCRAM SRAM
    { .startAddress = OCRAM_START_ADDRESS,
      .endAddress = OCRAM_END_ADDRESS, /* Flexible size which will be filled during ROM initialization. */
      .memoryProperty = kMemoryIsExecutable | kMemoryType_RAM,
      .memoryId = kMemoryInternal,
      .memoryInterface = &g_normalMemoryInterface },

    // OCRAM NS SRAM
    { .startAddress = OCRAM_NS_START_ADDRESS,
      .endAddress = OCRAM_NS_END_ADDRESS, /* Flexible size which will be filled during ROM initialization. */
      .memoryProperty = kMemoryIsExecutable | kMemoryType_RAM,
      .memoryId = kMemoryInternal,
      .memoryInterface = &g_normalMemoryInterface },

    // FlexSPI1
    { .startAddress = FLEXSPI1_START_ADDRESS,
      /* Flexible size which will be filled after FlexSPI1 initialization. */
      .endAddress = FLEXSPI1_END_ADDRESS,
      .memoryProperty = kMemoryNotExecutable,
      .memoryId = kMemoryFlexSpiNor,
      .memoryInterface = &g_flexspiMemoryInterface },

    // FlexSPI1 NS
    { .startAddress = FLEXSPI1_NS_START_ADDRESS,
      /* Flexible size which will be filled after FlexSPI1 initialization. */
      .endAddress = FLEXSPI1_NS_END_ADDRESS,
      .memoryProperty = kMemoryNotExecutable,
      .memoryId = kMemoryFlexSpiNor,
      .memoryInterface = &g_flexspiMemoryInterface },

    // FlexSPI1 Alias
    { .startAddress = FLEXSPI1_ALIAS_START_ADDRESS,
      /* Flexible size which will be filled after FlexSPI1 initialization. */
      .endAddress = FLEXSPI1_ALIAS_END_ADDRESS,
      .memoryProperty = kMemoryNotExecutable,
      .memoryId = kMemoryFlexSpiNor,
      .memoryInterface = &g_flexspiAliasAreaInterface },

    // FlexSPI1 Alias NS
    { .startAddress = FLEXSPI1_NS_ALIAS_START_ADDRESS,
      /* Flexible size which will be filled after FlexSPI1 initialization. */
      .endAddress = FLEXSPI1_NS_ALIAS_END_ADDRESS,
      .memoryProperty = kMemoryNotExecutable,
      .memoryId = kMemoryFlexSpiNor,
      .memoryInterface = &g_flexspiAliasAreaInterface },

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
