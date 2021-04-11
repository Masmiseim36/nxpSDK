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
    //! Frequency of the HIRC.
    kHIRC = 48000000,

    //! 8MHz low frequency IRC.
    kLIRC8M = 8000000,

    //! 2MHz low frequency IRC.
    kLIRC2M = 2000000,

    //! The core clock cannot go above 72MHz.
    kMaxCoreClock = 72000000u,
    //! The bus clock cannot go above 24MHz.
    kMaxBusClock = 24000000,

    kDivider_Min = 1,
    kDivider_Max = 16,
};

//! @brief Constants for sram partition
enum
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
#define CRC_CHECK_FAILURE_PIN_NUMBER 6
#define CRC_CHECK_FAILURE_PIN_PORT PORTA
#define CRC_CHECK_FAILURE_PIN_GPIO GPIOA
#define CRC_CHECK_FAILURE_PIN_DEFAULT_MODE 0
#define CRC_CHECK_FAILURE_PIN_GPIO_MODE 1

#endif // __TARGET_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
