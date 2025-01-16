/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016, 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "pmic_support.h"
#include "fsl_power.h"
/*${header:end}*/

/*${macro:start}*/
#define DEMO_IS_XIP_FLEXSPI()                                                                                   \
    ((((uint32_t)BOARD_InitDebugConsole >= 0x08000000U) && ((uint32_t)BOARD_InitDebugConsole < 0x10000000U)) || \
     (((uint32_t)BOARD_InitDebugConsole >= 0x18000000U) && ((uint32_t)BOARD_InitDebugConsole < 0x20000000U)))
/*${macro:end}*/

/*${function:start}*/
/*PLL status*/
extern const clock_sys_pll_config_t g_sysPllConfig_BOARD_BootClockRUN;
extern const clock_audio_pll_config_t g_audioPllConfig_BOARD_BootClockRUN;
AT_QUICKACCESS_SECTION_CODE(void BOARD_SetFlexspiClock(uint32_t src, uint32_t divider));

void BOARD_InitHardware(void)
{
    pca9420_modecfg_t pca9420ModeCfg[4];
    uint32_t i;

    /* BE CAUTIOUS TO SET CORRECT VOLTAGE RANGE ACCORDING TO YOUR BOARD/APPLICATION. PAD SUPPLY BEYOND THE RANGE DO
       HARM TO THE SILICON. */
    power_pad_vrange_t vrange = {.Vdde0Range = kPadVol_171_198,
                                 .Vdde1Range = kPadVol_171_360,
                                 /* SD0 voltage is switchable, but in power_manager demo, it's fixed 3.3V. */
                                 .Vdde2Range = kPadVol_300_360};

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Disable the clock for unused modules.*/
    CLOCK_DisableClock(kCLOCK_RomCtrlr);
    CLOCK_DisableClock(kCLOCK_OtpCtrl);
    CLOCK_DisableClock(kCLOCK_Rng);
    CLOCK_DisableClock(kCLOCK_Puf);
    CLOCK_DisableClock(kCLOCK_HashCrypt);
    CLOCK_DisableClock(kCLOCK_Flexcomm14);
    CLOCK_AttachClk(kNONE_to_FLEXCOMM14);
    CLOCK_DisableClock(kCLOCK_Flexcomm2);
    CLOCK_AttachClk(kNONE_to_FLEXCOMM2);
    CLOCK_DisableClock(kCLOCK_Crc);

    if (!DEMO_IS_XIP_FLEXSPI()) /* If not run XIP from FlexSPI flash, close FlexSPI clock for power saving.*/
    {
        BOARD_DeinitXip(FLEXSPI);
        CLOCK_DisableClock(kCLOCK_Flexspi);
        CLOCK_AttachClk(kNONE_to_FLEXSPI_CLK);
    }

    /* PMIC PCA9420 */
    BOARD_InitPmic();
    for (i = 0; i < ARRAY_SIZE(pca9420ModeCfg); i++)
    {
        PCA9420_GetDefaultModeConfig(&pca9420ModeCfg[i]);
    }
    BOARD_ConfigPMICModes(pca9420ModeCfg, ARRAY_SIZE(pca9420ModeCfg));
    PCA9420_WriteModeConfigs(&pca9420Handle, kPCA9420_Mode0, &pca9420ModeCfg[0], ARRAY_SIZE(pca9420ModeCfg));

    /* Configure PMIC Vddcore value according to main clock. DSP not used in this demo. */
    BOARD_SetPmicVoltageForFreq(kPartTemp_0C_P85C, kVoltOpFullRange, CLOCK_GetFreq(kCLOCK_CoreSysClk), 0U);

    /* Indicate to power library that PMIC is used. */
    POWER_UpdatePmicRecoveryTime(1);

    POWER_SetPadVolRange(&vrange);
}

void BOARD_ConfigPMICModes(pca9420_modecfg_t *cfg, uint32_t num)
{
    assert(cfg);

    /* Configuration PMIC mode to align with power lib like below:
     *  0b00    run mode, no special.
     *  0b01    deep sleep mode, vddcore 0.7V.
     *  0b10    deep powerdown mode, vddcore off.
     *  0b11    full deep powerdown mode vdd1v8 and vddcore off. */

    /* Mode 1: VDDCORE 0.7V. */
    cfg[1].sw1OutVolt = kPCA9420_Sw1OutVolt0V700;

    /* Mode 2: VDDCORE off. */
    cfg[2].enableSw1Out = false;

    /* Mode 3: VDDCORE, VDD1V8 and VDDIO off. */
    cfg[3].enableSw1Out  = false;
    cfg[3].enableSw2Out  = false;
    cfg[3].enableLdo2Out = false;
}
#if POWER_DOWN_PLL_BEFORE_DEEP_SLEEP
void BOARD_DisablePll(void)
{
    if (DEMO_IS_XIP_FLEXSPI())
    {
        /* Let FlexSPI run on FRO. */
        BOARD_SetFlexspiClock(3U, 1U);
    }
    /* Let CPU run on ffro before power down SYS PLL. */
    CLOCK_AttachClk(kFFRO_to_MAIN_CLK);
    /* Disable the PFD clock output first. */
    CLOCK_DeinitSysPfd(kCLOCK_Pfd0);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd2);
    CLOCK_DeinitAudioPfd(kCLOCK_Pfd0);
    /* Disable PLL. */
    CLOCK_DeinitSysPll();
    CLOCK_DeinitAudioPll();
    /* Power down SysOsc */
    POWER_EnablePD(kPDRUNCFG_PD_SYSXTAL);
}

void BOARD_RestorePll(void)
{
    /* Power on SysOsc */
    POWER_DisablePD(kPDRUNCFG_PD_SYSXTAL);
    CLOCK_EnableSysOscClk(true, true, BOARD_SYSOSC_SETTLING_US);
    /*Restore PLL*/
    CLOCK_InitSysPll(&g_sysPllConfig_BOARD_BootClockRUN);
    CLOCK_InitAudioPll(&g_audioPllConfig_BOARD_BootClockRUN);
    /*Restore PFD*/
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 19);   /* Enable main PLL clock 500MHz. */
    CLOCK_InitSysPfd(kCLOCK_Pfd2, 24);   /* Enable aux0 PLL clock 396MHz for SDIO */
    CLOCK_InitAudioPfd(kCLOCK_Pfd0, 26); /* Configure audio_pll_clk to 24.576Mhz */
    /* Let CPU run on SYS PLL PFD0 with divider 2 (250Mhz). */
    CLOCK_AttachClk(kMAIN_PLL_to_MAIN_CLK);
    if (DEMO_IS_XIP_FLEXSPI())
    {
        /* Move FlexSPI clock source from FRO clock to Main PLL clock. */
        BOARD_SetFlexspiClock(1U, 5U);
    }
}
#endif /* POWER_DOWN_PLL_BEFORE_DEEP_SLEEP */
/*${function:end}*/
