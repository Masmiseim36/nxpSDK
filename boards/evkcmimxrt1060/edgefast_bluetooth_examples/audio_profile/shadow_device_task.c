/*
 * FreeRTOS V202012.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright 2022 NXP
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
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * Demo for showing how to use the Device Shadow library's API. This version
 * of the Device Shadow API provides macros and helper functions for assembling MQTT topics
 * strings, and for determining whether an incoming MQTT message is related to the
 * device shadow.
 *
 * This example assumes there is a powerOn state in the device shadow. It does the
 * following operations:
 * 1. Assemble strings for the MQTT topics of device shadow, by using macros defined by the Device Shadow library.
 * 2. Subscribe to those MQTT topics using the MQTT Agent.
 * 3. Register callbacks for incoming shadow topic publishes with the subsciption_manager.
 * 3. Publish to report the current state of powerOn.
 * 5. Check if powerOn has been changed and send an update if so.
 * 6. If a publish to update reported state was sent, wait until either prvUpdateAcceptedCallback
 *    or prvUpdateRejectedCallback handle the response.
 * 7. Wait until time for next check and repeat from step 5.
 *
 * Meanwhile, when prvUpdateDeltaCallback receives changes to the shadow state,
 * it will apply them on the device.
 */

/* Standard includes. */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo config. */
#include "demo_config.h"

/* MQTT library includes. */
#include "core_mqtt_agent.h"

/* JSON library includes. */
#include "core_json.h"

/* Shadow API header. */
#include "shadow.h"

/* API to fetch thingname */
#include "kvstore.h"

/* MQTT Agent task APIs. */
#include "mqtt_agent_task.h"

#include "fsl_debug_console.h"
#include "app_music_control.h"

/**
 * @brief The name of the shadow to be used in the example.
 *
 * AWS IoT Shadow service supports named shadows and unnamed shadow. An unnamed shadow exist for a thing by default when
 * the thing name is registered with AWS IoT. In addition to this, named shadows can be
 * created for a thing to logically group a set of state properties for the device and access them with differently (for
 * example with differentiated policies).
 *
 * By default the example uses an unnamed shadow for updating the state of the device. To use the example with a named shadow,
 * set the configuration below to the name of the shadow instead.
 */
#define shadowexampleSHADOW_NAME           SHADOW_NAME_CLASSIC

/**
 * @brief The length of the shadow name without null termination.
 *
 * By default the example uses an unnamed shadow for updating the state of the device. To use the example with a named shadow,
 * set the configuration below to length of the shadow name without null termination.
 */
#define shadowexampleSHADOW_NAME_LENGTH    SHADOW_NAME_CLASSIC_LENGTH

/**
 * @brief Time in ms to wait between checking for updates to report.
 */
#define shadowexampleMS_BETWEEN_SYNC                   ( 1000U )

/**
 * @brief This demo uses task notifications to signal tasks from MQTT callback
 * functions. shadowexampleMS_TO_WAIT_FOR_NOTIFICATION defines the time, in ticks,
 * to wait for such a callback.
 */
#define shadowexampleMS_TO_WAIT_FOR_NOTIFICATION       ( 10000 )

/**
 * @brief The maximum amount of time in milliseconds to wait for the commands
 * to be posted to the MQTT agent should the MQTT agent's command queue be full.
 * Tasks wait in the Blocked state, so don't use any CPU time.
 */
#define shadowexampleMAX_COMMAND_SEND_BLOCK_TIME_MS    ( 200 )


/**
 * @brief The configuration defines maximum length of thing name supported by AWS IoT.
 */
#define shadowexampleTHINGNAME_MAX_LENGTH    ( 128 )

/**
 * @brief Maximum length of the topic buffer used to get the shadow.
 */
#define shadowexampleTOPIC_LENGTH_GET                        \
    SHADOW_TOPIC_LEN_GET( shadowexampleTHINGNAME_MAX_LENGTH, \
                          shadowexampleSHADOW_NAME_LENGTH )


/**
 * @brief Maximum length of the topic buffer used to receive get accepted notifications
 * from AWS IoT shadow service.
 */
#define shadowexampleTOPIC_LENGTH_GET_ACCEPTED                   \
    SHADOW_TOPIC_LEN_GET_ACC( shadowexampleTHINGNAME_MAX_LENGTH, \
                              shadowexampleSHADOW_NAME_LENGTH )

/**
 * @brief Maximum length of the topic buffer used to receive get rejected notifications
 * from AWS IoT shadow service.
 */
#define shadowexampleTOPIC_LENGTH_GET_REJECTED                   \
    SHADOW_TOPIC_LEN_GET_REJ( shadowexampleTHINGNAME_MAX_LENGTH, \
                              shadowexampleSHADOW_NAME_LENGTH )

/**
 * @brief Maximum length of the topic buffer used to receive delta updates from AWS IoT Shadow service.
 */
#define shadowexampleTOPIC_LENGTH_UPDATE_DELTA                        \
    SHADOW_TOPIC_LEN_UPDATE_DELTA( shadowexampleTHINGNAME_MAX_LENGTH, \
                                   shadowexampleSHADOW_NAME_LENGTH )

/**
 * @brief Maximum length of the topic buffer used to receive update accepted notifications from AWS IoT Shadow service.
 */
#define shadowexampleTOPIC_LENGTH_UPDATE_ACCEPTED                   \
    SHADOW_TOPIC_LEN_UPDATE_ACC( shadowexampleTHINGNAME_MAX_LENGTH, \
                                 shadowexampleSHADOW_NAME_LENGTH )

/**
 * @brief Maximum length of the topic buffer used to receive update rejected notifications from AWS IoT Shadow service.
 */
#define shadowexampleTOPIC_LENGTH_UPDATE_REJECTED                   \
    SHADOW_TOPIC_LEN_UPDATE_REJ( shadowexampleTHINGNAME_MAX_LENGTH, \
                                 shadowexampleSHADOW_NAME_LENGTH )

/**
 * @brief Maximum length of the topic buffer used to report the state of the device to AWS IoT Shadow service.
 */
#define shadowexampleTOPIC_LENGTH_UPDATE                        \
    SHADOW_TOPIC_LEN_UPDATE( shadowexampleTHINGNAME_MAX_LENGTH, \
                             shadowexampleSHADOW_NAME_LENGTH )


#define shadowexampleDEVICE_STATE_OFF         ( ( uint8_t ) 0U )
#define shadowexampleDEVICE_STATE_ON          ( ( uint8_t ) 1U )


