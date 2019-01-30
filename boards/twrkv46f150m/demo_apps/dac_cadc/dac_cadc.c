/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_cadc.h"
#include "fsl_dac.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_CADC_BASEADDR ADC
#define DEMO_CADC_CHANNEL 5U
#define DEMO_DAC_BASEADDR DAC0
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
/* Initialize DAC */
static void Demo_DAC_Init(void);
/* Initialize CADC */
static void Demo_CADC_Init(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void Demo_DAC_Init(void)
{
    dac_config_t dacConfigStruct;

    /* Configure DAC with default configuration */
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true); /* Enable output. */
    DAC_ClearBufferStatusFlags(DEMO_DAC_BASEADDR, DAC_SR_DACBFRPTF_MASK);
}

static void Demo_CADC_Init(void)
{
    cadc_config_t cadcConfigStruct;
    cadc_converter_config_t cadcConverterConfigStruct;
    cadc_sample_config_t cadcSampleConfigStruct;

    /* Configure the CADC */
    CADC_GetDefaultConfig(&cadcConfigStruct);
    cadcConfigStruct.dualConverterScanMode = kCADC_DualConverterWorkAsOnceSequential;
    cadcConfigStruct.powerUpDelay = 0x2AU;
    cadcConfigStruct.enableSimultaneousMode = false;
    CADC_Init(DEMO_CADC_BASEADDR, &cadcConfigStruct);

    /* Trigger channel */
    CADC_EnableConverterDMA(DEMO_CADC_BASEADDR, kCADC_ConverterA, false);
    CADC_EnableConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);
    CADC_EnableConverterSyncInput(DEMO_CADC_BASEADDR, kCADC_ConverterA, false);
    CADC_DisableInterrupts(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanInterruptEnable);

    CADC_GetDefaultConverterConfig(&cadcConverterConfigStruct);
    cadcConverterConfigStruct.clockDivisor = 0xAU;
    cadcConverterConfigStruct.speedMode = kCADC_SpeedMode3;
    CADC_SetConverterConfig(DEMO_CADC_BASEADDR, kCADC_ConverterA, &cadcConverterConfigStruct);
    CADC_EnableConverterPower(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);

    /* Configure slot in conversion sequence. */
    /* Common setting. */
    CADC_EnableSample(DEMO_CADC_BASEADDR, 0U, false);
    cadcSampleConfigStruct.channelNumber = DEMO_CADC_CHANNEL;
    cadcSampleConfigStruct.enableDifferentialPair = 0U;
    cadcSampleConfigStruct.zeroCrossingMode = kCADC_ZeroCorssingDisabled;
    cadcSampleConfigStruct.lowLimitValue = 0U;
    cadcSampleConfigStruct.highLimitValue = 0xFFFFU;
    cadcSampleConfigStruct.offsetValue = 0U;
    cadcSampleConfigStruct.enableWaitSync = false;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 0U, &cadcSampleConfigStruct);
    CADC_EnableSample(DEMO_CADC_BASEADDR, 0U, true);
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 1U, &cadcSampleConfigStruct);
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t msg = ' ';
    uint32_t i = 0;
    uint32_t CadcConversionValue = 0U;
    float voltRead = 0;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nDAC CADC Demo!\r\n");
    PRINTF("\r\nChapter DAC CADC demo, for pins configuration information.\r\n");

    PRINTF("\r\nPress any key to start demo.\r\n");
    GETCHAR();
    PRINTF("\r\nDemo begin...\r\n");

    /* Init DAC & CADC */
    Demo_DAC_Init();
    Demo_CADC_Init();

    while (1)
    {
        PRINTF(
            "\r\n\r\nSelect DAC output level:\r\n\t1. 1.0 V\r\n\t2. 1.5 V\r\n\t3. 2.0 V\r\n\t4. 2.5 V\r\n\t5. 3.0 "
            "V\r\n-->");

        msg = ' ';
        while ((msg < '1') || (msg > '5'))
        {
            msg = GETCHAR();
            PUTCHAR(msg);
            switch (msg)
            {
                case '1':
                    DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, DAC_1_0_VOLTS);
                    break;
                case '2':
                    DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, DAC_1_5_VOLTS);
                    break;
                case '3':
                    DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, DAC_2_0_VOLTS);
                    break;
                case '4':
                    DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, DAC_2_5_VOLTS);
                    break;
                case '5':
                    DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, DAC_3_0_VOLTS);
                    break;
                default:
                    PRINTF("\r\nPlease input a valid number: 1-5 \r\n");
                    break;
            }
        }

        /* Wait for DAC stability */
        for (i = 0; i < 0xFFFF; i++)
        {
            __NOP();
        }

        CADC_ClearStatusFlags(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);
        CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA);

        while (kCADC_ConverterAEndOfScanFlag !=
               (kCADC_ConverterAEndOfScanFlag & CADC_GetStatusFlags(DEMO_CADC_BASEADDR)))
        {
        }

        /* Get Sample Result value */
        CadcConversionValue = CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, 0U);
        /* The lower three bits of ADC_RSLT register are always going to be zero */
        CadcConversionValue = CadcConversionValue >> 3;

        /* Clear buffer status Flags */
        DAC_ClearBufferStatusFlags(DEMO_DAC_BASEADDR, DAC_SR_DACBFRPTF_MASK);
        PRINTF("\r\n\r\nADC Value: %d\r\n", CadcConversionValue);

        /* Convert CADC value to a voltage based on 3.3V VREFH on board */
        voltRead = (float)(CadcConversionValue * (VREF_BRD / SE_12BIT));

        PRINTF("\r\nADC Voltage: %0.3f\r\n", voltRead);
        /* Clear status Flags */
        CADC_ClearStatusFlags(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);
        /* Determine what to do next based on user's request */
        PRINTF("\r\nWhat next?:\r\n\t1. Test another DAC output value.\r\n\t2. Terminate demo.\r\n-->");

        msg = ' ';
        while ((msg < '1') || (msg > '2'))
        {
            msg = GETCHAR();
            PUTCHAR(msg);
            PUTCHAR('\b');
        }

        /* Set next state */
        if (msg == '2')
        {
            DAC_Deinit(DEMO_DAC_BASEADDR);
            CADC_Deinit(DEMO_CADC_BASEADDR);
            break;
        }
    }
    PRINTF("\r\nDemo terminated! Reset device to begin again.\r\n");
    while (1)
    {
    }
}
