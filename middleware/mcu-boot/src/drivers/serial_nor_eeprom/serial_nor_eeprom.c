/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bootloader_common.h"
#include "lpspi/fsl_lpspi.h"
#include "serial_nor_eeprom/serial_nor_eeprom.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

//! @brief Write a single command byte to serial NOR/EEPROM.
static status_t serial_nor_eeprom_common_sequence(uint8_t command, uint32_t address, uint8_t *buffer, uint32_t lengthInBytes);
//! @brief Enable writes to serial NOR/EEPROM.
static status_t serial_nor_eeprom_enable_write(void);
//! @brief Check to see whether serial NOR/EEPROM device is busy.
static bool serial_nor_eeprom_is_busy(void);

static status_t serial_nor_eeprom_check_range(uint32_t address, uint32_t lengthInBytes);
static void serial_nor_eeprom_convert_command(uint8_t *command, uint32_t address);
static serial_nor_eeprom_command_tag_t serial_nor_eeprom_find_command_tag(uint8_t command);
static status_t serial_nor_eeprom_soft_erase(uint32_t address, uint32_t lengthInBytes);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

static serial_nor_eeprom_context_t s_serialNorEepromContext;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 *
 * serial_nor_eeprom_init - Init the Serial NOR/EEPROM
 *
 * This function initializes the Serial NOR/EEPROM.
 *
 */
status_t serial_nor_eeprom_init(serial_nor_eeprom_config_t *config, data_transfer_func_t func)
{
    // Cleanup context struct
    memset(&s_serialNorEepromContext, 0, sizeof(serial_nor_eeprom_context_t));

    // Check memory type
    if (config->memoryType >= kSerialNorEepromMemoryType_Terminator)
    {
        return kStatus_SerialNorEepromTypeInvalid;
    }

    // Check address length
    if (config->addressLengthInBits > kSerialNorEepromProperty_maxAddressLength)
    {
        return kStatus_SerialNorEepromSizeInvalid;
    }

    // Check memory size
    uint32_t maxMemorySize = (1 << config->addressLengthInBits);
    uint32_t minMemorySize = (1 << config->addressLengthInBits - 8U);
    if ((config->memorySizeInBytes > maxMemorySize)
        || (config->memorySizeInBytes < minMemorySize))
    {
        return kStatus_SerialNorEepromSizeInvalid;
    }

    // Over-write context.
    memcpy(&s_serialNorEepromContext, config, sizeof(serial_nor_eeprom_config_t));
    s_serialNorEepromContext.data_transfer_callback = func;

    // For EEPROM device, when the memory size is 512 bytes, the actual address byte length is still 8bit (bit7-0),
    //  besides it will take use of bit3 of command byte as bit8 of address
    if (s_serialNorEepromContext.memoryConfig.memorySizeInBytes == kSerialNorEepromProperty_uniqueMemorySize512B)
    {
        s_serialNorEepromContext.memoryConfig.addressLengthInBits = kSerialNorEepromProperty_uniqueAddressLength;
    }

    return kStatus_Success;
}

/******************************************************************************
 *
 * serial_nor_eeprom_read - Read from Serial NOR/EEPROM
 *
 * This function issues the Read command to Serial NOR/EEPROM and reads from the array.
 *
 */
status_t serial_nor_eeprom_read(uint32_t address, uint8_t *buffer, uint32_t lengthInBytes)
{
    status_t status = kStatus_Success;
    // Default read memory command is for less than 4bytes address memory
    uint8_t readMemoryCommand = s_serialNorEepromContext.memoryConfig.commandSet.readMemoryCommand;

    // Check the supplied address range.
    status = serial_nor_eeprom_check_range(address, lengthInBytes);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Check the command to see if it is valid
    if (readMemoryCommand == kSerialNorEepromCmd_Invalid)
    {
        return kStatus_SerialNorEepromCommandInvalid;
    }

    if (lengthInBytes != 0)
    {
        status = serial_nor_eeprom_common_sequence(readMemoryCommand, address, buffer, lengthInBytes);
    }

    return status;
}

/******************************************************************************
 *
 * serial_nor_eeprom_erase - Erase Serial NOR/EEPROM
 *
 * This function issues the Erase command to Serial NOR/EEPROM.
 *
 */
