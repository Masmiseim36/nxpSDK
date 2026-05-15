/*
 * Copyright 2022 NXP
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

/*******************************************************************************
 * Variable
 ******************************************************************************/
const usdhc_memory_conver_t s_memoryRegionConvertTable[] = {
    { M33_CODE_TCM_NS_START_ADDRESS_IN_M33, M33_CODE_TCM_NS_END_ADDRESS_IN_M33 + 1 /* Open interval. */,
      M33_CODE_TCM_START_ADDRESS_IN_OTHER - M33_CODE_TCM_NS_START_ADDRESS_IN_M33 }, /* M33 CODE TCM NS */
    { M33_SYS_TCM_NS_START_ADDRESS_IN_M33, M33_SYS_TCM_NS_END_ADDRESS_IN_M33 + 1 /* Open interval. */,
      M33_SYS_TCM_START_ADDRESS_IN_OTHER - M33_SYS_TCM_NS_START_ADDRESS_IN_M33 }, /* M33 SYS TCM NS */
    /*
     * Put SEMC at the 2nd for SDRAM usage.
     */
    { SEMC0_NS_CODE_START_ADDRESS, SEMC0_NS_CODE_END_ADDRESS + 1 /* Open intrval. */,
      SEMC0_NS_START_ADDRESS - SEMC0_NS_CODE_START_ADDRESS }, /* SEMC0 CODE ALIAS NS */
    /*
     * Put FlexSPI at the 3rd for FlexSPI RAM usage. Considering FlexSPI is used as FLASH in most cases, put it after
     * SEMC.
     */
    { FLEXSPI1_NS_ALIAS_START_ADDRESS, FLEXSPI1_NS_ALIAS_END_ADDRESS + 1 /* Open interval. */,
      FLEXSPI1_NS_START_ADDRESS - FLEXSPI1_NS_ALIAS_START_ADDRESS }, /* FlexSPI1 Alias NS */
    { FLEXSPI2_NS_ALIAS_START_ADDRESS, FLEXSPI2_NS_ALIAS_END_ADDRESS + 1 /* Open interval. */,
      FLEXSPI2_NS_START_ADDRESS - FLEXSPI2_NS_ALIAS_START_ADDRESS }, /* FlexSPI2 Alias NS */
    /*
     * Note: put ROM regions at the end, considering the address isn't at ROM regions for most cases.
     */
    { ROM_NS_START_ADDRESS_IN_M33, ROM_NS_END_ADDRES_IN_M33 + 1 /* Open interval. */,
      ROM_START_ADDRESS_IN_OTHER - ROM_NS_START_ADDRESS_IN_M33 }, /* ROM NS */
};

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t USDHC_MemorySpaceConvert(uint32_t *realAddress, uint32_t origAddress, uint32_t size)
{
    // First convert to nonsecure address. In this way, the entries of the table can be reduced.
    uint32_t address = address_to_nonsecure_address(origAddress);

    for (int i = 0; i < ARRAY_SIZE(s_memoryRegionConvertTable); i++)
    {
        if ((address >= s_memoryRegionConvertTable[i].startAddress) &&
            ((address + size) <= s_memoryRegionConvertTable[i].endAddress))
        {
            address += s_memoryRegionConvertTable[i].offset;
            break;
        }
        /*
         * Note: If the operation is crossing memory boundaries, return error.
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

    *realAddress = address;

    return kStatus_Success;
}
