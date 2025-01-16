/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
#include "app.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM);
    POWER_DisablePD(kPDRUNCFG_DSR_VDD2N_MEDIA);
    POWER_ApplyPD();

    /* The domain own FLEXIO, disable mask in SHARED_MASK0. */
    POWER_DisableLPRequestMask(kPower_MaskFlexio);

    CLOCK_AttachClk(kFRO1_DIV1_to_LPSPI16);
    CLOCK_SetClkDiv(kCLOCK_DivLpspi16Clk, 16U);
    CLOCK_AttachClk(kFRO1_DIV1_to_FLEXIO);
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 16U);

    RESET_ClearPeripheralReset(kFLEXIO0_RST_SHIFT_RSTn);
    RESET_ClearPeripheralReset(kLPSPI16_RST_SHIFT_RSTn);

    BOARD_InitAHBSC();
}
/*${function:end}*/
