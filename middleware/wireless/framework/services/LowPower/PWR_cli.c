/* -------------------------------------------------------------------------- */
/*                             Copyright 2023 NXP                             */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "PWR_Interface.h"
#include "PWR_cli.h"

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "timers.h"
#include "portable.h"

/* -------------------------------------------------------------------------- */
/*                             Private definitions                            */
/* -------------------------------------------------------------------------- */

#define LOWPOWER_DEFAULT_ENABLE_DURATION_MS (10000U)

#define CLI_PRINTF (void)PRINTF
#define CLI_SCANF  (void)SCANF
/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief FreeRTOS timer callback used to switch back to WFI
 *
 * \param[in] timer timer handle
 */
static void PWRCli_TimerCallback(TimerHandle_t timer_h);

/*!
 * \brief Updates low power constraints to apply a low power mode during a specific amount of time
 *
 * \param[in] nextMode low power mode to use during the next period
 * \param[in] timeMs duration of the next period in msec. If 0, the timer won't be updated nor started
 */
static void PWRCli_ConfigureNextLowPowerMode(PWR_LowpowerMode_t nextMode, uint32_t timeMs);

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static char const pwrCliMenu[] =
    "\r\n"
    "--------------------------------------------\r\n"
    "             PWR MENU\r\n"
    "--------------------------------------------\r\n"
    "  0 - Exit\r\n"
    "  1 - Refresh \r\n"
    "  2 - Configure next low power period\r\n"
    "  3 - Help\r\n";

static char const pwrCliModeMenu[] =
    "\r\n"
    "--------------------------------------------\r\n"
    "             PWR MODE MENU\r\n"
    "--------------------------------------------\r\n"
    "  0 - WFI\r\n"
    "  1 - Sleep\r\n"
    "  2 - DeepSleep\r\n"
    "  3 - PowerDown\r\n"
    "  4 - DeepPowerDown\r\n";

static char const pwrCliHelp[] =
    "\r\n"
    "This tool allows to select a low power mode which will be entered when the system is in idle.\r\n"
    "The user has to specify a specific amount of time while this mode will be applied.\r\n"
    "After this period, the default low power mode will be switched back to WFI.\r\n"
    "This is to make sure the serial interface becomes available again, because on some platforms the serial interface "
    "\r\n"
    "is not a wake up source for every low power modes. WFI will provide most compatibility.\r\n";

static PWR_LowpowerMode_t currentMode;
static TimerHandle_t      lpTimer = NULL;

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void PWRCli_Menu(void)
{
    uint32_t           choice;
    uint32_t           timeMs;
    static const char *scan_choice_str = "%u";

    static bool once = false;
    if (!once)
    {
        /* Create FreeRTOS timer which will be used to disable low power after a specific time */
        lpTimer = xTimerCreate("LP timer", (TickType_t)(LOWPOWER_DEFAULT_ENABLE_DURATION_MS / portTICK_PERIOD_MS),
                               pdFALSE, NULL, &PWRCli_TimerCallback);
        assert(lpTimer != NULL);
        once = true;
    }

    while (true)
    {
        CLI_PRINTF("%s\r\n", pwrCliMenu);
        CLI_PRINTF("Enter your choice : ");
        CLI_SCANF((char *)scan_choice_str, &choice);
        CLI_PRINTF("\r\n");

        switch (choice)
        {
            case 0u:
                break;

            case 1u:
                break;

            case 2u:
            {
                PWR_LowpowerMode_t nextMode;
                CLI_PRINTF("%s \r\n", pwrCliModeMenu);
                CLI_PRINTF("Enter your choice : ");
                CLI_SCANF((char *)scan_choice_str, &choice);
                CLI_PRINTF("\r\n");
                nextMode = (PWR_LowpowerMode_t)choice;
                CLI_PRINTF("How much time do you want to apply this mode ? (in msec) : ");
                CLI_SCANF((char *)scan_choice_str, &choice);
                CLI_PRINTF("\r\n");
                timeMs = choice;
                PWRCli_ConfigureNextLowPowerMode(nextMode, timeMs);
            }
            break;

            case 3u:
                CLI_PRINTF("%s\r\n", pwrCliHelp);
                break;

            default:
                CLI_PRINTF("Invalid Choice\r\n");
                break;
        }
        if (choice == 0u)
        {
            /* exit the CLI menu */
            break;
        }
    }
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static void PWRCli_TimerCallback(TimerHandle_t timer_h)
{
    (void)timer_h;

    CLI_PRINTF("\r\nLow power period ended\r\n");

    PWRCli_ConfigureNextLowPowerMode(PWR_WFI, 0U);
}

static void PWRCli_ConfigureNextLowPowerMode(PWR_LowpowerMode_t nextMode, uint32_t timeMs)
{
    (void)PWR_ReleaseLowPowerModeConstraint(currentMode); /* MISRA CID 26556646 */
    (void)PWR_SetLowPowerModeConstraint(nextMode);
    currentMode = nextMode;

    const char *ModeName[5] = {"WFI", "Sleep", "DeepSleep", "PowerDown", "DeepPowerDown"};

    if (currentMode <= PWR_DeepPowerDown)
    {
        CLI_PRINTF("%s  mode selected\r\n", ModeName[currentMode]); /* MISRA CID 26556642 */
    }
    else
    {
        assert(0);
    }

    if (timeMs > 0U)
    {
        /* Update timer duration */
        (void)xTimerChangePeriod(lpTimer, timeMs / portTICK_PERIOD_MS, 0);

        /* Start the timer, during this time, the configured low power mode will be used as much as possible by
         * the system, when the timer expires, the low power mode will be limited to WFI until next command
         * This is to make sure the serial interface becomes available again to the user */
        if (xTimerStart(lpTimer, 0) != pdPASS)
        {
            assert(0);
        }
    }
}
