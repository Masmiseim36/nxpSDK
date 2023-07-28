/*
 * Copyright 2018 NXP
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(__TARGET_CONFIG_H__)
#define __TARGET_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief Constants for FlexSPI features.
enum
{
    kFlexSpi_AhbMemoryMaxSizeMB = (256u * 1024u * 1024u),
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

//!@brief PIT backward compatible defintion
#define kCLOCK_Pit kCLOCK_Pit1

//!@brief FLEXSPI clock definitions
enum
{
    kFlexSpiSerialClk_30MHz = 1,
    kFlexSpiSerialClk_50MHz = 2,
    kFlexSpiSerialClk_60MHz = 3,
    kFlexSpiSerialClk_80MHz = 4,
    kFlexSpiSerialClk_100MHz = 5,
    kFlexSpiSerialClk_120MHz = 6,
    kFlexSpiSerialClk_133MHz = 7,
    kFlexSpiSerialClk_166MHz = 8,
    kFlexSpiSerialClk_200MHz = 9,
};

//!@brief FLEXSPI instnaces
enum
{
    kFlexspiInstance_1 = 1,
    kFlexspiInstance_2 = 2,
};

//!@brief FLEXSPI Boot Clock Source
enum
{
    kFlexSpiBootClkcSrc = 4,
};

enum
{
    kProduct_USB_PID = 0x0073,
    kProduct_USB_VID = 0x15a2,
};

enum
{
    kIndexITCM = 0,
    kIndexDTCM = 1,
    kIndexOCRAM = 2,
    kIndexFlexSpiNor = 3,
    kIndexFlexSpiNorAlias = 4,
    kIndexFlexSpiNor2 = 5,
    kIndexSemc = 6,
};

#define SEMC_MAX_CLK_FREQ kSemcClkFreq_200MHz
#define SEMC_2ND_MAX_CLK_FREQ kSemcClkFreq_166MHz

//!@brief IMXRT SOC System ID
#define IMXRT_SOC_SYSTEM_ID (0x1170UL)

#define BUGFIX_INC_VER_RETRIEVER                                                                                    \
    {                                                                                                               \
        0x490f480eu, 0x42886b00u, 0x2000d102u, 0x477043c0u, 0x490d480cu, 0x428869c0u, 0x480cd107u, 0x05c08c00u,     \
            0x2003d501u, 0x20024770u, 0x48094770u, 0x6b004909u, 0xd1014288u, 0x47702001u, 0x47702000u, 0x00216100u, \
            0x73282000u, 0x0022b740u, 0x68104a02u, 0x40c84900u, 0x00218fc0u, 0x7813e005u,                           \
    }

#endif // __TARGET_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
