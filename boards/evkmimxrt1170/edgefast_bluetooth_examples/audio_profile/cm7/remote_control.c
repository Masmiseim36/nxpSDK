/*
 * FreeRTOS Shadow Demo V1.2.0
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright 2017-2020 NXP
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */
/* Standard includes. */
#include <stdio.h>
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* SHADOW API header. */
#include "shadow.h"

/* JSON library includes. */
#include "core_json.h"

/* shadow demo helpers header. */
#include "mqtt_demo_helpers.h"

#include "platform/iot_threads.h"
#include "types/iot_network_types.h"
#include "aws_demo.h"

#include "queue.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app_music_control.h"



/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_FREERTOS_MSEC_TO_TICK(msec) \
    (((uint32_t)(msec) + 500uL / (uint32_t)configTICK_RATE_HZ) * (uint32_t)configTICK_RATE_HZ / 1000uL)

/** stack size for task that handles shadow delta and updates
 */
#define DEMO_REMOTE_CONTROL_TASK_STACK_SIZE (IOT_THREAD_DEFAULT_STACK_SIZE)

/**
 * @brief Size of the network buffer for MQTT packets.
 */
#define democonfigNETWORK_BUFFER_SIZE (1024U)

#ifndef THING_NAME
/**
 * @brief Predefined thing name.
 *
 * This is the example predefine thing name and could be compiled in ROM code.
 */
#define THING_NAME democonfigCLIENT_IDENTIFIER
#endif

/**
 * @brief The length of #THING_NAME.
 */
#define THING_NAME_LENGTH ((uint16_t)(sizeof(THING_NAME) - 1))

/**
 * @brief The maximum number of times to run the loop in this demo.
 */
#ifndef SHADOW_MAX_DEMO_COUNT
#define SHADOW_MAX_DEMO_COUNT (3)
#endif

/**
 * @brief Time in ticks to wait between each cycle of the demo implemented
 * by RunDeviceShadowDemo().
 */
#define DELAY_BETWEEN_DEMO_ITERATIONS_TICKS (pdMS_TO_TICKS(5000U))

/**
 * @brief The maximum number of times to call MQTT_ProcessLoop() when waiting
 * for a response for Shadow delete operation.
 */
#define MQTT_PROCESS_LOOP_DELETE_RESPONSE_COUNT_MAX (30U)

/**
 * @brief Timeout for MQTT_ProcessLoop in milliseconds.
 */
#define MQTT_PROCESS_LOOP_TIMEOUT_MS (700U)

/**
 * @brief JSON key for response code that indicates the type of error in
 * the error document received on topic `delete/rejected`.
 */
#define SHADOW_DELETE_REJECTED_ERROR_CODE_KEY "code"

/**
 * @brief Length of #SHADOW_DELETE_REJECTED_ERROR_CODE_KEY
 */
#define SHADOW_DELETE_REJECTED_ERROR_CODE_KEY_LENGTH ((uint16_t)(sizeof(SHADOW_DELETE_REJECTED_ERROR_CODE_KEY) - 1))

#define shadowMUSICLIST_LENGTH (SUPPORT_MAX_MUSIC_FILE_COUNT * SUPPORT_MAX_MUSIC_NAME_LENGTH)
#define SHADOW_JSON_BUFFER_LENGTH (210 + shadowMUSICLIST_LENGTH)

/**
 * @brief The MQTT context used for MQTT operation.
 */
static MQTTContext_t xMqttContext;

/**
 * @brief The network context used for SSL operation.
 */
static NetworkContext_t xNetworkContext;

/**
 * @brief Each compilation unit that consumes the NetworkContext must define it.
 * It should contain a single pointer to the type of your desired transport.
 * When using multiple transports in the same compilation unit, define this pointer as void *.
 *
 * @note Transport stacks are defined in
 * amazon-freertos/libraries/abstractions/transport/secure_sockets/transport_secure_sockets.h.
 */
struct NetworkContext
{
    SecureSocketsTransportParams_t *pParams;
};

