/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "osa_common.h"

#include "board.h"
#include "streamer_pcm_app.h"
#include "fsl_codec_common.h"
#include "fsl_wm8960.h"
#include "app_definitions.h"
#include "osa_memory.h"
#include "fsl_cache.h"

AT_NONCACHEABLE_SECTION_INIT(static pcm_rtos_t pcmHandle) = {0};
extern codec_handle_t codecHandle;

/*! @brief SAI Transmit IRQ handler.
 *
 * This function is used to handle or clear error state.
 */
static void SAI_UserTxIRQHandler(void)
{
    /* Clear the FEF (Tx FIFO underrun) flag. */
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    SAI_TxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);

    SDK_ISR_EXIT_BARRIER;
}

/*! @brief SAI IRQ handler.
 *
 * This function checks FIFO overrun/underrun errors and clears error state.
 */
void SAI1_IRQHandler(void)
{
    if (DEMO_SAI->TCSR & kSAI_FIFOErrorFlag)
        SAI_UserTxIRQHandler();
}

/*! @brief SAI EDMA transmit callback
 *
 * This function is called by the EDMA interface after a block of data has been
 * successfully written to the SAI.
 */
static void saiTxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    pcm_rtos_t *pcm = (pcm_rtos_t *)userData;
    BaseType_t reschedule;
    xSemaphoreGiveFromISR(pcm->semaphoreTX, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

void streamer_pcm_init(void)
{
    edma_config_t dmaConfig;

    NVIC_SetPriority(LPI2C1_IRQn, 5);

    NVIC_SetPriority(DEMO_SAI_TX_IRQ, 5U);

    NVIC_SetPriority(DMA1_DMA17_IRQn, 4U);
    NVIC_SetPriority(DMA0_DMA16_IRQn, 4U);

    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_DMA, &dmaConfig);
    /* Create DMA handle. */
    EDMA_CreateHandle(&pcmHandle.dmaTxHandle, DEMO_DMA, DEMO_TX_CHANNEL);
    /* SAI init */
    SAI_Init(DEMO_SAI);

    EnableIRQ(DEMO_SAI_TX_IRQ);
}

pcm_rtos_t *streamer_pcm_open(uint32_t num_buffers)
{
    pcmHandle.semaphoreTX = xSemaphoreCreateBinary();
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &pcmHandle.saiTxHandle, saiTxCallback, (void *)&pcmHandle,
                                   &pcmHandle.dmaTxHandle);
    return &pcmHandle;
}

pcm_rtos_t *streamer_pcm_rx_open(uint32_t num_buffers)
{
    return 0;
}

void streamer_pcm_start(pcm_rtos_t *pcm)
{
    /* Interrupts already enabled - nothing to do.
     * App/streamer can begin writing data to SAI. */
}

void streamer_pcm_close(pcm_rtos_t *pcm)
{
    /* Stop playback.  This will flush the SAI transmit buffers. */
    SAI_TransferTerminateSendEDMA(DEMO_SAI, &pcm->saiTxHandle);
    vSemaphoreDelete(pcmHandle.semaphoreTX);
}

void streamer_pcm_rx_close(pcm_rtos_t *pcm)
{
}

int streamer_pcm_write(pcm_rtos_t *pcm, uint8_t *data, uint32_t size)
{
    /* Ensure write size is a multiple of 32, otherwise EDMA will assert
     * failure.  Round down for the last chunk of a file/stream. */
    pcm->saiTx.dataSize = size - (size % 32);
    pcm->saiTx.data     = data;

    DCACHE_CleanByRange((uint32_t)pcm->saiTx.data, pcm->saiTx.dataSize);

    if (pcm->isFirstTx)
    {
        pcm->isFirstTx = 0;
    }
    else
    {
        /* Wait for the previous transfer to finish */
        if (xSemaphoreTake(pcm->semaphoreTX, portMAX_DELAY) != pdTRUE)
            return -1;
    }

    /* Start the consecutive transfer */
    SAI_TransferSendEDMA(DEMO_SAI, &pcm->saiTxHandle, &pcm->saiTx);

    return 0;
}

int streamer_pcm_read(pcm_rtos_t *pcm, uint8_t *data, uint32_t size)
{
    return 0;
}

/*! @brief Map an integer sample rate (Hz) to internal SAI enum */
static sai_sample_rate_t _pcm_map_sample_rate(uint32_t sample_rate)
{
    switch (sample_rate)
    {
        case 8000:
            return kSAI_SampleRate8KHz;
        case 11025:
            return kSAI_SampleRate11025Hz;
        case 12000:
            return kSAI_SampleRate12KHz;
        case 16000:
            return kSAI_SampleRate16KHz;
        case 24000:
            return kSAI_SampleRate24KHz;
        case 22050:
            return kSAI_SampleRate22050Hz;
        case 32000:
            return kSAI_SampleRate32KHz;
        case 44100:
            return kSAI_SampleRate44100Hz;
        case 48000:
        default:
            return kSAI_SampleRate48KHz;
    }
}

/*! @brief Map an integer bit width (bits) to internal SAI enum */
static sai_word_width_t _pcm_map_word_width(uint32_t bit_width)
{
    switch (bit_width)
    {
        case 8:
            return kSAI_WordWidth8bits;
        case 16:
            return kSAI_WordWidth16bits;
        case 24:
            return kSAI_WordWidth24bits;
        case 32:
            return kSAI_WordWidth32bits;
        default:
            return kSAI_WordWidth16bits;
    }
}

