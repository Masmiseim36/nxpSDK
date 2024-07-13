/* Copyright 2019-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef NXP_IOT_AGENT_ENABLE_LITE

#include <nxp_iot_agent_macros.h>
#include <nxp_iot_agent.h>
#include <iot_agent_mqtt_freertos.h>

#if NXP_IOT_AGENT_HAVE_SSS
#include <fsl_sss_api.h>
#endif

#include "core_mqtt_serializer.h"
#include "core_mqtt.h"

#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
#include "using_mbedtls.h"
#else
#include "transport_secure_sockets.h"
#include "iot_secure_sockets.h"
#include "iot_default_root_certificates.h"
#include "iot_init.h"
#endif

#ifdef NXP_IOT_AGENT_USE_COREJSON
#include "core_json.h"
#else
#include "jsmn.h"
#endif

#include <mbedtls/pk.h>


#if SSS_HAVE_MBEDTLS_ALT_SSS
#include "sss_mbedtls.h"
#include "ex_sss_boot.h"
#include "sm_types.h"
#include "nxLog_App.h"
#endif


#if NXP_IOT_AGENT_HAVE_SSS
#include <fsl_sss_api.h>
static iot_agent_status_t associateKeyPair(mbedtls_pk_context *pk, sss_object_t* service_private_key,
        iot_agent_keystore_t* keystore, uint32_t key_id);

static iot_agent_status_t write_cert_to_keystore(iot_agent_keystore_t* keystore,
		const nxp_iot_ServiceDescriptor* service_descriptor, uint32_t cert_id);

static iot_agent_status_t pubSub(iot_agent_context_t* iot_agent_context,
		const nxp_iot_ServiceDescriptor* service_descriptor);
extern ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx;
#endif

static iot_agent_status_t awsPubMqttMessage(const nxp_iot_ServiceDescriptor* service_descriptor);

static iot_agent_status_t customPubMqttMessage(const nxp_iot_ServiceDescriptor* service_descriptor);

static iot_agent_status_t azurePubMqttMessage(const nxp_iot_ServiceDescriptor* service_descriptor);

#if	(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)
static iot_agent_status_t pubSubCosOverRtp(iot_agent_context_t* iot_agent_context,
		const nxp_iot_ServiceDescriptor* service_descriptor);
#endif

typedef enum { NOT_ASSIGNED, ASSIGNING, ASSIGNED } registration_state;

#define mqttexampleCONNACK_RECV_TIMEOUT_MS      	1000U
#define mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS   200U
#define mqttexampleKEEP_ALIVE_TIMEOUT_SECONDS      	60U
#define mqttexampleKEEP_ALIVE_ACTUAL_INTERVAL_MS   	10000U
#define mqttexamplePUBLISH_INTERVAL_MS    			1000U
#define mqttexamplePUBLISH_WAIT_INTERVAL_MS   		5000U
#define MILLISECONDS_PER_SECOND                   	1000U
#define MILLISECONDS_PER_TICK                      	(MILLISECONDS_PER_SECOND / configTICK_RATE_HZ)
#define AGENT_NETWORK_BUFFER_SIZE 					1024U

#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
#define AWS_IOT_MQTT_ALPN "\x0ex-amzn-mqtt-ca"
#define AWS_IOT_CUSTOM_AUTH_ALPN "\x04mqtt"
#else
struct NetworkContext
{
    SecureSocketsTransportParams_t * pParams;
};

static SecureSocketsTransportParams_t secureSocketsTransportParams;
#endif

typedef struct MqttAgent_Context {
	MQTTContext_t* pMqttContext;
    NetworkContext_t* pNetworkContext;
	MQTTFixedBuffer_t* pNetworkBuffer;
	TransportInterface_t* pTransportInterface;
	MQTTEventCallback_t prvUserCallback;
	const char* pHostName;
	uint16_t port;
	const char* pRootCa;
	size_t rootCaSize;
	const char* pClientId;
	const char* pUserName;
	const char* pMqttPublishTopic;
	const char* pMqttPublishPayload;
	const char* pMqttSubscribeTopicFilter;
	bool tlsConnected;
	bool mqttConnected;
}MqttAgent_Context_t;


#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
uint32_t key_id = 0U;
uint32_t cert_id = 0U;

#if (NXP_IOT_AGENT_HAVE_PSA == 0)
static void iot_agent_convert_id_to_label(char* label, size_t label_size, uint32_t id);
#endif // (NXP_IOT_AGENT_HAVE_PSA == 0)
#endif // NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
#if NXP_IOT_AGENT_HAVE_SSS
// doc: trigger MQTT connection freertos - start
iot_agent_status_t iot_agent_verify_mqtt_connection_for_service(iot_agent_context_t* iot_agent_context,
        const nxp_iot_ServiceDescriptor* service_descriptor)
{
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    agent_status = pubSub(iot_agent_context, service_descriptor);
    AGENT_SUCCESS_OR_EXIT_MSG("MQTT connection test failed");
exit:
    return agent_status;
}

iot_agent_status_t iot_agent_verify_mqtt_connection(iot_agent_context_t* iot_agent_context)
{
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    size_t number_of_services = 0U;
	nxp_iot_ServiceDescriptor service_descriptor = nxp_iot_ServiceDescriptor_init_default;

    number_of_services = iot_agent_get_number_of_services(iot_agent_context);
    for (size_t i = 0U; i < number_of_services; i++)
    {
		agent_status = iot_agent_select_service_by_index(iot_agent_context, i, &service_descriptor);
		AGENT_SUCCESS_OR_EXIT();

		agent_status = iot_agent_verify_mqtt_connection_for_service(iot_agent_context, &service_descriptor);
		AGENT_SUCCESS_OR_EXIT();
	}
exit:
	iot_agent_free_service_descriptor(&service_descriptor);
    return agent_status;
}
// doc: trigger MQTT connection freertos - end

iot_agent_status_t iot_agent_cleanup_mqtt_config_files(void)
{
	return IOT_AGENT_SUCCESS;
}
#endif

#if	(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)
iot_agent_status_t iot_agent_verify_mqtt_connection_cos_over_rtp(iot_agent_context_t* iot_agent_context,
		const nxp_iot_ServiceDescriptor* service_descriptor)
{
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    agent_status = pubSubCosOverRtp(iot_agent_context, service_descriptor);
    AGENT_SUCCESS_OR_EXIT_MSG("MQTT connection test failed");
exit:
    return agent_status;
}

iot_agent_status_t iot_agent_cleanup_mqtt_config_files_cos_over_rtp(void)
{
	return IOT_AGENT_SUCCESS;
}

static iot_agent_status_t pubSubCosOverRtp(iot_agent_context_t* iot_agent_context,
        const nxp_iot_ServiceDescriptor* service_descriptor)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
	key_id = (uint32_t) (service_descriptor->client_key_sss_ref.object_id);
	cert_id = (uint32_t) (service_descriptor->client_certificate_sss_ref.object_id);
#else

	iot_agent_keystore_t* keystore = NULL;

	uint32_t key_id = (uint32_t) (service_descriptor->client_key_sss_ref.object_id);
	uint32_t cert_id = (uint32_t) (service_descriptor->client_certificate_sss_ref.object_id);

	uint32_t keystore_id = service_descriptor->client_key_sss_ref.endpoint_id;
	agent_status = iot_agent_get_keystore_by_id(iot_agent_context, keystore_id, &keystore);
	AGENT_SUCCESS_OR_EXIT();

    mbedtls_pk_context pk;
	mbedtls_pk_init(&pk);
#if NXP_IOT_AGENT_HAVE_SSS
	agent_status = associateKeyPair(&pk, keystore, key_id);
	AGENT_SUCCESS_OR_EXIT();

	pex_sss_demo_tls_ctx->client_cert_index = cert_id;
#endif
#if NXP_IOT_AGENT_HAVE_PSA
	psa_iot_tls_context.keyId = key_id;
	psa_iot_tls_context.certId = cert_id;
#endif

    BaseType_t sdk_result;
    sdk_result = IotSdk_Init();
	ASSERT_OR_EXIT_MSG(sdk_result == pdPASS, "\nFailure at IotSdk_Init\n");
#endif

    if(service_descriptor->service_type == nxp_iot_ServiceType_AWSSERVICE)
	{
		agent_status = awsPubMqttMessage(service_descriptor);
		AGENT_SUCCESS_OR_EXIT();
	}
    else if (service_descriptor->service_type == nxp_iot_ServiceType_AZURESERVICE)
    {
        agent_status = azurePubMqttMessage(service_descriptor);
        AGENT_SUCCESS_OR_EXIT();
    }
    else if (service_descriptor->service_type == nxp_iot_ServiceType_CUSTOMSERVICE)
    {
        agent_status = customPubMqttMessage(service_descriptor);
        AGENT_SUCCESS_OR_EXIT();
    }
	else
	{
		EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "Invalid service type.");
	}

exit:
#ifndef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
	IotSdk_Cleanup();
	mbedtls_pk_free(&pk);
#endif
	return agent_status;
}

#endif

#if NXP_IOT_AGENT_HAVE_SSS

iot_agent_status_t pubSub(iot_agent_context_t* iot_agent_context,
        const nxp_iot_ServiceDescriptor* service_descriptor)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	iot_agent_keystore_t* keystore = NULL;

	key_id = (uint32_t) (service_descriptor->identifier | 0x80000000);
	cert_id = (uint32_t) (service_descriptor->identifier | 0x40000000);

	uint32_t keystore_id = service_descriptor->client_key_sss_ref.endpoint_id;
	agent_status = iot_agent_get_keystore_by_id(iot_agent_context, keystore_id, &keystore);
	AGENT_SUCCESS_OR_EXIT();

	agent_status = write_cert_to_keystore(keystore, service_descriptor, cert_id);
	AGENT_SUCCESS_OR_EXIT();

    if(service_descriptor->service_type == nxp_iot_ServiceType_AWSSERVICE)
	{
		agent_status = awsPubMqttMessage(service_descriptor);
		AGENT_SUCCESS_OR_EXIT();
	}
    else if (service_descriptor->service_type == nxp_iot_ServiceType_AZURESERVICE)
    {
        agent_status = azurePubMqttMessage(service_descriptor);
        AGENT_SUCCESS_OR_EXIT();
    }
    else if (service_descriptor->service_type == nxp_iot_ServiceType_CUSTOMSERVICE)
    {
        agent_status = customPubMqttMessage(service_descriptor);
        AGENT_SUCCESS_OR_EXIT();
    }
	else
	{
		EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "Invalid service type.");
	}

exit:
	return agent_status;
}

iot_agent_status_t write_cert_to_keystore(iot_agent_keystore_t* keystore,
		const nxp_iot_ServiceDescriptor* service_descriptor, uint32_t cert_id)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	sss_status_t sss_status = kStatus_SSS_Success;
	sss_object_t service_cert = { 0 };

	pex_sss_demo_tls_ctx->client_cert_index = cert_id;

    sss_status = sss_key_object_init(&service_cert, keystore->sss_context);
    SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_init for keyPair failed with 0x%08x", sss_status)

    size_t cert_size = service_descriptor->client_certificate->size;
    const uint8_t *cert_data = service_descriptor->client_certificate->bytes;

    sss_status = sss_key_object_get_handle(&service_cert, cert_id);
    if (sss_status != kStatus_SSS_Success)
    {
    	sss_status = sss_key_object_allocate_handle(&service_cert, cert_id,
        		kSSS_KeyPart_Default, kSSS_CipherType_Binary, cert_size, kKeyObject_Mode_Persistent);
    	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_allocate_handle failed with 0x%08x.", sss_status);
    }
    else
    {
    	sss_status = sss_key_store_erase_key(keystore->sss_context, &service_cert);
    	SSS_SUCCESS_OR_EXIT_MSG("sss_key_store_erase_key failed with 0x%08x.", sss_status);
    }

    sss_status = sss_key_store_set_key(keystore->sss_context, &service_cert, cert_data, cert_size,
    		cert_size * 8, NULL, 0);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_store_set_key failed with 0x%08x.", sss_status);
exit:
	return agent_status;
}

iot_agent_status_t associateKeyPair(mbedtls_pk_context *pk, sss_object_t* service_private_key,
        iot_agent_keystore_t* keystore, uint32_t key_id)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	sss_status_t sss_status = kStatus_SSS_Success;
	int ret;

    pex_sss_demo_tls_ctx->client_keyPair_index = key_id;

    sss_status = sss_key_object_init(service_private_key, keystore->sss_context);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_init failed with 0x%08x.", sss_status);

    sss_status = sss_key_object_get_handle(service_private_key, key_id);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_get_handle failed with 0x%08x.", sss_status);

    ret = sss_mbedtls_associate_keypair(pk, service_private_key);
    ASSERT_OR_EXIT_MSG(ret == 0, "sss_mbedtls_associate_keypair failed with 0x%08x.", ret)

exit:
    return agent_status;
}
#endif

#define AWS_MQTT_TOPIC "sdk/test/cpp"
#define MQTT_DATA  "Hello from FreeRTOS"

static const size_t PUBLISH_ATTEMPTS = 4U;

static uint32_t prvGetTimeMs( void )
{
    TickType_t xTickCount = 0;
    uint32_t ulTimeMs = 0UL;

    /* Get the current tick count. */
    xTickCount = xTaskGetTickCount();

    /* Convert the ticks to milliseconds. */
    ulTimeMs = ( uint32_t ) xTickCount * MILLISECONDS_PER_TICK;

    return ulTimeMs;
}