/**
 * @brief Static buffer used to hold MQTT messages being sent and received.
 */
static uint8_t ucSharedBuffer[democonfigNETWORK_BUFFER_SIZE];

/**
 * @brief Static buffer used to hold MQTT messages being sent and received.
 */
static MQTTFixedBuffer_t xBuffer = {.pBuffer = ucSharedBuffer, .size = democonfigNETWORK_BUFFER_SIZE};

/**
 * @brief Status of the response of Shadow delete operation from AWS IoT
 * message broker.
 */
static BaseType_t xDeleteResponseReceived = pdFALSE;

/**
 * @brief Status of the Shadow delete operation.
 *
 * The Shadow delete status will be updated by the incoming publishes on the
 * MQTT topics for delete acknowledgement from AWS IoT message broker
 * (accepted/rejected). Shadow document is considered to be deleted if an
 * incoming publish is received on `delete/accepted` topic or an incoming
 * publish is received on `delete/rejected` topic with error code 404. Code 404
 * indicates that the Shadow document does not exist for the Thing yet.
 */
static BaseType_t xShadowDeleted = pdFALSE;

static char pcUpdateDocument[SHADOW_JSON_BUFFER_LENGTH];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
QueueHandle_t jsonDeltaQueue = NULL;
static uint32_t g_ParsedPlayIndex;
static uint32_t g_ParsedPlayState;

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief This example uses the MQTT library of the AWS IoT Device SDK for
 * Embedded C. This is the prototype of the callback function defined by
 * that library. It will be invoked whenever the MQTT library receives an
 * incoming message.
 *
 * @param[in] pxMqttContext MQTT context pointer.
 * @param[in] pxPacketInfo Packet Info pointer for the incoming packet.
 * @param[in] pxDeserializedInfo Deserialized information from the incoming packet.
 */
static void prvEventCallback(MQTTContext_t *pxMqttContext,
                             MQTTPacketInfo_t *pxPacketInfo,
                             MQTTDeserializedInfo_t *pxDeserializedInfo);

/**
 * @brief Process payload from /update/delta topic.
 *
 * This handler examines the version number and the powerOn state. If powerOn
 * state has changed, it sets a flag for the main function to take further actions.
 *
 * @param[in] pPublishInfo Deserialized publish info pointer for the incoming
 * packet.
 */
static void prvUpdateDeltaHandler(MQTTPublishInfo_t *pxPublishInfo);

/**
 * @brief Process payload from `/delete/rejected` topic.
 *
 * This handler examines the rejected message to look for the reject reason code.
 * If the reject reason code is `404`, an attempt was made to delete a shadow
 * document which was not present yet. This is considered to be success for this
 * demo application.
 *
 * @param[in] pxPublishInfo Deserialized publish info pointer for the incoming
 * packet.
 */
static void prvDeleteRejectedHandler(MQTTPublishInfo_t *pxPublishInfo);

/**
 * @brief Helper function to wait for a response for Shadow delete operation.
 *
 * @param[in] pxMQTTContext MQTT context pointer.
 *
 * @return pdPASS if successfully received a response for Shadow delete
 * operation; pdFAIL otherwise.
 */
static BaseType_t prvWaitForDeleteResponse(MQTTContext_t *pxMQTTContext);

