/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_sai.h"
#include "music.h"
#include "fsl_codec_common.h"
#include "fsl_asrc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OVER_SAMPLE_RATE (384U)

/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (2U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH          kSAI_WordWidth16bits
#define DEMO_ASRC_CONVERT_BUFFER_SIZE (150 * 1000U)
#ifndef DEMO_ASRC_OUTPUT_CLOCK_SOURCE
#define DEMO_ASRC_OUTPUT_CLOCK_SOURCE kASRC_ClockSourceBitClock0_SAI1_TX
#endif
#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 100U
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
sai_handle_t txHandle               = {0};
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
asrc_handle_t asrcHandle;
uint8_t asrcConvertBuffer[DEMO_ASRC_CONVERT_BUFFER_SIZE] = {0U};
asrc_channel_pair_config_t s_asrcChannelPairConfig       = {
    .audioDataChannels         = kASRC_ChannelsNumber2,
    .inClockSource             = kASRC_ClockSourceNotAvalible,
    .inSourceClock_Hz          = 0,
    .outClockSource            = DEMO_ASRC_OUTPUT_CLOCK_SOURCE,
    .outSourceClock_Hz         = DEMO_ASRC_OUTPUT_SOURCE_CLOCK_HZ,
    .sampleRateRatio           = kASRC_RatioUseIdealRatio,
    .inDataWidth               = kASRC_DataWidth16Bit,
    .inDataAlign               = kASRC_DataAlignLSB,
    .outDataWidth              = kASRC_DataWidth16Bit,
    .outDataAlign              = kASRC_DataAlignLSB,
    .outSignExtension          = kASRC_NoSignExtension,
    .outFifoThreshold          = FSL_ASRC_CHANNEL_PAIR_FIFO_DEPTH / 2,
    .inFifoThreshold           = FSL_ASRC_CHANNEL_PAIR_FIFO_DEPTH / 2,
    .bufStallWhenFifoEmptyFull = true,
};
/*******************************************************************************
 * Code
 ******************************************************************************/
static void sai_callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_SAI_TxIdle)
    {
        isSAIFinished = true;
    }
}

static void asrc_in_callback(ASRC_Type *base, asrc_handle_t *handle, status_t status, void *userData)
{
}

static void asrc_out_callback(ASRC_Type *base, asrc_handle_t *handle, status_t status, void *userData)
{
    isASRCFinished = true;
}
/*!
 * @brief Main function
 */
int main(void)
{
    sai_transfer_t xfer;
    asrc_transfer_t asrcConvert;
    sai_transceiver_t config;

    BOARD_InitHardware();

    PRINTF("ASRC m2m interrupt example\n\r");

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandle(DEMO_SAI, &txHandle, sai_callback, NULL);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    SAI_TransferTxSetConfig(DEMO_SAI, &txHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE_IN, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
    /* asrc init */
    ASRC_Init(DEMO_ASRC, DEMO_ASRC_PERIPHERAL_CLOCK);
    ASRC_TransferCreateHandle(DEMO_ASRC, &asrcHandle, DEMO_ASRC_CHANNEL_PAIR, asrc_in_callback, asrc_out_callback,
                              NULL);
    ASRC_TransferSetChannelPairConfig(DEMO_ASRC, &asrcHandle, &s_asrcChannelPairConfig, DEMO_AUDIO_SAMPLE_RATE_IN,
                                      DEMO_AUDIO_SAMPLE_RATE_OUT);

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
    xfer.data     = (uint8_t *)music;
    xfer.dataSize = MUSIC_LEN;
    SAI_TransferSendNonBlocking(DEMO_SAI, &txHandle, &xfer);
    /* Wait until finished */
    while (isSAIFinished != true)
    {
    }
    isSAIFinished = false;

    asrcConvert.inData      = music;
    asrcConvert.inDataSize  = MUSIC_LEN;
    asrcConvert.outData     = asrcConvertBuffer;
    asrcConvert.outDataSize = ASRC_GetOutSamplesSize(DEMO_ASRC, DEMO_ASRC_CHANNEL_PAIR, DEMO_AUDIO_SAMPLE_RATE_IN,
                                                     DEMO_AUDIO_SAMPLE_RATE_OUT, MUSIC_LEN);
    ASRC_TransferNonBlocking(ASRC, &asrcHandle, &asrcConvert);

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
    xfer.dataSize = DEMO_ASRC_CONVERT_BUFFER_SIZE;
    SAI_TransferSendNonBlocking(DEMO_SAI, &txHandle, &xfer);
    /* Wait until finished */
    while (isSAIFinished != true)
    {
    }

    PRINTF("ASRC m2m interrupt example finished\n\r ");
    while (1)
    {
    }
}
