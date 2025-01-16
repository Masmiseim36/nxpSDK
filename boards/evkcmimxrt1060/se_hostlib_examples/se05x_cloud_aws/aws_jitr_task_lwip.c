/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * Copyright 2017-2018,2020 NXP
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/**
 * @file aws_jitr_task.c
 *
 * @brief simple MQTT publish and subscribe for NXP Secure Element Demo
 *
 * This example takes the parameters from the aws_iot_config.h file and establishes a connection to the AWS IoT MQTT
 * Platform.
 *
 * It subscribes to topic - "$aws/things/" clientcredentialIOT_THING_NAME "/shadow/update/accepted"
 *
 * If all the certs are correct, you should be get messages from cloud to this thing.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>

#include <aws_client_credential_keys.h>

#include "aws_iot_config.h"
#include "iot_default_root_certificates.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sm_types.h"
#include "common_cloud_config.h"

/* Retry utilities include. */
#include "backoff_algorithm.h"

#if !defined (SE_CLOUD_MCU_SDK)
#include "git_version.h"
#else
#include "board.h"
#define BUILD_GIT_VERSION_TOP_SHA1 "NA"
#define BUILD_BOARD_INFO BOARD_NAME
#endif

#if AX_EMBEDDED
#include "board.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "sm_demo_utils.h"
#endif

#include "core_json.h"
#include "mbedtls/base64.h"
#include "sm_printf.h"

#if SSS_HAVE_MBEDTLS_ALT_A71CH
#include "ax_mbedtls.h"
#include <fsl_sscp_a71ch.h>
#endif
#if SSS_HAVE_MBEDTLS_ALT_SSS
#include "sss_mbedtls.h"
#include "ex_sss.h"
#endif
#include "iot_mqtt_agent_config.h"
#include "core_mqtt.h"
#include "using_mbedtls.h"
#include "logging_levels.h"
#include "logging.h"

#if SSS_HAVE_SSS
#include <fsl_sss_api.h>
#endif

#include "ledHandler.h"
#include "nxLog_App.h"

#include <ex_sss_boot.h>
extern ex_sss_boot_ctx_t * pex_sss_demo_boot_ctx;
extern ex_sss_cloud_ctx_t * pex_sss_demo_tls_ctx;

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define AWS_IOT_MQTT_ALPN "\x0ex-amzn-mqtt-ca"
#define AWS_IOT_CUSTOM_AUTH_ALPN "\x04mqtt"

#define SUB_TOPIC "$aws/things/" clientcredentialIOT_THING_NAME "/shadow/update/accepted"
#define PUB_TOPIC "sdkTest/sub"

/* Used to avoid warnings in case of unused parameters in function pointers */
#define IOT_UNUSED(x) (void)(x)
/*******************************************************************************
 * types
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
int HandleReceivedMsg(char *sJsonString, uint16_t len);
void awsPubSub(const U8 ax_uid[MODULE_UNIQUE_ID_LEN]);

BaseType_t EstablishMqttSession( MQTTContext_t * pxMqttContext,
                                 NetworkContext_t * pxNetworkContext,
                                 MQTTFixedBuffer_t * pxNetworkBuffer,
                                 MQTTEventCallback_t eventCallback);

/*******************************************************************************
 * Structures
 ******************************************************************************/

/**
 * @brief Structure to keep the MQTT publish packets until an ack is received
 * for QoS1 publishes.
 */
typedef struct PublishPackets
{
    /**
     * @brief Packet identifier of the publish packet.
     */
    uint16_t packetId;

    /**
     * @brief Publish info of the publish packet.
     */
    MQTTPublishInfo_t pubInfo;
} PublishPackets_t;

/**
 * @brief A pair containing a topic filter and its SUBACK status.
 */
typedef struct topicFilterContext
{
    const char * pcTopicFilter;
    MQTTSubAckStatus_t xSubAckStatus;
} topicFilterContext_t;

typedef struct ServerInfo
{
    const char * pHostName; /**< @brief Server host name. Must be NULL-terminated. */
    size_t hostNameLength;  /**< @brief Length of the server host name. */
    uint16_t port;          /**< @brief Server port in host-order. */
} ServerInfo_t;



/*******************************************************************************
 * AFR
 ******************************************************************************/

#define echoCLIENT_ID ((const uint8_t *)AWS_IOT_MQTT_CLIENT_ID)
/*******************************************************************************
 * Variables
 ******************************************************************************/
/**
 * @brief Global entry time into the application to use as a reference timestamp
 * in the #prvGetTimeMs function. #prvGetTimeMs will always return the difference
 * between the current time and the global entry time. This will reduce the chances
 * of overflow for the 32 bit unsigned integer used for holding the timestamp.
 */
static uint32_t ulGlobalEntryTimeMs;

/*-----------------------------------------------------------*/

/**
 * @brief Packet Identifier generated when Subscribe request was sent to the broker;
 * it is used to match received Subscribe ACK to the transmitted subscribe.
 */
static uint16_t globalSubscribePacketIdentifier = 0U;

/**
 * @brief Packet Identifier generated when Unsubscribe request was sent to the broker;
 * it is used to match received Unsubscribe ACK to the transmitted unsubscribe
 * request.
 */
static uint16_t globalUnsubscribePacketIdentifier = 0U;

/**
 * @brief A struct containing the context of a SUBACK; the SUBACK status
 * of a filter is updated when the event callback processes a SUBACK.
 */
static topicFilterContext_t xTopicFilterContext;

/**
 * @brief Array to keep the outgoing publish messages.
 * These stored outgoing publish messages are kept until a successful ack
 * is received.
 */
static PublishPackets_t outgoingPublishPackets[ MAX_OUTGOING_PUBLISHES ] = { 0 };

/**
 * @brief The flag to indicate the mqtt session changed.
 */
static bool mqttSessionEstablished = false;

/*******************************************************************************
 * Declarations
 ******************************************************************************/

