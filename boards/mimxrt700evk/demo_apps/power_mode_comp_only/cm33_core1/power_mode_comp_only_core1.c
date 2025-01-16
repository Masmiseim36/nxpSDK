/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_common.h"
#include "fsl_power.h"
#include "power_demo_config.h"
#include "fsl_lpuart.h"
#include "fsl_mu.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if (DEMO_POWER_CPU1_PRINT_ENABLE != 0U)
#define DEMO_LOG PRINTF
#else
#define DEMO_LOG(...)
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_targetMode = 0U;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/
#if (defined(APP_USE_PMC_INT) && (APP_USE_PMC_INT))
void PMC_IRQHandler(void)
{
    if ((POWER_GetEventFlags() & APP_PMC_INT_STATUS_MASK) == APP_PMC_INT_STATUS_MASK)
    {
        POWER_ClearEventFlags(APP_PMC_INT_STATUS_MASK);
        DEMO_LOG("The other CPU enterred Deep Sleep\r\n");
    }
}
#endif
void APP_MU_IRQHandler(void)
{
    if ((MU_GetStatusFlags(APP_MU) & kMU_Rx0FullFlag) == kMU_Rx0FullFlag)
    {
        g_targetMode = MU_ReceiveMsgNonBlocking(APP_MU, APP_MU_REG);
    }

    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
#if (DEMO_POWER_CPU1_PRINT_ENABLE != 0U)
    uint32_t freq = 0U;
#endif

    /* Init board hardware. */
    BOARD_InitHardware();

    /* Determine the power mode before bring up. */
    if ((POWER_GetEventFlags() & PMC_FLAGS_DEEPPDF_MASK) != 0)
    {
        DEMO_LOG("Board wake up from deep or full deep power down mode.\r\n");
        POWER_ClearEventFlags(PMC_FLAGS_DEEPPDF_MASK);
    }
    POWER_ClearEventFlags(0xFFFFFFFF);

    BOARD_NotifyBoot(); /* Set boot flag. */

#if APP_WAIT_INPUT_TO_START
    DEMO_LOG("Input any key to start the demo\r\n");
    GETCHAR();
#endif

    while (1)
    {
#if (DEMO_POWER_CPU1_PRINT_ENABLE != 0U)
        freq = CLOCK_GetCoreSysClkFreq();

        DEMO_LOG("\r\n####################  Power Mode Demo - %s ####################\r\n", APP_CORE_NAME);
        DEMO_LOG("    Build Time: %s--%s \r\n", __DATE__, __TIME__);
        DEMO_LOG("    Core Clock: %dHz \r\n", freq);
#endif

        switch (g_targetMode)
        {
            case DEMO_EVENT_ENTER_DPD:
                DEMO_LOG("Entering DPD...\r\n");
                BOARD_RequestDPD(APP_EXCLUDE_FROM_DEEP_POWERDOWN);
                break;

            case DEMO_EVENT_ENTER_FDPD:
                DEMO_LOG("Entering FDPD...\r\n");
                BOARD_RequestFDPD(APP_EXCLUDE_FROM_FULL_DEEP_POWERDOWN);
                break;

            default:
                DEMO_LOG("Entering Deep sleep...\r\n");
                g_targetMode = (uint32_t)kPower_DeepSleep;
                BOARD_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);
                break;
        }

        DEMO_LOG("Wakeup.\r\n");
    }
}