static void mqttUserCallback(MQTTContext_t * pxMqttContext,
                             MQTTPacketInfo_t * pxPacketInfo,
                             MQTTDeserializedInfo_t * pxDeserializedInfo )
{
	// in case of AWS and Custom Cloud, the reception of the packets from server is not used
	// just send out the notification in case a packet is received
	IOT_AGENT_DEBUG("MQTT packet received from server\n\r");
}

static iot_agent_status_t iot_agent_demo_mqtt_init(MqttAgent_Context_t* pMqttAgentContext)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

    ASSERT_OR_EXIT_MSG(pMqttAgentContext != NULL, "MQTT context pointer is null");
    memset(pMqttAgentContext, 0, sizeof(MqttAgent_Context_t));
    pMqttAgentContext->tlsConnected = false;
    pMqttAgentContext->mqttConnected = false;

 exit:
	return agent_status;
}

static iot_agent_status_t iot_agent_demo_mqtt_disconnect(MqttAgent_Context_t* pMqttAgentContext)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    MQTTStatus_t mqttStatus = MQTTSuccess;
#ifndef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
    TransportSocketStatus_t networkStatus = TRANSPORT_SOCKET_STATUS_SUCCESS;
#endif

    ASSERT_OR_EXIT_MSG(pMqttAgentContext != NULL, "MQTT context pointer is null");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pMqttContext != NULL, "Error in the MQTT Context parameters");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pNetworkContext != NULL, "Error in the MQTT Context parameters");

	if(pMqttAgentContext->mqttConnected == true) {
		mqttStatus = MQTT_Disconnect(pMqttAgentContext->pMqttContext);

		if(mqttStatus != MQTTSuccess )
		{
			IOT_AGENT_ERROR("MQTT_Disconnect failed with status=%s.", MQTT_Status_strerror(mqttStatus));
			agent_status = IOT_AGENT_FAILURE;
		}
	}

	if(pMqttAgentContext->tlsConnected == true )
	{
#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
    	TLS_FreeRTOS_Disconnect(pMqttAgentContext->pNetworkContext);
#else
		networkStatus = SecureSocketsTransport_Disconnect(pMqttAgentContext->pNetworkContext);

		if(networkStatus != TRANSPORT_SOCKET_STATUS_SUCCESS )
		{
			IOT_AGENT_ERROR("Secure Socket Disconnect failed");
			agent_status = IOT_AGENT_FAILURE;
		}
#endif
	}
