/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
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

//! @name Peripheral configuration macros
//@{

#define BL_CONFIG_LPUART_0 (1)
#define BL_CONFIG_LPUART (BL_CONFIG_LPUART_0)

#define BL_CONFIG_LPI2C_0 (1)
#define BL_CONFIG_LPI2C (BL_CONFIG_LPI2C_0)

#define BL_CONFIG_LPSPI_0 (1)
#define BL_CONFIG_LPSPI (BL_CONFIG_LPSPI_0)


#if !defined(BL_TARGET_FLASH) && !defined(BL_TARGET_RAM)
#define BL_TARGET_FLASH (1)
#endif

#if defined(BL_TARGET_RAM)
#define BL_FEATURE_FLASH_SECURITY (0)
#else
#define BL_FEATURE_FLASH_SECURITY (1)
#endif

#if !defined(BL_FEATURE_MIN_PROFILE)
#define BL_FEATURE_MIN_PROFILE (0)
#endif

// Determines whether to support Kinetis C90TFS FTFx flash
#define BL_FLASH_TYPE_KINETIS_C90TFS_FLASH (1)
#define BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE (1)

#define BL_HAS_MASS_ERASE (0)

#if defined(BL_TARGET_RAM)
#define BL_FEATURE_FLASH_SECURITY (0)
#else
#define BL_FEATURE_FLASH_SECURITY (1)
#endif

#define BL_FLASH_VERIFY_DISABLE (1)

#if !defined(BL_TARGET_RAM)
#define BL_FEATURE_CRC_CHECK (1)
#define BL_FEATURE_CRC_ASSERT (1)
#endif

#define BL_FEATURE_READ_MEMORY (1)
#define BL_FEATURE_FILL_MEMORY (1)
#define BL_FEATURE_ERASEALL_UNSECURE  FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_UNSECURE_CMD

#define BL_FEATURE_PHANTOM_UPDATE (1)

#define BL_HAS_QSPI_MODULE (0)

#define BL_FEATURE_ENCRYPTION (0)

#define BL_FEATURE_UART_AUTOBAUD_IRQ (1)

// Bootloader peripheral detection default timeout in milliseconds
// After coming out of reset the bootloader will spin in a peripheral detection
// loop for this amount of time. A zero value means no time out.
#if DEBUG
#define BL_DEFAULT_PERIPHERAL_DETECT_TIMEOUT 5000
#else
#define BL_DEFAULT_PERIPHERAL_DETECT_TIMEOUT 5000
#endif // DEBUG

// The bootloader will check this address for the application vector table upon startup.
#if !defined(BL_APP_VECTOR_TABLE_ADDRESS)
#define BL_APP_VECTOR_TABLE_ADDRESS 0xA000
#endif

// The bootloader will calculate the start address and end address of RAM based on following definitions.
#define RAM_LOWER_PART (1)
#define RAM_UPPER_PART (1)


#endif // __BOOTLOADER_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
