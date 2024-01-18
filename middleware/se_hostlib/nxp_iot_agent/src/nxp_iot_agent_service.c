/*
 * Copyright 2018-2021, 2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <nxp_iot_agent_service.h>
#include <nxp_iot_agent_utils.h>
#include <nxp_iot_agent_macros.h>

#include "./protobuf/pb.h"
#include "./protobuf/pb_decode.h"

#include "./protobuf/ServiceDescriptor.pb.h"

#if SSS_HAVE_HOSTCRYPTO_OPENSSL
#include <openssl/evp.h>
#endif
#if SSS_HAVE_HOSTCRYPTO_MBEDTLS
#include <mbedtls/version.h>
#include <mbedtls/sha256.h>
#endif

#include <string.h>

#define CONFIG_BUFFER_CHUNK_SIZE 64

/** @brief reads the buffer for the service out of the datastore
*   @param ctx
*   @param offset
*   @param buffer
*   @param expected_len
*   @return agent_status
*/
iot_agent_status_t iot_agent_service_read_buffer(const iot_agent_datastore_t* ctx, size_t offset, void* buffer, size_t expected_len)
{
	size_t len = expected_len;
	iot_agent_status_t agent_status = ctx->iface.read(ctx->context, buffer, offset, &len);
	ASSERT_OR_EXIT_SILENT(agent_status == IOT_AGENT_SUCCESS);
	ASSERT_OR_EXIT_SILENT(expected_len == len);
exit:
	return agent_status;
}

/** @brief This shall only be called on configuration data that was checked before to be valid, as
* 			there is no error checking done on this level any more.
*/
void iot_agent_service_read_header(const iot_agent_datastore_t* ctx, size_t offset, configuration_data_header_t* header)
{
	(void) iot_agent_service_read_buffer(ctx, offset, header, sizeof(*header));
}


#if SSS_HAVE_HOSTCRYPTO_OPENSSL

iot_agent_status_t iot_agent_service_calculate_cofiguration_checksum(const iot_agent_datastore_t* ctx,
	const configuration_data_header_t* header, uint8_t calculated_checksum[32])
{
	uint8_t buffer[CONFIG_BUFFER_CHUNK_SIZE] = { 0U };
	size_t offset = sizeof(header->checksum);
	size_t remaining = 0U;
	int success = 1;

	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
	EVP_MD_CTX* digest_context = EVP_MD_CTX_create();
#else
	EVP_MD_CTX* digest_context = EVP_MD_CTX_new();
#endif

	ASSERT_OR_EXIT_MSG((header->length >= offset), "Header length is lower than offset");
	remaining = header->length - offset;

	ASSERT_OR_EXIT_MSG(digest_context != NULL, "EVP_MD_CTX_new returned NULL");

	success &= EVP_DigestInit_ex(digest_context, EVP_sha256(), NULL);
	while (success && (remaining > 0U))
	{
		size_t chunk_size = remaining < sizeof(buffer) ? remaining : sizeof(buffer);
		iot_agent_service_read_buffer(ctx, offset, &buffer[0], chunk_size);
		success &= EVP_DigestUpdate(digest_context, &buffer[0], chunk_size);

		remaining -= chunk_size;
		offset += chunk_size;
	}

	if (success) {
		unsigned int calculted_checksum_len = (unsigned int)sizeof(header->checksum);
		success &= EVP_DigestFinal_ex(digest_context, &calculated_checksum[0], &calculted_checksum_len);
	}

	ASSERT_OR_EXIT_MSG((success == 1), "Error in checksum calculation");

exit:
#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
	EVP_MD_CTX_destroy(digest_context);
#else
	EVP_MD_CTX_free(digest_context);
#endif
	return agent_status;
}


#elif SSS_HAVE_HOSTCRYPTO_MBEDTLS

