/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_sai_sdma.h"
#include "music.h"
#include "fsl_codec_common.h"
#include "fsl_asrc_sdma.h"
#include "fsl_asrc.h"

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
#define DEMO_ASRC ASRC
#define DEMO_SAI_CLK_FREQ                                                                  \
    (CLOCK_GetPllFreq(kCLOCK_AudioPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootSai3)) / \
     (CLOCK_GetRootPostDivider(kCLOCK_RootSai3)))
#define DEMO_CODEC_WM8524 (1)
#define DEMO_CODEC_BUS_PIN (NULL)
#define DEMO_CODEC_BUS_PIN_NUM (0)
#define DEMO_CODEC_MUTE_PIN (GPIO5)
#define DEMO_CODEC_MUTE_PIN_NUM (21)
#define DEMO_IRQn I2S3_IRQn
#define DEMO_DMA SDMAARM2
#define DEMO_SAI_SDMA_CHANNEL (1)
#define DEMO_ASRC_IN_SDMA_CHANNEL 2
#define DEMO_ASRC_OUT_SDMA_CHANNEL 3
#define DEMO_ASRC_IN_SDMA_SOURCE 16
#define DEMO_ASRC_OUT_SDMA_SOURCE 17
#define DEMO_ASRC_CONTEXT kASRC_Context0
#define DEMO_SAI_TX_SOURCE (5)
/*set Bclk source to Mclk clock*/
#define DEMO_SAI_CLOCK_SOURCE (1U)
#define SDMA_FREQ_EQUALS_ARM (1U)
#define DEMO_ASRC_IN_SAMPLE_RATE 44100
#define DEMO_ASRC_OUT_SAMPLE_RATE 16000
#define OVER_SAMPLE_RATE (384U)
/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_CONVERT_SAMPLE_RATE (kSAI_SampleRate48KHz)
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
/* convert buffer size, please note that the maximum size  of once transfer that SDMA can support is 64k */
#define DEMO_AUDIO_BUFFER_SIZE (16000U)
#define DEMO_AUDIO_SAMPLES (MUSIC_LEN / 2U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_WM8524_Mute_GPIO(uint32_t output);
static void saiCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData);
static void saiPlayAudio(uint8_t *data, uint32_t dataSize);
static void asrcConvertAudio(void *in, void *out, uint32_t dataSize);

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
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t s_saiContext, 4);
volatile uint8_t isSaiFinishedCount = 0;
volatile bool isConvertFinished     = false;
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
/*! @brief ASRC sdma handle */
AT_NONCACHEABLE_SECTION_ALIGN(static asrc_sdma_handle_t s_asrcHandle, 4U);
AT_NONCACHEABLE_SECTION_ALIGN(static sdma_handle_t s_asrcInSDMAHandle, 4U);
AT_NONCACHEABLE_SECTION_ALIGN(static sdma_handle_t s_asrcOutSDMAHandle, 4U);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t s_asrcInSDMAcontext, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t s_asrcOutSDMAcontext, 4);
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_asrcOutBuffer[DEMO_AUDIO_BUFFER_SIZE * 8], 4);

/*! @brief ASRC context config */
static asrc_context_config_t s_asrcContextConfig;
/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_WM8524_Mute_GPIO(uint32_t output)
{
    GPIO_PinWrite(DEMO_CODEC_MUTE_PIN, DEMO_CODEC_MUTE_PIN_NUM, output);
}

static void saiCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData)
{
    isSaiFinishedCount++;
}

static void asrcInCallback(ASRC_Type *base, asrc_sdma_handle_t *handle, status_t status, void *userData)
{
}

static void asrcOutCallback(ASRC_Type *base, asrc_sdma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_ASRCQueueIdle)
    {
        isConvertFinished = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t *temp           = (uint8_t *)music;
    sdma_config_t dmaConfig = {0};
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

    PRINTF("ASRC memory to memory SDMA example.\n\r");

    memset(s_asrcOutBuffer, 0U, sizeof(s_asrcOutBuffer));

    /* Create SDMA handle */
    SDMA_GetDefaultConfig(&dmaConfig);
#ifdef SDMA_FREQ_EQUALS_ARM
    dmaConfig.ratio = kSDMA_ARMClockFreq;
#endif
    SDMA_Init(DEMO_DMA, &dmaConfig);

    /* create sai sdma handle */
    SDMA_CreateHandle(&dmaHandle, DEMO_DMA, DEMO_SAI_SDMA_CHANNEL, &s_saiContext);
    SDMA_SetChannelPriority(DEMO_DMA, DEMO_SAI_SDMA_CHANNEL, 2);

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleSDMA(DEMO_SAI, &txHandle, saiCallback, NULL, &dmaHandle, DEMO_SAI_TX_SOURCE);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);

