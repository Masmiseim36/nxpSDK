/*
 * The Clear BSD License
 * Copyright 2018 NXP
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

    PRINTF("The chip initial temperature is %.1f ¡æ. \r\n", temperature);

    while (1)
    {
        /* Get current temperature */
        temperature = TEMPMON_GetCurrentTemperature(DEMO_TEMPMON);

        if (temperatureReach && (temperature - DEMO_HIGHALARMTEMP > 0))
        {
            temperatureReach = false;

            PRINTF("The chip temperature has reached high temperature that is %.1f ¡æ. \r\n", temperature);
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

            PRINTF("The chip temperature has reached low temperature that is %.1f ¡æ. \r\n", temperature);
            PRINTF("The chip will return to the normal process . \r\n");

            /* Set the core frequency into 500MHz. */
            CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0);

            coreFrequency = CLOCK_GetFreq(kCLOCK_AhbClk);
            PRINTF("The chip core frequency is %d Hz. \r\n", coreFrequency);
        }
    }
}
