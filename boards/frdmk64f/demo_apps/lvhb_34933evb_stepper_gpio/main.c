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
#include "fsl_debug_console.h"

#include "wait_aml/wait_aml.h"
#include "lvhb.h"

lvhb_drv_config_t drvConfig; /* LVHB driver configuration structure. */

/* Timer interrupt handler. It clears an interrupt flag
 * and call on counter restart handler from LVHB driver.
 */
void FTM0_IRQHandler(void)
{
    uint16_t status;
    uint16_t enIntMask;

    status = TMR_AML_GetStatusFlags(drvConfig.tmrInstance);
    enIntMask = TMR_AML_GetEnabledInterrupts(drvConfig.tmrInstance);

    if (((tmrStsTimeOverflow & status) != 0U) &&
            ((tmrIntTimeOverflow & enIntMask) != 0U))
    {
        TMR_AML_ClearStatusFlags(drvConfig.tmrInstance, tmrStsTimeOverflow);

        /* Invoke OnCounterRestart LVHB event */
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
 * It demonstrates how to control motor in full-step mode
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

    if (LVHB_SetFullStepAcceleration(drvConfig, 0) != kStatus_Success)
    {
        return false;
    }

    /* Run motor in full-step mode (180 degrees). */
    PRINTF("\tExecute 100 full-steps in %s direction with speed of 50 full-steps/s.\r\n", forwardDir ? "forward" : "reverse");
    if (LVHB_MoveSteps(drvConfig, forwardDir, 100) != kStatus_Success)
    {
        return false;
    }

    WaitForCompletion(drvConfig);

    WAIT_AML_WaitSec(1);

    return true;
}

/*
 * This function demonstrates how to control motor using continual
 * mode, how to get full-step position, hold motor position
 * (coils are powered) and disable motor (position is not hold).
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
        PRINTF("\tExecute 100 full-steps in %s direction with speed of %d full-steps/s.\r\n", forward ? "forward" : "reverse", speed);
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

/**
 * There are several functions, which are described below.
 * Rotor align is performed after initialization. This function
 * executes 4 full step to set rotor to a full-step position.
 * Then BasicDemo and AdvancedDemo functions are called in a loop.
 *
 * Returns FALSE if an error occurred.
 */
static bool StartApplication(lvhb_drv_config_t* const drvConfig)
{
    bool forwardDir = true;     /* Motor direction. */

    /* Align motor to full-step position. */
    PRINTF("Aligning the rotor...\r\n");
    if (LVHB_AlignRotor(drvConfig) != kStatus_Success)
    {
        return false;
    }

    WaitForCompletion(drvConfig);

    while (true)
    {
        /* Basic stepper motor control. */
        PRINTF("\r\nBasic demo:\r\n");
        if (!BasicDemo(drvConfig, forwardDir))
        {
            return false;
        }

        /* Advanced motor control demo, which uses continual mode. */
        PRINTF("\r\nAdvanced demo:\r\n");
        if (!AdvancedDemo(drvConfig))
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
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();

    /* Fill LVHB driver config. */
    LVHB_GetDefaultConfig(&drvConfig, lvhbDeviceMC34933, lvhbMotorStepper);

    /* Timer settings */
    drvConfig.tmrInstance = 0;                                  /* FTM0 */
    drvConfig.tmrLvhbConfig.counterWidth = 16;
    drvConfig.tmrLvhbConfig.prescale = tmrPrescDiv_8;
    drvConfig.tmrLvhbConfig.srcClck_Hz = CLOCK_GetFreq(kCLOCK_BusClk);

    /* Pins */
    drvConfig.inputPins[lvhbBridge1] = lvhbPinsGpio;            /* IN1A (GPIO) + IN1B (GPIO) */
    drvConfig.inputPins[lvhbBridge2] = lvhbPinsGpio;            /* IN2A (GPIO) + IN2B (GPIO) */
    drvConfig.inxaPinInstance[lvhbBridge1] = instanceA;         /* IN1A - PTA2/FTM0_CH7 */
    drvConfig.inxaPinIndex[lvhbBridge1] = 2U;
    drvConfig.inxbPinInstance[lvhbBridge1] = instanceC;         /* IN1B - PTC2/FTM0_CH1 */
    drvConfig.inxbPinIndex[lvhbBridge1] = 2U;
    drvConfig.inxaPinInstance[lvhbBridge2] = instanceC;         /* IN2A - PTC12 */
    drvConfig.inxaPinIndex[lvhbBridge2] = 12U;
    drvConfig.inxbPinInstance[lvhbBridge2] = instanceC;         /* IN2B - PTC4/FTM0_CH3 */
    drvConfig.inxbPinIndex[lvhbBridge2] = 4U;

    if ((LVHB_ConfigureGpio(&drvConfig) != kStatus_Success) ||
        (LVHB_ConfigureTimer(&drvConfig, NULL) != kStatus_Success) ||
        (LVHB_Init(&drvConfig) != kStatus_Success))
    {
        PRINTF("An error occurred during initialization.\r\n");
    }
    else
    {
        /* Enable interrupts for FTM0. */
        EnableIRQ(FTM0_IRQn);

        if (!StartApplication(&drvConfig))
        {
            PRINTF("\tAn error occurred\r\n");
        }
    }

    for(;;)  /* Infinite loop to avoid leaving the main function */
    {
        __asm("NOP");
    }
}
