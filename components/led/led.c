/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "gpio.h"
#include "timer_manager.h"

#include "led.h"
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
#include "pwm.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LED_ENTER_CRITICAL() uint32_t regPrimask = DisableGlobalIRQ();
#define LED_EXIT_CRITICAL() EnableGlobalIRQ(regPrimask);

/* LED control type enumeration */
typedef enum _led_control_type
{
    kLED_TurnOff = 0x01U,    /*!< Turn Off */
    kLED_TurnOn,             /*!< Turn On */
    kLED_Flash,              /*!< Flash */
    kLED_TricolorCycleFlash, /*!< Tricolor Cycle Flash */
    kLED_CycleFlash,         /*!< Cycle Flash */
    kLED_Dimming,            /*!< Dimming */
} led_control_type_t;

/* LED Dimming state structure when dimming is enabled */
typedef struct _led_dimming
{
    uint8_t increasement;
    uint8_t powerDelta[sizeof(led_config_t) / sizeof(led_pin_config_t)];
} led_dimming_t;

/* LED state structure */
typedef struct _led_state
{
    volatile uint64_t periodCount;
    volatile uint64_t expiryPeriodCount;
    uint8_t gpioHandleBuffer[sizeof(led_config_t) / sizeof(led_pin_config_t)][HAL_GPIO_HANDLE_SIZE];
    hal_gpio_handle_t gpioHandle[sizeof(led_config_t) / sizeof(led_pin_config_t)];
    uint32_t flashCycle;
    uint32_t flashPeriod;
    uint8_t timerHandleBuffer[TIMER_HANDLE_SIZE];
    timer_handle_t timerHandle;
    led_color_t settingColor;
    led_color_t currentColor;
    led_color_t nextColor;
    led_config_t config;
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
    led_dimming_t dimming;
    uint8_t pwmHandleBuffer[sizeof(led_config_t) / sizeof(led_pin_config_t)][HAL_PWM_HANDLE_SIZE];
    hal_pwm_handle_t pwmHandle[sizeof(led_config_t) / sizeof(led_pin_config_t)];
#endif
    led_control_type_t controlType;
    uint8_t flashCount;
    uint8_t flashDuty;
    uint8_t dimmingPinAdded;
} led_state_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static led_status_t LED_SetStatus(led_state_t *ledState, led_color_t color, uint32_t threshold)
{
    led_pin_config_t *ledRgbPin;
    led_status_t status = kStatus_LED_Success;
    led_color_t colorSet;
    uint8_t count;

    ledState->expiryPeriodCount = ledState->periodCount + threshold;
    ledRgbPin = (led_pin_config_t *)&ledState->config.ledRgb;

    if (kLED_TypeRgb == ledState->config.type)
    {
        count = sizeof(led_config_t) / sizeof(led_pin_config_t);
    }
    else
    {
        count = 1;
    }

    for (int i = 0; i < count; i++)
    {
        colorSet = ((color >> (i * 8)) & (0xFF));
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
        if (ledRgbPin[i].dimmingEnable)
        {
            HAL_PwmUpdateDutycycle(ledState->pwmHandle[i], ledRgbPin[i].dimming.channel, kHAL_EdgeAlignedPwm,
                                   colorSet * 100 / 255);
        }
        else
#endif
        {
            if (kStatus_HAL_GpioSuccess !=
                HAL_GpioSetOutput(ledState->gpioHandle[i], (colorSet) ? (1 - ledRgbPin[i].gpio.pinStateDefault) :
                                                                        ledRgbPin[i].gpio.pinStateDefault))
            {
                status = kStatus_LED_Error;
            }
        }
    }

    return status;
}

