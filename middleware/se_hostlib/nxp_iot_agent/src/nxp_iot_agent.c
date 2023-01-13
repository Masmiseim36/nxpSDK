/*
 * Copyright 2018-2020, 2021, 2022 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */


#include <string.h>
#if !(defined(__ICCARM__) || defined(__CC_ARM) || defined(__ARMCC_VERSION))
#include <unistd.h>
#endif

#include <nxp_iot_agent.h>
#include <nxp_iot_agent_dispatcher.h>

#include "./protobuf/pb.h"
#include "./protobuf/pb_encode.h"
#include "./protobuf/pb_decode.h"

#include "./protobuf/Dispatcher.pb.h"
#include "./protobuf/Agent.pb.h"
#include "./protobuf/Datastore.pb.h"
#include "./protobuf/ServiceDescriptor.pb.h"

#if defined(SSS_USE_FTR_FILE)
#include <fsl_sss_ftr.h>
#else
#include <fsl_sss_ftr_default.h>
#endif

#if SSS_HAVE_HOSTCRYPTO_OPENSSL
#include <network_openssl.h>
#endif

#if SSS_HAVE_HOSTCRYPTO_MBEDTLS
#include <network_mbedtls.h>
#endif

#if SSS_HAVE_MBEDTLS_ALT_SSS
#include "sss_mbedtls.h"
#endif

#include <fsl_sss_api.h>
#include <nxp_iot_agent_utils.h>
#include <nxp_iot_agent_session.h>
#include <nxp_iot_agent_macros.h>
#include <nxp_iot_agent_datastore.h>
#include <nxp_iot_agent_endpoint.h>
#include <nxp_iot_agent_time.h>

#define IOT_AGENT_VERSION_MAJOR (1U)
#define IOT_AGENT_VERSION_MINOR (0U)
#define IOT_AGENT_VERSION_PATCH (0U)

#define IOT_AGENT_VERSION \
	(((IOT_AGENT_VERSION_MAJOR * 256) + IOT_AGENT_VERSION_MINOR) * 256 \
		+ IOT_AGENT_VERSION_PATCH)

const iot_agent_endpoint_interface_t iot_agent_endpoint_interface =
{
	&iot_agent_get_endpoint_info,
	&iot_agent_handle_request,
};

iot_agent_status_t iot_agent_init(iot_agent_context_t* ctx)
{
	memset(ctx, 0, sizeof(*ctx));
	return IOT_AGENT_SUCCESS;
}


iot_agent_status_t iot_agent_register_keystore(
	iot_agent_context_t* ctx,
	iot_agent_keystore_t* keystore)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	size_t index = 0U;
	ASSERT_OR_EXIT_MSG(ctx->numKeystores < NXP_IOT_AGENT_MAX_NUM_KEYSTORES,
		"Unable to register keystore, maximum number reached.");

	// Check for identifier uniqueness.
	agent_status = iot_agent_get_datastore_index_by_id(ctx, keystore->identifier, &index);
#if AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1
	ASSERT_OR_EXIT_MSG(agent_status != IOT_AGENT_SUCCESS, "Unable to register keystore with id [0x%08lx], "
		"a datastore with that id is already registered.", keystore->identifier);
	agent_status = iot_agent_get_keystore_index_by_id(ctx, keystore->identifier, &index);
	ASSERT_OR_EXIT_MSG(agent_status != IOT_AGENT_SUCCESS, "Unable to register keystore with id [0x%08lx], "
		"a keystore with that id is already registered.", keystore->identifier);
#else
	ASSERT_OR_EXIT_MSG(agent_status != IOT_AGENT_SUCCESS, "Unable to register keystore with id [0x%08x], "
		"a datastore with that id is already registered.", keystore->identifier);
	agent_status = iot_agent_get_keystore_index_by_id(ctx, keystore->identifier, &index);
	ASSERT_OR_EXIT_MSG(agent_status != IOT_AGENT_SUCCESS, "Unable to register keystore with id [0x%08x], "
		"a keystore with that id is already registered.", keystore->identifier);
#endif

	// Finally register it.
	ctx->keystores[ctx->numKeystores++] = keystore;
	agent_status = IOT_AGENT_SUCCESS;

exit:
	return agent_status;
}


iot_agent_status_t iot_agent_register_datastore(
	iot_agent_context_t* ctx,
	iot_agent_datastore_t* datastore)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	size_t index = 0U;
#if AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1
	ASSERT_OR_EXIT_MSG(datastore->identifier != (uint32_t)nxp_iot_DatastoreIdentifiers_DATASTORE_EDGELOCK2GO_ID,
		"Unable to register datastore with id [0x%08lx], this id is reserved", datastore->identifier);

	ASSERT_OR_EXIT_MSG(ctx->numDatastores < NXP_IOT_AGENT_MAX_NUM_DATASTORES,
		"Unable to register datastore, maximum number reached.");

	// Check for identifier uniqueness.
	agent_status = iot_agent_get_datastore_index_by_id(ctx, datastore->identifier, &index);
	ASSERT_OR_EXIT_MSG(agent_status != IOT_AGENT_SUCCESS, "Unable to register datastore with id [0x%08lx], "
		"a datastore with that id is already registered.", datastore->identifier);
	agent_status = iot_agent_get_keystore_index_by_id(ctx, datastore->identifier, &index);
	ASSERT_OR_EXIT_MSG(agent_status != IOT_AGENT_SUCCESS, "Unable to register datastore with id [0x%08lx], "
		"a keystore with that id is already registered.", datastore->identifier);
	agent_status = IOT_AGENT_SUCCESS;
