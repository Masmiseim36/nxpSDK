/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdarg.h>
#include "fsl_sema42.h"
#include "app.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef PRINTF
#undef PRINTF
#define PRINTF EZHV_PRINTF
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void EZHV_PRINTF(const char *ptr, ...)
{
    va_list ap;
    SEMA42_Lock(DEMO_SEMA42_INSTANCE, DEMO_SEMA42_PRINTF_GATE, DEMO_SEMA42_CORE_ID_EZHV);
    va_start(ap, ptr);
    DbgConsole_Vprintf(ptr, ap);
    va_end(ap);
    SEMA42_Unlock(DEMO_SEMA42_INSTANCE, DEMO_SEMA42_PRINTF_GATE);
}

void MachineExtInt_Handler(void)
{
    PRINTF("Ezhv received arm core mei request.\r\n");
    return;
}

/*! @brief Main function */
int main(void)
{
    BOARD_InitHardware();

    /* SEMA42 init */
    SEMA42_Init(DEMO_SEMA42_INSTANCE);

    PRINTF("Arm to ezhv interrupt request demo - message from ezhv core side.\r\n");

    /* Enable global interrupts. */
    EnableGlobalIRQ((uint32_t)kEZHV_Status_Mie);
    /* Enable machine external interrupt. */
    EnableMachineModeInt(kEZHV_Mie_Meie);

    PRINTF("Press any key to let the ezhv core enter wait state.\r\n");
    GETCHAR();
    asm volatile("wfi");
    PRINTF("Ezhv exit wait state.\r\n");

    while (1)
        ;
}
