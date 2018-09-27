/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "bootloader_common.h"
#include "bootloader/bootloader.h"
#include "memory/memory.h"
#include "spi_nor_eeprom_memory.h"
#include "crc/crc32.h"
#include "lpspi/fsl_lpspi.h"
#include "bootloader/bl_context.h"
#include "utilities/fsl_rtos_abstraction.h"
#include "utilities/fsl_assert.h"
#include "fusemap.h"
#include <string.h>

#if BL_FEATURE_GEN_KEYBLOB
#include "bootloader/bl_keyblob.h"
#endif // BL_FEATURE_GEN_KEYBLOB

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
//! @brief SPI NOR/EEPROM memory feature inforamation
//!
//! An instance is maintained in this file, will is used to keep key information for write and flush
//! operatations.
typedef struct _spi_nor_eeprom_context
{
    bool isConfigured;
    spi_nor_eeprom_page_program_info_t pageProgramInfo;
    data_transfer_func_t callback;
    spi_nor_eeprom_config_t config;
#if BL_FEATURE_GEN_KEYBLOB
    bool has_keyblob;
    uint32_t keyblob_offset;
#endif // BL_FEATURE_GEN_KEYBLOB
} spi_nor_eeprom_context_t;

#if BL_FEATURE_SPI_NOR_EEPROM_MODULE

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
static void lpspi_master_init(uint32_t instance, uint32_t moduleClk_Hz, uint32_t baudrate);
static status_t lpspi_data_transfer(uint8_t *buffer, uint32_t lengthInBytes, bus_transfer_option_t option);
static status_t bl_spi_nor_eeprom_init(void);
static void bl_spi_nor_eeprom_set_default_config_data(void);
static bool is_spi_nor_eeprom_configured(void);
static bool is_spi_nor_memory_erased(uint32_t address, uint32_t lengthInBytes);
static void spi_nor_eeprom_get_config(spi_nor_eeprom_config_t *config, spi_nor_eeprom_config_option_t *option);
#if BL_FEATURE_GEN_KEYBLOB
static status_t check_update_keyblob_info(void *config);
#endif // BL_FEATURE_GEN_KEYBLOB

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Spi NOR/EEPROM memory size option
uint32_t spi_nor_eeprom_memorySizeOption[16] = {
    kSpiMemorySize_512KBytes, kSpiMemorySize_1MBytes,   kSpiMemorySize_2MBytes,   kSpiMemorySize_4MBytes,
    kSpiMemorySize_8MBytes,   kSpiMemorySize_16MBytes,  kSpiMemorySize_32MBytes,  kSpiMemorySize_64MBytes,
    kSpiMemorySize_128MBytes, kSpiMemorySize_256MBytes, kSpiMemorySize_512MBytes, kSpiMemorySize_1GBytes,
    kSpiMemorySize_32KBytes,  kSpiMemorySize_64KBytes,  kSpiMemorySize_128KBytes, kSpiMemorySize_256KBytes
};

//! @brief Spi NOR/EEPROM sector size option
uint32_t spi_nor_eeprom_sectorSizeOption[7] = { kSpiMemorySectorSize_4KBytes,   kSpiMemorySectorSize_8KBytes,
                                                kSpiMemorySectorSize_32KBytes,  kSpiMemorySectorSize_64KBytes,
                                                kSpiMemorySectorSize_128KBytes, kSpiMemorySectorSize_256KBytes };

//! @brief Spi NOR/EEPROM page size option
uint32_t spi_nor_eeprom_pageSizeOption[6] = { kSpiMemoryPageSize_256Bytes,  kSpiMemoryPageSize_512Bytes,
                                              kSpiMemoryPageSize_1024Bytes, kSpiMemoryPageSize_32Bytes,
                                              kSpiMemoryPageSize_64Bytes,   kSpiMemoryPageSize_128Bytes };

//! @brief Interface to spi NOR/EEPROM memory operations
const external_memory_region_interface_t g_spiNorEepromMemoryInterface = {
    .init = spi_nor_eeprom_mem_init,
    .read = spi_nor_eeprom_mem_read,
    .write = spi_nor_eeprom_mem_write,
    .erase = spi_nor_eeprom_mem_erase,
    .config = spi_nor_eeprom_mem_config,
    .flush = spi_nor_eeprom_mem_flush,
    .finalize = NULL,
};

