/* 
 * Copyright 2018, 2019, 2020, 2021, 2022 NXP
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#if !(defined(__ICCARM__) || defined(__CC_ARM) || defined(__ARMCC_VERSION))
#include <nxp_iot_agent_datastore_fs.h>
#include <nxp_iot_agent_utils_protobuf.h>
#include <nxp_iot_agent_macros.h>

#include "./protobuf/Dispatcher.pb.h"
#include "./protobuf/Datastore.pb.h"

#define IOT_AGENT_DATASTORE_FS_VERSION_MAJOR (1U)
#define IOT_AGENT_DATASTORE_FS_VERSION_MINOR (0U)
#define IOT_AGENT_DATASTORE_FS_VERSION_PATCH (0U)
#define IOT_AGENT_DATASTORE_FS_VERSION ((uint32_t)(((IOT_AGENT_DATASTORE_FS_VERSION_MAJOR * 256U) \
		+ IOT_AGENT_DATASTORE_FS_VERSION_MINOR) * 256U + IOT_AGENT_DATASTORE_FS_VERSION_PATCH))


const iot_agent_datastore_interface_t iot_agent_datastore_fs_interface =
{
	&iot_agent_datastore_fs_destroy,
	&iot_agent_datastore_fs_allocate,
    &iot_agent_datastore_fs_write,
    &iot_agent_datastore_fs_read,
	&iot_agent_datastore_fs_commit,
	{
        &iot_agent_datastore_fs_get_endpoint_info,
        &iot_agent_datastore_fs_handle_request,
    },
};


iot_agent_status_t iot_agent_datastore_fs_open_file(FILE** fp,
	const char* basename, size_t index, const char* mode)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	size_t filename_size = strlen(basename) + 16U;
	char* filename = malloc(filename_size);

	if (filename == NULL)
	{
		return IOT_AGENT_ERROR_MEMORY;
	}

	snprintf(filename, filename_size, "%s.%u", basename, index);

	if (*fp != NULL) {
		fclose(*fp);
		*fp = NULL;
	}

	*fp = fopen(filename, mode);

	if (*fp == NULL) {
		agent_status = IOT_AGENT_ERROR_FILE_SYSTEM;
	}

	free(filename);
	return agent_status;
}


iot_agent_status_t iot_agent_datastore_fs_init(iot_agent_datastore_t* datastore,
	uint32_t identifier, const char* basename, iot_agent_datastore_validator_t validator)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	size_t basename_size = 0U;
	iot_agent_datastore_t validating_datastore = { 0 };
	bool valid[2] = { false };
	datastore->iface = iot_agent_datastore_fs_interface;
	datastore->type = IOT_AGENT_DS_FS;
	datastore->identifier = identifier;

	iot_agent_datastore_fs_context_t* datastore_context = malloc(sizeof(iot_agent_datastore_fs_context_t));
	ASSERT_OR_EXIT_STATUS(datastore_context != NULL, IOT_AGENT_ERROR_MEMORY);
	memset(datastore_context, 0, sizeof(iot_agent_datastore_fs_context_t));
	datastore->context = datastore_context;

	basename_size = strlen(basename) + 1U;
	datastore_context->basename = malloc(basename_size);
	ASSERT_OR_EXIT_STATUS(datastore_context->basename != NULL, IOT_AGENT_ERROR_MEMORY);
	memcpy(datastore_context->basename, basename, basename_size);

	validating_datastore.context = datastore_context;
	validating_datastore.iface = iot_agent_datastore_fs_interface;

	datastore_context->idx_read = 0U;
	agent_status = iot_agent_datastore_fs_open_file(&datastore_context->fp[datastore_context->idx_read], datastore_context->basename, datastore_context->idx_read, "r+b");
	if (agent_status == IOT_AGENT_SUCCESS) {
		valid[datastore_context->idx_read] = validator(&validating_datastore);
	}

	datastore_context->idx_read = 1U;
	agent_status = iot_agent_datastore_fs_open_file(&datastore_context->fp[datastore_context->idx_read], datastore_context->basename, datastore_context->idx_read, "r+b");
	if (agent_status == IOT_AGENT_SUCCESS) {
		valid[datastore_context->idx_read] = validator(&validating_datastore);
	}

	if (valid[0])
	{
		// Reopen file 1, discarding contents.
		agent_status = iot_agent_datastore_fs_open_file(&datastore_context->fp[1], datastore_context->basename, 1U, "w+b");
		AGENT_SUCCESS_OR_EXIT_MSG("Unable to open file [%s.%d] with mode [%s]", basename, 1, "w+b");
		datastore_context->idx_read = 0U;
		datastore_context->idx_write = 0U;
	}
	else if (valid[1])
	{
		// Reopen file 0, discarding contents.
		agent_status = iot_agent_datastore_fs_open_file(&datastore_context->fp[0], datastore_context->basename, 0U, "w+b");
		AGENT_SUCCESS_OR_EXIT_MSG("Unable to open file [%s.%d] with mode [%s]", basename, 0, "w+b");
		datastore_context->idx_read = 1U;
		datastore_context->idx_write = 1U;
	}
	else 
	{
		// None of them is valid, reopen both.
		agent_status = iot_agent_datastore_fs_open_file(&datastore_context->fp[0], datastore_context->basename, 0U, "w+b");
		AGENT_SUCCESS_OR_EXIT_MSG("Unable to open file [%s.%d] with mode [%s]", basename, 0, "w+b");

		agent_status = iot_agent_datastore_fs_open_file(&datastore_context->fp[1], datastore_context->basename, 1U, "w+b");
		AGENT_SUCCESS_OR_EXIT_MSG("Unable to open file [%s.%d] with mode [%s]", basename, 1, "w+b");

		// Use the first one as backing storage.
		datastore_context->idx_read = 0U;
		datastore_context->idx_write = 0U;
	}

exit:
	if (agent_status != IOT_AGENT_SUCCESS) {
		iot_agent_datastore_fs_destroy(datastore_context);
	}

	return agent_status;
}


iot_agent_status_t iot_agent_datastore_fs_destroy(
	void* context)
{
	iot_agent_datastore_fs_context_t* datastore_context = (iot_agent_datastore_fs_context_t*)context;
	if (datastore_context != NULL) {

		if (datastore_context->fp[0] != NULL) {
			fclose(datastore_context->fp[0]);
			datastore_context->fp[0] = NULL;
		}

		if (datastore_context->fp[1] != NULL) {
			fclose(datastore_context->fp[1]);
			datastore_context->fp[1] = NULL;
		}

		free(datastore_context->basename);
		datastore_context->basename = NULL;
	}
	return IOT_AGENT_SUCCESS;
}


iot_agent_status_t iot_agent_datastore_fs_allocate(
	void* context, size_t len)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	iot_agent_datastore_fs_context_t* datastore_context = (iot_agent_datastore_fs_context_t*)context;
	ASSERT_OR_EXIT_MSG(len > 0U, "Cannot allocate with size 0.");

	// If currently read and write indices are different, there is a transaction 
	// ongoing already. In that case recover by terminating (without commit) the 
	// ongoing transaction and starting a fresh one.
	if (datastore_context->idx_read != datastore_context->idx_write) {
		if (datastore_context->fp[datastore_context->idx_write] != NULL) {
			fclose(datastore_context->fp[datastore_context->idx_write]);
			datastore_context->fp[datastore_context->idx_write] = NULL;
		}
	}
	else {
		datastore_context->idx_write = (datastore_context->idx_write + 1U) & 0x01U;
	}

	agent_status = iot_agent_datastore_fs_open_file(&datastore_context->fp[datastore_context->idx_write], datastore_context->basename, datastore_context->idx_write, "w+b");
	ASSERT_OR_EXIT_STATUS_MSG(agent_status == IOT_AGENT_SUCCESS, IOT_AGENT_ERROR_FILE_SYSTEM, 
		"Unable to open file [%s.%u].", datastore_context->basename, datastore_context->idx_write);

exit:
	return agent_status;
}


iot_agent_status_t iot_agent_datastore_fs_write(
	void* context, size_t offset, const void* src, size_t len)
{
	int fp_status = 0;
	size_t written = 0U;
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	iot_agent_datastore_fs_context_t* datastore_context = (iot_agent_datastore_fs_context_t*) context;

	ASSERT_OR_EXIT_MSG(datastore_context != NULL, "datastore_context is NULL.");
	ASSERT_OR_EXIT_MSG(datastore_context->fp != NULL, "datastore_context->fp is NULL.");

	fp_status = fseek(datastore_context->fp[datastore_context->idx_write], offset, SEEK_SET);
	ASSERT_OR_EXIT_MSG(fp_status == 0, "fseek failed: 0x%08x.", fp_status);

	written = fwrite(src, sizeof(uint8_t), len, datastore_context->fp[datastore_context->idx_write]);
	AGENT_SUCCESS_OR_EXIT_MSG("Only %u bytes were written (wanted %u).", written, len);
exit:
	return agent_status;
}


iot_agent_status_t iot_agent_datastore_fs_read(
	void* context, void* dst, size_t offset, size_t* len)
{
	iot_agent_datastore_fs_context_t* datastore_context = (iot_agent_datastore_fs_context_t*)context;
	int fp_status = fseek(datastore_context->fp[datastore_context->idx_read], offset, SEEK_SET);
	if (fp_status != 0) { *len = 0U; }
	*len = fread(dst, sizeof(uint8_t), *len, datastore_context->fp[datastore_context->idx_read]);
	return IOT_AGENT_SUCCESS;
}


iot_agent_status_t iot_agent_datastore_fs_commit(
	void* context)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	iot_agent_datastore_fs_context_t* datastore_context = (iot_agent_datastore_fs_context_t*)context;

	ASSERT_OR_EXIT_MSG(datastore_context->idx_read != datastore_context->idx_write,
		"There is no transaction to commit.");

	// Flush the write file to make sure its stored.
	fflush(datastore_context->fp[datastore_context->idx_write]);

	// Empty the other file.
	fclose(datastore_context->fp[datastore_context->idx_read]);
	datastore_context->fp[datastore_context->idx_read] = NULL;
	agent_status = iot_agent_datastore_fs_open_file(&datastore_context->fp[datastore_context->idx_read], 
		datastore_context->basename, datastore_context->idx_read, "w+b");

	if (agent_status != IOT_AGENT_SUCCESS) {
		IOT_AGENT_ERROR("Unable to open file [%s.%u] with mode [%s]", datastore_context->basename, datastore_context->idx_read, "w+b");
	}

	// Make the reads point to the write buffer.
	datastore_context->idx_read = datastore_context->idx_write;

exit:
	return agent_status;
}


bool iot_agent_datastore_fs_get_endpoint_info(
	void *context, void* endpoint_information)
{
	AX_UNUSED_ARG(context);
	nxp_iot_EndpointInformation* info = (nxp_iot_EndpointInformation*)endpoint_information;
	info->has_version = true;
	info->version = IOT_AGENT_DATASTORE_FS_VERSION;

	nxp_iot_DatastoreAdditionalData additional_data = nxp_iot_DatastoreAdditionalData_init_default;

	additional_data.has_supportsTransactions = true;
	additional_data.supportsTransactions = true;

	info->has_additionalData = true;
	pb_ostream_t ostream = pb_ostream_from_buffer(info->additionalData.bytes, sizeof(info->additionalData.bytes));
	bool encode_status = pb_encode_delimited(&ostream, nxp_iot_AgentAdditionalData_fields, &additional_data);
	info->additionalData.size = (pb_size_t)ostream.bytes_written;
	return encode_status;
}


typedef struct iot_agent_datastore_fs_handle_write_data_t {
	iot_agent_datastore_fs_context_t* datastore_context;
	nxp_iot_DatastoreRequest* request;
} iot_agent_datastore_fs_handle_write_data_t;


bool iot_agent_datastore_fs_handle_write_data(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	AX_UNUSED_ARG(field);
	iot_agent_datastore_fs_handle_write_data_t* context = (iot_agent_datastore_fs_handle_write_data_t*)(*arg);
	iot_agent_datastore_fs_context_t* datastore_context = context->datastore_context;
	nxp_iot_DatastoreRequest* request = context->request;

	size_t offset = (size_t)request->offset;
	size_t len = stream->bytes_left;
	size_t remaining = len;
	uint8_t buffer[1024];
	
	memset(buffer, 0x00, sizeof(buffer));

	while (remaining > 0U) {
		size_t chunk_size = remaining < sizeof(buffer) ? remaining : sizeof(buffer);
		if (!pb_read(stream, (pb_byte_t*)(buffer), chunk_size)) {
			IOT_AGENT_ERROR("decoding of byte stream failed");
			return false;
		}
		iot_agent_datastore_fs_write(datastore_context, offset, buffer, chunk_size);
		offset += chunk_size;
		remaining -= chunk_size;
	}
	return true;
}


#define MAX_VARINT_SIZE 5U

bool iot_agent_datastore_fs_handle_request(pb_istream_t *istream,
	pb_ostream_t *ostream, const pb_field_t* message_type, void *context)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	bool result = true;

	iot_agent_response_buffer_t* response_buffer = NULL;
	uint8_t* oneof_length_buffer = NULL;
	uint8_t* datastore_response_length_buffer = NULL;
	uint8_t* data_length_buffer = NULL;
	size_t read_data_len = 0;

	nxp_iot_DatastoreRequest request = nxp_iot_DatastoreRequest_init_default;
	iot_agent_datastore_fs_handle_write_data_t write_args;

	result = nxp_iot_DatastoreRequest_fields == message_type;
	RESULT_TRUE_OR_EXIT_MSG("unsupported message type");

	// For writing, we write to the file directly.
	// Note: We rely here on the messages being encoded in 
	// the right sequence - offset needs to be decoded before the data!
	write_args.datastore_context = context;
	write_args.request = &request;

	request.data.funcs.decode = &iot_agent_datastore_fs_handle_write_data;
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
			pb_encode_varint(ostream, nxp_iot_DatastoreStatus_OK);
			RESULT_TRUE_OR_EXIT();

			// Data field:
			result = pb_encode_tag(ostream, PB_WT_STRING, nxp_iot_DatastoreResponse_data_tag);
			RESULT_TRUE_OR_EXIT();
			data_length_buffer = response_buffer->pos;
			result = pb_encode_varint(ostream, INT32_MAX); // placeholder
			RESULT_TRUE_OR_EXIT();

			result = response_buffer->remaining >= (size_t)request.length;
			RESULT_TRUE_OR_EXIT_MSG("not enough remaining free space in buffer for datastore read response");

			read_data_len = (size_t)request.length;
			agent_status = iot_agent_datastore_fs_read(context, response_buffer->pos, (size_t)request.offset, &read_data_len);
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

			agent_status = iot_agent_datastore_fs_allocate(context, (size_t)request.length);
			result = agent_status == IOT_AGENT_SUCCESS;
			RESULT_TRUE_OR_EXIT_MSG("iot_agent_datastore_fs_allocate failed");

			result = iot_agent_datastore_encode_datastore_ok_response(ostream);
			RESULT_TRUE_OR_EXIT_MSG("iot_agent_datastore_encode_datastore_ok_response failed");
		}
		else if (request.operation == nxp_iot_DatastoreOperation_COMMIT)
		{
			agent_status = iot_agent_datastore_fs_commit(context);
			result = agent_status == IOT_AGENT_SUCCESS;
			RESULT_TRUE_OR_EXIT_MSG("iot_agent_datastore_fs_commit failed");

			result = iot_agent_datastore_encode_datastore_ok_response(ostream);
			RESULT_TRUE_OR_EXIT_MSG("iot_agent_datastore_encode_datastore_ok_response failed");
		}
	}

exit:
	return result;
}
#endif

