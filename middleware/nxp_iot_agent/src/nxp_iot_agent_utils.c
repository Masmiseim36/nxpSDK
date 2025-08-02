/*
 * Copyright 2018-2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <nxp_iot_agent_utils.h>
#include <nxp_iot_agent_utils_internal.h>

#if !(defined(__ICCARM__) || defined(__CC_ARM) || defined(__ARMCC_VERSION))
#include <unistd.h>
#endif

#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <nxp_iot_agent_macros_openssl.h>
#if (OPENSSL_VERSION_NUMBER >= 0x30000000)
#include <openssl/core_names.h>
#include <openssl/param_build.h>
#include <openssl/params.h>
#include <openssl/provider.h>
#endif //if (OPENSSL_VERSION_NUMBER >= 0x30000000)
#endif //#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)

#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS == 1)
#include "network_mbedtls.h"
#include <mbedtls/version.h>
#include <mbedtls/sha256.h>
#include <mbedtls/oid.h>
#include <mbedtls/x509_crt.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include "psa_crypto_its.h"
#endif

#ifdef NXP_IOT_AGENT_ENABLE_LITE
#include <fsl_romapi_iap.h>
#include <task.h>

#define SYSTEM_IS_XIP_FLEXSPI()                                                                               \
    ((((uint32_t)SystemCoreClockUpdate >= 0x08000000U) && ((uint32_t)SystemCoreClockUpdate < 0x10000000U)) || \
     (((uint32_t)SystemCoreClockUpdate >= 0x18000000U) && ((uint32_t)SystemCoreClockUpdate < 0x20000000U)))

#define FCB_ADDRESS             0x08000400U
#define FLEXSPI_INSTANCE        0U
#define FLASH_OPTION_QSPI_SDR   0xc0000004U

#include <mcuxClPsaDriver_Oracle_Macros.h>
#include <mcuxClPsaDriver_Oracle_Utils.h>

extern const key_recipe_t recipe_el2goconn_auth_prk;
#endif

#include <nxp_iot_agent.h>
#include <nxp_iot_agent_service.h>
#include <nxp_iot_agent_macros.h>

#if !(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)
#include <unistd.h>
#endif //!(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)

#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
#include <fsl_sss_api.h>
#include <nxp_iot_agent_keystore_sss_se05x.h>
#include <nxp_iot_agent_macros_sss.h>
#endif
#if defined(NXP_IOT_AGENT_HAVE_PSA) && (NXP_IOT_AGENT_HAVE_PSA == 1)
#include <nxp_iot_agent_macros_psa.h>
#if !(defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL == 1))
#ifdef __ZEPHYR__
#include <zephyr/drivers/hwinfo.h>
#else
#include <fsl_silicon_id.h>
#endif //__ZEPHYR__
#endif //#if !(defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL == 1))
#endif //#if defined(NXP_IOT_AGENT_HAVE_PSA) && (NXP_IOT_AGENT_HAVE_PSA == 1)

// AES CBC
#define AES_CBC_BLOCK_SIZE  		16

extern const pb_bytes_array_t* iot_agent_trusted_root_ca_certificates;

#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)

extern void print_openssl_errors(char* function);

//    everything stored in DER is big - endian) in front of
//    the magic number.
// kk is the keyclass(for SE050: 0x10 or 0x20);
// ii is the index(0x00, 0x01, 0x02, 0x03).
// In case no meaningful values can are known
// for kk or ii, insert 0x00 instead.
// 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// 00 00 k3 k2 k1 k0 A5 A6 B5 B6 A5 A6 B5 B6 kk ii
const uint8_t prime256v1_private_key_template[] = {
	0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA5, 0xA6, 0xB5, 0xB6, 0xA5, 0xA6, 0xB5, 0xB6, 0x00, 0x00 };

#define MAX_ECC_PRIV_KEY_TEMPLATE_LEN  (32)

#define MAX_ECC_PUB_KEY_BUFFER_LEN     (1 + 2 * 96)

typedef struct key_ref_ecc_template_t
{
	int nid;
	const uint8_t* private_key;
	size_t private_key_len;
	size_t private_key_offset_key_id;
	size_t private_key_offset_key_index;
	size_t private_key_offset_storage_class;
} key_ref_ecc_template_t;

// This CAN be extended if we ever need to support other curves.
const key_ref_ecc_template_t key_ref_ecc_templates[] = {
	{
		NID_X9_62_prime256v1,
		prime256v1_private_key_template,
		sizeof(prime256v1_private_key_template) / sizeof(prime256v1_private_key_template[0]),
	    18,
	    30,
	    31,
	},
};

static const key_ref_ecc_template_t* find_key_ref_ecc_template(int nid) {
	size_t i;
	for (i = 0U; i < sizeof(key_ref_ecc_templates) / sizeof(key_ref_ecc_templates[0]); i++)
	{
		if (key_ref_ecc_templates[i].nid == nid) {
			return &key_ref_ecc_templates[i];
		}
	}
	return NULL;
}

iot_agent_status_t iot_agent_utils_gen_key_ref_ecc(sss_key_store_t *keyStore, sss_object_t *keyObject,
	int nid, iot_agent_keystore_key_ref_t* gen_key_ref)
{
	// We use the buffer for the public key and the private key. Ensure that it fits both!
	COMPILE_TIME_ASSERT(MAX_ECC_PUB_KEY_BUFFER_LEN > MAX_ECC_PRIV_KEY_TEMPLATE_LEN);

	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	sss_status_t sss_status = kStatus_SSS_Success;

	const key_ref_ecc_template_t* key_ref_template = NULL;

	uint8_t buffer[MAX_ECC_PUB_KEY_BUFFER_LEN] = {0U};
	size_t buffer_size = sizeof(buffer);
	const uint8_t* buffer_ptr_const = buffer;
	uint8_t* buffer_ptr;

	size_t public_key_length_bits = 0U;

	BIGNUM *bn_priv = NULL;
#if (OPENSSL_VERSION_NUMBER < 0x30000000)
	EC_KEY *ec_key = NULL;
	int openssl_status = 1;
#else //if (OPENSSL_VERSION_NUMBER < 0x30000000)
	OSSL_PARAM_BLD *param_bld = NULL;
	BIGNUM *bn_pub = NULL;
#endif //if (OPENSSL_VERSION_NUMBER < 0x30000000)

	ASSERT_OR_EXIT_MSG(keyStore != NULL, "keyStore is NULL.");
	ASSERT_OR_EXIT_MSG(keyObject != NULL, "keyObject is NULL.");
	ASSERT_OR_EXIT_MSG(gen_key_ref != NULL, "pkey is NULL.");

	key_ref_template = find_key_ref_ecc_template(nid);
	ASSERT_OR_EXIT_MSG(key_ref_template != NULL, "find_key_ref_ecc_template returned NULL.");

	// ##############################
	// read the public key out from the SE:

	buffer_size = sizeof(buffer);
	sss_status = sss_key_store_get_key(keyStore, keyObject, buffer, &buffer_size, &public_key_length_bits);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_store_get_key failed: 0x%04x [object id: 0x%04x].", sss_status, keyObject->keyId);

	// ##############################
	// create key object from public key


	buffer_ptr_const = buffer;
	ASSERT_OR_EXIT_MSG(buffer_size <= LONG_MAX, "Error in casting of buffer size.");


#if (OPENSSL_VERSION_NUMBER < 0x30000000)
	ec_key = d2i_EC_PUBKEY(NULL, &buffer_ptr_const, (long)buffer_size);
	OPENSSL_ASSERT_OR_EXIT(ec_key != NULL, "d2i_EC_PUBKEY");
#else //if (OPENSSL_VERSION_NUMBER < 0x30000000)
	// in OpenSSL 3.x the function d2i_EC_PUBKEY is deprecated, created the
	// bignum instead
	bn_pub = BN_bin2bn(buffer_ptr_const, (int)buffer_size, NULL);
	OPENSSL_ASSERT_OR_EXIT_STATUS(bn_pub != NULL, "BN_bin2bn",
		IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED);
#endif //if (OPENSSL_VERSION_NUMBER < 0x30000000)

	// ##############################
	// replace the private key part with a key reference to the objectid:

	memcpy(buffer, key_ref_template->private_key, key_ref_template->private_key_len);
	buffer_ptr = buffer + key_ref_template->private_key_offset_key_id;
	*(buffer_ptr++) = (uint8_t)(keyObject->keyId >> 24) & 0xFFU;
	*(buffer_ptr++) = (uint8_t)(keyObject->keyId >> 16) & 0xFFU;
	*(buffer_ptr++) = (uint8_t)(keyObject->keyId >> 8) & 0xFFU;
	*(buffer_ptr) = (uint8_t)(keyObject->keyId >> 0) & 0xFFU;

	ASSERT_OR_EXIT_MSG(key_ref_template->private_key_len <= INT32_MAX, "Error in casting of key length.");
	bn_priv = BN_bin2bn(buffer, (int)key_ref_template->private_key_len, NULL);
	ASSERT_OR_EXIT_MSG(bn_priv != NULL, "Error in getting the private key");

#if (OPENSSL_VERSION_NUMBER < 0x30000000)
	openssl_status = EC_KEY_set_private_key(ec_key, bn_priv);
	OPENSSL_SUCCESS_OR_EXIT_STATUS("EC_KEY_set_private_key",
		IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED);

	// Make the key reference its curve via OID
	EC_KEY_set_asn1_flag(ec_key, nid);

	// Return the ECC key as part of the output parameter.
	openssl_status = EVP_PKEY_assign_EC_KEY(((EVP_PKEY*)gen_key_ref->key_ref), ec_key);
	OPENSSL_SUCCESS_OR_EXIT_STATUS("EVP_PKEY_assign_EC_KEY", IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED);
	ec_key = NULL; // pkey takes ownership of the key!;
#else //if (OPENSSL_VERSION_NUMBER < 0x30000000)
	// in OpenSSL 3.x the function EVP_PKEY_assign_EC_KEY is deprecated. The EVP_PKEY is created
	// using OSSL_PARAM APIs. The EVP_PKEY creation function will create a session with Se05x
	// from sssProvider, which will not be closed properly causing the TLS handshake to fail
	// when trying to compute ECDSA since in the meantime Se05x was used from the agent.
	// For this reason we stop here with creatiing the OSSL_PARAM from key reference and store it in the context,
	// the EVP_PKEY creation will be handled in the network configuration
	param_bld = OSSL_PARAM_BLD_new();
	ASSERT_OR_EXIT_MSG(param_bld != NULL, "Setup params failed");

	ASSERT_OR_EXIT_MSG(OSSL_PARAM_BLD_push_utf8_string(param_bld, "group", "prime256v1", 0) == 1,
		"Setup params failed");
	ASSERT_OR_EXIT_MSG(OSSL_PARAM_BLD_push_BN(param_bld, OSSL_PKEY_PARAM_PRIV_KEY, bn_priv) == 1,
		"Error pushing private key");
	ASSERT_OR_EXIT_MSG(OSSL_PARAM_BLD_push_BN(param_bld, OSSL_PKEY_PARAM_PUB_KEY, bn_pub) == 1,
		"Error pushing public key");

	gen_key_ref->key_ref = ((OSSL_PARAM*)OSSL_PARAM_BLD_to_param(param_bld));
	ASSERT_OR_EXIT_MSG(gen_key_ref->key_ref != NULL, "The network context is NULL");

#endif //if (OPENSSL_VERSION_NUMBER < 0x30000000)

exit:
	BN_free(bn_priv);
#if (OPENSSL_VERSION_NUMBER < 0x30000000)
	EC_KEY_free(ec_key);
#else //if (OPENSSL_VERSION_NUMBER < 0x30000000)
	OSSL_PARAM_BLD_free(param_bld);
#endif //if (OPENSSL_VERSION_NUMBER < 0x30000000)
	return agent_status;
}

#define MAX_RSA_2048_PUB_KEY_BUFFER_LEN   512
#define MAX_RSA_PUB_KEY_BUFFER_LEN        MAX_RSA_2048_PUB_KEY_BUFFER_LEN

iot_agent_status_t iot_agent_utils_gen_key_ref_rsa(sss_key_store_t *keyStore, sss_object_t *keyObject,
	iot_agent_keystore_key_ref_t* gen_key_ref)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	sss_status_t sss_status = kStatus_SSS_Success;
	int openssl_status = 1;

	uint8_t buffer[MAX_RSA_PUB_KEY_BUFFER_LEN] = {0U};
	size_t buffer_size = sizeof(buffer);
	const uint8_t* buffer_ptr_const = buffer;

	size_t public_key_length_bits = 0U;
    BIGNUM* d = NULL;
    BIGNUM* p = NULL;
    BIGNUM* q = NULL;
    BIGNUM* dmp1 = NULL;
    BIGNUM* dmq1 = NULL;
    BIGNUM* iqmp = NULL;
#if (OPENSSL_VERSION_NUMBER < 0x30000000)
	RSA* rsa_key = NULL;
#else //if (OPENSSL_VERSION_NUMBER < 0x30000000)
	OSSL_PARAM_BLD *param_bld = NULL;
	BIGNUM* n = NULL;
	BIGNUM* e = NULL;
	EVP_PKEY* rsa_pub_key = NULL;
#endif //if (OPENSSL_VERSION_NUMBER < 0x30000000)

	ASSERT_OR_EXIT_MSG(keyStore != NULL, "keyStore is NULL.");
	ASSERT_OR_EXIT_MSG(keyObject != NULL, "keyObject is NULL.");
	ASSERT_OR_EXIT_MSG(gen_key_ref != NULL, "pkey is NULL.");

	// ##############################
	// read the public key out from the SE:

	sss_status = sss_key_store_get_key(keyStore, keyObject, buffer, &buffer_size, &public_key_length_bits);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_store_get_key failed: 0x%04x [object id: 0x%04x].", sss_status, keyObject->keyId);

	// ##############################
	// create key object from public key

	buffer_ptr_const = buffer;
	ASSERT_OR_EXIT_MSG(buffer_size <= LONG_MAX, "Error in casting of buffer size.");

#if (OPENSSL_VERSION_NUMBER < 0x30000000)
	rsa_key = d2i_RSA_PUBKEY(NULL, &buffer_ptr_const, (long)buffer_size);
	OPENSSL_ASSERT_OR_EXIT(rsa_key != NULL, "d2i_RSA_PUBKEY");
#else //if (OPENSSL_VERSION_NUMBER < 0x30000000)
	rsa_pub_key = d2i_PUBKEY(NULL, &buffer_ptr_const, (long)buffer_size);
	ASSERT_OR_EXIT_MSG(rsa_pub_key != NULL, "d2i_RSA_PUBKEY");
#endif //if (OPENSSL_VERSION_NUMBER < 0x30000000)
	// ##############################
	// replace the private key part with a key reference to the objectid:

	// Set references in key object
	d = BN_new();
	p = BN_new();
	q = BN_new();
	dmp1 = BN_new();
	dmq1 = BN_new();
	iqmp = BN_new();

	openssl_status = BN_set_word(p, 0x01U);
	OPENSSL_SUCCESS_OR_EXIT("BN_set_word p");

	openssl_status = BN_set_word(q, (BN_ULONG)keyObject->keyId);
	OPENSSL_SUCCESS_OR_EXIT("BN_set_word q");

	openssl_status = BN_set_word(d, 0U);
	OPENSSL_SUCCESS_OR_EXIT("BN_set_word d");

	openssl_status = BN_set_word(dmp1, 0U);
	OPENSSL_SUCCESS_OR_EXIT("BN_set_word dmp1");

	openssl_status = BN_set_word(dmq1, 0U);
	OPENSSL_SUCCESS_OR_EXIT("BN_set_word dmq1");

	openssl_status = BN_set_word(iqmp, 0xA5A6B5B6U);
	OPENSSL_SUCCESS_OR_EXIT("BN_set_word iqmp");

#if (OPENSSL_VERSION_NUMBER < 0x30000000)
#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
	rsa_key->d = d;
	rsa_key->p = p;
	rsa_key->q = q;
	rsa_key->dmp1 = dmp1;
	rsa_key->dmq1 = dmq1;
	rsa_key->iqmp = iqmp;
#else //if(OPENSSL_VERSION_NUMBER < 0x10100000L)
	openssl_status = RSA_set0_factors(rsa_key, p, q);
	OPENSSL_SUCCESS_OR_EXIT("RSA_set0_factors");

	openssl_status = RSA_set0_crt_params(rsa_key, dmp1, dmq1, iqmp);
	OPENSSL_SUCCESS_OR_EXIT("RSA_set0_crt_params");

	openssl_status = RSA_set0_key(rsa_key, NULL, NULL, d);
	OPENSSL_SUCCESS_OR_EXIT("RSA_set0_key");
#endif //if(OPENSSL_VERSION_NUMBER < 0x10100000L)
	openssl_status = EVP_PKEY_assign_RSA(gen_key_ref->key_ref, rsa_key);
	OPENSSL_SUCCESS_OR_EXIT_STATUS("EVP_PKEY_assign_RSA_KEY", IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED);
	rsa_key = NULL; // pkey takes ownership of the key!;
#else
	n = BN_new();
	e = BN_new();
	ASSERT_OR_EXIT_MSG(EVP_PKEY_get_bn_param(rsa_pub_key, OSSL_PKEY_PARAM_RSA_N, &n) == 1,
		"Error in getting the RSA modulus");
	ASSERT_OR_EXIT_MSG(EVP_PKEY_get_bn_param(rsa_pub_key, OSSL_PKEY_PARAM_RSA_E, &e) == 1,
		"Error in getting the RSA modulus");
	// in OpenSSL 3.x the function EVP_PKEY_assign_RSA is deprecated. The EVP_PKEY is created
	// using OSSL_PARAM APIs. The EVP_PKEY creation function will create a session with Se05x
	// from sssProvider, which will not be closed properly causing the TLS handshake to fail
	// when trying to compute RSA signature since in the meantime Se05x was used from the agent.
	// For this reason we stop here with creatiing the OSSL_PARAM from key reference and store it in the context,
	// the EVP_PKEY creation will be handled in the network configuration
	param_bld = OSSL_PARAM_BLD_new();
	ASSERT_OR_EXIT_MSG(param_bld != NULL, "Setup params failed");
	ASSERT_OR_EXIT_MSG(OSSL_PARAM_BLD_push_BN(param_bld, OSSL_PKEY_PARAM_RSA_N, n) == 1,
		"Setup params failed");
	ASSERT_OR_EXIT_MSG(OSSL_PARAM_BLD_push_BN(param_bld, OSSL_PKEY_PARAM_RSA_E, e) == 1,
		"Setup params failed");
	ASSERT_OR_EXIT_MSG(OSSL_PARAM_BLD_push_BN(param_bld, OSSL_PKEY_PARAM_RSA_D, d) == 1,
		"Setup params failed");
	ASSERT_OR_EXIT_MSG(OSSL_PARAM_BLD_push_BN(param_bld, OSSL_PKEY_PARAM_RSA_FACTOR1, p) == 1,
		"Setup params failed");
	ASSERT_OR_EXIT_MSG(OSSL_PARAM_BLD_push_BN(param_bld, OSSL_PKEY_PARAM_RSA_FACTOR2, q) == 1,
		"Setup params failed");
	ASSERT_OR_EXIT_MSG(OSSL_PARAM_BLD_push_BN(param_bld, OSSL_PKEY_PARAM_RSA_EXPONENT1, dmp1) == 1,
		"Setup params failed");
	ASSERT_OR_EXIT_MSG(OSSL_PARAM_BLD_push_BN(param_bld, OSSL_PKEY_PARAM_RSA_EXPONENT2, dmq1) == 1,
		"Setup params failed");
	ASSERT_OR_EXIT_MSG(OSSL_PARAM_BLD_push_BN(param_bld, OSSL_PKEY_PARAM_RSA_COEFFICIENT1, iqmp) == 1,
		"Setup params failed");
	gen_key_ref->key_ref = ((OSSL_PARAM*)OSSL_PARAM_BLD_to_param(param_bld));
	ASSERT_OR_EXIT_MSG(gen_key_ref->key_ref != NULL, "The network context is NULL");
#endif //if (OPENSSL_VERSION_NUMBER < 0x30000000)

exit:
#if (OPENSSL_VERSION_NUMBER < 0x30000000)
	RSA_free(rsa_key);
#else //if (OPENSSL_VERSION_NUMBER < 0x30000000)
	EVP_PKEY_free(rsa_pub_key);
	OSSL_PARAM_BLD_free(param_bld);
#endif //if (OPENSSL_VERSION_NUMBER < 0x30000000)
	return agent_status;
}

static iot_agent_status_t iot_agent_utils_der_to_pem_bio(BIO *bio_in, BIO* bio_out)
{
	// this will decode the DER format certificate
	// and convert that into PEM formatted certificate.
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	int openssl_status;
	X509* certificate = NULL;

	ASSERT_OR_EXIT_MSG(bio_in != NULL, "bio_in is NULL.");
	ASSERT_OR_EXIT_MSG(bio_out != NULL, "bio_out is NULL.");

	while ((certificate = d2i_X509_bio(bio_in, NULL)) != NULL) {
		openssl_status = PEM_write_bio_X509(bio_out, certificate);
		OPENSSL_SUCCESS_OR_EXIT("PEM_write_bio_X509");
		X509_free(certificate);
		certificate = NULL;
	}
exit:
	X509_free(certificate);
	return agent_status;
}


iot_agent_status_t iot_agent_utils_get_certificate_common_name(iot_agent_context_t* ctx,
	const nxp_iot_ServiceDescriptor* service_descriptor, char* common_name, size_t max_size)
{
	uint32_t certificate_id = 0U;
	iot_agent_keystore_t* keystore = NULL;
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	uint8_t cert_buffer[NXP_IOT_AGENT_EDGELOCK2GO_CLIENT_CERTIFICATE_BUFFER_SIZE];
	size_t cert_len = sizeof(cert_buffer);
	BIO *bio_in_cert = NULL;
	X509_NAME* subject_name = NULL;
	X509* client_cert = NULL;

	ASSERT_OR_EXIT_MSG(ctx != NULL, "ctx is NULL.");
	ASSERT_OR_EXIT_MSG(service_descriptor != NULL, "service_descriptor is NULL.");
	ASSERT_OR_EXIT_MSG(common_name != NULL, "common_name is NULL.");

	certificate_id = service_descriptor->client_certificate_sss_ref.object_id;

	agent_status = iot_agent_utils_get_keystore_from_service_descriptor(ctx, service_descriptor, &keystore);
	AGENT_SUCCESS_OR_EXIT();

	agent_status = iot_agent_utils_get_certificate_from_keystore(keystore, certificate_id, cert_buffer, &cert_len);
	AGENT_SUCCESS_OR_EXIT();

	ASSERT_OR_EXIT_MSG(cert_len <= INT32_MAX, "Certificate lenght exceed max int value");

	bio_in_cert = BIO_new_mem_buf(cert_buffer, (int)cert_len);
	client_cert = d2i_X509_bio(bio_in_cert, NULL);
	if (client_cert == NULL) {
		BIO_free(bio_in_cert);
		X509_free(client_cert);
		EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "Error in loading the client certificate");
	}
	subject_name = X509_get_subject_name(client_cert);
	ASSERT_OR_EXIT_MSG(max_size <= INT32_MAX, "Certificate lenght exceed max int value");
	X509_NAME_get_text_by_NID(subject_name, NID_commonName, common_name, max_size);
	BIO_free(bio_in_cert);
	X509_free(client_cert);
exit:
	return agent_status;
}

#endif //#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)

iot_agent_status_t iot_agent_utils_gen_key_ref(const iot_agent_keystore_t* keystore, uint32_t key_id,
	iot_agent_keystore_key_ref_t* gen_key_ref)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)
	sss_status_t sss_status;
	sss_key_store_t* sss_keystore = NULL;
	sss_object_t key = { 0 };

	ASSERT_OR_EXIT_MSG(keystore != NULL, "keystore is NULL.");
	ASSERT_OR_EXIT_MSG(gen_key_ref != NULL, "gen_key_ref is NULL.");

	agent_status = iot_agent_keystore_sss_se05x_get_sss_key_store(keystore->context, &sss_keystore);
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_keystore_sss_se05x_get_sss_key_store failed: 0x%08x", agent_status);

	sss_status = sss_key_object_init(&key, sss_keystore);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_init failed: 0x%04x.", sss_status);

	sss_status = sss_key_object_get_handle(&key, key_id);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_get_handle failed: 0x%04x [object id: 0x%04x].", sss_status, key.keyId);

	switch (key.cipherType)
	{
		case kSSS_CipherType_EC_NIST_P:
			agent_status = iot_agent_utils_gen_key_ref_ecc(sss_keystore, &key, NID_X9_62_prime256v1, gen_key_ref);
			AGENT_SUCCESS_OR_EXIT();
			break;
		case kSSS_CipherType_RSA:
		case kSSS_CipherType_RSA_CRT:
			agent_status = iot_agent_utils_gen_key_ref_rsa(sss_keystore, &key, gen_key_ref);
			AGENT_SUCCESS_OR_EXIT();
			break;
		default:
			IOT_AGENT_ERROR("Unsupported key cipherType [%u]", key.cipherType);
			agent_status = IOT_AGENT_FAILURE;
			break;
	}

exit:
#endif //#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)
	return agent_status;
}

iot_agent_status_t iot_agent_utils_write_key_ref_pem(iot_agent_keystore_key_ref_t* gen_key_ref, const char* filename)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)
	int openssl_status;
	BIO* out = NULL;
	EVP_PKEY* pkey_to_write = NULL;

	ASSERT_OR_EXIT_MSG(filename != NULL, "filename is NULL.");

	ASSERT_OR_EXIT_MSG((strstr(filename, "..")) == NULL, "Filename is not allowed");

	out = BIO_new_file(filename, "w");
	ASSERT_OR_EXIT_STATUS(out != NULL, IOT_AGENT_ERROR_FILE_SYSTEM);

	pkey_to_write = EVP_PKEY_new();
	OPENSSL_ASSERT_OR_EXIT_STATUS(pkey_to_write != NULL, "EVP_PKEY_new", IOT_AGENT_FAILURE);

#if (OPENSSL_VERSION_NUMBER < 0x30000000)
	pkey_to_write = gen_key_ref->key_ref;
#else

	EVP_PKEY_CTX* key_ctx = NULL;
	key_ctx = EVP_PKEY_CTX_new_from_name(NULL, "EC", "provider=nxp_prov");
	ASSERT_OR_EXIT_MSG(key_ctx != NULL, "Error getting context");

	ASSERT_OR_EXIT_MSG(EVP_PKEY_fromdata_init(key_ctx) == 1, "Error initializing key");

	ASSERT_OR_EXIT_MSG(EVP_PKEY_fromdata(key_ctx, &pkey_to_write, EVP_PKEY_KEYPAIR, gen_key_ref->key_ref) == 1,
		"Error getting  key from data");
#endif
	openssl_status = PEM_write_bio_PrivateKey(out, pkey_to_write, NULL, NULL, 0, NULL, NULL);
	OPENSSL_SUCCESS_OR_EXIT_STATUS("PEM_write_PrivateKey", IOT_AGENT_ERROR_FILE_SYSTEM);

exit:
	EVP_PKEY_free(pkey_to_write);
	BIO_free(out);
#endif //#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)
	return agent_status;
}

iot_agent_status_t iot_agent_utils_write_key_ref_pem_from_keystore(iot_agent_keystore_t *keyStore,
	const uint32_t keyid, const char* filename)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
#if (defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)) && (OPENSSL_VERSION_NUMBER < 0x30000000)
	ASSERT_OR_EXIT_MSG(keyStore != NULL, "Keystore context is NULL");
	ASSERT_OR_EXIT_MSG(filename != NULL, "Filename is NULL");

	iot_agent_keystore_key_ref_t gen_key_ref = { 0 };
	gen_key_ref.key_ref = ((EVP_PKEY*)EVP_PKEY_new());

	agent_status = iot_agent_utils_gen_key_ref(keyStore, keyid, &gen_key_ref);
	AGENT_SUCCESS_OR_EXIT();

	agent_status = iot_agent_utils_write_key_ref_pem(&gen_key_ref, filename);
	AGENT_SUCCESS_OR_EXIT();
exit:
#endif
	return agent_status;
}

iot_agent_status_t iot_agent_utils_write_certificate_pem(uint8_t* buffer, size_t len, const char* filename)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)
	int openssl_status;

	BIO *bio_in = NULL;
	BIO * bio_out = NULL;

	ASSERT_OR_EXIT_MSG(buffer != NULL, "buffer is NULL.");
	ASSERT_OR_EXIT_MSG(filename != NULL, "filename is NULL.");

	ASSERT_OR_EXIT_MSG(len <= INT32_MAX, "Error in length casting.");
	bio_in = BIO_new_mem_buf(&buffer[0], (int)len);
	OPENSSL_ASSERT_OR_EXIT(bio_in != NULL, "BIO_new_mem_buf");

	/* Open the PEM file for writing the certificate to disk. */
	ASSERT_OR_EXIT_MSG((strstr(filename, "..")) == NULL, "Filename is not allowed");

	bio_out = BIO_new_file(filename, "w");
	OPENSSL_ASSERT_OR_EXIT_STATUS(bio_out != NULL, "BIO_new_file", IOT_AGENT_ERROR_FILE_SYSTEM);

	agent_status = iot_agent_utils_der_to_pem_bio(bio_in, bio_out);
	AGENT_SUCCESS_OR_EXIT();

	openssl_status = BIO_flush(bio_out);
	OPENSSL_SUCCESS_OR_EXIT("BIO_flush");

