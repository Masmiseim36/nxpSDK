/*
 * Copyright 2018-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

/** @file */

#ifndef _NXP_IOT_AGENT_UTILS_H_
#define _NXP_IOT_AGENT_UTILS_H_

#include <nxp_iot_agent_context.h>
#include <nxp_iot_agent_keystore.h>

#include "../protobuf/ServiceDescriptor.pb.h"

/*! @defgroup edgelock2go_agent_utils EdgeLock 2GO agent generic util functions.
 * @ingroup edgelock2go_agent
 *
 * @brief Utility functions for the EdgeLock 2GO agent for writing and generating key references and certificates.
 *
 *
 * @addtogroup edgelock2go_agent_utils
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

 /**
  * @brief The size of the buffer to reserve for the
  * EdgeLock 2GO cloud service client certificate.
  */
#define NXP_IOT_AGENT_EDGELOCK2GO_CLIENT_CERTIFICATE_BUFFER_SIZE 4096


/*! @brief Create a PEM file containing a key.
 *
 * @param[in] keyStore Key store context
 * @param[in] keyid Key id
 * @param[in] filename Name of the PEM file to be created
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED upon crypto operation failure
 * @retval IOT_AGENT_ERROR_FILE_SYSTEM upon failure while creating or writing certificate file
 * @retval IOT_AGENT_FAILURE upon failure
*/
iot_agent_status_t iot_agent_utils_write_key_ref_pem_from_keystore(iot_agent_keystore_t *keyStore,
	const uint32_t keyid, const char* filename);

/*! @brief Create a PEM file containing a certificate.
 *
 * @param[in] keyStore Key store context
 * @param[in] certid Certificate id
 * @param[in] filename Name of the PEM file to be created
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED upon crypto operation failure
 * @retval IOT_AGENT_ERROR_FILE_SYSTEM upon failure while creating or writing certificate file
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_write_certificate_pem_from_keystore(iot_agent_keystore_t *keyStore,
	const uint32_t certid, const char* filename);


/*! @brief Create a PEM file containing a certificate chain.
 *
 * @param[in] buffer Buffer containing the encoded certificate
 * @param[in] len length of the buffer
 * @param[in] filename Name of the PEM file to be created
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED upon crypto operation failure
 * @retval IOT_AGENT_ERROR_FILE_SYSTEM upon failure while creating or writing certificate file
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_write_certificate_pem(uint8_t* buffer, size_t len, const char* filename);

/*! @brief Create a PEM file containing a key reference to the key of the currently selected service.
 *
 * \pre
 *   A service needs to be selected before calling this function.
 * @param[in] ctx  Context for the iot_agent
 * @param[in] filename Name of the PEM file to be created
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_write_key_ref_service_pem(const iot_agent_context_t* ctx,
	const char* filename);


/*! @brief Create a PEM file containing service certificate stored in keystore.
 *
 * @param[in] ctx  Context for the iot_agent
 * @param[in] service_descriptor Descriptor with service data
 * @param[in] filename Name of the PEM file to be created
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_write_certificate_pem_cos_over_rtp(iot_agent_context_t* ctx,
	const nxp_iot_ServiceDescriptor* service_descriptor, const char* filename);

/*! @brief Create a PEM file containing a service key pair stored in keystore.
 *
 * @param[in] ctx  Context for the iot_agent
 * @param[in] service_descriptor Descriptor with service data
 * @param[in] filename Name of the PEM file to be created
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_write_key_ref_pem_cos_over_rtp(const iot_agent_context_t* ctx,
	const nxp_iot_ServiceDescriptor* service_descriptor, const char* filename);

/** @brief Converts the service ID to key id.
 *
 * @param[in] service_id  Service ID @
 * param[out] key_id_ptr  Key ID
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_convert_service2key_id(uint64_t service_id, uint32_t *key_id_ptr);

/** @brief Maps a given service id to the range of keys that are managed by the EdgeLock 2GO cloud
 * service.
 *
 * @param[in] keystore The input keystore
 * @param[in] object_ids The list of object IDs
 * @param[in] num_objects The number of objects
 * @param[out] object_id The output object id
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_get_first_found_object(iot_agent_keystore_t* keystore,
	uint32_t* object_ids, size_t num_objects, uint32_t* object_id);

/*! @brief Gets the common name from the client certificte.
 *
 * @param[in] ctx  Context for the iot_agent
 * @param[in] service_descriptor Descriptor with service data
 * @param[in,out] common_name Common name string
 * @param[in] max_size Maximum size
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_get_certificate_common_name(iot_agent_context_t* ctx,
	const nxp_iot_ServiceDescriptor* service_descriptor, char* common_name, size_t max_size);


/*! @brief Read a certificate from a keystore.
 *
 * @param[in] keystore The input keystore
 * @param[in] certificate_id ID of the certificate
 * @param[out] cert Buffer including the certificate
 * @param[in, out] cert_len: the length of the certificate
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_get_certificate_from_keystore(iot_agent_keystore_t* keystore,
	uint32_t certificate_id, uint8_t* cert, size_t* cert_len);


/** @brief Checks whether a keystore contains the object with the defined key to use for
 * authenticating at the EdgeLock 2GO cloud service.
 *
 * The keystore might contain keys using ECC and/or RSA. If available, it will return
 * the object id of the ECC key, otherwise the object id of the RSA key.
 *
 * When neither is found or in case of other issues with the keystore, the function does
 * not return IOT_AGENT_SUCCESS.
 *
 * @param[in] keystore The input keystore
 * @param[out] object_id ID of the key
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_get_edgelock2go_key_id(iot_agent_keystore_t* keystore, uint32_t* object_id);


/** @brief Checks whether a keystore contains the object with the defined certificate to
 * use for authenticating at the EdgeLock 2GO cloud service.
 *
 * The keystore might contain keys using ECC and/or RSA. If available, it will return
 * the object id of the ECC certificate, otherwise the object id of the RSA certificate.
 *
 * When neither is found or in case of other issues with the keystore, the function does
 * not return IOT_AGENT_SUCCESS.
 *
 * @param[in] keystore The input keystore
 * @param[out] object_id ID of the certificate
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_get_edgelock2go_certificate_id(iot_agent_keystore_t* keystore, uint32_t* object_id);

/** @brief Creates a self signed certificate
 *
 * @param[in] keystore The input keystore
 * @param[out] certificate_buffer Buffer including the certificate
 * @param[in, out] certificate_buffer_size: the length of the certificate
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_create_self_signed_edgelock2go_certificate(
        iot_agent_keystore_t* keystore, uint8_t* certificate_buffer, 
        size_t* certificate_buffer_size);

/** @brief Assemble a service descriptor for the connection to EdgeLock 2GO cloud service
 * and write it to a datastore.
 *
 * It is assumed that keys (either ECC or RSA) for authentication are stored 
 * in \p keystore.
 * 
 * If \p client_certificate is NULL, it is assumed that the client certificate is stored 
 * in \p keystore as well - in this case the resulting datastore contents will contain a reference 
 * to the certificate. 
 * 
 * If \p client_certificate is not NULL, it shall point to a client certificate, which will be copied 
 * into the service descriptor.
 *
 * @param[in] keystore The input keystore
 * @param[out] datastore The datastore where to write
 * @param[in] hostname The hostname URL
 * @param[in] port The port number
 * @param[in] trusted_root_ca_certificates Buffer including the root certificates
 * @param[in] client_certificate Buffer including client certificate
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_write_edgelock2go_datastore(iot_agent_keystore_t *keystore,
	iot_agent_datastore_t* datastore, const char* hostname, uint32_t port,
	const pb_bytes_array_t* trusted_root_ca_certificates, const pb_bytes_array_t* client_certificate);

/** @brief Assemble a service descriptor for the connection to EdgeLock 2GO cloud service
 * and write it to a datastore.
 *
 * The function gets the EDGELOCK2GO_HOSTNAME and EDGELOCK2GO_PORT from different inputs
 * respecting the following priority order:
 * - Command line arguments
 * - Environment variables
 * - Existing valid datastore file
 * - Macro defines
 *
 * Both hostname and port must be provided on the input that the function is configure it from there
 * 
 * It is assumed that credentials (either ECC or RSA) for client certificate
 *
 * authentication are stored in \p keystore.
 * @param[in] keystore The input keystore
 * @param[out] datastore The datastore where to write
 * @param[in] argc Number of arguments
 * @param[in] argv Argument array
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 *
 */
