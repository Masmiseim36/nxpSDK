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

void EventManagerVector0_Handler(void)
{
    PRINTF("EZHV core wake up by blc slice0 vector event.\r\n");
}

#if (defined(DEMO_TEST_VETORED_EVENT) && DEMO_TEST_VETORED_EVENT == true)
/*! @brief Vector event */
static void DEMO_BlcVECase()
{
    EZHV_DeInitEmCfg();
    EZHV_EnableEmVectoredEvent(kEM_EventBLC0);
    EZHV_EnableEmVectoredInterrupt();

    EZHV_EnableBlc(kBLC_IDX_0, kBLC_CFG_RISING_EDGE, kBLC_SEL_INPUT_0);
    EZHV_EnableBlcEvent(kBLC_IDX_0);

    EnableMachineModeInt(kEZHV_Mie_Meie);
    EnableGlobalIRQ((uint32_t)kEZHV_Status_Mie);

    PRINTF("Vector event - Press any key to let the ezhv core enter wait state.\r\n");
    GETCHAR();
    asm volatile("wfi");

    while(1);
}
#endif /* DEMO_TEST_VETORED_EVENT */

#if (defined(DEMO_TEST_NORMAL_EVENT) && DEMO_TEST_NORMAL_EVENT == true)
/*! @brief Normal event */
static void DEMO_BlcNECase()
{
    EZHV_DeInitEmCfg();
    EZHV_EnableEmNormalEvent(kEM_EventBLC0);
    EZHV_EnableEmWakeExitEn();
    EZHV_EnableEmEventAutoClear();

    EZHV_EnableBlc(kBLC_IDX_0, kBLC_CFG_RISING_EDGE, kBLC_SEL_INPUT_0);
    EZHV_EnableBlcEvent(kBLC_IDX_0);

    PRINTF("Normal event - press any key to let the ezhv core enter wait state.\r\n");
    GETCHAR();
    asm volatile("wfi");

    PRINTF("EZHV core wake up by blc slice0 normal event.\r\n");
    while (1);
}
#endif /* DEMO_TEST_NORMAL_EVENT */

/*! @brief Main function */
int main(void)
{
    BOARD_InitHardware();

    SEMA42_Init(DEMO_SEMA42_INSTANCE);

    PRINTF("BLC detect demo - message from ezhv core side.\r\n");

#if (defined(DEMO_TEST_VETORED_EVENT) && DEMO_TEST_VETORED_EVENT == true)
    DEMO_BlcVECase();
#elif (defined(DEMO_TEST_NORMAL_EVENT) && DEMO_TEST_NORMAL_EVENT == true)
    DEMO_BlcNECase();
#else
    PRINTF("Demo execute failed.\r\n");
#endif
}
