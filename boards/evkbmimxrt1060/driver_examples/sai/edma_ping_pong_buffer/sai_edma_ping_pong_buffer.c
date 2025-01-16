/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "music.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
#include "fsl_debug_console.h"
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_SIZE (256U)
#define BUFFER_NUM  (2)
#define PLAY_COUNT  (100)
#ifndef DEMO_CODEC_INIT_DELAY_MS
#define DEMO_CODEC_INIT_DELAY_MS (1000U)
#endif
#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 100U
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_SAI_RXConfig(sai_transceiver_t *config, sai_sync_mode_t sync);
/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(DEMO_QUICKACCESS_SECTION_CACHEABLE) && DEMO_QUICKACCESS_SECTION_CACHEABLE
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle);
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxHandle);
#else
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t txHandle);
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t rxHandle);
#endif
static edma_handle_t s_dmaTxHandle = {0};
static edma_handle_t s_dmaRxHandle = {0};
extern codec_config_t boardCodecConfig;
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_buffer[BUFFER_NUM][BUFFER_SIZE], 4);
volatile bool isFinished             = false;
volatile uint32_t finishIndex        = 0U;
volatile uint32_t emptyBlock         = BUFFER_NUM;
static volatile bool s_Transfer_Done = false;
static volatile uint32_t s_playIndex = 0U;
static volatile uint32_t s_playCount = 0U;
static sai_transceiver_t s_saiConfig;
static codec_handle_t s_codecHandle;
sai_transfer_t saiRxPingPongTransfer[2U] = {
    {
        .data     = s_buffer[0],
        .dataSize = BUFFER_SIZE,
    },
    {
        .data     = s_buffer[1],
        .dataSize = BUFFER_SIZE,
    },
};

sai_transfer_t saiTxPingPongTransfer[2U] = {
    {
        .data     = s_buffer[1],
        .dataSize = BUFFER_SIZE,
    },
    {
        .data     = s_buffer[0],
        .dataSize = BUFFER_SIZE,
    },
};
/*******************************************************************************
 * Code
 ******************************************************************************/
void DelayMS(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        SDK_DelayAtLeastUs(1000, SystemCoreClock);
    }
}

static void rx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    __NOP();
}

static void tx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    __NOP();
}

/*!
 * @brief Main function
 */
int main(void)
{
    edma_config_t dmaConfig = {0};

    BOARD_InitHardware();

    PRINTF("SAI EDMA ping pong buffer example started!\n\r");

    memset(s_buffer, 0, BUFFER_NUM * BUFFER_SIZE);

    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
#if defined(BOARD_GetEDMAConfig)
    BOARD_GetEDMAConfig(dmaConfig);
#endif
    EDMA_Init(DEMO_DMA, &dmaConfig);
    EDMA_CreateHandle(&s_dmaTxHandle, DEMO_DMA, DEMO_TX_EDMA_CHANNEL);
    EDMA_CreateHandle(&s_dmaRxHandle, DEMO_DMA, DEMO_RX_EDMA_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_DMA, DEMO_TX_EDMA_CHANNEL, DEMO_SAI_TX_EDMA_CHANNEL);
    EDMA_SetChannelMux(DEMO_DMA, DEMO_RX_EDMA_CHANNEL, DEMO_SAI_RX_EDMA_CHANNEL);
#endif

    /* SAI init */
    SAI_Init(DEMO_SAI);

    /* I2S mode configurations */
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, tx_callback, NULL, &s_dmaTxHandle);
    SAI_TransferRxCreateHandleEDMA(DEMO_SAI, &rxHandle, rx_callback, NULL, &s_dmaRxHandle);

    SAI_GetClassicI2SConfig(&s_saiConfig, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, 1U << DEMO_SAI_CHANNEL);
    s_saiConfig.syncMode    = DEMO_SAI_TX_SYNC_MODE;
    s_saiConfig.masterSlave = DEMO_SAI_MASTER_SLAVE;
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &s_saiConfig);
    s_saiConfig.masterSlave = DEMO_SAI_MASTER_SLAVE;
    s_saiConfig.syncMode    = DEMO_SAI_RX_SYNC_MODE;
    SAI_TransferRxSetConfigEDMA(DEMO_SAI, &rxHandle, &s_saiConfig);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
    SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

    /* Use default setting to init codec */
    if (CODEC_Init(&s_codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    if (CODEC_SetVolume(&s_codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                        DEMO_CODEC_VOLUME) != kStatus_Success)
    {
        assert(false);
    }
    /* delay for codec output stable */
    DelayMS(DEMO_CODEC_INIT_DELAY_MS);

    SAI_TransferReceiveLoopEDMA(DEMO_SAI, &rxHandle, &saiRxPingPongTransfer[0], 2U);
    SAI_TransferSendLoopEDMA(DEMO_SAI, &txHandle, &saiTxPingPongTransfer[0], 2U);

    while (1)
    {
    }
}

#if defined(DEMO_SAITxIRQHandler)
void DEMO_SAITxIRQHandler(void)
{
    /* Clear the FIFO error flag */
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);

    /* Reset FIFO */
    SAI_TxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);
    SDK_ISR_EXIT_BARRIER;
}
#endif
