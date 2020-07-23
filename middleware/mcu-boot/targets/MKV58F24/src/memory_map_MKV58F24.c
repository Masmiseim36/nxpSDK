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

//! @brief Memory map for KV58F22
//!
//! This map is not const because it is updated at runtime with the actual sizes of
//! flash and RAM for the chip we're running on.
memory_map_entry_t g_memoryMap[] = {
    { 0x10000000, 0x100fffff, kMemoryIsExecutable | kMemoryType_FLASH, &g_flashMemoryInterface },   // Flash array (1MB)
    { 0x00000000, 0x0000ffff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },  // ITCM SRAM (64KB)
    { 0x20000000, 0x2001ffff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },    // DTCM SRAM (128KB)
    { 0x2f000000, 0x2f00ffff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },   // OCRAM (64KB)
    { 0 }                                                                       // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
