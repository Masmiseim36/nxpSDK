/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __BOOTLOADER_COMMON_H__
#define __BOOTLOADER_COMMON_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#if !defined(WIN32)
#include <stdbool.h>
#endif
#include "blfwk/bootloader_config.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
* Definitions
******************************************************************************/

/*! @brief Construct a status code value from a group and code number. */
#define MAKE_STATUS(group, code) ((((group)*100) + (code)))

/*! @brief Construct the version number for drivers. */
#define MAKE_VERSION(major, minor, bugfix) (((major) << 16) | ((minor) << 8) | (bugfix))

/* Debug console type definition. */
#define DEBUG_CONSOLE_DEVICE_TYPE_NONE 0U     /*!< No debug console.             */
#define DEBUG_CONSOLE_DEVICE_TYPE_UART 1U     /*!< Debug console base on UART.   */
#define DEBUG_CONSOLE_DEVICE_TYPE_LPUART 2U   /*!< Debug console base on LPUART. */
#define DEBUG_CONSOLE_DEVICE_TYPE_LPSCI 3U    /*!< Debug console base on LPSCI.  */
#define DEBUG_CONSOLE_DEVICE_TYPE_USBCDC 4U   /*!< Debug console base on USBCDC. */
#define DEBUG_CONSOLE_DEVICE_TYPE_FLEXCOMM 5U /*!< Debug console base on USBCDC. */
#define DEBUG_CONSOLE_DEVICE_TYPE_IUART 6U    /*!< Debug console base on i.MX UART. */
#define DEBUG_CONSOLE_DEVICE_TYPE_VUSART 7U   /*!< Debug console base on LPC_USART. */

