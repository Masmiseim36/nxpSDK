/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lpm.h"
#include "fsl_gpc.h"
#include "fsl_dcdc.h"
#include "fsl_gpt.h"
#include "specific.h"

#include "power_mode_switch_bm.h"
#include "clock_config.h"

#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LPM_GPC_IMR_NUM (sizeof(GPC->IMR) / sizeof(GPC->IMR[0]))

typedef struct _lpm_clock_context
{
    uint32_t armDiv;
    uint32_t ahbDiv;
    uint32_t ipgDiv;
    uint32_t perDiv;
    uint32_t perSel;
    uint32_t periphSel;
    uint32_t preperiphSel;
    uint32_t pfd480;
    uint32_t pfd528;
    uint32_t pllArm_loopdiv;
    uint32_t pllArm;
    uint32_t pllSys;
    uint32_t pllUsb1;
    uint32_t pllUsb2;
    uint32_t pllAudio;
    uint32_t pllVideo;
    uint32_t pllEnet;
} lpm_clock_context_t;

typedef void (*lpm_system_func_t)(uint32_t context);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static lpm_power_mode_t s_curMode;
static lpm_clock_context_t s_clockContext;
volatile static uint32_t s_DllBackupValue  = 0;
volatile static uint32_t s_OverRunFlag     = 0;
volatile static uint32_t s_LowPowerRunFlag = 0;
volatile static uint32_t s_ClearLPFlag     = 0;
static uint32_t g_savedPrimask;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void LPM_SetClockMode(clock_mode_t mode, uint32_t clpcr)
{
    switch (mode)
    {
        case kCLOCK_ModeRun:
            CCM->CLPCR = clpcr;
            break;
        default:
            /*
             * ERR007265: CCM: When improper low-power sequence is used,
             * the SoC enters low power mode before the ARM core executes WFI.
             *
             * Software workaround:
             * 1) Software should trigger IRQ #41 (GPR_IRQ) to be always pending
             *    by setting IOMUXC_GPR_GPR1_GINT.
             * 2) Software should then unmask IRQ #41 in GPC before setting CCM
             *    Low-Power mode.
             * 3) Software should mask IRQ #41 right after CCM Low-Power mode
             *    is set (set bits 0-1 of CCM_CLPCR).
             *
             */
            LPM_EnableWakeupSource(GPR_IRQ_IRQn);
            CCM->CLPCR = clpcr;
            LPM_DisableWakeupSource(GPR_IRQ_IRQn);
            break;
    }
}

void LPM_SwitchToXtalOSC(void)
{
    /* Restore XTAL-OSC and enable detector */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_XTAL_24M_PWD_MASK; /* Power up */
    while ((XTALOSC24M->LOWPWR_CTRL & XTALOSC24M_LOWPWR_CTRL_XTALOSC_PWRUP_STAT_MASK) == 0)
    {
    }
    CCM_ANALOG->MISC0_SET = CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK; /* detect freq */
    while ((CCM_ANALOG->MISC0 & CCM_ANALOG_MISC0_OSC_XTALOK_MASK) == 0)
    {
    }
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK;

    /* Switch to XTAL-OSC */
    XTALOSC24M->LOWPWR_CTRL_CLR = XTALOSC24M_LOWPWR_CTRL_CLR_OSC_SEL_MASK;
    /* Turn off XTAL-OSC detector */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK;

    /* Wait CCM operation finishes */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* Take some delay */
    LPM_DELAY(40);
}

void LPM_SwitchToRcOSC(void)
{
    /* Switch to RC-OSC */
    XTALOSC24M->LOWPWR_CTRL_SET = XTALOSC24M_LOWPWR_CTRL_SET_OSC_SEL_MASK;
    /* Turn off XTAL-OSC */
    CCM_ANALOG->MISC0_SET = CCM_ANALOG_MISC0_XTAL_24M_PWD_MASK; /* Power down */

    /* Wait CCM operation finishes */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* Take some delay */
    LPM_DELAY(40);
}

