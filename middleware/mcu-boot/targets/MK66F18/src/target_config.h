/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(__TARGET_CONFIG_H__)
#define __TARGET_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define kCLOCK_Pit kCLOCK_Pit0

//! @brief Constants for clock frequencies.
enum _target_clocks
{
    kDefaultClock = 20971520u,
    kMcgOutWithFullSpeedUsb = 48000000u,
    kMcgOutWithHighSpeedUsb = 120000000u,
    kMinCoreClockWithFsUsbSupport = 20000000u,
    kMaxCoreClock = 180000000u,
    kMaxBusClock = 60000000u,
    kMaxFlexBusClock = 60000000u,
    kMaxFlashClock = 28000000u,

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

#endif // __TARGET_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
