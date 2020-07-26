/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aisv2.h"
#include "aisv2_app.h"
#include "ais_streamer.h"
#include "audio_processing_task.h"
#include "mbedtls/gcm.h"
#include "mqtt_connection.h"
#include "reconnection_task.h"
#include "streamer_pcm.h"
#include "mbedtls/base64.h"

/*! @brief MQTT connection parameters */
#define AIS_MQTT_TIMEOUT pdMS_TO_TICKS(1000)
#define AIS_MQTT_NUM_RETRIES 3

#define AIS_TOPIC_MICROPHONE_ROTATE_SEQUENCE_OFFSET (10)
#define AIS_TOPIC_EVENT_ROTATE_SEQUENCE_OFFSET (5)

/* Forward function declarations for AIS-private functions. */
uint8_t AIS_SeqBuffer_Size(ais_seq_buf_t *list);
bool AIS_SeqBuffer_HasSeq(ais_seq_buf_t *list, uint32_t seq);
bool AIS_SeqBuffer_Insert(ais_seq_buf_t *list, ais_seq_buf_t *packet, aisTopic_t topic);
void AIS_SeqBuffer_Pop(ais_seq_buf_t *list, aisTopic_t topic);

/* Node of linked lists used for queuing json publishing events */
typedef struct
{
    ais_handle_t *handle;
    aisTopic_t topic;
    cJSON *json;
    bool encrypt;
    void *next;
} json_publish_t;

extern ais_app_data_t appData;

/* One list for each topic */
static json_publish_t *s_JsonPublishQueues[AIS_TOPIC_LAST] = {0};
/* One lock for each topic */
static SemaphoreHandle_t s_JsonPublishLocks[AIS_TOPIC_LAST] = {0};
/* Used for signaling that a json publish event is available */
static TaskHandle_t s_publishTaskHandle = NULL;

/* time stamp for the last ais event topic publish */
static TickType_t s_lastTopicPublishTick[AIS_TOPIC_LAST] = {0};

const aisTopicMap_t aisTopicMap[] = {{AIS_TOPIC_CONNECTION_FROMCLIENT, "connection/fromclient"},
                                     {AIS_TOPIC_CONNECTION_FROMSERVICE, "connection/fromservice"},
                                     {AIS_TOPIC_CAPABILITIES_PUBLISH, "capabilities/publish"},
                                     {AIS_TOPIC_CAPABILITIES_ACKNOWLEDGE, "capabilities/acknowledge"},
                                     {AIS_TOPIC_DIRECTIVE, "directive"},
                                     {AIS_TOPIC_EVENT, "event"},
                                     {AIS_TOPIC_MICROPHONE, "microphone"},
                                     {AIS_TOPIC_SPEAKER, "speaker"}};

static const char *_ais_map_topic(aisTopic_t topic)
{
    for (int i = 0; i < AIS_TOPIC_LAST; i++)
    {
        if (aisTopicMap[i].topic == topic)
            return aisTopicMap[i].string;
    }

    return NULL;
}

void AIS_CheckForSecretRotate(ais_handle_t *handle, aisTopic_t topic)
{
    /* If there has been a rotate secret request */
    if (handle->rotateSecretReq == true)
    {
        /* If the secret for this topic hasn't been rotated */
        if (handle->rotateSecret[topic] == true)
        {
            /* If the sequence is equal to the expected rotate sequence */
            if (handle->rotateTopicSequence[topic] == handle->topicSequence[topic])
            {
                configPRINTF(("[AIS] Secret rotated for topic %s at sequence %d\r\n", _ais_map_topic(topic),
                              handle->rotateTopicSequence[topic]));
                /* Rotate the secret */
                handle->topicSecret[topic]  = handle->config->tempRegistrationConfig.sharedSecret;
                handle->rotateSecret[topic] = false;
            }
        }
    }
}

status_t AIS_Crypt(ais_handle_t *handle,
                   const uint8_t *key,
                   const uint8_t *in,
                   uint8_t *out,
                   uint32_t length,
                   uint8_t *iv,
                   uint8_t *mac,
                   aisCryptMode_t mode)
{
    mbedtls_gcm_context ctx;
    int ret = 0;

    /* Initialize AES-GCM crypto interface. */
    mbedtls_gcm_init(&ctx);
    mbedtls_gcm_setkey(&ctx, MBEDTLS_CIPHER_ID_AES, key, 128);

    if (mode == AIS_CRYPT_ENCRYPT)
    {
        mbedtls_gcm_crypt_and_tag(&ctx, MBEDTLS_GCM_ENCRYPT, length, iv, AIS_CRYPT_IV_LENGTH,
                                  NULL, // No AAD used
                                  0,    // No AAD used
                                  in, out, AIS_CRYPT_MAC_LENGTH, mac);
    }
    else
    {
        ret = mbedtls_gcm_auth_decrypt(&ctx, length, iv, AIS_CRYPT_IV_LENGTH,
                                       NULL, // No AAD used
                                       0,    // No AAD used
                                       mac, AIS_CRYPT_MAC_LENGTH, in, out);
    }

    mbedtls_gcm_free(&ctx);

    /* Failure will occur on a decrypt with a tag that doesn't match. */
    if (ret != 0)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}

/*!
 * @brief Build a topic string
 *
 * This function builds a usable topic string from the template, combining the
 * requested topic with the root, client ID, and version.
 *
 * @param handle AIS handle
 * @param topic Topic endpoint string
 * @param buffer Output buffer to write the full topic string to
 * @param size Size in bytes of the output buffer
 *
 * @returns 0 on success, otherwise returns number of bytes needed in buffer.
 */
static int32_t AIS_MakeTopic(ais_handle_t *handle, aisTopic_t topic, char *buffer)
{
    const char *topicStr;
    int ret;

    topicStr = _ais_map_topic(topic);

    ret = snprintf(buffer, AIS_TOPIC_MAX_LENGTH, AIS_TOPIC_TEMPLATE, handle->config->registrationConfig.awsPartnerRoot,
                   handle->config->awsClientId, topicStr);
    if (ret > AIS_TOPIC_MAX_LENGTH)
    {
        return ret;
    }

    return 0;
}

