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

    SYSMPU_Type *base = SYSMPU;
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Disable SYSMPU. */
    base->CESR &= ~SYSMPU_CESR_VLD_MASK;

    PRINTF("HSADC sequential conversion example.\r\n");

    /* Initialize the HSADC common digital control.
     * "kHSADC_DualConverterWorkAsTriggeredSequential" is used in this case. A long conversion sequence which contails
     * at most 16 sample slots would be executed by both converter. The sequence would be controlled by converter A's
     * control logic such as start, stop, DMA enable, sync input etc.
     */
    HSADC_GetDefaultConfig(&hsadcConfigStruct);
    hsadcConfigStruct.dualConverterScanMode = kHSADC_DualConverterWorkAsTriggeredSequential;
    HSADC_Init(DEMO_HSADC_BASEADDR, &hsadcConfigStruct);

    /* Configure each converter. */
    HSADC_GetDefaultConverterConfig(&hsadcConverterConfigStruct);
    HSADC_SetConverterConfig(DEMO_HSADC_BASEADDR, kHSADC_ConverterA | kHSADC_ConverterB, &hsadcConverterConfigStruct);
    /* Enable the power for each converter. */
    HSADC_EnableConverterPower(DEMO_HSADC_BASEADDR, kHSADC_ConverterA | kHSADC_ConverterB, true);
    while (
        (kHSADC_ConverterAPowerDownFlag | kHSADC_ConverterBPowerDownFlag) ==
        ((kHSADC_ConverterAPowerDownFlag | kHSADC_ConverterBPowerDownFlag) & HSADC_GetStatusFlags(DEMO_HSADC_BASEADDR)))
    {
    }
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
    HSADC_SetSampleConfig(DEMO_HSADC_BASEADDR, 2U, &hsadcSampleConfigStruct);
    hsadcSampleConfigStruct.channelNumber          = (DEMO_HSADC_CONVB_CHN_NUM2 + 8U);
    hsadcSampleConfigStruct.channel67MuxNumber     = DEMO_HSADC_CONVB_CHN67_MUX_NUM2;
    hsadcSampleConfigStruct.enableDifferentialPair = DEMO_HSADC_CONVB_CHN_NUM2_ENABLE_DIFF;
    HSADC_SetSampleConfig(DEMO_HSADC_BASEADDR, 3U, &hsadcSampleConfigStruct);
    /* Enable the sample slot.
     * The conversion sequence for converter A includes sample 0 and sample 1, while the sequence for converter B
     * includes sample 2 and 3. Sample slot 0~15 can reference to any input 0~7 of converter A and converter B in
     * sequential mode.
     */
    sampleMask = HSADC_SAMPLE_MASK(0U)    /* For converter A. */
                 | HSADC_SAMPLE_MASK(1U)  /* For converter A. */
                 | HSADC_SAMPLE_MASK(2U)  /* For converter B. */
                 | HSADC_SAMPLE_MASK(3U); /* For converter B. */
    HSADC_EnableSample(DEMO_HSADC_BASEADDR, sampleMask, true);
    HSADC_EnableSample(DEMO_HSADC_BASEADDR, (uint16_t)(~sampleMask), false); /* Disable other sample slots. */

    /* Calibrate the converter after power up period. */
    HSADC_DoAutoCalibration(DEMO_HSADC_BASEADDR, (kHSADC_ConverterA | kHSADC_ConverterB),
                            (kHSADC_CalibrationModeSingleEnded | kHSADC_CalibrationModeDifferential));
    /* Wait the calibration process complete. Only converter A's End of Scan flag will be set after calibration
    process in sequential mode. */
    while ((kHSADC_ConverterAEndOfScanFlag | kHSADC_ConverterAEndOfCalibrationFlag |
            kHSADC_ConverterBEndOfCalibrationFlag) !=
           ((kHSADC_ConverterAEndOfScanFlag | kHSADC_ConverterAEndOfCalibrationFlag |
             kHSADC_ConverterBEndOfCalibrationFlag) &
            HSADC_GetStatusFlags(DEMO_HSADC_BASEADDR)))
    {
    }
    HSADC_ClearStatusFlags(DEMO_HSADC_BASEADDR, kHSADC_ConverterAEndOfScanFlag | kHSADC_ConverterAEndOfCalibrationFlag |
                                                    kHSADC_ConverterBEndOfCalibrationFlag);

    PRINTF("Press any key to trigger the conversion ...\r\n");
    PRINTF("\r\nSample 0\tSample 1\tSample 2\tSample 3\r\n");
    while (1)
    {
        GETCHAR();
        PRINTF("\r\n");

        /* Trigger the converter.
         * Trigger converter A would execute the long sequential conversion when in
         * "kHSADC_DualConverterWorkAsTriggeredSequential" work mode.
         */
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
            PRINTF("%d\t\t", (int16_t)HSADC_GetSampleResultValue(DEMO_HSADC_BASEADDR, 2U));
            PRINTF("%d", (int16_t)HSADC_GetSampleResultValue(DEMO_HSADC_BASEADDR, 3U));
        }
        HSADC_ClearStatusFlags(DEMO_HSADC_BASEADDR, kHSADC_ConverterAEndOfScanFlag);
    }
}
