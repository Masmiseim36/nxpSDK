/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __TFM_SPM_DB_IPC_INC__
#define __TFM_SPM_DB_IPC_INC__

#include "psa_manifest/sid.h"

/**************************************************************************/
/** IRQ count per partition */
/**************************************************************************/
#ifdef TFM_PARTITION_PROTECTED_STORAGE
#define TFM_PARTITION_TFM_SP_PS_IRQ_COUNT 0
#endif /* TFM_PARTITION_PROTECTED_STORAGE */

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
#define TFM_PARTITION_TFM_SP_ITS_IRQ_COUNT 0
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */

#ifdef TFM_PARTITION_AUDIT_LOG
#define TFM_PARTITION_TFM_SP_AUDIT_LOG_IRQ_COUNT 0
#endif /* TFM_PARTITION_AUDIT_LOG */

#ifdef TFM_PARTITION_CRYPTO
#define TFM_PARTITION_TFM_SP_CRYPTO_IRQ_COUNT 0
#endif /* TFM_PARTITION_CRYPTO */

#ifdef TFM_PARTITION_PLATFORM
#define TFM_PARTITION_TFM_SP_PLATFORM_IRQ_COUNT 0
#endif /* TFM_PARTITION_PLATFORM */

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
#define TFM_PARTITION_TFM_SP_INITIAL_ATTESTATION_IRQ_COUNT 0
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

#ifdef TFM_PARTITION_TEST_CORE
#define TFM_PARTITION_TFM_SP_CORE_TEST_IRQ_COUNT 0
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
#define TFM_PARTITION_TFM_SP_CORE_TEST_2_IRQ_COUNT 0
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
#define TFM_PARTITION_TFM_SP_SECURE_TEST_PARTITION_IRQ_COUNT 0
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
#define TFM_PARTITION_TFM_SP_IPC_SERVICE_TEST_IRQ_COUNT 0
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
#define TFM_PARTITION_TFM_SP_IPC_CLIENT_TEST_IRQ_COUNT 0
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
#define TFM_PARTITION_TFM_IRQ_TEST_1_IRQ_COUNT 1
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_TEST_PS
#define TFM_PARTITION_TFM_SP_PS_TEST_IRQ_COUNT 0
#endif /* TFM_PARTITION_TEST_PS */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
#define TFM_PARTITION_TFM_SP_SECURE_CLIENT_2_IRQ_COUNT 0
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_PSA_PROXY
#define TFM_PARTITION_TFM_SP_PSA_PROXY_IRQ_COUNT 0
#endif /* TFM_PARTITION_PSA_PROXY */

#ifdef TFM_PARTITION_FIRMWARE_UPDATE
#define TFM_PARTITION_TFM_SP_FWU_IRQ_COUNT 0
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */

#ifdef TFM_PARTITION_FFM11
#define TFM_PARTITION_TFM_SP_FFM11_IRQ_COUNT 0
#endif /* TFM_PARTITION_FFM11 */

/**************************************************************************/
/** Declarations of partition init functions */
/**************************************************************************/
extern void tfm_nspm_thread_entry(void);

#ifdef TFM_PARTITION_PROTECTED_STORAGE
extern void tfm_ps_req_mngr_init(void);
#endif /* TFM_PARTITION_PROTECTED_STORAGE */

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
extern void tfm_its_req_mngr_init(void);
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */

#ifdef TFM_PARTITION_AUDIT_LOG
extern void audit_core_init(void);
#endif /* TFM_PARTITION_AUDIT_LOG */

#ifdef TFM_PARTITION_CRYPTO
extern void tfm_crypto_init(void);
#endif /* TFM_PARTITION_CRYPTO */

#ifdef TFM_PARTITION_PLATFORM
extern void platform_sp_init(void);
#endif /* TFM_PARTITION_PLATFORM */

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
extern void attest_partition_init(void);
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

