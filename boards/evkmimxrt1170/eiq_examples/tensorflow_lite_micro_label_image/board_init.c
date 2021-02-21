/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board_init.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "eiq_video_worker.h"
#include "board.h"

void BOARD_Init()
{
    BOARD_ConfigMPU();
    BOARD_InitLpuartPins();
    BOARD_BootClockRUN();
    BOARD_ResetDisplayMix();
    BOARD_InitDebugConsole();
}