/* SPI NOR/EEPROM context */
static spi_nor_eeprom_context_t s_spiNorEepromContext = {
    .isConfigured = false, .callback = lpspi_data_transfer,
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
#if BL_FEATURE_GEN_KEYBLOB
status_t check_update_keyblob_info(void *config)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((config == NULL) || (s_spiNorEepromContext.isConfigured == false))
        {
            break;
        }

        // Try to read Key blob info based on config
        keyblob_info_t *keyblob_info = (keyblob_info_t *)config;
        if (keyblob_info->option.B.tag != kKeyBlobInfoOption_Tag)
        {
            break;
        }

        int32_t keyblob_info_type = keyblob_info->option.B.type;
        if ((keyblob_info_type != kKeyBlobInfoType_Program) && (keyblob_info_type != kKeyBlobInfoType_Update))
        {
            break;
        }

        if (keyblob_info_type == kKeyBlobInfoType_Update)
        {
            status = keyblob_update(keyblob_info);
            if (status != kStatus_Success)
            {
                s_spiNorEepromContext.has_keyblob = false;
                break;
            }
            s_spiNorEepromContext.keyblob_offset = keyblob_info->keyblob_offset;
            s_spiNorEepromContext.has_keyblob = true;
        }
        else if (keyblob_info_type == kKeyBlobInfoType_Program)
        {
            if (!s_spiNorEepromContext.has_keyblob)
            {
                break;
            }
            uint32_t index = keyblob_info->option.B.image_index;
            if (index != 0)
            {
                break;
            }

            uint32_t image_start = 0;
            uint32_t image_max_size = 0;
            uint32_t page_size;
            status = spi_nor_eeprom_get_property(kSpiNorEepromProperty_StartAddress, &image_start);
            if (status != kStatus_Success)
            {
                break;
            }
            status = spi_nor_eeprom_get_property(kSpiNorEepromProperty_TotalFlashSize, &image_max_size);
            if (status != kStatus_Success)
            {
                break;
            }
            status = spi_nor_eeprom_get_property(kSpiNorEepromProperty_PageSize, &page_size);
            if (status != kStatus_Success)
            {
                break;
            }

            uint32_t keyblob_offset = s_spiNorEepromContext.keyblob_offset;
            uint32_t keyblob_addr = image_start + keyblob_offset;
            uint8_t *keyblob_buffer;
            uint32_t keyblob_size;
            status = keyblob_get(&keyblob_buffer, &keyblob_size);
            if (status != kStatus_Success)
            {
                break;
            }

            // Check key blob address range
            if ((keyblob_size + keyblob_offset) > image_max_size)
            {
                break;
            }

            // Invalid key blob address, key blob must be page size aligned.
            if (keyblob_addr & (page_size - 1))
            {
                break;
            }

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
            if (!is_spi_nor_memory_erased(keyblob_addr, page_size))
            {
                status = kStatusMemoryCumulativeWrite;
                break;
            }
#endif
            status = spi_nor_eeprom_mem_write(keyblob_addr, keyblob_size, keyblob_buffer);
            if (status != kStatus_Success)
            {
                break;
            }

            status = spi_nor_eeprom_mem_flush();
            if (status != kStatus_Success)
            {
                break;
            }
        }
    } while (0);

    return status;
}
#endif // #if BL_FEATURE_GEN_KEYBLOB

// See spi_nor_eeprom_memory.h for documentation on this function.
status_t spi_nor_eeprom_mem_init(void)
{
    status_t status = kStatus_Success;

    s_spiNorEepromContext.pageProgramInfo.storedBytes = 0;
    return status;
}

// See spi_nor_eeprom_memory.h for documentation on this function.
status_t spi_nor_eeprom_mem_read(uint32_t address, uint32_t length, uint8_t *restrict buffer)
{
    if (!s_spiNorEepromContext.isConfigured)
    {
        return kStatusMemoryNotConfigured;
    }

    return serial_nor_eeprom_read(address, (uint8_t *)buffer, length);
}

