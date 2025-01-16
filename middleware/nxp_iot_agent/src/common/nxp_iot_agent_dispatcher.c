/*
* Copyright 2018-2022,2024 NXP
*
* SPDX-License-Identifier: Apache-2.0
*
*/
#include <nxp_iot_agent_dispatcher.h>
#include <nxp_iot_agent_ver.h>
#include <nxp_iot_agent_utils_protobuf.h>
#include <nxp_iot_agent_macros.h>

#include <nxp_iot_agent.h>
#include <nxp_iot_agent_common.h>

#include "../protobuf/pb.h"
#include "../protobuf/pb_encode.h"
#include "../protobuf/pb_decode.h"

#include "../protobuf/Dispatcher.pb.h"
#include "../protobuf/Agent.pb.h"
#include "../protobuf/Apdu.pb.h"


// This is the overall package version that is part of the hello message from the client.
#define IOT_AGENT_DISPATCHER_VERSION_MAJOR (NXP_IOT_AGENT_VER_MAJOR)
#define IOT_AGENT_DISPATCHER_VERSION_MINOR (NXP_IOT_AGENT_VER_MINOR)
#define IOT_AGENT_DISPATCHER_VERSION_PATCH (NXP_IOT_AGENT_VER_DEV)
#define IOT_AGENT_DISPATCHER_VERSION ((uint32_t)(((IOT_AGENT_DISPATCHER_VERSION_MAJOR * 256U) \
		+ IOT_AGENT_DISPATCHER_VERSION_MINOR) * 256U + IOT_AGENT_DISPATCHER_VERSION_PATCH))


static iot_agent_endpoint_cache_table_entry_t* find_endpoint_in_cache(
	iot_agent_endpoint_cache_table_entry_t* cache, nxp_iot_EndpointType type, uint32_t id)
{
	for (size_t i = 0U; i < NXP_IOT_AGENT_MAX_NUM_ENDPOINTS; i++)
	{
		if (cache[i].type == type && cache[i].id == id)
			return &cache[i];
	}
	return NULL;
}


bool encode_responses_callback(pb_ostream_t *ostream, const pb_field_t *field, void* const* arg)
{
	AX_UNUSED_ARG(field);
	nxp_iot_ResponsePayload* response = (*arg);
	bool result = false;

	// The oneof selection for the datastore message:
	result = pb_encode_tag(ostream, PB_WT_STRING, nxp_iot_Responses_responses_tag);
	RESULT_TRUE_OR_EXIT_MSG("encoding of tag failed: %s\n", PB_GET_ERROR(ostream));

	// And encode the actual payload.
	result = pb_encode_delimited(ostream, nxp_iot_ResponsePayload_fields, response);
	RESULT_TRUE_OR_EXIT_MSG("pb_encode_delimited failed for nxp_iot_ResponsePayload: %s", PB_GET_ERROR(ostream));

exit:
	return result;
}


bool encode_responses_from_payload(pb_ostream_t *ostream, nxp_iot_ResponsePayload* response)
{
	nxp_iot_Responses responses = nxp_iot_Responses_init_default;
	responses.responses.arg = response;
	responses.responses.funcs.encode = &encode_responses_callback;

	if (!pb_encode(ostream, nxp_iot_Responses_fields, &responses))
	{
		IOT_AGENT_ERROR("pb_encode failed for nxp_iot_Responses");
		return false;
	}
	return true;
}


