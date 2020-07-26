/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Sentinel API Bridge (SAB) definition
 *
 */

#ifndef SAB_H__
#define SAB_H__

#include <stdint.h>

/*
 * The endianess convention in the whole API is litte endian
 * When a processor is not natively little endian, it is responsible
 * for proceeding with the appropriate conversions.
 */

/*
 * Data Plan Address (DPA) type
 * Cross processor architecture pointer type definition (32 and 64 bits architectures)
 * Pointers are used in the data plan only (payload address, result buffer address, ...
 */

typedef struct
{
    uint32_t lsb;
    uint32_t msb;
} sab_dpa_t;

typedef uint32_t sab_size_t;

typedef uint32_t sab_ctx_id_t;
typedef uint32_t sab_session_id_t;

typedef uint32_t sab_algorithm_t;
typedef uint32_t sab_mode_t;
typedef uint32_t sab_se_type_t;

typedef uint8_t sab_command_t;
typedef uint8_t sab_ranking_t;
typedef uint8_t sab_rating_t;

/*sab_status_t ranking*/

#define SAB_SUCCESS ((sab_ranking_t)0x3C)
#define SAB_FAILURE ((sab_ranking_t)0xC3)

#define SAB_BLOCKING_CONTEXT ((sab_rating_t)0x10)
#define SAB_NO_PENDING_STATUS ((sab_rating_t)0x11)
#define SAB_INVALID_ARGUMENT ((sab_rating_t)0x12)
#define SAB_RESOURCE_BUSY ((sab_rating_t)0x13)
#define SAB_UNKNOWN_SESSION ((sab_rating_t)0x14)
#define SAB_UNKNOWN_CONTEXT ((sab_rating_t)0x15)
#define SAB_UNKNOWN_COMMAND ((sab_rating_t)0x16)
#define SAB_UNKNOWN_LIFECYCLE ((sab_rating_t)0x17)

typedef struct
{
    sab_ranking_t ranking; /* Success / failure*/
    sab_rating_t rating;   /* Error code or status indication*/
    uint8_t size;          /* Nb of words including the status word in the command indication*/
    sab_command_t command; /* Identify the original command*/
} sab_status_t;

typedef uint32_t sab_object_t;
typedef uint32_t sab_key_store_t;

typedef uint32_t session_cfg_t;

#define SAP_SESSION_PRIO_NB_BITS 2
#define SAP_SESSION_PRIO_MSK ((1 << SAP_SESSION_PRIO_NB_BITS) - 1)
#define SAP_SESSION_PRIO_POS 0

#define SAP_SESSION_DID_NB_BITS 4
#define SAP_SESSION_DID_MSK ((1 << SAP_SESSION_DID_NB_BITS) - 1)
#define SAP_SESSION_DID_POS 2

#define SAP_SESSION_CFG_SIZE 1

/* To avoid warning for empty structure which is a placeholder
 * the pedantic flag is disabled for this section
 */