void LPM_SwitchFlexspiClock(lpm_power_mode_t power_mode)
{
    while (!((Flash_Type->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (Flash_Type->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    {
        ;
    }
    Flash_Type->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;

    if (FLEXSPI == Flash_Type)
    {
        /* Disable clock gate of flexspi. */
        CCM->CCGR6 &= (~CCM_CCGR6_CG5_MASK);

        /* Semc_clk_root_pre will be used as flexspi clock. */
        CLOCK_SET_MUX((uint32_t)kCLOCK_FlexspiMux, 0x0);
        /* Set divider for flexspi clock root 0. */
        CLOCK_SET_DIV((uint32_t)kCLOCK_FlexspiDiv, 0x0);

        /* Enable clock gate of flexspi. */
        CCM->CCGR6 |= (CCM_CCGR6_CG5_MASK);
    }
    else
    {
        SwitchFlexspi2Clock();
    }

    Flash_Type->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
    Flash_Type->MCR0 |= FLEXSPI_MCR0_SWRESET_MASK;
    while (Flash_Type->MCR0 & FLEXSPI_MCR0_SWRESET_MASK)
    {
    }
    while (!((Flash_Type->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (Flash_Type->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    {
        ;
    }

    /* Take some delay */
    LPM_DELAY(40);
}

void LPM_RestoreFlexspiClock(void)
{
    while (!((Flash_Type->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (Flash_Type->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    {
        ;
    }
    Flash_Type->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;

    if (FLEXSPI == Flash_Type)
    {
        /* Disable clock gate of flexspi. */
        CCM->CCGR6 &= (~CCM_CCGR6_CG5_MASK);

        /* Set divider for flexspi clock root 0. */
        CLOCK_SET_DIV((uint32_t)kCLOCK_FlexspiDiv, 0x1);
        /* PLL3 PFD0 will be used as flexspi clock. */
        CLOCK_SET_MUX((uint32_t)kCLOCK_FlexspiMux, 0x3);

        /* Enable clock gate of flexspi. */
        CCM->CCGR6 |= (CCM_CCGR6_CG5_MASK);
    }
    else
    {
        RestoreFlexspi2Clock();
    }

    Flash_Type->DLLCR[0] = s_DllBackupValue;
    Flash_Type->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
    Flash_Type->MCR0 |= FLEXSPI_MCR0_SWRESET_MASK;
    while (Flash_Type->MCR0 & FLEXSPI_MCR0_SWRESET_MASK)
    {
    }
    while (!((Flash_Type->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (Flash_Type->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    {
        ;
    }
    /* Take some delay */
    LPM_DELAY(40);
}

void LPM_SwitchBandgap(void)
{
    /* Switch bandgap */
    PMU->MISC0_SET              = CCM_ANALOG_MISC0_REFTOP_SELFBIASOFF_MASK >> 1;
    XTALOSC24M->LOWPWR_CTRL_SET = XTALOSC24M_LOWPWR_CTRL_LPBG_SEL_MASK;
    PMU->MISC0_SET              = CCM_ANALOG_MISC0_REFTOP_PWD_MASK;

    /* Wait CCM operation finishes */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* Take some delay */
    LPM_DELAY(40);
}

void LPM_RestoreBandgap(void)
{
    /* Restore bandgap */
    /* Turn on regular bandgap and wait for stable */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_REFTOP_PWD_MASK;
    while ((CCM_ANALOG->MISC0 & CCM_ANALOG_MISC0_REFTOP_VBGUP_MASK) == 0)
    {
    }
    /* Low power band gap disable */
    XTALOSC24M->LOWPWR_CTRL_CLR = XTALOSC24M_LOWPWR_CTRL_LPBG_SEL_MASK;
    PMU->MISC0_CLR              = CCM_ANALOG_MISC0_REFTOP_SELFBIASOFF_MASK >> 1;

    /* Wait CCM operation finishes */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* Take some delay */
    LPM_DELAY(40);
}

void LPM_DisableAndSwitchPLLs(lpm_power_mode_t power_mode)
{
    /* Application shouldn't rely on PLL in low power mode,
     * gate all PLL and PFD now */
    if (LPM_PowerModeSuspend == power_mode)
    {
        return;
    }

    s_clockContext.pfd480       = CCM_ANALOG->PFD_480;
    s_clockContext.pfd528       = CCM_ANALOG->PFD_528;
    s_clockContext.pllSys       = CCM_ANALOG->PLL_SYS;
    s_clockContext.pllUsb1      = CCM_ANALOG->PLL_USB1;
    s_clockContext.pllAudio     = CCM_ANALOG->PLL_AUDIO;
    s_clockContext.pllEnet      = CCM_ANALOG->PLL_ENET;
    s_clockContext.periphSel    = CLOCK_GetMux(kCLOCK_PeriphMux);
    s_clockContext.ipgDiv       = CLOCK_GetDiv(kCLOCK_IpgDiv);
    s_clockContext.ahbDiv       = CLOCK_GetDiv(kCLOCK_AhbDiv);
    s_clockContext.perSel       = CLOCK_GetMux(kCLOCK_PerclkMux);
    s_clockContext.perDiv       = CLOCK_GetDiv(kCLOCK_PerclkDiv);
    s_clockContext.preperiphSel = CLOCK_GetMux(kCLOCK_PrePeriphMux);
    s_clockContext.armDiv       = CLOCK_GetDiv(kCLOCK_ArmDiv);

    /* When need to close PLL, we need to close PLL that we do not used. */
#ifndef XIP_EXTERNAL_FLASH
    /* When need to close PLL, we need to use bypass clock first and then power it down. */
    /* Power off SYS PLL */
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_BYPASS_MASK;
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;
    /* If XIP in hyper flash, should switch to ARM PLL before disble USB1 PLL */
    /* Power off USB1 PLL */
    CCM_ANALOG->PLL_USB1_SET = CCM_ANALOG_PLL_USB1_BYPASS_MASK;
    CCM_ANALOG->PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_POWER_MASK;
    CCM_ANALOG->PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;
    CCM_ANALOG->PFD_480_SET  = CCM_ANALOG_PFD_480_PFD0_CLKGATE_MASK;
#endif
    /* Power off AUDIO PLL */
    CCM_ANALOG->PLL_AUDIO_SET = CCM_ANALOG_PLL_AUDIO_BYPASS_MASK;
    CCM_ANALOG->PLL_AUDIO_SET = CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK;
    /* Power off ENET PLL */
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_BYPASS_MASK;
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_POWERDOWN_MASK;

    if (LPM_PowerModeLowPowerRun == power_mode)
    {
        SwitchToLowPowerRunPLLs();
    }
    else if (LPM_PowerModeLPIdle == power_mode)
    {
        SwitchToLPIdlPLLs();
    }
    else
    {
        /* Direct return from RUN and Suspend */
        return;
    }

#if defined(XIP_EXTERNAL_FLASH)
    LPM_EnterCritical();
    LPM_SwitchFlexspiClock(power_mode);
    LPM_ExitCritical();

    /* Power off USB1 PLL */
    CCM_ANALOG->PLL_USB1_SET = CCM_ANALOG_PLL_USB1_BYPASS_MASK;
    CCM_ANALOG->PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_POWER_MASK;
    CCM_ANALOG->PFD_480_SET  = CCM_ANALOG_PFD_480_PFD0_CLKGATE_MASK;
#endif
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd0);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd1);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd2);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd3);

    CLOCK_DeinitSysPfd(kCLOCK_Pfd0);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd1);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd2);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd3);
}

void LPM_RestorePLLs(lpm_power_mode_t power_mode)
{
    /* Restore USB1 PLL */
    CCM_ANALOG->PLL_USB1_SET = CCM_ANALOG_PLL_USB1_BYPASS_MASK;
    CCM_ANALOG->PLL_USB1 =
        (s_clockContext.pllUsb1 & (~(CCM_ANALOG_PLL_USB1_POWER_MASK))) | CCM_ANALOG_PLL_USB1_BYPASS_MASK;
    CCM_ANALOG->PLL_USB1_SET = CCM_ANALOG_PLL_USB1_POWER_MASK;
    CCM_ANALOG->PFD_480_CLR  = CCM_ANALOG_PFD_480_PFD0_FRAC_MASK;
    CCM_ANALOG->PFD_480_SET  = CCM_ANALOG_PFD_480_PFD0_FRAC(35);
    CCM_ANALOG->PFD_480_CLR  = CCM_ANALOG_PFD_480_PFD0_CLKGATE_MASK;
    CCM_ANALOG->PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_BYPASS_MASK;

    if ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_POWER_MASK) != 0)
    {
        while ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_LOCK_MASK) == 0)
        {
        }
    }
    /* Wait CCM operation finishes */
    while (CCM->CDHIPR != 0)
    {
    }
#if defined(XIP_EXTERNAL_FLASH)
    LPM_EnterCritical();
    LPM_RestoreFlexspiClock();
    LPM_ExitCritical();
#endif

    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 1);
    CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0);
    CLOCK_SetMux(kCLOCK_PeriphMux, 1);
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x1);
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0);

    /* Restore AHB and IPG div */
    CCM->CBCDR = (CCM->CBCDR & ~(CCM_CBCDR_AHB_PODF_MASK | CCM_CBCDR_IPG_PODF_MASK | CCM_CBCDR_PERIPH_CLK_SEL_MASK)) |
                 CCM_CBCDR_AHB_PODF(s_clockContext.ahbDiv) | CCM_CBCDR_IPG_PODF(s_clockContext.ipgDiv) |
                 CCM_CBCDR_PERIPH_CLK_SEL(s_clockContext.periphSel);

    /* When need to enable PLL, we need to use bypass clock first and then switch pll back. */
    /* Restore SYS PLL PFD */
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_BYPASS_MASK;
    CCM_ANALOG->PLL_SYS_CLR = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;
    CCM_ANALOG->PLL_SYS     = s_clockContext.pllSys;
    if ((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_POWERDOWN_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_LOCK_MASK) == 0)
        {
        }
    }
    CCM_ANALOG->PFD_528_CLR = CCM_ANALOG_PFD_528_PFD3_CLKGATE_MASK;

    /* Restore AUDIO PLL */
    CCM_ANALOG->PLL_AUDIO_SET = CCM_ANALOG_PLL_AUDIO_BYPASS_MASK;
    CCM_ANALOG->PLL_AUDIO_CLR = CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK;
    CCM_ANALOG->PLL_AUDIO     = s_clockContext.pllAudio;
    if ((CCM_ANALOG->PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_LOCK_MASK) == 0)
        {
        }
    }

    /* Restore ENET PLL */
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_BYPASS_MASK;
    CCM_ANALOG->PLL_ENET_CLR = CCM_ANALOG_PLL_ENET_POWERDOWN_MASK;
    CCM_ANALOG->PLL_ENET     = s_clockContext.pllEnet;
    if ((CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_POWERDOWN_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_LOCK_MASK) == 0)
        {
        }
    }

    /* Switch clocks back */
    CCM->CBCMR =
        (CCM->CBCMR & ~CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK) | CCM_CBCMR_PRE_PERIPH_CLK_SEL(s_clockContext.preperiphSel);

    /* Wait CCM operation finishes */
    while (CCM->CDHIPR != 0)
    {
    }

    /* Restore Periphral clock */
    CCM->CSCMR1 = (CCM->CSCMR1 & ~CCM_CSCMR1_PERCLK_PODF_MASK) | CCM_CSCMR1_PERCLK_PODF(s_clockContext.perDiv) |
                  CCM_CSCMR1_PERCLK_CLK_SEL(s_clockContext.perSel);
}

