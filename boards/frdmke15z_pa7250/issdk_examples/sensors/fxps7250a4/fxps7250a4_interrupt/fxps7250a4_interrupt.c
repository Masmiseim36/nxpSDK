/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file fxps7250a4_interrupt.c
 * @brief The fxps7250a4_interrupt.c file implements the ISSDK FXPS7250A4 analog sensor
 *        driver example demonstration with interrupt mode.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lptmr.h"
#include "fsl_adc12.h"
#include "fsl_debug_console.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "gpio_driver.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
/* Timer timeout Callback. */
#define pa7250_odr_callback PWT_LPTMR0_IRQHandler
/* ADC completion Callback. */
#define adc12_irq_callback ADC0_IRQHandler

/* The desired ODR in milli seconds for the Pressure output */
#define FXPS7250A4_ODR_ms 1000U

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
volatile uint32_t gAdcConversionValue;
volatile bool bAdcCompletedFlag = false;
adc12_channel_config_t gAdcChannelConfigStruct;

//-----------------------------------------------------------------------
// Functions Prototypes
//-----------------------------------------------------------------------
void KE15Z_LPTMR0_ADC0_Initialize(void);

float analog_output_transfer_fn(float volt);

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
/* LPTMR based ODR control Timer Callback function. */
void pa7250_odr_callback(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    /* Trigger ADC Conversion */
    ADC12_SetChannelConfig(ADC0, FXPS7250A4_ADC12_CHANNEL_GROUP, &gAdcChannelConfigStruct);
}

/* ADC12 based voltage conversion completion IRQ Callback function. */
void adc12_irq_callback(void)
{
    /* Read conversion result to clear the conversion completed flag. */
    gAdcConversionValue = ADC12_GetChannelConversionValue(ADC0, FXPS7250A4_ADC12_CHANNEL_GROUP);
    bAdcCompletedFlag = true;
}

void KE15Z_LPTMR0_ADC0_Initialize(void)
{
    lptmr_config_t lptmrConfig;
    adc12_config_t adc12ConfigStruct;

    /* Configure ODR Timer. */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    LPTMR_Init(LPTMR0, &lptmrConfig);
    LPTMR_SetTimerPeriod(LPTMR0, MSEC_TO_COUNT(FXPS7250A4_ODR_ms, CLOCK_GetFreq(kCLOCK_LpoClk)));
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    EnableIRQ(PWT_LPTMR0_IRQn);
    LPTMR_StartTimer(LPTMR0);

    /* Configure ADC. */
    /* Set ADC12's clock source to be Slow IRC async clock. */
    CLOCK_SetIpSrc(kCLOCK_Adc0, kCLOCK_IpSrcSircAsync);
    EnableIRQ(ADC0_IRQn);
    ADC12_GetDefaultConfig(&adc12ConfigStruct);
    adc12ConfigStruct.clockSource = kADC12_ClockSourceAlt0;
    adc12ConfigStruct.resolution = kADC12_Resolution12Bit;
    ADC12_Init(ADC0, &adc12ConfigStruct);
    /* Set to software trigger mode. */
    ADC12_EnableHardwareTrigger(ADC0, false);
    /* Calibrate ADC. */
    if (kStatus_Success != ADC12_DoAutoCalibration(ADC0))
    {
        PRINTF("ADC calibration failed!\r\n");
    }
    gAdcChannelConfigStruct.channelNumber = FXPS7250A4_ADC12_CHANNEL;
    /* Enable the interrupt. */
    gAdcChannelConfigStruct.enableInterruptOnConversionCompleted = true;
}

float analog_output_transfer_fn(float volt)
{
    float pressure = 0.0;
	pressure = ((volt - 0.2)/0.02) + FXPS7250A4_OFFSET;
	
	return pressure;
}

/*!
 * @brief Main function
 */
int main(void)
{
    float outVolt = 0.0;
    float pressurekPa = 0.0;

    GENERIC_DRIVER_GPIO *gpioDriver = &Driver_GPIO_KSDK;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\n FXPS7250A4 ADC based Interrupt Example\r\n");

    /*! Initialize GREEN LED pin used by FRDM board */
    gpioDriver->pin_init(&GREEN_LED, GPIO_DIRECTION_OUT, NULL, NULL, NULL);

    /* Initialize LPTMR and ADC framework */
    KE15Z_LPTMR0_ADC0_Initialize();

    /* Forever loop */
    while (true)
    {
        if (false == bAdcCompletedFlag)
        {
            SMC_SetPowerModeWait(SMC);
            continue;
        }
        else
        {
            bAdcCompletedFlag = false;
            gpioDriver->toggle_pin(&GREEN_LED);
        }

        outVolt = (gAdcConversionValue * VCC)/BIT12RES;
		pressurekPa = analog_output_transfer_fn(outVolt);
        PRINTF("\r\n Raw Data Value: %d", gAdcConversionValue);
        PRINTF("\r\n Calculated Voltage Value: %0.2f Volts", outVolt);
        PRINTF("\r\n Analog Pressure Value: %0.2f kPa \n", pressurekPa);
        ASK_USER_TO_RESUME(10); /* Ask for user input after processing 10 samples. */
    }
}
