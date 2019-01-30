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

#if(defined(__GNUC__)&&defined(BL_TARGET_RAM))
#ifdef DEBUG
#undef DEBUG
#define NDEBUG
#endif
#endif

//! @name Peripheral configuration macros
//@{

#if !defined(BL_CONFIG_SCUART)
#define BL_CONFIG_SCUART (1)
#endif
#if !defined(BL_CONFIG_I2C)
#if defined(BL_TARGET_RAM)
#define BL_CONFIG_I2C (0)
#else
#define BL_CONFIG_I2C (1)
#endif
#endif
#if !defined(BL_CONFIG_DSPI)
#if defined(BL_TARGET_RAM)
#define BL_CONFIG_DSPI (0)
#else
#define BL_CONFIG_DSPI (1)
#endif
#endif
#if !defined(BL_CONFIG_CAN)
#define BL_CONFIG_CAN (1)
#endif

//@}

#if !defined(BL_TARGET_FLASH) && !defined(BL_TARGET_RAM)
#define BL_TARGET_FLASH (1)
#endif

#if defined(BL_TARGET_RAM)
#define BL_FEATURE_FLASH_SECURITY (0)
#else
#define BL_FEATURE_FLASH_SECURITY (1)
#endif

#if !defined(BL_TARGET_RAM)
#define BL_FEATURE_CRC_CHECK (1)
#endif

#define BL_FEATURE_QSPI_MODULE (0)

#define BL_FEATURE_ENCRYPTION (0)

#define BL_FEATURE_UART_AUTOBAUD_IRQ (1)

#define BL_FEATURE_CAN (BL_CONFIG_CAN)

#define BL_FEATURE_READ_MEMORY (1)

#if defined(BL_TARGET_RAM)
#define BL_FEATURE_FILL_MEMORY (0)
#else
#define BL_FEATURE_FILL_MEMORY (1)
#endif

#define BL_FEATURE_MIN_PROFILE (1)

// Bootloader peripheral detection default timeout in milliseconds
// After coming out of reset the bootloader will spin in a peripheral detection
// loop for this amount of time. A zero value means no time out.
#if DEBUG
#define BL_DEFAULT_PERIPHERAL_DETECT_TIMEOUT 0
#else
#define BL_DEFAULT_PERIPHERAL_DETECT_TIMEOUT 5000
#endif // DEBUG

#define BL_FEATURE_POWERDOWN (0)

// Bootloader powerdown timeout in milliseconds
// The bootloader enters a low power mode after waiting for this amount of time. A zero value means // no time out. Only
// applicable if BL_FEATURE_POWERDOWN is enabled.
#if DEBUG
#define BL_DEFAULT_POWERDOWN_TIMEOUT 10000
#else
#define BL_DEFAULT_POWERDOWN_TIMEOUT 600000
#endif // DEBUG

// The bootloader will check this address for the application vector table upon startup.
#if !defined(BL_APP_VECTOR_TABLE_ADDRESS)
#define BL_APP_VECTOR_TABLE_ADDRESS 0xa000
#endif

/* Serial Port Info */

/**************************************************************************
 * Note:
 *
 * Because of the changes to the UART modules, we can no longer define
 * the TERM_PORT as a base pointer.  The uart functions have been modified
 * accommodate this change.  Now, TERM_PORT_NUM must be defined as the
 * number of the UART port desired to use
 *
 * TERM_PORT_NUM = 0  -- This allows you to use UART0; default pins are
 *                        PTA14 and PTA15
 *
 * TERM_PORT_NUM = 1  -- This allows you to use UART1; default pins are
 *                        PTC3 and PTC4
 *
 * TERM_PORT_NUM = 2  -- This allows you to use UART2; default pins are
 *                        PTD2 and PTD3
 *
 *************************************************************************/
#define TERM_PORT_NUM 0

#define TERMINAL_BAUD 19200
#undef HW_FLOW_CONTROL

#endif // __BOOTLOADER_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
