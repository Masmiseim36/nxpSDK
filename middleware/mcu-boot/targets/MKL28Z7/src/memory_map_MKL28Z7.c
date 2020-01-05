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

//! @brief Memory map for KL28T512.
//!
//! This map is not const because it is updated at runtime with the actual sizes of
//! flash and RAM for the chip we're running on.
memory_map_entry_t g_memoryMap[] = {
    { 0x00000000, 0x0007ffff, kMemoryIsExecutable | kMemoryType_FLASH, &g_flashMemoryInterface },   // 512kB Flash array
    { 0x1fff8000, 0x20017fff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface },  // 128kB SRAM
    { 0x40000000, 0x4007ffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface }, // AIPS0 peripherals
    { 0x40080000, 0x400fffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface }, // AIPS1 peripherals
    { 0x44000000, 0x5fffffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface }, // BME
    { 0xf0000000, 0xf0006fff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface }, // M0+ Private Peripheral Bus 0 (CPU0)
    { 0xf8000000, 0xffffffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface }, // IOPORT (single-cycle GPIO)
    { 0 }                                                                       // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
