/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file lvhb.c
 *
 * Low voltage H-Bridge driver based on AML layer.
 * Supports boards based on MPC17510, MPC17511, MPC17529, MPC17531A, MPC17533,
 * MPC17C724 and MC34933.
 *
 * This module is common for all supported models.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lvhb.h"

/******************************************************************************
 * Constant variables
 ******************************************************************************/

/* Coefficient table for calculation of timer offset values for 32 micro-steps
 * per one full-step (in number of ticks). */
static const float LVHB_MICROSTEP_OFFSET_COEF[LVHB_MICROSTEP_CNT_MAX + 1] = {
    0.0000,
    0.0491,
    0.0980,
    0.1467,
    0.1951,
    0.2430,
    0.2903,
    0.3369,
    0.3827,
    0.4276,
    0.4714,
    0.5141,
    0.5556,
    0.5957,
    0.6344,
    0.6716,
    0.7071,
    0.7410,
    0.7730,
    0.8032,
    0.8315,
    0.8577,
    0.8819,
    0.9040,
    0.9239,
    0.9415,
    0.9569,
    0.9700,
    0.9808,
    0.9892,
    0.9952,
    0.9980,
    1.0000
};

/* Control sequence of channels output (1 electrical revolution) for full-stepping. */
static const uint8_t LVHB_FULLSTEP_SEQ[4] = {
    LVHB_FULLSTEP_CH_IN1A | LVHB_FULLSTEP_CH_IN2A, /* Channel IN1A and IN2A are active in 1st step. */
    LVHB_FULLSTEP_CH_IN1A | LVHB_FULLSTEP_CH_IN2B, /* Channel IN1A and IN2B are active in 2nd step. */
    LVHB_FULLSTEP_CH_IN1B | LVHB_FULLSTEP_CH_IN2B, /* Channel IN1B and IN2B are active in 3rd step. */
    LVHB_FULLSTEP_CH_IN1B | LVHB_FULLSTEP_CH_IN2A  /* Channel IN1B and IN2A are active in 4th step. */
};

/*******************************************************************************
 * Prototypes of internal functions
 ******************************************************************************/

/*
 * @brief This function returns number of PWM channels which must be allocated.
 * This function is internal only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 *
 * @return Number of channels.
 */
static uint8_t LVHB_GetPwmChCount(lvhb_drv_config_t* const drvConfig);

/*
 * @brief This function sets PWM channels to AML structure.
 * This function is internal only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param amlPwmCh Allocated array of PWM channels.
 */
static void LVHB_SetPwm(lvhb_drv_config_t const *drvConfig,
        tmr_aml_pwm_ch_t* const amlPwmCh);

/*
 * @brief This function sets outputs of specified H-Bridge to LOW.
 * This function is internal only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param bridge H-Bridge of device the motor is connected to. Only
 *        lvhbBridge1 value is correct when single H-Bridge device is used.
 */
static void LVHB_SetZeroOut(lvhb_drv_config_t* const drvConfig, lvhb_bridge_t bridge);

/*
 * @brief This function gets the square root. Taken from sqrt.c, function grupe.
 * Author: Eyal Lebedinsky  (eyal@ise.canberra.edu.au).
 * Web: https://ftp.samba.org/pub/eyal/samples/sqrt.c
 * This function is internal only.
 *
 * @param x Square root argument.
 *
 * @return Result of square root.
 */
static uint32_t LVHB_GetSqrt(uint32_t x);

/*
 * @brief This function performs initial computations (delay of first
 * step, number of steps for deceleration phase, etc.) and initializes
 * acceleration ramp data.
 * This function is internal only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param accel Acceleration in number of full-steps per second^2.
 * @param decel Deceleration in number of full-steps per second^2.
 * @param speed Desired motor speed in number of steps per second.
 * @param timerFreq Input timer frequency in Hz.
 * @param timerFreqFactored Input timer frequency factored by 0.676.
 */
static void LVHB_InitRamp(lvhb_drv_config_t* const drvConfig, uint32_t accel, uint32_t decel,
    uint32_t speed, uint32_t timerFreq, uint32_t timerFreqFactored);

/*
 * @brief This function computes delay of next step on acceleration ramp.
 * This function is internal only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param stepsExecuted Number of executed steps.
 *
 * @return Delay of next step in number of timer ticks.
 */
static uint32_t LVHB_GetNextRampDelay(lvhb_drv_config_t* const drvConfig, uint32_t stepsExecuted);

/*
 * @brief This function sets GPIO outputs of H-Bridge channels
 * for current full-step.
 * This function is internal only.
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 */
static void LVHB_SetFullGpio(lvhb_drv_config_t* const drvConfig);

/*
 * @brief This function computes new index to the full-step table.
 * Function is used in both full-step and micro-step mode.
 * This function is internal only.
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param forward Motor direction.
 * @param stepShift Number of full-steps executed in last round
 *                  (until counter restart interrupt).
 */
static void LVHB_UpdateFullIdx(lvhb_drv_config_t* const drvConfig, bool forward, uint8_t stepShift);

/*
 * @brief This function updates counter of executed steps, current
 * full-step position and index to full-step sequence table.
 * This function is internal only.
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 */
static void LVHB_UpdateFullCntr(lvhb_drv_config_t* const drvConfig);

/*
 * @brief This function starts full-stepping.
 * This function enables timer interrupt, sets desired speed, etc.
 * This function is internal only.
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param forward Motor direction.
 * @param steps Number of steps to be executed.
 * @param period Speed in number of timer ticks per period.
 */
static void LVHB_StartFullstepping(lvhb_drv_config_t* const drvConfig, bool forward,
    uint32_t steps, uint32_t period);

/*
 * @brief This function sets channel offsets according to current full-step.
 * It sets offset greater than period when permanent log. 1 at channel output
 * is needed, else it sets zero offset.
 * This function is internal only.
 *
 * This function may be used for the stepper motors control with
 * micro-stepping support only - ie. all IN pins are used as timer outputs.
 *
 * @param drvConfig Pointer to driver instance configuration.
 */
static void LVHB_SetFullOffset(lvhb_drv_config_t* const drvConfig);

/*
 * @brief This function updates step counters.
 * This function is internal only.
 *
 * This function may be used for the stepper motors control with
 * micro-stepping support only - ie. all IN pins are used as timer outputs.
 *
 * @param drvConfig Pointer to driver instance configuration.
 */
static void LVHB_GotoNextFull(lvhb_drv_config_t* const drvConfig);

/*
 * @brief This function sets channel offsets according to the current micro-step.
 * This function is internal only.
 *
 * This function may be used for the stepper motors control with
 * micro-stepping support only - ie. all IN pins are used as timer outputs.
 *
 * @param drvConfig Pointer to driver instance configuration.
 */
static void LVHB_SetMicroOffset(lvhb_drv_config_t* const drvConfig);

/*
 * @brief This function updates indexes to the micro-step and full-step table.
 * This function is internal only.
 *
 * This function may be used for the stepper motors control with
 * micro-stepping support only - ie. all IN pins are used as timer outputs.
 *
 * @param drvConfig Pointer to driver instance configuration.
 */
static void LVHB_UpdateMicroIdx(lvhb_drv_config_t* const drvConfig);

/*
 * @brief This function updates number of executed micro-steps and full-steps.
 * This function is internal only.
 *
 * This function may be used for the stepper motors control with
 * micro-stepping support only - ie. all IN pins are used as timer outputs.
 *
 * @param drvConfig Pointer to driver instance configuration.
 */
static void LVHB_UpdateMicroCntr(lvhb_drv_config_t* const drvConfig);

/*
 * @brief This function updates full-step and micro-step counters, timer channel
 * offsets and number of timer overflows for a next micro-step.
 * It also stops the timer interrupts in case the movement is done.
 * This function is internal only.
 *
 * This function may be used for the stepper motors control with
 * micro-stepping support only - ie. all IN pins are used as timer outputs.
 *
 * @param drvConfig Pointer to driver instance configuration.
 */
static void LVHB_GotoNextMicro(lvhb_drv_config_t* const drvConfig);

/*
 * @brief This function starts micro-stepping.
 * This function is internal only.
 *
 * This function may be used for the stepper motors control with
 * micro-stepping support only - ie. all IN pins are used as timer outputs.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param forward Motor direction.
 * @param microSteps Number of steps to be executed.
 */
static void LVHB_StartMicrostepping(lvhb_drv_config_t* const drvConfig, bool forward, uint32_t microSteps);

