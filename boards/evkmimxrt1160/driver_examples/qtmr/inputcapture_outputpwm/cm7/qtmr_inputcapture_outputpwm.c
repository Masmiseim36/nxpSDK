/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_qtmr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
    QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_EdgeFlag);

    qtmrIsrFlag = true;
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    qtmr_config_t qtmrConfig;
    uint32_t counterClock = 0;
    uint32_t timeCapt     = 0;
    uint32_t count        = 0;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("\r\n****Input capture example.****\n");
    PRINTF("\r\n****Provide a signal input to the QTMR pin****\n");

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

    /* Initial the input channel. */
    qtmrConfig.primarySource = QTMR_PRIMARY_SOURCE;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, &qtmrConfig);

    /* Setup the input capture */
    QTMR_SetupInputCapture(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, QTMR_CounterInputPin, false, true,
                           kQTMR_RisingEdge);

    /* Enable at the NVIC */
    EnableIRQ(QTMR_IRQ_ID);

    /* Enable timer compare interrupt */
    QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_EdgeInterruptEnable);

    /* Start the input channel to count on rising edge of the primary source clock */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_PriSrcRiseEdge);

    counterClock = QTMR_SOURCE_CLOCK / 1000U;

    /* Check whether occur interupt and wait the capture frequency stable */
    while (count < 5 || timeCapt == 0)
    {
        while (!(qtmrIsrFlag))
        {
        }
        qtmrIsrFlag = false;
        count++;
        timeCapt = BOARD_QTMR_BASEADDR->CHANNEL[BOARD_QTMR_INPUT_CAPTURE_CHANNEL].CAPT;
    }
    PRINTF("\r\nCaptured Period time=%d us\n", (timeCapt * 1000U) / counterClock);

    PRINTF("\r\n****Output PWM example.****\n");
    PRINTF("\r\n*********Make sure to connect an oscilloscope.*********\n");
    PRINTF("\r\n****A 50% duty cycle PWM wave is observed on an oscilloscope.****\n");

    /* Initial the output channel. */
    qtmrConfig.primarySource = QTMR_PRIMARY_SOURCE;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, &qtmrConfig);

    /* Generate a 50Khz PWM signal with 50% dutycycle */
    QTMR_SetupPwm(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, QTMR_PWM_OUTPUT_FREQUENCY, QTMR_DUTYCYCLE_PERCENT, false,
                  QTMR_SOURCE_CLOCK);

    /* Start the counter */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, kQTMR_PriSrcRiseEdge);

    while (1)
    {
    }
}
