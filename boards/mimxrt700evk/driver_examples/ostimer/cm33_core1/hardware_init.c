/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void EXAMPLE_EnterDeepSleep(void)
{
    POWER_SelectSleepSetpoint(kRegulator_Vdd1LDO, 0U); /* Select lowest voltage when DS. */
    POWER_EnableSleepRBB(kPower_BodyBiasVdd1 | kPower_BodyBiasVdd1Sram);
    /* Enter deep sleep mode by using power API. */
    POWER_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);
}

void BOARD_InitHardware(void)
{
    BOARD_InitAHBSC();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    /* Enable the used modules in sense side. */
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSEP_MAINCLK);
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK);
    POWER_DisablePD(kPDRUNCFG_GATE_FRO2);
    POWER_DisablePD(kPDRUNCFG_PD_FRO2);
    POWER_DisablePD(kPDRUNCFG_SHUT_RAM1_CLK);

    PMC1->PDRUNCFG2 &= ~(0x3FFC0000U); /* Power up all the SRAM partitions in Sense domain. */
    PMC1->PDRUNCFG3 &= ~(0x3FFC0000U);

    POWER_ApplyPD();

    CLOCK_AttachClk(kLPOSC_to_OSTIMER);
    CLOCK_SetClkDiv(kCLOCK_DivOstimerClk, 1U);
}
/*${function:end}*/
