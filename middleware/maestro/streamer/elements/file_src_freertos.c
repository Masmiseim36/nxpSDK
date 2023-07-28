/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    file_src_freertos.c
 * @brief  this file implement file source element.
 */

#include <string.h>
#include <stdio.h>

#include "audio_cfg.h"
#include "streamer_element.h"
#include "file_src_freertos.h"
#include "audio_sink.h"
#include "pipeline.h"
#include "streamer_element_properties.h"

int32_t filesrc_src_pad_process(StreamPad *pad);
static int32_t filesrc_change_state(StreamElement *element, PipelineState state);
static uint8_t filesrc_handle_src_event(StreamPad *pad, StreamEvent *event);
static uint8_t filesrc_handle_src_query(StreamPad *pad, StreamQuery *event);
static uint8_t filesrc_src_activate_push(StreamPad *pad, uint8_t active);
static uint8_t filesrc_src_activate_pull(StreamPad *pad, uint8_t active);
static uint8_t filesrc_src_activate(StreamPad *pad, uint8_t active);
static FlowReturn filesrc_pull(StreamPad *pad, StreamBuffer *buffer, uint32_t size, uint32_t offset);
static int32_t filesrc_get_property(StreamElement *element_ptr, uint16_t prop, uint64_t *val_ptr);
static int32_t filesrc_set_property(StreamElement *element_ptr, uint16_t prop, uintptr_t val);

int32_t filesrc_init(StreamElement *element)
{
    PadSrc *pad = NULL;
    int32_t i   = 0;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    ElementFileSrc *filesrc = (ElementFileSrc *)element;

    /* filesrc element specific init */
    filesrc->fd     = -1;
    filesrc->buffer = NULL;
    filesrc->size   = 0;
    memset(&filesrc->location, 0, MAX_LOCATION_PATH_LENGTH);
    filesrc->chunk_size    = FILESRC_DEFAULT_CHUNK_SIZE;
    filesrc->end_of_stream = false;
    filesrc->read_position = 0;
    filesrc->file_type     = RAW_DATA;

    /* set element change state function */
    filesrc->change_state = filesrc_change_state;
    filesrc->set_property = filesrc_set_property;
    filesrc->get_property = filesrc_get_property;

    /* init elements pads */
    for (i = 0; i < NUM_SRCS(TYPE_ELEMENT_FILE_SRC); i++)
    {
        pad = (PadSrc *)ELEMENT_SRC_PAD(filesrc, i);

        /* Set handlers */
        pad->event_handler      = filesrc_handle_src_event;
        pad->query_handler      = filesrc_handle_src_query;
        pad->activation_handler = filesrc_src_activate;

        pad->activate_pull = filesrc_src_activate_pull;
        pad->activate_push = filesrc_src_activate_push;

        /* Set source pad pull function */
        pad->pull_handler = filesrc_pull;

        /* decoder will be called instead when using CCI */
        pad->process_handler        = NULL;
        filesrc->g_fd[i]            = -1;
        filesrc->g_read_position[i] = 0;
        pad->process_precheck       = NULL; /* File Source will not block */
    }

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return 0;
}

int32_t filesrc_set_location(ElementHandle element, char *path)
{
    ElementFileSrc *src  = (ElementFileSrc *)element;
    StreamReturnType ret = STREAM_OK;
    int32_t fd;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    CHK_ARGS(src == NULL || path == NULL, STREAM_ERR_INVALID_ARGS);

    fd = file_open(path, FILE_RDONLY);
    if (fd >= 0)
    {
        file_close(fd);
        strncpy(src->location, path, (MAX_LOCATION_PATH_LENGTH - 1));
        if (src->location[MAX_LOCATION_PATH_LENGTH - 1] != 0)
        {
            STREAMER_LOG_ERR(DBG_FILESRC, ERRCODE_INTERNAL, "[FileSRC]reach the max pathname length\n");
            ret = STREAM_ERR_INVALID_ARGS;
        }
    }
    else
    {
        STREAMER_LOG_ERR(DBG_FILESRC, (uint32_t)fd, "[FileSRC]f_open failed\n");
        ret = STREAM_ERR_GENERAL;
    }

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return ret;
}

