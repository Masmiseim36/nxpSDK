/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    file_sink.c
 * @brief  This file contains the implementation for the File Sink
 */

#include <string.h>
#include "file_sink.h"
#include "pipeline.h"
#include "streamer_element_properties.h"

#include "file_utils.h"

/**
 * @brief Pull size for the AUDIO_PROC test pipeline with file source and file sink
 *
 */
#define FILESINK_PULL_SIZE 1920

/* Global variables */

/**
 * @brief Structure that holds information necessary for writing to a file
 *
 */
static FileDump file_dump;

/* Thread for writing data to the SD card */
void FILE_Dump_Task(void *param)
{
    /* Local variables */
    ElementFileSink *file_sink_element_loc = NULL;
    int8_t *data_ptr_loc                   = NULL;
    uint32_t size_loc                      = 0;
    uint32_t data_ptr_size_loc             = 0;
    int32_t ret_count                      = 0;

    while (1)
    {
        OSA_MutexLock(&(file_dump.fileDataMutex), osaWaitForever_c);

        /* Check if any data is available. If not, wait. */
        while ((file_dump.size < 1280) && file_dump.run)
        {
            OSA_MutexUnlock(&(file_dump.fileDataMutex));
            OSA_SemaphoreWait(file_dump.sem_Read, osaWaitForever_c);
            OSA_MutexLock(&(file_dump.fileDataMutex), osaWaitForever_c);
        }

        /* Check if the task should run and wrote down all available data */
        if (!file_dump.run && (file_dump.size == 0))
        {
            OSA_MutexUnlock(&(file_dump.fileDataMutex));
            break;
        }

        /* Create local copy of the global data */
        file_sink_element_loc = file_dump.file_sink_element;
        size_loc              = file_dump.size;

        /* If the new size differs from the previous size, then realloc the data_ptr_loc buffer */
        if (size_loc > data_ptr_size_loc)
        {
            data_ptr_size_loc = size_loc;
            if (data_ptr_loc != NULL)
            {
                OSA_MemoryFree(data_ptr_loc);
                data_ptr_loc = NULL;
            }
            if ((data_ptr_loc = (int8_t *)OSA_MemoryAllocate(data_ptr_size_loc)) == NULL)
            {
                STREAMER_LOG_ERR(DBG_FILE_SINK, ERRCODE_GENERAL_ERROR,
                                 "file sink [FILE_Dump_Task] (OSA_MemoryAllocate) error - lack of free memory.\n");
                file_dump.ret = FLOW_ERROR;
                break;
            }
        }

        /* Copy data to local variable */
        memcpy(data_ptr_loc, file_dump.data_ptr, size_loc);
        file_dump.size -= size_loc;

        OSA_SemaphorePost(file_dump.sem_Write);

        OSA_MutexUnlock(&(file_dump.fileDataMutex));

        /* Write data to the SD card */
        if (file_sink_element_loc->fd >= 0)
        {
            if (size_loc)
            {
                ret_count = file_write(file_sink_element_loc->fd, data_ptr_loc, size_loc);
                if (ret_count < 0)
                {
                    STREAMER_LOG_ERR(DBG_FILE_SINK, ERRCODE_GENERAL_ERROR, "file sink (f_write)error no %d.\n",
                                     ret_count);
                }
                if ((uint32_t)ret_count < size_loc)
                {
                    STREAMER_LOG_DEBUG(DBG_FILE_SINK, "file sink write failed 2\n");
                    STREAMER_LOG_DEBUG(DBG_FILE_SINK, "write %d, written %d \n", size_loc, ret_count);
                    file_dump.ret = FLOW_ERROR;
                    break;
                }
            }
        }
        else
        {
            file_dump.ret = FLOW_ERROR;
            break;
        }

        portYIELD_WITHIN_API();
    }

    /* Free up memmory */
    if (data_ptr_loc != NULL)
    {
        OSA_MemoryFree(data_ptr_loc);
        data_ptr_loc = NULL;
    }

    OSA_SemaphorePost(file_dump.sem_End);

    vTaskDelete(NULL);
}

