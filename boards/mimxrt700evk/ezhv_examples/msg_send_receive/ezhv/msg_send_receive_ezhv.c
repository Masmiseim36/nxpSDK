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

#if (defined(DEMO_TEST_VETORED_EVENT) && DEMO_TEST_VETORED_EVENT == true)
/*! @brief Vector event, VIE=0, WXN=1, after wake-up via internal signal ezhv_exit_wait,
 *  the core will not execute the interrupt service routine, but continue to execute the
 *  rest of the code.
 */
static void DEMO_BlcVECase()
{
    uint32_t ret;

    EZHV_DeInitEmCfg();
    EZHV_EnableEmVectoredEvent(kEM_EventBLC0);

    EZHV_EnableBlc(kBLC_IDX_0, kBLC_CFG_RISING_EDGE, kBLC_SEL_INPUT_0);
    EZHV_EnableBlcEvent(kBLC_IDX_0);

    EZHV_EnableEmWakeExitEn();

    PRINTF("Vector event - press any key to let the ezhv core suspend execution.\r\n");
    GETCHAR();

    XZMSG_IndIntChan(1U);

    ret = XZMSG_Rcvrdi(1);
    ret = (((ret & EZHV_EM_STAT_MASK) >> EZHV_EM_STAT_OFFSET) >> EZHV_EM_STAT_OFFSET);

    if ((ret & (kEM_EventBLCOverAll | kEM_EventBLC0)) == ret)
    {
        PRINTF("EZHV core wake up by blc slice0 vector event.\r\n");
    }
    else
    {
        PRINTF("EZHV core wake up by some other events.\r\n"); /* This branch should never be hint. */
    }

    while (1);
}
#endif /* DEMO_TEST_VETORED_EVENT */

#if (defined(DEMO_TEST_NORMAL_EVENT) && DEMO_TEST_NORMAL_EVENT == true)
/*! @brief Normal event, WXN=0, the message receive interface is used. */
static void DEMO_BlcNECase()
{
    uint32_t ret;

    EZHV_DeInitEmCfg();
    EZHV_EnableEmNormalEvent(kEM_EventBLC0);

    EZHV_EnableBlc(kBLC_IDX_0, kBLC_CFG_RISING_EDGE, kBLC_SEL_INPUT_0);
    EZHV_EnableBlcEvent(kBLC_IDX_0);

    PRINTF("Normal event - press any key to let the ezhv core suspend execution.\r\n");
    GETCHAR();

    XZMSG_IndIntChan(1U);

    ret = XZMSG_Rcvrdi(0);
    ret = (((ret & EZHV_EM_STAT_MASK) >> EZHV_EM_STAT_OFFSET) >> EZHV_EM_STAT_OFFSET);

    if ((ret & (kEM_EventBLCOverAll | kEM_EventBLC0)) == ret)
    {
        PRINTF("EZHV core wake up by blc slice0 normal event.\r\n");
    }
    else
    {
        PRINTF("EZHV core wake up by some other events.\r\n"); /* This branch should never be hint. */
    }

    while (1);
}
#endif /* DEMO_TEST_NORMAL_EVENT */

/*! @brief Main function */
int main(void)
{
    BOARD_InitHardware();

    SEMA42_Init(DEMO_SEMA42_INSTANCE);

    PRINTF("Message send receive demo - message from ezhv core side.\r\n");

#if (defined(DEMO_TEST_VETORED_EVENT) && DEMO_TEST_VETORED_EVENT == true)
    DEMO_BlcVECase();
#elif (defined(DEMO_TEST_NORMAL_EVENT) && DEMO_TEST_NORMAL_EVENT == true)
    DEMO_BlcNECase();
#else
    PRINTF("Demo execute failed.\r\n");
#endif
}
