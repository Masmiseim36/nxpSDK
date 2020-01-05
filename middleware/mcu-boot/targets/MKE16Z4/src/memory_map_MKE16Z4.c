/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "memory/memory.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Memory map for KE18Z256.
//!
//! This map is not const because it is updated at runtime with the actual sizes of
//! flash and RAM for the chip we're running on.
memory_map_entry_t g_memoryMap[] = {
    { 0x00000000, 0x0000ffff, kMemoryIsExecutable | kMemoryType_FLASH, &g_flashMemoryInterface },   // 64kB Flash array
    { 0x1ffff800, 0x200017FF, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },  // 8kB SRAM
    { 0 }                                                                       // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
