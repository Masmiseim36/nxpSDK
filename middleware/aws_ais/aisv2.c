/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <stdio.h>

#include "aisv2.h"
#include "aisv2_app.h"

#include "audio_processing_task.h"
#include "reconnection_task.h"
#include "mqtt_connection.h"
#include "sln_RT10xx_RGB_LED_driver.h"

/*! @brief AIS internal task settings */
#define AISV2_TASK_NAME "AIS_Task"
#define AISV2_TASK_STACK_SIZE 1024
#define AISV2_TASK_PRIORITY tskIDLE_PRIORITY

#define AISV2_PUBLISH_TASK_NAME "AIS_Publish_Task"
#define AISV2_PUBLISH_TASK_STACK_SIZE 1024
#define AISV2_PUBLISH_TASK_PRIORITY configTIMER_TASK_PRIORITY - 1

#define AIS_TX_MIC_BUFFER_SIZE (AIS_MIC_PACKET_SIZE + sizeof(binaryStream_t))
#define AIS_TX_JSON_BUFFER_SIZE (2048)

static uint8_t s_publishMicErrCount = 0;

__attribute__((section(
    ".ocram_non_cacheable_data"))) static ais_seq_directive_slot_t s_directivePool[AIS_SEQUENCE_BUFFER_SLOTS] = {0x00};
__attribute__((section(
    ".ocram_non_cacheable_data"))) static ais_seq_speaker_slot_t s_speakerPool[AIS_SEQUENCE_BUFFER_SLOTS]     = {0x00};

static const char *s_aisDisconnnectString[] = {"",
                                               "UNEXPECTED_SEQUENCE_NUMBER",
#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))
                                               "MESSAGE_TAMPERED",
#endif
                                               "API_VERSION_DEPRECATED",
                                               "ENCRYPTION_ERROR",
                                               "GOING_OFFLINE"};

/* Forward function declarations for AIS-private functions. */

MQTTBool_t AIS_CallbackSpeaker(void *pvUserData, const MQTTPublishData_t *const pxPublishParameters);
MQTTBool_t AIS_CallbackDirective(void *pvUserData, const MQTTPublishData_t *const pxPublishParameters);
MQTTBool_t AIS_CallbackCapabilities(void *pvUserData, const MQTTPublishData_t *const pxPublishParameters);
MQTTBool_t AIS_CallbackConnect(void *pvUserData, const MQTTPublishData_t *const pxPublishParameters);

status_t AIS_PublishMicrophone(ais_handle_t *handle, uint32_t size);
status_t AIS_Subscribe(ais_handle_t *handle, aisTopic_t topic, void *callback, void *callbackContext);
status_t AIS_Unsubscribe(ais_handle_t *handle, aisTopic_t topic);

bool AIS_ProcessSpeaker(ais_handle_t *handle, const char *data, uint32_t length);
bool AIS_ProcessDirective(ais_handle_t *handle, const char *data, uint32_t length);

status_t AIS_PublishConnect(ais_handle_t *handle);
status_t AIS_PublishDisconnect(ais_handle_t *handle, const char *code);
status_t AIS_PublishCapabilities(ais_handle_t *handle);

/*!
 * @brief Checks if the sequence receive timeout was triggered
 * @param savedTick time with which to compare the current one
 * @returns true if timeout happened, false otherwise
 */
static bool AIS_SeqTimeoutCheck(TickType_t savedTick);

uint8_t AIS_SeqBuffer_Size(ais_seq_buf_t *list)
{
    uint8_t i;

    for (i = 0; i < AIS_SEQUENCE_BUFFER_SLOTS; i++)
    {
        if (!list[i].data)
            break;
    }

    return i;
}

bool AIS_SeqBuffer_HasSeq(ais_seq_buf_t *list, uint32_t seq)
{
    for (uint8_t i = 0; i < AIS_SEQUENCE_BUFFER_SLOTS; i++)
    {
        if (!list[i].data || (seq < list[i].seq))
        {
            break;
        }
        else if (list[i].seq == seq)
        {
            return true;
        }
    }

    return false;
}

