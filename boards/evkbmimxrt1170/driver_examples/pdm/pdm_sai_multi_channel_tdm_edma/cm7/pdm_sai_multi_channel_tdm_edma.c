/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_pdm.h"
#include "fsl_pdm_edma.h"
#include "fsl_edma.h"
#include "fsl_sai.h"
#include "fsl_sai_edma.h"
#include "fsl_debug_console.h"
#include "fsl_codec_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_SIZE   (1024)
#define BUFFER_NUMBER (2)
#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 100U
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void pdmCallback(PDM_Type *base, pdm_edma_handle_t *handle, status_t status, void *userData);
static void saiCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(pdm_edma_handle_t s_pdmRxHandle_0, 4);
AT_NONCACHEABLE_SECTION_ALIGN(edma_handle_t s_pdmDmaHandle_0, 4);
AT_NONCACHEABLE_SECTION_ALIGN(edma_handle_t s_saiDmaHandle, 4);
#if defined(DEMO_QUICKACCESS_SECTION_CACHEABLE) && DEMO_QUICKACCESS_SECTION_CACHEABLE
AT_NONCACHEABLE_SECTION_ALIGN(sai_edma_handle_t s_saiTxHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(edma_tcd_t s_edmaTcd_0[2], 32U);
AT_NONCACHEABLE_SECTION_ALIGN(edma_tcd_t s_edmaTcd_1[2], 32U);
#else
AT_QUICKACCESS_SECTION_DATA_ALIGN(sai_edma_handle_t s_saiTxHandle, 4);
AT_QUICKACCESS_SECTION_DATA_ALIGN(edma_tcd_t s_edmaTcd_0[2], 32U);
AT_QUICKACCESS_SECTION_DATA_ALIGN(edma_tcd_t s_edmaTcd_1[2], 32U);
#endif
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_buffer[BUFFER_SIZE * BUFFER_NUMBER], 4);

pdm_edma_transfer_t pdmXfer[2] = {
    {
        .data         = s_buffer,
        .dataSize     = BUFFER_SIZE,
        .linkTransfer = &pdmXfer[1],
    },

    {.data = &s_buffer[BUFFER_SIZE], .dataSize = BUFFER_SIZE, .linkTransfer = &pdmXfer[0]},
};

static volatile uint32_t s_bufferValidBlock = BUFFER_NUMBER;
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
static const pdm_channel_config_t channelConfig = {
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
static void pdmCallback(PDM_Type *base, pdm_edma_handle_t *handle, status_t status, void *userData)
{
    if (s_bufferValidBlock)
    {
        s_bufferValidBlock--;
    }
}

static void saiCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxError == status)
    {
        /* Handle the error. */
    }
    else
    {
        if (s_bufferValidBlock < BUFFER_NUMBER)
        {
            s_bufferValidBlock++;
        }
    }
}

void PDM_ERROR_IRQHandler(void)
{
    uint32_t fifoStatus = 0U;

#if (defined(FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ) && (FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ == 1U))
    if (PDM_GetStatus(DEMO_PDM) & PDM_STAT_LOWFREQF_MASK)
    {
        PDM_ClearStatus(DEMO_PDM, PDM_STAT_LOWFREQF_MASK);
    }
#endif

    fifoStatus = PDM_GetFifoStatus(DEMO_PDM);
    if (fifoStatus)
    {
        PDM_ClearFIFOStatus(DEMO_PDM, fifoStatus);
    }
    SDK_ISR_EXIT_BARRIER;
}

void SAI_UserIRQHandler(void)
{
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    edma_config_t dmaConfig = {0};
    sai_transfer_t saiXfer;
    sai_transceiver_t config;

    BOARD_InitHardware();

    PRINTF("PDM SAI multi channel TDM edma example started!\n\r");

    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_EDMA, &dmaConfig);
    EDMA_CreateHandle(&s_pdmDmaHandle_0, DEMO_EDMA, DEMO_PDM_EDMA_CHANNEL_0);
    EDMA_CreateHandle(&s_saiDmaHandle, DEMO_EDMA, DEMO_SAI_EDMA_CHANNEL);

#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_EDMA, DEMO_PDM_EDMA_CHANNEL_0, DEMO_PDM_EDMA_SOURCE);
    EDMA_SetChannelMux(DEMO_EDMA, DEMO_SAI_EDMA_CHANNEL, DEMO_SAI_EDMA_SOURCE);
#endif

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &s_saiTxHandle, saiCallback, NULL, &s_saiDmaHandle);
    /* TDM mode configurations */
    SAI_GetTDMConfig(&config, kSAI_FrameSyncLenOneBitClk, DEMO_AUDIO_BIT_WIDTH, DEMO_AUDIO_DATA_CHANNEL,
                     kSAI_Channel0Mask);
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                        DEMO_CODEC_VOLUME) != kStatus_Success)
    {
        assert(false);
    }
    config.frameSync.frameSyncEarly = true;
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &s_saiTxHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MasterClockConfig();

    /* Setup pdm */
    PDM_Init(DEMO_PDM, &pdmConfig);

    PDM_TransferCreateHandleEDMA(DEMO_PDM, &s_pdmRxHandle_0, pdmCallback, NULL, &s_pdmDmaHandle_0);
    PDM_TransferInstallEDMATCDMemory(&s_pdmRxHandle_0, s_edmaTcd_0, 2);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &s_pdmRxHandle_0, DEMO_PDM_ENABLE_CHANNEL_0, &channelConfig);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &s_pdmRxHandle_0, DEMO_PDM_ENABLE_CHANNEL_1, &channelConfig);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &s_pdmRxHandle_0, DEMO_PDM_ENABLE_CHANNEL_2, &channelConfig);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &s_pdmRxHandle_0, DEMO_PDM_ENABLE_CHANNEL_3, &channelConfig);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &s_pdmRxHandle_0, DEMO_PDM_ENABLE_CHANNEL_4, &channelConfig);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &s_pdmRxHandle_0, DEMO_PDM_ENABLE_CHANNEL_5, &channelConfig);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &s_pdmRxHandle_0, DEMO_PDM_ENABLE_CHANNEL_6, &channelConfig);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &s_pdmRxHandle_0, DEMO_PDM_ENABLE_CHANNEL_7, &channelConfig);
    if (PDM_SetSampleRateConfig(DEMO_PDM, DEMO_PDM_CLK_FREQ, DEMO_AUDIO_SAMPLE_RATE) != kStatus_Success)
    {
        PRINTF("PDM configure sample rate failed.\r\n");
        return -1;
    }
    PDM_Reset(DEMO_PDM);

    PDM_TransferReceiveEDMA(DEMO_PDM, &s_pdmRxHandle_0, pdmXfer);

    while (1)
    {
        /* wait one buffer busy to send data */
        if (s_bufferValidBlock < BUFFER_NUMBER)
        {
            saiXfer.data     = (uint8_t *)((uint32_t)s_buffer + s_writeIndex * BUFFER_SIZE);
            saiXfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferSendEDMA(DEMO_SAI, &s_saiTxHandle, &saiXfer))
            {
                s_writeIndex++;
            }
            if (s_writeIndex == BUFFER_NUMBER)
            {
                s_writeIndex = 0U;
            }
        }
    }
}
