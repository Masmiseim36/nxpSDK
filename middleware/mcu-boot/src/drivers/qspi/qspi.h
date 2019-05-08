/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QSPI_H__
#define __QSPI_H__

#include "memory/memory.h"
#include "bootloader_common.h"

/*!
 * @addtogroup qspi_driver
 * @{
 */

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define QSPI_LUT_MAX_ENTRIES 64
#define QSPI_PRE_CMD_CNT 4 //!< the max number of pre commands
#define QSPI_FEATURE_ENABLE 1

#define QSPI_VERSION_NAME 'Q'

enum __qspi_config_block_tags
{
    //! @brief Tag value used to validate the qspi config block.
    kQspiConfigTag = FOUR_CHAR_CODE('k', 'q', 'c', 'f'),
    //!< kQspiVersion1.1 Support Parallel mode, timeout check
    //!< kQspiVersion1.2 Support configuring QuadSPI_FLSHCR[TDH]
    kQspiVersionTag = FOUR_CHAR_CODE(0, 2, 1, 'Q'),
};

//! @brief QSPI Flash mode options
typedef enum _qspiflash_mode_option
{
    kQspiFlashMode_Serial = 0,  //!< Serial mode
    kQspiFlashMode_Parallel = 1 //!< Parallel mode
} qspiflash_mode_option_t;

//! @brief External spi flash pad definition
typedef enum _qspiflash_pad
{
    kQspiFlashPad_Single = 0, //!< Single-pad spi flash
    kQspiFlashPad_Dual = 1,   //!< Dual-pad spi flash
    kQspiFlashPad_Quad = 2,   //!< Quad-pad spi flash
    kQspiFlashPad_Octal = 3   //!< Octal-pad spi flash
} qspiflash_pad_t;

//! @brief QSPI Serial Clock Frequency options
typedef enum _qspi_serial_clock_freq
{
    kQspiSerialClockFreq_Low = 0, //!< QuadSPI module works at low frequency
    kQspiSerialClockFreq_Mid = 1, //!< QuadSPI module works at mid frequency
    kQspiSerialClockFreq_High = 2 //!< QuadSPI module works at high frequency
} qspi_serial_clock_freq_t;

//! @brief QSPI flash property tag values
typedef enum _qspiflash_property
{
    kQspiFlashProperty_InitStatus = 0,
    kQspiFlaghProperty_StartAddress = 1,           //!< Tag used to retrieve start address
    kQspiFlashProperty_TotalFlashSizeInKBytes = 2, //!< Tag used to retrieve total flash size in terms of KByte
    kQspiFlashProperty_PageSize = 3,               //!< Tag used to retreive page size in terms of byte
    kQspiFlashProperty_SectorSize = 4,             //!< Tag used to retrieve sector size in term of byte
    kQspiFlashProperty_BlockSize = 5,              //!< Tag used to retrieve block size in terms of byte

    kQspiFlashProperty_TotalFlashSize = 0x10, //!< Tag used to retrieve total flash size in terms of byte
} qspiflash_property_t;

//! @brief Endianess supported by QSPI module
typedef enum _qspi_endianness
{
    kQspiEndianess_64BE = 0U, //!< 64-bit Big Endian
    kQspiEndianess_32LE = 1U, //!< 32-bit Little Endian
    kQspiEndianess_32BE = 2U, //!< 32-bit Big Endian
    kQspiEndianess_64LE = 3U, //!< 64-bit Little Endian
} qspi_endianess_t;

//! @brief Port Enablement Option
typedef enum _qspi_port_enable
{
    kQspiPort_EnablePortA = 0U,     //!< Only PORTA is enabled
    kQspiPort_EnableBothPorts = 1,  //!< Enable Both PoartA and PortB
    kQspiPort_EnablePortAviaPTC = 2,//!< Serial NOR device is connected on QSPI0A via PTC24-PTC29.
} qspi_port_enable_t;

//! @brief Definition for AHB data transfer size
typedef enum _qspi_ahb_data_transfer_size
{
    kQspiAHBDataTransferSize_64Bytes = 8U,   //!< AHB data transfer size is 64bytes
    kQspiAHBDataTransferSize_256Bytes = 32U, //!< AHB data transfer size is 256bytes
    kQspiAHBDataTransferSize_512Bytes = 64U, //!< AHB data transfer size is 512bytes
} qspi_ahb_data_transfer_size_t;

