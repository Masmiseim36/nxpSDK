/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "gpio_aml.h"
#include "wait_aml/wait_aml.h"
#include "lvhb.h"

lvhb_drv_config_t drvConfig; /* LVHB driver configuration structure. */

/* Timer interrupt handler. It clears an interrupt flag
 * and call on counter restart handler from LVHB driver.
 */
void TPM0_IRQHandler(void)
{
    uint16_t status;
    uint16_t enIntMask;

    status = TMR_AML_GetStatusFlags(drvConfig.tmrInstance);
    enIntMask = TMR_AML_GetEnabledInterrupts(drvConfig.tmrInstance);

    if (((tmrStsTimeOverflow & status) != 0U) &&
            ((tmrIntTimeOverflow & enIntMask) != 0U))
    {
        TMR_AML_ClearStatusFlags(drvConfig.tmrInstance, tmrStsTimeOverflow);

        /* Invoke OnCounterRestart MVHB event */
        LVHB_OnCounterRestart(&drvConfig);
    }
}

/*
 * On LVHB action complete event handler.
 */
void LVHB_OnActionComplete(lvhb_drv_config_t* const drvConfig)
{
    AML_UNUSED(drvConfig);
}

/*
 * Wait for completion of motor movement.
 */
static void WaitForCompletion(lvhb_drv_config_t* const drvConfig)
{
    while (LVHB_GetMotorStatus(drvConfig) == lvhbStatusRunning);
}

/*
 * Quick demo demonstrates how to use driver in a common way.
 * It demonstrates how to control motor in full-step and micro-step mode
 * and how to change motor speed.
 *
 * Returns FALSE if an error occurred, otherwise TRUE is returned.
 */
static bool BasicDemo(lvhb_drv_config_t* const drvConfig, bool forwardDir)
{
    /* Set the full-stepping speed. */
    if (LVHB_SetFullStepSpeed(drvConfig, 50) != kStatus_Success)
    {
        return false;
    }

    /* Set the full-stepping speed. */
    if (LVHB_SetMicroStepSpeed(drvConfig, 100) != kStatus_Success)
    {
        return false;
    }

    /* Set size of micro-step to 2 micro-steps per one full step. */
    if (LVHB_SetMicroStepSize(drvConfig, lvhbMicroStep2))
    {
        return false;
    }

    /* Run motor in full-step mode (180 degrees). */
    if (LVHB_MoveSteps(drvConfig, forwardDir, 100) != kStatus_Success)
    {
        return false;
    }

    WaitForCompletion(drvConfig);

    /* Move motor in micro-step mode back to zero position (180 degrees). */
    if (LVHB_MoveMicroSteps(drvConfig, !forwardDir, 100 * 2) != kStatus_Success)
    {
        return false;
    }

    WaitForCompletion(drvConfig);

    WAIT_AML_WaitSec(1);

    return true;
}

/*
 * This function demonstrates how to control motor using continual
 * mode, how to get full-step and micro-step position, hold motor
 * position (coils are powered) and disable motor (position is not hold).
 *
 * Returns FALSE if an error occurred, otherwise TRUE is returned.
 */
