/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_clock.h"
#include "fsl_pca9422.h"
#include "pmic_support.h"
#include "core1_support.h"
#include "fsl_cache.h"
#include "fsl_debug_console.h"
#include "fsl_mu.h"
#include "fsl_utick.h"
#include "power_demo_config.h"
#include "fsl_iopctl.h"
#include "FreeRTOS.h"
#include "fsl_ostimer.h"
#include "dsp_support.h"
#include "fsl_dsp.h"
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

/*${function:start}*/

/* Before accessing shared resource, such as PMIC I2C, debug, PMC registers, the shared sense main clock should be
 * enabled. */
void BOARD_EnableSharedSenseMainClk(void)
{
    CLOCK_EnableClock(kCLOCK_Sleepcon0);            /* make sure the sleepcon clock is enabled. */
    POWER_DisablePD(kPDRUNCFG_PD_FRO2);
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK); /* Keep Sense shared parts clock on. */
}

void BOARD_DisableSharedSenseMainClk(void)
{
    POWER_EnablePD(kPDRUNCFG_PD_FRO2);
    POWER_EnablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK); /* Keep Sense shared parts clock on. */
}

static void BOARD_BootClockRUN_InitFRO0(void)
{
    const clock_fro_config_t g_fro0Config_BOARD_BootClockRUN = {
#if (DEMO_MAINCLK_FREQ == DEMO_MAINCLK_FREQ_SP0)
        .targetFreq = 220000000, /* FRO0 TUNER output clock frequency: 325000000Hz */
#elif (DEMO_MAINCLK_FREQ == DEMO_MAINCLK_FREQ_SP1)
        .targetFreq = 192000000, /* FRO0 TUNER output clock frequency: 192000000Hz */
#else
        .targetFreq = 325000000, /* FRO0 TUNER output clock frequency: 220000000Hz */
#endif
        .range         = 100,   /* FRO0 range value: 100counts */
        .trim1DelayUs  = 15,    /* FRO0 Trim1 delay: 15us */
        .trim2DelayUs  = 150,   /* FRO0 Trim2 delay: 150us */
        .refDiv        = 1,     /* The FRO reference divider is 1 */
        .enableInt     = 0,     /* The FRO interrupts are disabled */
        .coarseTrimEn  = false, /* The coarse value autotrimming is disabled */
        .fastStartupEn = false, /* The fast startup is disabled */
    };

    /* Enable power and ungate the FRO0. */
    POWER_DisablePD(kPDRUNCFG_GATE_FRO0);
    POWER_DisablePD(kPDRUNCFG_PD_FRO0);
    /* Configure FRO clock module in closed loop (autotrimming) mode */
    CLOCK_EnableFroClkFreqCloseLoop(FRO0, &g_fro0Config_BOARD_BootClockRUN,
                                    kCLOCK_FroDiv1OutEn | kCLOCK_FroDiv3OutEn | kCLOCK_FroDiv6OutEn);
    /* Setup domain specific clock gates */
    CLOCK_EnableFro0ClkForDomain(kCLOCK_VddnComDomainEnable | kCLOCK_Vdd2CompDomainEnable);
}

void BOARD_ClockLPPreConfig(void)
{
    POWER_DisablePD(kPDRUNCFG_PD_FRO1);                        /* Make sure FRO1 is enabled. */

    BOARD_BootClockRUN_InitClockModule(kClockModule_XTAL_OSC); /* Enable SOSC used for FRO trim. */
    BOARD_BootClockRUN_InitFRO0();
#if (DEMO_MAINCLK_FREQ == DEMO_MAINCLK_FREQ_SP0)
    CLOCK_SetClkDiv(kCLOCK_DivCmptMainClk, 2U);
#else
    CLOCK_SetClkDiv(kCLOCK_DivCmptMainClk, 1U);
#endif

    /* Switch COMPUTE_MAIN_CLK selector to FRO0.FRO_MAX_VDD2_COMP_CLK */
    CLOCK_AttachClk(kFRO0_DIV1_to_COMPUTE_MAIN);

    /* Move clock to FRO0_DIV1. Note, for A0, AHB clock frequency must >= Flash clock frequency(ERR052440). */
    BOARD_SetXspiClock(XSPI0, 2, 1);
}

