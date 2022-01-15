/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "svc_num.h"
#include "tfm_arch.h"
#include "tfm_core_utils.h"


static void tfm_arch_init_state_ctx(struct tfm_state_context_t *p_stat_ctx,
                                    void *param, uintptr_t pfn)
{
    p_stat_ctx->r0 = (uint32_t)param;
    p_stat_ctx->ra = (uint32_t)pfn;
    /*
     * Prevent thread exits:
     * Only T32 is supported, so bit[0] must be 1. Clear the bit[0] of LR to
     * trigger a fault.
     */
    p_stat_ctx->lr = ((uint32_t)pfn) & (~1UL);
    p_stat_ctx->xpsr = XPSR_T32;
}

void tfm_arch_init_context(struct tfm_arch_ctx_t *p_actx,
                           void *param, uintptr_t pfn,
                           uintptr_t stk_btm, uintptr_t stk_top)
{
    struct tfm_state_context_t *p_stat_ctx =
            (struct tfm_state_context_t *)tfm_arch_seal_thread_stack(stk_top);

    /*
     * Shift back SP to leave space for holding common state context
     * since thread is kicked off through exception return.
     */
    p_stat_ctx--;

    /* First the common state context - ZERO it before usage. */
    spm_memset(p_stat_ctx, 0, sizeof(*p_stat_ctx));
    tfm_arch_init_state_ctx(p_stat_ctx, param, pfn);

    /* Then the architecture-specific context. */
    spm_memset(p_actx, 0, sizeof(*p_actx));
    tfm_arch_init_actx(p_actx, (uint32_t)p_stat_ctx, (uint32_t)stk_btm);
}