iot_agent_status_t iot_agent_service_calculate_cofiguration_checksum(const iot_agent_datastore_t* ctx,
	const configuration_data_header_t* header, uint8_t calculated_checksum[32])
{
	uint8_t buffer[CONFIG_BUFFER_CHUNK_SIZE] = { 0 };
	size_t offset = sizeof(header->checksum);
	size_t remaining = 0U;
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	ASSERT_OR_EXIT_MSG((header->length >= offset), "Header length is lower than offset");
	remaining = header->length - offset;

#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER < 0x03010000)
	mbedtls_sha256_context digest_context;
	mbedtls_sha256_init(&digest_context);
	mbedtls_sha256_starts_ret(&digest_context, 0);

	int failed = 0;
	while ((!failed) && (remaining > 0))
	{
		size_t chunk_size = remaining < sizeof(buffer) ? remaining : sizeof(buffer);
		iot_agent_service_read_buffer(ctx, offset, &buffer[0], chunk_size);
		failed |= mbedtls_sha256_update_ret(&digest_context, &buffer[0], chunk_size);

		remaining -= chunk_size;
		offset += chunk_size;
	}

	if (!failed) {
		failed |= mbedtls_sha256_finish_ret(&digest_context, &calculated_checksum[0]);
	}
#else
	mbedtls_sha256_context digest_context;
	mbedtls_sha256_init(&digest_context);
	mbedtls_sha256_starts(&digest_context, 0);

	int failed = 0;
	while ((!failed) && (remaining > 0))
	{
		size_t chunk_size = remaining < sizeof(buffer) ? remaining : sizeof(buffer);
		iot_agent_service_read_buffer(ctx, offset, &buffer[0], chunk_size);
		failed |= mbedtls_sha256_update(&digest_context, &buffer[0], chunk_size);

		remaining -= chunk_size;
		offset += chunk_size;
	}

	if (!failed) {
		failed |= mbedtls_sha256_finish(&digest_context, &calculated_checksum[0]);
	}
#endif
	ASSERT_OR_EXIT_MSG((failed == 0), "Error in checksum calculation");
exit:
	return agent_status;
}

#endif


bool iot_agent_service_is_configuration_data_valid(
	const iot_agent_datastore_t* ctx)
{
	configuration_data_header_t header = { 0U };
	uint8_t calculated_checksum[32] = { 0U };
	bool valid = false;

	iot_agent_status_t agent_status = iot_agent_service_read_buffer(ctx, 0U, &header, sizeof(header));
	ASSERT_OR_EXIT_SILENT(agent_status == IOT_AGENT_SUCCESS);
#if AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1
	ASSERT_OR_EXIT_MSG(header.length >= sizeof(header),
		"Length indicated in header [%lu] is too small to contain configuration data.",
		header.length);

	ASSERT_OR_EXIT_MSG(header.version == IOT_AGENT_CONFIGURATION_DATA_VERSION,
		"Version of configuration data [%lu] does not match version expected by firmware [%d]",
		header.version, IOT_AGENT_CONFIGURATION_DATA_VERSION);
#else
	ASSERT_OR_EXIT_MSG(header.length >= sizeof(header),
		"Length indicated in header [%d] is too small to contain configuration data.",
		header.length);

	ASSERT_OR_EXIT_MSG(header.version == IOT_AGENT_CONFIGURATION_DATA_VERSION,
		"Version of configuration data [%d] does not match version expected by firmware [%d]",
		header.version, IOT_AGENT_CONFIGURATION_DATA_VERSION);
#endif

	agent_status = iot_agent_service_calculate_cofiguration_checksum(ctx, &header, calculated_checksum);
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_service_calculate_cofiguration_checksum failed with 0x%08x", agent_status);

	valid = (memcmp(header.checksum, calculated_checksum, sizeof(calculated_checksum)) == 0);

exit:
	return valid;

}


/** @brief returns the string of the current service
*   @param service_descriptor
*   @param buffer
*   @return agent_status
*/
const char* STR_AWSSERVICE = "aws";
const char* STR_AZURESERVICE = "azure";
const char* STR_GOOGLESERVICE = "google";
const char* STR_IBMSERVICE = "ibm";
const char* STR_CUSTOMSERVICE = "custom";