status_t serial_nor_eeprom_erase(uint32_t address, uint32_t lengthInBytes)
{
    status_t status = kStatus_Success;
    uint32_t endAddress = address + lengthInBytes;
    uint32_t eraseStepSize = s_serialNorEepromContext.memoryConfig.sectorSizeInBytes;
    uint8_t eraseCommand = s_serialNorEepromContext.memoryConfig.commandSet.eraseSectorCommand;

    // Check the supplied address range.
    status = serial_nor_eeprom_check_range(address, lengthInBytes);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Erase sector command is the first choice
    if (eraseCommand == kSerialNorEepromCmd_Invalid)
    {
        eraseCommand = s_serialNorEepromContext.memoryConfig.commandSet.erasePageCommand;
        eraseStepSize = s_serialNorEepromContext.memoryConfig.pageSizeInBytes;
    }

    // Check the command to see if it is valid
    if (eraseCommand == kSerialNorEepromCmd_Invalid)
    {
        if (s_serialNorEepromContext.memoryConfig.memoryType == kSerialNorEepromMemoryType_NorFlash)
        {
            return kStatus_SerialNorEepromCommandInvalid;
        }
        else if (s_serialNorEepromContext.memoryConfig.memoryType == kSerialNorEepromMemoryType_Eeprom)
        {
            status = serial_nor_eeprom_soft_erase(address, lengthInBytes);
            return status;
        }
    }

    while (address < endAddress)
    {
        // A WREN command is required prior to writing the PE/SE command.
        status = serial_nor_eeprom_enable_write();
        if (status != kStatus_Success)
        {
            return status;
        }

        status = serial_nor_eeprom_common_sequence(eraseCommand, address, 0, 0);
        if (address % eraseStepSize)
        {
            address = ((address / eraseStepSize) + 1) * eraseStepSize;
        }
        else
        {
            address += eraseStepSize;
        }

        if (status == kStatus_Success)
        {
            uint8_t readStatusCommand = s_serialNorEepromContext.memoryConfig.commandSet.readStatusCommand;
            // Check the command to see if it is valid
            if (readStatusCommand == kSerialNorEepromCmd_Invalid)
            {
                return kStatus_SerialNorEepromCommandInvalid;
            }
            // Wait until the operation is completed
            while(serial_nor_eeprom_is_busy());
        }
        else
        {
            break;
        }
    }

    return status;
}

/******************************************************************************
 *
 * serial_nor_eeprom_erase_all - Erase All Serial NOR/EEPROM
 *
 * This function issues the Erase command to Serial NOR/EEPROM.
 *
 */
status_t serial_nor_eeprom_erase_all(void)
{
    status_t status = kStatus_Success;
    uint32_t address = 0U;
    uint32_t lengthInBytes = s_serialNorEepromContext.memoryConfig.memorySizeInBytes;
    uint8_t eraseCommand = s_serialNorEepromContext.memoryConfig.commandSet.eraseChipCommand;

    // Check the command to see if it is valid
    if (eraseCommand == kSerialNorEepromCmd_Invalid)
    {
        if (s_serialNorEepromContext.memoryConfig.memoryType == kSerialNorEepromMemoryType_NorFlash)
        {
            return kStatus_SerialNorEepromCommandInvalid;
        }
        else if (s_serialNorEepromContext.memoryConfig.memoryType == kSerialNorEepromMemoryType_Eeprom)
        {
            status = serial_nor_eeprom_soft_erase(address, lengthInBytes);
            return status;
        }
    }

    // A WREN command is required prior to writing the CE command.
    status = serial_nor_eeprom_enable_write();
    if (status != kStatus_Success)
    {
        return status;
    }
    // Send chip erase command.
    status = serial_nor_eeprom_common_sequence(eraseCommand, 0, 0, 0);

    if (status == kStatus_Success)
    {
        uint8_t readStatusCommand = s_serialNorEepromContext.memoryConfig.commandSet.readStatusCommand;
        // Check the command to see if it is valid
        if (readStatusCommand == kSerialNorEepromCmd_Invalid)
        {
            return kStatus_SerialNorEepromCommandInvalid;
        }
        // Wait until the operation is completed
        while(serial_nor_eeprom_is_busy());
    }

    return status;
}

/******************************************************************************
 *
 * serial_nor_eeprom_write - Write to Serial NOR/EEPROM
 *
 * This function issues the Write command to Serial NOR/EEPROM and writes to the array.
 *
 */