/*! @brief Publish MQTT microphone data to AIS service */
status_t AIS_PublishMicrophone(ais_handle_t *handle, uint32_t size)
{
    MQTTAgentPublishParams_t xPublishParameters;
    MQTTAgentReturnCode_t xReturned = eMQTTAgentSuccess;
    binaryStream_t *stream;
    char topicBuffer[AIS_TOPIC_MAX_LENGTH];
    uint32_t dataLength;
    uint8_t *dataEnc;

    AIS_MakeTopic(handle, AIS_TOPIC_MICROPHONE, topicBuffer);

    stream                        = (binaryStream_t *)handle->msgMicBuffer;
    stream->commonHeader.sequence = handle->topicSequence[AIS_TOPIC_MICROPHONE]++;

#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))
    stream->commonHeader.encryptedSequence = stream->commonHeader.sequence;
#endif

    stream->audio.binaryHeader.type   = 0;
    stream->audio.binaryHeader.length = size + 8;
    /* Add extra data for binaryStream_t type, in addition to mic data size. */
    dataLength = size + offsetof(binaryStream_t, audio.audioData.data);
    dataEnc    = (uint8_t *)stream + sizeof(plainTextHeader_t);

    configPRINTF(("[AIS] Publish microphone size: %d, offset: %lld, seq: %d\r\n", size, stream->audio.audioData.offset,
                  stream->commonHeader.sequence));

    /* Create IV data with crypto-secure RNG/hash */
    mbedtls_ctr_drbg_random(&handle->ctr_drbg, stream->commonHeader.iv, AIS_CRYPT_IV_LENGTH);

    /* Encrypt payload data for network transmission */
    /* NOTE: input and output buffer can be the same for encryption. */
    AIS_Crypt(handle, handle->topicSecret[AIS_TOPIC_MICROPHONE], (uint8_t *)dataEnc, (uint8_t *)dataEnc,
              dataLength - sizeof(plainTextHeader_t), stream->commonHeader.iv, stream->commonHeader.mac,
              AIS_CRYPT_ENCRYPT);

    /* Setup the publish parameters. */
    xPublishParameters.pucTopic      = (const uint8_t *)topicBuffer;
    xPublishParameters.usTopicLength = (uint16_t)strlen(topicBuffer);
    xPublishParameters.pvData        = stream;
    xPublishParameters.ulDataLength  = dataLength;
    xPublishParameters.xQoS          = eMQTTQoS0;

    if (APP_MQTT_Getv2Handle())
    {
        if (AIS_APP_MIC_PUBLISHING_HEAP_THRESHOLD > xPortGetFreeHeapSize())
        {
            /* Stop mic publishing when heap below the configured threshold;
             * Use this mechanism to prevent heap exhaustion when lwip thread hangs
             * due to bad connection, so the iot_threads are not freeing the memory,
             * while, at the same time, ais_task keeps creating mic publishing packages */
            configPRINTF(("Heap space below threshold, stopping mic publishing\r\n"));

            AIS_ClearState(handle, AIS_TASK_STATE_MICROPHONE);
            AIS_AppCallback_CloseMicrophone(handle);
        }
        else
        {
            xReturned = MQTT_AGENT_Publish(APP_MQTT_GetHandle(), &xPublishParameters, AIS_MQTT_TIMEOUT);
        }
    }
    else
    {
        xReturned = eMQTTAgentFailure;
    }

    AIS_CheckForSecretRotate(handle, AIS_TOPIC_MICROPHONE);

    if (xReturned != eMQTTAgentSuccess)
    {
        configPRINTF(("[AIS ERR] /microphone publish error, seq: %d\r\n", stream->commonHeader.sequence));
        return kStatus_Timeout;
    }
    else
    {
        return kStatus_Success;
    }
}

/*! @brief Publish MQTT JSON event to AIS service */
status_t AIS_PublishJSON(ais_handle_t *handle, aisTopic_t topic, cJSON *json, bool encrypt)
{
    MQTTAgentPublishParams_t xPublishParameters;
    MQTTAgentReturnCode_t xReturned;
    commonHeader_t *header = NULL;
    char *dataBuffer       = NULL;
    char *jsonStr          = NULL;
    char topicBuffer[AIS_TOPIC_MAX_LENGTH];
    uint32_t dataLength;
    uint32_t dataOffset = 0;
    uint32_t msec;
    status_t ret;

    /* Publishing rate for a topics should be limited to 50 ms, by Amazon requirements. */
    msec = (xTaskGetTickCount() - s_lastTopicPublishTick[topic]) * portTICK_PERIOD_MS;

    if (msec < 50)
    {
        vTaskDelay(portTICK_PERIOD_MS * (50 - msec));
    }
    s_lastTopicPublishTick[topic] = xTaskGetTickCount();

    AIS_MakeTopic(handle, topic, topicBuffer);

    jsonStr = cJSON_PrintUnformatted(json);

    if (encrypt)
    {
        /* Encrypted messages have a common header and data encrypted. */
        header = (commonHeader_t *)handle->msgJsonBuffer;
        memset(header, 0, sizeof(commonHeader_t));
        header->sequence = handle->topicSequence[topic]++;

#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))
        header->encryptedSequence = header->sequence;
#endif
        dataOffset = (sizeof(commonHeader_t) - sizeof(plainTextHeader_t));
        dataBuffer = (char *)handle->msgJsonBuffer + sizeof(plainTextHeader_t);
        dataLength = (uint32_t)strlen(jsonStr) + sizeof(commonHeader_t);
    }
    else
    {
        /* Not encrypted messages have no common header. */
        dataBuffer = (char *)handle->msgJsonBuffer;
        dataLength = (uint32_t)strlen(jsonStr);
    }

    /* Copy JSON into databuffer after encrypted sequence number */
    strcpy(dataBuffer + dataOffset, jsonStr);

    if (encrypt)
    {
        /* Create IV data with crypto-secure RNG/hash */
        mbedtls_ctr_drbg_random(&handle->ctr_drbg, header->iv, AIS_CRYPT_IV_LENGTH);

        /* Encrypt payload data for network transmission */
        /* NOTE: input and output buffer can be the same for encryption. */
        AIS_Crypt(handle, handle->topicSecret[topic], (uint8_t *)dataBuffer, (uint8_t *)dataBuffer,
                  dataLength - sizeof(plainTextHeader_t), header->iv, header->mac, AIS_CRYPT_ENCRYPT);
    }

    /* Setup the publish parameters. */
    xPublishParameters.pucTopic      = (const uint8_t *)topicBuffer;
    xPublishParameters.usTopicLength = (uint16_t)strlen(topicBuffer);
    xPublishParameters.pvData        = handle->msgJsonBuffer;
    xPublishParameters.ulDataLength  = dataLength;
    xPublishParameters.xQoS          = eMQTTQoS0;

    if (APP_MQTT_Getv2Handle())
    {
        xReturned = MQTT_AGENT_Publish(APP_MQTT_GetHandle(), &xPublishParameters, AIS_MQTT_TIMEOUT);
    }
    else
    {
        xReturned = eMQTTAgentFailure;
    }

    if (xReturned != eMQTTAgentSuccess)
    {
        configPRINTF(("[AIS ERR] event publish error on topic %s, seq: %d\r\n", _ais_map_topic(topic),
                      header ? header->sequence : handle->topicSequence[topic]));
        ret = kStatus_Timeout;
    }
    else
    {
        configPRINTF(("Successfully published on topic %s, seq: %d\r\n", _ais_map_topic(topic),
                      header ? header->sequence : handle->topicSequence[topic]));
        ret = kStatus_Success;
    }

    AIS_CheckForSecretRotate(handle, topic);

    cJSON_Delete(json);
    vPortFree(jsonStr);

    return ret;
}

