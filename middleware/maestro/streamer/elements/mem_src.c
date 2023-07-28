/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    mem_src.c
 * @brief   This file implement the memory source functions
 */

/*
 * INCLUDE FILES
 */
#include <string.h>
#include "streamer_element.h"
#include "mem_src.h"
#include "audio_sink.h"
#include "pipeline.h"
#include "streamer_element_properties.h"

/*
 * LOCAL MACROS
 */

/*
 * LOCAL FUNCTION PROTOTYPES
 */
static int32_t memsrc_src_pad_process(StreamPad *pad);
static int32_t memsrc_change_state(StreamElement *element, PipelineState state);
static uint8_t memsrc_handle_src_event(StreamPad *pad, StreamEvent *event);
static uint8_t memsrc_handle_src_query(StreamPad *pad, StreamQuery *event);
static uint8_t memsrc_src_activate_push(StreamPad *pad, uint8_t active);
static uint8_t memsrc_src_activate_pull(StreamPad *pad, uint8_t active);
static uint8_t memsrc_src_activate(StreamPad *pad, uint8_t active);
static FlowReturn memsrc_pull(StreamPad *pad, StreamBuffer *buffer, uint32_t size, uint32_t offset);
static int32_t memsrc_get_property(StreamElement *element_ptr, uint16_t prop, uint32_t *val_ptr);
static int32_t memsrc_set_property(StreamElement *element_ptr, uint16_t prop, uintptr_t val);

/*
 * LOCAL FUNCTIONS
 */

int32_t memsrc_init(StreamElement *element)
{
    PadSrc *pad = NULL;
    int32_t i   = 0;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    ElementMemSrc *memsrc = (ElementMemSrc *)element;

    /* filesrc element specific init */
    memsrc->buffer        = NULL;
    memsrc->size          = 0;
    memsrc->location      = NULL;
    memsrc->chunk_size    = MEMSRC_DEFAULT_CHUNK_SIZE;
    memsrc->end_of_stream = false;
    memsrc->read_position = 0;
    memsrc->mem_type      = RAW_DATA;

    /* set element change state function */
    memsrc->change_state = memsrc_change_state;
    memsrc->set_property = memsrc_set_property;
    memsrc->get_property = memsrc_get_property;

    /* init elements pads */
    for (i = 0; i < NUM_SRCS(TYPE_ELEMENT_MEM_SRC); i++)
    {
        pad = (PadSrc *)ELEMENT_SRC_PAD(memsrc, i);

        /* Set handlers */
        pad->event_handler      = memsrc_handle_src_event;
        pad->query_handler      = memsrc_handle_src_query;
        pad->activation_handler = memsrc_src_activate;

        pad->activate_pull = memsrc_src_activate_pull;
        pad->activate_push = memsrc_src_activate_push;

        /* Set source pad pull function */
        pad->pull_handler    = memsrc_pull;
        pad->process_handler = memsrc_src_pad_process;

        pad->process_precheck = NULL; /* Mem Source will not block */
    }

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return 0;
}

int32_t memsrc_set_buffer(ElementHandle element, int8_t *location, uint32_t size)
{
    ElementMemSrc *src   = (ElementMemSrc *)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    CHK_ARGS(src == NULL || location == NULL || size == 0, STREAM_ERR_INVALID_ARGS);

    src->location = location;
    src->size     = size;

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return ret;
}

int32_t memsrc_set_push_chunk_size(ElementHandle element, uint32_t chunk_size)
{
    ElementMemSrc *src   = (ElementMemSrc *)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    CHK_ARGS(src == NULL || chunk_size == 0, STREAM_ERR_INVALID_ARGS);

    src->chunk_size = chunk_size;

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return ret;
}

int32_t memsrc_get_push_chunk_size(ElementHandle element, uint32_t *chunk_size)
{
    ElementMemSrc *src   = (ElementMemSrc *)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    CHK_ARGS(src == NULL || chunk_size == NULL, STREAM_ERR_INVALID_ARGS);

    *chunk_size = src->chunk_size;

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return ret;
}

/*!
 *
 * @param element: Pointer to element
 * @param state: changed state.
 *
 * @brief Function handles the state change of the element in a pipeline.
 */
