/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "peripherals.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC16_CHANNEL_GROUP 0U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t adcResultValue = 0U;
/*******************************************************************************
 * Code
 ******************************************************************************/
void LPTMR0_IRQHandler(void)
{
    LPTMR_ClearStatusFlags(DEMO_LPTMR_PERIPHERAL, kLPTMR_TimerCompareFlag);
    PRINTF("\r\nThe ADC16 output value is %d.\r\n", adcResultValue);
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
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitPeripherals();

    LPTMR_StartTimer(DEMO_LPTMR_PERIPHERAL);

    while (1)
    {
        /*
         When in software trigger mode, each conversion would be launched once calling the "ADC16_ChannelConfigure()"
         function, which works like writing a conversion command and executing it. For another channel's conversion,
         just to change the "channelNumber" field in channel's configuration structure, and call the
         "ADC16_ChannelConfigure() again.
        */
        ADC16_SetChannelConfig(DEMO_ADC16_PERIPHERAL, DEMO_ADC16_CHANNEL_GROUP, DEMO_ADC16_channelsConfig);

        while (0U == (kADC16_ChannelConversionDoneFlag &
                      ADC16_GetChannelStatusFlags(DEMO_ADC16_PERIPHERAL, DEMO_ADC16_CHANNEL_GROUP)))
        {
        }
        adcResultValue = ADC16_GetChannelConversionValue(DEMO_ADC16_PERIPHERAL, DEMO_ADC16_CHANNEL_GROUP);
    }
}
