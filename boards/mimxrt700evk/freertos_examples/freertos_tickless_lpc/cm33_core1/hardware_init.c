/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include <stdbool.h>
#include "FreeRTOS.h"
#include "fsl_power.h"
#if configUSE_TICKLESS_IDLE == 2
#include "fsl_ostimer.h"
#include "app.h"
#endif
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitAHBSC();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    
    CLOCK_AttachClk(k32KHZ_WAKE_to_OSTIMER);
    CLOCK_SetClkDiv(kCLOCK_DivOstimerClk, 1U);
    RESET_ClearPeripheralReset(kPINT_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Pint);
}

#if configUSE_TICKLESS_IDLE == 2

/*!
 * @brief Fuction of OS timer.
 *
 * This function to return OS timer base address
 */

OSTIMER_Type *vPortGetOstimerBase(void)
{
    return TICKLESS_OSTIMER_BASE_PTR;
}

/*!
 * @brief Fuction of OS timer.
 *
 * This function to return OS timer interrupt number
 */

IRQn_Type vPortGetOstimerIrqn(void)
{
    return TICKLESS_OSTIMER_IRQn;
}
#endif
/*${function:end}*/
