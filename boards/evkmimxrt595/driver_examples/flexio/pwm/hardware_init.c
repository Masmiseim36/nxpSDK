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
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    const clock_frg_clk_config_t flexio_frg = {17U, kCLOCK_FrgMainClk, 255U, 0U};

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Clock setting for Flexio, 396MHz/33 = 12MHz */
    CLOCK_SetFRGClock(&flexio_frg);
    CLOCK_AttachClk(kFRG_to_FLEXIO);
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 33);

    RESET_ClearPeripheralReset(kFLEXIO_RST_SHIFT_RSTn);
}
/*${function:end}*/
