/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SERIAL_NOR_EEPROM_H__
#define __SERIAL_NOR_EEPROM_H__

#include "fsl_common.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief Constructs the four character code for tag
#if !defined(FOUR_CHAR_CODE)
#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))
#endif

//! @brief Constructs the version number
#if !defined(MAKE_VERSION)
#define MAKE_VERSION(major, minor, bugfix) (((major) << 16) | ((minor) << 8) | (bugfix))
#endif

//! @brief Error codes of serial NOR/EEPROM driver
typedef enum _serial_nor_eeprom_status
{
    kStatus_SerialNorEepromAddressInvalid = MAKE_STATUS(kStatusGroup_SerialNorEeprom, 0),
    kStatus_SerialNorEepromTransferError = MAKE_STATUS(kStatusGroup_SerialNorEeprom, 1),
    kStatus_SerialNorEepromTypeInvalid = MAKE_STATUS(kStatusGroup_SerialNorEeprom, 2),
    kStatus_SerialNorEepromSizeInvalid = MAKE_STATUS(kStatusGroup_SerialNorEeprom, 3),
    kStatus_SerialNorEepromCommandInvalid = MAKE_STATUS(kStatusGroup_SerialNorEeprom, 4),
} serial_nor_eeprom_status_t;

//! @brief Serial NOR/EEPROM command codes.
enum _serial_nor_eeprom_command
{
    // Below commands are common for NOR/EEPROM
    kSerialNorEepromCmd_Invalid = 0x00U,
    kSerialNorEepromCmd_WriteStatus = 0x01U,        /*!< WRSR: Write Status Register*/
    kSerialNorEepromCmd_WriteMemory = 0x02U,        /*!< WRITE: Write Byte/Page Data to Memory Array with less than 4-byte address*/
    kSerialNorEepromCmd_ReadMemory = 0x03U,         /*!< READ: Read Data from Memory Array with less than 4-byte address*/
    kSerialNorEepromCmd_WriteDisable = 0x04U,       /*!< WRDI: Reset Write Enable Latch*/
    kSerialNorEepromCmd_ReadStatus = 0x05U,         /*!< RDSR: Read Status Register*/
    kSerialNorEepromCmd_WriteEnable = 0x06U,        /*!< WREN: Set Write Enable Latch*/

    // Below command is special for AT25M02
    kSerialNorEepromCmd_PageWriteMemory = 0x07U,                    /*!< WRITE: Write Page Data to Memory Array with less than 4-byte address*/
    // Below commands are special for NOR FLASH
    kSerialNorEepromCmd_WriteMemoryA32 = 0x12U,                     /*!< 4PP: Write Byte/Page Data to Memory Array with 4-byte address*/
    kSerialNorEepromCmd_ReadMemoryA32 = 0x13U,                      /*!< 4READ: Read Data from Memory Array with 4-byte address*/
    kSerialNorEepromCmd_EraseChipNor = 0x60U,                       /*!< CE */
    // Below commands are special for NOR FLASH and 25AA512/1024
    kSerialNorEepromCmd_ErasePage = 0x42U,                          /*!< PE */
    kSerialNorEepromCmd_EraseSector4KB = 0x20U,                     /*!< SE4KB */
    kSerialNorEepromCmd_EraseSector32KB = 0x52U,                    /*!< SE32KB */
    kSerialNorEepromCmd_EraseSector = 0xd8U,                        /*!< SE */
    kSerialNorEepromCmd_ErasePageA32 = kSerialNorEepromCmd_Invalid, /*!< 4PE */
    kSerialNorEepromCmd_EraseSector4KBA32 = 0x21U,                  /*!< 4SE4KB */
    kSerialNorEepromCmd_EraseSectorA32 = 0xdcU,                     /*!< 4SE */
    kSerialNorEepromCmd_EraseChip = 0xc7U                           /*!< CE */
};

//! @brief Serial NOR/EEPROM status register bits.
enum _serial_nor_eeprom_status_bit
{
    kSerialNorEepromStatus_WriteInProgress = 0x01U,
    kSerialNorEepromStatus_WriteEnableLatch = 0x02U
};

//! @brief Serial NOR/EEPROM property constants.
enum _serial_nor_eeprom_property
{
    kSerialNorEepromProperty_maxAddressLength = 32U,
    kSerialNorEepromProperty_SectorSize = 8192U,