#ifdef TFM_PARTITION_TEST_CORE
extern void core_test_init(void);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
extern void core_test_2_init(void);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
extern void tfm_secure_client_service_init(void);
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
extern void ipc_service_test_main(void);
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
extern void ipc_client_test_main(void);
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
extern void tfm_irq_test_1_init(void);
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_TEST_PS
extern void tfm_ps_test_init(void);
#endif /* TFM_PARTITION_TEST_PS */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
extern void tfm_secure_client_2_init(void);
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_PSA_PROXY
extern void psa_proxy_sp_init(void);
#endif /* TFM_PARTITION_PSA_PROXY */

#ifdef TFM_PARTITION_FIRMWARE_UPDATE
extern void tfm_fwu_init(void);
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */

#ifdef TFM_PARTITION_FFM11
extern void tfm_ffm11_partition_main(void);
#endif /* TFM_PARTITION_FFM11 */

/**************************************************************************/
/** Memory region declarations */
/**************************************************************************/
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Base);
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Limit);

#ifdef TFM_PARTITION_PROTECTED_STORAGE
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_PS_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_PS_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_ps_stack[];
#endif /* TFM_PARTITION_PROTECTED_STORAGE */

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_ITS_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_ITS_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_its_stack[];
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */

#ifdef TFM_PARTITION_AUDIT_LOG
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_AUDIT_LOG_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_AUDIT_LOG_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_audit_log_stack[];
#endif /* TFM_PARTITION_AUDIT_LOG */

#ifdef TFM_PARTITION_CRYPTO
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_CRYPTO_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_CRYPTO_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_crypto_stack[];
#endif /* TFM_PARTITION_CRYPTO */

#ifdef TFM_PARTITION_PLATFORM
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_PLATFORM_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_PLATFORM_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_platform_stack[];
#endif /* TFM_PARTITION_PLATFORM */

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_INITIAL_ATTESTATION_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_INITIAL_ATTESTATION_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_initial_attestation_stack[];
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

#ifdef TFM_PARTITION_TEST_CORE
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_CORE_TEST_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_CORE_TEST_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_core_test_stack[];
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_CORE_TEST_2_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_CORE_TEST_2_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_core_test_2_stack[];
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_SECURE_TEST_PARTITION_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_SECURE_TEST_PARTITION_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_secure_test_partition_stack[];
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_IPC_SERVICE_TEST_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_IPC_SERVICE_TEST_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_ipc_service_test_stack[];
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_IPC_CLIENT_TEST_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_IPC_CLIENT_TEST_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_ipc_client_test_stack[];
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_IRQ_TEST_1_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_IRQ_TEST_1_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_irq_test_1_stack[];
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_TEST_PS
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_PS_TEST_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_PS_TEST_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_ps_test_stack[];
#endif /* TFM_PARTITION_TEST_PS */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_SECURE_CLIENT_2_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_SECURE_CLIENT_2_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_secure_client_2_stack[];
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_PSA_PROXY
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_PSA_PROXY_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_PSA_PROXY_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_psa_proxy_stack[];
#endif /* TFM_PARTITION_PSA_PROXY */

#ifdef TFM_PARTITION_FIRMWARE_UPDATE
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_FWU_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_FWU_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_fwu_stack[];
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */

#ifdef TFM_PARTITION_FFM11
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_FFM11_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_FFM11_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_ffm11_stack[];
#endif /* TFM_PARTITION_FFM11 */

/**************************************************************************/
/** Dependencies array for Secure Partition */
/**************************************************************************/
#ifdef TFM_PARTITION_PROTECTED_STORAGE
static uint32_t dependencies_TFM_SP_PS[] =
{
    TFM_CRYPTO_SID,
    TFM_ITS_SET_SID,
    TFM_ITS_GET_SID,
    TFM_ITS_GET_INFO_SID,
    TFM_ITS_REMOVE_SID,
    TFM_SP_PLATFORM_NV_COUNTER_SID,
};
#endif /* TFM_PARTITION_PROTECTED_STORAGE */

#ifdef TFM_PARTITION_CRYPTO
static uint32_t dependencies_TFM_SP_CRYPTO[] =
{
    TFM_ITS_SET_SID,
    TFM_ITS_GET_SID,
    TFM_ITS_GET_INFO_SID,
    TFM_ITS_REMOVE_SID,
};
#endif /* TFM_PARTITION_CRYPTO */

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
static uint32_t dependencies_TFM_SP_INITIAL_ATTESTATION[] =
{
    TFM_CRYPTO_SID,
};
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