/*
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

// Session management
typedef struct {
    sab_se_type_t subsystem;
    session_cfg_t session_cfg;
    uint32_t connectionMethod;
    uint32_t connectionData [SAP_SESSION_CFG_SIZE];
} sab_open_session_req_t;

typedef struct {
    sab_session_id_t session;
} sab_open_session_ind_t;

typedef struct {
    sab_session_id_t session;
} sab_close_session_req_t;

typedef struct {
} sab_close_session_ind_t;


// sab_crypto_symmetric
typedef struct {
    sab_session_id_t session;
    sab_object_t keyObject;    // TBD
    sab_algorithm_t algorithm;
    sab_mode_t mode;
} sab_symmetric_context_init_req_t;

typedef struct {
    sab_ctx_id_t context;
} sab_symmetric_context_init_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t srcData;
    sab_dpa_t destData;
    sab_size_t size;
    uint8_t initialCounter;
    uint8_t pad [3];
} sab_cipher_crypt_ctr_req_t;

typedef struct {
    sab_size_t szLeft;
    uint8_t initialCounter;
    uint8_t lastEncryptedCounter;
    uint8_t pad [2];
} sab_cipher_crypt_ctr_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t srcData;
    sab_size_t srcLen;
    sab_dpa_t destData;
    sab_size_t destLen;
} sab_cipher_finish_req_t;

typedef struct {
    sab_size_t destLen;
} sab_cipher_finish_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t iv;
    sab_size_t ivLen;
} sab_cipher_init_req_t;

typedef struct {
} sab_cipher_init_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t iv;
    sab_size_t ivLen;
    const sab_dpa_t srcData;
    sab_dpa_t destData;
    sab_size_t dataLen;
} sab_cipher_one_go_req_t;

typedef struct {
} sab_cipher_one_go_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t srcData;
    sab_size_t srcLen;
    sab_dpa_t destData;
    sab_size_t destLen;
} sab_cipher_update_req_t;

typedef struct {
    sab_size_t destLen;
} sab_cipher_update_ind_t;

// sab_crypto_aead

typedef struct {
    sab_session_id_t session;
    sab_object_t keyObject;     // TBD
    sab_algorithm_t algorithm;
    sab_mode_t mode;
} sab_aead_context_init_req_t;

typedef struct {
    sab_ctx_id_t context;
} sab_aead_context_init_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t srcData;
    sab_size_t srcLen;
    sab_dpa_t destData;
    sab_size_t destLen;
    sab_dpa_t tag;    // TBC
    sab_size_t tagLen;
} sab_aead_finish_req_t;

typedef struct {
    sab_size_t destLen;
    sab_size_t tagLen;
} sab_aead_finish_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t nonce;
    sab_size_t nonceLen;
    sab_size_t tagLen;
    sab_size_t aadLen;
    sab_size_t payloadLen;
} sab_aead_init_req_t;

typedef struct {
} sab_aead_init_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t srcData;
    sab_dpa_t destData;
    sab_size_t size;
    sab_dpa_t nonce;
    sab_size_t nonceLen;
    const sab_dpa_t aad;
    sab_size_t aadLen;
    sab_dpa_t tag;
    sab_size_t tagLen;
} sab_aead_one_go_req_t;

typedef struct {
    sab_size_t tagLen;
} sab_aead_one_go_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t srcData;
    sab_size_t srcLen;
    sab_dpa_t destData;
    sab_size_t destLen;
} sab_aead_update_req_t;

typedef struct {
    sab_size_t destLen;
} sab_aead_update_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t aadData;
    sab_size_t aadDataLen;
} sab_aead_update_aad_req_t;

typedef struct {
} sab_aead_update_aad_ind_t;

// sab_crypto_digest

typedef struct {
    sab_session_id_t session;
    sab_algorithm_t algorithm;
    sab_mode_t mode;
} sab_digest_context_init_req_t;

typedef struct {
    sab_ctx_id_t context;
} sab_digest_context_init_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t digest;
    sab_size_t digestLen;
} sab_digest_finish_req_t;

typedef struct {
    sab_size_t digestLen;
} sab_digest_finish_ind_t;

typedef struct {
    sab_ctx_id_t context;
} sab_digest_init_req_t;

typedef struct {
} sab_digest_init_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t message;
    sab_size_t messageLen;
    sab_dpa_t digest;
    sab_size_t digestLen;
} sab_digest_one_go_req_t;

typedef struct {
    sab_size_t digestLen;
} sab_digest_one_go_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t message;
    sab_size_t messageLen;
} sab_digest_update_req_t;

typedef struct {
} sab_digest_update_ind_t;

// sab_crypto_mac

typedef struct {
    sab_session_id_t session;
    sab_object_t keyObject;     // TBD
    sab_algorithm_t algorithm;
    sab_mode_t mode;
} sab_mac_context_init_req_t;

typedef struct {
    sab_ctx_id_t context;
} sab_mac_context_init_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t mac;
    sab_size_t macLen;
} sab_mac_finish_req_t;

typedef struct {
    sab_size_t macLen;
} sab_mac_finish_ind_t;

typedef struct {
    sab_ctx_id_t context;
} sab_mac_init_req_t;          // TBC

typedef struct {
} sab_mac_init_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t message;
    sab_size_t messageLen;
    sab_dpa_t mac;
    sab_size_t macLen;
} sab_mac_one_go_req_t;

typedef struct {
    sab_size_t macLen;
} sab_mac_one_go_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t message;
    sab_size_t messageLen;
} sab_mac_update_req_t;

typedef struct {
} sab_mac_update_ind_t;

// sab_crypto_asymmetric

typedef struct {
    sab_session_id_t session;
    sab_object_t keyObject;    // TBD
    sab_algorithm_t algorithm;
    sab_mode_t mode;
} sab_asymmetric_context_init_req_t;

typedef struct {
    sab_ctx_id_t context;
} sab_asymmetric_context_init_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t srcData;
    sab_size_t srcLen;
    sab_dpa_t destData;
    sab_size_t destLen;
} sab_asymmetric_decrypt_req_t;

typedef struct {
    sab_size_t destLen;
} sab_asymmetric_decrypt_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t srcData;
    sab_size_t srcLen;
    sab_dpa_t destData;
    sab_size_t destLen;
} sab_asymmetric_encrypt_req_t;

typedef struct {
    sab_size_t destLen;
} sab_asymmetric_encrypt_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t digest;
    sab_size_t digestLen;
    sab_dpa_t signature;
    sab_size_t signatureLen;
} sab_asymmetric_sign_digest_req_t;

typedef struct {
    sab_size_t signatureLen;
} sab_asymmetric_sign_digest_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t digest;
    sab_size_t digestLen;
    sab_dpa_t signature;
    sab_size_t signatureLen;
} sab_asymmetric_verify_digest_req_t;

typedef struct {
} sab_asymmetric_verify_digest_ind_t;

// sab_crypto_tunnelling

typedef struct {
    sab_session_id_t session;
} sab_init_tunnel_context_req_t;

typedef struct {
    sab_ctx_id_t context;
} sab_init_tunnel_context_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t data;
    sab_size_t dataLen;
    sab_object_t keyObjects;    // TBD
    uint32_t keyObjectCount;
    uint32_t tunnelType;
} sab_tunnel_req_t;

typedef struct {
} sab_tunnel_ind_t;

// sab_crypto_derive_key

typedef struct {
    sab_ctx_id_t context;
    sab_object_t otherPartyKeyObject;   // TBD
} sab_asymmetric_dh_derive_key_req_t;

typedef struct {
    sab_object_t derivedKeyObject;      // TBD
} sab_asymmetric_dh_derive_key_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t saltData;
    sab_size_t saltLen;
} sab_derive_key_req_t;

typedef struct {
    sab_object_t derivedKeyObject;   // TBD
} sab_derive_key_ind_t;

typedef struct {
    sab_session_id_t session;
    sab_object_t keyObject;      // TBD
    sab_algorithm_t algorithm;
    sab_mode_t mode;
} sab_derive_key_context_init_req_t;

typedef struct {
    sab_ctx_id_t context;
} sab_derive_key_context_init_ind_t;

// sab_key_object

typedef struct {
    sab_key_store_t keyStore;
} sab_key_object_init_req_t;

typedef struct {
    sab_object_t keyObject;
} sab_key_object_init_ind_t;

typedef struct {
    sab_object_t keyObject;
    uint32_t keyId;
    uint32_t keyType;
    size_t keyByteLenMax;
    uint32_t options;
} sab_key_object_allocate_handle_req_t;

typedef struct {
} sab_key_object_allocate_handle_ind_t;

typedef struct {
    sab_object_t keyObject;
    uint32_t access;
    uint32_t options;
} sab_key_object_set_access_req_t;

typedef struct {
} sab_key_object_set_access_ind_t;

typedef struct {
    sab_object_t keyObject;
    sab_mode_t purpose;
    uint32_t options;
} sab_key_object_set_purpose_req_t;

typedef struct {
} sab_key_object_set_purpose_ind_t;

typedef struct {
    sab_object_t keyObject;
    uint32_t user;
    uint32_t options;
} sab_key_object_set_user_req_t;

typedef struct {
} sab_key_object_set_user_ind_t;

// sab_key_store

typedef struct {
    uint32_t keyStoreId;
} sab_key_store_allocate_req_t;

typedef struct {
    sab_key_store_t keyStore;
} sab_key_store_allocate_ind_t;

typedef struct {
    sab_session_id_t session;
    sab_key_store_t keyStore;
} sab_key_store_context_init_req_t;

typedef struct {
    sab_key_store_t keyStore;
} sab_key_store_context_init_ind_t;

typedef struct {
    sab_key_store_t keyStore;
    sab_object_t keyObject;
    sab_dpa_t key;
    size_t keyBitLen;
    uint32_t options;
} sab_key_store_set_key_req_t;

typedef struct {
} sab_key_store_set_key_ind_t;

typedef struct {
    sab_key_store_t keyStore;
    sab_object_t keyObject;
    size_t keyBitLen;
    uint32_t options;
} sab_key_store_generate_key_req_t;

typedef struct {
} sab_key_store_generate_key_ind_t;

typedef struct {
    sab_key_store_t keyStore;
    sab_object_t keyObject;
    sab_dpa_t key;
    size_t keyBitLen;
} sab_key_store_get_key_req_t;

typedef struct {
} sab_key_store_get_key_ind_t;

typedef struct {
    sab_key_store_t keyStore;
    sab_object_t keyObject;
} sab_key_store_open_key_req_t;

typedef struct {
} sab_key_store_open_key_ind_t;

typedef struct {
    sab_key_store_t keyStore;
    sab_object_t keyObject;
} sab_key_store_freeze_key_req_t;

typedef struct {
} sab_key_store_freeze_key_ind_t;

typedef struct {
    sab_key_store_t keyStore;
    sab_object_t keyObject;
} sab_key_store_erase_key_req_t;

typedef struct {
} sab_key_store_erase_key_ind_t;

typedef struct {
    sab_session_id_t session;
} sab_mgmt_context_init_req_t;

typedef struct {
    sab_ctx_id_t context;
} sab_mgmt_context_init_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t destData;
    sab_size_t dataLen;
} sab_mgmt_attest_req_t;

typedef struct {
    sab_size_t dataLen;
} sab_mgmt_attest_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t destData;
    sab_size_t dataLen;
} sab_mgmt_blob_export_secret_req_t;

typedef struct {
    sab_size_t dataLen;
} sab_mgmt_blob_export_secret_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t srcData;
    sab_size_t dataLen;
} sab_mgmt_blob_import_secret_req_t;

typedef struct {
} sab_mgmt_blob_import_secret_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t srcData;
    sab_size_t dataLen;
} sab_mgmt_blob_load_secret_req_t;

typedef struct {
} sab_mgmt_blob_load_secret_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t destData;
    sab_size_t dataLen;
} sab_mgmt_export_secret_req_t;

typedef struct {
    sab_size_t dataLen;
} sab_mgmt_export_secret_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t  fuseId;
    sab_dpa_t  srcData;
    void *options;
    sab_size_t *optionsLen;
} sab_mgmt_fuse_program_req_t;

typedef struct {
} sab_mgmt_fuse_program_ind_t;

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t  fuseId;
    sab_dpa_t destData;
} sab_mgmt_fuse_read_req_t;

typedef struct {
} sab_mgmt_fuse_read_ind_t;

typedef struct {
    sab_ctx_id_t context;
    uint32_t lifecycleData;
} sab_mgmt_advance_lifecycle_req_t;

typedef struct {
} sab_mgmt_advance_lifecycle_ind_t;

typedef struct {
    sab_ctx_id_t context;
} sab_mgmt_get_lifecycle_req_t;

typedef struct {
    uint32_t lifecycleData;
} sab_mgmt_get_lifecycle_ind_t;

typedef struct {
    sab_ctx_id_t context;
    uint32_t  propertyId;
    sab_dpa_t destData;
    sab_size_t dataLen;
} sab_mgmt_get_property_req_t;

typedef struct {
    sab_size_t dataLen;
} sab_mgmt_get_property_ind_t;

#define SAB_SS_VERSION_NB_WORDS 2
#define SAB_NS_VERSION_NB_WORDS 8

typedef struct {
    sab_ctx_id_t context;
    sab_dpa_t version;
    uint32_t  versionWordCount;
} sab_mgmt_get_software_version_req_t;

typedef struct {
} sab_mgmt_get_software_version_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t srcData;
    sab_size_t dataLen;
} sab_mgmt_import_secret_req_t;

typedef struct {
} sab_mgmt_import_secret_ind_t;

typedef struct {
    sab_ctx_id_t context;
} sab_mgmt_integrity_check_enable_req_t;

typedef struct {
} sab_mgmt_integrity_check_enable_ind_t;

typedef struct {
    sab_ctx_id_t context;
    const sab_dpa_t srcData;
    sab_size_t dataLen;
} sab_mgmt_set_host_access_permission_req_t;

typedef struct {
} sab_mgmt_set_host_access_permission_ind_t;

typedef struct {
    sab_ctx_id_t context;
    uint32_t  propertyId;
    const sab_dpa_t srcData;
    sab_size_t dataLen;
} sab_mgmt_set_property_req_t;

typedef struct {
} sab_mgmt_set_property_ind_t;

typedef struct {
    sab_ctx_id_t context;
} sab_mgmt_set_return_fa_req_t;

typedef struct {
} sab_mgmt_set_return_fa_ind_t;

typedef struct {
} sab_get_status_ind_t;

typedef struct {
} sab_mgmt_ping_ind_t;

typedef struct {
    sab_ctx_id_t context;
} sab_context_free_req_t;

typedef struct {
} sab_context_free_ind_t;

#pragma GCC diagnostic pop

typedef struct {
    sab_status_t status;

    union _rsp {
        sab_cipher_crypt_ctr_ind_t sab_cipher_crypt_ctr_ind;
        sab_cipher_finish_ind_t sab_cipher_finish_ind;
        sab_cipher_init_ind_t sab_cipher_init_ind;
        sab_cipher_one_go_ind_t sab_cipher_one_go_ind;
        sab_cipher_update_ind_t sab_cipher_update_ind;
        sab_symmetric_context_init_ind_t sab_symmetric_context_init_ind;
        sab_aead_context_init_ind_t sab_aead_context_init_ind;
        sab_aead_finish_ind_t sab_aead_finish_ind;
        sab_aead_init_ind_t sab_aead_init_ind;
        sab_aead_one_go_ind_t sab_aead_one_go_ind;
        sab_aead_update_ind_t sab_aead_update_ind;
        sab_aead_update_aad_ind_t sab_aead_update_aad_ind;
        sab_digest_context_init_ind_t sab_digest_context_init_ind;
        sab_digest_finish_ind_t sab_digest_finish_ind;
        sab_digest_init_ind_t sab_digest_init_ind;
        sab_digest_one_go_ind_t sab_digest_one_go_ind;
        sab_digest_update_ind_t sab_digest_update_ind;
        sab_mac_context_init_ind_t sab_mac_context_init_ind;
        sab_mac_finish_ind_t sab_mac_finish_ind;
        sab_mac_init_ind_t sab_mac_init_ind;
        sab_mac_one_go_ind_t sab_mac_one_go_ind;
        sab_mac_update_ind_t sab_mac_update_ind;
        sab_asymmetric_context_init_ind_t sab_asymmetric_context_init_ind;
        sab_asymmetric_decrypt_ind_t sab_asymmetric_decrypt_ind;
        sab_asymmetric_encrypt_ind_t sab_asymmetric_encrypt_ind;
        sab_asymmetric_sign_digest_ind_t sab_asymmetric_sign_digest_ind;
        sab_asymmetric_verify_digest_ind_t sab_asymmetric_verify_digest_ind;
        sab_init_tunnel_context_ind_t sab_init_tunnel_context_ind;
        sab_tunnel_ind_t sab_tunnel_ind;
        sab_asymmetric_dh_derive_key_ind_t sab_asymmetric_dh_derive_key_ind;
        sab_derive_key_ind_t sab_derive_key_ind;
        sab_derive_key_context_init_ind_t sab_derive_key_context_init_ind;
        sab_key_object_init_ind_t sab_key_object_init_ind;
        sab_key_object_set_access_ind_t sab_key_object_set_access_ind;
        sab_key_object_set_purpose_ind_t sab_key_object_set_purpose_ind;
        sab_key_object_set_user_ind_t sab_key_object_set_user_ind;
        sab_key_store_allocate_ind_t sab_key_store_allocate_ind;
        sab_key_store_context_init_ind_t sab_key_store_context_init_ind;
        sab_open_session_ind_t sab_open_session_ind;
        sab_close_session_ind_t sab_close_session_ind;
        sab_mgmt_advance_lifecycle_ind_t sab_mgmt_advance_lifecycle_ind;
        sab_mgmt_attest_ind_t sab_mgmt_attest_ind;
        sab_mgmt_blob_export_secret_ind_t sab_mgmt_blob_export_secret_ind;
        sab_mgmt_blob_import_secret_ind_t sab_mgmt_blob_import_secret_ind;
        sab_mgmt_blob_load_secret_ind_t sab_mgmt_blob_load_secret_ind;
        sab_mgmt_context_init_ind_t sab_mgmt_context_init_ind;
        sab_mgmt_export_secret_ind_t sab_mgmt_export_secret_ind;
        sab_mgmt_fuse_program_ind_t sab_mgmt_fuse_program_ind;
        sab_mgmt_fuse_read_ind_t sab_mgmt_fuse_read_ind;
        sab_mgmt_get_lifecycle_ind_t sab_mgmt_get_lifecycle_ind;
        sab_mgmt_get_property_ind_t sab_mgmt_get_property_ind;
        sab_mgmt_get_software_version_ind_t sab_mgmt_get_software_version_ind;
        sab_mgmt_import_secret_ind_t sab_mgmt_import_secret_ind;
        sab_mgmt_integrity_check_enable_ind_t sab_mgmt_integrity_check_enable_ind;
        sab_mgmt_set_host_access_permission_ind_t sab_mgmt_set_host_access_permission_ind;
        sab_mgmt_set_property_ind_t sab_mgmt_set_property_ind;
        sab_mgmt_set_return_fa_ind_t sab_mgmt_set_return_fa_ind;
        sab_mgmt_ping_ind_t sab_mgmt_ping_ind;
        sab_context_free_ind_t sab_context_free_ind;
    } rsp;

} sab_status_ind_t;
*/