exit:
	BIO_free(bio_out);
	BIO_free(bio_in);
#endif
	return agent_status;
}

iot_agent_status_t iot_agent_utils_write_certificate_pem_from_keystore(iot_agent_keystore_t *keyStore,
	const uint32_t certid, const char* filename)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	ASSERT_OR_EXIT_MSG(keyStore != NULL, "keyStore is NULL.");
	ASSERT_OR_EXIT_MSG(filename != NULL, "filename is NULL.");

#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)
	uint8_t cert_buffer[NXP_IOT_AGENT_EDGELOCK2GO_CLIENT_CERTIFICATE_BUFFER_SIZE];
	size_t cert_len = sizeof(cert_buffer);

	agent_status = iot_agent_utils_get_certificate_from_keystore(keyStore, certid, cert_buffer, &cert_len);
	AGENT_SUCCESS_OR_EXIT();

	ASSERT_OR_EXIT_MSG((strstr(filename, "..")) == NULL, "Filename is not allowed");

	agent_status = iot_agent_utils_write_certificate_pem(cert_buffer, cert_len, filename);
	AGENT_SUCCESS_OR_EXIT();

#endif
exit:
	return agent_status;
}

iot_agent_status_t iot_agent_utils_write_key_ref_service_pem(const iot_agent_context_t* ctx,
	const char* filename)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
