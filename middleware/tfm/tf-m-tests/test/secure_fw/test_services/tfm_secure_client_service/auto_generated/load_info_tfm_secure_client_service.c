/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2021-2022 Cypress Semiconductor Corporation (an Infineon
 * company) or an affiliate of Cypress Semiconductor Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#include <stdint.h>
#include <stddef.h>
#include "config_tfm.h"
#include "region.h"
#include "region_defs.h"
#include "spm.h"
#include "load/interrupt_defs.h"
#include "load/partition_defs.h"
#include "load/service_defs.h"
#include "load/asset_defs.h"
#include "tfm_peripherals_def.h"
#include "psa_manifest/pid.h"
#include "psa_manifest/sid.h"
#include "psa_manifest/tfm_secure_client_service.h"

#define TFM_SP_SECURE_TEST_PARTITION_NDEPS                      (21)
#define TFM_SP_SECURE_TEST_PARTITION_NSERVS                     (1)
#if TFM_LVL == 3
#define TFM_SP_SECURE_TEST_PARTITION_NASSETS                    (1 + 1)
#else
#define TFM_SP_SECURE_TEST_PARTITION_NASSETS                    (1)
#endif
#define TFM_SP_SECURE_TEST_PARTITION_NIRQS                      (0)

/* Memory region declaration */
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_SECURE_TEST_PARTITION_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_SECURE_TEST_PARTITION_PRIVATE, _DATA_END$$Base);
#endif

extern uint8_t tfm_sp_secure_test_partition_stack[];

/* Entrypoint function declaration */
extern void tfm_secure_client_service_init(void);

/* Interrupt init functions */

/* partition load info type definition */
struct partition_tfm_sp_secure_test_partition_load_info_t {
    /* common length load data */
    struct partition_load_info_t    load_info;
    /* per-partition variable length load data */
    uintptr_t                       stack_addr;
    uintptr_t                       heap_addr;
    uint32_t                        deps[TFM_SP_SECURE_TEST_PARTITION_NDEPS];
    struct service_load_info_t      services[TFM_SP_SECURE_TEST_PARTITION_NSERVS];
#if TFM_LVL == 3
    struct asset_desc_t             assets[TFM_SP_SECURE_TEST_PARTITION_NASSETS];
#else
    struct asset_desc_t             assets[TFM_SP_SECURE_TEST_PARTITION_NASSETS];
#endif
} __attribute__((aligned(4)));

