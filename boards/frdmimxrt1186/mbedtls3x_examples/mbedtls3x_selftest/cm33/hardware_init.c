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
#include "ele_crypto.h"
#include "fsl_debug_console.h"
/*${header:end}*/

extern uint8_t ele_fw[];

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* We need FW to be loaded in order to get entropy */
    if (ELE_LoadFw(MU_RT_S3MUA, ele_fw) != kStatus_Success) {
        PRINTF("Load FW failed\n");
    }

}
/*${function:end}*/
