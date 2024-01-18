/*
 * Copyright 2018-2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <nxp_iot_agent_utils.h>

#if !(defined(__ICCARM__) || defined(__CC_ARM) || defined(__ARMCC_VERSION))
#include <unistd.h>
#endif

#if SSS_HAVE_HOSTCRYPTO_OPENSSL
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#endif

#if SSS_HAVE_HOSTCRYPTO_MBEDTLS
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

#if NXP_IOT_AGENT_HAVE_SSS
#include <fsl_sss_api.h>
#include <nxp_iot_agent_keystore_sss_se05x.h>
#endif
#if NXP_IOT_AGENT_HAVE_PSA
#if NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL
#else
#include <fsl_silicon_id.h>
#endif
#endif

// AES CBC
#define AES_CBC_BLOCK_SIZE  		16

#if SSS_HAVE_HOSTCRYPTO_OPENSSL

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

iot_agent_status_t iot_agent_utils_gen_key_ref_ecc(sss_key_store_t *keyStore, sss_object_t *keyObject, int nid, EVP_PKEY* pkey)
{
	// We use the buffer for the public key and the private key. Ensure that it fits both!
	COMPILE_TIME_ASSERT(MAX_ECC_PUB_KEY_BUFFER_LEN > MAX_ECC_PRIV_KEY_TEMPLATE_LEN);

	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	sss_status_t sss_status;
	int openssl_status;

	const key_ref_ecc_template_t* key_ref_template = NULL;

	uint8_t buffer[MAX_ECC_PUB_KEY_BUFFER_LEN];
	size_t buffer_size = sizeof(buffer);
	const uint8_t* buffer_ptr_const = buffer;
	uint8_t* buffer_ptr;

	size_t public_key_length_bits = 0U;

	BIGNUM *bn_priv = NULL;
	EC_KEY *ec_key = NULL;

	ASSERT_OR_EXIT_MSG(keyStore != NULL, "keyStore is NULL.");
	ASSERT_OR_EXIT_MSG(keyObject != NULL, "keyObject is NULL.");
	ASSERT_OR_EXIT_MSG(pkey != NULL, "pkey is NULL.");

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
	ec_key = d2i_EC_PUBKEY(NULL, &buffer_ptr_const, (long)buffer_size);
	OPENSSL_ASSERT_OR_EXIT(ec_key != NULL, "d2i_EC_PUBKEY");

	// ##############################
	// replace the private key part with a key reference to the objectid:

	memcpy(buffer, key_ref_template->private_key, key_ref_template->private_key_len);
	buffer_ptr = buffer + key_ref_template->private_key_offset_key_id;
	*(buffer_ptr++) = (uint8_t)(keyObject->keyId >> 24) & 0xFFU;
	*(buffer_ptr++) = (uint8_t)(keyObject->keyId >> 16) & 0xFFU;
	*(buffer_ptr++) = (uint8_t)(keyObject->keyId >> 8) & 0xFFU;
	*(buffer_ptr) = (uint8_t)(keyObject->keyId >> 0) & 0xFFU;

	// If we ever require key indices of storage classes, this is where those would go:
	//offset = key_ref_template->private_key_offset_key_index;
	//buffer[offset] = 0; // keyIndex;
	//offset = key_ref_template->private_key_offset_storage_class;
	//buffer[offset] = 0; // storageClass;

	bn_priv = BN_bin2bn(buffer, (int)key_ref_template->private_key_len, NULL);
	OPENSSL_ASSERT_OR_EXIT_STATUS(bn_priv != NULL, "BN_bin2bn",
		IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED);

	openssl_status = EC_KEY_set_private_key(ec_key, bn_priv);
	OPENSSL_SUCCESS_OR_EXIT_STATUS("EC_KEY_set_private_key",
		IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED);

	// Make the key reference its curve via OID
	EC_KEY_set_asn1_flag(ec_key, nid);

	// Return the ECC key as part of the output parameter.
	openssl_status = EVP_PKEY_assign_EC_KEY(pkey, ec_key);
	OPENSSL_SUCCESS_OR_EXIT_STATUS("EVP_PKEY_assign_EC_KEY", IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED);
	ec_key = NULL; // pkey takes ownership of the key!;

exit:
	BN_free(bn_priv);
	EC_KEY_free(ec_key);

	return agent_status;
}

#define MAX_RSA_2048_PUB_KEY_BUFFER_LEN   512
#define MAX_RSA_PUB_KEY_BUFFER_LEN        MAX_RSA_2048_PUB_KEY_BUFFER_LEN

iot_agent_status_t iot_agent_utils_gen_key_ref_rsa(sss_key_store_t *keyStore, sss_object_t *keyObject, EVP_PKEY* pkey)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	sss_status_t sss_status;
	int openssl_status;

	uint8_t buffer[MAX_RSA_PUB_KEY_BUFFER_LEN];
	size_t buffer_size = sizeof(buffer);
	const uint8_t* buffer_ptr_const = buffer;

	size_t public_key_length_bits = 0U;

	RSA* rsa_key = NULL;
    BIGNUM* d = NULL;
    BIGNUM* p = NULL;
    BIGNUM* q = NULL;
    BIGNUM* dmp1 = NULL;
    BIGNUM* dmq1 = NULL;
    BIGNUM* iqmp = NULL;

	ASSERT_OR_EXIT_MSG(keyStore != NULL, "keyStore is NULL.");
	ASSERT_OR_EXIT_MSG(keyObject != NULL, "keyObject is NULL.");
	ASSERT_OR_EXIT_MSG(pkey != NULL, "pkey is NULL.");

	// ##############################
	// read the public key out from the SE:

	sss_status = sss_key_store_get_key(keyStore, keyObject, buffer, &buffer_size, &public_key_length_bits);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_store_get_key failed: 0x%04x [object id: 0x%04x].", sss_status, keyObject->keyId);

	// ##############################
	// create key object from public key

	buffer_ptr_const = buffer;
	rsa_key = d2i_RSA_PUBKEY(NULL, &buffer_ptr_const, (long)buffer_size);
	OPENSSL_ASSERT_OR_EXIT(rsa_key != NULL, "d2i_RSA_PUBKEY");

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

#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
	rsa_key->d = d;
	rsa_key->p = p;
	rsa_key->q = q;
	rsa_key->dmp1 = dmp1;
	rsa_key->dmq1 = dmq1;
	rsa_key->iqmp = iqmp;
#else
	openssl_status = RSA_set0_factors(rsa_key, p, q);
	OPENSSL_SUCCESS_OR_EXIT("RSA_set0_factors");

	openssl_status = RSA_set0_crt_params(rsa_key, dmp1, dmq1, iqmp);
	OPENSSL_SUCCESS_OR_EXIT("RSA_set0_crt_params");

	openssl_status = RSA_set0_key(rsa_key, NULL, NULL, d);
	OPENSSL_SUCCESS_OR_EXIT("RSA_set0_key");
#endif

	openssl_status = EVP_PKEY_assign_RSA(pkey, rsa_key);
	OPENSSL_SUCCESS_OR_EXIT_STATUS("EVP_PKEY_assign_RSA_KEY", IOT_AGENT_ERROR_CRYPTO_ENGINE_FAILED);
	rsa_key = NULL; // pkey takes ownership of the key!;

exit:
	RSA_free(rsa_key);
	return agent_status;
}


iot_agent_status_t iot_agent_utils_gen_key_ref(sss_key_store_t *keyStore, sss_object_t *keyObject, const uint32_t keyid, EVP_PKEY* pkey)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	sss_status_t sss_status;

	ASSERT_OR_EXIT_MSG(keyStore != NULL, "keyStore is NULL.");
	ASSERT_OR_EXIT_MSG(keyObject != NULL, "keyObject is NULL.");
	ASSERT_OR_EXIT_MSG(pkey != NULL, "pkey is NULL.");

	sss_status = sss_key_object_get_handle(keyObject, keyid);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_get_handle failed: 0x%04x [object id: 0x%04x].", sss_status, keyObject->keyId);

	switch (keyObject->cipherType)
	{
	case kSSS_CipherType_EC_NIST_P:
		agent_status = iot_agent_utils_gen_key_ref_ecc(keyStore, keyObject, NID_X9_62_prime256v1, pkey);
		AGENT_SUCCESS_OR_EXIT();
		break;
	case kSSS_CipherType_RSA:
	case kSSS_CipherType_RSA_CRT:
		agent_status = iot_agent_utils_gen_key_ref_rsa(keyStore, keyObject, pkey);
		AGENT_SUCCESS_OR_EXIT();
		break;
	default:
		IOT_AGENT_ERROR("Unsupported key cipherType [%u]", keyObject->cipherType);
		agent_status = IOT_AGENT_FAILURE;
		break;
	}

exit:
	return agent_status;
}


iot_agent_status_t iot_agent_utils_write_key_ref_pem(sss_key_store_t *keyStore, sss_object_t *keyObject,
	const uint32_t keyid, const char* filename)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	int openssl_status;
	BIO* out = NULL;
	EVP_PKEY* pkey = NULL;

	ASSERT_OR_EXIT_MSG(keyStore != NULL, "keyStore is NULL.");
	ASSERT_OR_EXIT_MSG(keyObject != NULL, "keyObject is NULL.");
	ASSERT_OR_EXIT_MSG(filename != NULL, "filename is NULL.");

	ASSERT_OR_EXIT_MSG((strstr(filename, "..")) == NULL, "Filename is not allowed");

	out = BIO_new_file(filename, "w");
	ASSERT_OR_EXIT_STATUS(out != NULL, IOT_AGENT_ERROR_FILE_SYSTEM);

	pkey = EVP_PKEY_new();
	OPENSSL_ASSERT_OR_EXIT_STATUS(pkey != NULL, "EVP_PKEY_new", IOT_AGENT_FAILURE);

	iot_agent_utils_gen_key_ref(keyStore, keyObject, keyid, pkey);
	AGENT_SUCCESS_OR_EXIT();

	openssl_status = PEM_write_bio_PrivateKey(out, pkey, NULL, NULL, 0, NULL, NULL);
	OPENSSL_SUCCESS_OR_EXIT_STATUS("PEM_write_PrivateKey", IOT_AGENT_ERROR_FILE_SYSTEM);

exit:
	EVP_PKEY_free(pkey);
	BIO_free(out);
	return agent_status;
}


iot_agent_status_t iot_agent_utils_der_to_pem_bio(BIO *bio_in, BIO* bio_out)
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

iot_agent_status_t iot_agent_utils_write_certificate_pem(uint8_t* buffer, size_t len, const char* filename)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	int openssl_status;

	BIO *bio_in = NULL;
	BIO * bio_out = NULL;

	ASSERT_OR_EXIT_MSG(buffer != NULL, "buffer is NULL.");
	ASSERT_OR_EXIT_MSG(filename != NULL, "filename is NULL.");

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

	return agent_status;
}

iot_agent_status_t iot_agent_utils_write_certificate_pem_from_keystore(iot_agent_keystore_t *keyStore,
	const uint32_t certid, const char* filename)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	uint8_t cert_buffer[NXP_IOT_AGENT_EDGELOCK2GO_CLIENT_CERTIFICATE_BUFFER_SIZE];
	size_t cert_len = sizeof(cert_buffer);

	ASSERT_OR_EXIT_MSG(keyStore != NULL, "keyStore is NULL.");
	ASSERT_OR_EXIT_MSG(filename != NULL, "filename is NULL.");

	agent_status = iot_agent_utils_get_certificate_from_keystore(keyStore, certid, cert_buffer, &cert_len);
	AGENT_SUCCESS_OR_EXIT();

	ASSERT_OR_EXIT_MSG((strstr(filename, "..")) == NULL, "Filename is not allowed");

	agent_status = iot_agent_utils_write_certificate_pem(cert_buffer, cert_len, filename);
	AGENT_SUCCESS_OR_EXIT();

exit:
	return agent_status;
}

iot_agent_status_t iot_agent_utils_write_key_ref_service_pem(const iot_agent_context_t* ctx,
	const char* filename)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	sss_status_t sss_status;
	nxp_iot_ServiceDescriptor service_descriptor = { 0U };
	iot_agent_keystore_t* keystore = NULL;
	sss_key_store_t* sss_context = NULL;
	sss_object_t key = { 0 };
    uint32_t keystore_id = 0U;
	agent_status = iot_agent_get_service_descriptor(ctx, &service_descriptor);
	AGENT_SUCCESS_OR_EXIT();

	ASSERT_OR_EXIT_MSG(ctx != NULL, "ctx is NULL.");
	ASSERT_OR_EXIT_MSG(filename != NULL, "filename is NULL.");

	keystore_id = service_descriptor.client_key_sss_ref.endpoint_id;
	agent_status = iot_agent_get_keystore_by_id(ctx, keystore_id, &keystore);
	AGENT_SUCCESS_OR_EXIT();

	agent_status = iot_agent_keystore_sss_se05x_get_sss_key_store(keystore->context, &sss_context);
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_keystore_sss_se05x_get_sss_key_store failed: 0x%08x", agent_status);

	sss_status = sss_key_object_init(&key, sss_context);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_init failed: 0x%04x.", sss_status);

	agent_status = iot_agent_utils_convert_service2key_id(service_descriptor.identifier, &key.keyId);
	AGENT_SUCCESS_OR_EXIT();

	ASSERT_OR_EXIT_MSG((strstr(filename, "..")) == NULL, "Filename is not allowed");

	agent_status = iot_agent_utils_write_key_ref_pem(sss_context, &key, key.keyId, filename);
	AGENT_SUCCESS_OR_EXIT();

exit:
	iot_agent_free_service_descriptor(&service_descriptor);
	return agent_status;
}

#endif

#if NXP_IOT_AGENT_HAVE_SSS
static iot_agent_status_t iot_agent_utils_get_keystore_from_service_descriptor(const iot_agent_context_t* ctx,
	const nxp_iot_ServiceDescriptor* service_descriptor, iot_agent_keystore_t** keystore)
{
	uint32_t keystore_id = 0U;
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	ASSERT_OR_EXIT_MSG(ctx != NULL, "ctx is NULL.");
	ASSERT_OR_EXIT_MSG(service_descriptor != NULL, "service_descriptor is NULL.");
	ASSERT_OR_EXIT_MSG(keystore != NULL, "keystore is NULL.");

	keystore_id = service_descriptor->client_certificate_sss_ref.endpoint_id;

	agent_status = iot_agent_get_keystore_by_id(ctx, keystore_id, keystore);
	AGENT_SUCCESS_OR_EXIT();
exit:
	return agent_status;
}
#endif // NXP_IOT_AGENT_HAVE_SSS

#if	(SSS_HAVE_HOSTCRYPTO_OPENSSL)
iot_agent_status_t iot_agent_utils_write_certificate_pem_cos_over_rtp(iot_agent_context_t* ctx,
	const nxp_iot_ServiceDescriptor* service_descriptor, const char* filename)
{
	uint32_t certificate_id = 0U;
	iot_agent_keystore_t* keystore = NULL;
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
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
	return agent_status;
}

iot_agent_status_t iot_agent_utils_write_key_ref_pem_cos_over_rtp(const iot_agent_context_t* ctx,
	const nxp_iot_ServiceDescriptor* service_descriptor, const char* filename)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	sss_status_t sss_status;
	iot_agent_keystore_t* keystore = NULL;
	sss_key_store_t* sss_keystore = NULL;
	sss_object_t key = { 0 };

	ASSERT_OR_EXIT_MSG(ctx != NULL, "ctx is NULL.");
	ASSERT_OR_EXIT_MSG(service_descriptor != NULL, "service_descriptor is NULL.");
	ASSERT_OR_EXIT_MSG(filename != NULL, "filename is NULL.");

	agent_status = iot_agent_utils_get_keystore_from_service_descriptor(ctx, service_descriptor, &keystore);
	AGENT_SUCCESS_OR_EXIT();

	agent_status = iot_agent_keystore_sss_se05x_get_sss_key_store(keystore->context, &sss_keystore);
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_keystore_sss_se05x_get_sss_key_store failed: 0x%08x", agent_status);

	sss_status = sss_key_object_init(&key, sss_keystore);
	SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_init failed: 0x%04x.", sss_status);

	ASSERT_OR_EXIT_MSG((strstr(filename, "..")) == NULL, "Filename is not allowed");

	agent_status = iot_agent_utils_write_key_ref_pem(sss_keystore, &key, service_descriptor->client_key_sss_ref.object_id, filename);
	AGENT_SUCCESS_OR_EXIT();

exit:
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

	bio_in_cert = BIO_new_mem_buf(cert_buffer, (int)cert_len);
	client_cert = d2i_X509_bio(bio_in_cert, NULL);
	if (client_cert == NULL) {
		BIO_free(bio_in_cert);
		X509_free(client_cert);
		EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "Error in loading the client certificate");
	}
	subject_name = X509_get_subject_name(client_cert);
	X509_NAME_get_text_by_NID(subject_name, NID_commonName, common_name, max_size);
	BIO_free(bio_in_cert);
	X509_free(client_cert);
exit:
	return agent_status;
}

#endif //#if (SSS_HAVE_HOSTCRYPTO_OPENSSL)

#if	(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)

iot_agent_status_t iot_agent_utils_get_oid_value_in_subject(uint8_t* cert_buffer, size_t cert_len,
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

	if (mbedtls_x509_crt_parse_der(&client_cert, cert_buffer, cert_len))
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
			for(size_t i = 0; i < len; i++)
			{
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
#if NXP_IOT_AGENT_HAVE_SSS
	iot_agent_keystore_t* keystore = NULL;
#endif
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	uint8_t cert_buffer[NXP_IOT_AGENT_EDGELOCK2GO_CLIENT_CERTIFICATE_BUFFER_SIZE];
	size_t cert_len = sizeof(cert_buffer);

	ASSERT_OR_EXIT_MSG(ctx != NULL, "ctx is NULL.");
	ASSERT_OR_EXIT_MSG(service_descriptor != NULL, "service_descriptor is NULL.");
	ASSERT_OR_EXIT_MSG(common_name != NULL, "common_name is NULL.");

	certificate_id = service_descriptor->client_certificate_sss_ref.object_id;
#if NXP_IOT_AGENT_HAVE_SSS
	agent_status = iot_agent_utils_get_keystore_from_service_descriptor(ctx, service_descriptor, &keystore);
	AGENT_SUCCESS_OR_EXIT();

	agent_status = iot_agent_utils_get_certificate_from_keystore(keystore, certificate_id, cert_buffer, &cert_len);
	AGENT_SUCCESS_OR_EXIT();
#endif
#if NXP_IOT_AGENT_HAVE_PSA
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

	ASSERT_OR_EXIT_MSG(keystore != NULL, "keyStore is NULL.");
	ASSERT_OR_EXIT_MSG(cert != NULL, "cert is NULL.");
	ASSERT_OR_EXIT_MSG(cert_len != NULL, "cert_len is NULL.");
	ASSERT_OR_EXIT_MSG(keystore->type == IOT_AGENT_KS_SSS_SE05X, "keystore type %d is unsupported.", keystore->type);

#if NXP_IOT_AGENT_HAVE_SSS
	sss_status_t sss_status;
	sss_object_t certObj = { 0 };
	size_t cert_lenBits = 0U;

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
#endif
exit:
	return agent_status;
}

iot_agent_status_t iot_agent_get_first_found_object(iot_agent_keystore_t* keystore,
	uint32_t* object_ids, size_t num_objects, uint32_t* object_id)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	ASSERT_OR_EXIT_MSG(keystore != NULL, "keystore is NULL.");
	ASSERT_OR_EXIT_MSG(object_ids != NULL, "object_ids is NULL.");
	ASSERT_OR_EXIT_MSG(object_id != NULL, "object_id is NULL.");
	ASSERT_OR_EXIT_MSG(keystore->type == IOT_AGENT_KS_SSS_SE05X, "keystore type %d is unsupported.", keystore->type);

#if NXP_IOT_AGENT_HAVE_SSS
	sss_status_t sss_status;
	sss_object_t sss_object = { 0 };

	sss_key_store_t* sss_key_store = NULL;
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
#endif
exit:
	return agent_status;
}

iot_agent_status_t iot_agent_utils_get_edgelock2go_key_id(iot_agent_keystore_t* keystore, uint32_t* object_id)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	if (0) {}
#if NXP_IOT_AGENT_HAVE_SSS
	else if (keystore->type == IOT_AGENT_KS_SSS_SE05X) {

		uint32_t object_ids[] = { EDGELOCK2GO_KEYID_ECC, EDGELOCK2GO_KEYID_RSA };
		size_t num_objects = sizeof(object_ids) / sizeof(object_ids[0]);

		ASSERT_OR_EXIT_MSG(keystore != NULL, "keystore is NULL.");
		ASSERT_OR_EXIT_MSG(object_id != NULL, "object_id is NULL.");

		agent_status = iot_agent_get_first_found_object(keystore, object_ids, num_objects, object_id);
		AGENT_SUCCESS_OR_EXIT_MSG("Neither ECC nor RSA key found.");
	}
#endif
#if NXP_IOT_AGENT_HAVE_PSA
	else if (keystore->type == IOT_AGENT_KS_PSA) {
		*object_id = EDGELOCK2GO_KEYID_ECC;
	}
#endif
#if NXP_IOT_AGENT_HAVE_SSS
	exit:
#endif
	return agent_status;
}


iot_agent_status_t iot_agent_utils_get_edgelock2go_certificate_id(iot_agent_keystore_t* keystore, uint32_t* object_id)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	if (0) {}
#if NXP_IOT_AGENT_HAVE_SSS
	else if (keystore->type == IOT_AGENT_KS_SSS_SE05X) {
		uint32_t object_ids[] = { EDGELOCK2GO_CERTID_ECC, EDGELOCK2GO_CERTID_RSA };
		size_t num_objects = sizeof(object_ids) / sizeof(object_ids[0]);

		ASSERT_OR_EXIT_MSG(keystore != NULL, "keystore is NULL.");
		ASSERT_OR_EXIT_MSG(object_id != NULL, "object_id is NULL.");

		agent_status = iot_agent_get_first_found_object(keystore, object_ids, num_objects, object_id);
		AGENT_SUCCESS_OR_EXIT_MSG("Neither ECC nor RSA certificate found.");
	}
#endif
#if NXP_IOT_AGENT_HAVE_PSA
	else {
		// set status as failure, but without message
		agent_status = IOT_AGENT_FAILURE;
		goto exit;
	}
#endif
exit:
	return agent_status;
}

#if NXP_IOT_AGENT_HAVE_PSA

iot_agent_status_t iot_agent_utils_get_device_id(uint8_t* buffer, size_t* len) {
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

#if NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL
	// Fake for simulators, the UID in fact is in ITS.
	ASSERT_OR_EXIT_MSG(*len >= DEVICEID_LENGTH, "buffer is too small for device id, %u bytes are required.", DEVICEID_LENGTH);

	psa_status_t psa_status = psa_its_get(DEVICEID_KEYID, 0, DEVICEID_LENGTH, buffer, len);
	ASSERT_OR_EXIT_MSG(psa_status == PSA_SUCCESS, "psa_its_get failed: 0x%08x", psa_status);
#elif NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM
	// Devices do have an ID which can be retrieved:
	ASSERT_OR_EXIT_MSG(SILICONID_GetID(buffer, len) == kStatus_Success, "Error in getting the device ID");
#else
	agent_status = read_device_uuid(buffer, len);
#endif

exit:
	return agent_status;
}



iot_agent_status_t iot_agent_utils_create_self_signed_edgelock2go_certificate(
        iot_agent_keystore_t* keystore, uint8_t* certificate_buffer,
        size_t* certificate_buffer_size)
{
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    int ret = 1;
    mbedtls_pk_context loaded_issuer_key;
    const char* issuer_prefix = "O=NXP,OU=Plug and Trust,CN=NXP_DIE_ID_AUTH,serialNumber=";
    char issuer_name[256] = {0};

    mbedtls_x509write_cert crt;
    mbedtls_mpi serial;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char *pers = "edglock2go_self_signed_cert";

    mbedtls_x509write_crt_init( &crt );
    mbedtls_pk_init( &loaded_issuer_key );
    mbedtls_mpi_init( &serial );
    mbedtls_ctr_drbg_init( &ctr_drbg );
    mbedtls_entropy_init( &entropy );

    ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                               (const unsigned char *) pers,
                               strlen( pers ) );
    ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_ctr_drbg_seed failed: 0x%x", ret);

	uint8_t uuid[20] = { 0 };
	size_t uuid_len = sizeof(uuid);
	agent_status = iot_agent_utils_get_device_id(uuid, &uuid_len);
	ASSERT_OR_EXIT_MSG(agent_status == IOT_AGENT_SUCCESS, "iot_agent_utils_get_device_id failed with 0x%08x", agent_status);

	COMPILE_TIME_ASSERT(sizeof(issuer_name) > sizeof(*issuer_prefix) + sizeof(uuid) * 2);
	strcpy(issuer_name, issuer_prefix);
	char* pos = issuer_name + strlen(issuer_prefix);
	for (size_t i = 0; i < uuid_len; i++) {
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

    mbedtls_x509write_crt_set_subject_key( &crt, &loaded_issuer_key );
    mbedtls_x509write_crt_set_issuer_key( &crt, &loaded_issuer_key );

    ret = mbedtls_x509write_crt_set_issuer_name( &crt, issuer_name);
    ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_x509write_crt_set_issuer_name failed with 0x%08x", ret);

    ret = mbedtls_x509write_crt_set_subject_name( &crt, issuer_name);
    ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_x509write_crt_set_subject_name failed with 0x%08x", ret);

    mbedtls_x509write_crt_set_version( &crt, MBEDTLS_X509_CRT_VERSION_3 );

    mbedtls_md_type_t md_alg = MBEDTLS_MD_SHA256;
    if (mbedtls_pk_get_bitlen(&loaded_issuer_key) == 384)
    {
        md_alg = MBEDTLS_MD_SHA384;
    }
    mbedtls_x509write_crt_set_md_alg( &crt, md_alg);

    const uint8_t serial_bytes[1] = {1};
    ret = mbedtls_mpi_read_binary(&serial, serial_bytes, sizeof(serial_bytes));
    ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_mpi_read_binary failed with 0x%08x", ret);
    ret = mbedtls_x509write_crt_set_serial( &crt, &serial );
    ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_x509write_crt_set_serial failed with 0x%08x", ret);

    ret = mbedtls_x509write_crt_set_validity( &crt, "20220101000000", "99991231235959");
    ASSERT_OR_EXIT_MSG(ret == 0, "mbedtls_x509write_crt_set_validity failed with 0x%08x", ret);

    // Note: mbedtls_x509write_crt_der writes the cert to the end of the buffer!
    ret = mbedtls_x509write_crt_der(&crt, certificate_buffer, *certificate_buffer_size,
            mbedtls_ctr_drbg_random, &ctr_drbg);
    ASSERT_OR_EXIT_MSG(ret > 0, "mbedtls_x509write_crt_der failed with 0x%08x", ret);
    memmove(certificate_buffer, certificate_buffer + *certificate_buffer_size - ret, ret);
    *certificate_buffer_size = ret;

exit:
    mbedtls_x509write_crt_free( &crt );
    mbedtls_pk_free( &loaded_issuer_key );
    mbedtls_mpi_free( &serial );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );
    return agent_status;
}
#endif

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
#if SSS_HAVE_HOSTCRYPTO_OPENSSL
    int success = 1;
    unsigned int calculted_checksum_len = 0U;
    EVP_MD_CTX* digest_context = NULL;
#elif SSS_HAVE_HOSTCRYPTO_MBEDTLS
    int failed = 0U;
#endif
#if NXP_IOT_AGENT_HAVE_PSA
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
			service_descriptor.client_certificate_sss_ref.type = (nxp_iot_EndpointType)keystore->type;
			service_descriptor.client_certificate_sss_ref.has_endpoint_id = true;
			service_descriptor.client_certificate_sss_ref.endpoint_id = keystore->identifier;
			service_descriptor.client_certificate_sss_ref.has_object_id = true;
			service_descriptor.client_certificate_sss_ref.object_id = object_id;
		}
#if NXP_IOT_AGENT_HAVE_PSA
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

#if NXP_IOT_AGENT_VERIFY_EDGELOCK_2GO_SERVER_CERTIFICATE
	// Put the server certificates into the service descriptor.
	service_descriptor.server_certificate = (pb_bytes_array_t*) trusted_root_ca_certificates;
#endif

	// Serialize the service descriptor.
	pb_status = pb_encode_delimited(&stream, nxp_iot_ServiceDescriptor_fields, &service_descriptor);
	ASSERT_OR_EXIT_MSG(pb_status == true, "pb_encode_delimited for getting size failed.");
	encoded_size = stream.bytes_written;

	total_size = sizeof(configuration_data_header_t) + encoded_size;
	buffer = malloc(total_size);
	ostream = pb_ostream_from_buffer(buffer + sizeof(configuration_data_header_t), encoded_size);
	pb_status = pb_encode_delimited(&ostream, nxp_iot_ServiceDescriptor_fields, &service_descriptor);
	ASSERT_OR_EXIT_MSG(pb_status == true, "pb_encode_delimited failed.");

	// Create a header.
	header = (configuration_data_header_t*)buffer;
	memset(header, 0, sizeof(*header));
	header->length = (uint32_t)total_size;
	header->number_of_services = 1U;
	header->version = IOT_AGENT_CONFIGURATION_DATA_VERSION;

#if SSS_HAVE_HOSTCRYPTO_OPENSSL
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
#elif SSS_HAVE_HOSTCRYPTO_MBEDTLS
	mbedtls_sha256_context digest_context;
	mbedtls_sha256_init(&digest_context);
#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER < 0x03010000)
	mbedtls_sha256_starts_ret(&digest_context, 0U);
	failed |= mbedtls_sha256_update_ret(&digest_context, &buffer[sizeof(header->checksum)], total_size - sizeof(header->checksum));
	failed |= mbedtls_sha256_finish_ret(&digest_context, header->checksum);
#else
	mbedtls_sha256_starts(&digest_context, 0U);
	failed |= mbedtls_sha256_update(&digest_context, &buffer[sizeof(header->checksum)], total_size - sizeof(header->checksum));
	failed |= mbedtls_sha256_finish(&digest_context, header->checksum);
#endif
	ASSERT_OR_EXIT_MSG(failed == 0U, "Header checksum calculation failed.");
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

iot_agent_status_t iot_agent_utils_write_edgelock2go_datastore_from_env(iot_agent_keystore_t *keystore,
	iot_agent_datastore_t* datastore)
{
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    const char *hostname = EDGELOCK2GO_HOSTNAME;
    char *hostname_local = NULL;
    uint32_t port = EDGELOCK2GO_PORT;
    const char* edgelock2go_port_env = NULL;

	ASSERT_OR_EXIT_MSG(keystore != NULL, "keystore is NULL.");
	ASSERT_OR_EXIT_MSG(datastore != NULL, "datastore is NULL.");

    const char* edgelock2go_hostname_env = getenv("IOT_AGENT_TEST_EDGELOCK2GO_HOSTNAME");
    if (edgelock2go_hostname_env != NULL) {
        size_t len = strlen(edgelock2go_hostname_env);
        hostname_local = malloc(len + 1U);
        ASSERT_OR_EXIT(hostname_local!=NULL);
        memcpy(hostname_local, edgelock2go_hostname_env, len + 1U);
        hostname = hostname_local;
    }

    edgelock2go_port_env = getenv("IOT_AGENT_TEST_EDGELOCK2GO_PORT");

	if (edgelock2go_port_env != NULL) {
		port = (uint32_t)atoi(edgelock2go_port_env);
	}

    agent_status = iot_agent_utils_write_edgelock2go_datastore(keystore, datastore, hostname, port,
		iot_agent_trusted_root_ca_certificates, NULL);
    AGENT_SUCCESS_OR_EXIT();

exit:
    free(hostname_local);
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
        fclose(fd);
	}
exit:
	return agent_status;
}

#endif //!(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)

iot_agent_status_t iot_agent_utils_convert_service2key_id(uint64_t serviceId, uint32_t *keyId)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	ASSERT_OR_EXIT_MSG(keyId != NULL, "keyId is NULL.");

	if (serviceId > 0xFFFFFFFFUL)
	{
		return IOT_AGENT_FAILURE;
	}

	*keyId = EDGELOCK2GO_MANAGED_SERVICE_KEY_MIN | ((uint32_t) serviceId);

	if (*keyId > EDGELOCK2GO_MANAGED_SERVICE_KEY_MAX)
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

static size_t iot_agent_ceil_to_blocksize(size_t len, size_t blocksize)
{
    return ((len + (blocksize - 1)) / blocksize) * blocksize;
}

iot_agent_status_t nxp_iot_agent_unpad_iso7816d4(uint8_t *data, size_t *data_size) {
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	int count = *data_size -1;
	while (count > 0 && data[count] == 0) {
		count--;
	}

	ASSERT_OR_EXIT_MSG(data[count] == 0x80, "iso_7816_unpad failed, pad block corrupted");

	*data_size -= *data_size - count;
exit:
	return agent_status;
}

iot_agent_status_t iot_agent_pad_iso7816d4(uint8_t *data, size_t data_size,
        size_t unpadded_length, size_t blocksize, size_t* padded_length)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

    *padded_length = iot_agent_ceil_to_blocksize(
            unpadded_length + 1 /* always inserted padding 0x80 */, blocksize);

    ASSERT_OR_EXIT_MSG(data_size >= *padded_length, "Buffer size %u is too small to hold "
            "plaintext of length %u that will be padded up to length %u", data_size,
            unpadded_length, *padded_length);

    data[unpadded_length] = 0x80;
    memset(&data[unpadded_length + 1], 0, *padded_length - (unpadded_length + 1));

exit:
    return agent_status;
}
