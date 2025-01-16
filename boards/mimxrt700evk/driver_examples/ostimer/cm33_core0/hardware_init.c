/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_power.h"
/*${header:end}*/

/*${variable:start}*/
extern const clock_main_pll_config_t g_mainPllConfigBOARD_BootClockRUN;
extern const clock_audio_pll_config_t g_audioPllConfigBOARD_BootClockRUN;
/*${variable:end}*/
/*${function:start}*/
void BOARD_DisablePll(void)
{
    /* Special sequence is needed for the PLL power up/initialization. The application should manually handle the states
   changes for the PLL if the PLL power states configuration are different in Active mode and Deep Sleep mode. */

    /* Disable the PFD clock output first. */
    CLOCK_DeinitMainPfd(kCLOCK_Pfd0);
    CLOCK_DeinitMainPfd(kCLOCK_Pfd1);
    CLOCK_DeinitMainPfd(kCLOCK_Pfd2);
    CLOCK_DeinitMainPfd(kCLOCK_Pfd3);

    CLOCK_DeinitAudioPfd(kCLOCK_Pfd1);
    CLOCK_DeinitAudioPfd(kCLOCK_Pfd3);
    /* Disable PLL. */
    CLOCK_DeinitMainPll();
    CLOCK_DeinitAudioPll();
}

void BOARD_RestorePll(void)
{
    /*Restore PLL*/
    CLOCK_InitMainPll(&g_mainPllConfigBOARD_BootClockRUN);
    CLOCK_InitAudioPll(&g_audioPllConfigBOARD_BootClockRUN);
    /*Restore PFD*/
    CLOCK_InitMainPfd(kCLOCK_Pfd0, 19U);
    CLOCK_InitMainPfd(kCLOCK_Pfd1, 24U);
    CLOCK_InitMainPfd(kCLOCK_Pfd2, 18U);
    CLOCK_InitMainPfd(kCLOCK_Pfd3, 19U);

    CLOCK_InitAudioPfd(kCLOCK_Pfd1, 24U);
    CLOCK_InitAudioPfd(kCLOCK_Pfd3, 26U);
}

/* Change main clock to a safe source. */
void BOARD_ClockSafeConfig(void)
{
#if 0 /* If main clock is run on PLL, need switch to FRO. */
    /* Switch to FRO1 for safe configure. */
    CLOCK_AttachClk(kFRO1_DIV1_to_COMPUTE_BASE);
    CLOCK_AttachClk(kCOMPUTE_BASE_to_COMPUTE_MAIN);
    CLOCK_SetClkDiv(kCLOCK_DivCmptMainClk, 1U);
    CLOCK_SetClkDiv(kCLOCK_DivComputeRamClk, 1U);
    CLOCK_AttachClk(kFRO1_DIV1_to_RAM);
    CLOCK_AttachClk(kFRO1_DIV1_to_COMMON_BASE);
    CLOCK_AttachClk(kCOMMON_BASE_to_COMMON_VDDN);
    CLOCK_SetClkDiv(kCLOCK_DivCommonVddnClk, 1U);

    CLOCK_AttachClk(kFRO1_DIV3_to_MEDIA_VDD2_BASE);
    CLOCK_AttachClk(kFRO1_DIV1_to_MEDIA_VDDN_BASE);
    CLOCK_AttachClk(kMEDIA_VDD2_BASE_to_MEDIA_MAIN);
    CLOCK_AttachClk(kMEDIA_VDDN_BASE_to_MEDIA_VDDN);
#endif

    BOARD_XspiClockSafeConfig(); /*Change to common_base. */

    BOARD_DisablePll();
}

/* Restore main clock. */
void BOARD_RestoreClockConfig(void)
{
    BOARD_RestorePll();
#if 0 /* If main clock is run on PLL, need switch to FRO. */
    CLOCK_SetClkDiv(kCLOCK_DivCmptMainClk, 2U);
    CLOCK_AttachClk(kMAIN_PLL_PFD0_to_COMPUTE_MAIN);
    CLOCK_SetClkDiv(kCLOCK_DivMediaMainClk, 2U);
    CLOCK_AttachClk(kMAIN_PLL_PFD0_to_MEDIA_MAIN);
    CLOCK_SetClkDiv(kCLOCK_DivMediaVddnClk, 2U);
    CLOCK_AttachClk(kMAIN_PLL_PFD0_to_MEDIA_VDDN);
    CLOCK_SetClkDiv(kCLOCK_DivComputeRamClk, 2U);
    CLOCK_AttachClk(kMAIN_PLL_PFD0_to_RAM);
    CLOCK_SetClkDiv(kCLOCK_DivCommonVddnClk, 2U);
    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_COMMON_VDDN);
#endif
    BOARD_SetXspiClock(XSPI0, 3U, 1U); /* Main PLL PDF1 DIV2. */
}

void EXAMPLE_EnterDeepSleep(void)
{
    BOARD_ClockSafeConfig();

    /* Enter deep sleep mode by using power API. */
    POWER_SelectSleepSetpoint(kRegulator_Vdd2LDO, 0U); /* Select lowest voltage when DS. */
    POWER_EnableSleepRBB(kPower_BodyBiasVdd1 | kPower_BodyBiasVdd2 | kPower_BodyBiasVddn | kPower_BodyBiasVdd1Sram |
                         kPower_BodyBiasVdd2Sram);
    POWER_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);

    BOARD_RestoreClockConfig();
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_AttachClk(kLPOSC_to_OSTIMER);
    CLOCK_SetClkDiv(kCLOCK_DivOstimerClk, 1U);
}
/*${function:end}*/