static int32_t memsrc_change_state(StreamElement *element, PipelineState state)
{
    PipelineStateChange state_change;
    ElementMemSrc *memsrc = (ElementMemSrc *)element;
    int32_t ret           = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    CHK_ARGS(NULL == memsrc, STREAM_ERR_INVALID_ARGS);

    /* get the transition status with the new passed state and the existing
     * state of the decoder element
     */
    state_change = (PipelineStateChange)STATE_TRANSITION(memsrc->state, state);

    switch (state_change)
    {
        case STATE_CHANGE_NULL_TO_READY:
            STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]STATE_CHANGE_NULL_TO_READY\n");
            break;
        case STATE_CHANGE_READY_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]STATE_CHANGE_READY_TO_PAUSED\n");
            break;
        case STATE_CHANGE_PAUSED_TO_PLAYING:
            STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]STATE_CHANGE_PAUSED_TO_PLAYING\n");
            break;
        case STATE_CHANGE_PLAYING_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]STATE_CHANGE_PLAYING_TO_PAUSED\n");
            break;
            /* do nothing */
        case STATE_CHANGE_PAUSED_TO_READY:
            STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]STATE_CHANGE_PAUSED_TO_READY\n");
            break;
            /* Pads will be deactivated here. */
        case STATE_CHANGE_READY_TO_NULL:
            STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]STATE_CHANGE_READY_TO_NULL\n");
            break;
            /* close resources if anything was opened
             * N/A for decoder
             */
        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return ret;
}

/*!
 *
 * @param pad: Queue element src pad
 * @param active: To activate or deactivate pad.
 *
 * @brief queue src pad activatation handler. Activation is handled
 * by the peer sink pad, this should just handle the deactivation.
 */
static uint8_t memsrc_src_activate(StreamPad *pad, uint8_t active)
{
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    STREAMER_LOG_DEBUG(DBG_MEMSRC, "memsrc_src_activate(%d)\n", (uint32_t)active);

    if ((uint8_t) false == active)
    {
        PadScheduling mode = PAD_SCHEDULING(pad);

        if (SCHEDULING_PUSH == mode)
        {
            ElementMemSrc *memsrc = (ElementMemSrc *)PAD_PARENT(pad);

            STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]Deactivate src pad\n");
            ret = pad_activate_push(pad, false);
            if ((uint8_t) true == ret)
            {
                STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]Close MemSRC\n");

                /* reset */
                memsrc->end_of_stream = false;
                memsrc->read_position = 0;

                /* Free the chunk buffer */
                if (memsrc->buffer)
                {
                    OSA_MemoryFree(memsrc->buffer);
                    memsrc->buffer = NULL;
                }
            }
        }
        else if (SCHEDULING_PULL == mode)
        {
            ret = pad_activate_pull(pad, false);
        }
    }

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return ret;
}

/*!
 *
 * @param memsrc: Pointer to memsrc element
 * @param offset: Offset of the file to read data from
 * @param length: Number of bytes to read
 * @param buf: Pointer to buffer to read data
 *
 * @brief Reads the data from the memory array specified by the element.
 * NOTE: Function may or may not be able to read the required
 * length of data.
 * Returns FLOW_OK or FLOW_EOS when EOF file is encountered.
 *
 */