/*!
 * @param pad pad handle
 * @param active boolean activate/disactivate pad
 *
 * @returns uint8_t
 *
 * @brief This function activates the sink pad and allocates
 * memory for the file sink element.
 *
 */
static uint8_t filesink_sink_pad_activation_handler(StreamPad *pad, uint8_t active)
{
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_FILE_SINK);

    CHK_ARGS(NULL == pad, false);

    STREAMER_LOG_DEBUG(DBG_FILE_SINK, "[File SINK]Activate pad: %d\n", active);

    ElementFileSink *element = (ElementFileSink *)PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    if (true == ret)
    {
        if (pad->scheduling == SCHEDULING_PUSH)
        {
            // Activate/Deactivate the pad peer
            ret = pad_activate_push(pad->peer, active);
        }
        else if (pad->scheduling == SCHEDULING_PULL)
        {
            // A pad with pull scheduling "manually" pulls data from a peer pad.
            // Pull scheduling is implemented by invoking the chain handler (used for push mode),
            // that needs a buffer however, so we need to acquire it.
            if (true == active)
            {
                element->pullbuf = OSA_MemoryAllocate(FILESINK_PULL_SIZE + sizeof(AudioPacketHeader));
                if (element->pullbuf == NULL)
                {
                    return false;
                }
            }
            else
            {
                // Release the pull buffer, if it was allocated
                if (element->pullbuf != NULL)
                {
                    OSA_MemoryFree(element->pullbuf);
                    element->pullbuf = NULL;
                }
            }

            // Activate/Deactivate the pad peer
            ret = pad_activate_pull(pad->peer, active);
        }
    }
    else
    {
        // Release the pull buffer, if it was allocated
        if (element->pullbuf != NULL)
        {
            OSA_MemoryFree(element->pullbuf);
            element->pullbuf = NULL;
        }
    }

    STREAMER_FUNC_EXIT(DBG_FILE_SINK);
    return ret;
}

/*!
 * @param pad pad handle
 * @param buf input buffer
 *
 * @brief This pad function accumulates data from the decoder and
 * pushes it onto the selected audio driver.
 * The audio sink maintains a double buffer and starts sending data once both
 * the buffers are full. It blocks if the previous data is not sent out by the
 * driver for some reason.
 *
 * @returns FlowReturn
 */
