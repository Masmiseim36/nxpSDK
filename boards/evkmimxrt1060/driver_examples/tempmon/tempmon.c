/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_tempmon.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_TEMPMON TEMPMON
#define DEMO_TEMP_LOW_HIGH_IRQn TEMP_LOW_HIGH_IRQn
#define DEMO_TEMP_PANIC_IRQn TEMP_PANIC_IRQn
#define DEMO_TEMP_LOW_HIGH_IRQHandler TEMP_LOW_HIGH_IRQHandler
#define DEMO_TEMP_PANIC_IRQHandler TEMP_PANIC_IRQHandler

#define DEMO_HIGHALARMTEMP 42U
#define DEMO_LOWALARMTEMP 40U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

float temperature = 0U;

volatile bool temperatureReach = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_TEMP_LOW_HIGH_IRQHandler(void)
{
    temperatureReach = true;

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
    tempmon_config_t config;
    uint32_t coreFrequency;

    /* Board pin, clock, debug console init */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    EnableIRQ(DEMO_TEMP_LOW_HIGH_IRQn);

    PRINTF("TEMPMON driver example. \r\n");

    TEMPMON_GetDefaultConfig(&config);
    config.frequency = 0x03U;
    config.highAlarmTemp = DEMO_HIGHALARMTEMP;

    TEMPMON_Init(DEMO_TEMPMON, &config);
    TEMPMON_StartMeasure(DEMO_TEMPMON);

    /* Get temperature */
    temperature = TEMPMON_GetCurrentTemperature(DEMO_TEMPMON);

    PRINTF("The chip initial temperature is %.1f ��. \r\n", temperature);

    while (1)
    {
        /* Get current temperature */
        temperature = TEMPMON_GetCurrentTemperature(DEMO_TEMPMON);

        if (temperatureReach && (temperature - DEMO_HIGHALARMTEMP > 0))
        {
            temperatureReach = false;

            PRINTF("The chip temperature has reached high temperature that is %.1f ��. \r\n", temperature);
            PRINTF("The chip throttling back core frequency to waiting a desired cool down temperature . \r\n");

            /* Set the core frequency into 62.5MHz. */
            CLOCK_SetDiv(kCLOCK_AhbDiv, 0x07);

            coreFrequency = CLOCK_GetFreq(kCLOCK_AhbClk);
            PRINTF("The chip core frequency is %d Hz. \r\n", coreFrequency);

            /* Set low alarm temperature */
            TEMPMON_SetTempAlarm(DEMO_TEMPMON, DEMO_LOWALARMTEMP, kTEMPMON_LowAlarmMode);
        }

        if (temperatureReach && (temperature - DEMO_LOWALARMTEMP < 0))
        {
            temperatureReach = false;

            PRINTF("The chip temperature has reached low temperature that is %.1f ��. \r\n", temperature);
            PRINTF("The chip will return to the normal process . \r\n");

            /* Set the core frequency into 500MHz. */
            CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0);

            coreFrequency = CLOCK_GetFreq(kCLOCK_AhbClk);
            PRINTF("The chip core frequency is %d Hz. \r\n", coreFrequency);
        }
    }
}
