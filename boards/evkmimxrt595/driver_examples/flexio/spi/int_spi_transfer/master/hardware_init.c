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
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Clock setting for Flexio */
    CLOCK_AttachClk(FLEXIO_CLOCK_SRC);
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, FLEXIO_CLOCK_DIVIDER);

    /* Use 48 MHz clock for the FLEXCOMM5 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM5);

    RESET_ClearPeripheralReset(kFLEXIO_RST_SHIFT_RSTn);
}
/*${function:end}*/