static bool AdvancedDemo(lvhb_drv_config_t* const drvConfig)
{
    bool forward = true;  /* Motor direction. */

    if (LVHB_ResetFullStepPosition(drvConfig) != kStatus_Success)
    {
        return false;
    }

    /* Run motor in full-step mode using various speed. */
    for (uint16_t speed = 25; speed <= 100; speed += 25)
    {
        /* Set new full-step speed. Note that maximum speed also depends on used
         * stepper motor. */
        if (LVHB_SetFullStepSpeed(drvConfig, speed) != kStatus_Success)
        {
            continue;
        }

        /* Start continual movement. */
        if (LVHB_MoveContinual(drvConfig, forward) != kStatus_Success)
        {
            return false;
        }

        if (forward)
        {
            /* Wait until motor executes at least 100 full-steps in forward direction. */
            while (LVHB_GetFullStepPosition(drvConfig) < 100);
        }
        else
        {
            /* Wait until motor executes at least 100 full-steps in reversed direction. */
            while (LVHB_GetFullStepPosition(drvConfig) > 0);
        }

        /* Stop continual movement. */
        if (LVHB_StopContinualMovement(drvConfig) != kStatus_Success)
        {
            return false;
        }

        /* Wait until motor stops. */
        WaitForCompletion(drvConfig);

        /* Change direction. */
        forward = !forward;
    }

    forward = true;
    if (LVHB_ResetFullStepPosition(drvConfig) != kStatus_Success)
    {
        return false;
    }

    if (LVHB_SetMicroStepSize(drvConfig, lvhbMicroStep2) != kStatus_Success)
    {
        return false;
    }

    /* Run motor in micro-step mode using various speed. */
    for (uint16_t speed = 200; speed >= 50; speed -= 50)
    {
        /* Set new micro-step speed. */
        if (LVHB_SetMicroStepSpeed(drvConfig, speed) != kStatus_Success)
        {
            continue;
        }

        /* Start continual movement. */
        if (LVHB_MoveMicroContinual(drvConfig, forward) != kStatus_Success)
        {
            return false;
        }

        if (forward)
        {
            /* Wait until motor executes at least 200 micro-steps (i.e. 100
             * full-steps) in forward direction. */
            while (LVHB_GetMicroStepPosition(drvConfig) < 200);
        }
        else
        {
            /* Wait until motor executes at least 200 micro-steps (i.e. 100
             * full-steps) in reversed direction. */
            while (LVHB_GetMicroStepPosition(drvConfig) > 0);
        }

        /* Stop continual movement. */
        if (LVHB_StopContinualMovement(drvConfig) != kStatus_Success)
        {
            return false;
        }

        /* Wait until motor stops. */
        WaitForCompletion(drvConfig);

        /* Change direction. */
        forward = !forward;
    }

    /* Reset full-step position. */
    if (LVHB_ResetFullStepPosition(drvConfig) != kStatus_Success)
    {
        return false;
    }

    /* Set H-Bridge outputs to LOW.*/
    if (LVHB_DisableMotor(drvConfig) != kStatus_Success)
    {
        return false;
    }

    /* Wait for a while to see change of H-Bridge outputs. */
    WAIT_AML_WaitSec(1);

    /* No error. */
    return true;
}

/*
 * This function demonstrates how to change size of micro-step.
 * Possible values of micro-step size are: 2, 4, 8, 16 and 32 micro-steps per
 * one full-step.
 *
 * Returns FALSE if an error occurred, otherwise TRUE is returned.
 */
static bool SetMicroStepSizeDemo(lvhb_drv_config_t* const drvConfig)
{
    for (uint16_t msSize = 2; msSize <= 32; msSize <<= 1)
    {
        /* Set micro-stepping speed. */
        if (LVHB_SetMicroStepSpeed(drvConfig, 50 * msSize) != kStatus_Success)
        {
            return false;
        }

        /* Set size of micro-step. */
        if (LVHB_SetMicroStepSize(drvConfig, (lvhb_micro_step_t)(LVHB_MICROSTEP_CNT_MAX / msSize)) != kStatus_Success)
        {
            return false;
        }

        /* Move motor by 180 degrees in forward direction using micro-step mode. */
        if (LVHB_MoveMicroSteps(drvConfig, true, 100 * msSize) != kStatus_Success)
        {
            return false;
        }

        /* Wait until motor stops. */
        WaitForCompletion(drvConfig);

        /* Wait for a while. */
        WAIT_AML_WaitMs(200);

        /* Move motor by 180 degrees in reversed direction using micro-step mode.*/
        if (LVHB_MoveMicroSteps(drvConfig, false, 100 * msSize) != kStatus_Success)
        {
            return false;
        }

        /* Wait until motor stops. */
        WaitForCompletion(drvConfig);

        /* Wait for a while. */
        WAIT_AML_WaitMs(200);
    }

    /* No error. */
    return true;
}

/**
 * There are several functions, which are described below.
 * Rotor align is performed after initialization. This function
 * executes 4 full step to set rotor to a full-step position.
 * Then BasicDemo, AdvancedDemo and SetMicroStepSizeDemo functions
 * are called in a loop.
 *
 * Returns FALSE if an error occurred.
 */
