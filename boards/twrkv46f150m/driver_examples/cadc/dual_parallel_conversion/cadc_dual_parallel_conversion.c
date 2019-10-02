/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_cadc.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_CADC_BASEADDR ADC

#define DEMO_CADC_CHANNEL1_NUMBER 4U
#define DEMO_CADC_CHANNEL1_ENABLE_DIFF true

#define DEMO_CADC_CHANNEL2_NUMBER 6U
#define DEMO_CADC_CHANNEL2_ENABLE_DIFF true

#define DEMO_CADC_CHANNEL3_NUMBER 12U
#define DEMO_CADC_CHANNEL3_ENABLE_DIFF true

#define DEMO_CADC_CHANNEL4_NUMBER 14U
#define DEMO_CADC_CHANNEL4_ENABLE_DIFF true

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    cadc_config_t cadcConfigStruct;
    cadc_converter_config_t cadcConverterConfigStruct;
    cadc_sample_config_t cadcSampleConfigStruct;
    uint16_t sampleMask;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("CADC dual parallel conversion example.\r\n");

    /* Initialize the CADC common digital control.
     * "kCADC_DualConverterWorkAsTriggeredParallel" is used in this case. The two short conversion sequence would be
     * executed by each converter at the same time. Both converter shares the converter A control logic, including the
     * trigger input.
     */
    /*
     * cadcConfigStruct.dualConverterScanMode = kCADC_DualConverterWorkAsTriggeredParallel;
     * cadcConfigStruct.enableSimultaneousMode = true;
     * cadcConfigStruct.DMATriggerSoruce = kCADC_DMATriggerSourceAsEndOfScan;
     * cadcConfigStruct.idleWorkMode = kCADC_IdleKeepNormal;
     * cadcConfigStruct.powerUpDelay = 26U;
     */
    CADC_GetDefaultConfig(&cadcConfigStruct);
    CADC_Init(DEMO_CADC_BASEADDR, &cadcConfigStruct);

    /* Configure each converter. */
    CADC_GetDefaultConverterConfig(&cadcConverterConfigStruct);
    CADC_SetConverterConfig(DEMO_CADC_BASEADDR, kCADC_ConverterA | kCADC_ConverterB, &cadcConverterConfigStruct);
    /* Enable the power for each converter. */
    CADC_EnableConverterPower(DEMO_CADC_BASEADDR, kCADC_ConverterA | kCADC_ConverterB, true);
    while ((kCADC_ConverterAPowerDownFlag | kCADC_ConverterBPowerDownFlag) ==
           ((kCADC_ConverterAPowerDownFlag | kCADC_ConverterBPowerDownFlag) & CADC_GetStatusFlags(ADC)))
    {
    }
    /* Open the gate for each converter. */
    CADC_EnableConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA | kCADC_ConverterB, true);

    /* Configure the samples. */
    cadcSampleConfigStruct.channelGain      = kCADC_ChannelGainx1;
    cadcSampleConfigStruct.zeroCrossingMode = kCADC_ZeroCorssingDisabled;
    cadcSampleConfigStruct.highLimitValue   = 0xFFFFU;
    cadcSampleConfigStruct.lowLimitValue    = 0x0U;
    cadcSampleConfigStruct.offsetValue      = 0x0U;
    cadcSampleConfigStruct.enableWaitSync   = false;

    /* For converter A. */
    cadcSampleConfigStruct.channelNumber          = DEMO_CADC_CHANNEL1_NUMBER;
    cadcSampleConfigStruct.enableDifferentialPair = DEMO_CADC_CHANNEL1_ENABLE_DIFF;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 0U, &cadcSampleConfigStruct);
    cadcSampleConfigStruct.channelNumber          = DEMO_CADC_CHANNEL2_NUMBER;
    cadcSampleConfigStruct.enableDifferentialPair = DEMO_CADC_CHANNEL2_ENABLE_DIFF;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 1U, &cadcSampleConfigStruct);

    /* For converter B. */
    cadcSampleConfigStruct.channelNumber          = DEMO_CADC_CHANNEL3_NUMBER;
    cadcSampleConfigStruct.enableDifferentialPair = DEMO_CADC_CHANNEL3_ENABLE_DIFF;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 8U, &cadcSampleConfigStruct);
    cadcSampleConfigStruct.channelNumber          = DEMO_CADC_CHANNEL4_NUMBER;
    cadcSampleConfigStruct.enableDifferentialPair = DEMO_CADC_CHANNEL4_ENABLE_DIFF;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 9U, &cadcSampleConfigStruct);

    /* Enable the sample slot. */
    sampleMask = CADC_SAMPLE_MASK(0U)    /* For converter A. */
                 | CADC_SAMPLE_MASK(1U)  /* For converter A. */
                 | CADC_SAMPLE_MASK(8U)  /* For converter B. */
                 | CADC_SAMPLE_MASK(9U); /* For converter B. */
    CADC_EnableSample(DEMO_CADC_BASEADDR, sampleMask, true);
    CADC_EnableSample(DEMO_CADC_BASEADDR, (uint16_t)(~sampleMask), false); /* Disable other sample slot. */

    /* The conversion sequence for converter A includes sample 0 and sample 1, while the sequence for converter B
     * includes sample 8 and 9. */

    PRINTF("Press any key to trigger the conversion ...\r\n");
    PRINTF("\r\nSample 0\tSample 1\tSample 8\tSample 9\r\n");
    while (1)
    {
        PRINTF("\r\n");

        /* Trigger the converter.
         * Trigger converter A would execute both converter's conversion when in
         * "kCADC_DualConverterWorkAsTriggeredParallel" work mode. */
        GETCHAR();
        CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA);

        /* Wait the conversion to be done. */
        while (kCADC_ConverterAEndOfScanFlag !=
               (kCADC_ConverterAEndOfScanFlag & CADC_GetStatusFlags(DEMO_CADC_BASEADDR)))
        {
        }

        /* Read the result value. */
        if (sampleMask == (sampleMask & CADC_GetSampleReadyStatusFlags(DEMO_CADC_BASEADDR)))
        {
            PRINTF("%d\t\t", (int16_t)CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, 0U));
            PRINTF("%d\t\t", (int16_t)CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, 1U));
            PRINTF("%d\t\t", (int16_t)CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, 8U));
            PRINTF("%d", (int16_t)CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, 9U));
        }
        CADC_ClearStatusFlags(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);
    }
}