/* SAB command definitions */
#define SAB_PING_REQ ((sab_command_t)0x11)
#define SAB_GET_STATUS_REQ ((sab_command_t)0x12)
#define SAB_OPEN_SESSION_REQ ((sab_command_t)0x13)
#define SAB_CLOSE_SESSION_REQ ((sab_command_t)0x14)
#define SAB_CONTEXT_FREE_REQ ((sab_command_t)0x15)
#define SAB_CIPHER_CRYPT_CTR_REQ ((sab_command_t)0x20)
#define SAB_CIPHER_FINISH_REQ ((sab_command_t)0x21)
#define SAB_CIPHER_INIT_REQ ((sab_command_t)0x22)
#define SAB_CIPHER_ONE_GO_REQ ((sab_command_t)0x23)
#define SAB_CIPHER_UPDATE_REQ ((sab_command_t)0x24)
#define SAB_SYMMETRIC_CONTEXT_INIT_REQ ((sab_command_t)0x25)
#define SAB_AEAD_CONTEXT_INIT_REQ ((sab_command_t)0x26)
#define SAB_AEAD_FINISH_REQ ((sab_command_t)0x27)
#define SAB_AEAD_INIT_REQ ((sab_command_t)0x28)
#define SAB_AEAD_ONE_GO_REQ ((sab_command_t)0x29)
#define SAB_AEAD_UPDATE_REQ ((sab_command_t)0x2a)
#define SAB_AEAD_UPDATE_AAD_REQ ((sab_command_t)0x2b)
#define SAB_DIGEST_CONTEXT_INIT_REQ ((sab_command_t)0x2c)
#define SAB_DIGEST_FINISH_REQ ((sab_command_t)0x2d)
#define SAB_DIGEST_INIT_REQ ((sab_command_t)0x2e)
#define SAB_DIGEST_ONE_GO_REQ ((sab_command_t)0x2f)
#define SAB_DIGEST_UPDATE_REQ ((sab_command_t)0x30)
#define SAB_DIGEST_UPDATE_NONBLOCKING_REQ ((sab_command_t)0x31)
#define SAB_MAC_CONTEXT_INIT_REQ ((sab_command_t)0x32)
#define SAB_MAC_FINISH_REQ ((sab_command_t)0x33)
#define SAB_MAC_INIT_REQ ((sab_command_t)0x34)
#define SAB_MAC_ONE_GO_REQ ((sab_command_t)0x35)
#define SAB_MAC_UPDATE_REQ ((sab_command_t)0x36)
#define SAB_ASYMMETRIC_CONTEXT_INIT_REQ ((sab_command_t)0x37)
#define SAB_ASYMMETRIC_DECRYPT_REQ ((sab_command_t)0x38)
#define SAB_ASYMMETRIC_ENCRYPT_REQ ((sab_command_t)0x39)
#define SAB_ASYMMETRIC_SIGN_DIGEST_REQ ((sab_command_t)0x3a)
#define SAB_ASYMMETRIC_VERIFY_DIGEST_REQ ((sab_command_t)0x3b)
#define SAB_ASYMMETRIC_DH_DERIVE_KEY_REQ ((sab_command_t)0x3c)
#define SAB_TUNNEL_CONTEXT_INIT_REQ ((sab_command_t)0x3d)
#define SAB_TUNNEL_REQ ((sab_command_t)0x3e)
#define SAB_DERIVE_KEY_REQ ((sab_command_t)0x3f)
#define SAB_DERIVE_KEY_CONTEXT_INIT_REQ ((sab_command_t)0x40)
#define SAB_KEY_OBJECT_INIT_REQ ((sab_command_t)0x41)
#define SAB_KEY_OBJECT_ALLOCATE_HANDLE_REQ ((sab_command_t)0x42)
#define SAB_KEY_OBJECT_GET_HANDLE_REQ ((sab_command_t)0x43)
#define SAB_KEY_OBJECT_SET_ACCESS_REQ ((sab_command_t)0x44)
#define SAB_KEY_OBJECT_SET_PURPOSE_REQ ((sab_command_t)0x45)
#define SAB_KEY_OBJECT_SET_USER_REQ ((sab_command_t)0x46)
#define SAB_KEY_OBJECT_FREE_REQ ((sab_command_t)0x47)
#define SAB_KEY_STORE_ALLOCATE_REQ ((sab_command_t)0x48)
#define SAB_KEY_STORE_CONTEXT_INIT_REQ ((sab_command_t)0x49)
#define SAB_KEY_STORE_SAVE_REQ ((sab_command_t)0x4a)
#define SAB_KEY_STORE_LOAD_REQ ((sab_command_t)0x4b)
#define SAB_KEY_STORE_SET_KEY_REQ ((sab_command_t)0x4c)
#define SAB_KEY_STORE_GENERATE_KEY_REQ ((sab_command_t)0x4d)
#define SAB_KEY_STORE_GET_KEY_REQ ((sab_command_t)0x4e)
#define SAB_KEY_STORE_OPEN_KEY_REQ ((sab_command_t)0x4f)
#define SAB_KEY_STORE_FREEZE_KEY_REQ ((sab_command_t)0x50)
#define SAB_KEY_STORE_ERASE_KEY_REQ ((sab_command_t)0x51)
#define SAB_MGMT_ADVANCE_LIFECYCLE_REQ ((sab_command_t)0x60)
#define SAB_MGMT_ATTEST_REQ ((sab_command_t)0x61)
#define SAB_MGMT_BLOB_EXPORT_SECRET_REQ ((sab_command_t)0x62)
#define SAB_MGMT_BLOB_IMPORT_SECRET_REQ ((sab_command_t)0x63)
#define SAB_MGMT_BLOB_LOAD_SECRET_REQ ((sab_command_t)0x64)
#define SAB_MGMT_CONTEXT_INIT_REQ ((sab_command_t)0x65)
#define SAB_MGMT_EXPORT_SECRET_REQ ((sab_command_t)0x66)
#define SAB_MGMT_FUSE_PROGRAM_REQ ((sab_command_t)0x67)
#define SAB_MGMT_FUSE_READ_REQ ((sab_command_t)0x68)
#define SAB_MGMT_FUSE_SHADOW_PROGRAM_REQ ((sab_command_t)0x69)
#define SAB_MGMT_FUSE_SHADOW_READ_REQ ((sab_command_t)0x6a)
#define SAB_MGMT_GET_LIFECYCLE_REQ ((sab_command_t)0x6b)
#define SAB_MGMT_GET_PROPERTY_REQ ((sab_command_t)0x6c)
#define SAB_MGMT_GET_SOFTWARE_VERSION_REQ ((sab_command_t)0x6d)
#define SAB_MGMT_IMPORT_SECRET_REQ ((sab_command_t)0x6e)
#define SAB_MGMT_INTEGRITY_CHECK_ENABLE_REQ ((sab_command_t)0x6f)
#define SAB_MGMT_SET_HOST_ACCESS_PERMISSION_REQ ((sab_command_t)0x70)
#define SAB_MGMT_SET_PROPERTY_REQ ((sab_command_t)0x71)
#define SAB_MGMT_SET_RETURN_FA_REQ ((sab_command_t)0x72)
#define SAB_MGMT_GET_RANDOM_REQ ((sab_command_t)0x73)
#define SAB_MGMT_CLEAR_ALL_KEYS_REQ ((sab_command_t)0x74)
#define SAB_MGMT_MBIST_PREPARE_REQ ((sab_command_t)0x75)

