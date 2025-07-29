//*****************************************************************************
// boot_multicore_slave.c
//
// Provides simple functions to boot slave core in rt700 multicore system
//
//*****************************************************************************
//
// Copyright 2024,2025 NXP
// All rights reserved.
//
// SPDX-License-Identifier: BSD-3-Clause
//*****************************************************************************

#if defined(__MULTICORE_MASTER)

#include <stdint.h>
#include "fsl_glikey.h"

extern uint8_t __core_m33slave_START__;

void boot_multicore_slave(void)
{
    /* CPU0 to CPU1 communication case */
    /*Glikey write enable, GLIKEY4*/
    (void)GLIKEY_SyncReset(GLIKEY4);

    (void)GLIKEY_StartEnable(GLIKEY4, 1U);
    (void)GLIKEY_ContinueEnable(GLIKEY4, GLIKEY_CODEWORD_STEP1);
    (void)GLIKEY_ContinueEnable(GLIKEY4, GLIKEY_CODEWORD_STEP2);
    (void)GLIKEY_ContinueEnable(GLIKEY4, GLIKEY_CODEWORD_STEP3);
    (void)GLIKEY_ContinueEnable(GLIKEY4, GLIKEY_CODEWORD_STEP_EN);

    /* Boot source for Core 1 from RAM. */
    SYSCON3->CPU1_NSVTOR = ((uint32_t)(char *)((unsigned int *)&__core_m33slave_START__) >> 7U);
    SYSCON3->CPU1_SVTOR  = ((uint32_t)(char *)((unsigned int *)&__core_m33slave_START__) >> 7U);
    
    (void)GLIKEY_SyncReset(GLIKEY4);

    /* Enable cpu1 clock. */
    CLOCK_EnableClock(kCLOCK_Cpu1);

    /* Clear reset*/
    RESET_ClearPeripheralReset(kCPU1_RST_SHIFT_RSTn);

    /* Release cpu wait*/
    SYSCON3->CPU_STATUS &= ~SYSCON3_CPU_STATUS_CPU_WAIT_MASK;
    
}
#endif // defined (__MULTICORE_MASTER)
