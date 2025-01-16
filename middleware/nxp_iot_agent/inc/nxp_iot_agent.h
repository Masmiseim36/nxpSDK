/* 
 * Copyright 2018-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

/** @file */

#ifndef _NXP_IOT_AGENT_H_
#define _NXP_IOT_AGENT_H_

#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_service.h>
#include <nxp_iot_agent_keystore.h>
#include <nxp_iot_agent_datastore.h>
#include <nxp_iot_agent_context.h>
#include <nxp_iot_agent_dispatcher.h>
#include "../protobuf/Agent.pb.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @defgroup edgelock2go_agent_main EdgeLock 2GO agent main API to interact with the cloud.
 * @ingroup edgelock2go_agent
 *
 * @brief Main API for registering keystores, datastores and connecting to the cloud to update provisionings.
 */

/*!
 * @addtogroup edgelock2go_agent_main
 * @{
 */

/** @brief The keystore that stores the credentials for the EdgeLock 2GO cloud service. 
 * 
 * When connecting to the EdgeLock 2GO cloud service, for the client authentication a private key is
 * required. This key is expected to be in a keystore which is registered to the EdgeLock 2GO agent.
 *
 * In case there is a datastore which holds the information how to connect to the EdgeLock 2GO cloud 
 * service this datastore also holds the information where to get the private key.
 *
 * In case the EdgeLock 2GO agent needs to fall back to compile-time constant connection 
 * information, it does assume that the private key for the client authentication is stored in a 
 * keystore that is registered with this ID.
 */
#define EDGELOCK2GO_KEYSTORE_ID                 0x70000010U

/**
 * @brief The keyid on the PSA API to use for fetching a claimcode.
 */
#define CLAIMCODE_OBJ_ID    		       0xf00000e0U

typedef struct _nxp_iot_UpdateStatusReport nxp_iot_UpdateStatusReport;

/*! @brief Initialize EdgeLock 2GO agent context memory with zeros. 
 *
 * @param[in, out]  ctx EdgeLock 2GO agent context
 * @retval IOT_AGENT_SUCCESS Upon success
 */
iot_agent_status_t iot_agent_init(iot_agent_context_t* ctx);


/*! @brief Register a keystore endpoint.
 *
 * Note that the ownership for the keystore is not transferred.
 * The caller is responsible that the keystore is freed at the
 * appropriate time.
 *
 * It is not possible to register two endpoints with the
 * same identifier.
 *
 * @param ctx: Context for the iot_agent.
 * @param[in] keystore Keystore that is registered.
 *
 * @retval IOT_AGENT_SUCCESS Upon success
 */
iot_agent_status_t iot_agent_register_keystore(
	iot_agent_context_t* ctx,
	iot_agent_keystore_t* keystore);

/*! @brief Register datastore endpoint.
 *
 * Note that the ownership for the datastore is not transferred.
 * The caller is responsible that the datastore is freed at the
 * appropriate time.
 *
 * It is not possible to register two endpoints with the
 * same identifier.
 *
 * @param[in] ctx: Context for the iot_agent.
 * @param[in] datastore Datastore that is registered.

 * @retval IOT_AGENT_SUCCESS Upon success
 */
iot_agent_status_t iot_agent_register_datastore(
	iot_agent_context_t* ctx,
	iot_agent_datastore_t* datastore);

/** @brief Set the datastore that is used to hold the 
 * information to connect to the EdgeLock 2GO cloud service.
 *
 * Note that the ownership for the datastore is not transferred.
 * The caller is responsible that the datastore is freed at the 
 * appropriate time.
 */
iot_agent_status_t iot_agent_set_edgelock2go_datastore(iot_agent_context_t* ctx,
	iot_agent_datastore_t* datastore);


/*! @brief Update device configuration
 * Reach out to EdgeLock 2GO cloud service for checking and (if applicable) fetching
 * configuration updates for the device.
 * @param[in] agent_context: Context for the iot_agent.
 * @param[out] status_report: Provides a more detailed view on the operations performed 
 *    during the update and its outcomes. If the argument is NULL, no detailed status 
 *    is reported. 
 * \post
 *   In case of success, the status_report structure is filled using dynamically allocated 
 *   fields and needs to be freed after usage by calling #iot_agent_free_status_report.
 *
 * @retval IOT_AGENT_SUCCESS Upon success
 * @retval IOT_AGENT_FAILURE Upon failure
 */