/* Finds the first available memory location */
static void *_alocateSlot(aisTopic_t topic)
{
    if (topic == AIS_TOPIC_DIRECTIVE)
    {
        for (uint8_t i = 0; i < AIS_SEQUENCE_BUFFER_SLOTS; i++)
        {
            if (s_directivePool[i].isFree == true)
            {
                s_directivePool[i].isFree = false;
                return &s_directivePool[i];
            }
        }
    }

    if (topic == AIS_TOPIC_SPEAKER)
    {
        for (uint8_t i = 0; i < AIS_SEQUENCE_BUFFER_SLOTS; i++)
        {
            if (s_speakerPool[i].isFree == true)
            {
                s_speakerPool[i].isFree = false;
                return &s_speakerPool[i];
            }
        }
    }

    return NULL;
}

static void _initPool()
{
    for (uint8_t i = 0; i < AIS_SEQUENCE_BUFFER_SLOTS; i++)
    {
        s_speakerPool[i].isFree   = true;
        s_directivePool[i].isFree = true;
    }
}

/* Ordered insertion of the packet data. */
bool AIS_SeqBuffer_Insert(ais_seq_buf_t *list, ais_seq_buf_t *packet, aisTopic_t topic)
{
    uint8_t i                               = AIS_SeqBuffer_Size(list);
    ais_seq_directive_slot_t *dirPoolSlot   = NULL;
    ais_seq_speaker_slot_t *speakerPoolSlot = NULL;

    if (i == AIS_SEQUENCE_BUFFER_SLOTS)
    {
        return false;
    }

    while ((i > 0) && (packet->seq < list[i - 1].seq))
    {
        list[i].seq  = list[i - 1].seq;
        list[i].data = list[i - 1].data;
        list[i].size = list[i - 1].size;
        i--;
    }

    /* Copy the data into the OCRAM location */
    if (topic == AIS_TOPIC_DIRECTIVE)
    {
        dirPoolSlot = _alocateSlot(topic);
        if (dirPoolSlot == NULL)
        {
            return false;
        }

        memcpy(dirPoolSlot->data, packet->data, packet->size);
        packet->data = dirPoolSlot;
    }
    else if (topic == AIS_TOPIC_SPEAKER)
    {
        speakerPoolSlot = _alocateSlot(topic);
        if (speakerPoolSlot == NULL)
        {
            return false;
        }

        memcpy(speakerPoolSlot->data, packet->data, packet->size);
        packet->data = speakerPoolSlot;
    }
    else
    {
        return false;
    }

    list[i].data = packet->data;
    list[i].size = packet->size;
    list[i].seq  = packet->seq;

    return true;
}

void AIS_SeqBuffer_Pop(ais_seq_buf_t *list, aisTopic_t topic)
{
    uint8_t size                            = AIS_SeqBuffer_Size(list);
    ais_seq_directive_slot_t *dirPoolSlot   = NULL;
    ais_seq_speaker_slot_t *speakerPoolSlot = NULL;

    if (size == 0)
    {
        return;
    }

    /* Mark the memory location as free */
    if (topic == AIS_TOPIC_DIRECTIVE)
    {
        dirPoolSlot         = list[0].data;
        dirPoolSlot->isFree = true;
    }
    else if (topic == AIS_TOPIC_SPEAKER)
    {
        speakerPoolSlot         = list[0].data;
        speakerPoolSlot->isFree = true;
    }
    else
    {
        return;
    }

    for (uint8_t i = 0; i < size - 1; i++)
    {
        list[i].seq  = list[i + 1].seq;
        list[i].data = list[i + 1].data;
        list[i].size = list[i + 1].size;
    }

    list[size - 1].seq  = 0;
    list[size - 1].data = 0;
    list[size - 1].size = 0;
}

