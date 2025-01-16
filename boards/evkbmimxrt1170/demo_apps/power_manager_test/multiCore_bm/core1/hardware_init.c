/*
 * Copyright 2023 NXP
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

#include "fsl_clock.h"

#include "fsl_pm_core.h"
#include "fsl_pm_device.h"
/*${header:end}*/

/*${macro:start}*/

/*${macro:end}*/

/*${variable:start}*/
pm_wakeup_source_t g_MuWakeup;
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
}

void APP_RegisterNotify(void)
{
    return;
}

void APP_InitWakeupSource(void)
{
    // For secondary core, set MU interrupt as wakeup source.
    // Primary core will trigger general interrupt to wake up secondary core.
    PM_InitWakeupSource(&g_MuWakeup, PM_WSID_MUB_IRQ, NULL, true);
}

void APP_SetConstraints(uint8_t powerState)
{
    switch (powerState)
    {
        // setpoint 0
        case 0:
        {
            PM_SetConstraints(PM_LP_STATE_SP0, APP_SETPOINT0_CONSTRAINTS);
            break;
        }
        // setpoint 1
        case 1:
        {
            PM_SetConstraints(PM_LP_STATE_SP1, APP_SETPOINT1_CONSTRAINTS);
            break;
        }
        // setpoint 10
        case 2:
        {
            PM_SetConstraints(PM_LP_STATE_SP10, APP_SETPOINT10_CONSTRAINTS);
            break;
        }
        // setpoint 15
        case 3:
        {
            PM_SetConstraints(PM_LP_STATE_SP15, APP_SETPOINT15_CONSTRAINTS);
            break;
        }
    }
}

void APP_ReleaseConstraints(uint8_t powerState)
{
    switch (powerState)
    {
        // setpoint 0
        case 0:
        {
            PM_ReleaseConstraints(PM_LP_STATE_SP0, APP_SETPOINT0_CONSTRAINTS);
            break;
        }
        // setpoint 1
        case 1:
        {
            PM_ReleaseConstraints(PM_LP_STATE_SP1, APP_SETPOINT1_CONSTRAINTS);
            break;
        }
        // setpoint 10
        case 2:
        {
            PM_ReleaseConstraints(PM_LP_STATE_SP10, APP_SETPOINT10_CONSTRAINTS);
            break;
        }
        // setpoint 15
        case 3:
        {
            PM_ReleaseConstraints(PM_LP_STATE_SP15, APP_SETPOINT15_CONSTRAINTS);
            break;
        }
    }
}

/*${function:end}*/