/*******************************************************************************
 * Internal functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_GetPwmChCount
 * Description   : This function returns number of PWM channels which must be allocated.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static uint8_t LVHB_GetPwmChCount(lvhb_drv_config_t* const drvConfig)
{
    uint8_t chCnt = 0;

    /* Number of PWM channels used by the first H-Bridge. */
    if (drvConfig->inputPins[lvhbBridge1] == lvhbPinsPwm)
    {
        chCnt = 2;
    }
    else if (drvConfig->inputPins[lvhbBridge1] != lvhbPinsGpio)
    {
        chCnt = 1;
    }

    /* Number of PWM channels used by the second H-Bridge. */
    if (LVHB_IS_STEPPER_SELECTED(drvConfig) ||
        (LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device) && drvConfig->deviceConfig.secondaryBridgeUsed))
    {
        if (drvConfig->inputPins[lvhbBridge2] == lvhbPinsPwm)
        {
            chCnt += 2;
        }
        else if (drvConfig->inputPins[lvhbBridge2] != lvhbPinsGpio)
        {
            chCnt += 1;
        }
    }

    return chCnt;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetPwm
 * Description   : This function sets PWM channels to AML structure.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_SetPwm(lvhb_drv_config_t const *drvConfig,
        tmr_aml_pwm_ch_t* const amlPwmCh)
{
    int8_t chCnt = 0;

    /* PWM channels used by the first H-Bridge. */
    switch(drvConfig->inputPins[lvhbBridge1])
    {
        case lvhbPinsPwm:
            amlPwmCh[0].channel = drvConfig->tmrLvhbConfig.inxaChannelNumber[lvhbBridge1];
            amlPwmCh[1].channel = drvConfig->tmrLvhbConfig.inxbChannelNumber[lvhbBridge1];
            chCnt = 2;
            break;
        case lvhbPinsPwmGpio:
            amlPwmCh[0].channel = drvConfig->tmrLvhbConfig.inxaChannelNumber[lvhbBridge1];
            chCnt = 1;
            break;
        case lvhbPinsGpioPwm:
            amlPwmCh[0].channel = drvConfig->tmrLvhbConfig.inxbChannelNumber[lvhbBridge1];
            chCnt = 1;
            break;
        default:
            break;
    }

    /* PWM channels used by the second H-Bridge. */
    if (LVHB_IS_STEPPER_SELECTED(drvConfig) ||
        (LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device) && drvConfig->deviceConfig.secondaryBridgeUsed))
    {
        switch(drvConfig->inputPins[lvhbBridge2])
        {
            case lvhbPinsPwm:
                amlPwmCh[chCnt].channel = drvConfig->tmrLvhbConfig.inxaChannelNumber[lvhbBridge2];
                amlPwmCh[chCnt + 1].channel = drvConfig->tmrLvhbConfig.inxbChannelNumber[lvhbBridge2];
                chCnt += 2;
                break;
            case lvhbPinsPwmGpio:
                amlPwmCh[chCnt].channel = drvConfig->tmrLvhbConfig.inxaChannelNumber[lvhbBridge2];
                chCnt++;
                break;
            case lvhbPinsGpioPwm:
                amlPwmCh[chCnt].channel = drvConfig->tmrLvhbConfig.inxbChannelNumber[lvhbBridge2];
                chCnt++;
                break;
            default:
                break;
        }
    }

    for(int8_t i = 0; i < chCnt; i++)
    {
        /* Set initialization channels property. */
        amlPwmCh[i].dutyPercent = 0U;
        amlPwmCh[i].pwmAction = tmrPwmActHighTrue;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetZeroOut
 * Description   : This function sets outputs of specified H-Bridge to LOW.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_SetZeroOut(lvhb_drv_config_t* const drvConfig, lvhb_bridge_t bridge)
{
    lvhb_input_pins_t inputPins = drvConfig->inputPins[bridge];
    uint16_t periodTicks = 0;

    if (inputPins != lvhbPinsGpio)
    {
        periodTicks = TMR_AML_GetPeriodTicks(drvConfig->tmrInstance);
    }

    /* Set INxA/IN1 pin. */
    if ((inputPins == lvhbPinsPwm) || (inputPins == lvhbPinsPwmGpio))
    {
        TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                drvConfig->tmrLvhbConfig.inxaChannelNumber[bridge],
                LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? periodTicks : 0U);
    }
    else
    {
        GPIO_AML_WriteOutput(drvConfig->inxaPinInstance[bridge],
                drvConfig->inxaPinIndex[bridge],
                LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? true : false);
    }

    /* Set INxB/IN2 pin. */
    if ((inputPins == lvhbPinsPwm) || (inputPins == lvhbPinsGpioPwm))
    {
        TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                drvConfig->tmrLvhbConfig.inxbChannelNumber[bridge],
                LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? periodTicks : 0U);
    }
    else
    {
        GPIO_AML_WriteOutput(drvConfig->inxbPinInstance[bridge],
                drvConfig->inxbPinIndex[bridge],
                LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? true : false);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_GetSqrt
 * Description   : This function gets the square root. Taken from sqrt.c, function grupe.
 *                 This function is internal only.
 * Author:         Eyal Lebedinsky  (eyal@ise.canberra.edu.au).
 * Web:            https://ftp.samba.org/pub/eyal/samples/sqrt.c
 *
 *END**************************************************************************/
static uint32_t LVHB_GetSqrt(uint32_t x)
{
    register uint32_t xr;     /* Result register. */
    register uint32_t q2;     /* Scan-bit register. */
    register uint8_t f;       /* Flag */

    xr = 0;                   /* Clear result. */
    q2 = 0x40000000L;         /* Highest possible result bit. */
    do
    {
        if ((xr + q2) <= x)
        {
            x -= xr + q2;
            f = 1;            /* Set flag. */
        }
        else
        {
            f = 0;            /* Clear flag. */
        }

        xr >>= 1;

        if (f)
        {
            xr += q2;         /* Test flag. */
        }
    } while (q2 >>= 2);       /* Shift twice. */

    if (xr < x)
    {
        return xr +1;         /* Added for rounding. */
    }
    else
    {
        return xr;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_InitRamp
 * Description   : This function performs initial computations (delay of first
 *                 step, number of steps for deceleration phase, etc.) and
 *                 initializes acceleration ramp data.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_InitRamp(lvhb_drv_config_t* const drvConfig, uint32_t accel, uint32_t decel,
    uint32_t speed, uint32_t timerFreq, uint32_t timerFreqFactored)
{
    uint32_t maxAccelStepCnt; /* Number of steps for acceleration. */
    uint32_t maxSpeedStepCnt; /* Number of steps to reach desired speed. */

    lvhb_stepper_data_t *stepperData = &(drvConfig->deviceConfig.stepperData);
    uint32_t steps = stepperData->steps;

    /* Compute delay of full-step when max. speed is set. */
    stepperData->maxSpeedDelay = timerFreq / speed;
    if (stepperData->maxSpeedDelay == 0)
    {
        stepperData->maxSpeedDelay = 1;
    }

    /* Compute delay of first step on the ramp. */
    stepperData->currentDelay = (uint32_t)(((uint64_t)timerFreqFactored *
            (uint64_t)LVHB_GetSqrt((2 * 100000000) / accel)) / (uint64_t)10000U);
    stepperData->currentDelayRest = 0U;

    if (stepperData->currentDelay == 0U)
    {
        stepperData->currentDelay = 1U;
    }
    if (stepperData->currentDelay <= stepperData->maxSpeedDelay)
    {
        /* Acceleration speed is higher than desired speed so do not use
         * acceleration and deceleration drvConfig. */
        stepperData->currentDelay = stepperData->maxSpeedDelay;
        stepperData->decelStepCnt = 0U;
        stepperData->rampState = lvhbRampConstSpeed;
    }
    else
    {
        stepperData->rampState = lvhbRampUpSpeed;
    }

    if (steps <= 2U)
    {
        /* Do not use acceleration and deceleration.
         * Set the lowest value of acceleration ramp and run at constant speed. */
        stepperData->decelStepCnt = 0U;
        stepperData->rampState = lvhbRampOff;
    }
    else
    {
        /* Compute number of steps needed to achieve desired speed. */
        maxSpeedStepCnt = (speed * speed) / (2U * accel);
        if (maxSpeedStepCnt == 0U)
        {
            maxSpeedStepCnt = 1U;
        }

        /* Compute maximal number of steps needed for acceleration ramp (it is
         * half of steps specified by user when acceleration is equal to
         * deceleration). */
        maxAccelStepCnt = (uint32_t)(((uint64_t)steps * (uint64_t)decel) /
            (uint64_t)(accel + decel));
        if (maxAccelStepCnt == 0U)
        {
            maxAccelStepCnt = 1U;
        }

        /* Compute number of steps needed for deceleration phase. */
        if (maxAccelStepCnt <= maxSpeedStepCnt)
        {
            stepperData->decelStepCnt = steps - maxAccelStepCnt;
        }
        else
        {
            stepperData->decelStepCnt = (maxSpeedStepCnt * accel) / decel;
        }

        if (stepperData->decelStepCnt == 0U)
        {
            stepperData->decelStepCnt = 1U;
        }
    } /* END IF */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_GetNextRampDelay
 * Description   : This function computes delay of next step on acceleration ramp.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static uint32_t LVHB_GetNextRampDelay(lvhb_drv_config_t* const drvConfig, uint32_t stepsExecuted)
{
    uint32_t nextStepDelay;   /* Computed value of next step delay. */
    uint32_t delayOffset;     /* Offset of from previous step. */
    uint32_t dividend;        /* Value used in step delay computation. */
    uint64_t divisor;         /* Value used in step delay computation. */
    lvhb_stepper_data_t *stepperData = &(drvConfig->deviceConfig.stepperData);

    if (stepperData->rampState == lvhbRampUpSpeed)
    {
        /* Acceleration. */
        dividend = (2U * stepperData->currentDelay) + stepperData->currentDelayRest;
        divisor = ((uint64_t)4U * (uint64_t)stepsExecuted) + (uint64_t)1U;
        delayOffset = (uint32_t)((uint64_t)dividend / divisor);

        /* Compute new delay. */
        stepperData->currentDelay = stepperData->currentDelay - delayOffset;
        nextStepDelay = stepperData->currentDelay;
        /* Compute rest of division. */
        stepperData->currentDelayRest = (uint32_t)LVHB_COMPUTE_REMAINDER((uint64_t)dividend,
            divisor, (uint64_t)delayOffset);

        if (stepperData->currentDelay <= stepperData->maxSpeedDelay)
        {
            /* Speed set by user is achieved. */
            stepperData->rampState = lvhbRampConstSpeed;
            nextStepDelay = stepperData->maxSpeedDelay;
            stepperData->currentDelayRest = 0U;
        }
        if (stepsExecuted >= ((stepperData->steps) - stepperData->decelStepCnt))
        {
            /* Deceleration have to start immediately. */
            stepperData->rampState = lvhbRampDownSpeed;
            stepperData->currentDelayRest = 0U;
        }
    }
    else if (stepperData->rampState == lvhbRampConstSpeed)
    {
        /* Constant speed. */
        if (stepsExecuted >= ((stepperData->steps) - stepperData->decelStepCnt))
        {
            /* Start deceleration. */
            stepperData->rampState = lvhbRampDownSpeed;
        }
        nextStepDelay = stepperData->maxSpeedDelay;
    }
    else if (stepperData->rampState == lvhbRampDownSpeed)
    {
        /* Deceleration. */
        if (stepperData->decelStepCnt <= 1U)
        {
            /* End of deceleration. */
            stepperData->rampState = lvhbRampOff;
            stepperData->decelStepCnt = 0U;
            nextStepDelay = stepperData->currentDelay;
        }
        else
        {
            stepperData->decelStepCnt--;

            dividend = (2U * stepperData->currentDelay) + stepperData->currentDelayRest;
            divisor = ((uint64_t)4U * (uint64_t)stepperData->decelStepCnt) - (uint64_t)1U;
            delayOffset = (uint32_t)((uint64_t)dividend / divisor);

            /* Compute new delay. */
            stepperData->currentDelay = stepperData->currentDelay + delayOffset;
            nextStepDelay = stepperData->currentDelay;
            /* Compute rest of division. */
            stepperData->currentDelayRest = (uint32_t)LVHB_COMPUTE_REMAINDER((uint64_t)dividend,
                    divisor, (uint64_t)delayOffset);
        }
    }
    else
    {
        /* i.e. if (stepperData->rampState == lvhbRampOff) */
        /* Use the latest computed value. */
        nextStepDelay = stepperData->currentDelay;
    }

    return nextStepDelay;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetFullGpio
 * Description   : This function sets GPIO outputs of H-Bridge channels for
 *                 current full-step.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_SetFullGpio(lvhb_drv_config_t* const drvConfig)
{
    uint8_t fullStepIdx = drvConfig->deviceConfig.stepperData.fullStepIdx;

    GPIO_AML_WriteOutput(drvConfig->inxaPinInstance[lvhbBridge1], drvConfig->inxaPinIndex[lvhbBridge1], LVHB_FULLSTEP_SEQ[fullStepIdx] & LVHB_FULLSTEP_CH_IN1A);
    GPIO_AML_WriteOutput(drvConfig->inxbPinInstance[lvhbBridge1], drvConfig->inxbPinIndex[lvhbBridge1], LVHB_FULLSTEP_SEQ[fullStepIdx] & LVHB_FULLSTEP_CH_IN1B);
    GPIO_AML_WriteOutput(drvConfig->inxaPinInstance[lvhbBridge2], drvConfig->inxaPinIndex[lvhbBridge2], LVHB_FULLSTEP_SEQ[fullStepIdx] & LVHB_FULLSTEP_CH_IN2A);
    GPIO_AML_WriteOutput(drvConfig->inxbPinInstance[lvhbBridge2], drvConfig->inxbPinIndex[lvhbBridge2], LVHB_FULLSTEP_SEQ[fullStepIdx] & LVHB_FULLSTEP_CH_IN2B);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_UpdateFullIdx
 * Description   : This function computes new index to the full-step table.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_UpdateFullIdx(lvhb_drv_config_t* const drvConfig, bool forward, uint8_t stepShift)
{
    uint8_t fullStepIdx = drvConfig->deviceConfig.stepperData.fullStepIdx;

    /* Select only lower 3 bits (like modulo 4). */
    stepShift &= 3;

    if (forward)
    {
        drvConfig->deviceConfig.stepperData.fullStepIdx = (fullStepIdx + stepShift >= LVHB_FULLSTEP_CYCLE) ?
                ((fullStepIdx + stepShift) - LVHB_FULLSTEP_CYCLE) : (fullStepIdx + stepShift);
    }
    else
    {
        drvConfig->deviceConfig.stepperData.fullStepIdx = (fullStepIdx  < stepShift) ?
                ((fullStepIdx + LVHB_FULLSTEP_CYCLE) - stepShift) : (fullStepIdx - stepShift);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_UpdateFullCntr
 * Description   : This function updates counter of executed steps, current
 *                 full-step position and index to full-step sequence table.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_UpdateFullCntr(lvhb_drv_config_t* const drvConfig)
{
    lvhb_stepper_data_t *stepperData = &(drvConfig->deviceConfig.stepperData);

    LVHB_UpdateFullIdx(drvConfig, stepperData->forward, stepperData->fullStepsPerPeriod);

    stepperData->stepPos = (stepperData->forward) ?
            stepperData->stepPos + stepperData->fullStepsPerPeriod :
            stepperData->stepPos - stepperData->fullStepsPerPeriod;

    stepperData->stepCntr += stepperData->fullStepsPerPeriod;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_StartFullstepping
 * Description   : This function starts full-stepping.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_StartFullstepping(lvhb_drv_config_t* const drvConfig, bool forward,
    uint32_t steps, uint32_t period)
{
    lvhb_stepper_config_t *stepperConfig = &(drvConfig->deviceConfig.stepperConfig);
    lvhb_stepper_data_t *stepperData = &(drvConfig->deviceConfig.stepperData);
    lvhb_stepper_mode_t prevMode = stepperData->stepperMode;   /* Previous stepping mode. */

    stepperData->steps = steps;
    stepperData->stepCntr = 0;
    stepperData->stepperMode = lvhbStepperModeFullstep;
    stepperData->controlState = (LVHB_HAS_GPIO_IN_PINS(drvConfig)) ? lvhbStepperStateSlowStart : lvhbStepperStateSleepLoop;
    stepperData->forward = forward;
    stepperData->currentFullSpeed = period;
    stepperData->rampState = lvhbRampOff;
    stepperData->microPosInFull = 0;

    if (LVHB_HAS_PWM_IN_PINS(drvConfig))
    {
        stepperData->fullStepsPerPeriod = 1;
    }

    /* Set speed. */
    if (stepperConfig->fullStepAcceleration == 0)
    {
        stepperData->timerOverflows = (period / LVHB_TIMER_VALUE_MAX(drvConfig)) + 1;
        period = period / stepperData->timerOverflows;
    }
    else
    {
        /* Compute delay of first step. */
        LVHB_InitRamp(drvConfig, stepperConfig->fullStepAcceleration, stepperConfig->fullStepAcceleration,
                LVHB_FULLSTEP_IN_FREQ(drvConfig) / period, LVHB_FULLSTEP_IN_FREQ(drvConfig), LVHB_FULLSTEP_IN_FREQ_0676(drvConfig));

        stepperData->timerOverflows = (stepperData->currentDelay / LVHB_TIMER_VALUE_MAX(drvConfig)) + 1;
        period = stepperData->currentDelay / stepperData->timerOverflows;
    }

    TMR_AML_EnableInterrupts(drvConfig->tmrInstance, tmrIntTimeOverflow);

    if (LVHB_HAS_PWM_IN_PINS(drvConfig))
    {
        TMR_AML_ResetTimer(drvConfig->tmrInstance);
    }

    TMR_AML_SetPeriodTicks(drvConfig->tmrInstance, period);

    if (LVHB_HAS_PWM_IN_PINS(drvConfig) && (prevMode != lvhbStepperModeFullstep))
    {
        /* Set current micro-step (channels offset). */
        LVHB_SetFullOffset(drvConfig);
    }
    else
    {
        /* Set current full-step. */
        LVHB_SetFullGpio(drvConfig);
    }


    /* Signal for user. */
    stepperData->motorStatus = lvhbStatusRunning;
    stepperData->controlState = lvhbStepperStateInitCompleted;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetFullOffset
 * Description   : This function sets channel offsets according to current full-step.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_SetFullOffset(lvhb_drv_config_t* const drvConfig)
{
    uint32_t offset;      /* Channels offset. */
    uint8_t fullStepIdx = drvConfig->deviceConfig.stepperData.fullStepIdx;

    offset = ((LVHB_FULLSTEP_SEQ[fullStepIdx] & LVHB_FULLSTEP_CH_IN1A) != LVHB_FULLSTEP_CH_INACTIVE) ? LVHB_TIMER_VALUE_MAX(drvConfig) - 1 : 0;
    TMR_AML_SetOffsetTicks(drvConfig->tmrInstance, drvConfig->tmrLvhbConfig.inxaChannelNumber[0], offset);

    offset = ((LVHB_FULLSTEP_SEQ[fullStepIdx] & LVHB_FULLSTEP_CH_IN1B) != LVHB_FULLSTEP_CH_INACTIVE) ? LVHB_TIMER_VALUE_MAX(drvConfig) - 1 : 0;
    TMR_AML_SetOffsetTicks(drvConfig->tmrInstance, drvConfig->tmrLvhbConfig.inxbChannelNumber[0], offset);

    offset = ((LVHB_FULLSTEP_SEQ[fullStepIdx] & LVHB_FULLSTEP_CH_IN2A) != LVHB_FULLSTEP_CH_INACTIVE) ? LVHB_TIMER_VALUE_MAX(drvConfig) - 1 : 0;
    TMR_AML_SetOffsetTicks(drvConfig->tmrInstance, drvConfig->tmrLvhbConfig.inxaChannelNumber[1], offset);

    offset = ((LVHB_FULLSTEP_SEQ[fullStepIdx] & LVHB_FULLSTEP_CH_IN2B) != LVHB_FULLSTEP_CH_INACTIVE) ? LVHB_TIMER_VALUE_MAX(drvConfig) - 1 : 0;
    TMR_AML_SetOffsetTicks(drvConfig->tmrInstance, drvConfig->tmrLvhbConfig.inxbChannelNumber[1], offset);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_GotoNextFull
 * Description   : This function updates step counters.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_GotoNextFull(lvhb_drv_config_t* const drvConfig)
{
    lvhb_stepper_config_t *stepperConfig = &(drvConfig->deviceConfig.stepperConfig);
    lvhb_stepper_data_t *stepperData = &(drvConfig->deviceConfig.stepperData);
    uint32_t remainingSteps;  /* Number of remaining steps. */
    uint32_t nextPeriod; /* Used for ramp. */

    if (stepperData->continual && (stepperData->stepCntr > 0))
    {
        /* Decrease number of steps in continual movement. */
        stepperData->stepCntr--;
    }

    /* Increment by 1 is due to logic in OnCounterRestart handler. */
    remainingSteps = (stepperData->steps + 1) - (stepperData->stepCntr + 1);

    if (remainingSteps > 0)
    {
        LVHB_UpdateFullCntr(drvConfig);
        LVHB_SetFullOffset(drvConfig);

        /* Set number of timer overflows. */
        if (stepperConfig->fullStepAcceleration == 0)
        {
            stepperData->timerOverflows = (stepperData->currentFullSpeed / LVHB_TIMER_VALUE_MAX(drvConfig)) + 1;
        }
        else
        {
            /* Compute period of next step. */
            nextPeriod = LVHB_GetNextRampDelay(drvConfig, stepperData->stepCntr);
            stepperData->timerOverflows = (nextPeriod / LVHB_TIMER_VALUE_MAX(drvConfig)) + 1;
            nextPeriod = nextPeriod / stepperData->timerOverflows;
            TMR_AML_SetPeriodTicks(drvConfig->tmrInstance, nextPeriod);
        }
    }
    else
    {
        /* Stop changing steps. */
        TMR_AML_DisableInterrupts(drvConfig->tmrInstance, tmrIntTimeOverflow);
        stepperData->motorStatus = lvhbStatusStop;

        LVHB_OnActionComplete(drvConfig);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetMicrostep
 * Description   : This function sets channel offsets according to the current micro-step.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_SetMicroOffset(lvhb_drv_config_t* const drvConfig)
{
    lvhb_stepper_data_t *stepperData = &(drvConfig->deviceConfig.stepperData);
    uint16_t         offsetVal; /* Channels offset. */
    uint8_t          idx1;      /* Micro-step table index for first winding. */
    uint8_t          idx2;      /* Micro-step table index for second winding. */

    /* Compute indexes to the step table. */
    idx1 = (stepperData->microWind1Idx > LVHB_MICROSTEP_CNT_MAX) ?
            (LVHB_MICROSTEP_CNT_MAX * 2 - stepperData->microWind1Idx) :
            stepperData->microWind1Idx;
    idx2 = (stepperData->microWind2Idx > LVHB_MICROSTEP_CNT_MAX) ?
            (LVHB_MICROSTEP_CNT_MAX * 2 - stepperData->microWind2Idx) :
            stepperData->microWind2Idx;

    /* Update offset of TimerUnit channels */
    offsetVal = LVHB_GET_MICRO_OFFSET(drvConfig, LVHB_FULLSTEP_CH_IN1A, LVHB_FULLSTEP_SEQ[stepperData->fullStepIdx], idx1);
    TMR_AML_SetOffsetTicks(drvConfig->tmrInstance, drvConfig->tmrLvhbConfig.inxaChannelNumber[0], offsetVal);

    offsetVal = LVHB_GET_MICRO_OFFSET(drvConfig, LVHB_FULLSTEP_CH_IN1B, LVHB_FULLSTEP_SEQ[stepperData->fullStepIdx], idx1);
    TMR_AML_SetOffsetTicks(drvConfig->tmrInstance, drvConfig->tmrLvhbConfig.inxbChannelNumber[0], offsetVal);

    offsetVal = LVHB_GET_MICRO_OFFSET(drvConfig, LVHB_FULLSTEP_CH_IN2A, LVHB_FULLSTEP_SEQ[stepperData->fullStepIdx], idx2);
    TMR_AML_SetOffsetTicks(drvConfig->tmrInstance, drvConfig->tmrLvhbConfig.inxaChannelNumber[1], offsetVal);

    offsetVal = LVHB_GET_MICRO_OFFSET(drvConfig, LVHB_FULLSTEP_CH_IN2B, LVHB_FULLSTEP_SEQ[stepperData->fullStepIdx], idx2);
    TMR_AML_SetOffsetTicks(drvConfig->tmrInstance, drvConfig->tmrLvhbConfig.inxbChannelNumber[1], offsetVal);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_UpdateMicroIdx
 * Description   : This function updates indexes to the micro-step and full-step table.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_UpdateMicroIdx(lvhb_drv_config_t* const drvConfig)
{
    lvhb_stepper_config_t *stepperConfig = &(drvConfig->deviceConfig.stepperConfig);
    lvhb_stepper_data_t *stepperData = &(drvConfig->deviceConfig.stepperData);

    if (stepperData->forward)
    {
        /* Update indexes to the step table. */
        stepperData->microWind1Idx =
                (stepperData->microWind1Idx + stepperConfig->microStepPerStep == LVHB_MICROSTEP_CNT_MAX * 2) ?
                0 : stepperData->microWind1Idx + stepperConfig->microStepPerStep;

        stepperData->microWind2Idx =
                (stepperData->microWind2Idx + stepperConfig->microStepPerStep == LVHB_MICROSTEP_CNT_MAX * 2) ?
                0 : stepperData->microWind2Idx + stepperConfig->microStepPerStep;
    }
    else
    {
        /* Update indexes to the step table. */
        stepperData->microWind1Idx = (stepperData->microWind1Idx == 0) ?
                (LVHB_MICROSTEP_CNT_MAX * 2) - stepperConfig->microStepPerStep :
                stepperData->microWind1Idx - stepperConfig->microStepPerStep;

        stepperData->microWind2Idx = (stepperData->microWind2Idx == 0) ?
                (LVHB_MICROSTEP_CNT_MAX * 2) - stepperConfig->microStepPerStep :
                stepperData->microWind2Idx - stepperConfig->microStepPerStep;
    }

    /* Update full-step index. */
    if ((stepperData->microWind1Idx == LVHB_MICROSTEP_CNT_MAX) || (stepperData->microWind2Idx == LVHB_MICROSTEP_CNT_MAX))
    {
        /* Update full-step position. */
        LVHB_UpdateFullIdx(drvConfig, stepperData->forward, 1);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_UpdateMicroCntr
 * Description   : This function updates number of executed micro-steps and full-steps.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_UpdateMicroCntr(lvhb_drv_config_t* const drvConfig)
{
    lvhb_stepper_config_t *stepperConfig = &(drvConfig->deviceConfig.stepperConfig);
    lvhb_stepper_data_t *stepperData = &(drvConfig->deviceConfig.stepperData);
    if (stepperData->forward)
    {
        stepperData->microPosInFull += (int8_t)stepperConfig->microStepPerStep;

        if (stepperData->microPosInFull >= LVHB_MICROSTEP_CNT_MAX)
        {
            stepperData->microPosInFull = 0;
            stepperData->stepPos++;
        }
    }
    else
    {
        stepperData->microPosInFull -= (int8_t)stepperConfig->microStepPerStep;

        if (stepperData->microPosInFull <= -(int8_t)LVHB_MICROSTEP_CNT_MAX)
        {
            stepperData->microPosInFull = 0;
            stepperData->stepPos--;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_GotoNextMicro
 * Description   : This function updates step counters, timer channel offsets and
 *                 number of timer overflows for a next micro-step.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_GotoNextMicro(lvhb_drv_config_t* const drvConfig)
{
    lvhb_stepper_config_t *stepperConfig = &(drvConfig->deviceConfig.stepperConfig);
    lvhb_stepper_data_t *stepperData = &(drvConfig->deviceConfig.stepperData);
    uint32_t remainingSteps;    /* Number of remaining steps. */

    /* Note: micro-step counter is independent of micro-step size. */
    stepperData->stepCntr++;

    if (stepperData->continual && stepperData->stepCntr > 1)
    {
        /* Decrease number of steps in continual movement. */
        stepperData->stepCntr--;
    }

    /* Increment by 1 is due to logic in OnCounterRestart handler. */
    remainingSteps = (stepperData->steps + 1) - stepperData->stepCntr;

    if (remainingSteps > 0)
    {
        /* Set number of timer overflows. */
        if (stepperConfig->microStepAcceleration == 0)
        {
            stepperData->timerOverflows = stepperData->microSpeed;
        }
        else
        {
            stepperData->timerOverflows = LVHB_GetNextRampDelay(drvConfig, stepperData->stepCntr) / LVHB_MICROSTEP_PERIOD_TICKS(drvConfig);

            if (stepperData->timerOverflows == 0)
            {
                /* Minimal value is 1. */
                stepperData->timerOverflows = 1;
            }
        }

        LVHB_UpdateMicroIdx(drvConfig);
        LVHB_SetMicroOffset(drvConfig);
        LVHB_UpdateMicroCntr(drvConfig);
    }
    else
    {
        /* Stop changing micro-steps. */
        TMR_AML_DisableInterrupts(drvConfig->tmrInstance, tmrIntTimeOverflow);
        stepperData->motorStatus = lvhbStatusStop;

        LVHB_OnActionComplete(drvConfig);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_StartMicrostepping
 * Description   : This function starts micro-stepping.
 *                 This function is internal only.
 *
 *END**************************************************************************/
static void LVHB_StartMicrostepping(lvhb_drv_config_t* const drvConfig, bool forward, uint32_t microSteps)
{
    lvhb_stepper_config_t *stepperConfig = &(drvConfig->deviceConfig.stepperConfig);
    lvhb_stepper_data_t *stepperData = &(drvConfig->deviceConfig.stepperData);
    lvhb_stepper_mode_t prevMode = stepperData->stepperMode; /* Flag for previous stepping mode. */
    uint8_t idxOffset; /* Value used to correct micro-step index. */

    stepperData->timerOverflows = stepperData->microSpeed;
    stepperData->steps = microSteps;
    stepperData->stepCntr = 0;
    stepperData->stepperMode = lvhbStepperModeMicrostep;
    stepperData->controlState = lvhbStepperStateSleepLoop;

    if (drvConfig->deviceConfig.stepperConfig.microStepAcceleration > 0)
    {
        stepperData->rampState = lvhbRampOff;

        /* Compute delay of first step. */
        LVHB_InitRamp(drvConfig, stepperConfig->microStepAcceleration, stepperConfig->microStepAcceleration,
            LVHB_GET_MICRO_SPEED(drvConfig, stepperData->microSpeed), LVHB_MICROSTEP_IN_FREQ(drvConfig), LVHB_MICROSTEP_IN_FREQ_0676(drvConfig));
        stepperData->timerOverflows = stepperData->currentDelay / LVHB_MICROSTEP_PERIOD_TICKS(drvConfig);
    }

    if (prevMode != lvhbStepperModeMicrostep)
    {
        /* Set micro-step mode. */

        /* Compute indexes to the micro-step table according to the current full-step. */
        stepperData->microWind1Idx = (stepperData->fullStepIdx & 1) ? LVHB_WIND2_IDX_INIT : LVHB_WIND1_IDX_INIT;
        stepperData->microWind2Idx = (stepperData->fullStepIdx & 1) ? LVHB_WIND1_IDX_INIT : LVHB_WIND2_IDX_INIT;

        /* Set micro-step position to beginning. */
        stepperData->microPosInFull = 0;

        /* Set direction. */
        stepperData->forward = forward;

        TMR_AML_EnableInterrupts(drvConfig->tmrInstance, tmrIntTimeOverflow);
        TMR_AML_ResetTimer(drvConfig->tmrInstance);
        TMR_AML_SetPeriodTicks(drvConfig->tmrInstance, LVHB_MICROSTEP_PERIOD_TICKS(drvConfig));

        /* Set current micro-step (channels offset). */
        LVHB_SetMicroOffset(drvConfig);
    }
    else
    {
        /* Micro-stepping preceded. */

        /* Set desired motor direction */
        if (forward != stepperData->forward)
        {
            /* Change direction. */
            if ((stepperData->microWind1Idx == 0) || (stepperData->microWind1Idx == LVHB_MICROSTEP_CNT_MAX))
            {
                /* Correction of current full-step when the rotor is at the beginning or at the end of full-step. */
                LVHB_UpdateFullIdx(drvConfig, forward, 1);
            }

            stepperData->forward = forward;
        }

        /* Change micro-step indexes when user changed size of micro-step. */
        if (stepperConfig->microStepPerStep > 1U)
        {
            idxOffset = stepperData->microWind1Idx & (stepperConfig->microStepPerStep - 1U);
            if ((stepperData->microWind1Idx & idxOffset) != 0U)
            {
                /* Correct indexes. */
                stepperData->microWind1Idx = forward ?
                        (stepperData->microWind1Idx - idxOffset) :
                        (stepperData->microWind1Idx + (stepperConfig->microStepPerStep - idxOffset));
                stepperData->microWind2Idx = forward ?
                        (stepperData->microWind2Idx - idxOffset) :
                        (stepperData->microWind2Idx + (stepperConfig->microStepPerStep - idxOffset));
            }
        }

        TMR_AML_EnableInterrupts(drvConfig->tmrInstance, tmrIntTimeOverflow);
    }

    /* Signal for user. */
    stepperData->motorStatus = lvhbStatusRunning;
    stepperData->controlState = lvhbStepperStateInitCompleted;
}

/******************************************************************************
 * API
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_ConfigureGpio
 * Description   : This function configures GPIO for usage with this driver.
 *
 *END**************************************************************************/
status_t LVHB_ConfigureGpio(lvhb_drv_config_t *drvConfig)
{
    AML_ASSERT(drvConfig != NULL);

    if (LVHB_IS_STEPPER_SELECTED(drvConfig))
    {
        /* No combination of GPIO and timer channel pins is supported for stepper motor. */
        AML_ASSERT(LVHB_HAS_GPIO_IN_PINS(drvConfig) || LVHB_HAS_PWM_IN_PINS(drvConfig));
    }

    /* EN/OE/PSAVE pin. */
    if (LVHB_HAS_EN_PIN(drvConfig->deviceConfig.device))
    {
        GPIO_AML_SetDirection(drvConfig->enPinInstance, drvConfig->enPinIndex, gpioDirDigitalOutput);
    }

    /* GIN pin. */
    if (LVHB_HAS_GIN_PIN(drvConfig->deviceConfig.device))
    {
        GPIO_AML_SetDirection(drvConfig->ginPinInstance, drvConfig->ginPinIndex, gpioDirDigitalOutput);
    }

    /* IN1A/IN1 pin. */
    if ((drvConfig->inputPins[lvhbBridge1] == lvhbPinsGpio) || (drvConfig->inputPins[lvhbBridge1] == lvhbPinsGpioPwm))
    {
        GPIO_AML_SetDirection(drvConfig->inxaPinInstance[lvhbBridge1], drvConfig->inxaPinIndex[lvhbBridge1], gpioDirDigitalOutput);
    }

    /* IN1B/IN2 pin. */
    if ((drvConfig->inputPins[lvhbBridge1] == lvhbPinsGpio) || (drvConfig->inputPins[lvhbBridge1] == lvhbPinsPwmGpio))
    {
        GPIO_AML_SetDirection(drvConfig->inxbPinInstance[lvhbBridge1], drvConfig->inxbPinIndex[lvhbBridge1], gpioDirDigitalOutput);
    }

    if (LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device) &&
        (LVHB_IS_STEPPER_SELECTED(drvConfig) || drvConfig->deviceConfig.secondaryBridgeUsed))
    {
        /* IN2A pin. */
        if ((drvConfig->inputPins[lvhbBridge2] == lvhbPinsGpio) || (drvConfig->inputPins[lvhbBridge2] == lvhbPinsGpioPwm))
        {
            GPIO_AML_SetDirection(drvConfig->inxaPinInstance[lvhbBridge2], drvConfig->inxaPinIndex[lvhbBridge2], gpioDirDigitalOutput);
        }

        /* IN2B pin. */
        if ((drvConfig->inputPins[lvhbBridge2] == lvhbPinsGpio) || (drvConfig->inputPins[lvhbBridge2] == lvhbPinsPwmGpio))
        {
            GPIO_AML_SetDirection(drvConfig->inxbPinInstance[lvhbBridge2], drvConfig->inxbPinIndex[lvhbBridge2], gpioDirDigitalOutput);
        }
    }

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_ConfigureTimer
 * Description   : This function configures Timer for usage with this driver.
 *
 *END**************************************************************************/
status_t LVHB_ConfigureTimer(lvhb_drv_config_t* const drvConfig, tmr_sdk_config_t* const tmrSdkConfig)
{
    AML_ASSERT(drvConfig != NULL);

    if (LVHB_IS_STEPPER_SELECTED(drvConfig))
    {
        /* No combination of GPIO and timer channel pins is supported for stepper motor. */
        AML_ASSERT(LVHB_HAS_GPIO_IN_PINS(drvConfig) || LVHB_HAS_PWM_IN_PINS(drvConfig));

        /* Stepper motor may be controlled by a device with two H-Bridges only. */
        AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));
    }

    /* Count number of PWM Channels. */
    uint8_t pwmChCount = LVHB_GetPwmChCount(drvConfig);

    /* SDK configuration. */
    /* SDK configuration of FTM/TPM device for AML layer. */
    tmr_sdk_config_t lvhbSdkConf;
    /* SDK configuration pointer of FTM/TPM device for AML layer. */
    tmr_sdk_config_t *sdkConf;

    /* SDK configuration of channels in PWM mode. */
    tmr_sdk_pwm_ch_config_t sdkPwmCh[LVHB_PWM_CH_MAX];
    tmr_sdk_pwm_ch_config_t* sdkPwmChPtr = sdkPwmCh;

    /* AML configuration structures (SDK independent). */
    /* AML timer configuration. */
    tmr_aml_config_t amlConf;
    /* AML configuration of channels in PWM mode. */
    tmr_aml_pwm_ch_t amlPwmCh[LVHB_PWM_CH_MAX];

    /* Error code. */
    status_t status = kStatus_Success;

    if (LVHB_IS_BRUSHED_MOTOR_SELECTED(drvConfig) && (drvConfig->deviceConfig.brushPwmFrequency > LVHB_BRUSH_PWM_FREQ_MAX))
    {
        return kStatus_LVHB_InvalidDriverConfig;
    }

    if (tmrSdkConfig == NULL)
    {
        sdkConf = &lvhbSdkConf;

        /* AML (SDK independent) configuration of the timer including general
         * timer settings (clock source, etc.) and channels settings. */
        amlConf.gtbEnabled = false;
        amlConf.prescale = drvConfig->tmrLvhbConfig.prescale;
        amlConf.srcClck_Hz = drvConfig->tmrLvhbConfig.srcClck_Hz;
        amlConf.freqHz = (LVHB_IS_BRUSHED_MOTOR_SELECTED(drvConfig)) ? drvConfig->deviceConfig.brushPwmFrequency : LVHB_STEPPER_INIT_PWM_FREQ;

        /* PWM mode configuration of channels.
         * Note: default is edge aligned mode. */
        LVHB_SetPwm(drvConfig, amlPwmCh);

        amlConf.chPwm = (tmr_aml_pwm_ch_t*)amlPwmCh;
        amlConf.chPwmCnt = pwmChCount;
        amlConf.chOutCmpCnt = 0;

        /* Fill the SDK specific structure. */
        status = TMR_AML_FillSdkConfig(&amlConf, sdkConf,&sdkPwmChPtr, NULL, NULL);
    }
    else
    {
        sdkConf = tmrSdkConfig;
    }

    /* Setup the timer. */
    if (status == kStatus_Success)
    {
        status = TMR_AML_Init(drvConfig->tmrInstance, sdkConf);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_GetDefaultConfig
 * Description   : This function gets a default configuration of the driver for
 *                 specific device and motor type.
 *
 *END**************************************************************************/
status_t LVHB_GetDefaultConfig(lvhb_drv_config_t* const defaultConfig, lvhb_device_t device, lvhb_motor_type_t motorType)
{
    AML_ASSERT(defaultConfig != NULL);

    /* Device with one H-Bridge can not control a stepper motor. */
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(device) || (motorType == lvhbMotorBrushed));

    defaultConfig->deviceConfig.activeMode = false;
    defaultConfig->deviceConfig.device = device;
    defaultConfig->deviceConfig.gateDriverOutputHigh = false;
    defaultConfig->deviceConfig.motorType = motorType;

    if (motorType == lvhbMotorBrushed)
    {
        /* Brushed motor default configuration */
        defaultConfig->deviceConfig.brushPwmFrequency = LVHB_BRUSH_PWM_FREQ_DEF;
        defaultConfig->deviceConfig.brushConfig[lvhbBridge1].outputDirection = LVHB_BRUSH_DIR_DEF;
        defaultConfig->deviceConfig.brushConfig[lvhbBridge1].recirculation = LVHB_BRUSH_REC_DEF;

        if (LVHB_HAS_TWO_BRIDGES(defaultConfig->deviceConfig.device))
        {
            defaultConfig->deviceConfig.secondaryBridgeUsed = true;

            defaultConfig->deviceConfig.brushConfig[lvhbBridge2].outputDirection = LVHB_BRUSH_DIR_DEF;
            defaultConfig->deviceConfig.brushConfig[lvhbBridge2].recirculation = LVHB_BRUSH_REC_DEF;
        }
        else
        {
            defaultConfig->deviceConfig.secondaryBridgeUsed = false;
        }
    }
    else
    {
        /* Stepper motor default configuration */
        defaultConfig->deviceConfig.stepperConfig.fullStepAcceleration = LVHB_FULLSTEP_ACCEL_DEF;
        defaultConfig->deviceConfig.stepperConfig.fullStepSpeed = LVHB_FULLSTEP_SPEED_DEF;
        defaultConfig->deviceConfig.stepperConfig.microStepAcceleration = LVHB_MICROSTEP_ACCEL_DEF;
        defaultConfig->deviceConfig.stepperConfig.microStepPerStep = LVHB_MICROSTEP_PER_FS_DEF;
        defaultConfig->deviceConfig.stepperConfig.microStepPwmFrequency = LVHB_MICROSTEP_PWM_FREQ_DEF;
        defaultConfig->deviceConfig.stepperConfig.microStepSpeed = LVHB_MICROSTEP_SPEED_DEF;
    }

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_Init
 * Description   : This function initializes the device.
 *
 *END**************************************************************************/
status_t LVHB_Init(lvhb_drv_config_t* const initConfig)
{
    AML_ASSERT(initConfig != NULL);

    /* Device with one H-Bridge can not control a stepper motor. */
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(initConfig->deviceConfig.device) || LVHB_IS_BRUSHED_MOTOR_SELECTED(initConfig));

    /* Stepper motor can be controlled by four GPIO pins or four timer outputs. */
    AML_ASSERT(!LVHB_IS_STEPPER_SELECTED(initConfig) || LVHB_HAS_GPIO_IN_PINS(initConfig) || LVHB_HAS_PWM_IN_PINS(initConfig));

    /* Only implemented devices are supported. */
    AML_ASSERT((initConfig->deviceConfig.device == lvhbDeviceMC34933) ||
               (initConfig->deviceConfig.device == lvhbDeviceMPC17510) ||
               (initConfig->deviceConfig.device == lvhbDeviceMPC17511) ||
               (initConfig->deviceConfig.device == lvhbDeviceMPC17529) ||
               (initConfig->deviceConfig.device == lvhbDeviceMPC17531A) ||
               (initConfig->deviceConfig.device == lvhbDeviceMPC17533) ||
               (initConfig->deviceConfig.device == lvhbDeviceMPC17C724));

    status_t status = kStatus_Success;
    lvhb_stepper_config_t *stepperConfig = &(initConfig->deviceConfig.stepperConfig);
    lvhb_stepper_data_t *stepperData = &(initConfig->deviceConfig.stepperData);

    /* Check stepper configuration. */
    if (LVHB_IS_STEPPER_SELECTED(initConfig))
    {
        /* Check full-step speed. */
        if ((stepperConfig->fullStepSpeed < LVHB_FULLSTEP_SPEED_MIN) || (stepperConfig->fullStepSpeed > LVHB_FULLSTEP_SPEED_MAX))
        {
            return kStatus_LVHB_InvalidDriverConfig;
        }

        /* Check full-step acceleration. */
        if (stepperConfig->fullStepAcceleration > LVHB_FULLSTEP_ACCEL_MAX)
        {
            return kStatus_LVHB_InvalidDriverConfig;
        }

        if (LVHB_HAS_PWM_IN_PINS(initConfig))
        {
            /* Check micro-step speed. */
            if ((stepperConfig->microStepSpeed < LVHB_MICROSTEP_SPEED_MIN) || (stepperConfig->microStepSpeed > LVHB_MICROSTEP_SPEED_MAX))
            {
                return kStatus_LVHB_InvalidDriverConfig;
            }

            /* Check micro-step acceleration. */
            if (stepperConfig->microStepAcceleration > LVHB_MICROSTEP_ACCEL_MAX)
            {
                return kStatus_LVHB_InvalidDriverConfig;
            }

            if ((stepperConfig->microStepPerStep != lvhbMicroStep2) &&
                (stepperConfig->microStepPerStep != lvhbMicroStep4) &&
                (stepperConfig->microStepPerStep != lvhbMicroStep8) &&
                (stepperConfig->microStepPerStep != lvhbMicroStep16) &&
                (stepperConfig->microStepPerStep != lvhbMicroStep32))
            {
                return kStatus_LVHB_InvalidDriverConfig;
            }

            /* Check PWM frequency for micro-stepping. */
            if ((stepperConfig->microStepPwmFrequency < LVHB_MICROSTEP_PWM_FREQ_MIN) || (stepperConfig->microStepPwmFrequency > LVHB_MICROSTEP_PWM_FREQ_MAX))
            {
                return kStatus_LVHB_InvalidDriverConfig;
            }
        }
    }

    /* Initialization of stepper data. */
    if (LVHB_IS_STEPPER_SELECTED(initConfig))
    {
        stepperData->forward = true;
        stepperData->continual = false;
        stepperData->stepPos = 0;
        stepperData->steps = 0;
        stepperData->stepCntr = 0;
        stepperData->fullStepIdx = 0;
        stepperData->stepperMode = lvhbStepperModeDisabled;
        stepperData->motorStatus = lvhbStatusStop;
        stepperData->timerOverflows = 0;
        stepperData->controlState = lvhbStepperStateSleepLoop;
        stepperData->rampState = lvhbRampOff;
        stepperData->fullSpeed = LVHB_GET_FULL_SPEED_TICKS(initConfig, stepperConfig->fullStepSpeed);

        if (LVHB_HAS_GPIO_IN_PINS(initConfig))
        {
            /* Only 1 full-step per timer period. */
            stepperData->fullStepsPerPeriod = 1;
        }
        else
        {
            stepperData->fullStepsPerPeriod = 0;
            stepperData->microSpeed = LVHB_GET_MICRO_SPEED_OF(initConfig, stepperConfig->microStepSpeed);
            stepperData->microWind1Idx = LVHB_WIND1_IDX_INIT;
            stepperData->microWind2Idx = LVHB_WIND2_IDX_INIT;
            stepperData->microPosInFull = 0;

            for (uint16_t i = 0; i < LVHB_MICROSTEP_CNT_MAX + 1; i++)
            {
                stepperData->microStepOffset[i] = (uint16_t)(LVHB_MICROSTEP_OFFSET_COEF[i] * LVHB_MICROSTEP_PERIOD_TICKS(initConfig));
            }
        }
    }

    /* Initialization of IN1A, IN1B, IN2A, IN2B GPIO/PWM pins. */

    LVHB_SetZeroOut(initConfig, lvhbBridge1);

    if (LVHB_IS_STEPPER_SELECTED(initConfig) ||
        (LVHB_HAS_TWO_BRIDGES(initConfig->deviceConfig.device) && initConfig->deviceConfig.secondaryBridgeUsed))
    {
        LVHB_SetZeroOut(initConfig, lvhbBridge2);
    }

    /* EN/OE/PSAVE pin */
    if (LVHB_HAS_EN_PIN(initConfig->deviceConfig.device))
    {
        if ((status = LVHB_SetMode(initConfig, initConfig->deviceConfig.activeMode)) != kStatus_Success)
        {
            return status;
        }
    }

    /* GIN pin */
    if (LVHB_HAS_GIN_PIN(initConfig->deviceConfig.device))
    {
        if ((status = LVHB_SetGateDriver(initConfig, initConfig->deviceConfig.gateDriverOutputHigh)) != kStatus_Success)
        {
            return status;
        }
    }

    /* Timer start. */
    if (LVHB_IS_STEPPER_SELECTED(initConfig) ||
        (initConfig->inputPins[lvhbBridge1] != lvhbPinsGpio) ||
        (LVHB_HAS_TWO_BRIDGES(initConfig->deviceConfig.device) && (initConfig->inputPins[lvhbBridge2] != lvhbPinsGpio)))
    {
        TMR_AML_StartTimer(initConfig->tmrInstance);
    }

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_Deinit
 * Description   : This function deinitializes the driver.
 *
 *END**************************************************************************/
status_t LVHB_Deinit(lvhb_drv_config_t* const initConfig)
{
    AML_ASSERT(initConfig != NULL);

    /* Timer stop. */
    if (LVHB_IS_STEPPER_SELECTED(initConfig) ||
        (initConfig->inputPins[lvhbBridge1] != lvhbPinsGpio) ||
        (LVHB_HAS_TWO_BRIDGES(initConfig->deviceConfig.device) && (initConfig->inputPins[lvhbBridge2] != lvhbPinsGpio)))
    {
        TMR_AML_StopTimer(initConfig->tmrInstance);
    }

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetMode
 * Description   : This function sets H-Bridge device mode using enable pin.
 *
 *END**************************************************************************/
status_t LVHB_SetMode(lvhb_drv_config_t* const drvConfig, bool active)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_HAS_EN_PIN(drvConfig->deviceConfig.device));

    GPIO_AML_WriteOutput(drvConfig->enPinInstance, drvConfig->enPinIndex,
            LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? active : !active);

    drvConfig->deviceConfig.activeMode = active;

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetGateDriver
 * Description   : This function controls Gate Driver Input (GIN) pin.
 *
 *END**************************************************************************/
status_t LVHB_SetGateDriver(lvhb_drv_config_t* const drvConfig, bool outputHigh)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_HAS_GIN_PIN(drvConfig->deviceConfig.device));

    /* Gate Driver Input. LOW = True control signal for GOUT pin - L(GIN) => H(GOUT). */
    GPIO_AML_WriteOutput(drvConfig->ginPinInstance, drvConfig->ginPinIndex, !outputHigh);

    drvConfig->deviceConfig.gateDriverOutputHigh = outputHigh;

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_RotateFull
 * Description   : This function spins the motor in desired direction at full speed.
 *
 *END**************************************************************************/
status_t LVHB_RotateFull(lvhb_drv_config_t* const drvConfig, lvhb_output_state_t state, lvhb_bridge_t bridge)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_BRUSHED_MOTOR_SELECTED(drvConfig));
    AML_ASSERT(LVHB_CHECK_BRIDGE(drvConfig, bridge));

    bool inxa, inxb;

    if (state > lvhbOutputOn)
    {
        return kStatus_LVHB_InvalidArgument;
    }

    if (drvConfig->inputPins[bridge] != lvhbPinsGpio)
    {
        return LVHB_RotateProportional(drvConfig, (state == lvhbOutputOn) ? 100 : 0, bridge);
    }

    if (state == lvhbOutputOff && drvConfig->deviceConfig.brushConfig[bridge].recirculation == lvhbRecirculationLow)
    {
        /* Stop motor low side recirculation. */
        inxa = LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? true : false;
        inxb = LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? true : false;
    }
    else if (state == lvhbOutputOff && drvConfig->deviceConfig.brushConfig[bridge].recirculation == lvhbRecirculationHighImp)
    {
        /* Stop motor high side recirculation. */
        inxa = LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? false : true;
        inxb = LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? false : true;
    }
    else if (drvConfig->deviceConfig.brushConfig[bridge].outputDirection == lvhbDirectionALowBHigh)
    {
        /* Run Motor OutALow to OutBHigh. */
        inxa = false;
        inxb = true;
    }
    else
    {
        /* Run Motor OutAHigh to OutBLow. */
        inxa = true;
        inxb = false;
    }

    GPIO_AML_WriteOutput(drvConfig->inxaPinInstance[bridge], drvConfig->inxaPinIndex[bridge], inxa);
    GPIO_AML_WriteOutput(drvConfig->inxbPinInstance[bridge], drvConfig->inxbPinIndex[bridge], inxb);

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_RotateProportional
 * Description   : This function spins the motor in desired direction at PWM duty speed.
 *
 *END**************************************************************************/
status_t LVHB_RotateProportional(lvhb_drv_config_t* const drvConfig, uint8_t pwmDuty, lvhb_bridge_t bridge)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_BRUSHED_MOTOR_SELECTED(drvConfig));
    AML_ASSERT(LVHB_CHECK_BRIDGE(drvConfig, bridge));

    /* LVHB_RotateProportional can not be used for state control. Use LVHB_RotateFull function. */
    AML_ASSERT(drvConfig->inputPins[bridge] != lvhbPinsGpio);

    status_t status = kStatus_Success;
    lvhb_input_pins_t inputPins = drvConfig->inputPins[bridge];
    lvhb_recirculation_t recirculation = drvConfig->deviceConfig.brushConfig[bridge].recirculation;
    lvhb_output_direction_t direction = drvConfig->deviceConfig.brushConfig[bridge].outputDirection;
    const uint16_t periodTicks = TMR_AML_GetPeriodTicks(drvConfig->tmrInstance);
    const uint16_t ticks = pwmDuty * periodTicks / 100;

    if (LVHB_IS_MPC1751x(drvConfig->deviceConfig.device))
    {
        /* Due to logic of MPC17510/MPC17511. */
        recirculation = (recirculation == lvhbRecirculationLow) ? lvhbRecirculationHighImp : lvhbRecirculationLow;
    }

    switch(inputPins)
    {
        case lvhbPinsPwm:
            if (direction == lvhbDirectionALowBHigh && recirculation == lvhbRecirculationLow)
            {
                /* Low side control. */
                TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                        drvConfig->tmrLvhbConfig.inxaChannelNumber[bridge], 0U);
                TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                        drvConfig->tmrLvhbConfig.inxbChannelNumber[bridge], ticks);
            }
            else if (direction == lvhbDirectionALowBHigh && recirculation == lvhbRecirculationHighImp)
            {
                /* High-Impedance side control. */
                TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                        drvConfig->tmrLvhbConfig.inxaChannelNumber[bridge], periodTicks - ticks);
                TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                        drvConfig->tmrLvhbConfig.inxbChannelNumber[bridge], periodTicks);
            }
            else if (direction == lvhbDirectionAHighBLow && recirculation == lvhbRecirculationLow)
            {
                /* Low side control. */
                TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                        drvConfig->tmrLvhbConfig.inxaChannelNumber[bridge], ticks);
                TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                        drvConfig->tmrLvhbConfig.inxbChannelNumber[bridge], 0U);
            }
            else if (direction == lvhbDirectionAHighBLow && recirculation == lvhbRecirculationHighImp)
            {
                /* High-Impedance side control. */
                TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                        drvConfig->tmrLvhbConfig.inxaChannelNumber[bridge], periodTicks);
                TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                        drvConfig->tmrLvhbConfig.inxbChannelNumber[bridge], periodTicks - ticks);
            }
            break;

        case lvhbPinsGpioPwm:
            if ((recirculation == lvhbRecirculationLow && pwmDuty == 0) ||
                (direction == lvhbDirectionALowBHigh && pwmDuty > 0))
            {
                /* Low side control. */
                GPIO_AML_ClearOutput(drvConfig->inxaPinInstance[bridge], drvConfig->inxaPinIndex[bridge]);
                TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                        drvConfig->tmrLvhbConfig.inxbChannelNumber[bridge], ticks);
            }
            else if ((recirculation == lvhbRecirculationHighImp && pwmDuty == 0) ||
                     (direction == lvhbDirectionAHighBLow && pwmDuty > 0))
            {
                /* High-Impedance side control. */
                GPIO_AML_SetOutput(drvConfig->inxaPinInstance[bridge], drvConfig->inxaPinIndex[bridge]);
                TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                        drvConfig->tmrLvhbConfig.inxbChannelNumber[bridge], periodTicks - ticks);
            }
            break;

        case lvhbPinsPwmGpio:
            if ((recirculation == lvhbRecirculationHighImp && pwmDuty == 0) ||
                    (direction == lvhbDirectionALowBHigh  && pwmDuty > 0))
            {
                /* High side control. */
                TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                        drvConfig->tmrLvhbConfig.inxaChannelNumber[bridge], periodTicks - ticks);
                GPIO_AML_SetOutput(drvConfig->inxbPinInstance[bridge], drvConfig->inxbPinIndex[bridge]);
            }
            else if ((recirculation == lvhbRecirculationLow && pwmDuty == 0) ||
                    (direction == lvhbDirectionAHighBLow  && pwmDuty > 0))
            {
                /* Low side control. */
                TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                        drvConfig->tmrLvhbConfig.inxaChannelNumber[bridge], ticks);
                GPIO_AML_ClearOutput(drvConfig->inxbPinInstance[bridge], drvConfig->inxbPinIndex[bridge]);
            }
            break;

        default:
            status = kStatus_LVHB_InvalidDriverConfig;
            break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetTriState
 * Description   : This function sets output of specified H-Bridge to tri-state
 *                 (high impedance) using input control pins.
 *
 *END**************************************************************************/
status_t LVHB_SetTriState(lvhb_drv_config_t* const drvConfig, lvhb_bridge_t bridge)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_BRUSHED_MOTOR_SELECTED(drvConfig));
    AML_ASSERT(LVHB_CHECK_BRIDGE(drvConfig, bridge));

    lvhb_input_pins_t inputPins = drvConfig->inputPins[bridge];
    uint16_t periodTicks = 0;

    if (inputPins != lvhbPinsGpio)
    {
        periodTicks = TMR_AML_GetPeriodTicks(drvConfig->tmrInstance);
    }

    /* INxA/IN1 pin. */
    if ((inputPins == lvhbPinsPwm) || (inputPins == lvhbPinsPwmGpio))
    {
        TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                drvConfig->tmrLvhbConfig.inxaChannelNumber[bridge],
                LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? 0U : periodTicks);
    }
    else
    {
        GPIO_AML_WriteOutput(drvConfig->inxaPinInstance[bridge],
                drvConfig->inxaPinIndex[bridge],
                LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? false : true);
    }

    /* INxB/IN2 pin. */
    if ((inputPins == lvhbPinsPwm) || (inputPins == lvhbPinsGpioPwm))
    {
        TMR_AML_SetOffsetTicks(drvConfig->tmrInstance,
                drvConfig->tmrLvhbConfig.inxbChannelNumber[bridge],
                LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? 0U : periodTicks);
    }
    else
    {
        GPIO_AML_WriteOutput(drvConfig->inxbPinInstance[bridge],
                drvConfig->inxbPinIndex[bridge],
                LVHB_IS_MPC1751x(drvConfig->deviceConfig.device) ? false : true);
    }

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetDirection
 * Description   : This function sets direction of brush motor at specified
 *                 H-Bridge interface.
 *
 *END**************************************************************************/
status_t LVHB_SetDirection(lvhb_drv_config_t* const drvConfig, lvhb_output_direction_t direction, lvhb_bridge_t bridge)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_BRUSHED_MOTOR_SELECTED(drvConfig));
    AML_ASSERT(LVHB_CHECK_BRIDGE(drvConfig, bridge));

    if (direction != lvhbDirectionAHighBLow && direction != lvhbDirectionALowBHigh )
    {
        return kStatus_LVHB_InvalidArgument;
    }

    drvConfig->deviceConfig.brushConfig[bridge].outputDirection = direction;

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetRecirculation
 * Description   : This function sets low/high-impedance-side recirculation
 *                 of the H-Bridge.
 *
 *END**************************************************************************/
status_t LVHB_SetRecirculation(lvhb_drv_config_t* const drvConfig, lvhb_recirculation_t side, lvhb_bridge_t bridge)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_BRUSHED_MOTOR_SELECTED(drvConfig));
    AML_ASSERT(LVHB_CHECK_BRIDGE(drvConfig, bridge));

    if (side > lvhbRecirculationHighImp)
    {
        return kStatus_LVHB_InvalidArgument;
    }

    drvConfig->deviceConfig.brushConfig[bridge].recirculation = side;

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetFullStepSpeed
 * Description   : This function sets the speed of full-step mode.
 *
 *END**************************************************************************/
status_t LVHB_SetFullStepSpeed(lvhb_drv_config_t* const drvConfig, uint16_t stepsSec)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    if (drvConfig->deviceConfig.stepperData.motorStatus == lvhbStatusRunning)
    {
        return kStatus_LVHB_DeviceBusy;
    }

    if ((stepsSec < LVHB_FULLSTEP_SPEED_MIN) || (stepsSec > LVHB_FULLSTEP_SPEED_MAX))
    {
        return kStatus_LVHB_InvalidArgument;
    }

    /* Compute period ticks. */
    uint32_t period = LVHB_GET_FULL_SPEED_TICKS(drvConfig, stepsSec);

    if (period <= 1)
    {
        return kStatus_LVHB_InvalidArgument;
    }

    drvConfig->deviceConfig.stepperConfig.fullStepSpeed = stepsSec;
    drvConfig->deviceConfig.stepperData.fullSpeed = period;

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetMicroStepSpeed
 * Description   : This function sets the speed of micro-step mode.
 *
 *END**************************************************************************/
status_t LVHB_SetMicroStepSpeed(lvhb_drv_config_t* const drvConfig, uint16_t microStepsSec)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_PWM_IN_PINS(drvConfig));
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    uint32_t tmrOfCnt;

    if (drvConfig->deviceConfig.stepperData.motorStatus == lvhbStatusRunning)
    {
        return kStatus_LVHB_DeviceBusy;
    }

    if ((microStepsSec < LVHB_MICROSTEP_SPEED_MIN) || (microStepsSec > LVHB_MICROSTEP_SPEED_MAX))
    {
        return kStatus_LVHB_InvalidArgument;
    }

    /* Compute number of counter overflows per micro-step. */
    tmrOfCnt = LVHB_GET_MICRO_SPEED_OF(drvConfig, microStepsSec);

    if (tmrOfCnt == 0)
    {
        /* Speed is too high. */
        return kStatus_LVHB_InvalidArgument;
    }

    drvConfig->deviceConfig.stepperConfig.microStepSpeed = microStepsSec;
    drvConfig->deviceConfig.stepperData.microSpeed = tmrOfCnt;

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetFullStepAcceleration
 * Description   : This function sets the acceleration ramp of full-step mode.
 *
 *END**************************************************************************/
status_t LVHB_SetFullStepAcceleration(lvhb_drv_config_t* const drvConfig, uint32_t acceleration)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    if (drvConfig->deviceConfig.stepperData.motorStatus == lvhbStatusRunning)
    {
        return kStatus_LVHB_DeviceBusy;
    }

    if (acceleration > LVHB_FULLSTEP_ACCEL_MAX)
    {
        return kStatus_LVHB_InvalidArgument;
    }

    drvConfig->deviceConfig.stepperConfig.fullStepAcceleration = acceleration;

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetMicroStepAcceleration
 * Description   : This function sets the speed of micro-step mode.
 *
 *END**************************************************************************/
status_t LVHB_SetMicroStepAcceleration(lvhb_drv_config_t* const drvConfig, uint32_t acceleration)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_PWM_IN_PINS(drvConfig));
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    if (drvConfig->deviceConfig.stepperData.motorStatus == lvhbStatusRunning)
    {
        return kStatus_LVHB_DeviceBusy;
    }

    if (acceleration > LVHB_MICROSTEP_ACCEL_MAX)
    {
        return kStatus_LVHB_InvalidArgument;
    }

    drvConfig->deviceConfig.stepperConfig.microStepAcceleration = acceleration;

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_MoveSteps
 * Description   : This function moves motor by specified number of full-steps.
 *
 *END**************************************************************************/
status_t LVHB_MoveSteps(lvhb_drv_config_t* const drvConfig, bool forward, uint32_t steps)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    if (drvConfig->deviceConfig.stepperData.motorStatus == lvhbStatusRunning)
    {
        return kStatus_LVHB_DeviceBusy;
    }

    if ((steps == 0) | (steps > LVHB_STEPPER_MAX_STEPS))
    {
        return kStatus_LVHB_InvalidArgument;
    }

    /* Frequency of the timer can not be out of range for this application. */
    if ((LVHB_FULLSTEP_IN_FREQ(drvConfig) < LVHB_FULLSTEP_IN_FREQ_MIN) | (LVHB_FULLSTEP_IN_FREQ(drvConfig) > LVHB_FULLSTEP_IN_FREQ_MAX))
    {
        return kStatus_LVHB_TimerFrequency;
    }

    drvConfig->deviceConfig.stepperData.continual = false;
    drvConfig->deviceConfig.stepperData.forward = forward;

    LVHB_StartFullstepping(drvConfig, forward, steps, drvConfig->deviceConfig.stepperData.fullSpeed);

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_MoveMicroSteps
 * Description   : This function moves motor by specified number of micro-steps.
 *
 *END**************************************************************************/
status_t LVHB_MoveMicroSteps(lvhb_drv_config_t* const drvConfig, bool forward, uint32_t microSteps)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_PWM_IN_PINS(drvConfig));
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    if (drvConfig->deviceConfig.stepperData.motorStatus == lvhbStatusRunning)
    {
        return kStatus_LVHB_DeviceBusy;
    }

    if ((microSteps == 0) | (microSteps > LVHB_STEPPER_MAX_STEPS))
    {
        return kStatus_LVHB_InvalidArgument;
    }

    /* Frequency of the timer can not be out of range for this application. */
    if ((LVHB_MICROSTEP_IN_FREQ(drvConfig) < LVHB_MICROSTEP_IN_FREQ_MIN) | (LVHB_MICROSTEP_IN_FREQ(drvConfig) > LVHB_MICROSTEP_IN_FREQ_MAX))
    {
        return kStatus_LVHB_TimerFrequency;
    }

    drvConfig->deviceConfig.stepperData.continual = false;

    LVHB_StartMicrostepping(drvConfig, forward, microSteps);

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_MoveContinual
 * Description   : This function moves motor continually in full-step mode.
 *
 *END**************************************************************************/