/*! @brief Internal state execution for sending mic data */
static void AIS_StateMicrophone(ais_handle_t *handle)
{
    uint32_t size;
    uint8_t *micData;
    status_t ret;
    uint64_t offsetBackup = 0;

    binaryStream_t *stream;
    stream = (binaryStream_t *)handle->msgMicBuffer;
    memset(stream, 0, sizeof(binaryStream_t));

    stream->audio.audioData.offset = handle->micStream.audio.audioData.offset;
    offsetBackup                   = stream->audio.audioData.offset;

    /* Fill buffer with mic data to send to AIS.
     * Application responsible to returning from this call with mic data pointer
     * and size set. */
    micData = (uint8_t *)&(stream->audio.audioData.data);
    if (kStatus_Success == AIS_AppCallback_Microphone(&micData, &size))
    {
        /* Send mic data to AIS service. */
        ret = AIS_PublishMicrophone(handle, size);

        if (kStatus_Success == ret)
        {
            if (offsetBackup != handle->micStream.audio.audioData.offset)
            {
                configPRINTF(("WARNING: mic offset updated by other task, old offset: %llu, new offset: %llu\r\n",
                              offsetBackup, handle->micStream.audio.audioData.offset));

                /* do not increment the offset in this case */
            }
            else
            {
                handle->micStream.audio.audioData.offset += size;
            }

            s_publishMicErrCount = 0;
        }
        else
        {
            /* do not increment the offset in this case */
            /* error log printed in AIS_PublishMicrophone */
            s_publishMicErrCount += 1;
        }

        /* If too many consecutive mic publish errors, trigger an ais reconnect */
        if (s_publishMicErrCount >= AIS_MIC_PUBLISH_ERR_MAX)
        {
            s_publishMicErrCount = 0;
            if (reconnection_task_set_event(kReconnectAISDisconnect))
            {
                configPRINTF(("Publish microphone errors threshold reached, triggering a reconnection\r\n"));
            }
        }
    }
}

/*! @brief Internal state execution for receiving speaker data */
void AIS_StateSpeaker(ais_handle_t *handle)
{
    /* Call app handler to update playback status. */
    AIS_AppCallback_SpeakerState(handle);
}

/*! @brief Terminate streamer playback and receiving speaker data */
void AIS_TerminateSpeaker(ais_handle_t *handle)
{
    /* Call app handler to terminate playback. */
    AIS_AppCallback_TerminateSpeaker(handle);
}

void AIS_MessageResequence(ais_handle_t *handle, aisTopic_t topic, ais_process_func_t processFunc)
{
    ais_seq_buf_t *list     = NULL;
    ais_app_data_t *appData = AIS_APP_GetAppData();
    SemaphoreHandle_t mutex;
    uint8_t seqBufSize = 0;
    TickType_t *timer  = NULL;

    if (topic == AIS_TOPIC_DIRECTIVE)
    {
        list  = &handle->seqListDirective[0];
        mutex = handle->seqMutexDirective;
        timer = &handle->seqTimerDirective;
    }
    else if (topic == AIS_TOPIC_SPEAKER)
    {
        list  = &handle->seqListSpeaker[0];
        mutex = handle->seqMutexSpeaker;
        timer = &handle->seqTimerSpeaker;
    }
    else
    {
        /* Invalid topic */
        return;
    }

    xSemaphoreTake(mutex, portMAX_DELAY);

    seqBufSize = AIS_SeqBuffer_Size(list);

    bool processed = false;
    /* Check to see if there are any directives in the re-sequencing buffer. */
    while ((seqBufSize > 0) && (list[0].seq == handle->topicSequence[topic]))
    {
        if (!processFunc(handle, list[0].data, list[0].size))
        {
            if ((topic == AIS_TOPIC_SPEAKER) && (appData->overrunSequence == 0))
            {
                AIS_AppCallback_SpeakerOverflow(handle, handle->topicSequence[topic]);
            }
            break;
        }
        else if (topic == AIS_TOPIC_SPEAKER)
        {
            appData->prevSpeakerBufferState = appData->speakerBufferState;
            appData->speakerBufferState     = AIS_BUFFER_STATE_OVERRUN_WARNING;
        }

        if (!processed)
        {
            processed = true;
        }
        handle->topicSequence[topic]++;
        AIS_SeqBuffer_Pop(list, topic);
        seqBufSize--;

        AIS_CheckForSecretRotate(handle, topic);
    }

    if ((topic == AIS_TOPIC_SPEAKER))
    {
        /* Reset timer for next speaker sequence */
        if (processed && (*timer != 0))
        {
            *timer = xTaskGetTickCount();
        }
    }

    if (topic == AIS_TOPIC_DIRECTIVE)
    {
        if ((seqBufSize == 0) && (*timer != 0))
        {
            /* Stop timer for non-binary stream topics when no resequencing needed */
            *timer = 0;
        }
        else if ((seqBufSize > 0) && (*timer == 0))
        {
            /* Restart timer for non-binary stream topics when out of sequence */
            *timer = xTaskGetTickCount();
        }
    }

    /* Check to see if timer has expired before receiving message. */
    if (*timer != 0)
    {
        if (AIS_SeqTimeoutCheck(*timer))
        {
            configPRINTF(
                ("Didn't receive a message for over 10 seconds, last: %d, now: %d\r\n", *timer, xTaskGetTickCount()));
            *timer = 0;
            /* Timeout: disconnect from service. Pass information back to app to handle disconnect. */
            AIS_AppDisconnected(handle, AIS_DISCONNECT_INVALID_SEQUENCE,
                                s_aisDisconnnectString[AIS_DISCONNECT_INVALID_SEQUENCE]);
        }
    }

    xSemaphoreGive(mutex);
}

