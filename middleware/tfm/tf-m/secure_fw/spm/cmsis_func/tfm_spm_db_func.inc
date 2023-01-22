/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#ifndef __TFM_SPM_DB_FUNC_INC__
#define __TFM_SPM_DB_FUNC_INC__

#include "array.h"
#include "psa_manifest/sid.h"

/**************************************************************************/
/** IRQ count per partition */
/**************************************************************************/
#ifdef TFM_PARTITION_NS_AGENT_MAILBOX //NXP
#define TFM_PARTITION_TFM_NS_MAILBOX_AGENT_IRQ_COUNT 1
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP

#ifdef TFM_PARTITION_PROTECTED_STORAGE //NXP
#define TFM_PARTITION_TFM_SP_PS_IRQ_COUNT 0
#endif /* TFM_PARTITION_PROTECTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE //NXP
#define TFM_PARTITION_TFM_SP_ITS_IRQ_COUNT 0
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_AUDIT_LOG //NXP
#define TFM_PARTITION_TFM_SP_AUDIT_LOG_IRQ_COUNT 0
#endif /* TFM_PARTITION_AUDIT_LOG */ //NXP

#ifdef TFM_PARTITION_CRYPTO //NXP
#define TFM_PARTITION_TFM_SP_CRYPTO_IRQ_COUNT 0
#endif /* TFM_PARTITION_CRYPTO */ //NXP

#ifdef TFM_PARTITION_PLATFORM //NXP
#define TFM_PARTITION_TFM_SP_PLATFORM_IRQ_COUNT 0
#endif /* TFM_PARTITION_PLATFORM */ //NXP

#ifdef TFM_PARTITION_INITIAL_ATTESTATION //NXP
#define TFM_PARTITION_TFM_SP_INITIAL_ATTESTATION_IRQ_COUNT 0
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */ //NXP

#ifdef TFM_PARTITION_FIRMWARE_UPDATE //NXP
#define TFM_PARTITION_TFM_SP_FWU_IRQ_COUNT 0
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */ //NXP

#ifdef TEST_NS_CORE //NXP
#define TFM_PARTITION_TFM_SP_CORE_TEST_IRQ_COUNT 0
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_NS_CORE //NXP
#define TFM_PARTITION_TFM_SP_CORE_TEST_2_IRQ_COUNT 0
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
#define TFM_PARTITION_TFM_SP_SECURE_TEST_PARTITION_IRQ_COUNT 0
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
#define TFM_PARTITION_TFM_SP_IPC_SERVICE_TEST_IRQ_COUNT 0
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
#define TFM_PARTITION_TFM_SP_IPC_CLIENT_TEST_IRQ_COUNT 0
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TEST_S_PS //NXP
#define TFM_PARTITION_TFM_SP_PS_TEST_IRQ_COUNT 0
#endif /* TEST_S_PS */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
#define TFM_PARTITION_TFM_SP_SECURE_CLIENT_2_IRQ_COUNT 0
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TEST_NS_SLIH_IRQ //NXP
#define TFM_PARTITION_TFM_SP_SLIH_TEST_IRQ_COUNT 1
#endif /* TEST_NS_SLIH_IRQ */ //NXP

#ifdef TEST_NS_FLIH_IRQ //NXP
#define TFM_PARTITION_TFM_SP_FLIH_TEST_IRQ_COUNT 1
#endif /* TEST_NS_FLIH_IRQ */ //NXP

#ifdef TEST_PARTITION_FPU_TEST //NXP
#define TFM_PARTITION_TFM_SP_FPU_SERVICE_TEST_IRQ_COUNT 0
#endif /* TEST_PARTITION_FPU_TEST */ //NXP

/**************************************************************************/
/** Declarations of partition init functions */
/**************************************************************************/
#ifdef TFM_PARTITION_NS_AGENT_MAILBOX //NXP
extern void ns_agent_mailbox_entry(void);
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP

