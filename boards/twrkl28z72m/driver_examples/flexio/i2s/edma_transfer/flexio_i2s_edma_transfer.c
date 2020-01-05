/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_dmamux.h"
#include "fsl_flexio_i2s_edma.h"
#include "fsl_debug_console.h"

#include "fsl_sgtl5000.h"
#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_I2C LPI2C0
#define DEMO_FLEXIO_BASE FLEXIO0

#define DEMO_FLEXIO_CLKSRC kCLOCK_ScgFircClk
#define DEMO_FLEXIO_CLK_FREQ CLOCK_GetFreq(kCLOCK_ScgFircClk)
#define DEMO_I2C_CLKSRC kCLOCK_ScgFircClk
#define DEMO_I2C_CLK_FREQ CLOCK_GetFreq(kCLOCK_ScgFircClk)
#define EXAMPLE_DMA DMA0
#define EXAMPLE_TX_CHANNEL 1U
#define EXAMPLE_RX_CHANNEL 0U
#define EXAMPLE_TX_DMA_SOURCE kDmaRequestMux0FlexIO0Channel0
#define EXAMPLE_RX_DMA_SOURCE kDmaRequestMux0FlexIO0Channel1
#define BCLK_PIN 4U
#define FRAME_SYNC_PIN 5U
#define TX_DATA_PIN 1U
#define RX_DATA_PIN 0U

#define OVER_SAMPLE_RATE (384)
#define BUFFER_SIZE (128)
#define BUFFER_NUM (4)
#define PLAY_COUNT (5000)
#define ZERO_BUFFER_SIZE (BUFFER_SIZE * 2U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_INIT(flexio_i2s_edma_handle_t txHandle) = {0};
AT_NONCACHEABLE_SECTION_INIT(flexio_i2s_edma_handle_t rxHandle) = {0};
edma_handle_t txDmaHandle = {0};
edma_handle_t rxDmaHandle = {0};
AT_NONCACHEABLE_SECTION_ALIGN_INIT(static uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM], 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN_INIT(static uint8_t zeroBuff[ZERO_BUFFER_SIZE], 4) = {0};

codec_handle_t codecHandle = {0};
extern codec_config_t boardCodecConfig;
FLEXIO_I2S_Type base;
static volatile bool isTxFinished = false;
static volatile bool isRxFinished = false;
static volatile uint32_t beginCount = 0;
static volatile uint32_t sendCount = 0;
static volatile uint32_t receiveCount = 0;
static volatile uint8_t emptyBlock = 0;
static volatile bool isZeroBuffer = true;
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
    uint32_t sourceClockHz = 0U;
    flexio_i2s_format_t format;
    flexio_i2s_transfer_t txXfer, rxXfer;
    uint8_t txIndex = 0, rxIndex = 0;
    edma_config_t dmaConfig = {0};

    BOARD_InitPins();
    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();

    /* Choose clock source for LPI2C */
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_IpSrcFircAsync);

    /* Choose clock source for SAI */
    CLOCK_SetIpSrc(kCLOCK_Flexio0, kCLOCK_IpSrcFircAsync);
    BOARD_Codec_I2C_Init();
    PRINTF("FLEXIO I2S EDMA example started!\n\r");

    /* Set flexio i2s pin, shifter and timer */
    base.bclkPinIndex = BCLK_PIN;
    base.fsPinIndex = FRAME_SYNC_PIN;
    base.txPinIndex = TX_DATA_PIN;
    base.rxPinIndex = RX_DATA_PIN;
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
    base.fsTimerIndex = 1;
    base.flexioBase = DEMO_FLEXIO_BASE;

#if defined(DEMO_CODEC_WM8960) || defined(DEMO_CODEC_DA7212)
    /* Use SAI to do master clock if using WM8960 codec */
    sai_config_t saiConfig;
    /*
     * saiConfig.masterSlave = kSAI_Master;
     * saiConfig.mclkSource = kSAI_MclkSourceSysclk;
     * saiConfig.protocol = kSAI_BusLeftJustified;
     * saiConfig.syncMode = kSAI_ModeAsync;
     * saiConfig.mclkOutputEnable = true;
     */
    SAI_TxGetDefaultConfig(&saiConfig);
    SAI_TxInit(DEMO_SAI, &saiConfig);

    /* Configure the audio format */
    sai_transfer_format_t saiFormat = {0};
    saiFormat.bitWidth = kSAI_WordWidth32bits;
    saiFormat.channel = 0U;
    saiFormat.sampleRate_Hz = kSAI_SampleRate16KHz;
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    saiFormat.masterClockHz = OVER_SAMPLE_RATE * saiFormat.sampleRate_Hz;
#else
    saiFormat.masterClockHz = DEMO_SAI_CLK_FREQ;
#endif
    saiFormat.protocol = saiConfig.protocol;

    uint32_t masterClockSrc = DEMO_SAI_CLK_FREQ;
    SAI_TxSetFormat(DEMO_SAI, &saiFormat, masterClockSrc, saiFormat.masterClockHz);
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
    dmamuxBase = DMAMUX0;
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
    format.bitWidth = kFLEXIO_I2S_WordWidth32bits;
    format.sampleRate_Hz = kFLEXIO_I2S_SampleRate16KHz;

    /* Use default setting to init codec */
    CODEC_Init(&codecHandle, &boardCodecConfig);
    CODEC_SetFormat(&codecHandle, format.sampleRate_Hz * OVER_SAMPLE_RATE, format.sampleRate_Hz, format.bitWidth);

    FLEXIO_I2S_TransferTxCreateHandleEDMA(&base, &txHandle, txCallback, NULL, &txDmaHandle);
    FLEXIO_I2S_TransferRxCreateHandleEDMA(&base, &rxHandle, rxCallback, NULL, &rxDmaHandle);

    sourceClockHz = DEMO_FLEXIO_CLK_FREQ;
    FLEXIO_I2S_TransferSetFormatEDMA(&base, &txHandle, &format, sourceClockHz);
    FLEXIO_I2S_TransferSetFormatEDMA(&base, &rxHandle, &format, sourceClockHz);

    emptyBlock = BUFFER_NUM;
    beginCount = PLAY_COUNT;
    /* send zero buffer fistly to make sure RX data is put into TX queue */
    txXfer.dataSize = ZERO_BUFFER_SIZE;
    txXfer.data = zeroBuff;
    FLEXIO_I2S_TransferSendEDMA(&base, &txHandle, &txXfer);

    /* Wait until finished */
    while ((isTxFinished != true) || (isRxFinished != true))
    {
        if (emptyBlock > 0)
        {
            rxXfer.data = audioBuff + rxIndex * BUFFER_SIZE;
            rxXfer.dataSize = BUFFER_SIZE;
            if (FLEXIO_I2S_TransferReceiveEDMA(&base, &rxHandle, &rxXfer) == kStatus_Success)
            {
                rxIndex = (rxIndex + 1) % BUFFER_NUM;
            }
        }

        if (emptyBlock < BUFFER_NUM)
        {
            txXfer.dataSize = BUFFER_SIZE;
            txXfer.data = audioBuff + txIndex * BUFFER_SIZE;
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