static FlowReturn memsrc_read(ElementMemSrc *memsrc, int32_t offset, int32_t length, StreamBuffer *buf)
{
    int32_t bytes_read = length;
    int8_t *data_buf   = NULL;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    STREAMER_LOG_DEBUG(DBG_FILESRC, "[MemSRC]pos: %d\n", memsrc->read_position);

    /* if the required offset is not same as the last read offset */
    if (memsrc->read_position != offset)
    {
        memsrc->read_position = offset;
    }

    STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]read: size: %d, offset: %d\n", length, offset);

    if (memsrc->read_position >= memsrc->size && memsrc->size > 0)
    {
        goto eos;
    }
    if (memsrc->mem_type == RAW_DATA)
    {
        /* Set pointer to read data to buffer */
        data_buf = buf->buffer + sizeof(RawPacketHeader);
    }
    else if (memsrc->mem_type == AUDIO_DATA)
    {
        /* Set pointer to read data to buffer */
        data_buf = buf->buffer + sizeof(AudioPacketHeader);
    }

    /* Check if we are reading within the array bounds.
     * If not adjust the read size */
    if (memsrc->read_position + bytes_read > memsrc->size)
    {
        // Case of final read: read as many bytes as we have left
        if (memsrc->size > memsrc->read_position)
        {
            bytes_read = memsrc->size - memsrc->read_position;
        }
        else
        {
            goto eos;
        }
    }

    memcpy(data_buf, &(memsrc->location[memsrc->read_position]), bytes_read);

    /* increment read position */
    memsrc->read_position += bytes_read;

    /* update buffer offset */
    buf->offset = offset;

    if (memsrc->mem_type == RAW_DATA)
    {
        /* set buffer size = header size + actual data size read */
        buf->size = sizeof(RawPacketHeader) + bytes_read;
    }
    else if (memsrc->mem_type == AUDIO_DATA)
    {
        /* set buffer size = header size + actual data size read */
        buf->size = sizeof(AudioPacketHeader) + bytes_read;
    }

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return FLOW_OK;

eos:
    STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]Read pos: %d, size: %d, offset: %d\n", memsrc->read_position, memsrc->size,
                       offset);
    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return FLOW_EOS;
}

#if 0
/*!
 * memsrc_src_pad_process:
 *
 * @param pad: Element pad
 *
 * @brief Function is the loop function of the memsrc source pad which
 * runs in the task thread. It reads the chunk_size of data from
 * the memory and pushes it to peer element sink pad. When EOF memory
 * is reached, the task goes to PAUSE state.
 *
 */
int32_t memsrc_src_pad_process(StreamPad *pad)
{
    int32_t ret;
    ElementMemSrc *element = (ElementMemSrc *)PAD_PARENT(pad);

    StreamBuffer pullBuf = {.buffer = (int8_t *)element->buffer, .size = element->size, .offset = 0, .time = 0};

    ret = memsrc_pull(pad, &pullBuf, element->chunk_size, element->read_position);
    if (ret != 0)
    {
        return ret;
    }

    return pad_push(pad, &pullBuf);
}
#endif

/*!
 * memsrc_src_pad_process:
 *
 * @param pad: Element pad
 *
 * @brief Function is the loop function of the memsrc source pad which
 * runs in the task thread. It reads the chunk_size of data from
 * the memory and pushes it to peer element sink pad. When EOF memory
 * is reached, the task goes to PAUSE state.
 *
 */