/*! @brief Subscribe to MQTT topic */
status_t AIS_Subscribe(ais_handle_t *handle, aisTopic_t topic, void *callback, void *callbackContext)
{
    MQTTAgentReturnCode_t xReturned;
    MQTTAgentSubscribeParams_t xSubscribeParams;
    char topicBuffer[AIS_TOPIC_MAX_LENGTH];
    uint8_t retries;

    AIS_MakeTopic(handle, topic, topicBuffer);

    /* Setup subscribe parameters to subscribe to 'subscribeTopic' topic. */
    xSubscribeParams.pucTopic      = (const uint8_t *)topicBuffer;
    xSubscribeParams.usTopicLength = (uint16_t)strlen(topicBuffer);

    xSubscribeParams.pvPublishCallbackContext = callbackContext;
    xSubscribeParams.pxPublishCallback        = (MQTTPublishCallback_t)callback;
    xSubscribeParams.xQoS                     = eMQTTQoS1;

    for (retries = 0; retries < AIS_MQTT_NUM_RETRIES; retries++)
    {
        if (APP_MQTT_Getv2Handle())
        {
            xReturned = MQTT_AGENT_Subscribe(APP_MQTT_GetHandle(), &xSubscribeParams, AIS_MQTT_TIMEOUT);
        }
        else
        {
            xReturned = eMQTTAgentFailure;
        }

        if (xReturned != eMQTTAgentSuccess)
        {
            configPRINTF(("[AIS WARN] subscribe timeout\r\n"));
        }
        else
        {
            configPRINTF(("[AIS] subscribed: %s\r\n", topicBuffer));
            return kStatus_Success;
        }
    }

    configPRINTF(("[AIS ERR] topic subscribe failed: %s\r\n", topicBuffer));

    return kStatus_Timeout;
}

/*! @brief Unsubscribe from MQTT topic */
status_t AIS_Unsubscribe(ais_handle_t *handle, aisTopic_t topic)
{
    MQTTAgentReturnCode_t xReturned;
    MQTTAgentUnsubscribeParams_t params;
    char topicBuffer[AIS_TOPIC_MAX_LENGTH];

    AIS_MakeTopic(handle, topic, topicBuffer);

    params.pucTopic      = (const uint8_t *)topicBuffer;
    params.usTopicLength = (uint16_t)strlen(topicBuffer);

    if (APP_MQTT_Getv2Handle())
    {
        xReturned = MQTT_AGENT_Unsubscribe(APP_MQTT_GetHandle(), &params, AIS_MQTT_TIMEOUT);
    }
    else
    {
        xReturned = eMQTTAgentFailure;
    }

    if (xReturned != eMQTTAgentSuccess)
    {
        configPRINTF(("[AIS ERR] topic unsubscribe failed: %s\r\n", topicBuffer));
        return kStatus_Timeout;
    }
    else
    {
        configPRINTF(("[AIS] unsubscribed: %s\r\n", topicBuffer));
    }

    return kStatus_Success;
}

static void AIS_QueueSequence(
    ais_handle_t *handle, aisTopic_t topic, const char *data, uint32_t sequence, uint32_t length)
{
    ais_seq_buf_t *list;
    SemaphoreHandle_t mutex;
    uint8_t seqBufSize = 1;

    if (topic == AIS_TOPIC_DIRECTIVE)
    {
        list  = &handle->seqListDirective[0];
        mutex = handle->seqMutexDirective;
    }
    else if (topic == AIS_TOPIC_SPEAKER)
    {
        list  = &handle->seqListSpeaker[0];
        mutex = handle->seqMutexSpeaker;
    }
    else
    {
        /* Invalid topic received */
        return;
    }

    xSemaphoreTake(mutex, portMAX_DELAY);

    /* Add one here for the pending sequence we want to add */
    seqBufSize += AIS_SeqBuffer_Size(list);

    /* Only add onto the queue if we are not in overrun or if in overrun, avoid filling the queue as we are going to
     * receive the packets again */
    if ((appData.overrunSequence == 0) ||
        ((appData.overrunSequence != 0) && (seqBufSize < (AIS_SEQUENCE_BUFFER_SLOTS / 2))))
    {
        if (seqBufSize >= AIS_SEQUENCE_BUFFER_SLOTS)
        {
            configPRINTF(("[AIS ERR] Invalid directive sequencing\r\n"));

            for (int i = 0; i < AIS_SEQUENCE_BUFFER_SLOTS; i++)
            {
                AIS_SeqBuffer_Pop(list, topic);
            }
        }
        else if (!AIS_SeqBuffer_HasSeq(list, sequence))
        {
            ais_seq_buf_t packet;

            configPRINTF(("[AIS] Queue Size %d Sequence %d queued for topic %s\r\n", seqBufSize, sequence,
                          _ais_map_topic(topic)));

            packet.data = (void *)data;
            packet.size = length;
            packet.seq  = sequence;

            if (packet.data)
            {
                AIS_SeqBuffer_Insert(list, &packet, topic);
            }
        }
    }

    xSemaphoreGive(mutex);
}

/*! @brief Map string AIS state value to internal enum */
static ais_state_t _ais_map_state(const char *value)
{
    if (strcasecmp(value, AIS_EVENT_STATE_IDLE) == 0)
        return AIS_STATE_IDLE;
    else if (strcasecmp(value, AIS_EVENT_STATE_THINKING) == 0)
        return AIS_STATE_THINKING;
    else if (strcasecmp(value, AIS_EVENT_STATE_SPEAKING) == 0)
        return AIS_STATE_SPEAKING;
    else if (strcasecmp(value, AIS_EVENT_STATE_ALERTING) == 0)
        return AIS_STATE_ALERTING;
    else if (strcasecmp(value, AIS_EVENT_STATE_DO_NOT_DISTURB) == 0)
        return AIS_STATE_DO_NOT_DISTURB;
    else if (strcasecmp(value, AIS_EVENT_STATE_NOTIFICATION) == 0)
        return AIS_STATE_NOTIFICATION;
    else
        return AIS_STATE_INVALID;
}

