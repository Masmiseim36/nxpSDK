/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* FXOS8700 and MMA8451 have the same register address */
#define ACCEL_REG_OUT_X_MSB    0x01
#define ACCEL_REG_F_SETUP      0x09
#define ACCEL_REG_TRIG_CFG     0x0A
#define ACCEL_REG_INT_SOURCE   0x0C
#define ACCEL_REG_WHO_AM_I     0x0D
#define ACCEL_REG_XYZ_DATA_CFG 0x0E
#define ACCEL_REG_PULSE_CFG    0x21
#define ACCEL_REG_PULSE_SRC    0x22
#define ACCEL_REG_PULSE_THSX   0x23
#define ACCEL_REG_PULSE_THSY   0x24
#define ACCEL_REG_PULSE_THSZ   0x25
#define ACCEL_REG_PULSE_TMLT   0x26
#define ACCEL_REG_PULSE_LTCY   0x27
#define ACCEL_REG_CTRL1        0x2A
#define ACCEL_REG_CTRL2        0x2B
#define ACCEL_REG_CTRL3        0x2C
#define ACCEL_REG_CTRL4        0x2D
#define ACCEL_REG_CTRL5        0x2E

#define FXOS8700_WHOAMI 0xC7U
#define MMA8451_WHOAMI  0x1AU
#define MMA8652_WHOAMI  0x4AU
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_ACCEL_Init(void);
static status_t APP_ACCEL_WriteReg(uint8_t reg, uint8_t value);
static status_t APP_ACCEL_ReadRegs(uint8_t reg, uint8_t *pValues, uint32_t bytes);

/*******************************************************************************
 * Variables
 *******************************************************************************/
static volatile bool wakeFlag;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*  ACCEL write register wrap function */
static status_t APP_ACCEL_WriteReg(uint8_t reg, uint8_t value)
{
    return BOARD_Accel_I2C_Send(BOARD_ACCEL_I2C_ADDR, reg, 1, value);
}

/*  ACCEL read register wrap function */
static status_t APP_ACCEL_ReadRegs(uint8_t reg, uint8_t *pValues, uint32_t bytes)
{
    return BOARD_Accel_I2C_Receive(BOARD_ACCEL_I2C_ADDR, reg, 1, pValues, bytes);
}

void APP_ACCEL_IntHandler(void)
{
    wakeFlag = true;
}

