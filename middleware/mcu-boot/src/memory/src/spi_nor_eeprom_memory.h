/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SPI_NOR_EEPROM_MEMORY_H__
#define __SPI_NOR_EEPROM_MEMORY_H__

#include "memory/memory.h"
#include "serial_nor_eeprom/serial_nor_eeprom.h"

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

//! @brief Spi NOR/EEPROM driver config option tag
enum _spi_nor_eeprom_config_option_tags
{
    kSerialNorEepromCfgOption_Tag = 0x0c,
};

//! @brief Spi NOR/EEPROM driver config block tag
enum __spi_nor_eeprom_config_block_tags
{
    //! @brief Tag value used to validate the Serial NOR/EEPROM config block.
    kSpiNorEepromConfigTag = FOUR_CHAR_CODE('k', 's', 'e', 'c'),
};

//! @brief Spi NOR/EEPROM driver version
#define SPI_NOR_EEPROM_DRIVER_VERSION (MAKE_VERSION(1, 0, 0)) /*!< Version 1.0.0. */

//! @brief Spi NOR/EEPROM module properties
typedef enum _spi_nor_eeprom_property
{
    kSpiNorEepromProperty_InitStatus = 0,
    kSpiNorEepromProperty_StartAddress = 1,           //!< Tag used to retrieve start address
    kSpiNorEepromProperty_TotalFlashSizeInKBytes = 2, //!< Tag used to retrieve total flash size in terms of KByte
    kSpiNorEepromProperty_PageSize = 3,               //!< Tag used to retreive page size in terms of byte
    kSpiNorEepromProperty_SectorSize = 4,             //!< Tag used to retrieve sector size in term of byte
    kSpiNorEepromProperty_BlockSize = 5,              //!< Tag used to retrieve block size in terms of byte

    kSpiNorEepromProperty_TotalFlashSize = 0x10, //!< Tag used to retrieve total flash size in terms of byte
} spi_nor_eeprom_property_t;

//! @brief SPI module clock freq
typedef enum __spi_module_clock_freq
{
    kSpiModuleClk_60MHz = 60000000u,
    kSpiModuleClk_40MHz = 40000000u
} spi_module_clock_freq_t;

//! @brief SPI module instance
enum __spi_module_instance
{
    kSpiModuleInstance_Spi0 = 0u,
    kSpiModuleInstance_Spi1 = 1u,
    kSpiModuleInstance_Spi2 = 2u,
    kSpiModuleInstance_Spi3 = 3u,
    kSpiModuleInstance_Spi4 = 4u
};

//! @brief SPI module instance
enum __spi_module_pcs_instance
{
    kSpiModuleInstance_Spipcs0 = 0u,
    kSpiModuleInstance_Spipcs1 = 1u,
    kSpiModuleInstance_Spipcs2 = 2u,
    kSpiModuleInstance_Spipcs3 = 3u
};

//! @brief Spi NOR/EEPROM module address length
enum _spi_nor_eeprom_module_address_length
{
    kSpiNorEepromModuleAddr_24bits = 0u,
    kSpiNorEepromModuleAddr_8bits = 1u,
    kSpiNorEepromModuleAddr_16bits = 2u,
    kSpiNorEepromModuleAddr_32bits = 3u
};

//! @brief Spi NOR/EEPROM module clock freq
enum _spi_nor_eeprom_module_clock_freq
{
    kSpiNorEepromModuleClk_20MHz = 0u,
    kSpiNorEepromModuleClk_10MHz = 1u,
    kSpiNorEepromModuleClk_5MHz = 2u,
    kSpiNorEepromModuleClk_2MHz = 3u
};

//! @brief Spi NOR/EEPROM module clock freq value
enum _spi_nor_eeprom_module_clock_freq_value
{
    kSpiNorEepromModuleClkValue_20MHz = 20000000u,
    kSpiNorEepromModuleClkValue_10MHz = 10000000u,
    kSpiNorEepromModuleClkValue_5MHz = 5000000u,
    kSpiNorEepromModuleClkValue_2MHz = 2000000u
};

