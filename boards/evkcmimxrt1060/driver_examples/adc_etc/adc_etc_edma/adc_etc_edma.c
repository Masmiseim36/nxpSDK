/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_adc.h"
#include "fsl_adc_etc.h"
#include "fsl_pit.h"
#include "fsl_xbara.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "app.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool transferDoneFlag = false;
volatile uint32_t dmaCounter = 0U;
edma_handle_t edmaHandle = {0};
AT_NONCACHEABLE_SECTION_INIT(uint32_t destAddr) = 0U;
const uint32_t adcFullRange     = 4096U;
const float adcRefVoltage    = 3.3F;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief User callback function for EDMA transfer.
 */ 
void Demo_EdmaCallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        transferDoneFlag = true;
    }
}

/*!
 * @brief Configure ADC.
 */
static void DEMO_AdcConfiguration(void)
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

    /* Do auto hardware calibration. */
    if (kStatus_Success == ADC_DoAutoCalibration(DEMO_ADC_BASE))
    {
        PRINTF("\r\n Adc AutoCalibration Succeed.");
    }
    else
    {
        PRINTF("\r\n Adc AutoCalibration Failed.");
    }
}

/*!
 * @brief Configure XBARA.
 */
static void DEMO_XbaraConfiguration(void)
{
    /* Init xbara module. */
    XBARA_Init(DEMO_XBARA_BASE);

    /* Configure the XBARA signal connections. */
    XBARA_SetSignalsConnection(DEMO_XBARA_BASE, DEMO_XBARA_INPUT_PITCH0, DEMO_XBARA_OUTPUT_ADC_ETC);
}

/*!
 * @brief Configure PIT.
 */
static void DEMO_PitConfiguration(void)
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;

    /* Init pit module */
    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000000U, DEMO_PIT_SOURCE_CLOCK));
}

/*!
 * @brief Configure ADC_ETC.
 */
static void DEMO_AdcEtcConfiguration(void)
{
    adc_etc_config_t adcEtcConfig;
    adc_etc_trigger_config_t adcEtcTriggerConfig;
    adc_etc_trigger_chain_config_t adcEtcTriggerChainConfig;

    /* Initialize the ADC_ETC. */
    ADC_ETC_GetDefaultConfig(&adcEtcConfig);
    adcEtcConfig.XBARtriggerMask = 1U; /* Enable the external XBAR trigger0. */
    ADC_ETC_Init(DEMO_ADC_ETC_BASE, &adcEtcConfig);

    /* Set the external XBAR trigger0 configuration. */
    adcEtcTriggerConfig.enableSyncMode      = false;
    adcEtcTriggerConfig.enableSWTriggerMode = false;
    adcEtcTriggerConfig.triggerChainLength  = 0U;
    adcEtcTriggerConfig.triggerPriority     = 0U;
    adcEtcTriggerConfig.sampleIntervalDelay = 0U;
    adcEtcTriggerConfig.initialDelay        = 0U;
    ADC_ETC_SetTriggerConfig(DEMO_ADC_ETC_BASE, 0U, &adcEtcTriggerConfig);

    /* Set the external XBAR trigger0 chain configuration. */
    adcEtcTriggerChainConfig.enableB2BMode       = true;
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << DEMO_ADC_CHANNEL_GROUP0; /* Select ADC_HC0 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNEL0; /* ADC_HC0 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable; /* Enable the Done0 interrupt. */
#if defined(FSL_FEATURE_ADC_ETC_HAS_TRIGm_CHAIN_a_b_IEn_EN) && FSL_FEATURE_ADC_ETC_HAS_TRIGm_CHAIN_a_b_IEn_EN
    adcEtcTriggerChainConfig.enableIrq = true; /* Enable the IRQ. */
#endif  /* FSL_FEATURE_ADC_ETC_HAS_TRIGm_CHAIN_a_b_IEn_EN */
    ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, 0U, 0U, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain0. */

    ADC_ETC_EnableDMA(DEMO_ADC_ETC_BASE, 0U);
}

/*!
 * @brief Configure EDMA.
 */
static void DEMO_EdmaConfiguration(void)
{
    edma_config_t edmaConfig = {0};
    edma_transfer_config_t transferConfig;

    /* Configure DMAMUX. */
    DMAMUX_Init(DMAMUX);
    DMAMUX_SetSource(DMAMUX, DEMO_EDMA_CHANNEL, kDmaRequestMuxADC_ETC);
    DMAMUX_EnableChannel(DMAMUX, DEMO_EDMA_CHANNEL);

    /* Configure EDMA. */
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(DEMO_EDMA_INSTANCE, &edmaConfig);

    EDMA_CreateHandle(&edmaHandle, DEMO_EDMA_INSTANCE, DEMO_EDMA_CHANNEL);
    EDMA_SetCallback(&edmaHandle, Demo_EdmaCallback, NULL);

    void *srcAddr = ((uint32_t *)&(DEMO_ADC_ETC_BASE->TRIG[0U].TRIGn_RESULT_1_0));
    EDMA_PrepareTransferConfig(&transferConfig, srcAddr, sizeof(srcAddr), 0,
                               &destAddr, sizeof(&destAddr), 0, sizeof(&destAddr), (sizeof(&destAddr) * 1U));
    EDMA_SubmitTransfer(&edmaHandle, &transferConfig);
    EDMA_StartTransfer(&edmaHandle);
}

/*!
 * @brief Main function.
 */
int main(void)
{
    BOARD_InitHardware();

    DEMO_AdcConfiguration();
    DEMO_XbaraConfiguration();
    DEMO_PitConfiguration();
    DEMO_AdcEtcConfiguration();
    DEMO_EdmaConfiguration();

    PIT_StartTimer(PIT, kPIT_Chnl_0);
    PRINTF("\r\n Adc Edma Transfer Example Start.");

    while (1)
    {
        PRINTF("\r\n Press any key to print output adc current convert value and voltage.");
        GETCHAR();

        while (transferDoneFlag != true)
        {
        }

        PRINTF("\r\n ADC current convert value = %d, voltage = %.3f", (destAddr & 0xFFFU), (double)((float)(destAddr & 0xFFFU) / (float)adcFullRange * adcRefVoltage));

        transferDoneFlag = false;
        EDMA_StartTransfer(&edmaHandle);
    }
}