void BOARD_ClockLPPostConfig(void)
{
#if defined(DEMO_PVT_ON_CPU_DSP) && (DEMO_PVT_ON_CPU_DSP != 0U)
    /* Enable FRO0 clock for DSP. */
    CLOCK_EnableFro0ClkForDomain(kCLOCK_Vdd2CompDomainEnable | kCLOCK_Vdd2ComDomainEnable | kCLOCK_VddnComDomainEnable |
                                 kCLOCK_Vdd2DspDomainEnable);
#endif

    SystemCoreClockUpdate();
}

void BOARD_ConfigPMICModes(pca9422_modecfg_t *cfg, pca9422_power_mode_t mode)
{
    assert(cfg);

    switch (mode)
    {
        case kPCA9422_ActiveModeDVS0:
        case kPCA9422_ActiveModeDVS1:
        case kPCA9422_ActiveModeDVS2:
        case kPCA9422_ActiveModeDVS3:
        case kPCA9422_ActiveModeDVS4:
        case kPCA9422_ActiveModeDVS5:
        case kPCA9422_ActiveModeDVS6:
        case kPCA9422_ActiveModeDVS7:
            cfg[mode].sw1OutVolt  = 1000000U; /* VDD2 */
            cfg[mode].sw2OutVolt  = 1100000U; /* VDDN */
            cfg[mode].sw3OutVolt  = 1000000U; /* VDD1 */
            cfg[mode].sw4OutVolt  = 1800000U;
            cfg[mode].ldo1OutVolt = 1800000U; /* 1V8 AO */
            cfg[mode].ldo2OutVolt = 1800000U;
            cfg[mode].ldo3OutVolt = 1200000U;
            cfg[mode].ldo4OutVolt = 3300000U;
            break;

        /* PMIC_MODE[1:0] = 01b */
        case kPCA9422_SleepMode:
            cfg[mode].sw1OutVolt  = 500000U;
            cfg[mode].sw2OutVolt  = 600000U;
            cfg[mode].sw3OutVolt  = 500000U;
            cfg[mode].sw4OutVolt  = 1800000U;
            cfg[mode].ldo1OutVolt = 1800000U;
            cfg[mode].ldo2OutVolt = 1800000U;
            cfg[mode].ldo3OutVolt = 1200000U;
            cfg[mode].ldo4OutVolt = 3300000U;
            break;

            /* Note: the StandbyMode and DPStandbyMode use same register for voltage configuration. */
        case kPCA9422_StandbyMode:   /* PMIC_MODE[1:0] = 10b */
        case kPCA9422_DPStandbyMode: /* PMIC_MODE[1:0] = 11b */
            cfg[mode].sw1OutVolt  = 500000U;
            cfg[mode].sw2OutVolt  = 600000U;
            cfg[mode].sw3OutVolt  = 500000U;
            cfg[mode].sw4OutVolt  = 1800000U;
            cfg[mode].ldo1OutVolt = 1800000U;
            cfg[mode].ldo2OutVolt = 1800000U;
            cfg[mode].ldo3OutVolt = 1200000U;
            cfg[mode].ldo4OutVolt = 3300000U;
            break;

        default:
            break;
    }
}

/* Configure regulator output enable in Run mode. */
void BOARD_ConfigPMICRegEnable(pca9422_handle_t *handle)
{
    pca9422_regulatoren_t cfg;

    /* Configure Regulator Enable */
    PCA9422_GetDefaultRegEnableConfig(&cfg);

    /* All regulators enable in RUN state. */
    cfg.sw1Enable  = true;
    cfg.sw2Enable  = true;
    cfg.sw3Enable  = true;
    cfg.sw4Enable  = true;
    cfg.ldo1Enable = true;
    cfg.ldo2Enable = true;
    cfg.ldo3Enable = true;
    cfg.ldo4Enable = true;

    PCA9422_WriteRegEnableConfig(handle, cfg);
}

void BOARD_ConfigPMICEnMode(pca9422_handle_t *handle)
{
    pca9422_enmodecfg_t cfg;
    /* Configure ENMODE */
    PCA9422_GetDefaultEnModeConfig(&cfg);

    cfg.sw1OutEnMode  = kPCA9422_EnmodeOnActiveSleep;
    cfg.sw3OutEnMode  = kPCA9422_EnmodeOnActiveSleep;
    cfg.sw2OutEnMode  = kPCA9422_EnmodeOnActiveSleep;
    cfg.sw4OutEnMode  = kPCA9422_EnmodeOnActiveSleepStandby;
    cfg.ldo1OutEnMode = kPCA9422_EnmodeOnAll;
    cfg.ldo2OutEnMode = kPCA9422_EnmodeOnActiveSleepStandby;
    cfg.ldo3OutEnMode = kPCA9422_EnmodeOnActiveSleep;
    cfg.ldo4OutEnMode = kPCA9422_EnmodeOnActiveSleep;

    PCA9422_WriteEnModeConfig(handle, cfg);
}