iot_agent_status_t iot_agent_service_get_service_type_as_string(
	const nxp_iot_ServiceDescriptor* service_descriptor, const char** buffer)
{
	if (!service_descriptor->has_service_type) {
		IOT_AGENT_ERROR("Missing service type.");
		return IOT_AGENT_FAILURE;
	}

	switch (service_descriptor->service_type)
	{
	case nxp_iot_ServiceType_AWSSERVICE:
		*buffer = STR_AWSSERVICE;
		break;
	case nxp_iot_ServiceType_AZURESERVICE:
		*buffer = STR_AZURESERVICE;
		break;
	case nxp_iot_ServiceType_GOOGLESERVICE:
		*buffer = STR_GOOGLESERVICE;
		break;
	case nxp_iot_ServiceType_IBMSERVICE:
		*buffer = STR_IBMSERVICE;
		break;
	case nxp_iot_ServiceType_CUSTOMSERVICE:
		*buffer = STR_CUSTOMSERVICE;
		break;
	default:
		*buffer = NULL;
		IOT_AGENT_ERROR("Invalid service_type: %d", (int)service_descriptor->service_type);
		return IOT_AGENT_FAILURE;
	}
	return IOT_AGENT_SUCCESS;
}



/** @brief returns the string of the current protocol
*   @param service_descriptor
*   @param buffer
*   @return agent_status
*/
const char* STR_PROTOCOL_HTTPS = "HTTPS";
const char* STR_PROTOCOL_MQTTS = "MQTTS";
const char* STR_PROTOCOL_AMQPS = "AMQPS";
const char* STR_PROTOCOL_XMPP = "XMPP";
const char* STR_PROTOCOL_DDS = "DDS";
const char* STR_PROTOCOL_COAP = "COAP";
const char* STR_PROTOCOL_EMPTY = "empty";

iot_agent_status_t iot_agent_service_get_protocol_of_service_as_string(
	const nxp_iot_ServiceDescriptor* service_descriptor,
	const char** buffer)
{
	*buffer = STR_PROTOCOL_EMPTY;
	if (service_descriptor->has_protocol) {

		switch (service_descriptor->protocol)
		{
		case nxp_iot_ServiceProtocolType_HTTPS:
			*buffer = STR_PROTOCOL_HTTPS;
			break;
		case nxp_iot_ServiceProtocolType_MQTTS:
			*buffer = STR_PROTOCOL_MQTTS;
			break;
		case nxp_iot_ServiceProtocolType_AMQPS:
			*buffer = STR_PROTOCOL_AMQPS;
			break;
		case nxp_iot_ServiceProtocolType_XMPP:
			*buffer = STR_PROTOCOL_XMPP;
			break;
		case nxp_iot_ServiceProtocolType_DDS:
			*buffer = STR_PROTOCOL_DDS;
			break;
		case nxp_iot_ServiceProtocolType_COAP:
			*buffer = STR_PROTOCOL_COAP;
			break;
		default:
			*buffer = NULL;
			IOT_AGENT_ERROR("Invalid protocol: %d", (int)service_descriptor->protocol);
			return IOT_AGENT_FAILURE;
		}
	}
	return IOT_AGENT_SUCCESS;
}



/** @brief returns the number of the service
*   @param ctx
*   @return header.number_of_services
**/
size_t iot_agent_service_get_number_of_services(
	const iot_agent_datastore_t* ctx)
{
	configuration_data_header_t header = { 0U };
	iot_agent_service_read_header(ctx, 0U, &header);
	return (size_t)header.number_of_services;
}