exit:
	return agent_status;
}

static iot_agent_status_t iot_agent_demo_mqtt_connect(MqttAgent_Context_t* pMqttAgentContext)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    MQTTStatus_t mqttStatus = MQTTSuccess;
    MQTTConnectInfo_t connectInfo = { 0 };
    bool previousSession = false;
#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
    TlsTransportStatus_t xNetworkStatus = TLS_TRANSPORT_CONNECT_FAILURE;
    NetworkCredentials_t xNetworkCredentials = { 0 };
    const char * pcAlpnProtocols[] = { NULL, NULL };
#if NXP_IOT_AGENT_HAVE_PSA == 0
	char certLabel[20] = {'\0'};
    char keyLabel[20] = {'\0'};
#endif
#else
    TransportSocketStatus_t networkStatus = TRANSPORT_SOCKET_STATUS_SUCCESS;
    ServerInfo_t serverInfo = { 0 };
    SocketsConfig_t socketConfig = { 0 };
    BaseType_t mqtt_result = pdPASS;
#endif

    ASSERT_OR_EXIT_MSG(pMqttAgentContext != NULL, "MQTT context pointer is null");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pMqttContext != NULL, "Error in the MQTT Context parameters");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pNetworkContext != NULL, "Error in the MQTT Context parameters");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pNetworkBuffer != NULL, "Error in the MQTT Context parameters");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pTransportInterface != NULL, "Error in the MQTT Context parameters");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pHostName != NULL, "Error in the MQTT Context parameters");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pRootCa != NULL, "Error in the MQTT Context parameters");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pClientId != NULL, "Error in the MQTT Context parameters");

#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
   	pcAlpnProtocols[ 0 ] = AWS_IOT_MQTT_ALPN;
    xNetworkCredentials.pAlpnProtos = pcAlpnProtocols;
    /* Set the credentials for establishing a TLS connection. */
    xNetworkCredentials.pRootCa = ( unsigned char * )pMqttAgentContext->pRootCa;
    xNetworkCredentials.rootCaSize = pMqttAgentContext->rootCaSize;
	xNetworkCredentials.disableSni = pdFALSE;
#if NXP_IOT_AGENT_HAVE_PSA
	xNetworkCredentials.certId = cert_id;
	xNetworkCredentials.keyId = key_id;
#else
	iot_agent_convert_id_to_label(certLabel, sizeof(certLabel), cert_id);
	iot_agent_convert_id_to_label(keyLabel, sizeof(keyLabel), key_id);
	xNetworkCredentials.pClientCertLabel = certLabel;
	xNetworkCredentials.pPrivateKeyLabel = keyLabel;
#endif
    xNetworkStatus = TLS_FreeRTOS_Connect( pMqttAgentContext->pNetworkContext,
                                           pMqttAgentContext->pHostName,
                                           pMqttAgentContext->port,
                                           &xNetworkCredentials,
                                           mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS,
                                           mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS );
	pMqttAgentContext->tlsConnected = (xNetworkStatus == TLS_TRANSPORT_SUCCESS);
	if (!pMqttAgentContext->tlsConnected) {
		TLS_FreeRTOS_Disconnect( pMqttAgentContext->pNetworkContext );
		EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "Secure Socket Connect failed: Exiting");
	}
#else
    mqtt_result = SOCKETS_Init();
    ASSERT_OR_EXIT_MSG(pdPASS == mqtt_result, "SOCKETS_Init failed with 0x%08lx.", mqtt_result);

    pMqttAgentContext->pNetworkContext->pParams = &secureSocketsTransportParams;
    serverInfo.pHostName = pMqttAgentContext->pHostName;
    serverInfo.hostNameLength = strlen(pMqttAgentContext->pHostName);
    serverInfo.port = pMqttAgentContext->port;

    socketConfig.enableTls = true;

    if(serverInfo.port == 443)
    {
        socketConfig.pAlpnProtos = socketsAWS_IOT_ALPN_MQTT;
    }

    socketConfig.maxFragmentLength = 0;
    socketConfig.disableSni = false;
    socketConfig.sendTimeoutMs = mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS;
    socketConfig.recvTimeoutMs = mqttexampleTRANSPORT_SEND_RECV_TIMEOUT_MS;
    socketConfig.pRootCa = pMqttAgentContext->pRootCa;
    socketConfig.rootCaSize = pMqttAgentContext->rootCaSize;

	networkStatus = SecureSocketsTransport_Connect(pMqttAgentContext->pNetworkContext,
												   &serverInfo,
												   &socketConfig );

	pMqttAgentContext->tlsConnected = (networkStatus == TRANSPORT_SOCKET_STATUS_SUCCESS);
	ASSERT_OR_EXIT_MSG(pMqttAgentContext->tlsConnected, "Secure Socket Connect failed: Exiting");
#endif
    mqttStatus = MQTT_Init(pMqttAgentContext->pMqttContext,
    					   pMqttAgentContext->pTransportInterface,
                           prvGetTimeMs,
						   pMqttAgentContext->prvUserCallback,
                           pMqttAgentContext->pNetworkBuffer);
	ASSERT_OR_EXIT_MSG(MQTTSuccess == mqttStatus, "MQTT_Init failed with status=%s.", MQTT_Status_strerror(mqttStatus));

	connectInfo.cleanSession = true;
	connectInfo.pClientIdentifier = pMqttAgentContext->pClientId;
	connectInfo.clientIdentifierLength = strlen(pMqttAgentContext->pClientId);
	connectInfo.pUserName = pMqttAgentContext->pUserName;
	if (pMqttAgentContext->pUserName == NULL) {
		connectInfo.userNameLength = 0;
	} else {
		connectInfo.userNameLength =  strlen(pMqttAgentContext->pUserName);
	}

	connectInfo.pPassword = NULL;
	connectInfo.passwordLength = 0;
	connectInfo.keepAliveSeconds = mqttexampleKEEP_ALIVE_TIMEOUT_SECONDS;

	mqttStatus = MQTT_Connect(pMqttAgentContext->pMqttContext,
							  &connectInfo,
							  NULL,
							  mqttexampleCONNACK_RECV_TIMEOUT_MS,
							  &previousSession);
	pMqttAgentContext->mqttConnected = (mqttStatus == MQTTSuccess);
	if (! pMqttAgentContext->mqttConnected) {
		agent_status = iot_agent_demo_mqtt_disconnect(pMqttAgentContext);
	}
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->mqttConnected, "MQTT_Connect failed with status=%s.", MQTT_Status_strerror(mqttStatus));
exit:
	return agent_status;
}

