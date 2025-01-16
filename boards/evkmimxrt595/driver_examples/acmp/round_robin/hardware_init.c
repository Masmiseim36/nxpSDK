/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_rtc.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_power.h"
#include "fsl_acmp.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Let acmp run on main clock with divider 2 (198Mhz). */
    CLOCK_AttachClk(kMAIN_CLK_to_ACMP_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivAcmpClk, 2);

    POWER_DisablePD(kPDRUNCFG_PD_ACMP);
    RESET_PeripheralReset(kACMP0_RST_SHIFT_RSTn);
    /* Make sure ACMP voltage reference available*/
    POWER_SetAnalogBuffer(true);

    /* Enable OSC32K */
    CLOCK_EnableOsc32K(true);
    RTC_Init(RTC);
    CLOCK_AttachClk(kOSC32K_to_32KHZWAKE_CLK);
}

void BOARD_InitAcmpRoundRobinTrigger(uint32_t triggerPeroid_Ms)
{
    uint32_t cycles = MSEC_TO_COUNT(triggerPeroid_Ms, CLOCK_GetOsc32KFreq());
    acmp_discrete_mode_config_t config;

    ACMP_GetDefaultDiscreteModeConfig(&config);
    /* In the example, DAC is selected on positve port. It need to be continuous mode.
     * The other port should be discrete mode. */
    config.enablePositiveChannelDiscreteMode = false;
    config.enableNegativeChannelDiscreteMode = true;
    /* Both port under 1.8V, no divider needed */
    config.enableResistorDivider = false;
    ACMP_SetDiscreteModeConfig(DEMO_ACMP_BASEADDR, &config);

    /* Set up trigger timer */
    if (cycles == 0)
    {
        cycles = 2; /* Set RR_TIMER_RELOAD to 1 */
    }
    cycles = cycles > CMP_RR_TIMER_CR_RR_TIMER_RELOAD_MASK ? CMP_RR_TIMER_CR_RR_TIMER_RELOAD_MASK : cycles - 1;
    /* Set RR_TIMER_RELOAD and start counting. */
    DEMO_ACMP_BASEADDR->RR_TIMER_CR = CMP_RR_TIMER_CR_RR_TIMER_RELOAD(cycles) | CMP_RR_TIMER_CR_RR_TIMER_ENA_MASK;

    /* Enable round robin mode. */
    DEMO_ACMP_BASEADDR->C0 = (DEMO_ACMP_BASEADDR->C0 | CMP_C0_SE_MASK | CMP_C0_WE_MASK) & ~CMP_C0_CFx_MASK;
}

void BOARD_ClearAcmpRoundRobinTrigger(void)
{
    DEMO_ACMP_BASEADDR->RR_TIMER_CR = 0;
}

void BOARD_EnterStopMode(void)
{
    /* Enable deep sleep IRQ. */
    EnableDeepSleepIRQ(DEMO_ACMP_IRQ_ID);
    /* Enter deep sleep mode by using power API. */
    POWER_EnterDeepSleep(EXAMPLE_EXCLUDE_FROM_DEEPSLEEP);
}
/*${function:end}*/