static void LED_TimerEvent(void *param)
{
    led_state_t *ledState = (led_state_t *)param;
    uint32_t threshold = 0;
    led_color_t color;
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
    uint16_t power[sizeof(led_config_t) / sizeof(led_pin_config_t)];
    uint8_t count = sizeof(led_config_t) / sizeof(led_pin_config_t);
#endif

    assert(param);

    ledState->periodCount += LED_TIMER_INTERVAL;

    if (ledState->periodCount >= ledState->expiryPeriodCount)
    {
        switch (ledState->controlType)
        {
            case kLED_Flash:              /*!< Flash */
            case kLED_TricolorCycleFlash: /*!< Tricolor Cycle Flash */
            case kLED_CycleFlash:         /*!< Cycle Flash */
                if (LED_FLASH_CYCLE_FOREVER != ledState->flashCycle)
                {
                    if ((ledState->flashCycle))
                    {
                        if ((kLED_Black == ledState->currentColor) || (100 == ledState->flashDuty))
                        {
                            ledState->flashCycle--;
                        }
                    }
                }
                if (ledState->flashCycle)
                {
                    if ((100 > ledState->flashDuty))
                    {
                        color = ledState->nextColor;
                        ledState->nextColor = ledState->currentColor;
                        ledState->currentColor = color;
                    }

                    if ((kLED_Black == ledState->currentColor))
                    {
                        threshold = ledState->flashPeriod * (100 - ledState->flashDuty) / 100;
                    }
                    else
                    {
                        ledState->flashCount++;
                        if (kLED_TricolorCycleFlash == ledState->controlType)
                        {
                            ledState->currentColor = (led_color_t)(0xFF << ((ledState->flashCount % 3) * 8));
                        }
                        else if (kLED_CycleFlash == ledState->controlType)
                        {
                            color = 0;
                            if (!((ledState->flashCount) & 0x07))
                            {
                                (ledState->flashCount)++;
                            }
                            if ((ledState->flashCount) & 0x04)
                            {
                                color = (led_color_t)0xFF0000;
                            }
                            if ((ledState->flashCount) & 0x02)
                            {
                                color |= (led_color_t)0xFF00;
                            }
                            if ((ledState->flashCount) & 0x01)
                            {
                                color |= (led_color_t)0xFF;
                            }
                            ledState->currentColor = color;
                        }
                        else
                        {
                        }
                        threshold = ledState->flashPeriod * (ledState->flashDuty) / 100;
                    }

                    LED_SetStatus(ledState, ledState->currentColor, threshold);
                }
                break;
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
            case kLED_Dimming: /*!< Dimming */
                for (int i = 0; i < count; i++)
                {
                    int value = (ledState->currentColor >> (8 * i)) & 0xFF;
                    if ((ledState->dimming.increasement))
                    {
                        if ((value + ledState->dimming.powerDelta[i]) < 0xFF)
                        {
                            power[i] = value + ledState->dimming.powerDelta[i];
                        }
                        else
                        {
                            power[i] = 0xFF;
                        }
                    }
                    else
                    {
                        if (value > ledState->dimming.powerDelta[i])
                        {
                            power[i] = value - ledState->dimming.powerDelta[i];
                        }
                        else
                        {
                            power[i] = 0;
                        }
                    }

                    if (kLED_TypeMonochrome == ledState->config.type)
                    {
                        break;
                    }
                }
                ledState->currentColor = LED_MAKE_COLOR(power[0], power[1], power[2]);
                LED_SetStatus(ledState, ledState->currentColor, ledState->flashPeriod);
                break;
#endif
            default:
                break;
        }
    }
}

led_status_t LED_Init(led_handle_t ledHandle, led_config_t *ledConfig)
{
    led_state_t *ledState;
    led_pin_config_t *ledRgbConfigPin;
    hal_gpio_pin_config_t controlPin;
    uint32_t count;
    int i = 0;
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
    uint8_t rgbFlag = 0;
    uint8_t rgbDimmingFlag = 0;
#endif

    assert(ledConfig);
    assert(ledHandle);

    if (LED_HANDLE_SIZE < sizeof(led_state_t))
    {
        return kStatus_LED_Error;
    }

    if (kLED_TypeRgb == ledConfig->type)
    {
        count = sizeof(led_config_t) / sizeof(led_pin_config_t);
    }
    else
    {
        count = 1;
    }

    ledState = (led_state_t *)ledHandle;

    for (int i = 0; i < sizeof(led_state_t); i++)
    {
        *(((uint8_t *)ledState) + i) = 0x00;
    }

    ledState->timerHandle = (timer_handle_t)&ledState->timerHandleBuffer[0];
    if (kStatus_TimerSuccess != TM_Open(ledState->timerHandle))
    {
        return kStatus_LED_Error;
    }

    for (i = 0; i < sizeof(led_config_t); i++)
    {
        ((uint8_t *)&(ledState->config))[i] = ((uint8_t *)(ledConfig))[i];
    }

    ledState->settingColor = kLED_White;

    ledRgbConfigPin = (led_pin_config_t *)(&ledConfig->ledRgb);

    controlPin.direction = kHAL_GpioDirectionOut;
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
    for (i = 0; i < count; i++)
    {
        if (ledRgbConfigPin[i].dimmingEnable)
        {
            rgbDimmingFlag = 1;
        }
        else
        {
            rgbFlag = 1;
        }
    }
    if (rgbDimmingFlag && rgbFlag)
    {
        return kStatus_LED_Error;
    }
#endif
    for (i = 0; i < count; i++)
    {
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
        if (ledRgbConfigPin[i].dimmingEnable)
        {
            hal_pwm_setup_config_t setupConfig;
            ledState->pwmHandle[i] = (hal_pwm_handle_t)&ledState->pwmHandleBuffer[i][0];
            HAL_PwmInit(ledState->pwmHandle[i], ledRgbConfigPin[i].dimming.instace,
                        ledRgbConfigPin[i].dimming.sourceClock);
            setupConfig.dutyCyclePercent = 0;
            setupConfig.level = ledRgbConfigPin[i].dimming.pinStateDefault ? kHAL_PwmLowTrue : kHAL_PwmHighTrue;
            setupConfig.mode = kHAL_EdgeAlignedPwm;
            setupConfig.pwmFreq_Hz = 1000;
            HAL_PwmSetupPwm(ledState->pwmHandle[i], ledRgbConfigPin[i].dimming.channel, &setupConfig);
            ledState->dimmingPinAdded = 1;
        }
        else
#endif
        {
            controlPin.port = ledRgbConfigPin[i].gpio.port;
            controlPin.pin = ledRgbConfigPin[i].gpio.pin;
            ledState->gpioHandle[i] = (hal_gpio_handle_t)&ledState->gpioHandleBuffer[i][0];
            if (kStatus_HAL_GpioSuccess != HAL_GpioInit(ledState->gpioHandle[i], &controlPin))
            {
                return kStatus_LED_Error;
            }
        }
    }
    if (kStatus_TimerSuccess != TM_InstallCallback(ledState->timerHandle, LED_TimerEvent, ledHandle))
    {
        return kStatus_LED_Error;
    }
    if (kStatus_TimerSuccess != TM_Start(ledState->timerHandle, kTimerModeIntervalTimer, LED_TIMER_INTERVAL))
    {
        return kStatus_LED_Error;
    }

    if (kStatus_LED_Success != LED_TurnOnOff(ledState, 0))
    {
        return kStatus_LED_Error;
    }

    return kStatus_LED_Success;
}

