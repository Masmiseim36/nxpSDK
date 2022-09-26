
/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sco_audio_pl.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_sai.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
#include "fsl_wm8960.h"
#include "fsl_codec_adapter.h"
#include "fsl_debug_console.h"
#include "ringtone.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Sco loop back, data from sco input to sco output */
#define SCO_SAI_LOOPBACK (0)
/* Codec loop back data from mic to speaker */
#define CODEC_SAI_LOOPBACK (0)

#define OVER_SAMPLE_RATE        (256U)
#define BOARD_CODEC_SAI         SAI1
#define DEMO_CODEC_SAI_IRQ      SAI1_IRQn
#define SAI_CodecUserIRQHandler SAI1_IRQHandler
#define BOARD_SCO_SAI           SAI2
//#define DEMO_CODEC_SAI_IRQ      SAI1_IRQn
//#define SAI_CodecUserIRQHandler SAI1_IRQHandler

/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (1U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of sai1 clock */
#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7)) 
#define DEMO_SAI_CLK_FREQ                                                        \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
#define DEMO_SAI_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Sai1)
#endif
#define BOARD_SCO_DEMO_I2C LPI2C1

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define BOARD_SCO_DEMO_I2C_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

/* DMA */
#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7)) 
#define EXAMPLE_DMAMUX               DMAMUX
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
#define EXAMPLE_DMAMUX               DMAMUX0
#endif
#define EXAMPLE_DMA                  DMA0
#define EXAMPLE_DMA_IRQ              DMA0_DMA16_IRQn
#define EXAMPLE_MICBUF_TX_CHANNEL    (0U)
#define EXAMPLE_MICBUF_RX_CHANNEL    (1U)
#define EXAMPLE_SPKBUF_TX_CHANNEL    (2U)
#define EXAMPLE_SPKBUF_RX_CHANNEL    (3U)
#define EXAMPLE_SAI_MICBUF_RX_SOURCE kDmaRequestMuxSai1Rx
#define EXAMPLE_SAI_MICBUF_TX_SOURCE kDmaRequestMuxSai2Tx
#define EXAMPLE_SAI_SPKBUF_TX_SOURCE kDmaRequestMuxSai1Tx
#define EXAMPLE_SAI_SPKBUF_RX_SOURCE kDmaRequestMuxSai2Rx

#define AUDIO_INTERRUPT_PRIORITY (3U)
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (2U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth16bits
/* demo audio sample frequency */

#define BUFFER_SIZE      (1024U)
#define BUFFER_NUMBER    (4U)
#define AUDIO_DUMMY_SIZE (64U)

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

static uint32_t masterCodecClockHz = 0U;
static codec_handle_t codecScoHandle;
static wm8960_config_t wm8960ScoConfig = {
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .route     = kWM8960_RoutePlaybackandRecord,
    .rightInputSource = kWM8960_InputDifferentialMicInput2,
    .playSource       = kWM8960_PlaySourceDAC,
    .slaveAddress     = WM8960_I2C_ADDR,
    .bus              = kWM8960_BusPCMB,
    .format           = {.mclk_HZ = 6144000U, .sampleRate = 8000, .bitWidth = kWM8960_AudioBitWidth16bit},
    .master_slave     = false,
};
static codec_config_t boardCodecScoConfig = {.codecDevType = kCODEC_WM8960, .codecDevConfig = &wm8960ScoConfig};

/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
 *                              = 24 * (32 + 77/100)
 *                              = 786.48 MHz
 */
const clock_audio_pll_config_t audioCodecPllConfig = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};

AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t MicBuffer[BUFFER_NUMBER * BUFFER_SIZE], 4);
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txMicHandle);
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxMicHandle);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t SpeakerBuffer[BUFFER_NUMBER * BUFFER_SIZE], 4);
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txSpeakerHandle);
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxSpeakerHandle);
AT_NONCACHEABLE_SECTION(uint32_t g_AudioTxDummyBuffer[AUDIO_DUMMY_SIZE / 4U]);

