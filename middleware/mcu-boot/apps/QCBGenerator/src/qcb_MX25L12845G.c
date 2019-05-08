/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

const qspi_config_t qspi_config_block = {
    .tag = kQspiConfigTag,
    .version = {.version = kQspiVersionTag },
    .lengthInBytes = 512,
    .sflash_A1_size = 0x1000000,            // 16MB
    .sclk_freq = kQspiSerialClockFreq_High, // High frequency, 96MHz/4 = 24MHz
    .sflash_type = kQspiFlashPad_Quad,      // SPI Flash devices work under quad-pad mode
    .sflash_port = kQspiPort_EnablePortA,   // Only QSPI0A is enabled.
    .busy_bit_offset = 0,                   // Busy offset is 0, polarity: 1 means busy
    .ddr_mode_enable = 1,                   // Enable DDR mode
    .data_hold_time = 1,                    // Data aligned with 2x serial flash half clock
    .ddrsmp = 0,
    .dqs_enable = 0, // Disable DQS feature
    .dqs_loopback = 0,
    .pagesize = 256,      // Page Size : 256 bytes
    .sectorsize = 0x1000, // Sector Size: 4KB
    .ips_command_second_divider = 0,
    .device_mode_config_en = 1,  // Configure the device to 4-byte address mode
    .device_cmd = 0,             // Not needed.
    .write_cmd_ipcr = 5UL << 24, // Seq5 for setting address type to 4 bytes

    .look_up_table =
        {
                // Seq0 : 4x I/O DTR Read (maximum supported freq: 54MHz)
                /*
                CMD_DDR:        0xED - Quad Read, Single pad
                ADDR_DDR:       0x18 - 24bit address, Quad pads
                DUMMY:          0x06 - 6 dummy cycles, Quad pads
                READ_DDR:       0x80 - Read 128 bytes, Quad pads
                JUMP_ON_CS: 0
                */
                [0] = 0x2A1804ED, [1] = 0x3A800E06, [2] = 0x2400,

                // Seq1: Write Enable (maximum supported freq: 120MHz)
                /*
                CMD:      0x06 - Write Enable, Single pad
                */
                [4] = 0x406,

                // Seq2: Erase All (maximum supported freq: 120MHz)
                /*
                CMD:    0xC7 - Erase All chip, Single pad
                */
                [8] = 0x04C7,

                // Seq3: Read Status (maximum supported freq: 120MHz)
                /*
                CMD:    0x05 - Read Flag Status, single pad
                READ:   0x01 - Read 1 bytes
                */
                [12] = 0x1c010405,

                // Seq4: Page Program (maximum supported freq: 120MHz)
                /*
                CMD:    0x02 - Page Program, Single pad
                ADDR:   0x18 - 24bit address, Single pad
                WRITE:  0x40 - Write 64 bytes at one pass, Single Pad
                */
                [16] = 0x08180402, [17] = 0x2040,

                // Seq5: Enable Quad mode
                /*
                CMD:    0x01 - Write Status Register
                CMD:    0x40 - Treat write value as command, enable Quad mode
                */
                [20] = 0x04400401,

                // Seq7: Erase Sector
                /*
                CMD:  0x20 - Sector Erase, single pad
                ADDR: 0x18 - 24 bit address, single pad
                */
                [28] = 0x08180420,
        },
};