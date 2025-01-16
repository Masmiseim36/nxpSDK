/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_common.h"
#include "fsl_power.h"
#include "pmic_support.h"
#include "power_demo_config.h"
#include "fsl_lpuart.h"
#include "fsl_gpio.h"
#include "fsl_irtc.h"
#include "fsl_iopctl.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum _app_wakeup_source
{
    kAPP_WakeupSourcePin,   /*!< Wakeup by external pin. */
    kAPP_WakeupSourceRtc,   /*!< Wakeup by RTC.        */
    kAPP_WakeupSourceReset, /*!< Wakeup by Reset. */
} app_wakeup_source_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
const char *gWakeupInfoStr[] = {"Sleep", "Deep Sleep", "Deep Sleep Retention", "Deep Powerdown [Reset to wakeup]",
                                "Full Deep Powerdown [Reset to wakeup]"};
uint32_t gCurrentPowerMode;

static uint32_t s_wakeupTimeout;           /* Wakeup timeout. (Unit: seconds) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static uint32_t APP_GetUserSelection(void);
static void APP_InitWakeupPin(void);
/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_SW_IRQ_HANDLER(void)
{
    POWER_ModuleExitLPRequest(APP_SW_LP_REQ); /* Clear the low power request before accessing the GPIO. */
    GPIO_GpioClearInterruptFlags(APP_USER_WAKEUP_KEY_GPIO, 1U << APP_USER_WAKEUP_KEY_PIN);
    DisableDeepSleepIRQ(APP_SW_IRQN);

    CLOCK_DisableClock(APP_SW_CLOCK_EN);
}

void APP_RTC_IRQ_HANDLER(void)
{
    uint32_t flags = IRTC_GetStatusFlags(APP_RTC);
    if ((flags & kIRTC_AlarmFlag) != 0U)
    {
        /* Unlock to allow register write operation */
        IRTC_SetWriteProtection(APP_RTC, false);
        /*Clear all irtc flag */
        IRTC_ClearStatusFlags(APP_RTC, flags);
    }

    DisableDeepSleepIRQ(APP_RTC_IRQN);
}

/*
 * Get user selection from UART.
 */
static uint32_t APP_GetUserSelection(void)
{
    uint32_t ch;

    /* Clear rx overflow error once it happens during low power mode. */
    if (APP_USART_RX_ERROR == (APP_USART_RX_ERROR & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
    {
        LPUART_ClearStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR, APP_USART_RX_ERROR);
    }

    DEMO_LOG(
        "Select an option\r\n"
        "\t1. Sleep mode\r\n"
        "\t2. Deep Sleep mode\r\n"
        "\t3. Deep Sleep Retention mode\r\n"
        "\t4. Deep power down mode\r\n"
        "\t5. Full deep power down mode\r\n"
        "\t6. Active test mode\r\n");
    while (1)
    {
        ch = GETCHAR();
        if ((ch < '1') || (ch > '6'))
        {
            continue;
        }
        else
        {
            ch = ch - '1';
            break;
        }
    }
    switch (ch)
    {
        case 0:
            ch = kPower_Sleep;
            break;
        case 1:
            ch = kPower_DeepSleep;
            break;
        case 2:
            ch = kPower_DeepSleepRetention;
            break;
        case 3:
            ch = kPower_DeepPowerDown;
            break;
        case 4:
            ch = kPower_FullDeepPowerDown;
            break;
        default:
            break;
    }
    return ch;
}

/* Get wakeup source by user input. */
static app_wakeup_source_t APP_GetWakeupSource(uint32_t mode)
{
    uint8_t ch;

    while (true)
    {
        DEMO_LOG("Select the wake up source:\r\n");
        if ((mode != kPower_DeepPowerDown) && (mode != kPower_FullDeepPowerDown))
        {
            DEMO_LOG("Press T for RTC.\r\n");
            if (mode != kPower_DeepSleepRetention)
            {
                DEMO_LOG("Press S for wakeup pin(%s).\r\n", APP_USER_WAKEUP_KEY_NAME);
            }
            DEMO_LOG("Press N for Reset.\r\n");
            DEMO_LOG("\r\nWaiting for key press..\r\n\r\n");
            ch = GETCHAR();
            DEMO_LOG("%c\r\n", ch);

            if ((ch >= 'a') && (ch <= 'z'))
            {
                ch -= 'a' - 'A';
            }

            if (ch == 'T')
            {
                return kAPP_WakeupSourceRtc;
            }
            else if ((ch == 'S') && (mode != kPower_DeepSleepRetention))
            {
                return kAPP_WakeupSourcePin;
            }
            else if (ch == 'N')
            {
                return kAPP_WakeupSourceReset;
            }
            else
            {
                DEMO_LOG("Wrong value!\r\n");
            }
        }
        else
        {
            DEMO_LOG("Reset to wakeup.\r\n");
            return kAPP_WakeupSourceReset;
        }
    }
}

/*!
 * @brief Get input from user about wakeup timeout
 */
static uint8_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;

    while (1)
    {
        DEMO_LOG("Select the wake up timeout in seconds.\r\n");
        DEMO_LOG("The allowed range is 1s ~ 9s.\r\n");
        DEMO_LOG("Eg. enter 5 to wake up in 5 seconds.\r\n");
        DEMO_LOG("\r\nWaiting for input timeout value...\r\n\r\n");

        timeout = GETCHAR();
        DEMO_LOG("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            return timeout - '0';
        }
        DEMO_LOG("Wrong value!\r\n");
    }
}

