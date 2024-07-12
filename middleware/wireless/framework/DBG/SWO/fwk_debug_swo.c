/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include <stdint.h>
#include "fwk_debug_swo.h"
/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void DBG_InitSWO(int funnel)
{
    /* The trace logic is clock gated when debug/trace are disabled which will cause a stall on some register access
     * (LAR access below causes a stall fro instance) Set DEMCR.TRCENA to enable the debug/trace clock when a debugger
     * is not connected (we can still capture SWO via GPIO).
     */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* Write KEY value (0xC5ACCE55) to Lock Access Register to unlock the write access to ATB funnel by CPU. */
    (*(unsigned int *)0xE0044FB0U) = 0xC5ACCE55U;
    /* As documented in the RM, there is a FUNNEL between the application core ITM output
     * and the NBU (radio) ITM output before the SWO pin output.
     * The FUNNEL registers are located on the PPB bus at 0xE0044000.
     * Configure CTRL_REG FUNNEL register to enable port 0 (application core) or port 1 (NBU). Ports are controlled
     * via ENS0/ENS1 bits. */
    volatile unsigned int reg_val = *(unsigned int *)0xE0044000U;
    reg_val &= 0xFFFFFFFCU; /* first, clear ENS0 & ENS1 */
    reg_val |= (unsigned int)funnel;
    /* Chose funnel depending on which core you want the SWO to apply on */
    *(unsigned int *)0xE0044000U = reg_val;
}