static FlowReturn filesink_sink_pad_chain_handler(StreamPad *pad, StreamBuffer *buf)
{
    uint32_t pkt_hdr_size = 0;
    uint32_t data_size    = 0;
    FlowReturn ret        = FLOW_OK;

    STREAMER_FUNC_ENTER(DBG_FILE_SINK);

    CHK_ARGS((NULL == pad) || (NULL == buf), FLOW_UNEXPECTED);

    /* the buffer should be written to a file here */
    /* check what data packet header the buffer has  and typecast accordingly to
     * store the appropriate data
     */
    switch (((PacketHeader *)BUFFER_PTR(buf))->id)
    {
        case RAW_DATA:
            pkt_hdr_size = sizeof(RawPacketHeader);
            break;

        case AUDIO_DATA:
            pkt_hdr_size = sizeof(AudioPacketHeader);
            break;

        default:
            break;
    }

    OSA_MutexLock(&(file_dump.fileDataMutex), osaWaitForever_c);
    /* Get the parent element from the pad */
    file_dump.file_sink_element = (ElementFileSink *)PAD_PARENT(pad);

    /* size is the total buffer size including the packet header
     * Get the raw data size considering the offset as offset into raw data
     * from which data has to be written.
     */

    /* Calculate data size */
    data_size = !file_dump.file_sink_element->raw_write ? buf->size : buf->size - pkt_hdr_size;

    /* Allocate the global data buffer */
    if ((file_dump.data_ptr == NULL) && (data_size > 0))
    {
        /* Allocate 20x more memory than data_size is, due to a possible delay in the FILE_Dump_Task  */
        file_dump.data_ptr_size = data_size * 20;
        if ((file_dump.data_ptr = (int8_t *)OSA_MemoryAllocate(file_dump.data_ptr_size)) == NULL)
        {
            STREAMER_LOG_ERR(DBG_FILE_SINK, ERRCODE_GENERAL_ERROR,
                             "file sink (OSA_MemoryAllocate) error - lack of free memory.\n");
            goto Error;
        }
    }

    /* Wait if the global buffer is full */
    while (file_dump.size + data_size > file_dump.data_ptr_size)
    {
        OSA_MutexUnlock(&(file_dump.fileDataMutex));
        OSA_SemaphoreWait(file_dump.sem_Write, osaWaitForever_c);
        OSA_MutexLock(&(file_dump.fileDataMutex), osaWaitForever_c);
    }

    /* write only data to file
     * Note: writing with header may be useful as a debug feature which is
     * generally not ON.
     */

    /* Copy the data to the global data buffer */
    if (!file_dump.file_sink_element->raw_write)
    {
        if (data_size > 0)
        {
            memcpy(file_dump.data_ptr + file_dump.size, buf->buffer, data_size);
        }
    }
    else
    {
        if (data_size > 0)
        {
            memcpy(file_dump.data_ptr + file_dump.size, buf->buffer + pkt_hdr_size, data_size);
        }
    }

    /* Update data size */
    file_dump.size += data_size;
    OSA_SemaphorePost(file_dump.sem_Read);

    ret = file_dump.ret;
    OSA_MutexUnlock(&(file_dump.fileDataMutex));

    if (ret != FLOW_OK)
        goto Error;

    STREAMER_FUNC_EXIT(DBG_FILE_SINK);
    return ret;

Error:
    if (file_dump.file_sink_element->fd >= 0)
    {
        file_close(file_dump.file_sink_element->fd);
    }
    ret = FLOW_ERROR;
    /* Post EOS message to the pipeline message queue here */
    send_msg_element(PAD_PARENT_ELEMENT(pad), MSG_EOS, 0);
    STREAMER_FUNC_EXIT(DBG_FILE_SINK);
    return ret;
}

/**
 * @brief File sink pad process handler
 *
 * @param pad Pad pointer
 * @return int32_t
 */
static int32_t filesink_sink_pad_process_handler(StreamPad *pad)
{
    FlowReturn flowret;
    ElementFileSink *element = (ElementFileSink *)PAD_PARENT(pad);

    StreamBuffer pullBuf = {.buffer = (int8_t *)element->pullbuf, .size = FILESINK_PULL_SIZE, .offset = 0, .time = 0};

    // Pull data
    flowret = pad_pull_range(pad, 3840, FILESINK_PULL_SIZE, &pullBuf);
    if (flowret != FLOW_OK)
    {
        return (int32_t)flowret;
    }

    // Process
    return filesink_sink_pad_chain_handler(pad, &pullBuf);
}

/*!
 * @param pad pad handle
 * @param event event enumeration
 *
 * @returns uint8_t
 *
 * @brief This function is the calls the specified audio device
 * driver implementation of the sink pad event handler.
 *
 */

static uint8_t filesink_sink_pad_event_handler(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_FILE_SINK);

    CHK_ARGS(NULL == pad, false);

    /* Device driver type */

    STREAMER_LOG_DEBUG(DBG_FILE_SINK, "Event [File Sink]: %d\n", EVENT_TYPE(event) >> 4);

    if (false != ret)
    {
        /* handle the event */
        switch (EVENT_TYPE(event))
        {
            case EVENT_EOS:
                /* Post EOS message to the pipeline message queue here */
                send_msg_element(PAD_PARENT_ELEMENT(pad), MSG_EOS, 0);
                break;

            case EVENT_FLUSH_START:
                /* In case the current state is paused then unblock the pad
                 * signal. Chain function should also check the flushing
                 * flag once it comes out from a wait on the signal to
                 * handle this scenario.
                 */
                break;

            case EVENT_FLUSH_STOP:
                break;

            case EVENT_NEWSEGMENT:
                break;

            default:
                pad_send_event(pad, event);
                break;
        }
    }

    STREAMER_FUNC_EXIT(DBG_FILE_SINK);
    return ret;
}