int32_t filesrc_get_location(ElementHandle element, char **pp_path)
{
    ElementFileSrc *src  = (ElementFileSrc *)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    CHK_ARGS(src == NULL || pp_path == NULL, STREAM_ERR_INVALID_ARGS);

    *pp_path = src->location;

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return ret;
}

int32_t filesrc_set_push_chunk_size(ElementHandle element, uint32_t chunk_size)
{
    ElementFileSrc *src  = (ElementFileSrc *)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    CHK_ARGS(src == NULL || chunk_size == 0, STREAM_ERR_INVALID_ARGS);

    src->chunk_size = chunk_size;

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return ret;
}

int32_t filesrc_get_push_chunk_size(ElementHandle element, uint64_t *chunk_size)
{
    ElementFileSrc *src  = (ElementFileSrc *)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    CHK_ARGS(src == NULL || chunk_size == NULL, STREAM_ERR_INVALID_ARGS);

    *chunk_size = src->chunk_size;

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return ret;
}

/*!
 *
 * @param element: Pointer to element
 * @param state: changed state.
 *
 * @brief Function handles the state change of the element in a pipeline.
 */
static int32_t filesrc_change_state(StreamElement *element, PipelineState state)
{
    PipelineStateChange state_change;
    ElementFileSrc *filesrc = (ElementFileSrc *)element;
    int32_t ret             = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    CHK_ARGS(NULL == filesrc, STREAM_ERR_INVALID_ARGS);

    /* get the transition status with the new passed state and the existing
     * state of the decoder element
     */
    state_change = (PipelineStateChange)STATE_TRANSITION(filesrc->state, state);

    switch (state_change)
    {
        case STATE_CHANGE_NULL_TO_READY:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]STATE_CHANGE_NULL_TO_READY\n");
            break;
        case STATE_CHANGE_READY_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]STATE_CHANGE_READY_TO_PAUSED\n");
            break;
        case STATE_CHANGE_PAUSED_TO_PLAYING:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]STATE_CHANGE_PAUSED_TO_PLAYING\n");
            break;
        case STATE_CHANGE_PLAYING_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]STATE_CHANGE_PLAYING_TO_PAUSED\n");
            break;
            /* do nothing */
        case STATE_CHANGE_PAUSED_TO_READY:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]STATE_CHANGE_PAUSED_TO_READY\n");
            break;
            /* Pads will be deactivated here. */
        case STATE_CHANGE_READY_TO_NULL:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]STATE_CHANGE_READY_TO_NULL\n");
            break;
            /* close resources if anything was opened
             * N/A for decoder
             */
        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return ret;
}

/*!
 *
 * @param pad: Queue element src pad
 * @param active: To activate or deactivate pad.
 *
 * @brief Function is the queue src pad activatation handler. Activation is
 * handled by the peer sink pad, this should just handle the deactivation.
 */
static uint8_t filesrc_src_activate(StreamPad *pad, uint8_t active)
{
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    STREAMER_LOG_DEBUG(DBG_MEMSRC, "filesrc_src_activate(%d)\n", (uint32_t)active);

    if ((uint8_t) false == active)
    {
        PadScheduling mode = PAD_SCHEDULING(pad);

        if (SCHEDULING_PUSH == mode)
        {
            ElementFileSrc *filesrc = (ElementFileSrc *)PAD_PARENT(pad);

            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]Deactivate src pad\n");
            ret = pad_activate_push(pad, false);
            if ((uint8_t) true == ret)
            {
                STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]Close FileSRC\n");
                if (filesrc->fd >= 0)
                {
                    if (file_close(filesrc->fd) != 0)
                        ret = false;
                    filesrc->fd = -1;
                }

                /* reset */
                filesrc->size          = 0;
                filesrc->end_of_stream = false;
                filesrc->read_position = 0;

                /* Free the chunk buffer */
                if (filesrc->buffer)
                {
                    OSA_MemoryFree(filesrc->buffer);
                    filesrc->buffer = NULL;
                }
            }
        }
        else if (SCHEDULING_PULL == mode)
        {
            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]Deactivate src pad\n");
            ret = pad_activate_pull(pad, active);
        }
    }

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return ret;
}