    kSerialNorEepromProperty_uniqueMemorySize256B = 256U,
    kSerialNorEepromProperty_uniqueMemorySize512B = 512U,
    kSerialNorEepromProperty_uniqueAddressLength = 9U,
    kSerialNorEepromProperty_9thAddressBitMask = 0x08U,

    kSerialNorEepromProperty_ErasedValue = 0xFFU,
};

//! @brief Bus transfer options.
typedef enum _bus_transfer_option
{
    kBusTransferOption_Start = 0x00U,
    kBusTransferOption_DataIn = 0x01U,
    kBusTransferOption_DataOut = 0x02U,
    kBusTransferOption_End = 0x03U,
} bus_transfer_option_t;

//! @brief Bus data transfer API function type.
typedef status_t (*data_transfer_func_t)(uint8_t *, uint32_t, bus_transfer_option_t);

//! @brief Serial NOR/EEPROM memory type.
enum _serial_nor_eeprom_memory_type
{
    kSerialNorEepromMemoryType_NorFlash = 0x00u,
    kSerialNorEepromMemoryType_Eeprom = 0x01u,
    kSerialNorEepromMemoryType_Terminator = 0x02u
};

//! @brief Common serial NOR/EEPROM command set tag.
typedef enum _serial_nor_eeprom_command_tag
{
    kSerialNorEepromCmdTag_Invalid = 0x00U,
    kSerialNorEepromCmdTag_WriteStatus = 0x01U,
    kSerialNorEepromCmdTag_PageWriteMemory,
    kSerialNorEepromCmdTag_ReadMemory,
    kSerialNorEepromCmdTag_WriteDisable,
    kSerialNorEepromCmdTag_ReadStatus,
    kSerialNorEepromCmdTag_WriteEnable,
    kSerialNorEepromCmdTag_ErasePage,
    kSerialNorEepromCmdTag_EraseSector,
    kSerialNorEepromCmdTag_EraseChip
} serial_nor_eeprom_command_tag_t;

//! @brief Serial NOR/EEPROM command set structure
typedef struct __serial_nor_eeprom_command_set
{
    uint8_t writeStatusCommand;
    uint8_t pageWriteMemoryCommand;
    uint8_t readMemoryCommand;
    uint8_t writeDisableCommand;
    uint8_t readStatusCommand;
    uint8_t writeEnableCommand;
    uint8_t erasePageCommand;
    uint8_t eraseSectorCommand;
    uint8_t eraseChipCommand;
    uint8_t reserved[3];
} serial_nor_eeprom_command_set;

//! @brief Serial NOR/EEPROM Config block structure
typedef struct __serial_nor_eeprom_config
{
    uint8_t memoryType;                            //!< Determines the connected memory type
    uint8_t addressLengthInBits;                   //!< Nor/Eeprom address length
    uint8_t waitTime;                              //!< Wait time before read
    uint8_t reserved0;                             //!< Reserved0
    uint32_t memorySizeInBytes;                    //!< Nor/Eeprom memory size
    uint32_t pageSizeInBytes;                      //!< Nor/Eeprom page size
    uint32_t sectorSizeInBytes;                    //!< Nor/Eeprom sector size
    serial_nor_eeprom_command_set commandSet;      //!< Nor/Eeprom command set
} serial_nor_eeprom_config_t;

//! @brief Serial NOR/EEPROM context
typedef struct __serial_nor_eeprom_context
{
    serial_nor_eeprom_config_t memoryConfig;       //!< Memory config block
    data_transfer_func_t data_transfer_callback;   //!< Data transfer function for NOR/EEPROM
} serial_nor_eeprom_context_t;

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

//!@brief Initialize the connected serial NOR/EEPROM
status_t serial_nor_eeprom_init(serial_nor_eeprom_config_t *config, data_transfer_func_t func);

//!@brief Read the data from the connected serial NOR/EEPROM
status_t serial_nor_eeprom_read(uint32_t address, uint8_t *buffer, uint32_t lengthInBytes);

//!@brief Erase the connected serial NOR/EEPROM data
status_t serial_nor_eeprom_erase(uint32_t address, uint32_t lengthInBytes);

//!@brief Erase all the connected serial NOR/EEPROM data
status_t serial_nor_eeprom_erase_all(void);

//!@brief Write the data to the connected serial NOR/EEPROM
status_t serial_nor_eeprom_write(uint32_t address, uint8_t *buffer, uint32_t lengthInBytes);

#if defined(__cplusplus)
}
#endif

#endif /* #ifndef __SERIAL_NOR_EEPROM_H__ */

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
