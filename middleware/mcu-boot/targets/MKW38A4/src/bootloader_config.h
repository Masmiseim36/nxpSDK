/*
 * Copyright (c) 2014-2015, Freescale Semiconductor, Inc.
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

//! @name Peripheral configuration macros
//@{

#if !defined(BL_CONFIG_LPUART)
#define BL_CONFIG_LPUART (1u)
#define BL_CONFIG_LPUART_0 (1u)
#if !defined(BL_CONFIG_LIN)
#define BL_CONFIG_LIN (1u)
#endif
#endif
#if !defined(BL_CONFIG_I2C)
#define BL_CONFIG_I2C (1u)
#endif
#if !defined(BL_CONFIG_DSPI)
#define BL_CONFIG_DSPI (1u)
#endif
#if !defined(BL_CONFIG_CAN)
#define BL_CONFIG_CAN (1u)
#endif

//@}

#if !defined(BL_TARGET_FLASH) && !defined(BL_TARGET_RAM)
#define BL_TARGET_FLASH (1u)
#endif

#if defined(BL_TARGET_RAM)
#define BL_FEATURE_FLASH_SECURITY (0u)
#define BL_FEATURE_ERASEALL_UNSECURE (1u)
#else
#define BL_FEATURE_FLASH_SECURITY (1u)
#define BL_FEATURE_ERASEALL_UNSECURE (0u)
#endif

#define BL_FEATURE_ENABLE_FLASH_PROGRAM_SECTION (1u)

#if !defined(BL_TARGET_RAM)
#define BL_FEATURE_CRC_CHECK (1u)
// Determines whether to support reliable-update function and command
#define BL_FEATURE_RELIABLE_UPDATE (1u)
// Determines the method of reliable-update (hardware/software)
#define BL_FEATURE_HARDWARE_SWAP_UPDATE (1u)
#endif

// Determines whether to support Dflash
#define BL_FEATURE_SUPPORT_DFLASH (1u)
#define BL_FEATURE_SUPPORT_PFLASH_DFLASH_CROSS_WRITE (1u)

#define BL_FEATURE_QSPI_MODULE (0u)

#define BL_FEATURE_ENCRYPTION (0u)

#define BL_FEATURE_UART_AUTOBAUD_IRQ (1u)

#define BL_FEATURE_READ_MEMORY (1u)

#if defined(BL_TARGET_RAM)
#define BL_FEATURE_FILL_MEMORY (0u)
#else
#define BL_FEATURE_FILL_MEMORY (1u)
#endif

#define BL_FEATURE_MIN_PROFILE (0u)

#define BL_FEATURE_CORE_CLOCK_DEFAULT (1u)

#define BL_FEATURE_6PINS_PERIPHERAL (1u)

// Bootloader peripheral detection default timeout in milliseconds
// After coming out of reset the bootloader will spin in a peripheral detection
// loop for this amount of time. A zero value means no time out.
#if defined(DEBUG)
#define BL_DEFAULT_PERIPHERAL_DETECT_TIMEOUT 0u
#else
#define BL_DEFAULT_PERIPHERAL_DETECT_TIMEOUT 5000u
#endif // DEBUG

#define BL_FEATURE_POWERDOWN (0u)

// Bootloader powerdown timeout in milliseconds
// The bootloader enters a low power mode after waiting for this amount of time. A zero value means // no time out. Only
// applicable if BL_FEATURE_POWERDOWN is enabled.
#if defined(DEBUG)
#define BL_DEFAULT_POWERDOWN_TIMEOUT 10000u
#else
#define BL_DEFAULT_POWERDOWN_TIMEOUT 600000u
#endif // DEBUG

// The bootloader will check this address for the application vector table upon startup.
#if !defined(BL_APP_VECTOR_TABLE_ADDRESS)
#define BL_APP_VECTOR_TABLE_ADDRESS 0xa000u
#endif

#if !defined(BL_BACKUP_APP_START)
#define BL_BACKUP_APP_START 0x00020000u    // half of flash memory
#endif

#endif // __BOOTLOADER_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
