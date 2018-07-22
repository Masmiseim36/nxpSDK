/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

#include "fsl_debug_console.h"
#include "board.h"
#include "math.h"
#include "fsl_fxos.h"
#include "fsl_i2c.h"
#include "fsl_ftm.h"

#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_TIMER_BASEADDR TPM3
#define BOARD_FIRST_TIMER_CHANNEL 6U
#define BOARD_SECOND_TIMER_CHANNEL 5U
/* Get source clock for TPM driver */
#define BOARD_TIMER_SOURCE_CLOCK CLOCK_GetFreq(BUS_CLK)
/* I2C source clock */
#define ACCEL_I2C_CLK_SRC I2C3_CLK_SRC
#define I2C_BAUDRATE 100000U
/* Upper bound and lower bound angle values */
#define ANGLE_UPPER_BOUND 85U
#define ANGLE_LOWER_BOUND 5U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/
i2c_master_handle_t g_MasterHandle;
/* FXOS device address */
const uint8_t g_accel_address[] = {0x1CU, 0x1DU, 0x1EU, 0x1FU};

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Initialize timer module */
static void Timer_Init(void)
{
    ftm_config_t ftmInfo;
    ftm_chnl_pwm_signal_param_t ftmParam[2];

    /* Configure ftm params with frequency 24kHZ */
    ftmParam[0].chnlNumber = (ftm_chnl_t)BOARD_FIRST_TIMER_CHANNEL;
    ftmParam[0].level = kFTM_LowTrue;
    ftmParam[0].dutyCyclePercent = 0U;
    ftmParam[0].firstEdgeDelayPercent = 0U;

    ftmParam[1].chnlNumber = (ftm_chnl_t)BOARD_SECOND_TIMER_CHANNEL;
    ftmParam[1].level = kFTM_LowTrue;
    ftmParam[1].dutyCyclePercent = 0U;
    ftmParam[1].firstEdgeDelayPercent = 0U;

    /*
     * ftmInfo.prescale = kFTM_Prescale_Divide_1;
     * ftmInfo.bdmMode = kFTM_BdmMode_0;
     * ftmInfo.pwmSyncMode = kFTM_SoftwareTrigger;
     * ftmInfo.reloadPoints = 0;
     * ftmInfo.faultMode = kFTM_Fault_Disable;
     * ftmInfo.faultFilterValue = 0;
     * ftmInfo.deadTimePrescale = kFTM_Deadtime_Prescale_1;
     * ftmInfo.deadTimeValue = 0;
     * ftmInfo.extTriggers = 0;
     * ftmInfo.chnlInitState = 0;
     * ftmInfo.chnlPolarity = 0;
     * ftmInfo.useGlobalTimeBase = false;
     */
    FTM_GetDefaultConfig(&ftmInfo);
    /* Initialize FTM module */
    FTM_Init(BOARD_FTM_BASEADDR, &ftmInfo);

    FTM_SetupPwm(BOARD_FTM_BASEADDR, ftmParam, 2U, kFTM_EdgeAlignedPwm, 24000U, BOARD_TIMER_SOURCE_CLOCK);
    FTM_StartTimer(BOARD_FTM_BASEADDR, kFTM_SystemClock);
}

/* Update the duty cycle of an active pwm signal */
static void Board_UpdatePwm(uint16_t x, uint16_t y)
{
    /* Start PWM mode with updated duty cycle */
    FTM_UpdatePwmDutycycle(BOARD_FTM_BASEADDR, (ftm_chnl_t)BOARD_FIRST_TIMER_CHANNEL, kFTM_EdgeAlignedPwm, x);
    FTM_UpdatePwmDutycycle(BOARD_FTM_BASEADDR, (ftm_chnl_t)BOARD_SECOND_TIMER_CHANNEL, kFTM_EdgeAlignedPwm, y);
    /* Software trigger to update registers */
    FTM_SetSoftwareTrigger(BOARD_FTM_BASEADDR, true);
}