#if (defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)) && (OPENSSL_VERSION_NUMBER < 0x30000000)
	nxp_iot_ServiceDescriptor service_descriptor = { 0U };
	iot_agent_keystore_t* keystore = NULL;
    uint32_t keystore_id = 0U;
	uint32_t key_id = 0U;
	iot_agent_keystore_key_ref_t gen_key_ref = { 0 };
	gen_key_ref.key_ref = ((EVP_PKEY*)EVP_PKEY_new());

	agent_status = iot_agent_get_service_descriptor(ctx, &service_descriptor);
	AGENT_SUCCESS_OR_EXIT();

	ASSERT_OR_EXIT_MSG(ctx != NULL, "ctx is NULL.");
	ASSERT_OR_EXIT_MSG(filename != NULL, "filename is NULL.");

	keystore_id = service_descriptor.client_key_sss_ref.endpoint_id;
	agent_status = iot_agent_get_keystore_by_id(ctx, keystore_id, &keystore);
	AGENT_SUCCESS_OR_EXIT();

	agent_status = iot_agent_utils_convert_service2key_id(service_descriptor.identifier, &key_id);
	AGENT_SUCCESS_OR_EXIT();

	ASSERT_OR_EXIT_MSG((strstr(filename, "..")) == NULL, "Filename is not allowed");

	agent_status = iot_agent_utils_gen_key_ref(keystore, key_id, &gen_key_ref);
	AGENT_SUCCESS_OR_EXIT();

	agent_status = iot_agent_utils_write_key_ref_pem(&gen_key_ref, filename);
	AGENT_SUCCESS_OR_EXIT();
