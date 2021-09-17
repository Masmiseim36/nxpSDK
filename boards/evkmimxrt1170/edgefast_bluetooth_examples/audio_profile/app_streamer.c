/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "osa_common.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

#include "app_streamer.h"
#include "streamer_pcm.h"
#include "app_music_control.h"

#define APP_STREAMER_MSG_QUEUE     "app_queue"
#define STREAMER_TASK_NAME         "Streamer"
#define STREAMER_MESSAGE_TASK_NAME "StreamerMessage"

#define STREAMER_TASK_STACK_SIZE         8 * 1024
#define STREAMER_MESSAGE_TASK_STACK_SIZE 2 * 1024

OsaThread msg_thread;

#define ENABLE_STREAMER_MESSAGE_TASK (1U)

#if (defined ENABLE_STREAMER_MESSAGE_TASK) && (ENABLE_STREAMER_MESSAGE_TASK)
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
        osa_thread_destroy(NULL);
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
                PRINTF("STREAM_MSG_EOS\r\n");

                /* Indicate to other software layers that playing has ended. */
                /* STREAMER_Stop(handle); */
                music_play_next();
                break;
            case STREAM_MSG_UPDATE_POSITION:
                PRINTF("STREAM_MSG_UPDATE_POSITION\r\n");
                PRINTF("  position: %d ms\r\n", msg.event_data);
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

    osa_thread_destroy(NULL);
}
#endif

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
}

void STREAMER_Pause(streamer_handle_t *handle)
{
    streamer_set_state(handle->streamer, 0, STATE_PAUSED, true);
}

void STREAMER_Resume(streamer_handle_t *handle)
{
    streamer_set_state(handle->streamer, 0, STATE_PLAYING, true);
}

status_t STREAMER_file_Create(streamer_handle_t *handle, char *filename)
{
    STREAMER_CREATE_PARAM params;
#if (defined ENABLE_STREAMER_MESSAGE_TASK) && (ENABLE_STREAMER_MESSAGE_TASK)
    OsaThreadAttr thread_attr;
    int ret;
#endif

#if (defined ENABLE_STREAMER_MESSAGE_TASK) && (ENABLE_STREAMER_MESSAGE_TASK)
    /* Create message process thread */
    osa_thread_attr_init(&thread_attr);
    osa_thread_attr_set_name(&thread_attr, STREAMER_MESSAGE_TASK_NAME);
    osa_thread_attr_set_stack_size(&thread_attr, STREAMER_MESSAGE_TASK_STACK_SIZE / sizeof(portSTACK_TYPE));
    ret = osa_thread_create(&msg_thread, &thread_attr, STREAMER_MessageTask, (void *)handle);
    osa_thread_attr_destroy(&thread_attr);
    if (ERRCODE_NO_ERROR != ret)
    {
        return kStatus_Fail;
    }
#endif

    /* Create streamer */
#if (defined ENABLE_STREAMER_MESSAGE_TASK) && (ENABLE_STREAMER_MESSAGE_TASK)
    strcpy(params.out_mq_name, APP_STREAMER_MSG_QUEUE);
#else
    memset(params.out_mq_name, 0, sizeof(params.out_mq_name));
#endif
    params.stack_size    = STREAMER_TASK_STACK_SIZE / sizeof(portSTACK_TYPE);
    params.pipeline_type = STREAM_PIPELINE_FILESYSTEM;
    params.task_name     = STREAMER_TASK_NAME;
    params.in_dev_name   = "";
    params.out_dev_name  = "";

    handle->streamer = streamer_create(&params);
    if (!handle->streamer)
    {
        return kStatus_Fail;
    }

    streamer_set_file(handle->streamer, 0, filename, STATE_NULL, true);

    handle->audioPlaying = false;

    return kStatus_Success;
}

void STREAMER_Destroy(streamer_handle_t *handle)
{
    streamer_destroy(handle->streamer);
}

void STREAMER_Init(void)
{
    /* Initialize OSA*/
    osa_init();

    /* Initialize logging */
    init_logging();

    add_module_name(LOGMDL_STREAMER, "STREAMER");

    /* Uncomment below to turn on full debug logging for the streamer. */
    /* set_debug_module(0xffffffff); */
    /* set_debug_level(LOGLVL_DEBUG); */
    /* get_debug_state(); */

    /* Initialize streamer PCM management library. */
    streamer_pcm_init();
}
