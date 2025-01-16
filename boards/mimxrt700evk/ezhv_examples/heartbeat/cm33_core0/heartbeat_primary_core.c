/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdarg.h>
#include "fsl_sema42.h"
#include "pin_mux.h"
#include "app.h"
#include "board.h"
#include "ezhv_support.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef PRINTF
#undef PRINTF
#define PRINTF CM33_PRINTF
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void CM33_PRINTF(const char *ptr, ...)
{
    va_list ap;
    SEMA42_Lock(DEMO_SEMA42_INSTANCE, DEMO_SEMA42_PRINTF_GATE, DEMO_SEMA42_CORE_ID_CM33);
    va_start(ap, ptr);
    DbgConsole_Vprintf(ptr, ap);
    va_end(ap);
    SEMA42_Unlock(DEMO_SEMA42_INSTANCE, DEMO_SEMA42_PRINTF_GATE);
}

int main(void)
{
    BOARD_InitHardware();

    /* SEMA42 init */
    SEMA42_Init(DEMO_SEMA42_INSTANCE);
    /* Reset the sema42 gate */
    SEMA42_ResetAllGates(DEMO_SEMA42_INSTANCE);

    PRINTF("HeartBeat demo - message from primary core side.\r\n");

    /* Boot EZHV */
    BOARD_EZHV_Init();

    while (1)
    {
    }
}
