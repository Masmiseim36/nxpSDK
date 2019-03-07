/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
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

#ifndef MMDC_MAPSR_DVFS_MASK
#define MMDC_MAPSR_DVFS_MASK (0x200000U)
#define MMDC_MAPSR_DVACK_MASK (0x2000000U)
#endif

#define LPM_GPC_IMR_INDEX(irq) (((irq)-32U) / 32U)
#define LPM_GPC_IMR_MASK(irq) (1U << ((irq)&0x1F))
#define LPM_GPC_IMR_NUM (sizeof(GPC->IMR) / sizeof(GPC->IMR[0]))

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _lpm_mmdc_io_context
{
    uint32_t DQM0;
    uint32_t DQM1;
    uint32_t RAS_B;
    uint32_t CAS_B;
    uint32_t ODT0;
    uint32_t ODT1;
    uint32_t SDCLK0_P;
    uint32_t SDQS0_P;
    uint32_t SDQS1_P;
    uint32_t GRP_ADDDS;
    uint32_t GRP_DDRMODE_CTL;
    uint32_t GRP_B0DS;
    uint32_t GRP_B1DS;
    uint32_t GRP_DDRMODE;
} lpm_mmdc_io_context_t;

typedef struct _lpm_mmdc_context
{
    uint32_t powerDownTimer;
    uint32_t powerSaveDisable;
} lpm_mmdc_context_t;

typedef struct _lpm_clock_context
{
    uint32_t ahbDiv;
    uint32_t perDiv;
    uint32_t mmdcDiv;
    uint32_t pfd480;
    uint32_t pfd528;
    uint32_t pllArm;
    uint32_t pllSys;
    uint32_t pllUsb1;
    uint32_t pllUsb2;
    uint32_t pllAudio;
    uint32_t pllVideo;
    uint32_t pllEnet;
} lpm_clock_context_t;

#if defined(__GNUC__)
static lpm_mmdc_io_context_t s_mmdcIoContext __attribute__((section("OcramData")));
static lpm_mmdc_context_t s_mmdcContext __attribute__((section("OcramData")));
static lpm_clock_context_t s_clockContext __attribute__((section("OcramData")));
#else
static lpm_mmdc_io_context_t s_mmdcIoContext;
static lpm_mmdc_context_t s_mmdcContext;
static lpm_clock_context_t s_clockContext;
#endif

extern void LPM_ResumeIdle(void);
extern void LPM_ResumeDsm(void);
#if defined(__GNUC__)
static void LPM_SuspendMmdc(void) __attribute__((section("OcramText")));
static void LPM_ResumeMmdc(void) __attribute__((section("OcramText")));
static void LPM_DisablePLLs(void) __attribute__((section("OcramText")));
static void LPM_RestorePLLs(void) __attribute__((section("OcramText")));
static void LPM_SystemRestoreIdle(void) __attribute__((section("OcramText")));
static void LPM_SystemRestoreDsm(void) __attribute__((section("OcramText")));
uint32_t LPM_SystemResumeIdle(void) __attribute__((section("OcramText")));
uint32_t LPM_SystemResumeDsm(void) __attribute__((section("OcramText")));
void LPM_SystemIdle(uint32_t context) __attribute__((section("OcramText")));
void LPM_SystemDsm(uint32_t context) __attribute__((section("OcramText")));
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
static void LPM_SuspendMmdc(void)
{
    /* Disable DDR auto power saving */
    s_mmdcContext.powerSaveDisable = MMDC->MAPSR & MMDC_MAPSR_PSD_MASK;
    MMDC->MAPSR |= MMDC_MAPSR_PSD_MASK;
    /* Disable DDR power down timer */
    s_mmdcContext.powerDownTimer = MMDC->MDPDC & (MMDC_MDPDC_PWDT_0_MASK | MMDC_MDPDC_PWDT_1_MASK);
    MMDC->MDPDC &= ~(MMDC_MDPDC_PWDT_0_MASK | MMDC_MDPDC_PWDT_1_MASK);

    /* Force DDR to self refresh */
    MMDC->MAPSR |= MMDC_MAPSR_DVFS_MASK;
    while ((MMDC->MAPSR & MMDC_MAPSR_DVACK_MASK) == 0)
    {
    }

    /* Save DDR pins configuration */
    s_mmdcIoContext.DQM0 = IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_DQM0];
    s_mmdcIoContext.DQM1 = IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_DQM1];
    s_mmdcIoContext.RAS_B = IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_RAS_B];
    s_mmdcIoContext.CAS_B = IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_CAS_B];
    s_mmdcIoContext.ODT0 = IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_ODT0];
    s_mmdcIoContext.ODT1 = IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_ODT1];
    s_mmdcIoContext.SDCLK0_P = IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_SDCLK0_P];
    s_mmdcIoContext.SDQS0_P = IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS0_P];
    s_mmdcIoContext.SDQS1_P = IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS1_P];
    s_mmdcIoContext.GRP_ADDDS = IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_ADDDS];
    s_mmdcIoContext.GRP_DDRMODE_CTL = IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_DDRMODE_CTL];
    s_mmdcIoContext.GRP_B0DS = IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_B0DS];
    s_mmdcIoContext.GRP_B1DS = IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_B1DS];
    s_mmdcIoContext.GRP_DDRMODE = IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_DDRMODE];

    /* Configure MMDC pins to save power */
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_DQM0] = 0;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_DQM1] = 0;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_RAS_B] = 0;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_CAS_B] = 0;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_ODT0] = 0;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_ODT1] = 0;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_SDCLK0_P] = 0;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS0_P] = 0;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS1_P] = 0;
    IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_ADDDS] = 0;
    IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_DDRMODE_CTL] = 0;
    IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_B0DS] = 0;
    IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_B1DS] = 0;
    IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_DDRMODE] = 0;
}