status_t LVHB_MoveContinual(lvhb_drv_config_t* const drvConfig, bool forward)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(drvConfig->deviceConfig.stepperConfig.fullStepAcceleration == 0);
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    /* Number of full-steps executed to next counter restart interrupt. */
    uint32_t initSteps;

    if (drvConfig->deviceConfig.stepperData.motorStatus == lvhbStatusRunning)
    {
        return kStatus_LVHB_DeviceBusy;
    }

    /* Frequency of the timer can not be out of range for this application. */
    if ((LVHB_FULLSTEP_IN_FREQ(drvConfig) < LVHB_FULLSTEP_IN_FREQ_MIN) | (LVHB_FULLSTEP_IN_FREQ(drvConfig) > LVHB_FULLSTEP_IN_FREQ_MAX))
    {
        return kStatus_LVHB_TimerFrequency;
    }

    drvConfig->deviceConfig.stepperData.continual = true;
    drvConfig->deviceConfig.stepperData.forward = forward;

    /* Set initial number of steps. */
    initSteps = LVHB_FULLSTEP_CNT_CONT;

    /* Number of steps to be executed is incremented in event handler. */
    LVHB_StartFullstepping(drvConfig, forward, initSteps, drvConfig->deviceConfig.stepperData.fullSpeed);

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_MoveMicroContinual
 * Description   : This function moves motor continually in micro-step mode.
 *
 *END**************************************************************************/
