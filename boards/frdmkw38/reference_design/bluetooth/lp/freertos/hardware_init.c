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
#include "clock_config.h"
#include "PWR_Interface.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_BootClockRUN();

    /* for battery measurement */
    BOARD_InitAdc();

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
    /* Set up Serial interface clock , pin mux will be configured from serial Manager directly */
    BOARD_SerialClkInit();
    BOARD_InitSerialManager();
    /* Configure the serial wakeup pins. It shall be done before PWRLib_init call */
    PWR_SetWakeupPinsSerial(BOARD_WAKEUP_PIN_SERIAL_BITMAP, BOARD_GetSerialManagerInterface());

    /* Note : Set up the application callback can be done here by Serial_SetRxCallBack() but Application
        shall make sure the serial RX callback does not use any host stack ressources until
        the host stack is fully initialized                                           */
#endif

    /* Configure the keyboard wakeup pin */
    PWR_SetWakeupPins(BOARD_WAKEUP_PIN_KEYBOARD_BITMAP);

#if defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1)
    BOARD_InitDebugConsole();
#endif



}
/*${function:end}*/