#ifdef TFM_PARTITION_PROTECTED_STORAGE //NXP
extern void tfm_ps_entry(void);
#endif /* TFM_PARTITION_PROTECTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE //NXP
extern void tfm_its_entry(void);
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_AUDIT_LOG //NXP
extern void audit_core_init(void);
#endif /* TFM_PARTITION_AUDIT_LOG */ //NXP

#ifdef TFM_PARTITION_CRYPTO //NXP
extern void tfm_crypto_init(void);
#endif /* TFM_PARTITION_CRYPTO */ //NXP

#ifdef TFM_PARTITION_PLATFORM //NXP
extern void platform_sp_init(void);
#endif /* TFM_PARTITION_PLATFORM */ //NXP

#ifdef TFM_PARTITION_INITIAL_ATTESTATION //NXP
extern void attest_partition_init(void);
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */ //NXP

#ifdef TFM_PARTITION_FIRMWARE_UPDATE //NXP
extern void tfm_fwu_entry(void);
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */ //NXP

#ifdef TEST_NS_CORE //NXP
extern void core_test_init(void);
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_NS_CORE //NXP
extern void core_test_2_init(void);
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
extern void tfm_secure_client_service_init(void);
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
extern void ipc_service_test_main(void);
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
extern void ipc_client_test_main(void);
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TEST_S_PS //NXP
extern void tfm_ps_test_init(void);
#endif /* TEST_S_PS */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
extern void tfm_secure_client_2_init(void);
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TEST_NS_SLIH_IRQ //NXP
extern void tfm_slih_test_service_entry(void);
#endif /* TEST_NS_SLIH_IRQ */ //NXP

#ifdef TEST_NS_FLIH_IRQ //NXP
extern void tfm_flih_test_service_entry(void);
#endif /* TEST_NS_FLIH_IRQ */ //NXP

#ifdef TEST_PARTITION_FPU_TEST //NXP
extern void fpu_service_test_main(void);
#endif /* TEST_PARTITION_FPU_TEST */ //NXP

/**************************************************************************/
/** Context stacks for IRQ handling */
/**************************************************************************/
/* The max size of the context stack can be calculated as a function of the IRQ
 * count of the secure partition:
 *
 * max_stack_size = intr_ctx_size + (IRQ_CNT * (intr_ctx_size + hndl_ctx_size))
 *
 * where:
 *   intr_ctx: Frame pushed when the partition is interrupted
 *   hndl_ctx: Frame pushed when the partition is handling an interrupt
 */
static uint32_t ns_interrupt_ctx_stack[
        sizeof(struct interrupted_ctx_stack_frame_t) / sizeof(uint32_t)];

static uint32_t tfm_core_interrupt_ctx_stack[
        sizeof(struct interrupted_ctx_stack_frame_t) / sizeof(uint32_t)];