/*!
 *
 * @param filesrc: Pointer to filesrc element
 * @param offset: Offset of the file to read data from
 * @param length: Number of bytes to read
 * @param buf: Pointer to buffer to read data
 *
 * @brief Function reads the data from the file specified by the element.
 * NOTE: Function may or may not be able to read the required
 * length of data.
 * Returns FLOW_EOS when EOF file is encountered.
 * or FLOW_UNEXPECTED when read fails.
 *
 * */
static FlowReturn filesrc_read(ElementFileSrc *filesrc, uint32_t offset, uint32_t length, StreamBuffer *buf)
{
    int32_t ret = 0;
    int32_t bytesRead;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]pos: %d, offset: %d\n", filesrc->read_position, offset);

    /* if the required offset is not same as the last read offset */
    if (filesrc->read_position != offset)
    {
        ret = file_seek(filesrc->fd, offset);
        if (ret == 0)
        {
            /* update read position */
            filesrc->read_position = offset;
        }
        else
        {
            STREAMER_LOG_ERR(DBG_FILESRC, (uint32_t)ret, "[FileSRC] f_lseek failure\n");
        }
    }

    /* Seek done ? */
    if (ret == 0)
    {
        int32_t readlen  = (int32_t)length;
        int8_t *data_buf = NULL;

        STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]read: %d\n", length);

        if (filesrc->read_position >= filesrc->size && filesrc->size > 0)
        {
            goto eos;
        }
        if (filesrc->file_type == RAW_DATA)
        {
            /* Set pointer to read data to buffer */
            data_buf = buf->buffer + sizeof(RawPacketHeader);
        }
        else if (filesrc->file_type == AUDIO_DATA)
        {
            /* Set pointer to read data to buffer */
            data_buf = buf->buffer + sizeof(AudioPacketHeader);
        }

        /* read length of data */
        bytesRead = file_read(filesrc->fd, (void *)data_buf, (uint32_t)readlen);
        if (bytesRead < 0)
        {
            STREAMER_LOG_ERR(DBG_FILESRC, bytesRead, "[FileSRC] f_read failure\n");
            return FLOW_UNEXPECTED;
        }

        /* increment read position */
        filesrc->read_position += (uint32_t)bytesRead;

        /* update buffer offset */
        buf->offset = offset;

        if (filesrc->file_type == RAW_DATA)
        {
            /* set buffer size = header size + actual data size read */
            buf->size = sizeof(RawPacketHeader) + bytesRead;
        }
        else if (filesrc->file_type == AUDIO_DATA)
        {
            /* set buffer size = header size + actual data size read */
            buf->size = sizeof(AudioPacketHeader) + bytesRead;
        }

        STREAMER_FUNC_EXIT(DBG_FILESRC);
        return FLOW_OK;
    }

eos:
    STREAMER_LOG_DEBUG(DBG_FILESRC, "\tEND OF STREAM (READ)\n");
    STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]Read pos: %d, size: %d, offset: %d\n", filesrc->read_position,
                       filesrc->size, offset);
    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return FLOW_EOS;
}

/*!
 * filesrc_src_pad_process:
 *
 * @param pad: Element pad
 *
 * @brief Function is the loop function of the filesrc source pad which
 * runs in the task thread. It reads the chunk_size of data from
 * the file and pushes it to peer element sink pad. When EOF file
 * is reached, the task goes to PAUSE state.
 *
 */
