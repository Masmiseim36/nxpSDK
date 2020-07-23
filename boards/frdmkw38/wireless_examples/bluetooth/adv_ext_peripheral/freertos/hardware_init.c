/*
 * Copyright 2019-2020 NXP
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
    BOARD_BootClockRUN();

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
    /* Set up Serial interface clock , pin mux will be configured from serial Manager directly */
    BOARD_SerialClkInit();
    BOARD_InitSerialManager();

    /* Configure the serial wakeup pins. It shall be done before PWRLib_init call */
    PWR_SetWakeupPinsSerial(BOARD_WAKEUP_PIN_SERIAL_BITMAP, BOARD_GetSerialManagerInterface());
#endif

    /* Configure the keyboard wakeup pin */
    PWR_SetWakeupPins(BOARD_WAKEUP_PIN_KEYBOARD_BITMAP);
}
/*${function:end}*/
