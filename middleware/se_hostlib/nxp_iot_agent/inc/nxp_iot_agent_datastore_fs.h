/*
 * Copyright 2018-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#ifndef _NXP_IOT_AGENT_DATASTORE_FS_H_
#define _NXP_IOT_AGENT_DATASTORE_FS_H_

#if !(defined(__ICCARM__) || defined(__CC_ARM) || defined(__ARMCC_VERSION))
#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_datastore.h>

#if !(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)

/*! @defgroup edgelock2go_agent_datastore_fs EdgeLock 2GO agent FS datastore related functionality.
 * @ingroup edgelock2go_agent_datastore
 *
 * @brief Functions that define how to interact with a file-system based datastore. 
 */

/*!
 * @addtogroup edgelock2go_agent_datastore_fs
 * @{
 */

#define FILE_POINTER_NUMBER 2U

typedef struct iot_agent_datastore_fs_context_t {

	/** @brief For atomic updates, the datastore holds two file pointers, one that is currently 
	 * read from and a second one that is written to (as part of a transaction). */
	FILE* fp[FILE_POINTER_NUMBER];

	/** @brief The filename based on which the files for the datastore are determined. */
	char* basename;

	/** @brief The index of the fp from which reads will take place. */
	size_t idx_read;

	/** @brief The index of the fp to which writes will be done. */
	size_t idx_write;
} iot_agent_datastore_fs_context_t;


typedef bool(*iot_agent_datastore_validator_t)(const iot_agent_datastore_t* context);


/** @brief Initialize a fs datastore.
 *
 * The init function does allocate memory for the context and opens the file. 
 * The file must be closed and the context freed. This can be achieved by calling 
 * iot_agent_datastore_free.
 * 
 * The backing storage of the datastore consists of two files (for transaction support). The 
 * filenames of those files are based on the filename given here as argument \p basename. 
 * To \p basename, for one file, ".0" is appended, ".1" for the other.
 *
 * Both files are opened when initializing the datastore.
 * In case the file is existing, it is read in mode "r+b" (read/update), if it is not existing, a
 * new file is created with mode "w+b" (write/update).
 *
 * Validity of the contents of the file are checked using the 
 * provided \p validator, the contents of the first (with the lower number) 
 * file that is considered to be valid is taken as datastore contents, the other file is 
 * truncated.
 *
 * The datastore creates an internal copy of basename, the memory for \p basename can be
 * freed by the caller after initialization of the datastore.
 *
 * If the function does not return IOT_AGENT_SUCCESS, it can be assumed that neither memory remained
 * allocated nor any file remained open. The datastore is unusable.
 */
iot_agent_status_t iot_agent_datastore_fs_init(iot_agent_datastore_t* datastore, 
	uint32_t identifier, const char* basename, iot_agent_datastore_validator_t validator);

iot_agent_status_t iot_agent_datastore_fs_destroy(
	void* context);

iot_agent_status_t iot_agent_datastore_fs_allocate(
	void* context, size_t len);

iot_agent_status_t iot_agent_datastore_fs_write(
	void* context, size_t offset, const void* src, size_t len);

iot_agent_status_t iot_agent_datastore_fs_read(
	void* context, void* dst, size_t offset, size_t* len);

iot_agent_status_t iot_agent_datastore_fs_commit(
	void* context);

iot_agent_status_t iot_agent_datastore_fs_open_file(FILE** fp,
    const char* basename, size_t index, const char* mode);

bool iot_agent_datastore_fs_get_endpoint_info(
	void *context, void* endpoint_information);

bool iot_agent_datastore_fs_handle_request(pb_istream_t *istream,
	pb_ostream_t *ostream, const pb_field_t* message_type, void *context);

bool iot_agent_datastore_fs_handle_write_data(pb_istream_t *stream, const pb_field_t *field, void **arg);

extern const iot_agent_datastore_interface_t iot_agent_datastore_fs_interface;

/*!
*@}
*/ /* end of edgelock2go_agent_datastore_fs */
#endif // !(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1
#endif // !(defined(__ICCARM__) || defined(__CC_ARM) || defined(__ARMCC_VERSION))
#endif // _NXP_IOT_AGENT_DATASTORE_FS_H_
