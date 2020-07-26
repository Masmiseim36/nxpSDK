/*
 * Copyright 2018 NXP. 
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "osa_common.h"
#include "fsl_common.h"

#include "ais_streamer.h"
#include "streamer_pcm.h"
#include "af_error.h"

#define APP_STREAMER_MSG_QUEUE "app_queue"
#define STREAMER_TASK_NAME "Streamer"
#define STREAMER_MESSAGE_TASK_NAME "StreamerMessage"

#define STREAMER_TASK_STACK_SIZE 1024
#define STREAMER_MESSAGE_TASK_STACK_SIZE 256

ringbuf_t *audioBuffer;
/* internal mutex for accessing the audio buffer */
static OsaMutex audioBufMutex;

/*!
 * @brief Streamer task for communicating messages
 *
 * This function is the entry point of a task that is manually created by
 * STREAMER_Create.  It listens on a message queue and receives status updates
 * about errors, audio playback state and position.  The application can make
 * use of this data.
 *
 * @param arg Data to be passed to the task
 */
static void STREAMER_MessageTask(void *arg)
{
    OsaMq mq;
    STREAMER_MSG_T msg;
    streamer_handle_t *handle;
    bool exit_thread = false;
    int ret;

    handle = (streamer_handle_t *)arg;

    configPRINTF(("[STREAMER] Message Task started\r\n"));

    ret = osa_mq_open(&mq, APP_STREAMER_MSG_QUEUE, STREAMER_MSG_SIZE, true);
    if (ERRCODE_NO_ERROR != ret)
    {
        configPRINTF(("osa_mq_open failed: %d\r\n", ret));
        return;
    }

    do
    {
        ret = osa_mq_receive(&mq, (void *)&msg, STREAMER_MSG_SIZE, 0, NULL);
        if (ret != ERRCODE_NO_ERROR)
        {
            configPRINTF(("osa_mq_receiver error: %d\r\n", ret));
            continue;
        }

        switch (msg.id)
        {
            case STREAM_MSG_ERROR:
                configPRINTF(("STREAM_MSG_ERROR %d\r\n", msg.errorcode));

                if (handle->pvExceptionCallback != NULL)
                {
                	handle->pvExceptionCallback();
                }

                break;
            case STREAM_MSG_EOS:
                configPRINTF(("STREAM_MSG_EOS\r\n"));
                /* Indicate to other software layers that playing has ended. */
                handle->eos = true;
                break;
            case STREAM_MSG_UPDATE_POSITION:
                configPRINTF(("STREAM_MSG_UPDATE_POSITION\r\n"));
                configPRINTF(("  position: %d ms\r\n", msg.event_data));
                break;
            case STREAM_MSG_CLOSE_TASK:
                configPRINTF(("STREAM_MSG_CLOSE_TASK\r\n"));
                exit_thread = true;
                break;
            default:
                break;
        }

    } while (!exit_thread);

    osa_mq_close(&mq);
    osa_mq_destroy(APP_STREAMER_MSG_QUEUE);
}

int STREAMER_Read(uint8_t *data, uint32_t size)
{
    uint32_t bytes_read;

    xSemaphoreTake(audioBufMutex, portMAX_DELAY);
    bytes_read = ringbuf_read(audioBuffer, data, size);
    xSemaphoreGive(audioBufMutex);

    if (bytes_read != size)
    {
        /* Don't print this warning under normal conditions.
         * * Excessive calls can clog the logging and hide other messages. */
        /*
        configPRINTF(("[STREAMER WARN] read underrun: size: %d, read: %d\r\n", size, bytes_read));
        */
    }

    return bytes_read;
}

int STREAMER_Write(uint8_t *data, uint32_t size)
{
    uint32_t written;

    xSemaphoreTake(audioBufMutex, portMAX_DELAY);
    written = ringbuf_write(audioBuffer, data, size);
    xSemaphoreGive(audioBufMutex);

    if (written != size)
    {
        configPRINTF(("[STREAMER ERR] write overflow: size %d, written %d\r\n", size, written));
    }

    return written;
}

uint32_t STREAMER_GetQueued(streamer_handle_t *handle)
{
    uint32_t bufSize;

    xSemaphoreTake(audioBufMutex, portMAX_DELAY);
    bufSize = ringbuf_get_occupancy(audioBuffer);
    xSemaphoreGive(audioBufMutex);

    return bufSize;
}

uint32_t STREAMER_GetQueuedNotBlocking(streamer_handle_t *handle)
{
    return ringbuf_get_occupancy(audioBuffer);
}

