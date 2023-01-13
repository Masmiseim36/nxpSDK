/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_common.h"
#include "fsl_debug_console.h"

#include "app_streamer.h"
#include "streamer_pcm.h"
#include "app_music_control.h"
#include "maestro_logging.h"

#define APP_STREAMER_MSG_QUEUE     "app_queue"
#define STREAMER_TASK_NAME         "Streamer"
#define STREAMER_MESSAGE_TASK_NAME "StreamerMessage"

#define STREAMER_TASK_STACK_SIZE         8 * 1024
#define STREAMER_MESSAGE_TASK_STACK_SIZE 2 * 1024

OSA_TASK_HANDLE_DEFINE(msg_thread);

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
    STREAMER_MSG_T msg;
    streamer_handle_t *handle;
    bool exit_thread = false;
    osa_status_t ret;

    handle = (streamer_handle_t *)arg;

    PRINTF("[STREAMER] Message Task started\r\n");

    while (!handle->streamer)
    {
        OSA_TimeDelay(1);
    }
    if (handle->streamer->mq_out == NULL)
    {
        PRINTF("[EAP STREAMER] osa_mq_open failed: %d\r\n");
        OSA_TaskDestroy(msg_thread);
        return;
    }

    do
    {
        ret = OSA_MsgQGet(&handle->streamer->mq_out, (void *)&msg, osaWaitForever_c);
        if (ret != KOSA_StatusSuccess)
        {
            PRINTF("[EAP STREAMER] OSA_MsgQGet error: %d\r\n", ret);
            continue;
        }

        switch (msg.id)
        {
            case STREAM_MSG_DESTROY_PIPELINE:
                break;
            case STREAM_MSG_ERROR:
                PRINTF("STREAM_MSG_ERROR\r\n");
                exit_thread = true;
                STREAMER_Stop(handle);
                break;
            case STREAM_MSG_EOS:
                PRINTF("STREAM_MSG_EOS\r\n");
                exit_thread = true;
                music_play_next();
                break;
            case STREAM_MSG_UPDATE_POSITION:
                PRINTF("STREAM_MSG_UPDATE_POSITION\r\n");
                PRINTF("  position: %d ms\r\n", msg.event_data);
                break;
            case STREAM_MSG_UPDATE_DURATION:
                break;
            case STREAM_MSG_CLOSE_TASK:
                PRINTF("STREAM_MSG_CLOSE_TASK\r\n");
                exit_thread = true;
                break;
            default:
                break;
        }

    } while (!exit_thread);

    OSA_MsgQDestroy(&handle->streamer->mq_out);
    handle->streamer->mq_out = NULL;

    OSA_TaskDestroy(msg_thread);
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
    ELEMENT_PROPERTY_T prop;
    osa_task_def_t thread_attr;
    int ret;

    /* Create message process thread */
    thread_attr.tpriority = OSA_PRIORITY_HIGH;
    thread_attr.tname     = (uint8_t *)STREAMER_MESSAGE_TASK_NAME;
    thread_attr.pthread   = &STREAMER_MessageTask;
    thread_attr.stacksize = STREAMER_MESSAGE_TASK_STACK_SIZE;
    ret                   = OSA_TaskCreate(&msg_thread, &thread_attr, (void *)handle);
    if (KOSA_StatusSuccess != ret)
    {
        return kStatus_Fail;
    }

    /* Create streamer */
    strcpy(params.out_mq_name, APP_STREAMER_MSG_QUEUE);
    params.stack_size    = STREAMER_TASK_STACK_SIZE;
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

	(void)prop;  /* Unused variable */

    return kStatus_Success;
}

void STREAMER_Destroy(streamer_handle_t *handle)
{
    streamer_destroy(handle->streamer);
    handle->streamer = NULL;
}

void STREAMER_Init(void)
{
    /* Initialize logging */
    init_logging();
    /* Initialize streamer PCM management library. */
    streamer_pcm_init();
}
