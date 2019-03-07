/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
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

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_adc_5hc.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC_5HC_BASE ADC_5HC
#define DEMO_ADC_5HC_USER_CHANNEL 0U
#define DEMO_ADC_5HC_CHANNEL_GROUP 0U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void Hello(void)
{
    PRINTF("HELLO\r\n");
    TSC->INT_STATUS |= TSC_INT_STATUS_IDLE_SW_MASK;
}
/*!
 * @brief Main function
 */
int main(void)
{
    adc_5hc_config_t adcConfigStrcut;
    adc_5hc_channel_config_t adcChannelConfigStruct;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    PRINTF("\r\nADC_5HC polling Example.\r\n");

    /*
    *  config->enableAsynchronousClockOutput = true;
     *  config->enableOverWrite =               false;
       *  config->enableContinuousConversion =    false;
       *  config->enableHighSpeed =               false;
       *  config->enableLowPower =                false;
       *  config->enableLongSample =              false;
       *  config->referenceVoltageSource =        kADC_5HC_ReferenceVoltageSourceVref;
       *  config->samplePeriodMode =              kADC_5HC_SamplePeriod2or12Clocks;
       *  config->clockSource =                   kADC_5HC_ClockSourceAD;
       *  config->clockDriver =                   kADC_5HC_ClockDriver1;
       *  config->resolution =                    kADC_5HC_Resolution12Bit;
       */
    ADC_5HC_GetDefaultConfig(&adcConfigStrcut);
    ADC_5HC_Init(DEMO_ADC_5HC_BASE, &adcConfigStrcut);
    ADC_5HC_EnableHardwareTrigger(DEMO_ADC_5HC_BASE, false);

    /* Do auto hardware calibration. */
    if (kStatus_Success == ADC_5HC_DoAutoCalibration(DEMO_ADC_5HC_BASE))
    {
        PRINTF("ADC_5HC_DoAntoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC_5HC_DoAutoCalibration() Failed.\r\n");
    }

    /* Configure the user channel and interrupt. */
    adcChannelConfigStruct.channelNumber = DEMO_ADC_5HC_USER_CHANNEL;
    adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;

    while (1)
    {
        PRINTF("Press any key to get user channel's ADC_5HC value.\r\n");
        GETCHAR();
        /*
         When in software trigger mode, each conversion would be launched once calling the "ADC_5HC_ChannelConfigure()"
         function, which works like writing a conversion command and executing it. For another channel's conversion,
         just to change the "channelNumber" field in channel's configuration structure, and call the
         "ADC_5HC_ChannelConfigure() again.
        */
        ADC_5HC_SetChannelConfig(DEMO_ADC_5HC_BASE, DEMO_ADC_5HC_CHANNEL_GROUP, &adcChannelConfigStruct);
        while (0U == ADC_5HC_GetChannelStatusFlags(DEMO_ADC_5HC_BASE, DEMO_ADC_5HC_CHANNEL_GROUP))
        {
        }
        PRINTF("ADC_5HC Value: %d\r\n",
               ADC_5HC_GetChannelConversionValue(DEMO_ADC_5HC_BASE, DEMO_ADC_5HC_CHANNEL_GROUP));
    }
}