static iot_agent_status_t iot_agent_demo_mqtt_publish(MqttAgent_Context_t* pMqttAgentContext)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    MQTTStatus_t mqttStatus = MQTTSuccess;
    MQTTPublishInfo_t pubInfo;
    uint16_t packetId = 0U;

    ASSERT_OR_EXIT_MSG(pMqttAgentContext != NULL, "MQTT context pointer is null");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pMqttContext != NULL, "Error in the MQTT Context parameters");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pMqttPublishTopic != NULL, "Error in the MQTT Context parameters");

	memset(&pubInfo, 0, sizeof(MQTTPublishInfo_t));
    pubInfo.pTopicName = pMqttAgentContext->pMqttPublishTopic;
	size_t topicNameLength = strlen(pMqttAgentContext->pMqttPublishTopic);
	ASSERT_OR_EXIT_MSG(topicNameLength <= UINT16_MAX, "Error in topic name length.");
	pubInfo.topicNameLength = topicNameLength;
	pubInfo.pPayload = pMqttAgentContext->pMqttPublishPayload;
	if (pMqttAgentContext->pMqttPublishPayload == NULL) {
		pubInfo.payloadLength = 0;
	} else {
		pubInfo.payloadLength = strlen(pMqttAgentContext->pMqttPublishPayload);
	}
	pubInfo.qos = MQTTQoS0;

	packetId = MQTT_GetPacketId(pMqttAgentContext->pMqttContext);

	mqttStatus = MQTT_Publish(pMqttAgentContext->pMqttContext,
                              &pubInfo,
						   packetId);
	ASSERT_OR_EXIT_MSG(mqttStatus == MQTTSuccess, "MQTT_Publish failed with status=%s.", MQTT_Status_strerror(mqttStatus));
exit:
    return agent_status;
}

static iot_agent_status_t iot_agent_demo_mqtt_wait_packet(MqttAgent_Context_t* pMqttAgentContext) {
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    MQTTStatus_t mqttStatus = MQTTSuccess;

    ASSERT_OR_EXIT_MSG(pMqttAgentContext != NULL, "MQTT context pointer is null");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pMqttContext != NULL, "Error in the MQTT Context parameters");


#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
    mqttStatus = MQTT_ProcessLoop(pMqttAgentContext->pMqttContext);
#else
    mqttStatus = MQTT_ProcessLoop(pMqttAgentContext->pMqttContext, 0U);
#endif
	ASSERT_OR_EXIT_MSG(MQTTSuccess == mqttStatus, "MQTT_ProcessLoop failed with status=%s.", MQTT_Status_strerror(mqttStatus));
exit:
	return agent_status;
}

static iot_agent_status_t iot_agent_demo_mqtt_subscribe(MqttAgent_Context_t* pMqttAgentContext)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    MQTTStatus_t mqttStatus = MQTTSuccess;
    MQTTSubscribeInfo_t xMQTTSubscription;
    uint16_t packetId = 0U;

    ASSERT_OR_EXIT_MSG(pMqttAgentContext != NULL, "MQTT context pointer is null");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pMqttContext != NULL, "Error in the MQTT Context parameters");
    ASSERT_OR_EXIT_MSG(pMqttAgentContext->pMqttSubscribeTopicFilter != NULL, "Error in the MQTT Context parameters");

    xMQTTSubscription.qos = MQTTQoS0;
    xMQTTSubscription.pTopicFilter = pMqttAgentContext->pMqttSubscribeTopicFilter;
    xMQTTSubscription.topicFilterLength = strlen(pMqttAgentContext->pMqttSubscribeTopicFilter);
	packetId = MQTT_GetPacketId(pMqttAgentContext->pMqttContext);

    mqttStatus = MQTT_Subscribe(pMqttAgentContext->pMqttContext,
                                 &xMQTTSubscription,
                                 sizeof(xMQTTSubscription )/sizeof(MQTTSubscribeInfo_t),
								 packetId);

    ASSERT_OR_EXIT_MSG(MQTTSuccess == mqttStatus, "MQTT_Subscribe failed with status=%s.", MQTT_Status_strerror(mqttStatus));
exit:
    return agent_status;
}


#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
static const char tlsATS1_ROOT_CERTIFICATE_PEM[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n"
    "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"
    "b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n"
    "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n"
    "b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n"
    "ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n"
    "9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n"
    "IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n"
    "VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n"
    "93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n"
    "jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n"
    "AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n"
    "A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n"
    "U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n"
    "N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n"
    "o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n"
    "5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n"
    "rqXRfboQnoZsG4q5WTP468SQvvG5\n"
    "-----END CERTIFICATE-----\n";
static const uint32_t tlsATS1_ROOT_CERTIFICATE_LENGTH = sizeof( tlsATS1_ROOT_CERTIFICATE_PEM );

#endif

static iot_agent_status_t awsPubMqttMessage(const nxp_iot_ServiceDescriptor* service_descriptor)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    uint8_t sharedBuffer[AGENT_NETWORK_BUFFER_SIZE];
    MQTTContext_t mqttContext = { 0 };
    NetworkContext_t networkContext = { 0 };
    MQTTFixedBuffer_t networkBuffer = {
        .pBuffer = sharedBuffer,
        .size    = AGENT_NETWORK_BUFFER_SIZE
    };
    TransportInterface_t transportInterface = { 0 };
	size_t publishFails = 0;
	size_t publishCount = PUBLISH_ATTEMPTS;

    MqttAgent_Context_t mqttAgentContext = { 0 };
    size_t retryCount = 0U;
	agent_status = iot_agent_demo_mqtt_init(&mqttAgentContext);
    AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT Agent Context initialization");

    transportInterface.pNetworkContext = &networkContext;

#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
    transportInterface.send = TLS_FreeRTOS_Send;
    transportInterface.recv = TLS_FreeRTOS_Recv;
    mqttAgentContext.pRootCa = tlsATS1_ROOT_CERTIFICATE_PEM;
    mqttAgentContext.rootCaSize = tlsATS1_ROOT_CERTIFICATE_LENGTH;
#else
    networkContext.pParams = &secureSocketsTransportParams;
    transportInterface.send = SecureSocketsTransport_Send;
    transportInterface.recv = SecureSocketsTransport_Recv;
    mqttAgentContext.pRootCa = tlsCombi_ROOT_CERTIFICATE_PEM;
    mqttAgentContext.rootCaSize = tlsCombi_ROOT_CERTIFICATE_LENGTH;
#endif

    mqttAgentContext.pMqttContext = &mqttContext;
    mqttAgentContext.pNetworkContext = &networkContext;
    mqttAgentContext.pNetworkBuffer = &networkBuffer;
    mqttAgentContext.pTransportInterface = &transportInterface;
    mqttAgentContext.prvUserCallback = mqttUserCallback;
    mqttAgentContext.pHostName = service_descriptor->hostname;
    mqttAgentContext.port = service_descriptor->port;
    mqttAgentContext.pClientId = service_descriptor->client_id;
    mqttAgentContext.pUserName = NULL;

	while (retryCount < 5U) {
		IOT_AGENT_INFO("Attempt %d for connecting to AWS service '%s'...", retryCount + 1, service_descriptor->client_id);
		agent_status = iot_agent_demo_mqtt_connect(&mqttAgentContext);
		if (agent_status == IOT_AGENT_SUCCESS) {
			break;
		}

        vTaskDelay(pdMS_TO_TICKS(mqttexampleKEEP_ALIVE_ACTUAL_INTERVAL_MS * (1U + retryCount)));
        retryCount++;
	}

	AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT connection");

	mqttAgentContext.pMqttPublishTopic = AWS_MQTT_TOPIC;

	mqttAgentContext.pMqttPublishPayload = MQTT_DATA;

	while (publishCount > 0U) {
		vTaskDelay(pdMS_TO_TICKS(mqttexamplePUBLISH_INTERVAL_MS));
		agent_status = iot_agent_demo_mqtt_publish(&mqttAgentContext);

    	if (agent_status == IOT_AGENT_SUCCESS) {
    		IOT_AGENT_INFO("Echo successfully published");
    	}
		else {
			IOT_AGENT_INFO("ERROR: Echo failed to publish");
			publishFails++;
		}

		publishCount--;
	}

	ASSERT_OR_EXIT_MSG(publishFails < (PUBLISH_ATTEMPTS / 2U),
			"More than or equal to %d publish attempts failed (%d).", (PUBLISH_ATTEMPTS / 2), publishFails);

exit:
	agent_status = iot_agent_demo_mqtt_disconnect(&mqttAgentContext);
    return agent_status;
}