void APP_GetWakeupConfig(uint32_t targetMode)
{
    /* Get wakeup source by user input. */
    s_wakeupSource = APP_GetWakeupSource(targetMode);

    if (kAPP_WakeupSourceRtc == s_wakeupSource)
    {
        /* Wakeup source is Timer, user should input wakeup timeout value. */
        s_wakeupTimeout = APP_GetWakeupTimeout();
    }
    else
    {
        s_wakeupTimeout = 0U;
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
    gpioPinConfigStruct.outputLogic  = 0U;

    RESET_ClearPeripheralReset(APP_SW_RESET_RSTn);
    CLOCK_EnableClock(APP_SW_CLOCK_EN);

    GPIO_SetPinInterruptConfig(APP_USER_WAKEUP_KEY_GPIO, APP_USER_WAKEUP_KEY_PIN, kGPIO_InterruptFallingEdge);
    GPIO_PinInit(APP_USER_WAKEUP_KEY_GPIO, APP_USER_WAKEUP_KEY_PIN, &gpioPinConfigStruct);
}

static void APP_InitWakeupTimer(void)
{
    irtc_config_t irtcConfig;

    IRTC_GetDefaultConfig(&irtcConfig);
    if (IRTC_Init(APP_RTC, &irtcConfig) != kStatus_Success)
    {
        DEMO_LOG("RTC Init Failed.\r\n");
    }
}

static void APP_SetWakeupConfig(void)
{
    irtc_datetime_t datetime;

    if (s_wakeupSource == kAPP_WakeupSourceRtc)
    {
        /* Read the RTC seconds register to get current time in seconds */
        IRTC_GetDatetime(APP_RTC, &datetime);
        /* Add alarm seconds to current time */
        datetime.second += s_wakeupTimeout;
        if (datetime.second > 59U)
        {
            datetime.minute++;
            datetime.second -= 60U;
        }
        /* Unlock to allow register write operation */
        IRTC_SetWriteProtection(APP_RTC, false);
        /* Set alarm time in seconds */
        IRTC_SetAlarm(APP_RTC, &datetime);
        /* Enable RTC alarm interrupt */
        IRTC_EnableInterrupts(APP_RTC, kIRTC_AlarmInterruptEnable);
        EnableDeepSleepIRQ(APP_RTC_IRQN);
        DEMO_LOG("RTC wake up after %d seconds.\r\n", s_wakeupTimeout);
    }
    else if (s_wakeupSource == kAPP_WakeupSourcePin)
    {
        APP_InitWakeupPin();
        EnableDeepSleepIRQ(APP_SW_IRQN);
        /* Low power handshake to for async interrupt. */
        if (POWER_ModuleEnterLPRequest(APP_SW_LP_REQ) != kStatus_Success)
        {
            assert(false);
        }
        DEMO_LOG("Push wakeup PIN to wake up.\r\n");
    }
    else
    {
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t freq = 0U;

    /* Init board hardware. */
    BOARD_InitHardware();

    /* Determine the power mode before bring up. */
    if ((POWER_GetEventFlags() & PMC_FLAGS_DEEPPDF_MASK) != 0)
    {
        DEMO_LOG("Board wake up from deep or full deep power down mode.\r\n");
        POWER_ClearEventFlags(PMC_FLAGS_DEEPPDF_MASK);
    }
    POWER_ClearEventFlags(0xFFFFFFFF);

    APP_InitWakeupTimer();

    while (1)
    {
        freq = CLOCK_GetCoreSysClkFreq();

        DEMO_LOG("\r\n####################  Power Mode Switch - %s ####################\n\r\n", APP_CORE_NAME);
        DEMO_LOG("    Build Time: %s--%s \r\n", __DATE__, __TIME__);
        DEMO_LOG("    Core Clock: %dHz \r\n", freq);

        gCurrentPowerMode = APP_GetUserSelection();

        if (gCurrentPowerMode == 5U)
        {
            BOARD_RunActiveTest();
            continue;
        }

        APP_GetWakeupConfig(gCurrentPowerMode);
        APP_SetWakeupConfig();

        DEMO_LOG("Entering %s ...\r\n", gWakeupInfoStr[gCurrentPowerMode]);
        /* Enter the low power mode. */
        switch (gCurrentPowerMode)
        {
            case kPower_Sleep: /* Enter sleep mode. */
                BOARD_EnterSleep();
                break;
            case kPower_DeepSleep: /* Enter deep sleep mode. */
                BOARD_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);
                break;
            case kPower_DeepSleepRetention:
                BOARD_RequestDSR(APP_EXCLUDE_FROM_DSR);
                BOARD_RestorePeripheralsAfterDSR(); /* Restore the peripherals whose states are not retained in DSR. */
                break;
            case kPower_DeepPowerDown: /* Request deep power down(DPD) mode. The SOC enters DPD only when both domains
                                          requested DPD mode. */
                DEMO_LOG(
                    "The chip enters DPD only when both domains request entering DPD mode. Wakeup the device by "
                    "reset\r\n\r\n");
#if (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_PMIC)
                BOARD_SetPmicVoltageBeforeDeepPowerDown();
#endif
                BOARD_RequestDPD(APP_EXCLUDE_FROM_DEEP_POWERDOWN);
                /* After deep power down wakeup, the code will restart and cannot reach here. */
                break;
            case kPower_FullDeepPowerDown: /* Request full deep power down(FDPD) mode. The SOC enters FDPD only when
                                              both domains requested FDPD mode. */
                DEMO_LOG(
                    "The chip enters FDPD only when both domains request entering FDPD mode. Wakeup the device by "
                    "reset\r\n\r\n");
#if (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_PMIC)
                BOARD_SetPmicVoltageBeforeDeepPowerDown();
#endif
                BOARD_RequestFDPD(APP_EXCLUDE_FROM_FULL_DEEP_POWERDOWN);
                /* After full deep power down wakeup, the code will restart and cannot reach here. */
                break;
            default:
                break;
        }

        DEMO_LOG("Wakeup.\r\n");
    }
}