int32_t filesrc_src_pad_process(StreamPad *pad)
{
    ElementFileSrc *filesrc = (ElementFileSrc *)pad->parent;
    FlowReturn ret          = FLOW_OK;
    uint32_t length = 0, offset = 0;
    StreamBuffer buf;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    /*2. Get the chunk size to read from file */
    length = filesrc->chunk_size;
    offset = filesrc->read_position;

    /* init buffer metadata */
    buf.buffer = NULL;
    buf.size   = 0;
    buf.time   = (uint32_t)-1; /* Don't know anything about time */
    buf.offset = 0;

    /* Its an end of stream */
    if (filesrc->end_of_stream != (uint8_t) true)
    {
        /* Allocate required buffer to read data */
        buf.buffer = filesrc->buffer;

        /* read chunk_size of data */
        ret = filesrc_read(filesrc, offset, length, &buf);
        if (ret == FLOW_EOS)
        {
            StreamEvent event;

            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]EOS\n");

            filesrc->end_of_stream = true;

            /* send eos to peer pad */
            event_create_eos(&event);
            pad_push_event(pad, &event);

            STREAMER_FUNC_EXIT(DBG_FILESRC);
            return STREAM_OK;
        }
        else if (ret == FLOW_UNEXPECTED)
        {
            STREAMER_LOG_ERR(DBG_FILESRC, ret, "[FileSRC] Unexpected error %d\n", ret);
            STREAMER_FUNC_EXIT(DBG_FILESRC);
            /* send out EOS message to stop the playing */
            send_msg_element((StreamElement *)filesrc, MSG_EOS, 0);
            return STREAM_OK;
        }
        else if (ret != FLOW_ERROR)
        {
            /* Update audio packet header values as values may have been changed in the AUDIO_PROC element as part of a
             * crossover preset */
            if (filesrc->file_type == AUDIO_DATA)
            {
                AudioPacketHeader *pkt_hdr = NULL;
                /* set data type id */
                pkt_hdr                  = (AudioPacketHeader *)buf.buffer;
                pkt_hdr->sample_rate     = filesrc->sample_rate;
                pkt_hdr->bits_per_sample = filesrc->bit_width;
                pkt_hdr->num_channels    = filesrc->num_channels;
                AUDIO_FORMAT(pkt_hdr)    = AUDIO_SET_FORMAT(0, 0, 1, filesrc->bit_width);
                pkt_hdr->chunk_size      = filesrc->chunk_size;
            }

            /* No error then forward the data */
            /* push data to peer sink pad */
            if (pad_push(pad, &buf) != FLOW_OK)
            {
                STREAMER_LOG_ERR(DBG_FILESRC, ERRCODE_GENERAL_ERROR, "[FileSRC]Flow not ok\n");
                STREAMER_FUNC_EXIT(DBG_FILESRC);
                return STREAM_ERR_GENERAL;
            }
        }
        else
        {
            STREAMER_LOG_ERR(DBG_FILESRC, ERRCODE_GENERAL_ERROR, "[FileSRC] Streamer general error\n");
            STREAMER_FUNC_EXIT(DBG_FILESRC);
            return STREAM_ERR_GENERAL;
        }
    }
    else
    {
        // Due to forcing a context switch
        OSA_TimeDelay(1);
    }

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return STREAM_OK;
}

/*!
 *
 * @param pad: Element pad
 * @param buffer: Pointer of the buffer to read data
 * @param size: Number of bytes to read
 * @param offset: Offset to the file
 *
 * @brief Function is the pull function of the filesrc source pad in
 * SCHEDULING_PULL mode. It reads the size of data from
 * the offset to buffer.
 *
 */
static FlowReturn filesrc_pull(StreamPad *pad, StreamBuffer *buffer, uint32_t size, uint32_t offset)
{
    ElementFileSrc *filesrc = (ElementFileSrc *)pad->parent;
    FlowReturn ret          = FLOW_OK;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    /* Put Raw header in the buffer */
    ((PacketHeader *)BUFFER_PTR(buffer))->id = RAW_DATA;

    uint32_t StreamNo = 0;
    if ((StreamPad *)&filesrc->src_pad[0] == pad)
    {
        // Compressed Data PAD
        StreamNo = 0;
    }
    else if ((StreamPad *)&filesrc->src_pad[1] == pad)
    {
        // Atom Data PAD
        StreamNo = 1;
    }
    else
    {
        // Stsz Data PAD
        StreamNo = 2;
    }

    filesrc->read_position = filesrc->g_read_position[StreamNo];
    filesrc->fd            = filesrc->g_fd[StreamNo];

    /* read chunk_size of data */
    ret = filesrc_read(filesrc, offset, size, buffer);
    if (ret == FLOW_UNEXPECTED || ret == FLOW_EOS)
    {
        /* Its an end of stream */
        if (filesrc->end_of_stream != (uint8_t) true)
        {
            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]Pull EOS");

            /* Need not send the EOS event here. The element calling this
             * function should check the return value and send EOS event
             * downstream.
             */
            filesrc->end_of_stream = true;
        }
        goto pause;
    }

    filesrc->g_read_position[StreamNo] = filesrc->read_position;