/** @brief returns the number of the service
*   @param ctx
*   @param index
*   @param offset
*   @param service_descriptor
*   @return agent_status
*/
iot_agent_status_t iot_agent_service_get_service_offset_by_index(
	const iot_agent_datastore_t* datastore,
	size_t index,
	size_t* offset,
	nxp_iot_ServiceDescriptor* service_descriptor)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	size_t number_of_services = 0U;
	ASSERT_OR_EXIT_MSG(offset != NULL, "offset is NULL");
	ASSERT_OR_EXIT_MSG(service_descriptor != NULL, "service_descriptor is NULL");

	*offset = sizeof(configuration_data_header_t);
	number_of_services = iot_agent_service_get_number_of_services(datastore);
	for (size_t i = 0U; i < number_of_services; ++i) {

		datastore_stream_context_t datastore_stream_context = { datastore, *offset };
		pb_istream_t stream = { &datastore_read_callback, &datastore_stream_context, PB_SIZE_MAX, 0 };
		pb_release(nxp_iot_ServiceDescriptor_fields, service_descriptor);
		bool pb_status = pb_decode_delimited(&stream, nxp_iot_ServiceDescriptor_fields, service_descriptor);
		ASSERT_OR_EXIT_MSG(pb_status == true, "Unable to decode service descriptor at 0x%08x", *offset);

		if (i == index)
			break;

		*offset = datastore_stream_context.offset;
	}

exit:
	return agent_status;
}



/** @brief returns the number the service offset by id
*   @param datastore
*   @param service_id
*   @param offset
*   @param service_descriptor
*   @return agent_status
*/
iot_agent_status_t iot_agent_service_get_service_offset_by_id(
	const iot_agent_datastore_t* datastore,
	uint64_t service_id,
	size_t* offset,
	nxp_iot_ServiceDescriptor* service_descriptor)
{
	iot_agent_status_t agent_status = IOT_AGENT_FAILURE;
	size_t number_of_services = 0U;
	ASSERT_OR_EXIT_MSG(offset != NULL, "offset is NULL");
	ASSERT_OR_EXIT_MSG(service_descriptor != NULL, "service_descriptor is NULL");

	*offset = sizeof(configuration_data_header_t);
	number_of_services = iot_agent_service_get_number_of_services(datastore);
	for (size_t i = 0U; i < number_of_services; ++i) {
		datastore_stream_context_t datastore_stream_context = { datastore, *offset };
		pb_istream_t stream = { &datastore_read_callback, &datastore_stream_context, PB_SIZE_MAX, 0 };
		pb_release(nxp_iot_ServiceDescriptor_fields, service_descriptor);
		bool pb_status = pb_decode_delimited(&stream, nxp_iot_ServiceDescriptor_fields, service_descriptor);
		ASSERT_OR_EXIT_MSG(pb_status == true, "Unable to decode service descriptor at 0x%08x", *offset);

		if (service_descriptor->identifier == service_id) {
			agent_status = IOT_AGENT_SUCCESS;
			break;
		}

		*offset = datastore_stream_context.offset;
	}
exit:
	return agent_status;
}


/** @brief returns the number the service offset by id
*   @param datastore
*   @param service_id
*   @param offset
*   @param service_descriptor
*   @return agent_status
*/
iot_agent_status_t iot_agent_service_get_service_descriptor_of_service(
	const iot_agent_datastore_t* datastore,
	size_t offset,
	nxp_iot_ServiceDescriptor* service_descriptor)
{
	bool pb_status;
	datastore_stream_context_t datastore_stream_context = { datastore, offset };
	pb_istream_t stream = { &datastore_read_callback, &datastore_stream_context, PB_SIZE_MAX, 0 };
	iot_agent_service_free_service_descriptor(service_descriptor);
	pb_status = pb_decode_delimited(&stream, nxp_iot_ServiceDescriptor_fields, service_descriptor);
	return (pb_status == true) ? IOT_AGENT_SUCCESS : IOT_AGENT_FAILURE;
}


void iot_agent_service_free_service_descriptor(nxp_iot_ServiceDescriptor* service_descriptor)
{
	pb_release(nxp_iot_ServiceDescriptor_fields, service_descriptor);
}

