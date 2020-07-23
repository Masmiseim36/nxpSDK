/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "memory/memory.h"
#include "fsl_device_registers.h"
#include "utilities/fsl_assert.h"
#include "sram_init.h"

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

//! @brief Initialize address ranges of SRAM for chips belongs to cm0plus family
status_t sram_init(void)
{
#if defined(__CORE_CM0PLUS_H_GENERIC)

    uint32_t ram_size = 0u;

#if defined(FSL_FEATURE_SIM_OPT_HAS_RAMSIZE) && FSL_FEATURE_SIM_OPT_HAS_RAMSIZE
    uint32_t tmp = (SIM->SOPT1 & SIM_SOPT1_RAMSIZE_MASK) >> SIM_SOPT1_RAMSIZE_SHIFT;
    switch (tmp)
    {
        case 1u:
            ram_size = 8u * 1024u;
            break;
        case 3u:
            ram_size = 16u * 1024u;
            break;
        case 4u:
            ram_size = 24u * 1024u;
            break;
        case 5u:
            ram_size = 32u * 1024u;
            break;
        case 6u:
            ram_size = 48u * 1024u;
            break;
        case 7u:
            ram_size = 64u * 1024u;
            break;
        case 8u:
            ram_size = 96u * 1024u;
            break;
        case 9u:
            ram_size = 128u * 1024u;
            break;
        case 11u:
            ram_size = 256u * 1024u;
            break;
        default:
            ram_size = 0u;
            break;
    }
#else
    uint32_t tmp = (uint32_t)((SIM->SDID & (uint32_t)SIM_SDID_SRAMSIZE_MASK) >> (uint32_t)SIM_SDID_SRAMSIZE_SHIFT);

    // for KW41Z4, 1001 - 128KB; 0111 - 64 KB
    if (tmp <= (uint32_t)kMaxRamIndex)
    {
        ram_size = (uint32_t)kMinKlRamSize << tmp;
    }
    else
    {
        ram_size = (uint32_t)kMinKlRamSize << (tmp - 1u);              
    }
#endif

    assert(ram_size > 0u);

    if (ram_size > 0u)
    {
        union
        {
            memory_map_entry_t const *address;
            memory_map_entry_t *pMap;
        } map_ptr;
        map_ptr.address = &g_bootloaderContext.memoryMap[(uint32_t)kIndexSRAM];
        // Update address range of SRAM
        memory_map_entry_t *map = map_ptr.pMap;
        tmp = ram_size / ((uint32_t)kSram_LowerPart + (uint32_t)kSram_UpperPart);
        map->startAddress = (uint32_t)kSRAMSeparatrix - tmp * (uint32_t)kSram_LowerPart;   // start of SRAM
        map->endAddress = (uint32_t)kSRAMSeparatrix + tmp * (uint32_t)kSram_UpperPart - 1u; // end of SRAM
    }
#else
#error "This function only applies to cm0plus family"
#endif // __CORE_CM0PLUS_H_GENERIC

    return (int32_t)kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