/**
 * @brief Calculate and perform an exponential backoff with jitter delay for
 * the next retry attempt of a failed network operation with the server.
 *
 * The function generates a random number, calculates the next backoff period
 * with the generated random number, and performs the backoff delay operation if the
 * number of retries have not exhausted.
 *
 * @note The PKCS11 module is used to generate the random number as it allows access
 * to a True Random Number Generator (TRNG) if the vendor platform supports it.
 * It is recommended to seed the random number generator with a device-specific entropy
 * source so that probability of collisions from devices in connection retries is mitigated.
 *
 * @note The backoff period is calculated using the backoffAlgorithm library.
 *
 * @param[in, out] pxRetryAttempts The context to use for backoff period calculation
 * with the backoffAlgorithm library.
 *
 * @return pdPASS if calculating the backoff period was successful; otherwise pdFAIL
 * if there was failure in random number generation OR all retry attempts had exhausted.
 */
static BaseType_t prvBackoffForRetry( BackoffAlgorithmContext_t * pxRetryParams );

/**
 * @brief Connect to MQTT broker with reconnection retries.
 *
 * If connection fails, retry is attempted after a timeout.
 * Timeout value will exponentially increase until maximum
 * timeout value is reached or the number of attempts are exhausted.
 *
 * @param[out] pxNetworkContext The output parameter to return the created network context.
 *
 * @return The status of the final connection attempt.
 */
static TlsTransportStatus_t prvConnectToServerWithBackoffRetries( NetworkContext_t * pxNetworkContext );

/**
 * @brief Helper function to wait for a specific incoming packet from the
 * broker.
 *
 * @param[in] pxMQTTContext MQTT context pointer.
 * @param[in] usPacketType Packet type to wait for.
 *
 * @return pdFAIL if the expected packet from the broker never arrives;
 * pdPASS if it arrives.
 */
// static BaseType_t prvWaitForPacket( MQTTContext_t * pxMQTTContext,
//                                     uint16_t usPacketType );

/**
 * @brief Function to get the free index at which an outgoing publish
 * can be stored.
 *
 * @param[out] pucIndex The output parameter to return the index at which an
 * outgoing publish message can be stored.
 *
 * @return pdFAIL if no more publishes can be stored;
 * pdPASS if an index to store the next outgoing publish is obtained.
 */
static BaseType_t prvGetNextFreeIndexForOutgoingPublishes( uint8_t * pucIndex );

/**
 * @brief Function to clean up an outgoing publish at given index from the
 * #outgoingPublishPackets array.
 *
 * @param[in] ucIndex The index at which a publish message has to be cleaned up.
 */
static void vCleanupOutgoingPublishAt( uint8_t ucIndex );

/**
 * @brief Function to clean up all the outgoing publishes maintained in the
 * array.
 */
static void vCleanupOutgoingPublishes( void );

/**
 * @brief Function to clean up the publish packet with the given packet id.
 *
 * @param[in] usPacketId Packet identifier of the packet to be cleaned up from
 * the array.
 */
static void vCleanupOutgoingPublishWithPacketID( uint16_t usPacketId );

/**
 * @brief Function to resend the publishes if a session is re-established with
 * the broker. This function handles the resending of the QoS1 publish packets,
 * which are maintained locally.
 *
 * @param[in] pxMqttContext MQTT context pointer.
 */
static BaseType_t handlePublishResend( MQTTContext_t * pxMqttContext );

/**
 * @brief The timer query function provided to the MQTT context.
 *
 * @return Time in milliseconds.
 */
static uint32_t prvGetTimeMs( void );

/**
 * Demo specific publish callback handler
 */
static void demoPubCallbackHandler(MQTTContext_t * pxMqttContext,
                              MQTTPacketInfo_t * pxPacketInfo,
                              MQTTDeserializedInfo_t * pxDeserializedInfo);


/*******************************************************************************
 * Functions
 ******************************************************************************/

static BaseType_t xPkcs11GenerateRandomNumber( uint8_t * pusRandomNumBuffer,
                                        size_t xBufferLength )
{
    BaseType_t xStatus = pdPASS;
    CK_RV xResult = CKR_OK;
    CK_FUNCTION_LIST_PTR pxFunctionList = NULL;
    CK_SESSION_HANDLE xSession = CK_INVALID_HANDLE;

    if( ( pusRandomNumBuffer == NULL ) || ( xBufferLength == 0U ) )
    {
        LogError( ( "Cannot generate random number. Invalid parameters passed. "
                    "buffer=%p,bufferLen=%lu", pusRandomNumBuffer, xBufferLength ) );
        xStatus = pdFAIL;
    }

    if( xStatus == pdPASS )
    {
        /* Get list of functions supported by the PKCS #11 port. */
        xResult = C_GetFunctionList( &pxFunctionList );

        if( ( xResult != CKR_OK ) || ( pxFunctionList == NULL ) )
        {
            LogError( ( "Failed to generate random number. "
                        "PKCS #11 API, C_GetFunctionList, failed." ) );
            xStatus = pdFAIL;
        }
    }

    if( xStatus == pdPASS )
    {
        /* Initialize PKCS #11 module and create a new session. */
        xResult = xInitializePkcs11Session( &xSession );

        if( ( xResult != CKR_OK ) || ( xSession == CK_INVALID_HANDLE ) )
        {
            LogError( ( "Failed to generate random number. "
                        "Failed to initialize PKCS #11 session." ) );
            xStatus = pdFAIL;
        }
    }

    if( xStatus == pdPASS )
    {
        if( pxFunctionList->C_GenerateRandom( xSession,
                                              pusRandomNumBuffer,
                                              xBufferLength ) != CKR_OK )
        {
            xStatus = pdFAIL;
            LogError( ( "Failed to generate random number. "
                        "PKCS #11 API, C_GenerateRandom, failed to generate random number." ) );
        }
    }

    if( xStatus == pdPASS )
    {
        if( pxFunctionList->C_CloseSession( xSession ) != CKR_OK )
        {
            xStatus = pdFAIL;
            LogError( ( " Failed to close PKCS #11 session after generating random number." ) );
        }
    }

    return xStatus;
}