/*
 * SAB algorithm definition
 */
/* AES */
#define SAB_ALGO_AES_ECB ((sab_algorithm_t)0x00)
#define SAB_ALGO_AES_CBC ((sab_algorithm_t)0x01)
#define SAB_ALGO_AES_CTR ((sab_algorithm_t)0x02)
#define SAB_ALGO_AES_GCM ((sab_algorithm_t)0x03)
#define SAB_ALGO_AES_CCM ((sab_algorithm_t)0x04)
/* CHACHA_POLY */
#define SAB_ALGO_CHACHA_POLY ((sab_algorithm_t)0x05)
/* DES3 */
#define SAB_ALGO_DES3_ECB ((sab_algorithm_t)0x06)
#define SAB_ALGO_DES3_CBC ((sab_algorithm_t)0x07)
/* digest */
#define SAB_ALGO_SHA1 ((sab_algorithm_t)0x08)
#define SAB_ALGO_SHA224 ((sab_algorithm_t)0x09)
#define SAB_ALGO_SHA256 ((sab_algorithm_t)0x0A)
#define SAB_ALGO_SHA384 ((sab_algorithm_t)0x0B)
#define SAB_ALGO_SHA512 ((sab_algorithm_t)0x0C)
/* MAC */
#define SAB_ALGO_CMAC_AES ((sab_algorithm_t)0x0D)
#define SAB_ALGO_HMAC_SHA256 ((sab_algorithm_t)0x0E)
/* Diffie-Helmann */
#define SAB_ALGO_DH ((sab_algorithm_t)0x0F)
#define SAB_ALGO_ECDH ((sab_algorithm_t)0x10)
/* DSA */
#define SAB_ALGO_DSA_SHA1 ((sab_algorithm_t)0x11)
#define SAB_ALGO_DSA_SHA224 ((sab_algorithm_t)0x12)
#define SAB_ALGO_DSA_SHA256 ((sab_algorithm_t)0x13)
/* RSA */
#define SAB_ALGO_RSASSA_PKCS1_V1_5_SHA1 ((sab_algorithm_t)0x14)
#define SAB_ALGO_RSASSA_PKCS1_V1_5_SHA224 ((sab_algorithm_t)0x15)
#define SAB_ALGO_RSASSA_PKCS1_V1_5_SHA256 ((sab_algorithm_t)0x16)
#define SAB_ALGO_RSASSA_PKCS1_V1_5_SHA384 ((sab_algorithm_t)0x17)
#define SAB_ALGO_RSASSA_PKCS1_V1_5_SHA512 ((sab_algorithm_t)0x18)
#define SAB_ALGO_RSASSA_PKCS1_PSS_MGF1_SHA1 ((sab_algorithm_t)0x19)
#define SAB_ALGO_RSASSA_PKCS1_PSS_MGF1_SHA224 ((sab_algorithm_t)0x1A)
#define SAB_ALGO_RSASSA_PKCS1_PSS_MGF1_SHA256 ((sab_algorithm_t)0x1B)
#define SAB_ALGO_RSASSA_PKCS1_PSS_MGF1_SHA384 ((sab_algorithm_t)0x1C)
#define SAB_ALGO_RSASSA_PKCS1_PSS_MGF1_SHA512 ((sab_algorithm_t)0x1D)
/* ECDSA */
#define SAB_ALGO_ECDSA_SHA1 ((sab_algorithm_t)0x1E)
#define SAB_ALGO_ECDSA_SHA224 ((sab_algorithm_t)0x1F)
#define SAB_ALGO_ECDSA_SHA256 ((sab_algorithm_t)0x20)
#define SAB_ALGO_ECDSA_SHA384 ((sab_algorithm_t)0x21)
#define SAB_ALGO_ECDSA_SHA512 ((sab_algorithm_t)0x22)

