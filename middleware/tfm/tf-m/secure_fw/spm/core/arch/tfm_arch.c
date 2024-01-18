/*
 * Copyright (c) 2018-2023, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "compiler_ext_defs.h"
#include "security_defs.h"
#include "tfm_arch.h"
#include "tfm_core_trustzone.h"
#include "utilities.h"
#include "config_impl.h"

#if defined(__ICCARM__) && (CONFIG_TFM_FLOAT_ABI >= 1)
#pragma required = tfm_arch_clear_fp_data
#endif

__naked void tfm_arch_free_msp_and_exc_ret(uint32_t msp_base,
                                           uint32_t exc_return)
{
    __ASM volatile(
        SYNTAX_UNIFIED
        "mov     r4, r0                         \n"
        "mov     r5, r1                         \n"
#if (CONFIG_TFM_FLOAT_ABI > 0)
        "bl      tfm_arch_clear_fp_data         \n"
#endif
        "mov     sp, r4                         \n"
        "bx      r5                             \n"
    );
}
#if CONFIG_TFM_SPM_BACKEND_IPC == 1

extern uint32_t scheduler_lock;

void tfm_arch_set_context_ret_code(void *p_ctx_ctrl, uint32_t ret_code)
{
    struct context_ctrl_t *ctx_ctrl = (struct context_ctrl_t *)p_ctx_ctrl;

    /* Write the return value to the state context on stack. */
    ((struct full_context_t *)ctx_ctrl->sp)->stat_ctx.r0 = ret_code;
}

__naked void arch_acquire_sched_lock(void)
{
    __asm volatile(
        SYNTAX_UNIFIED
        "   ldr    r0, =scheduler_lock                 \n"
        "   movs   r1, #"M2S(SCHEDULER_LOCKED)"        \n"
        "   str    r1, [r0, #0]                        \n"
        "   dsb    #0xf                                \n"
        "   bx     lr                                  \n"
    );
}

__naked uint32_t arch_release_sched_lock(void)
{
    __asm volatile(
        SYNTAX_UNIFIED
        "ldr    r1, =scheduler_lock                    \n"
        "ldr    r0, [r1, #0]                           \n"
        "movs   r2, #"M2S(SCHEDULER_UNLOCKED)"         \n"/* Unlock scheduler */
        "str    r2, [r1, #0]                           \n"
        "dsb    #0xf                                   \n"
        "bx     lr                                     \n"
    );
}

/* Caution: Keep 'uint32_t' always for collecting thread return values! */
__naked uint32_t tfm_arch_trigger_pendsv(void)
{
    __ASM volatile(
        SYNTAX_UNIFIED
        "ldr     r0, =scheduler_lock                   \n"
        "ldr     r2, [r0, #0]                          \n"
        "cmp     r2, #"M2S(SCHEDULER_UNLOCKED)"        \n"
        /* Skip PendSV if scheduler is locked and mark scheduling attempted. */
        "bne     mark_schedule_attempted_and_exit      \n"
        "ldr     r0, ="M2S(SCB_ICSR_ADDR)"             \n"
        "ldr     r1, ="M2S(SCB_ICSR_PENDSVSET_BIT)"    \n"
        "str     r1, [r0, #0]                          \n"
        "dsb     #0xf                                  \n"
        "isb                                           \n"
        "bx      lr                                    \n"
    "mark_schedule_attempted_and_exit:                 \n"
        "movs    r2, #"M2S(SCHEDULER_ATTEMPTED)"       \n"
        "str     r2, [r0, #0]                          \n"
        "dsb     #0xf                                  \n"
        "bx      lr                                    \n"
    );
}
#endif

void tfm_arch_init_context(void *p_ctx_ctrl,
                           uintptr_t pfn, void *param, uintptr_t pfnlr)
{
    uintptr_t sp = ((struct context_ctrl_t *)p_ctx_ctrl)->sp;
    uintptr_t sp_limit = ((struct context_ctrl_t *)p_ctx_ctrl)->sp_limit;
    struct full_context_t *p_tctx =
            (struct full_context_t *)arch_seal_thread_stack(sp);

    /* Check if enough space on stack */
    if ((uintptr_t)p_tctx - sizeof(struct full_context_t) < sp_limit) {
        tfm_core_panic();
    }

    p_tctx--;

    spm_memset(p_tctx, 0, sizeof(*p_tctx));

    ARCH_CTXCTRL_EXCRET_PATTERN(&p_tctx->stat_ctx, param, pfn, pfnlr);

    ((struct context_ctrl_t *)p_ctx_ctrl)->exc_ret  = EXC_RETURN_THREAD_PSP;
    ((struct context_ctrl_t *)p_ctx_ctrl)->sp       = (uintptr_t)p_tctx;
}

uint32_t tfm_arch_refresh_hardware_context(void *p_ctx_ctrl)
{
    struct context_ctrl_t *ctx_ctrl;
    struct tfm_state_context_t *sc;

    ctx_ctrl  = (struct context_ctrl_t *)p_ctx_ctrl;
    sc = &(((struct full_context_t *)(ctx_ctrl->sp))->stat_ctx);

    tfm_arch_set_psplim(ctx_ctrl->sp_limit);
    __set_PSP((uintptr_t)sc);

    return ctx_ctrl->exc_ret;
}
