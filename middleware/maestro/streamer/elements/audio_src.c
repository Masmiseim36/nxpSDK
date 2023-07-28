/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    audio_src.c
 * @brief   This file implement audio source.
 */

/*
 * INCLUDE FILES
 */
#include <string.h>
#include "audio_cfg.h"
#include "streamer_element.h"
#include "audio_src.h"
#include "pipeline.h"
#include "streamer_element_properties.h"

#ifdef CASCFG_PLATFORM_FREERTOS
#include "audio_src_pcmrtos.h"
#endif

/*
 * LOCAL FUNCTION PROTOTYPES
 */
static int32_t audiosrc_src_process(StreamPad *pad);
static FlowReturn audiosrc_src_pull_handler(StreamPad *pad, StreamBuffer *buffer, uint32_t size, uint32_t offset);
static int32_t audiosrc_change_state(StreamElement *element, PipelineState state);
static uint8_t audiosrc_handle_src_event(StreamPad *pad, StreamEvent *event);
static uint8_t audiosrc_handle_src_query(StreamPad *pad, StreamQuery *event);
static uint8_t audiosrc_src_activate_push(StreamPad *pad, uint8_t active);
static uint8_t audiosrc_src_activate_pull(StreamPad *pad, uint8_t active);
static uint8_t audiosrc_src_activate(StreamPad *pad, uint8_t active);
static int32_t audiosrc_get_property(StreamElement *element_ptr, uint16_t prop, uint64_t *val_ptr);
static int32_t audiosrc_set_property(StreamElement *element_ptr, uint16_t prop, uint32_t val);
static int32_t audiosrc_set_device_name(ElementHandle element, const char *device_name, const char *output_device_name);
static int32_t audiosrc_set_device_type(ElementHandle element, uint32_t device_type);
static int32_t audiosrc_set_push_chunk_size(ElementHandle element, uint32_t chunk_size);
static int32_t audiosrc_get_push_chunk_size(ElementHandle element, uint64_t *chunk_size);
static int32_t audiosrc_set_sampling_rate(ElementHandle element, uint32_t sample_rate);
static int32_t audiosrc_set_continuous_read(ElementHandle element, bool continuous_read);
static int32_t audiosrc_set_dummy_tx_enable(ElementHandle element, bool dummy_tx);
static int32_t audiosrc_set_num_channels(ElementHandle element, uint32_t num_channels);

/*
 * LOCAL VARIABLES
 */
/*--------------------- Audio Source devices ----------------------------*/

/**
 * AudioSrcOpenDevice:
 */
static AudioSrcStreamErrorType (*AudioSrcOpenDevice[LAST_AUDIOSRC_DEVICE_DRIVER_TYPE])(ElementAudioSrc *audiosrc) = {

#ifdef CASCFG_PLATFORM_FREERTOS
    audio_src_pcmrtos_init_device,
#endif

};

/**
 * AudioSrcCloseDevice:
 */
static AudioSrcStreamErrorType (*AudioSrcCloseDevice[LAST_AUDIOSRC_DEVICE_DRIVER_TYPE])(ElementAudioSrc *audiosrc) = {
#ifdef CASCFG_PLATFORM_FREERTOS
    audio_src_pcmrtos_deinit_device,
#endif
};

/**
 * AudioSrcStartDevice:
 */
static AudioSrcStreamErrorType (*AudioSrcStartDevice[LAST_AUDIOSRC_DEVICE_DRIVER_TYPE])(ElementAudioSrc *audiosrc) = {
#ifdef CASCFG_PLATFORM_FREERTOS
    audio_src_pcmrtos_start_device,
#endif
};

/**
 * AudioSrcStopDevice:
 */
static AudioSrcStreamErrorType (*AudioSrcStopDevice[LAST_AUDIOSRC_DEVICE_DRIVER_TYPE])(ElementAudioSrc *audiosrc) = {
#ifdef CASCFG_PLATFORM_FREERTOS
    audio_src_pcmrtos_stop_device,
#endif
};

/**
 * AudioSrcReadDevice:
 */
static AudioSrcStreamErrorType (*AudioSrcReadDevice[LAST_AUDIOSRC_DEVICE_DRIVER_TYPE])(ElementAudioSrc *audiosrc,
                                                                                       uint32_t len,
                                                                                       StreamBuffer *buf) = {
#ifdef CASCFG_PLATFORM_FREERTOS
    audio_src_pcmrtos_read_device,
#endif

};

