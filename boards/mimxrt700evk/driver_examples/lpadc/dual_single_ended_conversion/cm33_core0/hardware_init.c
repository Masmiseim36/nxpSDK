/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();
    
    POWER_DisablePD(kPDRUNCFG_PD_ADC0);
    
    CLOCK_AttachClk(kFRO1_DIV1_to_SENSE_MAIN);
    CLOCK_AttachClk(kSENSE_BASE_to_ADC);
    CLOCK_SetClkDiv(kCLOCK_DivAdcClk, 1U);
}
/*${function:end}*/
