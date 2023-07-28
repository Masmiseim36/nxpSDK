/*
 * Copyright 2016-2021 NXP
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

//!@brief Unique ID constants
enum _uid_constrants
{
    kUniqueId_SizeInBytes = 8,
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
    kIndexITCM = 0,
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

//!@brief IMXRT SOC System ID
#define IMXRT_SOC_SYSTEM_ID (0x1015UL)

#endif // __TARGET_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