/*! @brief Internal state execution for all states.
 * This is called each iteration through the state machine. */
static void AIS_StateBackground(ais_handle_t *handle)
{
    /* Check pending disconnect flagged for error. */
    if (handle->pendDisconnect && !handle->pendDisconnectSent)
    {
        /* Set handle->pendDisconnectSent to true in AIS_AppDisconnected function */
        AIS_AppDisconnected(handle, handle->pendDisconnectCode, s_aisDisconnnectString[handle->pendDisconnectCode]);

        /* Return and do no further processing until a reconnect. */
        return;
    }

    /* Check resequence buffers for directive and speaker. */
    AIS_MessageResequence(handle, AIS_TOPIC_DIRECTIVE, AIS_ProcessDirective);
    AIS_MessageResequence(handle, AIS_TOPIC_SPEAKER, AIS_ProcessSpeaker);

    /* Check for a rotating secret. */
    if (handle->rotateSecretReq)
    {
        /* When all the topics secrets have been rotated, shuffle the memory and pointers around */
        if ((handle->rotateSecret[AIS_TOPIC_MICROPHONE] == false) &&
            (handle->rotateSecret[AIS_TOPIC_SPEAKER] == false) && (handle->rotateSecret[AIS_TOPIC_EVENT] == false) &&
            (handle->rotateSecret[AIS_TOPIC_DIRECTIVE] == false))
        {
            /* Flush the old secret */
            AIS_SetSecret(handle, handle->config->tempRegistrationConfig.sharedSecret);

            /* Point to the correct memory after moving the secret to long term location */
            AIS_UpdateTopicSecretPointer(handle, AIS_TOPIC_MICROPHONE, handle->config->registrationConfig.sharedSecret);

            AIS_UpdateTopicSecretPointer(handle, AIS_TOPIC_SPEAKER, handle->config->registrationConfig.sharedSecret);

            AIS_UpdateTopicSecretPointer(handle, AIS_TOPIC_DIRECTIVE, handle->config->registrationConfig.sharedSecret);

            AIS_UpdateTopicSecretPointer(handle, AIS_TOPIC_EVENT, handle->config->registrationConfig.sharedSecret);

            /* Clear up the redundant memory */
            memset(handle->config->tempRegistrationConfig.sharedSecret, 0,
                   sizeof(handle->config->tempRegistrationConfig.sharedSecret));

            /* All topics have been rotated and the clean up has been complete, time to clear the rotate request for
             * next time */
            handle->rotateSecretReq = false;
        }
    }
}

void AIS_SetState(ais_handle_t *handle, uint8_t state)
{
    if (state == AIS_TASK_STATE_IDLE)
        handle->state = AIS_TASK_STATE_IDLE;
    else
        handle->state |= state;
}

void AIS_ClearState(ais_handle_t *handle, uint8_t state)
{
    handle->state &= ~state;
}

/*! @brief Check if state bitmask is currently set */
bool AIS_CheckState(ais_handle_t *handle, uint8_t state)
{
    return handle->state & state;
}

void AIS_State_Lock(ais_handle_t *handle)
{
    xSemaphoreTakeRecursive(handle->aisStateLock, portMAX_DELAY);
}

void AIS_State_Unlock(ais_handle_t *handle)
{
    xSemaphoreGiveRecursive(handle->aisStateLock);
}