static bool StartApplication(lvhb_drv_config_t* const drvConfig)
{
    bool forwardDir = true;     /* Motor direction. */

    /* Align motor to full-step position. */
    if (LVHB_AlignRotor(drvConfig) != kStatus_Success)
    {
        return false;
    }

    WaitForCompletion(drvConfig);

    while (true)
    {
        /* Basic stepper motor control. */
        if (!BasicDemo(drvConfig, forwardDir))
        {
            return false;
        }

        /* Advanced motor control demo, which uses continual mode. */
        if (!AdvancedDemo(drvConfig))
        {
            return false;
        }

        /* Changing micro-step size. */
        if (!SetMicroStepSizeDemo(drvConfig))
        {
            return false;
        }

        /* Reverse direction. */
        forwardDir = !forwardDir;
    }
}

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    /* Fill LVHB driver config. */
    /* Use different LV H-Bridge device in case of other LV H-Bridge board
       supported by this example. */
    LVHB_GetDefaultConfig(&drvConfig, lvhbDeviceMC34933, lvhbMotorStepper);

    /* Timer settings */
    drvConfig.tmrInstance = 0;                                  /* TPM0 */
    drvConfig.tmrLvhbConfig.counterWidth = 16;
    drvConfig.tmrLvhbConfig.prescale = tmrPrescDiv_8;
    drvConfig.tmrLvhbConfig.srcClck_Hz = CLOCK_GetFreq(kCLOCK_McgIrc48MClk);

    /* Pins */
    drvConfig.inputPins[lvhbBridge1] = lvhbPinsPwm;             /* IN1A (PWM) + IN1B (PWM) */
    drvConfig.inputPins[lvhbBridge2] = lvhbPinsPwm;             /* IN2A (PWM) + IN2B (PWM) */
    drvConfig.tmrLvhbConfig.inxaChannelNumber[lvhbBridge1] = 0; /* IN1A - PTE24/TPM0_CH0 */
    drvConfig.tmrLvhbConfig.inxbChannelNumber[lvhbBridge1] = 5; /* IN1B - PTC9/TPM0_CH5 */
    drvConfig.tmrLvhbConfig.inxaChannelNumber[lvhbBridge2] = 4; /* IN2A - PTE31/TPM0_CH4 */
    drvConfig.tmrLvhbConfig.inxbChannelNumber[lvhbBridge2] = 2; /* IN2B - PTA5/TPM0_CH2 */

    /* EN pin settings needs to be configured for all supported LV H-Bridge boards 
       except FRDM-34933EVB. */
    drvConfig.enPinInstance = instanceA;                        /* EN - PTA2 */
    drvConfig.enPinIndex = 2;

    /* Turn off RED LED. */
    GPIO_AML_SetDirection(instanceB, BOARD_LED_RED_GPIO_PIN, gpioDirDigitalOutput);
    GPIO_AML_SetOutput(instanceB, BOARD_LED_RED_GPIO_PIN);

    CLOCK_SetTpmClock(kTPM_SystemClock);

    /* Initialize GPIO pins, timer and LVHB driver. */ 
    if ((LVHB_ConfigureGpio(&drvConfig) != kStatus_Success) ||
        (LVHB_ConfigureTimer(&drvConfig, NULL) != kStatus_Success) ||
        (LVHB_Init(&drvConfig) != kStatus_Success))
    {
        /* An error occurred during initialization. */
        GPIO_AML_ClearOutput(instanceB, BOARD_LED_RED_GPIO_PIN);
    }
    else
    {
        /* Enable interrupts for TPM0. */
        EnableIRQ(TPM0_IRQn);

        /* LV H-Bridge needs to be set active in case of other LV H-Bridge board
           supported by this example. */
        /* LVHB_SetMode(&drvConfig, true); */

        /* Run the demo application. */
        if (!StartApplication(&drvConfig))
        {
            /* An error occurred during execution. */
            GPIO_AML_ClearOutput(instanceB, BOARD_LED_RED_GPIO_PIN);

            /* LV H-Bridge should be set inactive in case of other LV H-Bridge board
               supported by this example. */
            /* LVHB_SetMode(&drvConfig, false); */
        }
    }

    for(;;)  /* Infinite loop to avoid leaving the main function */
    {
        __asm("NOP");
    }
}
