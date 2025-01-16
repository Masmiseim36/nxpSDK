/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "lcdif_support.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitMipiPanelPins();
    BOARD_InitPsRamPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitLcdifClock();

    GPIO_PortInit(GPIO, BOARD_MIPI_POWER_PORT);
    GPIO_PortInit(GPIO, BOARD_MIPI_BL_PORT);
    GPIO_PortInit(GPIO, BOARD_MIPI_RST_PORT);

    status_t status = BOARD_InitPsRam();
    if (status != kStatus_Success)
    {
        assert(false);
    }
}
/*${function:end}*/
