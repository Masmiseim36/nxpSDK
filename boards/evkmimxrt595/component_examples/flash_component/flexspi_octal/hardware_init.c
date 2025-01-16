/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "app.h"
#include "fsl_flexspi.h"
#include "fsl_flexspi_nor_flash.h"
#include "fsl_nor_flash.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
flexspi_mem_config_t mem_Config = {
    .deviceConfig =
        {
            .flexspiRootClk       = 99000000,
            .flashSize            = FLASH_SIZE,
            .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
            .CSInterval           = 2,
            .CSHoldTime           = 3,
            .CSSetupTime          = 3,
            .dataValidTime        = 2,
            .columnspace          = 0,
            .enableWordAddress    = 0,
            .AWRSeqIndex          = 0,
            .AWRSeqNumber         = 0,
            .ARDSeqIndex          = NOR_CMD_LUT_SEQ_IDX_READ,
            .ARDSeqNumber         = 1,
            .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
            .AHBWriteWaitInterval = 0,
        },
    .devicePort         = kFLEXSPI_PortA1,
    .deviceType         = kSerialNorCfgOption_DeviceType_MacronixOctalDDR,
    .CurrentCommandMode = kSerialNorCommandMode_1_1_1,
    .transferMode       = kSerialNorTransferMode_DDR,
    .quadMode           = kSerialNorQuadMode_NotConfig,
    .enhanceMode        = kSerialNorEnhanceMode_Disabled,
    .commandPads        = kFLEXSPI_8PAD,
    .queryPads          = kFLEXSPI_1PAD,
    .busyOffset         = 0,
    .busyBitPolarity    = 0,

};

nor_config_t norConfig = {
    .memControlConfig = &mem_Config,
    .driverBaseAddr   = EXAMPLE_FLEXSPI,
};

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    POWER_DisablePD(kPDRUNCFG_APD_FLEXSPI0_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_FLEXSPI0_SRAM);
    POWER_ApplyPD();

    CLOCK_AttachClk(kAUX0_PLL_to_FLEXSPI0_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivFlexspi0Clk, 4);
}

/*${function:end}*/
