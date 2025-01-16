/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_sai.h"
#include "fsl_codec_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_SIZE   (1024U)
#define BUFFER_NUMBER (4U)
#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 100U
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t Buffer[BUFFER_NUMBER * BUFFER_SIZE], 4);
sai_handle_t txHandle = {0}, rxHandle = {0};
static uint32_t tx_index = 0U, rx_index = 0U;
volatile uint32_t emptyBlock = BUFFER_NUMBER;
extern codec_config_t boardCodecConfig;
codec_handle_t codecHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void rx_callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
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

static void tx_callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
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
    sai_transceiver_t saiConfig;

    BOARD_InitHardware();

    PRINTF("SAI interrupt record playback example started!\n\r");

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandle(DEMO_SAI, &txHandle, tx_callback, NULL);
    SAI_TransferRxCreateHandle(DEMO_SAI, &rxHandle, rx_callback, NULL);

    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&saiConfig, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, 1U << DEMO_SAI_CHANNEL);
    saiConfig.syncMode    = DEMO_SAI_TX_SYNC_MODE;
    saiConfig.masterSlave = DEMO_SAI_MASTER_SLAVE;
    SAI_TransferTxSetConfig(DEMO_SAI, &txHandle, &saiConfig);
    saiConfig.syncMode = DEMO_SAI_RX_SYNC_MODE;
    SAI_TransferRxSetConfig(DEMO_SAI, &rxHandle, &saiConfig);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
    SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

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
    while (1)
    {
        if (emptyBlock > 0)
        {
            xfer.data     = Buffer + rx_index * BUFFER_SIZE;
            xfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferReceiveNonBlocking(DEMO_SAI, &rxHandle, &xfer))
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
            if (kStatus_Success == SAI_TransferSendNonBlocking(DEMO_SAI, &txHandle, &xfer))
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