/* Generate initial shadow document */
static uint32_t prvGenerateShadowJSON()
{
    volatile uint32_t length;
    uint32_t index;
    uint32_t musicCount = music_get_count();

    /* Init shadow document with settings desired and reported state of device. */
    length = snprintf(pcUpdateDocument, SHADOW_JSON_BUFFER_LENGTH,
                    "{"
                    "\"state\":{"
                    "\"desired\":{"
                    "\"playState\":%s,"
                    "\"playIndex\":%d"
                    "},"
                    "\"reported\":{"
                    "\"playState\":%s,"
                    "\"playIndex\":%d,"
                    "\"playerReady\":%s,"
                    "\"musicList\":",
                    music_get_play_state() ? "true" : "false", music_get_play_index(),
                    music_get_play_state() ? "true" : "false", music_get_play_index(), music_get_player_ready() ? "true" : "false");

    pcUpdateDocument[length] = '[';
    length += 1;

    if (musicCount != 0)
    {
        for (index = 0; index < (musicCount - 1); ++index)
        {
            length += snprintf(&pcUpdateDocument[length], SUPPORT_MAX_MUSIC_NAME_LENGTH, "\"%s\",", music_get(index));
        }
        length += snprintf(&pcUpdateDocument[length], SUPPORT_MAX_MUSIC_NAME_LENGTH, "\"%s\"", music_get(index));
    }
    pcUpdateDocument[length] = ']';
    length += 1;

    length += snprintf(&pcUpdateDocument[length], SHADOW_JSON_BUFFER_LENGTH - length,
                "}"
                "},"
                "\"clientToken\": \"token-%d\""
                "}",
                (int)xTaskGetTickCount());
    return length;
}

/* Reports current state to shadow */
static uint32_t prvReportShadowJSON()
{
    volatile uint32_t length;
    uint32_t index;
    uint32_t musicCount = music_get_count();

    length = snprintf(pcUpdateDocument, SHADOW_JSON_BUFFER_LENGTH,
                    "{"
                    "\"state\":{"
                    "\"reported\":{"
                    "\"playState\":%s,"
                    "\"playIndex\":%d,"
                    "\"playerReady\":%s,"
                    "\"musicList\":",
                    music_get_play_state() ? "true" : "false", music_get_play_index(), music_get_player_ready() ? "true" : "false");

    pcUpdateDocument[length] = '[';
    length += 1;
    if (musicCount != 0)
    {
        for (index = 0; index < (musicCount - 1); ++index)
        {
            length += snprintf(&pcUpdateDocument[length], SUPPORT_MAX_MUSIC_NAME_LENGTH, "\"%s\",", music_get(index));
        }
        length += snprintf(&pcUpdateDocument[length], SUPPORT_MAX_MUSIC_NAME_LENGTH, "\"%s\"", music_get(index));
    }
    pcUpdateDocument[length] = ']';
    length += 1;


    length += snprintf(&pcUpdateDocument[length], SHADOW_JSON_BUFFER_LENGTH - length,
                "}"
                "},"
                "\"clientToken\": \"token-%d\""
                "}",
                (int)xTaskGetTickCount());
    return length;
}

static BaseType_t prvWaitForDeleteResponse(MQTTContext_t *pxMQTTContext)
{
    uint8_t ucCount          = 0U;
    MQTTStatus_t xMQTTStatus = MQTTSuccess;
    BaseType_t xReturnStatus = pdPASS;

    assert(pxMQTTContext != NULL);

    while ((xDeleteResponseReceived != pdTRUE) && (ucCount++ < MQTT_PROCESS_LOOP_DELETE_RESPONSE_COUNT_MAX) &&
           (xMQTTStatus == MQTTSuccess))
    {
        /* Event callback will set #xDeleteResponseReceived when receiving an
         * incoming publish on either `delete/accepted` or `delete/rejected`
         * Shadow topics. */
        xMQTTStatus = MQTT_ProcessLoop(pxMQTTContext, MQTT_PROCESS_LOOP_TIMEOUT_MS);
    }

    if ((xMQTTStatus != MQTTSuccess) || (xDeleteResponseReceived != pdTRUE))
    {
        LogError(
            ("MQTT_ProcessLoop failed to receive a response for Shadow delete operation:"
             " LoopDuration=%u, MQTT Status=%s.",
             (MQTT_PROCESS_LOOP_TIMEOUT_MS * ucCount), MQTT_Status_strerror(xMQTTStatus)));
        xReturnStatus = pdFAIL;
    }

    return xReturnStatus;
}

