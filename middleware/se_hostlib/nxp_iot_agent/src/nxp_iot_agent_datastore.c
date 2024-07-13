/* 
 * Copyright 2018-2021,2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#include <nxp_iot_agent_datastore.h>

#include <nxp_iot_agent_utils.h>
#include <nxp_iot_agent_macros.h>
#include <nxp_iot_agent_dispatcher.h>
#include <pb_decode.h>
#include <pb_encode.h>

#include "./protobuf/Dispatcher.pb.h"
#include "./protobuf/Datastore.pb.h"


iot_agent_status_t iot_agent_datastore_free(iot_agent_datastore_t* datastore)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	if (datastore != NULL)
	{
        if (datastore->iface.destroy != NULL)
        {
            agent_status = datastore->iface.destroy(datastore->context);
            if (agent_status != IOT_AGENT_SUCCESS) {
#if AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1 && !defined(__ZEPHYR__)
            	IOT_AGENT_ERROR("Failed to destroy datastore [type: %lu, id: 0x%08lx]: 0x%08x",
                    datastore->type, datastore->identifier, agent_status);
#else
    			IOT_AGENT_ERROR("Failed to destroy datastore [type: %d, id: 0x%08x]: 0x%08x",
                    datastore->type, datastore->identifier, agent_status);
#endif
            }
        }
        free(datastore->context);
		datastore->context = NULL;
    }
	return agent_status;
}


iot_agent_status_t iot_agent_datastore_allocate(iot_agent_datastore_t* datastore, size_t len)
{
	return datastore->iface.allocate(datastore->context, len);
}


iot_agent_status_t iot_agent_datastore_read(iot_agent_datastore_t* datastore,
	void* dst, size_t offset, size_t* len)
{
	return datastore->iface.read(datastore->context, dst, offset, len);
}


iot_agent_status_t iot_agent_datastore_write(iot_agent_datastore_t* datastore, size_t offset,
	const void* src, size_t len)
{
	return datastore->iface.write(datastore->context, offset, src, len);
}


iot_agent_status_t iot_agent_datastore_commit(iot_agent_datastore_t* datastore)
{
	return datastore->iface.commit(datastore->context);
}


bool datastore_read_callback(pb_istream_t *stream, uint8_t *buf, size_t count)
{
	datastore_stream_context_t* datastore_stream_context = (datastore_stream_context_t*)stream->state;
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	size_t read = count;
	agent_status = datastore_stream_context->datastore->iface.read(datastore_stream_context->datastore->context,
		buf, datastore_stream_context->offset, &read);
	ASSERT_OR_EXIT_MSG(datastore_stream_context->offset <= (SIZE_MAX - read), "Wraparound in addition of offset and read");
	datastore_stream_context->offset += read;

	AGENT_SUCCESS_OR_EXIT_MSG("Read from datastore failed with 0x%04x", agent_status);

	ASSERT_OR_EXIT_MSG(read == count, "Needed to read %u bytes, but only %u bytes could be read.",
		count, read);

exit:
	return (agent_status == IOT_AGENT_SUCCESS);
}


bool iot_agent_datastore_encode_datastore_ok_response(pb_ostream_t *ostream)
{
	nxp_iot_ResponsePayload response = nxp_iot_ResponsePayload_init_default;
	
	nxp_iot_DatastoreResponse datastore = nxp_iot_DatastoreResponse_init_default;
	response.which_message = (pb_size_t)nxp_iot_ResponsePayload_datastore_tag;
	response.message.datastore = datastore;

	response.message.datastore.has_status = true;
	response.message.datastore.status = nxp_iot_DatastoreStatus_OK;

	if (!encode_responses_from_payload(ostream, &response))
	{
		IOT_AGENT_ERROR("encode_responses_from_payload failed");
		return false;
	}
	return true;
}

