/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "memory.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Memory map for MIMXRT685S.
//!
//! This map is not const because it is updated at runtime with the actual sizes of
//! flash and RAM for the chip we're running on.
//! @note See memory.h for index requirements.
memory_map_entry_t g_memoryMap[] = {
    // SRAM0 (size: 4.5MB)
    { 0x00000000u, 0x00477fffu, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },
    // System RAM (size: 4.5MB)
    { 0x20000000u, 0x20477fffu, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },
    { 0x30000000u, 0x30477fffu, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },
    // QSPI memory (Size will be updated later during interface initialization)
    { 0x08000000u, 0x0fffffffu, kMemoryIsExecutable | kMemoryType_FLASH | kMemorySkipInitError,
      &g_flexspiMemoryInterface },

    { 0 } // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