iot_agent_status_t iot_agent_update_device_configuration(iot_agent_context_t * agent_context, 
	nxp_iot_UpdateStatusReport* status_report);


/*! @brief Update device configuration from constants
 * 
 * Reach out to EdgeLock 2GO cloud service for checking and (if applicable) fetching
 * configuration updates for the device.
 * 
 * The connection details (hostname/port/server root certificates, etc.) are taken 
 * from the configuration constants in nxp_iot_agent_config.h.
 * 
 * It is necessary that an sss keystore that contains credentials (client key and client 
 * certificate) for connecting to the EdgeLock 2GO cloud service. The object ids to 
 * those credentials are settable via function arguments.
 *
 * @param[in] ctx: Context for the iot_agent.
 * @param[out] status_report: Provides a more detailed view on the operations performed
 *    during the update and its outcomes. If the argument is NULL, no detailed status
 *    is reported.
 * \post
 *   In case of success, the status_report structure is filled using dynamically allocated
 *   fields and needs to be freed after usage by calling #iot_agent_free_status_report.
 *
 * @retval IOT_AGENT_SUCCESS Upon success
 * @retval IOT_AGENT_FAILURE Upon failure
 */
iot_agent_status_t iot_agent_update_device_configuration_from_constants(
	iot_agent_context_t* agent_context,
	nxp_iot_UpdateStatusReport* status_report);

/*! @brief Update device configuration from service descriptor
 *
 * Reach out to EdgeLock 2GO cloud service for checking and (if applicable) fetching
 * configuration updates for the device.
 *
 * The connection details (hostname/port/server root certificates, etc.) are taken
 * from the service descriptor passed as input parameter
 *
 * It is necessary that an sss keystore that contains credentials (client key and client
 * certificate) for connecting to the EdgeLock 2GO cloud service. The object ids to
 * those credentials are settable via function arguments.
 *
 * @param[in] ctx: Context for the iot_agent.
 * @param[in] service_descriptor: Service descriptor including the connection parameters
 * @param[out] status_report: Provides a more detailed view on the operations performed
 *    during the update and its outcomes. If the argument is NULL, no detailed status
 *    is reported.
 * \post
 *   In case of success, the status_report structure is filled using dynamically allocated
 *   fields and needs to be freed after usage by calling #iot_agent_free_status_report.
 *
 * @retval IOT_AGENT_SUCCESS Upon success
 * @retval IOT_AGENT_FAILURE Upon failure
 */
iot_agent_status_t iot_agent_update_device_configuration_from_service_descriptor(
	iot_agent_context_t* agent_context,
	nxp_iot_ServiceDescriptor* service_descriptor,
	nxp_iot_UpdateStatusReport* status_report);

/*! @brief Update device configuration from datastore
 *
 * Reach out to EdgeLock 2GO cloud service for checking and (if applicable) fetching
 * configuration updates for the device.
 *
 * The connection details (hostname/port/server root certificates, etc.) are taken
 * from the datastore passed as input parameter
 *
 * It is necessary that an sss keystore that contains credentials (client key and client
 * certificate) for connecting to the EdgeLock 2GO cloud service. The object ids to
 * those credentials are settable via function arguments.
 *
 * @param[in] ctx: Context for the iot_agent.
 * @param[in] datastore: Datastore including the connection parameters
 * @param[out] status_report: Provides a more detailed view on the operations performed
 *    during the update and its outcomes. If the argument is NULL, no detailed status
 *    is reported.
 * \post
 *   In case of success, the status_report structure is filled using dynamically allocated
 *   fields and needs to be freed after usage by calling #iot_agent_free_status_report.
 *
 * @retval IOT_AGENT_SUCCESS Upon success
 * @retval IOT_AGENT_FAILURE Upon failure
 */
