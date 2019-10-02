/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_hsadc.h"

#include "pin_mux.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* HSADC base address. */
#define DEMO_HSADC_BASEADDR HSADC0

/* Following macros define the channels which will be sampled in the example.
 * Note that there are 2 kinds of configuration combination:
 * 1) DEMO_HSADC_CONV_CHN_NUM isn't 6 or 7;
 *    DEMO_HSADC_CONV_CHN_NUM must be configured to the value in the range 0~5;
 *    DEMO_HSADC_CONV_CHN67_MUX_NUM won't be functional and can be configured to any value.
 * 2) DEMO_HSADC_CONV_CHN_NUM is 6 or 7;
 *    DEMO_HSADC_CONV_CHN67_MUX_NUM must be configured to the value in the range 0~6.
 * No mater what kind of configuration combination, two adjacent channels will be sampled in differential mode if
 * DEMO_HSADC_CONV_CHN_NUM_ENABLE_DIFF is true.
 */
/* The converter A's channels to sample. */
#define DEMO_HSADC_CONVA_CHN_NUM1 3U /* Configuration combination 1, ADCA_CH2 and ADCA_CH3 */
#define DEMO_HSADC_CONVA_CHN67_MUX_NUM1 0U
#define DEMO_HSADC_CONVA_CHN_NUM1_ENABLE_DIFF true
#define DEMO_HSADC_CONVA_CHN_NUM2 6U /* Configuration combination 2, ADCA_CH6D */
#define DEMO_HSADC_CONVA_CHN67_MUX_NUM2 3U
#define DEMO_HSADC_CONVA_CHN_NUM2_ENABLE_DIFF false
/* The converter B's channels to sample. */
#define DEMO_HSADC_CONVB_CHN_NUM1 6U /* Configuration combination 2,  ADCB_CH6C(PTE2) */
#define DEMO_HSADC_CONVB_CHN67_MUX_NUM1 2U
#define DEMO_HSADC_CONVB_CHN_NUM1_ENABLE_DIFF false
#define DEMO_HSADC_CONVB_CHN_NUM2 7U /* Configuration combination 2, ADCB_CH7F(PTE1) */
#define DEMO_HSADC_CONVB_CHN67_MUX_NUM2 5U
#define DEMO_HSADC_CONVB_CHN_NUM2_ENABLE_DIFF false

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
    hsadc_config_t hsadcConfigStruct;
    hsadc_converter_config_t hsadcConverterConfigStruct;
    hsadc_sample_config_t hsadcSampleConfigStruct;
    uint16_t sampleMask;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("HSADC dual parallel conversion example.\r\n");

    /* Initialize the HSADC common digital control.
     * "kHSADC_DualConverterWorkAsTriggeredParallel" and simultaneous mode is used in this case. The two conversion
     * sequence would be executed by each converter at the same time. Both converter shares the converter A's control
     * logic such as start, stop, DMA enable, sync input etc.
     */
    HSADC_GetDefaultConfig(&hsadcConfigStruct);
    HSADC_Init(DEMO_HSADC_BASEADDR, &hsadcConfigStruct);

    /* Configure each converter. */
    HSADC_GetDefaultConverterConfig(&hsadcConverterConfigStruct);
    /* Enable the calibration in power up period. */
    hsadcConverterConfigStruct.powerUpCalibrationModeMask =
        (kHSADC_CalibrationModeSingleEnded | kHSADC_CalibrationModeDifferential);
    HSADC_SetConverterConfig(DEMO_HSADC_BASEADDR, kHSADC_ConverterA | kHSADC_ConverterB, &hsadcConverterConfigStruct);
    /* Enable the power for each converter. */
    HSADC_EnableConverterPower(DEMO_HSADC_BASEADDR, kHSADC_ConverterA | kHSADC_ConverterB, true);
    while (
        (kHSADC_ConverterAPowerDownFlag | kHSADC_ConverterBPowerDownFlag) ==
        ((kHSADC_ConverterAPowerDownFlag | kHSADC_ConverterBPowerDownFlag) & HSADC_GetStatusFlags(DEMO_HSADC_BASEADDR)))
    {
    }
    /* Wait the calibration process complete. None End of Scan flag will be set after power up calibration process. */
    while ((kHSADC_ConverterAEndOfCalibrationFlag | kHSADC_ConverterBEndOfCalibrationFlag) !=
           ((kHSADC_ConverterAEndOfCalibrationFlag | kHSADC_ConverterBEndOfCalibrationFlag) &
            HSADC_GetStatusFlags(DEMO_HSADC_BASEADDR)))
    {
    }
    HSADC_ClearStatusFlags(DEMO_HSADC_BASEADDR,
                           (kHSADC_ConverterAEndOfCalibrationFlag | kHSADC_ConverterBEndOfCalibrationFlag));
    /* Make each converter exit stop mode. */
    HSADC_EnableConverter(DEMO_HSADC_BASEADDR, kHSADC_ConverterA | kHSADC_ConverterB, true);

    /* Configure the samples. */
    HSADC_GetDefaultSampleConfig(&hsadcSampleConfigStruct);
    /* For converter A. */
    hsadcSampleConfigStruct.channelNumber          = DEMO_HSADC_CONVA_CHN_NUM1;
    hsadcSampleConfigStruct.channel67MuxNumber     = DEMO_HSADC_CONVA_CHN67_MUX_NUM1;
    hsadcSampleConfigStruct.enableDifferentialPair = DEMO_HSADC_CONVA_CHN_NUM1_ENABLE_DIFF;
    HSADC_SetSampleConfig(DEMO_HSADC_BASEADDR, 0U, &hsadcSampleConfigStruct);
    hsadcSampleConfigStruct.channelNumber          = DEMO_HSADC_CONVA_CHN_NUM2;
    hsadcSampleConfigStruct.channel67MuxNumber     = DEMO_HSADC_CONVA_CHN67_MUX_NUM2;
    hsadcSampleConfigStruct.enableDifferentialPair = DEMO_HSADC_CONVA_CHN_NUM2_ENABLE_DIFF;
    HSADC_SetSampleConfig(DEMO_HSADC_BASEADDR, 1U, &hsadcSampleConfigStruct);
    /* For converter B.
     * In HSADC_SetSampleConfig(), the channel number 0~7 represents input 0~7 of converter A and channel number 8~15
     * represents input 0~7 of converter B.
     */
    hsadcSampleConfigStruct.channelNumber          = (DEMO_HSADC_CONVB_CHN_NUM1 + 8U);
    hsadcSampleConfigStruct.channel67MuxNumber     = DEMO_HSADC_CONVB_CHN67_MUX_NUM1;
    hsadcSampleConfigStruct.enableDifferentialPair = DEMO_HSADC_CONVB_CHN_NUM1_ENABLE_DIFF;
    HSADC_SetSampleConfig(DEMO_HSADC_BASEADDR, 8U, &hsadcSampleConfigStruct);
    hsadcSampleConfigStruct.channelNumber          = (DEMO_HSADC_CONVB_CHN_NUM2 + 8U);
    hsadcSampleConfigStruct.channel67MuxNumber     = DEMO_HSADC_CONVB_CHN67_MUX_NUM2;
    hsadcSampleConfigStruct.enableDifferentialPair = DEMO_HSADC_CONVB_CHN_NUM2_ENABLE_DIFF;
    HSADC_SetSampleConfig(DEMO_HSADC_BASEADDR, 9U, &hsadcSampleConfigStruct);
    /* Enable the sample slot.
     * The conversion sequence for converter A includes sample slot 0 and 1, while the sequence for converter B
     * includes sample slot 8 and 9. Sample slot 0~7 can reference only to converter A and sample slot 8~15 can
     * reference only to converter B in parallel mode.
     */
    sampleMask = HSADC_SAMPLE_MASK(0U)    /* For converter A. */
                 | HSADC_SAMPLE_MASK(1U)  /* For converter A. */
                 | HSADC_SAMPLE_MASK(8U)  /* For converter B. */
                 | HSADC_SAMPLE_MASK(9U); /* For converter B. */
    HSADC_EnableSample(DEMO_HSADC_BASEADDR, sampleMask, true);
    HSADC_EnableSample(DEMO_HSADC_BASEADDR, (uint16_t)(~sampleMask), false); /* Disable other sample slots. */

    PRINTF("Press any key to trigger the conversion ...\r\n");
    PRINTF("\r\nSample 0\tSample 1\tSample 8\tSample 9\r\n");
    while (true)
    {
        PRINTF("\r\n");

        /* Trigger the converter.
         * Trigger converter A would execute both converter's conversion when in
         * "kHSADC_DualConverterWorkAsTriggeredParallel" and simultaneous work mode.
         */
        GETCHAR();
        HSADC_DoSoftwareTriggerConverter(DEMO_HSADC_BASEADDR, kHSADC_ConverterA);

        /* Wait the conversion to be done. */
        while (kHSADC_ConverterAEndOfScanFlag !=
               (kHSADC_ConverterAEndOfScanFlag & HSADC_GetStatusFlags(DEMO_HSADC_BASEADDR)))
        {
        }

        /* Read the result value. */
        if (sampleMask == (sampleMask & HSADC_GetSampleReadyStatusFlags(DEMO_HSADC_BASEADDR)))
        {
            PRINTF("%d\t\t", (int16_t)HSADC_GetSampleResultValue(DEMO_HSADC_BASEADDR, 0U));
            PRINTF("%d\t\t", (int16_t)HSADC_GetSampleResultValue(DEMO_HSADC_BASEADDR, 1U));
            PRINTF("%d\t\t", (int16_t)HSADC_GetSampleResultValue(DEMO_HSADC_BASEADDR, 8U));
            PRINTF("%d", (int16_t)HSADC_GetSampleResultValue(DEMO_HSADC_BASEADDR, 9U));
        }
        HSADC_ClearStatusFlags(DEMO_HSADC_BASEADDR, kHSADC_ConverterAEndOfScanFlag);
    }
}
