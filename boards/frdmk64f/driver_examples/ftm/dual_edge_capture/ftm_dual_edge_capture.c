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
#include "fsl_ftm.h"

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The Flextimer instance/channel used for board */
#define DEMO_FTM_BASEADDR FTM0

/* FTM channel pair used for the dual-edge capture, channel pair 0 uses channels 0 and 1 */
#define BOARD_FTM_INPUT_CAPTURE_CHANNEL_PAIR kFTM_Chnl_0

/* Interrupt number and interrupt handler for the FTM instance used */
#define FTM_INTERRUPT_NUMBER FTM0_IRQn
#define FTM_INPUT_CAPTURE_HANDLER FTM0_IRQHandler

/* Interrupt to enable and flag to read; depends on the FTM channel used for dual-edge capture */
#define FTM_FIRST_CHANNEL_INTERRUPT_ENABLE kFTM_Chnl0InterruptEnable
#define FTM_FIRST_CHANNEL_FLAG kFTM_Chnl0Flag
#define FTM_SECOND_CHANNEL_INTERRUPT_ENABLE kFTM_Chnl1InterruptEnable
#define FTM_SECOND_CHANNEL_FLAG kFTM_Chnl1Flag

/* Get source clock for FTM driver */
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool ftmFirstChannelInterruptFlag = false;
volatile bool ftmSecondChannelInterruptFlag = false;
/* Record FTM TOF interrupt times */
volatile uint32_t g_timerOverflowInterruptCount = 0u;
volatile uint32_t g_firstChannelOverflowCount = 0u;
volatile uint32_t g_secondChannelOverflowCount = 0u;

/*******************************************************************************
 * Code
 ******************************************************************************/
void FTM_INPUT_CAPTURE_HANDLER(void)
{
    if ((FTM_GetStatusFlags(DEMO_FTM_BASEADDR) & kFTM_TimeOverflowFlag) == kFTM_TimeOverflowFlag)
    {
        /* Clear overflow interrupt flag.*/
        FTM_ClearStatusFlags(DEMO_FTM_BASEADDR, kFTM_TimeOverflowFlag);
        g_timerOverflowInterruptCount++;
    }
    else if (((FTM_GetStatusFlags(DEMO_FTM_BASEADDR) & FTM_FIRST_CHANNEL_FLAG) == FTM_FIRST_CHANNEL_FLAG) && (ftmFirstChannelInterruptFlag == false))
    {
        /* Disable first channel interrupt.*/
        FTM_DisableInterrupts(DEMO_FTM_BASEADDR, FTM_FIRST_CHANNEL_INTERRUPT_ENABLE);
        g_firstChannelOverflowCount = g_timerOverflowInterruptCount;
        ftmFirstChannelInterruptFlag = true;
    }
    else if ((FTM_GetStatusFlags(DEMO_FTM_BASEADDR) & FTM_SECOND_CHANNEL_FLAG) == FTM_SECOND_CHANNEL_FLAG)
    {
        /* Clear second channel interrupt flag.*/
        FTM_ClearStatusFlags(DEMO_FTM_BASEADDR, FTM_SECOND_CHANNEL_FLAG);
        /* Disable second channel interrupt.*/
        FTM_DisableInterrupts(DEMO_FTM_BASEADDR, FTM_SECOND_CHANNEL_INTERRUPT_ENABLE);
        g_secondChannelOverflowCount = g_timerOverflowInterruptCount;
        ftmSecondChannelInterruptFlag = true;
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
    ftm_config_t ftmInfo;
    ftm_dual_edge_capture_param_t edgeParam;
    uint32_t capture1Val;
    uint32_t capture2Val;
    uint32_t pulseWidth;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Print a note to terminal */
    PRINTF("\r\nFTM dual-edge capture example\r\n");
    PRINTF("\r\nOnce the input signal is received the input capture values are printed");
    PRINTF("\r\nThe input signal's pulse width is calculated from the capture values & printed\r\n");

    FTM_GetDefaultConfig(&ftmInfo);
    /* Initialize FTM module */
    FTM_Init(DEMO_FTM_BASEADDR, &ftmInfo);

    edgeParam.mode = kFTM_OneShot;
    /* Set capture edges to calculate the pulse width of input signal */
    edgeParam.currChanEdgeMode = kFTM_RisingEdge;
    edgeParam.nextChanEdgeMode = kFTM_FallingEdge;

    /* Setup dual-edge capture on a FTM channel pair */
    FTM_SetupDualEdgeCapture(DEMO_FTM_BASEADDR, BOARD_FTM_INPUT_CAPTURE_CHANNEL_PAIR, &edgeParam, 0);

    /* Set the timer to be in free-running mode */
    DEMO_FTM_BASEADDR->MOD = 0xFFFF;

    /* Enable first channel interrupt */
    FTM_EnableInterrupts(DEMO_FTM_BASEADDR, FTM_FIRST_CHANNEL_INTERRUPT_ENABLE);
    
    /* Enable second channel interrupt when the second edge is detected */
    FTM_EnableInterrupts(DEMO_FTM_BASEADDR, FTM_SECOND_CHANNEL_INTERRUPT_ENABLE);

    /* Enable overflow interrupt */
    FTM_EnableInterrupts(DEMO_FTM_BASEADDR, kFTM_TimeOverflowInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(FTM_INTERRUPT_NUMBER);

    FTM_StartTimer(DEMO_FTM_BASEADDR, kFTM_SystemClock);
    
    while (ftmFirstChannelInterruptFlag != true)
    {
    }
    
    while (ftmSecondChannelInterruptFlag != true)
    {
    }
    
    /* Clear first channel interrupt flag after the second edge is detected.*/
    FTM_ClearStatusFlags(DEMO_FTM_BASEADDR, FTM_FIRST_CHANNEL_FLAG);
    
    /* Clear overflow interrupt flag.*/
    FTM_ClearStatusFlags(DEMO_FTM_BASEADDR, kFTM_TimeOverflowFlag);
    /* Disable overflow interrupt.*/
    FTM_DisableInterrupts(DEMO_FTM_BASEADDR, kFTM_TimeOverflowInterruptEnable);
    
    capture1Val = DEMO_FTM_BASEADDR->CONTROLS[BOARD_FTM_INPUT_CAPTURE_CHANNEL_PAIR * 2].CnV;
    capture2Val = DEMO_FTM_BASEADDR->CONTROLS[(BOARD_FTM_INPUT_CAPTURE_CHANNEL_PAIR * 2) + 1].CnV;
    PRINTF("\r\nCapture value C(n)V=%x\r\n", capture1Val);
    PRINTF("\r\nCapture value C(n+1)V=%x\r\n", capture2Val);

    /* FTM clock source is not prescaled and is
     * divided by 1000000 as the output is printed in microseconds
     */
    pulseWidth = (((g_secondChannelOverflowCount - g_firstChannelOverflowCount) * 65536 + capture2Val - capture1Val) + 1) / (FTM_SOURCE_CLOCK / 1000000);

    PRINTF("\r\nInput signals pulse width=%d us\r\n", pulseWidth);

    while (1)
    {
    }
}