OSA_SEMAPHORE_HANDLE_DEFINE(xSemaphoreScoAudio);
static edma_handle_t dmaTxMicHandle     = {0};
static edma_handle_t dmaRxMicHandle     = {0};
static edma_handle_t dmaTxSpeakerHandle = {0};
static edma_handle_t dmaRxSpeakerHandle = {0};
static uint32_t txMic_index = 0U, rxMic_index = 0U;
volatile uint32_t emptyMicBlock = BUFFER_NUMBER;
static uint32_t txSpeaker_index = 0U, rxSpeaker_index = 0U;
volatile uint32_t emptySpeakerBlock = BUFFER_NUMBER;
static uint32_t rxSpeaker_test = 0U, rxMic_test = 0U;
static volatile uint8_t s_ringTone = 0;
static uint32_t cpy_index = 0U, tx_index = 0U;
static volatile uint8_t sco_audio_setup = 0;
static SCO_AUDIO_EP_INFO s_ep_info;
static void BOARD_SCO_EnableSaiMclkOutput(bool enable);
static void BOARD_SCO_SAI_DeInit(void);
static API_RESULT board_audio_setup_pl_ext(uint8_t isRing, SCO_AUDIO_EP_INFO *ep_info);
API_RESULT sco_audio_start_pl_ext(void);
/* --------------------------------------------- Functions */
/* overwrite sco_audio_init_pl_ext of sco_audio_pl.c.
 * The follow functions can be overwritten too
 * if the actual example need implement them to
 * use different audio data.
 * sco_audio_init_pl_ext, sco_audio_shutdown_pl_ext,
 * sco_audio_setup_pl_ext, sco_audio_start_pl_ext,
 * sco_audio_stop_pl_ext, sco_audio_write_pl_ext.
 */

static void SAI_UserTxIRQHandler(I2S_Type *SAI)
{
    /* Clear the FEF (Tx FIFO underrun) flag. */
    SAI_TxClearStatusFlags(SAI, kSAI_FIFOErrorFlag);
    SAI_TxSoftwareReset(SAI, kSAI_ResetTypeFIFO);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
        exception return operation might vector to incorrect interrupt */
    __DSB();
}

static void SAI_UserRxIRQHandler(I2S_Type *SAI)
{
    /* Clear the FEF (Rx FIFO overflow) flag. */
    SAI_RxClearStatusFlags(SAI, kSAI_FIFOErrorFlag);
    SAI_RxSoftwareReset(SAI, kSAI_ResetTypeFIFO);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
        exception return operation might vector to incorrect interrupt */
    __DSB();
}

void SAI2_IRQHandler(void)
{
    if (SAI2->TCSR & kSAI_FIFOErrorFlag)
    {
        SAI_UserTxIRQHandler(SAI2);
    }

    if (SAI2->RCSR & kSAI_FIFOErrorFlag)
    {
        SAI_UserRxIRQHandler(SAI2);
    }
}

