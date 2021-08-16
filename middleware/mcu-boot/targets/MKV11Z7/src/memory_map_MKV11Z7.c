/*
 * Copyright (c) 2013 Freescale Semiconductor, Inc.
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
    { 0x00000000, 0x0001ffff, kMemoryIsExecutable | kMemoryType_FLASH, kMemoryInternal, &g_flashMemoryInterface },   // Flash array (128KB)
    { 0x1ffff000, 0x20002fff, kMemoryIsExecutable | kMemoryType_RAM, kMemoryInternal, &g_normalMemoryInterface },  // SRAM (16KB)
    { 0 }                                                                       // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