/*! @brief Map string AIS alarm type value to internal enum */
static ais_alert_type_t _ais_map_alert_type(const char *value)
{
    if (strcasecmp(value, AIS_EVENT_ALERT_TIMER) == 0)
        return AIS_ALERT_TYPE_TIMER;
    else if (strcasecmp(value, AIS_EVENT_ALERT_ALARM) == 0)
        return AIS_ALERT_TYPE_ALARM;
    else if (strcasecmp(value, AIS_EVENT_ALERT_REMINDER) == 0)
        return AIS_ALERT_TYPE_REMINDER;
    else
        return AIS_ALERT_TYPE_INVALID;
}

/* Return the number of bytes of actual audio data in a speaker sequence.
 * This will be used to determine if the audio buffer is large enough or the
 * message should be dropped. */
static uint32_t _getSpeakerSize(const char *data, uint32_t length)
{
    binaryAudioStream_t *stream;
    uint32_t index = 0, audioDataSize = 0;

    while (index < length)
    {
        stream = (binaryAudioStream_t *)((uint8_t *)data + index);
        if (stream->binaryHeader.type == 0)
        {
            /* Shift the offset to start processing the next frame */
            audioDataSize += (stream->binaryHeader.length - sizeof(stream->audioData.offset));
            /* Add the size of all the OPUS header packets */
            audioDataSize += (stream->binaryHeader.count + 1) * STREAMER_PCM_OPUS_HEADER_SIZE;
        }

        index += stream->binaryHeader.length + sizeof(binaryHeader_t);
    }

    return audioDataSize;
}

bool AIS_ProcessSpeaker(ais_handle_t *handle, const char *data, uint32_t length)
{
    binaryAudioStream_t *stream;
    uint32_t index, marker, audioDataSize;

    audioDataSize = _getSpeakerSize(data, length);
    if (audioDataSize >= AIS_AppCallback_SpeakerAvailableBuffer(handle))
    {
        /* Don't process this message since there isn't enough room to buffer
         * the audio data.  It will be added to the queue and checked again. */
        return false;
    }

    index = 0;

    /* Loop for all packets in the speaker message. These can contain marker or
     * OPUS frames depending on the type.
     */
    while (index < length)
    {
        stream = (binaryAudioStream_t *)((uint8_t *)data + index);

        /* Check header 'type' flag:
         * 1 is speaker metatdata
         * 0 is speaker audio binary data */

        if (stream->binaryHeader.type == 1)
        {
            marker = ((binaryMarker_t *)stream)->marker;
            AIS_AppCallback_SpeakerMarker(handle, marker);
        }
        else if (stream->binaryHeader.type == 0)
        {
#if (!defined(AIS_SPEC_REV_325) || (AIS_SPEC_REV_325 == 0))
            stream->binaryHeader.count = 0;
#endif
            /* Let the callback decide how to parse a speaker packet */
            AIS_AppCallback_Speaker(handle, (uint8_t *)&stream->audioData.data, stream->binaryHeader.length,
                                    stream->audioData.offset, stream->binaryHeader.count);
        }
        else
        {
            /* TODO: Unknown binary stream type. */
            return true;
        }

        index += stream->binaryHeader.length + sizeof(binaryHeader_t);
    }

    /* Indicate this message was processed successfully and can be discarded. */
    return true;
}

/*! @brief AIS topic callback for /speaker */
MQTTBool_t AIS_CallbackSpeaker(void *pvUserData, const MQTTPublishData_t *const pxPublishParameters)
{
    ais_handle_t *handle = (ais_handle_t *)pvUserData;
    commonHeader_t *header;
    status_t ret        = kStatus_Success;
    uint32_t dataLength = 0;
    uint32_t encLength  = 0;
    uint32_t dataOffset = (sizeof(commonHeader_t) - sizeof(plainTextHeader_t));
    char *dataOut;
    bool queueSeq = false;

    if (handle->pendDisconnect || (reconnection_task_get_state() != kStartState))
    {
        return eMQTTFalse;
    }

    /* Clear out msg crypt buffer before we copy in new data */
    memset(handle->msgCryptBuffer, 0x00, AIS_MQTT_MAX_RX_SIZE);
    memcpy(handle->msgCryptBuffer, pxPublishParameters->pvData, pxPublishParameters->ulDataLength);

    header    = (commonHeader_t *)handle->msgCryptBuffer;
    encLength = pxPublishParameters->ulDataLength - sizeof(plainTextHeader_t);
    dataOut   = handle->msgCryptBuffer + sizeof(plainTextHeader_t);

    configPRINTF(
        ("[AIS] Speaker Message Received. "
         "len: %d, seq: %d, exp: %d, buff: %d\r\n",
         pxPublishParameters->ulDataLength, header->sequence, handle->topicSequence[AIS_TOPIC_SPEAKER],
         STREAMER_GetQueuedNotBlocking(handle->audioPlayer)));

    if (encLength)
    {
        ret = AIS_Crypt(handle, handle->topicSecret[AIS_TOPIC_SPEAKER], (uint8_t *)dataOut, (uint8_t *)dataOut,
                        encLength, header->iv, header->mac, AIS_CRYPT_DECRYPT);
    }

    if (ret != kStatus_Success)
    {
        /* Set disconnect pending on failure due to tag mismatch or input error. */
        configPRINTF(("[AIS] ERROR: Encryption error, Speaker message SQN Exp: %d, data, length: %d, Data pointer: %x",
                      handle->topicSequence[AIS_TOPIC_SPEAKER], dataLength,
                      pxPublishParameters->pvData + sizeof(commonHeader_t)));

        handle->pendDisconnect     = true;
        handle->pendDisconnectCode = AIS_DISCONNECT_ENCRYPTION_ERROR;

        return eMQTTFalse;
    }

#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))

    if (header->encryptedSequence != header->sequence)
    {
        /* Set message tamper error. */
        configPRINTF(
            ("[AIS] ERROR: Message Tamper, ENC SQN: %d, SQN: %d", header->encryptedSequence, header->sequence));
        handle->pendDisconnect     = true;
        handle->pendDisconnectCode = AIS_DISCONNECT_MESSAGE_TAMPERED;

        return eMQTTFalse;
    }
#endif

    /* Move dataOut pointer beyond the encrypted sequence number */
    dataOut += dataOffset;
    dataLength = encLength - dataOffset;

    /* reset overrun sequence */
    if (appData.overrunSequence == header->sequence && appData.overrunSequence != 0)
    {
        configPRINTF(("Overrun sequence received again: %d\r\n", header->sequence));
        appData.overrunSequence = 0;
    }

    /* If a sequence comes in that is further than the max queue size, drop it, too far in the future */
    if (header->sequence > (handle->topicSequence[AIS_TOPIC_SPEAKER] + AIS_SEQUENCE_BUFFER_SLOTS))
    {
        configPRINTF(("[AIS WARN] Sequence not in processing range, ignoring: %d\r\n", header->sequence));
        return eMQTTFalse;
    }

    if (header->sequence < handle->topicSequence[AIS_TOPIC_SPEAKER])
    {
        /* We've already processed this sequence - drop it. */
        configPRINTF(("[AIS WARN] Duplicate sequence: %d\r\n", header->sequence));
        return eMQTTFalse;
    }

    if (appData.overrunSequence == 0)
    {
        /* Check to see is message is in sequence */
        if (header->sequence > handle->topicSequence[AIS_TOPIC_SPEAKER])
        {
            configPRINTF(("[AIS WARN] Out of sequence speaker message: %d\r\n", header->sequence));
            queueSeq = true;
        }
        else if (AIS_ProcessSpeaker(handle, dataOut, dataLength))
        {
            /* Sequence sent to the speaker for processing */
            handle->topicSequence[AIS_TOPIC_SPEAKER]++;

            /* Reset timer for next sequence if streaming hasn't stopped */
            if (handle->seqTimerSpeaker != 0)
            {
                handle->seqTimerSpeaker = xTaskGetTickCount();
            }

            AIS_CheckForSecretRotate(handle, AIS_TOPIC_SPEAKER);
        }
        else
        {
            /* Not enough audio buffer */
            AIS_AppCallback_SpeakerOverflow(handle, header->sequence);
        }
    }

    if (queueSeq)
    {
        AIS_QueueSequence(handle, AIS_TOPIC_SPEAKER, dataOut, header->sequence, dataLength);
    }

    /* Return false to indicate we do not take ownership of the MQTT buffer.
     * This will force the MQTT AGENT code to free the buffer back to the pool. */
    return eMQTTFalse;
}