// See spi_nor_eeprom_memory.h for documentation on this function.
status_t spi_nor_eeprom_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    status_t status = kStatus_Success;
    uint32_t pageProgramSize = s_spiNorEepromContext.config.memoryConfig.pageSizeInBytes;

    if (!s_spiNorEepromContext.isConfigured)
    {
        return kStatusMemoryNotConfigured;
    }

    if (pageProgramSize > kSpiNorEeprom_MaxPageSize)
    {
        pageProgramSize = kSpiNorEeprom_MaxPageSize;
    }

    while (length)
    {
        // Set start address when storing first byte into page program buffer
        if ((!s_spiNorEepromContext.pageProgramInfo.storedBytes) &&
            (!s_spiNorEepromContext.pageProgramInfo.startAddress))
        {
            s_spiNorEepromContext.pageProgramInfo.startAddress = address;
        }
        else
        {
            // Start page programming operation when meet discontinuous address
            if ((s_spiNorEepromContext.pageProgramInfo.startAddress +
                 s_spiNorEepromContext.pageProgramInfo.storedBytes) != address)
            {
                // flush cached data into target memory,
                status = spi_nor_eeprom_mem_flush();
                if (status != kStatus_Success)
                {
                    return status;
                }
                continue;
            }
        }

        uint32_t storeBytes;
        // Check to see if page program buffer will be filled with current data packet
        if ((s_spiNorEepromContext.pageProgramInfo.storedBytes + length) <= pageProgramSize)
        {
            storeBytes = length;
        }
        else
        {
            storeBytes = pageProgramSize - s_spiNorEepromContext.pageProgramInfo.storedBytes;
        }

        // Copy data to page program buffer
        if (buffer != &s_spiNorEepromContext.pageProgramInfo.buffer[s_spiNorEepromContext.pageProgramInfo.storedBytes])
        {
            memcpy(&s_spiNorEepromContext.pageProgramInfo.buffer[s_spiNorEepromContext.pageProgramInfo.storedBytes],
                   buffer, storeBytes);
        }

        s_spiNorEepromContext.pageProgramInfo.storedBytes += storeBytes;
        buffer += storeBytes;
        address += storeBytes;
        length -= storeBytes;

        // Start page programming operation when section program buffer is full
        if (s_spiNorEepromContext.pageProgramInfo.storedBytes == pageProgramSize)
        {
            // flush cached data into target memory,
            status = spi_nor_eeprom_mem_flush();
            if (status != kStatus_Success)
            {
                return status;
            }
        }
    }

    return kStatus_Success;
}

// See spi_nor_eeprom_memory.h for documentation on this function.
status_t spi_nor_eeprom_mem_flush(void)
{
    status_t status = kStatus_Success;

    if (s_spiNorEepromContext.pageProgramInfo.storedBytes)
    {
        uint32_t address = s_spiNorEepromContext.pageProgramInfo.startAddress;
        uint32_t length = s_spiNorEepromContext.pageProgramInfo.storedBytes;
        uint8_t readBuffer[kSpiNorEeprom_MaxPageSize];

        // Clear related states no matter following operations are executed successfully or not.
        s_spiNorEepromContext.pageProgramInfo.startAddress = 0;
        s_spiNorEepromContext.pageProgramInfo.storedBytes = 0;

        // Write data of aligned length to flash
        status = serial_nor_eeprom_write(address, s_spiNorEepromContext.pageProgramInfo.buffer, length);

        if (status != kStatus_Success)
        {
            return status;
        }

        // Verify whether the data has been programmed to Serial NOR/EEPROM flash successfully.
        status = serial_nor_eeprom_read(address, (uint8_t *)&readBuffer, length);
        if (status != kStatus_Success)
        {
            return status;
        }

        if (memcmp(s_spiNorEepromContext.pageProgramInfo.buffer, readBuffer, length))
        {
            return kStatusMemoryVerifyFailed;
        }
    }

    return status;
}

// See spi_nor_eeprom_memory.h for documentation on this function.
status_t spi_nor_eeprom_mem_erase(uint32_t address, uint32_t length)
{
    status_t status = kStatus_Success;

    if (!s_spiNorEepromContext.isConfigured)
    {
        return kStatusMemoryNotConfigured;
    }

    status = serial_nor_eeprom_erase(address, length);
    if (status != kStatus_Success)
    {
        return status;
    }
#if BL_FEATURE_SPI_NOR_EEPROM_MODULE_ERASE_VERIFY
    if (!is_spi_nor_memory_erased(address, length))
    {
        return kStatusMemoryVerifyFailed;
    }
#endif // BL_FEATURE_SPI_NOR_EEPROM_MODULE_ERASE_VERIFY
    return status;
}