#define shadowexampleGET_ACCEPTED_EVENT       ( ( uint32_t ) ( 1UL << 1 ) )
#define shadowexampleGET_REJECTED_EVENT       ( ( uint32_t ) ( 1UL << 2 ) )
#define shadowexampleUPDATE_DELTA_EVENT       ( ( uint32_t ) ( 1UL << 3 ) )
#define shadowexampleUPDATE_ACCEPTED_EVENT    ( ( uint32_t ) ( 1UL << 4 ) )
#define shadowexampleUPDATE_REJECTED_EVENT    ( ( uint32_t ) ( 1UL << 4 ) )


/**
 * @brief Defines the structure to use as the command callback context in this
 * demo.
 */
struct MQTTAgentCommandContext
{
    TaskHandle_t xShadowTaskHandle;
    bool xReturnStatus;
};


extern MQTTAgentContext_t xGlobalMqttAgentContext;

/*-----------------------------------------------------------*/
#if 0
static uint32_t ulCurrentVersion = 0U;
#endif
/**
 * @brief Match the received clientToken with the one sent in a device shadow
 * update. Set to 0 when not waiting on a response.
 */
static uint32_t ulClientToken = 0U;

/**
 * @brief Topic is used to get the shadow from AWS IoT shadow service.
 */
static char cGetTopic[ shadowexampleTOPIC_LENGTH_GET ];
static uint16_t usGetTopicLength = 0U;

/**
 * @brief Topic is used to receive an accepted notification for a GET message from AWS IoT shadow
 * service.
 */
static char cGetAcceptedTopic[ shadowexampleTOPIC_LENGTH_GET_ACCEPTED ];
static uint16_t usGetAcceptedTopicLength = 0U;

/**
 * @brief Topic is used to receive an accepted notification for a GET message from AWS IoT shadow
 * service.
 */
static char cGetRejectedTopic[ shadowexampleTOPIC_LENGTH_GET_REJECTED ];
static uint16_t usGetRejectedTopicLength = 0U;

/**
 * @brief Topic is used to receive delta updates on the device state from AWS IoT shadow
 * service.
 */
static char cUpdateDeltaTopic[ shadowexampleTOPIC_LENGTH_UPDATE_DELTA ];
static uint16_t usUpdateDeltaTopicLength = 0U;

/**
 * @brief Topic is used to receive update accepted notifications from AWS IoT shadow
 * service.
 */
static char cUpdateAcceptedTopic[ shadowexampleTOPIC_LENGTH_UPDATE_ACCEPTED ];
static uint16_t usUpdateAcceptedTopicLength = 0U;

/**
 * @brief Topic is used to receive update rejected notifications from AWS IoT shadow
 * service.
 */
static char cUpdateRejectedTopic[ shadowexampleTOPIC_LENGTH_UPDATE_REJECTED ];
static uint16_t usUpdateRejectedTopicLength = 0U;

/**
 * @brief Topic is used to report the state of device to AWS IoT shadow service.
 */
static char cUpdateTopic[ shadowexampleTOPIC_LENGTH_UPDATE ];
static uint16_t usUpdateTopicLength = 0U;

static char cThingName[ shadowexampleTHINGNAME_MAX_LENGTH + 1U ];
static size_t xThingNameLength = 0U;

/*-----------------------------------------------------------*/

/**
 * @brief Subscribe to the used device shadow topics.
 *
 * @return true if the subscribe is successful;
 * false otherwise.
 */
static bool prvSubscribeToShadowUpdateTopics( void );

/**
 * @brief Passed into MQTTAgent_Subscribe() as the callback to execute when the
 * broker ACKs the SUBSCRIBE message. Its implementation sends a notification
 * to the task that called MQTTAgent_Subscribe() to let the task know the
 * SUBSCRIBE operation completed. It also sets the xReturnStatus of the
 * structure passed in as the command's context to the value of the
 * xReturnStatus parameter - which enables the task to check the status of the
 * operation.
 *
 * See https://freertos.org/mqtt/mqtt-agent-demo.html#example_mqtt_api_call
 *
 * @param[in] pxCommandContext Context of the initial command.
 * @param[in] pxReturnInfo The result of the command.
 */
static void prvSubscribeCommandCallback( MQTTAgentCommandContext_t * pxCommandContext,
                                         MQTTAgentReturnInfo_t * pxReturnInfo );

/**
 * @brief The callback to execute when there is an incoming publish on the
 * topic for delta updates. It verifies the document and sets the
 * powerOn state accordingly.
 *
 * @param[in] pvIncomingPublishCallbackContext Context of the initial command.
 * @param[in] pxPublishInfo Deserialized publish.
 */
static void prvUpdateDeltaCallback( void * pxSubscriptionContext,
                                    MQTTPublishInfo_t * pxPublishInfo );

/**
 * @brief The callback to execute when there is an incoming publish on the
 * topic for accepted requests. It verifies the document is valid and is being waited on.
 * If so it updates the last reported state and notifies the task to inform completion
 * of the update request.
 *
 * @param[in] pvIncomingPublishCallbackContext Context of the initial command.
 * @param[in] pxPublishInfo Deserialized publish.
 */
static void prvUpdateAcceptedCallback( void * pxSubscriptionContext,
                                       MQTTPublishInfo_t * pxPublishInfo );


/**
 * @brief The callback to execute when there is an incoming publish on the
 * topic for rejected requests. It verifies the document is valid and is being waited on.
 * If so it notifies the task to inform completion of the update request.
 *
 * @param[in] pvIncomingPublishCallbackContext Context of the initial command.
 * @param[in] pxPublishInfo Deserialized publish.
 */
static void prvUpdateRejectedCallback( void * pxSubscriptionContext,
                                       MQTTPublishInfo_t * pxPublishInfo );


/**
 * @brief The callback to execute when there is an incoming publish on the
 * topic for rejected requests. It verifies the document is valid and is being waited on.
 * If so it notifies the task to inform completion of the update request.
 *
 * @param[in] pvIncomingPublishCallbackContext Context of the initial command.
 * @param[in] pxPublishInfo Deserialized publish.
 */
static void prvGetAcceptedCallback( void * pxSubscriptionContext,
                                    MQTTPublishInfo_t * pxPublishInfo );



/**
 * @brief The callback to execute when there is an incoming message on the GET rejected topic.
 * The callback varifies the document is valid and sends an event notification to the shadow update
 * task to perform the required action.
 *
 * @param[in] pvIncomingPublishCallbackContext Context of the initial command.
 * @param[in] pxPublishInfo Deserialized publish.
 */
static void prvGetRejectedCallback( void * pxSubscriptionContext,
                                    MQTTPublishInfo_t * pxPublishInfo );