static void prvDeleteRejectedHandler(MQTTPublishInfo_t *pxPublishInfo)
{
    JSONStatus_t result       = JSONSuccess;
    char *pcOutValue          = NULL;
    uint32_t ulOutValueLength = 0UL;
    uint32_t ulErrorCode      = 0UL;

    assert(pxPublishInfo != NULL);
    assert(pxPublishInfo->pPayload != NULL);

    LogInfo(("/delete/rejected json payload:%s.", (const char *)pxPublishInfo->pPayload));

    /* The payload will look similar to this:
     * {
     *    "code": error-code,
     *    "message": "error-message",
     *    "timestamp": timestamp,
     *    "clientToken": "token"
     * }
     */

    /* Make sure the payload is a valid json document. */
    result = JSON_Validate(pxPublishInfo->pPayload, pxPublishInfo->payloadLength);

    if (result == JSONSuccess)
    {
        /* Then we start to get the version value by JSON keyword "version". */
        result = JSON_Search((char *)pxPublishInfo->pPayload, pxPublishInfo->payloadLength,
                             SHADOW_DELETE_REJECTED_ERROR_CODE_KEY, SHADOW_DELETE_REJECTED_ERROR_CODE_KEY_LENGTH,
                             &pcOutValue, (size_t *)&ulOutValueLength);
    }
    else
    {
        LogError(("The json document is invalid!!"));
    }

    if (result == JSONSuccess)
    {
        LogInfo(("Error code is: %.*s.", ulOutValueLength, pcOutValue));

        /* Convert the extracted value to an unsigned integer value. */
        ulErrorCode = (uint32_t)strtoul(pcOutValue, NULL, 10);
    }
    else
    {
        LogError(("No error code in json document!!"));
    }

    LogInfo(("Error code:%lu.", ulErrorCode));

    /* Mark Shadow delete operation as a success if error code is 404. */
    if (ulErrorCode == 404)
    {
        xShadowDeleted = pdTRUE;
    }
}

static void prvUpdateDeltaHandler(MQTTPublishInfo_t *pxPublishInfo)
{
    static uint32_t ulCurrentVersion = 0; /* Remember the latestVersion # we've ever received */
    uint32_t ulVersion               = 0U;
    char *pcOutValue                 = NULL;
    uint32_t ulOutValueLength        = 0U;
    JSONStatus_t result              = JSONSuccess;
    uint8_t frowAWS;

    assert(pxPublishInfo != NULL);
    assert(pxPublishInfo->pPayload != NULL);

    LogInfo(("/update/delta json payload:%s.", (const char *)pxPublishInfo->pPayload));

    /* Make sure the payload is a valid json document. */
    result = JSON_Validate(pxPublishInfo->pPayload, pxPublishInfo->payloadLength);

    if (result == JSONSuccess)
    {
        /* Then we start to get the version value by JSON keyword "version". */
        result = JSON_Search((char *)pxPublishInfo->pPayload, pxPublishInfo->payloadLength, "version",
                             sizeof("version") - 1, &pcOutValue, (size_t *)&ulOutValueLength);
    }
    else
    {
        LogError(("The json document is invalid!!"));
    }

    if (result == JSONSuccess)
    {
        LogInfo(("version: %.*s", ulOutValueLength, pcOutValue));

        /* Convert the extracted value to an unsigned integer value. */
        ulVersion = (uint32_t)strtoul(pcOutValue, NULL, 10);
    }
    else
    {
        LogError(("No version in json document!!"));
    }

    LogInfo(("version:%d, ulCurrentVersion:%d \r\n", ulVersion, ulCurrentVersion));

    /* When the version is much newer than the on we retained, that means the LEDstate
     * state is valid for us. */
    if (ulVersion > ulCurrentVersion)
    {
        /* Set to received version as the current version. */
        ulCurrentVersion = ulVersion;

        g_ParsedPlayState = 0xFFFFFFFFu;
        g_ParsedPlayIndex = 0xFFFFFFFFu;

        result = JSON_Search((char *)pxPublishInfo->pPayload, pxPublishInfo->payloadLength, "state.playState",
                             sizeof("state.playState") - 1, &pcOutValue, (size_t *)&ulOutValueLength);
        if (result == JSONSuccess)
        {
            if ((pcOutValue[0] == 't') &&
                (pcOutValue[1] == 'r') &&
                (pcOutValue[2] == 'u') &&
                (pcOutValue[3] == 'e'))
            {
                g_ParsedPlayState = 1;
            }
            else
            {
                g_ParsedPlayState = 0;
            }
        }

        result = JSON_Search((char *)pxPublishInfo->pPayload, pxPublishInfo->payloadLength, "state.playIndex",
                             sizeof("state.playIndex") - 1, &pcOutValue, (size_t *)&ulOutValueLength);
        if (result == JSONSuccess)
        {
            /* Convert the LEDstate state value to an unsigned integer value. */
            g_ParsedPlayIndex = (uint32_t)strtoul(pcOutValue, NULL, 10);
        }

        frowAWS = 1;
        if (xQueueSend(jsonDeltaQueue, &frowAWS, (TickType_t)10) != pdPASS)
        {
            LogWarn(("Fail to send message to jsonDeltaQueue.\r\n"));
        }
    }
    else
    {
        /* In this demo, we discard the incoming message
         * if the version number is not newer than the latest
         * that we've received before. Your application may use a
         * different approach.
         */
        LogWarn(("The received version is smaller than current one!!"));
    }
}