static BaseType_t prvBackoffForRetry( BackoffAlgorithmContext_t * pxRetryParams )
{
    BaseType_t xReturnStatus = pdFAIL;
    uint16_t usNextRetryBackOff = 0U;
    BackoffAlgorithmStatus_t xBackoffAlgStatus = BackoffAlgorithmSuccess;

    /**
     * To calculate the backoff period for the next retry attempt, we will
     * generate a random number to provide to the backoffAlgorithm library.
     *
     * Note: The PKCS11 module is used to generate the random number as it allows access
     * to a True Random Number Generator (TRNG) if the vendor platform supports it.
     * It is recommended to use a random number generator seeded with a device-specific
     * entropy source so that probability of collisions from devices in connection retries
     * is mitigated.
     */
    uint32_t ulRandomNum = 0;

    if( xPkcs11GenerateRandomNumber( ( uint8_t * ) &ulRandomNum,
                                     sizeof( ulRandomNum ) ) == pdPASS )
    {
        /* Get back-off value (in milliseconds) for the next retry attempt. */
        xBackoffAlgStatus = BackoffAlgorithm_GetNextBackoff( pxRetryParams, ulRandomNum, &usNextRetryBackOff );

        if( xBackoffAlgStatus == BackoffAlgorithmRetriesExhausted )
        {
            LogError( ( "All retry attempts have exhausted. Operation will not be retried" ) );
        }
        else if( xBackoffAlgStatus == BackoffAlgorithmSuccess )
        {
            /* Perform the backoff delay. */
            vTaskDelay( pdMS_TO_TICKS( usNextRetryBackOff ) );

            xReturnStatus = pdPASS;

            LogInfo( ( "Retry attempt %lu out of maximum retry attempts %lu.",
                       ( pxRetryParams->attemptsDone + 1 ),
                       pxRetryParams->maxRetryAttempts ) );
        }
    }
    else
    {
        LogError( ( "Unable to retry operation with broker: Random number generation failed" ) );
    }

    return xReturnStatus;
}

/*-----------------------------------------------------------*/

static TlsTransportStatus_t prvConnectToServerWithBackoffRetries( NetworkContext_t * pxNetworkContext )
{
    TlsTransportStatus_t xNetworkStatus = TLS_TRANSPORT_SUCCESS;
    BackoffAlgorithmContext_t xReconnectParams = { 0 };
    BaseType_t xBackoffStatus = pdPASS;
    ServerInfo_t xServerInfo = { 0 };
    NetworkCredentials_t xNetworkCredentials = { 0 };
    char keyLabel[ 20 ] = {0};
    char certLabel[ 20 ] = {0};
    //SocketsConfig_t xSocketConfig = { 0 };
    const char * pcAlpnProtocols[] = { NULL, NULL };
   	pcAlpnProtocols[ 0 ] = AWS_IOT_MQTT_ALPN;

    /* Initialize information to connect to the MQTT broker. */
    xServerInfo.pHostName = (char *) clientcredentialMQTT_BROKER_ENDPOINT;
    xServerInfo.hostNameLength = sizeof( clientcredentialMQTT_BROKER_ENDPOINT ) - 1U;
    xServerInfo.port = clientcredentialMQTT_BROKER_PORT;

    /* Set the Secure Socket configurations. */
   // xSocketConfig.enableTls = true;

    /* Pass the ALPN protocol name depending on the port being used.
     * Please see more details about the ALPN protocol for the AWS IoT MQTT
     * endpoint in the link below.
     * https://aws.amazon.com/blogs/iot/mqtt-with-tls-client-authentication-on-port-443-why-it-is-useful-and-how-it-works/
     */
    if( xServerInfo.port == 443 )
    {
        xNetworkCredentials.pAlpnProtos = pcAlpnProtocols;
    }
    else if( xServerInfo.port == 8883 )
    {
        xNetworkCredentials.pAlpnProtos = NULL;
    }

    memset(certLabel, 0, sizeof(certLabel));
    if (snprintf(certLabel, sizeof(certLabel), "sss:%08lx", pex_sss_demo_tls_ctx->client_cert_index) < 0){
        LogError("snprintf error");
        return TLS_TRANSPORT_INVALID_CREDENTIALS;
    }
    memset(keyLabel, 0, sizeof(keyLabel));
    if (snprintf(keyLabel, sizeof(keyLabel), "sss:%08lx", pex_sss_demo_tls_ctx->client_keyPair_index) < 0){
        LogError("snprintf error");
        return TLS_TRANSPORT_INVALID_CREDENTIALS;
    }

    /* Set the credentials for establishing a TLS connection. */
    xNetworkCredentials.pRootCa = ( unsigned char * ) tlsCombi_ROOT_CERTIFICATE_PEM;
    xNetworkCredentials.rootCaSize = tlsCombi_ROOT_CERTIFICATE_LENGTH;
	xNetworkCredentials.pClientCertLabel = ( const char * ) &certLabel[ 0 ];
	xNetworkCredentials.pPrivateKeyLabel = ( const char * ) &keyLabel[ 0 ];
	xNetworkCredentials.disableSni = pdFALSE;


    /* Initialize reconnect attempts and interval. */
    BackoffAlgorithm_InitializeParams( &xReconnectParams,
                                       CONNECTION_RETRY_BACKOFF_BASE_MS,
                                       CONNECTION_RETRY_MAX_BACKOFF_DELAY_MS,
                                       CONNECTION_RETRY_MAX_ATTEMPTS );

    /* Attempt to connect to MQTT broker. If connection fails, retry after
     * a timeout. Timeout value will exponentially increase until maximum
     * attempts are reached.
     */
    do
    {
        /* Establish a TCP connection with the MQTT broker. This example connects to
         * the MQTT broker as specified in democonfigMQTT_BROKER_ENDPOINT and
         * democonfigMQTT_BROKER_PORT at the top of this file. */
        LogInfo( ( "Create a TCP connection to %s:%d.",
                clientcredentialMQTT_BROKER_ENDPOINT,
                clientcredentialMQTT_BROKER_PORT ) );

        xNetworkStatus = TLS_FreeRTOS_Connect( pxNetworkContext,
                                               clientcredentialMQTT_BROKER_ENDPOINT,
                                               clientcredentialMQTT_BROKER_PORT,
                                               &xNetworkCredentials,
                                               mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS,
                                               mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS );
        if( xNetworkStatus != TLS_TRANSPORT_SUCCESS )
        {
            LogWarn( ( "Connection to the broker failed. Attempting connection retry after backoff delay %d", xNetworkStatus) );

            /* As the connection attempt failed, we will retry the connection after an
             * exponential backoff with jitter delay. */

            /* Calculate the backoff period for the next retry attempt and perform the wait operation. */
            xBackoffStatus = prvBackoffForRetry( &xReconnectParams );
        }
    } while( ( xNetworkStatus != TLS_TRANSPORT_SUCCESS ) && ( xBackoffStatus == pdPASS ) );

    return xNetworkStatus;
}