status_t LVHB_MoveMicroContinual(lvhb_drv_config_t* const drvConfig, bool forward)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_PWM_IN_PINS(drvConfig));
    AML_ASSERT(drvConfig->deviceConfig.stepperConfig.microStepAcceleration == 0);
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    if (drvConfig->deviceConfig.stepperData.motorStatus == lvhbStatusRunning)
    {
        return kStatus_LVHB_DeviceBusy;
    }

    /* Frequency of the timer can not be out of range for this application. */
    if ((LVHB_MICROSTEP_IN_FREQ(drvConfig) < LVHB_MICROSTEP_IN_FREQ_MIN) | (LVHB_MICROSTEP_IN_FREQ(drvConfig) > LVHB_MICROSTEP_IN_FREQ_MAX))
    {
        return kStatus_LVHB_TimerFrequency;
    }

    drvConfig->deviceConfig.stepperData.continual = true;

    /* Number of micro-steps is incremented in event handler. */
    LVHB_StartMicrostepping(drvConfig, forward, 2);

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_StopContinualMovement
 * Description   : This function is intended to stop continual movement of stepper motor.
 *
 *END**************************************************************************/
status_t LVHB_StopContinualMovement(lvhb_drv_config_t* const drvConfig)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));

    if ((drvConfig->deviceConfig.stepperData.motorStatus != lvhbStatusRunning) ||
        !drvConfig->deviceConfig.stepperData.continual)
    {
        return kStatus_LVHB_NotInContinualMovement;
    }

    drvConfig->deviceConfig.stepperData.continual = false;

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_GetMotorStatus
 * Description   : This function returns status of stepper motor control.
 *
 *END**************************************************************************/