#else
	ASSERT_OR_EXIT_MSG(datastore->identifier != (uint32_t)nxp_iot_DatastoreIdentifiers_DATASTORE_EDGELOCK2GO_ID,
		"Unable to register datastore with id [0x%08x], this id is reserved", datastore->identifier);

	ASSERT_OR_EXIT_MSG(ctx->numDatastores < NXP_IOT_AGENT_MAX_NUM_DATASTORES,
		"Unable to register datastore, maximum number reached.");

	// Check for identifier uniqueness.
	agent_status = iot_agent_get_datastore_index_by_id(ctx, datastore->identifier, &index);
	ASSERT_OR_EXIT_MSG(agent_status != IOT_AGENT_SUCCESS, "Unable to register datastore with id [0x%08x], "
		"a datastore with that id is already registered.", datastore->identifier);
	agent_status = iot_agent_get_keystore_index_by_id(ctx, datastore->identifier, &index);
	ASSERT_OR_EXIT_MSG(agent_status != IOT_AGENT_SUCCESS, "Unable to register datastore with id [0x%08x], "
		"a keystore with that id is already registered.", datastore->identifier);
	agent_status = IOT_AGENT_SUCCESS;
#endif
	// Finally register it.
	ctx->datastores[ctx->numDatastores++] = datastore;

exit:
	return agent_status;
}


iot_agent_status_t iot_agent_set_edgelock2go_datastore(iot_agent_context_t* ctx,
	iot_agent_datastore_t* datastore)
{
	ctx->edgelock2go_datastore = datastore;
	return IOT_AGENT_SUCCESS;
}