#define WATSON_PUB_TOPIC "iot-2/evt/status/fmt/string"

#define CUSTOM_MQTT_USER_NAME "use-token-auth"
#define WATSON_ECHO_CLIENT_ID "dummy" //For IBM watson ClientID is optional (IBM verifies client from Device certificate)
#define WATSON_IOT_CLIENT_ID ((const char *)WATSON_ECHO_CLIENT_ID)

static const char tlsVERISIGN_ROOT_CERT_WATSON_PEM[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIElDCCA3ygAwIBAgIQAf2j627KdciIQ4tyS8+8kTANBgkqhkiG9w0BAQsFADBh\r\n"
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n"
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\r\n"
"QTAeFw0xMzAzMDgxMjAwMDBaFw0yMzAzMDgxMjAwMDBaME0xCzAJBgNVBAYTAlVT\r\n"
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxJzAlBgNVBAMTHkRpZ2lDZXJ0IFNIQTIg\r\n"
"U2VjdXJlIFNlcnZlciBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\r\n"
"ANyuWJBNwcQwFZA1W248ghX1LFy949v/cUP6ZCWA1O4Yok3wZtAKc24RmDYXZK83\r\n"
"nf36QYSvx6+M/hpzTc8zl5CilodTgyu5pnVILR1WN3vaMTIa16yrBvSqXUu3R0bd\r\n"
"KpPDkC55gIDvEwRqFDu1m5K+wgdlTvza/P96rtxcflUxDOg5B6TXvi/TC2rSsd9f\r\n"
"/ld0Uzs1gN2ujkSYs58O09rg1/RrKatEp0tYhG2SS4HD2nOLEpdIkARFdRrdNzGX\r\n"
"kujNVA075ME/OV4uuPNcfhCOhkEAjUVmR7ChZc6gqikJTvOX6+guqw9ypzAO+sf0\r\n"
"/RR3w6RbKFfCs/mC/bdFWJsCAwEAAaOCAVowggFWMBIGA1UdEwEB/wQIMAYBAf8C\r\n"
"AQAwDgYDVR0PAQH/BAQDAgGGMDQGCCsGAQUFBwEBBCgwJjAkBggrBgEFBQcwAYYY\r\n"
"aHR0cDovL29jc3AuZGlnaWNlcnQuY29tMHsGA1UdHwR0MHIwN6A1oDOGMWh0dHA6\r\n"
"Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEdsb2JhbFJvb3RDQS5jcmwwN6A1\r\n"
"oDOGMWh0dHA6Ly9jcmw0LmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEdsb2JhbFJvb3RD\r\n"
"QS5jcmwwPQYDVR0gBDYwNDAyBgRVHSAAMCowKAYIKwYBBQUHAgEWHGh0dHBzOi8v\r\n"
"d3d3LmRpZ2ljZXJ0LmNvbS9DUFMwHQYDVR0OBBYEFA+AYRyCMWHVLyjnjUY4tCzh\r\n"
"xtniMB8GA1UdIwQYMBaAFAPeUDVW0Uy7ZvCj4hsbw5eyPdFVMA0GCSqGSIb3DQEB\r\n"
"CwUAA4IBAQAjPt9L0jFCpbZ+QlwaRMxp0Wi0XUvgBCFsS+JtzLHgl4+mUwnNqipl\r\n"
"5TlPHoOlblyYoiQm5vuh7ZPHLgLGTUq/sELfeNqzqPlt/yGFUzZgTHbO7Djc1lGA\r\n"
"8MXW5dRNJ2Srm8c+cftIl7gzbckTB+6WohsYFfZcTEDts8Ls/3HB40f/1LkAtDdC\r\n"
"2iDJ6m6K7hQGrn2iWZiIqBtvLfTyyRRfJs8sjX7tN8Cp1Tm5gr8ZDOo0rwAhaPit\r\n"
"c+LJMto4JQtV05od8GiG7S5BNO98pVAdvzr508EIDObtHopYJeS4d60tbvVS3bR0\r\n"
"j6tJLp07kzQoH3jOlOrHvdPJbRzeXDLz\r\n"
"-----END CERTIFICATE-----\r\n";
static const uint32_t tlsVERISIGN_ROOT_CERT_WATSON_LENGTH = sizeof(tlsVERISIGN_ROOT_CERT_WATSON_PEM);

static iot_agent_status_t customPubMqttMessage(const nxp_iot_ServiceDescriptor* service_descriptor)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    uint8_t sharedBuffer[AGENT_NETWORK_BUFFER_SIZE];
    MQTTContext_t mqttContext = { 0 };
    NetworkContext_t networkContext = { 0 };
    MQTTFixedBuffer_t networkBuffer = {
        .pBuffer = sharedBuffer,
        .size    = AGENT_NETWORK_BUFFER_SIZE
    };
    TransportInterface_t transportInterface = { 0 };
	size_t publishFails = 0;
	size_t publishCount = PUBLISH_ATTEMPTS;

    MqttAgent_Context_t mqttAgentContext = { 0 };
    size_t retryCount = 0U;
	agent_status = iot_agent_demo_mqtt_init(&mqttAgentContext);
    AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT Agent Context initialization");

    transportInterface.pNetworkContext = &networkContext;

#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
    transportInterface.send = TLS_FreeRTOS_Send;
    transportInterface.recv = TLS_FreeRTOS_Recv;
#else
    networkContext.pParams = &secureSocketsTransportParams;
    transportInterface.send = SecureSocketsTransport_Send;
    transportInterface.recv = SecureSocketsTransport_Recv;
#endif

    mqttAgentContext.pMqttContext = &mqttContext;
    mqttAgentContext.pNetworkContext = &networkContext;
    mqttAgentContext.pNetworkBuffer = &networkBuffer;
    mqttAgentContext.pTransportInterface = &transportInterface;
    mqttAgentContext.prvUserCallback = mqttUserCallback;
    mqttAgentContext.pHostName = service_descriptor->hostname;
    mqttAgentContext.port = service_descriptor->port;
    mqttAgentContext.pRootCa = tlsVERISIGN_ROOT_CERT_WATSON_PEM;
    mqttAgentContext.rootCaSize = tlsVERISIGN_ROOT_CERT_WATSON_LENGTH;
    mqttAgentContext.pClientId = WATSON_IOT_CLIENT_ID;
    mqttAgentContext.pUserName = CUSTOM_MQTT_USER_NAME;

	while (retryCount < 5U) {
		IOT_AGENT_INFO("Attempt %d for connecting to AWS service '%s'...", retryCount + 1U, service_descriptor->client_id);
		agent_status = iot_agent_demo_mqtt_connect(&mqttAgentContext);
		if (agent_status == IOT_AGENT_SUCCESS) {
			break;
		}

        vTaskDelay(pdMS_TO_TICKS(mqttexampleKEEP_ALIVE_ACTUAL_INTERVAL_MS * (1U + 2U*retryCount)));
        retryCount++;
	}

	AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT connection");

	mqttAgentContext.pMqttPublishTopic = WATSON_PUB_TOPIC;
	mqttAgentContext.pMqttPublishPayload = MQTT_DATA;

	while (publishCount > 0U) {
		vTaskDelay(pdMS_TO_TICKS(mqttexamplePUBLISH_INTERVAL_MS));
		agent_status = iot_agent_demo_mqtt_publish(&mqttAgentContext);

    	if (agent_status == IOT_AGENT_SUCCESS) {
    		IOT_AGENT_INFO("Echo successfully published");
    	}
		else {
			IOT_AGENT_INFO("ERROR: Echo failed to publish");
			publishFails++;
		}

		publishCount--;
	}

	ASSERT_OR_EXIT_MSG(publishFails < (PUBLISH_ATTEMPTS / 2U),
			"More than or equal to %d publish attempts failed (%d).", (PUBLISH_ATTEMPTS / 2), publishFails);

exit:
	agent_status = iot_agent_demo_mqtt_disconnect(&mqttAgentContext);
	return agent_status;
}


