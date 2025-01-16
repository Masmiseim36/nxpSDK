/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_mu.h"
#include "app.h"

#include "board.h"
#include "fsl_pm_core.h"
#include "fsl_pm_device.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#define BOOT_FLAG              0x01U
#define REV_TARGET_MODE_FLAG   0x02U
#define REV_TIMEOUT_VALUE_FLAG 0x03U
/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_ALWAYS_ON_DATA(pm_handle_t g_pmHandle);
AT_ALWAYS_ON_DATA(uint8_t g_targetPowerMode);
AT_ALWAYS_ON_DATA(uint32_t g_irqMask);

static volatile uint32_t g_msgRecv;
static volatile bool isMsgReceived = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

void MU_IRQ_HANDLER(void)
{
    if (kMU_Rx0FullFlag & MU_GetStatusFlags(MU_BASE))
    {
        g_msgRecv     = MU_ReceiveMsgNonBlocking(MU_BASE, APP_CHN_MU_REG_NUM);
        isMsgReceived = true;
        /* We do not disable MU interrupt here since we always get input from core0. */
    }
    if (kMU_GenInt0Flag & MU_GetStatusFlags(MU_BASE))
    {
        // Primary core trigger general interrupt.
        MU_ClearStatusFlags(MU_BASE, kMU_GenInt0Flag);
    }
    SDK_ISR_EXIT_BARRIER;
}

int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    MU_Init(MU_BASE);

    // Sync with primary core.
    MU_SetFlags(MU_BASE, BOOT_FLAG);
    // Enable General Interrupt and Receive Interrupt.
    MU_EnableInterrupts(MU_BASE, APP_MU_GENINT_ENABLE | APP_MU_RXFULLINT_ENABLE);
    EnableIRQ(MU_IRQ);

    /* Init Power Manager */
    PM_CreateHandle(&g_pmHandle);

    APP_RegisterNotify();
    APP_InitWakeupSource();
    while (1)
    {
        // Polling to get target power mode from primary core.
        while (!isMsgReceived)
        {
        }
        isMsgReceived     = false;
        g_targetPowerMode = (uint8_t)g_msgRecv;

        // Inform primary core that target power mode is received.
        MU_SetFlags(MU_BASE, REV_TARGET_MODE_FLAG);

        while (!isMsgReceived)
        {
        }
        isMsgReceived      = false;
        uint32_t timeoutUs = g_msgRecv;
        // Inform primary core that timeout is received.
        MU_SetFlags(MU_BASE, REV_TIMEOUT_VALUE_FLAG);

        APP_SetConstraints(g_targetPowerMode);
        g_irqMask = DisableGlobalIRQ();
        PM_EnablePowerManager(true);
        PM_EnterLowPower(timeoutUs);
        PM_EnablePowerManager(false);
        EnableGlobalIRQ(g_irqMask);
        APP_ReleaseConstraints(g_targetPowerMode);
    }
}