iot_agent_status_t iot_agent_update_device_configuration_from_datastore(
	iot_agent_context_t* agent_context,
	iot_agent_datastore_t* datastore,
	nxp_iot_UpdateStatusReport* status_report);

/*! @brief Select service by given index
 * @param[in] ctx: Context for the iot_agent.
 * @param[in] index: Index of the service
 * @param[out] service_descriptor: Structure for holding a service descriptor.
 *   Must point to a valid service descriptor object upon invocation. 
 *   Any FT_POINTER fields in the service descriptor are freed before changing 
 *   the contents to the service_descriptor of the selected service.
 * \pre
 *   The configuration data associated to the context is valid.
 * \post
 *   In case of success, the service_descriptor structure is filled and
 *   needs to be freed after usage by
 *   calling #iot_agent_free_service_descriptor. In case of failure no 
 *   freeing is required. Also in case of failures, the contents 
 *   of service_descriptor are not guaranteed to remain intact.
 * @retval IOT_AGENT_SUCCESS Upon success
 * @see iot_agent_is_service_configuration_data_valid
 */
iot_agent_status_t iot_agent_select_service_by_index(
    iot_agent_context_t* ctx,
    size_t index,
	nxp_iot_ServiceDescriptor* service_descriptor);


/*! @brief Select service by given ID
 * @param[in] ctx: Context for the iot_agent.
 * @param[in] service_id: ID of the service
 * @param[out] service_descriptor: Structure for holding a service descriptor.
 *   Must point to a valid service descriptor object upon invocation.
 *   Any FT_POINTER fields in the service descriptor are freed before changing
 *   the contents to the service_descriptor of the selected service.
 * \pre
 *   The configuration data associated to the context is valid.
 * \post
 *   In case of success, the service_descriptor structure is filled and 
 *   needs to be freed after usage by 
 *   calling #iot_agent_free_service_descriptor. In case of failure no
 *   freeing is required. Also in case of failures, the contents
 *   of service_descriptor are not guaranteed to remain intact.
 * @retval IOT_AGENT_SUCCESS Upon success
 * @see iot_agent_is_service_configuration_data_valid
 */
iot_agent_status_t iot_agent_select_service_by_id(
    iot_agent_context_t* ctx,
    uint64_t service_id,
	nxp_iot_ServiceDescriptor* service_descriptor);


/*! @brief Checks whether service configuration data of all registered 
 *   datastores is valid.
 * @param[in] ctx: Context for the iot_agent.
 * @retval true Service configuration data of all registered datastores is valid
 * @retval false Service configuration data of a registered datastores is invalid
 */
bool iot_agent_is_service_configuration_data_valid(
    const iot_agent_context_t* ctx);


/*! @brief Returns total number of services of all registered datastores
 * @param[in] ctx: Context for the iot_agent.
 * @retval Total number of services of all registered datastores
 */
size_t iot_agent_get_number_of_services(
	const iot_agent_context_t* ctx);


/** @brief Get the service descriptor of the currently selected service.
 *
 * @param[in] ctx: The context of the agent.
 * @param[out] service_descriptor: Structure for holding a service descriptor.
 *   Must point to a valid service descriptor object upon invocation.
 *   Any FT_POINTER fields in the service descriptor are freed before changing
 *   the contents to the service_descriptor of the selected service.
 * \pre
 *   The configuration data associated to the context is valid.
 * \post
 *   In case of success, the service_descriptor structure is filled and
 *   needs to be freed after usage by
 *   calling #iot_agent_free_service_descriptor. In case of failure no
 *   freeing is required. Also in case of failures, the contents
 *   of service_descriptor are not guaranteed to remain intact.
 * @retval IOT_AGENT_SUCCESS Upon success
 */
iot_agent_status_t iot_agent_get_service_descriptor(
	const iot_agent_context_t* ctx,
	nxp_iot_ServiceDescriptor* service_descriptor);