//! @brief Error codes of QuadSPI driver
typedef enum _qspiflash_status
{
    //! @brief Error code which represents that flash size is error
    kStatus_QspiFlashSizeError = MAKE_STATUS(kStatusGroup_QuadSPIDriver, 0),
    //! @brief Error code which represents that start address for programming is not page aligned.
    kStatus_QspiFlashAlignmentError = MAKE_STATUS(kStatusGroup_QuadSPIDriver, 1),
    //! @brief Error code which represents that the address is invalid
    kStatus_QspiFlashAddressError = MAKE_STATUS(kStatusGroup_QuadSPIDriver, 2),
    //! @brief Error code which represents that the operation is not successfully executed.
    kStatus_QspiFlashCommandFailure = MAKE_STATUS(kStatusGroup_QuadSPIDriver, 3),
    //! @brief Error code which represents that the property is not supported
    kStatus_QspiFlashUnknownProperty = MAKE_STATUS(kStatusGroup_QuadSPIDriver, 4),
    //! @brief Error code which represents that qspi module is not configured yet.
    kStatus_QspiNotConfigured = MAKE_STATUS(kStatusGroup_QuadSPIDriver, 5),
    //! @brief Error code which represents that a command is not supported under
    //!        certain mode.
    kStatus_QspiCommandNotSupported = MAKE_STATUS(kStatusGroup_QuadSPIDriver, 6),
    //! @brief Error code which represents that operation is timeout
    kStatus_QspiCommandTimeout = MAKE_STATUS(kStatusGroup_QuadSPIDriver, 7),

    //! @brief Error code which represents that QSPI cannot perform write operation at expected frequency
    kStatus_QspiWriteFailure = MAKE_STATUS(kStatusGroup_QuadSPIDriver, 8),

    //! @brief Error code which represents that the QSPI module is busy, which may be caused by incorrect
    //        commands in LUT
    kStatusQspiModuleBusy = MAKE_STATUS(kStatusGroup_QuadSPIDriver, 9),
} qspiflash_status_t;

//! @brief QuadSPI Config block structure
typedef struct __sflash_configuration_parameter
{
    uint32_t tag;               //!< Set to magic number of 'kqcf'
    standard_version_t version; //!< version of config struct
                                //!< the version number i organized as following:
    uint32_t lengthInBytes;     //!< Total length of strcut in bytes

    uint32_t dqs_loopback;          //!< Sets DQS LoopBack Mode to enable Dummy Pad MCR[24]
    uint32_t data_hold_time;        //!< Serial flash data In Hold time, valid value: 0/1/2
    uint32_t reserved0[2];          //!< Reserved for K80
    uint32_t device_mode_config_en; //!< Determine if it is required to config working mode of external spi flash.
    uint32_t device_cmd;            //!< Command to be tranferred to device
    uint32_t write_cmd_ipcr;        //!< IPCR value of Write command
    uint32_t word_addressable;      //!< Determine if the serial flash is word addressable

    uint32_t cs_hold_time;         //!< CS hold time in terms of serial clock.(for example 1 serial clock cyle)
    uint32_t cs_setup_time;        //!< CS setup time in terms of serial clock.(for example 1 serial clock cyle)
    uint32_t sflash_A1_size;       //!< Size of flash connected on QSPI0A Ports and QSPI0A_SS0, in terms of Bytes
    uint32_t sflash_A2_size;       //!< Size of flash connected on QSPI0A Ports and QSPI0A_SS1, in terms of Bytes
    uint32_t sflash_B1_size;       //!< Size of flash connected on QSPI0B Ports and QSPI0B_SS0, in terms of Bytes
    uint32_t sflash_B2_size;       //!< Size of flash connected on QSPI0B Ports and QSPI0B_SS1, in terms of Bytes
    uint32_t sclk_freq;            //!< In  00 - 24MHz, 01 - 48MHz, 10 - 96MHz,(only for SDR Mode)
    uint32_t busy_bit_offset;      //!< Flash device busy bit offset in status register
    uint32_t sflash_type;          //!< SPI flash type: 0-Single,1--Dual 2--Quad, 3-- Octal
    uint32_t sflash_port;          //!< 0--Only Port-A, 1--Both PortA and PortB
    uint32_t ddr_mode_enable;      //!< Enable DDR mode if set to TRUE
    uint32_t dqs_enable;           //!< Enable DQS mode if set to TRUE.
    uint32_t parallel_mode_enable; //!< Enable Individual or parrallel mode.
    uint32_t portA_cs1;            //!< Enable PORTA CS1
    uint32_t portB_cs1;            //!< Enable PORTB CS1
    uint32_t fsphs;                //!< Full speed delay selection for SDR instructions
    uint32_t fsdly;                //!< Full speed phase selection for SDR instructions
    uint32_t ddrsmp;               //!< Select the sampling point for incomming data when serial flash is in DDR mdoe
    uint32_t
        look_up_table[QSPI_LUT_MAX_ENTRIES]; //!< Set of seq to perform optimum read on SFLASH as as per vendor SFLASH
    uint32_t column_address_space;           //!< The width of the column address
    uint32_t config_cmd_en;                  //!< Enable config commands
    uint32_t config_cmds[QSPI_PRE_CMD_CNT];  //!< Config comands, used to configure nor flash
    uint32_t config_cmds_args[QSPI_PRE_CMD_CNT]; //!< Config commands arguments
    uint32_t differential_clock_pin_enable;      //!< Differential flash clock pins enable
    uint32_t flash_CK2_clock_pin_enable;         //!< Flash CK2 clock pin enable
    uint32_t dqs_inverse_sel;                    //!< Select clock source for internal DQS generation
    uint32_t dqs_latency_enable;                 //!< DQS Latency Enable
    uint32_t dqs_loopback_internal;              //!< 0: dqs loopback from pad, 1: dqs loopback internally
    uint32_t dqs_phase_sel;                      //!< dqs phase sel
    uint32_t dqs_fa_delay_chain_sel;             //!< dqs fa delay chain selection
    uint32_t dqs_fb_delay_chain_sel;             //!< dqs fb delay chain selection
    uint32_t reserved1[2];                       //!< reserved
    uint32_t pagesize;                           //!< page Size of Serial Flash
    uint32_t sectorsize;                         //!< sector Size of Serial Flash

    uint32_t timeout_milliseconds; //!< timeout in terms of millisecond in case of infinite loop in qspi driver
                                   //!<  0 represents disabling timeout check. This value is valid since version 1.1.0
    uint32_t ips_command_second_divider; //!< second devider for all IPS commands.
    uint32_t need_multi_phases;          //!< Determine if multiple hases command are needed.
    uint32_t is_spansion_hyperflash;     //!< Determine if connected spi flash device belongs to Hyperflash family
    uint32_t pre_read_status_cmd_address_offset; //!< Address for PreReadStatus command
    uint32_t pre_unlock_cmd_address_offset;      //!< Address for PreWriteEnable command
    uint32_t unlock_cmd_address_offset;          //!< Address for WriteEnable command
    uint32_t pre_program_cmd_address_offset;     //!< Address for PreProgram command
    uint32_t pre_erase_cmd_address_offset;       //!< Address for PreErase command
    uint32_t erase_all_cmd_address_offset;       //!< Address for EraseAll command
    uint32_t reserved2[3];                       //!< Reserved words to make sure qspi config block is page-aligend.

} qspi_config_t, *SFLASH_CONFIGURATION_PARAM_PTR;

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