/*! @brief Status group numbers. */
enum _status_groups
{
    kStatusGroup_Generic = 0,                 /*!< Group number for generic status codes. */
    kStatusGroup_FLASH = 1,                   /*!< Group number for FLASH status codes. */
    kStatusGroup_LPSPI = 4,                   /*!< Group number for LPSPI status codes. */
    kStatusGroup_FLEXIO_SPI = 5,              /*!< Group number for FLEXIO SPI status codes. */
    kStatusGroup_DSPI = 6,                    /*!< Group number for DSPI status codes. */
    kStatusGroup_FLEXIO_UART = 7,             /*!< Group number for FLEXIO UART status codes. */
    kStatusGroup_FLEXIO_I2C = 8,              /*!< Group number for FLEXIO I2C status codes. */
    kStatusGroup_LPI2C = 9,                   /*!< Group number for LPI2C status codes. */
    kStatusGroup_UART = 10,                   /*!< Group number for UART status codes. */
    kStatusGroup_I2C = 11,                    /*!< Group number for UART status codes. */
    kStatusGroup_LPSCI = 12,                  /*!< Group number for LPSCI status codes. */
    kStatusGroup_LPUART = 13,                 /*!< Group number for LPUART status codes. */
    kStatusGroup_SPI = 14,                    /*!< Group number for SPI status code.*/
    kStatusGroup_XRDC = 15,                   /*!< Group number for XRDC status code.*/
    kStatusGroup_SEMA42 = 16,                 /*!< Group number for SEMA42 status code.*/
    kStatusGroup_SDHC = 17,                   /*!< Group number for SDHC status code */
    kStatusGroup_SDMMC = 18,                  /*!< Group number for SDMMC status code */
    kStatusGroup_SAI = 19,                    /*!< Group number for SAI status code */
    kStatusGroup_MCG = 20,                    /*!< Group number for MCG status codes. */
    kStatusGroup_SCG = 21,                    /*!< Group number for SCG status codes. */
    kStatusGroup_SDSPI = 22,                  /*!< Group number for SDSPI status codes. */
    kStatusGroup_FLEXIO_I2S = 23,             /*!< Group number for FLEXIO I2S status codes */
    kStatusGroup_FLEXIO_MCULCD = 24,          /*!< Group number for FLEXIO LCD status codes */
    kStatusGroup_FLASHIAP = 25,               /*!< Group number for FLASHIAP status codes */
    kStatusGroup_FLEXCOMM_I2C = 26,           /*!< Group number for FLEXCOMM I2C status codes */
    kStatusGroup_I2S = 27,                    /*!< Group number for I2S status codes */
    kStatusGroup_IUART = 28,                  /*!< Group number for IUART status codes */
    kStatusGroup_CSI = 29,                    /*!< Group number for CSI status codes */
    kStatusGroup_SDRAMC = 35,                 /*!< Group number for SDRAMC status codes. */
    kStatusGroup_POWER = 39,                  /*!< Group number for POWER status codes. */
    kStatusGroup_ENET = 40,                   /*!< Group number for ENET status codes. */
    kStatusGroup_PHY = 41,                    /*!< Group number for PHY status codes. */
    kStatusGroup_TRGMUX = 42,                 /*!< Group number for TRGMUX status codes. */
    kStatusGroup_SMARTCARD = 43,              /*!< Group number for SMARTCARD status codes. */
    kStatusGroup_LMEM = 44,                   /*!< Group number for LMEM status codes. */
    kStatusGroup_QSPI = 45,                   /*!< Group number for QSPI status codes. */
    kStatusGroup_DMA = 50,                    /*!< Group number for DMA status codes. */
    kStatusGroup_EDMA = 51,                   /*!< Group number for EDMA status codes. */
    kStatusGroup_DMAMGR = 52,                 /*!< Group number for DMAMGR status codes. */
    kStatusGroup_FLEXCAN = 53,                /*!< Group number for FlexCAN status codes. */
    kStatusGroup_LTC = 54,                    /*!< Group number for LTC status codes. */
    kStatusGroup_FLEXIO_CAMERA = 55,          /*!< Group number for FLEXIO CAMERA status codes. */
    kStatusGroup_LPC_SPI = 56,                /*!< Group number for LPC_SPI status codes. */
    kStatusGroup_LPC_USART = 57,              /*!< Group number for LPC_USART status codes. */
    kStatusGroup_DMIC = 58,                   /*!< Group number for DMIC status codes. */
    kStatusGroup_SDIF = 59,                   /*!< Group number for SDIF status codes.*/
    kStatusGroup_SPIFI = 60,                  /*!< Group number for SPIFI status codes. */
    kStatusGroup_OTP = 61,                    /*!< Group number for OTP status codes. */
    kStatusGroup_MCAN = 62,                   /*!< Group number for MCAN status codes. */
    kStatusGroup_CAAM = 63,                   /*!< Group number for CAAM status codes. */
    kStatusGroup_ECSPI = 64,                  /*!< Group number for ECSPI status codes. */
    kStatusGroup_USDHC = 65,                  /*!< Group number for USDHC status codes.*/
    kStatusGroup_LPC_I2C = 66,                /*!< Group number for LPC_I2C status codes.*/
    kStatusGroup_ESAI = 69,                   /*!< Group number for ESAI status codes. */
    kStatusGroup_FLEXSPI = 70,                /*!< Group number for FLEXSPI status codes. */
    kStatusGroup_MMDC = 71,                   /*!< Group number for MMDC status codes. */
    kStatusGroup_MICFIL = 72,                 /*!< Group number for MIC status codes. */
    kStatusGroup_SDMA = 73,                   /*!< Group number for SDMA status codes. */
    kStatusGroup_NOTIFIER = 98,               /*!< Group number for NOTIFIER status codes. */
    kStatusGroup_DebugConsole = 99,           /*!< Group number for debug console status codes. */
    kStatusGroup_ApplicationRangeStart = 100, /*!< Starting number for application groups. */
};

