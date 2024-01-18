/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Template file to be implemented */

#include "streamer_pcm.h"
#include "fsl_debug_console.h"

// AT_NONCACHEABLE_SECTION_INIT(static pcm_rtos_t pcmHandle) = {0};

/*! @brief Define IRQ handlers and callbacks here */

/*! @brief Initialization placeholder */
void streamer_pcm_init(void)
{

}

/*! @brief TX transfer creation */
int streamer_pcm_tx_open(uint32_t num_buffers)
{
    return 0;
}

/*! @brief RX transfer creation */
int streamer_pcm_rx_open(uint32_t num_buffers)
{
    return 0;
}

/*! @brief TX transfer closure */
void streamer_pcm_tx_close(void)
{

}

/*! @brief RX transfer closure */
void streamer_pcm_rx_close(void)
{

}

int streamer_pcm_write(uint8_t *data, uint32_t size)
{
    return 0;
}

int streamer_pcm_read(uint8_t *data, uint32_t size)
{
    return 0;
}

/*! @brief Map an integer sample rate (Hz) to internal SAI enum
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
*/

/*! @brief Map an integer bit width (bits) to internal SAI enum
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
*/

/*! @brief Map an integer number of channels to internal SAI enum
static sai_mono_stereo_t _pcm_map_channels(uint8_t num_channels)
{
    if (num_channels >= 2)
        return kSAI_Stereo;
    else
        return kSAI_MonoRight;
}
*/

int streamer_pcm_setparams(
    uint32_t sample_rate, uint32_t bit_width, uint8_t num_channels, bool transfer, bool dummy_tx, int volume)
{
    return 0;
}

void streamer_pcm_getparams(uint32_t *sample_rate, uint32_t *bit_width, uint8_t *num_channels)
{
    // *sample_rate  = pcmHandle.sample_rate;
    // *bit_width    = pcmHandle.bit_width;
    // *num_channels = pcmHandle.num_channels;
}

int streamer_pcm_mute(bool mute)
{
    // CODEC_SetMute(&codecHandle, DEMO_CODEC_CHANNEL, mute);

    return 0;
}

int streamer_pcm_set_volume(int volume)
{
    // int channel;

    // channel = (pcmHandle.num_channels == 1) ? kCODEC_PlayChannelHeadphoneLeft : DEMO_CODEC_CHANNEL;

    // if (volume <= 0)
    //     CODEC_SetMute(&codecHandle, channel, true);
    // else
    //     CODEC_SetVolume(&codecHandle, channel, volume > CODEC_VOLUME_MAX_VALUE ? CODEC_VOLUME_MAX_VALUE : volume);

    return 0;
}

int streamer_set_master_clock(int sample_rate)
{
//     int master_clock;
// #if (defined(DEMO_CODEC_CS42448) && (DEMO_CODEC_CS42448 == 1))
//     int divider    = DEMO_SAI1_CLOCK_SOURCE_DIVIDER;
//     int predivider = DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER;
// #endif

//     if (sample_rate % 8000 == 0 || sample_rate % 6000 == 0)
//     {
//         /* Configure Audio PLL clock to 786.432 MHz to  to be divisible by 48000 Hz */
//         const clock_audio_pll_config_t audioPllConfig48 = {
//             .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
//             .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
//             .numerator   = 96,  /* 30 bit numerator of fractional loop divider. */
//             .denominator = 125, /* 30 bit denominator of fractional loop divider */
//         };
//         CLOCK_InitAudioPll(&audioPllConfig48);
//     }
//     else
//     {
//         /* Configure Audio PLL clock to 722.5344 MHz to be divisible by 44100 Hz */
//         const clock_audio_pll_config_t audioPllConfig = {
//             .loopDivider = 30,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
//             .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
//             .numerator   = 66,  /* 30 bit numerator of fractional loop divider. */
//             .denominator = 625, /* 30 bit denominator of fractional loop divider */
//         };
//         CLOCK_InitAudioPll(&audioPllConfig);
//     }

// #if (defined(DEMO_CODEC_CS42448) && (DEMO_CODEC_CS42448 == 1))
//     switch (sample_rate)
//     {
//         case 11025:
//         case 12000:
//         case 24000:
//         {
//             divider = 15;
//             break;
//         }
//         case 8000:
//         {
//             divider = 23;
//             break;
//         }
//         case 16000:
//         {
//             divider = 11;
//             break;
//         }
//         case 32000:
//         {
//             divider = 5;
//             break;
//         }
//         case 22050:
//         case 44100:
//         case 48000:
//         default:
//             divider = 7;
//             break;
//     }

//     CLOCK_SetDiv(kCLOCK_Sai1PreDiv, predivider);
//     CLOCK_SetDiv(kCLOCK_Sai1Div, divider);
//     master_clock = CLOCK_GetFreq(kCLOCK_AudioPllClk) / (divider + 1U) / (predivider + 1U);
// #else
//     master_clock = DEMO_SAI_CLK_FREQ;
// #endif
//     return master_clock;
    return 0;
}
