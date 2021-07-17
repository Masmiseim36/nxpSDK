/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#include "tfm_secure_api.h"
#include "spm_partition_defs.h"

#ifdef TFM_PARTITION_PROTECTED_STORAGE
/******** TFM_SP_PS ********/
psa_status_t tfm_ps_set_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_ps_get_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_ps_get_info_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_ps_remove_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_ps_get_support_req(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_PROTECTED_STORAGE */

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
/******** TFM_SP_ITS ********/
psa_status_t tfm_its_set_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_its_get_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_its_get_info_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_its_remove_req(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */

#ifdef TFM_PARTITION_AUDIT_LOG
/******** TFM_SP_AUDIT_LOG ********/
psa_status_t audit_core_retrieve_record(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t audit_core_add_record(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t audit_core_get_info(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t audit_core_get_record_info(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t audit_core_delete_record(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_AUDIT_LOG */

#ifdef TFM_PARTITION_CRYPTO
/******** TFM_SP_CRYPTO ********/
psa_status_t tfm_crypto_get_key_attributes(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_open_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_close_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_reset_key_attributes(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_import_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_destroy_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_export_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_export_public_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_purge_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_copy_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_compute(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_compare(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_update(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_finish(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_verify(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_abort(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_hash_clone(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_compute(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_verify(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_sign_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_verify_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_update(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_sign_finish(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_verify_finish(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_mac_abort(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_encrypt(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_decrypt(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_encrypt_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_decrypt_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_generate_iv(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_set_iv(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_update(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_finish(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_cipher_abort(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_encrypt(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_decrypt(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_encrypt_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_decrypt_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_generate_nonce(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_set_nonce(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_set_lengths(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_update_ad(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_update(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_finish(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_verify(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_aead_abort(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_sign_hash(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_verify_hash(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_asymmetric_encrypt(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_asymmetric_decrypt(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_key_derivation_setup(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_key_derivation_get_capacity(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_key_derivation_set_capacity(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_key_derivation_input_bytes(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_key_derivation_input_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_key_derivation_key_agreement(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_key_derivation_output_bytes(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_key_derivation_output_key(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_key_derivation_abort(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_raw_key_agreement(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_generate_random(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_crypto_generate_key(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_CRYPTO */

#ifdef TFM_PARTITION_PLATFORM
/******** TFM_SP_PLATFORM ********/
psa_status_t platform_sp_system_reset(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t platform_sp_ioctl(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t platform_sp_nv_counter_read(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t platform_sp_nv_counter_increment(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_PLATFORM */

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
/******** TFM_SP_INITIAL_ATTESTATION ********/
psa_status_t initial_attest_get_token(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t initial_attest_get_token_size(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t initial_attest_get_public_key(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

#ifdef TFM_PARTITION_TEST_CORE
/******** TFM_SP_CORE_TEST ********/
psa_status_t spm_core_test_sfn(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_sfn_init_success(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_sfn_direct_recursion(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
/******** TFM_SP_CORE_TEST_2 ********/
psa_status_t spm_core_test_2_slave_service(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_sfn_invert(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_check_caller_client_id(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_get_every_second_byte(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_prepare_test_scenario(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_execute_test_scenario(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
/******** TFM_SP_SECURE_TEST_PARTITION ********/
psa_status_t tfm_secure_client_service_sfn_run_tests(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
/******** TFM_SP_IPC_SERVICE_TEST ********/
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
/******** TFM_SP_IPC_CLIENT_TEST ********/
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
/******** TFM_IRQ_TEST_1 ********/
psa_status_t spm_irq_test_1_prepare_test_scenario(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_irq_test_1_execute_test_scenario(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_TEST_PS
/******** TFM_SP_PS_TEST ********/
psa_status_t tfm_ps_test_prepare(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_TEST_PS */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
/******** TFM_SP_SECURE_CLIENT_2 ********/
psa_status_t tfm_secure_client_2_call(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_PSA_PROXY
/******** TFM_SP_PSA_PROXY ********/
#endif /* TFM_PARTITION_PSA_PROXY */

#ifdef TFM_PARTITION_FIRMWARE_UPDATE
/******** TFM_SP_FWU ********/
psa_status_t tfm_fwu_write_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_fwu_install_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_fwu_abort_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_fwu_query_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_fwu_request_reboot_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_fwu_accept_req(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */

#ifdef TFM_PARTITION_FFM11
/******** TFM_SP_FFM11 ********/
#endif /* TFM_PARTITION_FFM11 */


#define TFM_VENEER_FUNCTION(partition_name, sfn_name) \
    __tfm_secure_gateway_attributes__ \
    psa_status_t tfm_##sfn_name##_veneer(psa_invec *in_vec, \
                                         size_t in_len, \
                                         psa_outvec *out_vec, \
                                         size_t out_len) \
    { \
        bool is_ns = tfm_core_is_ns_client(); \
      \
        TFM_CORE_IOVEC_SFN_REQUEST(partition_name, \
                                   is_ns, \
                                   (void *) sfn_name, \
                                   in_vec, in_len, out_vec, out_len); \
    }

#ifdef TFM_PARTITION_PROTECTED_STORAGE
/******** TFM_SP_PS ********/
TFM_VENEER_FUNCTION(TFM_SP_PS, tfm_ps_set_req)
TFM_VENEER_FUNCTION(TFM_SP_PS, tfm_ps_get_req)
TFM_VENEER_FUNCTION(TFM_SP_PS, tfm_ps_get_info_req)
TFM_VENEER_FUNCTION(TFM_SP_PS, tfm_ps_remove_req)
TFM_VENEER_FUNCTION(TFM_SP_PS, tfm_ps_get_support_req)
#endif /* TFM_PARTITION_PROTECTED_STORAGE */

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
/******** TFM_SP_ITS ********/
TFM_VENEER_FUNCTION(TFM_SP_ITS, tfm_its_set_req)
TFM_VENEER_FUNCTION(TFM_SP_ITS, tfm_its_get_req)
TFM_VENEER_FUNCTION(TFM_SP_ITS, tfm_its_get_info_req)
TFM_VENEER_FUNCTION(TFM_SP_ITS, tfm_its_remove_req)
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */

#ifdef TFM_PARTITION_AUDIT_LOG
/******** TFM_SP_AUDIT_LOG ********/
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, audit_core_retrieve_record)
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, audit_core_add_record)
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, audit_core_get_info)
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, audit_core_get_record_info)
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, audit_core_delete_record)
#endif /* TFM_PARTITION_AUDIT_LOG */

#ifdef TFM_PARTITION_CRYPTO
/******** TFM_SP_CRYPTO ********/
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_get_key_attributes)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_open_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_close_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_reset_key_attributes)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_import_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_destroy_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_export_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_export_public_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_purge_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_copy_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_compute)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_compare)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_update)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_finish)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_verify)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_abort)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_hash_clone)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_compute)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_verify)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_sign_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_verify_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_update)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_sign_finish)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_verify_finish)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_mac_abort)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_encrypt)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_decrypt)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_encrypt_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_decrypt_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_generate_iv)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_set_iv)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_update)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_finish)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_cipher_abort)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_encrypt)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_decrypt)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_encrypt_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_decrypt_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_generate_nonce)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_set_nonce)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_set_lengths)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_update_ad)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_update)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_finish)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_verify)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_aead_abort)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_sign_hash)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_verify_hash)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_asymmetric_encrypt)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_asymmetric_decrypt)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_key_derivation_setup)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_key_derivation_get_capacity)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_key_derivation_set_capacity)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_key_derivation_input_bytes)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_key_derivation_input_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_key_derivation_key_agreement)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_key_derivation_output_bytes)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_key_derivation_output_key)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_key_derivation_abort)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_raw_key_agreement)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_generate_random)
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_generate_key)
#endif /* TFM_PARTITION_CRYPTO */

#ifdef TFM_PARTITION_PLATFORM
/******** TFM_SP_PLATFORM ********/
TFM_VENEER_FUNCTION(TFM_SP_PLATFORM, platform_sp_system_reset)
TFM_VENEER_FUNCTION(TFM_SP_PLATFORM, platform_sp_ioctl)
TFM_VENEER_FUNCTION(TFM_SP_PLATFORM, platform_sp_nv_counter_read)
TFM_VENEER_FUNCTION(TFM_SP_PLATFORM, platform_sp_nv_counter_increment)
#endif /* TFM_PARTITION_PLATFORM */

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
/******** TFM_SP_INITIAL_ATTESTATION ********/
TFM_VENEER_FUNCTION(TFM_SP_INITIAL_ATTESTATION, initial_attest_get_token)
TFM_VENEER_FUNCTION(TFM_SP_INITIAL_ATTESTATION, initial_attest_get_token_size)
TFM_VENEER_FUNCTION(TFM_SP_INITIAL_ATTESTATION, initial_attest_get_public_key)
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

#ifdef TFM_PARTITION_TEST_CORE
/******** TFM_SP_CORE_TEST ********/
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST, spm_core_test_sfn)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST, spm_core_test_sfn_init_success)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST, spm_core_test_sfn_direct_recursion)
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
/******** TFM_SP_CORE_TEST_2 ********/
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, spm_core_test_2_slave_service)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, spm_core_test_2_sfn_invert)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, spm_core_test_2_check_caller_client_id)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, spm_core_test_2_get_every_second_byte)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, spm_core_test_2_prepare_test_scenario)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, spm_core_test_2_execute_test_scenario)
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
/******** TFM_SP_SECURE_TEST_PARTITION ********/
TFM_VENEER_FUNCTION(TFM_SP_SECURE_TEST_PARTITION, tfm_secure_client_service_sfn_run_tests)
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
/******** TFM_SP_IPC_SERVICE_TEST ********/
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
/******** TFM_SP_IPC_CLIENT_TEST ********/
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
/******** TFM_IRQ_TEST_1 ********/
TFM_VENEER_FUNCTION(TFM_IRQ_TEST_1, spm_irq_test_1_prepare_test_scenario)
TFM_VENEER_FUNCTION(TFM_IRQ_TEST_1, spm_irq_test_1_execute_test_scenario)
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_TEST_PS
/******** TFM_SP_PS_TEST ********/
TFM_VENEER_FUNCTION(TFM_SP_PS_TEST, tfm_ps_test_prepare)
#endif /* TFM_PARTITION_TEST_PS */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
/******** TFM_SP_SECURE_CLIENT_2 ********/
TFM_VENEER_FUNCTION(TFM_SP_SECURE_CLIENT_2, tfm_secure_client_2_call)
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_PSA_PROXY
/******** TFM_SP_PSA_PROXY ********/
#endif /* TFM_PARTITION_PSA_PROXY */

#ifdef TFM_PARTITION_FIRMWARE_UPDATE
/******** TFM_SP_FWU ********/
TFM_VENEER_FUNCTION(TFM_SP_FWU, tfm_fwu_write_req)
TFM_VENEER_FUNCTION(TFM_SP_FWU, tfm_fwu_install_req)
TFM_VENEER_FUNCTION(TFM_SP_FWU, tfm_fwu_abort_req)
TFM_VENEER_FUNCTION(TFM_SP_FWU, tfm_fwu_query_req)
TFM_VENEER_FUNCTION(TFM_SP_FWU, tfm_fwu_request_reboot_req)
TFM_VENEER_FUNCTION(TFM_SP_FWU, tfm_fwu_accept_req)
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */

#ifdef TFM_PARTITION_FFM11
/******** TFM_SP_FFM11 ********/
#endif /* TFM_PARTITION_FFM11 */

