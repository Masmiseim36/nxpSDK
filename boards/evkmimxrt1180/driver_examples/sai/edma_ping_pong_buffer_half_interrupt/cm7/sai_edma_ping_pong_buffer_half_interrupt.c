/*
 * Copyright 2018-2020 NXP
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
#define BUFFER_SIZE (1600U)
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
void EDMA_TX_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);
extern void BOARD_SAI_RXConfig(sai_transceiver_t *config, sai_sync_mode_t sync);
/*******************************************************************************
 * Variables
 ******************************************************************************/
edma_handle_t g_dmaHandle = {0};
extern codec_config_t boardCodecConfig;
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t buffer[BUFFER_NUM * BUFFER_SIZE], 4);
volatile bool isFinished      = false;
volatile uint32_t finishIndex = 0U;
volatile uint32_t emptyBlock  = BUFFER_NUM;
#if defined(DEMO_QUICKACCESS_SECTION_CACHEABLE) && DEMO_QUICKACCESS_SECTION_CACHEABLE
AT_NONCACHEABLE_SECTION_ALIGN(static edma_tcd_t s_emdaTcd, 32);
#else
AT_QUICKACCESS_SECTION_DATA_ALIGN(static edma_tcd_t s_emdaTcd, 32);
#endif
static volatile bool s_Transfer_Done        = false;
static volatile uint32_t s_playIndex        = 0U;
static volatile uint32_t s_playCount        = 0U;
static volatile bool s_transferComplete     = false;
static volatile bool s_transferHalfComplete = false;
#if (defined(DEMO_EDMA_HAS_CHANNEL_CONFIG) && DEMO_EDMA_HAS_CHANNEL_CONFIG)
extern edma_config_t dmaConfig;
#else
edma_config_t dmaConfig = {0};
#endif

codec_handle_t codecHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* User callback function for EDMA transfer. */
void EDMA_TX_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        s_transferComplete = true;
    }
    else
    {
        s_transferHalfComplete = true;
    }

    s_playCount++;
}

void DelayMS(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        SDK_DelayAtLeastUs(1000, SystemCoreClock);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    edma_transfer_config_t transferConfig = {0};
    uint32_t destAddr                     = SAI_TxGetDataRegisterAddress(DEMO_SAI, DEMO_SAI_CHANNEL);
    sai_transceiver_t saiConfig;

    BOARD_InitHardware();

    PRINTF("SAI EDMA Ping Pong Buffer Half Interrupt example started!\n\r");

    memcpy(buffer, music, BUFFER_NUM * BUFFER_SIZE);
    s_playIndex = BUFFER_NUM * BUFFER_SIZE;

#if (!defined(DEMO_EDMA_HAS_CHANNEL_CONFIG) || (defined(DEMO_EDMA_HAS_CHANNEL_CONFIG) && !DEMO_EDMA_HAS_CHANNEL_CONFIG))
    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
#endif
    EDMA_Init(DEMO_DMA, &dmaConfig);
    EDMA_CreateHandle(&g_dmaHandle, DEMO_DMA, DEMO_EDMA_CHANNEL);
    EDMA_SetCallback(&g_dmaHandle, EDMA_TX_Callback, NULL);
    EDMA_ResetChannel(DEMO_DMA, g_dmaHandle.channel);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_DMA, DEMO_EDMA_CHANNEL, DEMO_SAI_EDMA_CHANNEL);
#endif

    /* SAI init */
    SAI_Init(DEMO_SAI);

    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&saiConfig, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, 1U << DEMO_SAI_CHANNEL);
    saiConfig.syncMode    = DEMO_SAI_TX_SYNC_MODE;
    saiConfig.masterSlave = DEMO_SAI_MASTER_SLAVE;
    SAI_TxSetConfig(DEMO_SAI, &saiConfig);
    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* sai rx configurations */
    BOARD_SAI_RXCONFIG(&saiConfig, DEMO_SAI_RX_SYNC_MODE);
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

    /* delay for codec output stable */
    DelayMS(DEMO_CODEC_INIT_DELAY_MS);

    /* Configure and submit transfer structure 1 */
    EDMA_PrepareTransfer(
        &transferConfig, buffer, DEMO_AUDIO_BIT_WIDTH / 8U, (void *)destAddr, DEMO_AUDIO_BIT_WIDTH / 8U,
        (FSL_FEATURE_SAI_FIFO_COUNTn(DEMO_SAI) - saiConfig.fifo.fifoWatermark) * (DEMO_AUDIO_BIT_WIDTH / 8U),
        BUFFER_SIZE * BUFFER_NUM, kEDMA_MemoryToPeripheral);

#if defined FSL_EDMA_DRIVER_EDMA4 && FSL_EDMA_DRIVER_EDMA4
    EDMA_TcdSetTransferConfigExt(DEMO_DMA, &s_emdaTcd, &transferConfig, &s_emdaTcd);
    EDMA_TcdEnableInterruptsExt(DEMO_DMA, &s_emdaTcd, kEDMA_MajorInterruptEnable | kEDMA_HalfInterruptEnable);
#else
    EDMA_TcdSetTransferConfig(&s_emdaTcd, &transferConfig, &s_emdaTcd);
    EDMA_TcdEnableInterrupts(&s_emdaTcd, kEDMA_MajorInterruptEnable | kEDMA_HalfInterruptEnable);
#endif
    EDMA_InstallTCD(DEMO_DMA, g_dmaHandle.channel, &s_emdaTcd);
    EDMA_StartTransfer(&g_dmaHandle);
    /* Enable DMA enable bit */
    SAI_TxEnableDMA(DEMO_SAI, kSAI_FIFORequestDMAEnable, true);
    /* Enable SAI Tx clock */
    SAI_TxEnable(DEMO_SAI, true);
    /* Enable the channel FIFO */
    SAI_TxSetChannelFIFOMask(DEMO_SAI, 1U << DEMO_SAI_CHANNEL);

    /* Waiting until finished. */
    while (s_playCount < PLAY_COUNT)
    {
        if (s_transferHalfComplete == true)
        {
            memcpy(&buffer[0], &music[s_playIndex], BUFFER_SIZE);
            s_playIndex += BUFFER_SIZE;
            if (s_playIndex >= MUSIC_LEN)
            {
                s_playIndex = 0U;
            }
            s_transferHalfComplete = false;
        }

        if (s_transferComplete == true)
        {
            memcpy(&buffer[BUFFER_SIZE], &music[s_playIndex], BUFFER_SIZE);
            s_playIndex += BUFFER_SIZE;
            if (s_playIndex >= MUSIC_LEN)
            {
                s_playIndex = 0U;
            }
            s_transferComplete = false;
        }
    }

    /* Once transfer finish, disable SAI instance. */
    SAI_Deinit(DEMO_SAI);
    PRINTF("\n\rSAI EDMA Ping Pong Buffer Half Interrupt example finished!\n\r ");
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