static void LPM_DisableRbcBypass(void)
{
    uint32_t gpcIMR[LPM_GPC_IMR_NUM];
    uint32_t i;

    /* Mask all GPC interrupts before disabling the RBC counters */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        gpcIMR[i]   = GPC->IMR[i];
        GPC->IMR[i] = 0xFFFFFFFFU;
    }

    /* Disable the RBC bypass counter */
    CCM->CCR &= ~CCM_CCR_RBC_EN_MASK;
    CCM->CCR &= ~CCM_CCR_REG_BYPASS_COUNT_MASK;

    /* Now delay for 2 CKIL cycles (61usec).
     * so a short loop should be enough.
     */
    for (i = 0; i < ARM_CORE_CURRENT_FREQ * 22; i++)
    {
        __NOP();
    }

    /* Recover all the GPC interrupts. */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = gpcIMR[i];
    }
}

static void LPM_SwitchSystemIdelPLLs(void)
{
    s_clockContext.pfd480       = CCM_ANALOG->PFD_480;
    s_clockContext.pfd528       = CCM_ANALOG->PFD_528;
    s_clockContext.pllSys       = CCM_ANALOG->PLL_SYS;
    s_clockContext.pllUsb1      = CCM_ANALOG->PLL_USB1;
    s_clockContext.pllAudio     = CCM_ANALOG->PLL_AUDIO;
    s_clockContext.pllEnet      = CCM_ANALOG->PLL_ENET;
    s_clockContext.periphSel    = CLOCK_GetMux(kCLOCK_PeriphMux);
    s_clockContext.ipgDiv       = CLOCK_GetDiv(kCLOCK_IpgDiv);
    s_clockContext.ahbDiv       = CLOCK_GetDiv(kCLOCK_AhbDiv);
    s_clockContext.perSel       = CLOCK_GetMux(kCLOCK_PerclkMux);
    s_clockContext.perDiv       = CLOCK_GetDiv(kCLOCK_PerclkDiv);
    s_clockContext.preperiphSel = CLOCK_GetMux(kCLOCK_PrePeriphMux);
    s_clockContext.armDiv       = CLOCK_GetDiv(kCLOCK_ArmDiv);

    /* When need to close PLL,
     * we need to close PLL that we do not used. */
#ifndef XIP_EXTERNAL_FLASH
    /* When need to close PLL, we need to use bypass clock first and then power it down. */
    /* Power off SYS PLL */
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_BYPASS_MASK;
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;
    /* If XIP in hyper flash, should switch to ARM PLL before disble USB1 PLL */
    /* Power off USB1 PLL */
    CCM_ANALOG->PLL_USB1_SET = CCM_ANALOG_PLL_USB1_BYPASS_MASK;
    CCM_ANALOG->PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_POWER_MASK;
    CCM_ANALOG->PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;
    CCM_ANALOG->PFD_480_SET  = CCM_ANALOG_PFD_480_PFD0_CLKGATE_MASK;
#endif
    /* Power off AUDIO PLL */
    CCM_ANALOG->PLL_AUDIO_SET = CCM_ANALOG_PLL_AUDIO_BYPASS_MASK;
    CCM_ANALOG->PLL_AUDIO_SET = CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK;
    /* Power off ENET PLL */
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_BYPASS_MASK;
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_POWERDOWN_MASK;

    SwitchToSystemIdlPLLs();

    /* Usb1_Pfd0 is used for flexspi clock */
    // CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd0);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd1);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd2);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd3);

    CLOCK_DeinitSysPfd(kCLOCK_Pfd0);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd1);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd2);
    // CLOCK_DeinitSysPfd(kCLOCK_Pfd3);
}

