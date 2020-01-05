/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_dmamux.h"
#include "fsl_flexio_i2s_edma.h"
#include "fsl_debug_console.h"
#include "fsl_codec_common.h"
#include "fsl_sgtl5000.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_codec_adapter.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI and I2C instance and clock */
#define DEMO_I2C I2C0
#define DEMO_FLEXIO_BASE FLEXIO0
#define DEMO_I2C_CLKSRC kCLOCK_BusClk
#define DEMO_I2C_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
/* Flexio clock source frequency shall be even times of bit clock */
#define DEMO_FLEXIO_CLKSRC kCLOCK_PllFllSelClk
#define DEMO_FLEXIO_CLK_FREQ CLOCK_GetFreq(kCLOCK_PllFllSelClk)
#define EXAMPLE_DMA DMA0
#define EXAMPLE_TX_CHANNEL 17U
#define EXAMPLE_RX_CHANNEL 16U
#define EXAMPLE_TX_DMA_SOURCE kDmaRequestMux0Group1FlexIO0Channel0
#define EXAMPLE_RX_DMA_SOURCE kDmaRequestMux0Group1FlexIO0Channel1
#define BCLK_PIN 4U
#define FRAME_SYNC_PIN 5U
#define TX_DATA_PIN 1U
#define RX_DATA_PIN 0U
#define OVER_SAMPLE_RATE (384)
#define BUFFER_SIZE (128)
#define BUFFER_NUM (4)
#define PLAY_COUNT (5000 * 2U)
#define ZERO_BUFFER_SIZE (BUFFER_SIZE * 2U)
/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kFLEXIO_I2S_SampleRate16KHz)
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
#define DEMO_AUDIO_BIT_WIDTH (kFLEXIO_I2S_WordWidth32bits)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
sgtl_config_t sgtlConfig = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = 60000000},
    .route        = kSGTL_RoutePlaybackandRecord,
    .slaveAddress = SGTL5000_I2C_ADDR,
    .bus          = kSGTL_BusI2S,
    .format = {.mclk_HZ = 24576000U, .sampleRate = 16000U, .bitWidth = 32, .sclkEdge = kSGTL_SclkValidEdgeFailling},
    .master_slave = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_SGTL5000, .codecDevConfig = &sgtlConfig};
AT_NONCACHEABLE_SECTION_INIT(flexio_i2s_edma_handle_t txHandle)                           = {0};
AT_NONCACHEABLE_SECTION_INIT(flexio_i2s_edma_handle_t rxHandle)                           = {0};
edma_handle_t txDmaHandle                                                                 = {0};
edma_handle_t rxDmaHandle                                                                 = {0};
AT_NONCACHEABLE_SECTION_ALIGN_INIT(static uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM], 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN_INIT(static uint8_t zeroBuff[ZERO_BUFFER_SIZE], 4)          = {0};

extern codec_config_t boardCodecConfig;
FLEXIO_I2S_Type base;
static volatile bool isTxFinished     = false;
static volatile bool isRxFinished     = false;
static volatile uint32_t beginCount   = 0;
static volatile uint32_t sendCount    = 0;
static volatile uint32_t receiveCount = 0;
static volatile uint8_t emptyBlock    = 0;
static volatile bool isZeroBuffer     = true;
#if defined(DEMO_CODEC_WM8960) || defined(DEMO_CODEC_DA7212)
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
#endif
codec_handle_t codecHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void txCallback(FLEXIO_I2S_Type *i2sBase, flexio_i2s_edma_handle_t *handle, status_t status, void *userData)
{
    if ((emptyBlock < BUFFER_NUM) && (!isZeroBuffer))
    {
        emptyBlock++;
        sendCount++;
    }

    if (isZeroBuffer)
    {
        isZeroBuffer = false;
    }

    if (sendCount == beginCount)
    {
        isTxFinished = true;
    }
}