void BOARD_RestorePeripheralsAfterDSR(void)
{
    DEMO_InitDebugConsole();
}

void BOARD_WaitCPU1Booted(void)
{
    RESET_ClearPeripheralReset(kMU1_RST_SHIFT_RSTn);
    MU_Init(APP_MU);

    /* Wait Core 1 is Boot Up */
    while (BOOT_FLAG != MU_GetFlags(APP_MU))
    {
    }
}

void APP_BootCore1(void)
{
    BOARD_ReleaseCore1Power();
    BOARD_InitAHBSC();
#ifdef CORE1_IMAGE_COPY_TO_RAM
    BOARD_CopyCore1Image(CORE1_BOOT_ADDRESS);
#endif
    BOARD_BootCore1(CORE1_BOOT_ADDRESS, CORE1_BOOT_ADDRESS);
    BOARD_WaitCPU1Booted();
}

void BOARD_BootDSP(void)
{
#if (DEMO_MAINCLK_FREQ == DEMO_MAINCLK_FREQ_SP0)
    BOARD_DSP_Init(2U, 2U, true); /* Select FRO0_MAX divider by 2 for HIFI clock. */
#else
    /* Boot HIFI4. */
    BOARD_DSP_Init(2U, 1U, true); /* Select FRO0_MAX divider by 1 for HIFI clock. */
#endif
}

void BOARD_PowerInitPMIC(void)
{
    pca9422_modecfg_t pca9422ModeCfg[12];
    uint32_t i;

    BOARD_InitPmic();
    for (i = 0; i < ARRAY_SIZE(pca9422ModeCfg); i++)
    {
        PCA9422_GetDefaultPowerModeConfig(&pca9422ModeCfg[i]);
    }

    for (i = 0; i < ARRAY_SIZE(pca9422ModeCfg); i++)
    {
        BOARD_ConfigPMICModes(pca9422ModeCfg, (pca9422_power_mode_t)i);
        PCA9422_WritePowerModeConfigs(&pca9422Handle, (pca9422_power_mode_t)i, pca9422ModeCfg[i]);
    }
    BOARD_ConfigPMICRegEnable(&pca9422Handle);
    BOARD_ConfigPMICEnMode(&pca9422Handle);

    /* Switch to a new DVS mode before re-configuring the VDD1/VDD2 per CPU frequency. */
    BOARD_SetPmicDVSPinStatus(0x1);
}

