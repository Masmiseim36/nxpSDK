/*
 * Copyright (c) 2013 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "memory/memory.h"
#include "qspi.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Memory map for K82F256.
//!
//! This map is not const because it is updated at runtime with the actual sizes of
//! flash and RAM for the chip we're running on.
//! @note Do not change the index of Flash, SRAM, or QSPI (see memory.h).
memory_map_entry_t g_memoryMap[] = {
    { 0x00000000, 0x0003ffff, kMemoryIsExecutable | kMemoryType_FLASH, &g_flashMemoryInterface },  // Flash array (256KB)
    { 0x1fff0000, 0x2002ffff, kMemoryIsExecutable | kMemoryType_RAM, &g_normalMemoryInterface }, // SRAM (256KB)
#if BL_FEATURE_QSPI_MODULE
    { 0x68000000, 0x6fffffff, kMemoryNotExecutable | kMemoryType_FLASH, &g_qspiMemoryInterface }, // QSPI memory
#endif
#if BL_FEATURE_QSPI_ALIAS_AREA
    { 0x04000000, 0x07ffffff, kMemoryNotExecutable | kMemoryType_FLASH, &g_qspiAliasAreaInterface }, // QSPI alias area
#endif                                                                           // BL_FEATURE_QSPI_ALIAS_AREA
    { 0x40000000, 0x4007ffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface },  // AIPS0 peripherals
    { 0x40080000, 0x400fefff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface },  // AIPS1 peripherals
    { 0x400ff000, 0x400fffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface },  // GPIO
    { 0xe0000000, 0xe00fffff, kMemoryNotExecutable | kMemoryType_Device, &g_deviceMemoryInterface },  // M4 private peripherals
    { 0 }                                                                        // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