pause:
    STREAMER_FUNC_EXIT(DBG_FILESRC);
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
static uint8_t filesrc_handle_src_event(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret = true;
    PadScheduling mode;
    ElementFileSrc *filesrc = (ElementFileSrc *)PAD_PARENT(pad);
    uint32_t offset         = EVENT_DATA(event);
    StreamDataFormat format = EVENT_FORMAT(event);
    uint8_t seek_in_time    = false;
    StreamEvent new_segment;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    /* handle the event */
    switch (EVENT_TYPE(event))
    {
            /* case EVENT_EOS: */
            /* case EVENT_NEWSEGMENT: */
        case EVENT_SEEK:
            /* Should come here only if in push mode. In case of pull mode, the
             * next element will change the offset in get_range function to
             * seek.
             */

            mode = PAD_SCHEDULING(pad);

            /* File source needs seek event in bytes format and the stream
             * should still be active
             */

            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]seek from %d to %d\n", filesrc->read_position, offset);

            /* First check if it is seekable */
            if (DATA_FORMAT_BYTES == format && filesrc->byte_seekable)
            {
                /* If the offset specified is more than the size of the file
                 * then restrict the offset to the file size. When file src
                 * thread will start giving data agin it will generate an EOS.
                 */
                if (offset > filesrc->size && filesrc->size > 0)
                {
                    STREAMER_LOG_WARN(DBG_FILESRC, ERRCODE_INTERNAL,
                                      "[FileSRC]Offset more than the file size: "
                                      "offset=%d size=%d\n",
                                      offset, filesrc->size);
                    offset = filesrc->size;

                    /* Return false as this offset position is not possible. */
                    ret = false;
                }
            }
            else if (DATA_FORMAT_TIME == format && filesrc->time_seekable)
            {
                seek_in_time = true;
            }
            else
            {
                /* Seek not supported, return with an error */
                ret = false;
                break;
            }

            if (mode == SCHEDULING_PUSH)
            {
                if (0 == file_seek(filesrc->fd, offset))
                {
                    filesrc->read_position = offset;
                }
            }
            if ((uint8_t) true == seek_in_time)
            {
                /* If this data is a new segment sent after a seek,
                 * then send a new segment event to the src pad. */
                event_create_new_segment(&new_segment, DATA_FORMAT_TIME, EVENT_DATA(event));
            }
            else
            {
                /* If this data is a new segment sent after a seek,
                 * then send a new segment event to the src pad. */
                event_create_new_segment(&new_segment, DATA_FORMAT_BYTES, offset);
            }
            /* Push the new segment event to the pad. This is in sync with
             * the data flow as we have the stream lock right now.
             */
            pad_push_event(pad, &new_segment);

            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]Send new segment\n");

            filesrc->end_of_stream = false;

            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]Seek: %d, Read Pos: %d\n", EVENT_DATA(event),
                               filesrc->read_position);
            break;

        case EVENT_FLUSH_START:
        case EVENT_FLUSH_STOP:
        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return ret;
}

/*!
 * @brief file_query
 * @details Attempt to query information from a file
 * @param fd   File handle
 * @param info Type of query
 * @param data Data pointer to result data
 * @returns Status of query
 * @retval STREAM_OK                    Query was successful
 * @retval STREAM_ERR_INFO_ABSENT       File does not supported specified query
 */
