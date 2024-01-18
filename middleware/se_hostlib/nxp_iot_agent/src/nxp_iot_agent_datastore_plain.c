/* 
 * Copyright 2018-2022 NXP
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 */
#include <nxp_iot_agent_datastore_plain.h>
#include <nxp_iot_agent_utils_protobuf.h>
#include <nxp_iot_agent_macros.h>

#include "./protobuf/Dispatcher.pb.h"
#include "./protobuf/Datastore.pb.h"

#define IOT_AGENT_DATASTORE_PLAIN_VERSION_MAJOR (1U)
#define IOT_AGENT_DATASTORE_PLAIN_VERSION_MINOR (0U)
#define IOT_AGENT_DATASTORE_PLAIN_VERSION_PATCH (0U)
#define IOT_AGENT_DATASTORE_PLAIN_VERSION ((uint32_t)(((IOT_AGENT_DATASTORE_PLAIN_VERSION_MAJOR * 256U) \
		+ IOT_AGENT_DATASTORE_PLAIN_VERSION_MINOR) * 256U + IOT_AGENT_DATASTORE_PLAIN_VERSION_PATCH))

const iot_agent_datastore_interface_t iot_agent_datastore_plain_interface =
{
	&iot_agent_datastore_plain_destroy,
	&iot_agent_datastore_plain_allocate,
    &iot_agent_datastore_plain_write,
    &iot_agent_datastore_plain_read,
	&iot_agent_datastore_plain_commit,
	{
        &iot_agent_datastore_plain_get_endpoint_info,
        &iot_agent_datastore_plain_handle_request,
    },
};

iot_agent_status_t iot_agent_datastore_plain_init(iot_agent_datastore_t* datastore,
	uint32_t identifier)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	datastore->iface = iot_agent_datastore_plain_interface;
	datastore->type = IOT_AGENT_DS_PLAIN;
	datastore->identifier = identifier;

	iot_agent_datastore_plain_context_t* datastore_context = malloc(sizeof(iot_agent_datastore_plain_context_t));
	ASSERT_OR_EXIT_STATUS(datastore_context != NULL, IOT_AGENT_ERROR_MEMORY);
	memset(datastore_context, 0, sizeof(iot_agent_datastore_plain_context_t));
	datastore->context = datastore_context;

exit:
	return agent_status;
}


iot_agent_status_t iot_agent_datastore_plain_destroy(
	void* context)
{
	iot_agent_datastore_plain_context_t* datastore_context = (iot_agent_datastore_plain_context_t*)context;
	
	if (datastore_context != NULL) {
		free(datastore_context->buffers[0]);
		datastore_context->buffers[0] = NULL;
		datastore_context->size[0] = 0U;

		free(datastore_context->buffers[1]);
		datastore_context->buffers[1] = NULL;
		datastore_context->size[1] = 0U;

		datastore_context->idx_read = 0U;
		datastore_context->idx_write = 0U;
	}

	return IOT_AGENT_SUCCESS;
}


/** @brief allocates a plain datastore
*	@pre len > 0 
*   @param context
*   @param len
*   @return agent_status
*/
iot_agent_status_t iot_agent_datastore_plain_allocate(
	void* context, size_t len)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	iot_agent_datastore_plain_context_t* datastore_context = (iot_agent_datastore_plain_context_t*) context;
	ASSERT_OR_EXIT_MSG(len > 0U, "Cannot allocate with size 0.");

	// If currently read and write indices are different, there is a transaction 
	// ongoing already. In that case recover by terminating (without commit) the 
	// ongoing transaction and starting a fresh one.
	if (datastore_context->idx_read != datastore_context->idx_write) {
		free(datastore_context->buffers[datastore_context->idx_write]);
		datastore_context->buffers[datastore_context->idx_write] = NULL;
		datastore_context->size[datastore_context->idx_write] = 0U;
	}
	else {
		datastore_context->idx_write = (datastore_context->idx_write + 1U) & 0x01U;
	}

	datastore_context->buffers[datastore_context->idx_write] = malloc(len);
	ASSERT_OR_EXIT_STATUS_MSG(datastore_context->buffers[datastore_context->idx_write] != NULL,
		IOT_AGENT_ERROR_MEMORY, "Unable to allocate memory of size %u", len);
	datastore_context->size[datastore_context->idx_write] = len;

