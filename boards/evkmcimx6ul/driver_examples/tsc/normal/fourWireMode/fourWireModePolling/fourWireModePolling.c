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
void example_TSCIRQn(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * The example indicate how to make TSC, ADC  and touchscreen work together in polling(auto-measure) mode.
 * Once touchscreen is touched, serial terminal will print the information about x/y-coordinate.
 */
int main(void)
{
    tsc_config_t k_tscConfig;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    PRINTF("TSC fourWireModePolling Example Start!\r\n");

    ADC_Configuration();

    TSC_GetDefaultConfig(&k_tscConfig);
    k_tscConfig.enableAutoMeasure = true;
    TSC_Init(DEMO_TSC_BASE, &k_tscConfig);
    TSC_EnableInterruptSignals(DEMO_TSC_BASE, kTSC_ValidSignalEnable);
    PRINTF("Please touch screen.\r\n");

    while (1)
    {
        TSC_SoftwareReset(DEMO_TSC_BASE);
        TSC_StartSenseDetection(DEMO_TSC_BASE);
        while ((TSC_GetStatusFlags(DEMO_TSC_BASE) & kTSC_StateMachineFlag) != kTSC_IdleState)
        {
        }
        if ((TSC_GetInterruptStatusFlags(DEMO_TSC_BASE) & kTSC_ValidSignalFlag) == kTSC_ValidSignalFlag)
        {
            TSC_ClearInterruptStatusFlags(DEMO_TSC_BASE, kTSC_ValidSignalFlag);
            PRINTF("x = %d, y = %d\r\n", TSC_GetMeasureValue(TSC, kTSC_XCoordinateValueSelection),
                   TSC_GetMeasureValue(TSC, kTSC_YCoordinateValueSelection));
        }
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
