/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "shell.h"
#include "clock_config.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_BootClockRUN();

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
    /* Set up Serial interface clock for SHELL, pin mux will be configured from serial Manager directly */
    BOARD_SerialClkInit();
    shell_init("");
    BOARD_SetSerialManagerInterface(shell_get_interface());
#endif

#if defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1)
    BOARD_InitDebugConsole();
#endif
}
/*${function:end}*/
