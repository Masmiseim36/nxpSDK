/*
 * Copyright 2016-2021 NXP
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_context.h"
#include "memory.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Memory map for RT105x.
//!
//! This map is not const because it is updated at runtime with the actual sizes of
//! flash and RAM for the chip we're running on.
//! @note Do not change the index of Flash, SRAM, or QSPI (see memory.h).
memory_map_entry_t g_memoryMap[] = {
    // ITCM SRAM(32KB)
    { 0x00000000, 0x00007fff, kMemoryIsExecutable | kMemoryType_RAM, kMemoryInternal, &g_normalMemoryInterface },
    // DTCM SRAM(32KB)
    { 0x20000000, 0x20007fff, kMemoryIsExecutable | kMemoryType_RAM, kMemoryInternal, &g_normalMemoryInterface },
    // OCRAM (64KB)
    { 0x20200000, 0x2020ffff, kMemoryIsExecutable | kMemoryType_RAM, kMemoryInternal, &g_normalMemoryInterface },
#if BL_FEATURE_FLEXSPI_NOR_MODULE
    // FlexSPI1 AMBA memory
    { 0x60000000, 0x7f7fffff, kMemoryNotExecutable | kMemoryType_FLASH, kMemoryFlexSpiNor, &g_flexspiMemoryInterface },
#endif // #if BL_FEATURE_FLEXSPI_NOR_MODULE
    // Terminator
    { 0 }
};

external_memory_map_entry_t g_externalMemoryMap[] = {
    { 0 } // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
