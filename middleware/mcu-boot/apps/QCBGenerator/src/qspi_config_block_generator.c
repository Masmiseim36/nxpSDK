/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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

//! @brief Build a 32-bit code from four character values.
//!
//! The resulting value is built with a byte order such that the string
//! being readable in expected order when viewed in a hex editor, if the value
//! is treated as a 32-bit little endian value.
#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))

enum __qspi_config_block_tags
{
    //! @brief Tag value used to validate the qspi config block.
    kQspiConfigTag = FOUR_CHAR_CODE('k', 'q', 'c', 'f'),
    kQspiVersionTag = FOUR_CHAR_CODE(0, 1, 1, 'Q'),
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

//! @brief Port Enablement Option
typedef enum _qspi_port_enable
{
    kQspiPort_EnablePortA = 0U,    //!< Only PORTA is enabled
    kQspiPort_EnableBothPorts = 1, //!< Enable Both PoartA and PortB
} qspi_port_enable_t;

//! @brief Definition for AHB data transfer size
typedef enum _qspi_ahb_data_transfer_size
{
    kQspiAHBDataTransferSize_64Bytes = 8U,   //!< AHB data transfer size is 64bytes
    kQspiAHBDataTransferSize_256Bytes = 32U, //!< AHB data transfer size is 256bytes
} qspi_ahb_data_transfer_size_t;

typedef union BootloaderVersion
{
    struct
    {
        uint32_t bugfix : 8; //!< bugfix version [7:0]
        uint32_t minor : 8;  //!< minor version [15:8]
        uint32_t major : 8;  //!< major version [23:16]
        uint32_t name : 8;   //!< name [31:24]
    } B;
    uint32_t version; //!< combined version numbers
} standard_version_t;

//! @brief QuadSPI Config block structure
#pragma pack(1)
typedef struct __sflash_configuration_parameter
{
    uint32_t tag;               //!< Set to magic number of 'kqcf'
    standard_version_t version; //!< version of config struct
    uint32_t lengthInBytes;     //!< Total length of strcut in bytes

    uint32_t dqs_loopback;          //!< Sets DQS LoopBack Mode to enable Dummy Pad MCR[24]
    uint32_t data_hold_time;        //!< Serial Flash data hold time, valid value: 0/1/2
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
#pragma pack()

int main(void)
{
    const qspi_config_t qspi_config_block = {
        .tag = kQspiConfigTag,                    // Fixed value, do not change
        .version = {.version = kQspiVersionTag }, // Fixed value, do not change
        .lengthInBytes = 512,                     // Fixed value, do not change
        .sflash_A1_size = 0x400000,               // 4MB
        .sflash_B1_size = 0x400000,               // 4MB
        .sclk_freq = kQspiSerialClockFreq_High,   // High frequency, in K82-256, it means 96MHz/1 = 96MHz
        .sflash_type = kQspiFlashPad_Quad,        // SPI Flash devices work under quad-pad mode
        .sflash_port = kQspiPort_EnableBothPorts, // Both QSPI0A and QSPI0B are enabled.
        .busy_bit_offset = 0,                     // Busy offset is 0
        .ddr_mode_enable = 0,                     // disable DDR mode
        .dqs_enable = 0,                          // Disable DQS feature
        .parallel_mode_enable = 0,                // QuadSPI module work under serial mode
        .pagesize = 256,                          // Page Size : 256 bytes
        .sectorsize = 0x1000,                     // Sector Size: 4KB
        .device_mode_config_en = 1,               // Enable quad mode for SPI flash
        .device_cmd = 0x40,                       // Enable quad mode via set bit 6 in status register to 1
        .write_cmd_ipcr = 0x05000000U,            // IPCR indicating seq id for Quad Mode Enable (5<<24)
        .ips_command_second_divider = 3,          // Set second divider for QSPI serial clock to 3
        .look_up_table =
            {
                    // Seq0 : Quad Read (maximum supported freq: 104MHz)
                    /*
                    CMD:        0xEB - Quad Read, Single pad
                    ADDR:       0x18 - 24bit address, Quad pads
                    DUMMY:      0x06 - 6 clock cycles, Quad pads
                    READ:       0x80 - Read 128 bytes, Quad pads
                    JUMP_ON_CS: 0
                    */
                    [0] = 0x0A1804EB, [1] = 0x1E800E06, [2] = 0x2400,

                    // Seq1: Write Enable (maximum supported freq: 104MHz)
                    /*
                    CMD:      0x06 - Write Enable, Single pad
                    */
                    [4] = 0x406,

                    // Seq2: Erase All (maximum supported freq: 104MHz)
                    /*
                    CMD:    0x60 - Erase All chip, Single pad
                    */
                    [8] = 0x460,

                    // Seq3: Read Status (maximum supported freq: 104MHz)
                    /*
                    CMD:    0x05 - Read Status, single pad
                    READ:   0x01 - Read 1 byte
                    */
                    [12] = 0x1c010405,

                    // Seq4: 4 I/O Page Program (maximum supported freq: 104MHz)
                    /*
                    CMD:    0x38 - 4 I/O Page Program, Single pad
                    ADDR:   0x18 - 24bit address, Quad pad
                    WRITE:  0x40 - Write 64 bytes at one pass, Quad pad
                    */
                    [16] = 0x0A180438, [17] = 0x2240,

                    // Seq5: Write status register to enable quad mode
                    /*
                    CMD:    0x01 - Write Status Register, single pad
                    WRITE:  0x01 - Write 1 byte of data, single pad
                    */
                    [20] = 0x20010401,

                    // Seq7: Erase Sector
                    /*
                    CMD:  0x20 - Sector Erase, single pad
                    ADDR: 0x18 - 24 bit address, single pad
                    */
                    [28] = 0x08180420,

                    // Seq8: Dummy
                    /*
                    CMD:    0 - Dummy command, used to force SPI flash to exit continuous read mode.
                            unnecessary here because the continuous read mode is not enabled.
                    */
                    [32] = 0,
            },
    };

    // Generate QSPI config block
    const char *filePath = "qspi_config_block.bin";
    FILE *fp = fopen(filePath, "wb+");
    if (fp == NULL)
    {
        printf("Cannot open/create %s\n", filePath);
    }
    else
    {
        size_t write_size = sizeof(qspi_config_block);
        size_t actual_write_size;

        actual_write_size = fwrite(&qspi_config_block, 1, write_size, fp);
        fclose(fp);

        if (actual_write_size == write_size)
        {
            printf("Generated %s successfully.\n\n", filePath);
        }
        else
        {
            printf("Failed to generate %s \n\n", filePath);
        }
    }

    system("pause");

    return 0;
}