int32_t file_query(int32_t fd, StreamInfoType info, StreamData *data)
{
    STREAMER_FUNC_ENTER(DBG_FILESRC);

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return STREAM_ERR_INFO_ABSENT;
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

static uint8_t filesrc_handle_src_query(StreamPad *pad, StreamQuery *query)
{
    uint8_t ret = false;
    ElementFileSrc *filesrc;
    StreamData *data;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    CHK_ARGS(NULL == pad || NULL == query || NULL == QUERY_DATA(query), false);
    data = QUERY_DATA(query);

    /* get the parent element from the pad */
    filesrc = (ElementFileSrc *)PAD_PARENT(pad);
    CHK_ARGS(filesrc == NULL, false);

    switch (QUERY_TYPE(query))
    {
        case INFO_DURATION:
            if (QUERY_FORMAT(query) == DATA_FORMAT_BYTES && filesrc->size > 0)
            {
                data->value32u = filesrc->size;
                ret            = true;
            }
            else if (QUERY_FORMAT(query) == DATA_FORMAT_TIME && filesrc->duration > 0)
            {
                data->value32u = filesrc->duration;
                ret            = true;
            }
            else
            {
                ret = false;
            }
            break;

        case INFO_TIME_SEEKABLE:
            data->valuebool = filesrc->time_seekable;
            ret             = true;
            break;

        case INFO_BYTE_SEEKABLE:
            data->valuebool = filesrc->byte_seekable;
            ret             = true;
            break;

            /* Try tag info from the the file source operations */
        case INFO_ALBUM:
        case INFO_ARTIST:
        case INFO_GENRE:
        case INFO_YEAR:
        case INFO_TITLE:
            if (STREAM_OK == file_query(filesrc->fd, QUERY_TYPE(query), data))
                ret = true;
            break;
        case INFO_SIZE:
            data->value32u = filesrc->size;
            ret            = true;
            break;

        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return ret;
}

/*!
 *
 * @param pad: Element pad
 * @param active: true if pad is to be activated else false to deactivate
 *
 * @brief Function activates or deactivates the filesrc source pad in PUSH mode.
 *
 */
static uint8_t filesrc_src_activate_push(StreamPad *pad, uint8_t active)
{
    ElementFileSrc *filesrc = (ElementFileSrc *)pad->parent;
    uint8_t ret             = false;
    int32_t fd;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    /* Activate pad in PUSH mode */
    if (active)
    {
        /* set default chunk size if not already set */
        if (filesrc->chunk_size == 0)
        {
            filesrc->chunk_size = FILESRC_DEFAULT_CHUNK_SIZE;
        }

        /* start of stream */
        filesrc->end_of_stream = false;
        if (filesrc->buffer)
        {
            OSA_MemoryFree(filesrc->buffer);
            filesrc->buffer = NULL;
        }

        /* reset to 0 */
        filesrc->read_position = 0;

        /* 1. Open file */
        if (filesrc->location[0] == '\0')
        {
            STREAMER_FUNC_EXIT(DBG_FILESRC);
            return false;
        }

        STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]Activate push mode: %s\n", filesrc->location);

        /* Set the fd to the global. */
        fd          = file_open(filesrc->location, FILE_RDONLY);
        filesrc->fd = fd;
        if (fd >= 0)
        {
            /* Allocate buffer equal to the data type header
             * size + chunk size */
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

            if (filesrc->file_type == RAW_DATA)
            {
                filesrc->buffer = OSA_MemoryAllocate(sizeof(RawPacketHeader) + filesrc->chunk_size);
            }
            else if (filesrc->file_type == AUDIO_DATA)
            {
                filesrc->buffer = OSA_MemoryAllocate(sizeof(AudioPacketHeader) + filesrc->chunk_size);
            }
            if (NULL == filesrc->buffer)
            {
                STREAMER_FUNC_EXIT(DBG_FILESRC);
                return false;
            }

            if (filesrc->file_type == RAW_DATA)
            {
                RawPacketHeader *pkt_hdr = NULL;
                /* set data type id */
                pkt_hdr = (RawPacketHeader *)filesrc->buffer;
                /* Init header data */
                pkt_hdr->id = RAW_DATA;
            }
            else if (filesrc->file_type == AUDIO_DATA)
            {
                AudioPacketHeader *pkt_hdr = NULL;
                /* set data type id */
                pkt_hdr                  = (AudioPacketHeader *)filesrc->buffer;
                pkt_hdr->sample_rate     = filesrc->sample_rate;
                pkt_hdr->bits_per_sample = filesrc->bit_width;
                pkt_hdr->num_channels    = filesrc->num_channels;
                AUDIO_FORMAT(pkt_hdr)    = AUDIO_SET_FORMAT(0, 0, 1, filesrc->bit_width);
                pkt_hdr->chunk_size      = filesrc->chunk_size;
                /* Init header data */
                pkt_hdr->id = AUDIO_DATA;
            }

            /* Get the seekable in bytes and size in bytes */
            filesrc->byte_seekable = true;
            filesrc->size          = file_getsize(filesrc->fd);

            STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]Size: %d, seekable: %d\n", filesrc->size, filesrc->byte_seekable);

            /* Get the duration and seekable in msec */
            filesrc->time_seekable = true;
            /* TODO: calculate duration correctly. */
            filesrc->duration = filesrc->size;
            ret               = true;
        }
        else
        {
            STREAMER_LOG_ERR(DBG_FILESRC, ret, "[FileSRC]f_open failed\n");
            ret = false;
        }
    }
    else
    {
        /* Deactivate pad in PUSH mode */
        STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]Deactivate push mode: %s\n", filesrc->location);

        if (filesrc->fd >= 0)
        {
            if (0 != file_close(filesrc->fd))
            {
                STREAMER_FUNC_EXIT(DBG_FILESRC);
                return false;
            }
            filesrc->fd = -1;
        }

        /* reset */
        filesrc->size          = 0;
        filesrc->end_of_stream = false;
        filesrc->read_position = 0;

        /* Free the chunk buffer */
        if (filesrc->buffer)
        {
            OSA_MemoryFree(filesrc->buffer);
            filesrc->buffer = NULL;
        }
        ret = true;
    }

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return ret;
}

