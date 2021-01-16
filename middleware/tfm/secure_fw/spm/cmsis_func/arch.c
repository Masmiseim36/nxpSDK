/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_secure_api.h"
#include "tfm_internal.h"
#include "tfm/tfm_spm_services.h"

nsfptr_t ns_entry;

void jump_to_ns_code(void)
{
    /* Calls the non-secure Reset_Handler to jump to the non-secure binary */
    ns_entry();
}

__attribute__((naked))
int32_t tfm_core_get_caller_client_id(int32_t *caller_client_id)
{
    __ASM volatile(
        "SVC %0\n"
        "BX LR\n"
        : : "I" (TFM_SVC_GET_CALLER_CLIENT_ID));
}

__attribute__((naked))
int32_t tfm_spm_request(void)
{
    __ASM volatile(
        "SVC    %0\n"
        "BX     lr\n"
        : : "I" (TFM_SVC_SPM_REQUEST));
}

__attribute__((naked))
int32_t tfm_spm_request_reset_vote(void)
{
    __ASM volatile(
        "MOVS   R0, %0\n"
        "B      tfm_spm_request\n"
        : : "I" (TFM_SPM_REQUEST_RESET_VOTE));
}

__attribute__((naked))
void tfm_enable_irq(psa_signal_t irq_signal)
{
    __ASM("SVC %0\n"
          "BX LR\n"
          : : "I" (TFM_SVC_ENABLE_IRQ));
}

__attribute__((naked))
void tfm_disable_irq(psa_signal_t irq_signal)
{
    __ASM("SVC %0\n"
          "BX LR\n"
          : : "I" (TFM_SVC_DISABLE_IRQ));
}

__attribute__((naked))
static psa_signal_t psa_wait_internal(psa_signal_t signal_mask,
                                      uint32_t timeout)
{
    __ASM("SVC %0\n"
          "BX LR\n"
          : : "I" (TFM_SVC_PSA_WAIT));
}

psa_signal_t psa_wait(psa_signal_t signal_mask, uint32_t timeout)
{
    /* FIXME: By using the 'WFI' instruction this function blocks until an
     * interrupt happens. It is necessary to do this here as tfm_core_psa_wait
     * runs with the priority of the SVC, so it cannot be interrupted, so
     * waiting in it for the required interrupt to happen is not an option.
     */
    psa_signal_t actual_signal_mask;

    while (1) {
        actual_signal_mask = psa_wait_internal(signal_mask, timeout);
        if ((actual_signal_mask & signal_mask) != 0) {
            return actual_signal_mask;
        }
        __WFI();
    }
}

__attribute__((naked))
void psa_eoi(psa_signal_t irq_signal)
{
    __ASM("SVC %0\n"
          "BX LR\n"
          : : "I" (TFM_SVC_PSA_EOI));
}

#if defined(__ARM_ARCH_8_1M_MAIN__) || defined(__ARM_ARCH_8M_MAIN__)
__attribute__((section("SFN"), naked))
int32_t tfm_core_sfn_request(const struct tfm_sfn_req_s *desc_ptr)
{
    __ASM volatile(
        "PUSH   {r4-r12, lr}                \n"
        "SVC    %[SVC_REQ]                  \n"
        "MOV    r4,  #0                     \n"
        "MOV    r5,  r4                     \n"
        "MOV    r6,  r4                     \n"
        "MOV    r7,  r4                     \n"
        "MOV    r8,  r4                     \n"
        "MOV    r9,  r4                     \n"
        "MOV    r10, r4                     \n"
        "MOV    r11, r4                     \n"
        "BLX    lr                          \n"
        "SVC    %[SVC_RET]                  \n"
        "POP    {r4-r12, pc}                \n"
        : : [SVC_REQ] "I" (TFM_SVC_SFN_REQUEST),
            [SVC_RET] "I" (TFM_SVC_SFN_RETURN)
        );
}

