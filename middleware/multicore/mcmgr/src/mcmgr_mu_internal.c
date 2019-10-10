/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_mu.h"
#include "mcmgr.h"
#include "mcmgr_internal_core_api.h"

#define MU_ISR_FLAG_BASE (20)
#define MU_ISR_COUNT (12)

/* Weak MU ISR stubs */
/* implement these in your application to override */

void MU_Tx3EmptyFlagISR(void);
void MU_Tx2EmptyFlagISR(void);
void MU_Tx1EmptyFlagISR(void);
void MU_Tx0EmptyFlagISR(void);
void MU_Rx3FullFlagISR(void);
void MU_Rx2FullFlagISR(void);
void MU_Rx1FullFlagISR(void);
void MU_Rx0FullFlagISR(void);
void MU_GenInt3FlagISR(void);
void MU_GenInt2FlagISR(void);
void MU_GenInt1FlagISR(void);
void MU_GenInt0FlagISR(void);

void MU_UnusedISR(void)
{
}

#pragma weak MU_Tx3EmptyFlagISR = MU_UnusedISR
#pragma weak MU_Tx2EmptyFlagISR = MU_UnusedISR
#pragma weak MU_Tx1EmptyFlagISR = MU_UnusedISR
#pragma weak MU_Tx0EmptyFlagISR = MU_UnusedISR
#pragma weak MU_Rx3FullFlagISR = MU_UnusedISR
#pragma weak MU_Rx2FullFlagISR = MU_UnusedISR
#pragma weak MU_Rx1FullFlagISR = MU_UnusedISR
#pragma weak MU_Rx0FullFlagISR = MU_UnusedISR
#pragma weak MU_GenInt3FlagISR = MU_UnusedISR
#pragma weak MU_GenInt2FlagISR = MU_UnusedISR
#pragma weak MU_GenInt1FlagISR = MU_UnusedISR
#pragma weak MU_GenInt0FlagISR = MU_UnusedISR

/* MU ISR table */
void (*const MU_interrupts[MU_ISR_COUNT])(void) = {
    MU_Tx3EmptyFlagISR, MU_Tx2EmptyFlagISR, MU_Tx1EmptyFlagISR, MU_Tx0EmptyFlagISR,
    MU_Rx3FullFlagISR,  MU_Rx2FullFlagISR,  MU_Rx1FullFlagISR,  MU_Rx0FullFlagISR,
    MU_GenInt3FlagISR,  MU_GenInt2FlagISR,  MU_GenInt1FlagISR,  MU_GenInt0FlagISR,
};

/* MU ISR router */
static void mu_isr(MU_Type *base)
{
    uint32_t flags;
    int i;
    flags = MU_GetStatusFlags(base);

#if (defined(FSL_FEATURE_MU_HAS_RESET_INT) && FSL_FEATURE_MU_HAS_RESET_INT)
    /* The other core reset assert interrupt pending */
    if(flags & kMU_ResetAssertInterruptFlag)
    {
        MU_ClearStatusFlags(base, kMU_ResetAssertInterruptFlag);
        if (MCMGR_eventTable[kMCMGR_RemoteCoreDownEvent].callback != NULL)
        {
            MCMGR_eventTable[kMCMGR_RemoteCoreDownEvent].callback(0, MCMGR_eventTable[kMCMGR_RemoteCoreDownEvent].callbackData);
        }
        return;
    }
#endif

    for (i = MU_ISR_FLAG_BASE; i < (MU_ISR_FLAG_BASE + MU_ISR_COUNT); i++)
    {
        if (flags & (1 << i))
        {
            MU_ClearStatusFlags(base, (1 << i));
            MU_interrupts[i - MU_ISR_FLAG_BASE]();
        }
    }
}

#if defined(FSL_FEATURE_MU_SIDE_A)
int MUA_IRQHandler()
{
    mu_isr(MUA);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
    return 0;
}
#elif defined(FSL_FEATURE_MU_SIDE_B)
int MUB_IRQHandler()
{
    mu_isr(MUB);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
    return 0;
}
#endif