// See spi_nor_eeprom_memory.h for documentation on this function.
status_t spi_nor_eeprom_mem_erase_all(void)
{
    status_t status = kStatus_Success;

    if (!s_spiNorEepromContext.isConfigured)
    {
        return kStatusMemoryNotConfigured;
    }

    status = serial_nor_eeprom_erase_all();
    if (status != kStatus_Success)
    {
        return status;
    }
#if BL_FEATURE_SPI_NOR_EEPROM_MODULE_ERASE_VERIFY
    if (!is_spi_nor_memory_erased(0, s_spiNorEepromContext.config.memoryConfig.memorySizeInBytes))
    {
        return kStatusMemoryVerifyFailed;
    }
#endif // BL_FEATURE_SPI_NOR_EEPROM_MODULE_ERASE_VERIFY
    return status;
}

static bool is_spi_nor_memory_erased(uint32_t address, uint32_t lengthInBytes)
{
    bool is_erased = true;
    uint32_t read_length;
    while (lengthInBytes)
    {
        uint32_t readBuffer[kSpiNorEeprom_MaxPageSize / sizeof(uint32_t)];
        read_length = lengthInBytes < sizeof(readBuffer) ? lengthInBytes : sizeof(readBuffer);
        serial_nor_eeprom_read(address, (uint8_t *)&readBuffer, read_length);
        uint32_t *buf_32 = &readBuffer[0];
        for (uint32_t i = 0; i < read_length / 4; i++)
        {
            if (*buf_32++ != 0xFFFFFFFFUL)
            {
                return false;
            }
        }
        lengthInBytes -= read_length;
        address += read_length;
    }

    return is_erased;
}

// See spi_nor_eeprom_memory.h for documentation on this function.
status_t spi_nor_eeprom_mem_config(uint32_t *config)
{
    status_t status = kStatus_InvalidArgument;

    bool isNorConfigOption = false;

    uint32_t startAddr = (uint32_t)config;
    uint32_t endAddr = startAddr + sizeof(spi_nor_eeprom_config_t) - 1;
    // Should check the config is in valid internal space.
    if ((!is_valid_application_location(startAddr)) || (!is_valid_application_location(endAddr)))
    {
        return kStatus_InvalidArgument;
    }

    // Try to check whether the 'config' variable is an option supported by spi_nor_get_config function
    spi_nor_eeprom_config_option_t *option = (spi_nor_eeprom_config_option_t *)startAddr;
    // Try to check whether the config variable stored at the specified addres is a whole config block
    spi_nor_eeprom_config_t *firmwareConfig = (spi_nor_eeprom_config_t *)config;
#if BL_FEATURE_GEN_KEYBLOB
    keyblob_info_t *keyblob_info = (keyblob_info_t *)config;
#endif // BL_FEATURE_GEN_KEYBLOB

    // Check tag.
    if (option->option0.B.tag == kSerialNorEepromCfgOption_Tag)
    {
        // Over-write config block.
        memset(&s_spiNorEepromContext.config, 0, sizeof(spi_nor_eeprom_config_t));
        /* Set default value for some common SPI config data */
        bl_spi_nor_eeprom_set_default_config_data();

        spi_nor_eeprom_get_config(&s_spiNorEepromContext.config, option);
        isNorConfigOption = true;
    }
#if BL_FEATURE_GEN_KEYBLOB
    else if (keyblob_info->option.B.tag == kKeyBlobInfoOption_Tag)
    {
        status = check_update_keyblob_info(config);
        if (status != kStatus_Success)
        {
            return status;
        }
        else
        {
            status = kStatus_Success;
        }
    }
#endif // BL_FEATURE_GEN_KEYBLOB
    else if (firmwareConfig->tag == kSpiNorEepromConfigTag)
    {
        // Over-write config block.
        memset(&s_spiNorEepromContext.config, 0, sizeof(spi_nor_eeprom_config_t));
        memcpy(&s_spiNorEepromContext.config, firmwareConfig, sizeof(spi_nor_eeprom_config_t));
        isNorConfigOption = true;
    }

    if (isNorConfigOption == true)
    {
        // First, mark SPI NOR/EEPROM as not configured.
        s_spiNorEepromContext.isConfigured = false;

        //  Re-Init SPI and Nor/Eeprom memory
        status = bl_spi_nor_eeprom_init();
        if (status == kStatus_Success)
        {
            s_spiNorEepromContext.isConfigured = true;
        }
    }

    return status;
}