__attribute__((section("SFN"), naked))
void priv_irq_handler_main(uint32_t partition_id, uint32_t unpriv_handler,
                           uint32_t irq_signal, uint32_t irq_line)
{
    __ASM(
          /* Save the callee saved registers*/
          "PUSH   {r4-r12, lr}              \n"
          /* Request SVC to configure environment for the unpriv IRQ handler */
          "SVC    %[SVC_REQ]                \n"
          /* clear the callee saved registers to prevent information leak */
          "MOV    r4,  #0                   \n"
          "MOV    r5,  r4                   \n"
          "MOV    r6,  r4                   \n"
          "MOV    r7,  r4                   \n"
          "MOV    r8,  r4                   \n"
          "MOV    r9,  r4                   \n"
          "MOV    r10, r4                   \n"
          "MOV    r11, r4                   \n"
          /* Branch to the unprivileged handler */
          "BLX    lr                        \n"
          /* Request SVC to reconfigure the environment of the interrupted
           * partition
           */
          "SVC    %[SVC_RET]                \n"
            /* restore callee saved registers and return */
          "POP    {r4-r12, pc}              \n"
          : : [SVC_REQ] "I" (TFM_SVC_DEPRIV_REQ)
            , [SVC_RET] "I" (TFM_SVC_DEPRIV_RET)
          );
}
#elif defined(__ARM_ARCH_8M_BASE__)
__attribute__((section("SFN"), naked))
int32_t tfm_core_sfn_request(const struct tfm_sfn_req_s *desc_ptr)
{
    __ASM volatile(
        "PUSH   {lr}                        \n"
        "PUSH   {r4-r7}                     \n"
        "MOV    r4, r8                      \n"
        "MOV    r5, r9                      \n"
        "MOV    r6, r10                     \n"
        "MOV    r7, r11                     \n"
        "PUSH   {r4-r7}                     \n"
        "MOV    r4, r12                     \n"
        "PUSH   {r4}                        \n"
        "SVC    %[SVC_REQ]                  \n"
        "MOVS   r4, #0                      \n"
        "MOV    r5, r4                      \n"
        "MOV    r6, r4                      \n"
        "MOV    r7, r4                      \n"
        "MOV    r8, r4                      \n"
        "MOV    r9, r4                      \n"
        "MOV    r10, r4                     \n"
        "MOV    r11, r4                     \n"
        "BLX    lr                          \n"
        "SVC    %[SVC_RET]                  \n"
        "POP    {r4}                        \n"
        "MOV    r12, r4                     \n"
        "POP    {r4-r7}                     \n"
        "MOV    r8, r4                      \n"
        "MOV    r9, r5                      \n"
        "MOV    r10, r6                     \n"
        "MOV    r11, r7                     \n"
        "POP    {r4-r7}                     \n"
        "POP    {pc}                        \n"
        : : [SVC_REQ] "I" (TFM_SVC_SFN_REQUEST),
            [SVC_RET] "I" (TFM_SVC_SFN_RETURN)
        );
}