exit:
	iot_agent_free_service_descriptor(&service_descriptor);
#endif
	return agent_status;
}

iot_agent_status_t iot_agent_utils_get_keystore_from_service_descriptor(const iot_agent_context_t* ctx,
	const nxp_iot_ServiceDescriptor* service_descriptor, iot_agent_keystore_t** keystore)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
	uint32_t keystore_id = 0U;
	ASSERT_OR_EXIT_MSG(ctx != NULL, "ctx is NULL.");
	ASSERT_OR_EXIT_MSG(service_descriptor != NULL, "service_descriptor is NULL.");
	ASSERT_OR_EXIT_MSG(keystore != NULL, "keystore is NULL.");

	keystore_id = service_descriptor->client_certificate_sss_ref.endpoint_id;

	agent_status = iot_agent_get_keystore_by_id(ctx, keystore_id, keystore);
	AGENT_SUCCESS_OR_EXIT();
exit:
#endif //#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
	return agent_status;
}

iot_agent_status_t iot_agent_utils_write_certificate_pem_cos_over_rtp(iot_agent_context_t* ctx,
	const nxp_iot_ServiceDescriptor* service_descriptor, const char* filename)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)
	uint32_t certificate_id = 0U;
	iot_agent_keystore_t* keystore = NULL;
	uint8_t cert_buffer[NXP_IOT_AGENT_EDGELOCK2GO_CLIENT_CERTIFICATE_BUFFER_SIZE];
	size_t cert_len = sizeof(cert_buffer);

	ASSERT_OR_EXIT_MSG(ctx != NULL, "ctx is NULL.");
	ASSERT_OR_EXIT_MSG(service_descriptor != NULL, "service_descriptor is NULL.");
	ASSERT_OR_EXIT_MSG(filename != NULL, "filename is NULL.");

	certificate_id = service_descriptor->client_certificate_sss_ref.object_id;

	agent_status = iot_agent_utils_get_keystore_from_service_descriptor(ctx, service_descriptor, &keystore);
	AGENT_SUCCESS_OR_EXIT();

	agent_status = iot_agent_utils_get_certificate_from_keystore(keystore, certificate_id, cert_buffer, &cert_len);
	AGENT_SUCCESS_OR_EXIT();

	ASSERT_OR_EXIT_MSG((strstr(filename, "..")) == NULL, "Filename is not allowed");

	agent_status = iot_agent_utils_write_certificate_pem(cert_buffer, cert_len, filename);
	AGENT_SUCCESS_OR_EXIT();
exit:
#endif
	return agent_status;
}

#if	(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)

static iot_agent_status_t iot_agent_utils_get_oid_value_in_subject(uint8_t* cert_buffer, size_t cert_len,
	char* oid, char* value, size_t max_size)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	mbedtls_x509_crt client_cert = {0};
	char* oid_name = NULL;
	mbedtls_x509_name* oid_ptr;
	size_t len = 0;
	bool oid_found = false;

	ASSERT_OR_EXIT_MSG(cert_buffer != NULL, "cert_buffer is NULL.");
	ASSERT_OR_EXIT_MSG(oid != NULL, "oid is NULL.");
	ASSERT_OR_EXIT_MSG(value != NULL, "value is NULL.");

	mbedtls_x509_crt_init(&client_cert);

	if (mbedtls_x509_crt_parse_der(&client_cert, cert_buffer, cert_len) != 0)
	{
		EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "Error in loading the client certificate");
	}

	oid_ptr = &client_cert.subject;

	while (oid_ptr != NULL)
	{
		mbedtls_oid_get_attr_short_name((mbedtls_asn1_buf*)oid_ptr, (const char**)&oid_name);
		if(strcmp(oid_name, oid) == 0)
		{
			len = oid_ptr->val.len;
			if (len > max_size)
			{
				EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "Error in loading the client certificate");
			}
			for(size_t i = 0U; i < len; i++)
			{
			  	ASSERT_OR_EXIT_MSG((oid_ptr->val.p[i] <= INT8_MAX), "Wrapparound in assigning");
				*(value + i) = oid_ptr->val.p[i];
			}
			oid_found = true;
		}

		oid_ptr = oid_ptr->next;
	}

	if (!oid_found)
	{
		EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "Unable to find the OID");
	}
exit:
	mbedtls_x509_crt_free(&client_cert);
	return agent_status;
}

iot_agent_status_t iot_agent_utils_get_certificate_common_name(iot_agent_context_t* ctx,
	const nxp_iot_ServiceDescriptor* service_descriptor, char* common_name, size_t max_size)
{
	uint32_t certificate_id = 0U;
#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
	iot_agent_keystore_t* keystore = NULL;
#endif
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	uint8_t cert_buffer[NXP_IOT_AGENT_EDGELOCK2GO_CLIENT_CERTIFICATE_BUFFER_SIZE];
	size_t cert_len = sizeof(cert_buffer);

	ASSERT_OR_EXIT_MSG(ctx != NULL, "ctx is NULL.");
	ASSERT_OR_EXIT_MSG(service_descriptor != NULL, "service_descriptor is NULL.");
	ASSERT_OR_EXIT_MSG(common_name != NULL, "common_name is NULL.");

	certificate_id = service_descriptor->client_certificate_sss_ref.object_id;
#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
	agent_status = iot_agent_utils_get_keystore_from_service_descriptor(ctx, service_descriptor, &keystore);
	AGENT_SUCCESS_OR_EXIT();

	agent_status = iot_agent_utils_get_certificate_from_keystore(keystore, certificate_id, cert_buffer, &cert_len);
	AGENT_SUCCESS_OR_EXIT();
#endif
#if defined(NXP_IOT_AGENT_HAVE_PSA) && (NXP_IOT_AGENT_HAVE_PSA == 1)
	psa_status_t psa_status = PSA_SUCCESS;
	psa_status = psa_export_key(certificate_id, cert_buffer, sizeof(cert_buffer), &cert_len);
    PSA_SUCCESS_OR_EXIT_MSG("Error in esporting client certificate");
#endif
	agent_status = iot_agent_utils_get_oid_value_in_subject(cert_buffer, cert_len, "CN", common_name, max_size);
	AGENT_SUCCESS_OR_EXIT();

exit:
	return agent_status;
}
#endif //#if (AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)

iot_agent_status_t iot_agent_utils_get_certificate_from_keystore(iot_agent_keystore_t* keystore,
	uint32_t certificate_id, uint8_t* cert, size_t* cert_len)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
	sss_status_t sss_status;
	sss_object_t certObj = { 0 };
	size_t cert_lenBits = 0U;

	ASSERT_OR_EXIT_MSG(keystore != NULL, "keyStore is NULL.");
	ASSERT_OR_EXIT_MSG(cert != NULL, "cert is NULL.");
	ASSERT_OR_EXIT_MSG(cert_len != NULL, "cert_len is NULL.");
	ASSERT_OR_EXIT_MSG(keystore->type == IOT_AGENT_KS_SSS_SE05X, "keystore type %d is unsupported.", keystore->type);

	ASSERT_OR_EXIT_MSG(*cert_len <= ((SIZE_MAX / 8U) - 1U), "Wraparound in length of bits.");
	cert_lenBits = (*cert_len) * 8U;
	sss_key_store_t* sss_key_store = NULL;

	agent_status = iot_agent_keystore_sss_se05x_get_sss_key_store(keystore->context, &sss_key_store);
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_keystore_sss_se05x_get_sss_key_store failed: 0x%08x", agent_status);

	sss_status = sss_key_object_init(&certObj, sss_key_store);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_init failed: 0x%08x.", sss_status);

	sss_status = sss_key_object_get_handle(&certObj, certificate_id);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_get_handle failed: 0x%08x.", sss_status);

	// The API needs to be passed a "size in bits" pointer. Does
	// not make any sense for certificates, but not all keystore types accept NULL,
	sss_status = sss_key_store_get_key(sss_key_store, &certObj, cert, cert_len, &cert_lenBits);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_store_get_key failed: 0x%08x.", sss_status);