static void LPM_ResumeMmdc(void)
{
    /* Restore DDR pins configuration */
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_DQM0] = s_mmdcIoContext.DQM0;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_DQM1] = s_mmdcIoContext.DQM1;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_RAS_B] = s_mmdcIoContext.RAS_B;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_CAS_B] = s_mmdcIoContext.CAS_B;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_ODT0] = s_mmdcIoContext.ODT0;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_ODT1] = s_mmdcIoContext.ODT1;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_SDCLK0_P] = s_mmdcIoContext.SDCLK0_P;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS0_P] = s_mmdcIoContext.SDQS0_P;
    IOMUXC->SW_PAD_CTL_PAD_DDR[kIOMUXC_SW_PAD_CTL_PAD_DRAM_SDQS1_P] = s_mmdcIoContext.SDQS1_P;
    IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_ADDDS] = s_mmdcIoContext.GRP_ADDDS;
    IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_DDRMODE_CTL] = s_mmdcIoContext.GRP_DDRMODE_CTL;
    IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_B0DS] = s_mmdcIoContext.GRP_B0DS;
    IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_B1DS] = s_mmdcIoContext.GRP_B1DS;
    IOMUXC->SW_PAD_CTL_GRP[kIOMUXC_SW_PAD_CTL_GRP_DDRMODE] = s_mmdcIoContext.GRP_DDRMODE;

    /* Reset read FIFO twice */
    MMDC->MPDGCTRL0 |= MMDC_MPDGCTRL0_RST_RD_FIFO_MASK;
    while ((MMDC->MPDGCTRL0 & MMDC_MPDGCTRL0_RST_RD_FIFO_MASK) != 0)
    {
    }
    MMDC->MPDGCTRL0 |= MMDC_MPDGCTRL0_RST_RD_FIFO_MASK;
    while ((MMDC->MPDGCTRL0 & MMDC_MPDGCTRL0_RST_RD_FIFO_MASK) != 0)
    {
    }

    /* Get DDR out of self refresh */
    MMDC->MAPSR &= ~MMDC_MAPSR_DVFS_MASK;
    while ((MMDC->MAPSR & MMDC_MAPSR_DVACK_MASK) != 0)
    {
    }

    /* Restore DDR power saving setting */
    MMDC->MDPDC |= s_mmdcContext.powerDownTimer;
    if (s_mmdcContext.powerSaveDisable)
    {
        MMDC->MAPSR |= MMDC_MAPSR_PSD_MASK;
    }
    else
    {
        MMDC->MAPSR &= ~MMDC_MAPSR_PSD_MASK;
    }
}

