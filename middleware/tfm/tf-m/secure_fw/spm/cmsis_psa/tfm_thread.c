/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include "tfm_arch.h"
#include "tfm_thread.h"
#include "utilities.h"
#include "svc_num.h"
#include "tfm_memory_utils.h"
#include "tfm_core_utils.h"

/* Force ZERO in case ZI(bss) clear is missing */
static struct tfm_core_thread_t *p_thrd_head = NULL; /* Head of all threads */
static struct tfm_core_thread_t *p_rnbl_head = NULL; /* Head of runnable */
static struct tfm_core_thread_t *p_curr_thrd = NULL; /* Current running */

/* Define Macro to fetch global to support future expansion (PERCPU e.g.) */
#define LIST_HEAD   p_thrd_head
#define RNBL_HEAD   p_rnbl_head
#define CURR_THRD   p_curr_thrd

/* Get next thread to run for scheduler */
struct tfm_core_thread_t *tfm_core_thrd_get_next(void)
{
    struct tfm_core_thread_t *pth = RNBL_HEAD;

    /*
     * First runnable thread has highest priority since threads are sorted with
     * priority.
     */
    while (pth && pth->state != THRD_STATE_RUNNABLE) {
        pth = pth->next;
    }

    return pth;
}

/* To get current running thread for caller */
struct tfm_core_thread_t *tfm_core_thrd_get_curr(void)
{
    return CURR_THRD;
}

void tfm_core_thrd_set_curr(struct tfm_core_thread_t *pth)
{
    if (!pth) {
        tfm_core_panic();
    }

    CURR_THRD = pth;
}

/* Insert a new thread into list by descending priority (Highest at head) */
static void insert_by_prior(struct tfm_core_thread_t **head,
                            struct tfm_core_thread_t *node)
{
    if (*head == NULL || (node->prior <= (*head)->prior)) {
        node->next = *head;
        *head = node;
    } else {
        struct tfm_core_thread_t *iter = *head;

        while (iter->next && (node->prior > iter->next->prior)) {
            iter = iter->next;
        }
        node->next = iter->next;
        iter->next = node;
    }
}

/* Set context members only. No validation here */
void tfm_core_thrd_init(struct tfm_core_thread_t *pth,
                        tfm_core_thrd_entry_t pfn, void *param,
                        uintptr_t stk_top, uintptr_t stk_btm)
{
    pth->prior = THRD_PRIOR_MEDIUM;
    pth->state = THRD_STATE_CREATING;
    pth->pfn = pfn;
    pth->param = param;
    pth->stk_btm = stk_btm;
    pth->stk_top = stk_top;
    pth->flih_ctx = stk_top;
}

uint32_t tfm_core_thrd_start(struct tfm_core_thread_t *pth)
{
    /* Validate parameters before really start */
    if ((pth->state != THRD_STATE_CREATING) ||
        (pth->pfn == NULL)                  ||
        (pth->stk_btm == 0)                 ||
        (pth->stk_top == 0)) {
        return THRD_ERR_INVALID_PARAM;
    }

    /* Thread management runs in handler mode; set context for thread mode. */
    tfm_arch_init_context(&pth->arch_ctx, pth->param, (uintptr_t)pth->pfn,
                          pth->stk_btm, pth->stk_top);

    /* Insert a new thread with priority */
    insert_by_prior(&LIST_HEAD, pth);

    /* Mark it as RUNNABLE after insertion */
    tfm_core_thrd_set_state(pth, THRD_STATE_RUNNABLE);

    return THRD_SUCCESS;
}

void tfm_core_thrd_set_state(struct tfm_core_thread_t *pth, uint32_t new_state)
{
    TFM_CORE_ASSERT(pth != NULL && new_state < THRD_STATE_INVALID);

    pth->state = new_state;

    /*
     * Set first runnable thread as head to reduce enumerate
     * depth while searching for a first runnable thread.
     */
    if ((pth->state == THRD_STATE_RUNNABLE) &&
        ((RNBL_HEAD == NULL) || (pth->prior < RNBL_HEAD->prior))) {
        RNBL_HEAD = pth;
    } else {
        RNBL_HEAD = LIST_HEAD;
    }
}

/* Scheduling won't happen immediately but after the exception returns */
void tfm_core_thrd_activate_schedule(void)
{
    tfm_arch_trigger_pendsv();
}

void tfm_core_thrd_start_scheduler(struct tfm_core_thread_t *pth)
{
    /*
     * There is no selected thread before scheduler start, assign the caller
     * provided thread as the current thread. Update the hardware PSP/PSPLIM
     * with the value in thread context to ensure they are identical.
     * This function can be called only ONCE; further calling triggers assert.
     */
    TFM_CORE_ASSERT(CURR_THRD == NULL);
    TFM_CORE_ASSERT(pth != NULL);
    TFM_CORE_ASSERT(pth->arch_ctx.sp != 0);

    tfm_arch_set_ctx(&pth->arch_ctx);

    CURR_THRD = pth;

    tfm_core_thrd_activate_schedule();
}

void tfm_core_thrd_switch_context(struct tfm_arch_ctx_t *p_actx,
                                  struct tfm_core_thread_t *prev,
                                  struct tfm_core_thread_t *next)
{
    TFM_CORE_ASSERT(prev != NULL);
    TFM_CORE_ASSERT(next != NULL);

    /*
     * First, update latest context into the current thread context.
     * Then, update background context with next thread's context.
     */
    spm_memcpy(&prev->arch_ctx, p_actx, sizeof(*p_actx));
    spm_memcpy(p_actx, &next->arch_ctx, sizeof(next->arch_ctx));

    prev->flih_ctx = prev->arch_ctx.sp;

    /* Update current thread indicator */
    CURR_THRD = next;
}
