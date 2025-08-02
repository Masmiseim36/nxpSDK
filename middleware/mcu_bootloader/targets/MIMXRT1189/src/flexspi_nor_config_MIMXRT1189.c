/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "bootloader.h"
#include "flexspi_nor_flash.h"
#include "flexspi_nor_memory.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern uint32_t flexspi_nor_get_amba_addr();
extern uint32_t flexspi_nor_get_alias_addr();
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
//!@brief Write FlexSPI persistent content
status_t flexspi_nor_write_persistent(const uint32_t data)
{
    SRC_GENERAL_REG->GPR[2] = data;

    return kStatus_Success;
}
//!@brief Read FlexSPI persistent content
status_t flexspi_nor_read_persistent(uint32_t *data)
{
    *data = SRC_GENERAL_REG->GPR[2];

    return kStatus_Success;
}

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

    g_memoryMap[kIndexFLEXSPI].memoryProperty = (kMemoryIsExecutable | kMemoryType_FLASH);
    g_memoryMap[kIndexFLEXSPI].startAddress = flexspi_nor_get_amba_addr();
    g_memoryMap[kIndexFLEXSPI].endAddress = g_memoryMap[kIndexFLEXSPI].startAddress + totalFlashSize - 1;

    g_memoryMap[kIndexFLEXSPI_NS].memoryProperty = (kMemoryIsExecutable | kMemoryType_FLASH);
    g_memoryMap[kIndexFLEXSPI_NS].startAddress = address_to_nonsecure_address(g_memoryMap[kIndexFLEXSPI].startAddress);
    g_memoryMap[kIndexFLEXSPI_NS].endAddress = address_to_nonsecure_address(g_memoryMap[kIndexFLEXSPI].endAddress);

#if BL_FEATURE_FLEXSPI_ALIAS_AREA
    g_memoryMap[kIndexFLEXSPI_Alias].memoryProperty = kMemoryIsExecutable | kMemoryType_FLASH;
    g_memoryMap[kIndexFLEXSPI_Alias].startAddress = flexspi_nor_get_alias_addr();
    g_memoryMap[kIndexFLEXSPI_Alias].endAddress =
        g_memoryMap[kIndexFLEXSPI_Alias].startAddress + MIN(totalFlashSize, kFlexSpiNorAliasAreaMaxSize) - 1;

    g_memoryMap[kIndexFLEXSPI_Alias_NS].memoryProperty = (kMemoryIsExecutable | kMemoryType_FLASH);
    g_memoryMap[kIndexFLEXSPI_Alias_NS].startAddress =
        address_to_nonsecure_address(g_memoryMap[kIndexFLEXSPI_Alias].startAddress);
    g_memoryMap[kIndexFLEXSPI_Alias_NS].endAddress =
        address_to_nonsecure_address(g_memoryMap[kIndexFLEXSPI_Alias].endAddress);

#endif // #if BL_FEATURE_FLEXSPI_ALIAS_AREA
    return kStatus_Success;
}

#if BL_FEATURE_FLEXSPI_ALIAS_AREA
//! @brief Convert flexspi alias address to amba address.
uint32_t flexspi_get_map_address(uint32_t aliasAddr)
{
    // Currently for RT1170/RT1160 only
    return aliasAddr + (g_memoryMap[kIndexFLEXSPI].startAddress - g_memoryMap[kIndexFLEXSPI_Alias].startAddress);
}
#endif