exit:
	return agent_status;
}


iot_agent_status_t iot_agent_datastore_plain_write(
	void* context, size_t offset, const void* src, size_t len)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	iot_agent_datastore_plain_context_t* datastore_context = (iot_agent_datastore_plain_context_t*) context;

	uint8_t* write_buffer = datastore_context->buffers[datastore_context->idx_write];
	size_t write_buffer_size = datastore_context->size[datastore_context->idx_write];

	ASSERT_OR_EXIT_MSG(datastore_context != NULL, "datastore_context is NULL.");
	ASSERT_OR_EXIT_MSG(write_buffer != NULL, "write_buffer is NULL.");
	ASSERT_OR_EXIT_MSG((offset + len) >= offset, "Overflow in addition calculation.");
	ASSERT_OR_EXIT_MSG(offset + len <= write_buffer_size, 
		"Writing out of bounds. Offset: %u, len: %u, write_buffer_size: %u.",
		offset, len, write_buffer_size);

	memcpy(write_buffer + offset, src, len);
exit:
	return agent_status;
}


iot_agent_status_t iot_agent_datastore_plain_read(
	void* context, void* dst, size_t offset, size_t* len)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	iot_agent_datastore_plain_context_t* datastore_context = (iot_agent_datastore_plain_context_t*) context;
	uint8_t* read_buffer = datastore_context->buffers[datastore_context->idx_read];
	size_t read_buffer_size = datastore_context->size[datastore_context->idx_read];

	if (read_buffer == NULL) { 
		*len = 0U;
		return agent_status;
	}
	ASSERT_OR_EXIT_MSG((offset + *len) >= offset, "Overflow in addition calculation.");
	if (offset + *len > read_buffer_size) { 
		*len = read_buffer_size - offset;
	}
	memcpy(dst, read_buffer + offset, *len);
exit:
	return agent_status;
}


bool iot_agent_datastore_plain_get_endpoint_info(
	void *context, void* endpoint_information)
{
	AX_UNUSED_ARG(context);
	nxp_iot_EndpointInformation* info = (nxp_iot_EndpointInformation*)endpoint_information;
	info->has_version = true;
	info->version = IOT_AGENT_DATASTORE_PLAIN_VERSION;

	nxp_iot_DatastoreAdditionalData additional_data = nxp_iot_DatastoreAdditionalData_init_default;

	additional_data.has_supportsTransactions = true;
	additional_data.supportsTransactions = true;

	info->has_additionalData = true;
	pb_ostream_t ostream = pb_ostream_from_buffer(info->additionalData.bytes, sizeof(info->additionalData.bytes));
	bool encode_status = pb_encode_delimited(&ostream, nxp_iot_AgentAdditionalData_fields, &additional_data);
	info->additionalData.size = (pb_size_t)ostream.bytes_written;
	return encode_status;
}


iot_agent_status_t iot_agent_datastore_plain_commit(void* context)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	iot_agent_datastore_plain_context_t* datastore_context = (iot_agent_datastore_plain_context_t*)context;

	ASSERT_OR_EXIT_MSG(datastore_context->idx_read != datastore_context->idx_write,
		"There is no transaction to commit.");

	// Free the current read buffer
	free(datastore_context->buffers[datastore_context->idx_read]);
	datastore_context->buffers[datastore_context->idx_read] = NULL;
	datastore_context->size[datastore_context->idx_read] = 0U;

	// Make the reads point to the write buffer.
	datastore_context->idx_read = datastore_context->idx_write;

