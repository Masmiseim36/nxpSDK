/* 
 * Copyright 2018-2019, 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */
#ifndef _NXP_IOT_AGENT_STATUS_H_
#define _NXP_IOT_AGENT_STATUS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*! 
 * @ingroup edgelock2go_agent_main
 *
 * @addtogroup edgelock2go_agent_main
 * @{
 */

/*! \public
 * @brief EdgeLock 2GO agent status codes.
 *
 * Enumeration of return values from the iot_agent_* functions.
 *  - Values less than -1 are specific error codes.
 *  - Value of -1 is a generic failure response.
 *  - Value of 0 is a generic success response.
 *  - Values greater than 0 are specific non-error return codes.
 */
typedef enum iot_agent_status_t {
	/** Success return value - no error occurred */
	IOT_AGENT_SUCCESS = 0,

	/** A generic error. Not enough information for a specific error code. */
	IOT_AGENT_FAILURE = -1,

	/** Unable to allocate memory. */
	IOT_AGENT_ERROR_MEMORY = -2,

	/** Error on file handling. */
	IOT_AGENT_ERROR_FILE_SYSTEM = -3,

	/** Invocation of a function of a crypto engine failed. */
	IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED = -20,

	/** The EdgeLock 2GO cloud service reported an error when updating a device. */
	IOT_AGENT_UPDATE_FAILED = -40,
} iot_agent_status_t;

/*!
 * @addtogroup edgelock2go_agent_main
 * @{
 */

#ifdef __cplusplus
}
#endif

#endif // #ifndef _NXP_IOT_AGENT_STATUS_H_