/**
 * @brief Creates the required shadow topics for the example, given thing name as the parameter.
 *
 * @param[in] pcThingName Pointer to the thing name
 * @param[in] xThingNameLength Length of the thing name.
 * @return true if the shadow topics are created successfully, false otherwise.
 */
static bool prvCreateShadowTopics( const char * pcThingName,
                                   size_t xThingNameLength );

/**
 * @brief Entry point of shadow demo.
 *
 * This main function demonstrates how to use the macros provided by the
 * Device Shadow library to assemble strings for the MQTT topics defined
 * by AWS IoT Device Shadow. It uses these macros for topics to subscribe
 * to:
 * - SHADOW_TOPIC_STRING_UPDATE_DELTA for "$aws/things/thingName/shadow/update/delta"
 * - SHADOW_TOPIC_STRING_UPDATE_ACCEPTED for "$aws/things/thingName/shadow/update/accepted"
 * - SHADOW_TOPIC_STRING_UPDATE_REJECTED for "$aws/things/thingName/shadow/update/rejected"
 *
 * It also uses these macros for topics to publish to:
 * - SHADOW_TOPIC_STIRNG_DELETE for "$aws/things/thingName/shadow/delete"
 * - SHADOW_TOPIC_STRING_UPDATE for "$aws/things/thingName/shadow/update"
 */
void vShadowDeviceTask( void * pvParameters );

/*-----------------------------------------------------------*/

static uint32_t g_ParsedPlayIndex;
static uint32_t g_ParsedPlayState;

#define shadowMUSICLIST_LENGTH (SUPPORT_MAX_MUSIC_FILE_COUNT * SUPPORT_MAX_MUSIC_NAME_LENGTH)
#define SHADOW_JSON_BUFFER_LENGTH (210 + shadowMUSICLIST_LENGTH)
char pcUpdateDocument[SHADOW_JSON_BUFFER_LENGTH];

/* Reports current state to shadow */
uint32_t prvReportShadowJSON()
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
                "\"clientToken\": \"%06lu\""
                "}",
                ( long unsigned )ulClientToken);
    return length;
}

static bool prvSubscribeToShadowUpdateTopics( void )
{
    bool xReturnStatus = false;
    MQTTStatus_t xStatus;
    uint32_t ulNotificationValue = 0U;
    MQTTAgentCommandInfo_t xCommandParams = { 0 };

    /* These must persist until the command is processed. */
    MQTTAgentSubscribeArgs_t xSubscribeArgs = { 0 };
    MQTTSubscribeInfo_t xSubscribeInfo[ 5 ];
    MQTTAgentCommandContext_t xCommandContext = { 0 };

    /* Subscribe to shadow topic for responses for incoming delta updates. */
    xSubscribeInfo[ 0 ].pTopicFilter = cGetAcceptedTopic;
    xSubscribeInfo[ 0 ].topicFilterLength = usGetAcceptedTopicLength;
    xSubscribeInfo[ 0 ].qos = MQTTQoS1;

    /* Subscribe to shadow topic for responses for incoming delta updates. */
    xSubscribeInfo[ 1 ].pTopicFilter = cGetRejectedTopic;
    xSubscribeInfo[ 1 ].topicFilterLength = usGetRejectedTopicLength;
    xSubscribeInfo[ 1 ].qos = MQTTQoS1;

    /* Subscribe to shadow topic for responses for incoming delta updates. */
    xSubscribeInfo[ 2 ].pTopicFilter = cUpdateDeltaTopic;
    xSubscribeInfo[ 2 ].topicFilterLength = usUpdateDeltaTopicLength;
    xSubscribeInfo[ 2 ].qos = MQTTQoS1;
    /* Subscribe to shadow topic for accepted responses for submitted updates. */
    xSubscribeInfo[ 3 ].pTopicFilter = cUpdateAcceptedTopic;
    xSubscribeInfo[ 3 ].topicFilterLength = usUpdateAcceptedTopicLength;
    xSubscribeInfo[ 3 ].qos = MQTTQoS1;
    /* Subscribe to shadow topic for rejected responses for submitted updates. */
    xSubscribeInfo[ 4 ].pTopicFilter = cUpdateRejectedTopic;
    xSubscribeInfo[ 4 ].topicFilterLength = usUpdateRejectedTopicLength;
    xSubscribeInfo[ 4 ].qos = MQTTQoS1;

    /* Complete the subscribe information. The topic string must persist for
     * duration of subscription - although in this case it is a static const so
     * will persist for the lifetime of the application. */
    xSubscribeArgs.pSubscribeInfo = xSubscribeInfo;
    xSubscribeArgs.numSubscriptions = 5;

    /* Loop in case the queue used to communicate with the MQTT agent is full and
     * attempts to post to it time out.  The queue will not become full if the
     * priority of the MQTT agent task is higher than the priority of the task
     * calling this function. */
    xTaskNotifyStateClear( NULL );

    xCommandContext.xReturnStatus = false;
    xCommandContext.xShadowTaskHandle = xTaskGetCurrentTaskHandle();

    xCommandParams.blockTimeMs = shadowexampleMAX_COMMAND_SEND_BLOCK_TIME_MS;
    xCommandParams.cmdCompleteCallback = prvSubscribeCommandCallback;
    xCommandParams.pCmdCompleteCallbackContext = &xCommandContext;
    LogInfo( ( "Sending subscribe request to agent for shadow topics." ) );

    do
    {
        /* If this fails, the agent's queue is full, so we retry until the agent
         * has more space in the queue. */
        xStatus = MQTTAgent_Subscribe( &xGlobalMqttAgentContext,
                                       &( xSubscribeArgs ),
                                       &xCommandParams );
    } while( xStatus != MQTTSuccess );

    /* Wait for acks from subscribe messages - this is optional.  If the
     * returned value is zero then the wait timed out. */
    ulNotificationValue = ulTaskNotifyTake( pdTRUE, pdMS_TO_TICKS( shadowexampleMS_TO_WAIT_FOR_NOTIFICATION ) );

    if( ulNotificationValue != 0U )
    {
        /* The callback sets the xReturnStatus member of the context. */
        if( xCommandContext.xReturnStatus != true )
        {
            LogError( ( "Failed to subscribe to shadow update topics." ) );
            xReturnStatus = false;
        }
        else
        {
            LogInfo( ( "Successfully subscribed to shadow update topics." ) );
            xReturnStatus = true;
        }
    }
    else
    {
        LogError( ( "Timed out to subscribe to shadow update topics." ) );
        xReturnStatus = false;
    }

    return xReturnStatus;
}

/*-----------------------------------------------------------*/

