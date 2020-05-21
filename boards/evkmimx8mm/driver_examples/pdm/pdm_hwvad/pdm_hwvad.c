/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_pdm.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PDM PDM
#define DEMO_PDM_CLK_FREQ 24576000U
#define DEMO_PDM_FIFO_WATERMARK (7U)
#define DEMO_PDM_QUALITY_MODE kPDM_QualityModeMedium
#define DEMO_PDM_CIC_OVERSAMPLE_RATE (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_SAMPLE_CLOCK_RATE (1536000U * 2)
#define DEMO_AUDIO_SAMPLE_RATE (48000)
#define PDM_HWVAD_SIGNAL_GAIN (0)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool s_hwvadFlag       = false;
static volatile uint32_t s_readIndex   = 0U;
static volatile uint32_t s_detectTimes = 50U;
static const pdm_config_t pdmConfig    = {
    .enableDoze        = false,
    .fifoWatermark     = DEMO_PDM_FIFO_WATERMARK,
    .qualityMode       = DEMO_PDM_QUALITY_MODE,
    .cicOverSampleRate = DEMO_PDM_CIC_OVERSAMPLE_RATE,
};

static const pdm_channel_config_t channelConfig = {
    .cutOffFreq = kPDM_DcRemoverCutOff21Hz,
    .gain       = kPDM_DfOutputGain1,
};

static const pdm_hwvad_config_t hwavdConfig = {
    .channel           = DEMO_PDM_ENABLE_CHANNEL_LEFT,
    .initializeTime    = 10U,
    .cicOverSampleRate = 0U,
    .inputGain         = 0U,
    .frameTime         = 10U,
    .cutOffFreq        = kPDM_HwvadHpfBypassed,
    .enableFrameEnergy = false,
    .enablePreFilter   = true,
};

static const pdm_hwvad_noise_filter_t noiseFilterConfig = {
    .enableAutoNoiseFilter = false,
    .enableNoiseMin        = true,
    .enableNoiseDecimation = true,
    .noiseFilterAdjustment = 0U,
    .noiseGain             = 7U,
    .enableNoiseDetectOR   = false,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
void PDM_HWVAD_EVENT_IRQHandler(void)
{
    if (PDM_GetHwvadInterruptStatusFlags(DEMO_PDM) & kPDM_HwvadStatusVoiceDetectFlag)
    {
        s_hwvadFlag = true;
        PDM_ClearHwvadInterruptStatusFlags(DEMO_PDM, kPDM_HwvadStatusVoiceDetectFlag);
    }
}

void PDM_HWVAD_ERROR_IRQHandler(void)
{
    PDM_ClearHwvadInterruptStatusFlags(DEMO_PDM, kPDM_HwvadStatusInputSaturation);
}

/*!
 * @brief Main function
 */
int main(void)
{
    volatile uint32_t i = 0U;

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();

    CLOCK_SetRootMux(kCLOCK_RootPdm, kCLOCK_PdmRootmuxAudioPll1); /* Set PDM source from OSC 25MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootPdm, 1U, 32U);                /* Set root clock to 25MHZ */

    PRINTF("PDM hwvad example started!\n\r");

    /* Set up pdm */
    PDM_Init(DEMO_PDM, &pdmConfig);
    PDM_SetChannelConfig(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_LEFT, &channelConfig);
    PDM_SetChannelConfig(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_RIGHT, &channelConfig);
    if (PDM_SetSampleRateConfig(DEMO_PDM, DEMO_PDM_CLK_FREQ, DEMO_AUDIO_SAMPLE_RATE) != kStatus_Success)
    {
        PRINTF("PDM configure sample rate failed.\r\n");
        return -1;
    }

    PDM_SetHwvadConfig(DEMO_PDM, &hwavdConfig);
    PDM_EnableHwvadInterrupts(DEMO_PDM, kPDM_HwvadErrorInterruptEnable | kPDM_HwvadInterruptEnable);
    PDM_ResetHwvad(DEMO_PDM);
    PDM_EnableHwvad(DEMO_PDM, true);

    PDM_SetHwvadSignalFilterConfig(DEMO_PDM, true, PDM_HWVAD_SIGNAL_GAIN);
    PDM_SetHwvadNoiseFilterConfig(DEMO_PDM, &noiseFilterConfig);

    PDM_Reset(DEMO_PDM);
    PDM_Enable(DEMO_PDM, true);

    /* wait HWVAD being initialized flag */
    while (PDM_GetHwvadInitialFlag(DEMO_PDM))
    {
    }

    for (i = 0; i < 3U; i++)
    {
        /* set HWVAD interal filter stauts initial */
        PDM_SetHwvadInternalFilterStatus(DEMO_PDM, kPDM_HwvadInternalFilterInitial);
    }

    /* set HWVAD interal filter normal operation */
    PDM_SetHwvadInternalFilterStatus(DEMO_PDM, kPDM_HwvadInternalFilterNormalOperation);

    NVIC_ClearPendingIRQ(PDM_HWVAD_EVENT_IRQn);
    NVIC_ClearPendingIRQ(PDM_HWVAD_ERROR_IRQn);
    EnableIRQ(PDM_HWVAD_EVENT_IRQn);
    EnableIRQ(PDM_HWVAD_ERROR_IRQn);

    while (s_detectTimes)
    {
        /* wait voice detect */
        while (!s_hwvadFlag)
        {
        }
        s_hwvadFlag = false;
        s_detectTimes--;
        PRINTF("\r\nVoice detected\r\n");
    }

    PDM_Deinit(DEMO_PDM);

    PRINTF("\n\rPDM hwvad example finished!\n\r ");
    while (1)
    {
    }
}
