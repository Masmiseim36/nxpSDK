/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sln_rgb_led_driver.h"

#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_flexio.h"

/**
 * @brief Set the frequency and duty cycle for a specific channel
 *
 * @param freq_Hz frequency in Hz
 * @param duty    Duty cycle from 1 to 99
 * @param channel PWM channel
 * @param pwm_pin PWM pin
 */
static void _flexio_pwm_init(uint32_t freq_Hz, uint32_t duty, uint8_t channel, uint8_t pwm_pin)
{
    assert((freq_Hz < FLEXIO_MAX_FREQUENCY) && (freq_Hz > FLEXIO_MIN_FREQUENCY));

    uint32_t lowerValue = 0; /* Number of clock cycles in high logic state in one period */
    uint32_t upperValue = 0; /* Number of clock cycles in low logic state in one period */
    uint32_t sum        = 0; /* Number of clock cycles in one period */
    flexio_timer_config_t fxioTimerConfig;

    /* Check parameter */
    if ((duty > 100) || (duty == 0))
    {
        duty = 50;
    }

    /* Configure the timer DEMO_FLEXIO_TIMER_CH for generating PWM for R */
    fxioTimerConfig.triggerSelect   = FLEXIO_TIMER_TRIGGER_SEL_SHIFTnSTAT(0U);
    fxioTimerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceInternal;
    fxioTimerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
    fxioTimerConfig.pinConfig       = kFLEXIO_PinConfigOutput;
    fxioTimerConfig.pinPolarity     = kFLEXIO_PinActiveHigh;
    fxioTimerConfig.timerMode       = kFLEXIO_TimerModeDisabled;
    fxioTimerConfig.timerOutput     = kFLEXIO_TimerOutputOneNotAffectedByReset;
    fxioTimerConfig.timerDecrement  = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    fxioTimerConfig.timerDisable    = kFLEXIO_TimerDisableNever;
    fxioTimerConfig.timerEnable     = kFLEXIO_TimerEnabledAlways;
    fxioTimerConfig.timerReset      = kFLEXIO_TimerResetNever;
    fxioTimerConfig.timerStart      = kFLEXIO_TimerStartBitDisabled;
    fxioTimerConfig.timerStop       = kFLEXIO_TimerStopBitDisabled;

    /* Calculate timer lower and upper values of TIMCMP */
    /* Calculate the nearest integer value for sum, using formula round(x) = (2 * floor(x) + 1) / 2 */
    /* sum = DEMO_FLEXIO_CLOCK_FREQUENCY / freq_H */
    sum = (LED_FLEXIO_CLOCK_FREQUENCY * 2 / freq_Hz + 1) / 2;
    /* Calculate the nearest integer value for lowerValue, the high period of the pwm output */
    /* lowerValue = sum * duty / 100 */
    lowerValue = (sum * duty / 50 + 1) / 2;
    /* Calculate upper value, the low period of the pwm output */
    upperValue                   = sum - lowerValue;
    fxioTimerConfig.timerCompare = ((upperValue - 1) << 8U) | (lowerValue - 1);

    fxioTimerConfig.pinSelect = pwm_pin; /* Set pwm output */
    FLEXIO_SetTimerConfig(LED_FLEXIO_BASEADDR, channel, &fxioTimerConfig);
}

/**
 * @brief Configures and starts the PWM for the LED's
 *
 * @param redPWMval   Red PWM duty cycle
 * @param greenPWMval Green PWM duty cycle
 * @param bluePWMval  Blue PWM duty cycle
 */
static void _start_pwm(uint8_t redPWMval, uint8_t greenPWMval, uint8_t bluePWMval)
{
    /* Set the PWM duty cycle */
    _flexio_pwm_init(LED_FLEXIO_FREQUENCY, redPWMval, FLEXIO_RED_TIMER_CH, FLEXIO_RED_OUTPUTPIN);
    _flexio_pwm_init(LED_FLEXIO_FREQUENCY, greenPWMval, FLEXIO_GREEN_TIMER_CH, FLEXIO_GREEN_OUTPUTPIN);
    _flexio_pwm_init(LED_FLEXIO_FREQUENCY, bluePWMval, FLEXIO_BLUE_TIMER_CH, FLEXIO_BLUE_OUTPUTPIN);

    /* Start the PWM */
    if (redPWMval != 0)
    {
        LED_FLEXIO_BASEADDR->TIMCTL[FLEXIO_RED_TIMER_CH] |= FLEXIO_TIMCTL_TIMOD(kFLEXIO_TimerModeDual8BitPWM);
    }
    if (greenPWMval != 0)
    {
        LED_FLEXIO_BASEADDR->TIMCTL[FLEXIO_GREEN_TIMER_CH] |= FLEXIO_TIMCTL_TIMOD(kFLEXIO_TimerModeDual8BitPWM);
    }
    if (bluePWMval != 0)
    {
        LED_FLEXIO_BASEADDR->TIMCTL[FLEXIO_BLUE_TIMER_CH] |= FLEXIO_TIMCTL_TIMOD(kFLEXIO_TimerModeDual8BitPWM);
    }
}