static void prvSubscribeCommandCallback( MQTTAgentCommandContext_t * pxCommandContext,
                                         MQTTAgentReturnInfo_t * pxReturnInfo )
{
    BaseType_t xSubscriptionAdded = pdFALSE;

    /* Check if the subscribe operation is a success. */
    if( pxReturnInfo->returnCode == MQTTSuccess )
    {
        /* Add subscriptions so that incoming publishes are routed to the application
         * callback. */
        xSubscriptionAdded = xAddMQTTTopicFilterCallback( cUpdateDeltaTopic,
                                                          usUpdateDeltaTopicLength,
                                                          prvUpdateDeltaCallback,
                                                          ( void * ) pxCommandContext->xShadowTaskHandle,
                                                          pdTRUE );
        configASSERT( xSubscriptionAdded == pdTRUE );

        xSubscriptionAdded = xAddMQTTTopicFilterCallback( cUpdateAcceptedTopic,
                                                          usUpdateAcceptedTopicLength,
                                                          prvUpdateAcceptedCallback,
                                                          ( void * ) pxCommandContext->xShadowTaskHandle,
                                                          pdTRUE );
        configASSERT( xSubscriptionAdded == pdTRUE );


        xSubscriptionAdded = xAddMQTTTopicFilterCallback( cUpdateRejectedTopic,
                                                          usUpdateRejectedTopicLength,
                                                          prvUpdateRejectedCallback,
                                                          ( void * ) pxCommandContext->xShadowTaskHandle,
                                                          pdTRUE );
        configASSERT( xSubscriptionAdded == pdTRUE );

        xSubscriptionAdded = xAddMQTTTopicFilterCallback( cGetAcceptedTopic,
                                                          usGetAcceptedTopicLength,
                                                          prvGetAcceptedCallback,
                                                          ( void * ) pxCommandContext->xShadowTaskHandle,
                                                          pdTRUE );
        configASSERT( xSubscriptionAdded == pdTRUE );

        xSubscriptionAdded = xAddMQTTTopicFilterCallback( cGetRejectedTopic,
                                                          usGetRejectedTopicLength,
                                                          prvGetRejectedCallback,
                                                          ( void * ) pxCommandContext->xShadowTaskHandle,
                                                          pdTRUE );
        configASSERT( xSubscriptionAdded == pdTRUE );
    }

    /* Store the result in the application defined context so the calling task
     * can check it. */
    pxCommandContext->xReturnStatus = xSubscriptionAdded;

    xTaskNotifyGive( pxCommandContext->xShadowTaskHandle );
}

/*-----------------------------------------------------------*/

static void prvUpdateDeltaCallback( void * pxSubscriptionContext,
                                    MQTTPublishInfo_t * pxPublishInfo )
{
    static uint32_t ulCurrentVersion = 0; /* Remember the latest version number we've received */
    uint32_t ulVersion = 0UL;
    char * pcOutValue = NULL;
    uint32_t ulOutValueLength = 0UL;
    JSONStatus_t result = JSONSuccess;
    JSONStatus_t result1 = JSONSuccess;
    TaskHandle_t xTaskToNotify = NULL;

    configASSERT( pxSubscriptionContext != NULL );
    configASSERT( pxPublishInfo != NULL );
    configASSERT( pxPublishInfo->pPayload != NULL );


    xTaskToNotify = ( TaskHandle_t ) ( pxSubscriptionContext );

    LogDebug( ( "/update/delta json payload:%.*s.",
                pxPublishInfo->payloadLength,
                ( const char * ) pxPublishInfo->pPayload ) );

    /* The payload will look similar to this:
     * {
     *      "state": {
     *          "powerOn": 1
     *      },
     *      "metadata": {
     *          "powerOn": {
     *              "timestamp": 1595437367
     *          }
     *      },
     *      "timestamp": 1595437367,
     *      "clientToken": "388062",
     *      "version": 12
     *  }
     */

    /* Make sure the payload is a valid json document. */
    result = JSON_Validate( pxPublishInfo->pPayload,
                            pxPublishInfo->payloadLength );

    if( result != JSONSuccess )
    {
        LogError( ( "Invalid JSON document received on update/delta topic!" ) );
    }

    if( result == JSONSuccess )
    {
        /* Obtain the version value. */
        result = JSON_Search( ( char * ) pxPublishInfo->pPayload,
                              pxPublishInfo->payloadLength,
                              "version",
                              sizeof( "version" ) - 1,
                              &pcOutValue,
                              ( size_t * ) &ulOutValueLength );

        if( result == JSONSuccess )
        {
            /* Convert the extracted value to an unsigned integer value. */
            ulVersion = ( uint32_t ) strtoul( pcOutValue, NULL, 10 );

            LogInfo( ( "Received delta update with version %lu.", ulVersion ) );

            /* Make sure the version is newer than the last one we received. */
            if( ulVersion <= ulCurrentVersion )
            {
                /* In this demo, we discard the incoming message
                 * if the version number is not newer than the latest
                 * that we've received before. Your application may use a
                 * different approach.
                 */
                LogWarn( ( "Ignoring delta update document with an older version %u. Current version is %u",
                           ( unsigned int ) ulVersion,
                           ( unsigned int ) ulCurrentVersion ) );
                result = JSONIllegalDocument;
            }
        }
        else
        {
            LogError( ( "Version field not found in delta update document." ) );
        }
    }

    if( result == JSONSuccess )
    {
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

        result1 = JSON_Search((char *)pxPublishInfo->pPayload, pxPublishInfo->payloadLength, "state.playIndex",
                             sizeof("state.playIndex") - 1, &pcOutValue, (size_t *)&ulOutValueLength);
        if (result1 == JSONSuccess)
        {
            /* Convert the LEDstate state value to an unsigned integer value. */
            g_ParsedPlayIndex = (uint32_t)strtoul(pcOutValue, NULL, 10);
        }

        if (( result == JSONSuccess ) || ( result1 == JSONSuccess ))
        {
            ulCurrentVersion = ulVersion;

            xTaskNotify( xTaskToNotify, shadowexampleUPDATE_DELTA_EVENT, eSetBits );
        }
        else
        {
            LogError( ( "Ignoring delta shadow document, state field not found." ) );
        }
    }
}

/*-----------------------------------------------------------*/

