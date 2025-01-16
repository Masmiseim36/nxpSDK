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

#include "fsl_gpc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOOT_FLAG              0x01U
#define REV_TARGET_MODE_FLAG   0x02U
#define REV_TIMEOUT_VALUE_FLAG 0x03U

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
    /* Init board hardware. */
    BOARD_InitHardware();
    PRINTF("\r\nPower Manager Multi-Core Bare-metal Demo.\r\n");
    PRINTF("\r\nPrimary Core Boot.\r\n");

    APP_GetPreviousState();

    /* Init MU. */
    MU_Init(MU_BASE);

    PRINTF("Starting Second Core...\r\n");
    APP_BootSecondCore();
    /* Wait for Second Core is booted. */
    while (BOOT_FLAG != MU_GetFlags(MU_BASE))
    {
    }
    PRINTF("Second Core application has been started.\r\n");

    /* Init Power Manager. */
    PM_CreateHandle(&g_pmHandle);
    APP_InitWakeupSource();
    APP_RegisterNotify();

    while (1)
    {
        g_targetPowerMode = APP_GetTargetPowerMode();
        if (g_targetPowerMode >= APP_TARGET_POWER_NUM)
        {
            PRINTF("\r\nWrong Input! Please reselect.\r\n");
            continue;
        }
        /* Send target power mode to second core. */
        MU_SendMsg(MU_BASE, APP_CHN_MU_REG_NUM, g_targetPowerMode);

        /* Waiting conform from second core. */
        while (REV_TARGET_MODE_FLAG != MU_GetFlags(MU_BASE))
        {
        }

        PRINTF("Selected to enter %s.\r\n", g_targetPowerNameArray[(uint8_t)g_targetPowerMode]);
        uint32_t timeoutUs = APP_GetWakeupTimeout();
        /* Send timeout value to second core. */
        MU_SendMsg(MU_BASE, APP_CHN_MU_REG_NUM, timeoutUs);
        /* Waiting conform from second core. */
        while (REV_TIMEOUT_VALUE_FLAG != MU_GetFlags(MU_BASE))
        {
        }
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
