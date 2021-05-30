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
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_afe.h"

#include "pin_mux.h"
#include "fsl_vref.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_AFE_CHANNEL 0U
/* About 23 mV */
#define DEMO_AFE_VREF_TRIM 46U
#define DEMO_AFE_BASEADDR AFE
#define DEMO_AFE_CH0_IRQ_HANDLER_FUNC AFE_CH0_IRQHandler
#define DEMO_AFE_IRQn AFE_CH0_IRQn

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void APP_VREF_Config(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_bAfeConvDone;   /* Conversion done flag */
volatile int32_t g_result0 = 0; /* Conversion result */

/*******************************************************************************
 * Code
 ******************************************************************************/

void APP_VREF_Config(void)
{
    vref_config_t config;

    /* Get vref default configure */
    /*
     * config.bufferMode = kVREF_ModeHighPowerBuffer;
     * config.bufferMode = kVREF_ModeTightRegulationBuffer;
     * config.enableExternalVoltRef = false;
     * config.enableLowRef = false;
     */
    VREF_GetDefaultConfig(&config);
#if defined(FSL_FEATURE_VREF_HAS_LOW_REFERENCE) && FSL_FEATURE_VREF_HAS_LOW_REFERENCE
    /* Enable low reference volt */
    config.enableLowRef = true;
#endif /* FSL_FEATURE_VREF_HAS_LOW_REFERENCE */
    /* Init vref */
    VREF_Init(VREF, &config);
#if defined(FSL_FEATURE_VREF_HAS_LOW_REFERENCE) && FSL_FEATURE_VREF_HAS_LOW_REFERENCE
    /* Vref set trim reference */
    VREF_SetLowReferenceTrimVal(VREF, 3U);
#endif /* FSL_FEATURE_VREF_HAS_LOW_REFERENCE */
    /* Vref set trim, this value will not be the same with every boards */
    VREF_SetTrimVal(VREF, DEMO_AFE_VREF_TRIM);
}

void DEMO_AFE_CH0_IRQ_HANDLER_FUNC(void)
{
    /* Check conversion complete */
    if ((kAFE_Channel0ConversionCompleteFlag & AFE_GetChannelStatusFlags(DEMO_AFE_BASEADDR)))
    {
        /* Set conversion done flag */
        g_bAfeConvDone = true;
        /* Read result conversion */
        g_result0 = (int32_t)AFE_GetChannelConversionValue(DEMO_AFE_BASEADDR, DEMO_AFE_CHANNEL);
    }
}

int main(void)
{
    afe_config_t afeExampleStruct;
    afe_channel_config_t afeChnExampleStruct;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Select clkSrc for AFEclk */
    CLOCK_SetAfeClkSrc(2U);
    /* Init vref. */
    APP_VREF_Config();
    /* Get AFE config default */
    /*
     * afeExampleStruct.enableLowPower = false;
     * afeExampleStruct.resultFormat = kAFE_ResultFormatRight;
     * afeExampleStruct.clockDivider = kAFE_ClockDivider2;
     * afeExampleStruct.clockSource = kAFE_ClockSource1;
     * afeExampleStruct.startupCount = 2U;
     */
    AFE_GetDefaultConfig(&afeExampleStruct);
    afeExampleStruct.startupCount = 80U; /* startupCnt = (Clk_freq/Clk_div)*20e-6 */

    /* Get channel config default */
    AFE_GetDefaultChannelConfig(&afeChnExampleStruct);
    /* Init AFE module */
    AFE_Init(DEMO_AFE_BASEADDR, &afeExampleStruct);
    /* Configure AFE channel 0 */
    AFE_SetChannelConfig(DEMO_AFE_BASEADDR, DEMO_AFE_CHANNEL, &afeChnExampleStruct);
    /* Enable interrupt */
    AFE_EnableChannelInterrupts(DEMO_AFE_BASEADDR, kAFE_Channel0InterruptEnable);
    /* Disable DMA */
    AFE_EnableChannelDMA(DEMO_AFE_BASEADDR, kAFE_Channel0DMAEnable, false);
    /* Enable NVIC */
    EnableIRQ(DEMO_AFE_IRQn);

    PRINTF("AFE Interrupt example.\r\n");

    while (1)
    {
        /* Wait next triger from user */
        PRINTF("Press any key to trigger AFE conversion\r\n");
        GETCHAR();
        /* Software trigger conversion */
        g_bAfeConvDone = false;

        AFE_DoSoftwareTriggerChannel(DEMO_AFE_BASEADDR, kAFE_Channel0Trigger);
        while (!g_bAfeConvDone)
        {
        }
        /* Print result */
        PRINTF("AFE value  = %d\r\n", g_result0);
    }
}
