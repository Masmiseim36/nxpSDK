/*
 * Copyright 2018, 2019, 2020, 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

/** @file */

#ifndef _NXP_IOT_AGENT_DATASTORE_H_
#define _NXP_IOT_AGENT_DATASTORE_H_

#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_endpoint.h>

/** @defgroup edgelock2go_agent_datastore EdgeLock 2GO agent datastore related functionality.
 * @ingroup edgelock2go_agent
 *
 * @brief Functions that define how to interact with a datastore. There exist multiple concrete 
 * implementations for different microcontrollers.
 */

 /*!
 * @addtogroup edgelock2go_agent_datastore
 * @{
 */

typedef iot_agent_status_t(*iot_agent_datastore_destroyer_t)(
	void* context);

typedef iot_agent_status_t (*iot_agent_datastore_allocator_t)(
	void* context, size_t len);

typedef iot_agent_status_t (*iot_agent_datastore_writer_t)(
	void* context, size_t offset, const void* src, size_t len);

typedef iot_agent_status_t (*iot_agent_datastore_reader_t)(
	void* context, void* dst, size_t offset, size_t* len);

typedef iot_agent_status_t(*iot_agent_datastore_committer_t)(
	void* context);

/**
 * @brief The interface any datastore needs to implement.
 */
typedef struct iot_agent_datastore_interface_t
{
	iot_agent_datastore_destroyer_t         destroy;
	iot_agent_datastore_allocator_t         allocate;
    iot_agent_datastore_writer_t            write;
    iot_agent_datastore_reader_t            read;
	iot_agent_datastore_committer_t         commit;
    iot_agent_endpoint_interface_t          endpoint_interface;
} iot_agent_datastore_interface_t;


/**
 * @brief A context holding the state of a datastore; this is passed to datastore interface functions.
 */
typedef struct iot_agent_datastore_t
{
    iot_agent_datastore_interface_t iface;
	uint32_t type;
	uint32_t identifier;
    void* context;
} iot_agent_datastore_t;


/**
 * @brief A stream for reading contents from within a datastore, starting at an offset.
 */
typedef struct datastore_stream_context_t 
{
	const iot_agent_datastore_t* datastore;
	size_t offset;
} datastore_stream_context_t;


typedef struct _nxp_iot_ResponsePayload nxp_iot_ResponsePayload;


/** @brief Destroy the datastore.
 *
 * Depending on the type of the datastore this is triggering actions on the datastore's context
 * itself by calling the _destroy() function of the datastore interface.
 *
 * This does not free the context of the datastore. 
 *
 * The \p datastore is not usable after a call to iot_agent_datastore_free.
 */
iot_agent_status_t iot_agent_datastore_free(iot_agent_datastore_t* datastore);


/** @brief Allocate memory in a datastore.
 *
 * Reserve memory in a datastore that can be written to by subsequent writes.
 *
 * If the memory allocation fails, this function does not return IOT_AGENT_SUCCESS.
 */
iot_agent_status_t iot_agent_datastore_allocate(iot_agent_datastore_t* datastore, size_t len);


/** @brief Read from a datastore to a buffer in memory. 
 *
 * Starting at position \p offset, \p len bytes are read from the datastore and copied to the memory
 * pointed to by \p dst.
 *
 * If the datastore does not hold \p len bytes or the read would be out of bounds, only as many
 * bytes as available are read.
 *
 * \p len is updated to hold the number of bytes that actually were read from the datastore.
 *
 * No length checks on \p dst are performed, the caller is responsible that the memory location is
 * writeable and can hold \p len bytes.
 */
iot_agent_status_t iot_agent_datastore_read(iot_agent_datastore_t* datastore, 
        void* dst, size_t offset, size_t* len);


/** @brief Write from a buffer in memory to a datastore.
 *
 * From \p src, \p len bytes are copied to the datastore. The first byte is written to the position
 * \p offset in the datastore.
 *
 * Length check on the destaination - the datastore - is performed, memory in the datastore needs to
 * be pre-allocated. If the data does not fit, this function does not return IOT_AGENT_SUCCESS.
 */
iot_agent_status_t iot_agent_datastore_write(iot_agent_datastore_t* datastore, size_t offset, 
        const void* src, size_t len);


/** @brief Commit a transaction to a datastore.
 *
 * A call to this function indicates that all necessary writes of a transaction are done and the 
 * datastore contens can be considered valid.
 * 
 * This is the indication to the datastore to invalidate old contents and from point in time 
 * onwards use the data that was updated with the transaction tht is committed.
 */
iot_agent_status_t iot_agent_datastore_commit(iot_agent_datastore_t* datastore);


bool datastore_read_callback(pb_istream_t *stream, uint8_t *buf, size_t count);


bool iot_agent_datastore_encode_datastore_ok_response(pb_ostream_t *ostream);


/*!
*@}
*/ /* end of edgelock2go_agent_datastore */


#endif // _NXP_IOT_AGENT_DATASTORE_H_