exit:
	sss_key_object_free(&certObj);
#endif
	return agent_status;
}

iot_agent_status_t iot_agent_get_first_found_object(iot_agent_keystore_t* keystore,
	uint32_t* object_ids, size_t num_objects, uint32_t* object_id)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
	sss_status_t sss_status;
	sss_object_t sss_object = { 0 };
	sss_key_store_t* sss_key_store = NULL;

	ASSERT_OR_EXIT_MSG(keystore != NULL, "keystore is NULL.");
	ASSERT_OR_EXIT_MSG(object_ids != NULL, "object_ids is NULL.");
	ASSERT_OR_EXIT_MSG(object_id != NULL, "object_id is NULL.");
	ASSERT_OR_EXIT_MSG(keystore->type == IOT_AGENT_KS_SSS_SE05X, "keystore type %d is unsupported.", keystore->type);

	agent_status = iot_agent_keystore_sss_se05x_get_sss_key_store(keystore->context, &sss_key_store);
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_keystore_sss_se05x_get_sss_key_store failed: 0x%08x", agent_status);

	sss_status = sss_key_object_init(&sss_object, sss_key_store);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_init failed: 0x%08x.", sss_status);

	for (size_t i = 0U; i < num_objects; i++) {
		*object_id = object_ids[i];
		sss_status = sss_key_object_get_handle(&sss_object, *object_id);
		if (sss_status == (sss_status_t)kStatus_SSS_Success) {
			break;
		}
	}
exit:
	sss_key_object_free(&sss_object);
#endif
	return agent_status;
}

iot_agent_status_t iot_agent_utils_get_edgelock2go_key_id(iot_agent_keystore_t* keystore, uint32_t* object_id)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	if (false) {}
#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
	else if (keystore->type == IOT_AGENT_KS_SSS_SE05X) {

		uint32_t object_ids[] = { EDGELOCK2GO_KEYID_ECC, EDGELOCK2GO_KEYID_RSA };
		size_t num_objects = sizeof(object_ids) / sizeof(object_ids[0]);

		ASSERT_OR_EXIT_MSG(keystore != NULL, "keystore is NULL.");
		ASSERT_OR_EXIT_MSG(object_id != NULL, "object_id is NULL.");

		agent_status = iot_agent_get_first_found_object(keystore, object_ids, num_objects, object_id);
		AGENT_SUCCESS_OR_EXIT_MSG("Neither ECC nor RSA key found.");
	}
#endif
#if defined(NXP_IOT_AGENT_HAVE_PSA) && (NXP_IOT_AGENT_HAVE_PSA == 1)
	else if (keystore->type == IOT_AGENT_KS_PSA) {
		*object_id = EDGELOCK2GO_KEYID_ECC;
	}
#endif
	else {}
#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
	exit:
#endif
	return agent_status;
}


iot_agent_status_t iot_agent_utils_get_edgelock2go_certificate_id(iot_agent_keystore_t* keystore, uint32_t* object_id)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	if (false) {}
#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
	else if (keystore->type == IOT_AGENT_KS_SSS_SE05X) {
		uint32_t object_ids[] = { EDGELOCK2GO_CERTID_ECC, EDGELOCK2GO_CERTID_RSA };
		size_t num_objects = sizeof(object_ids) / sizeof(object_ids[0]);

		ASSERT_OR_EXIT_MSG(keystore != NULL, "keystore is NULL.");
		ASSERT_OR_EXIT_MSG(object_id != NULL, "object_id is NULL.");

		agent_status = iot_agent_get_first_found_object(keystore, object_ids, num_objects, object_id);
		AGENT_SUCCESS_OR_EXIT_MSG("Neither ECC nor RSA certificate found.");
	}
#endif
#if defined(NXP_IOT_AGENT_HAVE_PSA) && (NXP_IOT_AGENT_HAVE_PSA == 1)
	else {
		// set status as failure, but without message
		agent_status = IOT_AGENT_FAILURE;
		goto exit;
	}
#endif
exit:
	return agent_status;
}

#if defined(NXP_IOT_AGENT_HAVE_PSA) && (NXP_IOT_AGENT_HAVE_PSA == 1)

iot_agent_status_t iot_agent_utils_get_device_id(uint8_t* buffer, size_t* len) {
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

#if defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL == 1)
	psa_status_t psa_status = PSA_SUCCESS;
	// Fake for simulators, the UID in fact is in ITS.
	ASSERT_OR_EXIT_MSG(*len >= DEVICEID_LENGTH, "buffer is too small for device id, %u bytes are required.", DEVICEID_LENGTH);

	psa_status = psa_its_get(DEVICEID_KEYID, 0U, DEVICEID_LENGTH, buffer, len);
	ASSERT_OR_EXIT_MSG(psa_status == PSA_SUCCESS, "psa_its_get failed: 0x%08x", psa_status);
#elif defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM == 1)
	// Devices do have an ID which can be retrieved:
#ifdef __ZEPHYR__
	ASSERT_OR_EXIT_MSG(*len >= DEVICEID_LENGTH, "buffer is too small for device id, %u bytes are required.", DEVICEID_LENGTH);
	ASSERT_OR_EXIT_MSG(hwinfo_get_device_id(buffer, *len) == DEVICEID_LENGTH, "Error in getting the device ID");
	*len = DEVICEID_LENGTH;
#else
	ASSERT_OR_EXIT_MSG(SILICONID_GetID(buffer, (uint32_t*)len) == kStatus_Success, "Error in getting the device ID");
#endif
#else
	agent_status = read_device_uuid(buffer, len);
#endif //#if defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL == 1)

exit:
	return agent_status;
}
#endif


iot_agent_status_t iot_agent_utils_create_self_signed_edgelock2go_certificate(
	iot_agent_keystore_t* keystore, uint8_t* certificate_buffer,
	size_t* certificate_buffer_size)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
#if defined(NXP_IOT_AGENT_HAVE_PSA) && (NXP_IOT_AGENT_HAVE_PSA == 1)
	int ret = 1;
	mbedtls_pk_context loaded_issuer_key;
	const char* issuer_prefix = "O=NXP,OU=Plug and Trust,CN=NXP_DIE_ID_AUTH,serialNumber=";
	char issuer_name[256] = { 0 };
	uint8_t uuid[20] = { 0U };
	mbedtls_md_type_t md_alg = MBEDTLS_MD_SHA256;
	const uint8_t serial_bytes[1] = { 1U };

	mbedtls_x509write_cert crt;
	mbedtls_mpi serial;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	const char *pers = "edglock2go_self_signed_cert";
	char* pos = issuer_name + strlen(issuer_prefix);
	size_t uuid_len = 0U;

	mbedtls_x509write_crt_init(&crt);
	mbedtls_pk_init(&loaded_issuer_key);
	mbedtls_mpi_init(&serial);
	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);

	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
		(const unsigned char *)pers,
		strlen(pers));
	ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_ctr_drbg_seed failed: 0x%x", ret);

	uuid_len = sizeof(uuid);
	agent_status = iot_agent_utils_get_device_id(uuid, &uuid_len);
	ASSERT_OR_EXIT_MSG(agent_status == IOT_AGENT_SUCCESS, "iot_agent_utils_get_device_id failed with 0x%08x", agent_status);

	COMPILE_TIME_ASSERT(sizeof(issuer_name) > sizeof(*issuer_prefix) + sizeof(uuid) * 2);
	strcpy(issuer_name, issuer_prefix);
	for (size_t i = 0U; i < uuid_len; i++) {
		sprintf(pos, "%02X", uuid[i]);
		pos += 2;
	}

#ifdef NXP_IOT_AGENT_ENABLE_LITE
	psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
	psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
	psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
	psa_set_key_bits(&attributes, 256);
	psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE | PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH);
	psa_set_key_lifetime(&attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT, PSA_KEY_LOCATION_S50_KEY_GEN_STORAGE));
	psa_set_key_id(&attributes, EDGELOCK2GO_KEYID_ECC);

	psa_key_id_t id = 0U;
	psa_status_t psa_status = psa_import_key(&attributes, (uint8_t const *)&recipe_el2goconn_auth_prk, mcuxClPsaDriver_Oracle_Utils_GetRecipeSize(&recipe_el2goconn_auth_prk), &id);
	ASSERT_OR_EXIT_MSG(psa_status == PSA_SUCCESS || psa_status == PSA_ERROR_ALREADY_EXISTS, "psa_import_key failed: 0x%x", psa_status);
#endif

	ret = mbedtls_pk_setup_opaque(&loaded_issuer_key, EDGELOCK2GO_KEYID_ECC);
	ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_pk_setup_opaque failed: 0x%x", ret);

	mbedtls_x509write_crt_set_subject_key(&crt, &loaded_issuer_key);
	mbedtls_x509write_crt_set_issuer_key(&crt, &loaded_issuer_key);

	ret = mbedtls_x509write_crt_set_issuer_name(&crt, issuer_name);
	ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_x509write_crt_set_issuer_name failed with 0x%08x", ret);

	ret = mbedtls_x509write_crt_set_subject_name(&crt, issuer_name);
	ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_x509write_crt_set_subject_name failed with 0x%08x", ret);

	mbedtls_x509write_crt_set_version(&crt, MBEDTLS_X509_CRT_VERSION_3);

	if (mbedtls_pk_get_bitlen(&loaded_issuer_key) == 384U)
	{
		md_alg = MBEDTLS_MD_SHA384;
	}
	mbedtls_x509write_crt_set_md_alg(&crt, md_alg);

	ret = mbedtls_mpi_read_binary(&serial, serial_bytes, sizeof(serial_bytes));
	ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_mpi_read_binary failed with 0x%08x", ret);
	ret = mbedtls_x509write_crt_set_serial(&crt, &serial);
	ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_x509write_crt_set_serial failed with 0x%08x", ret);

	ret = mbedtls_x509write_crt_set_validity(&crt, "20220101000000", "99991231235959");
	ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_x509write_crt_set_validity failed with 0x%08x", ret);

	// if the usage or other extension fields are not set, mbedTLS generates an extension field with 0 lenght
	// which is a not valid option. at least one of the extansions must be present
	mbedtls_x509write_crt_set_key_usage(&crt, MBEDTLS_X509_KU_DIGITAL_SIGNATURE);
	ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_x509write_crt_set_key_usage failed with 0x%08x", ret);

	// Note: mbedtls_x509write_crt_der writes the cert to the end of the buffer!
	ret = mbedtls_x509write_crt_der(&crt, certificate_buffer, *certificate_buffer_size,
		mbedtls_ctr_drbg_random, &ctr_drbg);
	ASSERT_OR_EXIT_MSG(ret > 0, "mbedtls_x509write_crt_der failed with 0x%08x", ret);
	memmove(certificate_buffer, certificate_buffer + *certificate_buffer_size - ret, ret);
	*certificate_buffer_size = ret;

