/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#ifndef __TFM_VENEERS_H__
#define __TFM_VENEERS_H__

#include "tfm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TFM_PARTITION_NS_AGENT_MAILBOX //NXP
/******** TFM_NS_MAILBOX_AGENT ********/
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP

#ifdef TFM_PARTITION_PROTECTED_STORAGE //NXP
/******** TFM_SP_PS ********/
psa_status_t tfm_ps_set_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_ps_get_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_ps_get_info_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_ps_remove_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_ps_get_support_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
#endif /* TFM_PARTITION_PROTECTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE //NXP
/******** TFM_SP_ITS ********/
psa_status_t tfm_its_set_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_its_get_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_its_get_info_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_its_remove_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_AUDIT_LOG //NXP
/******** TFM_SP_AUDIT_LOG ********/
psa_status_t tfm_audit_core_retrieve_record_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_audit_core_add_record_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_audit_core_get_info_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_audit_core_get_record_info_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_audit_core_delete_record_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
#endif /* TFM_PARTITION_AUDIT_LOG */ //NXP

#ifdef TFM_PARTITION_CRYPTO //NXP
/******** TFM_SP_CRYPTO ********/
psa_status_t tfm_crypto_api_dispatcher_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
#endif /* TFM_PARTITION_CRYPTO */ //NXP

#ifdef TFM_PARTITION_PLATFORM //NXP
/******** TFM_SP_PLATFORM ********/
psa_status_t tfm_platform_sp_system_reset_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_platform_sp_ioctl_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_platform_sp_nv_counter_read_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_platform_sp_nv_counter_increment_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
#endif /* TFM_PARTITION_PLATFORM */ //NXP

#ifdef TFM_PARTITION_INITIAL_ATTESTATION //NXP
/******** TFM_SP_INITIAL_ATTESTATION ********/
psa_status_t tfm_initial_attest_get_token_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_initial_attest_get_token_size_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */ //NXP

#ifdef TFM_PARTITION_FIRMWARE_UPDATE //NXP
/******** TFM_SP_FWU ********/
psa_status_t tfm_fwu_write_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_fwu_install_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_fwu_abort_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_fwu_query_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_fwu_request_reboot_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_fwu_accept_req_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */ //NXP

#ifdef TEST_NS_CORE //NXP
/******** TFM_SP_CORE_TEST ********/
psa_status_t tfm_spm_core_test_sfn_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_spm_core_test_sfn_init_success_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_spm_core_test_sfn_direct_recursion_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_NS_CORE //NXP
/******** TFM_SP_CORE_TEST_2 ********/
psa_status_t tfm_spm_core_test_2_slave_service_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_spm_core_test_2_sfn_invert_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_spm_core_test_2_check_caller_client_id_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
psa_status_t tfm_spm_core_test_2_get_every_second_byte_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
/******** TFM_SP_SECURE_TEST_PARTITION ********/
psa_status_t tfm_secure_client_service_sfn_run_tests_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
/******** TFM_SP_IPC_SERVICE_TEST ********/
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
/******** TFM_SP_IPC_CLIENT_TEST ********/
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TEST_S_PS //NXP
/******** TFM_SP_PS_TEST ********/
psa_status_t tfm_ps_test_prepare_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
#endif /* TEST_S_PS */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
/******** TFM_SP_SECURE_CLIENT_2 ********/
psa_status_t tfm_secure_client_2_call_veneer(psa_invec *in_vec, size_t in_len, psa_outvec *out_vec, size_t out_len);
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

#ifdef __cplusplus
}
#endif

#endif /* __TFM_VENEERS_H__ */
