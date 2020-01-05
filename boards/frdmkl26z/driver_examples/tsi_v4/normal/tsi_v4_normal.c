/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
 * o Neither the name of the copyright holder nor the names of its
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
#define PAD_TSI_ELECTRODE_1_NAME "E1"

/* TSI indication leds for electrode 1/2 */
#define LED_INIT() LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE() LED_RED_TOGGLE()

/* Get source clock for LPTMR driver */
#define LPTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)
/* Define LPTMR microseconds counts value */
#define LPTMR_USEC_COUNT (200000U)
/* Define the delta value to indicate a touch event */
#define TOUCH_DELTA_VALUE 100U

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
void TSI0_IRQHandler(void)
{
    if (TSI_GetMeasuredChannelNumber(TSI0) == BOARD_TSI_ELECTRODE_1)
    {
        if (TSI_GetCounter(TSI0) > (uint16_t)(buffer.calibratedData[BOARD_TSI_ELECTRODE_1] + TOUCH_DELTA_VALUE))
        {
            LED_TOGGLE(); /* Toggle the touch event indicating LED */
        }
    }

    /* Clear flags */
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
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
    LED_INIT();

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
    LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(LPTMR_USEC_COUNT, LPTMR_SOURCE_CLOCK));

    NVIC_EnableIRQ(TSI0_IRQn);
    TSI_EnableModule(TSI0, true); /* Enable module */

    PRINTF("\r\nTSI_V4 Normal_mode Example Start!\r\n");
    /*********  CALIBRATION PROCESS ************/
    memset((void *)&buffer, 0, sizeof(buffer));
    TSI_Calibrate(TSI0, &buffer);
    /* Print calibrated counter values */
    for (i = 0U; i < FSL_FEATURE_TSI_CHANNEL_COUNT; i++)
    {
        PRINTF("Calibrated counters for channel %d is: %d \r\n", i, buffer.calibratedData[i]);
    }

    /********** SOFTWARE TRIGGER SCAN USING POLLING METHOD ********/
    PRINTF("\r\nNOW, comes to the software trigger scan using polling method!\r\n");
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
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);

    /********** SOFTWARE TRIGGER SCAN USING INTERRUPT METHOD ********/
    PRINTF("\r\nNOW, comes to the software trigger scan using interrupt method!\r\n");
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_1);
    TSI_StartSoftwareTrigger(TSI0);
    while (TSI_IsScanInProgress(TSI0))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_1, TSI_GetCounter(TSI0));

    TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_2);
    TSI_StartSoftwareTrigger(TSI0);
    while (TSI_IsScanInProgress(TSI0))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_2, TSI_GetCounter(TSI0));

    /********** HARDWARE TRIGGER SCAN ********/
    PRINTF("\r\nNOW, comes to the hardware trigger scan method!\r\n");
    PRINTF("After running, touch pad %s each time, you will see LED toggles.\r\n", PAD_TSI_ELECTRODE_1_NAME);
    TSI_EnableModule(TSI0, false);
    TSI_EnableHardwareTriggerScan(TSI0, true);
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    /* Select BOARD_TSI_ELECTRODE_1 as detecting electrode. */
    TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_1);
    TSI_EnableModule(TSI0, true);
    LPTMR_StartTimer(LPTMR0); /* Start LPTMR triggering */

    while (1)
    {
    }
}
