/*
 * Copyright 2018-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

/** @file */

#ifndef _NXP_IOT_AGENT_UTILS_H_
#define _NXP_IOT_AGENT_UTILS_H_

#include <nxp_iot_agent.h>
#include <nxp_iot_agent_common.h>

#if NXP_IOT_AGENT_HAVE_SSS
#include <fsl_sss_api.h>
#endif

#if NXP_IOT_AGENT_HAVE_PSA
#include "psa_crypto_its.h"
#endif

#if SSS_HAVE_HOSTCRYPTO_OPENSSL
#include <openssl/ossl_typ.h>
#include <openssl/engine.h>
#endif

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

typedef struct iot_agent_context_t iot_agent_context_t;
typedef struct service_link_t service_link_t;
typedef struct iot_agent_datastore_t iot_agent_datastore_t;
typedef struct iot_agent_keystore_t iot_agent_keystore_t;

 /**
  * @brief The size of the buffer to reserve for the
  * EdgeLock 2GO cloud service client certificate.
  */
#define NXP_IOT_AGENT_EDGELOCK2GO_CLIENT_CERTIFICATE_BUFFER_SIZE 4096


#if SSS_HAVE_HOSTCRYPTO_OPENSSL

/*! @brief Create an EC_KEY key reference.
 *
 * @param[in]     keyStore Key store context
 * @param[in]     keyObject Reference to a key and it's properties. This must contain a valid key handle!
 * @param[in]     nid  NID of the OID of the curve name
 * @param[out]    out: Resulting key reference object

 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_ERROR_MEMORY upon allocation of memory failure
 * @retval IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED upon crypto operation failure
 * @retval IOT_AGENT_FAILURE upon failure
 */

iot_agent_status_t iot_agent_utils_gen_key_ref_ecc(sss_key_store_t *keyStore, sss_object_t *keyObject,
	int nid, EVP_PKEY* pkey);

/*! @brief Create an RSA_KEY key reference.
*
* @param[in]     keyStore Key store context
* @param[in]     keyObject Reference to a key and it's properties. This must contain a valid key handle!
* @param[out]    out Resulting key reference object

* @retval IOT_AGENT_SUCCESS upon success
* @retval IOT_AGENT_ERROR_MEMORY upon allocation of memory failure
* @retval IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED upon crypto operation failure
* @retval IOT_AGENT_FAILURE upon failure
*/
iot_agent_status_t iot_agent_utils_gen_key_ref_rsa(sss_key_store_t *keyStore, sss_object_t *keyObject,
	EVP_PKEY* pkey);

/*! @brief Create a key reference.
*
* @param[in]     keyStore Key store context
* @param[in]     keyObject Reference to a key and it's properties
* @param[out]    out Resulting key reference object

* @retval IOT_AGENT_SUCCESS upon success
* @retval IOT_AGENT_ERROR_MEMORY upon allocation of memory failure
* @retval IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED upon crypto operation failure
* @retval IOT_AGENT_FAILURE upon failure
*/
iot_agent_status_t iot_agent_utils_gen_key_ref(sss_key_store_t *keyStore, sss_object_t *keyObject, const uint32_t keyid, EVP_PKEY* pkey);


/*! @brief Create a PEM file containing a key reference.
*
* @param[in] keyStore Key store context
* @param[in] keyObject Reference to a key and it's properties
* @param[in] keyId Key id of key object
* @param[in] filename Name of the PEM file to be created

* @retval IOT_AGENT_SUCCESS upon success
* @retval IOT_AGENT_ERROR_MEMORY upon allocation of memory failure
* @retval IOT_AGENT_ERROR_FILE_SYSTEM upon failure while creating or writing key file
* @retval IOT_AGENT_FAILURE upon failure
*/
iot_agent_status_t iot_agent_utils_write_key_ref_pem(sss_key_store_t *keyStore,
	sss_object_t *keyObject, const uint32_t keyid, const char* filename);

/*! @brief Create a PEM file containing a certificate.
 *
 * @param[in] keyStore Key store context
 * @param[in] certid Certificate id
 * @param[in] filename Name of the PEM file to be created

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

* @retval IOT_AGENT_SUCCESS upon success
* @retval IOT_AGENT_FAILURE upon failure
*/
iot_agent_status_t iot_agent_utils_write_key_ref_service_pem(const iot_agent_context_t* ctx,
	const char* filename);

#endif


#if	(SSS_HAVE_HOSTCRYPTO_OPENSSL)
/*! @brief Create a PEM file containing service certificate stored in keystore.
*
* @param[in] ctx  Context for the iot_agent
* @param[in] service_descriptor Descriptor with service data
* @param[in] filename Name of the PEM file to be created

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

* @retval IOT_AGENT_SUCCESS upon success
* @retval IOT_AGENT_FAILURE upon failure
*/
iot_agent_status_t iot_agent_utils_write_key_ref_pem_cos_over_rtp(const iot_agent_context_t* ctx,
	const nxp_iot_ServiceDescriptor* service_descriptor, const char* filename);
