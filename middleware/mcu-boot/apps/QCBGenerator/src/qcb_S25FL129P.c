/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// This is the QCB for the case that one S25FL129P is connected to QuadSPI0A port.
const qspi_config_t qspi_config_block = {
    .tag = kQspiConfigTag,
    .version = {.version = kQspiVersionTag },
    .lengthInBytes = 512,
    .sflash_A1_size = 0x1000000,           // 16MB
    .sclk_freq = kQspiSerialClockFreq_Mid, // Mid frequency 96MHz / 2 = 48MHz
    .sflash_type = kQspiFlashPad_Quad,     // SPI Flash devices work under quad-pad mode
    .sflash_port = kQspiPort_EnablePortA,  // Only QuadSPI0A is enabled.
    .busy_bit_offset = 0,                  // Busy offset is 0
    .ddr_mode_enable = 0,                  // disable DDR mode
    .dqs_enable = 0,                       // Disable DQS feature
    .parallel_mode_enable = 0,             // QuadSPI module work under serial mode
    .pagesize = 256,                       // Page Size : 256 bytes
    .sectorsize = 0x1000,                  // Sector Size: 8KB
    .device_mode_config_en = 1,            // configure quad mode for spi flash
    .device_cmd = 0x0200,                  // Enable quad mode
    .write_cmd_ipcr = 0x05000000U,         // IPCR indicating enable seqid (5<<24)
    .ips_command_second_divider = 3,       // Set second divider for QSPI serial clock to 3
    .look_up_table =
        {
                // Seq0 : Quad Read (maximum supported freq: 80MHz)
                /*
                CMD:        0xEB - Quad Read, Single pad
                ADDR:       0x18 - 24bit address, Quad pads
                MODE:       0xB5 - Non continuous read mode
                DUMMY:      0x04 - 4 clock cycles, Quad pads
                READ:       0x80 - Read 128 bytes, Quad pads
                JUMP_ON_CS: 0 - Jump to address phase
                */
                [0] = 0x0A1804EB, [1] = 0x1E800E06, [2] = 0x2400,

                // Seq1: Write Enable (maximum supported freq: 80MHz)
                /*
                CMD:      0x06 - Write Enable, Single pad
                */
                [4] = 0x406,

                // Seq2: Erase All (maximum supported freq: 80MHz)
                /*
                CMD:    0x60 - Erase All chip, Single pad
                */
                [8] = 0x460,

                // Seq3: Read Status (maximum supported freq: 80MHz)
                /*
                CMD:    0x05 - Read Status, single pad
                READ:   0x01 - Read 1 byte
                */
                [12] = 0x1c010405,

                // Seq4: Quad Page Program (maximum supported freq: 80MHz)
                /*
                CMD:    0x32 - Quad Page Program, Single pad
                ADDR:   0x18 - 24bit address, Single pad
                WRITE:  0x40 - Write 64 bytes at one pass, Quad pads
                        (0x40 is ignored, as it will be overwritten by page size)
                */
                [16] = 0x08180432, [17] = 0x2040,

                // Seq5: Write status register to enable quad mode
                /*
                CMD:    0x01 - Write Status Register, single pad
                WRITE:  0x02 - Write 2 bytes of data, single pad
                */
                [20] = 0x20010402,

                // Seq7: Erase Sector
                /*
                CMD:  0xD8 - Sector Erase, single pad
                ADDR: 0x18 - 24 bit address, single pad
                */
                [28] = 0x081804D8,

                // Seq8: Dummy
                /*
                CMD:    0 - Dummy command, single pad
                        Not necessary, as the SPI flash device isn't work at continuous read mode.
                */
                [32] = 0,
        },
};