int32_t memsrc_src_pad_process(StreamPad *pad)
{
    ElementMemSrc *memsrc = (ElementMemSrc *)pad->parent;
    FlowReturn ret        = FLOW_OK;
    uint32_t length = 0, offset = 0;
    StreamBuffer buf;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    /*2. Get the chunk size to read from memory */
    length = memsrc->chunk_size;
    offset = memsrc->read_position;

    /* init buffer metadata */
    buf.buffer = NULL;
    buf.size   = 0;
    buf.time   = (uint32_t)-1; /* Don't know anything about time */
    buf.offset = 0;

    /* Its an end of stream */
    if (memsrc->end_of_stream != (uint8_t) true)
    {
        /* Allocate required buffer to read data */
        buf.buffer = memsrc->buffer;

        /* read chunk_size of data */
        ret = memsrc_read(memsrc, offset, length, &buf);
        if (ret == FLOW_EOS)
        {
            StreamEvent event;

            STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]EOS\n");

            memsrc->end_of_stream = true;

            /* send eos to peer pad */
            event_create_eos(&event);
            pad_push_event(pad, &event);

            StreamElement *memsrc_ele     = (StreamElement *)memsrc;
            Pipeline *pipeline            = ELEMENT_PARENT_PIPELINE(memsrc_ele);
            PipelineHandle pipelineHandle = (PipelineHandle)pipeline;
            if (!get_repeat_pipeline(pipelineHandle))
            {
                STREAMER_FUNC_EXIT(DBG_MEMSRC);
                return STREAM_OK;
            }

            /* Set pointer to read data to buffer */
            if (memsrc->mem_type == RAW_DATA)
            {
                memset(buf.buffer + sizeof(RawPacketHeader), 0, length);
                buf.size = sizeof(RawPacketHeader) + length;
            }
            else if (memsrc->mem_type == AUDIO_DATA)
            {
                /* Set pointer to read data to buffer */
                memset(buf.buffer + sizeof(AudioPacketHeader), 0, length);
                buf.size = sizeof(AudioPacketHeader) + length;
            }

            buf.offset = offset;
        }
        else if (ret == FLOW_UNEXPECTED)
        {
            STREAMER_LOG_ERR(DBG_MEMSRC, ret, "[MemSRC] Unexpected error %d\n", ret);
            STREAMER_FUNC_EXIT(DBG_MEMSRC);
            /* send out EOS message to stop the playing */
            send_msg_element((StreamElement *)memsrc, MSG_EOS, 0);
            return STREAM_OK;
        }
        else if (ret == FLOW_ERROR)
        {
            STREAMER_LOG_ERR(DBG_MEMSRC, ERRCODE_GENERAL_ERROR, "[MemSRC] Streamer general error\n");
            STREAMER_FUNC_EXIT(DBG_MEMSRC);
            return STREAM_ERR_GENERAL;
        }

        /* Update audio packet header values as values may have been changed in the AUDIO_PROC element as part of a
         * crossover preset */
        if (memsrc->mem_type == AUDIO_DATA)
        {
            AudioPacketHeader *pkt_hdr = NULL;
            /* set data type id */
            pkt_hdr                  = (AudioPacketHeader *)buf.buffer;
            pkt_hdr->sample_rate     = memsrc->sample_rate;
            pkt_hdr->bits_per_sample = memsrc->bit_width;
            pkt_hdr->num_channels    = memsrc->num_channels;
            AUDIO_FORMAT(pkt_hdr)    = AUDIO_SET_FORMAT(0, 0, 1, memsrc->bit_width);
            pkt_hdr->chunk_size      = memsrc->chunk_size;
        }

        /* No error then forward the data */
        /* push data to peer sink pad */
        if (pad_push(pad, &buf) != FLOW_OK)
        {
            STREAMER_LOG_ERR(DBG_MEMSRC, ERRCODE_GENERAL_ERROR, "[MemSRC]Flow not ok\n");
            STREAMER_FUNC_EXIT(DBG_MEMSRC);
            return STREAM_ERR_GENERAL;
        }
    }
    else
    {
        // Due to forcing a context switch
        OSA_TimeDelay(1);
    }

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return STREAM_OK;
}

/*!
 *
 * @param pad: Element pad
 * @param buffer: Pointer of the buffer to read data
 * @param size: Number of bytes to read
 * @param offset: Offset to the file
 *
 * @brief Function is the pull function of the memsrc source pad in
 * SCHEDULING_PULL mode. It reads the size of data from
 * the offset to buffer.
 *
 */
static FlowReturn memsrc_pull(StreamPad *pad, StreamBuffer *buffer, uint32_t size, uint32_t offset)
{
    ElementMemSrc *memsrc = (ElementMemSrc *)pad->parent;
    FlowReturn ret        = FLOW_OK;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    /* Put Raw header in the buffer */
    ((AudioPacketHeader *)BUFFER_PTR(buffer))->id = RAW_DATA;

    /* read chunk_size of data */
    ret = memsrc_read(memsrc, offset, size, buffer);
    if (ret == FLOW_EOS)
    {
        /* Its an end of stream */
        if (memsrc->end_of_stream != (uint8_t) true)
        {
            STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]Pull EOS");

            /* Need not send the EOS event here. The element calling this
             * function should check the return value and send EOS event
             * downstream.
             */
            memsrc->end_of_stream = true;
        }
    }

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return ret;
}

/*!
 *
 * @param pad: Element pad
 * @param event: type of event
 *
 * @brief Function handles the events of the element source pad.
 *
 */
