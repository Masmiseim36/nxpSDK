/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_tsc.h"
#include "fsl_adc_5hc.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC_BASE ADC_5HC
#define DEMO_TSC_BASE TSC

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void ADC_Configuration(void);
void PortPreCharge(void);
void PortXMeasure(void);
void PortYMeasure(void);
void PortTouchDetect(void);
void PortIntermediate(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * The example indicate how to make TSC, ADC  and touchscreen work together in debug mode.
 * In the debug mode, all TSC outputs will be controlled by software. Software can also observe
 * all TSC inputs through debug interface.
 * This example simulates non-debug(hardware) mode. It has six state: 1st-pre-charge, 1st-detect,
 * x-measure, y-measure, and 2nd-pre-charge, 2nd-detect.
 * Once touchscreen is touched, serial terminal will print the information about x/y-coordinate.
 */
int main(void)
{
    uint32_t i, k_XMeasureValue, k_YMeasureValue;
    tsc_config_t k_tscConfig;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    PRINTF("TSC DebugMode Example Start!\r\n");

    ADC_Configuration();

    TSC_GetDefaultConfig(&k_tscConfig);
    TSC_Init(DEMO_TSC_BASE, &k_tscConfig);
    TSC_EnableDebugMode(DEMO_TSC_BASE, true);

    PRINTF("Please touch screen!\r\n");

    while (1)
    {
        /* 1st Pre-charge stage. */
        PortPreCharge(); /* Set correct port mode to pre-charge. */
                         /* Wait enough time for the lower screen layer to achieve even-potential status.
                            Note: Using timer to delay could get more accurate results. */
        for (i = 0; i < 0xFFFFU; ++i)
        {
            __ASM("NOP");
        }
        PortIntermediate();

        /* 1st Screen detection stage. */
        PortTouchDetect(); /* Set correct port mode to detect. */
        TSC_DebugEnableDetection(DEMO_TSC_BASE, kTSC_Detection4WireMode, true);
        while (kTSC_DetectFourWireFlag != (kTSC_DetectFourWireFlag & TSC_GetStatusFlags(DEMO_TSC_BASE)))
        {
        }
        TSC_DebugEnableDetection(DEMO_TSC_BASE, kTSC_Detection4WireMode, false);
        PortIntermediate();

        /* X-measure stage. */
        PortXMeasure(); /* Set correct port mode to measure X-axis. */
        /* Before X-axis or Y-axis measurement, the screen need some time before even potential distribution ready.
           Note: Using timer to delay could get more accurate results. */
        for (i = 0; i < 0xFFFFU; ++i)
        {
            __ASM("NOP");
        }
        /* Hardware tirgger to ADC_HC3. */
        TSC_DebugTriggerSignalToADC(DEMO_TSC_BASE, kTSC_TriggerToChannel3, true);
        TSC_DebugTriggerSignalToADC(DEMO_TSC_BASE, kTSC_TriggerToChannel3, false);
        while (kTSC_ADCCOCOSignalFlag != (kTSC_ADCCOCOSignalFlag & TSC_GetADCStatusFlags(DEMO_TSC_BASE)))
        {
        }
        /* Clear ADC COCO flag. */
        TSC_DebugClearSignalToADC(DEMO_TSC_BASE, true);
        TSC_DebugClearSignalToADC(DEMO_TSC_BASE, false);
        k_XMeasureValue = kTSC_ADCConversionValueFlag & TSC_GetADCStatusFlags(DEMO_TSC_BASE);
        PortIntermediate();

        /* Y-measure stage. */
        PortYMeasure(); /* Set correct port mode to measure Y-axis. */
        /* Before X-axis or Y-axis measurement, the screen need some time before even potential distribution ready.
           Note: Using timer to delay could get more accurate results. */
        for (i = 0; i < 0xFFFFU; ++i)
        {
            __ASM("NOP");
        }
        /* Hardware tirgger to ADC_HC1. */
        TSC_DebugTriggerSignalToADC(DEMO_TSC_BASE, kTSC_TriggerToChannel1, true);
        TSC_DebugTriggerSignalToADC(DEMO_TSC_BASE, kTSC_TriggerToChannel1, false);
        while (kTSC_ADCCOCOSignalFlag != (kTSC_ADCCOCOSignalFlag & TSC_GetADCStatusFlags(DEMO_TSC_BASE)))
        {
        }
        /* Clear ADC COCO flag. */
        TSC_DebugClearSignalToADC(DEMO_TSC_BASE, true);
        TSC_DebugClearSignalToADC(DEMO_TSC_BASE, false);
        k_YMeasureValue = kTSC_ADCConversionValueFlag & TSC_GetADCStatusFlags(DEMO_TSC_BASE);
        PortIntermediate();

        /* 2nd Pre-charge stage. */
        PortPreCharge(); /* Set correct port mode to pre-charge. */
                         /* Wait enough time for the lower screen layer to achieve even-potential status.
                            Note: Using timer to delay could get more accurate results. */
        for (i = 0; i < 0xFFFFU; ++i)
        {
            __ASM("NOP");
        }
        PortIntermediate();

        /* 2nd Screen detectiopn stage. */
        PortTouchDetect(); /* Set correct port mode to detect. */
        TSC_DebugEnableDetection(DEMO_TSC_BASE, kTSC_Detection4WireMode, true);
        /* Wait enough time to read four wire detect status from analogue.
           Note: Using timer to delay could get more accurate results. */
        for (i = 0; i < 0xFFFFU; ++i)
        {
            __ASM("NOP");
        }
        /* If no touch has been detected, then previous measured coordinates¡¯ value is invalid. */
        if (kTSC_DetectFourWireFlag == (kTSC_DetectFourWireFlag & TSC_GetStatusFlags(DEMO_TSC_BASE)))
        {
            TSC_DebugEnableDetection(DEMO_TSC_BASE, kTSC_Detection4WireMode, false);
            PRINTF("x = %d, y = %d\r\n", k_XMeasureValue, k_YMeasureValue);
        }
        PortIntermediate();
    }
}

/*!
* @brief Set configuration of ADC working with TSC.
*/
void ADC_Configuration(void)
{
    adc_5hc_config_t k_adcConfig;
    adc_5hc_channel_config_t k_adcChannelConfig;

    /* Initialize the ADC module. */
    ADC_5HC_GetDefaultConfig(&k_adcConfig);
    ADC_5HC_Init(DEMO_ADC_BASE, &k_adcConfig);
    ADC_5HC_EnableHardwareTrigger(DEMO_ADC_BASE, true);

    /* Before TSC starts work, software driver configure ADC_HCx.
     * For four-wire resistive screen, x-coordinate measure triggers to ADC_HC3
     * and y-coordinate measure triggers to ADC_HC1. So we need configure ADC_HC1
     * and ADC_HC3 to make sure TSC could work normally.
     */
    k_adcChannelConfig.channelNumber = 1U; /* Channel1 is ynlr port. */
    k_adcChannelConfig.enableInterruptOnConversionCompleted = false;
    ADC_5HC_SetChannelConfig(DEMO_ADC_BASE, 3U, &k_adcChannelConfig);
    k_adcChannelConfig.channelNumber = 3U; /* Channel3 is xnur port. */
    ADC_5HC_SetChannelConfig(DEMO_ADC_BASE, 1U, &k_adcChannelConfig);

    /* Do auto hardware calibration. */
    if (kStatus_Success == ADC_5HC_DoAutoCalibration(DEMO_ADC_BASE))
    {
        PRINTF("ADC_5HC_DoAntoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC_5HC_DoAutoCalibration() Failed.\r\n");
    }
}

/*!
* @brief Set TSC port mode to work in pre-charge state.
*/
void PortPreCharge(void)
{
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_WiperPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YnlrPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YpllPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XnurPortSource, kTSC_Port200k_PullUpMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XpulPortSource, kTSC_Port200k_PullUpMode);
}

/*!
* @brief Set TSC port mode to work in X-axis measure state.
*/
void PortXMeasure(void)
{
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_WiperPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YnlrPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YpllPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XnurPortSource, kTSC_PortPullDownMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XpulPortSource, kTSC_PortPullUpMode);
}

/*!
* @brief Set TSC port mode to work in Y-axis measure state.
*/
void PortYMeasure(void)
{
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_WiperPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YnlrPortSource, kTSC_PortPullDownMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YpllPortSource, kTSC_PortPullUpMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XnurPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XpulPortSource, kTSC_PortOffMode);
}

/*!
* @brief Set TSC port mode to work in detect state.
*/
void PortTouchDetect(void)
{
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_WiperPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YnlrPortSource, kTSC_PortPullDownMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YpllPortSource, kTSC_PortPullDownMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XnurPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XpulPortSource, kTSC_Port200k_PullUpMode);
}

/*!
* @brief Set TSC port mode to work in intermediate state.
*        Intermediate state exists after pre-charge, detect, x- measure, y-measure, and
*        2nd-pre-charge, 2nd-detect.
*/
void PortIntermediate(void)
{
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_WiperPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YnlrPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YpllPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XnurPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XpulPortSource, kTSC_Port200k_PullUpMode);
}