/* This is the callback function invoked by the MQTT stack when it receives
 * incoming messages. This function demonstrates how to use the Shadow_MatchTopic
 * function to determine whether the incoming message is a device shadow message
 * or not. If it is, it handles the message depending on the message type.
 */
static void prvEventCallback(MQTTContext_t *pxMqttContext,
                             MQTTPacketInfo_t *pxPacketInfo,
                             MQTTDeserializedInfo_t *pxDeserializedInfo)
{
    ShadowMessageType_t messageType = ShadowMessageTypeMaxNum;
    const char *pcThingName         = NULL;
    uint16_t usThingNameLength      = 0U;
    uint16_t usPacketIdentifier;

    (void)pxMqttContext;

    assert(pxDeserializedInfo != NULL);
    assert(pxMqttContext != NULL);
    assert(pxPacketInfo != NULL);

    usPacketIdentifier = pxDeserializedInfo->packetIdentifier;

    /* Handle incoming publish. The lower 4 bits of the publish packet
     * type is used for the dup, QoS, and retain flags. Hence masking
     * out the lower bits to check if the packet is publish. */
    if ((pxPacketInfo->type & 0xF0U) == MQTT_PACKET_TYPE_PUBLISH)
    {
        assert(pxDeserializedInfo->pPublishInfo != NULL);
        LogInfo(("pPublishInfo->pTopicName:%s.", pxDeserializedInfo->pPublishInfo->pTopicName));

        /* Let the Device Shadow library tell us whether this is a device shadow message. */
        if (SHADOW_SUCCESS == Shadow_MatchTopic(pxDeserializedInfo->pPublishInfo->pTopicName,
                                                pxDeserializedInfo->pPublishInfo->topicNameLength, &messageType,
                                                &pcThingName, &usThingNameLength))
        {
            /* Upon successful return, the messageType has been filled in. */
            if (messageType == ShadowMessageTypeUpdateDelta)
            {
                /* Handler function to process payload. */
                prvUpdateDeltaHandler(pxDeserializedInfo->pPublishInfo);
            }
            else if (messageType == ShadowMessageTypeUpdateAccepted)
            {
                /* Handler function to process payload. */
                // prvUpdateAcceptedHandler( pxDeserializedInfo->pPublishInfo );
            }
            else if (messageType == ShadowMessageTypeUpdateDocuments)
            {
                LogInfo(
                    ("/update/documents json payload:%s.", (const char *)pxDeserializedInfo->pPublishInfo->pPayload));
            }
            else if (messageType == ShadowMessageTypeUpdateRejected)
            {
                LogInfo(
                    ("/update/rejected json payload:%s.", (const char *)pxDeserializedInfo->pPublishInfo->pPayload));
            }
            else if (messageType == ShadowMessageTypeDeleteAccepted)
            {
                LogInfo(("Received an MQTT incoming publish on /delete/accepted topic."));
                xShadowDeleted          = pdTRUE;
                xDeleteResponseReceived = pdTRUE;
            }
            else if (messageType == ShadowMessageTypeDeleteRejected)
            {
                /* Handler function to process payload. */
                prvDeleteRejectedHandler(pxDeserializedInfo->pPublishInfo);
                xDeleteResponseReceived = pdTRUE;
            }
            else
            {
                LogInfo(("Other message type:%d !!", messageType));
            }
        }
        else
        {
            LogError(
                ("Shadow_MatchTopic parse failed:%s !!", (const char *)pxDeserializedInfo->pPublishInfo->pTopicName));
        }
    }
    else
    {
        vHandleOtherIncomingPacket(pxPacketInfo, usPacketIdentifier);
    }
}

