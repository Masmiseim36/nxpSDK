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
#include "psa_manifest/tfm_ipc_service_test.h"

#define TFM_SP_IPC_SERVICE_TEST_NDEPS                           (0)
#define TFM_SP_IPC_SERVICE_TEST_NSERVS                          (7)
#if TFM_LVL == 3
#define TFM_SP_IPC_SERVICE_TEST_NASSETS                         (0 + 1)
#else
#define TFM_SP_IPC_SERVICE_TEST_NASSETS                         (0)
#endif
#define TFM_SP_IPC_SERVICE_TEST_NIRQS                           (0)

/* Memory region declaration */
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_IPC_SERVICE_TEST_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_IPC_SERVICE_TEST_PRIVATE, _DATA_END$$Base);
#endif

extern uint8_t tfm_sp_ipc_service_test_stack[];

/* Entrypoint function declaration */
extern void ipc_service_test_main(void);

/* Interrupt init functions */

/* partition load info type definition */
struct partition_tfm_sp_ipc_service_test_load_info_t {
    /* common length load data */
    struct partition_load_info_t    load_info;
    /* per-partition variable length load data */
    uintptr_t                       stack_addr;
    uintptr_t                       heap_addr;
    struct service_load_info_t      services[TFM_SP_IPC_SERVICE_TEST_NSERVS];
#if TFM_LVL == 3
    struct asset_desc_t             assets[TFM_SP_IPC_SERVICE_TEST_NASSETS];
#else
#endif
} __attribute__((aligned(4)));

/* Partition load, deps, service load data. Put to a dedicated section. */
#if defined(__ICCARM__)
#pragma location = ".part_load_priority_high"
__root
#endif /* __ICCARM__ */
const struct partition_tfm_sp_ipc_service_test_load_info_t tfm_sp_ipc_service_test_load
    __attribute__((used, section(".part_load_priority_high"))) = {
    .load_info = {
        .psa_ff_ver                 = 0x0101 | PARTITION_INFO_MAGIC,
        .pid                        = TFM_SP_IPC_SERVICE_TEST,
        .flags                      = 0
                                    | PARTITION_MODEL_IPC
                                    | PARTITION_MODEL_PSA_ROT
                                    | PARTITION_PRI_HIGH,
        .entry                      = ENTRY_TO_POSITION(ipc_service_test_main),
        .stack_size                 = 0x0220,
        .heap_size                  = 0,
        .ndeps                      = TFM_SP_IPC_SERVICE_TEST_NDEPS,
        .nservices                  = TFM_SP_IPC_SERVICE_TEST_NSERVS,
        .nassets                    = TFM_SP_IPC_SERVICE_TEST_NASSETS,
        .nirqs                      = TFM_SP_IPC_SERVICE_TEST_NIRQS,
    },
    .stack_addr                     = (uintptr_t)tfm_sp_ipc_service_test_stack,
    .heap_addr                      = 0,
    .services = {
        {
            .name_strid             = STRING_PTR_TO_STRID("IPC_SERVICE_TEST_BASIC"),
            .sfn                    = 0,
            .signal                 = IPC_SERVICE_TEST_BASIC_SIGNAL,

            .sid                    = 0x0000F080,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM"),
            .sfn                    = 0,
            .signal                 = IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM_SIGNAL,

            .sid                    = 0x0000F081,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM"),
            .sfn                    = 0,
            .signal                 = IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SIGNAL,

            .sid                    = 0x0000F082,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM"),
            .sfn                    = 0,
            .signal                 = IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_SIGNAL,

            .sid                    = 0x0000F083,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("IPC_SERVICE_TEST_CLIENT_PROGRAMMER_ERROR"),
            .sfn                    = 0,
            .signal                 = IPC_SERVICE_TEST_CLIENT_PROGRAMMER_ERROR_SIGNAL,

            .sid                    = 0x0000F084,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("IPC_SERVICE_TEST_STATELESS_ROT"),
            .sfn                    = 0,
            .signal                 = IPC_SERVICE_TEST_STATELESS_ROT_SIGNAL,

            .sid                    = 0x0000F085,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_FLAG_STATELESS | 0x6
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("IPC_SERVICE_TEST_MMIOVEC"),
            .sfn                    = 0,
            .signal                 = IPC_SERVICE_TEST_MMIOVEC_SIGNAL,

            .sid                    = 0x0000F086,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_FLAG_STATELESS | 0x7
                                    | SERVICE_FLAG_MM_IOVEC
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
    },
#if TFM_LVL == 3
    .assets                         = {
        {
            .mem.start              = (uintptr_t)&REGION_NAME(Image$$, PT_TFM_SP_IPC_SERVICE_TEST_PRIVATE, _DATA_START$$Base),
            .mem.limit              = (uintptr_t)&REGION_NAME(Image$$, PT_TFM_SP_IPC_SERVICE_TEST_PRIVATE, _DATA_END$$Base),
            .attr                   = ASSET_ATTR_READ_WRITE,
        },
    },
#else
#endif
};

/* Placeholder for partition and service runtime space. Do not reference it. */
#if defined(__ICCARM__)
#pragma location=".bss.part_runtime_priority_high"
__root
#endif /* __ICCARM__ */
static struct partition_t tfm_sp_ipc_service_test_partition_runtime_item
    __attribute__((used, section(".bss.part_runtime_priority_high")));
#if defined(__ICCARM__)
#pragma location = ".bss.serv_runtime_priority_high"
__root
#endif /* __ICCARM__ */
static struct service_t tfm_sp_ipc_service_test_service_runtime_item[TFM_SP_IPC_SERVICE_TEST_NSERVS]
    __attribute__((used, section(".bss.serv_runtime_priority_high")));
