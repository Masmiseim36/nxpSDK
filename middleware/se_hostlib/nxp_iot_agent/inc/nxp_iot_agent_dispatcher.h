/*
 * Copyright 2018-2021,2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

/** @file */

#ifndef _NXP_IOT_AGENT_DISPATCHER_H_
#define _NXP_IOT_AGENT_DISPATCHER_H_

#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_utils_protobuf.h>
#include <nxp_iot_agent_context.h>
#include <nxp_iot_agent_endpoint.h>

#include "../protobuf/pb.h"
#include "../protobuf/pb_encode.h"
#include "../protobuf/pb_decode.h"

#include "../protobuf/Dispatcher.pb.h"
#include "../protobuf/Agent.pb.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @defgroup edgelock2go_agent_dispatcher The EdgeLock 2GO agent APDU dispatcher.
 * @ingroup edgelock2go_agent
 *
 * @brief The dispatcher handles and decodes requests of endpoints.
 */

/*!
 * @addtogroup edgelock2go_agent_dispatcher
 *
 * @{
 */

typedef bool(*get_endpoint_info_callback_t)(
	void* context, void* endpoint_information);

typedef bool(*request_handler_callback_t)(pb_istream_t *istream,
	pb_ostream_t *ostream, const pb_field_t* message_type, void *endpoint_context);

typedef struct iot_agent_endpoint_cache_table_entry_t {
	nxp_iot_EndpointType type;
	uint32_t id;
	iot_agent_endpoint_interface_t endpoint_interface;
	void* endpoint_context;
} iot_agent_endpoint_cache_table_entry_t;

typedef enum _iot_agent_stream_type_t {
	/** Network stream */
	STREAM_TYPE_NETWORK = 0U,
	/** Buffer stream in Requests format */
	STREAM_TYPE_BUFFER_REQUESTS = 1U,
} iot_agent_stream_type_t;

typedef struct iot_agent_dispatcher_context_t {
	iot_agent_context_t* agent_context;
	void* network_context;
	nxp_iot_EndpointRequest* current_request;
	
	// Keep a mapping table from id of endpoints to
	// the actual endpoint request handers and their contexts for
	// faster lookup for message distribution.
	iot_agent_endpoint_cache_table_entry_t endpoints[NXP_IOT_AGENT_MAX_NUM_ENDPOINTS];

	bool closed;
	bool successful_crl_verification_done;

	// The service descriptor that is used for the connection to 
	// the EdgeLock 2GO cloud service.
	nxp_iot_ServiceDescriptor* service_descriptor;

	// An overall status from the EdgeLock 2GO cloud service indicating if 
	// all operations were done successfully.
	bool update_status;

	// A status report field that is filled during the update.
	// Note, this is optional. If the pointer is set to NULL, no detailed status 
	// report is available after an update.
	nxp_iot_UpdateStatusReport* status_report;

    iot_agent_stream_type_t stream_type;
} iot_agent_dispatcher_context_t;

/** Context structure for passing dispatcher information to callbacks for 
 * message decoding. */
typedef struct handle_request_payload_args_t {
	iot_agent_dispatcher_context_t* dispatcher_context;
	iot_agent_response_buffer_t* response_buffer;
} handle_request_payload_args_t;


typedef struct iot_agent_context_t iot_agent_context_t;

bool handle_request_payload(pb_istream_t *stream, const pb_field_t *field, void **arg);
bool handle_requests(pb_istream_t *stream, const pb_field_t *field, void **arg);

bool encode_responses_from_payload(pb_ostream_t *ostream, nxp_iot_ResponsePayload* response);
bool encode_responses_callback(pb_ostream_t *ostream, const pb_field_t *field, void* const* arg);
/*! @brief Dispatcher
* @param[in] dispatcher_context Context for the dispatcher
* @param[in] input Input stream
* @param[in] out Output stream
*/
iot_agent_status_t iot_agent_dispatcher(
	iot_agent_dispatcher_context_t* dispatcher_context,
	pb_istream_t * input,
	pb_ostream_t * output);

/*!
*@}
*/ /* end of edgelock2go_agent_dispatcher */


#ifdef __cplusplus
} // extern "C"
#endif

#endif // _NXP_IOT_AGENT_DISPATCHER_H_
