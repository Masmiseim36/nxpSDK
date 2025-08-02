/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_vref.h"
#include "fsl_lpadc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void LPADC_Configuration(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if defined(DEMO_LPADC_RESOLUTION_MODE) && (DEMO_LPADC_RESOLUTION_MODE == kLPADC_ConversionResolutionHigh)
const uint32_t g_Lpadc_FullRange   = 65536UL;
const uint32_t g_Lpadc_ResultShift = 0UL;
#else
const uint32_t g_Lpadc_FullRange   = 4096UL;
const uint32_t g_Lpadc_ResultShift = 3UL;
#endif /* FSL_FEATURE_LPADC_HAS_CMDL_MODE */

/*******************************************************************************
 * Code
 ******************************************************************************/

static void LPADC_Configuration(void)
{
    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;

    LPADC_GetDefaultConfig(&mLpadcConfigStruct);
    mLpadcConfigStruct.enableAnalogPreliminary = true;
#if defined(DEMO_LPADC_VREF_SOURCE)
    mLpadcConfigStruct.referenceVoltageSource = DEMO_LPADC_VREF_SOURCE;
#endif /* DEMO_LPADC_VREF_SOURCE */
#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS) && FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS
    mLpadcConfigStruct.conversionAverageMode = kLPADC_ConversionAverage1024;
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS */
    LPADC_Init(DEMO_LPADC_BASE, &mLpadcConfigStruct);

    /* Request LPADC calibration, offset calibration and aoto calibration. */
#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFSMODE) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFSMODE
    LPADC_SetOffsetCalibrationMode(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_CALIBRATION_MODE);
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFSMODE */

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFS) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFS
#if defined(DEMO_LPADC_DO_OFFSET_CALIBRATION) && DEMO_LPADC_DO_OFFSET_CALIBRATION
    LPADC_DoOffsetCalibration(DEMO_LPADC_BASE); /* Request offset calibration, automatic update OFSTRIM register. */
#else   /* Update OFSTRIM register manually. */

#if defined(FSL_FEATURE_LPADC_HAS_OFSTRIM) && FSL_FEATURE_LPADC_HAS_OFSTRIM
#if defined(FSL_FEATURE_LPADC_OFSTRIM_COUNT) && (FSL_FEATURE_LPADC_OFSTRIM_COUNT == 2U)
    LPADC_SetOffsetValue(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_VALUE_A, DEMO_LPADC_OFFSET_VALUE_B);
#elif defined(FSL_FEATURE_LPADC_OFSTRIM_COUNT) && (FSL_FEATURE_LPADC_OFSTRIM_COUNT == 1U)
    LPADC_SetOffsetValue(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_VALUE);
#endif /* FSL_FEATURE_LPADC_OFSTRIM_COUNT */

#else  /* For other OFSTRIM register type. */
    if (DEMO_LPADC_OFFSET_CALIBRATION_MODE == kLPADC_OffsetCalibration12bitMode)
    {
        LPADC_SetOffset12BitValue(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_VALUE_A, DEMO_LPADC_OFFSET_VALUE_B);
    }
    else
    {
        LPADC_SetOffset16BitValue(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_VALUE_A, DEMO_LPADC_OFFSET_VALUE_B);
    }
#endif /* FSL_FEATURE_LPADC_HAS_OFSTRIM */

#endif /* DEMO_LPADC_DO_OFFSET_CALIBRATION */
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFS */

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ) && FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ
    /* Request auto calibration (including gain error calibration and linearity error calibration). */
    LPADC_DoAutoCalibration(DEMO_LPADC_BASE);
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ */

#if (defined(FSL_FEATURE_LPADC_HAS_CFG_CALOFS) && FSL_FEATURE_LPADC_HAS_CFG_CALOFS)
    /* Do auto calibration. */
    LPADC_DoAutoCalibration(DEMO_LPADC_BASE);
#endif /* FSL_FEATURE_LPADC_HAS_CFG_CALOFS */

    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.channelNumber = DEMO_LPADC_USER_CHANNEL;
    mLpadcCommandConfigStruct.hardwareAverageMode = kLPADC_HardwareAverageCount1024;
    mLpadcCommandConfigStruct.sampleTimeMode = kLPADC_SampleTimeADCK131;
