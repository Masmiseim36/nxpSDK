/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_xbara.h"
#include "fsl_xbarb.h"
#include "board.h"
#include "peripherals.h"

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_XBARA_BASEADDR XBARA
#define DEMO_XBARB_BASEADDR XBARB

#define DEMO_XBARA_IRQ_HANDLER_FUNC XBARA_IRQHandler
#define DEMO_XBARA_IRQ_ID           XBARA_IRQn

#define DEMO_XBARB_INPUT_CMP_SIGNAL    kXBARB_InputCmp0Output
#define DEMO_XBARB_OUTPUT_AOI_SIGNAL_1 kXBARB_OutputAoiIn0

#define DEMO_XBARB_INPUT_PIT_SIGNAL    kXBARB_InputPitTrigger0
#define DEMO_XBARB_OUTPUT_AOI_SIGNAL_2 kXBARB_OutputAoiIn1

#define DEMO_XBARA_INPUT_AOI_SIGNAL kXBARA_InputAndOrInvert0
#define DEMO_XBARA_OUTPUT_SIGNAL    kXBARA_OutputDmamux18


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
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();
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