led_status_t LED_Deinit(led_handle_t ledHandle)
{
    led_state_t *ledState;
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
    led_pin_config_t *ledRgbConfigPin;
#endif

    assert(ledHandle);

    ledState = (led_state_t *)ledHandle;

    TM_Close(ledState->timerHandle);
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
    ledRgbConfigPin = (led_pin_config_t *)(&ledState->config.ledRgb);
#endif
    for (int i = 0; i < (sizeof(led_config_t) / sizeof(led_pin_config_t)); i++)
    {
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
        if (ledRgbConfigPin[i].dimmingEnable)
        {
            HAL_PwmDeinit(ledState->pwmHandle[i]);
        }
        else
#endif
        {
            HAL_GpioDeinit(ledState->gpioHandle[i]);
        }
        if ((kLED_TypeRgb != ledState->config.type))
        {
            break;
        }
    }
    ledState->dimmingPinAdded = 0;

    return kStatus_LED_Success;
}

led_status_t LED_TurnOnOff(led_handle_t ledHandle, uint8_t turnOnOff)
{
    led_state_t *ledState;
    led_status_t status;

    assert(ledHandle);

    ledState = (led_state_t *)ledHandle;
    LED_ENTER_CRITICAL();
    if (turnOnOff)
    {
        ledState->controlType = kLED_TurnOn;
        ledState->currentColor = ledState->settingColor;
    }
    else
    {
        ledState->controlType = kLED_TurnOff;
        ledState->currentColor = kLED_Black;
    }
    LED_EXIT_CRITICAL();

    status = LED_SetStatus(ledState, ledState->currentColor, 0);
    return status;
}

led_status_t LED_SetColor(led_handle_t ledHandle, led_color_t ledRgbColor)
{
    led_state_t *ledState;

    assert(ledHandle);

    ledState = (led_state_t *)ledHandle;

    if ((kLED_TypeRgb != ledState->config.type))
    {
        return kStatus_LED_Error;
    }

    ledState->settingColor = ledRgbColor;

    return kStatus_LED_Success;
}

led_status_t LED_Flash(led_handle_t ledHandle, led_flash_config_t *ledFlash)
{
    led_state_t *ledState;
    led_status_t status;

    assert(ledHandle);
    assert(ledFlash);
    assert(ledFlash->times);
    assert(ledFlash->duty <= 100);

    ledState = (led_state_t *)ledHandle;
    LED_ENTER_CRITICAL();

    if (kLED_FlashOneColor == ledFlash->flashType)
    {
        ledState->controlType = kLED_Flash;
    }
    else
    {
        if ((kLED_TypeRgb != ledState->config.type))
        {
            LED_EXIT_CRITICAL();
            return kStatus_LED_Error;
        }
        ledState->controlType = kLED_CycleFlash;
    }
    ledState->flashPeriod = ledFlash->period;
    ledState->flashDuty = ledFlash->duty;

    ledState->currentColor = ledState->settingColor;
    ledState->flashCycle = ledFlash->times;
    ledState->nextColor = kLED_Black;
    LED_EXIT_CRITICAL();
    status = LED_SetStatus(ledState, ledState->currentColor, (ledState->flashPeriod * ledState->flashDuty) / 100);

    return status;
}