// /pre open session from keystore
iot_agent_status_t iot_agent_update_device_configuration_from_service_descriptor(
	iot_agent_context_t* agent_context,
	nxp_iot_ServiceDescriptor* service_descriptor,
	nxp_iot_UpdateStatusReport* status_report)
{
#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
    axTimeMeasurement_t iot_agent_prepare_tls_time = { 0 };
    axTimeMeasurement_t network_connect_time = { 0 };
    axTimeMeasurement_t process_provision_time = { 0 };
    initMeasurement(&iot_agent_prepare_tls_time);
#endif
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	sss_status_t sss_status;
	int network_status = 0;
	uint32_t edgelock2go_keystore_id = 0U;
	sss_object_t private_key = { 0 };
	sss_object_t client_certificate = { 0 };
	size_t cert_size_bits = 0U;

	iot_agent_dispatcher_context_t dispatcher_context = { 0 };

	iot_agent_keystore_t* keystore = NULL;
	sss_key_store_t *sss_context = NULL;

	pb_istream_t input;
	pb_ostream_t output;

	uint8_t buffer[NXP_IOT_AGENT_EDGELOCK2GO_CLIENT_CERTIFICATE_BUFFER_SIZE];
	size_t buffer_size = sizeof(buffer);

#if SSS_HAVE_HOSTCRYPTO_OPENSSL
	BIO *bio_in = NULL;
	BIO *bio_in_verify = NULL;
	uint32_t private_key_object_id = 0U;

	openssl_network_config_t openssl_network_config = { 0 };
#endif

	ASSERT_OR_EXIT_MSG(service_descriptor->hostname != NULL, "service_descriptor does not contain a hostname.");
	ASSERT_OR_EXIT_MSG(service_descriptor->has_port, "service_descriptor does not contain a port.");

	// For the client key, we for now always expect to have it as a reference.
	ASSERT_OR_EXIT_MSG(service_descriptor->has_client_key_sss_ref, "service_descriptor does not contain a client_key_sss_ref.");
	ASSERT_OR_EXIT_MSG(service_descriptor->client_key_sss_ref.has_type, "client_key_sss_ref does not contain a type.");
	ASSERT_OR_EXIT_MSG(service_descriptor->client_key_sss_ref.has_endpoint_id, "client_key_sss_ref does not contain an endpoint_id.");
	ASSERT_OR_EXIT_MSG(service_descriptor->client_key_sss_ref.has_object_id, "client_key_sss_ref does not contain an object_id.");

	// For the client certificate, we for now always expect to have it as a reference.
	ASSERT_OR_EXIT_MSG(service_descriptor->has_client_certificate_sss_ref, "service_descriptor does not contain a client_certificate_sss_ref.");
	ASSERT_OR_EXIT_MSG(service_descriptor->client_certificate_sss_ref.has_type, "client_certificate_sss_ref does not contain a type.");
	ASSERT_OR_EXIT_MSG(service_descriptor->client_certificate_sss_ref.has_endpoint_id, "client_certificate_sss_ref does not contain an endpoint_id.");
	ASSERT_OR_EXIT_MSG(service_descriptor->client_certificate_sss_ref.has_object_id, "client_certificate_sss_ref does not contain an object_id.");

#if NXP_IOT_AGENT_VERIFY_EDGELOCK_2GO_SERVER_CERTIFICATE
	// For the trusted root certificates, we for now always expect to have its cintents as bytes.
	ASSERT_OR_EXIT_MSG(service_descriptor->server_certificate != NULL, "service_descriptor does not contain a server certificate");
#endif
#if AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1
	IOT_AGENT_INFO("Updating device configuration from [%s]:[%lu].", service_descriptor->hostname, service_descriptor->port);
#else
	IOT_AGENT_INFO("Updating device configuration from [%s]:[%d].", service_descriptor->hostname, service_descriptor->port);
#endif

	edgelock2go_keystore_id = service_descriptor->client_key_sss_ref.endpoint_id;
	agent_status = iot_agent_get_keystore_by_id(agent_context, edgelock2go_keystore_id, &keystore);
	AGENT_SUCCESS_OR_EXIT_MSG("Unable to get keystore for connection to EdgeLock 2GO.");

	sss_context = keystore->sss_context;

	agent_status = iot_agent_keystore_open_session(keystore);
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_keystore_open_session failed: 0x%08x", agent_status)

	sss_status = sss_key_object_init(&private_key, sss_context);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_init failed: 0x%08x.", sss_status);

	sss_status = sss_key_object_get_handle(&private_key, service_descriptor->client_key_sss_ref.object_id);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_get_handle failed: 0x%08x.", sss_status);

	// The API needs to be passed a "size in bits" pointer. Does
	// not make any sense for certificates, but not all keystore types accept NULL,
	sss_status = sss_key_object_init(&client_certificate, sss_context);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_init failed: 0x%08x.", sss_status);

	sss_status = sss_key_object_get_handle(&client_certificate, service_descriptor->client_certificate_sss_ref.object_id);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_get_handle failed: 0x%08x.", sss_status);

	// Independent from the crypto library, we need to read the certificate
	// from the secure object, all crypto libraries expect to have it available as bytes:
	sss_status = sss_key_store_get_key(sss_context, &client_certificate, buffer, &buffer_size, &cert_size_bits);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_store_get_key failed: 0x%04x.", sss_status);

	dispatcher_context.network_context = network_new();
	ASSERT_OR_EXIT_MSG(dispatcher_context.network_context != NULL, "network context is NULL");

	agent_status = iot_agent_init_dispatcher(&dispatcher_context, agent_context, service_descriptor, status_report);
	AGENT_SUCCESS_OR_EXIT();

#if SSS_HAVE_HOSTCRYPTO_OPENSSL
	IOT_AGENT_INFO("Close SE session before starting openssl engine session");
	iot_agent_keystore_close_session(keystore);

	// Start Opnssl engine. This must be done BEFORE any key creation!
	// Note, the oenssl engine will attempt to start a session to the secure
	// element at startup.
	network_status = network_openssl_init(dispatcher_context.network_context);
	ASSERT_OR_EXIT_MSG(network_status == NETWORK_STATUS_OK, "network_openssl_init failed with 0x%08x.", network_status);

	openssl_network_config.hostname = service_descriptor->hostname;
	openssl_network_config.port = (int)service_descriptor->port;
	openssl_network_config.private_key = EVP_PKEY_new();
	openssl_network_config.ca_chain = X509_STORE_new();

	bio_in = BIO_new_mem_buf(buffer, (int)buffer_size);
	OPENSSL_ASSERT_OR_EXIT(bio_in != NULL, "BIO_new_mem_buf");
	openssl_network_config.certificate = d2i_X509_bio(bio_in, NULL);

#if NXP_IOT_AGENT_VERIFY_EDGELOCK_2GO_SERVER_CERTIFICATE
	bio_in_verify = BIO_new_mem_buf(service_descriptor->server_certificate->bytes, (int)service_descriptor->server_certificate->size);
	OPENSSL_ASSERT_OR_EXIT(bio_in_verify != NULL, "BIO_new_mem_buf");
	while (true) {
		X509 *root_ca_cert = d2i_X509_bio(bio_in_verify, NULL);
		if (root_ca_cert == NULL) {
			// We try here to parse certificates until there is no more data left. The
			// last attempt (where no data is remaining) causes an error in the openssl
			// error queue (for unparsable certificate). We should clear that one here so it
			// can not cause unexpexted errors elsewhere. The error code is unused.
			(void)ERR_get_error();
			break;
		}
		int openssl_status = X509_STORE_add_cert(openssl_network_config.ca_chain, root_ca_cert);
		X509_free(root_ca_cert);
		OPENSSL_ASSERT_OR_EXIT(openssl_status == 1, "X509_STORE_add_cert");
	}
#endif

	network_status = network_openssl_engine_session_disconnect(dispatcher_context.network_context);
	ASSERT_OR_EXIT_MSG(network_status == NETWORK_STATUS_OK, "network_openssl_engine_session_disconnect failed with 0x%08x.", network_status);

	agent_status = iot_agent_keystore_open_session(keystore);
	AGENT_SUCCESS_OR_EXIT_MSG("Failed to re-connect to Secure Element.")

	private_key_object_id = service_descriptor->client_key_sss_ref.object_id;
	agent_status = iot_agent_utils_gen_key_ref(sss_context, &private_key, private_key_object_id, openssl_network_config.private_key);
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_utils_gen_key_ref failed: 0x%08x.", agent_status);

	iot_agent_keystore_close_session(keystore);

	network_status = network_openssl_engine_session_connect(dispatcher_context.network_context);
	ASSERT_OR_EXIT_MSG(network_status == NETWORK_STATUS_OK, "network_openssl_engine_session_connect failed with 0x%08x.", network_status);

	network_status = network_configure(dispatcher_context.network_context, &openssl_network_config);
	ASSERT_OR_EXIT_MSG(network_status == NETWORK_STATUS_OK, "network_configure failed with 0x%08x.", network_status);

#elif SSS_HAVE_HOSTCRYPTO_MBEDTLS

	mbedtls_network_config_t network_config = { 0 };
	network_config.hostname = service_descriptor->hostname;
	network_config.port = service_descriptor->port;

	mbedtls_x509_crt_init(&network_config.clicert);
	network_status = mbedtls_x509_crt_parse_der(&network_config.clicert, buffer, buffer_size);
	ASSERT_OR_EXIT_MSG(network_status == 0, "mbedtls_x509_crt_parse_der of client cert failed with 0x%08x", network_status);

#if NXP_IOT_AGENT_VERIFY_EDGELOCK_2GO_SERVER_CERTIFICATE
	mbedtls_x509_crt_init(&network_config.ca_chain);
	const uint8_t* pos = service_descriptor->server_certificate->bytes;
	const uint8_t* end = pos + service_descriptor->server_certificate->size;
	while (pos < end)
	{
		// We expect the certificate to start with a sequence tag, from that we get the length
		// so we can jump to the next one in the buffer.
		const uint8_t* start = pos;
		size_t len = 0;
		network_status = mbedtls_asn1_get_tag((unsigned char **)&pos, end, &len, MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE);
		ASSERT_OR_EXIT_MSG(network_status == 0, "mbedtls_asn1_get_tag of trusted root ca cert failed with 0x%08x", network_status);
		pos += len;

		network_status = mbedtls_x509_crt_parse_der(&network_config.ca_chain, start, end - start);
		ASSERT_OR_EXIT_MSG(network_status == 0, "mbedtls_x509_crt_parse_der of trusted root ca cert failed with 0x%08x", network_status);
	}
#endif

	network_status = network_configure(dispatcher_context.network_context, &network_config);
	ASSERT_OR_EXIT_MSG(network_status == 0, "network_configure failed with 0x%08x", network_status);

#if SSS_HAVE_MBEDTLS_ALT_SSS // This avoids a compilation error for combination !SSS_HAVE_MBEDTLS_ALT_SSS
	sss_mbedtls_associate_keypair(&((mbedtls_network_context_t*)dispatcher_context.network_context)->pkey, &private_key);
#endif

#else
#error
#endif

#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
    concludeMeasurement(&iot_agent_prepare_tls_time);
    iot_agent_time.prepare_tls_time = getMeasurement(&iot_agent_prepare_tls_time);

    initMeasurement(&network_connect_time);
#endif
	network_status = network_connect(dispatcher_context.network_context);
	ASSERT_OR_EXIT_MSG(network_status == NETWORK_STATUS_OK, "network_connect failed with 0x%08x.", network_status);
#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
    concludeMeasurement(&network_connect_time);
    iot_agent_time.network_connect = getMeasurement(&network_connect_time);
    initMeasurement(&process_provision_time);
#endif

#if SSS_HAVE_HOSTCRYPTO_OPENSSL

	// We need to distinguish here, if we need to do a CRL verification, this has to be the
	// first thing that we do after connection establishment and openssl still needs control over
	// the secure element in that case. Only if we do not want a CRL verification to de done, we
	// can release the engine's control over the secure element here.
#if ! NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO
	network_status = network_openssl_engine_session_disconnect(dispatcher_context.network_context);
	ASSERT_OR_EXIT_MSG(network_status == NETWORK_STATUS_OK, "network_openssl_engine_session_disconnect failed with 0x%08x.", network_status);

	agent_status = iot_agent_keystore_open_session(keystore);
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_keystore_open_session failed with 0x%08x.", agent_status)
#endif
#endif

	input = istream_from_socket(dispatcher_context.network_context);
	output = ostream_from_socket(dispatcher_context.network_context);

	agent_status = iot_agent_dispatcher(&dispatcher_context, &input, &output);
	AGENT_SUCCESS_OR_EXIT();

#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
    concludeMeasurement(&process_provision_time);
    iot_agent_time.process_provision_time = getMeasurement(&process_provision_time);
#endif
	network_status = network_disconnect(dispatcher_context.network_context);
	ASSERT_OR_EXIT_MSG(network_status == 0, "network_disconnect failed with 0x%08x", network_status);

#if SSS_HAVE_HOSTCRYPTO_OPENSSL
	network_status = network_openssl_engine_session_disconnect(dispatcher_context.network_context);
	ASSERT_OR_EXIT_MSG(network_status == 0, "network_openssl_engine_session_disconnect failed with 0x%08x", network_status);
#endif

	agent_status = iot_agent_keystore_open_session(keystore);
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_keystore_open_session failed with 0x%08x", agent_status);

	sss_status = sss_key_store_load(keystore->sss_context);
	ASSERT_OR_EXIT_MSG(sss_status == kStatus_SSS_Success, "sss_key_store_load failed with 0x%08x", sss_status);

exit:

#if SSS_HAVE_HOSTCRYPTO_OPENSSL
	network_status = network_openssl_engine_session_disconnect(dispatcher_context.network_context);
	if (network_status != NETWORK_STATUS_OK) {
		IOT_AGENT_ERROR("network_openssl_engine_session_disconnect failed with 0x%08x", network_status);
		agent_status = IOT_AGENT_FAILURE;
	}

	BIO_free(bio_in_verify);
	BIO_free(bio_in);
	X509_STORE_free(openssl_network_config.ca_chain);
	EVP_PKEY_free(openssl_network_config.private_key);
	X509_free(openssl_network_config.certificate);

#elif SSS_HAVE_HOSTCRYPTO_MBEDTLS
#if NXP_IOT_AGENT_VERIFY_EDGELOCK_2GO_SERVER_CERTIFICATE
	mbedtls_x509_crt_free(&network_config.ca_chain);
	#endif
	mbedtls_x509_crt_free(&network_config.clicert);
#endif

	network_free(dispatcher_context.network_context);

	// Finally also evaluate the message in the status from the dispatcher.
	return (agent_status == IOT_AGENT_SUCCESS && (! dispatcher_context.update_status))
		? IOT_AGENT_UPDATE_FAILED
		: agent_status;
}


