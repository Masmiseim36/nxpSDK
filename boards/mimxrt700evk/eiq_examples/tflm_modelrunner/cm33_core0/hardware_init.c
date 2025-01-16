/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
#include "app.h"
#include "timer.h"
#include "pmic_support.h"
/*${function:start}*/
int64_t os_clock_now(){
    return TIMER_GetTimeInUS();
}
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    //BOARD_InitBootClocks();
    //BOARD_InitDebugConsole();
    BOARD_InitPmicPins();
    BOARD_InitPmic();
    BOARD_SetPmicVdd2Voltage(1100000U); /* 1.1v for 325MHz clock. */

    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();
    BOARD_InitAHBSC();

    POWER_DisablePD(kPDRUNCFG_APD_NPU);
    POWER_DisablePD(kPDRUNCFG_PPD_NPU);
    POWER_ApplyPD();
    TIMER_Init();
#if defined(DEMO_USE_XSPI2) && DEMO_USE_XSPI2
    BOARD_InitPsRamPins_Xspi2();
    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_XSPI2);
    CLOCK_SetClkDiv(kCLOCK_DivXspi2Clk, 1u);     /*500MHz*/
#elif defined(DEMO_USE_XSPI1) && DEMO_USE_XSPI1
    BOARD_InitPsRamPins_Xspi1();
    CLOCK_AttachClk(kAUDIO_PLL_PFD1_to_XSPI1);
    CLOCK_SetClkDiv(kCLOCK_DivXspi1Clk, 1u);     /*400MHz*/
#endif
}
/*${function:end}*/
