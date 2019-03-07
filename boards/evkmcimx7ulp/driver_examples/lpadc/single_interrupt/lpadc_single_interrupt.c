/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_lpadc.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPADC_BASE ADC1
#define DEMO_LPADC_IRQn ADC1_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC ADC1_IRQHandler


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitDebugConsole(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_LpadcConversionCompletedFlag = false;
lpadc_conv_result_t g_LpadcResultConfigStruct;
const uint32_t g_Lpadc_12bitFullRange = 4096U;

/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_LPADC_IRQ_HANDLER_FUNC(void)
{
    if (LPADC_GetConvResult(DEMO_LPADC_BASE, &g_LpadcResultConfigStruct))
    {
        g_LpadcConversionCompletedFlag = true;
    }
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/*!
 * @brief Main function
 */
int main(void)
{
    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set the source for the ADC1 module */
    SCG0->FIRCDIV &= ~SCG_FIRCDIV_FIRCDIV2_MASK;
    SCG0->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(2); /* 24MHZ. */
    CLOCK_SetIpSrc(kCLOCK_Adc1, kCLOCK_IpSrcFircAsync);

    PRINTF("LPADC Interrupt Example\r\n");

    LPADC_GetDefaultConfig(&mLpadcConfigStruct);
    mLpadcConfigStruct.enableAnalogPreliminary = true;
    mLpadcConfigStruct.FIFOWatermark = 0U; /* Set watermark as 0U. */
    LPADC_Init(DEMO_LPADC_BASE, &mLpadcConfigStruct);

    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.channelNumber = 6U;                                 /* Take channel6A as ADC input.  */
    LPADC_SetConvCommandConfig(DEMO_LPADC_BASE, 15U, &mLpadcCommandConfigStruct); /* Configurate the CMD15 buffer. */

    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = 15U; /* CMD15 is executed. */
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(DEMO_LPADC_BASE, 0U, &mLpadcTriggerConfigStruct); /* Configurate the trigger0. */

    /* Enable the watermark interrupt. */
    LPADC_EnableInterrupts(DEMO_LPADC_BASE, kLPADC_FIFOWatermarkInterruptEnable);
    EnableIRQ(DEMO_LPADC_IRQn);

    PRINTF("ADC Full Range: %d\r\n", g_Lpadc_12bitFullRange);
#if defined(FSL_FEATURE_LPADC_HAS_CMDL_CSCALE) && FSL_FEATURE_LPADC_HAS_CMDL_CSCALE
    if (kLPADC_SampleFullScale == mLpadcCommandConfigStruct.sampleScaleMode)
    {
        PRINTF("Full channel scale (Factor of 1).\r\n");
    }
    else if (kLPADC_SamplePartScale == mLpadcCommandConfigStruct.sampleScaleMode)
    {
        PRINTF("Divided input voltage signal. (Factor of 30/64).\r\n");
    }
#endif

    /* When the number of datawords stored in the ADC Result FIFO is greater
    * than watermark value(0U), LPADC watermark interrupt would be triggered.
    */
    PRINTF("Please press any key to get user channel's ADC value.\r\n");
    GETCHAR();
    LPADC_DoSoftwareTrigger(DEMO_LPADC_BASE, 1U);
    while (!g_LpadcConversionCompletedFlag)
    {
    }
    PRINTF("ADC value: %d\r\n", ((g_LpadcResultConfigStruct.convValue) >> 3U));
    while (1)
    {
    }
}