#ifdef TFM_PARTITION_NS_AGENT_MAILBOX //NXP
static uint32_t ctx_stack_TFM_NS_MAILBOX_AGENT[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_NS_MAILBOX_AGENT_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP

#ifdef TFM_PARTITION_PROTECTED_STORAGE //NXP
static uint32_t ctx_stack_TFM_SP_PS[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_PS_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_PROTECTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE //NXP
static uint32_t ctx_stack_TFM_SP_ITS[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_ITS_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_AUDIT_LOG //NXP
static uint32_t ctx_stack_TFM_SP_AUDIT_LOG[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_AUDIT_LOG_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_AUDIT_LOG */ //NXP

#ifdef TFM_PARTITION_CRYPTO //NXP
static uint32_t ctx_stack_TFM_SP_CRYPTO[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_CRYPTO_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_CRYPTO */ //NXP

#ifdef TFM_PARTITION_PLATFORM //NXP
static uint32_t ctx_stack_TFM_SP_PLATFORM[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_PLATFORM_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_PLATFORM */ //NXP

#ifdef TFM_PARTITION_INITIAL_ATTESTATION //NXP
static uint32_t ctx_stack_TFM_SP_INITIAL_ATTESTATION[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_INITIAL_ATTESTATION_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */ //NXP

#ifdef TFM_PARTITION_FIRMWARE_UPDATE //NXP
static uint32_t ctx_stack_TFM_SP_FWU[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_FWU_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */ //NXP

#ifdef TEST_NS_CORE //NXP
static uint32_t ctx_stack_TFM_SP_CORE_TEST[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_CORE_TEST_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_NS_CORE //NXP
static uint32_t ctx_stack_TFM_SP_CORE_TEST_2[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_CORE_TEST_2_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
static uint32_t ctx_stack_TFM_SP_SECURE_TEST_PARTITION[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_SECURE_TEST_PARTITION_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
static uint32_t ctx_stack_TFM_SP_IPC_SERVICE_TEST[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_IPC_SERVICE_TEST_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
static uint32_t ctx_stack_TFM_SP_IPC_CLIENT_TEST[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_IPC_CLIENT_TEST_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TEST_S_PS //NXP
static uint32_t ctx_stack_TFM_SP_PS_TEST[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_PS_TEST_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TEST_S_PS */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
static uint32_t ctx_stack_TFM_SP_SECURE_CLIENT_2[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_SECURE_CLIENT_2_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TEST_NS_SLIH_IRQ //NXP
static uint32_t ctx_stack_TFM_SP_SLIH_TEST[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_SLIH_TEST_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TEST_NS_SLIH_IRQ */ //NXP

#ifdef TEST_NS_FLIH_IRQ //NXP
static uint32_t ctx_stack_TFM_SP_FLIH_TEST[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_FLIH_TEST_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TEST_NS_FLIH_IRQ */ //NXP

#ifdef TEST_PARTITION_FPU_TEST //NXP
static uint32_t ctx_stack_TFM_SP_FPU_SERVICE_TEST[
        (sizeof(struct interrupted_ctx_stack_frame_t) +
            (TFM_PARTITION_TFM_SP_FPU_SERVICE_TEST_IRQ_COUNT) * (
                sizeof(struct interrupted_ctx_stack_frame_t) +
                sizeof(struct handler_ctx_stack_frame_t)
        )) / sizeof(uint32_t)];
#endif /* TEST_PARTITION_FPU_TEST */ //NXP

uint32_t *ctx_stack_list[] =
{
    ns_interrupt_ctx_stack,
    tfm_core_interrupt_ctx_stack,
#ifdef TFM_PARTITION_NS_AGENT_MAILBOX //NXP
    ctx_stack_TFM_NS_MAILBOX_AGENT,
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP
#ifdef TFM_PARTITION_PROTECTED_STORAGE //NXP
    ctx_stack_TFM_SP_PS,
#endif /* TFM_PARTITION_PROTECTED_STORAGE */ //NXP
#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE //NXP
    ctx_stack_TFM_SP_ITS,
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */ //NXP
#ifdef TFM_PARTITION_AUDIT_LOG //NXP
    ctx_stack_TFM_SP_AUDIT_LOG,
#endif /* TFM_PARTITION_AUDIT_LOG */ //NXP
#ifdef TFM_PARTITION_CRYPTO //NXP
    ctx_stack_TFM_SP_CRYPTO,
#endif /* TFM_PARTITION_CRYPTO */ //NXP
#ifdef TFM_PARTITION_PLATFORM //NXP
    ctx_stack_TFM_SP_PLATFORM,
#endif /* TFM_PARTITION_PLATFORM */ //NXP
#ifdef TFM_PARTITION_INITIAL_ATTESTATION //NXP
    ctx_stack_TFM_SP_INITIAL_ATTESTATION,
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */ //NXP
#ifdef TFM_PARTITION_FIRMWARE_UPDATE //NXP
    ctx_stack_TFM_SP_FWU,
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */ //NXP
#ifdef TEST_NS_CORE //NXP
    ctx_stack_TFM_SP_CORE_TEST,
#endif /* TEST_NS_CORE */ //NXP
#ifdef TEST_NS_CORE //NXP
    ctx_stack_TFM_SP_CORE_TEST_2,
#endif /* TEST_NS_CORE */ //NXP
#ifdef TEST_FRAMEWORK_S //NXP
    ctx_stack_TFM_SP_SECURE_TEST_PARTITION,
#endif /* TEST_FRAMEWORK_S */ //NXP
#ifdef TFM_PARTITION_IPC_TEST //NXP
    ctx_stack_TFM_SP_IPC_SERVICE_TEST,
#endif /* TFM_PARTITION_IPC_TEST */ //NXP
#ifdef TFM_PARTITION_IPC_TEST //NXP
    ctx_stack_TFM_SP_IPC_CLIENT_TEST,
#endif /* TFM_PARTITION_IPC_TEST */ //NXP
#ifdef TEST_S_PS //NXP
    ctx_stack_TFM_SP_PS_TEST,
#endif /* TEST_S_PS */ //NXP
#ifdef TEST_FRAMEWORK_S //NXP
    ctx_stack_TFM_SP_SECURE_CLIENT_2,
#endif /* TEST_FRAMEWORK_S */ //NXP
#ifdef TEST_NS_SLIH_IRQ //NXP
    ctx_stack_TFM_SP_SLIH_TEST,
#endif /* TEST_NS_SLIH_IRQ */ //NXP
#ifdef TEST_NS_FLIH_IRQ //NXP
    ctx_stack_TFM_SP_FLIH_TEST,
#endif /* TEST_NS_FLIH_IRQ */ //NXP
#ifdef TEST_PARTITION_FPU_TEST //NXP
    ctx_stack_TFM_SP_FPU_SERVICE_TEST,
#endif /* TEST_PARTITION_FPU_TEST */ //NXP
};

/**************************************************************************/
/** Dependencies array for Secure Partition */
/**************************************************************************/
#ifdef TFM_PARTITION_PROTECTED_STORAGE //NXP
static int32_t dependencies_TFM_SP_PS[] =
{
    TFM_CRYPTO_SID,
    TFM_INTERNAL_TRUSTED_STORAGE_SERVICE_SID,
    TFM_PLATFORM_SERVICE_SID,
};
#endif /* TFM_PARTITION_PROTECTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_CRYPTO //NXP
static int32_t dependencies_TFM_SP_CRYPTO[] =
{
    TFM_INTERNAL_TRUSTED_STORAGE_SERVICE_SID,
};
#endif /* TFM_PARTITION_CRYPTO */ //NXP

#ifdef TFM_PARTITION_INITIAL_ATTESTATION //NXP
static int32_t dependencies_TFM_SP_INITIAL_ATTESTATION[] =
{
    TFM_CRYPTO_SID,
};
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */ //NXP

#ifdef TFM_PARTITION_FIRMWARE_UPDATE //NXP
static int32_t dependencies_TFM_SP_FWU[] =
{
    TFM_CRYPTO_SID,
    TFM_PLATFORM_SERVICE_SID,
};
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */ //NXP

#ifdef TEST_NS_CORE //NXP
static int32_t dependencies_TFM_SP_CORE_TEST[] =
{
    SPM_CORE_TEST_2_INVERT_SID,
    SPM_CORE_TEST_2_GET_EVERY_SECOND_BYTE_SID,
    SPM_CORE_TEST_2_SLAVE_SERVICE_SID,
};
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
static int32_t dependencies_TFM_SP_SECURE_TEST_PARTITION[] =
{
    TFM_SECURE_CLIENT_2_SID,
};
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
static int32_t dependencies_TFM_SP_IPC_CLIENT_TEST[] =
{
    IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SID,
    IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM_SID,
    IPC_SERVICE_TEST_BASIC_SID,
    IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_SID,
};
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TEST_S_PS //NXP
static int32_t dependencies_TFM_SP_PS_TEST[] =
{
    TFM_CRYPTO_SID,
    TFM_INTERNAL_TRUSTED_STORAGE_SERVICE_SID,
};
#endif /* TEST_S_PS */ //NXP

/**************************************************************************/
/** The static data of the partition list */
/**************************************************************************/
const struct spm_partition_static_data_t static_data_list[] =
{
    {
        .partition_id         = 0,
        .partition_flags      = SPM_PART_FLAG_NS_AGENT,
    },

    {
        .partition_id         = TFM_SP_CORE_ID,
        .partition_flags      = SPM_PART_FLAG_APP_ROT | SPM_PART_FLAG_PSA_ROT,
    },

#ifdef TFM_PARTITION_NS_AGENT_MAILBOX //NXP
    {
        .partition_id         = TFM_NS_MAILBOX_AGENT,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = ns_agent_mailbox_entry,
        .dependencies_num     = 0,
        .p_dependencies       = NULL,
    },
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP

#ifdef TFM_PARTITION_PROTECTED_STORAGE //NXP
    {
        .partition_id         = TFM_SP_PS,
        .partition_flags      = 0
                              | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = tfm_ps_entry,
        .dependencies_num     = 3,
        .p_dependencies       = dependencies_TFM_SP_PS,
    },
#endif /* TFM_PARTITION_PROTECTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE //NXP
    {
        .partition_id         = TFM_SP_ITS,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = tfm_its_entry,
        .dependencies_num     = 0,
        .p_dependencies       = NULL,
    },
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_AUDIT_LOG //NXP
    {
        .partition_id         = TFM_SP_AUDIT_LOG,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = audit_core_init,
        .dependencies_num     = 0,
        .p_dependencies       = NULL,
    },
#endif /* TFM_PARTITION_AUDIT_LOG */ //NXP

#ifdef TFM_PARTITION_CRYPTO //NXP
    {
        .partition_id         = TFM_SP_CRYPTO,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = tfm_crypto_init,
        .dependencies_num     = 1,
        .p_dependencies       = dependencies_TFM_SP_CRYPTO,
    },
#endif /* TFM_PARTITION_CRYPTO */ //NXP

#ifdef TFM_PARTITION_PLATFORM //NXP
    {
        .partition_id         = TFM_SP_PLATFORM,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = platform_sp_init,
        .dependencies_num     = 0,
        .p_dependencies       = NULL,
    },
#endif /* TFM_PARTITION_PLATFORM */ //NXP

#ifdef TFM_PARTITION_INITIAL_ATTESTATION //NXP
    {
        .partition_id         = TFM_SP_INITIAL_ATTESTATION,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = attest_partition_init,
        .dependencies_num     = 1,
        .p_dependencies       = dependencies_TFM_SP_INITIAL_ATTESTATION,
    },
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */ //NXP

#ifdef TFM_PARTITION_FIRMWARE_UPDATE //NXP
    {
        .partition_id         = TFM_SP_FWU,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = tfm_fwu_entry,
        .dependencies_num     = 2,
        .p_dependencies       = dependencies_TFM_SP_FWU,
    },
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */ //NXP

#ifdef TEST_NS_CORE //NXP
    {
        .partition_id         = TFM_SP_CORE_TEST,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = core_test_init,
        .dependencies_num     = 3,
        .p_dependencies       = dependencies_TFM_SP_CORE_TEST,
    },
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_NS_CORE //NXP
    {
        .partition_id         = TFM_SP_CORE_TEST_2,
        .partition_flags      = 0
                              | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = core_test_2_init,
        .dependencies_num     = 0,
        .p_dependencies       = NULL,
    },
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
    {
        .partition_id         = TFM_SP_SECURE_TEST_PARTITION,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = tfm_secure_client_service_init,
        .dependencies_num     = 1,
        .p_dependencies       = dependencies_TFM_SP_SECURE_TEST_PARTITION,
    },
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
    {
        .partition_id         = TFM_SP_IPC_SERVICE_TEST,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = ipc_service_test_main,
        .dependencies_num     = 0,
        .p_dependencies       = NULL,
    },
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
    {
        .partition_id         = TFM_SP_IPC_CLIENT_TEST,
        .partition_flags      = 0
                              | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = ipc_client_test_main,
        .dependencies_num     = 4,
        .p_dependencies       = dependencies_TFM_SP_IPC_CLIENT_TEST,
    },
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TEST_S_PS //NXP
    {
        .partition_id         = TFM_SP_PS_TEST,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = tfm_ps_test_init,
        .dependencies_num     = 2,
        .p_dependencies       = dependencies_TFM_SP_PS_TEST,
    },
#endif /* TEST_S_PS */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
    {
        .partition_id         = TFM_SP_SECURE_CLIENT_2,
        .partition_flags      = 0
                              | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = tfm_secure_client_2_init,
        .dependencies_num     = 0,
        .p_dependencies       = NULL,
    },
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TEST_NS_SLIH_IRQ //NXP
    {
        .partition_id         = TFM_SP_SLIH_TEST,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = tfm_slih_test_service_entry,
        .dependencies_num     = 0,
        .p_dependencies       = NULL,
    },
#endif /* TEST_NS_SLIH_IRQ */ //NXP

#ifdef TEST_NS_FLIH_IRQ //NXP
    {
        .partition_id         = TFM_SP_FLIH_TEST,
        .partition_flags      = 0
                              | SPM_PART_FLAG_PSA_ROT | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = tfm_flih_test_service_entry,
        .dependencies_num     = 0,
        .p_dependencies       = NULL,
    },
#endif /* TEST_NS_FLIH_IRQ */ //NXP

#ifdef TEST_PARTITION_FPU_TEST //NXP
    {
        .partition_id         = TFM_SP_FPU_SERVICE_TEST,
        .partition_flags      = 0
                              | SPM_PART_FLAG_APP_ROT
                              ,
        .partition_init       = fpu_service_test_main,
        .dependencies_num     = 0,
        .p_dependencies       = NULL,
    },
#endif /* TEST_PARTITION_FPU_TEST */ //NXP

};

/**************************************************************************/
/** The platform data of the partition list */
/**************************************************************************/
#ifdef TFM_PARTITION_AUDIT_LOG //NXP
const struct platform_data_t *
    platform_data_list_TFM_SP_AUDIT_LOG[] =
{
#ifdef AUDIT_UART_REDIRECTION
    TFM_PERIPHERAL_UART1,
#endif /* AUDIT_UART_REDIRECTION */
    NULL
};
#endif /* TFM_PARTITION_AUDIT_LOG */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
const struct platform_data_t *
    platform_data_list_TFM_SP_SECURE_TEST_PARTITION[] =
{
    TFM_PERIPHERAL_STD_UART,
    NULL
};
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TEST_NS_SLIH_IRQ //NXP
const struct platform_data_t *
    platform_data_list_TFM_SP_SLIH_TEST[] =
{
    TFM_PERIPHERAL_TIMER0,
    NULL
};
#endif /* TEST_NS_SLIH_IRQ */ //NXP

#ifdef TEST_NS_FLIH_IRQ //NXP
const struct platform_data_t *
    platform_data_list_TFM_SP_FLIH_TEST[] =
{
    TFM_PERIPHERAL_TIMER0,
    NULL
};
#endif /* TEST_NS_FLIH_IRQ */ //NXP

const struct platform_data_t **platform_data_list_list[] =
{
    NULL,
    NULL,

#ifdef TFM_PARTITION_NS_AGENT_MAILBOX //NXP
    NULL,
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP

#ifdef TFM_PARTITION_PROTECTED_STORAGE //NXP
    NULL,
#endif /* TFM_PARTITION_PROTECTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE //NXP
    NULL,
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */ //NXP

#ifdef TFM_PARTITION_AUDIT_LOG //NXP
    platform_data_list_TFM_SP_AUDIT_LOG,
#endif /* TFM_PARTITION_AUDIT_LOG */ //NXP

#ifdef TFM_PARTITION_CRYPTO //NXP
    NULL,
#endif /* TFM_PARTITION_CRYPTO */ //NXP

#ifdef TFM_PARTITION_PLATFORM //NXP
    NULL,
#endif /* TFM_PARTITION_PLATFORM */ //NXP

#ifdef TFM_PARTITION_INITIAL_ATTESTATION //NXP
    NULL,
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */ //NXP

#ifdef TFM_PARTITION_FIRMWARE_UPDATE //NXP
    NULL,
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */ //NXP

#ifdef TEST_NS_CORE //NXP
    NULL,
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_NS_CORE //NXP
    NULL,
#endif /* TEST_NS_CORE */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
    platform_data_list_TFM_SP_SECURE_TEST_PARTITION,
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
    NULL,
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TFM_PARTITION_IPC_TEST //NXP
    NULL,
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

#ifdef TEST_S_PS //NXP
    NULL,
#endif /* TEST_S_PS */ //NXP

#ifdef TEST_FRAMEWORK_S //NXP
    NULL,
#endif /* TEST_FRAMEWORK_S */ //NXP

#ifdef TEST_NS_SLIH_IRQ //NXP
    platform_data_list_TFM_SP_SLIH_TEST,
#endif /* TEST_NS_SLIH_IRQ */ //NXP

#ifdef TEST_NS_FLIH_IRQ //NXP
    platform_data_list_TFM_SP_FLIH_TEST,
#endif /* TEST_NS_FLIH_IRQ */ //NXP

#ifdef TEST_PARTITION_FPU_TEST //NXP
    NULL,
#endif /* TEST_PARTITION_FPU_TEST */ //NXP

};

/**************************************************************************/
/** The partition list for the DB */
/**************************************************************************/
static struct spm_partition_desc_t partition_list [] =
{
    {{0}}, /* placeholder for Non-secure internal partition */
    {{0}},   /* placeholder for TF-M Core internal partition */

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_NS_MAILBOX_AGENT */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_NS_AGENT_MAILBOX //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_PS */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_PROTECTED_STORAGE //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TFM_PARTITION_PROTECTED_STORAGE */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_ITS */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_AUDIT_LOG */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_AUDIT_LOG //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TFM_PARTITION_AUDIT_LOG */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_CRYPTO */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_CRYPTO //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TFM_PARTITION_CRYPTO */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_PLATFORM */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_PLATFORM //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TFM_PARTITION_PLATFORM */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_INITIAL_ATTESTATION */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_INITIAL_ATTESTATION //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_FWU */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_FIRMWARE_UPDATE //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_CORE_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TEST_NS_CORE //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TEST_NS_CORE */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_CORE_TEST_2 */
    /* -----------------------------------------------------------------------*/
#ifdef TEST_NS_CORE //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TEST_NS_CORE */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_SECURE_TEST_PARTITION */
    /* -----------------------------------------------------------------------*/
#ifdef TEST_FRAMEWORK_S //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TEST_FRAMEWORK_S */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_IPC_SERVICE_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_IPC_TEST //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_IPC_CLIENT_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TFM_PARTITION_IPC_TEST //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TFM_PARTITION_IPC_TEST */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_PS_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TEST_S_PS //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TEST_S_PS */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_SECURE_CLIENT_2 */
    /* -----------------------------------------------------------------------*/
#ifdef TEST_FRAMEWORK_S //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TEST_FRAMEWORK_S */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_SLIH_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TEST_NS_SLIH_IRQ //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TEST_NS_SLIH_IRQ */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_FLIH_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TEST_NS_FLIH_IRQ //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TEST_NS_FLIH_IRQ */ //NXP

    /* -----------------------------------------------------------------------*/
    /* - Partition DB record for TFM_SP_FPU_SERVICE_TEST */
    /* -----------------------------------------------------------------------*/
#ifdef TEST_PARTITION_FPU_TEST //NXP
    {
    /* Runtime data */
        .runtime_data             = {0},
        .static_data              = NULL,
        .platform_data_list       = NULL,
    },
#endif /* TEST_PARTITION_FPU_TEST */ //NXP

};

struct spm_partition_db_t g_spm_partition_db = {
    .is_init = 0,
    .partition_count = ARRAY_SIZE(partition_list),
    .running_partition_idx = 0,
    .partitions = partition_list,
};

#endif /* __TFM_SPM_DB_FUNC_INC__ */