static void rxMicCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_RxError == status)
    {
        /* Handle the error. */
    }
    else
    {
        emptyMicBlock--;
        rxMic_test++;
        OSA_SemaphorePost(xSemaphoreScoAudio);
    }
}
#if SCO_SAI_LOOPBACK
static void txMicCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxError == status)
    {
        /* Handle the error. */
    }
    else
    {
#if SCO_SAI_LOOPBACK
        emptySpeakerBlock++;
#else
        emptyMicBlock++;
#endif
        rxMic_test++;
        OSA_SemaphorePost(xSemaphoreScoAudio);
    }
}
#else
static void txMicCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    static volatile uint8_t s_8978ConsumerActualData = 0;
    sai_transfer_t xfer;
    if (s_ringTone == 1U)
    {
        s_8978ConsumerActualData = 0;
        xfer.dataSize            = AUDIO_DUMMY_SIZE;
        xfer.data                = (uint8_t *)&g_AudioTxDummyBuffer[0];
        SAI_TransferSendEDMA(BOARD_SCO_SAI, &txMicHandle, &xfer);
    }
    else
    {
        if (s_8978ConsumerActualData)
        {
            s_8978ConsumerActualData = 0;
            emptyMicBlock++;
            OSA_SemaphorePost(xSemaphoreScoAudio);

            if (emptyMicBlock < (BUFFER_NUMBER))
            {
                s_8978ConsumerActualData = 1;
                xfer.data                = MicBuffer + txMic_index * BUFFER_SIZE;
                xfer.dataSize            = BUFFER_SIZE;
                if (kStatus_Success == SAI_TransferSendEDMA(BOARD_SCO_SAI, &txMicHandle, &xfer))
                {
                    txMic_index++;
                }
                if (txMic_index == BUFFER_NUMBER)
                {
                    txMic_index = 0U;
                }
            }
            else
            {
                /* printf("mic dummy\r\n"); */
                s_8978ConsumerActualData = 0;
                xfer.dataSize            = AUDIO_DUMMY_SIZE;
                xfer.data                = (uint8_t *)&g_AudioTxDummyBuffer[0];
                SAI_TransferSendEDMA(BOARD_SCO_SAI, &txMicHandle, &xfer);
            }
        }
        else
        {
            if (emptyMicBlock < (BUFFER_NUMBER - 2))
            {
                s_8978ConsumerActualData = 1;
                xfer.data                = MicBuffer + txMic_index * BUFFER_SIZE;
                xfer.dataSize            = BUFFER_SIZE;
                if (kStatus_Success == SAI_TransferSendEDMA(BOARD_SCO_SAI, &txMicHandle, &xfer))
                {
                    txMic_index++;
                }
                if (txMic_index == BUFFER_NUMBER)
                {
                    txMic_index = 0U;
                }
            }
            else
            {
                /* printf("mic dummy\r\n"); */
                s_8978ConsumerActualData = 0;
                xfer.dataSize            = AUDIO_DUMMY_SIZE;
                xfer.data                = (uint8_t *)&g_AudioTxDummyBuffer[0];
                SAI_TransferSendEDMA(BOARD_SCO_SAI, &txMicHandle, &xfer);
            }
        }
    }
}
#endif
static void txSpeakerCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    static volatile uint8_t s_consumerActualData = 0;
    sai_transfer_t xfer;
    if (s_ringTone == 1U)
    {
        if ((emptySpeakerBlock > 0U) && (cpy_index < MUSIC_LEN / BUFFER_SIZE))
        {
            /* Fill in the buffers. */
            memcpy((uint8_t *)&SpeakerBuffer[BUFFER_SIZE * (cpy_index % BUFFER_NUMBER)],
                   (uint8_t *)&music[cpy_index * BUFFER_SIZE], sizeof(uint8_t) * BUFFER_SIZE);
            emptySpeakerBlock--;
            cpy_index++;
        }

        if (emptySpeakerBlock < BUFFER_NUMBER)
        {
            /*  xfer structure */
            xfer.data     = (uint8_t *)&SpeakerBuffer[BUFFER_SIZE * (tx_index % BUFFER_NUMBER)];
            xfer.dataSize = BUFFER_SIZE;
            /* Wait for available queue. */
            if (kStatus_Success == SAI_TransferSendEDMA(BOARD_CODEC_SAI, &txSpeakerHandle, &xfer))
            {
                tx_index++;
            }
            emptySpeakerBlock++;
        }
    }
    else
    {
        if (s_consumerActualData)
        {
            s_consumerActualData = 0;
            emptySpeakerBlock++;
            OSA_SemaphorePost(xSemaphoreScoAudio);

            if (emptySpeakerBlock < (BUFFER_NUMBER))
            {
                s_consumerActualData = 1;
                xfer.data            = SpeakerBuffer + txSpeaker_index * BUFFER_SIZE;
                xfer.dataSize        = BUFFER_SIZE;
                if (kStatus_Success == SAI_TransferSendEDMA(BOARD_CODEC_SAI, &txSpeakerHandle, &xfer))
                {
                    txSpeaker_index++;
                }
                if (txSpeaker_index == BUFFER_NUMBER)
                {
                    txSpeaker_index = 0U;
                }
            }
            else
            {
                s_consumerActualData = 0;
                xfer.dataSize        = AUDIO_DUMMY_SIZE;
                xfer.data            = (uint8_t *)&g_AudioTxDummyBuffer[0];
                SAI_TransferSendEDMA(BOARD_CODEC_SAI, &txSpeakerHandle, &xfer);
            }
        }
        else
        {
            if (emptySpeakerBlock < (BUFFER_NUMBER - 2))
            {
                s_consumerActualData = 1;
                xfer.data            = SpeakerBuffer + txSpeaker_index * BUFFER_SIZE;
                xfer.dataSize        = BUFFER_SIZE;
                if (kStatus_Success == SAI_TransferSendEDMA(BOARD_CODEC_SAI, &txSpeakerHandle, &xfer))
                {
                    txSpeaker_index++;
                }
                if (txSpeaker_index == BUFFER_NUMBER)
                {
                    txSpeaker_index = 0U;
                }
            }
            else
            {
                s_consumerActualData = 0;
                xfer.dataSize        = AUDIO_DUMMY_SIZE;
                xfer.data            = (uint8_t *)&g_AudioTxDummyBuffer[0];
                SAI_TransferSendEDMA(BOARD_CODEC_SAI, &txSpeakerHandle, &xfer);
            }
        }
    }
}

static void rxSpeakerCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxError == status)
    {
        /* Handle the error. */
    }
    else
    {
        emptySpeakerBlock--;
        OSA_SemaphorePost(xSemaphoreScoAudio);
        rxSpeaker_test++;
    }
}
static void BOARD_SCO_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7)) 
        IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
        IOMUXC_GPR->GPR0 |= IOMUXC_GPR_GPR0_SAI1_MCLK_DIR_MASK;
