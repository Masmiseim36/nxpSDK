/* 
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */
#ifndef _NXP_IOT_AGENT_UTILS_INTERNAL_H_
#define _NXP_IOT_AGENT_UTILS_INTERNAL_H_

#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_status.h>
#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>

#if NXP_IOT_AGENT_HAVE_SSS
#include <fsl_sss_api.h>
#endif

#if NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL
#include <openssl/ossl_typ.h>
#include <openssl/engine.h>
#endif

pb_ostream_t ostream_from_socket(void* network_context);
pb_istream_t istream_from_socket(void* network_context);


#if NXP_IOT_AGENT_HAVE_SSS
/**@ brief Start of the range of keys to use for keys of cloud services provisioned by EdgeLock 2GO. */
#define EDGELOCK2GO_MANAGED_SERVICE_KEY_MIN	0x80000000U

/**@ brief End of the range of keys to use for keys of cloud services provisioned by EdgeLock 2GO. */
#define EDGELOCK2GO_MANAGED_SERVICE_KEY_MAX	0x81000000U

/**
 * @brief The keyid on the SSS API to use for the keystore and keypair to use for connecting to
 * EdgeLock 2GO cloud service.
 */
#define EDGELOCK2GO_KEYID_ECC         0xF0000000U
#define EDGELOCK2GO_KEYID_RSA         0xF0000004U

/**
 * @brief The certid on the SSS API to use for the keystore to use for connecting to EdgeLock 2GO
 * cloud service.
 */
#define EDGELOCK2GO_CERTID_ECC        0xF0000001U
#define EDGELOCK2GO_CERTID_RSA        0xF0000005U

/**
 * @brief The attestation keyid on the SSS API to use for the attestation.
 */
#define EDGELOCK2GO_ATTESTATION_KEY_ECC         0xF0000012U
#define EDGELOCK2GO_ATTESTATION_KEY_RSA         0xF0000010U

#elif NXP_IOT_AGENT_HAVE_PSA

// TODO: Agree on all of these ids!

/**@ brief Start of the range of keys to use for keys of cloud services provisioned by EdgeLock 2GO. */
#define EDGELOCK2GO_MANAGED_SERVICE_KEY_MIN	0x08000000U

/**@ brief End of the range of keys to use for keys of cloud services provisioned by EdgeLock 2GO. */
#define EDGELOCK2GO_MANAGED_SERVICE_KEY_MAX	0x08100000U

/**
 * @brief The keyid on the PSA API to use for connecting to the EdgeLock 2GO cloud service.
 */
#if NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL || defined(NXP_IOT_AGENT_ENABLE_LITE)
#define EDGELOCK2GO_KEYID_ECC              0x3fff0201U
#else
#define EDGELOCK2GO_KEYID_ECC              0x7FFF816CU
#endif

/**
 * @brief The keyid on the PSA API to use for unwrapping EdgeLock 2GO cloud service key blobs into Sentinel50 slots.
 */
#define EL2GOIMPORT_KEK_SK                 0x3fff0210U

/**
 * @brief The keyid on the PSA API to use for decrypting EdgeLock 2GO cloud service key blobs into TF-M.
 */
#define EL2GOIMPORTTFM_KEK_SK                 0x3fff0211U

/**
 * @brief The keyid on the PSA API to use for validation of EdgeLock 2GO cloud service key blobs.
 */
#define EL2GOIMPORT_AUTH_SK                 0x3fff0212U

/**
 * @brief The keyid on the PSA API to use for fetching the device ID.
 */
#define DEVICEID_KEYID                     0x3fff0220U
#define DEVICEID_LENGTH                            16U

/**
 * @brief The keyid on the PSA API to use for fetching a claimcode.
 */
#define CLAIMCODE_OBJ_ID    		       0xf00000e0U

/**
 * @brief The keyid on the PSA API to use for OEM specific objects.
 */
#define EL2GO_OEM_FW_AUTH_KEY_HASH			0x7fff817aU
#define EL2GO_OEM_FW_DECRYPT_KEY			0x7fff817bU

#endif


#if NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL

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

#endif // NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL


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

#endif // #ifndef _NXP_IOT_AGENT_UTILS_INTERNAL_H_