static void LPM_SetTargetVoltage()
{
    if (s_OverRunFlag == 1)
    {
        /* change the DCDC_LP to 1.275V first */
        DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);
        s_OverRunFlag = 0;
    }
    else if (s_LowPowerRunFlag == 1)
    {
        /* Adjust voltage to 0.95V */
        DCDC_AdjustTargetVoltage(DCDC, 0x6, 0x1);
        s_LowPowerRunFlag = 0;
    }
    else
    {
        /* Adjust voltage to 1.15V */
        DCDC_AdjustTargetVoltage(DCDC, 0xE, 0x1);
    }
}

static void LPM_EnterSystemIdle(void)
{
    LPM_SwitchSystemIdelPLLs();

    /* Enable regular 2P5 and turn off weak 2P5 */
    PMU->REG_2P5 |= PMU_REG_2P5_ENABLE_LINREG_MASK;
    PMU->REG_2P5 &= ~PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;
    /* Enable regular 1P1 and turn off weak 1P1 */
    PMU->REG_1P1 |= PMU_REG_1P1_ENABLE_LINREG_MASK;
    PMU->REG_1P1 &= ~PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;

    /* Adjust voltage to 1.15V */
    DCDC_AdjustTargetVoltage(DCDC, 0xE, 0x1);
}

