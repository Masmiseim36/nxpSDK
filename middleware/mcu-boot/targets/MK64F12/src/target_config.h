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

//! @brief Constants for clock frequencies.
enum _target_clocks
{
    kDefaultClock = 20971520u,
    //! Frequency of the HIRC.
    kHIRC = 48000000,

    //! The minimum core clock cannot go below 20MHz when USB is enabled.
    kMinCoreClockWithUsbSupport = 20000000u,

    //! The core/system clock cannot go above 120MHz.
    kMaxCoreClock = 120000000,

    //! The bus clock cannot go above 60MHz.
    kMaxBusClock = 60000000,

    //! The flex bus clock cannot go above 50MHz.
    kMaxFlexBusClock = 50000000,

    //! The flash clock cannot go above 24MHz.
    kMaxFlashClock = 24000000,

    //! Valid clock divider range
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