exit:
	return agent_status;
}


#define MAX_VARINT_SIZE 5U

typedef struct iot_agent_datastore_plain_handle_write_data_t {
	iot_agent_datastore_plain_context_t* datastore_context;
	nxp_iot_DatastoreRequest* request;
} iot_agent_datastore_plain_handle_write_data_t;


bool iot_agent_datastore_plain_handle_write_data(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	AX_UNUSED_ARG(field);
	iot_agent_datastore_plain_handle_write_data_t* context = (iot_agent_datastore_plain_handle_write_data_t*)(*arg);
	iot_agent_datastore_plain_context_t* datastore_context = context->datastore_context;
	nxp_iot_DatastoreRequest* request = context->request;
	
	if (datastore_context == NULL) {
		IOT_AGENT_ERROR("datastore_context is NULL.");
		return false;
	}

	uint8_t* write_buffer = datastore_context->buffers[datastore_context->idx_write];
	size_t write_buffer_size = datastore_context->size[datastore_context->idx_write];

	size_t offset = (size_t)request->offset;
	size_t len = stream->bytes_left;

	if (write_buffer == NULL) {
		IOT_AGENT_ERROR("Write_buffer is NULL.");
		return false;
	}

	if ((offset + len) < offset) {
		IOT_AGENT_ERROR("Overflow in addition calculation.");
		return false;
	}

	if (offset + len > write_buffer_size) {
		IOT_AGENT_ERROR("Attempting to write beyond size of datastore.");
		return false;

	}

	if (!pb_read(stream, (pb_byte_t*)(write_buffer + offset), len)) {
		IOT_AGENT_ERROR("Decoding of byte stream failed.");
		return false;
	}

	return true;
}