#ifdef TFM_PARTITION_TEST_CORE
static uint32_t dependencies_TFM_SP_CORE_TEST[] =
{
    SPM_CORE_TEST_2_INVERT_SID,
    SPM_CORE_TEST_2_GET_EVERY_SECOND_BYTE_SID,
    SPM_CORE_TEST_2_SLAVE_SERVICE_SID,
};
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
static uint32_t dependencies_TFM_SP_SECURE_TEST_PARTITION[] =
{
    TFM_SECURE_CLIENT_2_SID,
    TFM_CRYPTO_SID,
    TFM_PS_SET_SID,
    TFM_PS_GET_SID,
    TFM_PS_GET_INFO_SID,
    TFM_PS_REMOVE_SID,
    TFM_PS_GET_SUPPORT_SID,
    TFM_ITS_SET_SID,
    TFM_ITS_GET_SID,
    TFM_ITS_GET_INFO_SID,
    TFM_ITS_REMOVE_SID,
    TFM_ATTEST_GET_TOKEN_SID,
    TFM_ATTEST_GET_TOKEN_SIZE_SID,
    TFM_ATTEST_GET_PUBLIC_KEY_SID,
    TFM_PS_TEST_PREPARE_SID,
    TFM_SP_PLATFORM_SYSTEM_RESET_SID,
    TFM_SP_PLATFORM_IOCTL_SID,
    TFM_FWU_WRITE_SID,
    TFM_FWU_INSTALL_SID,
    TFM_FWU_ABORT_SID,
    TFM_FWU_QUERY_SID,
    TFM_FWU_REQUEST_REBOOT_SID,
    TFM_FWU_ACCEPT_SID,
    TFM_FFM11_SERVICE1_SID,
};
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
static uint32_t dependencies_TFM_SP_IPC_CLIENT_TEST[] =
{
    IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SID,
    IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM_SID,
    IPC_SERVICE_TEST_BASIC_SID,
    IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_SID,
    TFM_SECURE_CLIENT_2_SID,
};
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_PS
static uint32_t dependencies_TFM_SP_PS_TEST[] =
{
    TFM_CRYPTO_SID,
    TFM_ITS_GET_SID,
    TFM_ITS_REMOVE_SID,
};
#endif /* TFM_PARTITION_TEST_PS */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
static uint32_t dependencies_TFM_SP_SECURE_CLIENT_2[] =
{
    TFM_ITS_GET_SID,
    TFM_CRYPTO_SID,
};
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_FIRMWARE_UPDATE
static uint32_t dependencies_TFM_SP_FWU[] =
{
    TFM_CRYPTO_SID,
    TFM_SP_PLATFORM_SYSTEM_RESET_SID,
};
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */


/**************************************************************************/
/** The platform data of the partition list */
/**************************************************************************/
#ifdef TFM_PARTITION_AUDIT_LOG
const struct platform_data_t *
    platform_data_list_TFM_SP_AUDIT_LOG[] =
{
#ifdef AUDIT_UART_REDIRECTION
    TFM_PERIPHERAL_UART1,
#endif /* AUDIT_UART_REDIRECTION */
    NULL
};
#endif /* TFM_PARTITION_AUDIT_LOG */

#ifdef TFM_PARTITION_TEST_CORE
const struct platform_data_t *
    platform_data_list_TFM_SP_CORE_TEST[] =
{
    TFM_PERIPHERAL_FPGA_IO,
    NULL
};
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
const struct platform_data_t *
    platform_data_list_TFM_SP_SECURE_TEST_PARTITION[] =
{
    TFM_PERIPHERAL_STD_UART,
    NULL
};
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_ENABLE_IRQ_TEST
const struct platform_data_t *
    platform_data_list_TFM_IRQ_TEST_1[] =
{
    TFM_PERIPHERAL_TIMER0,
    NULL
};
#endif /* TFM_ENABLE_IRQ_TEST */