static void prvUpdateAcceptedCallback( void * pxSubscriptionContext,
                                       MQTTPublishInfo_t * pxPublishInfo )
{
    char * pcOutValue = NULL;
    uint32_t ulOutValueLength = 0UL;
    uint32_t ulReceivedToken = 0UL;
    JSONStatus_t result = JSONSuccess;
    TaskHandle_t xTaskToNotify = NULL;

    /* Remove compiler warnings about unused parameters. */
    ( void ) pxSubscriptionContext;

    configASSERT( pxSubscriptionContext != NULL );
    configASSERT( pxPublishInfo != NULL );
    configASSERT( pxPublishInfo->pPayload != NULL );

    xTaskToNotify = ( TaskHandle_t ) ( pxSubscriptionContext );

    LogDebug( ( "/update/accepted JSON payload: %.*s.",
                pxPublishInfo->payloadLength,
                ( const char * ) pxPublishInfo->pPayload ) );

    /* Handle the reported state with state change in /update/accepted topic.
     * Thus we will retrieve the client token from the JSON document to see if
     * it's the same one we sent with reported state on the /update topic.
     * The payload will look similar to this:
     *  {
     *      "state": {
     *          "reported": {
     *             "powerOn": 1
     *          }
     *      },
     *      "metadata": {
     *          "reported": {
     *              "powerOn": {
     *                  "timestamp": 1596573647
     *              }
     *          }
     *      },
     *      "version": 14698,
     *      "timestamp": 1596573647,
     *      "clientToken": "022485"
     *  }
     */

    /* Make sure the payload is a valid json document. */
    result = JSON_Validate( pxPublishInfo->pPayload,
                            pxPublishInfo->payloadLength );

    if( result != JSONSuccess )
    {
        LogError( ( "Invalid JSON document recieved!" ) );
    }
    else
    {
        /* Get clientToken from json documents. */
        result = JSON_Search( ( char * ) pxPublishInfo->pPayload,
                              pxPublishInfo->payloadLength,
                              "clientToken",
                              sizeof( "clientToken" ) - 1,
                              &pcOutValue,
                              ( size_t * ) &ulOutValueLength );
    }

    if( result != JSONSuccess )
    {
        LogDebug( ( "Ignoring publish on /update/accepted with no clientToken field." ) );
    }
    else
    {
        /* Convert the code to an unsigned integer value. */
        ulReceivedToken = ( uint32_t ) strtoul( pcOutValue, NULL, 10 );

        /* If we are waiting for a response, ulClientToken will be the token for the response
         * we are waiting for, else it will be 0. ulRecievedToken may not match if the response is
         * not for us or if it is is a response that arrived after we timed out
         * waiting for it.
         */
        if( ulReceivedToken != ulClientToken )
        {
            LogDebug( ( "Ignoring publish on /update/accepted with clientToken %lu.", ( unsigned long ) ulReceivedToken ) );
        }
        else
        {
            LogInfo( ( "Received accepted response for update with token %lu. ", ( unsigned long ) ulClientToken ) );
            xTaskNotify( xTaskToNotify, shadowexampleUPDATE_ACCEPTED_EVENT, eSetBits );
        }
    }
}

/*-----------------------------------------------------------*/

static void prvUpdateRejectedCallback( void * pxSubscriptionContext,
                                       MQTTPublishInfo_t * pxPublishInfo )
{
    JSONStatus_t result = JSONSuccess;
    char * pcOutValue = NULL;
    uint32_t ulOutValueLength = 0UL;
    uint32_t ulReceivedToken = 0UL;
    TaskHandle_t xTaskToNotify = NULL;

    configASSERT( pxSubscriptionContext != NULL );
    configASSERT( pxPublishInfo != NULL );
    configASSERT( pxPublishInfo->pPayload != NULL );

    LogDebug( ( "/update/rejected json payload: %.*s.",
                pxPublishInfo->payloadLength,
                ( const char * ) pxPublishInfo->pPayload ) );

    /* The payload will look similar to this:
     * {
     *    "code": error-code,
     *    "message": "error-message",
     *    "timestamp": timestamp,
     *    "clientToken": "token"
     * }
     */

    xTaskToNotify = ( TaskHandle_t ) ( pxSubscriptionContext );

    /* Make sure the payload is a valid json document. */
    result = JSON_Validate( pxPublishInfo->pPayload,
                            pxPublishInfo->payloadLength );

    if( result != JSONSuccess )
    {
        LogError( ( "Invalid JSON document recieved on update/rejected topic." ) );
    }

    if( result == JSONSuccess )
    {
        /* Get clientToken from json documents. */
        result = JSON_Search( ( char * ) pxPublishInfo->pPayload,
                              pxPublishInfo->payloadLength,
                              "clientToken",
                              sizeof( "clientToken" ) - 1,
                              &pcOutValue,
                              ( size_t * ) &ulOutValueLength );

        if( result == JSONSuccess )
        {
            /* Convert the code to an unsigned integer value. */
            ulReceivedToken = ( uint32_t ) strtoul( pcOutValue, NULL, 10 );

            /* If we are waiting for a response, ulClientToken will be the token for the response
             * we are waiting for, else it will be 0. ulRecievedToken may not match if the response is
             * not for us or if it is is a response that arrived after we timed out
             * waiting for it.
             */
            if( ulReceivedToken != ulClientToken )
            {
                result = JSONIllegalDocument;
                LogDebug( ( "Ignoring publish on /update/rejected with clientToken %lu.", ( unsigned long ) ulReceivedToken ) );
            }
        }
    }

    if( result == JSONSuccess )
    {
        /*  Obtain the error code. */
        result = JSON_Search( ( char * ) pxPublishInfo->pPayload,
                              pxPublishInfo->payloadLength,
                              "code",
                              sizeof( "code" ) - 1,
                              &pcOutValue,
                              ( size_t * ) &ulOutValueLength );

        if( result != JSONSuccess )
        {
            LogWarn( ( "Received rejected response for update with token %lu and no error code.", ( unsigned long ) ulClientToken ) );
        }
        else
        {
            LogWarn( ( "Received rejected response for update with token %lu and error code %.*s.", ( unsigned long ) ulClientToken,
                       ulOutValueLength,
                       pcOutValue ) );
        }

        /* Wake up the shadow task which is waiting for this response. */
        xTaskNotify( xTaskToNotify, shadowexampleUPDATE_REJECTED_EVENT, eSetBits );
    }
}

/*-----------------------------------------------------------*/

