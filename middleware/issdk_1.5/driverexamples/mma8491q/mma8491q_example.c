/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * @file mma8491q_example.c
 * @brief The mma8491q_example.c file implements the ISSDK MMA8491Q sensor driver
 *        example demonstration with polling mode.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "board.h"
#include "pin_mux.h"
#include "fsl_lptmr.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"
#include "Driver_GPIO.h"
#include "fsl_i2c_cmsis.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "mma8491q_drv.h"
#include "systick_utils.h"
#include "gpio_driver.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
/* Timer timeout Callback. */
#define mma8491q_en_callback LPTMR0_IRQHandler
/* The LPTMR START STOP overhead. */
#define LPTMR_T_START_STOP 2U
/* Desired ODR rate in milli seconds (since the example uses LPTMR, this should be >= 10ms for reliable timing). */
#define MMA8491Q_T_ODR_ms 10U

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! @brief Address of Status Register. */
const registerreadlist_t cMma8491qStatus[] = {{.readFrom = MMA8491Q_STATUS, .numBytes = 1}, __END_READ_DATA__};

/*! @brief Address and size of Raw Acceleration Data. */
const registerreadlist_t cMma8491qOutput[] = {{.readFrom = MMA8491Q_OUT_X_MSB, .numBytes = MMA8491Q_DATA_SIZE},
                                              __END_READ_DATA__};

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
GENERIC_DRIVER_GPIO *pGpioDriver = &Driver_GPIO_KSDK; /* ISSDK GPIO Driver Handle. */
gpioConfigKSDK_t gGpioConfigInPins =                  /* SDK GPIO Config for Tilt Pins. */
    {
        .pinConfig = {kGPIO_DigitalInput, 0},
        .interruptMode = kPORT_InterruptOrDMADisabled,
        .portPinConfig = {0},
        .portPinConfig.mux = kPORT_MuxAsGpio,
};
mma8491q_context_t gSensorContext; /* MMA8491Q State Context parameters. */

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
/* LPTMR based EN control Timer Callback function. */
void mma8491q_en_callback(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    LPTMR_StopTimer(LPTMR0);
    gSensorContext.inTransition = false;
}

/* Function for MMA8491Q specific initialization tasks. */
void mma8491q_mode_fwk_init(mma8491q_context_t *pSensorContext, void *pEN)
{
    if (pSensorContext == NULL || pEN == NULL)
    {
        PRINTF("\r\n Error initializing MMA8491Q state framework. \r\n");
        return;
    }

    lptmr_config_t lptmrConfig;
    /* Initialize EN Timer. */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    LPTMR_Init(LPTMR0, &lptmrConfig);
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    EnableIRQ(LPTMR0_IRQn);

    /* In case required time between 2 samples - T_ON is greater than the RESET Time of sensor,
     * the ODR rate will be equal to the requested ODR - T_ON.
     * In case required time between 2 samples - T_ON is less than the RESET Time of sensor,
     * the ODR rate has to be at lest equal to the RESET Time for reliable operation. */
    pSensorContext->adjustedODR =
        (MMA8491Q_T_ODR_ms - MMA8491Q_T_ON_TYPICAL - LPTMR_T_START_STOP > MMA8491Q_T_RST_MIN) ?
            MMA8491Q_T_ODR_ms - MMA8491Q_T_ON_TYPICAL - LPTMR_T_START_STOP :
            MMA8491Q_T_RST_MIN;

    /* Initialize the Timer timeout variable. */
    pSensorContext->inTransition = false;
    /* Capture the GPIO handle. */
    pSensorContext->pGpioEN = pEN;

    /* Put the Sensor into Active Mode to enable I2C communication. */
    pSensorContext->inTransition = true;
    LPTMR_SetTimerPeriod(LPTMR0, MSEC_TO_COUNT(MMA8491Q_T_ON_TYPICAL, CLOCK_GetFreq(kCLOCK_LpoClk)));
    LPTMR_StartTimer(LPTMR0);

    /* Set EN = 1 to put the Sensor into Active Mode. */
    pGpioDriver->set_pin(pSensorContext->pGpioEN);
    /* Wait until Sensor is in ACTIVE Mode. */
    while (pSensorContext->inTransition)
    { /* Put CPU into wait mode, the timer expiry interrupt will awake the CPU. */
        SMC_SetPowerModeWait(SMC);
    }

    pSensorContext->sensorMode = SENSOR_MODE_ACTIVE;
}

/* Function to send sensor into Active Mode. */
void mma8491q_mode_activate(mma8491q_context_t *pSensorContext)
{
    if (pSensorContext == NULL || pSensorContext->sensorMode == SENSOR_MODE_STANDBY)
    {
        PRINTF("\r\n Error setting MMA8491Q to Active Mode. \r\n");
        return;
    }
    /* Wait until Sensor has entered SHUTDOWN Mode. */
    while (pSensorContext->inTransition)
    { /* Put CPU into wait mode, the timer expiry interrupt will awake the CPU. */
        SMC_SetPowerModeWait(SMC);
    }
    /* Start Timer for ACTIVE Mode. */
    pSensorContext->inTransition = true;
    LPTMR_SetTimerPeriod(LPTMR0, MSEC_TO_COUNT(MMA8491Q_T_ON_TYPICAL, CLOCK_GetFreq(kCLOCK_LpoClk)));
    LPTMR_StartTimer(LPTMR0);

    /* Set EN = 1 to put the Sensor into Active Mode. */
    pGpioDriver->set_pin(pSensorContext->pGpioEN);
    /* Wait until Sensor is in ACTIVE Mode. */
    while (pSensorContext->inTransition)
    { /* Put CPU into wait mode, the timer expiry interrupt will awake the CPU. */
        SMC_SetPowerModeWait(SMC);
    }

    pSensorContext->sensorMode = SENSOR_MODE_STANDBY;
}

