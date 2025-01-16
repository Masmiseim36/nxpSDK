/*
 * Copyright 2020-2021 NXP
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
#include "fsl_common.h"
/*${header:end}*/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*${function:start}*/
/* Get FLEXSPI NOR Configuration Block */
void FLEXSPI_NorFlash_GetConfig(flexspi_nor_config_t *config)
{
    config->memConfig.tag              = FLEXSPI_CFG_BLK_TAG;
    config->memConfig.version          = FLEXSPI_CFG_BLK_VERSION;
    config->memConfig.readSampleClkSrc = kFLEXSPIReadSampleClk_LoopbackFromDqsPad;
    config->memConfig.serialClkFreq =
        kFLEXSPISerialClk_133MHz;          /* Serial Flash Frequencey.See System Boot Chapter for more details */
    config->memConfig.sflashA1Size   = FLASH_SIZE;
    config->memConfig.csHoldTime     = 3U; /* Data hold time, default value: 3 */
    config->memConfig.csSetupTime    = 3U; /* Date setup time, default value: 3 */
    config->memConfig.deviceType     = kFLEXSPIDeviceType_SerialNOR; /* Flash device type default type: Serial NOR */
    config->memConfig.deviceModeType = kDeviceConfigCmdType_Generic;
    config->memConfig.columnAddressWidth  = 0U;
    config->memConfig.deviceModeCfgEnable = 0U;
    config->memConfig.waitTimeCfgCommands = 0U;
    config->memConfig.configCmdEnable     = 0U;
    /* Always enable Safe configuration Frequency */
    config->memConfig.controllerMiscOption = FSL_ROM_FLEXSPI_BITMASK(kFLEXSPIMiscOffset_SafeConfigFreqEnable);
    config->memConfig.sflashPadType = kSerialFlash_4Pads; /* Pad Type: 1 - Single, 2 - Dual, 4 - Quad, 8 - Octal */
    config->pageSize                = FLASH_PAGE_SIZE;
    config->sectorSize              = FLASH_SECTOR_SIZE;
    config->blockSize               = FLASH_BLOCK_SIZE;
    config->ipcmdSerialClkFreq      = kFLEXSPISerialClk_30MHz; /* Clock frequency for IP command */

    /* Fast Read Quad I/O */
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READ + 0U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xebU, RADDR_SDR, FLEXSPI_4PAD, 0x18U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READ + 1U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_4PAD, 0x06U, READ_SDR, FLEXSPI_4PAD, 0x4U);

    /* Read Status */
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x05U, READ_SDR, FLEXSPI_1PAD, 0x1U);

    /* Write Enable */
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x06U, STOP, FLEXSPI_1PAD, 0x0U);

    /* Page Program - quad mode */
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 0U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x32U, RADDR_SDR, FLEXSPI_1PAD, 0x18U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(WRITE_SDR, FLEXSPI_4PAD, 0x04U, STOP, FLEXSPI_1PAD, 0x0U);

    /* Sector Erase */
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x20U, RADDR_SDR, FLEXSPI_1PAD, 0x18U);

    /* Block Erase */
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xD8U, RADDR_SDR, FLEXSPI_1PAD, 0x18U);
}
#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
/* Configure clock for FlexSPI peripheral */
void BOARD_SetupFlexSpiClock(void)
{
    /* Disable FlexSPI peripheral */
    FLEXSPI->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;
    CLOCK_DisableClock(kCLOCK_FlexSpi);

    /* Init Usb1 PLL3 to 480MHZ. */
    CLOCK_InitUsb1Pll(&usb1PllConfig_BOARD_BootClockRUN);
    /* Init Usb1 PLL3->pfd0 360MHZ. */
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 24);
    /* Enable Usb1 PLL output for USBPHY1. */
    CCM_ANALOG->PLL_USB1 |= CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;

    /* Set FLEXSPI_PODF, FlexSPI out clock is 60MHZ. */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 5);
    /* Set flexspi clock source to PLL3->pfd0 */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 3);

    CLOCK_EnableClock(kCLOCK_FlexSpi);
    /* Enable FlexSPI peripheral */
    FLEXSPI->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
}
#endif

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
    BOARD_SetupFlexSpiClock();
#endif
    BOARD_InitDebugConsole();
}
/*${function:end}*/
