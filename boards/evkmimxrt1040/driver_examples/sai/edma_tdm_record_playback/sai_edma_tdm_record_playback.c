/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OVER_SAMPLE_RATE (384U)
#define BUFFER_SIZE      (1024U)
#define BUFFER_NUMBER    (4U)
/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate48KHz)
/* demo audio master clock */
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (8U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth32bits
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_InitCodec(void);
extern void BORAD_CodecReset(bool state);
extern void BOARD_Codec_CS42888_I2C_Init(void);
extern status_t BOARD_Codec_CS42888_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
extern status_t BOARD_Codec_CS42888_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t Buffer[BUFFER_NUMBER * BUFFER_SIZE], 4);
#if defined(DEMO_QUICKACCESS_SECTION_CACHEABLE) && DEMO_QUICKACCESS_SECTION_CACHEABLE
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle);
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxHandle);
#else
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t txHandle);
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t rxHandle);
#endif
static uint32_t tx_index = 0U, rx_index = 0U;
volatile uint32_t emptyBlock = BUFFER_NUMBER;
edma_handle_t dmaTxHandle = {0}, dmaRxHandle = {0};
extern codec_config_t boardCodecConfig;
codec_handle_t codecHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void rx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_RxError == status)
    {
        /* Handle the error. */
    }
    else
    {
        emptyBlock--;
    }
}

static void tx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxError == status)
    {
        /* Handle the error. */
    }
    else
    {
        emptyBlock++;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    sai_transfer_t xfer;
    edma_config_t dmaConfig = {0};
    sai_transceiver_t saiConfig;

    BOARD_InitHardware();

    PRINTF("SAI TDM record playback example started!\n\r");

    /* Init DMA and create handle for DMA */
    EDMA_GetDefaultConfig(&dmaConfig);
#if defined(BOARD_GetEDMAConfig)
    BOARD_GetEDMAConfig(dmaConfig);
#endif
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
    EDMA_CreateHandle(&dmaRxHandle, EXAMPLE_DMA, EXAMPLE_RX_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(EXAMPLE_DMA, EXAMPLE_TX_CHANNEL, EXAMPLE_SAI_TX_SOURCE);
    EDMA_SetChannelMux(EXAMPLE_DMA, EXAMPLE_RX_CHANNEL, EXAMPLE_SAI_RX_SOURCE);
#endif

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT 
    /* Init DMAMUX */
    DMAMUX_Init(EXAMPLE_DMAMUX);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL, (uint8_t)EXAMPLE_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_RX_CHANNEL, (uint8_t)EXAMPLE_SAI_RX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_RX_CHANNEL);
#endif

    /* SAI init */
    SAI_Init(DEMO_SAI);

    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, tx_callback, NULL, &dmaTxHandle);
    SAI_TransferRxCreateHandleEDMA(DEMO_SAI, &rxHandle, rx_callback, NULL, &dmaRxHandle);

    /* TDM mode configurations */
    SAI_GetTDMConfig(&saiConfig, kSAI_FrameSyncLenOneBitClk, DEMO_AUDIO_BIT_WIDTH, DEMO_AUDIO_DATA_CHANNEL,
                     kSAI_Channel0Mask);
    saiConfig.frameSync.frameSyncEarly = true;
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &saiConfig);
    SAI_TransferRxSetConfigEDMA(DEMO_SAI, &rxHandle, &saiConfig);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
    SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();
    /* CS42888 initialization */
    DEMO_InitCodec();

    while (1)
    {
        if (emptyBlock > 0)
        {
            xfer.data     = Buffer + rx_index * BUFFER_SIZE;
            xfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferReceiveEDMA(DEMO_SAI, &rxHandle, &xfer))
            {
                rx_index++;
            }
            if (rx_index == BUFFER_NUMBER)
            {
                rx_index = 0U;
            }
        }
        if (emptyBlock < BUFFER_NUMBER)
        {
            xfer.data     = Buffer + tx_index * BUFFER_SIZE;
            xfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer))
            {
                tx_index++;
            }
            if (tx_index == BUFFER_NUMBER)
            {
                tx_index = 0U;
            }
        }
    }
}

static void DEMO_InitCodec(void)
{
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        PRINTF("CODEC_Init failed!\r\n");
        assert(false);
    }

    PRINTF("\r\nCodec Init Done.\r\n");
}
