/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_xbara.h"
#include "fsl_xbarb.h"
#include "app.h"
#include "peripherals.h"

#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Initialize the XBAR module.
 *
 */
static void XBAR_Configuration(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_xbaraInterrupt = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

static void XBAR_Configuration(void)
{
    xbara_control_config_t xbaraConfig;

    /* Init XBAR module. */
    XBARA_Init(DEMO_XBARA_BASEADDR);
    XBARB_Init(DEMO_XBARB_BASEADDR);

    /* Configure the XBAR signal connections */
    XBARB_SetSignalsConnection(DEMO_XBARB_BASEADDR, DEMO_XBARB_INPUT_CMP_SIGNAL, DEMO_XBARB_OUTPUT_AOI_SIGNAL_1);
    XBARB_SetSignalsConnection(DEMO_XBARB_BASEADDR, DEMO_XBARB_INPUT_PIT_SIGNAL, DEMO_XBARB_OUTPUT_AOI_SIGNAL_2);
    XBARA_SetSignalsConnection(DEMO_XBARA_BASEADDR, DEMO_XBARA_INPUT_AOI_SIGNAL, DEMO_XBARA_OUTPUT_SIGNAL);

    /* Configure the XBARA interrupt */
    xbaraConfig.activeEdge  = kXBARA_EdgeRising;
    xbaraConfig.requestType = kXBARA_RequestInterruptEnalbe;
    XBARA_SetOutputSignalConfig(DEMO_XBARA_BASEADDR, DEMO_XBARA_OUTPUT_SIGNAL, &xbaraConfig);

    /* Enable at the NVIC. */
    EnableIRQ(DEMO_XBARA_IRQ_ID);
}

void DEMO_XBARA_IRQ_HANDLER_FUNC(void)
{
    /* Clear interrupt flag */
    XBARA_ClearStatusFlags(DEMO_XBARA_BASEADDR, kXBARA_EdgeDetectionOut0);
    g_xbaraInterrupt = true;
    SDK_ISR_EXIT_BARRIER;
}

int main(void)
{
    /* Init board hardware */
    BOARD_InitHardware();
    /* Init XBAR */
    XBAR_Configuration();

    PRINTF("XBAR and AOI Peripheral Demo: Start...\r\n");

    while (1)
    {
        if (g_xbaraInterrupt)
        {
            g_xbaraInterrupt = false;
            PRINTF("XBAR interrupt occurred\r\n\r\n");
        }
    }
}