/* Function to send sensor into Shutdown Mode. */
void mma8491q_mode_shutdown(mma8491q_context_t *pSensorContext)
{
    if (pSensorContext == NULL)
    {
        PRINTF("\r\n Error setting MMA8491Q to Shutdown Mode. \r\n");
        return;
    }

    /* Start Timer for SHUTDOWN Mode. */
    pSensorContext->inTransition = true;
    LPTMR_SetTimerPeriod(LPTMR0, MSEC_TO_COUNT(pSensorContext->adjustedODR, CLOCK_GetFreq(kCLOCK_LpoClk)));
    LPTMR_StartTimer(LPTMR0);

    /* Set EN = 0 to put the Sensor into Shutdown Mode. */
    pGpioDriver->clr_pin(pSensorContext->pGpioEN);
    pSensorContext->sensorMode = SENSOR_MODE_SHUTDOWN;
}

/*!
 * @brief Main function
 */
int main(void)
{
    int32_t status;
    uint8_t dataReady;
    uint8_t data[MMA8491Q_DATA_SIZE];
    mma8491q_acceldata_t rawData = {.timestamp = 0};

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!
    mma8491q_i2c_sensorhandle_t mma8491qDriver;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_SystickEnable();
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK MMA8491Q sensor driver example demonstration with poll mode\r\n");

    /* Tilt Pin IDs. */
    pGpioDriver->pin_init(&MMA8491_EN, GPIO_DIRECTION_OUT, NULL, NULL, NULL);
    pGpioDriver->pin_init(&MMA8491_XOUT, GPIO_DIRECTION_IN, &gGpioConfigInPins, NULL, NULL);
    pGpioDriver->pin_init(&MMA8491_YOUT, GPIO_DIRECTION_IN, &gGpioConfigInPins, NULL, NULL);
    pGpioDriver->pin_init(&MMA8491_ZOUT, GPIO_DIRECTION_IN, &gGpioConfigInPins, NULL, NULL);

    /*! Initialize the I2C driver. */
    status = I2Cdrv->Initialize(I2C_S_SIGNAL_EVENT);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Initialization Failed\r\n");
        return -1;
    }

    /*! Set the I2C Power mode. */
    status = I2Cdrv->PowerControl(ARM_POWER_FULL);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Power Mode setting Failed\r\n");
        return -1;
    }

    /*! Set the I2C bus speed. */
    status = I2Cdrv->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Control Mode setting Failed\r\n");
        return -1;
    }

    /*! Initialize MMA8491_EN Pin handling framework. */
    mma8491q_mode_fwk_init(&gSensorContext, &MMA8491_EN);

    /* Initialize the Sensor Driver. */
    status = MMA8491Q_I2C_Initialize(&mma8491qDriver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, MMA8491_I2C_ADDR);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n Sensor Initialization Failed\r\n");
        return -1;
    }
    PRINTF("\r\n Successfully Initiliazed Sensor\r\n");

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    MMA8491Q_I2C_SetIdleTask(&mma8491qDriver, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);

    for (;;) /* Forever loop */
    {
        /* Put the Sensor in Active Mode. */
        mma8491q_mode_activate(&gSensorContext);

        /*! Wait for data ready from the MMA8491Q. */
        status = MMA8491Q_I2C_ReadData(&mma8491qDriver, cMma8491qStatus, &dataReady);
        if (0 == (dataReady & MMA8491Q_STATUS_ZYXDR_MASK))
        { /* Loop, if new sample is not available. */
            continue;
        }

        /*! Read the raw sensor data from the MMA8491Q. */
        status = MMA8491Q_I2C_ReadData(&mma8491qDriver, cMma8491qOutput, data);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Read Failed. \r\n");
            return -1;
        }

        /* Put the sensor in Shutdown Mode. */
        mma8491q_mode_shutdown(&gSensorContext);

        /* Read Tilt Output */
        rawData.tilt[0] = pGpioDriver->read_pin(&MMA8491_XOUT);
        rawData.tilt[1] = pGpioDriver->read_pin(&MMA8491_YOUT);
        rawData.tilt[2] = pGpioDriver->read_pin(&MMA8491_ZOUT);

        /*! Process the sample and convert the raw sensor data. */
        rawData.accel[0] = ((int16_t)data[0] << 8) | (data[1]);
        rawData.accel[1] = ((int16_t)data[2] << 8) | (data[3]);
        rawData.accel[2] = ((int16_t)data[4] << 8) | (data[5]);

        PRINTF("\r\n Tilt :  X = %d  Y = %d  Z = %d ", rawData.tilt[0], rawData.tilt[1], rawData.tilt[2]);
        PRINTF("|| Accel : X = %5d  Y = %5d  Z = %5d \r\n", rawData.accel[0], rawData.accel[1], rawData.accel[2]);
        ASK_USER_TO_RESUME(1000); /* Ask for user input after processing 1000 samples. */
    }
}
