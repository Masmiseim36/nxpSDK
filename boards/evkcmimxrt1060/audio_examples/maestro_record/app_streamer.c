/*
 * Copyright 2020-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"

#include "app_streamer.h"
#include "streamer_pcm.h"
#include "maestro_logging.h"
#ifdef VIT_PROC
#include "vit_proc.h"
#endif
#ifdef VOICE_SEEKER_PROC
#include "voice_seeker.h"
#endif
#include "app_definitions.h"
#if (((defined(VIT_PROC) && !defined(VOICE_SEEKER_PROC))) && DEMO_CODEC_CS42448 == 1)
#error "Please define VOICE_SEEKER_PROC in order to use VIT with DEMO_CODEC_CS42448"
#endif

#define APP_STREAMER_MSG_QUEUE     "app_queue"
#define STREAMER_TASK_NAME         "Streamer"
#define STREAMER_MESSAGE_TASK_NAME "StreamerMessage"

#ifdef OPUS_ENCODE
/* The STREAMER_OPUS_TASK_STACK_SIZE value is different for the IAR because the IAR allocates memory for VLA (variable
 * length array - used in the opus encoder) in the application heap and the opus task stack would be largely unused.
 */
#if defined(__ICCARM__)
#define STREAMER_OPUS_TASK_STACK_SIZE 5 * 1024
#else
#define STREAMER_OPUS_TASK_STACK_SIZE 30 * 1024
#endif
#endif

#define STREAMER_TASK_STACK_SIZE         16 * 1024
#define STREAMER_MESSAGE_TASK_STACK_SIZE 2 * 1024
#define MAX_FILE_NAME_LENGTH             100