//! @brief Spi NOR/EEPROM module page size
enum _spi_nor_eeprom_module_page_size_bytes
{
    kSpiMemoryPageSize_256Bytes = 256u,
    kSpiMemoryPageSize_512Bytes = 512u,
    kSpiMemoryPageSize_1024Bytes = 1024u,
    kSpiMemoryPageSize_32Bytes = 32u,
    kSpiMemoryPageSize_64Bytes = 64u,
    kSpiMemoryPageSize_128Bytes = 128u
};

//! @brief Spi NOR/EEPROM module sector size
enum _spi_nor_eeprom_module_sector_size_bytes
{
    kSpiMemorySectorSize_4KBytes = 0x1000u,
    kSpiMemorySectorSize_8KBytes = 0x2000u,
    kSpiMemorySectorSize_32KBytes = 0x8000u,
    kSpiMemorySectorSize_64KBytes = 0x10000u,
    kSpiMemorySectorSize_128KBytes = 0x20000u,
    kSpiMemorySectorSize_256KBytes = 0x40000u
};

//! @brief Spi NOR/EEPROM module memory size
enum _spi_nor_eeprom_module_memory_size_bytes
{
    kSpiMemorySize_512KBytes = 0x80000u,
    kSpiMemorySize_1MBytes = 0x100000u,
    kSpiMemorySize_2MBytes = 0x200000u,
    kSpiMemorySize_4MBytes = 0x400000u,
    kSpiMemorySize_8MBytes = 0x800000u,
    kSpiMemorySize_16MBytes = 0x1000000u,
    kSpiMemorySize_32MBytes = 0x2000000u,
    kSpiMemorySize_64MBytes = 0x4000000u,
    kSpiMemorySize_128MBytes = 0x8000000u,
    kSpiMemorySize_256MBytes = 0x10000000u,
    kSpiMemorySize_512MBytes = 0x20000000u,
    kSpiMemorySize_1GBytes = 0x40000000u,
    kSpiMemorySize_32KBytes = 0x8000u,
    kSpiMemorySize_64KBytes = 0x10000u,
    kSpiMemorySize_128KBytes = 0x20000u,
    kSpiMemorySize_256KBytes = 0x40000u
};

//! @brief Spi NOR/EEPROM module timing constants
enum __spi_nor_eeprom_module_timing_constants
{
    // Atmel AT250x0, 250
    // Atmel AT25512, 100/200
    // Atmel AT25M02, 200
    // Microchip, 25AAxxx, 25/50/100/150/200/250
    // ST, M950x0, 15/25/30/60
    // ST, M95512, 30/60/90/200
    // ST, M95M02, 60
    // Onsemi, CAT250x0, 15/30
    // Onsemi, CAT25MO2, 30/60
    kSpiEepromModuleTiming_MinPcsSetupHold_ns = 250u,

    // Atmel AT250x0, 250
    // Atmel AT25512, 100/200
    // Atmel AT25M02, 200
    // Microchip, 25AAxxx, 50
    // ST, M950x0, 20/40/90/100
    // ST, M95512, 40/100/200
    // ST, M95M02, 90
    // Onsemi, CAT250x0, 70/140
    // Onsemi, CAT25MO2, 40/80
    kSpiEepromModuleTiming_MinPcsHigh_ns = 300u

};

//! @brief Spi NOR/EEPROM selection constants.
enum _spi_nor_eeprom_selection_code
{
    kSpiNorEepromSelection_Connected = true,
    kSpiNorEepromSelection_Disconnected = false
};

//! @brief Spi NOR/EEPROM configuration option
typedef struct _spi_nor_eeprom_config_option
{
    union
    {
        struct
        {
            uint32_t page_size : 4;   //!< Nor/Eeprom page size
            uint32_t sector_size : 4; //!< Nor/Eeprom sector size
            uint32_t memory_size : 4; //!< Nor/Eeprom memory size
            uint32_t memory_type : 4; //!< Determines the connected memory type
            uint32_t pcs_index : 4;   //!< PCS instance of SPI module
            uint32_t spi_index : 4;   //!< Index of SPI module
            uint32_t option_size : 4; //!< Option size, in terms of uint32_t, size = (option_size + 1) * 4
            uint32_t tag : 4;         //!< Tag, must be 0x0c
        } B;
        uint32_t U;
    } option0;

    union
    {
        struct
        {
            uint32_t spi_speed : 4; //!< SPI transfer speed to connected NOR/EEPROM
            uint32_t reserved : 28; //!< Reserved for future use
        } B;
        uint32_t U;
    } option1;

} spi_nor_eeprom_config_option_t;