#endif
    }
    else
    {
#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7)) 
        IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK);
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
        IOMUXC_GPR->GPR0 &= (~IOMUXC_GPR_GPR0_SAI1_MCLK_DIR_MASK);
#endif
    }
}

static void BOARD_SCO_Codec_Init(void)
{
    CODEC_Init(&codecScoHandle, &boardCodecScoConfig);
}

static void BOARD_SCO_SAI_Init(uint32_t samplingRate, UCHAR bitWidth)
{
    sai_transceiver_t codecConfig, scoConfig;
    SAI_Init(BOARD_CODEC_SAI);
    SAI_Init(BOARD_SCO_SAI);

    wm8960ScoConfig.bus =  kWM8960_BusPCMB;
    /* Enable SAI transmit and FIFO error interrupts. */
    SAI_TxEnableInterrupts(SAI2, kSAI_FIFOErrorInterruptEnable);
    SAI_RxEnableInterrupts(SAI2, kSAI_FIFOErrorInterruptEnable);
    EnableIRQ(SAI2_IRQn);

    SAI_TransferTxCreateHandleEDMA(BOARD_CODEC_SAI, &txSpeakerHandle, txSpeakerCallback, NULL, &dmaTxSpeakerHandle);
    SAI_TransferRxCreateHandleEDMA(BOARD_CODEC_SAI, &rxMicHandle, rxMicCallback, NULL, &dmaRxMicHandle);
    SAI_TransferTxCreateHandleEDMA(BOARD_SCO_SAI, &txMicHandle, txMicCallback, NULL, &dmaTxMicHandle);
    SAI_TransferRxCreateHandleEDMA(BOARD_SCO_SAI, &rxSpeakerHandle, rxSpeakerCallback, NULL, &dmaRxSpeakerHandle);

    /* I2S mode configurations */
    SAI_GetDSPConfig(&codecConfig, kSAI_FrameSyncLenOneBitClk, (sai_word_width_t)bitWidth, kSAI_MonoLeft, 1U << 0);
    codecConfig.frameSync.frameSyncEarly = false;
    codecConfig.syncMode                 = kSAI_ModeAsync;
    codecConfig.masterSlave              = kSAI_Master;
    // codecConfig.serialData.dataWordNum = 1U;
    SAI_TransferTxSetConfigEDMA(BOARD_CODEC_SAI, &txSpeakerHandle, &codecConfig);
    SAI_GetDSPConfig(&codecConfig, kSAI_FrameSyncLenOneBitClk, (sai_word_width_t)bitWidth, kSAI_MonoRight, 1U << 0);
    codecConfig.frameSync.frameSyncEarly = false;
    codecConfig.masterSlave              = kSAI_Master;
    codecConfig.syncMode                 = kSAI_ModeSync;
    SAI_TransferRxSetConfigEDMA(BOARD_CODEC_SAI, &rxMicHandle, &codecConfig);


    /* PCM mode configurations */
    SAI_GetDSPConfig(&scoConfig, kSAI_FrameSyncLenOneBitClk, (sai_word_width_t)bitWidth, kSAI_MonoLeft, 1U << 0);
    scoConfig.bitClock.bclkPolarity       = kSAI_SampleOnFallingEdge;
    scoConfig.frameSync.frameSyncPolarity = kSAI_PolarityActiveHigh;
    scoConfig.frameSync.frameSyncEarly    = true;
    scoConfig.serialData.dataWordNum      = 1U;
    // SAI_GetClassicI2SConfig(&saiConfig, ep_info->sample_len,  kSAI_MonoLeft, 1U << 0);
    scoConfig.syncMode    = kSAI_ModeAsync;
    scoConfig.masterSlave = kSAI_Slave;
    SAI_TransferRxSetConfigEDMA(SAI2, &rxSpeakerHandle, &scoConfig);

    SAI_GetDSPConfig(&scoConfig, kSAI_FrameSyncLenOneBitClk, (sai_word_width_t)bitWidth, kSAI_MonoLeft, 1U << 0);
    scoConfig.bitClock.bclkPolarity       = kSAI_PolarityActiveHigh;
    scoConfig.frameSync.frameSyncPolarity = kSAI_PolarityActiveHigh;
    scoConfig.frameSync.frameSyncEarly    = true;
    scoConfig.serialData.dataWordNum      = 1U;
    scoConfig.syncMode                    = kSAI_ModeSync;
    scoConfig.masterSlave                 = kSAI_Master;
    SAI_TransferTxSetConfigEDMA(SAI2, &txMicHandle, &scoConfig);

#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
    masterCodecClockHz = OVER_SAMPLE_RATE * sampleRate_Hz;
#else
    masterCodecClockHz = DEMO_SAI_CLK_FREQ;
#endif
    /* set bit clock divider */
    SAI_TxSetBitClockRate(BOARD_CODEC_SAI, masterCodecClockHz, samplingRate, bitWidth, DEMO_AUDIO_DATA_CHANNEL);
    SAI_RxSetBitClockRate(BOARD_CODEC_SAI, masterCodecClockHz, samplingRate, bitWidth, DEMO_AUDIO_DATA_CHANNEL);

    NVIC_SetPriority(SAI2_IRQn, 5U);
    NVIC_SetPriority(DMA0_DMA16_IRQn, 5U);
    NVIC_SetPriority(DMA1_DMA17_IRQn, 5U);
    NVIC_SetPriority(DMA2_DMA18_IRQn, 5U);
    NVIC_SetPriority(DMA3_DMA19_IRQn, 5U);
}

