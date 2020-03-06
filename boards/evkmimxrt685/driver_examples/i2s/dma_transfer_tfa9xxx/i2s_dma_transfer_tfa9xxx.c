/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
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
#include "fsl_dma.h"
#include "fsl_i2c.h"
#include "fsl_i2s.h"
#include "fsl_i2s_dma.h"
#include "fsl_tfa9xxx.h"
#include "fsl_codec_common.h"
#include "music.h"

#include "pin_mux.h"
#include <stdbool.h>
#include "fsl_codec_adapter.h"
#include "tfa_config_TFA9894N2.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_I2C (I2C4)
#define DEMO_I2S_MASTER_CLOCK_FREQUENCY CLOCK_GetMclkClkFreq()
#define DEMO_AUDIO_BIT_WIDTH (16)
#define DEMO_AUDIO_SAMPLE_RATE (48000)
#define DEMO_AUDIO_PROTOCOL kCODEC_BusI2S
#define DEMO_I2S_TX (I2S1)
#define DEMO_DMA (DMA0)
#define DEMO_I2S_TX_CHANNEL (3)
#define DEMO_I2S_CLOCK_DIVIDER 16
#define DEMO_I2S_TX_MODE kI2S_MasterSlaveNormalMaster

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void StartSoundPlayback(void);

static void TxCallback(I2S_Type *base, i2s_dma_handle_t *handle, status_t completionStatus, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
tfa9xxx_config_t tfa9xxxConfigLeft = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
    .slaveAddress = TFA9XXX_I2C_ADDR_0,
    .protocol     = kTFA9XXX_BusI2S,
    .format       = {.sampleRate = kTFA9XXX_AudioSampleRate48KHz, .bitWidth = kTFA9XXX_AudioBitWidth16bit},
    .tfaContainer = tfa_container_bin,
    .deviceIndex  = 0,
};

tfa9xxx_config_t tfa9xxxConfigRight = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
    .slaveAddress = TFA9XXX_I2C_ADDR_1,
    .protocol     = kTFA9XXX_BusI2S,
    .format       = {.sampleRate = kTFA9XXX_AudioSampleRate48KHz, .bitWidth = kTFA9XXX_AudioBitWidth16bit},
    .tfaContainer = tfa_container_bin,
    .deviceIndex  = 1,
};

codec_config_t boardCodecConfigLeft  = {.codecDevType = kCODEC_TFA9XXX, .codecDevConfig = &tfa9xxxConfigLeft};
codec_config_t boardCodecConfigRight = {.codecDevType = kCODEC_TFA9XXX, .codecDevConfig = &tfa9xxxConfigRight};


static dma_handle_t s_DmaTxHandle;
static i2s_config_t s_TxConfig;
static i2s_dma_handle_t s_TxHandle;
static i2s_transfer_t s_TxTransfer;
extern codec_config_t boardCodecConfigLeft;
extern codec_config_t boardCodecConfigRight;
codec_handle_t codecHandleLeft;
codec_handle_t codecHandleRight;

//global volatile counter used for sleep/wait
 volatile uint32_t g_systickCounter;
/*******************************************************************************
 * Code
 ******************************************************************************/
void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

/* delay n milli-seconds */
void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while(g_systickCounter != 0U)
    {
    }
}
/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_InputMux);

    /* attach main clock to I3C */
    CLOCK_AttachClk(kMAIN_CLK_to_I3C_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cClk, 20);

    /* attach AUDIO PLL clock to FLEXCOMM1 (I2S1) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM1);
    /* attach AUDIO PLL clock to FLEXCOMM3 (I2S3) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM3);

    /* attach AUDIO PLL clock to MCLK */
    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 1);
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;

    tfa9xxxConfigLeft.i2cConfig.codecI2CSourceClock  = CLOCK_GetI3cClkFreq();
    tfa9xxxConfigRight.i2cConfig.codecI2CSourceClock = tfa9xxxConfigLeft.i2cConfig.codecI2CSourceClock;

    /* Set shared signal set 0: SCK, WS from Flexcomm1 */
    SYSCTL1->SHAREDCTRLSET[0] = SYSCTL1_SHAREDCTRLSET_SHAREDSCKSEL(1) | SYSCTL1_SHAREDCTRLSET_SHAREDWSSEL(1);
    /* Set flexcomm3 SCK, WS from shared signal set 0 */
    SYSCTL1->FCCTRLSEL[3] = SYSCTL1_FCCTRLSEL_SCKINSEL(1) | SYSCTL1_FCCTRLSEL_WSINSEL(1);

    // System Tick Configuration, generate 1ms interrupt
    SysTick_Config(SystemCoreClock / 1000U);
    
    PRINTF("Configure TFA9XXX amplifier\r\n");

    /* protocol: i2s
     * sampleRate: 48K
     * bitwidth:16
     */
    if (CODEC_Init(&codecHandleLeft, &boardCodecConfigLeft) != kStatus_Success)
    {
        PRINTF("TFA9XXX_Init left failed!\r\n");
    }
    else
    {
        CODEC_SetVolume(&codecHandleLeft, kCODEC_PlayChannelLeft0 | kCODEC_PlayChannelRight0, 50);
    }
    if (CODEC_Init(&codecHandleRight, &boardCodecConfigRight) != kStatus_Success)
    {
        PRINTF("TFA9XXX_Init right failed!\r\n");
    }
    else
    {
        CODEC_SetVolume(&codecHandleRight, kCODEC_PlayChannelLeft0 | kCODEC_PlayChannelRight0, 100);
    }
    
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
    s_TxConfig.divider     = DEMO_I2S_CLOCK_DIVIDER;
    s_TxConfig.masterSlave = DEMO_I2S_TX_MODE;

    I2S_TxInit(DEMO_I2S_TX, &s_TxConfig);

    DMA_Init(DEMO_DMA);

    DMA_EnableChannel(DEMO_DMA, DEMO_I2S_TX_CHANNEL);
    DMA_SetChannelPriority(DEMO_DMA, DEMO_I2S_TX_CHANNEL, kDMA_ChannelPriority3);
    DMA_CreateHandle(&s_DmaTxHandle, DEMO_DMA, DEMO_I2S_TX_CHANNEL);

    StartSoundPlayback();
    
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
     * finishes transfer, the other immediately starts */
    I2S_TxTransferSendDMA(DEMO_I2S_TX, &s_TxHandle, s_TxTransfer);
    I2S_TxTransferSendDMA(DEMO_I2S_TX, &s_TxHandle, s_TxTransfer);
}

static void TxCallback(I2S_Type *base, i2s_dma_handle_t *handle, status_t completionStatus, void *userData)
{
    /* Enqueue the same original buffer all over again */
    i2s_transfer_t *transfer = (i2s_transfer_t *)userData;
    I2S_TxTransferSendDMA(base, handle, *transfer);
}
