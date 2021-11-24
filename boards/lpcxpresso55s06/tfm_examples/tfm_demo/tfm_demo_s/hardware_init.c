/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();

    /* Flash operations (erase, blank check, program) and reading single word can only be performed for CPU frequencies
    of up to 100 MHz. Cannot be performed for frequencies above 100 MHz. */
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();
}

void SystemInit(void)
{
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));    /* set CP10, CP11 Full Access in Secure mode */
    SCB_NS->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10, CP11 Full Access in Non-secure mode */
#endif                                                    /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

    SCB->CPACR |= ((3UL << 0 * 2) | (3UL << 1 * 2));    /* set CP0, CP1 Full Access in Secure mode (enable PowerQuad) */
    SCB_NS->CPACR |= ((3UL << 0 * 2) | (3UL << 1 * 2)); /* set CP0, CP1 Full Access in Normal mode (enable PowerQuad) */

    SCB->NSACR |= ((3UL << 0) | (3UL << 10)); /* enable CP0, CP1, CP10, CP11 Non-secure Access */

    extern void *__Vectors;
    SCB->VTOR = (uint32_t)&__Vectors;

    SYSCON->TRACECLKDIV = 0;
/* Optionally enable RAM banks that may be off by default at reset */
#if !defined(DONT_ENABLE_DISABLED_RAMBANKS)
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL0_SRAM_CTRL1_MASK | SYSCON_AHBCLKCTRL0_SRAM_CTRL2_MASK;
#endif
    /* Following code is to reset PUF to remove over consumption */
    /* Enable PUF register clock to access register */
    SYSCON->AHBCLKCTRLSET[2] = SYSCON_AHBCLKCTRL2_PUF_MASK;
    /* Release PUF reset */
    SYSCON->PRESETCTRLCLR[2] = SYSCON_PRESETCTRL2_PUF_RST_MASK;
    /* Enable PUF SRAM */
    PUF_SRAM_CTRL->CFG |= PUF_SRAM_CTRL_CFG_ENABLE_MASK | PUF_SRAM_CTRL_CFG_CKGATING_MASK;
    /* Disable PUF register clock. */
    // Delaying the line of code below until the PUF State Machine execution is completed:
    // Shutting down the clock to early will prevent the state machine from reaching the end.
    // => Wait for status bit in PUF Controller Registers before stop PUF clock.
    while (!(PUF_SRAM_CTRL->INT_STATUS & PUF_SRAM_CTRL_INT_STATUS_READY_MASK))
        ;
    SYSCON->AHBCLKCTRLCLR[2] = SYSCON_AHBCLKCTRL2_PUF_MASK;
}

/*${function:end}*/