/*! @brief Internal AIS task entry point */
static void AIS_Task(void *arg)
{
    ais_handle_t *handle = (ais_handle_t *)arg;

    configPRINTF(("[AIS] Starting AIS Task\r\n"));

    /* Enter AIS state loop.
     * Will never return, unless error occurs.
     * Deinit function will delete this thread. */

    while (1)
    {
        if (handle->aisConnected)
        {
            if (AIS_CheckState(handle, AIS_TASK_STATE_MICROPHONE))
            {
                AIS_StateMicrophone(handle);
            }

            if (AIS_CheckState(handle, AIS_TASK_STATE_SPEAKER))
            {
                AIS_StateSpeaker(handle);
            }

            if (AIS_CheckState(handle, AIS_TASK_STATE_ERROR))
            {
                goto error;
            }

            /* Execute background tasks in all states. */
            AIS_StateBackground(handle);
        }

        /* Break in the scheduler to let other tasks execute. */
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }

error:
    configPRINTF(("[AIS ERROR] Task failure: exiting\r\n"));
    vTaskDelete(NULL);
}

status_t AIS_Connect(ais_handle_t *handle)
{
    EventBits_t eventRet;
    status_t ret;

    /* Verify configuration has been applied. */
    if (!handle->config)
    {
        configPRINTF(("[AIS ERROR] Configuration not set\r\n"));
        ret = kStatus_Fail;
        goto exit_connect;
    }

    /* Initialize application-specific functionality on each connection. */
    AIS_AppInit();

    /* Reset topic sequence numbers to zero. */
    memset(handle->topicSequence, 0, sizeof(handle->topicSequence));

    /* Flush the old secret as we aren't going to be using it now */
    memset(handle->config->tempRegistrationConfig.sharedSecret, 0,
           sizeof(handle->config->tempRegistrationConfig.sharedSecret));

    /* Point the pointers to the latest secret */
    handle->topicSecret[AIS_TOPIC_MICROPHONE]               = handle->config->registrationConfig.sharedSecret;
    handle->topicSecret[AIS_TOPIC_EVENT]                    = handle->config->registrationConfig.sharedSecret;
    handle->topicSecret[AIS_TOPIC_SPEAKER]                  = handle->config->registrationConfig.sharedSecret;
    handle->topicSecret[AIS_TOPIC_DIRECTIVE]                = handle->config->registrationConfig.sharedSecret;
    handle->topicSecret[AIS_TOPIC_CAPABILITIES_ACKNOWLEDGE] = handle->config->registrationConfig.sharedSecret;
    handle->topicSecret[AIS_TOPIC_CAPABILITIES_PUBLISH]     = handle->config->registrationConfig.sharedSecret;

    /* No need to rotate for each topic */
    handle->rotateSecret[AIS_TOPIC_MICROPHONE] = false;
    handle->rotateSecret[AIS_TOPIC_EVENT]      = false;
    handle->rotateSecret[AIS_TOPIC_SPEAKER]    = false;
    handle->rotateSecret[AIS_TOPIC_DIRECTIVE]  = false;

    /* Reset the rotate topic sequence to 0 */
    handle->rotateTopicSequence[AIS_TOPIC_MICROPHONE] = 0;
    handle->rotateTopicSequence[AIS_TOPIC_EVENT]      = 0;
    handle->rotateTopicSequence[AIS_TOPIC_SPEAKER]    = 0;
    handle->rotateTopicSequence[AIS_TOPIC_DIRECTIVE]  = 0;

    /* Reset the rotate secret req */
    handle->rotateSecretReq = false;

    /* Reset sequencing timers */
    handle->seqTimerDirective = 0;
    handle->seqTimerSpeaker   = 0;

    /* Reset offset to zero */
    handle->micStream.audio.audioData.offset = 0;

    /* No pending disconnects. */
    handle->pendDisconnect     = false;
    handle->pendDisconnectSent = false;
    handle->pendDisconnectCode = AIS_DISCONNECT_NONE;

    if (handle->firstConnect)
    {
        /* Subscribe to the directive topic. */
        ret = AIS_Subscribe(handle, AIS_TOPIC_DIRECTIVE, (void *)AIS_CallbackDirective, (void *)handle);
        if (ret != kStatus_Success)
        {
            goto exit_connect;
        }

        /* Subscribe to the speaker topic. */
        ret = AIS_Subscribe(handle, AIS_TOPIC_SPEAKER, (void *)AIS_CallbackSpeaker, (void *)handle);
        if (ret != kStatus_Success)
        {
            goto exit_connect;
        }

        /* Subscribe to the capabilities/acknowledge topic. */
        ret =
            AIS_Subscribe(handle, AIS_TOPIC_CAPABILITIES_ACKNOWLEDGE, (void *)AIS_CallbackCapabilities, (void *)handle);
        if (ret != kStatus_Success)
        {
            goto exit_connect;
        }

        /* Subscribe to the connection/fromservice topic. */
        ret = AIS_Subscribe(handle, AIS_TOPIC_CONNECTION_FROMSERVICE, (void *)AIS_CallbackConnect, (void *)handle);
        if (ret != kStatus_Success)
        {
            goto exit_connect;
        }
    }

    /* Publish connect request */
    ret = AIS_PublishConnect(handle);
    if (ret != kStatus_Success)
    {
        goto exit_connect;
    }

    /* Wait up to 10 seconds for a response. */
    eventRet =
        xEventGroupWaitBits(handle->aisEvent, AIS_MSG_ACCEPT | AIS_MSG_REJECT, pdTRUE, pdFALSE, AIS_MSG_RETRY_TIMEOUT);
    if ((eventRet & AIS_MSG_ACCEPT) == AIS_MSG_ACCEPT)
    {
        ret = kStatus_Success;
    }
    else if ((eventRet & AIS_MSG_REJECT) == AIS_MSG_REJECT)
    {
        /* Capabilities rejected. */
        ret = kStatus_Fail;
        goto exit_connect;
    }
    else
    {
        /* Capabilities timed out. */
        configPRINTF(("[AIS ERR] Connect timed out\r\n"));
        ret = kStatus_Fail;
        goto exit_connect;
    }

    xEventGroupClearBits(handle->aisEvent, AIS_MSG_ACCEPT | AIS_MSG_REJECT);

    /* Send capabilities and wait for acceptance, only on first connection. */
    if (handle->firstConnect)
    {
        ret = AIS_PublishCapabilities(handle);
        if (ret != kStatus_Success)
        {
            goto exit_connect;
        }

        /* Wait up to 10 seconds for a response. */
        eventRet = xEventGroupWaitBits(handle->aisEvent, AIS_MSG_ACCEPT | AIS_MSG_REJECT, pdTRUE, pdFALSE,
                                       AIS_MSG_RETRY_TIMEOUT);
        if ((eventRet & AIS_MSG_ACCEPT) == AIS_MSG_ACCEPT)
        {
            /* Capabilities accepted. */
            handle->firstConnect = false;
            ret                  = kStatus_Success;
        }
        else if ((eventRet & AIS_MSG_REJECT) == AIS_MSG_REJECT)
        {
            /* Capabilities rejected. */
            ret = kStatus_Fail;
        }
        else
        {
            /* Capabilities timed out. */
            configPRINTF(("[AIS ERR] Capabilities timed out\r\n"));
            ret = kStatus_Fail;
        }

        xEventGroupClearBits(handle->aisEvent, AIS_MSG_ACCEPT | AIS_MSG_REJECT);
    }

    /* Reset message re-sequencing lists. */
    memset(handle->seqListDirective, 0, sizeof(handle->seqListDirective));
    memset(handle->seqListSpeaker, 0, sizeof(handle->seqListSpeaker));

    /* Mark client as connected */
    handle->state        = 0;
    handle->aisConnected = true;

exit_connect:
    return ret;
}