static void LPM_SystemIdleRestore()
{
    /* Restore voltage setting before */
    LPM_SetTargetVoltage();

    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 1);
    CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0);
    CLOCK_SetMux(kCLOCK_PeriphMux, 1);
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x1);
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0);

    /* Restore AHB and IPG div */
    CCM->CBCDR = (CCM->CBCDR & ~(CCM_CBCDR_AHB_PODF_MASK | CCM_CBCDR_IPG_PODF_MASK | CCM_CBCDR_PERIPH_CLK_SEL_MASK)) |
                 CCM_CBCDR_AHB_PODF(s_clockContext.ahbDiv) | CCM_CBCDR_IPG_PODF(s_clockContext.ipgDiv) |
                 CCM_CBCDR_PERIPH_CLK_SEL(s_clockContext.periphSel);

    /* When need to enable PLL, we need to use bypass clock first and then switch pll back. */
    /* Restore SYS PLL PFD */
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_BYPASS_MASK;
    CCM_ANALOG->PLL_SYS_CLR = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;
    CCM_ANALOG->PLL_SYS     = s_clockContext.pllSys;
    if ((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_POWERDOWN_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_LOCK_MASK) == 0)
        {
        }
    }
    CCM_ANALOG->PFD_528_CLR = CCM_ANALOG_PFD_528_PFD3_CLKGATE_MASK;

    /* Restore AUDIO PLL */
    CCM_ANALOG->PLL_AUDIO_SET = CCM_ANALOG_PLL_AUDIO_BYPASS_MASK;
    CCM_ANALOG->PLL_AUDIO_CLR = CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK;
    CCM_ANALOG->PLL_AUDIO     = s_clockContext.pllAudio;
    if ((CCM_ANALOG->PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_LOCK_MASK) == 0)
        {
        }
    }

    /* Restore ENET PLL */
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_BYPASS_MASK;
    CCM_ANALOG->PLL_ENET_CLR = CCM_ANALOG_PLL_ENET_POWERDOWN_MASK;
    CCM_ANALOG->PLL_ENET     = s_clockContext.pllEnet;
    if ((CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_POWERDOWN_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_LOCK_MASK) == 0)
        {
        }
    }

    /* Switch clocks back */
    CCM->CBCMR =
        (CCM->CBCMR & ~CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK) | CCM_CBCMR_PRE_PERIPH_CLK_SEL(s_clockContext.preperiphSel);

    /* Wait CCM operation finishes */
    while (CCM->CDHIPR != 0)
    {
    }

    /* Restore Periphral clock */
    CCM->CSCMR1 = (CCM->CSCMR1 & ~CCM_CSCMR1_PERCLK_PODF_MASK) | CCM_CSCMR1_PERCLK_PODF(s_clockContext.perDiv) |
                  CCM_CSCMR1_PERCLK_CLK_SEL(s_clockContext.perSel);
}

static void LPM_LowPowerIdle(void)
{
    /* Switch DCDC to use DCDC internal OSC */
    DCDC_SetClockSource(DCDC, kDCDC_ClockInternalOsc);

    /* Power down USBPHY */
    USBPHY->CTRL = 0xFFFFFFFF;

    LPM_DisableAndSwitchPLLs(LPM_PowerModeLPIdle);

    /* Enable weak 2P5 and turn off regular 2P5 */
    PMU->REG_2P5 |= PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;
    PMU->REG_2P5 &= ~PMU_REG_2P5_ENABLE_LINREG_MASK;
    /* Enable weak 1P1 and turn off regular 1P1 */
    PMU->REG_1P1 |= PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;
    PMU->REG_1P1 &= ~PMU_REG_1P1_ENABLE_LINREG_MASK;

    LPM_EnterCritical();
    LPM_SwitchToRcOSC();
    LPM_ExitCritical();

    CLOCK_DeinitExternalClk();

    /* Set voltage to 0.95V */
    DCDC_AdjustTargetVoltage(DCDC, 0x6, 0x1);
    DCDC_BootIntoDCM(DCDC);
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;

    LPM_EnterCritical();
    LPM_SwitchBandgap();
    LPM_ExitCritical();
}

void LPM_LowPowerIdlerRestore()
{
    DCDC_BootIntoCCM(DCDC);
    /* Restore voltage to 1.275v */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);

    /* Switch DCDC to use DCDC External OSC */
    DCDC_SetClockSource(DCDC, kDCDC_ClockExternalOsc);

    /* Disconnect vdd_snvs_in and connect vdd_high_in */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_DISCON_HIGH_SNVS_MASK;
    /* Increase OSC current to normal */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_OSC_I_MASK;

    LPM_EnterCritical();
    LPM_SwitchToXtalOSC();
    LPM_RestoreBandgap();
    LPM_ExitCritical();

    /* Enable regular 2P5 and wait for stable */
    PMU->REG_2P5_SET = PMU_REG_2P5_ENABLE_LINREG_MASK;
    while ((PMU->REG_2P5 & PMU_REG_2P5_OK_VDD2P5_MASK) == 0)
    {
    }
    /* Turn off weak 2P5 */
    PMU->REG_2P5_CLR = PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;

    /* Enable regular 1P1 and wait for stable */
    PMU->REG_1P1_SET = PMU_REG_1P1_ENABLE_LINREG_MASK;
    while ((PMU->REG_1P1 & PMU_REG_1P1_OK_VDD1P1_MASK) == 0)
    {
    }
    /* Turn off weak 1P1 */
    PMU->REG_1P1_CLR = PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;

    LPM_RestorePLLs(LPM_PowerModeLPIdle);
}