static bool is_spi_nor_eeprom_configured(void)
{
    return s_spiNorEepromContext.isConfigured;
}

//! @brief Get Property from spi NOR/EEPROM driver
status_t spi_nor_eeprom_get_property(uint32_t whichProperty, uint32_t *value)
{
    if (value == NULL)
    {
        return kStatus_InvalidArgument;
    }

    switch (whichProperty)
    {
        case kSpiNorEepromProperty_InitStatus:
            *value = is_spi_nor_eeprom_configured() ? kStatus_Success : kStatusMemoryNotConfigured;
            break;
        case kSpiNorEepromProperty_StartAddress:
            *value = kSpiNorEeprom_StartAddress;
            break;
        case kSpiNorEepromProperty_PageSize:
            *value = s_spiNorEepromContext.config.memoryConfig.pageSizeInBytes;
            break;
        case kSpiNorEepromProperty_TotalFlashSizeInKBytes:
            *value = s_spiNorEepromContext.config.memoryConfig.memorySizeInBytes / 1024u;
            break;
        case kSpiNorEepromProperty_SectorSize:
            *value = s_spiNorEepromContext.config.memoryConfig.sectorSizeInBytes;
            break;
        case kSpiNorEepromProperty_TotalFlashSize:
            *value = s_spiNorEepromContext.config.memoryConfig.memorySizeInBytes;
            break;

        default: // catch inputs that are not recognized
            return kStatus_InvalidArgument;
    }

    return kStatus_Success;
}

static void lpspi_master_init(uint32_t instance, uint32_t moduleClk_Hz, uint32_t baudrate)
{
    lpspi_master_config_t masterConfig;
    uint32_t lpspiBaseAddr[] = LPSPI_BASE_ADDRS;

    memset(&masterConfig, 0, sizeof(lpspi_master_config_t));

    masterConfig.baudRate = baudrate;
    masterConfig.bitsPerFrame = 8;
    masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
    masterConfig.direction = kLPSPI_MsbFirst;

    masterConfig.pcsToSckDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;

    masterConfig.whichPcs = kLPSPI_Pcs2; /* To do */
    masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    masterConfig.pinCfg = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig = kLpspiDataOutRetained;

    LPSPI_MasterInit((LPSPI_Type *)lpspiBaseAddr[instance], &masterConfig, moduleClk_Hz);
}

static status_t lpspi_data_transfer(uint8_t *buffer, uint32_t lengthInBytes, bus_transfer_option_t option)
{
    status_t status = kStatus_LPSPI_Busy;
    uint8_t spiIndex = s_spiNorEepromContext.config.spiConfig.spiIndex;
    uint8_t spiPcsx = s_spiNorEepromContext.config.spiConfig.spiPcsx;
    lpspi_transfer_t masterXfer;
    uint32_t lpspiBaseAddr[] = LPSPI_BASE_ADDRS;

    memset(&masterXfer, 0, sizeof(lpspi_transfer_t));
    masterXfer.configFlags = kLPSPI_MasterPcsContinuous;

    if (option == kBusTransferOption_Start)
    {
        spi_pcs_pin_control(spiIndex, spiPcsx, kSpiNorEepromSelection_Connected);
        return kStatus_Success;
    }
    else if (option == kBusTransferOption_End)
    {
        spi_pcs_pin_control(spiIndex, spiPcsx, kSpiNorEepromSelection_Disconnected);
        return kStatus_Success;
    }
    else if (option == kBusTransferOption_DataIn)
    {
        masterXfer.rxData = buffer;
    }
    else if (option == kBusTransferOption_DataOut)
    {
        masterXfer.txData = buffer;
    }
    masterXfer.dataSize = lengthInBytes;

    while (status == kStatus_LPSPI_Busy)
    {
        status = LPSPI_MasterTransferBlocking((LPSPI_Type *)lpspiBaseAddr[spiIndex], &masterXfer);
    }
    if (status != kStatus_Success)
    {
        spi_pcs_pin_control(spiIndex, spiPcsx, kSpiNorEepromSelection_Disconnected);
        return kStatus_SerialNorEepromTransferError;
    }

    return kStatus_Success;
}

