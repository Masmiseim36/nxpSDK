/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#include <stdint.h>
#include <stddef.h>
#include "region.h"
#include "spm_ipc.h"
#include "load/irq_defs.h"
#include "load/partition_defs.h"
#include "load/service_defs.h"
#include "load/asset_defs.h"
#include "tfm_peripherals_def.h"
#include "psa_manifest/pid.h"
#include "psa_manifest/sid.h"
#include "psa_manifest/tfm_secure_client_service.h"

#define TFM_SP_SECURE_TEST_PARTITION_NDEPS                      (16)
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
#pragma location = ".part_load"
__root
#endif /* __ICCARM__ */
const struct partition_tfm_sp_secure_test_partition_load_info_t tfm_sp_secure_test_partition_load
    __attribute__((used, section(".part_load"))) = {
    .load_info = {
        .psa_ff_ver                 = 0x0100 | PARTITION_INFO_MAGIC,
        .pid                        = TFM_SP_SECURE_TEST_PARTITION,
        .flags                      = 0
                                    | SPM_PART_FLAG_IPC
                                    | SPM_PART_FLAG_PSA_ROT
                                    | PARTITION_PRI_NORMAL,
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
        TFM_CRYPTO_SID,
        TFM_PROTECTED_STORAGE_SERVICE_SID,
        TFM_INTERNAL_TRUSTED_STORAGE_SERVICE_SID,
        TFM_ATTESTATION_SERVICE_SID,
        TFM_PS_TEST_PREPARE_SID,
        TFM_SP_PLATFORM_SYSTEM_RESET_SID,
        TFM_SP_PLATFORM_IOCTL_SID,
        TFM_FWU_WRITE_SID,
        TFM_FWU_INSTALL_SID,
        TFM_FWU_ABORT_SID,
        TFM_FWU_QUERY_SID,
        TFM_FWU_REQUEST_REBOOT_SID,
        TFM_FWU_ACCEPT_SID,
        IPC_SERVICE_TEST_STATELESS_ROT_SID,
        TFM_ATTEST_TEST_GET_PUBLIC_KEY_SID,
    },
    .services = {
        {
            .name_strid             = STRING_PTR_TO_STRID("TFM_SECURE_CLIENT_SFN_RUN_TESTS"),
            .signal                 = TFM_SECURE_CLIENT_SFN_RUN_TESTS_SIGNAL,
            .sid                    = 0x0000F000,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
    },
#if TFM_LVL == 3
    .assets                         = {
        {
            .mem.addr_x             = PART_REGION_ADDR(PT_TFM_SP_SECURE_TEST_PARTITION_PRIVATE, _DATA_START$$Base),
            .mem.addr_y             = PART_REGION_ADDR(PT_TFM_SP_SECURE_TEST_PARTITION_PRIVATE, _DATA_END$$Base),
            .attr                   = ASSET_MEM_RD_BIT | ASSET_MEM_WR_BIT,
        },
        {
            .dev.addr_ref           = PTR_TO_REFERENCE(TFM_PERIPHERAL_STD_UART),
            .attr                   = ASSET_DEV_REF_BIT,
        },
    },
#else
    .assets                         = {
        {
            .dev.addr_ref           = PTR_TO_REFERENCE(TFM_PERIPHERAL_STD_UART),
            .attr                   = ASSET_DEV_REF_BIT,
        },
    },
#endif
};

/* Placeholder for partition and service runtime space. Do not reference it. */
#if defined(__ICCARM__)
#pragma location=".bss.part_runtime"
__root
#endif /* __ICCARM__ */
static struct partition_t tfm_sp_secure_test_partition_partition_runtime_item
    __attribute__((used, section(".bss.part_runtime")));
#if defined(__ICCARM__)
#pragma location = ".bss.serv_runtime"
__root
#endif /* __ICCARM__ */
static struct service_t tfm_sp_secure_test_partition_service_runtime_item[TFM_SP_SECURE_TEST_PARTITION_NSERVS]
    __attribute__((used, section(".bss.serv_runtime")));
