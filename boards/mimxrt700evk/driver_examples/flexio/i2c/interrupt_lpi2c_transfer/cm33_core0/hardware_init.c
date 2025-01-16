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
#include "fsl_power.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    /* The domain own LPSPI14 and FLEXIO, disable mask in SHARED_MASK0. */
    POWER_DisableLPRequestMask(kPower_MaskFlexio);

    CLOCK_AttachClk(kFCCLK0_to_FLEXCOMM8);
    
    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_FLEXIO);
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 15U);
    
    RESET_ClearPeripheralReset(kFLEXIO0_RST_SHIFT_RSTn);
}
/*${function:end}*/
