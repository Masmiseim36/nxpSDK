/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "fwk_sln_platform.h"

#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_sln_task.h"
#include "fwk_perf.h"

#include "fwk_audio_processing.h"
#include "hal_audio_processing_dev.h"

/*******************************************************************************
 * Global Vars
 ******************************************************************************/

typedef struct
{
    fwk_task_data_t commonData;
    fwk_message_t audioReqMsg;
    audio_processing_dev_t *devs[MAXIMUM_AUDIO_PROCESSING_DEV]; /* registered audio procesing devices */
    fwk_message_t msgs[MAXIMUM_AUDIO_PROCESSING_DEV];           /* input messages */
} audio_task_data_t;

typedef struct
{
    fwk_task_t task;
    audio_task_data_t audioData;
} audio_task_t;

/*
 * audio processing task
 */
static audio_task_t s_AudioTask;

#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_AudioTaskStack[AUDIO_PROCESSING_TASK_STACK];
FWKDATA static StaticTask_t s_AudioTaskTCB;
static void *s_AudioTaskTCBReference = (void *)&s_AudioTaskTCB;
#else
static void *s_AudioTaskStack        = NULL;
static void *s_AudioTaskTCBReference = NULL;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
static int _FWK_AudioProcessing_DeviceCallback(
    const audio_processing_dev_t *dev, audio_processing_event_t event, void *param, unsigned int size, uint8_t fromISR)
{
    fwk_message_t *pAudioReqMsg;
    if (event == kAudioProcessingEvent_Done)
    {
        pAudioReqMsg              = &s_AudioTask.audioData.audioReqMsg;
        pAudioReqMsg->id          = kFWKMessageID_VAlgoASRInputProcess;
        pAudioReqMsg->audio.devId = dev->id;
        pAudioReqMsg->audio.data  = param;
        pAudioReqMsg->audio.size  = size;
        if (fromISR)
        {
            FWK_Message_PutFromIsr(kFWKTaskID_VoiceAlgo, &pAudioReqMsg);
        }
        else
        {
            FWK_Message_Put(kFWKTaskID_VoiceAlgo, &pAudioReqMsg);
        }
    }

    if (event == kAudioProcessingEvent_Dump)
    {
        pAudioReqMsg              = &s_AudioTask.audioData.msgs[0];
        pAudioReqMsg->id          = kFWKMessageID_InputAudioReceived;
        pAudioReqMsg->audio.devId = dev->id;
        pAudioReqMsg->audio.data  = param;
        pAudioReqMsg->audio.size  = size;
        if (fromISR)
        {
            FWK_Message_PutFromIsr(kFWKTaskID_Output, &pAudioReqMsg);
        }
        else
        {
            FWK_Message_Put(kFWKTaskID_Output, &pAudioReqMsg);
        }
    }
    return 0;
}

static int _FWK_AudioProcessing_TaskInit(fwk_task_data_t *pTaskData)
{
    if (pTaskData == NULL)
        return -1;
    int error                         = 0;
    audio_task_data_t *pAudioTaskData = (audio_task_data_t *)pTaskData;

    /* init the input dev */
    for (int i = 0; i < MAXIMUM_AUDIO_PROCESSING_DEV; i++)
    {
        audio_processing_dev_t *pDev = pAudioTaskData->devs[i];
        if (pDev != NULL && pDev->ops->init != NULL)
        {
            LOGD("INIT audio processing dev [%d]", i);
            error = pDev->ops->init(pDev, _FWK_AudioProcessing_DeviceCallback);

            if (error)
            {
                LOGE("INIT audio processing dev [%d] error: %d", i, error);
            }
        }
    }

    for (int i = 0; i < MAXIMUM_AUDIO_PROCESSING_DEV; i++)
    {
        audio_processing_dev_t *pDev = pAudioTaskData->devs[i];
        if (pDev != NULL && pDev->ops->start != NULL)
        {
            LOGD("START audio processing dev [%d]", i);
            error = pDev->ops->start(pDev);

            if (error)
            {
                LOGE("START audio processing dev [%d] error: %d", i, error);
                return error;
            }
        }
    }

    return error;
}