lvhb_motor_status_t LVHB_GetMotorStatus(lvhb_drv_config_t* const drvConfig)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));

    return drvConfig->deviceConfig.stepperData.motorStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_AlignRotor
 * Description   : This function aligns rotor to the full-step position.
 *
 *END**************************************************************************/
status_t LVHB_AlignRotor(lvhb_drv_config_t* const drvConfig)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    if (drvConfig->deviceConfig.stepperData.motorStatus == lvhbStatusRunning)
    {
        return kStatus_LVHB_DeviceBusy;
    }

    /* Frequency of the timer can not be out of range for this application. */
    if ((LVHB_FULLSTEP_IN_FREQ(drvConfig) < LVHB_FULLSTEP_IN_FREQ_MIN) | (LVHB_FULLSTEP_IN_FREQ(drvConfig) > LVHB_FULLSTEP_IN_FREQ_MAX))
    {
        return kStatus_LVHB_TimerFrequency;
    }

    /* Initialize channels output to first full-step. */
    drvConfig->deviceConfig.stepperData.fullStepIdx = 0;

    /* Do not record the movement in counter of steps. */
    drvConfig->deviceConfig.stepperData.stepPos -= 4;

    LVHB_StartFullstepping(drvConfig, true, 4, LVHB_GET_FULL_SPEED_TICKS(drvConfig, LVHB_FULLSTEP_SPEED_MIN));

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_SetMicroStepSize
 * Description   : This function changes the size of micro-step.
 *
 *END**************************************************************************/