static void bl_spi_nor_eeprom_set_default_config_data(void)
{
    s_spiNorEepromContext.config.tag = kSpiNorEepromConfigTag;
    s_spiNorEepromContext.config.version = SPI_NOR_EEPROM_DRIVER_VERSION;

    // Set memoryConfig.commandSet
    // Note: sectorSizeInBytes, erasePageCommand, eraseSectorCommand won't be supported by default
    s_spiNorEepromContext.config.memoryConfig.commandSet.writeStatusCommand = kSerialNorEepromCmd_WriteStatus;
    s_spiNorEepromContext.config.memoryConfig.commandSet.pageWriteMemoryCommand = kSerialNorEepromCmd_WriteMemory;
    s_spiNorEepromContext.config.memoryConfig.commandSet.readMemoryCommand = kSerialNorEepromCmd_ReadMemory;
    s_spiNorEepromContext.config.memoryConfig.commandSet.writeDisableCommand = kSerialNorEepromCmd_WriteDisable;
    s_spiNorEepromContext.config.memoryConfig.commandSet.readStatusCommand = kSerialNorEepromCmd_ReadStatus;
    s_spiNorEepromContext.config.memoryConfig.commandSet.writeEnableCommand = kSerialNorEepromCmd_WriteEnable;
}

static void spi_nor_eeprom_get_config(spi_nor_eeprom_config_t *config, spi_nor_eeprom_config_option_t *option)
{
    config->memoryConfig.pageSizeInBytes = spi_nor_eeprom_pageSizeOption[option->option0.B.page_size];
    config->memoryConfig.sectorSizeInBytes = spi_nor_eeprom_sectorSizeOption[option->option0.B.sector_size];
    config->memoryConfig.memorySizeInBytes = spi_nor_eeprom_memorySizeOption[option->option0.B.memory_size];

    switch (option->option0.B.memory_type)
    {
        case kSerialNorEepromMemoryType_Eeprom:
            config->memoryConfig.memoryType = 1u;

            // Note: For spi eeprom flash, only the 2-bytes, 3-bytes and 4-bytes address memory are supported now.
            if (config->memoryConfig.memorySizeInBytes <= kSpiMemorySize_64KBytes)
            {
                config->memoryConfig.addressLengthInBits = 16u;
            }
            else if (config->memoryConfig.memorySizeInBytes <= kSpiMemorySize_16MBytes)
            {
                config->memoryConfig.addressLengthInBits = 24u;
            }
            else
            {
                config->memoryConfig.addressLengthInBits = 32u;
            }
            break;
        case kSerialNorEepromMemoryType_Terminator:
            config->memoryConfig.memoryType = 2u;
            break;
        case kSerialNorEepromMemoryType_NorFlash:
        default:
            config->memoryConfig.memoryType = 0u;

            // Note: For spi nor flash, only the 3-bytes and 4-bytes address memory are supported now.
            config->memoryConfig.commandSet.eraseChipCommand = kSerialNorEepromCmd_EraseChipNor;
            if (config->memoryConfig.memorySizeInBytes <= kSpiMemorySize_16MBytes)
            {
                config->memoryConfig.addressLengthInBits = 24u;

                config->memoryConfig.commandSet.erasePageCommand = kSerialNorEepromCmd_ErasePage;
                if (config->memoryConfig.sectorSizeInBytes >= kSpiMemorySectorSize_64KBytes)
                {
                    config->memoryConfig.commandSet.eraseSectorCommand = kSerialNorEepromCmd_EraseSector;
                }
                else if (config->memoryConfig.sectorSizeInBytes == kSpiMemorySectorSize_4KBytes)
                {
                    config->memoryConfig.commandSet.eraseSectorCommand = kSerialNorEepromCmd_EraseSector4KB;
                }
                else if (config->memoryConfig.sectorSizeInBytes == kSpiMemorySectorSize_32KBytes)
                {
                    config->memoryConfig.commandSet.eraseSectorCommand = kSerialNorEepromCmd_EraseSector32KB;
                }
            }
            else
            {
                config->memoryConfig.addressLengthInBits = 32u;

                // For 4-bytes addess memory, the read/write memory command may be different
                config->memoryConfig.commandSet.pageWriteMemoryCommand = kSerialNorEepromCmd_WriteMemoryA32;
                config->memoryConfig.commandSet.readMemoryCommand = kSerialNorEepromCmd_ReadMemoryA32;
                config->memoryConfig.commandSet.erasePageCommand = kSerialNorEepromCmd_ErasePageA32;
                if (config->memoryConfig.sectorSizeInBytes >= kSpiMemorySectorSize_64KBytes)
                {
                    config->memoryConfig.commandSet.eraseSectorCommand = kSerialNorEepromCmd_EraseSectorA32;
                }
                else if (config->memoryConfig.sectorSizeInBytes == kSpiMemorySectorSize_4KBytes)
                {
                    config->memoryConfig.commandSet.eraseSectorCommand = kSerialNorEepromCmd_EraseSector4KBA32;
                }
            }
            break;
    }

    switch (option->option0.B.spi_index)
    {
        case kSpiModuleInstance_Spi1:
            config->spiConfig.spiIndex = 1u;
            break;
        case kSpiModuleInstance_Spi2:
            config->spiConfig.spiIndex = 2u;
            break;
        case kSpiModuleInstance_Spi3:
            config->spiConfig.spiIndex = 3u;
            break;
        case kSpiModuleInstance_Spi4:
            config->spiConfig.spiIndex = 4u;
            break;
        case kSpiModuleInstance_Spi0:
        default:
            config->spiConfig.spiIndex = 0;
            break;
    }

    switch (option->option0.B.pcs_index)
    {
        case kSpiModuleInstance_Spipcs1:
            config->spiConfig.spiPcsx = 1u;
            break;
        case kSpiModuleInstance_Spipcs2:
            config->spiConfig.spiPcsx = 2u;
            break;
        case kSpiModuleInstance_Spipcs3:
            config->spiConfig.spiPcsx = 3u;
            break;
        case kSpiModuleInstance_Spipcs0:
        default:
            config->spiConfig.spiPcsx = 0;
            break;
    }

    if (option->option0.B.option_size == 0)
    {
        config->spiConfig.spiSpeed_Hz = kSpiNorEepromModuleClkValue_20MHz;
    }
    else
    {
        switch (option->option1.B.spi_speed)
        {
            case kSpiNorEepromModuleClk_10MHz:
                config->spiConfig.spiSpeed_Hz = kSpiNorEepromModuleClkValue_10MHz;
                break;
            case kSpiNorEepromModuleClk_5MHz:
                config->spiConfig.spiSpeed_Hz = kSpiNorEepromModuleClkValue_5MHz;
                break;
            case kSpiNorEepromModuleClk_2MHz:
                config->spiConfig.spiSpeed_Hz = kSpiNorEepromModuleClkValue_2MHz;
                break;
            case kSpiNorEepromModuleClk_20MHz:
            default:
                config->spiConfig.spiSpeed_Hz = kSpiNorEepromModuleClkValue_20MHz;
                break;
        }
    }
}