/* Init the accelerometer */
static void APP_ACCEL_Init(void)
{
    uint8_t value;
    status_t status;

    status = APP_ACCEL_ReadRegs(ACCEL_REG_WHO_AM_I, &value, 1);
    assert(status == kStatus_Success);

    if (value != FXOS8700_WHOAMI && value != MMA8451_WHOAMI && value != MMA8652_WHOAMI)
    {
        PRINTF("The sensor on board not supported, device id (%d)\r\n", value);
        return;
    }

    /* Put ACCEL into standby mode. */
    status = APP_ACCEL_WriteReg(ACCEL_REG_CTRL1, 0x0U);
    assert(status == kStatus_Success);
    /* Set FIFO trigger mode, watermark 20. */
    status = APP_ACCEL_WriteReg(ACCEL_REG_F_SETUP, 0xD4U);
    assert(status == kStatus_Success);
    /* Set the Trigger for Tap Detection. */
    status = APP_ACCEL_WriteReg(ACCEL_REG_TRIG_CFG, 0x8U);
    assert(status == kStatus_Success);
    /* Enable X and Y and Z Single Pulse. */
    status = APP_ACCEL_WriteReg(ACCEL_REG_PULSE_CFG, 0x15U);
    assert(status == kStatus_Success);
    /* Set X Threshold to 1.575g. 1.575/0.063=25. */
    status = APP_ACCEL_WriteReg(ACCEL_REG_PULSE_THSX, 0x19U);
    assert(status == kStatus_Success);
    /* Set X Threshold to 1.575g. 1.575/0.063=25. */
    status = APP_ACCEL_WriteReg(ACCEL_REG_PULSE_THSY, 0x19U);
    assert(status == kStatus_Success);
    /* Set X Threshold to 2.52g. 2.52/0.063=40. */
    status = APP_ACCEL_WriteReg(ACCEL_REG_PULSE_THSZ, 0x28U);
    assert(status == kStatus_Success);
    /* Set Time Limit for Tap Detection to 50 ms No LPF Enabled. 50/0.625=80. */
    status = APP_ACCEL_WriteReg(ACCEL_REG_PULSE_TMLT, 0x50U);
    assert(status == kStatus_Success);
    /* Set Latency Timer to 300 ms. 300/1.25=240. */
    status = APP_ACCEL_WriteReg(ACCEL_REG_PULSE_LTCY, 0xF0U);
    assert(status == kStatus_Success);
    /* Enable TAP interrupt and set interrupt Pin to INT1. */
    status = APP_ACCEL_WriteReg(ACCEL_REG_CTRL4, 0x8U);
    assert(status == kStatus_Success);
    status = APP_ACCEL_WriteReg(ACCEL_REG_CTRL5, 0x8U);
    assert(status == kStatus_Success);
    /* Configure the data range to 4g. */
    status = APP_ACCEL_WriteReg(ACCEL_REG_XYZ_DATA_CFG, 0x01U);
    assert(status == kStatus_Success);
    /* Put ACCEL out of the standby, active to 800Hz low noise, normal read mode */
    status = APP_ACCEL_WriteReg(ACCEL_REG_CTRL1, 0x5U);
    if (status != kStatus_Success)
    {
        assert(false);
    }
}

int main(void)
{
    int16_t xData, yData, zData;
    uint8_t values[6];
    uint8_t value;
    status_t status;
    uint32_t i;

    BOARD_InitHardware();

    PRINTF("\r\nI2C example -- Accelerometer Event Trigger\r\n");

    /* I2C initialize */
    BOARD_Accel_I2C_Init();

    /* Init the FXOS8700 accelerometer */
    APP_ACCEL_Init();

    /* Init wakeup pin. */
    EXAMPLE_InitWakeupPin();

    PRINTF("Press any key to enter low power mode\r\n");
    GETCHAR();
    PRINTF("Enter deep sleep, tap the board to wake it up.\r\n");
    EXAMPLE_EnterDeepSleep();
    PRINTF("Woken up!\r\n");

    if (!wakeFlag)
    {
        assert(false);
    }

    status = APP_ACCEL_ReadRegs(ACCEL_REG_INT_SOURCE, &value, 1);
    if (status != kStatus_Success)
    {
        assert(false);
    }

    PRINTF("Interrupt source 0x%x.\r\n", value);

    if ((value & 0x8) == 0x8) /* pulse interrupt */
    {
        /* Read and clear pulse interrupt source. */
        status = APP_ACCEL_ReadRegs(ACCEL_REG_PULSE_SRC, &value, 1);
        assert(status == kStatus_Success);
        PRINTF("Pulse trigger source 0x%x.\r\n", value);
    }

    for (i = 0; i < 32; i++)
    {
        status = APP_ACCEL_ReadRegs(ACCEL_REG_OUT_X_MSB, values, 6);
        assert(status == kStatus_Success);
        /* Calculate X, Y, Z value */
        xData = (((((int16_t)(int8_t)values[0]) << 8) | values[1])) / 4;
        yData = (((((int16_t)(int8_t)values[2]) << 8) | values[3])) / 4;
        zData = (((((int16_t)(int8_t)values[4]) << 8) | values[5])) / 4;
        /* Output data*/
        PRINTF("%2d -- X:%5d,  Y:%5d,  Z:%5d\r\n", i, xData, yData, zData);
    }

    while (1)
    {
    }
}