bool iot_agent_datastore_plain_handle_request(pb_istream_t *istream,
	pb_ostream_t *ostream, const pb_field_t* message_type, void *context)
{
	iot_agent_datastore_plain_context_t* datastore_context = (iot_agent_datastore_plain_context_t*)context;
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	bool result = true;

	iot_agent_response_buffer_t* response_buffer = NULL;
	uint8_t* oneof_length_buffer = NULL;
	uint8_t* datastore_response_length_buffer = NULL;
	uint8_t* data_length_buffer = NULL;
	size_t read_data_len = 0;

	nxp_iot_DatastoreRequest request = nxp_iot_DatastoreRequest_init_default;
	iot_agent_datastore_plain_handle_write_data_t write_args = { 0 };

	result = nxp_iot_DatastoreRequest_fields == message_type;
	RESULT_TRUE_OR_EXIT_MSG("unsupported message type");

	write_args.datastore_context = datastore_context;
	write_args.request = &request;

	request.data.funcs.decode = &iot_agent_datastore_plain_handle_write_data;
	request.data.arg = &write_args;

	result = pb_decode_delimited(istream, nxp_iot_DatastoreRequest_fields, &request);
	RESULT_TRUE_OR_EXIT_MSG("decoding of server message failed: %s\n", PB_GET_ERROR(istream));

	if (request.has_operation)
	{
		if (request.operation == nxp_iot_DatastoreOperation_READ)
		{
			// For reading, we read from the file and write to the response 
			// buffer in-place. This means we need to take care about 
			// encoding ourselves.
			response_buffer = pb_ostream_response_buffer_get_internal_buffer(ostream);

			// The oneof selection for the datastore message:
			result = pb_encode_tag(ostream, PB_WT_STRING, nxp_iot_Responses_responses_tag);
			RESULT_TRUE_OR_EXIT();
			oneof_length_buffer = response_buffer->pos;
			result = pb_encode_varint(ostream, INT32_MAX); // placeholder
			RESULT_TRUE_OR_EXIT();

			// The datastore response:
			result = pb_encode_tag(ostream, PB_WT_STRING, nxp_iot_ResponsePayload_datastore_tag);
			RESULT_TRUE_OR_EXIT();
			datastore_response_length_buffer = response_buffer->pos;
			result = pb_encode_varint(ostream, INT32_MAX); // placeholder
			RESULT_TRUE_OR_EXIT();

			// Status field:
			result = pb_encode_tag(ostream, PB_WT_VARINT, nxp_iot_DatastoreResponse_status_tag);
			RESULT_TRUE_OR_EXIT();
			result = pb_encode_varint(ostream, nxp_iot_DatastoreStatus_OK);
			RESULT_TRUE_OR_EXIT();

			// Data field:
			result = pb_encode_tag(ostream, PB_WT_STRING, nxp_iot_DatastoreResponse_data_tag);
			RESULT_TRUE_OR_EXIT();
			data_length_buffer = response_buffer->pos;
			result = pb_encode_varint(ostream, INT32_MAX); // placeholder
			RESULT_TRUE_OR_EXIT();

			// Write the data:
			read_data_len = datastore_context->size[datastore_context->idx_read] - (size_t)request.offset;
			if ((size_t) request.length < read_data_len) read_data_len = (size_t)request.length;

			result = response_buffer->remaining >= read_data_len;
			RESULT_TRUE_OR_EXIT_MSG("not enough remaining free space in buffer for datastore read response");

			agent_status = iot_agent_datastore_plain_read(datastore_context, response_buffer->pos, (size_t)request.offset, &read_data_len);
			pb_response_buffer_consume_bytes(response_buffer, read_data_len);
			result = agent_status == IOT_AGENT_SUCCESS;
			RESULT_TRUE_OR_EXIT_MSG("unable to read from datastore");

			// Fill the length in the data field:
			result = encode_varint_with_fixed_length(read_data_len, data_length_buffer, MAX_VARINT_SIZE);
			RESULT_TRUE_OR_EXIT();

			// Fill the length in the datastore response field:
			result = encode_varint_with_fixed_length(read_data_len + (size_t)(data_length_buffer - datastore_response_length_buffer),
				datastore_response_length_buffer, MAX_VARINT_SIZE);
			RESULT_TRUE_OR_EXIT();

			// Fill the length in the oneof message field:
			result = encode_varint_with_fixed_length(read_data_len + (size_t)(data_length_buffer - oneof_length_buffer),
				oneof_length_buffer, MAX_VARINT_SIZE);
			RESULT_TRUE_OR_EXIT();
		}
		else if (request.operation == nxp_iot_DatastoreOperation_WRITE)
		{
			result = iot_agent_datastore_encode_datastore_ok_response(ostream);
			RESULT_TRUE_OR_EXIT_MSG("iot_agent_datastore_encode_datastore_ok_response failed");
		}
		else if (request.operation == nxp_iot_DatastoreOperation_ALLOCATE)
		{
			result = request.has_length;
			RESULT_TRUE_OR_EXIT_MSG("missing length for allocate operation");
				
			agent_status = iot_agent_datastore_plain_allocate(context, (size_t)request.length);
			result = agent_status == IOT_AGENT_SUCCESS;
			RESULT_TRUE_OR_EXIT_MSG("iot_agent_datastore_plain_allocate failed");

			result = iot_agent_datastore_encode_datastore_ok_response(ostream);
			RESULT_TRUE_OR_EXIT_MSG("iot_agent_datastore_encode_datastore_ok_response failed");
		}
		else if (request.operation == nxp_iot_DatastoreOperation_COMMIT)
		{
			agent_status = iot_agent_datastore_plain_commit(context);
			result = agent_status == IOT_AGENT_SUCCESS;
			RESULT_TRUE_OR_EXIT_MSG("iot_agent_datastore_plain_commit failed");

			result = iot_agent_datastore_encode_datastore_ok_response(ostream);
			RESULT_TRUE_OR_EXIT_MSG("iot_agent_datastore_encode_datastore_ok_response failed");
		}
	}

exit:
	return result;
}