static void LPM_DisablePLLs(void)
{
    /* CCSR STEP_SEL already selects 24M OSC in BOARD_BootClockRUN() */
    /* Let CPU run on Step MUX (24M) */
    CCM->CCSR |= CCM_CCSR_PLL1_SW_CLK_SEL_MASK;

    /* Switch AHB clock source from PLL to OSC */
    CCM->CBCMR = (CCM->CBCMR & ~CCM_CBCMR_PERIPH_CLK2_SEL_MASK) | CCM_CBCMR_PERIPH_CLK2_SEL(1);
    CCM->CBCDR |= CCM_CBCDR_PERIPH_CLK_SEL_MASK;
    /* Set AHB clock to 3M */
    s_clockContext.ahbDiv = CCM->CBCDR & CCM_CBCDR_AHB_PODF_MASK;
    CCM->CBCDR |= CCM_CBCDR_AHB_PODF_MASK;
    /* Set serial clock to 6M */
    s_clockContext.perDiv = CCM->CSCMR1 & CCM_CSCMR1_PERCLK_PODF_MASK;
    CCM->CSCMR1 = (CCM->CSCMR1 & ~CCM_CSCMR1_PERCLK_PODF_MASK) | CCM_CSCMR1_PERCLK_PODF(3);

    /* Switch MMDC clock source from PLL to OSC / 3 */
    CCM->CBCMR |= CCM_CBCMR_PERIPH2_CLK2_SEL_MASK;
    CCM->CBCDR = (CCM->CBCDR & ~CCM_CBCDR_PERIPH2_CLK2_PODF_MASK) | CCM_CBCDR_PERIPH2_CLK2_PODF(2);
    CCM->CBCDR |= CCM_CBCDR_PERIPH2_CLK_SEL_MASK;
    /* Set MMDC clock to 1M */
    s_clockContext.mmdcDiv = CCM->CBCDR & CCM_CBCDR_FABRIC_MMDC_PODF_MASK;
    CCM->CBCDR |= CCM_CBCDR_FABRIC_MMDC_PODF_MASK;
    /* Wait CCM operation finishes */
    while (CCM->CDHIPR != 0)
    {
    }

    /* Application shouldn't rely on PLL in low power mode,
     * gate all PLL and PFD now */
    s_clockContext.pfd480 = CCM_ANALOG->PFD_480;
    s_clockContext.pfd528 = CCM_ANALOG->PFD_528;
    CCM_ANALOG->PFD_480 |= CCM_ANALOG_PFD_480_PFD0_CLKGATE_MASK | CCM_ANALOG_PFD_480_PFD1_CLKGATE_MASK |
                           CCM_ANALOG_PFD_480_PFD2_CLKGATE_MASK | CCM_ANALOG_PFD_480_PFD3_CLKGATE_MASK;
    CCM_ANALOG->PFD_528 |= CCM_ANALOG_PFD_528_PFD0_CLKGATE_MASK | CCM_ANALOG_PFD_528_PFD1_CLKGATE_MASK |
                           CCM_ANALOG_PFD_528_PFD2_CLKGATE_MASK | CCM_ANALOG_PFD_528_PFD3_CLKGATE_MASK;
    /* Power off ARM PLL */
    s_clockContext.pllArm = CCM_ANALOG->PLL_ARM;
    CCM_ANALOG->PLL_ARM = CCM_ANALOG_PLL_ARM_POWERDOWN_MASK;
    /* Power off SYS PLL */
    s_clockContext.pllSys = CCM_ANALOG->PLL_SYS;
    CCM_ANALOG->PLL_SYS = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;
    /* Power off USB1 PLL */
    s_clockContext.pllUsb1 = CCM_ANALOG->PLL_USB1;
    CCM_ANALOG->PLL_USB1 = 0;
    /* Power off USB2 PLL */
    s_clockContext.pllUsb2 = CCM_ANALOG->PLL_USB2;
    CCM_ANALOG->PLL_USB2 = 0;
    /* Power off AUDIO PLL */
    s_clockContext.pllAudio = CCM_ANALOG->PLL_AUDIO;
    CCM_ANALOG->PLL_AUDIO = CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK;
    /* Power off VIDEO PLL */
    s_clockContext.pllVideo = CCM_ANALOG->PLL_VIDEO;
    CCM_ANALOG->PLL_VIDEO = CCM_ANALOG_PLL_VIDEO_POWERDOWN_MASK;
    /* Power off ENET PLL */
    s_clockContext.pllEnet = CCM_ANALOG->PLL_ENET;
    CCM_ANALOG->PLL_ENET = CCM_ANALOG_PLL_ENET_POWERDOWN_MASK;
}