void AIS_Disconnected(ais_handle_t *handle)
{
    handle->state             = 0;
    handle->aisConnected      = false;
    handle->firstConnect      = true;
    handle->seqTimerDirective = 0;
    handle->seqTimerSpeaker   = 0;
}

status_t AIS_Disconnect(ais_handle_t *handle, ais_disconnect_code_t code)
{
    status_t ret;

    configPRINTF(("[AIS] Client disconnect requested: %s\r\n", s_aisDisconnnectString[code]));

    if (AIS_Unsubscribe(handle, AIS_TOPIC_DIRECTIVE) != kStatus_Success ||
        AIS_Unsubscribe(handle, AIS_TOPIC_SPEAKER) != kStatus_Success ||
        AIS_Unsubscribe(handle, AIS_TOPIC_CAPABILITIES_ACKNOWLEDGE) != kStatus_Success ||
        AIS_Unsubscribe(handle, AIS_TOPIC_CONNECTION_FROMSERVICE) != kStatus_Success)
    {
        return kStatus_Fail;
    }

    ret = AIS_PublishDisconnect(handle, s_aisDisconnnectString[code]);

    if (ret != kStatus_Success)
    {
        return kStatus_Fail;
    }

    AIS_Disconnected(handle);

    return kStatus_Success;
}