/*SAB mode definitions. Default is blocking calls*/

#define SAB_NON_BLOCKING_FLAG (1U << 7)

#define SAB_MODE_ENCRYPT ((sab_mode_t)0x00)
#define SAB_MODE_DECRYPT ((sab_mode_t)0x01)
#define SAB_MODE_SIGN ((sab_mode_t)0x02)
#define SAB_MODE_VERIFY ((sab_mode_t)0x03)
#define SAB_MODE_COMPUTE_SHARED_SECRET ((sab_mode_t)0x04)
#define SAB_MODE_DIGEST ((sab_mode_t)0x05)
#define SAB_MODE_MAC ((sab_mode_t)0x06)

/*SAB mode non blocking definitions*/
#define SAB_MODE_ENCRYPT_NB ((sab_mode_t)(SAB_NON_BLOCKING_FLAG | SAB_MODE_ENCRYPT))
#define SAB_MODE_DECRYPT_NB ((sab_mode_t)(SAB_NON_BLOCKING_FLAG | SAB_MODE_DECRYPT))
#define SAB_MODE_SIGN_NB ((sab_mode_t)(SAB_NON_BLOCKING_FLAG | SAB_MODE_SIGN))
#define SAB_MODE_VERIFY_NB ((sab_mode_t)(SAB_NON_BLOCKING_FLAG | SAB_MODE_VERIFY))
#define SAB_MODE_COMPUTE_SHARED_SECRET_NB ((sab_mode_t)(SAB_NON_BLOCKING_FLAG | SAB_MODE_COMPUTE_SHARED_SECRET))
#define SAB_MODE_DIGEST_NB ((sab_mode_t)(SAB_NON_BLOCKING_FLAG | SAB_MODE_DIGEST))
#define SAB_MODE_MAC_NB ((sab_mode_t)(SAB_NON_BLOCKING_FLAG | SAB_MODE_MAC))