/*!
 * @param element element
 * @param new_state file sink state to be changed
 *
 * @brief This function changes the file sink state and calls
 * the appropriate apis for that state and takes the actions and then returns.
 * PipelineState Transition:
 *      NULL to READY: Open file
 *      READY to PAUSE: Activate sink pads and their peer pads
 *      PAUSE to PLAY: Start Writing
 *      PAUSE to READY: Deactivate sink pads and their peer pads
 *      READY to NULL: Close file
 *
 * @returns int32_t
 *
 */
static int32_t filesink_change_state(StreamElement *element, PipelineState new_state)
{
    PipelineStateChange state_change;
    ElementFileSink *file_sink_element = (ElementFileSink *)element;
    int32_t ret                        = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_FILE_SINK);

    CHK_ARGS(NULL == file_sink_element, STREAM_ERR_INVALID_ARGS);

    /* get the transition status with the new passed state and the existing
     * state of the sink element
     */
    state_change = (PipelineStateChange)STATE_TRANSITION(file_sink_element->state, new_state);

    file_sink_element->state = new_state;
    switch (state_change)
    {
        case STATE_CHANGE_NULL_TO_READY:
            STREAMER_LOG_DEBUG(DBG_FILE_SINK, "[File SINK]STATE_CHANGE_NULL_TO_READY\n");
            /* Open file for writing */
            file_sink_element->fd = file_open(file_sink_element->location, FILE_WRONLY | FILE_TRUNC);
            if (file_sink_element->fd < 0)
            {
                STREAMER_LOG_ERR(DBG_FILE_SINK, ERRCODE_GENERAL_ERROR, " file open error file sink\n");
                file_close(file_sink_element->fd);
                ret = STREAM_ERR_GENERAL;
                goto Error;
            }

            /* file_dump struct initiallization */
            file_dump.file_sink_element = NULL;
            file_dump.data_ptr          = NULL;
            file_dump.data_ptr_size     = 0;
            file_dump.size              = 0;
            file_dump.ret               = FLOW_OK;
            file_dump.run               = true;
            file_dump.first_run         = true;
            OSA_SemaphoreCreateBinary(file_dump.sem_Read);
            OSA_SemaphoreCreateBinary(file_dump.sem_Write);
            OSA_SemaphoreCreateBinary(file_dump.sem_End);
            OSA_MutexCreate(&(file_dump.fileDataMutex));

            if (xTaskCreate(FILE_Dump_Task, "FILE Dump Task", 512, NULL, configMAX_PRIORITIES - 2, NULL) != pdPASS)
            {
                STREAMER_LOG_DEBUG(DBG_FILE_SINK, "[File SINK]Failed to create application task\r\n");
                ret = STREAM_ERR_GENERAL;
                goto Error;
            }
            break;

        case STATE_CHANGE_PAUSED_TO_PLAYING:
            STREAMER_LOG_DEBUG(DBG_FILE_SINK, "[File SINK]STATE_CHANGE_PAUSED_TO_PLAYING\n");
            /* Unblock the pad */
            break;

        case STATE_CHANGE_READY_TO_NULL:
            /* Create a condition for correct stopping the FILE_Dump_task */
            file_dump.run = false;
            OSA_SemaphorePost(file_dump.sem_Read);

            /* Wait for task ends */
            OSA_SemaphoreWait(file_dump.sem_End, osaWaitForever_c);

            /* Delete semaphores, mutex */
            OSA_SemaphoreDestroy(file_dump.sem_Read);
            OSA_SemaphoreDestroy(file_dump.sem_Write);
            OSA_SemaphoreDestroy(file_dump.sem_End);
            OSA_MutexDestroy(&(file_dump.fileDataMutex));

            /* Free up memmory */
            if (file_dump.data_ptr != NULL)
            {
                OSA_MemoryFree(file_dump.data_ptr);
                file_dump.data_ptr = NULL;
            }

            if (file_sink_element->fd >= 0)
            {
                /* XXX */
                /* Flush the contents to file */
                // fflush(file_sink_element->fd);

                /* Close file after writing */
                file_close(file_sink_element->fd);
            }
            break;

            /* When going to pause state then set the pad to blocked state so
             * that data flow is blocked.
             */
        case STATE_CHANGE_READY_TO_PAUSED:
        case STATE_CHANGE_PLAYING_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_FILE_SINK, "[File SINK]STATE_CHANGE_XXX_TO_PAUSE\n");
            break;

            /* deactivate pads */
        case STATE_CHANGE_PAUSED_TO_READY:
            STREAMER_LOG_DEBUG(DBG_FILE_SINK, "[File SINK]STATE_CHANGE_PAUSED_TO_READY\n");
            break;

        default:
            break;
    }

