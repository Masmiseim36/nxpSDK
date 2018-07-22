/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
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

#include "board.h"
#include "fsl_tsi_v4.h"
#include "fsl_debug_console.h"
#include "fsl_lptmr.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Available PAD names on board */
#define PAD_TSI_ELECTRODE_1_NAME "D9"
#define PAD_TSI_ELECTRODE_2_NAME "D8"

/* TSI leds of electrode 1/2 */
#define LED1_INIT() LED_RED_INIT(LOGIC_LED_OFF)
#define LED1_ON() LED_RED_ON()
#define LED1_OFF() LED_RED_OFF()

#define LED2_INIT() LED_GREEN_INIT(LOGIC_LED_OFF)
#define LED2_ON() LED_GREEN_ON()
#define LED2_OFF() LED_GREEN_OFF()

/* Get source clock for LPTMR driver */
#define LPTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
tsi_calibration_data_t buffer;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Delay n ms */
void DelayMs(uint8_t n)
{
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;                      /* use core clock */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;                       /* disable interrupt */
    SysTick->LOAD = n * ((CLOCK_GetFreq(kCLOCK_CoreSysClk)) / 1000U); /* n ms */
    SysTick->VAL = 0U;                                                /* clear COUNTFLAG */

    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    /* wait for timeout */
    while (0U == (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
    {
    }

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void TSI0_IRQHandler(void)
{
    /* Clear flags */
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
    LED1_OFF();
    LED2_OFF();
    PRINTF("Enter into TSI0 IRQHandler...\r\n");

    if (TSI_GetMeasuredChannelNumber(TSI0) == BOARD_TSI_ELECTRODE_1)
    {
        if (TSI_GetCounter(TSI0) > (buffer.calibratedData[BOARD_TSI_ELECTRODE_1] + 100U))
        {
            LED1_ON();
            DelayMs(50U);
        }
        PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_1, TSI_GetCounter(TSI0));
    }
    if (TSI_GetMeasuredChannelNumber(TSI0) == BOARD_TSI_ELECTRODE_2)
    {
        if (TSI_GetCounter(TSI0) > (buffer.calibratedData[BOARD_TSI_ELECTRODE_2] + 100U))
        {
            LED2_ON();
            DelayMs(50U);
        }
        PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_2, TSI_GetCounter(TSI0));
    }
    LED1_OFF();
    LED2_OFF();
}

/*!
 * @brief Main function
 */
int main(void)
{
    volatile uint32_t i = 0;
    tsi_config_t tsiConfig_normal = {0};
    lptmr_config_t lptmrConfig;
    memset((void *)&lptmrConfig, 0, sizeof(lptmrConfig));

    /* Initialize standard SDK demo application pins */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Init tsi Leds in Demo app */
    LED1_INIT();
    LED2_INIT();

    /* Configure LPTMR */
    /*
     * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig.enableFreeRunning = false;
     * lptmrConfig.bypassPrescaler = true;
     * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    /* TSI default hardware configuration for normal mode */
    TSI_GetNormalModeDefaultConfig(&tsiConfig_normal);

    /* Initialize the LPTMR */
    LPTMR_Init(LPTMR0, &lptmrConfig);
    /* Initialize the TSI */
    TSI_Init(TSI0, &tsiConfig_normal);

    /* Set timer period */
    LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(500000U, LPTMR_SOURCE_CLOCK));

    NVIC_EnableIRQ(TSI0_IRQn);
    TSI_EnableModule(TSI0, true); /* Enable module */

    PRINTF("\r\nTSI_V4 Normal_mode Example Start!\r\n");
    /*********  TEST CALIBRATION PROCESS ************/
    memset((void *)&buffer, 0, sizeof(buffer));
    TSI_Calibrate(TSI0, &buffer);
    /* Print calibrated counter values */
    for (i = 0U; i < FSL_FEATURE_TSI_CHANNEL_COUNT; i++)
    {
        PRINTF("Calibrated counters for channel %d is: %d \r\n", i, buffer.calibratedData[i]);
    }

    /********** TEST SOFTWARE TRIGGER SCAN USING POLLING METHOD ********/
    PRINTF("\r\nNOW, Test the software trigger scan using polling method!\r\n");
    TSI_EnableHardwareTriggerScan(TSI0, false); /* Enable software trigger scan */
    TSI_DisableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_1);
    TSI_StartSoftwareTrigger(TSI0);
    while (!(TSI_GetStatusFlags(TSI0) & kTSI_EndOfScanFlag))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_1, TSI_GetCounter(TSI0));

    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_2);
    TSI_StartSoftwareTrigger(TSI0);
    while (!(TSI_GetStatusFlags(TSI0) & kTSI_EndOfScanFlag))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_2, TSI_GetCounter(TSI0));

    /********** TEST SOFTWARE TRIGGER SCAN USING INTERRUPT METHOD ********/
    PRINTF("\r\nNOW, Test the software trigger scan using interrupt method!\r\n");
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_1);
    TSI_StartSoftwareTrigger(TSI0);
    while (TSI_IsScanInProgress(TSI0))
    {
    }

    TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_2);
    TSI_StartSoftwareTrigger(TSI0);
    while (TSI_IsScanInProgress(TSI0))
    {
    }

    /********** TEST HARDWARE TRIGGER SCAN ********/
    PRINTF("\r\nNOW, Test the hardware trigger scan method!\r\n");
    PRINTF("After running, touch pad %s, you will see LED shines.\r\n", PAD_TSI_ELECTRODE_1_NAME);
    TSI_EnableModule(TSI0, false);
    TSI_EnableHardwareTriggerScan(TSI0, true);
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);

    TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_1); /* Select TSI_CHANNEL_1 as detecting electrode. */
    TSI_EnableModule(TSI0, true);
    LPTMR_StartTimer(LPTMR0); /* Start LPTMR triggering */

    while (1)
    {
    }
}
