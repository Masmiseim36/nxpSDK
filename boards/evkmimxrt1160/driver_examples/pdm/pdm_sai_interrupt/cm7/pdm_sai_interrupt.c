/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_pdm.h"
#include "fsl_sai.h"
#include "fsl_debug_console.h"
#include "fsl_codec_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_SIZE   (DEMO_PDM_FIFO_WATERMARK * FSL_FEATURE_PDM_FIFO_WIDTH * 2U)
#define BUFFER_NUMBER (256U)
#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 100U
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void saiCallback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
sai_handle_t s_saiTxHandle         = {0};
static volatile bool isSaiFinished = false;
SDK_ALIGN(static uint8_t txBuff[BUFFER_SIZE * BUFFER_NUMBER], 4);
static volatile bool s_dataReadFinishedFlag = false;
static volatile uint32_t s_readIndex        = 0U;
static volatile uint32_t s_writeIndex       = 0U;
static const pdm_config_t pdmConfig         = {
#if defined(FSL_FEATURE_PDM_HAS_DECIMATION_FILTER_BYPASS) && FSL_FEATURE_PDM_HAS_DECIMATION_FILTER_BYPASS
    .enableFilterBypass = false,
#endif
    .enableDoze        = false,
    .fifoWatermark     = DEMO_PDM_FIFO_WATERMARK,
    .qualityMode       = DEMO_PDM_QUALITY_MODE,
    .cicOverSampleRate = DEMO_PDM_CIC_OVERSAMPLE_RATE,
};
static pdm_channel_config_t channelConfig = {
#if (defined(FSL_FEATURE_PDM_HAS_DC_OUT_CTRL) && (FSL_FEATURE_PDM_HAS_DC_OUT_CTRL))
#ifdef DEMO_PDM_CHANNEL_OUTPUT_CUTOFF_FREQUENCY
    .outputCutOffFreq = DEMO_PDM_CHANNEL_OUTPUT_CUTOFF_FREQUENCY,
#else
    .outputCutOffFreq = kPDM_DcRemoverCutOff40Hz,
#endif
#endif

#if !(defined(FSL_FEATURE_PDM_DC_CTRL_VALUE_FIXED) && (FSL_FEATURE_PDM_DC_CTRL_VALUE_FIXED))
#ifdef DEMO_PDM_CHANNEL_CUTOFF_FREQUENCY
    .cutOffFreq = DEMO_PDM_CHANNEL_CUTOFF_FREQUENCY,
#else
    .cutOffFreq = kPDM_DcRemoverCutOff152Hz,
#endif
#endif

#ifdef DEMO_PDM_CHANNEL_GAIN
    .gain = DEMO_PDM_CHANNEL_GAIN,
#else
    .gain       = kPDM_DfOutputGain7,
#endif
};
codec_handle_t codecHandle;
extern codec_config_t boardCodecConfig;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void saiCallback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
    isSaiFinished = true;
}

static void pdm_error_irqHandler(void)
{
    uint32_t status = 0U;

#if (defined(FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ) && (FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ == 1U))
    if (PDM_GetStatus(DEMO_PDM) & PDM_STAT_LOWFREQF_MASK)
    {
        PDM_ClearStatus(DEMO_PDM, PDM_STAT_LOWFREQF_MASK);
    }
#endif

    status = PDM_GetFifoStatus(DEMO_PDM);
    if (status != 0U)
    {
        PDM_ClearFIFOStatus(DEMO_PDM, status);
    }

#if defined(FSL_FEATURE_PDM_HAS_RANGE_CTRL) && FSL_FEATURE_PDM_HAS_RANGE_CTRL
    status = PDM_GetRangeStatus(DEMO_PDM);
    if (status != 0U)
    {
        PDM_ClearRangeStatus(DEMO_PDM, status);
    }
#else
    status = PDM_GetOutputStatus(DEMO_PDM);
    if (status != 0U)
    {
        PDM_ClearOutputStatus(DEMO_PDM, status);
    }
#endif
}

