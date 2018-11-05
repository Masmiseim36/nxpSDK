/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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
#include "fsl_enc.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_xbara.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ENC_BASEADDR ENC
#define DEMO_ENC_INDEX_IRQ_ID ENC_INDEX_IRQn

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_EncIndexCounter = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/

void NVIC_Configuration(void)
{
    EnableIRQ(DEMO_ENC_INDEX_IRQ_ID); /* Enable the interrupt for ENC_INDEX. */
}

/*!
 * @brief ISR for INDEX event
 */
void ENC_INDEX_IRQHandler(void)
{
    g_EncIndexCounter++;
    ENC_ClearStatusFlags(DEMO_ENC_BASEADDR, kENC_INDEXPulseFlag);
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
    enc_config_t mEncConfigStruct;
    uint32_t mCurPosValue;

    XBARA_Init(XBARA);
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nENC INDEX Interrupt Example.\r\n");

    /* Initialize the ENC module. */
    ENC_GetDefaultConfig(&mEncConfigStruct);
    /* Configure the INDEX action. */
    mEncConfigStruct.INDEXTriggerMode = kENC_INDEXTriggerOnRisingEdge;
    /* Setup the hardware. */
    ENC_Init(DEMO_ENC_BASEADDR, &mEncConfigStruct);
    /* Update position counter from initial position register. */
    ENC_DoSoftwareLoadInitialPositionValue(DEMO_ENC_BASEADDR);

    PRINTF("Press any key to get the encoder values ...\r\n");

    /* Interrupts. */
    NVIC_Configuration();
    ENC_ClearStatusFlags(DEMO_ENC_BASEADDR, kENC_INDEXPulseFlag);
    ENC_EnableInterrupts(DEMO_ENC_BASEADDR, kENC_INDEXPulseInterruptEnable);

    while (1)
    {
        GETCHAR();
        PRINTF("\r\n");

        /* This read operation would capture all the positon counter to responding hold registers. */
        mCurPosValue = ENC_GetPositionValue(DEMO_ENC_BASEADDR);

        PRINTF("Current position value: %ld\r\n", mCurPosValue);
        PRINTF("Current position differential value: %d\r\n",
               (int16_t)ENC_GetHoldPositionDifferenceValue(DEMO_ENC_BASEADDR));
        PRINTF("Current position revolution value: %d\r\n", ENC_GetHoldRevolutionValue(DEMO_ENC_BASEADDR));
        PRINTF("g_EncIndexCounter: %d\r\n", g_EncIndexCounter);
    }
}