static void LPM_SystemDsm()
{
    uint32_t i;
    uint32_t gpcIMR[LPM_GPC_IMR_NUM];

    BOARD_SetLPClockGate();

    /* Turn off FlexRAM1 */
    PGC->MEGA_CTRL |= PGC_MEGA_CTRL_PCR_MASK;
    /* Turn off FlexRAM0 */
    GPC->CNTR |= GPC_CNTR_PDRAM0_PGE_MASK;

    /* Clean and disable data cache to make sure context is saved into DDR */
    SCB_CleanDCache();
    SCB_DisableDCache();

    /* Adjust LP voltage to 0.925V */
    DCDC_AdjustTargetVoltage(DCDC, 0x12, 0x1);

    /* Switch DCDC to use DCDC internal OSC */
    DCDC_SetClockSource(DCDC, kDCDC_ClockInternalOsc);

    /* Power down USBPHY */
    USBPHY->CTRL = 0xFFFFFFFF;

    /* Power down CPU when requested */
    PGC->CPU_CTRL = PGC_CPU_CTRL_PCR_MASK;

    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* Disconnect vdd_high_in and connect vdd_snvs_in */
    CCM_ANALOG->MISC0_SET = CCM_ANALOG_MISC0_DISCON_HIGH_SNVS_MASK;
    /* STOP_MODE config, turn off all analog except RTC in stop mode */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_STOP_MODE_CONFIG_MASK;

    /* Mask all GPC interrupts before enabling the RBC counters to
       avoid the counter starting too early if an interupt is already
       pending.
    */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        gpcIMR[i]   = GPC->IMR[i];
        GPC->IMR[i] = 0xFFFFFFFFU;
    }

    /* Enable the RBC bypass counter here to hold off the interrupts. RBC counter
     * = 32 (1ms). Minimum RBC delay should be 400us.
     */
    CCM->CCR = (CCM_CCR_RBC_EN_MASK | CCM_CCR_COSC_EN_MASK | CCM_CCR_OSCNT(0xAF));

    /* Recover all the GPC interrupts. */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = gpcIMR[i];
    }

    /* Now delay for a short while (3usec) at this point
     * so a short loop should be enough. This delay is required to ensure that
     * the RBC counter can start counting in case an interrupt is already pending
     * or in case an interrupt arrives just as ARM is about to assert DSM_request.
     */
    for (i = 0; i < ARM_CORE_CURRENT_FREQ; i++)
    {
        __NOP();
    }
}

void LPM_SystemRestoreDsm(void)
{
    /* Enable clock of ARM platform memories when entering LPM */
    CCM->CGPR |= CCM_CGPR_INT_MEM_CLK_LPM_MASK;

    /* Clear ARM power gate setting */
    PGC->CPU_CTRL &= ~PGC_CPU_CTRL_PCR_MASK;
    /* Keep megamix power on when STOP */
    PGC->MEGA_CTRL &= ~PGC_MEGA_CTRL_PCR_MASK;
    /* Clear FET ODRIVE */
    PMU->REG_CORE_CLR = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* Disconnect vdd_snvs_in and connect vdd_high_in */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_DISCON_HIGH_SNVS_MASK;
}

void LPM_SystemResumeDsm(void)
{
    uint32_t clpcr;

    clpcr = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));

    GPC->CNTR &= ~GPC_CNTR_PDRAM0_PGE_MASK;

    /* Clear resume entry */
    SRC->GPR[0] = 0U;
    SRC->GPR[1] = 0U;

    LPM_SystemRestoreDsm();

    /* RBC bypass enabled in LPM_SystemDsm */
    LPM_DisableRbcBypass();
    LPM_SetClockMode(kCLOCK_ModeRun, clpcr);
}

void LPM_RestoreLowPowerRunPLLs(void)
{
    /* Switch DCDC to use DCDC External OSC */
    DCDC_SetClockSource(DCDC, kDCDC_ClockExternalOsc);

    /* Switch to XTAL-OSC */
    LPM_EnterCritical();
    LPM_SwitchToXtalOSC();
    LPM_RestoreBandgap();
    LPM_ExitCritical();

    /* Enable regular 2P5 and wait for stable */
    PMU->REG_2P5_SET = PMU_REG_2P5_ENABLE_LINREG_MASK;
    while ((PMU->REG_2P5 & PMU_REG_2P5_OK_VDD2P5_MASK) == 0)
    {
    }
    /* Turn off weak 2P5 */
    PMU->REG_2P5_CLR = PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;

    /* Enable regular 1P1 and wait for stable */
    PMU->REG_1P1_SET = PMU_REG_1P1_ENABLE_LINREG_MASK;
    while ((PMU->REG_1P1 & PMU_REG_1P1_OK_VDD1P1_MASK) == 0)
    {
    }
    /* Turn off weak 1P1 */
    PMU->REG_1P1_CLR = PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;

    /* Restore USB1 PLL */
    CCM_ANALOG->PLL_USB1_SET = CCM_ANALOG_PLL_USB1_BYPASS_MASK;
    CCM_ANALOG->PLL_USB1 =
        (s_clockContext.pllUsb1 & (~(CCM_ANALOG_PLL_USB1_POWER_MASK))) | CCM_ANALOG_PLL_USB1_BYPASS_MASK;
    CCM_ANALOG->PLL_USB1_SET = CCM_ANALOG_PLL_USB1_POWER_MASK;
    CCM_ANALOG->PFD_480_CLR  = CCM_ANALOG_PFD_480_PFD0_FRAC_MASK;
    CCM_ANALOG->PFD_480_SET  = CCM_ANALOG_PFD_480_PFD0_FRAC(35);
    CCM_ANALOG->PFD_480_CLR  = CCM_ANALOG_PFD_480_PFD0_CLKGATE_MASK;
    CCM_ANALOG->PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_BYPASS_MASK;

    if ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_POWER_MASK) != 0)
    {
        while ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_LOCK_MASK) == 0)
        {
        }
    }
    /* Wait CCM operation finishes */
    while (CCM->CDHIPR != 0)
    {
    }
#if defined(XIP_EXTERNAL_FLASH)
    LPM_EnterCritical();
    LPM_RestoreFlexspiClock();
    LPM_ExitCritical();