static uint8_t memsrc_handle_src_event(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret             = true;
    ElementMemSrc *memsrc   = (ElementMemSrc *)PAD_PARENT(pad);
    uint32_t offset         = EVENT_DATA(event);
    StreamDataFormat format = EVENT_FORMAT(event);
    StreamEvent flush_event;
    StreamEvent new_segment;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    /* handle the event */
    switch (EVENT_TYPE(event))
    {
            /* case EVENT_EOS: */
            /* case EVENT_NEWSEGMENT: */

        case EVENT_SEEK:
            /* memory source needs seek event in bytes format and the stream
             * should still be actibe
             */

            STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]seek from %d to %d\n", memsrc->read_position, offset);
            /* First check if it is byte seekable */
            if (offset != 0)
            {
                if (DATA_FORMAT_BYTES == format)
                {
                    /* If the offset specified is more than the size of the file
                     * then restrict the offset to the file size. When file src
                     * thread will start giving data agin it will generate an EOS.
                     */
                    if (offset > memsrc->size && memsrc->size > 0)
                    {
                        STREAMER_LOG_WARN(DBG_MEMSRC, ERRCODE_INTERNAL,
                                          "[MemSRC]Offset more than the memsrc size: "
                                          "offset=%d size=%d\n",
                                          offset, memsrc->size);
                        offset = memsrc->size;

                        /* Return false as this offset position is not possible. */
                        ret = false;
                    }
                }
                else
                {
                    /* Seek not supported, return with an error */
                    ret = false;
                    break;
                }
            }

            event_create_flush_start(&flush_event);
            pad_push_event(pad, &flush_event);

            event_create_flush_stop(&flush_event);
            pad_push_event(pad, &flush_event);

            memsrc->read_position = offset;

            /* If this data is a new segment sent after a seek, then send a new
             * segement event to the src pad. */
            event_create_new_segment(&new_segment, DATA_FORMAT_BYTES, offset);

            /* Push the new segment event to the pad. This is in sync with
             * the data flow as we have the stream lock right now.
             */
            pad_push_event(pad, &new_segment);

            STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]Send new segment\n");

            memsrc->end_of_stream = false;

            STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]Seek: %d, Read Pos: %d\n", EVENT_DATA(event),
                               memsrc->read_position);

            break;

        case EVENT_FLUSH_START:
        case EVENT_FLUSH_STOP:
        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return ret;
}

/*!
 *
 * @param pad: pad handle
 * @param query: query
 *
 * @brief Function Description: This function is the empty impl of the src pad
 * query handler.
 *
 * @returns uint8_t
 */

