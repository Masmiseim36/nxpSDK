/*
 * Copyright 2020, 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "display_support.h"
#include "camera_support.h"
#include "board.h"
#include "fsl_soc_src.h"

void BOARD_Init()
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitLpuartPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMipiCameraPins();
    /*
     * Reset the displaymix, otherwise during debugging, the
     * debugger may not reset the display, then the behavior
     * is not right.
     */
    SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);
    BOARD_InitMipiPanelPins();
    BOARD_MIPIPanelTouch_I2C_Init();
}
