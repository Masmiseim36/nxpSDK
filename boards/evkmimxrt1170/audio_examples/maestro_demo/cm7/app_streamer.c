/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "osa_common.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

#include "app_streamer.h"
#include "streamer_pcm.h"
#ifdef VIT_PROC
#include "vit_proc.h"
#endif

#ifdef EAP_PROC
#include "eap_proc.h"
static ext_proc_args eap_args;
#endif

#define APP_STREAMER_MSG_QUEUE     "app_queue"
#define STREAMER_TASK_NAME         "Streamer"
#define STREAMER_MESSAGE_TASK_NAME "StreamerMessage"

#define STREAMER_TASK_STACK_SIZE         4 * 1024
#define STREAMER_MESSAGE_TASK_STACK_SIZE 512

ringbuf_t *audioBuffer;
OsaMutex audioMutex;
OsaThread msg_thread;

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

    PRINTF("[STREAMER] Message Task started\r\n");

    ret = osa_mq_open(&mq, APP_STREAMER_MSG_QUEUE, STREAMER_MSG_SIZE, true);
    if (ERRCODE_NO_ERROR != ret)
    {
        PRINTF("osa_mq_open failed: %d\r\n", ret);
        return;
    }

    do
    {
        ret = osa_mq_receive(&mq, (void *)&msg, STREAMER_MSG_SIZE, 0, NULL);
        if (ret != ERRCODE_NO_ERROR)
        {
            PRINTF("osa_mq_receiver error: %d\r\n", ret);
            continue;
        }

        switch (msg.id)
        {
            case STREAM_MSG_ERROR:
                PRINTF("STREAM_MSG_ERROR\r\n");
                exit_thread = true;
                STREAMER_Stop(handle);
                break;
            case STREAM_MSG_EOS:
                PRINTF("\nSTREAM_MSG_EOS\r\n");

                /* Indicate to other software layers that playing has ended. */
                STREAMER_Stop(handle);
                break;
            case STREAM_MSG_UPDATE_POSITION:
                PRINTF("STREAM_MSG_UPDATE_POSITION..");
                PRINTF(" position: %d ms\r", msg.event_data);
                break;
            case STREAM_MSG_CLOSE_TASK:
                PRINTF("STREAM_MSG_CLOSE_TASK\r\n");
                exit_thread = true;
                break;
            default:
                break;
        }

    } while (!exit_thread);

    osa_mq_close(&mq);
    osa_mq_destroy(APP_STREAMER_MSG_QUEUE);

    osa_thread_destroy(msg_thread);
}

int STREAMER_Read(uint8_t *data, uint32_t size)
{
    uint32_t bytes_read;

    osa_mutex_lock(&audioMutex);
    bytes_read = ringbuf_read(audioBuffer, data, size);
    osa_mutex_unlock(&audioMutex);

    if (bytes_read != size)
    {
        PRINTF("[STREAMER WARN] read underrun: size: %d, read: %d\r\n", size, bytes_read);
    }

    return bytes_read;
}

int STREAMER_Write(uint8_t *data, uint32_t size)
{
    uint32_t written;

    osa_mutex_lock(&audioMutex);
    written = ringbuf_write(audioBuffer, data, size);
    osa_mutex_unlock(&audioMutex);

    if (written != size)
    {
        PRINTF("[STREAMER ERR] write overflow: size %d, written %d\r\n", size, written);
    }

    return written;
}

bool STREAMER_IsPlaying(streamer_handle_t *handle)
{
    return handle->audioPlaying;
}

void STREAMER_Start(streamer_handle_t *handle)
{
    PRINTF("[STREAMER] start playback\r\n");

    handle->audioPlaying = true;
    streamer_set_state(handle->streamer, 0, STATE_PLAYING, true);
}

void STREAMER_Stop(streamer_handle_t *handle)
{
    PRINTF("[STREAMER] stop playback\r\n");

    handle->audioPlaying = false;
    streamer_set_state(handle->streamer, 0, STATE_NULL, true);

    /* Empty input ringbuffer. */
    if (audioBuffer)
    {
        ringbuf_clear(audioBuffer);
    }
}