iot_agent_status_t iot_agent_utils_der_to_pem_bio(BIO *bio_in, BIO* bio_out);

#endif

iot_agent_status_t iot_agent_utils_convert_service2key_id(uint64_t service_id, uint32_t *key_id);

/** @brief Maps a given service id to the range of keys that are managed by the EdgeLock 2GO cloud
 * service.
 *
 * @param[in] service_id  Service ID @param[out] key_id  Key ID
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

* @retval IOT_AGENT_SUCCESS upon success
* @retval IOT_AGENT_FAILURE upon failure
*/
iot_agent_status_t iot_agent_utils_get_certificate_common_name(iot_agent_context_t* ctx,
	const nxp_iot_ServiceDescriptor* service_descriptor, char* common_name, size_t max_size);


/*! @brief Read a certificate from a keystore.
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
 */
iot_agent_status_t iot_agent_utils_get_edgelock2go_certificate_id(iot_agent_keystore_t* keystore, uint32_t* object_id);

#if SSS_HAVE_HOSTCRYPTO_MBEDTLS
iot_agent_status_t iot_agent_utils_create_self_signed_edgelock2go_certificate(
        iot_agent_keystore_t* keystore, uint8_t* certificate_buffer, 
        size_t* certificate_buffer_size);
#endif

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
 */
iot_agent_status_t iot_agent_utils_write_edgelock2go_datastore(iot_agent_keystore_t *keystore,
	iot_agent_datastore_t* datastore, const char* hostname, uint32_t port,
	const pb_bytes_array_t* trusted_root_ca_certificates, const pb_bytes_array_t* client_certificate);


/** @brief Assemble a service descriptor for the connection to EdgeLock 2GO cloud service
 * and write it to a datastore.
 *
 * Per default this function takes the hostname and port from the defines
 *   - EDGELOCK2GO_HOSTNAME and
 *   - EDGELOCK2GO_PORT
 *
 * For testing purposes, it is possible to set-up the connection parameters
 * to the EdgeLock 2GO cloud service from externally by passing in information
 * via environment variables.
 *
 * The following env variables are considered:
 *   - IOT_AGENT_TEST_EDGELOCK2GO_HOSTNAME
 *   - IOT_AGENT_TEST_EDGELOCK2GO_PORT
 *
 * It is assumed that credentials (either ECC or RSA) for client certificate
 * authentication are stored in \p keystore.
 */
iot_agent_status_t iot_agent_utils_write_edgelock2go_datastore_from_env(iot_agent_keystore_t *keystore,
	iot_agent_datastore_t* datastore);


/**
* Checks existence of a file. If required it forces creation of file.
*
* @param[in] filename Name of the file
* @param[in] forceCreation Switch to force creation of the file

* @retval IOT_AGENT_SUCCESS upon success
* @retval IOT_AGENT_FAILURE upon failure
*/

iot_agent_status_t iot_agent_keystore_file_existence(const char *filename, bool forceCreation);

/** @brief Retrieve the UID of a device.
 *
 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_utils_get_device_id(uint8_t* buffer, size_t* len);

/** @brief Updates the length in order to remove padded data.
 *
 * \p padded data
 * \p padded data size
 */
iot_agent_status_t nxp_iot_agent_unpad_iso7816d4(uint8_t *data, size_t *data_size);

/** @brief Append padding bytes to a given message.
 *
 * Padding is done in-place, meaning the data buffer has to be big enough to hold also the padding
 * bytes.
 */
iot_agent_status_t iot_agent_pad_iso7816d4(uint8_t *data, size_t data_size, 
        size_t unpadded_length, size_t blocksize, size_t* padded_length);

#ifdef NXP_IOT_AGENT_ENABLE_LITE
/**
* Writes data to a specified flash area
*
* @param[in] area Flash area
* @param[in] area_size Size of the flash area
* @param[in] data Data to write
* @param[in] data_size Size of the data to write

* @retval IOT_AGENT_SUCCESS upon success
* @retval IOT_AGENT_FAILURE upon failure
*/
iot_agent_status_t iot_agent_utils_write_to_flash(const uint8_t *area, size_t area_size, const uint8_t *data, 
    size_t data_size);

/**
* Erases a specified flash area
*
* @param[in] area Flash area
* @param[in] area_size Size of the flash area

* @retval IOT_AGENT_SUCCESS upon success
* @retval IOT_AGENT_FAILURE upon failure
*/
iot_agent_status_t iot_agent_utils_erase_from_flash(const uint8_t *area, size_t area_size);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

/*!
 *@}
 */ /* end of edgelock2go_agent_utils */

#endif // #ifndef _NXP_IOT_AGENT_UTILS_H_