OSA_MUTEX_HANDLE_DEFINE(audioMutex);
OSA_TASK_HANDLE_DEFINE(msg_thread);

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

    while (handle->streamer->message_channel_out.is_mq_created == false)
    {
        OSA_TimeDelay(1);
    }

    PRINTF("[STREAMER] Message Task started\r\n");

    do
    {
        ret = OSA_MsgQGet(handle->streamer->message_channel_out.mq, (void *)&msg, osaWaitForever_c);
        if (ret != KOSA_StatusSuccess)
        {
            PRINTF("OSA_MsgQGet error: %d\r\n", ret);
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
                exit_thread = true;
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

    OSA_MsgQDestroy(handle->streamer->message_channel_out.mq);
    handle->streamer->message_channel_out.is_mq_created = false;

    OSA_TaskDestroy(msg_thread);
}

bool STREAMER_IsPlaying(streamer_handle_t *handle)
{
    return handle->audioPlaying;
}

void STREAMER_Start(streamer_handle_t *handle)
{
    PRINTF("[STREAMER] start \r\n");

    handle->audioPlaying = true;
    streamer_set_state(handle->streamer, 0, STATE_PLAYING, true);
}

void STREAMER_Stop(streamer_handle_t *handle)
{
    PRINTF("[STREAMER] stop \r\n");

    handle->audioPlaying = false;
    streamer_set_state(handle->streamer, 0, STATE_NULL, true);
}

status_t STREAMER_mic_Create(streamer_handle_t *handle, ElementIndex out_sink, char *file_name)
{
    STREAMER_CREATE_PARAM params;
    osa_task_def_t thread_attr;
    ELEMENT_PROPERTY_T prop;
    int ret, num_elements = 3;
    ElementIndex element_ids[3];

    /* There are several possibilities how the pipeline will be constructed:
     * 1) out_sink is VIT
     * 	  a) VoiceSeeker is used => Microphone -> VoiceSeeker -> VIT
     * 	  b) No VoiceSeeker => Microphone -> VIT
     * 2) out sink is a speaker or a file
     *    a) VIT can be used just with 1 channel input
     *    b) VIT processing is not part of the pipeline
     */
    element_ids[0] = ELEMENT_MICROPHONE_INDEX;
#ifdef VIT_PROC
    if (out_sink == ELEMENT_VIT_INDEX)
    {
#ifdef VOICE_SEEKER_PROC
        element_ids[1] = ELEMENT_VOICESEEKER_INDEX;
        element_ids[2] = out_sink;
#else
        element_ids[1] = out_sink;
        num_elements   = 2;
#endif // VOICE_SEEKER_PROC
    }
    else
#endif // VIT_PROC
    {
        element_ids[1] = out_sink;
        num_elements   = 2;
    }
    PipelineElements pipelineElements = {element_ids, num_elements};

    /* Create streamer */
    strcpy(params.out_mq_name, APP_STREAMER_MSG_QUEUE);
    params.stack_size  = STREAMER_TASK_STACK_SIZE;
    params.task_name   = STREAMER_TASK_NAME;
    params.in_dev_name = "microphone";
    params.elements    = pipelineElements;

    handle->streamer = streamer_create(&params);
    if (!handle->streamer)
    {
        return kStatus_Fail;
    }

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

#ifdef VOICE_SEEKER_PROC
    if (is_element_in_pipeline(pipelineElements, ELEMENT_VOICESEEKER_INDEX))
    {
        EXT_PROCESS_DESC_T voice_seeker_proc = {VoiceSeeker_Initialize_func, VoiceSeeker_Execute_func,
                                                VoiceSeeker_Deinit_func, NULL};
        prop.prop                            = PROP_VOICESEEKER_FUNCPTR;
        prop.val                             = (uintptr_t)&voice_seeker_proc;
        streamer_set_property(handle->streamer, 0, prop, true);
    }
#endif // VOICE_SEEKER_PROC
#ifdef VIT_PROC
    if (is_element_in_pipeline(pipelineElements, ELEMENT_VIT_INDEX) ||
        is_element_in_pipeline(pipelineElements, ELEMENT_VIT_PROC_INDEX))
    {
        EXT_PROCESS_DESC_T vit_proc = {VIT_Initialize_func, VIT_Execute_func, VIT_Deinit_func, NULL};

        if (is_element_in_pipeline(pipelineElements, ELEMENT_VIT_INDEX))
            prop.prop = PROP_VITSINK_FUNCPTR;
        else if (is_element_in_pipeline(pipelineElements, ELEMENT_VIT_PROC_INDEX))
            prop.prop = PROP_VIT_PROC_FUNCPTR;
        prop.val = (uintptr_t)&vit_proc;
        streamer_set_property(handle->streamer, 0, prop, true);
    }
#endif // VIT_PROC

#if ((DEMO_CHANNEL_NUM > 2) && !defined(VOICE_SEEKER_PROC))
    if (out_sink == ELEMENT_VIT_INDEX)
    {
        PRINTF(
            "[STREAMER] Please enable VoiceSeeker, it must be used if more than one channel is used and VIT is "
            "enabled.\r\n");
        return kStatus_Fail;
    }
#endif

    prop.prop = PROP_MICROPHONE_SET_NUM_CHANNELS;
    prop.val  = DEMO_MIC_CHANNEL_NUM;
    streamer_set_property(handle->streamer, 0, prop, true);

    prop.prop = PROP_MICROPHONE_SET_BITS_PER_SAMPLE;
    prop.val  = DEMO_AUDIO_BIT_WIDTH;
    streamer_set_property(handle->streamer, 0, prop, true);

    prop.prop = PROP_MICROPHONE_SET_FRAME_MS;
    prop.val  = DEMO_MIC_FRAME_SIZE;
    streamer_set_property(handle->streamer, 0, prop, true);

    prop.prop = PROP_MICROPHONE_SET_SAMPLE_RATE;
    prop.val  = DEMO_AUDIO_SAMPLE_RATE;
    streamer_set_property(handle->streamer, 0, prop, true);

    EXT_AUDIOELEMENT_DESC_T appFunctions = {
        .open_func      = streamer_pcm_rx_open,
        .close_func     = streamer_pcm_rx_close,
        .start_func     = NULL,
        .process_func   = streamer_pcm_read,
        .set_param_func = streamer_pcm_setparams,
        .get_param_func = streamer_pcm_getparams,
        .mute_func      = streamer_pcm_mute,
        .volume_func    = streamer_pcm_set_volume,
    };
    prop.prop = PROP_MICROPHONE_SET_APP_FUNCTIONS;
    prop.val  = (uintptr_t)&appFunctions;
    streamer_set_property(handle->streamer, 0, prop, true);

    if (out_sink == ELEMENT_SPEAKER_INDEX)
    {
        appFunctions.open_func      = streamer_pcm_tx_open;
        appFunctions.close_func     = streamer_pcm_tx_close;
        appFunctions.start_func     = NULL;
        appFunctions.process_func   = streamer_pcm_write;
        appFunctions.set_param_func = streamer_pcm_setparams;
        appFunctions.get_param_func = streamer_pcm_getparams;
        appFunctions.mute_func      = streamer_pcm_mute;
        appFunctions.volume_func    = streamer_pcm_set_volume;

        prop.prop = PROP_SPEAKER_SET_APP_FUNCTIONS;
        prop.val  = (uintptr_t)&appFunctions;
        streamer_set_property(handle->streamer, 0, prop, true);
    }
    else
    {
        prop.prop = PROP_MICROPHONE_SET_DUMMY_TX_ENABLE;
        prop.val  = true;
        streamer_set_property(handle->streamer, 0, prop, true);
    }

    if (out_sink == ELEMENT_FILE_SINK_INDEX)
    {
        char file_name_val[MAX_FILE_NAME_LENGTH];
        memcpy(file_name_val, file_name == NULL ? "tmp" : file_name, MAX_FILE_NAME_LENGTH);
        strcat(file_name_val, ".pcm");

        prop.prop = PROP_FILESINK_LOCATION;
        prop.val  = (uintptr_t)file_name_val;
        streamer_set_property(handle->streamer, 0, prop, true);
    }

    return kStatus_Success;
}

#ifdef OPUS_ENCODE
status_t STREAMER_opusmem2mem_Create(streamer_handle_t *handle,
                                     CeiBitstreamInfo *info,
                                     MEMSRC_SET_BUFFER_T *inBuf,
                                     SET_BUFFER_DESC_T *outBuf)
{
    STREAMER_CREATE_PARAM params;
    osa_task_def_t thread_attr;
    ELEMENT_PROPERTY_T prop;
    int ret;
    ElementIndex element_ids[]        = {ELEMENT_MEM_SRC_INDEX, ELEMENT_ENCODER_INDEX, ELEMENT_MEM_SINK_INDEX};
    int num_elements                  = sizeof(element_ids) / sizeof(ElementIndex);
    PipelineElements pipelineElements = {element_ids, num_elements};

    /* Create streamer */
    strcpy(params.out_mq_name, APP_STREAMER_MSG_QUEUE);
    params.stack_size   = STREAMER_OPUS_TASK_STACK_SIZE;
    params.task_name    = STREAMER_TASK_NAME;
    params.in_dev_name  = "";
    params.out_dev_name = "";
    params.elements     = pipelineElements;

    handle->streamer = streamer_create(&params);
    if (!handle->streamer)
    {
        return kStatus_Fail;
    }

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

    prop.prop = PROP_MEMSRC_SET_BUFF;
    prop.val  = (uintptr_t)inBuf;
    ret       = streamer_set_property(handle->streamer, 0, prop, true);
    if (ret != STREAM_OK)
    {
        streamer_destroy(handle->streamer);
        handle->streamer = NULL;

        return kStatus_Fail;
    }

    prop.prop = PROP_MEMSINK_BUFFER_DESC;
    prop.val  = (uintptr_t)outBuf;
    ret       = streamer_set_property(handle->streamer, 0, prop, true);
    if (ret != STREAM_OK)
    {
        streamer_destroy(handle->streamer);
        handle->streamer = NULL;

        return kStatus_Fail;
    }

    prop.prop = PROP_ENCODER_TYPE;
    prop.val  = (uintptr_t)CEIENC_OPUS;
    ret       = streamer_set_property(handle->streamer, 0, prop, true);
    if (ret != STREAM_OK)
    {
        streamer_destroy(handle->streamer);
        handle->streamer = NULL;

        return kStatus_Fail;
    }

    prop.prop = PROP_ENCODER_BITSTREAMINFO;
    prop.val  = (uintptr_t)info;
    ret       = streamer_set_property(handle->streamer, 0, prop, true);
    if (ret != STREAM_OK)
    {
        streamer_destroy(handle->streamer);
        handle->streamer = NULL;

        return kStatus_Fail;
    }

    return kStatus_Success;
}
#endif

void STREAMER_Destroy(streamer_handle_t *handle)
{
    streamer_destroy(handle->streamer);
    handle->streamer = NULL;
    
    deinit_logging();
}

void STREAMER_Init(void)
{
    /* Initialize logging */
    init_logging();

    /* Uncomment below to turn on full debug logging for the streamer. */
    // set_debug_module(0xffffffff);
    // set_debug_level(LOGLVL_DEBUG);
    // get_debug_state();

    /* Initialize streamer PCM management library. */
    streamer_pcm_init();
}
