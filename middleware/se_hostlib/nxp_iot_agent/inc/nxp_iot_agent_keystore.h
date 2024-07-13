/*
 * Copyright 2018-2021,2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

/** @file */

#ifndef _NXP_IOT_AGENT_KEYSTORE_H_
#define _NXP_IOT_AGENT_KEYSTORE_H_

#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_endpoint.h>
#if NXP_IOT_AGENT_HAVE_SSS
#include <fsl_sss_api.h>
#endif

/*!@defgroup edgelock2go_agent_keystore EdgeLock 2GO agent keystore functionality.
 * @ingroup edgelock2go_agent
 *
 * @brief Functions to interact with a keystore. There are concrete implementations of this API for different SEs.
 */


/*!
 * @addtogroup edgelock2go_agent_keystore
 * @{
 */


typedef struct pb_istream_s pb_istream_t;
typedef struct pb_ostream_s pb_ostream_t;
typedef struct pb_field_s pb_field_t;


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
#if NXP_IOT_AGENT_HAVE_SSS
	sss_key_store_t* sss_context;
#endif
	void* context;
} iot_agent_keystore_t;


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
