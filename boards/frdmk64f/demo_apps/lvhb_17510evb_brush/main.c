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
 * This function uses method LVHB_RotateProportional of the LVHB driver.
 * Duty is increasing to reach 100%. Then duty is decreased to 0%.
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
 * This function uses LVHB_RotateProportional and LVHB_SetTriState methods
 * of LVHB driver. It runs motor in forward direction using 100% PWM duty.
 * Then LVHB_SetTriState method is called. Motor should slowly decrease its
 * speed to zero. Brake of motor (LVHB_RotateProportional with 0% duty) follows.
 *
 * The function returns FALSE when an error occurred. Otherwise TRUE is returned.
 */
static bool SetTriState(lvhb_drv_config_t* const drvConfig)
{
    status_t error;

    PRINTF("\tRun motor with PWM duty set to 100 percent\r\n");
    if ((error = LVHB_RotateProportional(drvConfig, 100, lvhbBridge1)) != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(2);

    PRINTF("\tSet output to tri-state (freewheel)\r\n");
    if ((error = LVHB_SetTriState(drvConfig, lvhbBridge1)) != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(4);

    PRINTF("\tRun motor with PWM duty set to 100 percent\r\n");
    if ((error = LVHB_RotateProportional(drvConfig, 100, lvhbBridge1)) != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(2);

    PRINTF("\tSet output to LOW (brake)\r\n");
    if ((error = LVHB_RotateProportional(drvConfig, 0, lvhbBridge1)) != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(4);

    PRINTF("\tResult: OK\r\n");
    return true;
}

/**
 * This function uses LVHB_RotateProportional and LVHB_SetMode methods
 * of LVHB driver. It runs motor in forward direction using 100% PWM duty.
 * Then LVHB_SetMode method is called. Outputs of H-Bridge device should be
 * changed to LOW and motor brakes. After a while the mode is set back to
 * normal operational mode.
 *
 * The function returns FALSE when an error occurred. Otherwise TRUE is returned.
 */
static bool SetMode(lvhb_drv_config_t* const drvConfig)
{
    status_t error;

    /* Run motor. */
    PRINTF("\tRun motor with PWM duty set to 100 percent\r\n");
    if ((error = LVHB_RotateProportional(drvConfig, 100, lvhbBridge1)) != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(2);

    PRINTF("\tSet sleep mode (EN pin to LOW)\r\n");
    if ((error = LVHB_SetMode(drvConfig, false)) != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(2);

    PRINTF("\tSet normal mode (EN pin to HIGH)\r\n");
    if ((error = LVHB_SetMode(drvConfig, true)) != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(2);

    PRINTF("\tStop motor\r\n");
    if ((error = LVHB_RotateProportional(drvConfig, 0, lvhbBridge1)) != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    PRINTF("\tResult: OK\r\n");
    return true;
}

/**
 * This function uses method LVHB_SetGateDriver of the LVHB driver.
 * It controls H-Bridge output GOUT.
 *
 * The function returns FALSE when an error occurred. Otherwise TRUE is returned.
 */
static bool SetGateDriver(lvhb_drv_config_t* const drvConfig)
{
    status_t error;

    PRINTF("\tDefault value of GOUT is LOW\r\n");

    /* Wait. */
    WAIT_AML_WaitSec(4);

    PRINTF("\tSet GOUT to HIGH\r\n");
    if ((error = LVHB_SetGateDriver(drvConfig, true)) != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
        return false;
    }

    /* Wait. */
    WAIT_AML_WaitSec(4);

    PRINTF("\tSet GOUT back to LOW\r\n");
    if ((error = LVHB_SetGateDriver(drvConfig, false)) != kStatus_Success)
    {
        PRINTF("\tReturned error code %u\r\n", (unsigned int) error);
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
    PRINTF("\r\n1. Test method SetTriState\r\n");
    if (!SetTriState(drvConfig))
    {
        return false;
    }

    PRINTF("\r\n2. Test method SetMode\r\n");
    if (!SetMode(drvConfig))
    {
        return false;
    }

    PRINTF("\r\n3. Test method SetGateDriver\r\n");
    if (!SetGateDriver(drvConfig))
    {
        return false;
    }

    PRINTF("\r\n4. Test method RotateProportional and SetDirection\r\n");
    while (true)
    {
        if (LVHB_SetDirection(drvConfig, lvhbDirectionAHighBLow, lvhbBridge1) != kStatus_Success)
        {
            return false;
        }

        PRINTF("\tUse method RotateProportional in forward direction\r\n");
        if (!RotateProportional(drvConfig))
        {
            return false;
        }

        if (LVHB_SetDirection(drvConfig, lvhbDirectionALowBHigh, lvhbBridge1) != kStatus_Success)
        {
            return false;
        }

        PRINTF("\tUse method RotateProportional in reverse direction\r\n");
        if (!RotateProportional(drvConfig))
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
    LVHB_GetDefaultConfig(&drvConfig, lvhbDeviceMPC17510, lvhbMotorBrushed);

    drvConfig.deviceConfig.activeMode = true;
    drvConfig.deviceConfig.gateDriverOutputHigh = false;
    drvConfig.deviceConfig.brushPwmFrequency = 5000;

    /* Timer settings */
    drvConfig.tmrInstance = 0;                                  /* FTM0 */
    drvConfig.tmrLvhbConfig.counterWidth = 16;
    drvConfig.tmrLvhbConfig.prescale = tmrPrescDiv_8;
    drvConfig.tmrLvhbConfig.srcClck_Hz = CLOCK_GetFreq(kCLOCK_BusClk);

    /* Pins */
    drvConfig.inputPins[lvhbBridge1] = lvhbPinsPwm;             /* IN1 (PWM) + IN2 (PWM) */
    drvConfig.tmrLvhbConfig.inxaChannelNumber[lvhbBridge1] = 7; /* IN1 - PTA2/FTM0_CH7 */
    drvConfig.tmrLvhbConfig.inxbChannelNumber[lvhbBridge1] = 1; /* IN2 - PTC2/FTM0_CH1 */

    drvConfig.enPinInstance = instanceC;                        /* EN - PTC17 */
    drvConfig.enPinIndex = 17U;
    drvConfig.ginPinInstance = instanceE;                       /* GIN - PTE25 */
    drvConfig.ginPinIndex = 25U;

    if ((LVHB_ConfigureGpio(&drvConfig) != kStatus_Success) ||
        (LVHB_ConfigureTimer(&drvConfig, NULL) != kStatus_Success) ||
        (LVHB_Init(&drvConfig) != kStatus_Success))
    {
        PRINTF("An error occurred during initialization.\r\n");
    }
    else if (!StartApplication(&drvConfig))
    {
        PRINTF("\tAn error occurred\r\n");
    }

    for(;;)  /* Infinite loop to avoid leaving the main function */
    {
        __asm("NOP");
    }
}
