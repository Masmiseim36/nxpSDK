/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP

 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_dma.h"
#include "fsl_i2c.h"
#include "fsl_i2s.h"
#include "fsl_i2s_dma.h"
#include "fsl_tfa9896.h"
#include "fsl_codec_common.h"
#include "music.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void StartSoundPlayback(void);

static void TxCallback(I2S_Type *base, i2s_dma_handle_t *handle, status_t completionStatus, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static dma_handle_t s_DmaTxHandle;
static i2s_config_t s_TxConfig;
static i2s_dma_handle_t s_TxHandle;
static i2s_transfer_t s_TxTransfer;
extern codec_config_t boardCodecConfigL;
extern codec_config_t boardCodecConfigR;
codec_handle_t codecHandleLeft;
codec_handle_t codecHandleRight;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("Configure I2S\r\n");

    /*
     * masterSlave = kI2S_MasterSlaveNormalMaster;
     * mode = kI2S_ModeI2sClassic;
     * rightLow = false;
     * leftJust = false;
     * pdmData = false;
     * sckPol = false;
     * wsPol = false;
     * divider = 1;
     * oneChannel = false;
     * dataLength = 16;
     * frameLength = 32;
     * position = 0;
     * watermark = 4;
     * txEmptyZero = true;
     * pack48 = false;
     */
    I2S_TxGetDefaultConfig(&s_TxConfig);
    s_TxConfig.divider = DEMO_I2S_CLOCK_DIVIDER;

    I2S_TxInit(DEMO_I2S_TX, &s_TxConfig);

    DMA_Init(DEMO_DMA);

    DMA_EnableChannel(DEMO_DMA, DEMO_I2S_TX_CHANNEL);
    DMA_SetChannelPriority(DEMO_DMA, DEMO_I2S_TX_CHANNEL, kDMA_ChannelPriority3);
    DMA_CreateHandle(&s_DmaTxHandle, DEMO_DMA, DEMO_I2S_TX_CHANNEL);

    StartSoundPlayback();

    PRINTF("Initialize left TFA9896\r\n");
    if (CODEC_Init(&codecHandleLeft, &boardCodecConfigL) != kStatus_Success)
    {
        PRINTF("TFA9896_Init failed for left device!\r\n");
        assert(false);
    }
    PRINTF("Initialize right TFA9896\r\n");
    if (CODEC_Init(&codecHandleRight, &boardCodecConfigR) != kStatus_Success)
    {
        PRINTF("TFA9896_Init failed for right device!\r\n");
        assert(false);
    }
#ifdef TFA9896_VOLUME_DEMO
#define ANY_CHANNEL 0x1
    int vol_iter = 0, i = 0;
    for (vol_iter = 0; vol_iter < 100; vol_iter++)
    {
        if (CODEC_SetVolume(&codecHandleLeft, ANY_CHANNEL, vol_iter) != kStatus_Success)
        {
            PRINTF("TFA9896_SetVolume failed for left device!\r\n");
            assert(false);
        }
        for (i = 0; i < 0xfff; i++)
        {
        }
        if (CODEC_SetVolume(&codecHandleRight, ANY_CHANNEL, vol_iter) != kStatus_Success)
        {
            PRINTF("TFA9896_SetVolume failed for right device!\r\n");
            assert(false);
        }
        for (i = 0; i < 4 * 0xfff; i++)
        {
        }
    }

    for (vol_iter = 100; vol_iter > 0; vol_iter--)
    {
        if (CODEC_SetVolume(&codecHandleLeft, ANY_CHANNEL, vol_iter) != kStatus_Success)
        {
            PRINTF("TFA9896_SetVolume failed for left device!\r\n");
            assert(false);
        }
        for (i = 0; i < 0xfff; i++)
        {
        }
        if (CODEC_SetVolume(&codecHandleRight, ANY_CHANNEL, vol_iter) != kStatus_Success)
        {
            PRINTF("TFA9896_SetVolume failed for right device!\r\n");
            assert(false);
        }
        for (i = 0; i < 10 * 0xfff; i++)
        {
        }
    }
    for (vol_iter = 0; vol_iter < 100; vol_iter++)
    {
        if (CODEC_SetVolume(&codecHandleLeft, ANY_CHANNEL, vol_iter) != kStatus_Success)
        {
            PRINTF("TFA9896_SetVolume failed for left device!\r\n");
            assert(false);
        }
        for (i = 0; i < 0xfff; i++)
        {
            ;
        }
        if (CODEC_SetVolume(&codecHandleRight, ANY_CHANNEL, vol_iter) != kStatus_Success)
        {
            PRINTF("TFA9896_SetVolume failed for right device!\r\n");
            assert(false);
        }
        for (i = 0; i < 10 * 0xfff; i++)
        {
        }
    }
#endif
    while (1)
    {
    }
}

static void StartSoundPlayback(void)
{
    PRINTF("Setup looping playback of sine wave\r\n");

    s_TxTransfer.data     = &g_Music[0];
    s_TxTransfer.dataSize = sizeof(g_Music);

    I2S_TxTransferCreateHandleDMA(DEMO_I2S_TX, &s_TxHandle, &s_DmaTxHandle, TxCallback, (void *)&s_TxTransfer);
    /* need to queue two transmit buffers so when the first one
     * finishes transfer, the other immediatelly starts */
    I2S_TxTransferSendDMA(DEMO_I2S_TX, &s_TxHandle, s_TxTransfer);
    I2S_TxTransferSendDMA(DEMO_I2S_TX, &s_TxHandle, s_TxTransfer);
}

static void TxCallback(I2S_Type *base, i2s_dma_handle_t *handle, status_t completionStatus, void *userData)
{
    /* Enqueue the same original buffer all over again */
    i2s_transfer_t *transfer = (i2s_transfer_t *)userData;
    I2S_TxTransferSendDMA(base, handle, *transfer);
}
