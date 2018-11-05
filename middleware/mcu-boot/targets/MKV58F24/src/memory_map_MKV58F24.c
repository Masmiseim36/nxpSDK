/*
 * Copyright (c) 2013 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "memory/memory.h"

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
    { 0x40000000, 0x4007ffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface }, // AIPS peripherals
    { 0x400ff000, 0x400fffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface }, // GPIO
    { 0xe0000000, 0xe00fffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface }, // M7 private peripherals
    { 0 }                                                                       // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