static void BOARD_SCO_DMA_EDMA_Config(void)
{
    edma_config_t dmaConfig = {0};

    /* Init DMAMUX */
    DMAMUX_Init(EXAMPLE_DMAMUX);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_MICBUF_TX_CHANNEL, (uint8_t)EXAMPLE_SAI_MICBUF_TX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_MICBUF_TX_CHANNEL);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_MICBUF_RX_CHANNEL, (uint8_t)EXAMPLE_SAI_MICBUF_RX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_MICBUF_RX_CHANNEL);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_SPKBUF_TX_CHANNEL, (uint8_t)EXAMPLE_SAI_SPKBUF_TX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_SPKBUF_TX_CHANNEL);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_SPKBUF_RX_CHANNEL, (uint8_t)EXAMPLE_SAI_SPKBUF_RX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_SPKBUF_RX_CHANNEL);

    /* Init DMA and create handle for DMA */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaTxMicHandle, EXAMPLE_DMA, EXAMPLE_MICBUF_TX_CHANNEL);
    EDMA_CreateHandle(&dmaRxMicHandle, EXAMPLE_DMA, EXAMPLE_MICBUF_RX_CHANNEL);
    EDMA_CreateHandle(&dmaTxSpeakerHandle, EXAMPLE_DMA, EXAMPLE_SPKBUF_TX_CHANNEL);
    EDMA_CreateHandle(&dmaRxSpeakerHandle, EXAMPLE_DMA, EXAMPLE_SPKBUF_RX_CHANNEL);
    //  NVIC_SetPriority((IRQn_Type)EXAMPLE_DMA_IRQ, AUDIO_INTERRUPT_PRIORITY);
}

static void BOARD_SCO_SAI_DeInit(void)
{
    SAI_Deinit(BOARD_CODEC_SAI);
    SAI_Deinit(BOARD_SCO_SAI);
    DisableIRQ(SAI2_IRQn);
}
/*Initialize sco audio interface and codec.*/
static void Init_Board_Sco_Audio(uint32_t samplingRate, UCHAR bitWidth)
{
    PRINTF("Init Audio SCO SAI and CODEC samplingRate :%d  bitWidth:%d \r\n",samplingRate,bitWidth );
    CLOCK_InitAudioPll(&audioCodecPllConfig);

#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7)) 
    /*Clock setting fPRINTFor LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*Clock setting for SAI1*/
    CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);
#endif

    BOARD_SCO_EnableSaiMclkOutput(true);
    wm8960ScoConfig.format.sampleRate = samplingRate;
    BOARD_SCO_DMA_EDMA_Config();
    BOARD_SCO_SAI_Init(samplingRate, bitWidth);
    BOARD_SCO_Codec_Init();
}
static void Init_Board_RingTone_Audio(uint32_t samplingRate, UCHAR bitWidth)
{
    PRINTF("Init Audio CODEC for RingTone\r\n");
    sai_transceiver_t config;
    edma_config_t dmaConfig = {0};
    CLOCK_InitAudioPll(&audioCodecPllConfig);

#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7)) 
    /*Clock setting fPRINTFor LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*Clock setting for SAI1*/
    CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);