static status_t bl_spi_nor_eeprom_init(void)
{
    status_t status;

    uint8_t spiIndex = s_spiNorEepromContext.config.spiConfig.spiIndex;
    uint32_t spiSpeed_Hz = s_spiNorEepromContext.config.spiConfig.spiSpeed_Hz;

    /* Config SPI pin*/
    spi_iomux_config(&s_spiNorEepromContext.config.spiConfig);

    /* Enable SPI Clock Gate */
    spi_clock_gate_enable(spiIndex);

    /* Note: Make sure that SPI module clk freq is an even number of NOR/EEPROM clk (SPI baudrate),
     *  so the duty cycle of SPI SCK can be 50% */
    /* Configure SPI with a desired frequency */
    spi_clock_config(spiIndex, kSpiModuleClk_40MHz);

    /* Note: SPI baudrate (NOR/EEPROM speed) = SPI module clk / (n+2) */
    /* Config selected SPI module as master mode for data transfer*/
    lpspi_master_init(spiIndex, kSpiModuleClk_40MHz, spiSpeed_Hz);

    /* Initialize NOR/EEPROM connected via SPI*/
    status = serial_nor_eeprom_init(&s_spiNorEepromContext.config.memoryConfig, s_spiNorEepromContext.callback);

    return status;
}
#endif // #if BL_FEATURE_SPI_NOR_EEPROM_MODULE
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