void prvShadowInitState(void)
{
    g_ParsedPlayIndex = 0xFFFFFFFFu;
    g_ParsedPlayState = 0xFFFFFFFFu;
}

void app_music_shadow_update(uint8_t playIndexSet, uint8_t playStateSet, uint8_t playerReadySet)
{
    uint8_t fromAWS = 0U;

    if (jsonDeltaQueue == NULL)
    {
        return;
    }

    if (xQueueSend(jsonDeltaQueue, &fromAWS, (TickType_t)10) != pdPASS)
    {
        configPRINTF(("Fail to send message to jsonDeltaQueue.\r\n"));
        /* return pdFALSE - don't take ownership */
        return;
    }

    /* return pdTRUE - take ownership of the mqtt buffer - must be returned by SHADOW_ReturnMQTTBuffer */
    return;
}

/**
 * @brief Entry point of remote control demo.
 *
 * This main function demonstrates how to use the macros provided by the
 * Device Shadow library to assemble strings for the MQTT topics defined
 * by AWS IoT Device Shadow. It uses these macros for topics to subscribe
 * to:
 * - SHADOW_TOPIC_STRING_UPDATE_DELTA for "$aws/things/thingName/shadow/update/delta"
 *
 * It also uses these macros for topics to publish to:
 * - SHADOW_TOPIC_STRING_DELETE for "$aws/things/thingName/shadow/delete"
 * - SHADOW_TOPIC_STRING_UPDATE for "$aws/things/thingName/shadow/update"
 *
 * The helper functions this demo uses for MQTT operations have internal
 * loops to process incoming messages. Those are not the focus of this demo
 * and therefor, are placed in a separate file shadow_demo_helpers.c.
 */