exit:
	mbedtls_x509write_crt_free(&crt);
	mbedtls_pk_free(&loaded_issuer_key);
	mbedtls_mpi_free(&serial);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
#endif
	return agent_status;
}

iot_agent_status_t iot_agent_utils_write_edgelock2go_datastore(iot_agent_keystore_t *keystore,
	iot_agent_datastore_t* datastore, const char* hostname, uint32_t port,
	const pb_bytes_array_t* trusted_root_ca_certificates, const pb_bytes_array_t* client_certificate)
{

	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	nxp_iot_ServiceDescriptor service_descriptor = nxp_iot_ServiceDescriptor_init_default;


	uint32_t object_id = 0U;
	uint8_t* buffer = NULL;

    pb_ostream_t stream = PB_OSTREAM_SIZING;
    bool pb_status = false;
    size_t encoded_size = 0U;
    size_t total_size = 0U;
    pb_ostream_t ostream = { 0 };
    configuration_data_header_t* header = NULL;
#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)
    int success = 1;
    unsigned int calculted_checksum_len = 0U;
    EVP_MD_CTX* digest_context = NULL;
#elif defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS == 1)
    int failed = 0;
#endif
#if defined(NXP_IOT_AGENT_HAVE_PSA) && (NXP_IOT_AGENT_HAVE_PSA == 1)
	PB_BYTES_ARRAY_T(1024) client_certificate_buffer = { 0 };
#endif

	ASSERT_OR_EXIT_MSG(keystore != NULL, "keystore is NULL.");
	ASSERT_OR_EXIT_MSG(datastore != NULL, "datastore is NULL.");
	ASSERT_OR_EXIT_MSG(hostname != NULL, "hostname is NULL.");
	ASSERT_OR_EXIT_MSG(trusted_root_ca_certificates != NULL, "trusted_root_ca_certificates is NULL.");

	service_descriptor.hostname = (char*)hostname;
	service_descriptor.has_port = true;
	service_descriptor.port = port;

	service_descriptor.has_client_key_sss_ref = true;
	service_descriptor.client_key_sss_ref.has_type = true;
	ASSERT_OR_EXIT_MSG(keystore->type <= _nxp_iot_EndpointType_MAX, "Error in keystore type");
	service_descriptor.client_key_sss_ref.type = (nxp_iot_EndpointType)keystore->type;
	service_descriptor.client_key_sss_ref.has_endpoint_id = true;
	service_descriptor.client_key_sss_ref.endpoint_id = keystore->identifier;
	service_descriptor.client_key_sss_ref.has_object_id = true;
	agent_status = iot_agent_utils_get_edgelock2go_key_id(keystore, &object_id);
	service_descriptor.client_key_sss_ref.object_id = object_id;
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_utils_get_edgelock2go_key_id failed: 0x%08x", agent_status);

	if (client_certificate == NULL) {
		agent_status = iot_agent_utils_get_edgelock2go_certificate_id(keystore, &object_id);
		if (agent_status == IOT_AGENT_SUCCESS) {
			service_descriptor.has_client_certificate_sss_ref = true;
			service_descriptor.client_certificate_sss_ref.has_type = true;
			ASSERT_OR_EXIT_MSG(keystore->type <= _nxp_iot_EndpointType_MAX, "Error in keystore type");
			service_descriptor.client_certificate_sss_ref.type = (nxp_iot_EndpointType)keystore->type;
			service_descriptor.client_certificate_sss_ref.has_endpoint_id = true;
			service_descriptor.client_certificate_sss_ref.endpoint_id = keystore->identifier;
			service_descriptor.client_certificate_sss_ref.has_object_id = true;
			service_descriptor.client_certificate_sss_ref.object_id = object_id;
		}
#if defined(NXP_IOT_AGENT_HAVE_PSA) && (NXP_IOT_AGENT_HAVE_PSA == 1)
        else {
			size_t certificate_buffer_size = sizeof(client_certificate_buffer.bytes);
            agent_status = iot_agent_utils_create_self_signed_edgelock2go_certificate(keystore,
                    client_certificate_buffer.bytes, &certificate_buffer_size);
            AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_utils_create_self_signed_edgelock2go_certificate failed: 0x%08x", agent_status);
			client_certificate_buffer.size = certificate_buffer_size;
			service_descriptor.client_certificate = (pb_bytes_array_t*) &client_certificate_buffer;
		}
#endif
	}
	else {
		service_descriptor.client_certificate = (pb_bytes_array_t*) client_certificate;
	}

#if defined(NXP_IOT_AGENT_VERIFY_EDGELOCK_2GO_SERVER_CERTIFICATE) && (NXP_IOT_AGENT_VERIFY_EDGELOCK_2GO_SERVER_CERTIFICATE == 1)
	// Put the server certificates into the service descriptor.
	service_descriptor.server_certificate = (pb_bytes_array_t*) trusted_root_ca_certificates;
#endif

	// Serialize the service descriptor.
	pb_status = pb_encode_delimited(&stream, nxp_iot_ServiceDescriptor_fields, &service_descriptor);
	ASSERT_OR_EXIT_MSG(pb_status == true, "pb_encode_delimited for getting size failed.");
	encoded_size = stream.bytes_written;

	ASSERT_OR_EXIT_MSG(encoded_size <= (SIZE_MAX - sizeof(configuration_data_header_t)), "Wraparound in the total size calculation.");
	total_size = sizeof(configuration_data_header_t) + encoded_size;
	buffer = malloc(total_size);
	ASSERT_OR_EXIT_MSG(buffer != NULL, "Allocated buffer is NULL.");
	ostream = pb_ostream_from_buffer(buffer + sizeof(configuration_data_header_t), encoded_size);
	pb_status = pb_encode_delimited(&ostream, nxp_iot_ServiceDescriptor_fields, &service_descriptor);
	ASSERT_OR_EXIT_MSG(pb_status == true, "pb_encode_delimited failed.");

	// Create a header.
	union {
		uint8_t* pui8;
		configuration_data_header_t* pheader;
	}u_access;

	u_access.pui8 = buffer;
	header = u_access.pheader;
	memset(header, 0, sizeof(*header));
	header->length = (uint32_t)total_size;
	header->number_of_services = 1U;
	header->version = IOT_AGENT_CONFIGURATION_DATA_VERSION;

#if defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL == 1)
#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
	digest_context = EVP_MD_CTX_create();
#else
	digest_context = EVP_MD_CTX_new();
#endif
	ASSERT_OR_EXIT_MSG(digest_context != NULL, "EVP_MD_CTX_new returned NULL.");
	success &= EVP_DigestInit_ex(digest_context, EVP_sha256(), NULL);
	success &= EVP_DigestUpdate(digest_context, &buffer[sizeof(header->checksum)], total_size - sizeof(header->checksum));
	calculted_checksum_len = (unsigned int)sizeof(header->checksum);
	success &= EVP_DigestFinal_ex(digest_context, header->checksum, &calculted_checksum_len);
	ASSERT_OR_EXIT_MSG(success == 1, "Header checksum calculation failed.");
#elif defined(NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS) && (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS == 1)
	ASSERT_OR_EXIT_MSG(total_size >= sizeof(header->checksum), "Buffer overflow in SHA calculation.");
	mbedtls_sha256_context digest_context;
	mbedtls_sha256_init(&digest_context);
#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER < 0x03010000)
	mbedtls_sha256_starts_ret(&digest_context, 0U);
	failed |= mbedtls_sha256_update_ret(&digest_context, &buffer[sizeof(header->checksum)], total_size - sizeof(header->checksum));
	failed |= mbedtls_sha256_finish_ret(&digest_context, header->checksum);
#else
	mbedtls_sha256_starts(&digest_context, 0);
	failed |= mbedtls_sha256_update(&digest_context, &buffer[sizeof(header->checksum)], total_size - sizeof(header->checksum));
	failed |= mbedtls_sha256_finish(&digest_context, header->checksum);
#endif
	ASSERT_OR_EXIT_MSG(failed == 0, "Header checksum calculation failed.");
#endif

	// Write everything to the datastore context.
	agent_status = iot_agent_datastore_allocate(datastore, total_size);
    AGENT_SUCCESS_OR_EXIT();
	agent_status = iot_agent_datastore_write(datastore, 0U, buffer, total_size);
    AGENT_SUCCESS_OR_EXIT();
	agent_status = iot_agent_datastore_commit(datastore);
	AGENT_SUCCESS_OR_EXIT_MSG("Writing header to datastore failed.");

exit:
	free(buffer);
	return agent_status;
}
#if !(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)
static iot_agent_status_t iot_agent_utils_update_edgelock2go_datastore(iot_agent_keystore_t *keystore,
	iot_agent_datastore_t* datastore, const char* hostname, uint32_t port, const pb_bytes_array_t* trusted_root_ca_certificates)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	nxp_iot_ServiceDescriptor service_descriptor = nxp_iot_ServiceDescriptor_init_default;

	ASSERT_OR_EXIT_MSG(hostname != NULL, "Hostname pointer is NULL");
	ASSERT_OR_EXIT_MSG(keystore != NULL, "Port pointer is NULL");
	ASSERT_OR_EXIT_MSG(datastore != NULL, "Port pointer is NULL");

	// the update of the datastore will not happen if there is a valid store indcluding same hostname and port
	if (iot_agent_service_is_configuration_data_valid(datastore)) {

		size_t number_of_services = iot_agent_service_get_number_of_services(datastore);
		size_t offset = 0U;
		for (size_t i = 0U; i < number_of_services; i++)
		{
			agent_status = iot_agent_service_get_service_offset_by_index(datastore, i, &offset, &service_descriptor);
			AGENT_SUCCESS_OR_EXIT();

			// We are only interested in service descriptors that are to be used for connecting
			// to EdgeLock 2GO cloud service.
			if (service_descriptor.has_service_type && (service_descriptor.service_type == nxp_iot_ServiceType_EL2GOSERVICE)) {
				continue;
			}

			if (strcmp(service_descriptor.hostname, hostname) == 0) {
				if ((service_descriptor.has_port) && (service_descriptor.port == port) && (trusted_root_ca_certificates == NULL)) {
				IOT_AGENT_TRACE("EdgeLock 2GO hostname and port match, update of datastore not required");
					agent_status = IOT_AGENT_SUCCESS;
					goto exit;
				}
			}
		}
	}

	IOT_AGENT_TRACE("Update of the EdgeLock 2GO datastore with hostname and port: %s:%d", hostname, port);
	agent_status = iot_agent_utils_write_edgelock2go_datastore(keystore, datastore,
		hostname, port, trusted_root_ca_certificates, NULL);

