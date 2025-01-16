/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/********************** End of forword declaration. ***************/
/*${function:start}*/
void GPIO_ClearStopRequest(void)
{
    CCM->GPR_SHARED8 &= ~CCM_GPR_SHARED8_m33_gpio1_ipg_stop_MASK;
    CCM->GPR_SHARED12 &= ~CCM_GPR_SHARED12_m7_gpio1_ipg_stop_MASK;
}

void GPIO_SetStopRequest(void)
{
    CCM->GPR_SHARED8 |= CCM_GPR_SHARED8_m33_gpio1_ipg_stop_MASK;
    CCM->GPR_SHARED12 |= CCM_GPR_SHARED12_m7_gpio1_ipg_stop_MASK;
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