status_t LVHB_SetMicroStepSize(lvhb_drv_config_t* const drvConfig, lvhb_micro_step_t microStepsPerStep)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_PWM_IN_PINS(drvConfig));
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    if (drvConfig->deviceConfig.stepperData.motorStatus == lvhbStatusRunning)
    {
        return kStatus_LVHB_DeviceBusy;
    }

    if ((microStepsPerStep != lvhbMicroStep2) &&
        (microStepsPerStep != lvhbMicroStep4) &&
        (microStepsPerStep != lvhbMicroStep8) &&
        (microStepsPerStep != lvhbMicroStep16) &&
        (microStepsPerStep != lvhbMicroStep32))
    {
        return kStatus_LVHB_InvalidArgument;
    }

    drvConfig->deviceConfig.stepperConfig.microStepPerStep = microStepsPerStep;

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_GetFullStepPosition
 * Description   : This function returns the current full-step position.
 *
 *END**************************************************************************/
int32_t LVHB_GetFullStepPosition(lvhb_drv_config_t* const drvConfig)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    return drvConfig->deviceConfig.stepperData.stepPos;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_GetMicroStepPosition
 * Description   : This function returns the current micro-step position.
 *
 *END**************************************************************************/
int32_t LVHB_GetMicroStepPosition(lvhb_drv_config_t* const drvConfig)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_PWM_IN_PINS(drvConfig));
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    return (drvConfig->deviceConfig.stepperData.stepPos * LVHB_MICROSTEP_CNT_MAX
            + drvConfig->deviceConfig.stepperData.microPosInFull) / drvConfig->deviceConfig.stepperConfig.microStepPerStep;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_ResetFullStepPosition
 * Description   : This function sets the counter of full-steps to zero.
 *
 *END**************************************************************************/