status_t STREAMER_Create(streamer_handle_t *handle)
{
    STREAMER_CREATE_PARAM params;
    ELEMENT_PROPERTY_T prop;
    OsaThreadAttr thread_attr;
    int ret;

    osa_mutex_create(&audioMutex, false);
    audioBuffer = ringbuf_create(AUDIO_BUFFER_SIZE);
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
    params.stack_size    = STREAMER_TASK_STACK_SIZE;
    params.pipeline_type = STREAM_PIPELINE_NETBUF;
    params.task_name     = STREAMER_TASK_NAME;
    params.in_dev_name   = "";
    params.out_dev_name  = "";

    handle->streamer = streamer_create(&params);
    if (!handle->streamer)
    {
        return kStatus_Fail;
    }

    prop.prop = PROP_NETBUFSRC_SET_CALLBACK;
    prop.val  = (uintptr_t)STREAMER_Read;

    streamer_set_property(handle->streamer, prop, true);

    prop.prop = PROP_DECODER_DECODER_TYPE;
    prop.val  = DECODER_TYPE_MP3;

    streamer_set_property(handle->streamer, prop, true);

    handle->audioPlaying = false;

    return kStatus_Success;
}

status_t STREAMER_mic_Create(streamer_handle_t *handle, out_sink_t out_sink)
{
    STREAMER_CREATE_PARAM params;
    OsaThreadAttr thread_attr;
    ELEMENT_PROPERTY_T prop;
    int ret;

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

    switch (out_sink)
    {
        case AUDIO_SINK:
            params.pipeline_type = STREAM_PIPELINE_PCM;
            params.out_dev_name  = "";
            break;

        case FILE_SINK:
            params.pipeline_type = STREAM_PIPELINE_MIC2FILE;
            params.out_dev_name  = "file";
            break;

        case VIT_SINK:
            params.pipeline_type = STREAM_PIPELINE_VIT;
            params.out_dev_name  = "";
            break;

        default:
            PRINTF("[STREAMER ERR] wrong type of sink\r\n");
            return kStatus_InvalidArgument;
    }

    params.task_name   = STREAMER_TASK_NAME;
    params.in_dev_name = "microphone";

    handle->streamer = streamer_create(&params);
    if (!handle->streamer)
    {
        return kStatus_Fail;
    }

#ifdef VIT_PROC
    if (params.pipeline_type == STREAM_PIPELINE_VIT)
    {
        EXT_PROCESS_DESC_T vit_proc = {VIT_Initialize_func, VIT_Execute_func, VIT_Deinit_func, &Vit_Language, 0};

        prop.prop = PROP_VITSINK_FPOINT;
        prop.val  = (uintptr_t)&vit_proc;

        streamer_set_property(handle->streamer, prop, true);
    }
#else
    if (params.pipeline_type == STREAM_PIPELINE_VIT)
    {
        PRINTF("[STREAMER] VIT pipeline not available for this config\r\n switching to audio sink");
        params.pipeline_type = STREAM_PIPELINE_PCM;
    }
#endif

    prop.prop = PROP_AUDIOSRC_SET_SAMPLE_RATE;
    prop.val  = 16000;

    streamer_set_property(handle->streamer, prop, true);

    prop.prop = PROP_AUDIOSRC_SET_CHUNK_SIZE;
    prop.val  = 160;

    streamer_set_property(handle->streamer, prop, true);

    return kStatus_Success;
}

status_t STREAMER_file_Create(streamer_handle_t *handle, char *filename, int eap_par)
{
    STREAMER_CREATE_PARAM params;
    OsaThreadAttr thread_attr;
    int ret;

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
#ifdef EAP_PROC
    params.pipeline_type = STREAM_PIPELINE_EAP;
#else
    params.pipeline_type = STREAM_PIPELINE_FILESYSTEM;
#endif
    params.task_name    = STREAMER_TASK_NAME;
    params.in_dev_name  = "";
    params.out_dev_name = "";

    handle->streamer = streamer_create(&params);
    if (!handle->streamer)
    {
        return kStatus_Fail;
    }

    streamer_set_file(handle->streamer, 0, filename, STATE_NULL, true);

#ifdef EAP_PROC
    ELEMENT_PROPERTY_T prop;
    eap_args.preset_num         = eap_par;
    EXT_PROCESS_DESC_T eap_proc = {EAP_Initialize_func, EAP_Execute_func, EAP_Deinit_func, &eap_args, 0};

    prop.prop = PROP_EAP_FPOINT;
    prop.val  = (uintptr_t)&eap_proc;

    streamer_set_property(handle->streamer, prop, true);

#endif

    handle->audioPlaying = false;

    return kStatus_Success;
}

void STREAMER_Destroy(streamer_handle_t *handle)
{
    streamer_destroy(handle->streamer);

    if (audioMutex != NULL)
    {
        osa_mutex_destroy(&audioMutex);
        audioMutex = NULL;
    }
    if (audioBuffer != NULL)
    {
        ringbuf_destroy(audioBuffer);
        audioBuffer = NULL;
    }
    deinit_logging();
    osa_deinit();
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
    // set_debug_level(LOGLVL_DEBUG);
    // get_debug_state();

    /* Initialize streamer PCM management library. */
    streamer_pcm_init();
}