Error:
    STREAMER_FUNC_EXIT(DBG_FILE_SINK);
    return ret;
}

/*!
 * @param pad pad handle
 * @param query input buffer
 *
 * @brief This pad function handles query from the pipeline. If
 * not able to handle the query then it pushes the query to its peer.
 *
 * @returns uint8_t
 */
static uint8_t filesink_sink_pad_query_handler(StreamPad *pad, StreamQuery *query)
{
    ElementFileSink *element;
    StreamPad *peer;
    uint8_t ret = false;

    STREAMER_FUNC_ENTER(DBG_FILE_SINK);

    CHK_ARGS(NULL == pad || NULL == query || NULL == QUERY_DATA(query), false);

    peer = PAD_PEER(pad);
    CHK_ARGS(NULL == peer, false);

    /* get the parent element from the pad */
    element = (ElementFileSink *)PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    STREAMER_LOG_DEBUG(DBG_FILE_SINK, "filesink_sink_pad_query_handler: query[%d]\n", QUERY_TYPE(query));

    switch (QUERY_TYPE(query))
    {
        default:
            /* Relay all query downstream. File SINK will only handle current elapsed
             * time.
             */
            ret = pad_query(peer, query);
            break;
    }

    STREAMER_FUNC_EXIT(DBG_FILE_SINK);
    return ret;
}

/*!
 * @param element_ptr: file sink element instance
 * @param prop: element property
 * @param val:  value to be set
 *
 * @brief This function sets audio sink element's properties
 *
 * @returns StreamReturnType
 *
 * @retval STREAM_OK  success
 * @retval STREAM_ERR_NO_MEM do not support this property
 *
 */
static int32_t filesink_set_property(StreamElement *element_ptr, uint16_t prop, uint32_t val)
{
    uint32_t ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_FILE_SINK);

    STREAMER_LOG_DEBUG(DBG_FILE_SINK, "[filesink_set_property]: %d\n", prop);
    switch (prop)
    {
        case PROP_FILESINK_LOCATION:
            ret = filesink_set_location((ElementHandle)element_ptr, (char *)val);
            STREAMER_LOG_DEBUG(DBG_FILE_SINK, "set location: %s, ret=%d\n", (int8_t *)val, ret);
            break;

        case PROP_FILESINK_RAW_WRITE_MODE:
            ret = filesink_set_write_mode((ElementHandle)element_ptr, (uint8_t)val);
            break;

        default:
            ret = STREAM_ERR_GENERAL;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_FILE_SINK);
    return ret;
}

/*!
 *
 * @param element_ptr: audio sink element instance
 * @param prop:        element property
 * @param val_ptr:     value to be acquired

 * @brief This function gets audio sink element's properties
 *
 * @returns StreamReturnType
 *
 * @retval STREAM_OK success
 *
 */
