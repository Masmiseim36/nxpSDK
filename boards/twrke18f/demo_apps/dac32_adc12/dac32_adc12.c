/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_adc12.h"
#include "fsl_dac32.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC12_BASEADDR ADC0
#define DEMO_ADC12_CHANNEL_GROUP 0U
#define DEMO_ADC12_USER_CHANNEL 9U
#define DEMO_DAC32_BASEADDR DAC0
#define DAC_1_0_VOLTS 1241U
#define DAC_1_5_VOLTS 1862U
#define DAC_2_0_VOLTS 2482U
#define DAC_2_5_VOLTS 3103U
#define DAC_3_0_VOLTS 3724U

#define VREF_BRD 3.300
#define SE_12BIT 4096.0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
const uint32_t g_Adc12_12bitFullRange = 4096U;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    dac32_config_t dac32ConfigStruct;
    adc12_config_t adc12ConfigStruct;
    adc12_channel_config_t adc12ChannelConfigStruct;
    uint8_t msg = ' ';
    float voltRead;
    uint32_t adc12Value;

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(kCLOCK_Adc0, kCLOCK_IpSrcFircAsync);

    /* Configure the DAC32. */
    DAC32_GetDefaultConfig(&dac32ConfigStruct);
    DAC32_Init(DEMO_DAC32_BASEADDR, &dac32ConfigStruct);
    DAC32_Enable(DEMO_DAC32_BASEADDR, true);
    DAC32_EnableBufferOutput(DEMO_DAC32_BASEADDR, true);

    /* Initialize ADC12. */
    ADC12_GetDefaultConfig(&adc12ConfigStruct);
    adc12ConfigStruct.resolution = kADC12_Resolution12Bit;
    ADC12_Init(DEMO_ADC12_BASEADDR, &adc12ConfigStruct);
    /* Set to software trigger mode. */
    ADC12_EnableHardwareTrigger(DEMO_ADC12_BASEADDR, false);

    /* Calibrate ADC12. */
    if (kStatus_Success != ADC12_DoAutoCalibration(DEMO_ADC12_BASEADDR))
    {
        PRINTF("ADC calibration failed!\r\n");
    }

    /* Prepare ADC12 channel setting. */
    adc12ChannelConfigStruct.channelNumber                        = DEMO_ADC12_USER_CHANNEL; /* Set channel number. */
    adc12ChannelConfigStruct.enableInterruptOnConversionCompleted = false; /* Disable conversion interrupt. */

    PRINTF("DAC32 ADC12 Demo!\r\n");
    PRINTF("ADC Full Range: %d\r\n", g_Adc12_12bitFullRange);
    PRINTF("Press any key to start demo ...\r\n");
    GETCHAR();
    PRINTF("\r\nDemo begins...\r\n");

    while (1)
    {
        PRINTF(
            "Select DAC32 output level:\r\n"
            "\t 1. 1.0 V\r\n"
            "\t 2. 1.5 V\r\n"
            "\t 3. 2.0 V\r\n"
            "\t 4. 2.5 V\r\n"
            "\t 5. 3.0 V\r\n"
            "-->");

        /* Get the token and output the value with DAC32. */
        msg = ' ';
        msg = GETCHAR();
        PUTCHAR(msg);
        PRINTF("\r\n");
        switch (msg)
        {
            case '1':
                DAC32_SetBufferValue(DEMO_DAC32_BASEADDR, 0U, DAC_1_0_VOLTS);
                break;
            case '2':
                DAC32_SetBufferValue(DEMO_DAC32_BASEADDR, 0U, DAC_1_5_VOLTS);
                break;
            case '3':
                DAC32_SetBufferValue(DEMO_DAC32_BASEADDR, 0U, DAC_2_0_VOLTS);
                break;
            case '4':
                DAC32_SetBufferValue(DEMO_DAC32_BASEADDR, 0U, DAC_2_5_VOLTS);
                break;
            case '5':
                DAC32_SetBufferValue(DEMO_DAC32_BASEADDR, 0U, DAC_3_0_VOLTS);
                break;
            default:
                PRINTF("Please input a valid number: 1-5\r\n");
                break;
        }

        /* Trigger the conversion and read the value with ADC12 module. */
        ADC12_SetChannelConfig(DEMO_ADC12_BASEADDR, DEMO_ADC12_CHANNEL_GROUP, &adc12ChannelConfigStruct);
        while (0U == (kADC12_ChannelConversionCompletedFlag &
                      ADC12_GetChannelStatusFlags(DEMO_ADC12_BASEADDR, DEMO_ADC12_CHANNEL_GROUP)))
        {
        }
        adc12Value = ADC12_GetChannelConversionValue(DEMO_ADC12_BASEADDR, DEMO_ADC12_CHANNEL_GROUP);
        PRINTF("ADC Value: %d\r\n", adc12Value);

        /* Convert ADC12 value to a voltage based on 3.3V VREFH on board. */
        voltRead = (float)(adc12Value * (VREF_BRD / SE_12BIT));
        PRINTF("ADC Voltage: %0.3f\r\n", voltRead);

        /* Determine what to do next based on user's request. */
        PRINTF(
            "What next?:\r\n"
            "\t 1. Test another DAC output value.\r\n"
            "\t 2. Terminate demo.\r\n"
            "-->");
        msg = ' ';

        /* Wait in loop if the token is unavailable. */
        while ((msg < '1') || (msg > '2'))
        {
            msg = GETCHAR();
            PUTCHAR(msg);
            PUTCHAR('\b');
            PRINTF("\r\n");
        }

        if (msg == '2') /* Terminate the demo. */
        {
            DAC32_Deinit(DEMO_DAC32_BASEADDR);
            ADC12_Deinit(DEMO_ADC12_BASEADDR);
            break;
        }
        else
        {
        }
    }

    /* Demo End ... */
    PRINTF("\r\nDemo terminated! Reset device to begin again.\r\n");
    while (1)
    {
    }
}
