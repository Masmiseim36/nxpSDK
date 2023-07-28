/*
 * Copyright 2018-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    audio_src_pcmrtos.c
 * @brief   This file implement audio source element of pcmrtos.
 */

#include "pipeline.h"
#include "audio_cfg.h"
#include "audio_src_pcmrtos.h"
#include <string.h>

/*!
 * @param audio_src_element: pointer to an audio src instance
 *
 * @brief This function initializes the PCM manager device parameters.
 *
 * @returns AudioSrcStreamErrorType
 * @retval AUDIO_SRC_SUCCESS successful
 * @retval AUDIO_SRC_ERROR_INVALID_ARGS invalid argument
 * @retval AUDIO_SRC_FAILED failed to get pcm manager file handle
 */
AudioSrcStreamErrorType audiosrc_pcmrtos_init_params(ElementAudioSrc *audio_src_element)
{
    PCMRtosDeviceInfo *dev_info;
    int ret;
    uint32_t dev_sample_rate;
    uint32_t dev_bit_width;
    uint8_t dev_num_channels;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    dev_info = (PCMRtosDeviceInfo *)audio_src_element->device_info;

    if (!dev_info)
    {
        STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_NOT_SUPPORTED, "[Audio SRC] device_info or pcm_handle is null\n");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_FAILED;
    }

    ret = streamer_pcm_setparams(dev_info->pcm_handle, audio_src_element->sample_rate,
                                 audio_src_element->pkt_hdr.bits_per_sample, audio_src_element->pkt_hdr.num_channels,
                                 false, audio_src_element->dummy_tx, 0);
    if (ret != 0)
    {
        STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_NOT_SUPPORTED, "[Audio SRC] failed to set device params\n");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_FAILED;
    }

    streamer_pcm_getparams(dev_info->pcm_handle, &dev_sample_rate, &dev_bit_width, &dev_num_channels);

    if (dev_bit_width != audio_src_element->pkt_hdr.bits_per_sample ||
        dev_num_channels != audio_src_element->pkt_hdr.num_channels)
    {
        STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_NOT_SUPPORTED,
                         "[Audio SRC] unsupported PCM device parameters: bit width and/or channels number \n");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_FAILED;
    }

    dev_info->init_params_done = true;

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);

    return AUDIOSRC_SUCCESS;
}

AudioSrcStreamErrorType audio_src_pcmrtos_init_device(ElementAudioSrc *audio_src_element)
{
    PCMRtosDeviceInfo *dev_info;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    if (audio_src_element == NULL)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "Audio SRC element is NULL");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_ERROR_INVALID_ARGS;
    }

    dev_info = (PCMRtosDeviceInfo *)audio_src_element->device_info;

    dev_info = OSA_MemoryAllocate(sizeof(PCMRtosDeviceInfo));
    if (!dev_info)
        return AUDIOSRC_ERROR_OUT_OF_HEAP_MEMORY;

    memset(dev_info, 0, sizeof(PCMRtosDeviceInfo));

    dev_info->device_state    = AUDIO_SRC_DEVICE_STATE_OPENED;
    dev_info->pcm_handle      = NULL;
    dev_info->continuous_read = true;

    audio_src_element->device_info = dev_info;

    STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "Audio SRC inited");

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);

    return AUDIOSRC_SUCCESS;
}

AudioSrcStreamErrorType audio_src_pcmrtos_deinit_device(ElementAudioSrc *audio_src_element)
{
    PCMRtosDeviceInfo *dev_info;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    if (audio_src_element == NULL)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "Audio SRC element is NULL");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_ERROR_INVALID_ARGS;
    }

    if (audio_src_element->device_info == NULL)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "Device info is NULL");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_ERROR_DEVICE_CLOSE_FAILED;
    }

    STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, audio_src_element->device_info);

    dev_info = (PCMRtosDeviceInfo *)audio_src_element->device_info;

    if (!dev_info)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "Device info is NULL");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_ERROR_DEVICE_CLOSE_FAILED;
    }

    dev_info->device_state = AUDIO_SRC_DEVICE_STATE_CLOSED;

    OSA_MemoryFree(audio_src_element->device_info);
    audio_src_element->device_info = NULL;

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);

    return AUDIOSRC_SUCCESS;
}

AudioSrcStreamErrorType audio_src_pcmrtos_start_device(ElementAudioSrc *audio_src_element)
{
    PCMRtosDeviceInfo *dev_info;
    uint16_t alloc_size = 0;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    if (audio_src_element == NULL)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "Device info is NULL");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_ERROR_INVALID_ARGS;
    }

    dev_info = (PCMRtosDeviceInfo *)audio_src_element->device_info;

    if (!dev_info)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_ERROR_INVALID_ARGS;
    }

    dev_info->pcm_handle = streamer_pcm_rx_open(AUDIO_SRC_BUFFER_NUM);

    if (!dev_info->pcm_handle)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "Open file handler failed\n");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_FAILED;
    }

    audio_src_element->chunk_size = audio_src_element->pkt_hdr.sample_rate *
                                    audio_src_element->pkt_hdr.bits_per_sample / 8 *
                                    audio_src_element->pkt_hdr.num_channels * audio_src_element->frame_ms / 1000;
    audio_src_element->pkt_hdr.chunk_size = audio_src_element->chunk_size;
    audio_src_element->first_run          = true;

    alloc_size = sizeof(AudioPacketHeader) + audio_src_element->chunk_size;
    for (int i = 0; i < AUDIO_SRC_BUFFER_NUM; i++)
    {
        dev_info->unaligned_buf[i] = OSA_MemoryAllocate(alloc_size + SIZE_ALIGNMENT);
        memset(dev_info->unaligned_buf[i], 0, alloc_size + SIZE_ALIGNMENT);
        dev_info->audbuf[i] = (char *)MEM_ALIGN(dev_info->unaligned_buf[i], SIZE_ALIGNMENT);
        if (!dev_info->audbuf[i])
        {
            STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_OUT_OF_MEMORY, "[Audio SRC] Audio buffer malloc failed \n");
            return AUDIOSRC_ERROR_OUT_OF_HEAP_MEMORY;
        }
    }

    streamer_pcm_start(dev_info->pcm_handle);

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);

    return AUDIOSRC_SUCCESS;
}

