/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*${header:start}*/
#include "fsl_flexspi.h"
#include "sln_flash_config.h"
/*${header:end}*/

/*${variable:start}*/

flexspi_device_config_t deviceconfig = {
    .flexspiRootClk       = 120000000,
    .flashSize            = FLASH_SIZE / 1024, /* the value is in Kb */
    .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval           = 2,
    .CSHoldTime           = 3,
    .CSSetupTime          = 3,
    .dataValidTime        = 0,
    .columnspace          = 0,
    .enableWordAddress    = 0,
    .AWRSeqIndex          = 0,
    .AWRSeqNumber         = 0,
    .ARDSeqIndex          = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD,
    .ARDSeqNumber         = 1,
    .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
};

const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
    /* Normal READ */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD]     = FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,
                                                               kFLEXSPI_1PAD,
                                                               W25Q_FastReadQuad,
                                                               kFLEXSPI_Command_RADDR_SDR,
                                                               kFLEXSPI_4PAD,
                                                               NORFLASH_ADDR_LENGTH),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_MODE8_SDR, kFLEXSPI_4PAD, 0x00, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x04),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x0),

    /* Fast Read QUAD 4byte address*/
    [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD_4BYTE]     = FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,
                                                                     kFLEXSPI_1PAD,
                                                                     W25Q_FastReadQuad4B,
                                                                     kFLEXSPI_Command_RADDR_SDR,
                                                                     kFLEXSPI_4PAD,
                                                                     NORFLASH_ADDR_LENGTH4B),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD_4BYTE + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_MODE8_SDR, kFLEXSPI_4PAD, 0x00, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x04),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD_4BYTE + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x0),

    /* Write enable*/
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_WriteEnable, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Page program QUAD*/
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD] = FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,
                                                                 kFLEXSPI_1PAD,
                                                                 W25Q_PageProgramQuad,
                                                                 kFLEXSPI_Command_RADDR_SDR,
                                                                 kFLEXSPI_1PAD,
                                                                 NORFLASH_ADDR_LENGTH),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Page program QUAD 4b*/
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD_4BYTE] = FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,
                                                                       kFLEXSPI_1PAD,
                                                                       W25Q_PageProgramQuad4B,
                                                                       kFLEXSPI_Command_RADDR_SDR,
                                                                       kFLEXSPI_1PAD,
                                                                       NORFLASH_ADDR_LENGTH4B),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD_4BYTE + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* ENTER 4B*/
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTER4BAADDRESS] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_Enter4BAddressMode, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* GET ID*/
    [4 * NOR_CMD_LUT_SEQ_IDX_READID] = FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,
                                                       kFLEXSPI_1PAD,
                                                       W25Q_ManufactDeviceID,
                                                       kFLEXSPI_Command_DUMMY_SDR,
                                                       kFLEXSPI_1PAD,
                                                       NORFLASH_ADDR_LENGTH),

    [4 * NOR_CMD_LUT_SEQ_IDX_READID + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUSREG] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_ReadStatusReg1, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUSREG2] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_ReadStatusReg2, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUSREG3] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_ReadStatusReg3, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

    [4 * NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_WriteStatusReg2, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04),

    /* ERASE SECTOR */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] = FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,
                                                            kFLEXSPI_1PAD,
                                                            W25Q_SectorErase,
                                                            kFLEXSPI_Command_RADDR_SDR,
                                                            kFLEXSPI_1PAD,
                                                            NORFLASH_ADDR_LENGTH),

    /* ERASE SECTOR 4B*/
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_4BYTE] = FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,
                                                                  kFLEXSPI_1PAD,
                                                                  W25Q_SectorErase4B,
                                                                  kFLEXSPI_Command_RADDR_SDR,
                                                                  kFLEXSPI_1PAD,
                                                                  NORFLASH_ADDR_LENGTH4B),

    [4 * NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK] = FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,
                                                           kFLEXSPI_1PAD,
                                                           W25Q_BlockErase,
                                                           kFLEXSPI_Command_RADDR_SDR,
                                                           kFLEXSPI_1PAD,
                                                           NORFLASH_ADDR_LENGTH),

};

/*${variable:end}*/

/*${function:start}*/
/*${function:end}*/