status_t serial_nor_eeprom_write(uint32_t address, uint8_t *buffer, uint32_t lengthInBytes)
{
    status_t status = kStatus_Success;
    uint32_t pageSize = s_serialNorEepromContext.memoryConfig.pageSizeInBytes;
    // Default write memory command is for less than 4bytes address memory
    uint8_t writeMemoryCommand = s_serialNorEepromContext.memoryConfig.commandSet.pageWriteMemoryCommand;

    // Check the supplied address range.
    status = serial_nor_eeprom_check_range(address, lengthInBytes);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Check the command to see if it is valid
    if (writeMemoryCommand == kSerialNorEepromCmd_Invalid)
    {
        return kStatus_SerialNorEepromCommandInvalid;
    }

    while (lengthInBytes)
    {
        uint32_t programSize;

        // A WREN command is required prior to writing the WRITE command.
        status = serial_nor_eeprom_enable_write();
        if (status != kStatus_Success)
        {
            return status;
        }

        // Note: all transmitted data that goes beyond the end of the currently selected page
        //    are programmed from the starting address of the same page
        if (address % pageSize)
        {
            programSize = pageSize - (address % pageSize);
        }
        else
        {
            programSize = pageSize;
        }
        if (lengthInBytes < programSize)
        {
            programSize = lengthInBytes;
        }

        status = serial_nor_eeprom_common_sequence(writeMemoryCommand, address, buffer, programSize);

        address += programSize;
        buffer += programSize;
        lengthInBytes -= programSize;

        if (status == kStatus_Success)
        {
            uint8_t readStatusCommand = s_serialNorEepromContext.memoryConfig.commandSet.readStatusCommand;
            // Check the command to see if it is valid
            if (readStatusCommand == kSerialNorEepromCmd_Invalid)
            {
                return kStatus_SerialNorEepromCommandInvalid;
            }
            // Wait until the operation is completed
            while(serial_nor_eeprom_is_busy());
        }
        else
        {
            break;
        }
    }

    return status;
}

