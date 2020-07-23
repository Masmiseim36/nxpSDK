/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_context.h"
#include "memory.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Memory map for KV11Z7
//!
//! This map is not const because it is updated at runtime with the actual sizes of
//! flash and RAM for the chip we're running on.
memory_map_entry_t g_memoryMap[] = {
    { 0x00000000, 0x0007ffff, kMemoryIsExecutable | kMemoryType_FLASH, &g_flashMemoryInterface },    // Flash array (512KB)
    { 0x1fffc000, 0x2000bfff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },     // SRAM (64KB)
    { 0 }                                                                       // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