/**
 * @brief Get the duty cycle for the selected color
 *
 * @param color       The color for which we want the duty cycle
 * @param redPWMval   The value for the red LED
 * @param greenPWMval The value for the green LED
 * @param bluePWMval  The value for the blue LED
 */
static void _get_rgb_pwm_values(rgbLedColor_t color, uint8_t *redPWMval, uint8_t *greenPWMval, uint8_t *bluePWMval)
{
    /* Color tuning. */
    switch (color)
    {
        case LED_COLOR_RED:
            *redPWMval   = 99;
            *greenPWMval = 0;
            *bluePWMval  = 0;
            break;
        case LED_COLOR_ORANGE:
            *redPWMval   = 99;
            *greenPWMval = 30;
            *bluePWMval  = 0;
            break;
        case LED_COLOR_YELLOW:
            *redPWMval   = 99;
            *greenPWMval = 80;
            *bluePWMval  = 0;
            break;
        case LED_COLOR_GREEN:
            *redPWMval   = 0;
            *greenPWMval = 99;
            *bluePWMval  = 0;
            break;
        case LED_COLOR_BLUE:
            *redPWMval   = 0;
            *greenPWMval = 0;
            *bluePWMval  = 99;
            break;
        case LED_COLOR_PURPLE:
            *redPWMval   = 99;
            *greenPWMval = 0;
            *bluePWMval  = 99;
            break;
        case LED_COLOR_CYAN:
            *redPWMval   = 0;
            *greenPWMval = 99;
            *bluePWMval  = 99;
            break;
        case LED_COLOR_WHITE:
            *redPWMval   = 99;
            *greenPWMval = 99;
            *bluePWMval  = 99;
            break;
        case LED_COLOR_OFF:
            *redPWMval   = 0;
            *greenPWMval = 0;
            *bluePWMval  = 0;
            break;
        default:
            *redPWMval   = 0;
            *greenPWMval = 0;
            *bluePWMval  = 0;
            break;
    }
}

status_t RGB_LED_Init(void)
{
    flexio_config_t fxioUserConfig;

    FLEXIO_GetDefaultConfig(&fxioUserConfig);
    FLEXIO_Init(LED_FLEXIO_BASEADDR, &fxioUserConfig);

    return kStatus_Success;
}

void RGB_LED_SetColor(rgbLedColor_t color)
{
    uint8_t redPWMval;
    uint8_t greenPWMval;
    uint8_t bluePWMval;

    _get_rgb_pwm_values(color, &redPWMval, &greenPWMval, &bluePWMval);
    _start_pwm(redPWMval, greenPWMval, bluePWMval);
}

void RGB_LED_SetBrightnessColor(rgb_led_brightness_t brightness, rgbLedColor_t color)
{
    uint8_t redPWMval;
    uint8_t greenPWMval;
    uint8_t bluePWMval;

    _get_rgb_pwm_values(color, &redPWMval, &greenPWMval, &bluePWMval);

    /* brightness = 0 (Off), 1 (Low), 2 (Medium) or 3 (High) */
    /* tune the PWM values based on brightness */
    if (brightness == LED_BRIGHT_OFF)
    {
        /* off */
        redPWMval   = 0;
        greenPWMval = 0;
        bluePWMval  = 0;
    }
    else if (brightness == LED_BRIGHT_LOW)
    {
        /* low */
        redPWMval /= 3;
        greenPWMval /= 3;
        bluePWMval /= 3;
    }
    else if (brightness == LED_BRIGHT_MEDIUM)
    {
        /* medium */
        redPWMval   = (redPWMval * 2) / 3;
        greenPWMval = (greenPWMval * 2) / 3;
        bluePWMval  = (bluePWMval * 2) / 3;
    }
    else if (brightness == LED_BRIGHT_HIGH)
    {
        /* high */
    }
    else
    {
        /* error */
        return;
    }

    _start_pwm(redPWMval, greenPWMval, bluePWMval);
}
