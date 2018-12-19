/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_flexram.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_FLEXRAM FLEXRAM
#define APP_FLEXRAM_IRQ FLEXRAM_IRQn
#define APP_FLEXRAM_OCRAM_START_ADDR 0x20200000
#define APP_FLEXRAM_OCRAM_MAGIC_ADDR 0x202000a8
#define APP_FLEXRAM_IRQ_HANDLER FLEXRAM_IRQHandler
#define APP_DSB() __DSB()
#define APP_ISB() __ISB()


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
* @brief ocram access function.
*
*/
static void OCRAM_Access(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bool s_flexram_ocram_access_error_match = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_FLEXRAM_IRQ_HANDLER(void)
{
    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_OCRAMAccessError)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_OCRAMAccessError);
        s_flexram_ocram_access_error_match = true;
    }

/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin, clock, debug console init */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    SCB_DisableDCache();
#endif

    PRINTF("\r\nFLEXRAM ram access driver example.\r\n");

    /* enable IRQ */
    EnableIRQ(APP_FLEXRAM_IRQ);
    /* init flexram */
    FLEXRAM_Init(APP_FLEXRAM);
    /* access the default ocram */
    OCRAM_Access();

    PRINTF("\r\nFLEXRAM ram access driver example finish.\r\n");

    while (1)
    {
    }
}

static void OCRAM_Access(void)
{
    uint8_t *ocramAddr = (uint8_t *)APP_FLEXRAM_OCRAM_START_ADDR;

    /* enable FLEXRAM OCRAM access error interrupt and OCRAM magic address match interrupt */
    FLEXRAM_EnableInterruptSignal(APP_FLEXRAM, kFLEXRAM_OCRAMAccessError);

    for (;;)
    {
        *ocramAddr = 0xCC;
        /* Synchronizes the execution stream with memory accesses */
        APP_DSB();
        APP_ISB();

        /* check ocram access error event */
        if (s_flexram_ocram_access_error_match)
        {
            PRINTF("\r\nOCRAM access 0x%x to boundary.\r\n", (uint32_t)(ocramAddr));
            s_flexram_ocram_access_error_match = false;
            break;
        }
        ocramAddr++;
    }
}