/**************************************************************************/
/** The static data of the partition list */
/**************************************************************************/
const struct partition_static_t static_data_list[] =
{
    {
        .psa_ff_ver           = 0x0100,
        .pid                  = TFM_SP_NON_SECURE_ID,
#if TFM_MULTI_CORE_TOPOLOGY
        .flags                = SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_IPC,
#else
        .flags                = SPM_PART_FLAG_APP_ROT | SPM_PART_FLAG_IPC,
#endif
        .priority             = TFM_PRIORITY_LOW,
        .entry                = tfm_nspm_thread_entry,
        .stack_base_addr      = 0,
        .stack_size           = 0
    },

#ifdef TFM_PARTITION_PROTECTED_STORAGE
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_PS,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = tfm_ps_req_mngr_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 6,
        .deps                 = dependencies_TFM_SP_PS,
    },
#endif /* TFM_PARTITION_PROTECTED_STORAGE */

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_ITS,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = tfm_its_req_mngr_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 0,
        .deps                 = NULL,
    },
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */

#ifdef TFM_PARTITION_AUDIT_LOG
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_AUDIT_LOG,
        .flags                 = 0
                               | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = audit_core_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = (uintptr_t)platform_data_list_TFM_SP_AUDIT_LOG,
        .ndeps                = 0,
        .deps                 = NULL,
    },
#endif /* TFM_PARTITION_AUDIT_LOG */

#ifdef TFM_PARTITION_CRYPTO
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_CRYPTO,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = tfm_crypto_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 4,
        .deps                 = dependencies_TFM_SP_CRYPTO,
    },
#endif /* TFM_PARTITION_CRYPTO */

#ifdef TFM_PARTITION_PLATFORM
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_PLATFORM,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = platform_sp_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 0,
        .deps                 = NULL,
    },
#endif /* TFM_PARTITION_PLATFORM */

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_INITIAL_ATTESTATION,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = attest_partition_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 1,
        .deps                 = dependencies_TFM_SP_INITIAL_ATTESTATION,
    },
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

#ifdef TFM_PARTITION_TEST_CORE
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_CORE_TEST,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = core_test_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = (uintptr_t)platform_data_list_TFM_SP_CORE_TEST,
        .ndeps                = 3,
        .deps                 = dependencies_TFM_SP_CORE_TEST,
    },
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_CORE_TEST_2,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = core_test_2_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 0,
        .deps                 = NULL,
    },
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_SECURE_TEST_PARTITION,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = tfm_secure_client_service_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = (uintptr_t)platform_data_list_TFM_SP_SECURE_TEST_PARTITION,
        .ndeps                = 24,
        .deps                 = dependencies_TFM_SP_SECURE_TEST_PARTITION,
    },
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_IPC_SERVICE_TEST,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(HIGH),
        .entry                = ipc_service_test_main,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 0,
        .deps                 = NULL,
    },
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_IPC_CLIENT_TEST,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = ipc_client_test_main,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 5,
        .deps                 = dependencies_TFM_SP_IPC_CLIENT_TEST,
    },
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_IRQ_TEST_1,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT //NXP was SPM_PART_FLAG_PSA_ROT, otherwise IRQ test is failed
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = tfm_irq_test_1_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = (uintptr_t)platform_data_list_TFM_IRQ_TEST_1,
        .ndeps                = 0,
        .deps                 = NULL,
    },
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_TEST_PS
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_PS_TEST,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = tfm_ps_test_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 3,
        .deps                 = dependencies_TFM_SP_PS_TEST,
    },
#endif /* TFM_PARTITION_TEST_PS */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_SECURE_CLIENT_2,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = tfm_secure_client_2_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 2,
        .deps                 = dependencies_TFM_SP_SECURE_CLIENT_2,
    },
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_PSA_PROXY
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_PSA_PROXY,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(HIGH),
        .entry                = psa_proxy_sp_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 0,
        .deps                 = NULL,
    },
#endif /* TFM_PARTITION_PSA_PROXY */