static void prvGetAcceptedCallback( void * pxSubscriptionContext,
                                    MQTTPublishInfo_t * pxPublishInfo )
{
    JSONStatus_t result = JSONSuccess;
    char * pcOutValue = NULL;
    uint32_t ulOutValueLength = 0UL;
    TaskHandle_t xTaskToNotify = NULL;
    uint8_t ucReceivedState;

    configASSERT( pxSubscriptionContext != NULL );
    configASSERT( pxPublishInfo != NULL );
    configASSERT( pxPublishInfo->pPayload != NULL );

    LogDebug( ( "/get/accepted json payload: %.*s.",
                pxPublishInfo->payloadLength,
                ( const char * ) pxPublishInfo->pPayload ) );

    /*
     *  The payload will look similar to this:
     *  {
     *      "state": {
     *             "powerOn": 1
     *          }
     *      },
     *      "version": 14698,
     *      "timestamp": 1596573647,
     *      "clientToken": "022485"
     *  }
     */

    xTaskToNotify = ( TaskHandle_t ) ( pxSubscriptionContext );

    /* Make sure the payload is a valid json document. */
    result = JSON_Validate( pxPublishInfo->pPayload,
                            pxPublishInfo->payloadLength );

    if( result != JSONSuccess )
    {
        LogError( ( "Invalid JSON document recieved on /get/accepted topic." ) );
    }

    if( result == JSONSuccess )
    {
        /*  Obtain the version of the document. */
        result = JSON_Search( ( char * ) pxPublishInfo->pPayload,
                              pxPublishInfo->payloadLength,
                              "version",
                              sizeof( "version" ) - 1,
                              &pcOutValue,
                              ( size_t * ) &ulOutValueLength );

        if( result == JSONSuccess )
        {
#if 0
            /* Convert the version number to an unsigned integer value. */
            ulReceivedVersion = ( uint32_t ) strtoul( pcOutValue, NULL, 10 );
#endif
        }
        else
        {
            LogWarn( ( "Received GET accepted response with no version." ) );
        }
    }

    if( result == JSONSuccess )
    {
#if 0
        /*  Obtain the state of the device from shadow. */
        result = JSON_Search( ( char * ) pxPublishInfo->pPayload,
                              pxPublishInfo->payloadLength,
                              "state.delta.powerOn",
                              sizeof( "state.delta.powerOn" ) - 1,
                              &pcOutValue,
                              ( size_t * ) &ulOutValueLength );

        if( result == JSONSuccess )
        {
            /* Convert the state to an unsigned integer value. */
            ucReceivedState = ( uint8_t ) strtoul( pcOutValue, NULL, 10 );

            if( ulReceivedVersion > ulCurrentVersion )
            {
                LogInfo( ( "Setting device state to %u.", ( unsigned int ) ucReceivedState ) );
                ucDeviceState = ucReceivedState;
                ulCurrentVersion = ulReceivedVersion;
            }
            else
            {
                LogInfo( ( "Ignoring update to device state with an older version,"
                           "current version : %lu, received version: %lu.",
                           ulCurrentVersion,
                           ulReceivedVersion ) );
            }
        }
        else
        {
            LogInfo( ( "Ignoring GET accepted response without a delta field." ) );
        }
#endif

        ( void ) xTaskNotify( xTaskToNotify, shadowexampleGET_ACCEPTED_EVENT, eSetBits );
    }

	(void)ucReceivedState;  /* Unused variable */
}


/*-----------------------------------------------------------*/

static void prvGetRejectedCallback( void * pxSubscriptionContext,
                                    MQTTPublishInfo_t * pxPublishInfo )
{
    JSONStatus_t result = JSONSuccess;
    char * pcOutValue = NULL;
    uint32_t ulOutValueLength = 0UL;
    TaskHandle_t xTaskToNotify = NULL;

    configASSERT( pxSubscriptionContext != NULL );
    configASSERT( pxPublishInfo != NULL );
    configASSERT( pxPublishInfo->pPayload != NULL );

    LogDebug( ( "/get/rejected json payload: %.*s.",
                pxPublishInfo->payloadLength,
                ( const char * ) pxPublishInfo->pPayload ) );

    /*
     *  The payload will look similar to this:
     * {
     *    "code": error-code,
     *    "message": "error-message",
     *    "timestamp": timestamp,
     *    "clientToken": "token"
     * }
     */

    xTaskToNotify = ( TaskHandle_t ) ( pxSubscriptionContext );

    /* Make sure the payload is a valid json document. */
    result = JSON_Validate( pxPublishInfo->pPayload,
                            pxPublishInfo->payloadLength );

    if( result != JSONSuccess )
    {
        LogError( ( "Invalid JSON document received on get/rejected topic.!" ) );
    }

    if( result == JSONSuccess )
    {
        /*  Obtain the error code. */
        result = JSON_Search( ( char * ) pxPublishInfo->pPayload,
                              pxPublishInfo->payloadLength,
                              "code",
                              sizeof( "code" ) - 1,
                              &pcOutValue,
                              ( size_t * ) &ulOutValueLength );

        if( result != JSONSuccess )
        {
            LogWarn( ( "Received rejected response for get without error code." ) );
        }
        else
        {
            LogWarn( ( "Received rejected response for get with error code %.*s.",
                       ulOutValueLength,
                       pcOutValue ) );
        }
    }

    if( result == JSONSuccess )
    {
        ( void ) xTaskNotify( xTaskToNotify, shadowexampleGET_REJECTED_EVENT, eSetBits );
    }
}

/*-----------------------------------------------------------*/

static bool prvGetShadowFromCloud( void )
{
    static MQTTPublishInfo_t xPublishInfo;
    MQTTAgentCommandInfo_t xCommandParams = { 0 };
    MQTTStatus_t xCommandStatus = MQTTSendFailed;
    bool xReturnStatus = false;
    uint32_t ulNotifiedValue = 0U;
    BaseType_t xNotifyStatus = pdFALSE;

    memset(&xPublishInfo, 0, sizeof (xPublishInfo));
    /* Set up MQTTPublishInfo_t for the GET message. */
    xPublishInfo.qos = MQTTQoS1;
    xPublishInfo.pTopicName = cGetTopic;
    xPublishInfo.topicNameLength = usGetTopicLength;
    xPublishInfo.pPayload = "";
    xPublishInfo.payloadLength = 0U;

    /*
     * We do not need a completion callback here since for publishes, we expect to get a
     * response on the appropriate topics for accepted or rejected reports. */
    xCommandParams.blockTimeMs = shadowexampleMAX_COMMAND_SEND_BLOCK_TIME_MS;
    xCommandParams.cmdCompleteCallback = NULL;

    /* Create a new client token and save it for use in the update accepted and rejected callbacks. */
    ulClientToken = ( xTaskGetTickCount() % 1000000 );


    /* Send update. */
    LogInfo( ( "Publishing to /get message using client token %lu.", ( unsigned long ) ( ulClientToken ) ) );

    xCommandStatus = MQTTAgent_Publish( &xGlobalMqttAgentContext,
                                        &xPublishInfo,
                                        &xCommandParams );

    if( xCommandStatus == MQTTSuccess )
    {
        LogInfo( ( "Successfully sent a publish message to /get topic." ) );
        xReturnStatus = true;
    }
    else
    {
        xReturnStatus = false;
    }

    if( xReturnStatus == true )
    {
        xNotifyStatus = xTaskNotifyWait( 0x00,
                                         ( shadowexampleGET_ACCEPTED_EVENT | shadowexampleGET_REJECTED_EVENT ),
                                         &ulNotifiedValue,
                                         pdMS_TO_TICKS( shadowexampleMS_TO_WAIT_FOR_NOTIFICATION ) );

        if( xNotifyStatus == pdTRUE )
        {
            if( ( ulNotifiedValue & shadowexampleGET_ACCEPTED_EVENT ) != 0 )
            {
                LogInfo( ( "Received an accepted response for shadow GET request. " ) );
            }
            else if( ( ulNotifiedValue & shadowexampleGET_REJECTED_EVENT ) != 0 )
            {
                LogWarn( ( "Cannot find a shadow document in cloud." ) );
                xReturnStatus = false;
            }
            else
            {
                LogError( ( "Timedout waiting for a response for /get request." ) );
                xReturnStatus = false;
            }
        }
        else
        {
            LogError( ( "Timedout waiting for a response for /get request." ) );
            xReturnStatus = false;
        }
    }

    ulClientToken = 0;

    return xReturnStatus;
}