bool handle_request_payload(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	AX_UNUSED_ARG(field);
	handle_request_payload_args_t* handle_args = (handle_request_payload_args_t*)(*arg);
	iot_agent_dispatcher_context_t* dispatcher_context = handle_args->dispatcher_context;
	iot_agent_response_buffer_t* response_buffer = handle_args->response_buffer;

	nxp_iot_EndpointRequest* request = dispatcher_context->current_request;

	pb_ostream_t ostream = pb_ostream_from_response_buffer(response_buffer);

	const pb_field_t* message_type = decode_unionmessage_type(stream, nxp_iot_RequestPayload_fields);

#if NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO
	// SEMS Lite will not do CRL verification
	if (dispatcher_context->stream_type != STREAM_TYPE_BUFFER_REQUESTS) {
		if (!dispatcher_context->successful_crl_verification_done) {
			// Only a selected subset of messages are allowed before a CRL verification has beed done.
			if (!((message_type == nxp_iot_AgentHelloRequest_fields) ||
				(message_type == nxp_iot_AgentGoodbyeRequest_fields) ||
				(message_type == nxp_iot_AgentCrlRequest_fields))) {
				IOT_AGENT_ERROR("Unexpected message received (no CRL check done yet), stopping processing.");
				return false;
			}
		}
	}
#endif

	// Intercept the messages that can and should be handled in the dispatcher directly.
	if (message_type == nxp_iot_AgentHelloRequest_fields)
	{
		nxp_iot_AgentHelloRequest request2 = nxp_iot_AgentHelloRequest_init_default;
		if (!pb_decode_delimited(stream, nxp_iot_AgentHelloRequest_fields, &request2)) {
			IOT_AGENT_ERROR("Decode Server Message failed: %s\n", PB_GET_ERROR(stream));
			return false;
		}

		nxp_iot_ResponsePayload response = nxp_iot_ResponsePayload_init_default;
		nxp_iot_AgentHelloResponse hello = nxp_iot_AgentHelloResponse_init_default;
		response.which_message = nxp_iot_ResponsePayload_hello_tag;
		response.message.hello = hello;
		response.message.hello.has_version = true;
		response.message.hello.version = IOT_AGENT_DISPATCHER_VERSION;

		response.message.hello.has_communicationBufferSizeRequests = true;
		response.message.hello.communicationBufferSizeRequests = NXP_IOT_AGENT_REQUEST_BUFFER_SIZE;

		response.message.hello.has_communicationBufferSizeResponses = true;
		response.message.hello.communicationBufferSizeResponses = NXP_IOT_AGENT_RESPONSE_BUFFER_SIZE;

		response.message.hello.has_useMultipleRequestsPerMessage = true;
		response.message.hello.useMultipleRequestsPerMessage = true;

		for (size_t i = 0U; i < NXP_IOT_AGENT_MAX_NUM_ENDPOINTS; i++)
		{
			if (dispatcher_context->endpoints[i].type == nxp_iot_EndpointType_INVALID)
				break;

			iot_agent_endpoint_cache_table_entry_t* endpoint = &dispatcher_context->endpoints[i];
			nxp_iot_EndpointInformation* info = &response.message.hello.endpoints[response.message.hello.endpoints_count];
			info->has_type = true;
			info->type = endpoint->type;
			info->has_endpointId = true;
			info->endpointId = endpoint->id;

			if (!endpoint->endpoint_interface.get_endpoint_information(endpoint->endpoint_context, info))
			{
				IOT_AGENT_ERROR("get_endpoint_info failed");
				return false;
			}
			response.message.hello.endpoints_count++;
		}

		// And encode the actual paylod including a responses tag and a length in front.
		if (!encode_responses_from_payload(&ostream, &response))
		{
			IOT_AGENT_ERROR("pb_encode_delimited failed for nxp_iot_ResponsePayload");
			return false;
		}
	}
	else
	{
        nxp_iot_EndpointType type = nxp_iot_EndpointType_INVALID;
        uint32_t endpointId = 0U;

		// TODO: Maybe for SEMS Lite it it possible to add the id and type to the package? It
		// is just 4 bytes...

        // All other messages can not be handled here and are delegated to an endpoint!
        if (dispatcher_context->stream_type == STREAM_TYPE_NETWORK) {
    		if (!request->has_type)
    		{
    			IOT_AGENT_ERROR("Endpoint message is lacking an endpoint type.");
    			return false;
    		}

    		if (!request->has_endpointId)
    		{
    			IOT_AGENT_ERROR("Endpoint message is lacking an endpoint id.");
    			return false;
    		}
            type = request->type;
            endpointId = request->endpointId;
        }
        else {
            // SEMS Lite agent use fixed type and endpoint id.
            type = IOT_AGENT_KS_SSS_SE05X;
            endpointId = IOT_AGENT_KEYSTORE_ID_SEMSLITE;
        }

		iot_agent_endpoint_cache_table_entry_t* endpoint = find_endpoint_in_cache(
			dispatcher_context->endpoints, type, endpointId);
		if (endpoint == NULL)
		{
			IOT_AGENT_ERROR("The requested endpoint was not found.");
			return false;
		}

		// The response handler is supposed to encode a nxp_iot_ResponsePayload
		// response onto the ostream! It can be empty, but at least the length
		// field must be present!
		if (!endpoint->endpoint_interface.handle_request(stream, &ostream, message_type, endpoint->endpoint_context))
		{
			IOT_AGENT_ERROR("handling request by endpoint failed");
			return false;
		}
	}
	return true;
}