#endif

    BOARD_SCO_EnableSaiMclkOutput(true);
    wm8960ScoConfig.format.sampleRate = samplingRate;
    wm8960ScoConfig.bus =  kWM8960_BusI2S;

    /* Init DMAMUX */
    DMAMUX_Init(EXAMPLE_DMAMUX);

    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_SPKBUF_TX_CHANNEL, (uint8_t)EXAMPLE_SAI_SPKBUF_TX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_SPKBUF_TX_CHANNEL);

    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);

    EDMA_CreateHandle(&dmaTxSpeakerHandle, EXAMPLE_DMA, EXAMPLE_SPKBUF_TX_CHANNEL);

    SAI_Init(BOARD_CODEC_SAI);
    SAI_TransferTxCreateHandleEDMA(BOARD_CODEC_SAI, &txSpeakerHandle, txSpeakerCallback, NULL, &dmaTxSpeakerHandle);
    SAI_GetClassicI2SConfig(&config, (sai_word_width_t)bitWidth, kSAI_Stereo, 1U << 0);
    config.syncMode    = kSAI_ModeAsync;
    config.masterSlave = kSAI_Master;
    SAI_TransferTxSetConfigEDMA(BOARD_CODEC_SAI, &txSpeakerHandle, &config);
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
    masterCodecClockHz = OVER_SAMPLE_RATE * sampleRate_Hz;
#else
    masterCodecClockHz = DEMO_SAI_CLK_FREQ;
#endif
    /* set bit clock divider */
    SAI_TxSetBitClockRate(BOARD_CODEC_SAI, masterCodecClockHz, samplingRate, bitWidth, DEMO_AUDIO_DATA_CHANNEL);

    BOARD_SCO_Codec_Init();
}

static API_RESULT board_audio_setup_pl_ext(uint8_t isRing, SCO_AUDIO_EP_INFO *ep_info)
{
    txMic_index     = 0U;
    rxMic_index     = 0U;
    emptyMicBlock   = BUFFER_NUMBER;
    txSpeaker_index = 0U, rxSpeaker_index = 0U;
    emptySpeakerBlock = BUFFER_NUMBER;

#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7)) 
    BOARD_InitScoPins();
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
    BOARD_InitM2ScoPins();
#endif

    if (isRing)
    {
        Init_Board_RingTone_Audio(ep_info->sampl_freq, ep_info->sample_len);
    }
    else
    {
        Init_Board_Sco_Audio(ep_info->sampl_freq, ep_info->sample_len);
    }
    return API_SUCCESS;
}

static uint32_t count = 0;

void SCO_Edma_Task(void *handle)
{
    sai_transfer_t xfer;

    while (1)
    {
        OSA_SemaphoreWait(xSemaphoreScoAudio, osaWaitForever_c);
        count++;
#ifdef SCO_DEBUG_MSG
        if (count % 300 == 0)
        {
            PRINTF("@(%d  %d)", emptyMicBlock, emptySpeakerBlock);
            PRINTF("#( %d %d)", rxSpeaker_test, rxMic_test);
        }
#endif
#if SCO_SAI_LOOPBACK
        if (emptySpeakerBlock > 0)
        {
            xfer.data     = SpeakerBuffer + rxSpeaker_index * BUFFER_SIZE;
            xfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferReceiveEDMA(BOARD_SCO_SAI, &rxSpeakerHandle, &xfer))
            {
                rxSpeaker_index++;
            }
            if (rxSpeaker_index == BUFFER_NUMBER)
            {
                rxSpeaker_index = 0U;
            }
        }
        if (emptySpeakerBlock < BUFFER_NUMBER)
        {
            xfer.data     = SpeakerBuffer + txSpeaker_index * BUFFER_SIZE;
            xfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferSendEDMA(BOARD_SCO_SAI, &txMicHandle, &xfer))
            {
                txSpeaker_index++;
            }
            if (txSpeaker_index == BUFFER_NUMBER)
            {
                txSpeaker_index = 0U;
            }
        }
#else
        if (emptyMicBlock > 0U)
        {
            xfer.data     = MicBuffer + rxMic_index * BUFFER_SIZE;
            xfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferReceiveEDMA(BOARD_CODEC_SAI, &rxMicHandle, &xfer))
            {
                rxMic_index++;
            }
            if (rxMic_index == BUFFER_NUMBER)
            {
                rxMic_index = 0U;
            }
        }
#if CODEC_SAI_LOOPBACK
        if (emptyMicBlock < BUFFER_NUMBER)
        {
            xfer.data     = MicBuffer + txMic_index * BUFFER_SIZE;
            xfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferSendEDMA(BOARD_CODEC_SAI, &txSpeakerHandle, &xfer))
            {
                txMic_index++;
            }
            if (txMic_index == BUFFER_NUMBER)
            {
                txMic_index = 0U;
            }
        }
#else
        if (emptySpeakerBlock > 0U)
        {
            xfer.data     = SpeakerBuffer + rxSpeaker_index * BUFFER_SIZE;
            xfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferReceiveEDMA(BOARD_SCO_SAI, &rxSpeakerHandle, &xfer))
            {
                rxSpeaker_index++;
            }
            if (rxSpeaker_index == BUFFER_NUMBER)
            {
                rxSpeaker_index = 0U;
            }
        }