/*-----------------------------------------------------------*/

static bool prvUpdateShadowDocument( void )
{
    static MQTTPublishInfo_t xPublishInfo;
    MQTTAgentCommandInfo_t xCommandParams = { 0 };
    MQTTStatus_t xCommandStatus = MQTTSendFailed;
    bool xReturnStatus = false;
    uint32_t ulNotifiedValue = 0U;
    BaseType_t xNotifyStatus = pdFALSE;


    memset(&xPublishInfo, 0, sizeof (xPublishInfo));
    /* Set up MQTTPublishInfo_t for the GET message. */
    xPublishInfo.qos = MQTTQoS0;
    xPublishInfo.pTopicName = cUpdateTopic;
    xPublishInfo.topicNameLength = usUpdateTopicLength;
    xPublishInfo.pPayload = pcUpdateDocument;


    /*
     * We do not need a completion callback here since for publishes, we expect to get a
     * response on the appropriate topics for accepted or rejected reports. */
    xCommandParams.blockTimeMs = shadowexampleMAX_COMMAND_SEND_BLOCK_TIME_MS;
    xCommandParams.cmdCompleteCallback = NULL;

    /* Create a new client token and save it for use in the update accepted and rejected callbacks. */
    ulClientToken = ( xTaskGetTickCount() % 1000000 );

    /* Generate update report. */
    ( void ) memset( pcUpdateDocument,
                     0x00,
                     sizeof( pcUpdateDocument ) );

    xPublishInfo.payloadLength = prvReportShadowJSON();

    //configASSERT( xPublishInfo.payloadLength <= ( shadowexampleSHADOW_REPORTED_JSON_LENGTH + 1 ) );

    /* Send update. */
    LogInfo( ( "Publishing to /update with following client token %lu.", ( long unsigned ) ulClientToken ) );

    xCommandStatus = MQTTAgent_Publish( &xGlobalMqttAgentContext,
                                        &xPublishInfo,
                                        &xCommandParams );

    if( xCommandStatus == MQTTSuccess )
    {
        LogInfo( ( "Successfully sent a publish message to /update topic." ) );
        xReturnStatus = true;
    }
    else
    {
        xReturnStatus = false;
    }

    if( xReturnStatus == true )
    {
        xNotifyStatus = xTaskNotifyWait( 0x00,
                                         ( shadowexampleUPDATE_ACCEPTED_EVENT | shadowexampleUPDATE_REJECTED_EVENT ),
                                         &ulNotifiedValue,
                                         pdMS_TO_TICKS( shadowexampleMS_TO_WAIT_FOR_NOTIFICATION ) );

        if( xNotifyStatus == pdTRUE )
        {
            if( ( ulNotifiedValue & shadowexampleUPDATE_ACCEPTED_EVENT ) != 0 )
            {
                LogInfo( ( "Successfully received a shadow update accepted message from cloud. " ) );
            }
            else if( ( ulNotifiedValue & shadowexampleGET_REJECTED_EVENT ) != 0 )
            {
                LogError( ( "Received a shadow update rejected message from cloud." ) );
                xReturnStatus = false;
            }
            else
            {
                LogError( ( "Timedout waiting for a response for shadow update request." ) );
                xReturnStatus = false;
            }
        }
        else
        {
            LogError( ( "Timedout waiting for a response for shadow update request." ) );
            xReturnStatus = false;
        }
    }

    ulClientToken = 0U;

    return xReturnStatus;
}

/*-----------------------------------------------------------*/