status_t LVHB_ResetFullStepPosition(lvhb_drv_config_t* const drvConfig)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    if (drvConfig->deviceConfig.stepperData.motorStatus == lvhbStatusRunning)
    {
        return kStatus_LVHB_DeviceBusy;
    }

    drvConfig->deviceConfig.stepperData.stepPos = 0;
    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_DisableMotor
 * Description   : This function disables the stepper motor.
 *
 *END**************************************************************************/
status_t LVHB_DisableMotor(lvhb_drv_config_t* const drvConfig)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));
    AML_ASSERT(LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device));

    /* Disable interrupt. */
    TMR_AML_DisableInterrupts(drvConfig->tmrInstance, tmrIntTimeOverflow);

    /* Update Stepper configuration structure. */
    drvConfig->deviceConfig.stepperData.stepperMode = lvhbStepperModeDisabled;
    drvConfig->deviceConfig.stepperData.motorStatus = lvhbStatusStop;

    if (LVHB_HAS_GPIO_IN_PINS(drvConfig))
    {
        GPIO_AML_ClearOutput(drvConfig->inxaPinInstance[lvhbBridge1], drvConfig->inxaPinIndex[lvhbBridge1]);
        GPIO_AML_ClearOutput(drvConfig->inxbPinInstance[lvhbBridge1], drvConfig->inxbPinIndex[lvhbBridge1]);
        GPIO_AML_ClearOutput(drvConfig->inxaPinInstance[lvhbBridge2], drvConfig->inxaPinIndex[lvhbBridge2]);
        GPIO_AML_ClearOutput(drvConfig->inxbPinInstance[lvhbBridge2], drvConfig->inxbPinIndex[lvhbBridge2]);
    }
    else
    {
        TMR_AML_SetOffsetTicks(drvConfig->tmrInstance, drvConfig->tmrLvhbConfig.inxaChannelNumber[lvhbBridge1], 0);
        TMR_AML_SetOffsetTicks(drvConfig->tmrInstance, drvConfig->tmrLvhbConfig.inxbChannelNumber[lvhbBridge1], 0);
        TMR_AML_SetOffsetTicks(drvConfig->tmrInstance, drvConfig->tmrLvhbConfig.inxaChannelNumber[lvhbBridge2], 0);
        TMR_AML_SetOffsetTicks(drvConfig->tmrInstance, drvConfig->tmrLvhbConfig.inxbChannelNumber[lvhbBridge2], 0);

        /* Clear Timer counter register. */
        TMR_AML_ResetTimer(drvConfig->tmrInstance);
    }

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LVHB_OnCounterRestart
 * Description   : Counter restart event handler.
 *
 *END**************************************************************************/
