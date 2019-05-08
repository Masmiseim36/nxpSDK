/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
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
    kFLL_72MHz = 72000000ul,
    kFLL_48MHz = 48000000ul,
    kFLL_HighFreq = 72000000ul,
    kHIRC = 48000000u,
    kMinCoreClockWithUsbSupport = 20000000u,
    kMaxCoreClock = 720000000u,
    kMaxBusClock = 25000000u,
    kMaxFlashClock = 25000000u,

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

//! CRC check pinmux configurations
// Note: This default muxing slot of selected crc check failure pin must be ALT0
#define CRC_CHECK_FAILURE_PIN_NUMBER 5
#define CRC_CHECK_FAILURE_PIN_PORT PORTC
#define CRC_CHECK_FAILURE_PIN_GPIO GPIOC
#define CRC_CHECK_FAILURE_PIN_DEFAULT_MODE 0
#define CRC_CHECK_FAILURE_PIN_GPIO_MODE 1

#endif // __TARGET_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