/*-----------------------------------------------------------*/

BaseType_t SubscribeToTopic( MQTTContext_t * pxMqttContext,
                             const char * pcTopicFilter,
                             uint16_t usTopicFilterLength )
{
    MQTTStatus_t xMqttStatus = MQTTSuccess;
    BackoffAlgorithmContext_t xRetryParams;
    //BaseType_t xBackoffStatus = pdFAIL;
    BaseType_t xSubscribeStatus = pdPASS;
    MQTTSubscribeInfo_t xMQTTSubscription;

    assert( pxMqttContext != NULL );
    assert( pcTopicFilter != NULL );
    assert( usTopicFilterLength > 0 );

    /* Some fields not used so start with everything at 0. */
    ( void ) memset( ( void * ) &xMQTTSubscription, 0x00, sizeof( xMQTTSubscription ) );

    /* Initialize the status of the subscription acknowledgement. */
    //xTopicFilterContext.pcTopicFilter = pcTopicFilter;
    //xTopicFilterContext.xSubAckStatus = MQTTSubAckFailure;

    /* Get a unique packet id. */
    globalSubscribePacketIdentifier = MQTT_GetPacketId( pxMqttContext );

    /* Subscribe to the #pcTopicFilter topic filter that is passed by a demo application. */
    xMQTTSubscription.qos = MQTTQoS0;
    xMQTTSubscription.pTopicFilter = pcTopicFilter;
    xMQTTSubscription.topicFilterLength = usTopicFilterLength;

    /* Initialize retry attempts and interval. */
    BackoffAlgorithm_InitializeParams( &xRetryParams,
                                       SUBSCRIBE_RETRY_BACKOFF_BASE_MS,
                                       SUBSCRIBE_RETRY_MAX_BACKOFF_DELAY_MS,
                                       SUBSCRIBE_RETRY_MAX_ATTEMPTS );

    /* The client should now be connected to the broker. Subscribe to the topic
        * as specified in #pcTopicFilter by sending a subscribe packet. */
    LogInfo( ( "Attempt to subscribe to the MQTT topic %s.", pcTopicFilter ) );
    xMqttStatus = MQTT_Subscribe( pxMqttContext,
                                    &xMQTTSubscription,
                                    sizeof( xMQTTSubscription ) / sizeof( MQTTSubscribeInfo_t ),
                                    globalSubscribePacketIdentifier );

    if( xMqttStatus != MQTTSuccess )
    {

        LogError( ( "Failed to SUBSCRIBE to MQTT topic %s. Error=%s",
                    pcTopicFilter, usTopicFilterLength ) );
        xSubscribeStatus = pdFAIL;
    }

    return xSubscribeStatus;
}

/*-----------------------------------------------------------*/

static BaseType_t prvGetNextFreeIndexForOutgoingPublishes( uint8_t * pucIndex )
{
    BaseType_t xReturnStatus = pdFAIL;
    uint8_t ucIndex = 0;

    assert( outgoingPublishPackets != NULL );
    assert( pucIndex != NULL );

    for( ucIndex = 0; ucIndex < MAX_OUTGOING_PUBLISHES; ucIndex++ )
    {
        /* A free ucIndex is marked by invalid packet id.
         * Check if the the ucIndex has a free slot. */
        if( outgoingPublishPackets[ ucIndex ].packetId == MQTT_PACKET_ID_INVALID )
        {
            xReturnStatus = pdPASS;
            break;
        }
    }

    /* Copy the available ucIndex into the output param. */
    *pucIndex = ucIndex;

    return xReturnStatus;
}

/*-----------------------------------------------------------*/

static void vCleanupOutgoingPublishAt( uint8_t ucIndex )
{
    assert( outgoingPublishPackets != NULL );
    assert( ucIndex < MAX_OUTGOING_PUBLISHES );

    /* Clear the outgoing publish packet. */
    ( void ) memset( &( outgoingPublishPackets[ ucIndex ] ),
                     0x00,
                     sizeof( outgoingPublishPackets[ ucIndex ] ) );
}

/*-----------------------------------------------------------*/

static void vCleanupOutgoingPublishes( void )
{
    assert( outgoingPublishPackets != NULL );

    /* Clean up all the outgoing publish packets. */
    ( void ) memset( outgoingPublishPackets, 0x00, sizeof( outgoingPublishPackets ) );
}

/*-----------------------------------------------------------*/

static void vCleanupOutgoingPublishWithPacketID( uint16_t usPacketId )
{
    uint8_t ucIndex = 0;

    assert( outgoingPublishPackets != NULL );
    assert( usPacketId != MQTT_PACKET_ID_INVALID );

    /* Clean up all the saved outgoing publishes. */
    for( ; ucIndex < MAX_OUTGOING_PUBLISHES; ucIndex++ )
    {
        if( outgoingPublishPackets[ ucIndex ].packetId == usPacketId )
        {
            vCleanupOutgoingPublishAt( ucIndex );
            LogInfo( ( "Cleaned up outgoing publish packet with packet id %u.\n\n",
                       usPacketId ) );
            break;
        }
    }
}

/*-----------------------------------------------------------*/

