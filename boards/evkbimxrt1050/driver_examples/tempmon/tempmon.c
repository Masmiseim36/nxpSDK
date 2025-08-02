/*
 * Copyright 2018-2021, 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_tempmon.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

float temperature = 0U;

volatile bool temperatureReach     = false;
uint32_t temperatureReachHighCount = 0x01U;
uint32_t temperatureReachLowCount  = 0x01U;

/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_TEMP_LOW_HIGH_IRQHandler(void)
{
    temperatureReach = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    tempmon_config_t config;
    uint32_t coreFrequency;
    int32_t maxHot = 0U;

    maxHot = (int32_t)(uint32_t)((OCOTP->ANA1 & TEMPMON_HOTTEMPMASK) >> TEMPMON_HOTTEMPSHIFT);

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();
    EnableIRQ(DEMO_TEMP_LOW_HIGH_IRQn);

    PRINTF("TEMPMON driver example. \r\n");

    TEMPMON_GetDefaultConfig(&config);
    config.frequency     = 0x03U;
    config.highAlarmTemp = DEMO_HIGH_ALARM_TEMP;
    config.lowAlarmTemp  = DEMO_LOW_ALARM_TEMP;
    
    /* Set the panic alarm temperature to the maximum temperature which the SoC can operate.
     * Different SoC has different qualified temperature level based on AEC-Q100 standard by default,
     * such as Consumer(0 to +95 degrees celsius), Industrial(-40 to +105 degrees celsius),
     * Automotive(-40 to +125 degrees celsius). Users need to correctly set the panic, high and low
     * alarm temperature within the allowable range according to SoC characteristics and application requirements.
     *
     * The panic threshold (panicAlarmTemp) is a special programmable threshold in that if the temperature
     * increases above this value and the temperature-panic-reset interrupt is enabled in the System Reset Controller,
     * the hardware will assume that software no longer has control over the thermal situation and will initiate a reset
     * of the chip. 
     */
    config.panicAlarmTemp = (int16_t)maxHot;

    TEMPMON_Init(DEMO_TEMP_MONITOR, &config);
    TEMPMON_StartMeasure(DEMO_TEMP_MONITOR);

    /* Get temperature */
    temperature = TEMPMON_GetCurrentTemperature(DEMO_TEMP_MONITOR);

    PRINTF("The chip initial temperature is %.1f degrees celsius. \r\n", (double)temperature);

    while (1)
    {
        /* Get current temperature */
        temperature = TEMPMON_GetCurrentTemperature(DEMO_TEMP_MONITOR);

        if (temperatureReach && (temperature - DEMO_HIGH_ALARM_TEMP > 0))
        {
            temperatureReach = false;

            if (0x01U == temperatureReachHighCount)
            {
                PRINTF("The chip temperature has reached high temperature that is %.1f degrees celsius. \r\n",
                       (double)temperature);
                PRINTF("The chip throttling back core frequency to waiting a desired cool down temperature . \r\n");

                /* Set the core frequency into a lower frequency. */
                CLOCK_SetDiv(DEMO_CLOCK_DIV, 0x07);

                coreFrequency = CLOCK_GetFreq(DEMO_CLOCK_SOURCE);
                PRINTF("The chip core frequency is %d Hz. \r\n", coreFrequency);

                /* Set low alarm temperature */
                TEMPMON_SetTempAlarm(DEMO_TEMP_MONITOR, DEMO_LOW_ALARM_TEMP, kTEMPMON_LowAlarmMode);

                temperatureReachHighCount++;
            }
        }

        if (temperatureReach && (temperature - DEMO_LOW_ALARM_TEMP < 0))
        {
            temperatureReach = false;

            if (0x01U == temperatureReachLowCount)
            {
                PRINTF("The chip temperature has reached low temperature that is %.1f degrees celsius. \r\n",
                       (double)temperature);
                PRINTF("The chip will return to the normal process . \r\n");

                /* Set the core frequency into a higher frequency. */
                CLOCK_SetDiv(DEMO_CLOCK_DIV, 0x0);

                coreFrequency = CLOCK_GetFreq(DEMO_CLOCK_SOURCE);
                PRINTF("The chip core frequency is %d Hz. \r\n", coreFrequency);

                temperatureReachLowCount++;
            }
        }
    }
}
