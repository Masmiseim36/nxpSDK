/*
 * Copyright 2018-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <nxp_iot_agent_keystore_sss_se05x.h>
#include <nxp_iot_agent_session.h>
#include <nxp_iot_agent_macros.h>
#include <nxp_iot_agent_dispatcher.h>
#include <nxp_iot_agent_time.h>

#if NXP_IOT_AGENT_HAVE_SSS

#include <nxp_iot_agent_utils_protobuf.h>

#include "../protobuf/Dispatcher.pb.h"
#include "../protobuf/Agent.pb.h"
#include "../protobuf/Datastore.pb.h"
#include "../protobuf/Apdu.pb.h"

#include <smCom.h>
#include "se05x_tlv.h"
#include "sm_apdu.h"

#define IOT_AGENT_KEYSTORE_SSS_SE05x_VERSION_MAJOR (1U)
#define IOT_AGENT_KEYSTORE_SSS_SE05x_VERSION_MINOR (0U)
#define IOT_AGENT_KEYSTORE_SSS_SE05x_VERSION_PATCH (0U)
#define IOT_AGENT_KEYSTORE_SSS_SE05x_VERSION (((IOT_AGENT_KEYSTORE_SSS_SE05x_VERSION_MAJOR * 256U) \
		+ IOT_AGENT_KEYSTORE_SSS_SE05x_VERSION_MINOR) * 256U + IOT_AGENT_KEYSTORE_SSS_SE05x_VERSION_PATCH)


const iot_agent_keystore_interface_t iot_agent_keystore_sss_se05x_interface =
{
	&iot_agent_keystore_sss_se05x_destroy,
	&iot_agent_keystore_sss_se05x_open_session,
	&iot_agent_keystore_sss_se05x_close_session,
	{
		&iot_agent_keystore_sss_se05x_get_endpoint_info,
		&iot_agent_keystore_sss_se05x_handle_request,
	}
};


iot_agent_status_t iot_agent_keystore_sss_se05x_init(iot_agent_keystore_t* keystore,
	uint32_t identifier, ex_sss_boot_ctx_t* boot_context, bool is_session_open)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	keystore->iface = iot_agent_keystore_sss_se05x_interface;
	keystore->type = IOT_AGENT_KS_SSS_SE05X;
	keystore->identifier = identifier;

	iot_agent_keystore_sss_se05x_context_t* keystore_context =
		malloc(sizeof(iot_agent_keystore_sss_se05x_context_t));
	ASSERT_OR_EXIT_STATUS(keystore_context != NULL, IOT_AGENT_ERROR_MEMORY);
	memset(keystore_context, 0, sizeof(iot_agent_keystore_sss_se05x_context_t));
	keystore->context = keystore_context;

	keystore_context->boot_context = boot_context;
	keystore_context->sss_context = &boot_context->ks;
	keystore_context->session_open = is_session_open;

exit:
	return agent_status;
}


iot_agent_status_t iot_agent_keystore_sss_se05x_destroy(void *context)
{
	AX_UNUSED_ARG(context);
	return IOT_AGENT_SUCCESS;
}


iot_agent_status_t iot_agent_keystore_sss_se05x_open_session(void *context)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	iot_agent_keystore_sss_se05x_context_t* keystore_context = (iot_agent_keystore_sss_se05x_context_t*)context;
	ASSERT_OR_EXIT_MSG(context != NULL, "context is NULL");
	if (!keystore_context->session_open) {
		agent_status = iot_agent_session_connect(keystore_context->boot_context);
		AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_session_connect failed with 0x%08x", agent_status);
		keystore_context->session_open = true;
	}
exit:
	return agent_status;
}


void iot_agent_keystore_sss_se05x_close_session(void *context)
{
	iot_agent_keystore_sss_se05x_context_t* keystore_context = (iot_agent_keystore_sss_se05x_context_t*)context;
	if (context != NULL && keystore_context->session_open) {
		iot_agent_session_disconnect(keystore_context->boot_context);
		keystore_context->session_open = false;
	}
}


bool iot_agent_keystore_sss_se05x_get_endpoint_info(
	void *context, void* endpoint_information)
{
	AX_UNUSED_ARG(context);
	nxp_iot_EndpointInformation* info = (nxp_iot_EndpointInformation*)endpoint_information;
	info->has_version = true;
	info->version = IOT_AGENT_KEYSTORE_SSS_SE05x_VERSION;
	return true;
}


bool iot_agent_keystore_sss_se05x_handle_request(pb_istream_t *istream,
	pb_ostream_t *ostream, const pb_field_t* message_type, void *context)
{
    if (message_type == nxp_iot_ApduRequest_fields)
	{
		nxp_iot_ApduRequest request = nxp_iot_ApduRequest_init_default;

		uint8_t apdu_memory[SE05X_MAX_BUF_SIZE_CMD] = {0U};
		uint8_t response_memory[SE05X_MAX_BUF_SIZE_RSP] = {0U};
        uint8_t expectation_memory[512] = {0U};

		buffer_t read_buffer = {0};
		read_buffer.buf = &apdu_memory[0];
		read_buffer.len = sizeof(apdu_memory);

		buffer_t write_buffer = {0};
		write_buffer.buf = &response_memory[0];
		write_buffer.len = sizeof(response_memory);

        expectation_t expectation = {0};
		expectation.buf = &expectation_memory[0];
        expectation.len = sizeof(expectation_memory);
		expectation.offset = 0U;

		request.message.funcs.decode = &decode_byte_field;
		request.message.arg = &read_buffer;

		request.expectation.funcs.decode = &decode_expect_field;
		request.expectation.arg = &expectation;

		if (!pb_decode_delimited(istream, nxp_iot_ApduRequest_fields, &request)) {
			IOT_AGENT_ERROR("Decode Server Message failed: %s\n", PB_GET_ERROR(istream));
			return false;
		}

		// Attention, DoAPDUTxRx does write to the input buffer in case it is encrypting/macing
		// the message (for platform SCP). Also, there seems to be no way to specify an input buffer
		// size. This makes this API a bit dangerous. We need to make sure that our buffers hold
		// enough space to be able to accommodate for the additional data required for SCP. This
		// is given because the buffer is bigger than the biggest APDU that SE050 can receive.
		// The EdgeLock 2GO cloud service will never send plain APDUs that would not fit into
		// the SE050's APDU buffer after being wrapped for platform SCP.
        size_t len = write_buffer.len;
		iot_agent_keystore_sss_se05x_context_t *keystore_context = (iot_agent_keystore_sss_se05x_context_t *) context;
		sss_se05x_session_t *pSession = (sss_se05x_session_t *)&(keystore_context->boot_context->session);
        smStatus_t sm_comm_status;

#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
        axTimeMeasurement_t apdu_time = { 0 };
        initMeasurement(&apdu_time);
#endif
		sm_comm_status = DoAPDUTxRx(&(pSession->s_ctx), read_buffer.buf, read_buffer.len, write_buffer.buf, &len);
#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
        concludeMeasurement(&apdu_time);
		long measured_time = getMeasurement(&apdu_time);
		if (iot_agent_time.apdu_time > (LONG_MAX - measured_time))
		{
			IOT_AGENT_ERROR("Error in the time measurement");
			return false;
		}
        iot_agent_time.apdu_time += measured_time;
#endif

		if (sm_comm_status == SM_OK)
		{
			// If the APDU exchange was successful, transmit back the response. The contents are already in
			// the write_buffer, the length needs to be adjusted from the length returned from the APDU exchange.
			write_buffer.len = len;
		}
		else {
			// If the APDU exchange was not successful, one of the reasons is that the SW is not 9000. We do, however,
			// want to have the SW on the server side. The SW is returned by the APDU exchange function (it can also
			// be SM_NOT_OK, but thats also valuable information), so in this case we send the return value of the
			// function back to the server.
			write_buffer.buf[0] = (uint8_t)((sm_comm_status >> 8) & 0xFFU);
			write_buffer.buf[1] = (uint8_t)(sm_comm_status & 0xFFU);
			write_buffer.len = 2U;
		}

		nxp_iot_ResponsePayload response = nxp_iot_ResponsePayload_init_default;
		nxp_iot_ApduResponse apdu = nxp_iot_ApduResponse_init_default;
		response.which_message = nxp_iot_ResponsePayload_apdu_tag;
		response.message.apdu = apdu;

		response.message.apdu.message.funcs.encode = &encode_byte_field;
		response.message.apdu.message.arg = &write_buffer;

		// And encode the actual paylod including a responses tag and a length in front.
		if (!encode_responses_from_payload(ostream, &response))
		{
			IOT_AGENT_ERROR("encode_responses_from_payload failed");
			return false;
		}
	}
	else
	{
		IOT_AGENT_ERROR("unsupported message type");
		return false;
	}
	return true;
}

iot_agent_status_t iot_agent_keystore_sss_se05x_get_sss_key_store(iot_agent_keystore_sss_se05x_context_t* context,
	sss_key_store_t** sss_key_store)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	ASSERT_OR_EXIT_MSG(context != NULL, "context is NULL.");
	*sss_key_store = context->sss_context;
exit:
	return agent_status;
}


#endif // NXP_IOT_AGENT_HAVE_SSS