#if !(defined FSL_FEATURE_PDM_HAS_NO_INDEPENDENT_ERROR_IRQ && FSL_FEATURE_PDM_HAS_NO_INDEPENDENT_ERROR_IRQ)
void PDM_ERROR_IRQHandler(void)
{
    pdm_error_irqHandler();
    SDK_ISR_EXIT_BARRIER;
}
#endif

void PDM_EVENT_IRQHandler(void)
{
    uint32_t status = PDM_GetStatus(DEMO_PDM);
    /* handle PDM error status */
#if (defined FSL_FEATURE_PDM_HAS_NO_INDEPENDENT_ERROR_IRQ && FSL_FEATURE_PDM_HAS_NO_INDEPENDENT_ERROR_IRQ)
    pdm_error_irqHandler();
#endif
    /* recieve data */
    if ((1U << DEMO_PDM_ENABLE_CHANNEL_LEFT) & status)
    {
        if ((s_readIndex >= s_writeIndex) || (s_readIndex <= (s_writeIndex - 1U)))
        {
            PDM_ReadFifo(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_LEFT, 2, &txBuff[s_readIndex * BUFFER_SIZE],
                         DEMO_PDM_FIFO_WATERMARK, FSL_FEATURE_PDM_FIFO_WIDTH);
            s_readIndex += 1U;
            if (s_readIndex == BUFFER_NUMBER)
            {
                s_readIndex = 0U;
            }
            s_dataReadFinishedFlag = true;
        }
    }

    PDM_ClearStatus(DEMO_PDM, status);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    sai_transfer_t xfer;
    sai_transceiver_t config;

    BOARD_InitHardware();

    PRINTF("PDM sai interrupt example started!\n\r");

    memset(txBuff, 0U, sizeof(txBuff));

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandle(DEMO_SAI, &s_saiTxHandle, saiCallback, NULL);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, 1U << DEMO_SAI_CHANNEL);
    config.bitClock.bclkSource = DEMO_SAI_CLOCK_SOURCE;
#if defined BOARD_SAI_RXCONFIG
    config.syncMode = DEMO_SAI_TX_SYNC_MODE;
#endif
    config.masterSlave = DEMO_SAI_MASTER_SLAVE;
    SAI_TransferTxSetConfig(DEMO_SAI, &s_saiTxHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
#if defined BOARD_SAI_RXCONFIG
    BOARD_SAI_RXCONFIG(&config, DEMO_SAI_RX_SYNC_MODE);
#endif

    /* master clock configurations */
    BOARD_MasterClockConfig();
#if defined DEMO_BOARD_CODEC_INIT
    DEMO_BOARD_CODEC_INIT();
#else
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                              DEMO_CODEC_VOLUME) != kStatus_Success)
    {
        assert(false);
    }
#endif
    /* Set up pdm */
    PDM_Init(DEMO_PDM, &pdmConfig);
    PDM_SetChannelConfig(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_LEFT, &channelConfig);
    PDM_SetChannelConfig(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_RIGHT, &channelConfig);
    if (PDM_SetSampleRateConfig(DEMO_PDM, DEMO_PDM_CLK_FREQ, DEMO_AUDIO_SAMPLE_RATE) != kStatus_Success)
    {
        PRINTF("PDM configure sample rate failed.\r\n");
        return -1;
    }
    PDM_Reset(DEMO_PDM);
    PDM_EnableInterrupts(DEMO_PDM, kPDM_ErrorInterruptEnable | kPDM_FIFOInterruptEnable);
    EnableIRQ(PDM_EVENT_IRQn);
    PDM_Enable(DEMO_PDM, true);

    /* wait data read finish */
    while (1)
    {
        if (s_readIndex != s_writeIndex)
        {
            /*  xfer structure */
            xfer.data     = (uint8_t *)(&txBuff[s_writeIndex * BUFFER_SIZE]);
            xfer.dataSize = BUFFER_SIZE;
            if (SAI_TransferSendNonBlocking(DEMO_SAI, &s_saiTxHandle, &xfer) != kStatus_SAI_QueueFull)
            {
                if ((s_writeIndex += 1U) >= BUFFER_NUMBER)
                {
                    s_writeIndex = 0U;
                }
            }
        }
    }
}