#ifdef TFM_PARTITION_FIRMWARE_UPDATE
    {
        .psa_ff_ver            = 0x0100,
        .pid                   = TFM_SP_FWU,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = tfm_fwu_init,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 2,
        .deps                 = dependencies_TFM_SP_FWU,
    },
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */

#ifdef TFM_PARTITION_FFM11
    {
        .psa_ff_ver            = 0x0101,
        .pid                   = TFM_SP_FFM11,
        .flags                 = SPM_PART_FLAG_IPC
                               | SPM_PART_FLAG_APP_ROT
                              ,
        .priority             = TFM_PRIORITY(NORMAL),
        .entry                = tfm_ffm11_partition_main,
        .stack_base_addr      = 0,
        .stack_size           = 0,
        .platform_data        = 0,
        .ndeps                = 0,
        .deps                 = NULL,
    },
#endif /* TFM_PARTITION_FFM11 */

};

/**************************************************************************/
/** The memory data of the partition list */
/**************************************************************************/
const struct tfm_spm_partition_memory_data_t memory_data_list[] =
{
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(ARM_LIB_STACK, $$ZI$$Base),
        .data_limit           = PART_REGION_ADDR(ARM_LIB_STACK, $$ZI$$Limit),
#endif
        .stack_bottom         = PART_REGION_ADDR(ARM_LIB_STACK, $$ZI$$Base),
        .stack_top            = PART_REGION_ADDR(ARM_LIB_STACK, $$ZI$$Limit),
    },
#ifdef TFM_PARTITION_PROTECTED_STORAGE
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_PS_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_PS_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_ps_stack,
        .stack_top            = (uint32_t)(tfm_sp_ps_stack + 0x800),
    },
#endif /* TFM_PARTITION_PROTECTED_STORAGE */

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_ITS_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_ITS_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_its_stack,
        .stack_top            = (uint32_t)(tfm_sp_its_stack + 0x900), //NXP was 0x680, otherwise stack overflow
    },
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */

#ifdef TFM_PARTITION_AUDIT_LOG
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_AUDIT_LOG_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_AUDIT_LOG_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_audit_log_stack,
        .stack_top            = (uint32_t)(tfm_sp_audit_log_stack + 0x0200),
    },
#endif /* TFM_PARTITION_AUDIT_LOG */

#ifdef TFM_PARTITION_CRYPTO
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_CRYPTO_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_CRYPTO_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_crypto_stack,
        .stack_top            = (uint32_t)(tfm_sp_crypto_stack + 0x2000),
    },
#endif /* TFM_PARTITION_CRYPTO */

#ifdef TFM_PARTITION_PLATFORM
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_PLATFORM_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_PLATFORM_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_platform_stack,
        .stack_top            = (uint32_t)(tfm_sp_platform_stack + 0x0500),
    },
#endif /* TFM_PARTITION_PLATFORM */

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_INITIAL_ATTESTATION_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_INITIAL_ATTESTATION_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_initial_attestation_stack,
        .stack_top            = (uint32_t)(tfm_sp_initial_attestation_stack + 0x0A80),
    },
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

#ifdef TFM_PARTITION_TEST_CORE
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_CORE_TEST_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_CORE_TEST_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_core_test_stack,
        .stack_top            = (uint32_t)(tfm_sp_core_test_stack + 0x0380),
    },
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_CORE_TEST_2_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_CORE_TEST_2_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_core_test_2_stack,
        .stack_top            = (uint32_t)(tfm_sp_core_test_2_stack + 0x0280),
    },
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_SECURE_TEST_PARTITION_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_SECURE_TEST_PARTITION_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_secure_test_partition_stack,
        .stack_top            = (uint32_t)(tfm_sp_secure_test_partition_stack + 0x0D00),
    },
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_IPC_SERVICE_TEST_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_IPC_SERVICE_TEST_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_ipc_service_test_stack,
        .stack_top            = (uint32_t)(tfm_sp_ipc_service_test_stack + 0x0220),
    },
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_IPC_CLIENT_TEST_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_IPC_CLIENT_TEST_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_ipc_client_test_stack,
        .stack_top            = (uint32_t)(tfm_sp_ipc_client_test_stack + 0x0300),
    },
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_IRQ_TEST_1_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_IRQ_TEST_1_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_irq_test_1_stack,
        .stack_top            = (uint32_t)(tfm_irq_test_1_stack + 0x0400),
    },