void vHandleOtherIncomingPacket( MQTTPacketInfo_t * pxPacketInfo,
                                 uint16_t usPacketIdentifier )
{
    MQTTStatus_t xResult = MQTTSuccess;
    uint8_t * pucPayload = NULL;
    size_t xSize = 0;

    /* Handle other packets. */
    switch( pxPacketInfo->type )
    {
        case MQTT_PACKET_TYPE_SUBACK:
            LogInfo( ( "MQTT_PACKET_TYPE_SUBACK.\n\n" ) );
            /* A SUBACK from the broker, containing the server response to our subscription request, has been received.
             * It contains the status code indicating server approval/rejection for the subscription to the single topic
             * requested. The SUBACK will be parsed to obtain the status code, and this status code will be stored in global
             * variable #xTopicFilterContext. */
            xResult = MQTT_GetSubAckStatusCodes( pxPacketInfo, &pucPayload, &xSize );

            /* MQTT_GetSubAckStatusCodes always returns success if called with packet info
             * from the event callback and non-NULL parameters. */
            configASSERT( xResult == MQTTSuccess );
            /* Only a single topic filter is expected for each SUBSCRIBE packet. */
            configASSERT( xSize == 1UL );
            xTopicFilterContext.xSubAckStatus = *pucPayload;

            if( xTopicFilterContext.xSubAckStatus != MQTTSubAckFailure )
            {
                LogInfo( ( "Subscribed to the topic %s with maximum QoS %u.",
                           xTopicFilterContext.pcTopicFilter,
                           xTopicFilterContext.xSubAckStatus ) );
            }

            /* Make sure ACK packet identifier matches with Request packet identifier. */
            assert( globalSubscribePacketIdentifier == usPacketIdentifier );
            break;

        case MQTT_PACKET_TYPE_UNSUBACK:
            LogInfo( ( "MQTT_PACKET_TYPE_UNSUBACK.\n\n" ) );
            /* Make sure ACK packet identifier matches with Request packet identifier. */
            assert( globalUnsubscribePacketIdentifier == usPacketIdentifier );
            break;

        case MQTT_PACKET_TYPE_PINGRESP:

            /* Nothing to be done from application as library handles
             * PINGRESP. */
            LogWarn( ( "PINGRESP should not be handled by the application "
                       "callback when using MQTT_ProcessLoop.\n\n" ) );
            break;

        case MQTT_PACKET_TYPE_PUBACK:
            LogInfo( ( "PUBACK received for packet id %u.\n\n",
                       usPacketIdentifier ) );
            /* Cleanup publish packet when a PUBACK is received. */
            vCleanupOutgoingPublishWithPacketID( usPacketIdentifier );
            break;

        /* Any other packet type is invalid. */
        default:
            LogError( ( "Unknown packet type received:(%02x).\n\n",
                        pxPacketInfo->type ) );
    }
}

/*-----------------------------------------------------------*/

static BaseType_t handlePublishResend( MQTTContext_t * pxMqttContext )
{
    BaseType_t xReturnStatus = pdPASS;
    MQTTStatus_t eMqttStatus = MQTTSuccess;
    uint8_t ucIndex = 0U;

    assert( outgoingPublishPackets != NULL );

    /* Resend all the QoS1 publishes still in the array. These are the
     * publishes that hasn't received a PUBACK. When a PUBACK is
     * received, the publish is removed from the array. */
    for( ucIndex = 0U; ucIndex < MAX_OUTGOING_PUBLISHES; ucIndex++ )
    {
        if( outgoingPublishPackets[ ucIndex ].packetId != MQTT_PACKET_ID_INVALID )
        {
            outgoingPublishPackets[ ucIndex ].pubInfo.dup = true;

            LogInfo( ( "Sending duplicate PUBLISH with packet id %u.",
                       outgoingPublishPackets[ ucIndex ].packetId ) );
            eMqttStatus = MQTT_Publish( pxMqttContext,
                                        &outgoingPublishPackets[ ucIndex ].pubInfo,
                                        outgoingPublishPackets[ ucIndex ].packetId );

            if( eMqttStatus != MQTTSuccess )
            {
                LogError( ( "Sending duplicate PUBLISH for packet id %u "
                            " failed with status %s.",
                            outgoingPublishPackets[ ucIndex ].packetId,
                            MQTT_Status_strerror( eMqttStatus ) ) );
                xReturnStatus = pdFAIL;
                break;
            }
            else
            {
                LogInfo( ( "Sent duplicate PUBLISH successfully for packet id %u.\n\n",
                           outgoingPublishPackets[ ucIndex ].packetId ) );
            }
        }
    }

    return xReturnStatus;
}

/*-----------------------------------------------------------*/

