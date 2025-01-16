/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_soc_src.h"
#include "clock_config.h"
#include "board.h"
#include "camera_support.h"
/*${header:end}*/

/*${function:start}*/

static void BOARD_ResetDisplayMix(void)
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
    BOARD_EarlyInitCamera();
    BOARD_InitLpuartPins();
    BOARD_InitMipiPanelPins();
    BOARD_InitMipiCameraPins();
    BOARD_InitDebugConsole();
}

/*${function:end}*/
