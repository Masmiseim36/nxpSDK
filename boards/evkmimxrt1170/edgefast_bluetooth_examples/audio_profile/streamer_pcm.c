/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "osa_common.h"
#include "board.h"
#include "app_music_control.h"
#include "streamer_pcm.h"

#define STREAMER_BUFFER_COUNT (6)

pcm_rtos_t pcmHandle = {0};
uint8_t * s_StreamerBufferArray[STREAMER_BUFFER_COUNT];
volatile uint32_t s_StreamerBufferDataLengthArray[STREAMER_BUFFER_COUNT];
volatile uint32_t s_StreamerBufferPosArray[STREAMER_BUFFER_COUNT];
volatile uint8_t s_StreamerBufferWriteIndex = 0;
volatile uint8_t s_StreamerBufferReadIndex = 0;
volatile uint8_t s_StreamerBufferCount = 0;

#if (defined(STREAMER_CODEC_EANBLE) && (STREAMER_CODEC_EANBLE))
extern codec_handle_t codecHandle;
#define OVER_SAMPLE_RATE (384U)
#endif

void streamer_pcm_init(void)
{
}

pcm_rtos_t *streamer_pcm_open(uint32_t num_buffers)
{
    s_StreamerBufferCount = num_buffers;
    for (uint32_t index = 0U; index < num_buffers; ++index)
    {
        s_StreamerBufferDataLengthArray[index] = 0U;
        s_StreamerBufferPosArray[index] = 0U;
    }
    s_StreamerBufferWriteIndex = 0U;
    s_StreamerBufferReadIndex = 0U;
    pcmHandle.streamerStop = 0U;
    pcmHandle.streamerGettingData = 0U;
    return &pcmHandle;
}

pcm_rtos_t *streamer_pcm_rx_open(uint32_t num_buffers)
{
    return NULL;
}

void streamer_pcm_start(pcm_rtos_t *pcm)
{
}

void streamer_pcm_close(pcm_rtos_t *pcm)
{
}

void streamer_pcm_rx_close(pcm_rtos_t *pcm)
{
}

int streamer_pcm_write(pcm_rtos_t *pcm, uint8_t *data, uint32_t size)
{
    while (s_StreamerBufferDataLengthArray[s_StreamerBufferWriteIndex] > 0)
    {
        if (pcmHandle.streamerStop)
        {
            /* discard data, let the task run. */
            return 0;
        }
        osa_time_delay(1);
    }

    s_StreamerBufferArray[s_StreamerBufferWriteIndex] = data;
    s_StreamerBufferDataLengthArray[s_StreamerBufferWriteIndex] = size;
    s_StreamerBufferPosArray[s_StreamerBufferWriteIndex] = 0U;
    s_StreamerBufferWriteIndex++;
    if (s_StreamerBufferWriteIndex >= s_StreamerBufferCount)
    {
        s_StreamerBufferWriteIndex = 0U;
    }

    return 0;
}

int streamer_pcm_read(pcm_rtos_t *pcm, uint8_t *data, uint8_t *next_buffer, uint32_t size)
{
    return 0;
}

int streamer_pcm_setparams(
    pcm_rtos_t *pcm, uint32_t sample_rate, uint32_t bit_width, uint8_t num_channels, bool transfer)
{
    pcm->sample_rate  = sample_rate;
    pcm->bit_width    = bit_width;
    pcm->num_channels = num_channels;

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
    return 0;
}

int streamer_pcm_set_volume(uint32_t volume)
{
    return 0;
}

int streamer_pcm_get_data(uint8_t *dest, uint32_t length)
{
    uint32_t copyLength = 0;
    uint32_t totalLength = 0;

    pcmHandle.streamerGettingData = 1U;
    if (pcmHandle.streamerStop)
    {
        /* send dummy data when stopped */
        memset(dest, 0x00U, length);
        pcmHandle.streamerGettingData = 0U;
        return 1;
    }

    for (uint8_t index = 0; index < s_StreamerBufferCount; ++index)
    {
        totalLength += s_StreamerBufferDataLengthArray[index];
    }

    if (totalLength < length)
    {
        /* send dummy data when stopped */
        memset(dest, 0x00U, length);
        pcmHandle.streamerGettingData = 0U;
        return 1;
    }

    while (copyLength < length)
    {
        if (s_StreamerBufferDataLengthArray[s_StreamerBufferReadIndex] > (length - copyLength))
        {
            memcpy(&dest[copyLength], &s_StreamerBufferArray[s_StreamerBufferReadIndex][s_StreamerBufferPosArray[s_StreamerBufferReadIndex]],
                    (length - copyLength));

            s_StreamerBufferDataLengthArray[s_StreamerBufferReadIndex] -= (length - copyLength);
            s_StreamerBufferPosArray[s_StreamerBufferReadIndex] += (length - copyLength);

            copyLength += (length - copyLength);
        }
        else
        {
            memcpy(&dest[copyLength], &s_StreamerBufferArray[s_StreamerBufferReadIndex][s_StreamerBufferPosArray[s_StreamerBufferReadIndex]],
                                                        s_StreamerBufferDataLengthArray[s_StreamerBufferReadIndex]);
            copyLength += s_StreamerBufferDataLengthArray[s_StreamerBufferReadIndex];

            s_StreamerBufferDataLengthArray[s_StreamerBufferReadIndex] = 0U;
            s_StreamerBufferPosArray[s_StreamerBufferReadIndex] = 0U;

            s_StreamerBufferReadIndex++;
            if (s_StreamerBufferReadIndex >= s_StreamerBufferCount)
            {
                s_StreamerBufferReadIndex = 0U;
            }
        }
    }

    pcmHandle.streamerGettingData = 0U;
    return 1;
}

void streamer_pcm_get_mp3_file_info(uint32_t *sample_rate, uint32_t *bit_width, uint8_t *num_channels)
{
    *sample_rate  = pcmHandle.sample_rate;
    *bit_width    = pcmHandle.bit_width;
    *num_channels = pcmHandle.num_channels;
}

void streamer_pcm_stop(void)
{
    while (pcmHandle.streamerGettingData)
    {
        /*delay 1ms */
        vTaskDelay((((1)*configTICK_RATE_HZ + 999U) / 1000U));
    }
    pcmHandle.streamerStop = 1U;
}
