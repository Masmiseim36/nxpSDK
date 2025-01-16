/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pgmc.h"
/*${header:end}*/

/*${function:start}*/
void APP_TriggerPMICStandby(bool enable)
{
    PGMC_PPC_TriggerPMICStandbySoftMode(PGMC_PPC0, enable);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

/*${function:end}*/
