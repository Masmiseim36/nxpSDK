/*
* Copyright 2019-2020 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
 
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "PWR_Interface.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();

#if defined(gDCDC_Enabled_d) && (gDCDC_Enabled_d)
        /* Init DCDC module */
        BOARD_DCDCInit();
#endif

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
    /* Set up Serial interface clock , pin mux will be configured from serial Manager directly */
    BOARD_SerialClkInit();
    BOARD_InitSerialManager();
#endif

#if defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1)
    BOARD_InitDebugConsole();
#endif

    /* Configure the keyboard wakeup pin */
    PWR_SetWakeupPins(BOARD_WAKEUP_PIN_KEYBOARD_BITMAP);
}
/*${function:end}*/