/* Partition load, deps, service load data. Put to a dedicated section. */
#if defined(__ICCARM__)
#pragma location = ".part_load_priority_low"
__root
#endif /* __ICCARM__ */
const struct partition_tfm_sp_secure_test_partition_load_info_t tfm_sp_secure_test_partition_load
    __attribute__((used, section(".part_load_priority_low"))) = {
    .load_info = {
        .psa_ff_ver                 = 0x0101 | PARTITION_INFO_MAGIC,
        .pid                        = TFM_SP_SECURE_TEST_PARTITION,
        .flags                      = 0
                                    | PARTITION_MODEL_IPC
                                    | PARTITION_MODEL_PSA_ROT
                                    | PARTITION_PRI_LOW,
        .entry                      = ENTRY_TO_POSITION(tfm_secure_client_service_init),
        .stack_size                 = 0x0D00,
        .heap_size                  = 0,
        .ndeps                      = TFM_SP_SECURE_TEST_PARTITION_NDEPS,
        .nservices                  = TFM_SP_SECURE_TEST_PARTITION_NSERVS,
        .nassets                    = TFM_SP_SECURE_TEST_PARTITION_NASSETS,
        .nirqs                      = TFM_SP_SECURE_TEST_PARTITION_NIRQS,
    },
    .stack_addr                     = (uintptr_t)tfm_sp_secure_test_partition_stack,
    .heap_addr                      = 0,
    .deps = {
        TFM_SECURE_CLIENT_2_SID,
#ifdef TFM_CRYPTO_SID
        TFM_CRYPTO_SID,
#endif
#ifdef TFM_PROTECTED_STORAGE_SERVICE_SID
        TFM_PROTECTED_STORAGE_SERVICE_SID,
#endif
#ifdef TFM_INTERNAL_TRUSTED_STORAGE_SERVICE_SID
        TFM_INTERNAL_TRUSTED_STORAGE_SERVICE_SID,
#endif
#ifdef TFM_ATTESTATION_SERVICE_SID
        TFM_ATTESTATION_SERVICE_SID,
#endif
#ifdef TFM_PS_TEST_PREPARE_SID
        TFM_PS_TEST_PREPARE_SID,
#endif
#ifdef TFM_PLATFORM_SERVICE_SID
        TFM_PLATFORM_SERVICE_SID,
#endif
#ifdef TFM_FIRMWARE_UPDATE_SERVICE_SID
        TFM_FIRMWARE_UPDATE_SERVICE_SID,
#endif
#ifdef IPC_SERVICE_TEST_BASIC_SID
        IPC_SERVICE_TEST_BASIC_SID,
#endif
#ifdef IPC_SERVICE_TEST_STATELESS_ROT_SID
        IPC_SERVICE_TEST_STATELESS_ROT_SID,
#endif
#ifdef TFM_FPU_CHECK_FP_CALLEE_REGISTER_SID
        TFM_FPU_CHECK_FP_CALLEE_REGISTER_SID,
#endif
#ifdef TFM_FPU_TEST_NS_PREEMPT_S_SID
        TFM_FPU_TEST_NS_PREEMPT_S_SID,
#endif
#ifdef IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_SID
        IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_SID,
#endif
#ifdef IPC_CLIENT_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SID
        IPC_CLIENT_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SID,
#endif
#ifdef IPC_CLIENT_TEST_APP_ACCESS_PSA_MEM_SID
        IPC_CLIENT_TEST_APP_ACCESS_PSA_MEM_SID,
#endif
#ifdef IPC_CLIENT_TEST_MEM_CHECK_SID
        IPC_CLIENT_TEST_MEM_CHECK_SID,
#endif
#ifdef IPC_CLIENT_TEST_RETRIEVE_APP_MEM_SID
        IPC_CLIENT_TEST_RETRIEVE_APP_MEM_SID,
#endif
#ifdef SFN_TEST_STATELESS_SID
        SFN_TEST_STATELESS_SID,
#endif
#ifdef SFN_TEST_CONNECTION_BASED_SID
        SFN_TEST_CONNECTION_BASED_SID,
#endif
#ifdef TFM_DELEGATED_ATTESTATION_SID
        TFM_DELEGATED_ATTESTATION_SID,
#endif
#ifdef TFM_MEASURED_BOOT_SID
        TFM_MEASURED_BOOT_SID,
#endif
    },
    .services = {
        {
            .name_strid             = STRING_PTR_TO_STRID("TFM_SECURE_CLIENT_SRV_DUMMY"),
            .sfn                    = 0,
            .signal                 = TFM_SECURE_CLIENT_SRV_DUMMY_SIGNAL,

            .sid                    = 0x0000F000,
            .flags                  = 0
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
    },
#if TFM_LVL == 3
    .assets                         = {
        {
            .mem.start              = (uintptr_t)&REGION_NAME(Image$$, PT_TFM_SP_SECURE_TEST_PARTITION_PRIVATE, _DATA_START$$Base),
            .mem.limit              = (uintptr_t)&REGION_NAME(Image$$, PT_TFM_SP_SECURE_TEST_PARTITION_PRIVATE, _DATA_END$$Base),
            .attr                   = ASSET_ATTR_READ_WRITE,
        },
        {
            .dev.dev_ref            = PTR_TO_REFERENCE(TFM_PERIPHERAL_STD_UART),
            .attr                   = ASSET_ATTR_NAMED_MMIO
                                    | ASSET_ATTR_READ_WRITE,
        },
    },
#else
    .assets                         = {
        {
            .dev.dev_ref            = PTR_TO_REFERENCE(TFM_PERIPHERAL_STD_UART),
            .attr                   = ASSET_ATTR_NAMED_MMIO
                                    | ASSET_ATTR_READ_WRITE,
        },
    },
#endif
};

/* Placeholder for partition and service runtime space. Do not reference it. */
#if defined(__ICCARM__)
#pragma location=".bss.part_runtime_priority_low"
__root
#endif /* __ICCARM__ */
static struct partition_t tfm_sp_secure_test_partition_partition_runtime_item
    __attribute__((used, section(".bss.part_runtime_priority_low")));
#if defined(__ICCARM__)
#pragma location = ".bss.serv_runtime_priority_low"
__root
#endif /* __ICCARM__ */
static struct service_t tfm_sp_secure_test_partition_service_runtime_item[TFM_SP_SECURE_TEST_PARTITION_NSERVS]
    __attribute__((used, section(".bss.serv_runtime_priority_low")));