/*SAB Security Enclave Type definitions*/
#define SAB_SE_TYPE_Software ((sab_se_type_t)0x00)
#define SAB_SE_TYPE_SECO ((sab_se_type_t)0x01)
#define SAB_SE_TYPE_S200 ((sab_se_type_t)0x02)
#define SAB_SE_TYPE_S300 ((sab_se_type_t)0x03)
#define SAB_SE_TYPE_S400 ((sab_se_type_t)0x04)
#define SAB_SE_TYPE_S500 ((sab_se_type_t)0x05)
#define SAB_SE_TYPE_SECURE_ELEMENT ((sab_se_type_t)0x06)

/* SAB Key Type definitions */
#define SAB_KEY_TYPE_SYMMETRIC (0x00)
#define SAB_KEY_TYPE_ASYMMETRIC (0x01)

/* SAB Fuse Id definitions */
#define SAB_FUSEID_TESTER_LOCK (0x00)
#define SAB_FUSEID_OTFAD_KEK_LOCK (0x01)
#define SAB_FUSEID_DUK0_LOCK (0x02)
#define SAB_FUSEID_CTRK_LOCK (0x03)
#define SAB_FUSEID_MTRK_LOCK (0x04)
#define SAB_FUSEID_KEY_MASK_LOCK (0x05)
#define SAB_FUSEID_DCFG_CC_SOCU_L1_LOCK (0x06)
#define SAB_FUSEID_DCFG_CC_SOCU_L2_LOCK (0x07)
#define SAB_FUSEID_FW_UPD_KEK_LOCK (0x08)
#define SAB_FUSEID_OTFAD_KEK (0x09)
#define SAB_FUSEID_DUK0 (0x0A)
#define SAB_FUSEID_DTRK (0x0B)
#define SAB_FUSEID_SEC_FW_KEK (0x0C)
#define SAB_FUSEID_FW_UPD_KEK (0x0D)
#define SAB_FUSEID_KEY_MASK (0x0E)
#define SAB_FUSEID_WRITE_ONLY (0x0F)
#define SAB_FUSEID_LIFECYCLE (0x0F)
#define SAB_FUSEID_DBG_AUTH_VU (0x10)
#define SAB_FUSEID_IMG_KEY_REVOKE (0x11)
#define SAB_FUSEID_UUID (0x12)
#define SAB_FUSEID_CTRK (0x13)
#define SAB_FUSEID_MTRK (0x14)
#define SAB_FUSEID_DCFG_CC_SOCU_L1 (0x15)
#define SAB_FUSEID_DCFG_CC_SOCU_L2 (0x16)
#define SAB_FUSEID_SS_VER_CNT (0x17)
#define SAB_FUSEID_NS_VER_CNT (0x18)
#define SAB_FUSEID_LAST (0x19)

/* NBOOT special keys */
/*
#define NBOOT_DUK0       (0x80000001U)
#define NBOOT_OTFAD_KEK  (0x80000002U)
#define NBOOT_SEC_FW_KEK (0x80000003U)
#define NBOOT_FW_UPD_KEK (0x80000004U)
#define NBOOT_DTRK       (0x80000005U)
*/

/* get/set properties defines */
#define SAB_MGMT_PROPERTY_ID_DICE_CDI (0x10)
#define SAB_MGMT_PROPERTY_ID_IMAGE_HASH (0x20)

#endif /* sab_h__ */
