/* 
 * Copyright 2018-2021,2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#ifndef _NXP_IOT_AGENT_DATASTORE_PLAIN_H_
#define _NXP_IOT_AGENT_DATASTORE_PLAIN_H_

#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_datastore.h>


/** @defgroup edgelock2go_agent_datastore_plain EdgeLock 2GO agent in-memory datastore related functionality.
 * @ingroup edgelock2go_agent_datastore
 *
 * @brief Functions that define how to interact with a in-memory based datastore.
 */

/*!
 * @addtogroup edgelock2go_agent_datastore_plain
 * @{
 */

#define BUFFER_NUMBER 2U

typedef struct iot_agent_datastore_plain_context_t {
	// In order to demonstrate atomic updates with transactions, this datastore 
	// implementation contains two buffers. One is holding the current data. 
	// On allocate, the other one is allocated. The subsequent writes will update 
	// the freshly allocated one, keeping the original data in place. Reads will 
	// still come from the original buffer.
	// When committing, the original buffer is deleted and all accesses are done 
	// on the new buffer from that point onwards.
	uint8_t* buffers[BUFFER_NUMBER];
	size_t size[BUFFER_NUMBER];

	/** The index of the buffers from which reads will take place. */
	size_t idx_read;

	/** The index of the buffers to which writes will be done. */
	size_t idx_write;

} iot_agent_datastore_plain_context_t;


/** @brief Initialize a plain datastore and allocate its context.
 *
 * The init function does allocate memory for the context as well as for the 
 * contents of the datastore. To free allocated memory, use
 * iot_agent_datastore_free.
 *
 * If the function does not return IOT_AGENT_SUCCESS, it can be assumed that no 
 * memory was allocated. The datastore is unusable.
 */
iot_agent_status_t iot_agent_datastore_plain_init(iot_agent_datastore_t* datastore,
	uint32_t identifier);

iot_agent_status_t iot_agent_datastore_plain_destroy(
	void* context);

iot_agent_status_t iot_agent_datastore_plain_allocate(
	void* context, size_t len);

iot_agent_status_t iot_agent_datastore_plain_write(
	void* context, size_t offset, const void* src, size_t len);

iot_agent_status_t iot_agent_datastore_plain_read(
	void* context, void* dst, size_t offset, size_t* len);

iot_agent_status_t iot_agent_datastore_plain_commit(
	void* context);

bool iot_agent_datastore_plain_get_endpoint_info(
	void *context, void* endpoint_information);

bool iot_agent_datastore_plain_handle_request(pb_istream_t *istream,
	pb_ostream_t *ostream, const pb_field_t* message_type, void *context);

extern const iot_agent_datastore_interface_t iot_agent_datastore_plain_interface;

bool iot_agent_datastore_plain_handle_write_data(pb_istream_t *stream, const pb_field_t *field, void **arg);

/*!
 *@}
 */ /* end of edgelock2go_agent_datastore_plain */

#endif // _NXP_IOT_AGENT_DATASTORE_PLAIN_H_