BaseType_t EstablishMqttSession( MQTTContext_t * pxMqttContext,
                                 NetworkContext_t * pxNetworkContext,
                                 MQTTFixedBuffer_t * pxNetworkBuffer,
                                 MQTTEventCallback_t eventCallback)
{
    BaseType_t xReturnStatus = pdPASS;
    MQTTStatus_t eMqttStatus = MQTTSuccess;
    MQTTConnectInfo_t xConnectInfo = { 0 };
    TransportInterface_t xTransport = { 0 };
    bool sessionPresent = false;

    assert( pxMqttContext != NULL );
    assert( pxNetworkContext != NULL );

    /* Initialize the mqtt context and network context. */
    ( void ) memset( pxMqttContext, 0U, sizeof( MQTTContext_t ) );
    ( void ) memset( pxNetworkContext, 0U, sizeof( NetworkContext_t ) );


    //pxNetworkContext->pParams = &xSecureSocketsTransportParams;

    if( prvConnectToServerWithBackoffRetries( pxNetworkContext ) != TLS_TRANSPORT_SUCCESS )
    {
        /* Log error to indicate connection failure after all
         * reconnect attempts are over. */
        LogError( ( "Failed to connect to MQTT broker %.*s.",
                    strlen( clientcredentialMQTT_BROKER_ENDPOINT ),
                    clientcredentialMQTT_BROKER_ENDPOINT ) );
        xReturnStatus = pdFAIL;
    }
    else
    {
        /* Fill in Transport Interface send and receive function pointers. */
        xTransport.pNetworkContext = pxNetworkContext;
        xTransport.send = TLS_FreeRTOS_Send;
        xTransport.recv = TLS_FreeRTOS_Recv;

        /* Initialize MQTT library. */
        eMqttStatus = MQTT_Init( pxMqttContext,
                                 &xTransport,
                                 prvGetTimeMs,
                                 eventCallback,
                                 pxNetworkBuffer );

        if( eMqttStatus != MQTTSuccess )
        {
            xReturnStatus = pdFAIL;
            LogError( ( "MQTT init failed with status %s.",
                        MQTT_Status_strerror( eMqttStatus ) ) );
        }
        else
        {
            /* Establish MQTT session by sending a CONNECT packet. */

            /* Many fields not used in this demo so start with everything at 0. */
            ( void ) memset( ( void * ) &xConnectInfo, 0x00, sizeof( xConnectInfo ) );

            /* Start with a clean session i.e. direct the MQTT broker to discard any
             * previous session data. Also, establishing a connection with clean session
             * will ensure that the broker does not store any data when this client
             * gets disconnected. */
            xConnectInfo.cleanSession = true;

            /* The client identifier is used to uniquely identify this MQTT client to
             * the MQTT broker. In a production device the identifier can be something
             * unique, such as a device serial number. */
            xConnectInfo.pClientIdentifier = (const char *)echoCLIENT_ID;
            xConnectInfo.clientIdentifierLength = (uint16_t)strlen((const char *)echoCLIENT_ID);

            /* Use the metrics string as username to report the OS and MQTT client version
             * metrics to AWS IoT. */
            xConnectInfo.pUserName = NULL;
            xConnectInfo.userNameLength = 0;
            /* Password for authentication is not used. */
            xConnectInfo.pPassword = NULL;
            xConnectInfo.passwordLength = 0;

            /* The maximum time interval in seconds which is allowed to elapse
             * between two Control Packets.
             * It is the responsibility of the Client to ensure that the interval between
             * Control Packets being sent does not exceed the this Keep Alive value. In the
             * absence of sending any other Control Packets, the Client MUST send a
             * PINGREQ Packet. */
            xConnectInfo.keepAliveSeconds = mqttexampleKEEP_ALIVE_TIMEOUT_SECONDS;

            /* Send MQTT CONNECT packet to broker. */
            eMqttStatus = MQTT_Connect( pxMqttContext,
                                        &xConnectInfo,
                                        NULL,
                                        mqttexampleCONNACK_RECV_TIMEOUT_MS,
                                        &sessionPresent );

            if( eMqttStatus != MQTTSuccess )
            {
                xReturnStatus = pdFAIL;
                LogError( ( "Connection with MQTT broker failed with status %s.",
                            MQTT_Status_strerror( eMqttStatus ) ) );
            }
            else
            {
                LogInfo( ( "MQTT connection successfully established with broker.\n\n" ) );
            }
        }

        if( xReturnStatus == pdPASS )
        {
            /* Keep a flag for indicating if MQTT session is established. This
             * flag will mark that an MQTT DISCONNECT has to be sent at the end
             * of the demo even if there are intermediate failures. */
            mqttSessionEstablished = true;
        }

        if( xReturnStatus == pdPASS )
        {
            /* Check if session is present and if there are any outgoing publishes
             * that need to resend. This is only valid if the broker is
             * re-establishing a session which was already present. */
            if( sessionPresent == true )
            {
                LogInfo( ( "An MQTT session with broker is re-established. "
                           "Resending unacked publishes." ) );

                /* Handle all the resend of publish messages. */
                xReturnStatus = handlePublishResend( pxMqttContext );
            }
            else
            {
                LogInfo( ( "A clean MQTT connection is established."
                           " Cleaning up all the stored outgoing publishes.\n\n" ) );

                /* Clean up the outgoing publishes waiting for ack as this new
                 * connection doesn't re-establish an existing session. */
                vCleanupOutgoingPublishes();
            }
        }
    }

    return xReturnStatus;
}

/*-----------------------------------------------------------*/

BaseType_t DisconnectMqttSession( MQTTContext_t * pxMqttContext,
                                  NetworkContext_t * pxNetworkContext )
{
    MQTTStatus_t eMqttStatus = MQTTSuccess;
    BaseType_t xReturnStatus = pdPASS;

    assert( pxMqttContext != NULL );
    assert( pxNetworkContext != NULL );

    if( mqttSessionEstablished == true )
    {
        /* Send DISCONNECT. */
        eMqttStatus = MQTT_Disconnect( pxMqttContext );

        if( eMqttStatus != MQTTSuccess )
        {
            LogError( ( "Sending MQTT DISCONNECT failed with status=%s.",
                        MQTT_Status_strerror( eMqttStatus ) ) );
            xReturnStatus = pdFAIL;
        }
    }

    /* Close the network connection.  */
    TLS_FreeRTOS_Disconnect( pxNetworkContext );

    return xReturnStatus;
}

/*-----------------------------------------------------------*/

BaseType_t UnsubscribeFromTopic( MQTTContext_t * pxMqttContext,
                                 const char * pcTopicFilter,
                                 uint16_t usTopicFilterLength )
{
    BaseType_t xReturnStatus = pdPASS;
    MQTTStatus_t eMqttStatus;
    MQTTSubscribeInfo_t pSubscriptionList[ 1 ];

    assert( pxMqttContext != NULL );
    assert( pcTopicFilter != NULL );
    assert( usTopicFilterLength > 0 );

    /* Start with everything at 0. */
    ( void ) memset( ( void * ) pSubscriptionList, 0x00, sizeof( pSubscriptionList ) );

    /* This example subscribes to only one topic and uses QOS1. */
    pSubscriptionList[ 0 ].qos = MQTTQoS0;
    pSubscriptionList[ 0 ].pTopicFilter = pcTopicFilter;
    pSubscriptionList[ 0 ].topicFilterLength = usTopicFilterLength;

    /* Generate packet identifier for the UNSUBSCRIBE packet. */
    globalUnsubscribePacketIdentifier = MQTT_GetPacketId( pxMqttContext );

    /* Send UNSUBSCRIBE packet. */
    eMqttStatus = MQTT_Unsubscribe( pxMqttContext,
                                    pSubscriptionList,
                                    sizeof( pSubscriptionList ) / sizeof( MQTTSubscribeInfo_t ),
                                    globalUnsubscribePacketIdentifier );

    if( eMqttStatus != MQTTSuccess )
    {
        LogError( ( "Failed to send UNSUBSCRIBE packet to broker with error = %s.",
                    MQTT_Status_strerror( eMqttStatus ) ) );
        xReturnStatus = pdFAIL;
    }
    // else
    // {
    //     LogInfo( ( "UNSUBSCRIBE sent topic %.*s to broker.\n\n",
    //                usTopicFilterLength,
    //                pcTopicFilter ) );

    //     /* Process incoming packet from the broker. Acknowledgment for unsubscription
    //      * ( UNSUBACK ) will be received here. */
    //     xReturnStatus = prvWaitForPacket( pxMqttContext, MQTT_PACKET_TYPE_UNSUBACK );

    //     if( xReturnStatus == pdFAIL )
    //     {
    //         LogError( ( "UNSUBACK never arrived when unsubscribing to topic %s.",
    //                     pcTopicFilter ) );
    //     }
    // }

    return xReturnStatus;
}