static bool prvCreateShadowTopics( const char * pcThingName,
                                   size_t xThingNameLength )
{
    bool xStatus = true;
    ShadowStatus_t xShadowStatus = SHADOW_FAIL;


    if( xStatus == true )
    {
        xShadowStatus = Shadow_AssembleTopicString( ShadowTopicStringTypeGet,
                                                    pcThingName,
                                                    xThingNameLength,
                                                    shadowexampleSHADOW_NAME,
                                                    shadowexampleSHADOW_NAME_LENGTH,
                                                    cGetTopic,
                                                    shadowexampleTOPIC_LENGTH_GET,
                                                    &usGetTopicLength );

        if( xShadowStatus != SHADOW_SUCCESS )
        {
            LogError( ( "Fail to construct shadow get topic, error = %u.", xShadowStatus ) );
            xStatus = false;
        }
    }

    if( xStatus == true )
    {
        xShadowStatus = Shadow_AssembleTopicString( ShadowTopicStringTypeGetAccepted,
                                                    pcThingName,
                                                    xThingNameLength,
                                                    shadowexampleSHADOW_NAME,
                                                    shadowexampleSHADOW_NAME_LENGTH,
                                                    cGetAcceptedTopic,
                                                    shadowexampleTOPIC_LENGTH_GET_ACCEPTED,
                                                    &usGetAcceptedTopicLength );

        if( xShadowStatus != SHADOW_SUCCESS )
        {
            LogError( ( "Fail to construct shadow get accepted topic, error = %u.", xShadowStatus ) );
            xStatus = false;
        }
    }

    if( xStatus == true )
    {
        xShadowStatus = Shadow_AssembleTopicString( ShadowTopicStringTypeGetRejected,
                                                    pcThingName,
                                                    xThingNameLength,
                                                    shadowexampleSHADOW_NAME,
                                                    shadowexampleSHADOW_NAME_LENGTH,
                                                    cGetRejectedTopic,
                                                    shadowexampleTOPIC_LENGTH_GET_REJECTED,
                                                    &usGetRejectedTopicLength );

        if( xShadowStatus != SHADOW_SUCCESS )
        {
            LogError( ( "Fail to construct shadow get rejected topic, error = %u.", xShadowStatus ) );
            xStatus = false;
        }
    }

    if( xStatus == true )
    {
        xShadowStatus = Shadow_AssembleTopicString( ShadowTopicStringTypeUpdate,
                                                    pcThingName,
                                                    xThingNameLength,
                                                    shadowexampleSHADOW_NAME,
                                                    shadowexampleSHADOW_NAME_LENGTH,
                                                    cUpdateTopic,
                                                    shadowexampleTOPIC_LENGTH_UPDATE,
                                                    &usUpdateTopicLength );

        if( xShadowStatus != SHADOW_SUCCESS )
        {
            LogError( ( "Fail to construct update topic, error = %u.", xShadowStatus ) );
            xStatus = false;
        }
    }

    if( xStatus == true )
    {
        xShadowStatus = Shadow_AssembleTopicString( ShadowTopicStringTypeUpdateAccepted,
                                                    pcThingName,
                                                    xThingNameLength,
                                                    shadowexampleSHADOW_NAME,
                                                    shadowexampleSHADOW_NAME_LENGTH,
                                                    cUpdateAcceptedTopic,
                                                    shadowexampleTOPIC_LENGTH_UPDATE_ACCEPTED,
                                                    &usUpdateAcceptedTopicLength );

        if( xShadowStatus != SHADOW_SUCCESS )
        {
            LogError( ( "Fail to construct update accepted topic, error = %u.", xShadowStatus ) );
            xStatus = false;
        }
    }

    if( xStatus == true )
    {
        xShadowStatus = Shadow_AssembleTopicString( ShadowTopicStringTypeUpdateRejected,
                                                    pcThingName,
                                                    xThingNameLength,
                                                    shadowexampleSHADOW_NAME,
                                                    shadowexampleSHADOW_NAME_LENGTH,
                                                    cUpdateRejectedTopic,
                                                    shadowexampleTOPIC_LENGTH_UPDATE_REJECTED,
                                                    &usUpdateRejectedTopicLength );

        if( xShadowStatus != SHADOW_SUCCESS )
        {
            LogError( ( "Fail to construct update rejected topic, error = %u.", xShadowStatus ) );
            xStatus = false;
        }
    }

    if( xStatus == true )
    {
        xShadowStatus = Shadow_AssembleTopicString( ShadowTopicStringTypeUpdateDelta,
                                                    pcThingName,
                                                    xThingNameLength,
                                                    shadowexampleSHADOW_NAME,
                                                    shadowexampleSHADOW_NAME_LENGTH,
                                                    cUpdateDeltaTopic,
                                                    shadowexampleTOPIC_LENGTH_UPDATE_DELTA,
                                                    &usUpdateDeltaTopicLength );

        if( xShadowStatus != SHADOW_SUCCESS )
        {
            LogError( ( "Fail to construct shadow update delta topic, error = %u.", xShadowStatus ) );
            xStatus = false;
        }
    }

    return xStatus;
}

/*-----------------------------------------------------------*/

void vShadowDeviceTask( void * pvParameters )
{
    bool xStatus = false;
    bool xShadowSynced = false;
    BaseType_t xNotifyStatus = pdFALSE;
    uint32_t ulNotifiedValue = 0;

    g_ParsedPlayIndex = 0xFFFFFFFFu;
    g_ParsedPlayState = 0xFFFFFFFFu;
    /* Remove compiler warnings about unused parameters. */
    ( void ) pvParameters;

    xThingNameLength = KVStore_getValueLength( KVS_CORE_THING_NAME );

    if( ( xThingNameLength > 0 ) && ( xThingNameLength <= shadowexampleTHINGNAME_MAX_LENGTH ) )
    {
        memset( cThingName, 0x00, sizeof( cThingName ) );
        ( void ) KVStore_getString( KVS_CORE_THING_NAME, cThingName, sizeof( cThingName ) );
        xStatus = true;
    }
    else
    {
        LogError( ( "Failed to get thing name from KV store, thing name length received = %u, "
                    "max thing name length supported = %u", xThingNameLength, shadowexampleTHINGNAME_MAX_LENGTH ) );
        xStatus = false;
    }

    if( xStatus == true )
    {
        if( xGetMQTTAgentState() != MQTT_AGENT_STATE_CONNECTED )
        {
            ( void ) xWaitForMQTTAgentState( MQTT_AGENT_STATE_CONNECTED, portMAX_DELAY );
        }

        LogInfo( ( "MQTT Agent is connected. Initializing shadow device task." ) );
    }

    if( xStatus == true )
    {
        xStatus = prvCreateShadowTopics( cThingName, xThingNameLength );
    }

    if( xStatus == true )
    {
        /* Subscribe to Shadow topics. */
        xStatus = prvSubscribeToShadowUpdateTopics();
    }

    if( xStatus == true )
    {
        for( ; ; )
        {
            if( xShadowSynced == false )
            {
                xShadowSynced = prvGetShadowFromCloud();
            }

            if( xShadowSynced == true )
            {
                xNotifyStatus = xTaskNotifyWait( 0x00,
                                                 ( shadowexampleUPDATE_DELTA_EVENT ),
                                                 &ulNotifiedValue,
                                                 portMAX_DELAY );

                if( xNotifyStatus == pdTRUE )
                {
                    if( ( ulNotifiedValue & shadowexampleUPDATE_DELTA_EVENT ) != 0 )
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
                                music_pause();
                            }
                        }

                        if ((g_ParsedPlayIndex != 0xFFFFFFFFu) && (g_ParsedPlayIndex != music_get_play_index()))
                        {
                            PRINTF("start play %d\r\n", g_ParsedPlayIndex);
                            music_play_index(g_ParsedPlayIndex);
                        }

#if 0
                        /* don't push current music state to AWS here, vShadowUpdateTask will do */
                        xShadowSynced = prvUpdateShadowDocument();
#endif

                        g_ParsedPlayState = 0xFFFFFFFFu;
                        g_ParsedPlayIndex = 0xFFFFFFFFu;
                    }
                }
                else
                {
                    xShadowSynced = false;
                }
            }

            if( xShadowSynced == false )
            {
                LogDebug( ( "Sleeping until next sync check." ) );
                vTaskDelay( pdMS_TO_TICKS( shadowexampleMS_BETWEEN_SYNC ) );
            }
        }
    }

    vTaskDelete( NULL );
}

/*-----------------------------------------------------------*/
