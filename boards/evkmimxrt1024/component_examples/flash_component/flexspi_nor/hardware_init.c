/*
 * Copyright 2020 NXP
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
/*${header:end}*/

/*${function:start}*/
flexspi_mem_config_t mem_Config = {
    .deviceConfig =
        {
            .flexspiRootClk       = 120000000,
            .flashSize            = FLASH_SIZE,
            .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
            .CSInterval           = 2,
            .CSHoldTime           = 3,
            .CSSetupTime          = 3,
            .dataValidTime        = 0,
            .columnspace          = 0,
            .enableWordAddress    = 0,
            .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
            .AHBWriteWaitInterval = 0,
        },
    .devicePort      = kFLEXSPI_PortA1,
    .deviceType      = kSerialNorCfgOption_DeviceType_ReadSFDP_SDR,
    .quadMode        = kSerialNorQuadMode_NotConfig,
    .transferMode    = kSerialNorTransferMode_SDR,
    .enhanceMode     = kSerialNorEnhanceMode_Disabled,
    .commandPads     = kFLEXSPI_1PAD,
    .queryPads       = kFLEXSPI_1PAD,
    .statusOverride  = 0,
    .busyOffset      = 0,
    .busyBitPolarity = 0,

};

nor_config_t norConfig = {
    .memControlConfig = &mem_Config,
    .driverBaseAddr   = EXAMPLE_FLEXSPI,
};

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

/*${function:end}*/
