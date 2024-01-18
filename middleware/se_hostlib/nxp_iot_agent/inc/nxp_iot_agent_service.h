/*
 * Copyright 2018-2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

/** @file */

#ifndef _NXP_IOT_AGENT_SERVICE_H_
#define _NXP_IOT_AGENT_SERVICE_H_

#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_datastore.h>

#include "../protobuf/ServiceDescriptor.pb.h"

/*! @defgroup edgelock2go_agent_service EdgeLock 2GO agent service information.
 *
 * @ingroup edgelock2go_agent
 *
 * @brief Functionality to work with service descriptors. A service descriptor represents all information from a single cloud provisioning.
 *
 */

/*!
 * @addtogroup edgelock2go_agent_service
 * @{
 */

#define IOT_AGENT_CONFIGURATION_DATA_VERSION (0U)

typedef uint8_t public_key_identifier_t[16];
typedef uint8_t service_identifier_t[32];

/**
 * @brief The header of configuration data stored in a datastore.
 *
 */
#if defined(_WIN32)
#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
typedef struct
#elif defined(__unix) || defined(__GNUC__) || defined(__clang__)
typedef struct __attribute__((packed, aligned(1)))
#else // _WIN32
typedef __PACKED_STRUCT
#endif
configuration_data_header_t {
	/** @brief SHA256 of configuration data (all data after the checksum field). */
	uint8_t checksum[32];

	/** @brief Total length of the stored data (incl. all fields of configuration_data_header_t). */
	uint32_t length;

	/** @brief Version number of the structure of the configuration data. */
	uint32_t version;

	/** @brief Number of stored service configurations. */
	uint32_t number_of_services;
} configuration_data_header_t;
#if defined(_WIN32)
#pragma pack(pop)   /* restore original alignment from stack */
#endif

typedef struct iot_agent_keystore_t iot_agent_keystore_t;
typedef struct iot_agent_datastore_t iot_agent_datastore_t;

typedef struct service_link_t
{
	// Reference to the datastore which contains the service descriptor
	const iot_agent_datastore_t* datastore;
	// Location of the service descriptor in the datastore
	size_t offset;

	// Reference to the keystore
	const iot_agent_keystore_t* keystore;

} service_link_t;


/*! @brief Checks service configuration data of all registered datastores are valid
 * @param[in] ctx: Context for the iot_agent.
 * @retval true Service configuration data of all registered datastores are valid
 * @retval false Service configuration data of a registered datastores is invalid
 */
bool iot_agent_service_is_configuration_data_valid(
		const iot_agent_datastore_t* ctx);


/**
 * @brief Get a textual description of the service type.
 *
 * Service type strings are internal constant c-strings. This function returns a pointer to such a
 * string, ownership remains with the iot_agent.
 *
 * @param[in] service_descriptor The service descriptor to get the type for.
 * @param[out] buffer A pointer to a c-string that is changed to point to the textual representation of
 *   the service type.
 *
 * @retval IOT_AGENT_SUCCESS The service type could be resolved.
 * @retval IOT_AGENT_FAILURE The service type is invalid.
 */
iot_agent_status_t iot_agent_service_get_service_type_as_string(
	const nxp_iot_ServiceDescriptor* service_descriptor, const char** buffer);


/**
 * @brief Get a textual description of the protocol.
 *
 * Protocol strings are internal constant c-strings. This function returns a pointer to such a
 * string, ownership remains with the iot_agent.
 *
 * @param[in] service_descriptor The service descriptor to get the protocol for.
 * @param[out] buffer A pointer to a c-string that is changed to point to the textual representation of
 *   the protocol.
 *
 * @retval IOT_AGENT_SUCCESS The service type could be resolved.
 * @retval IOT_AGENT_FAILURE The service type is invalid.
 */
iot_agent_status_t iot_agent_service_get_protocol_of_service_as_string(
	const nxp_iot_ServiceDescriptor* service_descriptor, const char** buffer);


/**
 * @brief Get the number of services that are available.
 * @param[in] ctx: Context for the iot_agent.
 * @return The number of services that are avilable.
 */
size_t iot_agent_service_get_number_of_services(
		const iot_agent_datastore_t* ctx);


/**
 * @brief Get the offset and the service descriptor of a service specified by service_id.
 *
 * @param[in] ctx: The datastore to query for the service.
 * @param[in] service_id: ID of the service
 * @param[out] offset: The offset of the service descriptor within the datastore.
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
iot_agent_status_t iot_agent_service_get_service_offset_by_id(
		const iot_agent_datastore_t* ctx,
		uint64_t service_id,
		size_t* offset,
		nxp_iot_ServiceDescriptor* service_descriptor);

/**
 * @brief Get the offset and the service descriptor of a service specified by index.
 *
 * @param[in] ctx: The datastore to query for the service.
 * @param[in] index: The index of the service (within the given datastore).
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
iot_agent_status_t iot_agent_service_get_service_offset_by_index(
		const iot_agent_datastore_t* datastore,
		size_t index,
		size_t* offset,
		nxp_iot_ServiceDescriptor* service_descriptor);



/**
 * @brief Get the service descriptor of a service specified by offset.
 *
 * @param[in] ctx: The datastore to query for the service.
 * @param[in] offset: The offset of the service - the memory location in 
 *   the given datastore.
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
iot_agent_status_t iot_agent_service_get_service_descriptor_of_service(
	 const iot_agent_datastore_t* datastore,
	 size_t offset,
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
void iot_agent_service_free_service_descriptor(
	 nxp_iot_ServiceDescriptor* service_descriptor);
iot_agent_status_t iot_agent_service_read_buffer(const iot_agent_datastore_t* ctx, size_t offset, void* buffer, size_t expected_len);
void iot_agent_service_read_header(const iot_agent_datastore_t* ctx, size_t offset, configuration_data_header_t* header);
#if SSS_HAVE_HOSTCRYPTO_OPENSSL
iot_agent_status_t iot_agent_service_calculate_cofiguration_checksum(const iot_agent_datastore_t* ctx,
    const configuration_data_header_t* header, uint8_t calculated_checksum[32]);
#elif SSS_HAVE_HOSTCRYPTO_MBEDTLS
iot_agent_status_t iot_agent_service_calculate_cofiguration_checksum(const iot_agent_datastore_t* ctx,
    const configuration_data_header_t* header, uint8_t calculated_checksum[32]);
#endif
/*!
 *@}
 */ /* end of edgelock2go_agent_service */

#endif // _NXP_IOT_AGENT_SERVICE_H_
