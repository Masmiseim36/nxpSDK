/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_flexspi.h"
#include "bootloader_common.h"
#include "flexspi_nor_flash.h"
#include "flexspi_nor_memory.h"
#include "fsl_assert.h"
#include "fsl_device_registers.h"
#include "fusemap.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
extern uint32_t flexspi_nor_get_amba_addr();
extern uint32_t flexspi_nor_get_alias_addr();

////////////////////////////////////////////////////////////////////////////////
// Codes
////////////////////////////////////////////////////////////////////////////////
//! @brief Update memory map of flexspi NOR Flash
status_t flexspi_nor_update_map(void)
{
    status_t status;
    uint32_t totalFlashSize;

    status = flexspi_nor_get_property(kFlexspiNorProperty_TotalFlashSize, &totalFlashSize);
    if (status != kStatus_Success)
    {
        return status;
    }

    g_memoryMap[kIndexFlexSpiNor].memoryProperty = (kMemoryIsExecutable | kMemoryType_FLASH);

    g_memoryMap[kIndexFlexSpiNor].startAddress = flexspi_nor_get_amba_addr();
    g_memoryMap[kIndexFlexSpiNor].endAddress = g_memoryMap[kIndexFlexSpiNor].startAddress + totalFlashSize - 1;
#if BL_FEATURE_FLEXSPI_ALIAS_AREA
    g_memoryMap[kIndexFlexSpiNorAlias].memoryProperty = kMemoryIsExecutable | kMemoryType_FLASH;
    g_memoryMap[kIndexFlexSpiNorAlias].endAddress =
        g_memoryMap[kIndexFlexSpiNorAlias].startAddress + MIN(totalFlashSize, kFlexSpiNorAliasAreaMaxSize) - 1;
#endif // #if BL_FEATURE_FLEXSPI_ALIAS_AREA
    return kStatus_Success;
}

#if BL_FEATURE_FLEXSPI_ALIAS_AREA
//! @brief Convert flexspi alias address to amba address.
uint32_t flexspi_get_map_address(uint32_t aliasAddr)
{
    // Currently for RT1170/RT1160 only
    return aliasAddr + (g_memoryMap[kIndexFlexSpiNor].startAddress - g_memoryMap[kIndexFlexSpiNorAlias].startAddress);
}
#endif

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
