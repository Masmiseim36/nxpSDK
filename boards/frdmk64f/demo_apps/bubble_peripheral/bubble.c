/*
 * The Clear BSD License
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define I2C_RELEASE_BUS_COUNT 100U
/* Upper bound and lower bound angle values */
#define ANGLE_UPPER_BOUND 85U
#define ANGLE_LOWER_BOUND 5U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);
static void Board_UpdatePwm(uint16_t x, uint16_t y);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile int16_t g_xAngle = 0;
volatile int16_t g_yAngle = 0;
volatile int16_t g_xDuty = 0;
volatile int16_t g_yDuty = 0;
/* FXOS device address */
const uint8_t g_accel_address[] = {0x1CU, 0x1DU, 0x1EU, 0x1FU};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;

    BOARD_Init_I2C_GPIO_pins();

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(BOARD_ACCEL_I2C_SDA_GPIO, BOARD_ACCEL_I2C_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(BOARD_ACCEL_I2C_SCL_GPIO, BOARD_ACCEL_I2C_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(BOARD_ACCEL_I2C_SDA_GPIO, BOARD_ACCEL_I2C_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(BOARD_ACCEL_I2C_SCL_GPIO, BOARD_ACCEL_I2C_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(BOARD_ACCEL_I2C_SCL_GPIO, BOARD_ACCEL_I2C_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(BOARD_ACCEL_I2C_SDA_GPIO, BOARD_ACCEL_I2C_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(BOARD_ACCEL_I2C_SCL_GPIO, BOARD_ACCEL_I2C_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(BOARD_ACCEL_I2C_SDA_GPIO, BOARD_ACCEL_I2C_SDA_PIN, 1U);
    i2c_release_bus_delay();
}

void BOARD_TIMER_IRQHANDLER(void)
{
    uint32_t status = FTM_GetStatusFlags(BOARD_TIMER_PERIPHERAL);

    if (status & kFTM_TimeOverflowFlag)
    {
        Board_UpdatePwm(g_xDuty, g_yDuty);
        if (g_xDuty)
        {
            LED_GREEN_ON();
        }
        else
        {
            LED_GREEN_OFF();
        }
        if (g_yDuty)
        {
            LED_RED_ON();
        }
        else
        {
            LED_RED_OFF();
        }
        FTM_ClearStatusFlags(BOARD_TIMER_PERIPHERAL, kFTM_TimeOverflowFlag);
    }
    if (status & kFTM_Chnl0Flag)
    {
        LED_GREEN_OFF();
        FTM_ClearStatusFlags(BOARD_TIMER_PERIPHERAL, kFTM_Chnl0Flag);
    }
    if (status & kFTM_Chnl1Flag)
    {
        LED_RED_OFF();
        FTM_ClearStatusFlags(BOARD_TIMER_PERIPHERAL, kFTM_Chnl1Flag);
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    exception return operation might vector to incorrect interrupt */
    __DSB();
}

/* Update the duty cycle of an active pwm signal */
static void Board_UpdatePwm(uint16_t x, uint16_t y)
{
    /* Start PWM mode with updated duty cycle */
    FTM_UpdatePwmDutycycle(BOARD_TIMER_PERIPHERAL, kFTM_Chnl_0, kFTM_EdgeAlignedPwm, x);
    FTM_UpdatePwmDutycycle(BOARD_TIMER_PERIPHERAL, kFTM_Chnl_1, kFTM_EdgeAlignedPwm, y);
    /* Software trigger to update registers */
    FTM_SetSoftwareTrigger(BOARD_TIMER_PERIPHERAL, true);
}

int main(void)
{
    fxos_handle_t fxosHandle = {0};
    fxos_data_t sensorData = {0};
    fxos_config_t config = {0};
    uint8_t sensorRange = 0;
    uint8_t dataScale = 0;
    int16_t xData = 0;
    int16_t yData = 0;
    uint8_t i = 0;
    uint8_t array_addr_size = 0;
    status_t result = kStatus_Fail;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();
    BOARD_InitPeripherals();

    /* Configure the I2C function */
    config.I2C_SendFunc = BOARD_Accel_I2C_Send;
    config.I2C_ReceiveFunc = BOARD_Accel_I2C_Receive;

    /* Find sensor devices */
    array_addr_size = sizeof(g_accel_address) / sizeof(g_accel_address[0]);
    for (i = 0; i < array_addr_size; i++)
    {
        config.slaveAddress = g_accel_address[i];
        /* Initialize accelerometer sensor */
        result = FXOS_Init(&fxosHandle, &config);
        if (result == kStatus_Success)
        {
            break;
        }
    }

    if (result != kStatus_Success)
    {
        PRINTF("\r\nSensor device initialize failed!\r\n");
        return -1;
    }

    /* Get sensor range */
    if (FXOS_ReadReg(&fxosHandle, XYZ_DATA_CFG_REG, &sensorRange, 1) != kStatus_Success)
    {
        return -1;
    }
    if (sensorRange == 0x00)
    {
        dataScale = 2U;
    }
    else if (sensorRange == 0x01)
    {
        dataScale = 4U;
    }
    else if (sensorRange == 0x10)
    {
        dataScale = 8U;
    }
    else
    {
    }

    /* Start timer */
    FTM_StartTimer(BOARD_TIMER_PERIPHERAL, kFTM_SystemClock);

    /* Print a note to terminal */
    PRINTF("\r\nWelcome to the BUBBLE example\r\n");
    PRINTF("\r\nYou will see the LED brightness change when the angle of the board changes\r\n");

    /* Main loop. Get sensor data and update duty cycle */
    while (1)
    {
        /* Get new accelerometer data. */
        if (FXOS_ReadSensorData(&fxosHandle, &sensorData) != kStatus_Success)
        {
            return -1;
        }

        /* Get the X and Y data from the sensor data structure in 14 bit left format data*/
        xData = (int16_t)((uint16_t)((uint16_t)sensorData.accelXMSB << 8) | (uint16_t)sensorData.accelXLSB) / 4U;
        yData = (int16_t)((uint16_t)((uint16_t)sensorData.accelYMSB << 8) | (uint16_t)sensorData.accelYLSB) / 4U;

        /* Convert raw data to angle (normalize to 0-90 degrees). No negative angles. */
        g_xAngle = (int16_t)floor((double)xData * (double)dataScale * 90 / 8192);
        if (g_xAngle < 0)
        {
            g_xAngle *= -1;
        }
        g_yAngle = (int16_t)floor((double)yData * (double)dataScale * 90 / 8192);
        if (g_yAngle < 0)
        {
            g_yAngle *= -1;
        }

        /* Update duty cycle to turn on LEDs when angles ~ 90 */
        if (g_xAngle > ANGLE_UPPER_BOUND)
        {
            g_xDuty = 100;
        }
        if (g_yAngle > ANGLE_UPPER_BOUND)
        {
            g_yDuty = 100;
        }
        /* Update duty cycle to turn off LEDs when angles ~ 0 */
        if (g_xAngle < ANGLE_LOWER_BOUND)
        {
            g_xDuty = 0;
        }
        if (g_yAngle < ANGLE_LOWER_BOUND)
        {
            g_yDuty = 0;
        }
        /* Print out the angle. */
        PRINTF("x= %2d y = %2d\r\n", g_xAngle, g_yAngle);
    }
}