bool handle_requests(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	AX_UNUSED_ARG(field);
	handle_request_payload_args_t* handle_args = (handle_request_payload_args_t*)(*arg);
	iot_agent_dispatcher_context_t* dispatcher_context = handle_args->dispatcher_context;

	nxp_iot_EndpointRequest request;
	request.payload.funcs.decode = &handle_request_payload;
	request.payload.arg = handle_args;

	dispatcher_context->current_request = &request;
	if (!pb_decode(stream, nxp_iot_EndpointRequest_fields, &request)) {
		IOT_AGENT_ERROR("Decode Server Message failed: %s", PB_GET_ERROR(stream));
		dispatcher_context->current_request = NULL;
		return false;
	}
	dispatcher_context->current_request = NULL;
	return true;
}

iot_agent_status_t iot_agent_dispatcher(
	iot_agent_dispatcher_context_t* dispatcher_context,
	pb_istream_t * input,
	pb_ostream_t * output)
{
	uint8_t response_buffer_memory[NXP_IOT_AGENT_RESPONSE_BUFFER_SIZE];
	iot_agent_response_buffer_t response_buffer;
	handle_request_payload_args_t handle_request_args = { 0 };
	handle_request_args.dispatcher_context = dispatcher_context;
	handle_request_args.response_buffer = &response_buffer;

	while (! dispatcher_context->closed)
	{
		// We want to do inplace encoding of responses. At the end the full response is
		// encapsulated in a Responses message and prepended by a length and a (for framing).
		// This is done on the fly during streaming.

		response_buffer.start = &response_buffer_memory[0];
		response_buffer.pos = response_buffer.start;
		response_buffer.remaining = sizeof(response_buffer_memory);

        if (dispatcher_context->stream_type == STREAM_TYPE_NETWORK) {
    		nxp_iot_EndpointRequests request = nxp_iot_EndpointRequests_init_default;
    		request.requests.funcs.decode = &handle_requests;
    		request.requests.arg = &handle_request_args;

    		if (!pb_decode_delimited(input, nxp_iot_EndpointRequests_fields, &request)) {
    			IOT_AGENT_ERROR("Decode Server Message failed: %s", PB_GET_ERROR(input));
    			return IOT_AGENT_FAILURE;
    		}
        }
        else if (dispatcher_context->stream_type == STREAM_TYPE_BUFFER_REQUESTS) {
            nxp_iot_Requests requests = nxp_iot_Requests_init_default;
		    requests.payload.funcs.decode = &handle_request_payload;
		    requests.payload.arg = &handle_request_args;

    		if (!pb_decode_delimited(input, nxp_iot_Requests_fields, &requests)) {
    			IOT_AGENT_ERROR("Decode Server Requests Message failed: %s", PB_GET_ERROR(input));
    			return IOT_AGENT_FAILURE;
    		}
        }
        else {
            IOT_AGENT_ERROR("Unknown stream type: %d", dispatcher_context->stream_type);
                return IOT_AGENT_FAILURE;
        }

		// Assemble the response on toplevel...
		int sub = response_buffer.pos - response_buffer.start;
		if (sub < 0)
		{
			IOT_AGENT_ERROR("Error in length calculation.");
			return IOT_AGENT_FAILURE;
		}
		size_t total_length = (size_t)sub;
		if (total_length > 0U)
		{
			// An then the buffer contents
			if (!pb_encode_string(output, response_buffer.start, total_length))
			{
				IOT_AGENT_ERROR("pb_encode_string failed top level message");
				return IOT_AGENT_FAILURE;
			}
		}

        if (dispatcher_context->stream_type == STREAM_TYPE_BUFFER_REQUESTS)
            break;
	}

// We want to be sure that the endpoint types and service types defined on the C interface level correspnd
// to the ones that are defined on the protocol layer.

COMPILE_TIME_ASSERT(IOT_AGENT_KS_SSS_A71CH_NOT_SUPPORTED == nxp_iot_EndpointType_KS_SSS_A71CH);
COMPILE_TIME_ASSERT(IOT_AGENT_KS_SSS_SE05X == nxp_iot_EndpointType_KS_SSS_SE05X);
COMPILE_TIME_ASSERT(IOT_AGENT_KS_SSS_RPC_NOT_SUPPORTED == nxp_iot_EndpointType_KS_SSS_RPC);
COMPILE_TIME_ASSERT(IOT_AGENT_KS_PSA == nxp_iot_EndpointType_KS_PSA);
COMPILE_TIME_ASSERT(IOT_AGENT_DS_FS == nxp_iot_EndpointType_DS_FS);
COMPILE_TIME_ASSERT(IOT_AGENT_DS_PLAIN == nxp_iot_EndpointType_DS_PLAIN);

COMPILE_TIME_ASSERT(NXP_IOT_AGENT_MAX_NUM_ENDPOINTS <=
		(sizeof(((nxp_iot_AgentHelloResponse*)0)->endpoints) / sizeof(((nxp_iot_AgentHelloResponse*)0)->endpoints[0])));

COMPILE_TIME_ASSERT(IOT_AGENT_AWSSERVICE == nxp_iot_ServiceType_AWSSERVICE);
COMPILE_TIME_ASSERT(IOT_AGENT_GOOGLESERVICE == nxp_iot_ServiceType_GOOGLESERVICE);
COMPILE_TIME_ASSERT(IOT_AGENT_AZURESERVICE == nxp_iot_ServiceType_AZURESERVICE);
COMPILE_TIME_ASSERT(IOT_AGENT_IBMSERVICE == nxp_iot_ServiceType_IBMSERVICE);
COMPILE_TIME_ASSERT(IOT_AGENT_CUSTOMSERVICE == nxp_iot_ServiceType_CUSTOMSERVICE);

COMPILE_TIME_ASSERT(IOT_AGENT_PROTOCOL_HTTPS == nxp_iot_ServiceProtocolType_HTTPS);
COMPILE_TIME_ASSERT(IOT_AGENT_PROTOCOL_MQTTS == nxp_iot_ServiceProtocolType_MQTTS);
COMPILE_TIME_ASSERT(IOT_AGENT_PROTOCOL_AMQPS == nxp_iot_ServiceProtocolType_AMQPS);
COMPILE_TIME_ASSERT(IOT_AGENT_PROTOCOL_XMPP	 == nxp_iot_ServiceProtocolType_XMPP);
COMPILE_TIME_ASSERT(IOT_AGENT_PROTOCOL_DDS   == nxp_iot_ServiceProtocolType_DDS);
COMPILE_TIME_ASSERT(IOT_AGENT_PROTOCOL_COAP  == nxp_iot_ServiceProtocolType_COAP);

COMPILE_TIME_ASSERT(NXP_IOT_AGENT_CORRELATION_ID_LENGTH == sizeof(((nxp_iot_UpdateStatusReport*)0)->correlationId));

return IOT_AGENT_SUCCESS;
}
