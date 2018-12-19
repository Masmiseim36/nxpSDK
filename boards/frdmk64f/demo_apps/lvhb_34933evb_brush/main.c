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

/* On LVHB action complete event handler. */
void LVHB_OnActionComplete(lvhb_drv_config_t* const drvConfig)
{
    AML_UNUSED(drvConfig);
}

/**
 * This function uses LVHB_RotateProportional method of LVHB driver.
 * Duty of H-Bridge interface 1 is increasing to reach 100%. Then duty is decreased to 0%.
 *
 * The function returns FALSE when an error occurred. Otherwise TRUE is returned.
 */
static bool RotateProportional(lvhb_drv_config_t* const drvConfig)
{
    status_t error;

    /* Change regularly PWM duty to reach 100% value. */
    for (uint8_t pwmDuty = 0; pwmDuty <= 100; pwmDuty++)
    {
        if ((error = LVHB_RotateProportional(drvConfig, pwmDuty, lvhbBridge1)) != kStatus_Success)
        {
            PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
            return false;
        }

        /* Wait. */
        WAIT_AML_WaitMs(100);
    }

    /* Change regularly PWM duty to decrease its value to zero. */
    for (int8_t pwmDuty = 100; pwmDuty >= 0; pwmDuty--)
    {
        if ((error = LVHB_RotateProportional(drvConfig, (uint8_t)pwmDuty, lvhbBridge1)) != kStatus_Success)
        {
            PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
            return false;
        }

        /* Wait. */
        WAIT_AML_WaitMs(100);
    }

    PRINTF("\tResult: OK\r\n");

    return true;
}

/**
 * This function uses LVHB_RotateFull method of LVHBridge driver.
 * It runs motor connected to H-Bridge interface 2. Motor stops. Then it runs
 * at the highest speed. Finally, it stops again.
 *
 * The function returns FALSE when an error occurred. Otherwise TRUE is returned.
 */
