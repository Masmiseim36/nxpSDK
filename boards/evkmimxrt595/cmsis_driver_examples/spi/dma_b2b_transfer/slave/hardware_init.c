/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Configure DMAMUX. */
    RESET_PeripheralReset(kINPUTMUX_RST_SHIFT_RSTn);

    INPUTMUX_Init(INPUTMUX);
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM5);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Flexcomm5RxToDmac0Ch10RequestEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Flexcomm5TxToDmac0Ch11RequestEna, true);
    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

uint32_t SPI5_GetFreq(void)
{
    return CLOCK_GetFlexcommClkFreq(5U);
}
/*${function:end}*/
