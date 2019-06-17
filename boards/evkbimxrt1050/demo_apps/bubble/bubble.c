/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "math.h"
#include "fsl_qtmr.h"
#include "fsl_fxos.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_QTMR_BASEADDR TMR3
#define BOARD_QTMR_FIRST_CHANNEL (kQTMR_Channel_0)
#define QTMR_IRQ_HANDLER TMR3_IRQHandler
#define QTMR_IRQ_ID TMR3_IRQn

/* Get source clock for QTMR driver */
#define QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)

/* LPI2C */
#define I2C_BAUDRATE (100000U)
/* Upper bound and lower bound angle values */
#define ANGLE_UPPER_BOUND 85U
#define ANGLE_LOWER_BOUND 5U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void Board_UpdatePwm(uint16_t x);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile int16_t g_xAngle = 0;
volatile int16_t g_yAngle = 0;
volatile int16_t g_xDuty  = 0;
volatile int16_t g_yDuty  = 0;
/* FXOS device address */
const uint8_t g_accel_address[] = {0x1CU, 0x1DU, 0x1EU, 0x1FU};

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Initialize timer module */
static void Timer_Init(void)
{
    qtmr_config_t qtmrConfig;

    /*
     * qtmrConfig.debugMode = kQTMR_RunNormalInDebug;
     * qtmrConfig.enableExternalForce = false;
     * qtmrConfig.enableMasterMode = false;
     * qtmrConfig.faultFilterCount = 0;
     * qtmrConfig.faultFilterPeriod = 0;
     * qtmrConfig.primarySource = kQTMR_ClockDivide_2;
     * qtmrConfig.secondarySource = kQTMR_Counter0InputPin;
     */
    QTMR_GetDefaultConfig(&qtmrConfig);
    qtmrConfig.primarySource = kQTMR_ClockDivide_128;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_QTMR_FIRST_CHANNEL, &qtmrConfig);

    /* Enable timer compare interrupt */
    QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_QTMR_FIRST_CHANNEL,
                          kQTMR_Compare1InterruptEnable | kQTMR_Compare2InterruptEnable);
    NVIC_EnableIRQ(QTMR_IRQ_ID);

    /* Generate a 50Khz PWM signal with 50% dutycycle */
    QTMR_SetupPwm(BOARD_QTMR_BASEADDR, BOARD_QTMR_FIRST_CHANNEL, 50000, 50, false, QTMR_SOURCE_CLOCK / 128);
    /* Start the counter */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_QTMR_FIRST_CHANNEL, kQTMR_PriSrcRiseEdge);
}

void QTMR_IRQ_HANDLER(void)
{
    uint32_t status = QTMR_GetStatus(BOARD_QTMR_BASEADDR, BOARD_QTMR_FIRST_CHANNEL);

    if (status & kQTMR_Compare1Flag)
    {
        USER_LED_ON();
        QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_FIRST_CHANNEL, kQTMR_Compare1Flag);
    }
    if (status & kQTMR_Compare2Flag)
    {
        USER_LED_OFF();
        Board_UpdatePwm(g_xDuty);
        QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_FIRST_CHANNEL, kQTMR_Compare2Flag);
    }
    __DSB();
}

void UpdatePwmDutycycle(TMR_Type *base, qtmr_channel_selection_t channel, uint32_t pwmFreqHz, uint8_t dutyCyclePercent)
{
    QTMR_StopTimer(base, channel);
    QTMR_SetupPwm(base, channel, pwmFreqHz, dutyCyclePercent, false, QTMR_SOURCE_CLOCK / 128);
    QTMR_StartTimer(base, channel, kQTMR_PriSrcRiseEdge);
}

/* Update the duty cycle of an active pwm signal */
static void Board_UpdatePwm(uint16_t x)
{
    UpdatePwmDutycycle(BOARD_QTMR_BASEADDR, (qtmr_channel_selection_t)BOARD_QTMR_FIRST_CHANNEL, 50000, x);
}

int main(void)
{
    fxos_handle_t fxosHandle = {0};
    fxos_data_t sensorData   = {0};
    fxos_config_t config     = {0};
    status_t result;
    gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    uint8_t sensorRange          = 0;
    uint8_t dataScale            = 0;
    int16_t xData                = 0;
    int16_t yData                = 0;
    uint8_t i                    = 0;
    uint8_t array_addr_size      = 0;

    /* Board pin, clock, debug console init */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();

    /* Clock setting for LPI2C */
    CLOCK_SetMux(kCLOCK_Lpi2cMux, BOARD_ACCEL_I2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, BOARD_ACCEL_I2C_CLOCK_SOURCE_DIVIDER);

    /* Init output LED GPIO. */
    GPIO_PinInit(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &led_config);
    /* Initialize LED pins below */
    USER_LED_INIT(LOGIC_LED_OFF);

    /* I2C initialize */
    BOARD_Accel_I2C_Init();
    /* Configure the I2C function */
    config.I2C_SendFunc    = BOARD_Accel_I2C_Send;
    config.I2C_ReceiveFunc = BOARD_Accel_I2C_Receive;

    /* Initialize sensor devices */
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

    USER_LED_INIT(0);

    /* Init timer */
    Timer_Init();

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
        g_xDuty = g_xAngle * 100 / 90;
        g_yDuty = g_yAngle * 100 / 90;
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
        /* Print out the angle data. */
        PRINTF("x= %2d y = %2d\r\n", g_xAngle, g_yAngle);
    }
}