static void LPM_RestorePLLs(void)
{
    /* Power on ARM PLL and wait for locked */
    CCM_ANALOG->PLL_ARM = s_clockContext.pllArm;
    while ((CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_LOCK_MASK) == 0)
    {
    }

    /* Power on SYS PLL and wait for locked */
    CCM_ANALOG->PLL_SYS = s_clockContext.pllSys;
    while ((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_LOCK_MASK) == 0)
    {
    }

    /* Restore USB1 PLL */
    CCM_ANALOG->PLL_USB1 = s_clockContext.pllUsb1;
    if ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_POWER_MASK) != 0)
    {
        while ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_LOCK_MASK) == 0)
        {
        }
    }

    /* Restore USB2 PLL */
    CCM_ANALOG->PLL_USB2 = s_clockContext.pllUsb2;
    if ((CCM_ANALOG->PLL_USB2 & CCM_ANALOG_PLL_USB2_POWER_MASK) != 0)
    {
        while ((CCM_ANALOG->PLL_USB2 & CCM_ANALOG_PLL_USB2_LOCK_MASK) == 0)
        {
        }
    }

    /* Restore AUDIO PLL */
    CCM_ANALOG->PLL_AUDIO = s_clockContext.pllAudio;
    if ((CCM_ANALOG->PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_LOCK_MASK) == 0)
        {
        }
    }

    /* Restore VIDEO PLL */
    CCM_ANALOG->PLL_VIDEO = s_clockContext.pllVideo;
    if ((CCM_ANALOG->PLL_VIDEO & CCM_ANALOG_PLL_VIDEO_POWERDOWN_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_VIDEO & CCM_ANALOG_PLL_VIDEO_LOCK_MASK) == 0)
        {
        }
    }

    /* Restore ENET PLL */
    CCM_ANALOG->PLL_ENET = s_clockContext.pllEnet;
    if ((CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_POWERDOWN_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_LOCK_MASK) == 0)
        {
        }
    }

    /* Restore SYS PLL PFD */
    CCM_ANALOG->PFD_528 = s_clockContext.pfd528;
    /* Restore USB1 PLL PFD */
    CCM_ANALOG->PFD_480 = s_clockContext.pfd480;

    /* Restore AHB div */
    CCM->CBCDR = (CCM->CBCDR & ~CCM_CBCDR_AHB_PODF_MASK) | s_clockContext.ahbDiv;
    /* Restore Periphral clock div */
    CCM->CSCMR1 = (CCM->CSCMR1 & ~CCM_CSCMR1_PERCLK_PODF_MASK) | s_clockContext.perDiv;
    /* Switch AHB clock source from OSC to PLL */
    CCM->CBCDR &= ~CCM_CBCDR_PERIPH_CLK_SEL_MASK;

    /* Restore MMDC div */
    CCM->CBCDR = (CCM->CBCDR & ~CCM_CBCDR_FABRIC_MMDC_PODF_MASK) | s_clockContext.mmdcDiv;
    /* Switch MMDC clock source from OSC to PLL */
    CCM->CBCDR &= ~CCM_CBCDR_PERIPH2_CLK_SEL_MASK;

    /* Wait CCM operation finishes */
    while (CCM->CDHIPR != 0)
    {
    }

    /* Restore CPU frequency */
    CCM->CCSR &= ~CCM_CCSR_PLL1_SW_CLK_SEL_MASK;
}