static const char AZURE_SERVER_ROOT_CERTIFICATE_PEM[] =
    /* DigiCert Baltimore Root */
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\r\n"
"RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\r\n"
"VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\r\n"
"DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\r\n"
"ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\r\n"
"VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\r\n"
"mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\r\n"
"IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\r\n"
"mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\r\n"
"XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\r\n"
"dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\r\n"
"jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\r\n"
"BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\r\n"
"DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\r\n"
"9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\r\n"
"jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\r\n"
"Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\r\n"
"ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\r\n"
"R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\r\n"
"-----END CERTIFICATE-----\r\n"
/*DigiCert Global Root G2*/
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\r\n"
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n"
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\r\n"
"MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\r\n"
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\r\n"
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\r\n"
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\r\n"
"2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\r\n"
"1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\r\n"
"q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\r\n"
"tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\r\n"
"vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\r\n"
"BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\r\n"
"5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\r\n"
"1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\r\n"
"NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\r\n"
"Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\r\n"
"8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\r\n"
"pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\r\n"
"MrY=\r\n"
"-----END CERTIFICATE-----\r\n";

static const uint32_t AZURE_SERVER_ROOT_CERTIFICATE_PEM_LENGTH = sizeof(AZURE_SERVER_ROOT_CERTIFICATE_PEM);

#define AZURE_MQTT_REGISTER_HOSTNAME      "global.azure-devices-provisioning.net"
#define AZURE_MQTT_REGISTER_PORT          8883
#define AZURE_MQTT_REGISTRATION_MSG_TOPIC "$dps/registrations/PUT/iotdps-register/?$rid=1"
#define AZURE_MQTT_PUBLISH_MSG_OPID_AZURE "$dps/registrations/GET/iotdps-get-operationstatus/?$rid=2&operationId="
#define AZURE_MQTT_SUBSCRIBE_MSG_TOPIC    "$dps/registrations/res/#"
#define TOKEN_NO                          50

typedef struct azure_registration_info_t
{
	char assignedHub[256];
	char deviceId[256];
	char registrationId[256];
	char operationId[256];
	char username[256];
	registration_state state;
} azure_registration_info_t;

typedef struct azure_connection_info
{
	char hostname[256];
	char topic[256];
	char username[256];
} azure_connection_info_t;

static azure_registration_info_t* pAzureRegInfo;

#ifdef NXP_IOT_AGENT_USE_COREJSON
static iot_agent_status_t get_value_from_tag(char *js, size_t js_size, const char * key, size_t key_size, char * value, size_t max_value_size) {
  	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	char* value_ptr;
	size_t value_size = 0U;
	ASSERT_OR_EXIT_MSG(js != NULL, "The input pointer is NULL");
	ASSERT_OR_EXIT_MSG(key != NULL, "The input pointer is NULL");
	ASSERT_OR_EXIT_MSG(value != NULL, "The input pointer is NULL");
  	ASSERT_OR_EXIT_MSG(JSON_Search(js, js_size, key, key_size, &value_ptr, &value_size) == JSONSuccess, "Error in JSON string parsing");
	ASSERT_OR_EXIT_MSG(value_size < max_value_size, "To less space allocated for the value buffer");
	strncpy(value, value_ptr, value_size);
exit:
	return agent_status;
}	  
#else
static iot_agent_status_t get_value_from_tag(char *js, const char * key, char * value)
{
	jsmn_parser p;
	jsmntok_t tokens[TOKEN_NO]; /* We expect no more than 10 JSON tokens */
	jsmn_init(&p);
	int count = jsmn_parse(&p, js, strlen(js), tokens, TOKEN_NO);
	for (int i = 1; i < count; i += 2)
	{
		jsmntok_t *t = &tokens[i];
		char *tag = js + t->start;
		if (!memcmp(tag, key, t->end - t->start))
		{
			t = &tokens[i + 1];
			memcpy(value, js + t->start, t->end - t->start);
			value[t->end - t->start] = '\0';
			return IOT_AGENT_SUCCESS;
		}
	}
	return IOT_AGENT_FAILURE;
}
#endif

