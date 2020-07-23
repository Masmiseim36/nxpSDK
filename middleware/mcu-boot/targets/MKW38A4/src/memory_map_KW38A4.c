/*
 * Copyright (c) 2013, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "memory/memory.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Memory map for KV46F15
//!
//! This map is not const because it is updated at runtime with the actual sizes of
//! flash and RAM for the chip we're running on.
memory_map_entry_t g_memoryMap[] = {
    { 0x00000000u, 0x0003ffffu, (uint32_t)kMemoryIsExecutable | (uint32_t)kMemoryType_FLASH, &g_flashMemoryInterface },   // Flash array (256KB)
    { 0x1fffc000u, 0x2000bfffu, (uint32_t)kMemoryIsExecutable | (uint32_t)kMemoryType_RAM, &g_normalMemoryInterface },  // SRAM (64KB)
#if BL_FEATURE_SUPPORT_DFLASH    
    { 0x10000000u, 0x1003ffffu, (uint32_t)kMemoryIsExecutable | (uint32_t)kMemoryType_DFLASH, &g_dFlashMemoryInterface }, // DFlash array (256KB)
#if FSL_FEATURE_FLASH_HAS_FLEX_NVM_ALIAS    
    { 0x00040000u, 0x0007ffffu, (uint32_t)kMemoryIsExecutable | (uint32_t)kMemoryType_DFLASH | (uint32_t)kMemoryAliasAddr, &g_dFlashMemoryInterface }, // DFlash array (256KB)
#endif // FSL_FEATURE_FLASH_HAS_FLEX_NVM_ALIAS   
#endif     
    { 0x40000000u, 0x4007ffffu, (uint32_t)kMemoryNotExecutable | (uint32_t)kMemoryType_Device, &g_deviceMemoryInterface }, // AIPS peripherals
    { 0x400ff000u, 0x400fffffu, (uint32_t)kMemoryNotExecutable | (uint32_t)kMemoryType_Device, &g_deviceMemoryInterface }, // GPIO
    { 0xe0000000u, 0xe00fffffu, (uint32_t)kMemoryNotExecutable | (uint32_t)kMemoryType_Device, &g_deviceMemoryInterface }, // M0+ private peripherals
    { 0u }                                                                       // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
