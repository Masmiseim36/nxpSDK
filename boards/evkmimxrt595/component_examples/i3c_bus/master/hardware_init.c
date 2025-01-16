/*
 * Copyright 2019, 2023 NXP
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
           .funcs = (i3c_device_hw_ops_t *)&master_ops, .resource = &demo_masterResource, .isSecondary = false};

void BOARD_InitHardware(void)
{
    /* Attach 24MHz to I3C. */
    CLOCK_AttachClk(kFRO_DIV8_to_I3C_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cClk, 1);

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    demo_masterResource.clockInHz = I3C_MASTER_CLOCK_FREQUENCY;
}
/*${function:end}*/
