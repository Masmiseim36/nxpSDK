/*
 * Copyright (c) 2014-2015 Freescale Semiconductor, Inc.
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

#if (defined(__GNUC__) && defined(BL_TARGET_RAM))
#ifdef DEBUG
#undef DEBUG
#endif
#endif

//! @name Peripheral configuration macros
//@{

#define BL_FEATURE_ROM_UART_PORT (0)
#define BL_FEATURE_ROM_I2C_PORT (0)
#define BL_FEATURE_ROM_SPI_PORT (0)

#if !defined(BL_CONFIG_LPUART_0)
#define BL_CONFIG_LPUART_0 (1)
#endif
#if !defined(BL_CONFIG_LPUART_1)
#define BL_CONFIG_LPUART_1 (BL_FEATURE_ROM_UART_PORT)
#endif
#if !defined(BL_CONFIG_LPUART_2)
#define BL_CONFIG_LPUART_2 (BL_FEATURE_ROM_UART_PORT)
#endif
#define BL_CONFIG_LPUART (BL_CONFIG_LPUART_0 || BL_CONFIG_LPUART_1 || BL_CONFIG_LPUART_2)

#if !defined(BL_CONFIG_LPI2C_0)
#define BL_CONFIG_LPI2C_0 (0)
#endif
#if !defined(BL_CONFIG_LPI2C_1)
#define BL_CONFIG_LPI2C_1 (1)
#endif
#if !defined(BL_CONFIG_LPI2C_2)
#define BL_CONFIG_LPI2C_2 (0)
#endif
#define BL_CONFIG_LPI2C (BL_CONFIG_LPI2C_0 || BL_CONFIG_LPI2C_1 || BL_CONFIG_LPI2C_2)

#if !defined(BL_CONFIG_LPSPI_0)
#define BL_CONFIG_LPSPI_0 (0)
#endif
#if !defined(BL_CONFIG_LPSPI_1)
#define BL_CONFIG_LPSPI_1 (0)
#endif
#if !defined(BL_CONFIG_LPSPI_2)
#define BL_CONFIG_LPSPI_2 (1)
#endif
#define BL_CONFIG_LPSPI (BL_CONFIG_LPSPI_0 || BL_CONFIG_LPSPI_1 || BL_CONFIG_LPSPI_2)

#if !defined(BL_CONFIG_USB_HID)
#define BL_CONFIG_USB_HID (1)
#endif
#if !defined(BL_CONFIG_USB_MSC)
#define BL_CONFIG_USB_MSC (0)
#endif
//@}

#if !defined(BL_TARGET_FLASH) && !defined(BL_TARGET_RAM)
#define BL_TARGET_FLASH (1)
#endif

// Determines whether to support Kinetis C90TFS FTFx flash
#define BL_FLASH_TYPE_KINETIS_C90TFS_FLASH (1)
#if defined(BL_TARGET_RAM)
#define BL_FEATURE_FLASH_SECURITY (0)
#else
#define BL_FEATURE_FLASH_SECURITY (1)
#endif

#define BL_FEATURE_MIN_PROFILE (0)

#define BL_FEATURE_ERASEALL_UNSECURE (0)
#define BL_FEATURE_FLASH_VERIFY_DISABLE (0)

#if !defined(BL_TARGET_RAM)
#define BL_FEATURE_CRC_CHECK (1)
#endif

#define BL_FEATURE_READ_MEMORY (1)

#define BL_FEATURE_QSPI_MODULE (0)

#define BL_FEATURE_ENCRYPTION_MMCAU (1)
#define BL_FEATURE_ENCRYPTION (BL_FEATURE_ENCRYPTION_MMCAU)

#define BL_FEATURE_UART_AUTOBAUD_IRQ (1)

#define BL_HAS_MULTI_CORE (0)

#define BL_FEATURE_UART_RX_PULLUP (1)

// Bootloader peripheral detection default timeout in milliseconds
// After coming out of reset the bootloader will spin in a peripheral detection
// loop for this amount of time. A zero value means no time out.
#if DEBUG
#define BL_DEFAULT_PERIPHERAL_DETECT_TIMEOUT 0
#else
#define BL_DEFAULT_PERIPHERAL_DETECT_TIMEOUT 5000
#endif // DEBUG

// The bootloader will check this address for the application vector table upon startup.
#if !defined(BL_APP_VECTOR_TABLE_ADDRESS)
#define BL_APP_VECTOR_TABLE_ADDRESS 0xA000
#endif

#endif // __BOOTLOADER_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