#endif
#endif
    }
}
void sco_audio_init_pl_ext(void)
{
    return;
}

void sco_audio_shutdown_pl_ext(void)
{
    return;
}

API_RESULT sco_audio_setup_pl_ext(SCO_AUDIO_EP_INFO *ep_info)
{
    sco_audio_setup = 1;
    if(s_ringTone == 0U)
    {
        board_audio_setup_pl_ext(false, ep_info);
    }
    memcpy(&s_ep_info, ep_info, sizeof(SCO_AUDIO_EP_INFO));
    return API_SUCCESS;
}

API_RESULT sco_audio_start_pl_ext(void)
{
    static uint32_t taskCreated = 0;
    sai_transfer_t xfer;
    BaseType_t result = 0;
    if(s_ringTone == 1U)
    {
        s_ringTone = 0;
        return API_SUCCESS;
    }
    if (taskCreated == 0)
    {
        OSA_SemaphoreCreate(xSemaphoreScoAudio, 0);
        result = xTaskCreate(SCO_Edma_Task, "SCO_Edma", 1024, NULL, 5, NULL);
        assert(pdPASS == result);
        taskCreated = 1U;
        (void)result;
    }
    for (uint8_t index = 0; index < BUFFER_NUMBER; ++index)
    {
        xfer.data     = MicBuffer + rxMic_index * BUFFER_SIZE;
        xfer.dataSize = BUFFER_SIZE;
        if (kStatus_Success == SAI_TransferReceiveEDMA(BOARD_CODEC_SAI, &rxMicHandle, &xfer))
        {
            rxMic_index++;
        }
        if (rxMic_index == BUFFER_NUMBER)
        {
            rxMic_index = 0U;
        }
    }
    emptyMicBlock = BUFFER_NUMBER;

    for (uint8_t index = 0; index < BUFFER_NUMBER; ++index)
    {
        xfer.data     = SpeakerBuffer + rxSpeaker_index * BUFFER_SIZE;
        xfer.dataSize = BUFFER_SIZE;
        if (kStatus_Success == SAI_TransferReceiveEDMA(BOARD_SCO_SAI, &rxSpeakerHandle, &xfer))
        {
            rxSpeaker_index++;
        }
        if (rxSpeaker_index == BUFFER_NUMBER)
        {
            rxSpeaker_index = 0U;
        }
    }
    emptySpeakerBlock = BUFFER_NUMBER;
    BOARD_SCO_EnableSaiMclkOutput(true);

    /* play dummy data to codec */
    xfer.dataSize = AUDIO_DUMMY_SIZE;
    xfer.data     = (uint8_t *)&g_AudioTxDummyBuffer[0];
    SAI_TransferSendEDMA(BOARD_CODEC_SAI, &txSpeakerHandle, &xfer);

    /* play dummy data to 8978 */
    xfer.dataSize = AUDIO_DUMMY_SIZE;
    xfer.data     = (uint8_t *)&g_AudioTxDummyBuffer[0];
    SAI_TransferSendEDMA(BOARD_SCO_SAI, &txMicHandle, &xfer);

    return API_SUCCESS;
}

API_RESULT sco_audio_stop_pl_ext(void)
{
    BOARD_SCO_EnableSaiMclkOutput(false);
    if (sco_audio_setup == 1)
    {
        BOARD_SCO_SAI_DeInit();
    }
    sco_audio_setup = 0;
    EM_usleep(200U*1000U );
    return API_SUCCESS;
}

#ifdef HCI_SCO
void sco_audio_spkr_play_pl_ext(UCHAR *m_data, UINT16 m_datalen)
{
    /* Write to Codec */
}
#endif /* HCI_SCO */