static uint8_t memsrc_handle_src_query(StreamPad *pad, StreamQuery *query)
{
    uint8_t ret = false;
    ElementMemSrc *memsrc;
    StreamData *data;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    CHK_ARGS(NULL == pad || NULL == query || NULL == QUERY_DATA(query), false);
    data = QUERY_DATA(query);

    /* get the parent element from the pad */
    memsrc = (ElementMemSrc *)PAD_PARENT(pad);
    CHK_ARGS(memsrc == NULL, false);

    switch (QUERY_TYPE(query))
    {
        case INFO_DURATION:
            if (QUERY_FORMAT(query) == DATA_FORMAT_BYTES && memsrc->size > 0)
            {
                data->value32u = memsrc->size;
                ret            = true;
            }
            else
            {
                ret = false;
            }
            break;

        case INFO_TIME_SEEKABLE:
            data->valuebool = false;
            ret             = true;
            break;

        case INFO_BYTE_SEEKABLE:
            data->valuebool = true;
            ret             = true;
            break;

            /* Try tag info from the the file source operations */
        case INFO_ALBUM:
        case INFO_ARTIST:
        case INFO_GENRE:
        case INFO_YEAR:
        case INFO_TITLE:
            ret = false;
            break;
        case INFO_SIZE:
            data->value32u = memsrc->size;
            ret            = true;
            break;
        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return ret;
}

/*!
 *
 * @param pad: Element pad
 * @param active: true if pad is to be activated else false to deactivate
 *
 * @brief Function activates or deactivates the memsrc source pad in PUSH mode.
 *
 */
static uint8_t memsrc_src_activate_push(StreamPad *pad, uint8_t active)
{
    ElementMemSrc *memsrc = (ElementMemSrc *)pad->parent;
    uint8_t ret           = false;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    /* Activate pad in PUSH mode */
    if (active)
    {
        /* set default chunk size if not already set */
        if (memsrc->chunk_size == 0)
        {
            memsrc->chunk_size = MEMSRC_DEFAULT_CHUNK_SIZE;
        }

        /* start of stream */
        memsrc->end_of_stream = false;
        /* reset to 0 */
        memsrc->read_position = 0;

        /* Check buffer location */
        if (memsrc->location == NULL)
        {
            STREAMER_FUNC_EXIT(DBG_FILESRC);
            return false;
        }

        /* Allocate buffer equal to the data type header size + chunk size */
        /* buffer -> ---------------
         *           | Data Header |
         *           ---------------
         *           | Actual data |
         *           | *********** |
         *           | *********** |
         *           | *********** |
         *           | *********** |
         *           ---------------
         */

        if (memsrc->mem_type == RAW_DATA)
        {
            memsrc->buffer = OSA_MemoryAllocate(sizeof(RawPacketHeader) + memsrc->chunk_size);
        }
        else if (memsrc->mem_type == AUDIO_DATA)
        {
            memsrc->buffer = OSA_MemoryAllocate(sizeof(AudioPacketHeader) + memsrc->chunk_size);
        }
        if (NULL == memsrc->buffer)
        {
            STREAMER_FUNC_EXIT(DBG_MEMSRC);
            return false;
        }

        if (memsrc->mem_type == RAW_DATA)
        {
            RawPacketHeader *pkt_hdr = NULL;
            /* set data type id */
            pkt_hdr = (RawPacketHeader *)memsrc->buffer;
            /* Init header data */
            pkt_hdr->id = RAW_DATA;
        }
        else if (memsrc->mem_type == AUDIO_DATA)
        {
            AudioPacketHeader *pkt_hdr = NULL;
            pkt_hdr                    = (AudioPacketHeader *)memsrc->buffer;
            pkt_hdr->sample_rate       = memsrc->sample_rate;
            pkt_hdr->bits_per_sample   = memsrc->bit_width;
            pkt_hdr->num_channels      = memsrc->num_channels;
            AUDIO_FORMAT(pkt_hdr)      = AUDIO_SET_FORMAT(0, 0, 1, memsrc->bit_width);
            pkt_hdr->chunk_size        = memsrc->chunk_size;
            /* Init header data */
            pkt_hdr->id = AUDIO_DATA;
        }

        if (memsrc->size == 0)
        {
            ret = false;
        }

        /* start the task to start streaming/pushing data */
        /* Create the task for the pad */
        ret = true;
    }
    else
    {
        /* reset */
        memsrc->size          = 0;
        memsrc->end_of_stream = false;
        memsrc->read_position = 0;

        /* Free the chunk buffer */
        if (memsrc->buffer)
        {
            OSA_MemoryFree(memsrc->buffer);
            memsrc->buffer = NULL;
        }
        ret = true;
    }

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return ret;
}

/*!
 *
 * @param pad: Element pad
 * @param active: true if pad is to be activated else false to deactivate
 *
 * @brief Function activates or deactivates the memsrc source pad in PULL mode.
 *
 */
static uint8_t memsrc_src_activate_pull(StreamPad *pad, uint8_t active)
{
    ElementMemSrc *memsrc = (ElementMemSrc *)pad->parent;
    uint8_t ret           = false;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    /* Activate pad in PULL mode */
    if (active)
    {
        /* set default chunk size if not already set */
        if (memsrc->chunk_size == 0)
        {
            memsrc->chunk_size = MEMSRC_DEFAULT_CHUNK_SIZE;
        }

        /* start of stream */
        memsrc->end_of_stream = false;
        if (memsrc->buffer)
        {
            OSA_MemoryFree(memsrc->buffer);
            memsrc->buffer = NULL;
        }

        /* reset to 0 */
        memsrc->read_position = 0;

        /* Check buffer location */
        if (memsrc->location == NULL)
        {
            goto no_location;
        }

        STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]Activate pull mode: %s\n", "memsrc");

        if (memsrc->size == 0)
        {
            ret = false;
        }
        ret = true;
    }
    else
    {
        /* Deactivate pad in PULL mode */
        STREAMER_LOG_DEBUG(DBG_MEMSRC, "[MemSRC]Deactivate pull mode: %s\n", "memsrc");

        /* reset */
        memsrc->size          = 0;
        memsrc->end_of_stream = false;
        memsrc->read_position = 0;

        /* Free the chunk buffer */
        if (memsrc->buffer)
        {
            OSA_MemoryFree(memsrc->buffer);
            memsrc->buffer = NULL;
        }

        ret = true;
    }

no_location:
    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return ret;
}