//! @brief Serial NOR/EEPROM peripheral Config block structure
typedef struct __spi_nor_eeprom_peripheral_config
{
    uint8_t spiIndex;     //!< Index of SPI module
    uint8_t spiPcsx;      //!< PCS instance of SPI module
    uint8_t reserved0[2]; //!< Reserved0
    uint32_t spiSpeed_Hz; //!< SPI transfer speed to connected NOR/EEPROM
} spi_nor_eeprom_peripheral_config_t;

//! @brief Serial NOR/EEPROM Config block structure
typedef struct __spi_nor_eeprom_config
{
    uint32_t tag;                                 //!< Set to magic number of 'ksec'
    uint32_t version;                             //!< version of config struct
    spi_nor_eeprom_peripheral_config_t spiConfig; //!< Spi configuration struct
    serial_nor_eeprom_config_t memoryConfig;      //!< Nor/Eeprom configuration struct
} spi_nor_eeprom_config_t;

enum
{
    kSpiNorEeprom_StartAddress = 0,

    //! @brief Max page size used to create a page buffer
    kSpiNorEeprom_MaxPageSize = 1024U,
};

//! @brief SPI NOR/EEPROM memory page program memory info
//!
//! An instance is maintained in this file, will is used to keep key information for write and flush
//! operatations.
typedef struct _spi_nor_eeprom_page_program_info
{
    uint32_t startAddress;                     //!< This address is used to record the address which is used
                                               //!< to write the whole page into nor/eeprom memory
    uint32_t storedBytes;                      //!< A variable which is used to indicate if the buffer is full.
    uint8_t buffer[kSpiNorEeprom_MaxPageSize]; //!< A buffer which is used to buffer a full page of data
} spi_nor_eeprom_page_program_info_t;

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

//! @name Serial NOR/EEPROM memory
//@{

//! @brief Enable clock gate of SPI
extern void spi_clock_gate_enable(uint32_t instance);

//! @brief Configure Clock for SPI
extern void spi_clock_config(uint32_t instance, spi_module_clock_freq_t freq);

//! @brief Control SPI pcs pin
extern void spi_pcs_pin_control(uint32_t instance, uint8_t pcsx, bool isSelected);

//! @brief Configure IOMUX for SPI
extern void spi_iomux_config(spi_nor_eeprom_peripheral_config_t *config);

//! @brief Initialize Serial NOR/EEPROM memory
status_t spi_nor_eeprom_mem_init(void);

//! @brief Read Serial NOR/EEPROM memory.
status_t spi_nor_eeprom_mem_read(uint32_t address, uint32_t length, uint8_t *restrict buffer);

//! @brief Write Serial NOR/EEPROM memory.
status_t spi_nor_eeprom_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer);

//! @brief  Erase Serial NOR/EEPROM memory
status_t spi_nor_eeprom_mem_erase(uint32_t address, uint32_t length);

//! @brief  Erase all Serial NOR/EEPROM memory
status_t spi_nor_eeprom_mem_erase_all(void);

//! @brief Configure Serial NOR/EEPROM memory
status_t spi_nor_eeprom_mem_config(uint32_t *config);

//! @brief flush cached data to Serial NOR/EEPROM memory
status_t spi_nor_eeprom_mem_flush(void);

//! @brief finalize the read/write operation of Serial NOR/EEPROM memory
status_t spi_nor_eeprom_mem_finalize(void);

//@}

#if defined(__cplusplus)
}
#endif

#endif /* #ifndef __SPI_NOR_EEPROM_MEMORY_H__ */

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