/*-----------------------------------------------------------*/

BaseType_t PublishToTopic( MQTTContext_t * pxMqttContext,
                           const char * pcTopicFilter,
                           int32_t topicFilterLength,
                           const char * pcPayload,
                           size_t payloadLength )
{
    BaseType_t xReturnStatus = pdPASS;
    MQTTStatus_t eMqttStatus = MQTTSuccess;
    uint8_t ucPublishIndex = MAX_OUTGOING_PUBLISHES;

    assert( pxMqttContext != NULL );
    assert( pcTopicFilter != NULL );
    assert( topicFilterLength > 0 );

    /* Get the next free index for the outgoing publish. All QoS1 outgoing
     * publishes are stored until a PUBACK is received. These messages are
     * stored for supporting a resend if a network connection is broken before
     * receiving a PUBACK. */
    xReturnStatus = prvGetNextFreeIndexForOutgoingPublishes( &ucPublishIndex );

    if( xReturnStatus == pdFAIL )
    {
        LogError( ( "Unable to find a free spot for outgoing PUBLISH message.\n\n" ) );
    }
    else
    {
        LogInfo( ( "the published payload:%.*s \r\n ", payloadLength, pcPayload ) );
        /* This example publishes to only one topic and uses QOS1. */
        outgoingPublishPackets[ ucPublishIndex ].pubInfo.qos = MQTTQoS0;
        outgoingPublishPackets[ ucPublishIndex ].pubInfo.pTopicName = pcTopicFilter;
        outgoingPublishPackets[ ucPublishIndex ].pubInfo.topicNameLength = topicFilterLength;
        outgoingPublishPackets[ ucPublishIndex ].pubInfo.pPayload = pcPayload;
        outgoingPublishPackets[ ucPublishIndex ].pubInfo.payloadLength = payloadLength;

        /* Get a new packet id. */
        outgoingPublishPackets[ ucPublishIndex ].packetId = MQTT_GetPacketId( pxMqttContext );

        /* Send PUBLISH packet. */
        eMqttStatus = MQTT_Publish( pxMqttContext,
                                    &outgoingPublishPackets[ ucPublishIndex ].pubInfo,
                                    outgoingPublishPackets[ ucPublishIndex ].packetId );

        if( eMqttStatus != MQTTSuccess )
        {
            LogError( ( "Failed to send PUBLISH packet to broker with error = %s.",
                        MQTT_Status_strerror( eMqttStatus ) ) );
            vCleanupOutgoingPublishAt( ucPublishIndex );
            xReturnStatus = pdFAIL;
        }
        else
        {
            LogInfo( ( "PUBLISH sent for topic %.*s to broker with packet ID %u.\n\n",
                       topicFilterLength,
                       pcTopicFilter,
                       outgoingPublishPackets[ ucPublishIndex ].packetId ) );

            /* Calling MQTT_ProcessLoop to process incoming publish echo, since
             * application subscribed to the same topic the broker will send
             * publish message back to the application. This function also
             * sends ping request to broker if MQTT_KEEP_ALIVE_INTERVAL_SECONDS
             * has expired since the last MQTT packet sent and receive
             * ping responses. */
//            eMqttStatus = MQTT_ProcessLoop( pxMqttContext, mqttexamplePROCESS_LOOP_TIMEOUT_MS );

            if( eMqttStatus != MQTTSuccess )
            {
                LogWarn( ( "MQTT_ProcessLoop returned with status = %s.",
                           MQTT_Status_strerror( eMqttStatus ) ) );
            }
        }
    }

    return xReturnStatus;
}

/*-----------------------------------------------------------*/

BaseType_t ProcessLoop( MQTTContext_t * pxMqttContext,
                        uint32_t ulTimeoutMs )
{
    BaseType_t xReturnStatus = pdFAIL;
    MQTTStatus_t eMqttStatus = MQTTSuccess;

    eMqttStatus = MQTT_ProcessLoop( pxMqttContext);

    if( eMqttStatus != MQTTSuccess )
    {
        LogWarn( ( "MQTT_ProcessLoop returned with status = %s.",
                   MQTT_Status_strerror( eMqttStatus ) ) );
    }
    else
    {
        LogDebug( ( "MQTT_ProcessLoop successful." ) );
        xReturnStatus = pdPASS;
    }

    return xReturnStatus;
}

/*-----------------------------------------------------------*/

static uint32_t prvGetTimeMs( void )
{
    TickType_t xTickCount = 0;
    uint32_t ulTimeMs = 0UL;

    /* Get the current tick count. */
    xTickCount = xTaskGetTickCount();

    /* Convert the ticks to milliseconds. */
    ulTimeMs = ( uint32_t ) xTickCount * MILLISECONDS_PER_TICK;

    /* Reduce ulGlobalEntryTimeMs from obtained time so as to always return the
     * elapsed time in the application. */
    ulTimeMs = ( uint32_t ) ( ulTimeMs - ulGlobalEntryTimeMs );

    return ulTimeMs;
}