/*! @brief Free all FT_POINTER fields of a service descriptor.
 *
 * When selecting a service, a service descriptor is read from a 
 * datastore. A service descriptor can contain fields of variable 
 * length (binary data (certificates, etc.) or text (hostname, etc.)).
 * Those fields use dynamically allocated memory. The memory is freed
 * by calling this function.
 *
 * @param[in] service_descriptor: Reference to service descriptor
 */
void iot_agent_free_service_descriptor(
	nxp_iot_ServiceDescriptor* service_descriptor);


/*! @brief Free all FT_POINTER fields of a update status report.
 *
 * When a status report is filled during updating a device configuration, it 
 * contains pointer fields which use dynamically allocated memory. 
 * The memory is freed by calling this function.
 *
 * @param[in] status_report: Reference to status_report
 */
void iot_agent_free_update_status_report(nxp_iot_UpdateStatusReport* status_report);

/*! @brief Print status report.
 *
 * It prints the status report of the agent execution.
 *
 * @param[in] status_report: Reference to status_report
 */
void iot_agent_print_status_report(const nxp_iot_UpdateStatusReport* status_report);

/** @brief Get the index of a datastore based on its identifier.
 *
 * @retval IOT_STATUS_SUCCESS: A datastore with the \p id was found 
 *   in the agent's context and its index is returned in \p index.
 */
iot_agent_status_t iot_agent_get_datastore_index_by_id(
	const iot_agent_context_t* ctx,
	const uint32_t id,
	size_t* index);


/** @brief Get a reference to a datastore based on its identifier.
 *
 * @retval IOT_STATUS_SUCCESS: A datastore with the \p id was found 
 *   in the agent's context and a pointer to it is returned in \p datastore.
 */
iot_agent_status_t iot_agent_get_datastore_by_id(
	const iot_agent_context_t* ctx,
	const uint32_t id,
	iot_agent_datastore_t** datastore);


/** @brief Get the index of a keystore based in its identifier.
 *
 * @retval IOT_STATUS_SUCCESS: A keystore with the \p id was found 
 *   in the agent's context and its index is returned in \p index.
 */
iot_agent_status_t iot_agent_get_keystore_index_by_id(
	const iot_agent_context_t* ctx,
	const uint32_t id,
	size_t* index);


/** @brief Get a reference to a keystore based on its identifier.
 *
 * @retval IOT_STATUS_SUCCESS: A keystore with the \p id was found 
 *   in the agent's context and a pointer to it is returned in \p keystore.
 */
iot_agent_status_t iot_agent_get_keystore_by_id(
	const iot_agent_context_t* ctx,
	const uint32_t id,
	iot_agent_keystore_t** keystore);


/*! @brief Get an endpoint information of the endpoint
 * @param[in] context Reference to end point context
 * @param[in] endpoint_information Reference to end point information
 */
bool iot_agent_get_endpoint_info(void* context, void* endpoint_information);


typedef struct pb_istream_s pb_istream_t;
typedef struct pb_ostream_s pb_ostream_t;
typedef struct pb_field_s pb_field_t;


/*! @brief handle request by end point
 * @param[in] istream Input stream
 * @param[in] ostream Output stream
 * @param[in] message_type a pointer to the message type fields array
 * @param[in] context End point context
 **/
bool iot_agent_handle_request(pb_istream_t *istream,
	pb_ostream_t *ostream, const pb_field_t* message_type, void *context);

/*! @brief Initialize Dispatcher
 * @param[in] dispatcher_context Context for the dispatcher
 * @param[in] agent_context Context for the agent
 * @param[in] service_descriptor The service descriptor containing the connection parameters to
 *   connect to the EdgeLock 2GO cloud service.
 * @param[out] status_report A pointer to a structure that gets filled with a status report after
 *   the update is complete. If NULL is given, no status report is created.
 */
iot_agent_status_t iot_agent_init_dispatcher(
    iot_agent_dispatcher_context_t * dispatcher_context,
	iot_agent_context_t * agent_context,
	nxp_iot_ServiceDescriptor* service_descriptor,
	nxp_iot_UpdateStatusReport* status_report);
/*!
 *@}
 */ /* end of edgelock2go_agent_main */


#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef _NXP_IOT_AGENT_H_
