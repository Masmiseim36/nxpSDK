/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_smartcard_emvsim.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

extern smartcard_context_t *g_smartcardContext;

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

/*! @brief IRQ handler for emvsim */
void EMVSIM1_IRQHandler(void)
{
    SMARTCARD_EMVSIM_IRQHandler(BOARD_SMARTCARD_MODULE, g_smartcardContext);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
/*${function:end}*/
