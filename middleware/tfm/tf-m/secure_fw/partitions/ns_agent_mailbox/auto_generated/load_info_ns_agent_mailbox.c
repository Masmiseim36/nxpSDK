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
#include "psa_manifest/ns_agent_mailbox.h"

#define TFM_NS_MAILBOX_AGENT_NDEPS                              (0)
#define TFM_NS_MAILBOX_AGENT_NSERVS                             (0)
#if TFM_LVL == 3
#define TFM_NS_MAILBOX_AGENT_NASSETS                            (0 + 1)
#else
#define TFM_NS_MAILBOX_AGENT_NASSETS                            (0)
#endif
#define TFM_NS_MAILBOX_AGENT_NIRQS                              (1)

/* Memory region declaration */
#if TFM_LVL == 3
REGION_DECLARE(Image$$, PT_TFM_NS_MAILBOX_AGENT_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_NS_MAILBOX_AGENT_PRIVATE, _DATA_END$$Base);
#endif

extern uint8_t tfm_ns_mailbox_agent_stack[];

/* Entrypoint function declaration */
extern void ns_agent_mailbox_entry(void);

/* Interrupt init functions */
extern enum tfm_hal_status_t mailbox_irq_init(void *p_pt,
                                                            struct irq_load_info_t *p_ildi);

/* partition load info type definition */
struct partition_tfm_ns_mailbox_agent_load_info_t {
    /* common length load data */
    struct partition_load_info_t    load_info;
    /* per-partition variable length load data */
    uintptr_t                       stack_addr;
    uintptr_t                       heap_addr;
#if TFM_LVL == 3
    struct asset_desc_t             assets[TFM_NS_MAILBOX_AGENT_NASSETS];
#else
#endif
    struct irq_load_info_t          irqs[TFM_NS_MAILBOX_AGENT_NIRQS];
} __attribute__((aligned(4)));

/* Partition load, deps, service load data. Put to a dedicated section. */
#if defined(__ICCARM__)
#pragma location = ".part_load_priority_low"
__root
#endif /* __ICCARM__ */
const struct partition_tfm_ns_mailbox_agent_load_info_t tfm_ns_mailbox_agent_load
    __attribute__((used, section(".part_load_priority_low"))) = {
    .load_info = {
        .psa_ff_ver                 = 0x0101 | PARTITION_INFO_MAGIC,
        .pid                        = TFM_NS_MAILBOX_AGENT,
        .flags                      = 0
                                    | PARTITION_MODEL_IPC
                                    | PARTITION_MODEL_PSA_ROT
                                    | PARTITION_NS_AGENT
                                    | PARTITION_PRI_LOW,
        .entry                      = ENTRY_TO_POSITION(ns_agent_mailbox_entry),
        .stack_size                 = 0x800,
        .heap_size                  = 0,
        .ndeps                      = TFM_NS_MAILBOX_AGENT_NDEPS,
        .nservices                  = TFM_NS_MAILBOX_AGENT_NSERVS,
        .nassets                    = TFM_NS_MAILBOX_AGENT_NASSETS,
        .nirqs                      = TFM_NS_MAILBOX_AGENT_NIRQS,
    },
    .stack_addr                     = (uintptr_t)tfm_ns_mailbox_agent_stack,
    .heap_addr                      = 0,
#if TFM_LVL == 3
    .assets                         = {
        {
            .mem.start              = PART_REGION_ADDR(PT_TFM_NS_MAILBOX_AGENT_PRIVATE, _DATA_START$$Base),
            .mem.limit              = PART_REGION_ADDR(PT_TFM_NS_MAILBOX_AGENT_PRIVATE, _DATA_END$$Base),
            .attr                   = ASSET_ATTR_READ_WRITE,
        },
    },
#else
#endif
    .irqs = {
        {
            .init = mailbox_irq_init,
            .flih_func = 0,
            .pid = TFM_NS_MAILBOX_AGENT,
            .source = MAILBOX_IRQ,
            .signal = MAILBOX_SIGNAL,
        },
    },
};

/* Placeholder for partition and service runtime space. Do not reference it. */
#if defined(__ICCARM__)
#pragma location=".bss.part_runtime_priority_low"
__root
#endif /* __ICCARM__ */
static struct partition_t tfm_ns_mailbox_agent_partition_runtime_item
    __attribute__((used, section(".bss.part_runtime_priority_low")));