static void azureRegistrationCallback( MQTTContext_t * pxMqttContext,
        MQTTPacketInfo_t * pxPacketInfo,
        MQTTDeserializedInfo_t * pxDeserializedInfo )
{
    if( ( pxPacketInfo->type & 0xF0U ) == MQTT_PACKET_TYPE_PUBLISH )
    {
		IOT_AGENT_INFO("Publish message received on topic");

		char opid[256] = {'\0'};
		char status[64] = {'\0'};

		size_t payloadLength = pxDeserializedInfo->pPublishInfo->payloadLength + 1U;
		char* payload = pvPortMalloc(payloadLength);

		memcpy(payload, pxDeserializedInfo->pPublishInfo->pPayload, payloadLength - 1U);
		payload[payloadLength - 1U] = '\0';
#ifdef NXP_IOT_AGENT_USE_COREJSON
		get_value_from_tag(payload, payloadLength, "operationId", strlen("operationId"), opid, sizeof(opid));
		get_value_from_tag(payload, payloadLength, "status", strlen("status"), status, sizeof(status));
#else
		get_value_from_tag(payload, "operationId", opid);
		get_value_from_tag(payload, "status", status);
#endif
		azure_registration_info_t* reg_info = pAzureRegInfo;

		if(strcmp(status, "assigning") == 0)
		{
			IOT_AGENT_INFO("Device State is now ASSIGNING");
			strcpy(reg_info->operationId, AZURE_MQTT_PUBLISH_MSG_OPID_AZURE);
			strcat(reg_info->operationId, opid);

			reg_info->state = ASSIGNING;
		}
		else if(strcmp(status, "assigned") == 0)
		{
			IOT_AGENT_INFO("Device State is now ASSIGNED");
#ifdef NXP_IOT_AGENT_USE_COREJSON
			char* registrationState = pvPortMalloc(payloadLength);

			memset(registrationState, '\0', payloadLength);
			get_value_from_tag(payload, payloadLength, "registrationState", strlen("registrationState"), registrationState, payloadLength);
			get_value_from_tag(registrationState, strlen(registrationState), "registrationId", strlen("registrationId"), reg_info->registrationId, sizeof(reg_info->registrationId));
			get_value_from_tag(registrationState, strlen(registrationState), "assignedHub", strlen("assignedHub"), reg_info->assignedHub, sizeof(reg_info->assignedHub));
			get_value_from_tag(registrationState, strlen(registrationState), "deviceId", strlen("deviceId"), reg_info->deviceId, sizeof(reg_info->deviceId));
			vPortFree(registrationState);
#else
			get_value_from_tag(payload, "registrationId", reg_info->registrationId);
			get_value_from_tag(payload, "assignedHub", reg_info->assignedHub);
			get_value_from_tag(payload, "deviceId", reg_info->deviceId);
#endif
			reg_info->state = ASSIGNED;
		}

		vPortFree(payload);
	}
	else if(( pxPacketInfo->type & 0xF0U ) == MQTT_PACKET_TYPE_DISCONNECT)
	{
		IOT_AGENT_INFO("Disconnect message received");
	}
	else if(( pxPacketInfo->type & 0xF0U ) == MQTT_PACKET_TYPE_SUBACK)
	{
		IOT_AGENT_INFO("Subscription ack message received");
	}
	else
	{
		IOT_AGENT_INFO("Unknown message received\r\n");
	}
}

static iot_agent_status_t formatRegistrationUsername(azure_registration_info_t* reg_info, const char* idscope, const char* deviceid)
{
  	int result = 0;
	result = snprintf(reg_info->username,
		sizeof(reg_info->username),
		"%s/registrations/%s/api-version=2018-11-01&ClientVersion=1.4.0",
		idscope,
		deviceid);
	if (result < 0)
	{
		return IOT_AGENT_FAILURE;
	}

	size_t n = result;

	if(n > sizeof(reg_info->username))
	{
		return IOT_AGENT_FAILURE;
	}

	return IOT_AGENT_SUCCESS;
}

static bool formatConnectionOptions(azure_connection_info_t* conn_info, char* hubname, char* deviceid)
{
  	int result = 0;
	result = snprintf(conn_info->username,
		sizeof(conn_info->username),
		"%s/%s/?api-version=2018-06-30",
		hubname,
		deviceid);
	if (result < 0)
	{
	  	return false;
	}
	size_t m = result;

	result = snprintf(conn_info->topic,
		sizeof(conn_info->topic),
		"devices/%s/messages/events/",
		deviceid);
	if (result < 0)
	{
	  	return false;
	}
	size_t n = result;

	if (n > sizeof(conn_info->topic) || m > sizeof(conn_info->username))
	{
		return false;
	}
	return true;
}

static iot_agent_status_t azureRegister(const nxp_iot_ServiceDescriptor* service_descriptor, azure_registration_info_t* reg_info)
{
    iot_agent_status_t agent_status = IOT_AGENT_FAILURE;
    uint8_t sharedBuffer[AGENT_NETWORK_BUFFER_SIZE];
    MQTTContext_t mqttContext = { 0 };
    NetworkContext_t networkContext = { 0 };
    MQTTFixedBuffer_t networkBuffer = {
        .pBuffer = sharedBuffer,
        .size    = AGENT_NETWORK_BUFFER_SIZE
    };
    TransportInterface_t transportInterface = { 0 };
    MqttAgent_Context_t mqttAgentContext = { 0 };
    size_t retryCount = 0U;
    size_t maxWaiting = 0U;

    formatRegistrationUsername(reg_info, service_descriptor->azure_id_scope, service_descriptor->azure_registration_id);

    // store reg info in a global pointer, this will be used in the callback function to set some states
    pAzureRegInfo = reg_info;

    agent_status = iot_agent_demo_mqtt_init(&mqttAgentContext);
    AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT Agent Context initialization");

    transportInterface.pNetworkContext = &networkContext;
#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
    transportInterface.send = TLS_FreeRTOS_Send;
    transportInterface.recv = TLS_FreeRTOS_Recv;
#else
    networkContext.pParams = &secureSocketsTransportParams;
    transportInterface.send = SecureSocketsTransport_Send;
    transportInterface.recv = SecureSocketsTransport_Recv;
#endif
    mqttAgentContext.pMqttContext = &mqttContext;
    mqttAgentContext.pNetworkContext = &networkContext;
    mqttAgentContext.pNetworkBuffer = &networkBuffer;
    mqttAgentContext.pTransportInterface = &transportInterface;
    mqttAgentContext.prvUserCallback = azureRegistrationCallback;
    mqttAgentContext.pHostName = AZURE_MQTT_REGISTER_HOSTNAME;
    mqttAgentContext.port = AZURE_MQTT_REGISTER_PORT;
    mqttAgentContext.pRootCa = AZURE_SERVER_ROOT_CERTIFICATE_PEM;
    mqttAgentContext.rootCaSize = AZURE_SERVER_ROOT_CERTIFICATE_PEM_LENGTH;
    mqttAgentContext.pClientId = service_descriptor->azure_registration_id;
    mqttAgentContext.pUserName = reg_info->username;

	while (retryCount < 5U) {
		IOT_AGENT_INFO("\nMQTT attempting to register Azure Service '%s' (the operation migth take around 1 minute)...", service_descriptor->azure_registration_id);
		agent_status = iot_agent_demo_mqtt_connect(&mqttAgentContext);
		if (agent_status == IOT_AGENT_SUCCESS) {
			break;
		}

        vTaskDelay(pdMS_TO_TICKS(mqttexampleKEEP_ALIVE_ACTUAL_INTERVAL_MS * (1U + 2U*retryCount)));
        retryCount++;
	}

	AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT connection");

	mqttAgentContext.pMqttSubscribeTopicFilter = AZURE_MQTT_SUBSCRIBE_MSG_TOPIC;

	agent_status = iot_agent_demo_mqtt_subscribe(&mqttAgentContext);
	AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT subscription");

    reg_info->state = NOT_ASSIGNED;

	mqttAgentContext.pMqttPublishTopic = AZURE_MQTT_REGISTRATION_MSG_TOPIC;
	mqttAgentContext.pMqttPublishPayload = NULL;

	agent_status = iot_agent_demo_mqtt_publish(&mqttAgentContext);
	AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT publish");

    while (reg_info->state != ASSIGNING && maxWaiting < 20U)
    {
    	agent_status = iot_agent_demo_mqtt_wait_packet(&mqttAgentContext);
    	AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT packet waiting");

        vTaskDelay(pdMS_TO_TICKS(mqttexamplePUBLISH_WAIT_INTERVAL_MS));
     	maxWaiting++;
	}


	mqttAgentContext.pMqttPublishTopic = reg_info->operationId;
	mqttAgentContext.pMqttPublishPayload = NULL;
	maxWaiting = 0U;

    while (reg_info->state != ASSIGNED && maxWaiting < 20U)
    {
        vTaskDelay(pdMS_TO_TICKS(mqttexamplePUBLISH_WAIT_INTERVAL_MS));

    	agent_status = iot_agent_demo_mqtt_publish(&mqttAgentContext);
    	AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT publish");

    	agent_status = iot_agent_demo_mqtt_wait_packet(&mqttAgentContext);
    	AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT packet waiting");
    	maxWaiting++;
	}
    if(reg_info->state == ASSIGNED)
    {
    	IOT_AGENT_INFO("State is ASSIGNED");
    }

exit:
	agent_status = iot_agent_demo_mqtt_disconnect(&mqttAgentContext);
	return agent_status;

}

