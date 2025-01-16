/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    // BOARD_InitDebugConsole();
    SystemCoreClockUpdate();

    POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM);
    POWER_DisablePD(kPDRUNCFG_DSR_VDD2N_MEDIA);
    POWER_ApplyPD();

    CLOCK_AttachClk(kFRO1_DIV1_to_LPSPI14);
    CLOCK_SetClkDiv(kCLOCK_DivLpspi14Clk, 8U);
    RESET_ClearPeripheralReset(kLPSPI14_RST_SHIFT_RSTn);

    RESET_ClearPeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio1);

    BOARD_InitAHBSC();
}
/*${function:end}*/
