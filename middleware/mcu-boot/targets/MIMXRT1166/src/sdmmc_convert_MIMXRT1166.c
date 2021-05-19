/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "memory_config.h"
#include "sdmmc_init.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/
typedef struct _usdhc_memory_convert
{
    uint32_t startAddress; /*!< Start address for a memory region.(closed interval) */
    uint32_t endAddress;   /*!< End address for a memory region.(open interval, better for shift operation) */
                           /*
                            * Note: if offset + address is overflow, that means real address < origin address.
                            */
    uint32_t offset;       /*!< Offset of the real address accoring the memory region. */
} usdhc_memory_conver_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern bool is_cm4_boot(void);

/*******************************************************************************
 * Variable
 ******************************************************************************/
const usdhc_memory_conver_t s_memoryRegionConvertTable[] = {
    { M4_TCM_SRAM_START_ADDRESS, M4_TCM_SRAM_END_ADDRESS + 1 /*open interval*/,
      M4_OCRAM_START_ADDRESS - M4_TCM_SRAM_START_ADDRESS }, /* ITCM + DTCM */
                                                            /*
                                                             * Note: put rom at the 2nd, considering the address isn't at rom for most cases.
                                                             */
    { M4_ROM_ALIAS_START_ADDRESS, M4_ROM_ALIAS_END_ADDRES + 1 /*open interval*/,
      ROM_START_ADDRESS - M4_ROM_ALIAS_START_ADDRESS }, /* ROM */
};

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t USDHC_MemorySpaceConvert(uint32_t *realAddress, uint32_t origAddress, uint32_t size)
{
    uint32_t address = origAddress;

    if (is_cm4_boot())
    {
        for (int i = 0; i < ARRAY_SIZE(s_memoryRegionConvertTable); i++)
        {
            if ((address >= s_memoryRegionConvertTable[i].startAddress) &&
                ((address + size) <= s_memoryRegionConvertTable[i].endAddress))
            {
                address += s_memoryRegionConvertTable[i].offset;
                break;
            }
            /*
             * Note: For i.MXRT1170, M7 ITCM and M4 ROMCP alia, M7 DTCM and M4 DTCM are overlapped. If the operation is
             * crossing memory boundaries, return error.
             */
            else if ((address < s_memoryRegionConvertTable[i].endAddress) &&
                     ((address + size) > s_memoryRegionConvertTable[i].endAddress))
            {
                return kStatus_USDHC_PrepareAdmaDescriptorFailed;
            }
            else if ((address < s_memoryRegionConvertTable[i].startAddress) &&
                     ((address + size) > s_memoryRegionConvertTable[i].startAddress))
            {
                return kStatus_USDHC_PrepareAdmaDescriptorFailed;
            }
        }
    }

    *realAddress = address;

    return kStatus_Success;
}