#endif
}

void LPM_SystemOverRun(void)
{
    /* change the DCDC_LP to 1.275V first */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);

    SwitchToRunModePLLs();

    s_OverRunFlag = 1;
    s_ClearLPFlag = 0;
}

void LPM_SystemFullRun(void)
{
    /* change the DCDC_LP to 1.15V */
    DCDC_AdjustTargetVoltage(DCDC, 0xE, 0x1);

    SwitchToFullRunPLLs();

    s_ClearLPFlag = 0;
}

void LPM_SystemLowSpeedRun(void)
{
    /* Adjust voltage to 1.15V */
    DCDC_AdjustTargetVoltage(DCDC, 0xE, 0x1);

    /* Power off AUDIO PLL */
    CCM_ANALOG->PLL_AUDIO_SET = CCM_ANALOG_PLL_AUDIO_BYPASS_MASK;
    CCM_ANALOG->PLL_AUDIO_SET = CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK;
    /* Power off ENET PLL */
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_BYPASS_MASK;
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_POWERDOWN_MASK;

    SwitchToLowSpeedRunPLLs();

    s_ClearLPFlag = 0;
}

void LPM_SystemLowPowerRun(void)
{
    /* Adjust voltage to 0.95V */
    DCDC_AdjustTargetVoltage(DCDC, 0x6, 0x1);

    /* Switch DCDC to use DCDC internal OSC */
    DCDC_SetClockSource(DCDC, kDCDC_ClockInternalOsc);

    /* Power down USBPHY */
    USBPHY->CTRL = 0xFFFFFFFF;

    LPM_DisableAndSwitchPLLs(LPM_PowerModeLowPowerRun);

    /* Enable weak 2P5 and turn off regular 2P5 */
    PMU->REG_2P5 |= PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;
    PMU->REG_2P5 &= ~PMU_REG_2P5_ENABLE_LINREG_MASK;
    /* Enable weak 1P1 and turn off regular 1P1 */
    PMU->REG_1P1 |= PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;
    PMU->REG_1P1 &= ~PMU_REG_1P1_ENABLE_LINREG_MASK;

    LPM_EnterCritical();
    LPM_SwitchToRcOSC();
    LPM_SwitchBandgap();
    LPM_ExitCritical();

    s_LowPowerRunFlag = 1;
    s_ClearLPFlag     = 1;
}

bool LPM_Init(lpm_power_mode_t run_mode)
{
    uint32_t i;
    uint32_t tmp_reg = 0;

    if (run_mode > LPM_PowerModeRunEnd)
    {
        return false;
    }
    s_curMode = run_mode;
    CLOCK_SetMode(kCLOCK_ModeRun);

    CCM->CGPR |= CCM_CGPR_INT_MEM_CLK_LPM_MASK;

    /* Enable RC OSC. It needs at least 4ms to be stable, so self tuning need to be enabled. */
    XTALOSC24M->LOWPWR_CTRL |= XTALOSC24M_LOWPWR_CTRL_RC_OSC_EN_MASK;
    /* Configure RC OSC */
    XTALOSC24M->OSC_CONFIG0 = XTALOSC24M_OSC_CONFIG0_RC_OSC_PROG_CUR(0x4) | XTALOSC24M_OSC_CONFIG0_SET_HYST_MINUS(0x2) |
                              XTALOSC24M_OSC_CONFIG0_RC_OSC_PROG(0xA7) | XTALOSC24M_OSC_CONFIG0_START_MASK |
                              XTALOSC24M_OSC_CONFIG0_ENABLE_MASK;
    XTALOSC24M->OSC_CONFIG1 = XTALOSC24M_OSC_CONFIG1_COUNT_RC_CUR(0x40) | XTALOSC24M_OSC_CONFIG1_COUNT_RC_TRG(0x2DC);
    /* Take some delay */
    LPM_DELAY(40);
    /* Add some hysteresis */
    tmp_reg = XTALOSC24M->OSC_CONFIG0;
    tmp_reg &= ~(XTALOSC24M_OSC_CONFIG0_HYST_PLUS_MASK | XTALOSC24M_OSC_CONFIG0_HYST_MINUS_MASK);
    tmp_reg |= XTALOSC24M_OSC_CONFIG0_HYST_PLUS(3) | XTALOSC24M_OSC_CONFIG0_HYST_MINUS(3);
    XTALOSC24M->OSC_CONFIG0 = tmp_reg;
    /* Set COUNT_1M_TRG */
    tmp_reg = XTALOSC24M->OSC_CONFIG2;
    tmp_reg &= ~XTALOSC24M_OSC_CONFIG2_COUNT_1M_TRG_MASK;
    tmp_reg |= XTALOSC24M_OSC_CONFIG2_COUNT_1M_TRG(0x2d7);
    XTALOSC24M->OSC_CONFIG2 = tmp_reg;
    /* Hardware requires to read OSC_CONFIG0 or OSC_CONFIG1 to make OSC_CONFIG2 write work */
    tmp_reg                 = XTALOSC24M->OSC_CONFIG1;
    XTALOSC24M->OSC_CONFIG1 = tmp_reg;

    s_DllBackupValue = FLEXSPI->DLLCR[0];

    /* ERR007265 */
    IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_GINT_MASK;

    /* Initialize GPC to mask all IRQs */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = 0xFFFFFFFFU;
    }

    return true;
}

