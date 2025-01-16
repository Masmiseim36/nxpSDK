/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_common.h"
#include "fsl_power.h"
#include "fsl_inputmux.h"
#include "fsl_pint.h"
#include "fsl_usart.h"
#include "pmic_support.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
const char *gWakeupInfoStr[] = {"Sleep [Press the user key to wakeup]", "Deep Sleep [Press the user key to wakeup]",
#if (defined(FSL_FEATURE_SYSCON_HAS_POWERDOWN_MODE) && FSL_FEATURE_SYSCON_HAS_POWERDOWN_MODE)
                                "Powerdown [Reset to wakeup]", "Deep Powerdown [Reset to wakeup]"};
#else
                                "Deep Powerdown [Reset to wakeup]", "Full Deep Powerdown [Reset to wakeup]"};
#endif
uint32_t gCurrentPowerMode;
volatile bool pintFlag = false;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static uint32_t APP_GetUserSelection(void);
static void APP_InitWakeupPin(void);
static void pint_intr_callback(pint_pin_int_t pintr, uint32_t pmatch_status);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    /* Determine the power mode before bring up. */
    if ((POWER_GetEventFlags() & PMC_FLAGS_DEEPPDF_MASK) != 0)
    {
        PRINTF("Board wake up from deep or full deep power down mode.\r\n");
        POWER_ClearEventFlags(PMC_FLAGS_DEEPPDF_MASK);
    }

    APP_InitWakeupPin();

    PRINTF("Power Manager Demo.\r\n");
    PRINTF("The \"user key\" is: %s\r\n", APP_USER_WAKEUP_KEY_NAME);

    while (1)
    {
        gCurrentPowerMode = APP_GetUserSelection();
        PRINTF("Entering %s ...\r\n", gWakeupInfoStr[gCurrentPowerMode]);
        pintFlag = false;
        /* Enter the low power mode. */
        switch (gCurrentPowerMode)
        {
            case kPmu_Sleep: /* Enter sleep mode. */
                POWER_EnterSleep();
                break;
            case kPmu_Deep_Sleep: /* Enter deep sleep mode. */
                BOARD_SetPmicVoltageBeforeDeepSleep();
#if POWER_DOWN_PLL_BEFORE_DEEP_SLEEP
                /* Disable Pll before enter deep sleep mode */
                BOARD_DisablePll();
#endif
                BOARD_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);
#if POWER_DOWN_PLL_BEFORE_DEEP_SLEEP
                /* Restore Pll before enter deep sleep mode */
                BOARD_RestorePll();
#endif
                BOARD_RestorePmicVoltageAfterDeepSleep();
                break;
            case kPmu_Deep_PowerDown: /* Enter deep power down mode. */
                PRINTF(
                    "Press any key to confirm to enter the deep power down mode and wakeup the device by "
                    "reset.\r\n\r\n");
                GETCHAR();
                BOARD_SetPmicVoltageBeforeDeepPowerDown();
                BOARD_EnterDeepPowerDown(APP_EXCLUDE_FROM_DEEP_POWERDOWN);
                /* After deep power down wakeup, the code will restart and cannot reach here. */
                break;
            case kPmu_Full_Deep_PowerDown: /* Enter full deep power down mode. */
                PRINTF(
                    "Press any key to confirm to enter the full deep power down mode and wakeup the device by "
                    "reset.\r\n\r\n");
                GETCHAR();
                BOARD_SetPmicVoltageBeforeDeepPowerDown();
                POWER_EnterFullDeepPowerDown(APP_EXCLUDE_FROM_FULL_DEEP_POWERDOWN);
                /* After full deep power down wakeup, the code will restart and cannot reach here. */
                break;
            default:
                break;
        }
        if (pintFlag)
        {
            PRINTF("Pin event occurs\r\n");
        }
        PRINTF("Wakeup.\r\n");
    }
}

/*
 * Setup a GPIO input pin as wakeup source.
 */
static void APP_InitWakeupPin(void)
{
    gpio_pin_config_t gpioPinConfigStruct;

    /* Set SW pin as GPIO input. */
    gpioPinConfigStruct.pinDirection = kGPIO_DigitalInput;
    GPIO_PinInit(APP_USER_WAKEUP_KEY_GPIO, APP_USER_WAKEUP_KEY_PORT, APP_USER_WAKEUP_KEY_PIN, &gpioPinConfigStruct);

    /* Configure the Input Mux block and connect the trigger source to PinInt channle. */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, APP_USER_WAKEUP_KEY_INPUTMUX_SEL); /* Using channel 0. */
    INPUTMUX_Deinit(INPUTMUX); /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */

    /* Configure the interrupt for SW pin. */
    PINT_Init(PINT);
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge, pint_intr_callback);
    PINT_EnableCallback(PINT); /* Enable callbacks for PINT */

    EnableDeepSleepIRQ(PIN_INT0_IRQn);
}

/*
 * Callback function when wakeup key is pressed.
 */
static void pint_intr_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    pintFlag = true;
}

/*
 * Get user selection from UART.
 */
static uint32_t APP_GetUserSelection(void)
{
    uint32_t ch;

    /* Clear rx overflow error once it happens during low power mode. */
    if (APP_USART_RX_ERROR == (APP_USART_RX_ERROR & USART_GetStatusFlags((USART_Type *)BOARD_DEBUG_UART_BASEADDR)))
    {
        USART_ClearStatusFlags((USART_Type *)BOARD_DEBUG_UART_BASEADDR, APP_USART_RX_ERROR);
    }

    PRINTF(
        "Select an option\r\n"
        "\t1. Sleep mode\r\n"
        "\t2. Deep Sleep mode\r\n"
        "\t3. Deep power down mode\r\n"
        "\t4. Full deep power down mode\r\n");
    while (1)
    {
        ch = GETCHAR();
        if ((ch < '1') || (ch > '4')) /* Only '1', '2', '3' , '4'. */
        {
            continue;
        }
        else
        {
            ch = ch - '1'; /* Only 0, 1, 2 , 3 . */
            break;
        }
    }
    switch (ch)
    {
        case 0:
            ch = kPmu_Sleep;
            break;
        case 1:
            ch = kPmu_Deep_Sleep;
            break;
        case 2:
            ch = kPmu_Deep_PowerDown;
            break;
        case 3:
            ch = kPmu_Full_Deep_PowerDown;
            break;
        default:
            break;
    }
    return ch;
}