static void _FWK_AudioProcessing_MessageHandle(fwk_message_t *pMsg, fwk_task_data_t *pTaskData)
{
    if ((pMsg == NULL) || (pTaskData == NULL))
    {
        return;
    }

    audio_task_data_t *pAudioTaskData = (audio_task_data_t *)pTaskData;
    switch (pMsg->id)
    {
        case kFWKMessageID_InputAudioReceived:
        {
            for (int i = 0; i < MAXIMUM_AUDIO_PROCESSING_DEV; i++)
            {
                hal_audio_processing_status_t status;
                audio_processing_dev_t *pDev = pAudioTaskData->devs[i];
                status                       = pDev->ops->run(pDev, pMsg->raw.data);
                if (status != kStatus_HAL_AudioProcessingSuccess)
                {
                    LOGE("Audio processing dev [%d] run failed error: %d", i, status);
                }

                if (pMsg->raw.freeAfterConsumed)
                {
                    SLN_FREE(pMsg->raw.data);
                }
            }
        }
        break;
        case kFWKMessageID_InputNotify:
        {
            for (int i = 0; i < MAXIMUM_AUDIO_PROCESSING_DEV; i++)
            {
                hal_audio_processing_status_t status;
                audio_processing_dev_t *pDev = pAudioTaskData->devs[i];
                status                       = pDev->ops->inputNotify(pDev, pMsg->raw.data);

                if (status != kStatus_HAL_AudioProcessingSuccess)
                {
                    LOGE("inputNotify fail");
                }

                if (pMsg->raw.freeAfterConsumed)
                {
                    SLN_FREE(pMsg->raw.data);
                }
            }
        }
        default:
            break;
    }
}
int FWK_AudioProcessing_Init()
{
    /* create the input task */
    LOGD("[AUDIOPROCESSING]:Init...");

    for (int i = 0; i < MAXIMUM_AUDIO_PROCESSING_DEV; i++)
    {
        s_AudioTask.audioData.devs[i] = NULL;
    }

    for (int i = 0; i < MAXIMUM_AUDIO_PROCESSING_DEV; i++)
    {
        fwk_message_t *pMsg = &s_AudioTask.audioData.msgs[i];
        pMsg->input.devId   = i;
    }

    return 0;
}

int FWK_AudioProcessing_Deinit()
{
    return 0;
}

int FWK_AudioProcessing_Start()
{
    LOGD("[AudioProcessing]:Starting...");
    int error = 0;

    s_AudioTask.task.msgHandle  = _FWK_AudioProcessing_MessageHandle;
    s_AudioTask.task.taskInit   = _FWK_AudioProcessing_TaskInit;
    s_AudioTask.task.data       = (fwk_task_data_t *)&(s_AudioTask.audioData);
    s_AudioTask.task.taskId     = kFWKTaskID_Audio;
    s_AudioTask.task.delayMs    = 1;
    s_AudioTask.task.taskStack  = s_AudioTaskStack;
    s_AudioTask.task.taskBuffer = s_AudioTaskTCBReference;

    FWK_Task_Start((fwk_task_t *)&s_AudioTask.task, AUDIO_PROCESSING_TASK_NAME, AUDIO_PROCESSING_TASK_STACK,
                   AUDIO_PROCESSING_TASK_PRIORITY);

    LOGD("[AudioProcessing]:Started");

    return error;
}

int FWK_AudioProcessing_DeviceRegister(audio_processing_dev_t *dev)
{
    int error = -1;

    for (int i = 0; i < MAXIMUM_AUDIO_PROCESSING_DEV; i++)
    {
        if (s_AudioTask.audioData.devs[i] == NULL)
        {
            dev->id                       = i;
            s_AudioTask.audioData.devs[i] = dev;
            return 0;
        }
    }

    return error;
}
