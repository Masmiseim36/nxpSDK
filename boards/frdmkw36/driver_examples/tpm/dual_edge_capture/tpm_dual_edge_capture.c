/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_tpm.h"

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include <stdbool.h>
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_TPM_BASEADDR TPM1
#define DEMO_TPM_BASEADDR BOARD_TPM_BASEADDR

/* TPM channel pair used for the dual-edge capture, channel pair 0 uses channels 1 and 2 */
#define BOARD_TPM_INPUT_CAPTURE_CHANNEL_PAIR kTPM_Chnl_0

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM1_IRQn
#define TPM_INPUT_CAPTURE_HANDLER TPM1_IRQHandler

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl1InterruptEnable
#define TPM_CHANNEL_FLAG kTPM_Chnl1Flag

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgFllClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool tpmIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void TPM_INPUT_CAPTURE_HANDLER(void)
{
    tpmIsrFlag = true;

    /* Clear interrupt flag.*/
    TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, TPM_CHANNEL_FLAG);
}

/*!
 * @brief Main function
 */
int main(void)
{
    tpm_config_t tpmInfo;
    tpm_dual_edge_capture_param_t edgeParam;
    uint32_t capture1Val = 0;
    uint32_t capture2Val = 0;
    uint32_t pulseWidth = 0;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Select the clock source for the TPM counter as MCGPLLCLK */
    CLOCK_SetTpmClock(1U);

    /* Print a note to terminal */
    PRINTF("\r\nTPM dual-edge capture example\r\n");
    PRINTF("\r\nOnce the input signal is received the input capture values are printed");
    PRINTF("\r\nThe input signal's pulse width is calculated from the capture values & printed\r\n");

    TPM_GetDefaultConfig(&tpmInfo);
    /* Initialize TPM module */
    TPM_Init(DEMO_TPM_BASEADDR, &tpmInfo);

    edgeParam.enableSwap = false;
    /* Set capture edges to calculate the pulse width of input signal */
    edgeParam.currChanEdgeMode = kTPM_RisingEdge;
    edgeParam.nextChanEdgeMode = kTPM_FallingEdge;

    /* Setup dual-edge capture on a TPM channel pair */
    TPM_SetupDualEdgeCapture(DEMO_TPM_BASEADDR, BOARD_TPM_INPUT_CAPTURE_CHANNEL_PAIR, &edgeParam, 0);

    /* Set the timer to be in free-running mode */
    DEMO_TPM_BASEADDR->MOD = 0xFFFF;

    /* Enable channel interrupt when the second edge is detected */
    TPM_EnableInterrupts(DEMO_TPM_BASEADDR, TPM_CHANNEL_INTERRUPT_ENABLE);

    /* Enable at the NVIC */
    EnableIRQ(TPM_INTERRUPT_NUMBER);

    TPM_StartTimer(DEMO_TPM_BASEADDR, kTPM_SystemClock);

    while (tpmIsrFlag != true)
    {
    }

    capture1Val = DEMO_TPM_BASEADDR->CONTROLS[BOARD_TPM_INPUT_CAPTURE_CHANNEL_PAIR * 2].CnV;
    capture2Val = DEMO_TPM_BASEADDR->CONTROLS[(BOARD_TPM_INPUT_CAPTURE_CHANNEL_PAIR * 2) + 1].CnV;
    PRINTF("\r\nCapture value C(n)V=%x\r\n", capture1Val);
    PRINTF("\r\nCapture value C(n+1)V=%x\r\n", capture2Val);

    /* TPM clock source is not prescaled and is
     * divided by 1000000 as the output is printed in microseconds
     */
    pulseWidth = ((capture2Val - capture1Val) + 1) / (TPM_SOURCE_CLOCK / 1000000);

    PRINTF("\r\nInput signals pulse width=%d us\r\n", pulseWidth);
    while (1)
    {
    }
}