void AIS_SetConfig(ais_handle_t *handle, ais_config_t *config)
{
    /* Store configuration in local handle. */
    handle->config = config;

    /* Initialize mbedTLS entropy for random number generation. */
    mbedtls_entropy_init(&handle->entropy);
    mbedtls_ctr_drbg_init(&handle->ctr_drbg);
    mbedtls_ctr_drbg_seed(&handle->ctr_drbg, mbedtls_entropy_func, &handle->entropy,
                          (uint8_t *)handle->config->awsClientId, strlen(handle->config->awsClientId));
}

void AIS_SetSecret(ais_handle_t *handle, uint8_t *secret)
{
    memcpy(handle->config->registrationConfig.sharedSecret, secret, AIS_SECRET_LENGTH);
}

status_t AIS_Init(ais_handle_t *handle, void *audioPlayer)
{
    BaseType_t taskReturn;

    memset(handle, 0, sizeof(ais_handle_t));
    /* Initialize the queuing pool with 0 */
    _initPool();

    /* Initialize state.
     * NOTE that all uninitialized boolean flags will be 'false'. */
    handle->state        = AIS_TASK_STATE_IDLE;
    handle->audioPlayer  = audioPlayer;
    handle->firstConnect = true;
    handle->aisStateLock = xSemaphoreCreateRecursiveMutex();

    /* Initialize event for signaling connect/capabilities retry. */
    handle->aisEvent = xEventGroupCreate();

    /* Allocate heap memory for outgoing MQTT messages. */
    handle->msgMicBuffer = pvPortMalloc(AIS_TX_MIC_BUFFER_SIZE);
    if (!handle->msgMicBuffer)
        return kStatus_Fail;

    handle->msgJsonBuffer = pvPortMalloc(AIS_TX_JSON_BUFFER_SIZE);
    if (!handle->msgJsonBuffer)
        return kStatus_Fail;

    handle->msgCryptBuffer = pvPortMalloc(AIS_MQTT_MAX_RX_SIZE);
    if (!handle->msgCryptBuffer)
        return kStatus_Fail;

    handle->seqMutexDirective = xSemaphoreCreateMutex();
    handle->seqMutexSpeaker   = xSemaphoreCreateMutex();

    /* TODO: read offline alerts from flash into RAM. */

    /* Create the task that publishes messages to the MQTT broker every five
     * milliseconds.  This task, in turn, creates the task that echoes data received
     * from the broker back to the broker. */
    taskReturn = xTaskCreate(AIS_Task, AISV2_TASK_NAME, AISV2_TASK_STACK_SIZE, (void *)handle, AISV2_TASK_PRIORITY,
                             &handle->mqttTask);
    if (taskReturn != pdPASS)
        return kStatus_Fail;

    taskReturn = xTaskCreate(AIS_PublishTask, AISV2_PUBLISH_TASK_NAME, AISV2_PUBLISH_TASK_STACK_SIZE, (void *)handle,
                             AISV2_PUBLISH_TASK_PRIORITY, NULL);
    if (taskReturn != pdPASS)
        return kStatus_Fail;

    return kStatus_Success;
}

void AIS_Deinit(ais_handle_t *handle)
{
    configPRINTF(("[AIS] Destroying AIS Task\r\n"));

    /* Unsubscribe and disconnect from MQTT, if still connected */
    AIS_Disconnect(handle, AIS_DISCONNECT_GOING_OFFLINE);

    /* Wait 1 second for messages to finish sending */
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    /* Destroy AIS task */
    /* TODO: send notification to task for graceful shutdown? */
    vTaskDelete(handle->mqttTask);

    vPortFree(handle->msgMicBuffer);
    vPortFree(handle->msgJsonBuffer);
    vPortFree(handle->msgCryptBuffer);

    vSemaphoreDelete(handle->aisStateLock);
    vSemaphoreDelete(handle->seqMutexDirective);
    vSemaphoreDelete(handle->seqMutexSpeaker);

    vEventGroupDelete(handle->aisEvent);
}

static bool AIS_SeqTimeoutCheck(TickType_t savedTick)
{
    return ((xTaskGetTickCount() - savedTick) >= AIS_MSG_SEQ_TIMEOUT_MSEC);
}
