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
* Function contains endless loop with several test cases
* intended to control stepper motor.
*
* Return FALSE if an error occurred.
*/
static bool StartApplication(lvhb_drv_config_t* const drvConfig)
{
    /* Initial speed in full-step mode. Micro-stepping speed is derived from
    * full-stepping speed. */
    const uint16_t INIT_SPEED = 200;
    const uint16_t MAX_SPEED = 500;   /* Maximal speed. */
    uint16_t speed = INIT_SPEED;      /* Current motor speed. */

    /* Align motor to full-step position. */
    if (LVHB_AlignRotor(drvConfig) != kStatus_Success)
    {
        return false;
    }

    WaitForCompletion(drvConfig);

    while (true)
    {
        /* Set full-step speed. */
        if (LVHB_SetFullStepSpeed(drvConfig, speed) != kStatus_Success)
        {
            return false;
        }

        /* Move 800 full-steps in forward. */
        if (LVHB_MoveSteps(drvConfig, true, 800) != kStatus_Success)
        {
            return false;
        }

        /* Wait while motor is running. */
        WaitForCompletion(drvConfig);

        /* Set micro-step size to 8. */
        if (LVHB_SetMicroStepSize(drvConfig, lvhbMicroStep8) != kStatus_Success)
        {
            return false;
        }

        if (LVHB_SetMicroStepSpeed(drvConfig, speed * 8) != kStatus_Success)
        {
            return false;
        }

        /* Move motor by 3200 micro-steps (i.e. 800 full-steps) in reverse direction. */
        if (LVHB_MoveMicroSteps(drvConfig, false, 800 * 8) != kStatus_Success)
        {
            return false;
        }

        /* Wait while motor is running. */
        WaitForCompletion(drvConfig);

        /* Set micro-step size to 2. */
        if (LVHB_SetMicroStepSize(drvConfig, lvhbMicroStep2) != kStatus_Success)
        {
            return false;
        }

        if (LVHB_SetMicroStepSpeed(drvConfig, speed * 2) != kStatus_Success)
        {
            return false;
        }

        /* Move motor by 800 micro-steps (i.e. 800 full-steps) in reverse direction. */
        if (LVHB_MoveMicroSteps(drvConfig, false, 800 * 2) != kStatus_Success)
        {
            return false;
        }

        /* Wait while motor is running. */
        WaitForCompletion(drvConfig);

        /* Increase speed. */
        speed += 100;
        if (speed > MAX_SPEED)
        {
            speed = INIT_SPEED;
        }

        /* Wait for a while and hold current motor position. */
        WAIT_AML_WaitSec(1);
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

    drvConfig.deviceConfig.stepperConfig.fullStepSpeed = 200;
    drvConfig.deviceConfig.stepperConfig.fullStepAcceleration = 200;
    drvConfig.deviceConfig.stepperConfig.microStepPerStep = lvhbMicroStep8;
    drvConfig.deviceConfig.stepperConfig.microStepPwmFrequency = 20000;
    drvConfig.deviceConfig.stepperConfig.microStepSpeed = 400;
    drvConfig.deviceConfig.stepperConfig.microStepAcceleration = 400;

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