static void rxCallback(FLEXIO_I2S_Type *i2sBase, flexio_i2s_edma_handle_t *handle, status_t status, void *userData)
{
    if (emptyBlock > 0)
    {
        emptyBlock--;
        receiveCount++;
    }

    if (receiveCount == beginCount)
    {
        isRxFinished = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    DMAMUX_Type *dmamuxBase = NULL;
    flexio_i2s_config_t config;
    flexio_i2s_format_t format;
    flexio_i2s_transfer_t txXfer, rxXfer;
    uint8_t txIndex = 0, rxIndex = 0;
    edma_config_t dmaConfig = {0};

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetFlexio0Clock(1U);
    CLOCK_SetPllFllSelClock(3, 0, 0);
    BOARD_Codec_I2C_Init();
    PRINTF("FLEXIO I2S EDMA example started!\n\r");

    /* Set flexio i2s pin, shifter and timer */
    base.bclkPinIndex = BCLK_PIN;
    base.fsPinIndex   = FRAME_SYNC_PIN;
    base.txPinIndex   = TX_DATA_PIN;
    base.rxPinIndex   = RX_DATA_PIN;
#if defined FLEXIO_TX_SHIFTER_INDEX
    base.txShifterIndex = FLEXIO_TX_SHIFTER_INDEX;
#else
    base.txShifterIndex = 0;
#endif
#if defined FLEXIO_RX_SHIFTER_INDEX
    base.rxShifterIndex = FLEXIO_RX_SHIFTER_INDEX;
#else
    base.rxShifterIndex = 1;
#endif
    base.bclkTimerIndex = 0;
    base.fsTimerIndex   = 1;
    base.flexioBase     = DEMO_FLEXIO_BASE;

#if defined(DEMO_CODEC_WM8960) || defined(DEMO_CODEC_DA7212)
    /* SAI init */
    SAI_Init(DEMO_SAI);

    /* I2S mode configurations */
    sai_transceiver_t saiConfig;
    SAI_GetClassicI2SConfig(&saiConfig, (sai_word_width_t)DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    SAI_TxSetConfig(DEMO_SAI, &saiConfig);

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

    SAI_TxEnable(DEMO_SAI, true);
#endif
    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&txDmaHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
    EDMA_CreateHandle(&rxDmaHandle, EXAMPLE_DMA, EXAMPLE_RX_CHANNEL);

#if defined EXAMPLE_DMAMUX
    dmamuxBase = EXAMPLE_DMAMUX;
#else
    dmamuxBase          = DMAMUX0;
#endif
    DMAMUX_Init(dmamuxBase);
    DMAMUX_SetSource(dmamuxBase, EXAMPLE_TX_CHANNEL, EXAMPLE_TX_DMA_SOURCE);
    DMAMUX_EnableChannel(dmamuxBase, EXAMPLE_TX_CHANNEL);
    DMAMUX_SetSource(dmamuxBase, EXAMPLE_RX_CHANNEL, EXAMPLE_RX_DMA_SOURCE);
    DMAMUX_EnableChannel(dmamuxBase, EXAMPLE_RX_CHANNEL);

    /* Init SAI module */
    /*
     * config.enableI2S = true;
     */
    FLEXIO_I2S_GetDefaultConfig(&config);
    FLEXIO_I2S_Init(&base, &config);

    /* Configure the audio format */
    format.bitWidth      = DEMO_AUDIO_BIT_WIDTH;
    format.sampleRate_Hz = DEMO_AUDIO_SAMPLE_RATE;

    /* Use default setting to init codec */
    CODEC_Init(&codecHandle, &boardCodecConfig);

    FLEXIO_I2S_TransferTxCreateHandleEDMA(&base, &txHandle, txCallback, NULL, &txDmaHandle);
    FLEXIO_I2S_TransferRxCreateHandleEDMA(&base, &rxHandle, rxCallback, NULL, &rxDmaHandle);

    FLEXIO_I2S_TransferSetFormatEDMA(&base, &txHandle, &format, DEMO_FLEXIO_CLK_FREQ);
    FLEXIO_I2S_TransferSetFormatEDMA(&base, &rxHandle, &format, DEMO_FLEXIO_CLK_FREQ);

    emptyBlock = BUFFER_NUM;
    beginCount = PLAY_COUNT;
    /* send zero buffer fistly to make sure RX data is put into TX queue */
    txXfer.dataSize = ZERO_BUFFER_SIZE;
    txXfer.data     = zeroBuff;
    FLEXIO_I2S_TransferSendEDMA(&base, &txHandle, &txXfer);

    /* Wait until finished */
    while ((isTxFinished != true) || (isRxFinished != true))
    {
        if (emptyBlock > 0)
        {
            rxXfer.data     = audioBuff + rxIndex * BUFFER_SIZE;
            rxXfer.dataSize = BUFFER_SIZE;
            if (FLEXIO_I2S_TransferReceiveEDMA(&base, &rxHandle, &rxXfer) == kStatus_Success)
            {
                rxIndex = (rxIndex + 1) % BUFFER_NUM;
            }
        }

        if (emptyBlock < BUFFER_NUM)
        {
            txXfer.dataSize = BUFFER_SIZE;
            txXfer.data     = audioBuff + txIndex * BUFFER_SIZE;
            if (FLEXIO_I2S_TransferSendEDMA(&base, &txHandle, &txXfer) == kStatus_Success)
            {
                txIndex = (txIndex + 1) % BUFFER_NUM;
            }
        }
    }

    PRINTF("\n\r FLEXIO I2S EDMA example finished!\n\r ");

    while (1)
    {
    }
}
