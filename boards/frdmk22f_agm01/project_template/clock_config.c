/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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

#include "fsl_common.h"
#include "fsl_smc.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Code
******************************************************************************/

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
    BOARD_BootClockRUN();
}

/*
 * How to setup clock using clock driver functions:
 *
 * 1. CLOCK_SetSimSafeDivs, to make sure core clock, bus clock, flexbus clock
 *    and flash clock are in allowed range during clock mode switch.
 *
 * 2. Call CLOCK_Osc0Init to setup OSC clock, if it is used in target mode.
 *
 * 3. Set MCG configuration, MCG includes three parts: FLL clock, PLL clock and
 *    internal reference clock(MCGIRCLK). Follow the steps to setup:
 *
 *    1). Call CLOCK_BootToXxxMode to set MCG to target mode.
 *
 *    2). If target mode is FBI/BLPI/PBI mode, the MCGIRCLK has been configured
 *        correctly. For other modes, need to call CLOCK_SetInternalRefClkConfig
 *        explicitly to setup MCGIRCLK.
 *
 *    3). Don't need to configure FLL explicitly, because if target mode is FLL
 *        mode, then FLL has been configured by the function CLOCK_BootToXxxMode,
 *        if the target mode is not FLL mode, the FLL is disabled.
 *
 *    4). If target mode is PEE/PBE/PEI/PBI mode, then the related PLL has been
 *        setup by CLOCK_BootToXxxMode. In FBE/FBI/FEE/FBE mode, the PLL could
 *        be enabled independently, call CLOCK_EnablePll0 explicitly in this case.
 *
 * 4. Call CLOCK_SetSimConfig to set the clock configuration in SIM.
 */

void BOARD_BootClockVLPR(void)
{
    const sim_clock_config_t simConfig = {
        .pllFllSel = 3U, .er32kSrc = 2U, .clkdiv1 = 0x00040000U,
    };

    CLOCK_SetSimSafeDivs();

    CLOCK_BootToBlpiMode(0U, kMCG_IrcFast, kMCG_IrclkEnable);

    CLOCK_SetSimConfig(&simConfig);

    SystemCoreClock = 4000000U;

    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    SMC_SetPowerModeVlpr(SMC);
    while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateVlpr)
    {
    }
}

void BOARD_BootClockRUN(void)
{
    const mcg_pll_config_t pll0Config = {
        .enableMode = 0U, .prdiv = 0x3U, .vdiv = 0x10U,
    };

    const sim_clock_config_t simConfig = {
        .pllFllSel = 1U, .er32kSrc = 2U, .clkdiv1 = 0x01230000U,
    };

    CLOCK_SetSimSafeDivs();

    BOARD_InitOsc0();

    CLOCK_BootToPeeMode(kMCG_OscselOsc, kMCG_PllClkSelPll0, &pll0Config);

    CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcSlow, 0U);

    CLOCK_SetSimConfig(&simConfig);

    SystemCoreClock = 80000000U;
}

void BOARD_BootClockHSRUN(void)
{
    const mcg_pll_config_t pll0Config = {
        .enableMode = 0U, .prdiv = 0x1U, .vdiv = 0x6U,
    };
    const sim_clock_config_t simConfig = {
        .pllFllSel = 1U, .er32kSrc = 2U, .clkdiv1 = 0x01340000U,
    };

    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    SMC_SetPowerModeHsrun(SMC);
    while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateHsrun)
    {
    }

    CLOCK_SetSimSafeDivs();

    BOARD_InitOsc0();

    CLOCK_BootToPeeMode(kMCG_OscselOsc, kMCG_PllClkSelPll0, &pll0Config);

    CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcSlow, 0U);

    CLOCK_SetSimConfig(&simConfig);

    SystemCoreClock = 120000000U;
}

void BOARD_InitOsc0(void)
{
    const osc_config_t oscConfig = {.freq = BOARD_XTAL0_CLK_HZ,
                                    .capLoad = 0,
                                    .workMode = kOSC_ModeOscLowPower,
                                    .oscerConfig = {
                                        .enableMode = kOSC_ErClkEnable,
#if (defined(FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER) && FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER)
                                        .erclkDiv = 0U,
#endif
                                    }};

    CLOCK_InitOsc0(&oscConfig);

    /* Passing the XTAL0 frequency to clock driver. */
    CLOCK_SetXtal0Freq(BOARD_XTAL0_CLK_HZ);
}