void LVHB_OnCounterRestart(lvhb_drv_config_t* const drvConfig)
{
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(LVHB_IS_STEPPER_SELECTED(drvConfig));

    uint32_t stepDelay; /* Number of timer period ticks. */
    lvhb_stepper_config_t *stepperConfig = &(drvConfig->deviceConfig.stepperConfig);
    lvhb_stepper_data_t *stepperData = &(drvConfig->deviceConfig.stepperData);

    if (LVHB_HAS_PWM_IN_PINS(drvConfig))
    {
        /* Stepper motor - PWM channels. */
        if ((stepperData->controlState == lvhbStepperStateInitCompleted) && (stepperData->stepperMode != lvhbStepperModeDisabled))
        {
            if (stepperData->timerOverflows <= 1)
            {
                if (stepperData->stepperMode == lvhbStepperModeMicrostep)
                {
                    /* Micro-stepping - go to next micro-step, update counters, etc. */
                    LVHB_GotoNextMicro(drvConfig);
                }
                if (stepperData->stepperMode == lvhbStepperModeFullstep)
                {
                    /* Full-stepping - go to next full-step, update counters, etc. */
                    LVHB_GotoNextFull(drvConfig);
                }
            }
            else {
                /* Update number of remaining timer overflows. */
                stepperData->timerOverflows--;
            }
        }
    }
    else
    {
        /* Stepper motor - GPIO pins. */
        if ((stepperData->controlState == lvhbStepperStateInitCompleted) && (stepperData->stepperMode == lvhbStepperModeFullstep))
        {
            if (stepperData->timerOverflows <= 1)
            {
                if (stepperData->stepCntr < stepperData->steps)
                {
                    /* Desired full-step count not reached. */
                    if (stepperData->controlState != lvhbStepperStateSlowStart)
                    {
                        LVHB_UpdateFullCntr(drvConfig);

                        /* Change output signal values. */
                        LVHB_SetFullGpio(drvConfig);

                        if (stepperData->continual)
                        {
                            /* Decrease number of steps in continual movement. */
                            stepperData->steps++;
                        }
                    }
                    else
                    {
                        /* Slow start means that channels output is not changed in the first step. */
                        stepperData->controlState = lvhbStepperStateInitCompleted;
                    }

                    if (stepperConfig->fullStepAcceleration == 0)
                    {
                        /* Set timerOverflows for next full-step. */
                        stepperData->timerOverflows = stepperData->currentFullSpeed / LVHB_TIMER_VALUE_MAX(drvConfig) + 1;
                    }
                    else
                    {
                        /* Set value of next full-step (timer period). */
                        stepDelay = LVHB_GetNextRampDelay(drvConfig, stepperData->stepCntr + 1);
                        stepperData->timerOverflows = stepDelay / LVHB_TIMER_VALUE_MAX(drvConfig) + 1;
                        stepDelay = stepDelay / stepperData->timerOverflows;
                        TMR_AML_SetPeriodTicks(drvConfig->tmrInstance, stepDelay);
                    }
                }
                else
                {
                    /* Desired full-step count reached -> stop motor. */
                    TMR_AML_DisableInterrupts(drvConfig->tmrInstance, tmrIntTimeOverflow);
                    stepperData->motorStatus = lvhbStatusStop;
                    LVHB_OnActionComplete(drvConfig);
                }
            }
            else
            {
                /* More timer overflows must be executed before changing the full-step. */
                stepperData->timerOverflows--;
                return;
            }
        }
    }
}
