/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_debug_console.h"
#include "app.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_inputmux.h"
#include "board.h"
/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Enables the clock for the Input Mux */
    CLOCK_EnableClock(kCLOCK_InputMux);
    /* MUB interrupt signal is selected for DSP interrupt input 1 */
    INPUTMUX_AttachSignal(INPUTMUX, 1U, kINPUTMUX_MuBToDspInterrupt);
}

/*${function:end}*/
