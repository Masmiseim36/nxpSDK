/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "fsl_pm_core.h"
#include "fsl_pm_device.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_pint.h"
#include "fsl_power.h"
#include "pmic_support.h"
/*${header:end}*/

/*${variable:start}*/
extern pm_handle_t g_pmHndle;
volatile bool pintFlag = false;

AT_ALWAYS_ON_DATA(pm_wakeup_source_t g_UserkeyWakeupSource);
AT_ALWAYS_ON_DATA(pm_wakeup_source_t g_UserkeyWakeupSource1);

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify1) = {
    .notifyCallback = APP_ControlCallback_notify,
    .data           = NULL,
};

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Init board hardware. */
    pca9420_modecfg_t pca9420ModeCfg[4];
    uint32_t i;

    /* BE CAUTIOUS TO SET CORRECT VOLTAGE RANGE ACCORDING TO YOUR BOARD/APPLICATION. PAD SUPPLY BEYOND THE RANGE DO
       HARM TO THE SILICON. */
    power_pad_vrange_t vrange = {.Vdde0Range = kPadVol_171_198,
                                 .Vdde1Range = kPadVol_171_198,
                                 .Vdde2Range = kPadVol_171_198,
                                 .Vdde3Range = kPadVol_300_360,
                                 .Vdde4Range = kPadVol_171_198};

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* PMIC PCA9420 */
    BOARD_InitPmic();
    for (i = 0; i < ARRAY_SIZE(pca9420ModeCfg); i++)
    {
        PCA9420_GetDefaultModeConfig(&pca9420ModeCfg[i]);
    }
    BOARD_ConfigPMICModes(pca9420ModeCfg, ARRAY_SIZE(pca9420ModeCfg));
    PCA9420_WriteModeConfigs(&pca9420Handle, kPCA9420_Mode0, &pca9420ModeCfg[0], ARRAY_SIZE(pca9420ModeCfg));

    /* Configure PMIC Vddcore value according to CM33 clock. DSP not used in this demo. */
    BOARD_SetPmicVoltageForFreq(CLOCK_GetFreq(kCLOCK_CoreSysClk), 0U);

    /* Indicate to power library that PMIC is used. */
    POWER_UpdatePmicRecoveryTime(1);

    POWER_SetPadVolRange(&vrange);
}

void BOARD_ConfigPMICModes(pca9420_modecfg_t *cfg, uint32_t num)
{
    assert(cfg);

    /* Configuration PMIC mode to align with power lib like below:
     *  0b00    run mode, no special.
     *  0b01    deep sleep mode, vddcore 0.6V.
     *  0b10    deep powerdown mode, vddcore off.
     *  0b11    full deep powerdown mode vdd1v8 and vddcore off. */

    /* Mode 1: VDDCORE 0.6V. */
    cfg[1].sw1OutVolt = kPCA9420_Sw1OutVolt0V600;
    /* Mode 2: VDDCORE off. */
    cfg[2].enableSw1Out = false;

    /* Mode 3: VDDCORE, VDD1V8 and VDDIO off. */
    cfg[3].enableSw1Out  = false;
    cfg[3].enableSw2Out  = false;
    cfg[3].enableLdo2Out = false;
}

status_t APP_ControlCallback_notify(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    if (eventType == kPM_EventEnteringSleep)
    {
        PRINTF("APP_ControlCallback ENTERING LP -- Notify.\r\n");
    }
    else
    {
        PRINTF("APP_ControlCallback EXITING LP -- Notify.\r\n");
    }

    return kStatus_Success;
}

static void pint_intr_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    pintFlag = true;
}

void APP_InitWakeupSource(void)
{
    PM_InitWakeupSource(&g_UserkeyWakeupSource, (uint32_t)PIN_INT0_IRQn, NULL, true);
    gpio_pin_config_t gpioPinConfigStruct;

    /* Set SW pin as GPIO input. */
    gpioPinConfigStruct.pinDirection = kGPIO_DigitalInput;
    GPIO_PinInit(APP_USER_WAKEUP_KEY_GPIO, APP_USER_WAKEUP_KEY_PORT, APP_USER_WAKEUP_KEY_PIN, &gpioPinConfigStruct);

    /* Configure the Input Mux block and connect the trigger source to PinInt channle. */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, APP_USER_WAKEUP_KEY_INPUTMUX_SEL); /* Using channel 0. */

    /* Configure the interrupt for SW pin. */
    PINT_Init(PINT);
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge, pint_intr_callback);

    PM_InitWakeupSource(&g_UserkeyWakeupSource1, (uint32_t)PIN_INT1_IRQn, NULL, true);
    gpio_pin_config_t gpioPinConfigStruct1;
    gpioPinConfigStruct1.pinDirection = kGPIO_DigitalInput;
    GPIO_PinInit(APP_USER_WAKEUP_KEY1_GPIO, APP_USER_WAKEUP_KEY1_PORT, APP_USER_WAKEUP_KEY1_PIN, &gpioPinConfigStruct1);

    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt1, APP_USER_WAKEUP_KEY1_INPUTMUX_SEL); /* Using channel 0. */
    INPUTMUX_Deinit(INPUTMUX); /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */

    PINT_PinInterruptConfig(PINT, kPINT_PinInt1, kPINT_PinIntEnableFallEdge, pint_intr_callback);
    PINT_EnableCallback(PINT); /* Enable callbacks for PINT */
}

uint32_t APP_GetWakeupTimeout(void)
{
    PRINTF(
        "TIMER NOT IMPLEMENTED -- Please use:\r\n"
        "User buttons (SW1/SW2) to wakeup from Sleep or DeepSleep, \r\n"
        "Reset button to wakeup from Deep Power Down or Full Deep Power Down. \r\n\n");
    return 0;
}

void APP_RegisterNotify(void)
{
    PM_RegisterNotify(kPM_NotifyGroup0, &g_notify1);
}

void APP_SetConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case PM_LP_STATE_SLEEP:
        {
            PM_SetConstraints(PM_LP_STATE_SLEEP, APP_SLEEP_CONSTRAINTS);
            break;
        }

        case PM_LP_STATE_DEEP_SLEEP:
        {
            PM_SetConstraints(PM_LP_STATE_DEEP_SLEEP, APP_DEEP_SLEEP_CONSTRAINTS);
            break;
        }

        case PM_LP_STATE_DEEP_POWER_DOWN:
        {
            PM_SetConstraints(PM_LP_STATE_DEEP_POWER_DOWN, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }

        case PM_LP_STATE_FULL_DEEP_POWER_DOWN:
        {
            PM_SetConstraints(PM_LP_STATE_FULL_DEEP_POWER_DOWN, APP_FULL_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }

        default:
        {
            /* This branch will never be hit. */
            break;
        }
    }
}

void APP_ReleaseConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case PM_LP_STATE_SLEEP:
        {
            PM_ReleaseConstraints(PM_LP_STATE_SLEEP, APP_SLEEP_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_DEEP_SLEEP:
        {
            PM_ReleaseConstraints(PM_LP_STATE_DEEP_SLEEP, APP_DEEP_SLEEP_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_DEEP_POWER_DOWN:
        {
            PM_ReleaseConstraints(PM_LP_STATE_DEEP_POWER_DOWN, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_FULL_DEEP_POWER_DOWN:
        {
            PM_ReleaseConstraints(PM_LP_STATE_FULL_DEEP_POWER_DOWN, APP_FULL_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        default:
        {
            /* This branch should never be hit. */
            break;
        }
    }
}
/*${function:end}*/
