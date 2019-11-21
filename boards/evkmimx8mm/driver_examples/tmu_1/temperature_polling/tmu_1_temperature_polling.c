/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_tmu.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_TMU_BASE TMU
#define DEMO_TMU_AMPGAIN_VALUE kTMU_AmplifierGain8_08
#define DEMO_TMU_AMPVREF_VALUE kTMU_AmplifierReferenceVoltage652_5

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Delay function
 *
 * @param count delay count
 */
static void Delay(uint32_t count);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void Delay(uint32_t count)
{
    volatile uint32_t i;

    for (i = 0U; i < count; ++i)
    {
        __asm("NOP");
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    tmu_config_t k_tmuConfig;

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();

    PRINTF("TMU temperature polling example.\r\n");

    /* Initialize the TMU mode. */
    k_tmuConfig.amplifierGain = DEMO_TMU_AMPGAIN_VALUE;
    k_tmuConfig.amplifierVref = DEMO_TMU_AMPVREF_VALUE;
    k_tmuConfig.averageLPF    = kTMU_AverageLowPassFilter0_5;
    TMU_Init(DEMO_TMU_BASE, &k_tmuConfig);

    TMU_Enable(DEMO_TMU_BASE, true);

    while (true)
    {
        uint32_t temp;
        TMU_GetAverageTemperature(TMU, &temp);
        if (temp < 31U || temp > 146U)
        {
            PRINTF("Temperature out of scope!\r\n");
        }
        else
        {
            PRINTF("Average temperature is %d��C\r\n", temp - 21U);
        }
        Delay(20000000U);
    }
}