/*
 * LOCAL FUNCTIONS
 */

int32_t audiosrc_init(StreamElement *element)
{
    PadSrc *pad = NULL;
    int32_t i   = 0;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    ElementAudioSrc *audiosrc = (ElementAudioSrc *)element;

    /* Audio source specific initialization */
    memset(&audiosrc->device_name, 0, MAX_DEVICE_NAME_LENGTH);
    strncpy(audiosrc->device_name, "default", 8);
    memset(&audiosrc->pkt_hdr, 0, sizeof(AudioPacketHeader));

    /* set element change state function */
    audiosrc->change_state            = audiosrc_change_state;
    audiosrc->device_driver_type      = (uint32_t)AUDIOSRC_PCMRTOS;
    audiosrc->pkt_hdr.id              = AUDIO_DATA;
    audiosrc->pkt_hdr.sign            = AUDIOSRC_DEFAULT_SIGN;
    audiosrc->pkt_hdr.sample_rate     = AUDIOSRC_DEFAULT_SAMPLE_RATE;
    audiosrc->pkt_hdr.num_channels    = AUDIOSRC_DEFAULT_NUM_CHANNELS;
    audiosrc->pkt_hdr.endian          = AF_LITTLE_ENDIAN;
    audiosrc->pkt_hdr.bits_per_sample = AUDIOSRC_DEFAULT_BITS_PER_SAMPLE;
    audiosrc->pkt_hdr.chunk_size      = AUDIOSRC_DEFAULT_CHUNK_SIZE;
    audiosrc->chunk_size              = AUDIOSRC_DEFAULT_CHUNK_SIZE;
    audiosrc->sample_rate             = AUDIOSRC_DEFAULT_SAMPLE_RATE;
    audiosrc->frame_ms                = AUDIOSRC_DEFAULT_FRAME_MS;
    audiosrc->buffer                  = NULL;
    audiosrc->end_of_stream           = false;
    audiosrc->retries                 = 0;
    audiosrc->continuous_read         = false;

    /* set element change state function */
    audiosrc->change_state = audiosrc_change_state;
    audiosrc->set_property = audiosrc_set_property;
    audiosrc->get_property = audiosrc_get_property;

    audiosrc->pkt_hdr.format = AUDIO_SET_FORMAT(false, AF_LITTLE_ENDIAN, true, 16);

    /* init elements pads */
    for (i = 0; i < NUM_SRCS(TYPE_ELEMENT_AUDIO_SRC); i++)
    {
        pad = (PadSrc *)ELEMENT_SRC_PAD(audiosrc, i);

        /* Set handlers */
        /* Audio Source is always in push mode */
        pad->event_handler      = audiosrc_handle_src_event;
        pad->query_handler      = audiosrc_handle_src_query;
        pad->activation_handler = audiosrc_src_activate;
        pad->activate_push      = audiosrc_src_activate_push;
        pad->activate_pull      = audiosrc_src_activate_pull;
        /* Set source pad pull function */
        pad->pull_handler     = audiosrc_src_pull_handler;
        pad->process_handler  = audiosrc_src_process;
        pad->process_precheck = NULL;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return FLOW_OK;
}

/*!
 * audiosrc_set_device_name:
 * @param element: element handle
 * @param device_name: device name for ipod, line in and mic.
 * @param output_device_name: the output device name is the corresponding device that is in
 *                            this streamer sink. This info is used to config and open the
 *                            pcmmgr device.
 *
 * @brief This function sets the device name.
 *
 * @returns StreamReturnType
 */
static int32_t audiosrc_set_device_name(ElementHandle element, const char *device_name, const char *output_device_name)
{
    ElementAudioSrc *src = (ElementAudioSrc *)(uintptr_t)element;
    StreamReturnType ret = STREAM_OK;
    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    CHK_ARGS(src == NULL || device_name == NULL || output_device_name == NULL, STREAM_ERR_INVALID_ARGS);
    strncpy(src->device_name, device_name, MAX_DEVICE_NAME_LENGTH - 1);
    src->device_name[MAX_DEVICE_NAME_LENGTH - 1] = '\0';

    strncpy(src->output_device_name, output_device_name, MAX_DEVICE_NAME_LENGTH - 1);
    src->output_device_name[MAX_DEVICE_NAME_LENGTH - 1] = '\0';
    STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]set_device_name: %s\n", device_name);

    STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]set_output_device_name: %s\n", output_device_name);

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 * @param element: element handle
 * @param device_type: device type
 *
 * @brief This function sets the device driver type.
 *
 * @returns StreamReturnType
 */
