/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_soc_src.h"
#include "board.h"
#include "app.h"
#include "display_support.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_ResetDisplayMix(void)
{
    /*
     * Reset the displaymix, otherwise during debugging, the
     * debugger may not reset the display, then the behavior
     * is not right.
     */
    SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);
    while (kSRC_SliceResetInProcess == SRC_GetSliceResetState(SRC, kSRC_DisplaySlice))
    {
    }
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();
    BOARD_ResetDisplayMix();
    BOARD_InitLpuartPins();
    BOARD_InitMipiPanelPins();
#if (DEMO_PANEL == DEMO_PANEL_RASPI_7INCH)
    BOARD_MIPIPanelTouch_I2C_Init();
    BOARD_InitLpi2cPins();
#endif
    BOARD_InitDebugConsole();
}
/*${function:end}*/
