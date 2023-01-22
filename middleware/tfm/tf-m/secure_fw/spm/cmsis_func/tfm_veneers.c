/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#include "tfm_secure_api.h"
#include "spm_partition_defs.h"
#include "security_defs.h"

#ifdef TFM_PARTITION_NS_AGENT_MAILBOX //NXP
/******** TFM_NS_MAILBOX_AGENT ********/
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP

#ifdef TFM_PARTITION_PROTECTED_STORAGE //NXP
/******** TFM_SP_PS ********/
psa_status_t tfm_ps_set_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_ps_get_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_ps_get_info_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_ps_remove_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_ps_get_support_req(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_PROTECTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE //NXP
/******** TFM_SP_ITS ********/
psa_status_t tfm_its_set_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_its_get_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_its_get_info_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_its_remove_req(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_AUDIT_LOG //NXP
/******** TFM_SP_AUDIT_LOG ********/
psa_status_t audit_core_retrieve_record(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t audit_core_add_record(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t audit_core_get_info(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t audit_core_get_record_info(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t audit_core_delete_record(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_AUDIT_LOG */ //NXP

#ifdef TFM_PARTITION_CRYPTO //NXP
/******** TFM_SP_CRYPTO ********/
psa_status_t tfm_crypto_api_dispatcher(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_CRYPTO */ //NXP

#ifdef TFM_PARTITION_PLATFORM //NXP
/******** TFM_SP_PLATFORM ********/
psa_status_t platform_sp_system_reset(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t platform_sp_ioctl(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t platform_sp_nv_counter_read(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t platform_sp_nv_counter_increment(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_PLATFORM */ //NXP

#ifdef TFM_PARTITION_INITIAL_ATTESTATION //NXP
/******** TFM_SP_INITIAL_ATTESTATION ********/
psa_status_t initial_attest_get_token_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t initial_attest_get_token_size_req(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */ //NXP

#ifdef TFM_PARTITION_FIRMWARE_UPDATE //NXP
/******** TFM_SP_FWU ********/
psa_status_t tfm_fwu_write_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_fwu_install_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_fwu_abort_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_fwu_query_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_fwu_request_reboot_req(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t tfm_fwu_accept_req(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */ //NXP

#ifdef TEST_NS_CORE //NXP
/******** TFM_SP_CORE_TEST ********/
psa_status_t spm_core_test_sfn(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_sfn_init_success(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_sfn_direct_recursion(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_NS_CORE //NXP
/******** TFM_SP_CORE_TEST_2 ********/
psa_status_t spm_core_test_2_slave_service(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_sfn_invert(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_check_caller_client_id(psa_invec *, size_t, psa_outvec *, size_t);
psa_status_t spm_core_test_2_get_every_second_byte(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
/******** TFM_SP_SECURE_TEST_PARTITION ********/
psa_status_t tfm_secure_client_service_sfn_run_tests(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
/******** TFM_SP_IPC_SERVICE_TEST ********/
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
/******** TFM_SP_IPC_CLIENT_TEST ********/
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TEST_S_PS //NXP
/******** TFM_SP_PS_TEST ********/
psa_status_t tfm_ps_test_prepare(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TEST_S_PS */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
/******** TFM_SP_SECURE_CLIENT_2 ********/
psa_status_t tfm_secure_client_2_call(psa_invec *, size_t, psa_outvec *, size_t);
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TEST_NS_SLIH_IRQ //NXP
/******** TFM_SP_SLIH_TEST ********/
#endif /* TEST_NS_SLIH_IRQ */ //NXP

#ifdef TEST_NS_FLIH_IRQ //NXP
/******** TFM_SP_FLIH_TEST ********/
#endif /* TEST_NS_FLIH_IRQ */ //NXP

#ifdef TEST_PARTITION_FPU_TEST //NXP
/******** TFM_SP_FPU_SERVICE_TEST ********/
#endif /* TEST_PARTITION_FPU_TEST */ //NXP


#define TFM_VENEER_FUNCTION(partition_name, func_name, sfn_name) \
    __tz_c_veneer \
    psa_status_t func_name##_veneer(psa_invec *in_vec, \
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

#ifdef TFM_PARTITION_NS_AGENT_MAILBOX //NXP
/******** TFM_NS_MAILBOX_AGENT ********/
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP

#ifdef TFM_PARTITION_PROTECTED_STORAGE //NXP
/******** TFM_SP_PS ********/
TFM_VENEER_FUNCTION(TFM_SP_PS, tfm_ps_set_req, tfm_ps_set_req)
TFM_VENEER_FUNCTION(TFM_SP_PS, tfm_ps_get_req, tfm_ps_get_req)
TFM_VENEER_FUNCTION(TFM_SP_PS, tfm_ps_get_info_req, tfm_ps_get_info_req)
TFM_VENEER_FUNCTION(TFM_SP_PS, tfm_ps_remove_req, tfm_ps_remove_req)
TFM_VENEER_FUNCTION(TFM_SP_PS, tfm_ps_get_support_req, tfm_ps_get_support_req)
#endif /* TFM_PARTITION_PROTECTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE //NXP
/******** TFM_SP_ITS ********/
TFM_VENEER_FUNCTION(TFM_SP_ITS, tfm_its_set_req, tfm_its_set_req)
TFM_VENEER_FUNCTION(TFM_SP_ITS, tfm_its_get_req, tfm_its_get_req)
TFM_VENEER_FUNCTION(TFM_SP_ITS, tfm_its_get_info_req, tfm_its_get_info_req)
TFM_VENEER_FUNCTION(TFM_SP_ITS, tfm_its_remove_req, tfm_its_remove_req)
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_AUDIT_LOG //NXP
/******** TFM_SP_AUDIT_LOG ********/
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, tfm_audit_core_retrieve_record, audit_core_retrieve_record)
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, tfm_audit_core_add_record, audit_core_add_record)
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, tfm_audit_core_get_info, audit_core_get_info)
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, tfm_audit_core_get_record_info, audit_core_get_record_info)
TFM_VENEER_FUNCTION(TFM_SP_AUDIT_LOG, tfm_audit_core_delete_record, audit_core_delete_record)
#endif /* TFM_PARTITION_AUDIT_LOG */ //NXP

#ifdef TFM_PARTITION_CRYPTO //NXP
/******** TFM_SP_CRYPTO ********/
TFM_VENEER_FUNCTION(TFM_SP_CRYPTO, tfm_crypto_api_dispatcher, tfm_crypto_api_dispatcher)
#endif /* TFM_PARTITION_CRYPTO */ //NXP

#ifdef TFM_PARTITION_PLATFORM //NXP
/******** TFM_SP_PLATFORM ********/
TFM_VENEER_FUNCTION(TFM_SP_PLATFORM, tfm_platform_sp_system_reset, platform_sp_system_reset)
TFM_VENEER_FUNCTION(TFM_SP_PLATFORM, tfm_platform_sp_ioctl, platform_sp_ioctl)
TFM_VENEER_FUNCTION(TFM_SP_PLATFORM, tfm_platform_sp_nv_counter_read, platform_sp_nv_counter_read)
TFM_VENEER_FUNCTION(TFM_SP_PLATFORM, tfm_platform_sp_nv_counter_increment, platform_sp_nv_counter_increment)
#endif /* TFM_PARTITION_PLATFORM */ //NXP

#ifdef TFM_PARTITION_INITIAL_ATTESTATION //NXP
/******** TFM_SP_INITIAL_ATTESTATION ********/
TFM_VENEER_FUNCTION(TFM_SP_INITIAL_ATTESTATION, tfm_initial_attest_get_token_req, initial_attest_get_token_req)
TFM_VENEER_FUNCTION(TFM_SP_INITIAL_ATTESTATION, tfm_initial_attest_get_token_size_req, initial_attest_get_token_size_req)
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */ //NXP

#ifdef TFM_PARTITION_FIRMWARE_UPDATE //NXP
/******** TFM_SP_FWU ********/
TFM_VENEER_FUNCTION(TFM_SP_FWU, tfm_fwu_write_req, tfm_fwu_write_req)
TFM_VENEER_FUNCTION(TFM_SP_FWU, tfm_fwu_install_req, tfm_fwu_install_req)
TFM_VENEER_FUNCTION(TFM_SP_FWU, tfm_fwu_abort_req, tfm_fwu_abort_req)
TFM_VENEER_FUNCTION(TFM_SP_FWU, tfm_fwu_query_req, tfm_fwu_query_req)
TFM_VENEER_FUNCTION(TFM_SP_FWU, tfm_fwu_request_reboot_req, tfm_fwu_request_reboot_req)
TFM_VENEER_FUNCTION(TFM_SP_FWU, tfm_fwu_accept_req, tfm_fwu_accept_req)
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */ //NXP

#ifdef TEST_NS_CORE //NXP
/******** TFM_SP_CORE_TEST ********/
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST, tfm_spm_core_test_sfn, spm_core_test_sfn)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST, tfm_spm_core_test_sfn_init_success, spm_core_test_sfn_init_success)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST, tfm_spm_core_test_sfn_direct_recursion, spm_core_test_sfn_direct_recursion)
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_NS_CORE //NXP
/******** TFM_SP_CORE_TEST_2 ********/
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, tfm_spm_core_test_2_slave_service, spm_core_test_2_slave_service)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, tfm_spm_core_test_2_sfn_invert, spm_core_test_2_sfn_invert)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, tfm_spm_core_test_2_check_caller_client_id, spm_core_test_2_check_caller_client_id)
TFM_VENEER_FUNCTION(TFM_SP_CORE_TEST_2, tfm_spm_core_test_2_get_every_second_byte, spm_core_test_2_get_every_second_byte)
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
/******** TFM_SP_SECURE_TEST_PARTITION ********/
TFM_VENEER_FUNCTION(TFM_SP_SECURE_TEST_PARTITION, tfm_secure_client_service_sfn_run_tests, tfm_secure_client_service_sfn_run_tests)
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
/******** TFM_SP_IPC_SERVICE_TEST ********/
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
/******** TFM_SP_IPC_CLIENT_TEST ********/
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TEST_S_PS //NXP
/******** TFM_SP_PS_TEST ********/
TFM_VENEER_FUNCTION(TFM_SP_PS_TEST, tfm_ps_test_prepare, tfm_ps_test_prepare)
#endif /* TEST_S_PS */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
/******** TFM_SP_SECURE_CLIENT_2 ********/
TFM_VENEER_FUNCTION(TFM_SP_SECURE_CLIENT_2, tfm_secure_client_2_call, tfm_secure_client_2_call)
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TEST_NS_SLIH_IRQ //NXP
/******** TFM_SP_SLIH_TEST ********/
#endif /* TEST_NS_SLIH_IRQ */ //NXP

#ifdef TEST_NS_FLIH_IRQ //NXP
/******** TFM_SP_FLIH_TEST ********/
#endif /* TEST_NS_FLIH_IRQ */ //NXP

#ifdef TEST_PARTITION_FPU_TEST //NXP
/******** TFM_SP_FPU_SERVICE_TEST ********/
#endif /* TEST_PARTITION_FPU_TEST */ //NXP