static void AIS_ProcessDirectiveJSON(ais_handle_t *handle, cJSON *directive)
{
    cJSON *header, *name, *payload, *value;

    header = cJSON_GetObjectItemCaseSensitive(directive, "header");
    name   = cJSON_GetObjectItemCaseSensitive(header, "name");

    if (!cJSON_IsString(name) || !name->valuestring)
    {
        /* TODO: send ExceptionEncountered to AIS for malformed message. */

        /* Invalid parsing of name from directive - exit. */
        configPRINTF(("[AIS] INVALID directive received\r\n"));
        return;
    }

    if (strcmp(name->valuestring, "RotateSecret") == 0)
    {
        configPRINTF(("[AIS] RotateSecret received\r\n"));

        cJSON *secret;
        uint32_t directiveSequence, speakerSequence;
        size_t lenServerKey;

        payload           = cJSON_GetObjectItemCaseSensitive(directive, "payload");
        secret            = cJSON_GetObjectItemCaseSensitive(payload, "newSecret");
        value             = cJSON_GetObjectItemCaseSensitive(payload, "directiveSequenceNumber");
        directiveSequence = cJSON_IsNumber(value) ? value->valuedouble : 0;
        value             = cJSON_GetObjectItemCaseSensitive(payload, "speakerSequenceNumber");
        speakerSequence   = cJSON_IsNumber(value) ? value->valuedouble : 0;

        size_t encoded_secret_len = strlen((char *)secret->valuestring);

        mbedtls_base64_decode(handle->config->tempRegistrationConfig.sharedSecret, 16, &lenServerKey,
                              (const unsigned char *)secret->valuestring, encoded_secret_len);

        // memcpy(handle->config->tempRegistrationConfig.sharedSecret, secret->valuestring, AIS_SECRET_LENGTH);
        handle->rotateTopicSequence[AIS_TOPIC_SPEAKER]   = speakerSequence;
        handle->rotateTopicSequence[AIS_TOPIC_DIRECTIVE] = directiveSequence;
        handle->rotateTopicSequence[AIS_TOPIC_MICROPHONE] =
            handle->topicSequence[AIS_TOPIC_MICROPHONE] + AIS_TOPIC_MICROPHONE_ROTATE_SEQUENCE_OFFSET;
        handle->rotateTopicSequence[AIS_TOPIC_EVENT] =
            handle->topicSequence[AIS_TOPIC_EVENT] + AIS_TOPIC_EVENT_ROTATE_SEQUENCE_OFFSET;
        handle->rotateSecret[AIS_TOPIC_SPEAKER]    = true;
        handle->rotateSecret[AIS_TOPIC_DIRECTIVE]  = true;
        handle->rotateSecret[AIS_TOPIC_MICROPHONE] = true;
        handle->rotateSecret[AIS_TOPIC_EVENT]      = true;
        handle->rotateSecretReq                    = true;

        /* Update the persistent-storage version of the secret. */
        memcpy(handle->config->tempRegistrationConfig.awsPartnerRoot, handle->config->registrationConfig.awsPartnerRoot,
               sizeof(handle->config->tempRegistrationConfig.awsPartnerRoot));

        AIS_AppRegistrationInfo(&handle->config->tempRegistrationConfig);

        /* Send +1 for the event since the SecretRotate will use the old secret. */
        AIS_EventSecretRotated(handle, handle->rotateTopicSequence[AIS_TOPIC_EVENT],
                               handle->rotateTopicSequence[AIS_TOPIC_MICROPHONE]);
    }
    else if (strcmp(name->valuestring, "SetAttentionState") == 0)
    {
        configPRINTF(("[AIS] SetAttentionState received\r\n"));

        uint64_t offset = 0;
        bool immediate  = true;

        payload = cJSON_GetObjectItemCaseSensitive(directive, "payload");
        value   = cJSON_GetObjectItemCaseSensitive(payload, "offset");
        if (value)
        {
            offset    = cJSON_IsNumber(value) ? value->valuedouble : 0;
            immediate = false;
        }

        value = cJSON_GetObjectItemCaseSensitive(payload, "state");

        /* If the device is in listening mode, ignore attention states */
        if (!AIS_CheckState(handle, AIS_TASK_STATE_MICROPHONE))
        {
            AIS_AppCallback_SetAttentionState(_ais_map_state(value->valuestring), offset, immediate);
        }
        else
        {
            configPRINTF(("[AIS] Currently in listening mode, ignoring state change\r\n"));
        }
    }
    else if (strcmp(name->valuestring, "OpenSpeaker") == 0)
    {
        configPRINTF(("[AIS] OpenSpeaker received\r\n"));

        uint64_t offset;

        payload = cJSON_GetObjectItemCaseSensitive(directive, "payload");
        value   = cJSON_GetObjectItemCaseSensitive(payload, "offset");
        offset  = cJSON_IsNumber(value) ? value->valuedouble : 0;

        AIS_AppCallback_OpenSpeaker(handle, offset);
        AIS_SetState(handle, AIS_TASK_STATE_SPEAKER);

        /* Start sequence timer for stream buffer */
        handle->seqTimerSpeaker = xTaskGetTickCount();
    }
    else if (strcmp(name->valuestring, "CloseSpeaker") == 0)
    {
        configPRINTF(("[AIS] CloseSpeaker received\r\n"));

        uint64_t offset = 0;
        bool immediate  = true;

        payload = cJSON_GetObjectItemCaseSensitive(directive, "payload");
        value   = cJSON_GetObjectItemCaseSensitive(payload, "offset");
        if (value)
        {
            offset    = cJSON_IsNumber(value) ? value->valuedouble : 0;
            immediate = false;
        }

        /* TODO: Send SpeakerClosed response when playback complete. */

        AIS_AppCallback_CloseSpeaker(handle, offset, immediate);
    }
    else if (strcmp(name->valuestring, "OpenMicrophone") == 0)
    {
        configPRINTF(("[AIS] OpenMicrophone received\r\n"));

        uint32_t timeout;
        cJSON *token, *type;
        char *tokenstr = NULL, *typestr = NULL;

        payload = cJSON_GetObjectItemCaseSensitive(directive, "payload");
        value   = cJSON_GetObjectItemCaseSensitive(payload, "timeoutInMilliseconds");

        /* Assume signed 32-bit int is sufficient for msec timeout. */
        timeout = cJSON_IsNumber(value) ? value->valueint : 0;

        value = cJSON_GetObjectItemCaseSensitive(payload, "initiator");
        if (value)
        {
            type    = cJSON_GetObjectItemCaseSensitive(value, "type");
            payload = cJSON_GetObjectItemCaseSensitive(value, "payload");
            token   = cJSON_GetObjectItemCaseSensitive(payload, "token");

            typestr  = type->valuestring;
            tokenstr = token->valuestring;
        }

        if (!AIS_CheckState(handle, AIS_TASK_STATE_MICROPHONE))
        {
            AIS_AppCallback_OpenMicrophone(timeout, typestr, tokenstr);
        }
    }
    else if (strcmp(name->valuestring, "CloseMicrophone") == 0)
    {
        configPRINTF(("[AIS] CloseMicrophone received\r\n"));

        AIS_ClearState(handle, AIS_TASK_STATE_MICROPHONE);
        AIS_AppCallback_CloseMicrophone(handle);
    }
    else if (strcmp(name->valuestring, "SetVolume") == 0)
    {
        configPRINTF(("[AIS] SetVolume received\r\n"));

        uint32_t volume;
        uint64_t offset = 0;
        bool immediate  = true;

        payload = cJSON_GetObjectItemCaseSensitive(directive, "payload");

        value  = cJSON_GetObjectItemCaseSensitive(payload, "volume");
        volume = cJSON_IsNumber(value) ? value->valueint : 0;
        value  = cJSON_GetObjectItemCaseSensitive(payload, "offset");
        if (value)
        {
            offset    = cJSON_IsNumber(value) ? value->valuedouble : 0;
            immediate = false;
        }

        AIS_AppCallback_SetVolume(handle, volume, offset, immediate);
    }
    else if (strcmp(name->valuestring, "SetAlertVolume") == 0)
    {
        configPRINTF(("[AIS] SetAlertVolume received\r\n"));

        uint32_t volume;

        payload = cJSON_GetObjectItemCaseSensitive(directive, "payload");

        value  = cJSON_GetObjectItemCaseSensitive(payload, "volume");
        volume = cJSON_IsNumber(value) ? value->valueint : 0;

        AIS_AppCallback_SetAlertVolume(volume);
    }
    else if (strcmp(name->valuestring, "SetClock") == 0)
    {
        configPRINTF(("[AIS] SetClock received\r\n"));

        uint64_t time;

        payload = cJSON_GetObjectItemCaseSensitive(directive, "payload");
        value   = cJSON_GetObjectItemCaseSensitive(payload, "currentTime");
        time    = cJSON_IsNumber(value) ? value->valuedouble : 0;

        AIS_AppCallback_SetClock(time);
    }
    else if (strcmp(name->valuestring, "SetAlert") == 0)
    {
        configPRINTF(("[AIS] SetAlert received\r\n"));

        cJSON *token, *type;
        uint32_t duration;
        uint64_t time;

        payload  = cJSON_GetObjectItemCaseSensitive(directive, "payload");
        token    = cJSON_GetObjectItemCaseSensitive(payload, "token");
        value    = cJSON_GetObjectItemCaseSensitive(payload, "scheduledTime");
        time     = cJSON_IsNumber(value) ? value->valuedouble : 0;
        value    = cJSON_GetObjectItemCaseSensitive(payload, "durationInMilliseconds");
        duration = cJSON_IsNumber(value) ? value->valueint : 0;
        type     = cJSON_GetObjectItemCaseSensitive(payload, "type");

        AIS_AppCallback_SetAlert(token->valuestring, time, duration, _ais_map_alert_type(type->valuestring));
    }
    else if (strcmp(name->valuestring, "DeleteAlert") == 0)
    {
        configPRINTF(("[AIS] DeleteAlert received\r\n"));

        payload = cJSON_GetObjectItemCaseSensitive(directive, "payload");
        value   = cJSON_GetObjectItemCaseSensitive(payload, "token");

        AIS_AppCallback_DeleteAlert(value->valuestring);
    }
    else if (strcmp(name->valuestring, "Exception") == 0)
    {
        configPRINTF(("[AIS] Exception received\r\n"));

        cJSON *code, *description;

        /* TODO: parse code, message. */
        /* Transition to internal state based on severity of code. */

        payload     = cJSON_GetObjectItemCaseSensitive(directive, "payload");
        code        = cJSON_GetObjectItemCaseSensitive(payload, "code");
        description = cJSON_GetObjectItemCaseSensitive(payload, "description");

        AIS_AppCallback_Exception(code->valuestring, description->valuestring);
    }
    else
    {
        configPRINTF(("[AIS] UNKNOWN topic received\r\n"));
    }
}