static int32_t audiosrc_set_device_type(ElementHandle element, uint32_t device_type)
{
    ElementAudioSrc *src = (ElementAudioSrc *)(uintptr_t)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    CHK_ARGS(src == NULL || device_type >= LAST_AUDIOSRC_DEVICE_DRIVER_TYPE, STREAM_ERR_INVALID_ARGS);
    src->device_driver_type = device_type;

    STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]set_device_type: %d\n", device_type);

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 * @param element: element handle
 * @param chunk_size: chunk size
 *
 * @brief This function sets the chunk size in which the audio
 * source sends the data when src pad is activated in push mode.
 *
 * @returns StreamReturnType
 */
static int32_t audiosrc_set_push_chunk_size(ElementHandle element, uint32_t chunk_size)
{
    ElementAudioSrc *src = (ElementAudioSrc *)(uintptr_t)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    CHK_ARGS(src == NULL || chunk_size == 0, STREAM_ERR_INVALID_ARGS);

    src->chunk_size         = chunk_size;
    src->pkt_hdr.chunk_size = chunk_size;

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 * @param element: element handle
 * @param chunk_size: chunk size
 *
 * @brief This function gets the chunk size
 *
 * @returns StreamReturnType
 */
static int32_t audiosrc_get_push_chunk_size(ElementHandle element, uint64_t *chunk_size)
{
    ElementAudioSrc *src = (ElementAudioSrc *)(uintptr_t)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    CHK_ARGS(src == NULL || chunk_size == 0, STREAM_ERR_INVALID_ARGS);

    *chunk_size = src->pkt_hdr.chunk_size;

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 *
 * @param element: element handle
 * @param sample_rate: sample rate to be set
 *
 * @brief This function sets the device name.
 *
 * @returns StreamReturnType
 */
static int32_t audiosrc_set_sampling_rate(ElementHandle element, uint32_t sample_rate)
{
    ElementAudioSrc *src = (ElementAudioSrc *)(uintptr_t)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    CHK_ARGS(src == NULL, STREAM_ERR_INVALID_ARGS);

    src->sample_rate         = sample_rate;
    src->pkt_hdr.sample_rate = sample_rate;

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 *
 * @param element: element handle
 * @param continuous_read: determines whether the device is in continuous
 *                         read state or not.
 *
 * @brief This function sets the continuous read property.
 *
 * @returns StreamReturnType
 */
static int32_t audiosrc_set_continuous_read(ElementHandle element, bool continuous_read)
{
    ElementAudioSrc *src = (ElementAudioSrc *)(uintptr_t)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    CHK_ARGS(src == NULL, STREAM_ERR_INVALID_ARGS);

    src->continuous_read = continuous_read;

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 *
 * @param element: element handle
 * @param dummy_tx: enable sai tx to have clocks on rx correctly
 *
 *
 * @brief This function sets the dummy rx enable property.
 *
 * @returns StreamReturnType
 */
static int32_t audiosrc_set_dummy_tx_enable(ElementHandle element, bool dummy_tx)
{
    ElementAudioSrc *src = (ElementAudioSrc *)(uintptr_t)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    CHK_ARGS(src == NULL, STREAM_ERR_INVALID_ARGS);

    src->dummy_tx = dummy_tx;

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 * @param element: element handle
 * @param num_channels: number of channels
 *
 * @brief This function sets the number of channels in which the audio
 * source sends the data when src pad is activated in push mode.
 *
 * @returns StreamReturnType
 */
static int32_t audiosrc_set_num_channels(ElementHandle element, uint32_t num_channels)
{
    ElementAudioSrc *src = (ElementAudioSrc *)(uintptr_t)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    CHK_ARGS(src == NULL || num_channels == 0, STREAM_ERR_INVALID_ARGS);

    src->pkt_hdr.num_channels = num_channels;

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 * @param element: element handle
 * @param bits_per_sample: bits per sample
 *
 * @brief This function sets the number of channels in which the audio
 * source sends the data when src pad is activated in push mode.
 *
 * @returns StreamReturnType
 */
static int32_t audiosrc_set_bits_per_sample(ElementHandle element, uint32_t bits_per_sample)
{
    ElementAudioSrc *src = (ElementAudioSrc *)(uintptr_t)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    CHK_ARGS(src == NULL || bits_per_sample == 0 || (bits_per_sample % 8 != 0), STREAM_ERR_INVALID_ARGS);

    src->pkt_hdr.bits_per_sample = bits_per_sample;
    src->pkt_hdr.format          = AUDIO_SET_FORMAT(false, AF_LITTLE_ENDIAN, true, bits_per_sample);

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 * @param element: element handle
 * @param frame_ms: frame_ms
 *
 * @brief This function sets the number of frame millisecond in which the audio
 * source sends the data when src pad is activated in push mode.
 *
 * @returns StreamReturnType
 */
static int32_t audiosrc_set_frame_ms(ElementHandle element, uint32_t frame_ms)
{
    ElementAudioSrc *src = (ElementAudioSrc *)(uintptr_t)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    CHK_ARGS(src == NULL || frame_ms == 0, STREAM_ERR_INVALID_ARGS);

    src->frame_ms = frame_ms;

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 *
 * @param element: Pointer to element
 * @param state: changed state.
 *
 * @brief Function handles the state change of the element in a pipeline.
 */
static int32_t audiosrc_change_state(StreamElement *element, PipelineState state)
{
    PipelineStateChange state_change;
    ElementAudioSrc *audiosrc = (ElementAudioSrc *)element;
    int32_t ret               = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    CHK_ARGS(NULL == audiosrc, STREAM_ERR_INVALID_ARGS);

    /* get the transition status with the new passed state and the existing
     * state of the audio source element
     */
    state_change = (PipelineStateChange)STATE_TRANSITION(audiosrc->state, state);

    switch (state_change)
    {
        case STATE_CHANGE_NULL_TO_READY:
            STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]STATE_CHANGE_NULL_TO_READY\n");
            break;

        case STATE_CHANGE_PAUSED_TO_PLAYING:
            if (AudioSrcStartDevice[audiosrc->device_driver_type])
            {
                if (AudioSrcStartDevice[audiosrc->device_driver_type](audiosrc))
                    STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_INTERNAL, "[AudioSRC] Start device error\n");
            }

            STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]STATE_CHANGE_PAUSED_TO_PLAYING\n");
            break;

        case STATE_CHANGE_PLAYING_TO_PAUSED:
            if (AudioSrcStopDevice[audiosrc->device_driver_type])
            {
                if (AudioSrcStopDevice[audiosrc->device_driver_type](audiosrc))
                    STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_INTERNAL, "[AudioSRC] Stop device error\n");
            }

            STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]STATE_CHANGE_PLAYING_TO_PAUSED\n");
            /* no blocking of data in this state. Probably blocking can be done
             * in the output of the record pipeline if required
             */
            break;

        case STATE_CHANGE_READY_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]STATE_CHANGE_READY_TO_PAUSED\n");
            /* no blocking of data in this state. Probably blocking can be done
             * in the output of the record pipeline if required
             */
            break;
            /* do nothing */
        case STATE_CHANGE_PAUSED_TO_READY:
            STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]STATE_CHANGE_PAUSED_TO_READY\n");
            break;
            /* Pads will be deactivated here. */
        case STATE_CHANGE_READY_TO_NULL:
            STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]STATE_CHANGE_READY_TO_NULL\n");
            break;
            /* close resources if anything was opened
             *
             */
        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 *
 * @param pad: Queue element src pad
 * @param active: To activate or deactivate pad.
 *
 * @brief Queue src pad activation handler. Activation is handled
 * by the peer sink pad, this should just handle the deactivation.
 */
static uint8_t audiosrc_src_activate(StreamPad *pad, uint8_t active)
{
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    if ((uint8_t) false == active)
    {
        ElementAudioSrc *audiosrc = (ElementAudioSrc *)PAD_PARENT(pad);
        PadScheduling mode        = PAD_SCHEDULING(pad);

        STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]Deactivate src pad\n");

        if (SCHEDULING_PUSH == mode)
        {
            /* NOTE: eos event shifted to read_loop */
            ret = pad_activate_push(pad, false);
        }
        else if (SCHEDULING_PULL == mode)
        {
            ret = pad_activate_pull(pad, false);
        }

        if ((uint8_t) true == ret)
        {
            STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]Close AudioSRC\n");

            /* call the device driver specific close function */
            AudioSrcCloseDevice[audiosrc->device_driver_type](audiosrc);
        }
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 *
 * @param audiosrc: Pointer to audiosrc element
 * @param offset: Offset of the file to read data from (not used here)
 * @param length: Number of bytes to read
 * @param buf: Pointer to buffer to read data
 *
 * @brief Function reads the data from the file specified by the element.
 * NOTE: Function may or may not be able to read the required
 * length of data.
 *
 * Returns FLOW_OK or FLOW_ERROR when read fails.
 *
 */
static FlowReturn audiosrc_read(ElementAudioSrc *audiosrc, uint32_t offset, uint32_t length, StreamBuffer *buf)
{
    AudioSrcStreamErrorType ret = AUDIOSRC_SUCCESS;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    /* call the driver specific read function */
    ret = AudioSrcReadDevice[audiosrc->device_driver_type](audiosrc, length, buf);

    /* read failed */
    if (ret == AUDIOSRC_ERROR_READ_TIME_OUT)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return FLOW_TIME_OUT;
    }
    if (ret != AUDIOSRC_SUCCESS)
    {
        goto read_failed;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return FLOW_OK;

read_failed:
    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return FLOW_ERROR;
}

/*!
 *
 * @param pad: Element pad
 *
 * @brief Function is the loop function of the audiosrc source pad which
 * runs in the task thread. It reads the chunk_size of data
 * and pushes it to peer element sink pad.
 *
 */
static int32_t audiosrc_src_process(StreamPad *pad)
{
    ElementAudioSrc *audiosrc = (ElementAudioSrc *)pad->parent;
    FlowReturn ret            = FLOW_OK;
    uint32_t length = 0, offset = 0;
    StreamBuffer buf;
    StreamEvent event;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    /*2. Get the chunk size to read from file */
    length = audiosrc->chunk_size;

    /* init buffer metadata */
    buf.buffer = NULL;
    buf.size   = 0;
    buf.time   = (uint32_t)-1; /* Don't know anything about time */
    buf.offset = 0;

    /* Its an end of stream */
    if (audiosrc->end_of_stream != (uint8_t) true)
    {
        /* read chunk_size of data */
        ret = audiosrc_read(audiosrc, offset, length, &buf);

        if (ret == FLOW_TIME_OUT)
        {
            if (audiosrc->state == STATE_PLAYING)
            {
                STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_GENERAL_ERROR, "[AudioSRC]Flow not ok1\n");
                goto audio_data_end;
            }

            if (buf.size > sizeof(AudioPacketHeader))
            {
                if (pad_push(pad, &buf) != FLOW_OK)
                {
                    STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_GENERAL_ERROR, "[AudioSRC]Flow not ok2\n");
                    goto pause;
                }
            }
            goto pause;
        }
        else if (ret != FLOW_ERROR)
        {
            /* No error then forward the data */
            /* push data to peer sink pad */
            if ((uint8_t) true == audiosrc->continuous_read)
            {
                audiosrc->retries = 0;
            }

            ret = pad_push(pad, &buf);
            if (ret != FLOW_OK)
            {
                STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_GENERAL_ERROR, "[AudioSRC]Flow not ok3\n");
                goto pause;
            }
        }
        else
        {
        audio_data_end:

            STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_GENERAL_ERROR,
                             "[AudioSRC]Unexpected error. A continuous playing device "
                             "may be disconnected and the audio source is shutting "
                             "down.\n");

            /* Create and send EOS event */
            /* send eos to peer pad */
            event_create_eos(&event);
            pad_push_event(pad, &event);

            audiosrc->end_of_stream = true;

            goto pause;
        }
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;

