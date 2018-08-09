/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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
#include "fsl_adc.h"
#include "fsl_adc_etc.h"
#include "fsl_pit.h"
#include "fsl_xbara.h"
#include "fsl_common.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC_BASE ADC1
#define DEMO_ADC_USER_CHANNEL 16U
#define DEMO_ADC_CHANNEL_GROUP0 0U
#define DEMO_ADC_CHANNEL_GROUP1 1U

#define DEMO_ADC_ETC_BASE ADC_ETC
#define DEMO_ADC_ETC_CHAIN_LENGTH 1U /* Chain length is 2. */
#define DEMO_ADC_ETC_CHANNEL0 15U
#define DEMO_ADC_ETC_CHANNEL1 0U
#define EXAMPLE_ADC_ETC_DONE0_Handler ADC_ETC_IRQ0_IRQHandler
#define EXAMPLE_ADC_ETC_DONE1_Handler ADC_ETC_IRQ1_IRQHandler

#define DEMO_XBARA_BASE XBARA1
#define DEMO_XBARA_INPUT_PITCH0 kXBARA1_InputPitTrigger0
#define DEMO_XBARA_OUTPUT_ADC_ETC kXBARA1_OutputAdcEtcXbar0Trig0

#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void ADC_Configuration(void);
void XBARA_Configuration(void);
void PIT_Configuration(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_AdcConversionValue0;
volatile uint32_t g_AdcConversionValue1;
/*******************************************************************************
* Code
******************************************************************************/
void EXAMPLE_ADC_ETC_DONE0_Handler(void)
{
    ADC_ETC_ClearInterruptStatusFlags(DEMO_ADC_ETC_BASE, kADC_ETC_Trg0TriggerSource, kADC_ETC_Done0StatusFlagMask);
    g_AdcConversionValue0 = ADC_ETC_GetADCConversionValue(DEMO_ADC_ETC_BASE, 0U, 0U); /* Get trigger0 chain0 result. */
}

void EXAMPLE_ADC_ETC_DONE1_Handler(void)
{
    ADC_ETC_ClearInterruptStatusFlags(DEMO_ADC_ETC_BASE, kADC_ETC_Trg0TriggerSource, kADC_ETC_Done1StatusFlagMask);
    g_AdcConversionValue1 = ADC_ETC_GetADCConversionValue(DEMO_ADC_ETC_BASE, 0U, 1U); /* Get trigger0 chain1 result. */
}

/*!
 * @brief Main function.
 */
int main(void)
{
    adc_etc_config_t adcEtcConfig;
    adc_etc_trigger_config_t adcEtcTriggerConfig;
    adc_etc_trigger_chain_config_t adcEtcTriggerChainConfig;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set PERCLK_CLK source to OSC_CLK*/
    CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
    /* Set PERCLK_CLK divider to 1 */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);

    PRINTF("ADC_ETC_Hardware_Trigger_Conv Example Start!\r\n");

    ADC_Configuration();
    XBARA_Configuration();
    PIT_Configuration();

    /* Initialize the ADC_ETC. */
    ADC_ETC_GetDefaultConfig(&adcEtcConfig);
    adcEtcConfig.XBARtriggerMask = 1U; /* Enable the external XBAR trigger0. */
    ADC_ETC_Init(DEMO_ADC_ETC_BASE, &adcEtcConfig);

    /* Set the external XBAR trigger0 configuration. */
    adcEtcTriggerConfig.enableSyncMode = false;
    adcEtcTriggerConfig.enableSWTriggerMode = false;
    adcEtcTriggerConfig.triggerChainLength = DEMO_ADC_ETC_CHAIN_LENGTH; /* Chain length is 2. */
    adcEtcTriggerConfig.triggerPriority = 0U;
    adcEtcTriggerConfig.sampleIntervalDelay = 0U;
    adcEtcTriggerConfig.initialDelay = 0U;
    ADC_ETC_SetTriggerConfig(DEMO_ADC_ETC_BASE, 0U, &adcEtcTriggerConfig);

    /* Set the external XBAR trigger0 chain configuration. */
    adcEtcTriggerChainConfig.enableB2BMode = true;
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U
                                                   << DEMO_ADC_CHANNEL_GROUP0; /* Select ADC_HC0 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect =
        DEMO_ADC_ETC_CHANNEL0; /* ADC_HC0 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable; /* Enable the Done0 interrupt. */
    ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, 0U, 0U,
                                  &adcEtcTriggerChainConfig); /* Configure the trigger0 chain0. */
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U
                                                   << DEMO_ADC_CHANNEL_GROUP1; /* Select ADC_HC1 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect =
        DEMO_ADC_ETC_CHANNEL1; /* ADC_HC1 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done1InterruptEnable; /* Enable the Done1 interrupt. */
    ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, 0U, 1U,
                                  &adcEtcTriggerChainConfig); /* Configure the trigger0 chain1. */

    /* Enable the NVIC. */
    EnableIRQ(ADC_ETC_IRQ0_IRQn);
    EnableIRQ(ADC_ETC_IRQ1_IRQn);

    /* Start PIT channel0. */
    PIT_StartTimer(PIT, kPIT_Chnl_0);

    PRINTF("Please press any key to get user channel's ADC value.\r\n");

    while (1)
    {
        GETCHAR();
        PRINTF("ADC conversion vaule is %d and %d\r\n", g_AdcConversionValue0, g_AdcConversionValue1);
    }
}

/*!
* @brief Configure ADC to working with ADC_ETC.
*/
void ADC_Configuration(void)
{
    adc_config_t k_adcConfig;
    adc_channel_config_t adcChannelConfigStruct;

    /* Initialize the ADC module. */
    ADC_GetDefaultConfig(&k_adcConfig);
    ADC_Init(DEMO_ADC_BASE, &k_adcConfig);
    ADC_EnableHardwareTrigger(DEMO_ADC_BASE, true);

    adcChannelConfigStruct.channelNumber = DEMO_ADC_USER_CHANNEL; /* External channel selection from ADC_ETC. */
    adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    ADC_SetChannelConfig(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP0, &adcChannelConfigStruct);
    ADC_SetChannelConfig(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP1, &adcChannelConfigStruct);

    /* Do auto hardware calibration. */
    if (kStatus_Success == ADC_DoAutoCalibration(DEMO_ADC_BASE))
    {
        PRINTF("ADC_DoAntoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC_DoAutoCalibration() Failed.\r\n");
    }
}

/*!
* @brief Configure XBARA to work with ADC_ETC.
*/
void XBARA_Configuration(void)
{
    /* Init xbara module. */
    XBARA_Init(DEMO_XBARA_BASE);

    /* Configure the XBARA signal connections. */
    XBARA_SetSignalsConnection(DEMO_XBARA_BASE, DEMO_XBARA_INPUT_PITCH0, DEMO_XBARA_OUTPUT_ADC_ETC);
}

/*!
* @brief Configuration PIT to trigger ADC_ETC.
*/
void PIT_Configuration(void)
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;

    /* Init pit module */
    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000000U, PIT_SOURCE_CLOCK));
}