/*!
 *
 * @param pad: Element pad
 * @param active: true if pad is to be activated else false to deactivate
 *
 * @brief Function activates or deactivates the filesrc source pad in PULL mode.
 *
 * */
static uint8_t filesrc_src_activate_pull(StreamPad *pad, uint8_t active)
{
    ElementFileSrc *filesrc = (ElementFileSrc *)pad->parent;
    uint8_t ret             = false;
    int32_t fd;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    uint32_t StreamNo = 0;
    if ((StreamPad *)&filesrc->src_pad[0] == pad)
    {
        // Compressed Data PAD
        StreamNo = 0;
    }
    else if ((StreamPad *)&filesrc->src_pad[1] == pad)
    {
        // Atom Data PAD
        StreamNo = 1;
    }
    else
    {
        // Stsz Data PAD
        StreamNo = 2;
    }

    /* Activate pad in PULL mode */
    if (active)
    {
        /* set default chunk size if not already set */
        if (filesrc->chunk_size == 0)
        {
            filesrc->chunk_size = FILESRC_DEFAULT_CHUNK_SIZE;
        }

        /* start of stream */
        filesrc->end_of_stream = false;
        if (filesrc->buffer)
        {
            OSA_MemoryFree(filesrc->buffer);
            filesrc->buffer = NULL;
        }

        /* reset to 0 */
        filesrc->read_position             = 0;
        filesrc->g_read_position[StreamNo] = 0;

        /* 1. Open file */
        if (filesrc->location[0] == '\0')
        {
            goto no_filename;
        }

        STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]Activate pull mode: %s\n", filesrc->location);

        /* Set the fd to the global. */
        fd                      = file_open(filesrc->location, FILE_RDONLY);
        filesrc->fd             = fd;
        filesrc->g_fd[StreamNo] = filesrc->fd;

        if (fd >= 0)
        {
            /* Get the seekable in bytes and size in bytes */
            filesrc->byte_seekable = true;
            filesrc->size          = file_getsize(filesrc->fd);
            /* Get the duration and seekable in msec */
            filesrc->time_seekable = true;
            /* TODO: calculate duration correctly. */
            filesrc->duration = filesrc->size;

            ret = true;
        }
        else
        {
            STREAMER_LOG_ERR(DBG_FILESRC, ret, "[FileSRC]f_open failed\n");
            ret = false;
        }
    }
    else
    {
        /* Deactivate pad in PULL mode */
        STREAMER_LOG_DEBUG(DBG_FILESRC, "[FileSRC]Deactivate pull mode: %s\n", filesrc->location);
        filesrc->fd = filesrc->g_fd[StreamNo];
        if (filesrc->fd >= 0)
        {
            if (0 != file_close(filesrc->fd))
            {
                ret = false;
            }
            filesrc->fd             = -1;
            filesrc->g_fd[StreamNo] = -1;
        }

        /* reset */
        filesrc->size                      = 0;
        filesrc->end_of_stream             = false;
        filesrc->read_position             = 0;
        filesrc->g_read_position[StreamNo] = 0;
        if (filesrc->buffer)
        {
            OSA_MemoryFree(filesrc->buffer);
            filesrc->buffer = NULL;
        }

        ret = true;
    }

