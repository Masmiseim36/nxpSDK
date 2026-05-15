/*
 * Copyright 2022 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __BOOTLOADER_CONFIG_H__
#define __BOOTLOADER_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////
// Bootloader Configuration Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Bootloader Mode Configurations
////////////////////////////////////////////////////////////////////////////////
// Determines whether to support isp boot via peripherals.
#define BL_FEATURE_ISP_BOOT (1)

////////////////////////////////////////////////////////////////////////////////
// Bootloader Peripheral Configurations
////////////////////////////////////////////////////////////////////////////////
// UART ports
#define BL_FEATURE_ROM_UART_PORT (1)

#if !defined(BL_CONFIG_LPUART_1)
#define BL_CONFIG_LPUART_1 (BL_FEATURE_ROM_UART_PORT)
#define BL_FEATURE_UART_RX_PULLUP (1)
#endif

#define BL_CONFIG_LPUART (BL_CONFIG_LPUART_1)

#define BL_FEATURE_UART_AUTOBAUD_IRQ (1)

// SPI ports
#define BL_FEATURE_ROM_SPI_PORT (1)

#if !defined(BL_CONFIG_LPSPI_1)
#define BL_CONFIG_LPSPI_1 (BL_FEATURE_ROM_SPI_PORT)
#endif

#define BL_CONFIG_LPSPI (BL_CONFIG_LPSPI_1)

// USB HS port
#define BL_CONFIG_HS_USB_HID (1U) // i.MX RT Series only supoort HS USB

////////////////////////////////////////////////////////////////////////////////
// Bootloader Feature Configurations
////////////////////////////////////////////////////////////////////////////////
#if !defined(BL_TARGET_FLASH) && !defined(BL_TARGET_RAM)
#define BL_TARGET_FLASH (0)
#endif

#define BL_FEATURE_MIN_PROFILE (0)

#if !defined(BL_TARGET_RAM)
#define BL_FEATURE_CRC_CHECK (1)
#endif

// Bootloader peripheral detection default timeout in milliseconds
// After coming out of reset the bootloader will spin in a peripheral detection
// loop for this amount of time. A zero value means no time out.
#if DEBUG
#define BL_DEFAULT_PERIPHERAL_DETECT_TIMEOUT 0
#else
#define BL_DEFAULT_PERIPHERAL_DETECT_TIMEOUT 5000
#endif // DEBUG

#define BL_FEATURE_LIFECYCLE_UPDATE (1)

////////////////////////////////////////////////////////////////////////////////
// Memory Module Common Configurations
////////////////////////////////////////////////////////////////////////////////
#define BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE (1)

////////////////////////////////////////////////////////////////////////////////
// Internal Memory Module Configurations
////////////////////////////////////////////////////////////////////////////////
// Internal Flash features
#define BL_FEATURE_HAS_NO_INTERNAL_FLASH (1)
#define BL_FEATURE_HAS_INTERNAL_FLASH (0)

#define BL_FEATURE_EFUSE_MODULE (1)
#define BL_FEATURE_EDGELOCK_MODULE (1)

////////////////////////////////////////////////////////////////////////////////
// Keyblob generation
////////////////////////////////////////////////////////////////////////////////
#define BL_FEATURE_GEN_KEYBLOB (1)

#define BL_FEATURE_KEYBLOB_DEK_KEY_DATA_SIZE (0x28)
#define BL_FEATURE_KEYBLOB_DEK_BLOB_EXPORT_SIZE (0x60)

#define BL_FEATURE_KEYBLOB_OTFAD_KEY_NUM (4)
#define BL_FEATURE_KEYBLOB_OTFAD_KEY_DATA_SIZE (48)
#define BL_FEATURE_KEYBLOB_OTFAD_BLOB_EXPORT_SIZE (64)

#define BL_FEATURE_KEYBLOB_IEE_KEY_DATA_SIZE (0x68)
#define BL_FEATURE_KEYBLOB_IEE_BLOB_EXPORT_SIZE (0x98)

#define BL_FEATURE_KEYBLOB_DATA_KEY_MAX_SIZE (BL_FEATURE_KEYBLOB_OTFAD_KEY_DATA_SIZE * BL_FEATURE_KEYBLOB_OTFAD_KEY_NUM)
#define BL_FEATURE_KEYBLOB_BLOB_MAX_SIZE (BL_FEATURE_KEYBLOB_OTFAD_BLOB_EXPORT_SIZE * BL_FEATURE_KEYBLOB_OTFAD_KEY_NUM)

////////////////////////////////////////////////////////////////////////////////
// XIP External Memory Module Configurations
////////////////////////////////////////////////////////////////////////////////
#define BL_FEATURE_FLEXSPI_NOR_MODULE (1)

#if BL_FEATURE_FLEXSPI_NOR_MODULE
#define BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL (1)
#define BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE (1)
//#define BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE (2)
#define BL_FEATURE_FLEXSPI_NOR_MODULE_ERASE_VERIFY (1)
#define BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING (0)
#define BL_FLASH_CFG_BLOCK_OFFSET (0x400u)
#define BL_PROT_REGION_BLOCK_OFFSET(i) (BL_FLASH_CFG_BLOCK_OFFSET * (i + 1))
#define BL_FLEXSPI_AMBA_BASE FlexSPI1_AMBA_BASE
//#define BL_FLEXSPI_AMBA_BASE FlexSPI2_AMBA_BASE
#define BL_FEATURE_FLEXSPI_ALIAS_AREA (1)
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE
#define FLEXSPI_NOR_FEATURE_ENABLE_PROGRAMMING (BL_FEATURE_MEM_WRITE_ENABLE)

#define BL_FEATURE_SEMC_NOR_MODULE (0)

////////////////////////////////////////////////////////////////////////////////
// Non-XIP External Memory Module Configurations
////////////////////////////////////////////////////////////////////////////////
#define BL_FEATURE_EXPAND_MEMORY (1)

#define BL_FEATURE_SPINAND_MODULE (1)

#if BL_FEATURE_SPINAND_MODULE
#if (!BL_FEATURE_EXPAND_MEMORY) && (BL_FEATURE_SPINAND_MODULE)
#error "BL_FEATURE_EXPAND_MEMORY" must be enabled to enable the SPI NAND feature.
#endif
#define BL_FEATURE_SPINAND_MODULE_ERASE_VERIFY (1)
#define BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI (1)
#define BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL (1)
#define BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE (1)
#define BL_FEATURE_SPINAND_MODULE_MAX_PERIPHERAL_INSTANCE (2)
#endif // BL_FEATURE_SPINAND_MODULE

#define BL_FEATURE_MMC_MODULE (1)

#if BL_FEATURE_MMC_MODULE
#define BL_FEATURE_MMC_MODULE_ERASE_VERIFY (1)
#define BL_FEATURE_MMC_MODULE_ENABLE_PERMANENT_CONFIG (0)
#endif // BL_FEATURE_MMC_MODULE

#define BL_FEATURE_SD_MODULE (1)

#if BL_FEATURE_SD_MODULE
#define BL_FEATURE_SD_MODULE_ERASE_VERIFY (1)
#endif // BL_FEATURE_SD_MODULE

#define BL_FEATURE_SEMC_NAND_MODULE (1)

#if BL_FEATURE_EXPAND_MEMORY
#define BL_FEATURE_EXTERNAL_MEMORY_PROPERTY (1)
#endif // BL_FEATURE_EXPAND_MEMORY

#define BL_FEATURE_SPI_NOR_EEPROM_MODULE (1)
#define BL_FEATURE_SPI_PERIPHERAL_INSTANCE (1)
#if BL_FEATURE_SPI_NOR_EEPROM_MODULE
#define BL_FEATURE_SPI_NOR_EEPROM_MODULE_ERASE_VERIFY (1)
#endif // BL_FEATURE_SPI_NOR_EEPROM_MODULE

////////////////////////////////////////////////////////////////////////////////
// Protocal Configurations
////////////////////////////////////////////////////////////////////////////////
#define BL_FEATURE_EXPAND_PACKET_SIZE (1)

#define BL_EXPANDED_FRAMING_PACKET_SIZE (512)

// Make sure that BL_EXPANDED_USB_HID_PACKET_SIZE < 1020
#define BL_EXPANDED_USB_HID_PACKET_SIZE (1012)

// Make sure that BL_EXPAND_SDIO_SLAVE_PACKET_SIZE + sizeof(framing_data_packet_t)(equals to 6) is power of 2.
#define BL_EXPAND_SDIO_SLAVE_PACKET_SIZE (1018)

////////////////////////////////////////////////////////////////////////////////
// Un-Categoried Configurations
////////////////////////////////////////////////////////////////////////////////
#define BL_FETAURE_USE_STD_EXCEPTION_HANDLER (0)

////////////////////////////////////////////////////////////////////////////////
// Not Support Commands
////////////////////////////////////////////////////////////////////////////////
#define BL_FETAURE_RECV_SB (0)

#endif // __BOOTLOADER_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