void LPM_Deinit(void)
{
    /* ERR007265 */
    IOMUXC_GPR->GPR1 &= ~IOMUXC_GPR_GPR1_GINT_MASK;
}

void LPM_EnableWakeupSource(uint32_t irq)
{
    GPC_EnableIRQ(GPC, irq);
}

void LPM_DisableWakeupSource(uint32_t irq)
{
    GPC_DisableIRQ(GPC, irq);
}

bool LPM_SetPowerMode(lpm_power_mode_t mode)
{
    bool ret = true;

    if (mode == s_curMode)
    {
        return ret;
    }

    return ret;
}

void ENTER_SLEEP_PROCESSING(void)
{
    uint32_t clpcr;

    APP_PowerPreSwitchHook(APP_GetLPMPowerMode());

    clpcr = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));

    switch (APP_GetLPMPowerMode())
    {
        case LPM_PowerModeOverRun:
        case LPM_PowerModeFullRun:
        case LPM_PowerModeLowSpeedRun:
        case LPM_PowerModeLowPowerRun:
            break;
        case LPM_PowerModeSysIdle:
            LPM_SetClockMode(kCLOCK_ModeWait, clpcr | CCM_CLPCR_LPM(kCLOCK_ModeWait) |
                                                  CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | CCM_CLPCR_STBY_COUNT_MASK | 0x1C |
                                                  0x08280000);
            BOARD_SetLPClockGate();
            LPM_EnterSystemIdle();
            IOMUXC_GPR->GPR8  = 0xaaaaaaaa;
            IOMUXC_GPR->GPR12 = 0x0000000a;
            PRINTF("\r\n *******System idel!**********\r\n");
            break;
        case LPM_PowerModeLPIdle:
            LPM_SetClockMode(kCLOCK_ModeWait, clpcr | CCM_CLPCR_LPM(kCLOCK_ModeWait) |
                                                  CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | CCM_CLPCR_STBY_COUNT_MASK | 0x1C |
                                                  0x08280000);
            BOARD_SetLPClockGate();
            LPM_LowPowerIdle();
            IOMUXC_GPR->GPR8  = 0xaaaaaaaa;
            IOMUXC_GPR->GPR12 = 0x0000000a;
            PRINTF("\r\n *******Low power idel!**********\r\n");
            break;
        case LPM_PowerModeSuspend:
            LPM_SetClockMode(kCLOCK_ModeStop, clpcr | CCM_CLPCR_LPM(kCLOCK_ModeStop) | CCM_CLPCR_VSTBY_MASK |
                                                  CCM_CLPCR_STBY_COUNT_MASK | CCM_CLPCR_SBYOS_MASK |
                                                  CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | 0x08280000);
            LPM_SystemDsm();
            break;
        default:
            assert(false);
            break;
    }
}

void EXIT_SLEEP_PROCESSING()
{
    uint32_t clpcr;

    clpcr = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));

    switch (APP_GetLPMPowerMode())
    {
        case LPM_PowerModeOverRun:
        case LPM_PowerModeFullRun:
        case LPM_PowerModeLowSpeedRun:
        case LPM_PowerModeLowPowerRun:
            break;
        case LPM_PowerModeSysIdle:
            IOMUXC_GPR->GPR8  = 0x00000000;
            IOMUXC_GPR->GPR12 = 0x00000000;
            LPM_SystemIdleRestore();
            LPM_SetClockMode(kCLOCK_ModeRun, clpcr);
            break;
        case LPM_PowerModeLPIdle:
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            IOMUXC_GPR->GPR8  = 0x00000000;
            IOMUXC_GPR->GPR12 = 0x00000000;
            /* Restore Idel to low power run mode */
            if ((s_LowPowerRunFlag == 1) && (s_ClearLPFlag == 1))
            {
                /* Adjust voltage to 0.95V */
                DCDC_AdjustTargetVoltage(DCDC, 0x6, 0x1);
                s_LowPowerRunFlag = 0;

                CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 1);
                CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0x0);
                CLOCK_SetMux(kCLOCK_PeriphMux, 1);
                CLOCK_SetDiv(kCLOCK_IpgDiv, 0x1);
                CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0);
            }
            else
            {
                LPM_LowPowerIdlerRestore();
            }
            LPM_SetClockMode(kCLOCK_ModeRun, clpcr);
            break;
        case LPM_PowerModeSuspend:
            /* Restore when wakeup from suspend reset */
            LPM_SystemResumeDsm();

            /* recover handshaking */
            IOMUXC_GPR->GPR4  = 0x00000000;
            IOMUXC_GPR->GPR7  = 0x00000000;
            IOMUXC_GPR->GPR8  = 0x00000000;
            IOMUXC_GPR->GPR12 = 0x00000000;

            CCM->CCR &= ~CCM_CCR_REG_BYPASS_COUNT_MASK;
            break;
        default:
            assert(false);
            break;
    }

    APP_PowerPostSwitchHook(APP_GetLPMPowerMode());
}

void LPM_SetSleepPowerMode(lpm_power_mode_t mode)
{
    g_savedPrimask = DisableGlobalIRQ();
    __DSB();
    __ISB();

    ENTER_SLEEP_PROCESSING();
    __DSB();
    __WFI();
    __ISB();
    EXIT_SLEEP_PROCESSING();

    EnableGlobalIRQ(g_savedPrimask);
    __DSB();
    __ISB();
}
