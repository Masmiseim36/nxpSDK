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
    PRINTF("EZHV core wake up by heartbeat vector event.\r\n");
}

#if (defined(DEMO_TEST_VETORED_EVENT) && DEMO_TEST_VETORED_EVENT == true)
/*! @brief Vector event */
void DEMO_HeartBeatVECase(void)
{
    uint32_t heartbeatCount = 1000U;

    EZHV_DeInitEmCfg();

    /* EZHV event manager */
    EZHV_EnableEmVectoredEvent(kEM_EventHeartBeat);

    /* Enable vectored event interrupt, set VIE=1 */
    EZHV_EnableEmVectoredInterrupt();

    /* Heartbeat enable */
    EZHV_WriteMiscReg(heartbeatCount);
    EZHV_EnableEmHeartBeat();

    /* Enable machine external interrupt */
    EnableMachineModeInt(kEZHV_Mie_Meie);

    EnableGlobalIRQ((uint32_t)kEZHV_Status_Mie);

    EZHV_PRINTF("EZHV heartbeat vector event example.\r\n");
    EZHV_PRINTF("Press any key to start the program.\r\n");
    GETCHAR();

    while (1)
    {
        asm volatile("wfi");
        SDK_DelayAtLeastUs(1000 * 1000, CLOCK_GetMediaMainClkFreq());
    }
}
#endif /* DEMO_TEST_VETORED_EVENT */

#if (defined(DEMO_TEST_NORMAL_EVENT) && DEMO_TEST_NORMAL_EVENT == true)
/*! @brief Normal event */
void DEMO_HeartBeatNECase(void)
{
    uint32_t heartbeatCount = 1000U;

    EZHV_DeInitEmCfg();

    /* EZHV event manager */
    EZHV_EnableEmNormalEvent(kEM_EventHeartBeat);

    EZHV_EnableEmWakeExitEn();
    EZHV_EnableEmEventAutoClear();

    /* Heartbeat enable */
    EZHV_WriteMiscReg(heartbeatCount);
    EZHV_EnableEmHeartBeat();

    PRINTF("EZHV heartbeat normal event example.\r\n");
    PRINTF("Press any key to start the program.\r\n");
    GETCHAR();

    while (1)
    {
        asm volatile("wfi");
        PRINTF("EZHV wake up by heartbeat normal event.\r\n");
        SDK_DelayAtLeastUs(1000 * 1000, CLOCK_GetMediaMainClkFreq());
    }
}
#endif /* DEMO_TEST_NORMAL_EVENT */

/*! @brief Main function */
int main(void)
{
    BOARD_InitHardware();

    /* SEMA42 init */
    SEMA42_Init(DEMO_SEMA42_INSTANCE);

    PRINTF("HeartBeat demo - message from ezhv core side.\r\n");

#if (defined(DEMO_TEST_VETORED_EVENT) && DEMO_TEST_VETORED_EVENT == true)
    DEMO_HeartBeatVECase();
#elif (defined(DEMO_TEST_NORMAL_EVENT) && DEMO_TEST_NORMAL_EVENT == true)
    DEMO_HeartBeatNECase();
#else
    PRINTF("Demo execute failed.\r\n");
#endif
}