enum _bl_status_groups
{
    kStatusGroup_Bootloader = kStatusGroup_ApplicationRangeStart, //!< Bootloader status group number (100).
    kStatusGroup_SBLoader,                                        //!< SB loader status group number (101).
    kStatusGroup_MemoryInterface,                                 //!< Memory interface status group number (102).
    kStatusGroup_PropertyStore,                                   //!< Property store status group number (103).
    kStatusGroup_AppCrcCheck,                                     //!< Application crc check status group number (104).
    kStatusGroup_Packetizer,                                      //!< Packetizer status group number (105).
    kStatusGroup_ReliableUpdate,                                  //!< Reliable Update status groupt number (106).
    kStatusGroup_Authentication,                                  //!< Authentication feature (107).
    kStatusGroup_RomApi                                           //!< ROM API status group number (108).
};

/*! @brief Generic status return codes. */
enum _generic_status
{
    kStatus_Success = MAKE_STATUS(kStatusGroup_Generic, 0),
    kStatus_Fail = MAKE_STATUS(kStatusGroup_Generic, 1),
    kStatus_ReadOnly = MAKE_STATUS(kStatusGroup_Generic, 2),
    kStatus_OutOfRange = MAKE_STATUS(kStatusGroup_Generic, 3),
    kStatus_InvalidArgument = MAKE_STATUS(kStatusGroup_Generic, 4),
    kStatus_Timeout = MAKE_STATUS(kStatusGroup_Generic, 5),
    kStatus_NoTransferInProgress = MAKE_STATUS(kStatusGroup_Generic, 6),
};

//! @brief Bootloader status codes.
//! @ingroup bl_core
enum _bootloader_status
{
    kStatus_UnknownCommand = MAKE_STATUS(kStatusGroup_Bootloader, 0),
    kStatus_SecurityViolation = MAKE_STATUS(kStatusGroup_Bootloader, 1),
    kStatus_AbortDataPhase = MAKE_STATUS(kStatusGroup_Bootloader, 2),
    kStatus_Ping = MAKE_STATUS(kStatusGroup_Bootloader, 3),
    kStatus_NoResponse = MAKE_STATUS(kStatusGroup_Bootloader, 4),
    kStatus_NoResponseExpected = MAKE_STATUS(kStatusGroup_Bootloader, 5),
    kStatus_CommandUnsupported = MAKE_STATUS(kStatusGroup_Bootloader, 6),
};

// !@brief SB loader status codes.
enum _sbloader_status
{
    kStatusRomLdrSectionOverrun = MAKE_STATUS(kStatusGroup_SBLoader, 0),
    kStatusRomLdrSignature = MAKE_STATUS(kStatusGroup_SBLoader, 1),
    kStatusRomLdrSectionLength = MAKE_STATUS(kStatusGroup_SBLoader, 2),
    kStatusRomLdrUnencryptedOnly = MAKE_STATUS(kStatusGroup_SBLoader, 3),
    kStatusRomLdrEOFReached = MAKE_STATUS(kStatusGroup_SBLoader, 4),
    kStatusRomLdrChecksum = MAKE_STATUS(kStatusGroup_SBLoader, 5),
    kStatusRomLdrCrc32Error = MAKE_STATUS(kStatusGroup_SBLoader, 6),
    kStatusRomLdrUnknownCommand = MAKE_STATUS(kStatusGroup_SBLoader, 7),
    kStatusRomLdrIdNotFound = MAKE_STATUS(kStatusGroup_SBLoader, 8),
    kStatusRomLdrDataUnderrun = MAKE_STATUS(kStatusGroup_SBLoader, 9),
    kStatusRomLdrJumpReturned = MAKE_STATUS(kStatusGroup_SBLoader, 10),
    kStatusRomLdrCallFailed = MAKE_STATUS(kStatusGroup_SBLoader, 11),
    kStatusRomLdrKeyNotFound = MAKE_STATUS(kStatusGroup_SBLoader, 12),
    kStatusRomLdrSecureOnly = MAKE_STATUS(kStatusGroup_SBLoader, 13),
    kStatusRomLdrResetReturned = MAKE_STATUS(kStatusGroup_SBLoader, 14),
    kStatusRomLdrRollbackBlocked = MAKE_STATUS(kStatusGroup_SBLoader, 15),
    kStatusRomLdrInvalidSectionMacCount = MAKE_STATUS(kStatusGroup_SBLoader, 16),
    kStatusRomLdrUnexpectedCommand = MAKE_STATUS(kStatusGroup_SBLoader, 17),
};

