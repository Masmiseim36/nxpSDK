/*
 * Copyright 2020, 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#ifndef _NXP_IOT_AGENT_ENDPOINT_H_
#define _NXP_IOT_AGENT_ENDPOINT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pb_istream_s pb_istream_t;
typedef struct pb_ostream_s pb_ostream_t;
typedef struct pb_field_s pb_field_t;

/** @brief Provide information about the endpoint to the EdgeLock 2GO cloud service. 
 */
typedef bool(*iot_agent_datastore_get_endpoint_info_t)( void* context, void* endpoint_information);

/** @brief Handle an endpoint request from EdgeLock 2GO cloud service. 
 */
typedef bool(*iot_agent_endpoint_request_handler_t)(pb_istream_t *istream, pb_ostream_t *ostream,
        const pb_field_t* message_type, void *context);

/** @brief The interface any endpoint needs to implement.
 *
 * This is the interface the EdgeLock 2GO cloud service is using to communicate to an endpoint.
 * After connecting to the EdgeLock 2GO cloud service it collects information about the available
 * endpoints. For this the get_endpoint_information is called.
 * 
 * For the application logic itself, EdgeLock 2GO cloud service is sending request to an endpoint
 * which need to be handled by handle_request. The request messages that are sent to the endpoint do
 * depend on the type of endpoint.
 */
typedef struct iot_agent_endpoint_interface_t { 
	iot_agent_datastore_get_endpoint_info_t get_endpoint_information; 
	iot_agent_endpoint_request_handler_t handle_request; 
} iot_agent_endpoint_interface_t;


#ifdef __cplusplus
}
#endif

#endif // _NXP_IOT_AGENT_ENDPOINT_H_
