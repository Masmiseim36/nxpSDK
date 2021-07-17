/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(__TARGET_CONFIG_H__)
#define __TARGET_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief OTA bootloader start address
#define __ROM_START (0x08000000u)

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
    kSRAMSeparatrix = (uint32_t)0x20000000 //!< This value is the start address of SRAM_U
};

//! @brief FlexSPI supported speed defintions
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

extern uint32_t get_flexspinor_instance(void);
extern uint32_t get_flexspinor_amba_base(void);

#define ROM_API_HAS_FLEXSPI_NOR_INIT (1)
#define ROM_API_HAS_FELXSPI_NOR_PROGRAM (1)
#define ROM_API_HAS_FLEXSPI_NOR_ERASE (1)
#define ROM_API_HAS_FLEXSPI_NOR_ERASE_ALL (1)
#define ROM_API_HAS_FLEXSPI_NOR_READ (1)
#define ROM_API_HAS_FLEXSPI_NOR_GET_CFG (1)
#define ROM_API_HAS_FLEXSPI_CLEAR_CACHE (1)
#define ROM_API_HAS_FLEXSPI_XFER (1)

// USB PLL divider select value
#define OTP_USB_PLL_DIV_SEL_IDX (0x63)
#define OTP_USB_PLL_DIV_SEL_SHIFT (19u)
#define OTP_USB_PLL_DIV_SEL_MASK (0x7u << OTP_USB_PLL_DIV_SEL_SHIFT)
#define OTP_USB_PLL_DIV_SEL_VALUE() \
    ((OCOTP->OTP_SHADOW[OTP_USB_PLL_DIV_SEL_IDX] & OTP_USB_PLL_DIV_SEL_MASK) >> OTP_USB_PLL_DIV_SEL_SHIFT)

#endif // __TARGET_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