pause:
    /* pause task */
    STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]Task pausing..\n");
    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/**
 * @brief Function is the pull function of the audiosrc source pad in
 * SCHEDULING_PULL mode. It reads the size of data from
 * the offset to buffer.
 *
 * @param pad    Pad pointer
 * @param buffer Buffer pointer
 * @param size   Data size
 * @param offset Offset to be read
 * @return FlowReturn
 */
static FlowReturn audiosrc_src_pull_handler(StreamPad *pad, StreamBuffer *buffer, uint32_t size, uint32_t offset)
{
    ElementAudioSrc *audiosrc = (ElementAudioSrc *)pad->parent;
    FlowReturn ret            = FLOW_OK;
    uint32_t length           = 0;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    /*2. Get the chunk size to read from file */
    length = audiosrc->chunk_size;

    /* init buffer metadata */
    buffer->buffer = NULL;
    buffer->size   = 0;
    buffer->time   = (uint32_t)-1; /* Don't know anything about time */
    buffer->offset = offset;

    /* Its an end of stream */
    if (audiosrc->end_of_stream != (uint8_t) true)
    {
        /* read chunk_size of data */
        ret = audiosrc_read(audiosrc, offset, length, buffer);

        if (ret == FLOW_TIME_OUT)
        {
            if (audiosrc->state == STATE_PLAYING)
            {
                STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_GENERAL_ERROR, "[AudioSRC]Flow not ok1\n");
                goto audio_data_end;
            }

            goto pause;
        }
        else if (ret == FLOW_ERROR)
        {
        audio_data_end:
            STREAMER_LOG_ERR(DBG_AUDIO_SRC, ERRCODE_GENERAL_ERROR, "[AudioSRC]Unexpected error\n");

            /* send eos to pulling pad */
            ret                     = FLOW_EOS;
            audiosrc->end_of_stream = true;

            goto pause;
        }
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;

pause:
    /* pause task */
    STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, "[AudioSRC]Task pausing..\n");
    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 *
 * @param pad: Element pad
 * @param event: type of event
 *
 * @brief Function handles the events of the element source pad.
 *
 * */
