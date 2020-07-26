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

/*****************************************************************************
 *  Includes
 ****************************************************************************/

#include "board.h"
#include "fsl_tsi_v2.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Available PAD names on board */
#define PAD_TSI_ELECTRODE_1_NAME "D7"
#define PAD_TSI_ELECTRODE_2_NAME "D8"

/* TSI indication leds for electrode 1/2 */
#define LED1_INIT() LED_ORANGE_INIT(LOGIC_LED_OFF)
#define LED1_ON() LED_ORANGE_ON()
#define LED1_OFF() LED_ORANGE_OFF()

#define LED2_INIT() LED_YELLOW_INIT(LOGIC_LED_OFF)
#define LED2_ON() LED_YELLOW_ON()
#define LED2_OFF() LED_YELLOW_OFF()

/* Define the delta value to indicate a touch event */
#define TOUCH_DELTA_VALUE 100U
#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

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
    SysTick->LOAD = n * ((CORE_CLK_FREQ) / 1000U); /* n ms */
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
    if (TSI_IsChannelEnabled(TSI0, BOARD_TSI_ELECTRODE_1))
    {
        if (TSI_GetNormalModeCounter(TSI0, BOARD_TSI_ELECTRODE_1) >
            (uint16_t)(buffer.calibratedData[BOARD_TSI_ELECTRODE_1] + TOUCH_DELTA_VALUE))
        {
            LED2_ON();
            DelayMs(50);
            DelayMs(50);
            LED2_OFF();
        }
    }
    if (TSI_IsChannelEnabled(TSI0, BOARD_TSI_ELECTRODE_2))
    {
        if (TSI_GetNormalModeCounter(TSI0, BOARD_TSI_ELECTRODE_2) >
            (buffer.calibratedData[BOARD_TSI_ELECTRODE_2] + TOUCH_DELTA_VALUE))
        {
            LED1_ON();
            DelayMs(50);
            DelayMs(50);
            LED1_OFF();
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
    volatile uint32_t i = 0U;
    tsi_config_t tsiConfig_normal = {0};

    /* Initialize standard SDK demo application pins */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Init tsi Leds in Demo app */
    LED1_INIT();
    LED2_INIT();

    TSI_GetNormalModeDefaultConfig(&tsiConfig_normal);
    TSI_Init(TSI0, &tsiConfig_normal);
    NVIC_EnableIRQ(TSI0_IRQn);
    TSI_EnableModule(TSI0, true); /* Enable module */

    PRINTF("\r\nTSI_V2 Normal_mode Example Start!\r\n");
    /*********  TSI_V2 CALIBRATION PROCESS ************/
    memset((void *)&buffer, 0, sizeof(buffer));
    TSI_Calibrate(TSI0, &buffer);
    /* Print calibrated counter values */
    for (i = 0U; i < FSL_FEATURE_TSI_CHANNEL_COUNT; i++)
    {
        PRINTF("Calibrated counters for channel %d is: %d \r\n", i, buffer.calibratedData[i]);
    }

    /********** SOFTWARE TRIGGER SCAN USING POLLING METHOD ********/
    PRINTF("\r\nNOW, comes to the software trigger scan using polling method!\r\n");
    TSI_EnablePeriodicalScan(TSI0, false); /* Enable Software trigger scan */
    TSI_DisableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_EnableChannel(TSI0, BOARD_TSI_ELECTRODE_1, true);
    TSI_StartSoftwareTrigger(TSI0);
    while (!(TSI_GetStatusFlags(TSI0) & kTSI_EndOfScanFlag))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_1,
           TSI_GetNormalModeCounter(TSI0, BOARD_TSI_ELECTRODE_1));

    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_EnableChannel(TSI0, BOARD_TSI_ELECTRODE_2, true);
    TSI_StartSoftwareTrigger(TSI0);
    while (!(TSI_GetStatusFlags(TSI0) & kTSI_EndOfScanFlag))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_2,
           TSI_GetNormalModeCounter(TSI0, BOARD_TSI_ELECTRODE_2));
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);

    /********** SOFTWARE TRIGGER SCAN USING INTERRUPT METHOD ********/
    PRINTF("\r\nNOW, comes to the software trigger scan using interrupt method!\r\n");
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_EnableChannel(TSI0, BOARD_TSI_ELECTRODE_1, true);
    TSI_StartSoftwareTrigger(TSI0);
    while (TSI_IsScanInProgress(TSI0))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_1,
           TSI_GetNormalModeCounter(TSI0, BOARD_TSI_ELECTRODE_1));

    TSI_EnableChannel(TSI0, BOARD_TSI_ELECTRODE_2, true);
    TSI_StartSoftwareTrigger(TSI0);
    while (TSI_IsScanInProgress(TSI0))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_2,
           TSI_GetNormalModeCounter(TSI0, BOARD_TSI_ELECTRODE_2));

    /********** PERIOD TRIGGER SCAN ********/
    PRINTF("\r\nNOW, comes to the periodical scan method!\r\n");
    PRINTF("After running, touch pad %s or %s, you will see LED shines.\r\n", PAD_TSI_ELECTRODE_1_NAME, \
                                                                              PAD_TSI_ELECTRODE_2_NAME);
    TSI_EnableModule(TSI0, false); /* Disable module */
    TSI_EnablePeriodicalScan(TSI0, true);
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);

    TSI_EnableChannel(TSI0, BOARD_TSI_ELECTRODE_1, true); /* Select electrode 1 as detecting electrode. */
    TSI_EnableModule(TSI0, true);                         /* Enable module, the hardware trigger scan begins */

    while (1)
    {
    }
}