exit:
	return agent_status;
}

static iot_agent_status_t iot_agent_utils_get_el2go_hostname_and_port_from_cmdline(iot_agent_keystore_t *keystore,
	iot_agent_datastore_t* datastore, const pb_bytes_array_t* trusted_root_ca_certificates, int argc, const char *argv[]) {
	iot_agent_status_t agent_status = IOT_AGENT_FAILURE;
	int ret = 0;
	char* edgelock2go_hostname = NULL;
	char* edgelock2go_port_str = NULL;

	ASSERT_OR_EXIT_MSG(keystore != NULL, "Port pointer is NULL");
	ASSERT_OR_EXIT_MSG(datastore != NULL, "Port pointer is NULL");

	// There should be at least 2 not null parameters
	if (argc > 2                  /* Alteast 1 cli argument */
		&& argv != NULL           /* argv not null */
		) {

		for (int i = 1; i < argc; i++) {
			if (argv[i] != NULL) {
				ret = strncmp("EDGELOCK2GO_HOSTNAME=", argv[i], sizeof("EDGELOCK2GO_HOSTNAME=") - 1U);
				if (ret == 0) {
					edgelock2go_hostname = (char*)(argv[i] + sizeof("EDGELOCK2GO_HOSTNAME=") - 1U);
					ASSERT_OR_EXIT_MSG(strlen(edgelock2go_hostname) > 0U, "Hostname argument is empty");
				}

				ret = strncmp("EDGELOCK2GO_PORT=", argv[i], sizeof("EDGELOCK2GO_PORT=") - 1U);
				if (ret == 0) {
					edgelock2go_port_str = (char*)(argv[i] + sizeof("EDGELOCK2GO_PORT=") - 1U);
				}

				if ((edgelock2go_hostname != NULL) && (edgelock2go_port_str != NULL)) {
					int int_port = atoi(edgelock2go_port_str);
					ASSERT_OR_EXIT_MSG(int_port >= 0, "Port is negative value.");
					agent_status = iot_agent_utils_update_edgelock2go_datastore(keystore, datastore,
						edgelock2go_hostname, (uint32_t)int_port, trusted_root_ca_certificates);
					goto exit;
				}
			}
		}
	}
exit:
	return agent_status;
}

static iot_agent_status_t iot_agent_utils_get_el2go_hostname_and_port_from_env(iot_agent_keystore_t *keystore,
	iot_agent_datastore_t* datastore, const pb_bytes_array_t* trusted_root_ca_certificates) {
	iot_agent_status_t agent_status = IOT_AGENT_FAILURE;
	char* edgelock2go_hostname = NULL;
	size_t len = 0U;
	int int_port = 0;
#if defined(_WIN32) || defined(_WIN64)
	char* edgelock2go_hostname_str = NULL;
	char* edgelock2go_port_str = NULL;
	size_t edgelock2go_hostname_env_size = 0U;
	size_t edgelock2go_port_env_size = 0U;
#else
	const char* edgelock2go_hostname_str = NULL;
	const char* edgelock2go_port_str = NULL;
#endif

	ASSERT_OR_EXIT_MSG(keystore != NULL, "Port pointer is NULL");
	ASSERT_OR_EXIT_MSG(datastore != NULL, "Port pointer is NULL");


#if defined(_WIN32) || defined(_WIN64)
	ASSERT_OR_EXIT_MSG(_dupenv_s(&edgelock2go_hostname_str, &edgelock2go_hostname_env_size, "EDGELOCK2GO_HOSTNAME") == 0, "Error in getting environmental variable");
#else
	edgelock2go_hostname_str = getenv("EDGELOCK2GO_HOSTNAME");
#endif

	if (edgelock2go_hostname_str == NULL) {
		goto exit;
	}

	len = strlen(edgelock2go_hostname_str);
	edgelock2go_hostname = malloc(len + 1U);
	ASSERT_OR_EXIT(edgelock2go_hostname != NULL);
	memset(edgelock2go_hostname, '\0', len + 1U);
	memcpy(edgelock2go_hostname, edgelock2go_hostname_str, len);

#if defined(_WIN32) || defined(_WIN64)
	ASSERT_OR_EXIT_MSG(_dupenv_s(&edgelock2go_port_str, &edgelock2go_port_env_size, "EDGELOCK2GO_PORT") == 0, "Error in getting environmental variable");
#else
	edgelock2go_port_str = getenv("EDGELOCK2GO_PORT");
#endif

	if (edgelock2go_port_str == NULL) {
		goto exit;
	}

	int_port = atoi(edgelock2go_port_str);
	ASSERT_OR_EXIT_MSG(int_port >= 0, "Port is negative value.");
	agent_status = iot_agent_utils_update_edgelock2go_datastore(keystore, datastore,
		edgelock2go_hostname, (uint32_t)int_port, trusted_root_ca_certificates);

exit:
	free(edgelock2go_hostname);
	return agent_status;
}
#endif //!(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)

iot_agent_status_t iot_agent_utils_configure_edgelock2go_datastore(iot_agent_keystore_t *keystore,
	iot_agent_datastore_t* datastore, const pb_bytes_array_t* trusted_root_ca_certificates,
	int argc, const char *argv[]) {
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	const pb_bytes_array_t* used_root_ca_certificates = iot_agent_trusted_root_ca_certificates;

	if (trusted_root_ca_certificates != NULL) {
		used_root_ca_certificates = trusted_root_ca_certificates;
	}

	ASSERT_OR_EXIT_MSG(keystore != NULL, "Port pointer is NULL");
	ASSERT_OR_EXIT_MSG(datastore != NULL, "Port pointer is NULL");

#if !(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)
	// in FreeRTOS and Zephyr the setting through command line or environment vairable is not supported
	if (iot_agent_utils_get_el2go_hostname_and_port_from_cmdline(keystore, datastore, used_root_ca_certificates, argc, argv) == IOT_AGENT_SUCCESS) {
		IOT_AGENT_TRACE("EL2GO hostname set from command line");
		goto exit;
	}

	if (iot_agent_utils_get_el2go_hostname_and_port_from_env(keystore, datastore, used_root_ca_certificates) == IOT_AGENT_SUCCESS) {
		IOT_AGENT_TRACE("EL2GO hostname set from environment variable");
		goto exit;
	}
#endif //!(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)

	// in this phase check if the configuration data is valid, and fill it with macro defined variables
	if (!iot_agent_service_is_configuration_data_valid(datastore)) {
		IOT_AGENT_TRACE("Invalid EL2GO datastore file, use hostname and port from macros");
		iot_agent_utils_write_edgelock2go_datastore(keystore, datastore,
			EDGELOCK2GO_HOSTNAME, EDGELOCK2GO_PORT, used_root_ca_certificates, NULL);
	}

exit:
	return agent_status;
}

#if !(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)
iot_agent_status_t iot_agent_keystore_file_existence(const char *Filename, bool forceCreation)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	ASSERT_OR_EXIT_MSG(Filename != NULL, "Filename is NULL.");

	ASSERT_OR_EXIT_MSG((strstr(Filename, "..")) == NULL, "Filename is not allowed");

	if (ACCESS(Filename, R_OK) != 0 || forceCreation)
	{
		FILE* fd = fopen(Filename, "w");
		if (fd == NULL)
		{
			EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "Unable to ensure file existence [%s]", Filename);
		}
		ASSERT_OR_EXIT_MSG(fclose(fd) == 0, "Error in closing the file");
	}
exit:
	return agent_status;
}

#endif //!(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)

iot_agent_status_t iot_agent_utils_convert_service2key_id(uint64_t service_id, uint32_t *key_id_ptr)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	ASSERT_OR_EXIT_MSG(key_id_ptr != NULL, "key_id is NULL.");

	if (service_id > 0xFFFFFFFFUL)
	{
		return IOT_AGENT_FAILURE;
	}

	*key_id_ptr = EDGELOCK2GO_MANAGED_SERVICE_KEY_MIN | ((uint32_t) service_id);

	if (*key_id_ptr > EDGELOCK2GO_MANAGED_SERVICE_KEY_MAX)
	{
		agent_status = IOT_AGENT_FAILURE;
	}
	else
	{
		agent_status = IOT_AGENT_SUCCESS;
	}
exit:
	return agent_status;
}