int RunRemoteControlDemo(bool awsIotMqttMode,
                                const char *pIdentifier,
                                void *pNetworkServerInfo,
                                void *pNetworkCredentialInfo,
                                const IotNetworkInterface_t *pNetworkInterface)
{
    BaseType_t xDemoStatus   = pdPASS;
    BaseType_t xDemoRunCount = 0UL;
    size_t jsonLength        = 0;
    uint8_t fromAWS;

    /* Remove compiler warnings about unused parameters. */
    (void)awsIotMqttMode;
    (void)pIdentifier;
    (void)pNetworkServerInfo;
    (void)pNetworkCredentialInfo;
    (void)pNetworkInterface;

    prvShadowInitState();
    jsonDeltaQueue = xQueueCreate(8, sizeof(uint8_t));
    if (jsonDeltaQueue == NULL)
    {
        configPRINTF(("Failed to create jsonDeltaQueue queue.\r\n"));
        vTaskDelete(NULL);
    }

    do
    {
        xDemoStatus = EstablishMqttSession(&xMqttContext, &xNetworkContext, &xBuffer, prvEventCallback);
        if (xDemoStatus == pdFAIL)
        {
            /* Log error to indicate connection failure. */
            LogError(("Failed to connect to MQTT broker."));
        }
        else
        {
            /* Reset the shadow delete status flags. */
            xDeleteResponseReceived = pdFALSE;
            xShadowDeleted          = pdFALSE;

            /* First of all, try to delete any Shadow document in the cloud.
             * Try to subscribe to `delete/accepted` and `delete/rejected` topics. */
            xDemoStatus = SubscribeToTopic(&xMqttContext, SHADOW_TOPIC_STRING_DELETE_ACCEPTED(THING_NAME),
                                           SHADOW_TOPIC_LENGTH_DELETE_ACCEPTED(THING_NAME_LENGTH));

            if (xDemoStatus == pdPASS)
            {
                /* Try to subscribe to `delete/rejected` topic. */
                xDemoStatus = SubscribeToTopic(&xMqttContext, SHADOW_TOPIC_STRING_DELETE_REJECTED(THING_NAME),
                                               SHADOW_TOPIC_LENGTH_DELETE_REJECTED(THING_NAME_LENGTH));
            }

            if (xDemoStatus == pdPASS)
            {
                /* Publish to Shadow `delete` topic to attempt to delete the
                 * Shadow document if exists. */
                xDemoStatus = PublishToTopic(&xMqttContext, SHADOW_TOPIC_STRING_DELETE(THING_NAME),
                                             SHADOW_TOPIC_LENGTH_DELETE(THING_NAME_LENGTH), pcUpdateDocument, 0U);
            }

            /* Wait for an incoming publish on `delete/accepted` or `delete/rejected`
             * topics, if not already received a publish. */
            if ((xDemoStatus == pdPASS) && (xDeleteResponseReceived != pdTRUE))
            {
                xDemoStatus = prvWaitForDeleteResponse(&xMqttContext);
            }

            /* Unsubscribe from the `delete/accepted` and 'delete/rejected` topics.*/
            if (xDemoStatus == pdPASS)
            {
                xDemoStatus = UnsubscribeFromTopic(&xMqttContext, SHADOW_TOPIC_STRING_DELETE_ACCEPTED(THING_NAME),
                                                   SHADOW_TOPIC_LENGTH_DELETE_ACCEPTED(THING_NAME_LENGTH));
            }

            if (xDemoStatus == pdPASS)
            {
                xDemoStatus = UnsubscribeFromTopic(&xMqttContext, SHADOW_TOPIC_STRING_DELETE_REJECTED(THING_NAME),
                                                   SHADOW_TOPIC_LENGTH_DELETE_REJECTED(THING_NAME_LENGTH));
            }

            /* Check if Shadow document delete was successful. A delete can be
             * successful in cases listed below.
             *  1. If an incoming publish is received on `delete/accepted` topic.
             *  2. If an incoming publish is received on `delete/rejected` topic
             *     with error code 404. This indicates that a Shadow document was
             *     not present for the Thing. */
            if (xShadowDeleted == pdFALSE)
            {
                LogError(("Shadow delete operation failed."));
                xDemoStatus = pdFAIL;
            }

            /* Then try to subscribe Shadow delta and Shadow updated topics. */
            if (xDemoStatus == pdPASS)
            {
                xDemoStatus = SubscribeToTopic(&xMqttContext, SHADOW_TOPIC_STRING_UPDATE_DELTA(THING_NAME),
                                               SHADOW_TOPIC_LENGTH_UPDATE_DELTA(THING_NAME_LENGTH));
            }

            /* Then we publish a desired state to the /update topic. Since we've deleted
             * the device shadow at the beginning of the demo, this will cause a delta message
             * to be published, which we have subscribed to.
             */
            if (xDemoStatus == pdPASS)
            {
                (void)memset(pcUpdateDocument, 0x00, sizeof(pcUpdateDocument));

                jsonLength = prvGenerateShadowJSON();

                xDemoStatus =
                    PublishToTopic(&xMqttContext, SHADOW_TOPIC_STRING_UPDATE(THING_NAME),
                                   SHADOW_TOPIC_LENGTH_UPDATE(THING_NAME_LENGTH), pcUpdateDocument, jsonLength);
            }

            if (xDemoStatus == pdPASS)
            {
                LogInfo(("AWS Remote Control Demo initialized."));
                LogInfo(("Use mobile application to control the remote device."));

                prvShadowInitState();
                music_play_aws_ready();
                while (true)
                {
                    static BaseType_t wait_result;
                    wait_result = xQueueReceive(jsonDeltaQueue, &fromAWS, APP_FREERTOS_MSEC_TO_TICK(100));
                    {
                        xDemoStatus = ProcessLoop(&xMqttContext, 10);
                        if (xDemoStatus == pdPASS)
                        {
                            if ((g_ParsedPlayState != 0xFFFFFFFFu) && (g_ParsedPlayState != music_get_play_state()))
                            {
                                if (g_ParsedPlayState)
                                {
                                    PRINTF("start play\r\n");
                                    music_play();
                                }
                                else
                                {
                                    PRINTF("stop play\r\n");
                                    //music_stop();
                                    music_pause();
                                }
                            }

                            if ((g_ParsedPlayIndex != 0xFFFFFFFFu) && (g_ParsedPlayIndex != music_get_play_index()))
                            {
                                PRINTF("start play %d\r\n", g_ParsedPlayIndex);
                                music_play_index(g_ParsedPlayIndex);
                            }

                            if ((g_ParsedPlayState == music_get_play_state()) &&
                                (g_ParsedPlayIndex == music_get_play_index()))
                            {
                                /* update device shadow */
                                jsonLength = prvReportShadowJSON();
                                xDemoStatus = PublishToTopic(&xMqttContext, SHADOW_TOPIC_STRING_UPDATE(THING_NAME),
                                                             SHADOW_TOPIC_LENGTH_UPDATE(THING_NAME_LENGTH),
                                                             pcUpdateDocument, jsonLength);
                                if (xDemoStatus == pdPASS)
                                {
                                    LogInfo(("Successfully performed update."));
                                }
                                else
                                {
                                    LogInfo(("Update failed."));
                                }
                            }
                            g_ParsedPlayState = 0xFFFFFFFFu;
                            g_ParsedPlayIndex = 0xFFFFFFFFu;
                        }
                        else
                        {
                            LogError(("ProcessLoop Failed"));
                            break;
                        }

                        if ((wait_result == pdTRUE) && (!fromAWS))
                        {
                            jsonLength = prvGenerateShadowJSON();

                            xDemoStatus =
                                PublishToTopic(&xMqttContext, SHADOW_TOPIC_STRING_UPDATE(THING_NAME),
                                               SHADOW_TOPIC_LENGTH_UPDATE(THING_NAME_LENGTH), pcUpdateDocument, jsonLength);
                        }
                    }
                }
            }

            /* The MQTT session is always disconnected, even if there were prior failures. */
            xDemoStatus = DisconnectMqttSession(&xMqttContext, &xNetworkContext);
        }

        /* Increment the demo run count. */
        xDemoRunCount++;

        /* Attempt to retry a failed iteration of demo for up to #SHADOW_MAX_DEMO_COUNT times. */
        if (xDemoRunCount < SHADOW_MAX_DEMO_COUNT)
        {
            LogWarn(("Demo iteration %lu failed. Retrying...", xDemoRunCount));
            vTaskDelay(DELAY_BETWEEN_DEMO_ITERATIONS_TICKS);
        }
        /* Failed all #SHADOW_MAX_DEMO_COUNT demo iterations. */
        else
        {
            LogError(("All %d demo iterations failed.", SHADOW_MAX_DEMO_COUNT));
            break;
        }
    } while (true);

    return EXIT_FAILURE;
}

void vStartMusicPlayerDemoTask(void)
{
    static demoContext_t mqttDemoContext = {.networkTypes                = AWSIOT_NETWORK_TYPE_WIFI,
                                            .demoFunction                = RunRemoteControlDemo,
                                            .networkConnectedCallback    = NULL,
                                            .networkDisconnectedCallback = NULL};

    Iot_CreateDetachedThread(runDemoTask, &mqttDemoContext, tskIDLE_PRIORITY + 3, DEMO_REMOTE_CONTROL_TASK_STACK_SIZE);
}
