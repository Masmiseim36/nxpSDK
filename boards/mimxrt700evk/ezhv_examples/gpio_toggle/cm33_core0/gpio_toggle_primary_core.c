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
#include "fsl_ezhv.h"

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

static void EZHV_Callback(void *param)
{
    uint32_t tmp = EZHV_GetEzhv2ArmIntChan();

    if (tmp)
    {
        EZHV_ClearEzhv2ArmIntChan((ezhv2arm_int_chan_t)tmp);
        PRINTF("Ezhv irq channel = 0x%x\r\n", tmp);
    }
}

int main(void)
{
    BOARD_InitHardware();

    /* SEMA42 init */
    SEMA42_Init(DEMO_SEMA42_INSTANCE);
    /* Reset the sema42 gate */
    SEMA42_ResetAllGates(DEMO_SEMA42_INSTANCE);

    EZHV_EnableEzhv2ArmIntChan(kEZHV_EzhvToArmIntChan0);
    PRINTF("Gpio toggle demo - message from primary core side.\r\n");
    /* Boot EZHV */
    BOARD_EZHV_Init();

    /* enable ezhv interrupt */
    EnableIRQ(EZHV_IRQn);
    EZHV_SetCallback(EZHV_Callback, 0U, NULL);

    while (1)
    {
    }
}
