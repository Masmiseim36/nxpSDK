/*
 * Copyright 2018-2021,2024-2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

/** @file */

#ifndef _NXP_IOT_AGENT_KEYSTORE_H_
#define _NXP_IOT_AGENT_KEYSTORE_H_

#include <nxp_iot_agent_status.h>
#include <nxp_iot_agent_endpoint.h>
#include <nxp_iot_agent_platform.h>

/*!@defgroup edgelock2go_agent_keystore EdgeLock 2GO agent keystore functionality.
 * @ingroup edgelock2go_agent
 *
 * @brief Functions to interact with a keystore. There are concrete implementations of this API for different SEs.
 */


/*!
 * @addtogroup edgelock2go_agent_keystore
 * @{
 */

typedef iot_agent_status_t(*iot_agent_keystore_destroyer_t)(
	void* context);

typedef iot_agent_status_t(*iot_agent_keystore_session_opener_t)(
	void* context);

typedef void(*iot_agent_keystore_session_closer_t)(
	void* context);


/**
 * @brief The interface any keystore needs to implement.
 */
typedef struct iot_agent_keystore_interface_t
{
	iot_agent_keystore_destroyer_t          destroy;
	iot_agent_keystore_session_opener_t     open_session;
	iot_agent_keystore_session_closer_t     close_session;
	iot_agent_endpoint_interface_t          endpoint_interface;
} iot_agent_keystore_interface_t;


/**
 * @brief A structure binding a keystore interface and a keystore context to a keystore instance.
 */
typedef struct iot_agent_keystore_t
{
	iot_agent_keystore_interface_t iface;
	uint32_t type;
	uint32_t identifier;
	void* context;
} iot_agent_keystore_t;

/**
 * @brief A structure used to abstract key reference between different OpenSSL versions.
 */
typedef struct iot_agent_keystore_key_ref_t
{
	void* key_ref;
}iot_agent_keystore_key_ref_t;

/** @brief Initialize an Agent keystore.
 *
 * The Agent keystore; internally executes all the necessary operations which
 * allows the specific keystore to be initialized as for example openining a SE5x session
 * with the secure element if necessary
 *
 * If the function does not return IOT_AGENT_SUCCESS, it can be assumed that
 * no memory was allocated. The datastore is unusable.
 */
iot_agent_status_t iot_agent_keystore_init(iot_agent_keystore_t* keystore,
	uint32_t identifier, iot_agent_platform_context_t* platform_context);

/** @brief Destroy the keystore.
 *
 * Depending on the type of the keystore this is triggering actions on the keystore's context
 * itself by calling the _destroy() function of the keystore interface.
 *
 * The \p keystore is not usable after a call to iot_agent_keystore_free.
 */
iot_agent_status_t iot_agent_keystore_free(iot_agent_keystore_t* keystore);


/** @brief Open a session/connection to a keystore.
 */
iot_agent_status_t iot_agent_keystore_open_session(iot_agent_keystore_t* keystore);


/** @brief Close a session/connection to a keystore.
*/
void iot_agent_keystore_close_session(iot_agent_keystore_t* keystore);


/*!
*@}
*/ /* end of edgelock2go_agent_keystore */

#endif // _NXP_IOT_AGENT_KEYSTORE_H_