bool AIS_ProcessDirective(ais_handle_t *handle, const char *data, uint32_t length)
{
    cJSON *json, *directive, *directives;

    json = cJSON_Parse(data);
    if (!json)
    {
        /* TODO: reject this and send Exception. */
        return false;
    }

    directives = cJSON_GetObjectItemCaseSensitive(json, "directives");

    /* Process each directive in the JSON, in order. */
    cJSON_ArrayForEach(directive, directives)
    {
        AIS_ProcessDirectiveJSON(handle, directive);
    }

    /* Free heap memory allocated with cJSON_Parse() */
    cJSON_Delete(json);

    /* Indicate this message was processed successfully and can be discarded. */
    return true;
}

/*! @brief AIS topic callback for /directive */
MQTTBool_t AIS_CallbackDirective(void *pvUserData, const MQTTPublishData_t *const pxPublishParameters)
{
    ais_handle_t *handle = (ais_handle_t *)pvUserData;
    commonHeader_t *header;
    status_t ret;
    uint32_t dataLength = 0;
    uint32_t encLength  = 0;
    uint32_t dataOffset = (sizeof(commonHeader_t) - sizeof(plainTextHeader_t));
    char *dataOut;

    /* Clear out msg crypt buffer before we copy in new data */
    memset(handle->msgCryptBuffer, 0x00, AIS_MQTT_MAX_RX_SIZE);
    memcpy(handle->msgCryptBuffer, pxPublishParameters->pvData, pxPublishParameters->ulDataLength);

    header    = (commonHeader_t *)handle->msgCryptBuffer;
    encLength = pxPublishParameters->ulDataLength - sizeof(plainTextHeader_t);
    dataOut   = handle->msgCryptBuffer + sizeof(plainTextHeader_t);

    configPRINTF(("[AIS] directive received, sequence: %d\r\n", header->sequence));

    ret = AIS_Crypt(handle, handle->topicSecret[AIS_TOPIC_DIRECTIVE], (uint8_t *)dataOut, (uint8_t *)dataOut, encLength,
                    header->iv, header->mac, AIS_CRYPT_DECRYPT);

    if (ret != kStatus_Success)
    {
        /* Set disconnect pending on failure due to tag mismatch or input error. */
        handle->pendDisconnect     = true;
        handle->pendDisconnectCode = AIS_DISCONNECT_ENCRYPTION_ERROR;

        return eMQTTFalse;
    }

#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))

    if (header->encryptedSequence != header->sequence)
    {
        /* Set message tamper error. */
        handle->pendDisconnect     = true;
        handle->pendDisconnectCode = AIS_DISCONNECT_MESSAGE_TAMPERED;

        return eMQTTFalse;
    }
#endif

    /* Move dataOut pointer beyond the encrypted sequence number */
    dataOut += dataOffset;
    dataLength = encLength - dataOffset;

    /* NUL-terminate the JSON string. */
    dataOut[dataLength] = '\0';

    /* If a sequence comes in that is further than the max queue size, drop it, too far in the future */
    if (header->sequence > (handle->topicSequence[AIS_TOPIC_DIRECTIVE] + AIS_SEQUENCE_BUFFER_SLOTS))
    {
        configPRINTF(("[AIS WARN] Sequence not in processing range, ignoring: %d\r\n", header->sequence));
        return eMQTTFalse;
    }
    /* Check to see is message is in sequence */
    if (header->sequence > handle->topicSequence[AIS_TOPIC_DIRECTIVE])
    {
        configPRINTF(("[AIS WARN] Out of sequence directive: %d\r\n", header->sequence));

        AIS_QueueSequence(handle, AIS_TOPIC_DIRECTIVE, dataOut, header->sequence, dataLength);
        return eMQTTFalse;
    }
    else if (header->sequence < handle->topicSequence[AIS_TOPIC_DIRECTIVE])
    {
        /* We've already processed this sequence - drop it. */
        configPRINTF(("[AIS WARN] Duplicate sequence: %d\r\n", header->sequence));
        return eMQTTFalse;
    }

    /* Directives are always 'processed' immediately, so ignore response. */
    AIS_ProcessDirective(handle, dataOut, dataLength);

    handle->topicSequence[AIS_TOPIC_DIRECTIVE]++;

    AIS_CheckForSecretRotate(handle, AIS_TOPIC_DIRECTIVE);

    /* Return false to indicate we do not take ownership of the MQTT buffer.
     * This will force the MQTT AGENT code to free the buffer back to the pool. */
    return eMQTTFalse;
}

