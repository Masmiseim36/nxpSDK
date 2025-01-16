/*
 * Copyright 2020 NXP
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
void BOARD_PUF_Enable(void)
{
    /* Enable PUF in shadow registers */
    uint32_t fuse    = 0;
    uint32_t enabled = 0;

    /* Load walues from OCOTP */
    fuse = OCOTP->FUSEN[6].FUSE;

    enabled = (fuse & 0x00000040) >> 6U;

    /* Check whether PUF is disabled and FUSE word locked */
    if (enabled == 0)
    {
        PRINTF("Error: PUF is not enabled in FUSE (0x860[6])!/r/n");
        while (1)
            ;
    }

    /* Enable PUF in FUSE 6 shadow register (no FUSE itself) */
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_PUF_Enable();
}
/*${function:end}*/