__attribute__((section("SFN"), naked))
void priv_irq_handler_main(uint32_t partition_id, uint32_t unpriv_handler,
                           uint32_t irq_signal, uint32_t irq_line)
{
    __ASM(
          /* Save the callee saved registers*/
          "PUSH   {r4-r7, lr}               \n"
          "MOV    r4, r8                    \n"
          "MOV    r5, r9                    \n"
          "MOV    r6, r10                   \n"
          "MOV    r7, r11                   \n"
          "PUSH   {r4-r7}                   \n"
          "MOV    r4, r12                   \n"
          "PUSH   {r4}                      \n"
          /* Request SVC to configure environment for the unpriv IRQ handler */
          "SVC    %[SVC_REQ]                \n"
          /* clear the callee saved registers to prevent information leak */
          "MOVS   r4, #0                    \n"
          "MOV    r5, r4                    \n"
          "MOV    r6, r4                    \n"
          "MOV    r7, r4                    \n"
          "MOV    r8, r4                    \n"
          "MOV    r9, r4                    \n"
          "MOV    r10, r4                   \n"
          "MOV    r11, r4                   \n"
          /* Branch to the unprivileged handler */
          "BLX    lr                        \n"
          /* Request SVC to reconfigure the environment of the interrupted
           * partition
           */
          "SVC    %[SVC_RET]                \n"
          /* restore callee saved registers and return */
          "POP    {r4}                      \n"
          "MOV    r12, r4                   \n"
          "POP    {r4-r7}                   \n"
          "MOV    r8, r4                    \n"
          "MOV    r9, r5                    \n"
          "MOV    r10, r6                   \n"
          "MOV    r11, r7                   \n"
          "POP   {r4-r7, pc}                \n"
          : : [SVC_REQ] "I" (TFM_SVC_DEPRIV_REQ)
          , [SVC_RET] "I" (TFM_SVC_DEPRIV_RET)
          );
}
#endif

#if defined(__ARM_ARCH_8_1M_MAIN__) || \
    defined(__ARM_ARCH_8M_MAIN__)  || defined(__ARM_ARCH_8M_BASE__)
void tfm_arch_prioritize_secure_exception(void)
{
    uint32_t VECTKEY;
    SCB_Type *scb = SCB;
    uint32_t AIRCR;

    /* Set PRIS flag in AIRCR */
    AIRCR = scb->AIRCR;
    VECTKEY = (~AIRCR & SCB_AIRCR_VECTKEYSTAT_Msk);
    scb->AIRCR = SCB_AIRCR_PRIS_Msk |
                 VECTKEY |
                 (AIRCR & ~SCB_AIRCR_VECTKEY_Msk);
}
#elif defined(__ARM_ARCH_6M__) || defined(__ARM_ARCH_7M__) || \
      defined(__ARM_ARCH_7EM__)
void tfm_arch_prioritize_secure_exception(void)
{
}
#endif

#if defined(__ARM_ARCH_8_1M_MAIN__) || defined(__ARM_ARCH_8M_MAIN__)
__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile(
    "MRS     r2, MSP                        \n"
    /* Check store SP in thread mode to r0 */
    "TST     lr, #4                         \n"
    "ITE     EQ                             \n"
    "MOVEQ   r0, r2                         \n"
    "MRSNE   r0, PSP                        \n"
    "MOV     r1, lr                         \n"
    "BL      tfm_core_svc_handler           \n"
    "BX      r0                             \n"
    );
}
#elif defined(__ARM_ARCH_8M_BASE__)
__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile(
    "MRS     r2, MSP                        \n"
    "MOVS    r1, #4                         \n"
    "MOV     r3, lr                         \n"
    "MOV     r0, r2                         \n"
    "TST     r1, r3                         \n"
    "BEQ     handler                        \n"
    /* If SVC was made from thread mode, overwrite r0 with PSP */
    "MRS     r0, PSP                        \n"
    "handler:                               \n"
    "MOV     r1, lr                         \n"
    "BL      tfm_core_svc_handler           \n"
    "BX      r0                             \n"
    );
}
#elif defined(__ARM_ARCH_6M__) || defined(__ARM_ARCH_7M__) || \
      defined(__ARM_ARCH_7EM__)
__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile(
    "MOVS    r0, #4                \n" /* Check store SP in thread mode to r0 */
    "MOV     r1, lr                \n"
    "TST     r0, r1                \n"
    "BEQ     handler               \n"
    "MRS     r0, PSP               \n" /* Coming from thread mode */
    "B       sp_stored             \n"
    "handler:                      \n"
    "BX      lr                    \n" /* Coming from handler mode */
    "sp_stored:                    \n"
    "MOV     r1, lr                \n"
    "BL      tfm_core_svc_handler  \n"
    "BX      r0                    \n"
    );
}
#endif
