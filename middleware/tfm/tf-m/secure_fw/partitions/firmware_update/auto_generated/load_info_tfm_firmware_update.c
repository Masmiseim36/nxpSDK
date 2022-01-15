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
#include "psa_manifest/tfm_firmware_update.h"

#define TFM_SP_FWU_NDEPS                                        (2)
#define TFM_SP_FWU_NSERVS                                       (6)
#if TFM_LVL == 3
#define TFM_SP_FWU_NASSETS                                      (0 + 1)
#else
#define TFM_SP_FWU_NASSETS                                      (0)
#endif
#define TFM_SP_FWU_NIRQS                                        (0)

/* Memory region declaration */
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_FWU_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_FWU_PRIVATE, _DATA_END$$Base);
#endif
extern uint8_t tfm_sp_fwu_stack[];

/* Entrypoint function declaration */
extern void tfm_fwu_init(void);

/* partition load info type definition */
struct partition_tfm_sp_fwu_load_info_t {
    /* common length load data */
    struct partition_load_info_t    load_info;
    /* per-partition variable length load data */
    uintptr_t                       stack_addr;
    uintptr_t                       heap_addr;
    uint32_t                        deps[TFM_SP_FWU_NDEPS];
    struct service_load_info_t      services[TFM_SP_FWU_NSERVS];
#if TFM_LVL == 3
    struct asset_desc_t             assets[TFM_SP_FWU_NASSETS];
#else
#endif
} __attribute__((aligned(4)));

/* Partition load, deps, service load data. Put to a dedicated section. */
#if defined(__ICCARM__)
#pragma location = ".part_load"
__root
#endif /* __ICCARM__ */
const struct partition_tfm_sp_fwu_load_info_t tfm_sp_fwu_load
    __attribute__((used, section(".part_load"))) = {
    .load_info = {
        .psa_ff_ver                 = 0x0100 | PARTITION_INFO_MAGIC,
        .pid                        = TFM_SP_FWU,
        .flags                      = 0
                                    | SPM_PART_FLAG_IPC
                                    | SPM_PART_FLAG_PSA_ROT
                                    | PARTITION_PRI_NORMAL,
        .entry                      = ENTRY_TO_POSITION(tfm_fwu_init),
        .stack_size                 = 0x2000,
        .heap_size                  = 0,
        .ndeps                      = TFM_SP_FWU_NDEPS,
        .nservices                  = TFM_SP_FWU_NSERVS,
        .nassets                    = TFM_SP_FWU_NASSETS,
        .nirqs                      = TFM_SP_FWU_NIRQS,
    },
    .stack_addr                     = (uintptr_t)tfm_sp_fwu_stack,
    .heap_addr                      = 0,
    .deps = {
        TFM_CRYPTO_SID,
        TFM_SP_PLATFORM_SYSTEM_RESET_SID,
    },
    .services = {
        {
            .name_strid             = STRING_PTR_TO_STRID("TFM_FWU_WRITE"),
            .signal                 = TFM_FWU_WRITE_SIGNAL,
            .sid                    = 0x000000A0,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("TFM_FWU_INSTALL"),
            .signal                 = TFM_FWU_INSTALL_SIGNAL,
            .sid                    = 0x000000A1,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("TFM_FWU_ABORT"),
            .signal                 = TFM_FWU_ABORT_SIGNAL,
            .sid                    = 0x000000A2,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("TFM_FWU_QUERY"),
            .signal                 = TFM_FWU_QUERY_SIGNAL,
            .sid                    = 0x000000A3,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("TFM_FWU_REQUEST_REBOOT"),
            .signal                 = TFM_FWU_REQUEST_REBOOT_SIGNAL,
            .sid                    = 0x000000A4,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("TFM_FWU_ACCEPT"),
            .signal                 = TFM_FWU_ACCEPT_SIGNAL,
            .sid                    = 0x000000A5,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
    },
#if TFM_LVL == 3
    .assets                         = {
        {
            .mem.addr_x             = PART_REGION_ADDR(PT_TFM_SP_FWU_PRIVATE, _DATA_START$$Base),
            .mem.addr_y             = PART_REGION_ADDR(PT_TFM_SP_FWU_PRIVATE, _DATA_END$$Base),
            .attr                   = ASSET_MEM_RD_BIT | ASSET_MEM_WR_BIT,
        },
    },
#else
#endif
};

/* Placeholder for partition and service runtime space. Do not reference it. */
#if defined(__ICCARM__)
#pragma location=".bss.part_runtime"
__root
#endif /* __ICCARM__ */
static struct partition_t tfm_sp_fwu_partition_runtime_item
    __attribute__((used, section(".bss.part_runtime")));
#if defined(__ICCARM__)
#pragma location = ".bss.serv_runtime"
__root
#endif /* __ICCARM__ */
static struct service_t tfm_sp_fwu_service_runtime_item[TFM_SP_FWU_NSERVS]
    __attribute__((used, section(".bss.serv_runtime")));