led_status_t LED_Blip(led_handle_t ledHandle)
{
    led_flash_config_t ledFlash;

    ledFlash.duty = 50;
    ledFlash.flashType = kLED_FlashOneColor;
    ledFlash.period = LED_BLIP_INTERVAL;
    ledFlash.times = 1;
    return LED_Flash(ledHandle, &ledFlash);
}

led_status_t LED_Dimming(led_handle_t ledHandle, uint16_t dimmingPeriod, uint8_t increasement)
{
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
    led_state_t *ledState;
    led_status_t status;
    int i;
    uint16_t power[sizeof(led_config_t) / sizeof(led_pin_config_t)];
    uint8_t value;
    uint8_t count = sizeof(led_config_t) / sizeof(led_pin_config_t);

    assert(ledHandle);
    assert(dimmingPeriod);

    ledState = (led_state_t *)ledHandle;

    if (!ledState->dimmingPinAdded)
    {
        return kStatus_LED_Error;
    }

    LED_ENTER_CRITICAL();

    ledState->controlType = kLED_Dimming;
    ledState->flashPeriod = LED_DIMMING_UPDATE_INTERVAL;
    ledState->flashDuty = 100;
    ledState->dimming.increasement = increasement;

    if ((kLED_TypeRgb == ledState->config.type))
    {
        ledState->currentColor = ledState->settingColor;
    }

    for (i = 0; i < count; i++)
    {
        ledState->dimming.powerDelta[i] = 0;
    }

    for (i = 0; i < count; i++)
    {
        value = (ledState->currentColor >> (8 * (i))) & 0xFF;
        if (ledState->dimming.increasement)
        {
            ledState->dimming.powerDelta[i] = (0xFF - value) * LED_DIMMING_UPDATE_INTERVAL / (dimmingPeriod);
            if ((value + ledState->dimming.powerDelta[i]) < 0xFF)
            {
                power[i] = value + ledState->dimming.powerDelta[i];
            }
            else
            {
                power[i] = 0xFF;
            }
        }
        else
        {
            ledState->dimming.powerDelta[i] = (value)*LED_DIMMING_UPDATE_INTERVAL / (dimmingPeriod);
            if (value > ledState->dimming.powerDelta[i])
            {
                power[i] = value - ledState->dimming.powerDelta[i];
            }
            else
            {
                power[i] = 0;
            }
        }

        if (kLED_TypeMonochrome == ledState->config.type)
        {
            break;
        }
    }

    ledState->currentColor = LED_MAKE_COLOR(power[0], power[1], power[2]);
    ledState->flashCycle = LED_FLASH_CYCLE_FOREVER;
    LED_EXIT_CRITICAL();
    status = LED_SetStatus(ledState, ledState->currentColor, ledState->flashPeriod);

    return status;
#else
    return kStatus_LED_Error;
#endif
}

led_status_t LED_EnterLowpower(led_handle_t ledHandle)
{
#if 0
    led_state_t* ledState;
    led_pin_config_t* ledRgbPin;
    led_status_t status;
    int count;

    assert(ledHandle);

    ledState = (led_state_t*)ledHandle;

    ledRgbPin = (led_pin_config_t*)&ledState->config.ledRgb;
    if (kLED_TypeRgb == ledState->config.type)
    {
        count = sizeof(led_config_t) / sizeof(led_pin_config_t);
    }
    else
    {
        count = 1;
    }

    for (int i = 0;i < count; i++)
    {
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
        if (ledRgbPin[i].dimmingEnable)
        {
            HAL_PwmEnterLowpower(ledRgbPin[i].dimming.instace, ledRgbPin[i].dimming.channel);
        }
        else
#endif
        {
            HAL_GpioEnterLowpower(ledState->gpioHandle[i]);
        }
    }
#endif
    return kStatus_LED_Success;
}

led_status_t LED_ExitLowpower(led_handle_t ledHandle)
{
#if 0
    led_state_t* ledState;
    led_pin_config_t* ledRgbPin;
    led_status_t status;
    int count;

    assert(ledHandle);

    ledState = (led_state_t*)ledHandle;

    ledRgbPin = (led_pin_config_t*)&ledState->config.ledRgb;
    if (kLED_TypeRgb == ledState->config.type)
    {
        count = sizeof(led_config_t) / sizeof(led_pin_config_t);
    }
    else
    {
        count = 1;
    }

    for (int i = 0;i < count; i++)
    {
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
        if (ledRgbPin[i].dimmingEnable)
        {
            HAL_PwmExitLowpower(ledRgbPin[i].dimming.instace, ledRgbPin[i].dimming.channel);
        }
        else
#endif
        {
            HAL_GpioExitLowpower(ledState->gpioHandle[i]);
        }
    }
#endif
    return kStatus_LED_Success;
}
