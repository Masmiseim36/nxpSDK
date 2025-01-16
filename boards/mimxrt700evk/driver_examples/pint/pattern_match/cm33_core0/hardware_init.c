/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_pint.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Connect trigger sources to PINT, PINT should be enabled before configuring INPUTMUX. */
    RESET_ClearPeripheralReset(kPINT_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Pint);
    INPUTMUX_Init(INPUTMUX0);
    INPUTMUX_AttachSignal(INPUTMUX0, kPINT_PinInt0, DEMO_PINT_PIN_INT0_SRC);
    INPUTMUX_AttachSignal(INPUTMUX0, kPINT_PinInt1, DEMO_PINT_PIN_INT1_SRC);
    INPUTMUX_Deinit(INPUTMUX0);
}
/*${function:end}*/