int main(void)
{
    fxos_handle_t fxosHandle = {0};
    fxos_data_t sensorData = {0};
    i2c_master_config_t i2cConfig = {0};
    uint32_t i2cSourceClock = 0;
    int16_t xData = 0;
    int16_t yData = 0;
    int16_t xAngle = 0;
    int16_t yAngle = 0;
    uint8_t i = 0;
    uint8_t regResult = 0;
    uint8_t array_addr_size = 0;
    bool foundDevice = false;

    i2cSourceClock = CLOCK_GetFreq(ACCEL_I2C_CLK_SRC);
    fxosHandle.base = BOARD_ACCEL_I2C_BASEADDR;
    fxosHandle.i2cHandle = &g_MasterHandle;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*
     * i2cConfig.baudRate_Bps = 100000U;
     * i2cConfig.enableHighDrive = false;
     * i2cConfig.enableStopHold = false;
     * i2cConfig.glitchFilterWidth = 0U;
     * i2cConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(BOARD_ACCEL_I2C_BASEADDR, &i2cConfig, i2cSourceClock);
    I2C_MasterTransferCreateHandle(BOARD_ACCEL_I2C_BASEADDR, &g_MasterHandle, NULL, NULL);

    /* Find sensor devices */
    array_addr_size = sizeof(g_accel_address) / sizeof(g_accel_address[0]);
    for (i = 0; i < array_addr_size; i++)
    {
        fxosHandle.xfer.slaveAddress = g_accel_address[i];
        if (FXOS_ReadReg(&fxosHandle, WHO_AM_I_REG, &regResult, 1) == kStatus_Success)
        {
            foundDevice = true;
            break;
        }
        if ((i == (array_addr_size - 1)) && (!foundDevice))
        {
            PRINTF("\r\nDo not found sensor device\r\n");
            while (1)
            {
            };
        }
    }

    /* Init accelerometer sensor */
    FXOS_Init(&fxosHandle);
    /* Init timer */
    Timer_Init();

    /* Print a note to terminal */
    PRINTF("\r\nWelcome to BUBBLE example\r\n");
    PRINTF("\r\nYou will see the change of LED brightness when change angles of board\r\n");

    /* Main loop. Get sensor data and update duty cycle */
    while (1)
    {
        /* Get new accelerometer data. */
        FXOS_ReadSensorData(&fxosHandle, &sensorData);

        /* Get the X and Y data from the sensor data structure.fxos_data */
        xData = (int16_t)((uint16_t)((uint16_t)sensorData.accelXMSB << 8) | (uint16_t)sensorData.accelXLSB);
        yData = (int16_t)((uint16_t)((uint16_t)sensorData.accelYMSB << 8) | (uint16_t)sensorData.accelYLSB);

        /* Convert raw data to angle (normalize to 0-90 degrees). No negative angles. */
        xAngle = (int16_t)floor((double)xData * 0.011);
        if (xAngle < 0)
        {
            xAngle *= -1;
        }
        yAngle = (int16_t)floor((double)yData * 0.011);
        if (yAngle < 0)
        {
            yAngle *= -1;
        }
        /* Update angles to turn on LEDs when angles ~ 90 */
        if (xAngle > ANGLE_UPPER_BOUND)
        {
            xAngle = 100;
        }
        if (yAngle > ANGLE_UPPER_BOUND)
        {
            yAngle = 100;
        }
        /* Update angles to turn off LEDs when angles ~ 0 */
        if (xAngle < ANGLE_LOWER_BOUND)
        {
            xAngle = 0;
        }
        if (yAngle < ANGLE_LOWER_BOUND)
        {
            yAngle = 0;
        }

        Board_UpdatePwm(xAngle, yAngle);

        /* Print out the raw accelerometer data. */
        PRINTF("x= %d y = %d\r\n", xData, yData);
    }
}