void BOARD_InitPowerConfig(void)
{
    /* BE CAUTIOUS TO SET CORRECT VOLTAGE RANGE ACCORDING TO YOUR BOARD/APPLICATION. PAD SUPPLY BEYOND THE RANGE DO
       HARM TO THE SILICON. */
    POWER_SetPio2VoltRange(kPadVol_300_360);

    /* Disable PLL. */
    CLKCTL2->MAINPLL0PFDDOMAINEN  = 0;
    CLKCTL2->AUDIOPLL0PFDDOMAINEN = 0;
    /* Disable PLL. */
    CLOCK_DeinitMainPll();
    CLOCK_DeinitAudioPll();

    /* Disable the clock for unused modules. */
    CLOCK_DisableClock(kCLOCK_Mmu0);
    CLOCK_DisableClock(kCLOCK_Mmu1);
    CLOCK_DisableClock(kCLOCK_Pkc);
    CLOCK_DisableClock(kCLOCK_PkcRam);
    CLOCK_DisableClock(kCLOCK_Syspm0);
    CLOCK_DisableClock(kCLOCK_Syspm1);
    CLOCK_DisableClock(kCLOCK_PrinceExe);
    CLOCK_DisableClock(kCLOCK_Prince0);
    CLOCK_DisableClock(kCLOCK_Prince1);
    CLOCK_DisableClock(kCLOCK_Iopctl0);
    CLOCK_DisableClock(kCLOCK_Ocotp0);
    CLOCK_DisableClock(kCLOCK_Cdog0); /* B0 default enabled. */
    CLOCK_DisableClock(kCLOCK_Glikey3);
    CLOCK_DisableClock(kCLOCK_Glikey4);
    CLOCK_DisableClock(kCLOCK_Glikey5);
    CLOCK_DisableClock(kCLOCK_Hifi4AccessRamArbiter1);
    CLOCK_DisableClock(kCLOCK_MediaAccessRamArbiter0);
    CLOCK_DisableClock(kCLOCK_MediaAccessRamArbiter1);
    CLOCK_DisableClock(kCLOCK_Hifi4);
    CLOCK_DisableClock(kCLOCK_Romcp);

    CLOCK_AttachClk(kNONE_to_DSP);
    CLOCK_AttachClk(kNONE_to_TPIU); /* Default is gated. */
    CLOCK_AttachClk(kNONE_to_FCCLK1);
    CLOCK_AttachClk(kNONE_to_FCCLK2);
    CLOCK_AttachClk(kNONE_to_FCCLK3);
    CLOCK_AttachClk(kNONE_to_TRNG);
    CLOCK_AttachClk(kNONE_to_SDIO0);
    CLOCK_AttachClk(kNONE_to_SDIO1);

    /* Keep the used resources on. */
    BOARD_EnableSharedSenseMainClk();     /* Keep Sense shared parts clock on. */
    POWER_DisablePD(kPDRUNCFG_GATE_FRO0); /* Just use PD bit to control FRO. */
    POWER_DisablePD(kPDRUNCFG_GATE_FRO2);
    POWER_DisablePD(kPDRUNCFG_SHUT_RAM1_CLK);
    POWER_ApplyPD();
    POWER_DisableLPRequestMask(kPower_MaskAll); /* Let's compute control all the shared resources. */
}

