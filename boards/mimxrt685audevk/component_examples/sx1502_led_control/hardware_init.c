/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_sx1502.h"
/*${header:end}*/

/*${variable:start}*/
sx1502_config_t sx1502Config = {
    .initRegDataValue  = SX1502_INIT_REGDATA,
    .initRegDirValue   = SX1502_INIT_REGDIR,
    .sx1502I2CInstance = BOARD_SX1502_I2C_INSTANCE,
};

sx1502_handle_t sx1502Handle;
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM1);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    RESET_PeripheralReset(kHSGPIO0_RST_SHIFT_RSTn);

    sx1502Config.sx1502I2CSourceClock = CLOCK_GetFlexCommClkFreq(1U);
}
/*${function:end}*/