iot_agent_status_t iot_agent_update_device_configuration_from_datastore(
	iot_agent_context_t* agent_context,
	iot_agent_datastore_t* datastore,
	nxp_iot_UpdateStatusReport* status_report)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	// We do create a service descriptor here on the stack. It does reference memory for
	// certificate, this memory is allocated when parsing a service descriptor from
	// the datastore. Hence the service_descriptor needs to be freed before exiting.
	nxp_iot_ServiceDescriptor service_descriptor = nxp_iot_ServiceDescriptor_init_default;

	size_t number_of_services = iot_agent_service_get_number_of_services(datastore);
	size_t offset = 0U;
	for (size_t i = 0U; i < number_of_services; i++)
	{
		agent_status = iot_agent_service_get_service_offset_by_index(datastore, i, &offset, &service_descriptor);
		AGENT_SUCCESS_OR_EXIT();

		// We are only interested in service descriptors that are to be used for connecting
		// to EdgeLock 2GO cloud service.
		if (service_descriptor.has_service_type && (service_descriptor.service_type == nxp_iot_ServiceType_EL2GOSERVICE)) {
			continue;
		}

		// Try to connect and update with the data found in the service_descriptor.
		// The first time an update does succeeed, we stop trying further service descriptors.
		// This is essential, as the update of the device configuration could have altered the
		// datastore contents, thus the iteration over its contents must stop.
		agent_status = iot_agent_update_device_configuration_from_service_descriptor(agent_context, &service_descriptor, status_report);
		if (agent_status == IOT_AGENT_SUCCESS) break;
	}