static iot_agent_status_t iot_agent_ceil_to_blocksize(size_t len, size_t blocksize, size_t* padded_length)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	ASSERT_OR_EXIT_MSG(blocksize >= 1U, "Overflow in calculation of padded length");
	ASSERT_OR_EXIT_MSG(len <= (SIZE_MAX - (blocksize - 1U)), "Overflow in calculation of padded length");
	*padded_length = ((len + (blocksize - 1U)) / blocksize) * blocksize;
exit:
    return agent_status;
}

iot_agent_status_t nxp_iot_agent_unpad_iso7816d4(uint8_t *data, size_t *data_size) {
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	size_t count = 0U;
	ASSERT_OR_EXIT_MSG(*data_size <= (SIZE_MAX - 1U), "Wraparound in data size.");
	ASSERT_OR_EXIT_MSG(*data_size >= 1U, "Data size should be greater the 1.");
	count = *data_size - 1U;
	while ((count > 0U) && (data[count] == 0U)) {
		count--;
	}
	ASSERT_OR_EXIT_MSG(count < *data_size, "Overflow in data buffer.");
	ASSERT_OR_EXIT_MSG(data[count] == 0x80U, "iso_7816_unpad failed, pad block corrupted.");
	ASSERT_OR_EXIT_MSG(*data_size >= (*data_size - count), "Overflow in data buffer.");

	*data_size -= *data_size - count;
exit:
	return agent_status;
}

iot_agent_status_t iot_agent_pad_iso7816d4(uint8_t *data, size_t data_size,
        size_t unpadded_length, size_t blocksize, size_t* padded_length)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	ASSERT_OR_EXIT_MSG(unpadded_length <= SIZE_MAX - 1U, "Wraparound in length variable.");
    agent_status = iot_agent_ceil_to_blocksize(
            unpadded_length + 1U /* always inserted padding 0x80 */, blocksize, padded_length);
	AGENT_SUCCESS_OR_EXIT_MSG("Error in calculation of padded length");
    ASSERT_OR_EXIT_MSG(data_size >= *padded_length, "Buffer size %u is too small to hold "
            "plaintext of length %u that will be padded up to length %u", data_size,
            unpadded_length, *padded_length);

	ASSERT_OR_EXIT_MSG(unpadded_length < data_size, "Overflow in the data buffer");
    data[unpadded_length] = 0x80U;
	ASSERT_OR_EXIT_MSG(unpadded_length <= (SIZE_MAX - 1U - (*padded_length - (unpadded_length + 1U))), "Wraparound in the data buffer");
	ASSERT_OR_EXIT_MSG(*padded_length >= (unpadded_length + 1U), "Wraparound in the data buffer");
    memset(&data[unpadded_length + 1U], 0, *padded_length - (unpadded_length + 1U));

exit:
    return agent_status;
}

#ifdef NXP_IOT_AGENT_ENABLE_LITE

#include "flash_config.h"
#include <fsl_cache.h>
#include <nxp_iot_agent_flash_config.h>

typedef struct _flexspi_cache_status
{
    volatile bool CacheEnableFlag;
} flexspi_cache_status_t;

static void disable_cache(flexspi_cache_status_t *cacheStatus)
{
    /* Disable cache */
    CACHE64_DisableCache(CACHE64_CTRL0);
    cacheStatus->CacheEnableFlag = true;
}

static void enable_cache(flexspi_cache_status_t cacheStatus)
{
    if (cacheStatus.CacheEnableFlag)
    {
        /* Enable cache. */
        CACHE64_EnableCache(CACHE64_CTRL0);
    }
}

static status_t program_memory(api_core_context_t *context, uint32_t address, uint32_t length, const void *data)
{
    __disable_irq(); 

    flexspi_cache_status_t cacheStatus;
    disable_cache(&cacheStatus);

    status_t status = iap_mem_write(context, address, length, data, kMemoryID_FlexspiNor);

    enable_cache(cacheStatus);
    __enable_irq();
    return status;
}

static status_t erase_memory(api_core_context_t *context, uint32_t address, uint32_t sector_size)
{
    __disable_irq();
    flexspi_cache_status_t cacheStatus;
    disable_cache(&cacheStatus); 

    status_t status = iap_mem_erase(context, address, sector_size, kMemoryID_FlexspiNor); 

    enable_cache(cacheStatus);
    __enable_irq();

    return status;
}

static status_t flush_memory(api_core_context_t *context)
{
    __disable_irq();
    flexspi_cache_status_t cacheStatus;
    disable_cache(&cacheStatus); 

    status_t status = iap_mem_flush(context); 

    enable_cache(cacheStatus);
    __enable_irq();

    return status;
}

iot_agent_status_t iot_agent_utils_write_to_flash(const uint8_t *area, size_t area_size, const uint8_t *data, size_t data_size)
{
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

    ASSERT_OR_EXIT_MSG(area != NULL, "area address null");
    ASSERT_OR_EXIT_MSG(data != NULL, "data address is null");
    ASSERT_OR_EXIT_MSG(data_size <= area_size, "data_size is bigger than area_size");

    ASSERT_OR_EXIT_STATUS_SILENT(data_size != 0U, IOT_AGENT_SUCCESS);

    api_core_context_t context  = {0};
    uint8_t iap_api_arena[4096] = {0};
    kp_api_init_param_t params  = {0};
    params.allocStart           = (uint32_t)&iap_api_arena;
    params.allocSize            = sizeof(iap_api_arena);

    ASSERT_OR_EXIT(iap_api_init(&context, &params) == kStatus_Success);

    flexspi_nor_config_t flashConfig = {0};
    if (!SYSTEM_IS_XIP_FLEXSPI())
    {
        // In case of RAM execution we found out a limitation in case the Flash configuration
        // is loaded throught the flexspi_nor_get_config; in case of SW reset, only the RW61x chip
        // reset (but not the Flash), causing the function giving back the default Flash setting
        // used by the boot ROM for initial FCB read. For this reason the Flash config is stored in the
        // application variable flexspi_config_agent

        /**********************************************************************************************************************
         * API: flexspi_nor_set_clock_source
         *********************************************************************************************************************/
        uint32_t flexspi_clock_source = 0x0;
        status_t status = flexspi_nor_set_clock_source(flexspi_clock_source);
        if (kStatus_Success != status)
        {
            PRINTF("flexspi_nor_set_clock_source returned with code [%d]\r\n", status);
        }

        /**********************************************************************************************************************
         * API: flexspi_clock_config
         *********************************************************************************************************************/
        uint32_t flexspi_freqOption    = 0x1;
        uint32_t flexspi_sampleClkMode = 0x0;
        flexspi_clock_config(FLEXSPI_INSTANCE, flexspi_freqOption, flexspi_sampleClkMode);	  

        flashConfig = flexspi_config_agent;
    }
    else
    {
        flashConfig = *((flexspi_nor_config_t *)FCB_ADDRESS);
    }

    ASSERT_OR_EXIT_MSG(iap_mem_config(&context, (uint32_t *)&flashConfig, kMemoryID_FlexspiNor) == kStatus_Success,
					   "Error in Flash memory configuration");

    ASSERT_OR_EXIT_MSG(erase_memory(&context, (uint32_t)area, area_size) == kStatus_Success,
					   "Error in Flash memory erasing");
    ASSERT_OR_EXIT_MSG(program_memory(&context, (uint32_t)area, data_size, data) == kStatus_Success,
					   "Error in Flash memory writing");
    ASSERT_OR_EXIT_MSG(flush_memory(&context) == kStatus_Success,
					   "Error in Flash memory flushing");

    ASSERT_OR_EXIT_MSG(memcmp(area, data, data_size) == 0,
					   "Error in data check after memory write");

exit:
    return agent_status;
}

iot_agent_status_t iot_agent_utils_erase_from_flash(const uint8_t *area, size_t area_size)
{
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

    ASSERT_OR_EXIT_MSG(area != NULL, "area address null");

    ASSERT_OR_EXIT_STATUS_SILENT(area_size != 0, IOT_AGENT_SUCCESS);

    api_core_context_t context  = {0};
    uint8_t iap_api_arena[4096] = {0};
    kp_api_init_param_t params  = {0};
    params.allocStart           = (uint32_t)&iap_api_arena;
    params.allocSize            = sizeof(iap_api_arena);

    ASSERT_OR_EXIT(iap_api_init(&context, &params) == kStatus_Success);

    flexspi_nor_config_t flashConfig = {0};
    if (!SYSTEM_IS_XIP_FLEXSPI())
    {
        // In case of RAM execution we found out a limitation in case the Flash configuration
        // is loaded throught the flexspi_nor_get_config; in case of SW reset, only the RW61x chip
        // reset (but not the Flash), causing the function giving back the default Flash setting
        // used by the boot ROM for initial FCB read. For this reason the Flash config is stored in the
        // application variable flexspi_config_agent

        /**********************************************************************************************************************
         * API: flexspi_nor_set_clock_source
         *********************************************************************************************************************/
        uint32_t flexspi_clock_source = 0x0;
        status_t status = flexspi_nor_set_clock_source(flexspi_clock_source);
        if (kStatus_Success != status)
        {
            PRINTF("flexspi_nor_set_clock_source returned with code [%d]\r\n", status);
        }

        /**********************************************************************************************************************
         * API: flexspi_clock_config
         *********************************************************************************************************************/
        uint32_t flexspi_freqOption    = 0x1;
        uint32_t flexspi_sampleClkMode = 0x0;
        flexspi_clock_config(FLEXSPI_INSTANCE, flexspi_freqOption, flexspi_sampleClkMode);	  

        flashConfig = flexspi_config_agent;
    }
    else
    {
        flashConfig = *((flexspi_nor_config_t *)FCB_ADDRESS);
    }

    ASSERT_OR_EXIT_MSG(iap_mem_config(&context, (uint32_t *)&flashConfig, kMemoryID_FlexspiNor) == kStatus_Success,
					   "Error in Flash memory configuration");

    ASSERT_OR_EXIT_MSG(erase_memory(&context, (uint32_t)area, area_size) == kStatus_Success,
					   "Error in Flash memory erasing");
    ASSERT_OR_EXIT_MSG(flush_memory(&context) == kStatus_Success,
					   "Error in Flash memory flushing");

exit:
    return agent_status;
}
#endif // NXP_IOT_AGENT_ENABLE_LITE