/*! @brief AIS topic callback for /capabilities */
MQTTBool_t AIS_CallbackCapabilities(void *pvUserData, const MQTTPublishData_t *const pxPublishParameters)
{
    ais_handle_t *handle = (ais_handle_t *)pvUserData;
    cJSON *json, *payload, *code, *desc;
    commonHeader_t *header;
    status_t ret;
    uint32_t dataLength = 0;
    uint32_t encLength  = 0;
    uint32_t dataOffset = (sizeof(commonHeader_t) - sizeof(plainTextHeader_t));
    char *dataOut;

    /* Clear out msg crypt buffer before we copy in new data */
    memset(handle->msgCryptBuffer, 0x00, AIS_MQTT_MAX_RX_SIZE);
    memcpy(handle->msgCryptBuffer, pxPublishParameters->pvData, pxPublishParameters->ulDataLength);

    header    = (commonHeader_t *)handle->msgCryptBuffer;
    encLength = pxPublishParameters->ulDataLength - sizeof(plainTextHeader_t);
    dataOut   = handle->msgCryptBuffer + sizeof(plainTextHeader_t);

    configPRINTF(("[AIS] Capabilities response received\r\n"));

    ret = AIS_Crypt(handle, handle->topicSecret[AIS_TOPIC_CAPABILITIES_ACKNOWLEDGE], (uint8_t *)dataOut,
                    (uint8_t *)dataOut, encLength, header->iv, header->mac, AIS_CRYPT_DECRYPT);

    if (ret != kStatus_Success)
    {
        /* Set disconnect pending on failure due to tag mismatch or input error. */
        handle->pendDisconnect     = true;
        handle->pendDisconnectCode = AIS_DISCONNECT_ENCRYPTION_ERROR;

        return eMQTTFalse;
    }

#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))

    if (header->encryptedSequence != header->sequence)
    {
        /* Set message tamper error. */
        handle->pendDisconnect     = true;
        handle->pendDisconnectCode = AIS_DISCONNECT_MESSAGE_TAMPERED;

        return eMQTTFalse;
    }
#endif

    /* Move dataOut pointer beyond the encrypted sequence number */
    dataOut += dataOffset;
    dataLength = encLength - dataOffset;

    /* NUL-terminate the JSON string. */
    dataOut[dataLength] = '\0';

    json = cJSON_Parse(dataOut);
    if (!json)
    {
        /* TODO: reject this and send Exception. */
    }

    payload = cJSON_GetObjectItemCaseSensitive(json, "payload");
    code    = cJSON_GetObjectItemCaseSensitive(payload, "code");
    desc    = cJSON_GetObjectItemCaseSensitive(payload, "description");

    if (cJSON_IsString(code))
    {
        if (strcmp(code->valuestring, "CAPABILITIES_ACCEPTED") == 0)
        {
            configPRINTF(("[AIS] Capabilities ACCEPTED\r\n"));
            xEventGroupSetBits(handle->aisEvent, AIS_MSG_ACCEPT);
        }
        else if (cJSON_IsString(desc))
        {
            configPRINTF(("[AIS] Capabilities REJECTED: %s, %s\r\n", code->valuestring, desc->valuestring));
            xEventGroupSetBits(handle->aisEvent, AIS_MSG_REJECT);
        }
        else
        {
            configPRINTF(("[AIS] Capabilities REJECTED: %s\r\n", code->valuestring));
            xEventGroupSetBits(handle->aisEvent, AIS_MSG_REJECT);
        }
    }

    /* Free heap memory allocated with cJSON_Parse() */
    cJSON_Delete(json);

    /* Return false to indicate we do not take ownership of the MQTT buffer.
     * This will force the MQTT AGENT code to free the buffer back to the pool. */
    return eMQTTFalse;
}

