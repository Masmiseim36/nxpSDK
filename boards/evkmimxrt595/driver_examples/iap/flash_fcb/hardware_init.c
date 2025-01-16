/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${macro:start}*/
#define FLASH_CONFIG_BLOCK_TAG     (0x42464346)
#define FLASH_CONFIG_BLOCK_VERSION (0x56010400)

#define CMD_SDR        0x01
#define CMD_DDR        0x21
#define RADDR_SDR      0x02
#define RADDR_DDR      0x22
#define WRITE_SDR      0x08
#define WRITE_DDR      0x28
#define READ_SDR       0x09
#define READ_DDR       0x29
#define DUMMY_SDR      0x0C
#define DUMMY_DDR      0x2C
#define DUMMY_RWDS_SDR 0x0D
#define DUMMY_RWDS_DDR 0x2D
#define STOP_EXE       0

#define FLEXSPI_1PAD 0
#define FLEXSPI_2PAD 1
#define FLEXSPI_4PAD 2
#define FLEXSPI_8PAD 3

#define FLEXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)                                                              \
    (FLEXSPI_LUT_OPERAND0(op0) | FLEXSPI_LUT_NUM_PADS0(pad0) | FLEXSPI_LUT_OPCODE0(cmd0) | FLEXSPI_LUT_OPERAND1(op1) | \
     FLEXSPI_LUT_NUM_PADS1(pad1) | FLEXSPI_LUT_OPCODE1(cmd1))
/*${macro:end}*/

/*${variable:start}*/
flexspi_nor_config_t config = {
    .memConfig =
        {
            .tag                 = FLASH_CONFIG_BLOCK_TAG,
            .version             = FLASH_CONFIG_BLOCK_VERSION,
            .readSampleClkSrc    = 3, /* External DQS signal. */
            .csHoldTime          = 3,
            .csSetupTime         = 3,
            .deviceModeCfgEnable = 1,
            .deviceModeType      = kDeviceConfigCmdType_Spi2Xpi,
            .waitTimeCfgCommands = 1,
            .deviceModeSeq =
                {
                    .seqNum   = 1,
                    .seqId    = 6, /* See Lookup table for more details. */
                    .reserved = 0,
                },
            .deviceModeArg        = 2,    /* Enable OPI DDR mode. */
            .controllerMiscOption = 0x50, /* Safe Configuration Frequency enable & DDR clock confiuration indication.*/
            .deviceType           = 1,
            .sflashPadType        = kSerialFlash_8Pads,
            .serialClkFreq        = 4, /* 80MHZ, refer to ReferenceManual. */
            .sflashA1Size         = 64ul * 1024u * 1024u,
            .lookupTable =
                {
                    /* Read */
                    [0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0xEE, CMD_DDR, FLEXSPI_8PAD, 0x11),
                    [1] = FLEXSPI_LUT_SEQ(RADDR_DDR, FLEXSPI_8PAD, 0x20, DUMMY_DDR, FLEXSPI_8PAD, 0x04),
                    [2] = FLEXSPI_LUT_SEQ(READ_DDR, FLEXSPI_8PAD, 0x04, STOP_EXE, FLEXSPI_1PAD, 0x00),

                    /* Read Status SPI */
                    [4 * 1 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x05, READ_SDR, FLEXSPI_1PAD, 0x04),

                    /* Read Status OPI */
                    [4 * 2 + 0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x05, CMD_DDR, FLEXSPI_8PAD, 0xFA),
                    [4 * 2 + 1] = FLEXSPI_LUT_SEQ(RADDR_DDR, FLEXSPI_8PAD, 0x20, DUMMY_DDR, FLEXSPI_8PAD, 0x14),
                    [4 * 2 + 2] = FLEXSPI_LUT_SEQ(READ_DDR, FLEXSPI_8PAD, 0x04, STOP_EXE, FLEXSPI_1PAD, 0x00),

                    /* Write Enable */
                    [4 * 3 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x06, STOP_EXE, FLEXSPI_1PAD, 0x00),
                    /* Write Enable - OPI */
                    [4 * 4 + 0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x06, CMD_DDR, FLEXSPI_8PAD, 0xF9),

                    /* Sector Erase */
                    [4 * 5 + 0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x21, CMD_DDR, FLEXSPI_8PAD, 0xDE),
                    [4 * 5 + 1] = FLEXSPI_LUT_SEQ(RADDR_DDR, FLEXSPI_8PAD, 0x20, STOP_EXE, FLEXSPI_1PAD, 0x00),

                    /* Enable OPI DDR mode */
                    [4 * 6 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x72, CMD_SDR, FLEXSPI_1PAD, 0x00),
                    [4 * 6 + 1] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x00, CMD_SDR, FLEXSPI_1PAD, 0x00),
                    [4 * 6 + 2] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x00, WRITE_SDR, FLEXSPI_1PAD, 0x01),

                    /* Block erase (64KB) */
                    [4 * 8 + 0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0xDC, CMD_DDR, FLEXSPI_8PAD, 0x23),
                    [4 * 8 + 1] = FLEXSPI_LUT_SEQ(RADDR_DDR, FLEXSPI_8PAD, 0x20, STOP_EXE, FLEXSPI_1PAD, 0x00),

                    /* Page program */
                    [4 * 9 + 0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x12, CMD_DDR, FLEXSPI_8PAD, 0xED),
                    [4 * 9 + 1] = FLEXSPI_LUT_SEQ(RADDR_DDR, FLEXSPI_8PAD, 0x20, WRITE_DDR, FLEXSPI_8PAD, 0x04),

                    /* Read identification */
                    [4 * 11 + 0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x60, CMD_DDR, FLEXSPI_8PAD, 0xf0),
                },
        },
    .pageSize      = 256u,
    .sectorSize    = 4u * 1024u,
    .blockSize     = 64u * 1024u,
    .serialNorType = 2, /* Serial Nor Type XPI. */

};
/*${variable:end}*/

/*${function:start}*/
AT_QUICKACCESS_SECTION_CODE(status_t Demo_FlexspiNorInit(uint32_t instance, flexspi_nor_config_t *config))
{
    /* Wait until the FLEXSPI is idle */
    register uint32_t delaycnt = 10000u;

    while ((delaycnt--) != 0U)
    {
    }
    status_t status = IAP_FlexspiNorInit(instance, config);

    return status;
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

/*${function:end}*/