iot_agent_status_t iot_agent_utils_configure_edgelock2go_datastore(iot_agent_keystore_t *keystore,
	iot_agent_datastore_t* datastore, int argc, const char *argv[]);

/**
 * Checks existence of a file. If required it forces creation of file.
 *
 * @param[in] filename Name of the file
 * @param[in] forceCreation Switch to force creation of the file
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */

iot_agent_status_t iot_agent_keystore_file_existence(const char *filename, bool forceCreation);

/** @brief Retrieve the UID of a device.
 *
 * @param[out] buffer The buffer including the UUID
 * @param[in,out] len The length of the buffer
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_get_device_id(uint8_t* buffer, size_t* len);

/** @brief Updates the length in order to remove padded data.
 *
 * @param[in,out] data Buffer to be unpad
 * @param[in,out] data_size The length of the buffer
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 *
 */
iot_agent_status_t nxp_iot_agent_unpad_iso7816d4(uint8_t *data, size_t *data_size);

/** @brief Append padding bytes to a given message.
 *
 * Padding is done in-place, meaning the data buffer has to be big enough to hold also the padding
 * bytes.
 *
 * @param[in,out] data Buffer to be unpad
 * @param[in] data_size The length of the buffer
 * @param[in] unpadded_length The length of the unpadded part
 * @param[in] blocksize Size of the block
 * @param[in,out] padded_length The length of the padded part
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 *
 *
 */
iot_agent_status_t iot_agent_pad_iso7816d4(uint8_t *data, size_t data_size, 
        size_t unpadded_length, size_t blocksize, size_t* padded_length);


#ifdef __cplusplus
} // extern "C"
#endif

/*!
 *@}
 */ /* end of edgelock2go_agent_utils */

#endif // #ifndef _NXP_IOT_AGENT_UTILS_H_