static void LPM_SystemRestoreIdle(void)
{
    /* Clear ARM power gate setting */
    PGC->CPU_CTRL = 0U;

    /* Disconnect vdd_snvs_in and connect vdd_high_in */
    CCM_ANALOG->MISC0 &= ~CCM_ANALOG_MISC0_DISCON_HIGH_SNVS_MASK;
    /* Increase OSC current to normal */
    CCM_ANALOG->MISC0 &= ~CCM_ANALOG_MISC0_OSC_I_MASK;
    /* Restore XTAL-OSC and enable detector */
    CCM_ANALOG->MISC0 = (CCM_ANALOG->MISC0 & ~CCM_ANALOG_MISC0_XTAL_24M_PWD_MASK) | CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK;
    /* Wait for OSC stable */
    while ((CCM_ANALOG->MISC0 & CCM_ANALOG_MISC0_OSC_XTALOK_MASK) == 0)
    {
    }

    /* Switch to XTAL-OSC */
    PMU->LOWPWR_CTRL &= ~PMU_LOWPWR_CTRL_OSC_SEL_MASK;

    /* Turn off XTAL-OSC detector */
    CCM_ANALOG->MISC0 &= ~CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK;

    if ((PMU->REG_CORE & PMU_REG_CORE_REG0_TARG_MASK) == PMU_REG_CORE_REG0_TARG_MASK)
    { /* ARM LDO is bypassed */
        /* Turn on regular bandgap and wait for stable */
        CCM_ANALOG->MISC0 &= ~CCM_ANALOG_MISC0_REFTOP_PWD_MASK;
        while ((CCM_ANALOG->MISC0 & CCM_ANALOG_MISC0_REFTOP_VBGUP_MASK) == 0)
        {
        }
        /* Enable the self-bias circuit in bandgap */
        CCM_ANALOG->MISC0 |= CCM_ANALOG_MISC0_REFTOP_SELFBIASOFF_MASK;
        /* Turn on the bias current from the regular bandgap */
        PMU->LOWPWR_CTRL &= ~PMU_LOWPWR_CTRL_REFTOP_IBIAS_OFF_MASK;
        /* Low power band gap disable */
        PMU->LOWPWR_CTRL &= ~PMU_LOWPWR_CTRL_LPBG_SEL_MASK;
    }

    /* Enable regular 2P5 and wait for stable */
    PMU->REG_2P5 |= PMU_REG_2P5_ENABLE_LINREG_MASK;
    while ((PMU->REG_2P5 & PMU_REG_2P5_OK_VDD2P5_MASK) == 0)
    {
    }
    /* Turn off weak 2P5 */
    PMU->REG_2P5 &= ~PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;

    /* Enable regular 1P1 and wait for stable */
    PMU->REG_1P1 |= PMU_REG_1P1_ENABLE_LINREG_MASK;
    while ((PMU->REG_1P1 & PMU_REG_1P1_OK_VDD1P1_MASK) == 0)
    {
    }
    /* Turn off weak 1P1 */
    PMU->REG_1P1 &= ~PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;

    LPM_RestorePLLs();
    LPM_ResumeMmdc();
}

static void LPM_SystemRestoreDsm(void)
{
    /* Enable clock of ARM platform memories when entering LPM */
    CCM->CGPR |= CCM_CGPR_INT_MEM_CLK_LPM_MASK;

    /* Clear ARM power gate setting */
    PGC->CPU_CTRL = 0U;
    /* Keep megamix power on when STOP */
    PGC->MEGA_CTRL = 0U;
    /* Clear weak 2P5 */
    PMU->REG_2P5 &= ~PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;
    /* Clear FET ODRIVE */
    PMU->REG_CORE &= ~PMU_REG_CORE_FET_ODRIVE_MASK;
    /* Disconnect vdd_snvs_in and connect vdd_high_in */
    CCM_ANALOG->MISC0 &= ~CCM_ANALOG_MISC0_DISCON_HIGH_SNVS_MASK;

    /* Turn off weak 1P1 */
    PMU->REG_1P1 &= ~PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;

    LPM_ResumeMmdc();
}