static void BOARD_DisableCache(CACHE64_CTRL_Type *base)
{
    if ((base->CCR & CACHE64_CTRL_CCR_ENCACHE_MASK) == CACHE64_CTRL_CCR_ENCACHE_MASK)
    {
        /* First, push any modified contents. */
        base->CCR |= CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK | CACHE64_CTRL_CCR_GO_MASK;

        /* Wait until the cache command completes. */
        while ((base->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)
        {
        }

        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        base->CCR &= ~(CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK);

        /* Now disable the cache. */
        base->CCR &= ~CACHE64_CTRL_CCR_ENCACHE_MASK;
    }
}

static inline void BOARD_ConfigSupplySetpoints(void)
{
    /* The LVD need correctly configured even using PMIC supply. */
    status_t ret = kStatus_Success;

    power_lvd_voltage_t dcdcLvd = {
      .VDDN.lvl1 = 900000U,
      .VDDN.lvl0 = 500000U,
    };

    ret = POWER_ConfigLvdSetpoints(kRegulator_DCDC, &dcdcLvd);
    if (ret != kStatus_Success)
    {
        PRINTF("VDDNLVD configuration failed %d\r\n", ret);
    }

    power_lvd_voltage_t vdd2Lvd = {
      .VDD12.lvl3 = 800000U,
      .VDD12.lvl2 = 700000U,
      .VDD12.lvl1 = 600000U,
      .VDD12.lvl0 = 500000U,
    };

    ret = POWER_ConfigLvdSetpoints(kRegulator_Vdd2LDO, &vdd2Lvd);
    if (kStatus_Success != ret)
    {
        PRINTF("VDD2LVD configuration failed %d\r\n", ret);
    }

    POWER_SelectRunSetpoint(kRegulator_Vdd2LDO, 1U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_SelectRunSetpoint(kRegulator_DCDC, 1U);
    POWER_SelectSleepSetpoint(kRegulator_DCDC, 0U);
    POWER_SelectRunSetpoint(kRegulator_Vdd1LDO, 1U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd1LDO, 0U);

    POWER_ApplyPD();
}


void BOARD_PowerConfigAfterCPU1Booted(void)
{
    /* Turn off unused resources. */
    CLOCK_DisableClock(kCLOCK_Glikey0);
    CLOCK_DisableClock(kCLOCK_Glikey1);
    CLOCK_DisableClock(kCLOCK_Glikey2);
    CLOCK_DisableClock(kCLOCK_CompAccessRamArbiter1);
    CLKCTL0->RAMCLKSEL = 0;          /* Sense access RAM arbiter0 clock. */
    CLOCK_DisableClock(kCLOCK_Cpu1); /*Let CPU1 control it's clock. */

#if (DEMO_POWER_ENABLE_DEBUG == 0U)
    CLOCK_DisableClock(kCLOCK_Dbg);
#endif

    if (!IS_XIP_XSPI0())
    {
        BOARD_DisableCache(CACHE64_CTRL0);
        CLOCK_DisableClock(kCLOCK_Cache64ctrl0);
        CLOCK_DisableClock(kCLOCK_Xspi0);
        CLOCK_AttachClk(kNONE_to_XSPI0);
        POWER_EnablePD(kPDRUNCFG_APD_XSPI0);
        POWER_EnablePD(kPDRUNCFG_PPD_XSPI0);
        POWER_ApplyPD();
    }

    if (!IS_XIP_XSPI1())
    {
        BOARD_DisableCache(CACHE64_CTRL1);
        CLOCK_DisableClock(kCLOCK_Cache64ctrl1);
        CLOCK_DisableClock(kCLOCK_Xspi1);
        CLOCK_AttachClk(kNONE_to_XSPI1);
        POWER_EnablePD(kPDRUNCFG_APD_XSPI1);
        POWER_EnablePD(kPDRUNCFG_PPD_XSPI1);
        POWER_ApplyPD();
    }
    POWER_EnablePD(kPDRUNCFG_PD_ADC0);
    POWER_EnablePD(kPDRUNCFG_SHUT_MEDIA_MAINCLK);
    POWER_EnablePD(kPDRUNCFG_SHUT_RAM1_CLK);

    POWER_EnablePD(kPDRUNCFG_APD_XSPI2);
    POWER_EnablePD(kPDRUNCFG_PPD_XSPI2);
    POWER_EnablePD(kPDRUNCFG_APD_DMA0_1_PKC_ETF);
    POWER_EnablePD(kPDRUNCFG_PPD_DMA0_1_PKC_ETF);
    POWER_EnablePD(kPDRUNCFG_APD_USB0_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_USB0_SRAM);
    POWER_EnablePD(kPDRUNCFG_APD_USB1_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_USB1_SRAM);
    POWER_EnablePD(kPDRUNCFG_APD_SDHC0_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_SDHC0_SRAM);
    POWER_EnablePD(kPDRUNCFG_APD_SDHC1_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_SDHC1_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_OCOTP);

    SYSCON0->COMP_AUTOGATE_EN = 0x7U; /* MBUS_EN bit disabled to allow other master accessing RAM0. */

    POWER_EnablePD(kPDRUNCFG_DSR_VDD2N_MEDIA);
    POWER_ApplyPD();

    PMC0->PDRUNCFG1 = 0x7FFFFFFFU; /* Power down ROM, Power down or set low-power mode for HVD, LVD, GDET. 0x4020F0A4 */

    POWER_EnablePD(kPDRUNCFG_SHUT_SENSEP_MAINCLK); /* Let Sense control private parts clock. */
    POWER_EnablePD(kPDRUNCFG_PD_FRO1); /* Note: Sense boots using FRO1 and switchs to FRO2(Sense can't control FRO1). */

    POWER_EnablePD(kPDRUNCFG_SHUT_RAM0_CLK);                        /* Sense access RAM arbiter0 clock. */
    POWER_EnablePD(kPDRUNCFG_SHUT_RAM1_CLK);                        /* Compute access RAM arbiter1 clock. */

    POWER_EnableRunAFBB(kPower_BodyBiasVdd2 | kPower_BodyBiasVddn); /* Configure VDD2 AFBB mode during active.*/
    POWER_EnableRunNBB(kPower_BodyBiasVdd2Sram);
    POWER_EnableRunRBB(kPower_BodyBiasVdd1 | kPower_BodyBiasVdd1Sram);
    POWER_EnableSleepRBB(kPower_BodyBiasVddn | kPower_BodyBiasVdd2Sram | kPower_BodyBiasVdd2 | kPower_BodyBiasVdd1 |
                         kPower_BodyBiasVdd1Sram);

    BOARD_ConfigSupplySetpoints();

    BOARD_SetPmicVdd1Voltage(POWER_CalcVoltLevel(kRegulator_Vdd2LDO, SystemCoreClock, 0U));
#if 0
    BOARD_SetPmicVdd1Voltage(
        POWER_CalcVoltLevel(kRegulator_Vdd1LDO, DEMO_SENSE_M33_CPU_CLOCK_FREQ, 0U)); /* CPU1 frequency 32MHZ. */
#else
    BOARD_SetPmicVdd1Voltage(DEMO_SENSE_MIN_VDD1_VOLT);  /* Sense shared main clock is running, add some margin for VDD1. */
#endif
}

void DEMO_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;

    CLOCK_AttachClk(kFRO0_DIV1_to_FCCLK0);
    CLOCK_SetClkDiv(kCLOCK_DivFcclk0Clk, 10U);

    /* Attach FC0 clock to LP_FLEXCOMM (debug console) */
    CLOCK_AttachClk(kFCCLK0_to_FLEXCOMM0);

    uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

void DEMO_DeinitDebugConsole(void)
{
    DbgConsole_Deinit();
    CLOCK_AttachClk(kNONE_to_FCCLK0);
}

void BOARD_DisableIoPads()
{
    uint8_t port, pin;

    IOPCTL_PinMuxSet(4U, 11U, 0U);

    if (IS_XIP_XSPI0() == 0U)
    {
        port = 6U;
        for (pin = 0U; pin <= 12U; pin++)
        {
            IOPCTL_PinMuxSet(port, pin, 0U);
        }
    }

    if (IS_XIP_XSPI1() == 0U)
    {
        port = 5U;
        for (pin = 0U; pin <= 20U; pin++)
        {
            IOPCTL_PinMuxSet(port, pin, 0U);
        }
    }
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_DisableIoPads();
    BOARD_InitPins();

    /* PMIC is used. */
    BOARD_PowerInitPMIC();
    POWER_SetVddnSupplySrc(kVddSrc_PMIC);
    POWER_SetVdd1SupplySrc(kVddSrc_PMIC);
    POWER_SetVdd2SupplySrc(kVddSrc_PMIC);
    POWER_DisableRegulators(kPower_SCPC);
    POWER_SetRunRegulatorMode(kRegulator_DCDC, kPower_DCDCMode_ULP);
    POWER_SetSleepRegulatorMode(kRegulator_DCDC, kPower_DCDCMode_ULP);
#if (MAX_VDDCORE > 1000000U)
    BOARD_SetPmicVdd2Voltage(MAX_VDDCORE);
#endif

    BOARD_BootClockRUN();

    DEMO_InitDebugConsole();

    clock_osc32k_config_t config = {
        .bypass = false, .monitorEnable = false, .lowPowerMode = true, .cap = kCLOCK_Osc32kCapPf16};
    /* Configure OSC32K. */
    CLOCK_EnableOsc32K(&config);

    CLOCK_AttachClk(kOSC32K_to_32K_WAKE);
    CLOCK_AttachClk(k32KHZ_WAKE_to_OSTIMER);
    CLOCK_SetClkDiv(kCLOCK_DivOstimerClk, 1U);
}

/*! Disable clock for modules for cpu run only or sleep. */
static inline void BOARD_DisableClocks(void)
{
    CLOCK_DisableClock(kCLOCK_LPI2c15);

    CLOCK_DisableClock(kCLOCK_Gpio7);
    CLOCK_DisableClock(kCLOCK_Mu1);
    CLOCK_DisableClock(kCLOCK_Syscon0);
    CLOCK_DisableClock(kCLOCK_Syscon3);
    CLOCK_DisableClock(kCLOCK_Iopctl0);
    CLOCK_DisableClock(kCLOCK_Iopctl1);
    CLOCK_DisableClock(kCLOCK_Sema420);

    if (!IS_XIP_XSPI0() && !IS_XIP_XSPI1())
    {
#if defined(DEMO_PVT_ON_CPU_DSP) && (DEMO_PVT_ON_CPU_DSP != 0U)
        CLOCK_EnableFro0ClkForDomain(kCLOCK_Vdd2CompDomainEnable | kCLOCK_Vdd2DspDomainEnable);
#else
        CLOCK_EnableFro0ClkForDomain(kCLOCK_Vdd2CompDomainEnable);
#endif
    }

    CLOCK_EnableFroClkOutput(FRO0, kCLOCK_FroDiv1OutEn);
}

static inline void BOARD_RestoreClocks(void)
{
    /* Restore clock, power for used modules. */
    CLOCK_EnableFroClkOutput(FRO0, kCLOCK_FroDiv1OutEn | kCLOCK_FroDiv3OutEn | kCLOCK_FroDiv6OutEn);
#if defined(DEMO_PVT_ON_CPU_DSP) && (DEMO_PVT_ON_CPU_DSP != 0U)
    CLOCK_EnableFro0ClkForDomain(kCLOCK_Vdd2CompDomainEnable | kCLOCK_VddnComDomainEnable | kCLOCK_Vdd2ComDomainEnable |
                                 kCLOCK_Vdd2DspDomainEnable);
#else
    CLOCK_EnableFro0ClkForDomain(kCLOCK_Vdd2CompDomainEnable | kCLOCK_VddnComDomainEnable);
#endif
    CLOCK_EnableClock(kCLOCK_Syscon0);
    CLOCK_EnableClock(kCLOCK_Syscon3);
    CLOCK_EnableClock(kCLOCK_Iopctl0);
    CLOCK_EnableClock(kCLOCK_Iopctl1);
    CLOCK_EnableClock(kCLOCK_Mu1);
    CLOCK_EnableClock(kCLOCK_Sema420);
    CLOCK_EnableClock(kCLOCK_Gpio7);
    CLOCK_EnableClock(kCLOCK_LPI2c15);
}

void BOARD_EnterSleep(void)
{
    uint32_t irqMask;

    BOARD_PMIC_I2C_Deinit();
    CLOCK_AttachClk(kNONE_to_LPI2C15);
    DEMO_DeinitDebugConsole();
    CLOCK_AttachClk(kNONE_to_FCCLK0);

    BOARD_DisableClocks();

    if (!IS_XIP_XSPI0() && !IS_XIP_XSPI1())
    {
        /* XSPI0 and XSPI1 memory interface located in VDDN_COM. */
        /* PMIC DVS_CTRL pins are located in VDDN, if powered down, the warm reset will not able to reset 
           the PMIC to DVS0 mode which with safe power supply. So the warm reset will fail to reset the board. */
        /* POWER_EnablePD(kPDRUNCFG_DSR_VDDN_COM); */
        /* POWER_EnableRunRBB(kPower_BodyBiasVddn); */
        POWER_EnablePD(kPDRUNCFG_SHUT_COMNN_MAINCLK);
        POWER_ApplyPD();
    }

    irqMask = DisableGlobalIRQ();

    POWER_EnablePD(kPDRUNCFG_PD_LPOSC);
    BOARD_DisableSharedSenseMainClk();

    POWER_EnterSleep();

    POWER_DisablePD(kPDRUNCFG_PD_LPOSC);
    BOARD_EnableSharedSenseMainClk();
    EnableGlobalIRQ(irqMask);
    __ISB();

    if (!IS_XIP_XSPI0() && !IS_XIP_XSPI1())
    {
        /* XSPI0 and XSPI1 memory interface located in VDDN_COM. */
        /* POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM); */
        /* POWER_EnableRunAFBB(kPower_BodyBiasVddn); */
        POWER_DisablePD(kPDRUNCFG_SHUT_COMNN_MAINCLK);
        POWER_ApplyPD();
    }

    BOARD_RestoreClocks();
    DEMO_InitDebugConsole();
    CLOCK_AttachClk(kSENSE_BASE_to_LPI2C15);
    BOARD_PMIC_I2C_Init();
}

void BOARD_EnterDeepSleep(const uint32_t exclude_from_pd[7])
{
    POWER_EnterDeepSleep(exclude_from_pd);
}

/* Init the wait timer used for PVT task to periodic check whether the supply can go lower. */
void pvt_wait_timer_init(void)
{
    CLOCK_AttachClk(kLPOSC_to_UTICK0_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivUtick0Clk, 1);
    UTICK_Init(UTICK0);

    NVIC_ClearPendingIRQ((IRQn_Type)UTICK0_IRQn);
    NVIC_SetPriority((IRQn_Type)UTICK0_IRQn, 4U);
}

void pvt_wait_timer_start(uint32_t ms, utick_callback_t cb)
{
    UTICK_SetTick(UTICK0, kUTICK_Onetime, (ms * 1000U) - 1U, cb);
}
/*${function:end}*/
