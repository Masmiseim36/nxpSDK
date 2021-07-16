/*
 * Copyright 2014-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#if !defined(__TARGET_CONFIG_H__)
#define __TARGET_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief Constants for FlexSPI features.
enum
{
    kFlexSpi_AhbMemoryMaxSizeMB = (504u * 1024u * 1024u),
};

//! @brief Version constants for the target.
enum _target_version_constants
{
    kTarget_Version_Name = 'T',
    kTarget_Version_Major = 1,
    kTarget_Version_Minor = 0,
    kTarget_Version_Bugfix = 0
};

//! @brief Memory Map index constants
enum
{
    kIndexSRAM = 0,
    kIndexDTCM = 1,
    kIndexOCRAM = 2,
    kIndexFlexSpiNor = 3,
    kIndexSemcNor = 4,
    kSRAMSeparatrix = (uint32_t)0x20000000 //!< This value is the start address of SRAM_U
};

//! @brief FlexSPI supported speed defintions
enum
{
    kFlexSpiSerialClk_30MHz = 1,
    kFlexSpiSerialClk_50MHz = 2,
    kFlexSpiSerialClk_60MHz = 3,
    kFlexSpiSerialClk_75MHz = 4,
    kFlexSpiSerialClk_80MHz = 5,
    kFlexSpiSerialClk_100MHz = 6,
    kFlexSpiSerialClk_133MHz = 7,
    kFlexSpiSerialClk_166MHz = 8,
    kFlexSpiSerialClk_200MHz = 9,
};

#define SEMC_MAX_CLK_FREQ kSemcClkFreq_133MHz
#define SEMC_2ND_MAX_CLK_FREQ kSemcClkFreq_108MHz

#define ROM_API_HAS_FLEXSPI_NOR_INIT (1)
#define ROM_API_HAS_FELXSPI_NOR_PROGRAM (1)
#define ROM_API_HAS_FLEXSPI_NOR_ERASE (1)
#define ROM_API_HAS_FLEXSPI_NOR_ERASE_SECTOR (1)
#define ROM_API_HAS_FLEXSPI_NOR_ERASE_BLOCK (1)
#define ROM_API_HAS_FLEXSPI_NOR_ERASE_ALL (0)
#define ROM_API_HAS_FLEXSPI_NOR_READ (0)
#define ROM_API_HAS_FLEXSPI_NOR_GET_CFG (0)
#define ROM_API_HAS_FLEXSPI_CLEAR_CACHE (1)
#define ROM_API_HAS_FLEXSPI_XFER (1)

//!@brief IMXRT SOC System ID
#define IMXRT_SOC_SYSTEM_ID (0x1020UL)

#endif // __TARGET_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