void LPM_SystemIdle(uint32_t context)
{
    uint32_t i;
    uint32_t gpcIMR[LPM_GPC_IMR_NUM];

    /* Set resume entry */
    SRC->GPR[0] = (uint32_t)LPM_ResumeIdle;
    SRC->GPR[1] = context;

    /* Power down CPU when requested */
    PGC->CPU_CTRL = PGC_CPU_CTRL_PCR_MASK;
#if (defined(MCIMX6G0_SERIES) || defined(MCIMX6G1_SERIES) || defined(MCIMX6G2_SERIES) || defined(MCIMX6G3_SERIES))
    /*
     * i.MX6UltraLite v1.0 ARM power up uses IPG/2048 as clock source,
     * from v1.1, PGC_CPU_PUPSCR bit [5] is re-defined to switch
     * clock to IPG/32, enable this bit to speed up the ARM power
     * up process in low power idle case(IPG@1.5Mhz). So the sw and
     * sw2iso need to be adjusted as below:
     * sw_new(sw2iso_new) = (2048 * 1.5 / (66 * 32)) * sw(sw2iso) = 1.45 * sw(sw2iso)
     */
    PGC->CPU_PUPSCR = PGC_CPU_PUPSCR_SW2ISO(2) | PGC_CPU_PUPSCR_SW(0x11);
#else
    PGC->CPU_PUPSCR = PGC_CPU_PUPSCR_SW2ISO(1) | PGC_CPU_PUPSCR_SW(0);
#endif
    PGC->CPU_PDNSCR = PGC_CPU_PDNSCR_ISO2SW(1) | PGC_CPU_PDNSCR_ISO(1);

    /* Clean and disable data cache to make sure context is saved into DDR */
    L1C_CleanDataCacheAll();
    L1C_DisableDataCache();

    LPM_SuspendMmdc();
    LPM_DisablePLLs();

    /* Enable weak 2P5 and turn off regular 2P5 */
    PMU->REG_2P5 |= PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;
    PMU->REG_2P5 &= ~PMU_REG_2P5_ENABLE_LINREG_MASK;
    /* Enable weak 1P1 and turn off regular 1P1 */
    PMU->REG_1P1 |= PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;
    PMU->REG_1P1 &= ~PMU_REG_1P1_ENABLE_LINREG_MASK;

    if ((PMU->REG_CORE & PMU_REG_CORE_REG0_TARG_MASK) == PMU_REG_CORE_REG0_TARG_MASK)
    { /* ARM LDO is bypassed */
        /* Low power band gap enable */
        PMU->LOWPWR_CTRL |= PMU_LOWPWR_CTRL_LPBG_SEL_MASK;
        /* Turn off the bias current from the regular bandgap */
        PMU->LOWPWR_CTRL |= PMU_LOWPWR_CTRL_REFTOP_IBIAS_OFF_MASK;
        /* Disable the self-bias circuit in bandgap */
        CCM_ANALOG->MISC0 &= ~CCM_ANALOG_MISC0_REFTOP_SELFBIASOFF_MASK;
        /* Turn off regular bandgap */
        CCM_ANALOG->MISC0 |= CCM_ANALOG_MISC0_REFTOP_PWD_MASK;
    }

    /* Switch to RC-OSC */
    PMU->LOWPWR_CTRL |= PMU_LOWPWR_CTRL_OSC_SEL_MASK;
    /* Turn off XTAL-OSC */
    CCM_ANALOG->MISC0 |= CCM_ANALOG_MISC0_XTAL_24M_PWD_MASK;
    /* Lower OSC current by 37.5% */
    CCM_ANALOG->MISC0 |= CCM_ANALOG_MISC0_OSC_I_MASK;
    /* Disconnect vdd_high_in and connect vdd_snvs_in */
    CCM_ANALOG->MISC0 |= CCM_ANALOG_MISC0_DISCON_HIGH_SNVS_MASK;

    /* Mask all GPC interrupts before enabling the RBC counters to
       avoid the counter starting too early if an interupt is already
       pending.
    */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        gpcIMR[i] = GPC->IMR[i];
        GPC->IMR[i] = 0xFFFFFFFFU;
    }

    /* Enable the RBC bypass counter here to hold off the interrupts. RBC counter
     * = 4 (120us). With this setting, the latency from wakeup interrupt to ARM
     * power up is ~130uS.
     */
    CCM->CCR = (CCM->CCR & ~CCM_CCR_REG_BYPASS_COUNT_MASK) | CCM_CCR_REG_BYPASS_COUNT(4);
    CCM->CCR |= CCM_CCR_RBC_EN_MASK;
    /* Recover all the GPC interrupts. */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = gpcIMR[i];
    }

    /* Now delay for a short while (3usec) ARM is at 24MHz at this point
     * so a short loop should be enough. This delay is required to ensure that
     * the RBC counter can start counting in case an interrupt is already pending
     * or in case an interrupt arrives just as ARM is about to assert DSM_request.
     */
    for (i = 0; i < 24; i++)
    {
        __NOP();
    }

    __WFI();
    __NOP();
    __NOP();
    __NOP();
    __NOP();

    /* Interrupt occurs before system idle */
    LPM_SystemRestoreIdle();
}