#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_TEST_PS
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_PS_TEST_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_PS_TEST_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_ps_test_stack,
        .stack_top            = (uint32_t)(tfm_sp_ps_test_stack + 0x500),
    },
#endif /* TFM_PARTITION_TEST_PS */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_SECURE_CLIENT_2_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_SECURE_CLIENT_2_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_secure_client_2_stack,
        .stack_top            = (uint32_t)(tfm_sp_secure_client_2_stack + 0x300),
    },
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_PSA_PROXY
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_PSA_PROXY_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_PSA_PROXY_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_psa_proxy_stack,
        .stack_top            = (uint32_t)(tfm_sp_psa_proxy_stack + 0x0A00),
    },
#endif /* TFM_PARTITION_PSA_PROXY */

#ifdef TFM_PARTITION_FIRMWARE_UPDATE
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_FWU_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_FWU_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_fwu_stack,
        .stack_top            = (uint32_t)(tfm_sp_fwu_stack + 0x2000),
    },
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */

#ifdef TFM_PARTITION_FFM11
    {
#if TFM_LVL == 3
        .data_start           = PART_REGION_ADDR(PT_TFM_SP_FFM11_PRIVATE, _DATA_START$$Base),
        .data_limit           = PART_REGION_ADDR(PT_TFM_SP_FFM11_PRIVATE, _DATA_END$$Base),
#endif
        .stack_bottom         = (uint32_t)tfm_sp_ffm11_stack,
        .stack_top            = (uint32_t)(tfm_sp_ffm11_stack + 0x200),
    },
#endif /* TFM_PARTITION_FFM11 */

};

/**************************************************************************/
/** The partition list for the DB */
/**************************************************************************/
static struct partition_t partition_list [] =
{
    {0}, /* placeholder for Non-secure internal partition */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_PS */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_PROTECTED_STORAGE
    {0},
#endif /* TFM_PARTITION_PROTECTED_STORAGE */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_ITS */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
    {0},
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_AUDIT_LOG */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_AUDIT_LOG
    {0},
#endif /* TFM_PARTITION_AUDIT_LOG */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_CRYPTO */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_CRYPTO
    {0},
#endif /* TFM_PARTITION_CRYPTO */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_PLATFORM */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_PLATFORM
    {0},
#endif /* TFM_PARTITION_PLATFORM */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_INITIAL_ATTESTATION */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_INITIAL_ATTESTATION
    {0},
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_CORE_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_TEST_CORE
    {0},
#endif /* TFM_PARTITION_TEST_CORE */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_CORE_TEST_2 */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_TEST_CORE
    {0},
#endif /* TFM_PARTITION_TEST_CORE */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_SECURE_TEST_PARTITION */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    {0},
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_IPC_SERVICE_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_TEST_CORE_IPC
    {0},
#endif /* TFM_PARTITION_TEST_CORE_IPC */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_IPC_CLIENT_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_TEST_CORE_IPC
    {0},
#endif /* TFM_PARTITION_TEST_CORE_IPC */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_IRQ_TEST_1 */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_ENABLE_IRQ_TEST
    {0},
#endif /* TFM_ENABLE_IRQ_TEST */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_PS_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_TEST_PS
    {0},
#endif /* TFM_PARTITION_TEST_PS */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_SECURE_CLIENT_2 */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    {0},
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_PSA_PROXY */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_PSA_PROXY
    {0},
#endif /* TFM_PARTITION_PSA_PROXY */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_FWU */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_FIRMWARE_UPDATE
    {0},
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_FFM11 */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_FFM11
    {0},
#endif /* TFM_PARTITION_FFM11 */

};

struct spm_partition_db_t g_spm_partition_db = {
    .partition_count = sizeof(partition_list) / sizeof(partition_list[0]),
    .partitions = partition_list,
};

#endif /* __TFM_SPM_DB_IPC_INC__ */