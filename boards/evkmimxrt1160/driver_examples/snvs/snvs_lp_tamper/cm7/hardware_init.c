/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_SNVS_TamperFuse()
{
    /* check tamper function setting */
    uint32_t fuse006;
    fuse006 = OCOTP->FUSEN[6].FUSE;
    if ((fuse006 & 0xc000) != 0)
    {
        PRINTF("Error: SNVS Tamper FUSE not enabled! /r/n");
    }
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_SNVS_TamperFuse();
}
/*${function:end}*/