exit:
	iot_agent_free_service_descriptor(&service_descriptor);
	return agent_status;
}


iot_agent_status_t iot_agent_update_device_configuration_from_constants(
	iot_agent_context_t* agent_context, uint32_t client_key_object_id, uint32_t client_cert_object_id,
	nxp_iot_UpdateStatusReport* status_report)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	// We do create a service descriptor here on the stack. It does reference memory for
	// certificate, however, the referened memory is owned by this function and ownership is never
	// handed over to the service descriptor. This means that the service descriptor does not have to
	// be - in fact it must not be - released.
	nxp_iot_ServiceDescriptor service_descriptor = nxp_iot_ServiceDescriptor_init_default;

	service_descriptor.has_client_key_sss_ref = true;
	service_descriptor.client_key_sss_ref.has_type = true;
	service_descriptor.client_key_sss_ref.type = nxp_iot_EndpointType_KS_SSS_SE05X;
	service_descriptor.client_key_sss_ref.has_endpoint_id = true;
	service_descriptor.client_key_sss_ref.endpoint_id = EDGELOCK2GO_KEYSTORE_ID;
	service_descriptor.client_key_sss_ref.has_object_id = true;
	service_descriptor.client_key_sss_ref.object_id = client_key_object_id;

	service_descriptor.has_client_certificate_sss_ref = true;
	service_descriptor.client_certificate_sss_ref.has_type = true;
	service_descriptor.client_certificate_sss_ref.type = nxp_iot_EndpointType_KS_SSS_SE05X;
	service_descriptor.client_certificate_sss_ref.has_endpoint_id = true;
	service_descriptor.client_certificate_sss_ref.endpoint_id = EDGELOCK2GO_KEYSTORE_ID;
	service_descriptor.client_certificate_sss_ref.has_object_id = true;
	service_descriptor.client_certificate_sss_ref.object_id = client_cert_object_id;

#if NXP_IOT_AGENT_VERIFY_EDGELOCK_2GO_SERVER_CERTIFICATE
	// Put the server certificates into the service descriptor.
	service_descriptor.server_certificate = (pb_bytes_array_t*) iot_agent_trusted_root_ca_certificates;
#endif
	service_descriptor.hostname = EDGELOCK2GO_HOSTNAME;
	service_descriptor.has_port = true;
	service_descriptor.port = EDGELOCK2GO_PORT;

	agent_status = iot_agent_update_device_configuration_from_service_descriptor(agent_context, &service_descriptor, status_report);
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_update_device_configuration_from_service_descriptor failed with 0x%08x.", agent_status);

exit:
	return agent_status;
}