/*!
 *
 * @param element_ptr: element pointer
 * @param prop: properties
 * @param val_ptr: parameters
 *
 * @brief This function get memory source element properties
 *
 * @returns StreamErrorType
 *
 */
static int32_t memsrc_get_property(StreamElement *element_ptr, uint16_t prop, uint32_t *val_ptr)
{
    int32_t ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    if (NULL == val_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_MEMSRC);
        return STREAM_ERR_INVALID_ARGS;
    }

    switch (prop)
    {
        case PROP_MEMSRC_GET_CHUNK_SIZE:
            ret = memsrc_get_push_chunk_size((ElementHandle)element_ptr, val_ptr);
            break;

        default:
            ret = STREAM_ERR_INVALID_ARGS;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return ret;
}

/*!
 *
 * @param element_ptr: element pointer
 * @param prop: properties
 * @param val: parameters
 *
 * @brief This function set memory source element properties
 *
 * @returns StreamErrorType
 *
 */
static int32_t memsrc_set_property(StreamElement *element_ptr, uint16_t prop, uintptr_t val)
{
    int32_t ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_MEMSRC);

    switch (prop)
    {
        case PROP_MEMSRC_SET_BUFF:
        {
            MEMSRC_SET_BUFFER_T *desc_ptr = (MEMSRC_SET_BUFFER_T *)val;

            ret = memsrc_set_buffer((ElementHandle)element_ptr, desc_ptr->location, desc_ptr->size);
        }
        break;

        case PROP_MEMSRC_SET_CHUNK_SIZE:
            ret = memsrc_set_push_chunk_size((ElementHandle)element_ptr, val);
            break;

        case PROP_MEMSRC_SET_SAMPLE_RATE:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "set sample rate:%d\n", val);
            if (val <= 0 || val > 96000)
            {
                ret = STREAM_ERR_INVALID_ARGS;
            }
            else
            {
                ElementMemSrc *src = (ElementMemSrc *)element_ptr;
                src->sample_rate   = (uint32_t)val;
            }
            break;

        case PROP_MEMSRC_SET_NUM_CHANNELS:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "set number of channels:%d\n", val);
            if (val <= 0 || val > 8)
            {
                ret = STREAM_ERR_INVALID_ARGS;
            }
            else
            {
                ElementMemSrc *src = (ElementMemSrc *)element_ptr;
                src->num_channels  = (uint8_t)val;
            }
            break;

        case PROP_MEMSRC_SET_BIT_WIDTH:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "set bit width:%d\n", val);
            if (val <= 0 || val > 32 || val % 8 != 0)
            {
                ret = STREAM_ERR_INVALID_ARGS;
            }
            else
            {
                ElementMemSrc *src = (ElementMemSrc *)element_ptr;
                src->bit_width     = (uint8_t)val;
            }
            break;

        case PROP_MEMSRC_SET_MEM_TYPE:
            STREAMER_LOG_DEBUG(DBG_MEMSRC, "set mem type:%d\n", val);
            if (val > 1)
            {
                ret = STREAM_ERR_INVALID_ARGS;
            }
            else
            {
                ElementMemSrc *src = (ElementMemSrc *)element_ptr;
                src->mem_type      = (uint8_t)val;
                if (val == AUDIO_DATA)
                {
                    PadSrc *pad          = (PadSrc *)ELEMENT_SRC_PAD(src, 0);
                    pad->process_handler = memsrc_src_pad_process;
                }
            }
            break;
        default:
            ret = STREAM_ERR_INVALID_ARGS;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_MEMSRC);
    return ret;
}
