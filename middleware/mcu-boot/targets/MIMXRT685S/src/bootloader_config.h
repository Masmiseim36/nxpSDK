/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __BOOTLOADER_CONFIG_H__
#define __BOOTLOADER_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//
// Bootloader configuration options
//

//==============================================================================
//! @name Device type configuration macros
//@{
#if !defined(BL_DEVICE_IS_LPC_SERIES)
#define BL_DEVICE_IS_LPC_SERIES (1)
#endif
//@}
//==============================================================================
//! @name Target-resident configuration macros
//@{
#if !defined(BL_TARGET_FLASH)
#define BL_TARGET_FLASH (1)
#endif
#define BL_FEATURE_HAS_NO_INTERNAL_FLASH (1)
//@}

//==============================================================================
//! @name External memory configuration macros
//@{
// Determines whether to support qspi flash
#define BL_FEATURE_FLEXSPI_NOR_MODULE (1)
// The FLASH is a Octal FLASH which support from Macronix, connected via PORTB
#define BL_FLLEXSPI_NOR_CFG_BLOCK (1)
#define BL_FLEXSPI_NOR_CFG_BLOCK_OFFSET (0x400u)
#ifndef BL_FLEXSPI_NOR_CFG_BLOCK
#define BL_FLEXSPI_NOR_CFG_OPT0 (0xc1533052u)
#define BL_FLEXSPI_NOR_CFG_OPT1 (0x20000014u)
#endif

#if BL_FEATURE_FLEXSPI_NOR_MODULE
#define BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE (0)
#define BL_FEATURE_FLEXSPI_NOR_MODULE_ERASE_VERIFY (1)
#define BL_FLEXSPI_AMBA_BASE (0x08000000u)
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE
#define FLEXSPI_NOR_FEATURE_ENABLE_PROGRAMMING (BL_FEATURE_MEM_WRITE_ENABLE)
//@}

// The bootloader will check this address for the application vector table upon startup.
#if !defined(BL_APP_VECTOR_TABLE_ADDRESS)
#define BL_APP_VECTOR_TABLE_ADDRESS (0x08040000u)
#endif

/* Backup application Info */
#if !defined(BL_TARGET_RAM)
#define BL_FEATURE_RELIABLE_UPDATE (1)
#define BL_FEATURE_HARDWARE_SWAP_UPDATE (0)
#define BL_FEATURE_HARDWARE_SWAP_ENG_GPR (0)
// Note the BAKCUP APP START address must be sector-aligned.
#define BL_FEATURE_FLASH_SECTOR_SIZE (0x1000u)

#define BL_FEATURE_HAS_FLEXSPI_NOR_ROMAPI (1)
#define ROM_API_HAS_FLEXSPI_XFER (1)

#define BL_FEATURE_FLASH_START 0x08000000u
#define BL_FEATURE_FLASH_SIZE 0x4000000u
#define BL_FEATURE_BOOTLOADER_START BL_FEATURE_FLASH_START
#define BL_FEATURE_BOOTLOADER_SIZE 0x40000u
#ifndef BL_FEATURE_FLASH_SECTOR_SIZE
#define BL_FEATURE_FLASH_SECTOR_SIZE 0x1000u
#endif
#define BL_FEATURE_BOOT_META_START \
    (BL_FEATURE_BOOTLOADER_START + BL_FEATURE_BOOTLOADER_SIZE - 4 * BL_FEATURE_FLASH_SECTOR_SIZE)
#define BL_FEATURE_SWAP_META_START (BL_FEATURE_BOOT_META_START + 2 * BL_FEATURE_FLASH_SECTOR_SIZE)
#define BL_FEATURE_PRIMARY_IMG_START (BL_FEATURE_BOOTLOADER_START + BL_FEATURE_BOOTLOADER_SIZE)
#define BL_FEATURE_PRIMARY_IMG_PARTITION_SIZE 0x100000
#define BL_FEATURE_SECONDARY_IMG_START (BL_FEATURE_PRIMARY_IMG_START + BL_FEATURE_PRIMARY_IMG_PARTITION_SIZE)
#define BL_FEATURE_SCRATCH_PARTITION_SIZE (0x1000)
#define BL_FEATURE_SECOND_IMG_PARTITION_SIZE (BL_FEATURE_PRIMARY_IMG_PARTITION_SIZE + BL_FEATURE_SCRATCH_PARTITION_SIZE)
#define BL_FEATURE_USER_DATA_PARTITION_START (BL_FEATURE_SECONDARY_IMG_START + BL_FEATURE_SECOND_IMG_PARTITION_SIZE)
#define BL_FEATURE_USER_DATA_PARTITION_SIZE \
    (BL_FEATURE_FLASH_START + BL_FEATURE_FLASH_SIZE - BL_FEATURE_USER_DATA_PARTITION_START)

#define BL_IMG_META_OFFSET (BL_FEATURE_FLASH_SECTOR_SIZE * 2)

#endif // !BL_TARGET_RAM

#define PRINT_UART USART0

#endif // __BOOTLOADER_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