/*! @brief AIS topic callback for /connection */
MQTTBool_t AIS_CallbackConnect(void *pvUserData, const MQTTPublishData_t *const pxPublishParameters)
{
    ais_handle_t *handle = (ais_handle_t *)pvUserData;
    cJSON *json, *payload, *code, *desc;
    char *data;

    configPRINTF(("[AIS] Connection response received\r\n"));

    data = (char *)pxPublishParameters->pvData;

    json = cJSON_Parse(data);
    if (!json)
    {
        /* TODO: reject this and send Exception. */
    }

    /* Check type of message: acknowledge or disconnect. */
    payload = cJSON_GetObjectItemCaseSensitive(json, "header");
    code    = cJSON_GetObjectItemCaseSensitive(payload, "name");

    if (strcmp(code->valuestring, "Disconnect") == 0)
    {
        if (reconnection_task_set_event(kReconnectAISDisconnect))
        {
            configPRINTF(("[AIS] Service disconnect\r\n"));
            goto connect_exit;
        }
    }

    payload = cJSON_GetObjectItemCaseSensitive(json, "payload");
    code    = cJSON_GetObjectItemCaseSensitive(payload, "code");
    desc    = cJSON_GetObjectItemCaseSensitive(payload, "description");

    if (cJSON_IsString(code))
    {
        if (strcmp(code->valuestring, "CONNECTION_ESTABLISHED") == 0)
        {
            configPRINTF(("[AIS] Connection Established\r\n"));
            xEventGroupSetBits(handle->aisEvent, AIS_MSG_ACCEPT);
        }
        else
        {
            if (cJSON_IsString(desc))
            {
                configPRINTF(("[AIS] Connection Failed: %s, %s\r\n", code->valuestring, desc->valuestring));
            }
            else
            {
                configPRINTF(("[AIS] Connection Failed: %s\r\n", code->valuestring));
            }

            if (strcmp(code->valuestring, "ENCRYPTION_ERROR") == 0)
            {
                AIS_Deregister();
            }
            xEventGroupSetBits(handle->aisEvent, AIS_MSG_REJECT);
        }
    }

connect_exit:
    /* Free heap memory allocated with cJSON_Parse() */
    cJSON_Delete(json);

    /* Return false to indicate we do not take ownership of the MQTT buffer.
     * This will force the MQTT AGENT code to free the buffer back to the pool. */
    return eMQTTFalse;
}

status_t AIS_UpdateTopicSecretPointer(ais_handle_t *handle, aisTopic_t topic, uint8_t *secret)
{
    xSemaphoreTake(s_JsonPublishLocks[topic], portMAX_DELAY);
    handle->topicSecret[topic] = secret;
    xSemaphoreGive(s_JsonPublishLocks[topic]);

    return kStatus_Success;
}

status_t AIS_SendJSONToPublishing(ais_handle_t *handle, aisTopic_t topic, cJSON *json, bool encrypt)
{
    json_publish_t *new_json_publish = NULL, *json_publish_it = NULL;

    /* allocate new json_publish_t structure */
    new_json_publish = pvPortMalloc(sizeof(json_publish_t));

    if (NULL == new_json_publish)
    {
        configPRINTF(("Error allocating json_publish_t structure\r\n"));
        return kStatus_Fail;
    }

    /* populate structure */
    new_json_publish->handle  = handle;
    new_json_publish->topic   = topic;
    new_json_publish->json    = json;
    new_json_publish->encrypt = encrypt;
    new_json_publish->next    = NULL;

    /* protect publishing lists through mutex acquiring */
    xSemaphoreTake(s_JsonPublishLocks[topic], portMAX_DELAY);

    json_publish_it = s_JsonPublishQueues[topic];

    if (NULL == json_publish_it)
    {
        s_JsonPublishQueues[topic] = new_json_publish;
    }
    else
        while (json_publish_it)
        {
            if (json_publish_it->next)
            {
                json_publish_it = json_publish_it->next;
            }
            else
            {
                /* add new item as last element in linked list */
                json_publish_it->next = new_json_publish;
                break;
            }
        }

    xSemaphoreGive(s_JsonPublishLocks[topic]);

    /* signal publishing task */
    xTaskNotifyGive(s_publishTaskHandle);

    return kStatus_Success;
}

void AIS_PublishTask(void *arg)
{
    json_publish_t *json_publish_it = NULL;

    /* allocate resources */
    for (int i = 0; i < AIS_TOPIC_LAST; i++)
    {
        s_JsonPublishLocks[i] = xSemaphoreCreateMutex();

        if (NULL == s_JsonPublishLocks[i])
        {
            configPRINTF(("Error creating mutex\r\n"));
            goto error;
        }
    }

    s_publishTaskHandle = xTaskGetCurrentTaskHandle();

    while (1)
    {
        /* wait to be notified to publish */
        ulTaskNotifyTake(ULONG_MAX, portMAX_DELAY);

        /* publish everything we have in every list */
        for (int i = 0; i < AIS_TOPIC_LAST; i++)
        {
            while (s_JsonPublishQueues[i])
            {
                /* mutual exclusion for each publish list */
                xSemaphoreTake(s_JsonPublishLocks[i], portMAX_DELAY);

                json_publish_it        = s_JsonPublishQueues[i];
                s_JsonPublishQueues[i] = json_publish_it->next;

                xSemaphoreGive(s_JsonPublishLocks[i]);

                AIS_PublishJSON(json_publish_it->handle, json_publish_it->topic, json_publish_it->json,
                                json_publish_it->encrypt);

                vPortFree(json_publish_it);
            }
        }
    }

error:
    /* free resources */
    for (int i = 0; i < AIS_TOPIC_LAST; i++)
    {
        if (s_JsonPublishLocks[i])
        {
            vSemaphoreDelete(s_JsonPublishLocks[i]);
        }
    }

    configPRINTF(("Task failure: exiting\r\n"));
    vTaskDelete(NULL);
}