no_filename:
    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return ret;
}

/*!
 *
 * @param element_ptr: element pointer
 * @param prop: properties
 * @param val_ptr: parameters
 *
 * @brief This function get file source element properties
 *
 * @returns StreamErrorType
 *
 */
static int32_t filesrc_get_property(StreamElement *element_ptr, uint16_t prop, uint64_t *val_ptr)
{
    int32_t ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    if (NULL == val_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_FILESRC);
        return STREAM_ERR_INVALID_ARGS;
    }

    switch (prop)
    {
        case PROP_FILESRC_GET_CHUNK_SIZE:
            ret = filesrc_get_push_chunk_size((ElementHandle)element_ptr, val_ptr);
            break;

        default:
            ret = STREAM_ERR_INVALID_ARGS;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return ret;
}

/*!
 *
 * @param element_ptr: element pointer
 * @param prop: properties
 * @param val: parameters
 *
 * @brief This function set file source element properties
 *
 * @returns StreamErrorType
 *
 */
static int32_t filesrc_set_property(StreamElement *element_ptr, uint16_t prop, uintptr_t val)
{
    int32_t ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_FILESRC);

    switch (prop)
    {
        case PROP_FILESRC_SET_LOCATION:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "set location:%s\n", (int8_t *)val);
            ret = filesrc_set_location((ElementHandle)element_ptr, (char *)val);
            break;

        case PROP_FILESRC_SET_CHUNK_SIZE:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "set chunk size:%d\n", val);
            ret = filesrc_set_push_chunk_size((ElementHandle)element_ptr, (uint32_t)val);
            break;

        case PROP_FILESRC_SET_SAMPLE_RATE:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "set sample rate:%d\n", val);
            if (val <= 0 || val > 96000)
            {
                ret = STREAM_ERR_INVALID_ARGS;
            }
            else
            {
                ElementFileSrc *src = (ElementFileSrc *)element_ptr;
                src->sample_rate    = (uint32_t)val;
            }
            break;

        case PROP_FILESRC_SET_NUM_CHANNELS:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "set number of channels:%d\n", val);
            if (val <= 0 || val > 8)
            {
                ret = STREAM_ERR_INVALID_ARGS;
            }
            else
            {
                ElementFileSrc *src = (ElementFileSrc *)element_ptr;
                src->num_channels   = (uint8_t)val;
            }
            break;

        case PROP_FILESRC_SET_BIT_WIDTH:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "set bit width:%d\n", val);
            if (val <= 0 || val > 32 || val % 8 != 0)
            {
                ret = STREAM_ERR_INVALID_ARGS;
            }
            else
            {
                ElementFileSrc *src = (ElementFileSrc *)element_ptr;
                src->bit_width      = (uint8_t)val;
            }
            break;

        case PROP_FILESRC_SET_FILE_TYPE:
            STREAMER_LOG_DEBUG(DBG_FILESRC, "set file type:%d\n", val);
            if (val > 1)
            {
                ret = STREAM_ERR_INVALID_ARGS;
            }
            else
            {
                ElementFileSrc *src = (ElementFileSrc *)element_ptr;
                src->file_type      = (uint8_t)val;
                if (val == AUDIO_DATA)
                {
                    PadSrc *pad          = (PadSrc *)ELEMENT_SRC_PAD(src, 0);
                    pad->process_handler = filesrc_src_pad_process;
                }
            }
            break;

        default:
            ret = STREAM_ERR_INVALID_ARGS;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_FILESRC);
    return ret;
}
