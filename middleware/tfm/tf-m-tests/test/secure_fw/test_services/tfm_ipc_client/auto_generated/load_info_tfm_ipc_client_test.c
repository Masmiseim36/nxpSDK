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
#include "region.h"
#include "region_defs.h"
#include "spm_ipc.h"
#include "load/interrupt_defs.h"
#include "load/partition_defs.h"
#include "load/service_defs.h"
#include "load/asset_defs.h"
#include "tfm_peripherals_def.h"
#include "psa_manifest/pid.h"
#include "psa_manifest/sid.h"
#include "psa_manifest/tfm_ipc_client_test.h"

#define TFM_SP_IPC_CLIENT_TEST_NDEPS                            (5)
#define TFM_SP_IPC_CLIENT_TEST_NSERVS                           (5)
#if TFM_LVL == 3
#define TFM_SP_IPC_CLIENT_TEST_NASSETS                          (0 + 1)
#else
#define TFM_SP_IPC_CLIENT_TEST_NASSETS                          (0)
#endif
#define TFM_SP_IPC_CLIENT_TEST_NIRQS                            (0)

/* Memory region declaration */
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_IPC_CLIENT_TEST_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_IPC_CLIENT_TEST_PRIVATE, _DATA_END$$Base);
#endif

extern uint8_t tfm_sp_ipc_client_test_stack[];

/* Entrypoint function declaration */
extern void ipc_client_test_main(void);

/* Interrupt init functions */

/* partition load info type definition */
struct partition_tfm_sp_ipc_client_test_load_info_t {
    /* common length load data */
    struct partition_load_info_t    load_info;
    /* per-partition variable length load data */
    uintptr_t                       stack_addr;
    uintptr_t                       heap_addr;
    uint32_t                        deps[TFM_SP_IPC_CLIENT_TEST_NDEPS];
    struct service_load_info_t      services[TFM_SP_IPC_CLIENT_TEST_NSERVS];
#if TFM_LVL == 3
    struct asset_desc_t             assets[TFM_SP_IPC_CLIENT_TEST_NASSETS];
#else
#endif
} __attribute__((aligned(4)));

/* Partition load, deps, service load data. Put to a dedicated section. */
#if defined(__ICCARM__)
#pragma location = ".part_load_priority_normal"
__root
#endif /* __ICCARM__ */
const struct partition_tfm_sp_ipc_client_test_load_info_t tfm_sp_ipc_client_test_load
    __attribute__((used, section(".part_load_priority_normal"))) = {
    .load_info = {
        .psa_ff_ver                 = 0x0100 | PARTITION_INFO_MAGIC,
        .pid                        = TFM_SP_IPC_CLIENT_TEST,
        .flags                      = 0
                                    | PARTITION_MODEL_IPC
                                    | PARTITION_PRI_NORMAL,
        .entry                      = ENTRY_TO_POSITION(ipc_client_test_main),
        .stack_size                 = 0x0300,
        .heap_size                  = 0,
        .ndeps                      = TFM_SP_IPC_CLIENT_TEST_NDEPS,
        .nservices                  = TFM_SP_IPC_CLIENT_TEST_NSERVS,
        .nassets                    = TFM_SP_IPC_CLIENT_TEST_NASSETS,
        .nirqs                      = TFM_SP_IPC_CLIENT_TEST_NIRQS,
    },
    .stack_addr                     = (uintptr_t)tfm_sp_ipc_client_test_stack,
    .heap_addr                      = 0,
    .deps = {
        IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SID,
        IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM_SID,
        IPC_SERVICE_TEST_BASIC_SID,
        IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_SID,
#ifdef TFM_SECURE_CLIENT_2_SID
        TFM_SECURE_CLIENT_2_SID,
#endif
    },
    .services = {
        {
            .name_strid             = STRING_PTR_TO_STRID("IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM"),
            .sfn                    = 0,
            .signal                 = IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_SIGNAL,

            .sid                    = 0x0000F061,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("IPC_CLIENT_TEST_PSA_ACCESS_APP_READ_ONLY_MEM"),
            .sfn                    = 0,
            .signal                 = IPC_CLIENT_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SIGNAL,

            .sid                    = 0x0000F062,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("IPC_CLIENT_TEST_APP_ACCESS_PSA_MEM"),
            .sfn                    = 0,
            .signal                 = IPC_CLIENT_TEST_APP_ACCESS_PSA_MEM_SIGNAL,

            .sid                    = 0x0000F063,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("IPC_CLIENT_TEST_MEM_CHECK"),
            .sfn                    = 0,
            .signal                 = IPC_CLIENT_TEST_MEM_CHECK_SIGNAL,

            .sid                    = 0x0000F064,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("IPC_CLIENT_TEST_RETRIEVE_APP_MEM"),
            .sfn                    = 0,
            .signal                 = IPC_CLIENT_TEST_RETRIEVE_APP_MEM_SIGNAL,

            .sid                    = 0x0000F065,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
    },
#if TFM_LVL == 3
    .assets                         = {
        {
            .mem.start              = PART_REGION_ADDR(PT_TFM_SP_IPC_CLIENT_TEST_PRIVATE, _DATA_START$$Base),
            .mem.limit              = PART_REGION_ADDR(PT_TFM_SP_IPC_CLIENT_TEST_PRIVATE, _DATA_END$$Base),
            .attr                   = ASSET_ATTR_READ_WRITE,
        },
    },
#else
#endif
};

/* Placeholder for partition and service runtime space. Do not reference it. */
#if defined(__ICCARM__)
#pragma location=".bss.part_runtime_priority_normal"
__root
#endif /* __ICCARM__ */
static struct partition_t tfm_sp_ipc_client_test_partition_runtime_item
    __attribute__((used, section(".bss.part_runtime_priority_normal")));
#if defined(__ICCARM__)
#pragma location = ".bss.serv_runtime_priority_normal"
__root
#endif /* __ICCARM__ */
static struct service_t tfm_sp_ipc_client_test_service_runtime_item[TFM_SP_IPC_CLIENT_TEST_NSERVS]
    __attribute__((used, section(".bss.serv_runtime_priority_normal")));
