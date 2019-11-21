/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_sai.h"
#include "music.h"
#include "fsl_codec_common.h"

#include "fsl_wm8524.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_codec_adapter.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_SAI (I2S3)
#define DEMO_SAI_CLK_FREQ                                                                  \
    (CLOCK_GetPllFreq(kCLOCK_AudioPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootSai3)) / \
     (CLOCK_GetRootPostDivider(kCLOCK_RootSai3)))
#define DEMO_CODEC_WM8524 (1)
#define DEMO_CODEC_BUS_PIN (NULL)
#define DEMO_CODEC_BUS_PIN_NUM (0)
#define DEMO_CODEC_MUTE_PIN (GPIO5)
#define DEMO_CODEC_MUTE_PIN_NUM (21)
/*set Bclk source to Mclk clock*/
#define DEMO_SAI_CLOCK_SOURCE (1U)
#define OVER_SAMPLE_RATE (384U)

#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate16KHz)

#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
#define DEMO_AUDIO_MASTER_CLOCK OVER_SAMPLE_RATE *DEMO_AUDIO_SAMPLE_RATE
#else
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#endif
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (2U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth16bits
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_WM8524_Mute_GPIO(uint32_t output);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static wm8524_config_t wm8524Config = {
    .setMute     = BOARD_WM8524_Mute_GPIO,
    .setProtocol = NULL,
    .protocol    = kWM8524_ProtocolI2S,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8524, .codecDevConfig = &wm8524Config};

sai_handle_t txHandle           = {0};
static volatile bool isFinished = false;
extern codec_config_t boardCodecConfig;
#if (defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)) || \
    (defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER))
sai_master_clock_t mclkConfig = {
#if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    .mclkOutputEnable = true,
#if !(defined(FSL_FEATURE_SAI_HAS_NO_MCR_MICS) && (FSL_FEATURE_SAI_HAS_NO_MCR_MICS))
    .mclkSource = kSAI_MclkSourceSysclk,
#endif
#endif
};
#endif
uint8_t codecHandleBuffer[CODEC_HANDLE_SIZE] = {0U};
codec_handle_t *codecHandle                  = (codec_handle_t *)codecHandleBuffer;
/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_WM8524_Mute_GPIO(uint32_t output)
{
    GPIO_PinWrite(DEMO_CODEC_MUTE_PIN, DEMO_CODEC_MUTE_PIN_NUM, output);
}

static void callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
    isFinished = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    sai_transfer_t xfer;
    uint32_t temp = 0;
    sai_transceiver_t config;

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();

    CLOCK_SetRootMux(kCLOCK_RootSai3, kCLOCK_SaiRootmuxAudioPll1); /* Set SAI source to AUDIO PLL1 786432000HZ*/
    CLOCK_SetRootDivider(kCLOCK_RootSai3, 1U, 32U);                /* Set root clock to 786432000HZ / 32 = 24.576M */
    /* gpio initialization */
    gpio_pin_config_t gpioConfig = {kGPIO_DigitalOutput, 1};
    GPIO_PinInit(DEMO_CODEC_MUTE_PIN, DEMO_CODEC_MUTE_PIN_NUM, &gpioConfig);

    PRINTF("SAI example started!\n\r");

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandle(DEMO_SAI, &txHandle, callback, NULL);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    SAI_TransferTxSetConfig(DEMO_SAI, &txHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
#if (defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)) || \
    (defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER))
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    mclkConfig.mclkHz          = DEMO_AUDIO_MASTER_CLOCK;
    mclkConfig.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;
#endif
    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
#endif

    /* Use default setting to init codec */
    CODEC_Init(codecHandle, &boardCodecConfig);

    /*  xfer structure */
    temp          = (uint32_t)music;
    xfer.data     = (uint8_t *)temp;
    xfer.dataSize = MUSIC_LEN;
    SAI_TransferSendNonBlocking(DEMO_SAI, &txHandle, &xfer);
    /* Wait until finished */
    while (isFinished != true)
    {
    }

    PRINTF("\n\r SAI example finished!\n\r ");
    while (1)
    {
    }
}