static bool RotateFull(lvhb_drv_config_t* const drvConfig)
{
    status_t error;

    /* Stop motor. */
    error = LVHB_RotateFull(drvConfig, lvhbOutputOff, lvhbBridge2);
    if (error != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(2);

    /* Run motor. */
    error = LVHB_RotateFull(drvConfig, lvhbOutputOn, lvhbBridge2);
    if (error != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(2);

    /* Stop motor. */
    error = LVHB_RotateFull(drvConfig, lvhbOutputOff, lvhbBridge2);
    if (error != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(2);

    PRINTF("\tResult: OK\r\n");
    return true;
}

/**
 * This function sets output of specified H-Bridge interface.
 * H-Bridge interface 1 allows to set speed of motor (method
 * RotateProportional). H-Bridge interface 2 enables to set
 * only on/off states (method RotateFull).
 *
 * Parameter drvConfig - Pointer to driver instance configuration.
 * Parameter rotate    - TRUE for rotation, FALSE for stop.
 * Parameter bridge    - Selection of H-Bridge interface.
 *
 * The function returns FALSE when an error occurred. Otherwise TRUE is returned.
 */
static bool Rotate(lvhb_drv_config_t* const drvConfig, bool rotate, lvhb_bridge_t bridge)
{
    status_t error;

    if (bridge == lvhbBridge1)
    {
        /* Set output of H-Bridge interface 1. */
        error = LVHB_RotateProportional(drvConfig, rotate ? 100 : 0, lvhbBridge1);
    }
    else
    {
        /* Set output of H-Bridge interface 2. */
        error = LVHB_RotateFull(drvConfig, (lvhb_output_state_t) rotate, lvhbBridge2);
    }

    if (error != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    return true;
}

/**
 * This function uses LVHB_RotateProportional and LVHB_SetTriState methods
 * of LVHB driver. It runs motor in forward direction using 100% PWM duty.
 * Then LVHB_SetTriState method is called. Motor should slowly decrease its
 * speed to zero. Brake of motor (LVHB_RotateProportional with 0% duty) follows.
 *
 * The function returns FALSE when an error occurred. Otherwise TRUE is returned.
 */
static bool SetTriState(lvhb_drv_config_t* const drvConfig, lvhb_bridge_t bridge)
{
    status_t error;

    PRINTF("\tRun motor\r\n");
    if (!Rotate(drvConfig, true, bridge))
    {
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(2);

    PRINTF("\tSet output to tri-state (freewheel)\r\n");
    if ((error = LVHB_SetTriState(drvConfig, bridge)) != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(4);

    PRINTF("\tRun motor\r\n");
    if (!Rotate(drvConfig, true, bridge))
    {
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(2);

    PRINTF("\tSet output to LOW (brake)\r\n");
    if (!Rotate(drvConfig, false, bridge))
    {
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(4);

    PRINTF("\tResult: OK\r\n");
    return true;
}

/**
 * This function contains several test cases to show usage of the LVHBridge driver
 * methods.
 *
 * The function returns FALSE when an error occurred.
 */
static bool StartApplication(lvhb_drv_config_t* const drvConfig)
{

    PRINTF("\r\n1. Test method SetTriState using H-Bridge interface 1\r\n");
    if (!SetTriState(drvConfig, lvhbBridge1))
    {
        return false;
    }


    PRINTF("\r\n2. Test method SetTriState using H-Bridge interface 2\r\n");
    if (!SetTriState(drvConfig, lvhbBridge2))
    {
        return false;
    }

    PRINTF("\r\n3. Test method RotateProportional, RotateFull and SetDirection\r\n");
    while (true)
    {

        /* H-Bridge interface 1. */
        if (LVHB_SetDirection(drvConfig, lvhbDirectionAHighBLow, lvhbBridge1) != kStatus_Success)
        {
            return false;
        }

        PRINTF("\tUse method RotateProportional in forward direction to control interface 1\r\n");
        if (!RotateProportional(drvConfig))
        {
            return false;
        }

        if (LVHB_SetDirection(drvConfig, lvhbDirectionALowBHigh, lvhbBridge1) != kStatus_Success)
        {
            return false;
        }

        PRINTF("\tUse method RotateProportional in reverse direction to control interface 1\r\n");
        if (!RotateProportional(drvConfig))
        {
            return false;
        }

        /* H-Bridge interface 2. */
        if (LVHB_SetDirection(drvConfig, lvhbDirectionAHighBLow, lvhbBridge2) != kStatus_Success)
        {
            return false;
        }

        PRINTF("\tUse method RotateFull in forward direction to control interface 2\r\n");
        if (!RotateFull(drvConfig))
        {
            return false;
        }

        if (LVHB_SetDirection(drvConfig, lvhbDirectionALowBHigh, lvhbBridge2) != kStatus_Success)
        {
            return false;
        }

        PRINTF("\tUse method RotateFull in reverse direction to control interface 2\r\n");
        if (!RotateFull(drvConfig))
        {
            return false;
        }
    }
}

/*
 * @brief   Application entry point.
 */
int main(void) {
    lvhb_drv_config_t drvConfig;

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();

    /* Fill LVHB driver config. */
    LVHB_GetDefaultConfig(&drvConfig, lvhbDeviceMC34933, lvhbMotorBrushed);

    drvConfig.deviceConfig.brushPwmFrequency = 10000;
    drvConfig.deviceConfig.secondaryBridgeUsed = true;

    /* Timer settings */
    drvConfig.tmrInstance = 0;                                  /* FTM0 */
    drvConfig.tmrLvhbConfig.counterWidth = 16;
    drvConfig.tmrLvhbConfig.prescale = tmrPrescDiv_8;
    drvConfig.tmrLvhbConfig.srcClck_Hz = CLOCK_GetFreq(kCLOCK_BusClk);

    /* Pins */
    drvConfig.inputPins[lvhbBridge1] = lvhbPinsGpioPwm;         /* IN1A (GPIO) + IN1B (PWM) */
    drvConfig.inxaPinInstance[lvhbBridge1] = instanceA;         /* IN1A - PTA2/FTM0_CH7 */
    drvConfig.inxaPinIndex[lvhbBridge1] = 2U;
    drvConfig.tmrLvhbConfig.inxbChannelNumber[lvhbBridge1] = 1; /* IN1B - PTC2/FTM0_CH1 */

    drvConfig.inputPins[lvhbBridge2] = lvhbPinsGpio;            /* IN2A (GPIO) + IN2B (GPIO) */
    drvConfig.inxaPinInstance[lvhbBridge2] = instanceC;         /* IN2A - PTC12 */
    drvConfig.inxaPinIndex[lvhbBridge2] = 12U;
    drvConfig.inxbPinInstance[lvhbBridge2] = instanceC;         /* IN2B - PTC4/FTM0_CH3 */
    drvConfig.inxbPinIndex[lvhbBridge2] = 4U;

    if ((LVHB_ConfigureGpio(&drvConfig) != kStatus_Success) ||
        (LVHB_ConfigureTimer(&drvConfig, NULL) != kStatus_Success) ||
        (LVHB_Init(&drvConfig) != kStatus_Success))
    {
        /* An error occurred during initialization. */
        PRINTF("An error occurred during initialization.\r\n");
    }
    else if (!StartApplication(&drvConfig))
    {
        /* An error occurred during execution. */
        PRINTF("\tAn error occurred\r\n");
    }

    for(;;)  /* Infinite loop to avoid leaving the main function */
    {
        __asm("NOP");
    }
}
