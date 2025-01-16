/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_component_i3c_adapter.h"
/*${header:end}*/

/*${function:start}*/
i3c_master_adapter_resource_t demo_masterResource = {.base      = EXAMPLE_MASTER,
                                                     .transMode = kI3C_MasterTransferInterruptMode};
i3c_device_control_info_t i3cMasterCtlInfo        = {
           .funcs = (i3c_device_hw_ops_t *)&master_ops, .resource = &demo_masterResource, .isSecondary = true};
void BOARD_InitHardware(void)
{
    /* Attach main clock to I3C, 396MHz / 4 = 99MHz. */
    CLOCK_AttachClk(kMAIN_CLK_to_I3C_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cClk, 4);

    /* Attach lposc_1m clock to I3C time control, clear halt for slow clock. */
    CLOCK_AttachClk(kLPOSC_to_I3C_TC_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cTcClk, 1);
    CLOCK_SetClkDiv(kCLOCK_DivI3cSlowClk, 1);

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    demo_masterResource.clockInHz = I3C_MASTER_CLOCK_FREQUENCY;
}
/*${function:end}*/
