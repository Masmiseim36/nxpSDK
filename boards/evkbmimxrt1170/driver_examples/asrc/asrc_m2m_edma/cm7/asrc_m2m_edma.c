/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_sai_edma.h"
#include "music.h"
#include "fsl_codec_common.h"
#include "fsl_asrc_edma.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OVER_SAMPLE_RATE (384U)

/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (2U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH          kSAI_WordWidth16bits
#define DEMO_ASRC_INPUT_SAMPLES       (48000)
#define DEMO_ASRC_CONVERT_BUFFER_SIZE (100 * 1000U)
#ifndef DEMO_ASRC_INPUT_CLOCK_SOURCE
#define DEMO_ASRC_INPUT_CLOCK_SOURCE kASRC_ClockSourceBitClock0_SAI1_TX
#endif
#ifndef DEMO_ASRC_OUTPUT_CLOCK_SOURCE
#define DEMO_ASRC_OUTPUT_CLOCK_SOURCE kASRC_ClockSourceBitClock0_SAI1_TX
#endif
#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 100U
#endif

edma_config_t dmaConfig = {0};
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
edma_handle_t saiDmaHandle     = {0};
edma_handle_t asrcInDmaHandle  = {0};
edma_handle_t asrcOutDmaHandle = {0};
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t saiHandle);
AT_QUICKACCESS_SECTION_DATA(asrc_edma_handle_t asrcHandle);
static volatile bool isSAIFinished  = false;
static volatile bool isASRCFinished = false;
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
uint8_t asrcConvertBuffer[DEMO_ASRC_CONVERT_BUFFER_SIZE] = {0U};
asrc_channel_pair_config_t s_asrcChannelPairConfig       = {
    .audioDataChannels         = kASRC_ChannelsNumber2,
    .inClockSource             = DEMO_ASRC_INPUT_CLOCK_SOURCE,
    .inSourceClock_Hz          = DEMO_ASRC_OUTPUT_SOURCE_CLOCK_HZ,
    .outClockSource            = DEMO_ASRC_OUTPUT_CLOCK_SOURCE,
    .outSourceClock_Hz         = DEMO_ASRC_OUTPUT_SOURCE_CLOCK_HZ,
    .sampleRateRatio           = kASRC_RatioUseInternalMeasured,
    .inDataWidth               = kASRC_DataWidth16Bit,
    .inDataAlign               = kASRC_DataAlignLSB,
    .outDataWidth              = kASRC_DataWidth16Bit,
    .outDataAlign              = kASRC_DataAlignLSB,
    .outSignExtension          = kASRC_NoSignExtension,
    .outFifoThreshold          = FSL_ASRC_CHANNEL_PAIR_FIFO_DEPTH / 2,
    .inFifoThreshold           = FSL_ASRC_CHANNEL_PAIR_FIFO_DEPTH / 2,
    .bufStallWhenFifoEmptyFull = false,
};
/*******************************************************************************
 * Code
 ******************************************************************************/
static void sai_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_SAI_TxIdle)
    {
        isSAIFinished = true;
    }
}

static void asrc_callback(ASRC_Type *base, asrc_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_ASRCOutQueueIdle)
    {
        isASRCFinished = true;
    }
}
/*!
 * @brief Main function
 */