API_RESULT platform_audio_play_outgoing_call_ringtone(void)
{
    sai_transfer_t xfer;
    emptyMicBlock = BUFFER_NUMBER;
    if (s_ringTone == 0)
    {
        printf("platform_audio_outgoing_call_ringtone() emptyMicBlock %d\n", emptyMicBlock);
        board_audio_setup_pl_ext(false, &s_ep_info);
        BOARD_SCO_EnableSaiMclkOutput(false);
        s_ringTone = 1;
    }
    if (sco_audio_setup == 1)
    {
        memset(MicBuffer, 0x0, BUFFER_NUMBER * BUFFER_SIZE);
        if ((emptyMicBlock > 0U) && (cpy_index < MUSIC_LEN / BUFFER_SIZE))
        {
            /* Fill in the buffers. */
            memcpy((uint8_t *)&MicBuffer[BUFFER_SIZE * (tx_index % BUFFER_NUMBER)],
            (uint8_t *)&music[cpy_index * BUFFER_SIZE], sizeof(uint8_t) * BUFFER_SIZE);
            emptyMicBlock--;
            cpy_index++;
            if (cpy_index > (MUSIC_LEN / BUFFER_SIZE))
            {
                cpy_index = 0U;
            }
        }
        if (emptyMicBlock < BUFFER_NUMBER)
        {
            /*  xfer structure */
            xfer.data     = (uint8_t *)&MicBuffer[BUFFER_SIZE * (tx_index % BUFFER_NUMBER)];
            xfer.dataSize = BUFFER_SIZE;
            /* Wait for available queue. */
            if (kStatus_Success == SAI_TransferSendEDMA(BOARD_SCO_SAI, &txMicHandle, &xfer))
            {
                tx_index++;
                if (tx_index == BUFFER_NUMBER)
                {
                    tx_index = 0U;
                }
            }
            emptyMicBlock++;
        }
    }
    return API_SUCCESS;
}

API_RESULT platform_audio_play_ringtone(void)
{
    cpy_index  = 0;
    tx_index = 0U;
    sai_transfer_t xfer;
    emptySpeakerBlock = BUFFER_NUMBER;
    if (s_ringTone == 0)
    {
        if (sco_audio_setup == 1)
        {
            BOARD_SCO_EnableSaiMclkOutput(false);
            BOARD_SCO_SAI_DeInit();
        }

        SCO_AUDIO_EP_INFO ep_info;
        ep_info.sampl_freq = 16000U;
        ep_info.sample_len = 16U;
        board_audio_setup_pl_ext(true, &ep_info);
        s_ringTone = 1;
    }


    memset(SpeakerBuffer, 0x0, BUFFER_NUMBER * BUFFER_SIZE);
    if ((emptySpeakerBlock > 0U) && (cpy_index < MUSIC_LEN / BUFFER_SIZE))
    {
        /* Fill in the buffers. */
        memcpy((uint8_t *)&SpeakerBuffer[BUFFER_SIZE * (cpy_index % BUFFER_NUMBER)],
               (uint8_t *)&music[cpy_index * BUFFER_SIZE], sizeof(uint8_t) * BUFFER_SIZE);
        emptySpeakerBlock--;
        cpy_index++;
    }
    if (emptySpeakerBlock < BUFFER_NUMBER)
    {
        /*  xfer structure */
        xfer.data     = (uint8_t *)&SpeakerBuffer[BUFFER_SIZE * (tx_index % BUFFER_NUMBER)];
        xfer.dataSize = BUFFER_SIZE;
        /* Wait for available queue. */
        if (kStatus_Success == SAI_TransferSendEDMA(BOARD_CODEC_SAI, &txSpeakerHandle, &xfer))
        {
            tx_index++;
        }
        emptySpeakerBlock++;
    }
    return API_SUCCESS;
}

API_RESULT sco_audio_set_speaker_volume(UCHAR volume)
{
  if(sco_audio_setup == 0)
  {
    return API_FAILURE;
  }
  /* HFP support 0- 15, codec support 0-100*/
  if( kStatus_Success == CODEC_SetVolume(&codecScoHandle, kSAI_MonoLeft, ((volume*6U)+9U)))
  {
      if( kStatus_Success == CODEC_SetVolume(&codecScoHandle, kSAI_MonoRight, ((volume*6U)+9U)))
      {
          return API_SUCCESS;
      }
  }

  return API_FAILURE;
}

void sco_audio_play_ringtone_pl_ext (void)
{
  platform_audio_play_ringtone();
}

void sco_audio_play_outgoing_ringtone_pl_ext (void)
{
  platform_audio_play_outgoing_call_ringtone();
}

void sco_audio_play_ringtone_exit_pl_ext(void)
{
   if (s_ringTone == 1)
   {
        BOARD_SCO_EnableSaiMclkOutput(false);
        BOARD_SCO_SAI_DeInit();
        memset(SpeakerBuffer, 0x0, BUFFER_NUMBER * BUFFER_SIZE);
        memset(MicBuffer, 0x0, BUFFER_NUMBER * BUFFER_SIZE);
        emptyMicBlock = 0U;
        s_ringTone = 0U;
        if (sco_audio_setup == 1)
        {
            printf("sco_audio_play_ringtone_exit_pl_ext\n");
            board_audio_setup_pl_ext(false, &s_ep_info);
            sco_audio_start_pl_ext();
        }
    }
}
