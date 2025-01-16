/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_inputmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Configure DMAMUX. */
    RESET_PeripheralReset(kINPUTMUX_RST_SHIFT_RSTn);

    INPUTMUX_Init(INPUTMUX);
    /* Enable DMA request */
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmic0Ch0ToDmac0Ch16RequestEna, true);
    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

    /* DMIC uses 48MHz FRO clock */
    CLOCK_AttachClk(kFRO_DIV4_to_DMIC);
    /*48MHz divided by 60 = 800 KHz PDM clock --> gives 16kHz sample rate */
    CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 60);
}
/*${function:end}*/