/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
static void prvEventCallback( MQTTContext_t * pxMqttContext,
                              MQTTPacketInfo_t * pxPacketInfo,
                              MQTTDeserializedInfo_t * pxDeserializedInfo )
{
    uint16_t usPacketIdentifier;

    ( void ) pxMqttContext;

    assert( pxDeserializedInfo != NULL );
    assert( pxMqttContext != NULL );
    assert( pxPacketInfo != NULL );

    usPacketIdentifier = pxDeserializedInfo->packetIdentifier;

    /* Handle incoming publish. The lower 4 bits of the publish packet
     * type is used for the dup, QoS, and retain flags. Hence masking
     * out the lower bits to check if the packet is publish. */
    if( ( pxPacketInfo->type & 0xF0U ) == MQTT_PACKET_TYPE_PUBLISH )
    {
        assert( pxDeserializedInfo->pPublishInfo != NULL );
        LogInfo( ( "pPublishInfo->pTopicName:%s.", pxDeserializedInfo->pPublishInfo->pTopicName ) );

        demoPubCallbackHandler(pxMqttContext, pxPacketInfo, pxDeserializedInfo);
    }
    else
    {
        vHandleOtherIncomingPacket( pxPacketInfo, usPacketIdentifier );
    }
}
/*-----------------------------------------------------------*/

int HandleReceivedMsg(char *sJsonString, uint16_t len)
{
    ledColor_t led = LED_INVALID;
    ledState_t state = LED_OFF;
    char* valuePtr;
    size_t valueSize = 0U;

    if(JSON_Search(sJsonString, len, "user", 4, &valuePtr, &valueSize) == JSONSuccess) {
        //printf("User %s\n", valuePtr);
    }

    if (JSON_Search(sJsonString, len, "red", 3, &valuePtr, &valueSize) == JSONSuccess){
        printf("LED Color: Red");
        led = RED;
    }
    else if(JSON_Search(sJsonString, len, "green", 5, &valuePtr, &valueSize) == JSONSuccess) {
        printf("LED Color: Green");
        led = GREEN;
    }
    else if(JSON_Search(sJsonString, len, "blue", 4, &valuePtr, &valueSize) == JSONSuccess) {
        printf("LED Color: Blue");
        led = BLUE;
    }
    else {
        //printf("Key not found\n");
    }

    if (strncmp(valuePtr, "on", valueSize) == 0) {
        printf("- state: LED ON\n");
        state = LED_ON;
    }
    else if (strncmp(valuePtr, "off", valueSize) == 0) {
        printf("- state: LED OFF\n");
        state = LED_OFF;
    }
    else if (strncmp(valuePtr, "toggle", valueSize) == 0) {
        printf("- state: LED TOGGLE\n");
        state = LED_TOGGLE;
    }
    else {
        led = LED_INVALID;
        //printf("Unexpected value: %.*s\n", valuePtr);
    }

    if (LED_INVALID != led) {
        led_handler(led, state);
    }

    return 0;
}

void awsPubSub_task(void *pvParameters)
{
    U8 AxUID[MODULE_UNIQUE_ID_LEN] = {0};

    /*keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM Not used variable to avoid warning set it to NULL*/
    // keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;

#if SSS_HAVE_MBEDTLS_ALT_A71CH
    HLSE_CONNECTION_PARAMS params;
    HLSE_RET_CODE hlseret;
    HLSE_COMMUNICATION_STATE commState;

    hlseret = HLSE_Connect(&params, &commState);
    if (hlseret != HLSE_SW_OK) {
        LOG_E("Failed to establish connection to Secure Module");
        while (1)
            ;
    }
#endif

    LOG_I("AWS subscribe publish example\n\n");

#if AX_EMBEDDED
    BOARD_InitNetwork_MAC(AxUID);
#endif

    awsPubSub(AxUID);
}

void demoPubCallbackHandler(MQTTContext_t * pxMqttContext,
                              MQTTPacketInfo_t * pxPacketInfo,
                              MQTTDeserializedInfo_t * pxDeserializedInfo)
{
    /* Received a published message */
    HandleReceivedMsg((char *)pxDeserializedInfo->pPublishInfo->pPayload, pxDeserializedInfo->pPublishInfo->payloadLength);

}

void awsPubSub(const U8 ax_uid[MODULE_UNIQUE_ID_LEN])
{
    BaseType_t xResult =pdFAIL;
    char cPayload[100];
    int i = 0;

    uint8_t ucSharedBuffer[ democonfigNETWORK_BUFFER_SIZE ];

    MQTTContext_t xMqttContext;
    NetworkContext_t xNetworkContext;
    MQTTFixedBuffer_t xNetworkBuffer = {
        .pBuffer = ucSharedBuffer,
        .size    = democonfigNETWORK_BUFFER_SIZE
    };

    xResult = EstablishMqttSession( &xMqttContext,
                                     &xNetworkContext,
                                     &xNetworkBuffer,
                                     prvEventCallback);
    if (xResult != pdPASS) {
        LOG_E("Failed to establish MQTT connection");
        goto exit;
    }

    sprintf(cPayload, "{\"msg\" : \"%s : %d\"}", "hello from SDK QOS0", (int)i++);

    LOG_I("MQTT Connection successfully established");
    xResult = SubscribeToTopic( &xMqttContext,
            SUB_TOPIC,
            strlen(SUB_TOPIC) );
    if (xResult != pdPASS) {
        LOG_E("MQTT Failed to subscribe");
        goto exit;
    }

    LOG_I("MQTT Subscribed");
    xResult = PublishToTopic( &xMqttContext,
            PUB_TOPIC,
            strlen(PUB_TOPIC),
            cPayload,
            strlen(cPayload) );
    if (xResult != pdPASS) {
        LOG_E("Failed to publish message %s", cPayload);
        goto exit;
    }
    LOG_I("Published message %s", cPayload);

exit:
    if (xResult != pdPASS) {
        LOG_E("AWS demo failed");
    }
    else {
        LOG_I("AWS demo passed");
    }
    LOG_I("Demo Example Finished");
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