iot_agent_status_t iot_agent_update_device_configuration(iot_agent_context_t *agent_context,
	nxp_iot_UpdateStatusReport* status_report)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	iot_agent_datastore_t* datastore = agent_context->edgelock2go_datastore;
	bool configuration_valid = false;

	// First, check whether we do have a datastore registered that
	// can hold information about how to connect to EdgeLock 2GO cloud service.
	if (datastore != NULL) {
		configuration_valid = iot_agent_service_is_configuration_data_valid(datastore);
	}

	// If we found a datastore that has a valid configuration, use it
	if (agent_status == IOT_AGENT_SUCCESS && configuration_valid) {
		agent_status = iot_agent_update_device_configuration_from_datastore(agent_context, datastore, status_report);
		AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_update_device_configuration_from_datastore failed with 0x%08x", agent_status);
	}
	else {
		iot_agent_keystore_t* keystore = NULL;
		sss_key_store_t* sss_context = NULL;
		uint32_t client_key_object_id = 0U;
		uint32_t client_cert_object_id = 0U;

		agent_status = iot_agent_get_keystore_by_id(agent_context, EDGELOCK2GO_KEYSTORE_ID, &keystore);
		AGENT_SUCCESS_OR_EXIT_MSG("Unable to get keystore for connection to EdgeLock 2GO.");

		sss_context = keystore->sss_context;

		agent_status = iot_agent_keystore_open_session(keystore);
		AGENT_SUCCESS_OR_EXIT_MSG("Failed to re-connect to Secure Element.")

		agent_status = iot_agent_utils_get_edgelock2go_key_id(sss_context, &client_key_object_id);
		AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_utils_get_edgelock2go_key_id failed: 0x%08x", agent_status);

		agent_status = iot_agent_utils_get_edgelock2go_certificate_id(sss_context, &client_key_object_id);
		AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_utils_get_edgelock2go_certificate_id failed: 0x%08x", agent_status);

		// There is no valid datastore, we fall back to compile-time constants for the
		// information that otherwise is in the datastore.
		agent_status = iot_agent_update_device_configuration_from_constants(agent_context, client_key_object_id, client_cert_object_id, status_report);
		AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_update_device_configuration_from_constants failed with 0x%08x", agent_status);
	}
exit:
	return agent_status;
}


iot_agent_status_t iot_agent_select_service_by_index(
	iot_agent_context_t* ctx,
	size_t index,
	nxp_iot_ServiceDescriptor* service_descriptor)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	size_t remaining = index;

	ASSERT_OR_EXIT_MSG(ctx->numDatastores <= NXP_IOT_AGENT_MAX_NUM_DATASTORES, "Number of maximum datastores exceeded");

	for (size_t i = 0U; i < ctx->numDatastores; i++)
	{
		iot_agent_datastore_t* datastore = ctx->datastores[i];
		size_t numServices = iot_agent_service_get_number_of_services(datastore);
		if (remaining < numServices)
		{
			size_t offset;
			agent_status = iot_agent_service_get_service_offset_by_index(datastore, remaining, &offset, service_descriptor);
			AGENT_SUCCESS_OR_EXIT();

			ctx->selectedService.datastore = datastore;
			ctx->selectedService.offset = offset;
			break;
		}
		remaining -= numServices;
	}

exit:
	return agent_status;
}


iot_agent_status_t iot_agent_select_service_by_id(
	iot_agent_context_t* ctx,
	uint64_t service_id,
	nxp_iot_ServiceDescriptor* service_descriptor)
{
	iot_agent_status_t agent_status = IOT_AGENT_FAILURE;

	ASSERT_OR_EXIT_MSG(ctx->numDatastores <= NXP_IOT_AGENT_MAX_NUM_DATASTORES, "Number of maximum datastores exceeded");

	for (size_t i = 0U; i < ctx->numDatastores; i++)
	{
		iot_agent_datastore_t* datastore = ctx->datastores[i];
		size_t offset;
		agent_status = iot_agent_service_get_service_offset_by_id(datastore, service_id, &offset, service_descriptor);

		// If we did not find the id in this datastore, continue with the next one.
		if (agent_status != IOT_AGENT_SUCCESS)
			continue;

		// We found it, record the data and stop searching.
		agent_status = IOT_AGENT_SUCCESS;
		ctx->selectedService.datastore = datastore;
		ctx->selectedService.offset = offset;
		break;
	}
exit:
	return agent_status;
}


bool iot_agent_is_service_configuration_data_valid(
	const iot_agent_context_t* ctx)
{
	if (ctx->numDatastores > NXP_IOT_AGENT_MAX_NUM_DATASTORES)
	{
		return false;
	}

	for (size_t i = 0U; i < ctx->numDatastores; i++)
	{
		const iot_agent_datastore_t* datastore = ctx->datastores[i];
		if (!iot_agent_service_is_configuration_data_valid(datastore))
		{
			return false;
		}
	}
	return true;
}


size_t iot_agent_get_number_of_services(
	const iot_agent_context_t* ctx)
{
	size_t totalNumServices = 0U;

	if (ctx->numDatastores > NXP_IOT_AGENT_MAX_NUM_DATASTORES)
	{
		return 0U;
	}

	for (size_t i = 0U; i < ctx->numDatastores; i++)
	{
		const iot_agent_datastore_t* datastore = ctx->datastores[i];
		totalNumServices += iot_agent_service_get_number_of_services(datastore);
	}
	return totalNumServices;
}


iot_agent_status_t iot_agent_get_service_descriptor(const iot_agent_context_t* ctx,
	nxp_iot_ServiceDescriptor* service_descriptor)
{
	return iot_agent_service_get_service_descriptor_of_service(
		ctx->selectedService.datastore, ctx->selectedService.offset,
		service_descriptor);
}


void iot_agent_free_service_descriptor(nxp_iot_ServiceDescriptor* service_descriptor)
{
	iot_agent_service_free_service_descriptor(service_descriptor);
}


void iot_agent_free_update_status_report(nxp_iot_UpdateStatusReport* status_report)
{
	pb_release(nxp_iot_UpdateStatusReport_fields, status_report);
}


iot_agent_status_t iot_agent_get_datastore_index_by_id(
	const iot_agent_context_t* ctx,
	const uint32_t id,
	size_t* index)
{
	for (size_t i = 0U; i < ctx->numDatastores; i++)
	{
		if (ctx->datastores[i]->identifier == id) {
			*index = i;
			return IOT_AGENT_SUCCESS;
		}
	}
	return IOT_AGENT_FAILURE;
}


