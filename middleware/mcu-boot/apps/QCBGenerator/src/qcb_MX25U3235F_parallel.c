/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// This is the QCB for the case that two MX25U3235F are connected to QuadSPI0A and QuadSPI0B ports.
// Work under parallel mode
const qspi_config_t qspi_config_block = {
    .tag = kQspiConfigTag,
    .version = {.version = kQspiVersionTag },
    .lengthInBytes = 512,
    .sflash_A1_size = 0x400000,               // 4MB
    .sflash_B1_size = 0x400000,               // 4MB
    .sclk_freq = kQspiSerialClockFreq_High,   // High frequency
    .sflash_type = kQspiFlashPad_Quad,        // SPI Flash devices work under quad-pad mode
    .sflash_port = kQspiPort_EnableBothPorts, // Both QSPI0A and QSPI0B are enabled.
    .busy_bit_offset = 0,                     // Busy offset is 0
    .ddr_mode_enable = 0,                     // disable DDR mode
    .dqs_enable = 0,                          // Disable DQS feature
    .parallel_mode_enable = 1,                // QuadSPI module work under parallel mode
    .pagesize = 512,                          // Page Size : 256 *2 = 512 bytes
    .sectorsize = 0x2000,                     // Sector Size: 4KB * 2 = 8KB
    .device_mode_config_en = 1,               // configure quad mode for spi flash
    .device_cmd = 0x40,                       // Enable quad mode
    .write_cmd_ipcr = 0x05000000U,            // IPCR indicating enable seqid (5<<24)
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

                // Seq4: Page Program (maximum supported freq: 104MHz)
                /*
                CMD:    0x02 - Page Program, Single pad
                ADDR:   0x18 - 24bit address, Single pad
                WRITE:  0x40 - Write 64 bytes at one pass, Single pad
                        (0x40 is ignored, as it will be overwritten by page size)
                */
                [16] = 0x08180402, [17] = 0x2040,

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
                        unecessary here because the continous read mode isn't enabled.
                */
                [32] = 0,
        },
};