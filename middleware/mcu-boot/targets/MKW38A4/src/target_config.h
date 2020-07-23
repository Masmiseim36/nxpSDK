/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(__TARGET_CONFIG_H__)
#define __TARGET_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief Constants for clock frequencies.
enum _target_clocks
{
    kDefaultClock = 20971520u,
    kHIRC = 48000000u,
    kMinCoreClockWithUsbSupport = 20000000u,
    kMaxCoreClock = 48000000u,
    kMaxBusClock = 24000000u,
    kMaxFlashClock = 24000000u,

    kDivider_Min = 1,
    kDivider_Max = 16,
};

//! @brief Constants for sram partition
enum _sram_partition
{
    kSram_LowerPart = 1,
    kSram_UpperPart = 3,
};

//! @brief Version constants for the target.
enum _target_version_constants
{
    kTarget_Version_Name = 'T',
    kTarget_Version_Major = 1,
    kTarget_Version_Minor = 0,
    kTarget_Version_Bugfix = 0
};

#define FSL_FEATURE_FLASH_HAS_FLEX_NVM_ALIAS (1)
#define FSL_FEATURE_FLASH_FLEX_NVM_ALIAS_START_ADDRESS (0x00040000)
#define FSL_FEATURE_FLASH_FLEX_NVM_ALIAS_BLOCK_SIZE (262144)
#define FSL_FEATURE_FLASH_FLEX_NVM_ALIAS_BLOCK_COUNT (1)

#if defined (KW37A4_SERIES)
#define KW3_789_SERIES (KW37A4_SERIES)
#elif defined KW37Z4_SERIES
#define KW3_789_SERIES (KW37Z4_SERIES)
#elif defined KW38A4_SERIES 
#define KW3_789_SERIES (KW38A4_SERIES)
#elif defined KW38Z4_SERIES
#define KW3_789_SERIES (KW38Z4_SERIES)
#elif defined (KW39A4_SERIES)
#define KW3_789_SERIES (KW39A4_SERIES)
#else
#error "No valid CPU defined!"
#endif

#endif // __TARGET_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