iot_agent_status_t iot_agent_get_datastore_by_id(
	const iot_agent_context_t* ctx,
	const uint32_t id,
	iot_agent_datastore_t** datastore)
{
	size_t index = 0U;
	iot_agent_status_t agent_status = iot_agent_get_datastore_index_by_id(ctx, id, &index);
	AGENT_SUCCESS_OR_EXIT();

	*datastore = ctx->datastores[index];

exit:
	return agent_status;
}


iot_agent_status_t iot_agent_get_keystore_index_by_id(
	const iot_agent_context_t* ctx,
	const uint32_t id,
	size_t* index)
{
	for (size_t i = 0U; i < ctx->numKeystores; i++)
	{
		if (ctx->keystores[i]->identifier == id) {
			*index = i;
			return IOT_AGENT_SUCCESS;
		}
	}
	return IOT_AGENT_FAILURE;
}


iot_agent_status_t iot_agent_get_keystore_by_id(
	const iot_agent_context_t* ctx,
	const uint32_t id,
	iot_agent_keystore_t** keystore)
{
	size_t index = 0U;
	iot_agent_status_t agent_status = iot_agent_get_keystore_index_by_id(ctx, id, &index);
	AGENT_SUCCESS_OR_EXIT();

	*keystore = ctx->keystores[index];

exit:
	return agent_status;
}


bool iot_agent_get_endpoint_info(void* context, void* endpoint_information)
{
	AX_UNUSED_ARG(context);
	nxp_iot_EndpointInformation* info = (nxp_iot_EndpointInformation*) endpoint_information;
	info->has_version = true;
	info->version = IOT_AGENT_VERSION;

	nxp_iot_AgentAdditionalData additional_data = nxp_iot_AgentAdditionalData_init_default;

	additional_data.has_serviceDescriptorVersion = true;
	additional_data.serviceDescriptorVersion = nxp_iot_ServiceDescriptorVersion_PROTOBUF;

#if NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO
	additional_data.has_requestCrl = true;
	additional_data.requestCrl = true;
#endif

	info->has_additionalData = true;
	pb_ostream_t ostream = pb_ostream_from_buffer(info->additionalData.bytes, sizeof(info->additionalData.bytes));
	if (!pb_encode_delimited(&ostream, nxp_iot_AgentAdditionalData_fields, &additional_data))
	{
		return false;
	}
	info->additionalData.size = (pb_size_t)ostream.bytes_written;

	return true;
}


bool iot_agent_handle_request(pb_istream_t *istream, pb_ostream_t *ostream,
	const pb_field_t* message_type, void *context)
{
	iot_agent_dispatcher_context_t* dispatcher_context = (iot_agent_dispatcher_context_t*)context;
	if (message_type == nxp_iot_AgentGoodbyeRequest_fields)
	{
		nxp_iot_AgentGoodbyeRequest request = nxp_iot_AgentGoodbyeRequest_init_default;
		if (!pb_decode_delimited(istream, nxp_iot_AgentGoodbyeRequest_fields, &request)) {
			IOT_AGENT_ERROR("Decode Server Message failed: %s\n", PB_GET_ERROR(istream));
			return false;
		}

		if (request.has_status) {
			dispatcher_context->update_status = request.status.status == nxp_iot_UpdateStatusReport_UpdateStatus_SUCCESS;
			if (dispatcher_context->status_report != NULL) {
				nxp_iot_UpdateStatusReport defaultReport = nxp_iot_UpdateStatusReport_init_zero;

				// First, release an eventually existing old status report.
				pb_release(nxp_iot_UpdateStatusReport_fields, dispatcher_context->status_report);

				// Copy the status report to the dispatcher. Note, this takes ownership of all the FT_POINTER fields.
				// Therefore it is necessary to clear them in the request (in order not to free them below).
				memcpy(dispatcher_context->status_report, &request.status, sizeof(*(dispatcher_context->status_report)));

				// Clear the FT_POINTER (actually all) fields in the status report in the request by
				// overwriting with a zero initialized report.
				memcpy(&request.status, &defaultReport, sizeof(request.status));
			}
		}

		dispatcher_context->closed = true;
		pb_release(nxp_iot_AgentGoodbyeRequest_fields, &request);
		return true;
	}
#if NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO
	else if (message_type == nxp_iot_AgentCrlRequest_fields) {
#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
        axTimeMeasurement_t iot_agent_crl_time = { 0 };
        initMeasurement(&iot_agent_crl_time);
#endif
		nxp_iot_AgentCrlRequest request = nxp_iot_AgentCrlRequest_init_default;
		if (!pb_decode_delimited(istream, nxp_iot_AgentCrlRequest_fields, &request)) {
			IOT_AGENT_ERROR("Decode Server Message failed: %s\n", PB_GET_ERROR(istream));
			return false;
		}

		nxp_iot_ResponsePayload response = nxp_iot_ResponsePayload_init_default;
		nxp_iot_AgentCrlResponse response_crl = nxp_iot_AgentCrlResponse_init_default;
		response.which_message = nxp_iot_ResponsePayload_crl_tag;
		response.message.crl = response_crl;

		if (request.crl != NULL) {
			nxp_iot_ServiceDescriptor* service_descriptor = dispatcher_context->service_descriptor;
			int network_status = network_verify_server_certificate(dispatcher_context->network_context,
				service_descriptor->server_certificate->bytes, (size_t)service_descriptor->server_certificate->size,
				request.crl->bytes, (size_t)request.crl->size, &response_crl.error);
			if (network_status != NETWORK_STATUS_OK) {
				IOT_AGENT_ERROR("Verification of server certificate failed.");
				response_crl.has_error = true;
			}
			dispatcher_context->successful_crl_verification_done = network_status == 0;
		}

		pb_release(nxp_iot_AgentCrlRequest_fields, &request);

		if (!dispatcher_context->successful_crl_verification_done) {
			dispatcher_context->closed = true;
		}

#if SSS_HAVE_HOSTCRYPTO_OPENSSL
		{
			int network_status = network_openssl_engine_session_disconnect((openssl_network_context_t*)dispatcher_context->network_context);
			if (network_status != NETWORK_STATUS_OK) {
				IOT_AGENT_ERROR("network_openssl_engine_session_disconnect failed with 0x%08x.", network_status);
				return false;
			}

			nxp_iot_ServiceDescriptor* service_descriptor = dispatcher_context->service_descriptor;
			iot_agent_context_t* agent_context = dispatcher_context->agent_context;

			iot_agent_keystore_t* keystore = NULL;
			uint32_t id = service_descriptor->client_key_sss_ref.endpoint_id;
			iot_agent_status_t agent_status = iot_agent_get_keystore_by_id(agent_context, id, &keystore);
			if (agent_status != IOT_AGENT_SUCCESS) {
				IOT_AGENT_ERROR("iot_agent_get_keystore_by_id for id 0x%08x failed with 0x%08x.", id, agent_status);
				return false;
			}

			agent_status = iot_agent_keystore_open_session(keystore);
			if (agent_status != IOT_AGENT_SUCCESS) {
				IOT_AGENT_ERROR("iot_agent_keystore_open_session failed with 0x%08x.", agent_status);
				return false;
			}
		}
#endif

		// And encode the actual payload.
		if (! encode_responses_from_payload(ostream, &response))
		{
			IOT_AGENT_ERROR("encode_responses_from_payload failed");
			return false;
		}
#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
        concludeMeasurement(&iot_agent_crl_time);
        iot_agent_time.crl_time = getMeasurement(&iot_agent_crl_time);
#endif
		return true;
	}
#endif
	return false;
}