static uint8_t audiosrc_handle_src_event(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    /* handle the event */
    switch (EVENT_TYPE(event))
    {
            /* case EVENT_EOS: */
            /* case EVENT_NEWSEGMENT: */

        case EVENT_SEEK:
        case EVENT_FLUSH_START:
        case EVENT_FLUSH_STOP:
        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 *
 * @param pad: pad handle
 * @param query: query
 *
 * @brief This function is the empty impl of the src pad
 * query handler.
 *
 * @returns uint8_t
 */

static uint8_t audiosrc_handle_src_query(StreamPad *pad, StreamQuery *query)
{
    return false;
}

/*!
 *
 * @param pad: Element pad
 * @param active: true if pad is to be activated else false to deactivate
 *
 * @brief Activates or deactivates the audiosrc source pad in PUSH mode.
 *
 **/
static uint8_t audiosrc_src_activate_push(StreamPad *pad, uint8_t active)
{
    ElementAudioSrc *audiosrc = (ElementAudioSrc *)pad->parent;
    uint8_t ret               = false;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, " [Audio Src]: Activate push: %d\n", active);

    /* Activate pad in PUSH mode */
    if (active)
    {
        /* set default chunk size if not already set */
        if (audiosrc->chunk_size == 0)
        {
            audiosrc->chunk_size = AUDIOSRC_DEFAULT_CHUNK_SIZE;
        }

        /* reset */
        audiosrc->end_of_stream = false;

        /* NOTE: Other audiosrc element variables are not changed here as they
         * may be set by the user APIs and may not want to change
         */

        /* 1. Open file */
        if (audiosrc->device_name[0] == '\0')
        {
            goto no_device;
        }
        /* Audio device enumeration shall not be done for devices through
         * alsa/oss.The device set by the user is used
         */
        /*
         * Init will Open pcm device for audio capture
         */
        /* call the device driver specific open function */
        ret = AudioSrcOpenDevice[audiosrc->device_driver_type](audiosrc);

        /* check for device open success */
        ret = ret == 0;
    }
    else
    {
        ret = true;
    }

no_device:
    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 *
 * @param pad: Element pad
 * @param active: true if pad is to be activated else false to deactivate
 *
 * @brief Activates or deactivates the audiosrc source pad in PUSH mode.
 *
 **/
static uint8_t audiosrc_src_activate_pull(StreamPad *pad, uint8_t active)
{
    ElementAudioSrc *audiosrc = (ElementAudioSrc *)pad->parent;
    uint8_t ret               = false;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    STREAMER_LOG_DEBUG(DBG_AUDIO_SRC, " [Audio Src]: Activate push: %d\n", active);

    /* Activate pad in PULL mode */
    if (active)
    {
        /* set default chunk size if not already set */
        if (audiosrc->chunk_size == 0)
        {
            audiosrc->chunk_size = AUDIOSRC_DEFAULT_CHUNK_SIZE;
        }

        /* reset */
        audiosrc->end_of_stream = false;

        /* NOTE: Other audiosrc element variables are not changed here as they
         * may be set by the user APIs and may not want to change
         */

        /* 1. Open file */
        if (audiosrc->device_name[0] == '\0')
        {
            goto no_device;
        }
        /* Audio device enumeration shall not be done for devices through
         * alsa/oss.The device set by the user is used
         */
        /*
         * Init will Open pcm device for audio capture
         */
        /* call the device driver specific open function */
        ret = AudioSrcOpenDevice[audiosrc->device_driver_type](audiosrc);

        /* check for device open success */
        ret = ret == 0;
    }
    else
    {
        ret = true;
    }

no_device:
    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 *
 * @param element_ptr: element pointer
 * @param prop: properties
 * @param val_ptr: parameters
 *
 * @brief This function get audio source element properties
 *
 * @returns AudioSinkStreamErrorType
 *
 */
static int32_t audiosrc_get_property(StreamElement *element_ptr, uint16_t prop, uint64_t *val_ptr)
{
    int32_t ret = AUDIOSRC_SUCCESS;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    if (NULL == val_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
        return AUDIOSRC_ERROR_INVALID_ARGS;
    }

    switch (prop)
    {
        case PROP_AUDIOSRC_GET_CHUNK_SIZE:
            ret = audiosrc_get_push_chunk_size((ElementHandle)(uintptr_t)element_ptr, val_ptr);
            break;

        default:
            ret = AUDIOSRC_ERROR_INVALID_ARGS;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}

/*!
 *
 * @param element_ptr: element pointer
 * @param prop: properties
 * @param val: parameters
 *
 * @brief This function set audio source element properties
 *
 * @returns AudioSinkStreamErrorType
 *
 */
static int32_t audiosrc_set_property(StreamElement *element_ptr, uint16_t prop, uint32_t val)
{
    int32_t ret = AUDIOSRC_SUCCESS;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SRC);

    switch (prop)
    {
        case PROP_AUDIOSRC_SET_DEVICE_TYPE:
            ret = audiosrc_set_device_type((ElementHandle)(uintptr_t)element_ptr, val);
            break;

        case PROP_AUDIOSRC_SET_CHUNK_SIZE:
            ret = audiosrc_set_push_chunk_size((ElementHandle)(uintptr_t)element_ptr, val);
            break;

        case PROP_AUDIOSRC_SET_SAMPLE_RATE:
            ret = audiosrc_set_sampling_rate((ElementHandle)(uintptr_t)element_ptr, val);
            break;

        case PROP_AUDIOSRC_SET_DEVICE_NAME:
            ret = audiosrc_set_device_name((ElementHandle)(uintptr_t)element_ptr,
                                           ((AUDSRC_SET_NAME_T *)(uintptr_t)val)->device_name,
                                           ((AUDSRC_SET_NAME_T *)(uintptr_t)val)->output_device_name);
            break;

        case PROP_AUDIOSRC_SET_CONTINUOUS_READ:
            ret = audiosrc_set_continuous_read((ElementHandle)(uintptr_t)element_ptr, (bool)val);
            break;
        case PROP_AUDIOSRC_SET_DUMMY_TX_ENABLE:
            ret = audiosrc_set_dummy_tx_enable((ElementHandle)(uintptr_t)element_ptr, (bool)val);
            break;
        case PROP_AUDIOSRC_SET_NUM_CHANNELS:
            ret = audiosrc_set_num_channels((ElementHandle)(uintptr_t)element_ptr, val);
            break;
        case PROP_AUDIOSRC_SET_BITS_PER_SAMPLE:
            ret = audiosrc_set_bits_per_sample((ElementHandle)(uintptr_t)element_ptr, val);
            break;
        case PROP_AUDIOSRC_SET_FRAME_MS:
            ret = audiosrc_set_frame_ms((ElementHandle)(uintptr_t)element_ptr, val);
            break;
        default:
            ret = AUDIOSRC_ERROR_INVALID_ARGS;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SRC);
    return ret;
}
