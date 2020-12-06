/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sln_RT10xx_RGB_LED_driver.h"

#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_pwm.h"
#include "fsl_xbara.h"

#define BOARD_PWM_BASEADDR PWM2
#define PWM_SRC_CLK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)

status_t RGB_LED_Init(void)
{
    /* Structure of initialize PWM */
    pwm_config_t pwmConfig;

    uint16_t deadTimeVal;
    pwm_signal_param_t pwmRed;
    pwm_signal_param_t pwmGreen;
    pwm_signal_param_t pwmBlue;
    uint32_t pwmSourceClockInHz;
    uint32_t pwmFrequencyInHz = 1;

    // CLOCK_SetDiv(kCLOCK_AhbDiv, 0x2); /* Set AHB PODF to 2, divide by 3 */
    // CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3); /* Set IPG PODF to 3, divede by 4 */

    /* Set the PWM Fault inputs to a low value */
    XBARA_Init(XBARA1);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm2Fault0);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm2Fault1);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault2);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault3);

    PWM_GetDefaultConfig(&pwmConfig);

    /* Allows us to see pwm in Debug Mode*/
    pwmConfig.enableDebugMode = true;

    /* Initialize submodule 0 (Red LED) */
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_0, &pwmConfig) == kStatus_Fail)
    {
        // PRINTF("PWM initialization failed\n");
        return kStatus_Fail;
    }
    // pwmConfig.clockSource = kPWM_Submodule0Clock;
    // pwmConfig.initializationControl = kPWM_Initialize_MasterSync;

    /* Initialize submodule 1 (Green LED) */
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_1, &pwmConfig) == kStatus_Fail)
    {
        // PRINTF("PWM initialization failed\n");
        return kStatus_Fail;
    }

    /* Initialize submodule 3 (Blue Led) */
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_3, &pwmConfig) == kStatus_Fail)
    {
        // PRINTF("PWM initialization failed\n");
        return kStatus_Fail;
    }

    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;

    /* Set deadtime count, we set this to about 650ns */
    deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;

    pwmRed.pwmChannel       = kPWM_PwmA;
    pwmRed.level            = kPWM_LowTrue;
    pwmRed.dutyCyclePercent = 0; /* 0 percent dutycycle */
    pwmRed.deadtimeValue    = deadTimeVal;

    pwmGreen.pwmChannel       = kPWM_PwmA;
    pwmGreen.level            = kPWM_LowTrue;
    pwmGreen.dutyCyclePercent = 0; /* 0 percent dutycycle */
    pwmGreen.deadtimeValue    = deadTimeVal;

    pwmBlue.pwmChannel       = kPWM_PwmA;
    pwmBlue.level            = kPWM_LowTrue;
    pwmBlue.dutyCyclePercent = 0; /* 0 percent dutycycle */
    pwmBlue.deadtimeValue    = deadTimeVal;

    /*********** PWMA_SM0 - phase A, configuration, setup 2 channel as an example ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_0, &pwmRed, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);

    /*********** PWMA_SM1 - phase B configuration, setup PWM A channel only ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_1, &pwmGreen, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);

    /*********** PWMA_SM2 - phase C configuration, setup PWM A channel only ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_3, &pwmBlue, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);

    /* Set the load okay bit for all submodules to load registers from their buffer */
    PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_3, true);

    /* Start the PWM generation from Submodules 0, 1 and 3 */
    PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_3);

    return kStatus_Success;
}

void RGB_LED_SetColor(rgbLedColor_t color)
{
    uint8_t redPWMval;
    uint8_t greenPWMval;
    uint8_t bluePWMval;

    /*Colours can be tuned more....will continue to add more colours and tuning. */
    switch (color)
    {
        case LED_COLOR_RED:
            redPWMval   = 100;
            greenPWMval = 0;
            bluePWMval  = 0;
            break;
        case LED_COLOR_ORANGE:
            redPWMval   = 100;
            greenPWMval = 60;
            bluePWMval  = 0;
            break;
        case LED_COLOR_YELLOW:
            redPWMval   = 40;
            greenPWMval = 90;
            bluePWMval  = 0;
            break;
        case LED_COLOR_GREEN:
            redPWMval   = 0;
            greenPWMval = 100;
            bluePWMval  = 0;
            break;
        case LED_COLOR_BLUE:
            redPWMval   = 0;
            greenPWMval = 0;
            bluePWMval  = 100;
            break;
        case LED_COLOR_PURPLE:
            redPWMval   = 60;
            greenPWMval = 0;
            bluePWMval  = 100;
            break;
        case LED_COLOR_CYAN:
            redPWMval   = 0;
            greenPWMval = 100;
            bluePWMval  = 100;
            break;
        case LED_COLOR_WHITE:
            redPWMval   = 15;
            greenPWMval = 100;
            bluePWMval  = 100;
            break;
        case LED_COLOR_OFF:
            redPWMval   = 0;
            greenPWMval = 0;
            bluePWMval  = 0;
            break;
        default:
            redPWMval   = 0;
            greenPWMval = 0;
            bluePWMval  = 0;
            break;
    }

    /* Update duty cycles for all 3 PWM signals */
    PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_SignedCenterAligned, redPWMval);
    PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA, kPWM_SignedCenterAligned, (greenPWMval >> 1));
    PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_3, kPWM_PwmA, kPWM_SignedCenterAligned, (bluePWMval >> 3));

    /* Set the load okay bit for all submodules to load registers from their buffer */
    PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_3, true);
}

void RGB_LED_SetBrightnessColor(rgb_led_brightness_t brightness, rgbLedColor_t color)
{
    uint8_t redPWMval;
    uint8_t greenPWMval;
    uint8_t bluePWMval;

    /*Colours can be tuned more....will continue to add more colours and tuning. */
    switch (color)
    {
        case LED_COLOR_RED:
            redPWMval   = 100;
            greenPWMval = 0;
            bluePWMval  = 0;
            break;
        case LED_COLOR_ORANGE:
            redPWMval   = 100;
            greenPWMval = 60;
            bluePWMval  = 0;
            break;
        case LED_COLOR_YELLOW:
            redPWMval   = 40;
            greenPWMval = 90;
            bluePWMval  = 0;
            break;
        case LED_COLOR_GREEN:
            redPWMval   = 0;
            greenPWMval = 100;
            bluePWMval  = 0;
            break;
        case LED_COLOR_BLUE:
            redPWMval   = 0;
            greenPWMval = 0;
            bluePWMval  = 100;
            break;
        case LED_COLOR_PURPLE:
            redPWMval   = 60;
            greenPWMval = 0;
            bluePWMval  = 100;
            break;
        case LED_COLOR_CYAN:
            redPWMval   = 0;
            greenPWMval = 100;
            bluePWMval  = 100;
            break;
        case LED_COLOR_WHITE:
            redPWMval   = 15;
            greenPWMval = 100;
            bluePWMval  = 100;
            break;
        case LED_COLOR_OFF:
            redPWMval   = 0;
            greenPWMval = 0;
            bluePWMval  = 0;
            break;
        default:
            redPWMval   = 0;
            greenPWMval = 0;
            bluePWMval  = 0;
            break;
    }

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

    /* Update duty cycles for all 3 PWM signals */
    PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_SignedCenterAligned, redPWMval);
    PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA, kPWM_SignedCenterAligned, (greenPWMval >> 1));
    PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_3, kPWM_PwmA, kPWM_SignedCenterAligned, (bluePWMval >> 3));

    /* Set the load okay bit for all submodules to load registers from their buffer */
    PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_3, true);
}
