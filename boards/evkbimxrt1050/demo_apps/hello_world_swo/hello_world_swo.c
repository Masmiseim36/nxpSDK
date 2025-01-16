/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_userPress          = false;
volatile bool g_timeOut            = false;
volatile uint32_t g_systickCounter = 1000U;
/*******************************************************************************
 * Code
 ******************************************************************************/
void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
    else
    {
        g_systickCounter = 1000U;
        g_timeOut        = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();
    BOARD_InitDebugConsoleSWO(DEMO_DEBUG_CONSOLE_SWO_PORT, DEMO_DEBUG_CONSOLE_SWO_BAUDRATE);

    /* Set systick reload value to generate 1ms interrupt */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }

    while (1)
    {
        if (g_userPress)
        {
            PRINTF("SWO: hello_world\r\n");
            g_userPress = false;
        }
        if (g_timeOut)
        {
            PRINTF("SWO: timer_trigger\r\n");
            g_timeOut = false;
        }
    }
}