int main(void)
{
    sai_transfer_t xfer;
    asrc_transfer_t asrcConvert;
    sai_transceiver_t config;
    uint8_t *audioData     = (uint8_t *)music;
    uint32_t outputBufSize = 0U;

    BOARD_InitHardware();

    PRINTF("ASRC m2m edma example\n\r");

    EDMA_GetDefaultConfig(&dmaConfig);
    BOARD_SAI_EDMA_CONFIG(&dmaConfig);
    EDMA_Init(DEMO_SAI_DMA, &dmaConfig);
    EDMA_CreateHandle(&saiDmaHandle, DEMO_SAI_DMA, DEMO_SAI_CHANNEL);

    EDMA_GetDefaultConfig(&dmaConfig);
    BOARD_ASRC_EDMA_CONFIG(&dmaConfig);
    EDMA_Init(DEMO_ASRC_DMA, &dmaConfig);
    EDMA_CreateHandle(&asrcInDmaHandle, DEMO_ASRC_DMA, DEMO_ASRC_IN_CHANNEL);
    EDMA_CreateHandle(&asrcOutDmaHandle, DEMO_ASRC_DMA, DEMO_ASRC_OUT_CHANNEL);

#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_ASRC_DMA, DEMO_ASRC_IN_CHANNEL, DEMO_ASRC_IN_EDMA_CHANNEL);
    EDMA_SetChannelMux(DEMO_ASRC_DMA, DEMO_ASRC_OUT_CHANNEL, DEMO_ASRC_OUT_EDMA_CHANNEL);
    EDMA_SetChannelMux(DEMO_SAI_DMA, DEMO_SAI_CHANNEL, DEMO_SAI_DMA_CHANNEL);
#endif

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &saiHandle, sai_callback, NULL, &saiDmaHandle);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    config.fifo.fifoWatermark = FSL_FEATURE_SAI_FIFO_COUNTn(DEMO_SAI) - 2U;
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &saiHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE_IN, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
    /* asrc init */
    ASRC_Init(DEMO_ASRC, DEMO_ASRC_PERIPHERAL_CLOCK);
    ASRC_TransferInCreateHandleEDMA(DEMO_ASRC, &asrcHandle, DEMO_ASRC_CHANNEL_PAIR, asrc_callback, &asrcInDmaHandle,
                                    NULL, NULL);
    ASRC_TransferOutCreateHandleEDMA(DEMO_ASRC, &asrcHandle, DEMO_ASRC_CHANNEL_PAIR, asrc_callback, &asrcOutDmaHandle,
                                     NULL, NULL);
    ASRC_TransferSetChannelPairConfigEDMA(DEMO_ASRC, &asrcHandle, &s_asrcChannelPairConfig, DEMO_AUDIO_SAMPLE_RATE_IN,
                                          DEMO_AUDIO_SAMPLE_RATE_OUT);
    outputBufSize = ASRC_GetOutSamplesSizeEDMA(DEMO_ASRC, &asrcHandle, DEMO_AUDIO_SAMPLE_RATE_IN,
                                               DEMO_AUDIO_SAMPLE_RATE_OUT, MUSIC_LEN);
    /* Use default setting to init codec */
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }

    if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                        DEMO_CODEC_VOLUME) != kStatus_Success)
    {
        assert(false);
    }

    PRINTF("Playback raw audio data\r\n");
    PRINTF("    sample rate : %d\r\n", DEMO_AUDIO_SAMPLE_RATE_IN);
    PRINTF("    channel number: %d\r\n", DEMO_AUDIO_DATA_CHANNEL);
    PRINTF("    frequency: 215HZ.\r\n\r\n");

    /*  xfer structure */
    xfer.data     = audioData;
    xfer.dataSize = MUSIC_LEN;
    SAI_TransferSendEDMA(DEMO_SAI, &saiHandle, &xfer);
    /* Wait until finished */
    while (isSAIFinished != true)
    {
    }
    isSAIFinished = false;

    asrcConvert.inData      = audioData;
    asrcConvert.inDataSize  = MUSIC_LEN;
    asrcConvert.outData     = asrcConvertBuffer;
    asrcConvert.outDataSize = outputBufSize;
    ASRC_TransferEDMA(ASRC, &asrcHandle, &asrcConvert);

    while (!isASRCFinished)
    {
    }

    PRINTF("Playback converted audio data\r\n");
    PRINTF("    sample rate : %d\r\n", DEMO_AUDIO_SAMPLE_RATE_OUT);
    PRINTF("    channel number: %d\r\n", DEMO_AUDIO_DATA_CHANNEL);
    PRINTF("    frequency: 215HZ.\r\n\r\n");

    /* reset sample rate configuration */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE_OUT, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
    if (CODEC_SetFormat(&codecHandle, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE_OUT, DEMO_AUDIO_BIT_WIDTH) !=
        kStatus_Success)
    {
        assert(false);
    }
    /*  xfer structure */
    xfer.data     = (uint8_t *)asrcConvertBuffer;
    xfer.dataSize = outputBufSize;
    SAI_TransferSendEDMA(DEMO_SAI, &saiHandle, &xfer);
    /* Wait until finished */
    while (isSAIFinished != true)
    {
    }

    PRINTF("ASRC m2m edma example finished\n\r ");
    while (1)
    {
    }
}