bool STREAMER_IsPlaying(streamer_handle_t *handle)
{
    return handle->audioPlaying;
}

void STREAMER_SetVolume(uint32_t volume)
{
    streamer_pcm_set_volume(volume);
}

void STREAMER_Start(streamer_handle_t *handle)
{
    configPRINTF(("[STREAMER] start playback\r\n"));

    handle->audioPlaying = true;
    streamer_set_state(handle->streamer, 0, STATE_PLAYING, true);
}

uint32_t STREAMER_Stop(streamer_handle_t *handle)
{
    uint32_t flushedSize = 0;

    configPRINTF(("[STREAMER] stop playback\r\n"));

    handle->audioPlaying = false;
    streamer_set_state(handle->streamer, 0, STATE_NULL, true);

    flushedSize = STREAMER_GetQueued(handle);

    /* Flush input ringbuffer. */
    xSemaphoreTake(audioBufMutex, portMAX_DELAY);
    ringbuf_clear(audioBuffer);
    xSemaphoreGive(audioBufMutex);

    return flushedSize;
}

void STREAMER_Pause(streamer_handle_t *handle)
{
    configPRINTF(("[STREAMER] pause playback\r\n"));

    handle->audioPlaying = false;
    streamer_set_state(handle->streamer, 0, STATE_PAUSED, true);

    /* Don't clear out input ringbuffer so play can be resumed. */
}

status_t STREAMER_Create(streamer_handle_t *handle, streamer_decoder_t decoder)
{
    STREAMER_CREATE_PARAM params;
    ELEMENT_PROPERTY_T prop;
    OsaThread msg_thread;
    OsaThreadAttr thread_attr;
    int ret;

    audioBufMutex = xSemaphoreCreateMutex();
    if (!audioBufMutex)
    {
        return kStatus_Fail;
    }

    audioBuffer = ringbuf_create(AWS_AUDIO_BUFFER_SIZE);
    if (!audioBuffer)
    {
        return kStatus_Fail;
    }

    /* Create message process thread */
    osa_thread_attr_init(&thread_attr);
    osa_thread_attr_set_name(&thread_attr, STREAMER_MESSAGE_TASK_NAME);
    osa_thread_attr_set_stack_size(&thread_attr, STREAMER_MESSAGE_TASK_STACK_SIZE);
    ret = osa_thread_create(&msg_thread, &thread_attr, STREAMER_MessageTask, (void *)handle);
    osa_thread_attr_destroy(&thread_attr);
    if (ERRCODE_NO_ERROR != ret)
    {
        return kStatus_Fail;
    }

    /* Create streamer */
    strcpy(params.out_mq_name, APP_STREAMER_MSG_QUEUE);
    params.stack_size = STREAMER_TASK_STACK_SIZE;
    params.pipeline_type = STREAM_PIPELINE_NETBUF;
    params.task_name = STREAMER_TASK_NAME;
    params.in_dev_name = "";
    params.out_dev_name = "";

    handle->streamer = streamer_create(&params);
    if (!handle->streamer)
    {
        return kStatus_Fail;
    }

    prop.prop = PROP_NETBUFSRC_SET_CALLBACK;
    prop.val = (uintptr_t)STREAMER_Read;

    streamer_set_property(handle->streamer, prop, true);

    prop.prop = PROP_DECODER_DECODER_TYPE;
    if (decoder == AIS_DECODER_OPUS) {
        prop.val = DECODER_TYPE_OPUS;
    } else if (decoder == AIS_DECODER_MP3) {
        prop.val = DECODER_TYPE_MP3;
    } else {
        return kStatus_Fail;
    }

    streamer_set_property(handle->streamer, prop, true);

    handle->audioPlaying = false;
    handle->eos = false;

    return kStatus_Success;
}

void STREAMER_Destroy(streamer_handle_t *handle)
{
    streamer_destroy(handle->streamer);

    vSemaphoreDelete(audioBufMutex);
    ringbuf_destroy(audioBuffer);
}

void STREAMER_Init(void)
{
    /* Initialize OSA*/
    osa_init();

    /* Initialize logging */
    init_logging();

    add_module_name(LOGMDL_STREAMER, "STREAMER");

    /* Uncomment below to turn on full debug logging for the streamer. */
    // set_debug_module(0xffffffff);
    // set_debug_level(0xff);

    /* Initialize streamer PCM management library. */
    streamer_pcm_init();
}