#if defined(FSL_FEATURE_LPADC_HAS_CMDL_MODE) && FSL_FEATURE_LPADC_HAS_CMDL_MODE
    mLpadcCommandConfigStruct.conversionResolutionMode = DEMO_LPADC_RESOLUTION_MODE;
#endif /* FSL_FEATURE_LPADC_HAS_CMDL_MODE */
    LPADC_SetConvCommandConfig(DEMO_LPADC_BASE, DEMO_LPADC_USER_CMDID, &mLpadcCommandConfigStruct);

    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = DEMO_LPADC_USER_CMDID;
    LPADC_SetConvTriggerConfig(DEMO_LPADC_BASE, 0U, &mLpadcTriggerConfigStruct);
} 

/*!
 * @brief Main function
 */
int main(void)
{
    vref_config_t vrefConfig;
    lpadc_conv_result_t mLpadcResultConfigStruct;
    uint16_t adcValue = 0U;
    uint8_t defaultTrimVal = 0U;
    float defaultVoltage = 0.0F;
    uint8_t targetTrimVal = 0U;
    float actualVoltage = 0.0F;

    BOARD_InitHardware();
    PRINTF("\r\n vref lpadc example\r\n");

    /* Configure LPADC */
    LPADC_Configuration();

    /* Initialize the VREF mode. */
    VREF_GetDefaultConfig(&vrefConfig);
    VREF_Init(DEMO_VREF_BASE, &vrefConfig);

    defaultTrimVal = VREF_GetVrefTrimVal(DEMO_VREF_BASE); /* The default trim should between [0, 63], */
    PRINTF("\r\n vref default trim value is: %d", defaultTrimVal);

    LPADC_DoSoftwareTrigger(DEMO_LPADC_BASE, 1U);
#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2U))
    while (!LPADC_GetConvResult(DEMO_LPADC_BASE, &mLpadcResultConfigStruct, 0U))
#else
    while (!LPADC_GetConvResult(DEMO_LPADC_BASE, &mLpadcResultConfigStruct))
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */
    {
    }

    adcValue = mLpadcResultConfigStruct.convValue >> g_Lpadc_ResultShift;
    defaultVoltage  = DEMO_LPADC_REFERENCE_VOLTAGE * (float)adcValue / (float)g_Lpadc_FullRange;

    PRINTF("\r\n vref default output voltage is %.3fmv \r\n", (double)defaultVoltage);


    if (defaultTrimVal > 30U)
    {
        targetTrimVal = defaultTrimVal - 30U;
    }
    else
    {
        targetTrimVal = defaultTrimVal + 30U;
    }

    /* Reset trim value and calcute the difference between theoretical and actual values. */
    VREF_SetVrefTrimVal(DEMO_VREF_BASE, targetTrimVal);
    PRINTF("\r\n current setting trim value is %d", VREF_GetVrefTrimVal(DEMO_VREF_BASE));
    
    LPADC_DoSoftwareTrigger(DEMO_LPADC_BASE, 1U);
#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2U))
    while (!LPADC_GetConvResult(DEMO_LPADC_BASE, &mLpadcResultConfigStruct, 0U))
#else
    while (!LPADC_GetConvResult(DEMO_LPADC_BASE, &mLpadcResultConfigStruct))
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */
    {
    }

    adcValue = mLpadcResultConfigStruct.convValue >> g_Lpadc_ResultShift;
    actualVoltage  = DEMO_LPADC_REFERENCE_VOLTAGE * (float)adcValue / (float)g_Lpadc_FullRange;

    if (defaultTrimVal > 30U)
    {
        PRINTF("\r\n current theoretical voltage is %.3fmv", (double)(defaultVoltage - 20.0F));
    }
    else
    {
        PRINTF("\r\n current theoretical voltage is %.3fmv", (double)(defaultVoltage + 20.0F));
    }

    PRINTF("\r\n current actual voltage is %.3fmv \r\n", (double)actualVoltage);

    while (1)
    {
    }
}