iot_agent_status_t iot_agent_init_dispatcher(
    iot_agent_dispatcher_context_t * dispatcher_context,
	iot_agent_context_t* agent_context,
	nxp_iot_ServiceDescriptor* service_descriptor,
	nxp_iot_UpdateStatusReport* status_report)
{
    if ((dispatcher_context == NULL)||(agent_context == NULL))
    {
        IOT_AGENT_ERROR("Dispatcher Initialization Failed");
        return IOT_AGENT_FAILURE;
    }

    dispatcher_context->agent_context = agent_context;
    dispatcher_context->current_request = NULL;
    dispatcher_context->closed = false;
	dispatcher_context->successful_crl_verification_done = false;
	dispatcher_context->service_descriptor = service_descriptor;

	dispatcher_context->update_status = true;

	dispatcher_context->status_report = status_report;
	dispatcher_context->stream_type = STREAM_TYPE_NETWORK;

    for (size_t i = 0U; i < NXP_IOT_AGENT_MAX_NUM_ENDPOINTS; i++)
        dispatcher_context->endpoints[i].type = nxp_iot_EndpointType_INVALID;

    dispatcher_context->endpoints[0].type = nxp_iot_EndpointType_AGENT;
    dispatcher_context->endpoints[0].id = 0U;
    dispatcher_context->endpoints[0].endpoint_interface = iot_agent_endpoint_interface;
    dispatcher_context->endpoints[0].endpoint_context = dispatcher_context;
    size_t num_endpoints = 1U;

	if (agent_context->edgelock2go_datastore != NULL)
	{
		dispatcher_context->endpoints[num_endpoints].type = agent_context->edgelock2go_datastore->type;
		dispatcher_context->endpoints[num_endpoints].id = agent_context->edgelock2go_datastore->identifier;
		dispatcher_context->endpoints[num_endpoints].endpoint_interface = agent_context->edgelock2go_datastore->iface.endpoint_interface;
		dispatcher_context->endpoints[num_endpoints].endpoint_context = agent_context->edgelock2go_datastore->context;
		num_endpoints++;
	}

    for (size_t i = 0U; i < agent_context->numDatastores; i++)
    {
        dispatcher_context->endpoints[num_endpoints].type = agent_context->datastores[i]->type;
        dispatcher_context->endpoints[num_endpoints].id = agent_context->datastores[i]->identifier;
        dispatcher_context->endpoints[num_endpoints].endpoint_interface = agent_context->datastores[i]->iface.endpoint_interface;
        dispatcher_context->endpoints[num_endpoints].endpoint_context = agent_context->datastores[i]->context;
        num_endpoints++;
    }

    for (size_t i = 0U; i < agent_context->numKeystores; i++)
    {
        dispatcher_context->endpoints[num_endpoints].type = agent_context->keystores[i]->type;
        dispatcher_context->endpoints[num_endpoints].id = agent_context->keystores[i]->identifier;
        dispatcher_context->endpoints[num_endpoints].endpoint_interface = agent_context->keystores[i]->iface.endpoint_interface;
        dispatcher_context->endpoints[num_endpoints].endpoint_context = agent_context->keystores[i]->context;
        num_endpoints++;
	}

    return IOT_AGENT_SUCCESS;
}