#if defined DEMO_SAI_CLOCK_SOURCE
    config.bitClock.bclkSource = (sai_bclk_source_t)DEMO_SAI_CLOCK_SOURCE;
#endif

    SAI_TransferTxSetConfigSDMA(DEMO_SAI, &txHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_ASRC_IN_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
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

    /* Enable SAI interrupt to clear the FIFO error */
    EnableIRQ(DEMO_IRQn);
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);

    /* create ASRC sdma handle */
    SDMA_CreateHandle(&s_asrcInSDMAHandle, DEMO_DMA, DEMO_ASRC_IN_SDMA_CHANNEL, &s_asrcInSDMAcontext);
    SDMA_SetChannelPriority(DEMO_DMA, DEMO_ASRC_IN_SDMA_CHANNEL, 4);
    SDMA_CreateHandle(&s_asrcOutSDMAHandle, DEMO_DMA, DEMO_ASRC_OUT_SDMA_CHANNEL, &s_asrcOutSDMAcontext);
    SDMA_SetChannelPriority(DEMO_DMA, DEMO_ASRC_OUT_SDMA_CHANNEL, 3);

    ASRC_Init(DEMO_ASRC);
    ASRC_GetContextDefaultConfig(&s_asrcContextConfig, DEMO_AUDIO_DATA_CHANNEL, DEMO_ASRC_IN_SAMPLE_RATE,
                                 DEMO_ASRC_OUT_SAMPLE_RATE);
    ASRC_TransferInCreateHandleSDMA(DEMO_ASRC, &s_asrcHandle, asrcInCallback, &s_asrcInSDMAHandle,
                                    DEMO_ASRC_IN_SDMA_SOURCE, DEMO_ASRC_CONTEXT, NULL, NULL);
    ASRC_TransferOutCreateHandleSDMA(DEMO_ASRC, &s_asrcHandle, asrcOutCallback, &s_asrcOutSDMAHandle,
                                     DEMO_ASRC_OUT_SDMA_SOURCE, DEMO_ASRC_CONTEXT, NULL, NULL);
    if (ASRC_TransferSetContextConfigSDMA(DEMO_ASRC, &s_asrcHandle, &s_asrcContextConfig) != kStatus_Success)
    {
        PRINTF("ASRC context configure failed, please check.\r\n");
        return -1;
    }

    /* Use default setting to init codec */
    CODEC_Init(&codecHandle, &boardCodecConfig);

    PRINTF("Playback original 44.1K audio data.\n\r");
    saiPlayAudio(temp, MUSIC_LEN);

    /* reconfigure the sai bit clock*/
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_ASRC_OUT_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    PRINTF("Playback converted 16K audio data.\n\r");
    asrcConvertAudio(temp, s_asrcOutBuffer, MUSIC_LEN);
    saiPlayAudio((uint8_t *)s_asrcOutBuffer,
                 ASRC_GetContextOutSampleSize(DEMO_ASRC_IN_SAMPLE_RATE, MUSIC_LEN, 2, DEMO_ASRC_OUT_SAMPLE_RATE, 2));

    PRINTF("\n\r ASRC memory to memory SDMA example finished.\n\r ");
    while (1)
    {
    }
}

static void saiPlayAudio(uint8_t *data, uint32_t dataSize)
{
    sai_transfer_t xfer;
    uint32_t leftSize = dataSize;
    uint8_t count     = 0U;

    while (leftSize)
    {
        /*  xfer structure */
        xfer.data     = (uint8_t *)((uint32_t)data + dataSize - leftSize);
        xfer.dataSize = leftSize > 64000 ? 64000 : leftSize;
        if (SAI_TransferSendSDMA(DEMO_SAI, &txHandle, &xfer) == kStatus_Success)
        {
            leftSize -= xfer.dataSize;
            count++;
        }
    }
    /* Wait until finished */
    while (isSaiFinishedCount != count)
    {
    }
    isSaiFinishedCount = 0U;
}

static void asrcConvertAudio(void *in, void *out, uint32_t dataSize)
{
    asrc_transfer_t asrcTransfer;

    asrcTransfer.inDataAddr  = in;
    asrcTransfer.inDataSize  = dataSize;
    asrcTransfer.outDataAddr = out;
    asrcTransfer.outDataSize =
        ASRC_GetContextOutSampleSize(DEMO_ASRC_IN_SAMPLE_RATE, dataSize, 2, DEMO_ASRC_OUT_SAMPLE_RATE, 2);
    ;

    ASRC_TransferSDMA(DEMO_ASRC, &s_asrcHandle, &asrcTransfer);

    /* Wait until finished */
    while (!isConvertFinished)
    {
    }
}

void SAI_UserIRQHandler(void)
{
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    __DSB();
}
