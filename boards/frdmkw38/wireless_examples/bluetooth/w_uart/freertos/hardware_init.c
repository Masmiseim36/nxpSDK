/*
 * Copyright 2019-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_BootClockRUN();
}
/*${function:end}*/