/*! @brief Type used for all status and error return values. */
typedef int32_t status_t;

#ifndef NULL
#define NULL 0
#endif

// The following macros are to be used when trying to save code size for specific peripheral configurations
// that will only be using one peripheral instance. most of the peripheral driver code can use multiple instances but by
// just using one
// we can save space
#define USE_ONLY_UART(instance) (defined(BL_FEATURE_UART_OPTIMIZE) && (BL_UART_USED_INSTANCE == instance))
#define USE_ONLY_SPI(instance) (defined(BL_FEATURE_SPI_OPTIMIZE) && (BL_SPI_USED_INSTANCE == instance))
#define USE_ONLY_I2C(instance) (defined(BL_FEATURE_I2C_OPTIMIZE) && (BL_I2C_USED_INSTANCE == instance))

//! @name Min/max macros
//@{
#if !defined(MIN)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#if !defined(MAX)
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
//@}

//! @brief Computes the number of elements in an array.
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

//! @name Byte swap macros
//@{
#define BSWAP_16(x) (uint16_t)((((x)&0xFF00) >> 0x8) | (((x)&0xFF) << 0x8))
#define BSWAP_32(val) \
    (uint32_t)((BSWAP_16((uint32_t)(val) & (uint32_t)0xFFFF) << 0x10) | (BSWAP_16((uint32_t)((val) >> 0x10))))
//@}

//! @name Alignment macros
//@{
#ifndef ALIGN_DOWN
#define ALIGN_DOWN(x, a) ((x) & -(a))
#endif
#ifndef ALIGN_UP
#define ALIGN_UP(x, a) (-(-(x) & -(a)))
#endif
//@}

//! @brief Build a 32-bit code from four character values.
//!
//! The resulting value is built with a byte order such that the string
//! being readable in expected order when viewed in a hex editor, if the value
//! is treated as a 32-bit little endian value.
#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))

#if (defined(DEBUG) || defined(_DEBUG)) && !defined(DEBUG_PRINT_DISABLE)
static inline void debug_printf(const char *format, ...);

//! @brief Debug print utility.
//!
//! This print function will only output text when the @a DEBUG macro is defined.
static inline void debug_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
// Temporarily disable MISRA rule 14.2
#if defined(__ICCARM__)
#pragma diag_suppress = Pm049
#endif
    va_end(args);
#if defined(__ICCARM__)
#pragma diag_default = Pm049
#endif
}
#else // (DEBUG || _DEBUG) && !DEBUG_PRINT_DISABLE
// Empty macro to cause debug_printf() calls to disappear.
#define debug_printf(x, ...) \
    do                       \
    {                        \
    } while (false)
#endif // (DEBUG || _DEBUG) && !DEBUG_PRINT_DISABLE

//! @brief Structure of version property.
//!
//! @ingroup bl_core
typedef union StandardVersion
{
    struct
    {
        uint8_t bugfix; //!< bugfix version [7:0]
        uint8_t minor;  //!< minor version [15:8]
        uint8_t major;  //!< major version [23:16]
        char name;      //!< name [31:24]
    };
    uint32_t version; //!< combined version numbers

#if defined(__cplusplus)
    StandardVersion()
        : version(0)
    {
    }
    StandardVersion(uint32_t version)
        : version(version)
    {
    }
#endif
} standard_version_t;

#endif // __BOOTLOADER_COMMON_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
