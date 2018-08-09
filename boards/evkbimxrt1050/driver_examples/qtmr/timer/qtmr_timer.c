/*
 * The Clear BSD License
 * Copyright 2017 NXP
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

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_qtmr.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The QTMR instance/channel used for board */
#define BOARD_QTMR_BASEADDR TMR3
#define BOARD_FIRST_QTMR_CHANNEL kQTMR_Channel_0
#define BOARD_SECOND_QTMR_CHANNEL kQTMR_Channel_1
#define QTMR_ClockCounterOutput kQTMR_ClockCounter0Output

/* Interrupt number and interrupt handler for the QTMR instance used */
#define QTMR_IRQ_ID TMR3_IRQn
#define QTMR_IRQ_HANDLER TMR3_IRQHandler

/* Get source clock for QTMR driver */
#define QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool qtmrIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void QTMR_IRQ_HANDLER(void)
{
    /* Clear interrupt flag.*/
    QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareFlag);
    
    qtmrIsrFlag = true;
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
    uint8_t i = 0;
    qtmr_config_t qtmrConfig;

    /* Board pin, clock, debug console init */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\n*********QUADTIMER EXAMPLE START*********");

    /*
     * qtmrConfig.debugMode = kQTMR_RunNormalInDebug;
     * qtmrConfig.enableExternalForce = false;
     * qtmrConfig.enableMasterMode = false;
     * qtmrConfig.faultFilterCount = 0;
     * qtmrConfig.faultFilterPeriod = 0;
     * qtmrConfig.primarySource = kQTMR_ClockDivide_2;
     * qtmrConfig.secondarySource = kQTMR_Counter0InputPin;
     */
    QTMR_GetDefaultConfig(&qtmrConfig);
    /* Use IP bus clock div by 128 */
    qtmrConfig.primarySource = kQTMR_ClockDivide_128;

    PRINTF("\r\n****Timer use-case, 50 millisecond tick.****\n");
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, &qtmrConfig);

    /* Set timer period to be 50 millisecond */
    QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, MSEC_TO_COUNT(50U, (QTMR_SOURCE_CLOCK / 128)));

    /* Enable at the NVIC */
    EnableIRQ(QTMR_IRQ_ID);
    
    /* Enable timer compare interrupt */
    QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareInterruptEnable);

    /* Start the second channel to count on rising edge of the primary source clock */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_PriSrcRiseEdge);
    
    for (i = 0; i < 10; i++)
    {
        /* Check whether occur interupt */
        while (!(qtmrIsrFlag))
        {
        }
        PRINTF("\r\n Timer interrupt has occured !");
        qtmrIsrFlag = false;
    }
    QTMR_Deinit(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL);

    PRINTF("\r\n****Chain Timer use-case, 10 second tick.****\n");

    /* Init the first channel to use the IP Bus clock div by 128 */
    qtmrConfig.primarySource = kQTMR_ClockDivide_128;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, &qtmrConfig);

    /* Init the second channel to use output of the first channel as we are chaining the first channel and the second channel */
    qtmrConfig.primarySource = QTMR_ClockCounterOutput;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, &qtmrConfig);
   
    /* Set the first channel period to be 1 millisecond */
   QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, MSEC_TO_COUNT(1U, (QTMR_SOURCE_CLOCK / 128)));

    /* Set the second channel count which increases every millisecond, set compare event for 10 second */
    QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, 10000);

    /* Enable the second channel compare interrupt */
    QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareInterruptEnable);

    /* Start the second channel in cascase mode, chained to the first channel as set earlier via the primary source selection */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CascadeCount);

    /* Start the first channel to count on rising edge of the primary source clock */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, kQTMR_PriSrcRiseEdge);

    for (i = 0; i < 5; i++)
    {
        /* Check whether occur interupt */
        while (!(qtmrIsrFlag))
        {
        }
        PRINTF("\r\n Timer interrupt has occured !");
        qtmrIsrFlag = false;
    }
    PRINTF("\r\n*********QUADTIMER EXAMPLE END.*********");
    while (1)
    {
    }
}
