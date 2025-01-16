/*
 * Copyright 2020, 2024 NXP
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
    if (handle->streamer->message_channel_out.is_mq_created == false)
    {
        PRINTF("[EAP STREAMER] osa_mq_open failed: %d\r\n");
        OSA_TaskDestroy(msg_thread);
        return;
    }

    do
    {
        ret = OSA_MsgQGet(handle->streamer->message_channel_out.mq, (void *)&msg, osaWaitForever_c);
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
    
    if (handle->streamer->message_channel_out.is_mq_created == true)
    {
        if (OSA_MsgQDestroy(handle->streamer->message_channel_out.mq) != KOSA_StatusSuccess)
        {
            PRINTF("Could not destroy out message queue\r\n");
            OSA_TaskDestroy(msg_thread);
            return ;
        }
        handle->streamer->message_channel_out.is_mq_created = false;
    }
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
    ElementIndex element_ids[] = {ELEMENT_FILE_SRC_INDEX,
                                  ELEMENT_DECODER_INDEX,
#ifdef SSRC_PROC
                                  ELEMENT_SRC_INDEX,
#endif
                                  ELEMENT_SPEAKER_INDEX};
    int num_elements = sizeof(element_ids)/sizeof(ElementIndex);
    PipelineElements pipelineElements = {element_ids, num_elements};
	
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
    params.task_name     = STREAMER_TASK_NAME;
    params.in_dev_name   = "";
    params.out_dev_name  = "";
    params.elements = pipelineElements;

    handle->streamer = streamer_create(&params);
    if (!handle->streamer)
    {
        return kStatus_Fail;
    }

    streamer_set_file(handle->streamer, 0, filename, STATE_NULL, true);
	
    EXT_AUDIOELEMENT_DESC_T appFunctions = {
        .open_func      = streamer_pcm_open,
        .close_func     = NULL,
        .start_func     = NULL,
        .process_func   = streamer_pcm_write,
        .set_param_func = streamer_pcm_setparams,
        .get_param_func = streamer_pcm_getparams,
        .mute_func      = NULL,
        .volume_func    = NULL,
    };
    prop.prop = PROP_SPEAKER_SET_APP_FUNCTIONS;
    prop.val  = (uintptr_t)&appFunctions;
    streamer_set_property(handle->streamer, 0, prop, true);

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