// ***************************************************************************
//  serial_nor_eeprom_common_sequence - HAL common function
//
//          command        write a single command byte to eeprom
//          address        system address to be used
//          buffer         Pointer to the data buffer where to store the read/write data
//          lengthInBytes  number of bytes to be read/write
// ***************************************************************************
static status_t serial_nor_eeprom_common_sequence(uint8_t command, uint32_t address, uint8_t *buffer, uint32_t lengthInBytes)
{
    status_t status = kStatus_Success;
    uint8_t addressLength = s_serialNorEepromContext.memoryConfig.addressLengthInBits;
    uint8_t commandAddressBuffer[5];
    uint32_t dataBytesInBuffer;

    // Select the Serial NOR/EEPROM device
    s_serialNorEepromContext.data_transfer_callback(0, 0, kBusTransferOption_Start);

    // Find command tag
    serial_nor_eeprom_command_tag_t commandTag = serial_nor_eeprom_find_command_tag(command);

    // Convert the command (special EEPROM property)
    serial_nor_eeprom_convert_command(&command, address);
    if (addressLength == kSerialNorEepromProperty_uniqueAddressLength)
    {
        addressLength = ALIGN_DOWN(addressLength, 8U);
    }

    // Prepare the command
    commandAddressBuffer[0] = command;
    dataBytesInBuffer = 1;

    // Prepare the address if needed
    switch (commandTag)
    {
        case kSerialNorEepromCmdTag_ReadMemory:
        case kSerialNorEepromCmdTag_PageWriteMemory:
        case kSerialNorEepromCmdTag_ErasePage:
        case kSerialNorEepromCmdTag_EraseSector:
        {
            uint32_t addressBytes = ((addressLength - 1) / 8) + 1;
            // Note: address bits should be MSB
            for(uint8_t i = 0; i < addressBytes; i++)
            {
                uint8_t addressValue = (uint8_t)((address >> (8 * (addressBytes - 1 - i))) & 0x000000FF);
                commandAddressBuffer[dataBytesInBuffer + i] = addressValue;
            }
            dataBytesInBuffer += addressBytes;

            break;
        }
        default: break;
    }

    // Write the command and address to Serial NOR/EEPROM device
    if (dataBytesInBuffer != 0)
    {
        status = s_serialNorEepromContext.data_transfer_callback(commandAddressBuffer, dataBytesInBuffer, kBusTransferOption_DataOut);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    // Read/Write the data from/to Serial NOR/EEPROM device
    if (lengthInBytes != 0)
    {
        bus_transfer_option_t option = kBusTransferOption_DataIn;
        switch (commandTag)
        {
            case kSerialNorEepromCmdTag_ReadMemory:
            case kSerialNorEepromCmdTag_ReadStatus:
            {
                option = kBusTransferOption_DataIn;
                break;
            }
            case kSerialNorEepromCmdTag_PageWriteMemory:
            case kSerialNorEepromCmdTag_WriteStatus:
            {
                option = kBusTransferOption_DataOut;
                break;
            }
            default: break;
        }
        status = s_serialNorEepromContext.data_transfer_callback(buffer, lengthInBytes, option);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    // Disselect the Serial NOR/EEPROM device
    s_serialNorEepromContext.data_transfer_callback(0, 0, kBusTransferOption_End);

    return kStatus_Success;
}

static status_t serial_nor_eeprom_enable_write(void)
{
    status_t status;
    uint8_t statusRegister;

    uint8_t readStatusCommand = s_serialNorEepromContext.memoryConfig.commandSet.readStatusCommand;
    uint8_t writeEnableCommand = s_serialNorEepromContext.memoryConfig.commandSet.writeEnableCommand;

    // Check the command to see if it is valid
    if ((readStatusCommand == kSerialNorEepromCmd_Invalid) || (writeEnableCommand == kSerialNorEepromCmd_Invalid))
    {
        return kStatus_SerialNorEepromCommandInvalid;
    }

    do
    {
        do
        {
            status = serial_nor_eeprom_common_sequence(readStatusCommand, 0, &statusRegister, 1);
        }
        while (status != kStatus_Success);
        if (statusRegister & kSerialNorEepromStatus_WriteEnableLatch)
        {
            break;
        }
        else
        {
            serial_nor_eeprom_common_sequence(writeEnableCommand, 0, 0, 0);
        }
    }
    while (!(statusRegister & kSerialNorEepromStatus_WriteEnableLatch));

    return kStatus_Success;
}

static bool serial_nor_eeprom_is_busy(void)
{
     status_t status;
     uint8_t statusRegister;

     uint8_t readStatusCommand = s_serialNorEepromContext.memoryConfig.commandSet.readStatusCommand;

     do
     {
         status = serial_nor_eeprom_common_sequence(readStatusCommand, 0, &statusRegister, 1);
     }
     while (status != kStatus_Success);

     return (statusRegister & kSerialNorEepromStatus_WriteInProgress);
}

static status_t serial_nor_eeprom_check_range(uint32_t address, uint32_t lengthInBytes)
{
    uint32_t endAddress = address + lengthInBytes;
    if (endAddress > s_serialNorEepromContext.memoryConfig.memorySizeInBytes)
    {
        return kStatus_SerialNorEepromAddressInvalid;
    }

    return kStatus_Success;
}

static void serial_nor_eeprom_convert_command(uint8_t *command, uint32_t address)
{
    if ((s_serialNorEepromContext.memoryConfig.addressLengthInBits == kSerialNorEepromProperty_uniqueAddressLength)
        && (address >= kSerialNorEepromProperty_uniqueMemorySize256B))
    {
        *command |= kSerialNorEepromProperty_9thAddressBitMask;
    }
}

static serial_nor_eeprom_command_tag_t serial_nor_eeprom_find_command_tag(uint8_t command)
{
    serial_nor_eeprom_command_set *commandSet = &s_serialNorEepromContext.memoryConfig.commandSet;

    if (command == kSerialNorEepromCmd_Invalid)
    {
        return kSerialNorEepromCmdTag_Invalid;
    }

    if (command == commandSet->writeStatusCommand)
    {
        return kSerialNorEepromCmdTag_WriteStatus;
    }
    if (command == commandSet->pageWriteMemoryCommand)
    {
        return kSerialNorEepromCmdTag_PageWriteMemory;
    }
    if (command == commandSet->readMemoryCommand)
    {
        return kSerialNorEepromCmdTag_ReadMemory;
    }
    if (command == commandSet->writeDisableCommand)
    {
        return kSerialNorEepromCmdTag_WriteDisable;
    }
    if (command == commandSet->readStatusCommand)
    {
        return kSerialNorEepromCmdTag_ReadStatus;
    }
    if (command == commandSet->writeEnableCommand)
    {
        return kSerialNorEepromCmdTag_WriteEnable;
    }
    if (command == commandSet->erasePageCommand)
    {
        return kSerialNorEepromCmdTag_ErasePage;
    }
    if (command == commandSet->eraseSectorCommand)
    {
        return kSerialNorEepromCmdTag_EraseSector;
    }
    if (command == commandSet->eraseChipCommand)
    {
        return kSerialNorEepromCmdTag_EraseChip;
    }

    return kSerialNorEepromCmdTag_Invalid;
}

static status_t serial_nor_eeprom_soft_erase(uint32_t address, uint32_t lengthInBytes)
{
    status_t status = kStatus_Success;
    uint8_t blankBuffer[16];

    memset(blankBuffer, kSerialNorEepromProperty_ErasedValue, sizeof(blankBuffer));

    while (lengthInBytes)
    {
        uint32_t erasedBytes = sizeof(blankBuffer);
        if (lengthInBytes < sizeof(blankBuffer))
        {
            erasedBytes = lengthInBytes;
        }

        status = serial_nor_eeprom_write(address, blankBuffer, erasedBytes);
        if (status != kStatus_Success)
        {
            return status;
        }
        address += erasedBytes;
        lengthInBytes -= erasedBytes;
    }

    return kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