/*! @brief Map an integer number of channels to internal SAI enum */
static sai_mono_stereo_t _pcm_map_channels(uint8_t num_channels)
{
    if (num_channels >= 2)
        return kSAI_Stereo;
    else
        return kSAI_MonoRight;
}

int streamer_pcm_setparams(pcm_rtos_t *pcm,
                           uint32_t sample_rate,
                           uint32_t bit_width,
                           uint8_t num_channels,
                           bool transfer,
                           bool dummy_tx,
                           int volume)
{
    sai_transfer_format_t format = {0};
    sai_transceiver_t saiConfig;
    uint32_t masterClockHz = 0U;

    pcm->isFirstTx       = transfer ? 1U : pcm->isFirstTx;
    pcm->sample_rate     = sample_rate;
    pcm->bit_width       = bit_width;
    pcm->num_channels    = num_channels;
    pcm->dummy_tx_enable = dummy_tx;

    masterClockHz = streamer_set_master_clock(sample_rate);

    format.channel       = 0U;
    format.bitWidth      = _pcm_map_word_width(bit_width);
    format.sampleRate_Hz = _pcm_map_sample_rate(sample_rate);
    format.stereo        = _pcm_map_channels(num_channels);
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    format.masterClockHz = masterClockHz;
#endif

    SAI_GetClassicI2SConfig(&saiConfig, _pcm_map_word_width(bit_width), format.stereo, 1U << DEMO_SAI_CHANNEL);
    saiConfig.syncMode    = kSAI_ModeAsync;
    saiConfig.masterSlave = kSAI_Master;

    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &pcmHandle.saiTxHandle, &saiConfig);
    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, masterClockHz, _pcm_map_sample_rate(sample_rate), _pcm_map_word_width(bit_width),
                          DEMO_CHANNEL_NUM);
    /* Enable SAI transmit and FIFO error interrupts. */
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);

    streamer_pcm_set_volume(pcm, 0);

    CODEC_SetFormat(&codecHandle, masterClockHz, format.sampleRate_Hz, format.bitWidth);

    streamer_pcm_set_volume(pcm, volume);

    return 0;
}

void streamer_pcm_getparams(pcm_rtos_t *pcm, uint32_t *sample_rate, uint32_t *bit_width, uint8_t *num_channels)
{
    *sample_rate  = pcm->sample_rate;
    *bit_width    = pcm->bit_width;
    *num_channels = pcm->num_channels;
}

int streamer_pcm_mute(pcm_rtos_t *pcm, bool mute)
{
    CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, mute);

    return 0;
}

int streamer_pcm_set_volume(pcm_rtos_t *pcm, int volume)
{
    int channel;

    channel = (pcm->num_channels == 1) ? kCODEC_PlayChannelHeadphoneLeft :
                                         kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight;
    if (volume <= 0)
        CODEC_SetMute(&codecHandle, channel, true);
    else
        CODEC_SetVolume(&codecHandle, channel, volume > CODEC_VOLUME_MAX_VALUE ? CODEC_VOLUME_MAX_VALUE : volume);

    return 0;
}

int streamer_set_master_clock(int sample_rate)
{
    int master_clock;
#if DEMO_CODEC_CS42448
    int divider    = DEMO_SAI1_CLOCK_SOURCE_DIVIDER;
    int predivider = DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER;
#endif

    if (sample_rate % 8000 == 0 || sample_rate % 6000 == 0)
    {
        /* Configure Audio PLL clock to 786.432 MHz to  to be divisible by 48000 Hz */
        const clock_audio_pll_config_t audioPllConfig48 = {
            .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
            .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
            .numerator   = 96,  /* 30 bit numerator of fractional loop divider. */
            .denominator = 125, /* 30 bit denominator of fractional loop divider */
        };
        CLOCK_InitAudioPll(&audioPllConfig48);
    }
    else
    {
        /* Configure Audio PLL clock to 722.5344 MHz to be divisible by 44100 Hz */
        const clock_audio_pll_config_t audioPllConfig = {
            .loopDivider = 30,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
            .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
            .numerator   = 66,  /* 30 bit numerator of fractional loop divider. */
            .denominator = 625, /* 30 bit denominator of fractional loop divider */
        };
        CLOCK_InitAudioPll(&audioPllConfig);
    }
#if DEMO_CODEC_CS42448
    switch (sample_rate)
    {
        case 11025:
        case 12000:
        case 24000:
        {
            divider = 63;
            break;
        }
        case 8000:
        {
            predivider = 1;
        }
        case 16000:
        {
            divider = 47;
            break;
        }
        case 32000:
        {
            divider = 23;
            break;
        }
        case 22050:
        case 44100:
        case 48000:
        default:
            divider = 31;
            break;
    }

    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, predivider);
    CLOCK_SetDiv(kCLOCK_Sai1Div, divider);
    master_clock = CLOCK_GetFreq(kCLOCK_AudioPllClk) / (divider + 1U) / (predivider + 1U);
#else
    master_clock = DEMO_SAI_CLK_FREQ;
#endif
    return master_clock;
}
