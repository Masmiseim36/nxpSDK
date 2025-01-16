/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_common.h"
#include "fsl_pm_core.h"
#include "fsl_pm_device.h"

#include "fsl_debug_console.h"
#include "app.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static uint8_t APP_GetTargetPowerMode(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_ALWAYS_ON_DATA(pm_handle_t g_pmHandle);
AT_ALWAYS_ON_DATA(uint8_t g_targetPowerMode);
AT_ALWAYS_ON_DATA(uint32_t g_irqMask);
static const char *const g_targetPowerNameArray[APP_TARGET_POWER_NUM] = APP_POWER_NAME;

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    uint32_t timeoutUs = 0UL;

    BOARD_InitHardware();
    PRINTF("\r\nPower Manager Test.\r\n");
    PRINTF("\r\nNormal Boot.\r\n");
    PM_CreateHandle(&g_pmHandle);

    APP_RegisterNotify();
    APP_InitWakeupSource();
    while (1)
    {
        g_targetPowerMode = APP_GetTargetPowerMode();
        if (g_targetPowerMode >= APP_TARGET_POWER_NUM)
        {
            PRINTF("\r\nWrong Input! Please reselect.\r\n");
            continue;
        }
        PRINTF("Selected to enter %s.\r\n", g_targetPowerNameArray[(uint8_t)g_targetPowerMode]);
        timeoutUs = APP_GetWakeupTimeout();
        APP_SetConstraints(g_targetPowerMode);
        g_irqMask = DisableGlobalIRQ();
        PM_EnablePowerManager(true);
        PM_EnterLowPower(timeoutUs);
        PM_EnablePowerManager(false);
        EnableGlobalIRQ(g_irqMask);
        APP_ReleaseConstraints(g_targetPowerMode);
        PRINTF("\r\nNext Loop\r\n");
    }
}

static uint8_t APP_GetTargetPowerMode(void)
{
    uint32_t i;
    uint8_t ch;
    uint8_t g_targetPowerModeIndex;

    PRINTF("\r\nPlease select the desired power mode:\r\n");
    for (i = 0UL; i < APP_TARGET_POWER_NUM; i++)
    {
        PRINTF("\tPress %c to enter: %s\r\n", ('A' + i), g_targetPowerNameArray[i]);
    }

    PRINTF("\r\nWaiting for power mode select...\r\n\r\n");

    ch = GETCHAR();

    if ((ch >= 'a') && (ch <= 'z'))
    {
        ch -= 'a' - 'A';
    }

    g_targetPowerModeIndex = ch - 'A';

    return g_targetPowerModeIndex;
}
