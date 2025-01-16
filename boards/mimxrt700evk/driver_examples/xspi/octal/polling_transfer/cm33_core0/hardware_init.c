/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_xspi.h"
/*${header:end}*/

/*${variable:start}*/
xspi_device_ddr_config_t flashDDrConfig = 
{
    .ddrDataAlignedClk = kXSPI_DDRDataAlignedWith2xInternalRefClk,
    .enableDdr = true,
    .enableByteSwapInOctalMode = false,
};
/*!
 * @brief Configuration for MX25UW51345GXD100.
 * 
 * General Features:
 *      - Density: 512 Mb
 *      - Page Size: 256 byte
 *      - Support Data strobe signal
 *      - Maximum frequency(DDR mode):
 *          - Single I/O mode: 133MHz
 *          - Octal I/O mode:  200MHz
 */
xspi_device_config_t deviceConfig =
{
    .xspiRootClk = 400000000,  /*!< 400MHz */
    .enableCknPad = false,  /*!< Do not support differential clock */      
    .deviceInterface = kXSPI_StrandardExtendedSPI,  /*!< Support Single IO and Octal IO */
    .interfaceSettings.strandardExtendedSPISettings.pageSize = FLASH_PAGE_SIZE,   /*!< 256 byte page buffer. */
    .CSHoldTime = 3,
    .CSSetupTime = 3,
    .sampleClkConfig.sampleClkSource = kXSPI_SampleClkFromExternalDQS,  /*!< Device support Data strobe signal.  */
    .sampleClkConfig.enableDQSLatency = false,
    .sampleClkConfig.dllConfig.dllMode = kXSPI_AutoUpdateMode,
    .sampleClkConfig.dllConfig.useRefValue = true,
    .sampleClkConfig.dllConfig.enableCdl8 = true,
    .ptrDeviceDdrConfig = &flashDDrConfig,
    .addrMode = kXSPI_DeviceByteAddressable,
    .columnAddrWidth = 0U,
    .enableCASInterleaving = false,
    .deviceSize[0] = FLASH_SIZE,
    .deviceSize[1] = FLASH_SIZE,   /*!< Single DIE flash, so deviceSize1 should equal to deviceSize0. */
    .ptrDeviceRegInfo = NULL,      /*!< Not used in this example. */
};

/* 
 * Errata ERR052528: Limitation on LUT-Data Size < 8byte in xspi.
 * Description: Read command including RDSR command can't work if LUT data size in read status is less than 8.
 * Workaround: Use LUT data size of minimum 8 byte for read commands including RDSR.
 */
const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
    /*Read*/
    [5 * NOR_CMD_LUT_SEQ_IDX_READ] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xEE, kXSPI_Command_DDR, kXSPI_8PAD, 0x11),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x12),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 2] =
        XSPI_LUT_SEQ(kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x2, kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x8),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 3] =
        XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_8PAD, 0x0, 0, 0, 0),

    /*Read status SPI*/
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x05, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08),

    /* Read Status OPI */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x05, kXSPI_Command_DDR, kXSPI_8PAD, 0xFA),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x12),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI + 2] =
        XSPI_LUT_SEQ(kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x2, kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x8),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI + 3] =
        XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_8PAD, 0x0, 0, 0, 0),

    /*Write enable*/
    [5 * NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x06, kXSPI_Command_STOP, kXSPI_1PAD, 0x04),

    /* Write Enable - OPI */
    [5 * NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x06, kXSPI_Command_DDR, kXSPI_8PAD, 0xF9),

    /* Read ID */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI + 0] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x9F, kXSPI_Command_DDR, kXSPI_8PAD, 0x60),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI + 1] = XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x04), /*address is 0x00,0x00,0x00,0x00*/
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI + 2] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* Erase Sector */
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_SECTOR] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x21, kXSPI_Command_DDR, kXSPI_8PAD, 0xDE),
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_SECTOR + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_STOP, kXSPI_8PAD, 0x0),

    /* Enable OPI DDR mode */
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x72, kXSPI_Command_SDR, kXSPI_1PAD, 0x00),
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_OPI + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x00, kXSPI_Command_SDR, kXSPI_1PAD, 0x00),
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_OPI + 2] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x00, kXSPI_Command_WRITE_SDR,  kXSPI_1PAD, 0x01),

    /* Page program */
    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x12, kXSPI_Command_DDR, kXSPI_8PAD, 0xED),
    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x8),

    /* Erase Chip */
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_CHIP] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x60, kXSPI_Command_DDR, kXSPI_8PAD, 0x9F),
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitNorFlashPins();

    CLOCK_AttachClk(kMAIN_PLL_PFD1_to_XSPI0);
    CLOCK_SetClkDiv(kCLOCK_DivXspi0Clk, 1u);     /*400MHz*/
}
/*${function:end}*/