static int32_t filesink_get_property(StreamElement *element_ptr, uint16_t prop, uint32_t *val_ptr)
{
    int32_t ret                    = STREAM_OK;
    ElementFileSink *file_sink_ptr = (ElementFileSink *)element_ptr;

    STREAMER_FUNC_ENTER(DBG_FILE_SINK);

    switch (prop)
    {
        case PROP_FILESINK_LOCATION:
        {
            STRING_DESC_T *str_desc_ptr = (STRING_DESC_T *)val_ptr;

            if (str_desc_ptr->str == NULL)
            {
                str_desc_ptr->len = strlen(file_sink_ptr->location) + 1;
            }
            else
            {
                strncpy((char *)str_desc_ptr->str, file_sink_ptr->location, (str_desc_ptr->len - 1));
                str_desc_ptr->str[str_desc_ptr->len - 1] = '\0';
            }
        }
        break;

        case PROP_FILESINK_RAW_WRITE_MODE:
            *((uint8_t *)val_ptr) = file_sink_ptr->raw_write;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_FILE_SINK);
    return ret;
}

int32_t filesink_init_element(StreamElement *element)
{
    uint32_t i                         = 0;
    ElementFileSink *file_sink_element = (ElementFileSink *)element;

    STREAMER_FUNC_ENTER(DBG_FILE_SINK);

    if (NULL == file_sink_element)
    {
        STREAMER_FUNC_EXIT(DBG_FILE_SINK);
        return STREAM_ERR_INVALID_ARGS;
    }

    /* set element change state function */
    file_sink_element->change_state = filesink_change_state;
    file_sink_element->set_property = filesink_set_property;
    file_sink_element->get_property = filesink_get_property;

    /* initialize element fields */
    file_sink_element->chunk_size    = 0;
    file_sink_element->end_of_stream = 0;
    file_sink_element->raw_write     = FILE_RAW_WRITE;
    file_sink_element->fd            = 0;
    file_sink_element->pullbuf       = NULL;
    strncpy(file_sink_element->location, DEFAULT_FILESINK_LOCATION, sizeof(DEFAULT_FILESINK_LOCATION));

    /* initialize only sink pads handlers, no sink pads for file sink */
    for (i = 0; i < NUM_SINKS(TYPE_ELEMENT_FILE_SINK); i++)
    {
        PadSink *pad            = (PadSink *)ELEMENT_SINK_PAD(file_sink_element, i);
        pad->event_handler      = filesink_sink_pad_event_handler;
        pad->activation_handler = filesink_sink_pad_activation_handler;
        pad->chain_handler      = filesink_sink_pad_chain_handler;
        pad->query_handler      = filesink_sink_pad_query_handler;
        pad->process_precheck   = NULL;
        pad->process_handler    = filesink_sink_pad_process_handler;
    }

    STREAMER_FUNC_EXIT(DBG_FILE_SINK);
    return STREAM_OK;
}

int32_t filesink_set_location(ElementHandle element, char *path)
{
    ElementFileSink *file_sink_ptr = (ElementFileSink *)element;
    StreamReturnType ret           = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_FILE_SINK);

    CHK_ARGS(file_sink_ptr == NULL || path == NULL, STREAM_ERR_INVALID_ARGS);

    if (file_sink_ptr->state >= STATE_READY)
    {
        STREAMER_FUNC_EXIT(DBG_FILE_SINK);
        return STREAM_ERR_ELEMENT_BAD_STATUS;
    }
    if (strlen(path) >= MAX_LOCATION_PATH_LENGTH)
    {
        STREAMER_FUNC_EXIT(DBG_FILE_SINK);
        return STREAM_ERR_NO_MEM;
    }
    strncpy(file_sink_ptr->location, path, (MAX_LOCATION_PATH_LENGTH - 1));

    STREAMER_FUNC_EXIT(DBG_FILE_SINK);
    return ret;
}

int32_t filesink_set_write_mode(ElementHandle element, uint8_t write_mode)
{
    ElementFileSink *sink = (ElementFileSink *)element;
    StreamReturnType ret  = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_FILE_SINK);

    CHK_ARGS(sink == NULL, STREAM_ERR_INVALID_ARGS);

    sink->raw_write = write_mode;

    STREAMER_FUNC_EXIT(DBG_FILE_SINK);
    return ret;
}

/* End of File */