static iot_agent_status_t azurePubSub(const nxp_iot_ServiceDescriptor* service_descriptor, azure_registration_info_t* reg_info)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	uint8_t sharedBuffer[AGENT_NETWORK_BUFFER_SIZE];
    MQTTContext_t mqttContext = { 0 };
    NetworkContext_t networkContext = { 0 };
    MQTTFixedBuffer_t networkBuffer = {
        .pBuffer = sharedBuffer,
        .size    = AGENT_NETWORK_BUFFER_SIZE
    };
    TransportInterface_t transportInterface = { 0 };
	size_t publishFails = 0;
	size_t publishCount = PUBLISH_ATTEMPTS;
    azure_connection_info_t conn_info = { 0 };

    MqttAgent_Context_t mqttAgentContext = { 0 };
    size_t retryCount = 0U;

    formatConnectionOptions(&conn_info, reg_info->assignedHub, reg_info->deviceId);

    agent_status = iot_agent_demo_mqtt_init(&mqttAgentContext);
    AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT Agent Context initialization");

    transportInterface.pNetworkContext = &networkContext;

#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
    transportInterface.send = TLS_FreeRTOS_Send;
    transportInterface.recv = TLS_FreeRTOS_Recv;
#else
    networkContext.pParams = &secureSocketsTransportParams;
    transportInterface.send = SecureSocketsTransport_Send;
    transportInterface.recv = SecureSocketsTransport_Recv;
#endif

    mqttAgentContext.pMqttContext = &mqttContext;
    mqttAgentContext.pNetworkContext = &networkContext;
    mqttAgentContext.pNetworkBuffer = &networkBuffer;
    mqttAgentContext.pTransportInterface = &transportInterface;
    mqttAgentContext.prvUserCallback = mqttUserCallback;
    mqttAgentContext.pHostName = reg_info->assignedHub;
    mqttAgentContext.port = AZURE_MQTT_REGISTER_PORT;
    mqttAgentContext.pRootCa = AZURE_SERVER_ROOT_CERTIFICATE_PEM;
    mqttAgentContext.rootCaSize = AZURE_SERVER_ROOT_CERTIFICATE_PEM_LENGTH;
    mqttAgentContext.pClientId = reg_info->deviceId;
    mqttAgentContext.pUserName = conn_info.username;

	while (retryCount < 5U) {
		IOT_AGENT_INFO("Attempt %d for connecting to Azure service '%s'...", retryCount, reg_info->deviceId);
		agent_status = iot_agent_demo_mqtt_connect(&mqttAgentContext);
		if (agent_status == IOT_AGENT_SUCCESS) {
			break;
		}

        vTaskDelay(pdMS_TO_TICKS(mqttexampleKEEP_ALIVE_ACTUAL_INTERVAL_MS * (1U + 2U*retryCount)));
        retryCount++;
	}

	AGENT_SUCCESS_OR_EXIT_MSG("Error in MQTT connection");

	mqttAgentContext.pMqttPublishTopic = conn_info.topic;
	mqttAgentContext.pMqttPublishPayload = MQTT_DATA;

	while (publishCount > 0U)
	{
		vTaskDelay(pdMS_TO_TICKS(mqttexamplePUBLISH_INTERVAL_MS));
		agent_status = iot_agent_demo_mqtt_publish(&mqttAgentContext);

    	if (agent_status == IOT_AGENT_SUCCESS) {
    		IOT_AGENT_INFO("Echo successfully published");
    	}
		else {
			IOT_AGENT_INFO("ERROR: Echo failed to publish");
			publishFails++;
		}

		publishCount--;
	}

	ASSERT_OR_EXIT_MSG(publishFails < (PUBLISH_ATTEMPTS / 2U),
			"More than or equal to %d publish attempts failed (%d).", (PUBLISH_ATTEMPTS / 2U), publishFails);

exit:
	agent_status = iot_agent_demo_mqtt_disconnect(&mqttAgentContext);
	return agent_status;
}

static iot_agent_status_t azurePubMqttMessage(const nxp_iot_ServiceDescriptor* service_descriptor)
{
	azure_registration_info_t reg_info = { 0 };
	iot_agent_status_t status;
	status = azureRegister(service_descriptor, &reg_info);
	if(status != IOT_AGENT_SUCCESS)
	{
		return status;
	}

	status = azurePubSub(service_descriptor, &reg_info);
	if(status != IOT_AGENT_SUCCESS)
	{
		return status;
	}

	return IOT_AGENT_SUCCESS;
}

#ifdef NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
#if (NXP_IOT_AGENT_HAVE_PSA == 0)
static void iot_agent_convert_id_to_label(char* label, size_t label_size, uint32_t id) {
    memset(label, 0, label_size);
    uint32_t sss_keyId = (uint32_t)((((id >> (8 * 0)) & 0x000000FF) << (8 * 3)) |
                            (((id >> (8 * 1)) & 0x000000FF) << (8 * 2)) |
                            (((id >> (8 * 2)) & 0x000000FF) << (8 * 1)) |
                            (((id >> (8 * 3)) & 0x000000FF) << (8 * 0)));
    snprintf(label, label_size, "sss:%08lx", sss_keyId);
}
#endif // NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT
#endif // (NXP_IOT_AGENT_HAVE_PSA == 0)
#endif //NXP_IOT_AGENT_ENABLE_LITE