uint32_t LPM_SystemResumeIdle(void)
{
    uint32_t context = SRC->GPR[1];
    L1C_EnableInstructionCache();

    /* Clear resume entry */
    SRC->GPR[0] = 0U;
    SRC->GPR[1] = 0U;

    LPM_SystemRestoreIdle();

    return context;
}

void LPM_SystemDsm(uint32_t context)
{
    uint32_t i;
    uint32_t gpcIMR[LPM_GPC_IMR_NUM];

    /* Set resume entry */
    SRC->GPR[0] = (uint32_t)LPM_ResumeDsm;
    SRC->GPR[1] = context;

    /* Clean and disable data cache to make sure context is saved into DDR */
    L1C_CleanDataCacheAll();
    L1C_DisableDataCache();

    LPM_SuspendMmdc();

    /* Disable clock of ARM platform memories when entering LPM */
    CCM->CGPR &= ~CCM_CGPR_INT_MEM_CLK_LPM_MASK;

    /* Power down CPU when requested */
    PGC->CPU_CTRL = PGC_CPU_CTRL_PCR_MASK;
    PGC->CPU_PUPSCR = PGC_CPU_PUPSCR_SW2ISO(1) | PGC_CPU_PUPSCR_SW(0);
    PGC->CPU_PDNSCR = PGC_CPU_PDNSCR_ISO2SW(1) | PGC_CPU_PDNSCR_ISO(1);

    if ((CCM_ANALOG->MISC0 & CCM_ANALOG_MISC0_STOP_MODE_CONFIG_MASK) == 0)
    {
        /* Can only be enabled when STOP_MODE_CONFIG is 0 */
        PMU->REG_2P5 |= PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;
        /* Enable weak 1P1 */
        PMU->REG_1P1 |= PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;
    }

    PMU->REG_CORE |= PMU_REG_CORE_FET_ODRIVE_MASK;
    /* Disconnect vdd_high_in and connect vdd_snvs_in */
    CCM_ANALOG->MISC0 |= CCM_ANALOG_MISC0_DISCON_HIGH_SNVS_MASK;

    /* Mask all GPC interrupts before enabling the RBC counters to
       avoid the counter starting too early if an interupt is already
       pending.
    */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        gpcIMR[i] = GPC->IMR[i];
        GPC->IMR[i] = 0xFFFFFFFFU;
    }

    /* Enable the RBC bypass counter here to hold off the interrupts. RBC counter
     * = 32 (1ms). Minimum RBC delay should be 400us.
     */
    CCM->CCR = (CCM->CCR & ~CCM_CCR_REG_BYPASS_COUNT_MASK) | CCM_CCR_REG_BYPASS_COUNT(32);
    CCM->CCR |= CCM_CCR_RBC_EN_MASK;
    /* Recover all the GPC interrupts. */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = gpcIMR[i];
    }

    /* Now delay for a short while (3usec) ARM is at 528MHz at this point
     * so a short loop should be enough. This delay is required to ensure that
     * the RBC counter can start counting in case an interrupt is already pending
     * or in case an interrupt arrives just as ARM is about to assert DSM_request.
     */
    for (i = 0; i < 22 * 24; i++)
    {
        __NOP();
    }

    __WFI();
    __NOP();
    __NOP();
    __NOP();
    __NOP();

    LPM_SystemRestoreDsm();
}

uint32_t LPM_SystemResumeDsm(void)
{
    uint32_t context = SRC->GPR[1];

    L1C_EnableInstructionCache();

    /* Clear resume entry */
    SRC->GPR[0] = 0U;
    SRC->GPR[1] = 0U;

    LPM_SystemRestoreDsm();

    return context;
}