//! @brief Get Property from QSPI driver
//!
//! This function is used to retrieve qspi related properties
//!
//! @param wichProperty     Property tag
//! @param value  Pointer which is used to return the propery value
//!
//! @return An error code or kStatus_Success
status_t quadspi_get_property(uint32_t whichProperty, uint32_t *value);

//! @brief Page program to external spi flash
//!
//! This function programs certain length of data to expected address according
//! to parameters passed in
//!
//! @param dst_addr Destination Address to be programmed
//! @param src  Data to be programmed
//! @param lengthInBytes Size of data to be programmed
//!
//! @return An error code or kStatus_Success
status_t quadspi_page_program(uint32_t dst_addr, const uint32_t *src, uint32_t lengthInBytes);

//! @brief Intialize QSPI module according to parameters passed in
//!
//! This function intializes and configures QuadSPI module according to the
//! pointer which points to quad spi config block
//!
//! @param config_base Base address of a quadspi config block
//!
//! @return An error code or kStatus_Success
status_t quadspi_init(qspi_config_t *config_base);

//! @brief Get the status of quadspi configuration
//!
//! This function return True or False which represents the QuadSPI module is
//! successfully configured or not configured yet respectively.
//!
//! @return true or false
bool is_quadspi_configured(void);

//! @brief Configure pinmux of QSPI module according to parameters passed in
//!
//! This function configures pinmux for QSPI modules dynamically according to
//! the pointer which points to a qspi config block.
//! This function is target-specific, should be implemented in BSP.
//!
//! @param config_base A pointer which points to a qspi config block
//!
//! @return An error code or kStatus_Success
extern status_t quadspi_iomux_configuration(qspi_config_t *config_base);

//! @brief Configure QSPI serial clock frquency
//!
//! This function configures the serial clock frequency of QSPI module according
//! to the expected option.
//! This function is target-specific, should be implemented in BSP.
//!
//! @param freq enumerated variable represent serial clock frequency
extern void quadspi_serial_clock_configure(qspi_serial_clock_freq_t freq);

//! @brief Erase the entire spi flash devices
//!
//! @return An error code or kStatus_Success
status_t quadspi_erase_all(void);

//! @brief Erase one sector from the provided address
//!
//! @param address The start address of the sector to be erased.
//!
//! @return An error code or kStatus_Success
status_t quadspi_erase_sector(uint32_t address);

//! @brief Flush QSPI cache
//!
//! @return kStatus_Success
status_t quadspi_cache_clear(void);

#if defined(__cplusplus)
}

/*! @}*/

#endif // #ifndef __QSPI_H__

#endif

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