AudioSrcStreamErrorType audio_src_pcmrtos_stop_device(ElementAudioSrc *audio_src_element)
{
    PCMRtosDeviceInfo *dev_info;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    if (audio_src_element == NULL)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "Device info is NULL");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_ERROR_INVALID_ARGS;
    }

    dev_info = (PCMRtosDeviceInfo *)audio_src_element->device_info;
    if (!dev_info)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_ERROR_INVALID_ARGS;
    }

    streamer_pcm_mute(dev_info->pcm_handle, true);

    streamer_pcm_rx_close(dev_info->pcm_handle);

    for (int i = 0; i < AUDIO_SRC_BUFFER_NUM; i++)
    {
        if (dev_info->unaligned_buf[i])
        {
            OSA_MemoryFree(dev_info->unaligned_buf[i]);
            dev_info->unaligned_buf[i] = NULL;
        }
    }

    dev_info->pcm_handle       = NULL;
    dev_info->init_params_done = false;

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);

    return AUDIOSRC_SUCCESS;
}

AudioSrcStreamErrorType audio_src_pcmrtos_read_device(ElementAudioSrc *audio_src_element,
                                                      uint32_t length,
                                                      StreamBuffer *buf)
{
    AudioPacketHeader *pkt_hdr  = NULL;
    PCMRtosDeviceInfo *dev_info = NULL;
    char *buffer_ptr            = NULL;
    int8_t next_index           = 0;
    int ret;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    if (audio_src_element == NULL)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "Audio SRC element is NULL");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_ERROR_INVALID_ARGS;
    }

    dev_info = (PCMRtosDeviceInfo *)audio_src_element->device_info;

    if (!dev_info)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "Device info is NULL");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_ERROR_INVALID_ARGS;
    }

    if (!dev_info->pcm_handle)
    {
        STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_BUSY, "[Audio SRC] open file handler failed\n");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_FAILED;
    }

    if (!dev_info->init_params_done)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[Audio SRC] Init params\n");

        if (audiosrc_pcmrtos_init_params(audio_src_element) != AUDIOSRC_SUCCESS)
        {
            STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_BUSY, "[Audio SRC] open file handler failed\n");
            STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
            return AUDIOSRC_FAILED;
        }

        dev_info->buffer_queue.read_idx  = 0;
        dev_info->buffer_queue.write_idx = 0;
        dev_info->buffer_queue.size      = sizeof(dev_info->buffer_queue.buffer_idx);

        STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[Audio SRC] Init params done\n");
    }

    pkt_hdr             = (AudioPacketHeader *)(&audio_src_element->pkt_hdr);
    pkt_hdr->chunk_size = audio_src_element->chunk_size;

    next_index                      = ((int8_t)dev_info->buff_index + 1) % AUDIO_SRC_BUFFER_NUM;
    buffer_ptr                      = dev_info->audbuf[next_index];
    dev_info->buff_size[next_index] = pkt_hdr->chunk_size;

    memcpy(buffer_ptr, pkt_hdr, sizeof(AudioPacketHeader));
    ret = streamer_pcm_read(dev_info->pcm_handle, (uint8_t *)(buffer_ptr + sizeof(AudioPacketHeader)),
                            dev_info->buff_size[next_index]);

    dev_info->buffer_queue.buffer_idx[dev_info->buffer_queue.write_idx] = next_index;
    dev_info->buffer_queue.write_idx = (dev_info->buffer_queue.write_idx + 1) % dev_info->buffer_queue.size;

    if (ret < 0)
    {
        STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_BUSY, "[Audio SRC] failed to read data from mic\n");
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_FAILED;
    }
    else if ((ret == 0) && (!audio_src_element->first_run))
    {
        buf->buffer = (int8_t *)dev_info->audbuf[dev_info->buffer_queue.buffer_idx[dev_info->buffer_queue.read_idx]];
        buf->size   = dev_info->buff_size[dev_info->buff_index] + sizeof(AudioPacketHeader);
        dev_info->buffer_queue.read_idx = (dev_info->buffer_queue.read_idx + 1) % dev_info->buffer_queue.size;
    }
    else
    {
        if (audio_src_element->first_run)
        {
            memcpy(dev_info->audbuf[dev_info->buff_index], pkt_hdr, sizeof(AudioPacketHeader));
            audio_src_element->first_run = false;
        }

        /* Buffer with only audioPacket header */
        buf->buffer = (int8_t *)dev_info->audbuf[dev_info->buff_index];
        buf->size   = sizeof(AudioPacketHeader);
    }

    dev_info->buff_index = (dev_info->buff_index + 1U) % (uint8_t)AUDIO_SRC_BUFFER_NUM;

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);

    return AUDIOSRC_SUCCESS;
}
