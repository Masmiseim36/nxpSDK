/*
 * Copyright 2021-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _NXP_IOT_AGENT_KEYSTORE_PSA_H_
#define _NXP_IOT_AGENT_KEYSTORE_PSA_H_


#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_keystore.h>

#if SSS_HAVE_MBEDTLS_ALT_PSA

 /*! @defgroup edgelock2go_agent_keystore_psa related keystore functionality
 * @ingroup edgelock2go_agent_keystore
 *
 * @addtogroup edgelock2go_agent_keystore_psa
 * @{
 */

 /**
 * @brief A context holding the state of a keystore; this is passed to keystore interface functions.
 */
typedef struct iot_agent_keystore_psa_context_t
{
	uint32_t dummy;
} iot_agent_keystore_psa_context_t;


/** @brief Initialize a psa keystore.
 *
 * The init function does allocate memory for the context. To free it, use
 * iot_agent_datastore_free.
 *
 * If the function does not return IOT_AGENT_SUCCESS, it can be assumed that
 * no memory was allocated. The keystore is unusable.
 */
iot_agent_status_t iot_agent_keystore_psa_init(iot_agent_keystore_t* keystore,
	uint32_t identifier);


iot_agent_status_t iot_agent_keystore_psa_destroy(void *context);

iot_agent_status_t iot_agent_keystore_psa_open_session(void *context);

void iot_agent_keystore_psa_close_session(void *context);


/*! @brief Get an endpoint information of the keystore
 * @param[in] context End point context
 * @param[in] endpoint_information End point information
 *
 */
bool iot_agent_keystore_psa_get_endpoint_info(
	void *context, void* endpoint_information);

/** @copydoc iot_agent_endpoint_request_handler_t
*
*/
bool iot_agent_keystore_psa_handle_request(pb_istream_t *istream,
	pb_ostream_t *ostream, const pb_field_t* message_type, void *context);


extern const iot_agent_keystore_interface_t iot_agent_keystore_psa_interface;

/*!
*@}
*/ /* end of edgelock2go_agent_keystore_psa */

#endif // SSS_HAVE_MBEDTLS_ALT_PSA

#endif // _NXP_IOT_AGENT_KEYSTORE_PSA_H_
