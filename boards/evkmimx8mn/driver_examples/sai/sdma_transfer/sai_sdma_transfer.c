/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_sai_sdma.h"
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
#define DEMO_IRQn I2S3_IRQn
#define EXAMPLE_DMA SDMAARM2
#define EXAMPLE_CHANNEL (1)
#define SAI_TX_SOURCE (5)
/*set Bclk source to Mclk clock*/
#define DEMO_SAI_CLOCK_SOURCE (1U)
#define SDMA_FREQ_EQUALS_ARM (1U)
#define OVER_SAMPLE_RATE (384U)
/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate16KHz)
/* demo audio master clock */
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
static void callback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static wm8524_config_t wm8524Config = {
    .setMute     = BOARD_WM8524_Mute_GPIO,
    .setProtocol = NULL,
    .protocol    = kWM8524_ProtocolI2S,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8524, .codecDevConfig = &wm8524Config};

AT_NONCACHEABLE_SECTION_ALIGN(sai_sdma_handle_t txHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t dmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t context, 4);
volatile bool isFinished = false;
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
codec_handle_t codecHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_WM8524_Mute_GPIO(uint32_t output)
{
    GPIO_PinWrite(DEMO_CODEC_MUTE_PIN, DEMO_CODEC_MUTE_PIN_NUM, output);
}

static void callback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData)
{
    isFinished = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    sai_transfer_t xfer;
    sdma_config_t dmaConfig = {0};
    uint32_t temp           = 0;
    sai_transceiver_t config;

    /* M7 has its local cache and enabled by default,
     * need to set smart subsystems (0x28000000 ~ 0x3FFFFFFF)
     * non-cacheable before accessing this address region */
    BOARD_InitMemory();

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootMux(kCLOCK_RootSai3, kCLOCK_SaiRootmuxAudioPll1); /* Set SAI source to AUDIO PLL1 393215996HZ*/
    CLOCK_SetRootDivider(kCLOCK_RootSai3, 1U, 16U);                /* Set root clock to 393215996HZ / 16 = 24.575999M */
    /* gpio initialization */
    gpio_pin_config_t gpioConfig = {kGPIO_DigitalOutput, 1};
    GPIO_PinInit(DEMO_CODEC_MUTE_PIN, DEMO_CODEC_MUTE_PIN_NUM, &gpioConfig);

    PRINTF("SAI SDMA example started!\n\r");
    /* Create SDMA handle */
    SDMA_GetDefaultConfig(&dmaConfig);
#ifdef SDMA_FREQ_EQUALS_ARM
    dmaConfig.ratio = kSDMA_ARMClockFreq;
#endif

    SDMA_Init(EXAMPLE_DMA, &dmaConfig);
    SDMA_CreateHandle(&dmaHandle, EXAMPLE_DMA, EXAMPLE_CHANNEL, &context);
    SDMA_SetChannelPriority(EXAMPLE_DMA, EXAMPLE_CHANNEL, 2);

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleSDMA(DEMO_SAI, &txHandle, callback, NULL, &dmaHandle, SAI_TX_SOURCE);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);

#if defined DEMO_SAI_CLOCK_SOURCE
    config.bitClock.bclkSource = (sai_bclk_source_t)DEMO_SAI_CLOCK_SOURCE;
#endif

    SAI_TransferTxSetConfigSDMA(DEMO_SAI, &txHandle, &config);

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
    CODEC_Init(&codecHandle, &boardCodecConfig);

    /* Enable SAI interrupt to clear the FIFO error */
    EnableIRQ(DEMO_IRQn);
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);

    /*  xfer structure */
    temp          = (uint32_t)music;
    xfer.data     = (uint8_t *)temp;
    xfer.dataSize = MUSIC_LEN;
    SAI_TransferSendSDMA(DEMO_SAI, &txHandle, &xfer);

    /* Wait until finished */
    while (isFinished != true)
    {
    }

    CODEC_Deinit(&codecHandle);

    PRINTF("\n\r SAI SDMA example finished!\n\r ");
    while (1)
    {
    }
}

void SAI_UserIRQHandler(void)
{
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    __DSB();
}
