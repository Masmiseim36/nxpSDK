/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_tpm.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_TPM_BASEADDR TPM2

/* TPM channel pair used for the dual-edge capture, channel pair 0 uses channels 1 */
#define BOARD_TPM_INPUT_CAPTURE_CHANNEL_PAIR kTPM_Chnl_0

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM2_IRQn
#define TPM_INPUT_CAPTURE_HANDLER TPM2_IRQHandler

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_FIRST_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define TPM_FIRST_CHANNEL_FLAG kTPM_Chnl0Flag
#define TPM_SECOND_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl1InterruptEnable
#define TPM_SECOND_CHANNEL_FLAG kTPM_Chnl1Flag

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool tpmFirstChannelInterruptFlag = false;
volatile bool tpmSecondChannelInterruptFlag = false;
/* Record TPM TOF interrupt times */
volatile uint32_t g_timerOverflowInterruptCount = 0u;
volatile uint32_t g_firstChannelOverflowCount = 0u;
volatile uint32_t g_secondChannelOverflowCount = 0u;

/*******************************************************************************
 * Code
 ******************************************************************************/
void TPM_INPUT_CAPTURE_HANDLER(void)
{
    if ((TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & kTPM_TimeOverflowFlag) == kTPM_TimeOverflowFlag)
    {
        /* Clear overflow interrupt flag.*/
        TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, kTPM_TimeOverflowFlag);
        g_timerOverflowInterruptCount++;
    }
    else if ((TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & TPM_FIRST_CHANNEL_FLAG) == TPM_FIRST_CHANNEL_FLAG)
    {
        /* Clear first channel interrupt flag.*/
        TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, TPM_FIRST_CHANNEL_FLAG);
        /* Disable first channel interrupt.*/
        TPM_DisableInterrupts(DEMO_TPM_BASEADDR, TPM_FIRST_CHANNEL_INTERRUPT_ENABLE);
        g_firstChannelOverflowCount = g_timerOverflowInterruptCount;
        tpmFirstChannelInterruptFlag = true;
    }
    else if ((TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & TPM_SECOND_CHANNEL_FLAG) == TPM_SECOND_CHANNEL_FLAG)
    {
        /* Clear second channel interrupt flag.*/
        TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, TPM_SECOND_CHANNEL_FLAG);
        if(tpmFirstChannelInterruptFlag == true)
        {
            /* Disable second channel interrupt.*/
            TPM_DisableInterrupts(DEMO_TPM_BASEADDR, TPM_SECOND_CHANNEL_INTERRUPT_ENABLE);
            g_secondChannelOverflowCount = g_timerOverflowInterruptCount;
            tpmSecondChannelInterruptFlag = true;
        }
    }
    else
    {
    }
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

    /* TPM known issue of KL81, enable clock of TPM2 to use other TPM module */
    CLOCK_EnableClock(kCLOCK_Tpm2);
    /* Select the clock source for the TPM counter as kCLOCK_PllFllSelClk */
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
    
    /* Enable first channel interrupt */
    TPM_EnableInterrupts(DEMO_TPM_BASEADDR, TPM_FIRST_CHANNEL_INTERRUPT_ENABLE);

    /* Enable second channel interrupt when the second edge is detected */
    TPM_EnableInterrupts(DEMO_TPM_BASEADDR, TPM_SECOND_CHANNEL_INTERRUPT_ENABLE);

    /* Enable overflow interrupt */
    TPM_EnableInterrupts(DEMO_TPM_BASEADDR, kTPM_TimeOverflowInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(TPM_INTERRUPT_NUMBER);

    TPM_StartTimer(DEMO_TPM_BASEADDR, kTPM_SystemClock);

    while (tpmFirstChannelInterruptFlag != true)
    {
    }
    
    while (tpmSecondChannelInterruptFlag != true)
    {
    }

    /* Clear overflow interrupt flag */
    TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, kTPM_TimeOverflowFlag);
    /* Disable overflow interrupt.*/
    TPM_DisableInterrupts(DEMO_TPM_BASEADDR, kTPM_TimeOverflowInterruptEnable);

    capture1Val = DEMO_TPM_BASEADDR->CONTROLS[BOARD_TPM_INPUT_CAPTURE_CHANNEL_PAIR * 2].CnV;
    capture2Val = DEMO_TPM_BASEADDR->CONTROLS[(BOARD_TPM_INPUT_CAPTURE_CHANNEL_PAIR * 2) + 1].CnV;
    PRINTF("\r\nCapture value C(n)V=%x\r\n", capture1Val);
    PRINTF("\r\nCapture value C(n+1)V=%x\r\n", capture2Val);

    /* TPM clock source is not prescaled and is
     * divided by 1000000 as the output is printed in microseconds
     */
    pulseWidth = (((g_secondChannelOverflowCount - g_firstChannelOverflowCount) * 65536 + capture2Val - capture1Val) + 1) / (TPM_SOURCE_CLOCK / 1000000);

    PRINTF("\r\nInput signals pulse width=%d us\r\n", pulseWidth);
    while (1)
    {
    }
}
