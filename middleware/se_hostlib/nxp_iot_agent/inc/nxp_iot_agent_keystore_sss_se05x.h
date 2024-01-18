/*
 * Copyright 2018-2022 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _NXP_IOT_AGENT_KEYSTORE_SSS_SE05X_H_
#define _NXP_IOT_AGENT_KEYSTORE_SSS_SE05X_H_


#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_keystore.h>

#if NXP_IOT_AGENT_HAVE_SSS

#include <fsl_sss_se05x_apis.h>
#include <ex_sss_boot.h>

 /*! @defgroup edgelock2go_agent_keystore_se05x SE050 related keystore functionality
 * @ingroup edgelock2go_agent_keystore
 *
 * @addtogroup edgelock2go_agent_keystore_se05x
 * @{
 */

 /**
 * @brief A context holding the state of a keystore; this is passed to keystore interface functions.
 */
typedef struct iot_agent_keystore_sss_se05x_context_t
{
	ex_sss_boot_ctx_t* boot_context;
	sss_key_store_t* sss_context;
	bool session_open;
} iot_agent_keystore_sss_se05x_context_t;


/** @brief Initialize a se05x keystore.
 *
 * The init function does allocate memory for the context. To free it, use
 * iot_agent_datastore_free.
 *
 * It is assumed, that a boot_context for the secure element has been
 * established and boot_context is a valid context. The keystore does
 * not take ownership of the boot_context, its memory/lifetime has to be
 * managed by the caller. The keystore does, manipulate and depend on the
 * state within of the context.
 *
 * The parameter is_session_open indicates wheter boot_context at the
 * time of handover has an established session to the secure element.
 *
 * If the function does not return IOT_AGENT_SUCCESS, it can be assumed that
 * no memory was allocated. The datastore is unusable.
 */
iot_agent_status_t iot_agent_keystore_sss_se05x_init(iot_agent_keystore_t* keystore,
	uint32_t identifier, ex_sss_boot_ctx_t* boot_context, bool is_session_open);


iot_agent_status_t iot_agent_keystore_sss_se05x_destroy(void *context);

iot_agent_status_t iot_agent_keystore_sss_se05x_open_session(void *context);

void iot_agent_keystore_sss_se05x_close_session(void *context);


/*! @brief Get an endpoint information of the keystore
 * @param[in] context End point context
 * @param[in] endpoint_information End point information
 *
 */
bool iot_agent_keystore_sss_se05x_get_endpoint_info(
	void *context, void* endpoint_information);

/** @copydoc iot_agent_endpoint_request_handler_t
*
*/
bool iot_agent_keystore_sss_se05x_handle_request(pb_istream_t *istream,
	pb_ostream_t *ostream, const pb_field_t* message_type, void *context);

iot_agent_status_t iot_agent_keystore_sss_se05x_get_sss_key_store(iot_agent_keystore_sss_se05x_context_t* context,
	sss_key_store_t** sss_key_store);

extern const iot_agent_keystore_interface_t iot_agent_keystore_sss_se05x_interface;

/*!
*@}
*/ /* end of edgelock2go_agent_keystore_se05x */

#endif // #if NXP_IOT_AGENT_HAVE_SSS

#endif // _NXP_IOT_AGENT_KEYSTORE_SSS_SE05X_H_
