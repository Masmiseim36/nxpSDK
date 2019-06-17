/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lpm.h"
#include "fsl_gpc.h"
#include "fsl_dcdc.h"
#include "specific.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LPM_GPC_IMR_NUM (sizeof(GPC->IMR) / sizeof(GPC->IMR[0]))

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t g_savedPrimask;

/*******************************************************************************
 * Code
 ******************************************************************************/
void EnableRegularLDO(void)
{
    /*  Enable Regular LDO 2P5 and 1P1 */
    PMU->REG_2P5_SET = PMU_REG_2P5_ENABLE_LINREG_MASK;
    PMU->REG_1P1_SET = PMU_REG_1P1_ENABLE_LINREG_MASK;
}

void DisableRegularLDO(void)
{
    /* Disable Regular LDO 2P5 and 1P1 */
    PMU->REG_2P5_CLR = PMU_REG_2P5_ENABLE_LINREG_MASK;
    PMU->REG_1P1_CLR = PMU_REG_1P1_ENABLE_LINREG_MASK;
}

void EnableWeakLDO(void)
{
    /*  Enable Weak LDO 2P5 and 1P1 */
    PMU->REG_2P5_SET = PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;
    PMU->REG_1P1_SET = PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;

    SDK_DelayAtLeastUs(40);
}

void DisableWeakLDO(void)
{
    /* Disable Weak LDO 2P5 and 1P1 */
    PMU->REG_2P5_CLR = PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;
    PMU->REG_1P1_CLR = PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;
}

void BandgapOn(void)
{
    /* Turn on regular bandgap and wait for stable */
    PMU->MISC0_CLR = PMU_MISC0_REFTOP_PWD_MASK;
    while ((PMU->MISC0 & PMU_MISC0_REFTOP_VBGUP_MASK) == 0)
    {
    }
    /* Low power band gap disable */
    XTALOSC24M->LOWPWR_CTRL_CLR = XTALOSC24M_LOWPWR_CTRL_LPBG_SEL_MASK;
}

void BandgapOff(void)
{
    XTALOSC24M->LOWPWR_CTRL_SET = XTALOSC24M_LOWPWR_CTRL_LPBG_SEL_MASK;
    PMU->MISC0_SET              = PMU_MISC0_REFTOP_PWD_MASK;
}

/*!
 * @brief Set CCM MUX node to certain value.
 *
 * @param mux   Which mux node to set, see \ref clock_mux_t.
 * @param value Clock mux value to set, different mux has different value range.
 */
void CLOCK_SET_MUX(clock_mux_t mux, uint32_t value)
{
    uint32_t busyShift;

    busyShift               = CCM_TUPLE_BUSY_SHIFT(mux);
    CCM_TUPLE_REG(CCM, mux) = (CCM_TUPLE_REG(CCM, mux) & (~CCM_TUPLE_MASK(mux))) |
                              (((uint32_t)((value) << CCM_TUPLE_SHIFT(mux))) & CCM_TUPLE_MASK(mux));

    assert(busyShift <= CCM_NO_BUSY_WAIT);

    /* Clock switch need Handshake? */
    if (CCM_NO_BUSY_WAIT != busyShift)
    {
        /* Wait until CCM internal handshake finish. */
        while (CCM->CDHIPR & (1U << busyShift))
        {
        }
    }
}

/*!
 * @brief Set CCM DIV node to certain value.
 *
 * @param divider Which div node to set, see \ref clock_div_t.
 * @param value   Clock div value to set, different divider has different value range.
 */
void CLOCK_SET_DIV(clock_div_t divider, uint32_t value)
{
    uint32_t busyShift;

    busyShift                   = CCM_TUPLE_BUSY_SHIFT(divider);
    CCM_TUPLE_REG(CCM, divider) = (CCM_TUPLE_REG(CCM, divider) & (~CCM_TUPLE_MASK(divider))) |
                                  (((uint32_t)((value) << CCM_TUPLE_SHIFT(divider))) & CCM_TUPLE_MASK(divider));

    assert(busyShift <= CCM_NO_BUSY_WAIT);

    /* Clock switch need Handshake? */
    if (CCM_NO_BUSY_WAIT != busyShift)
    {
        /* Wait until CCM internal handshake finish. */
        while (CCM->CDHIPR & (1U << busyShift))
        {
        }
    }
}

void ClockSelectXtalOsc(void)
{
    /* Enable XTAL 24MHz clock source. */
    CLOCK_InitExternalClk(0);
    /* Wait CCM operation finishes */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* Take some delay */
    SDK_DelayAtLeastUs(40);
    /* Switch clock source to external OSC. */
    CLOCK_SwitchOsc(kCLOCK_XtalOsc);
    /* Turn off XTAL-OSC detector */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK;
    /* Power Down internal RC. */
    CLOCK_DeinitRcOsc24M();
}

void ClockSelectRcOsc(void)
{
    /* Enable internal RC. */
    XTALOSC24M->LOWPWR_CTRL |= XTALOSC24M_LOWPWR_CTRL_RC_OSC_EN_MASK;
    /* Wait CCM operation finishes */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* Take some delay */
    SDK_DelayAtLeastUs(4000);
    /* Switch clock source to internal RC. */
    XTALOSC24M->LOWPWR_CTRL_SET = XTALOSC24M_LOWPWR_CTRL_SET_OSC_SEL_MASK;
    /* Disable XTAL 24MHz clock source. */
    CCM_ANALOG->MISC0_SET = CCM_ANALOG_MISC0_XTAL_24M_PWD_MASK;
}

void LPM_Init(void)
{
    uint32_t i;
    uint32_t tmp_reg = 0;

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
    SDK_DelayAtLeastUs(4000);
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

    /* ERR007265 */
    IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_GINT_MASK;

    /* Initialize GPC to mask all IRQs */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = 0xFFFFFFFFU;
    }
}

void LPM_EnableWakeupSource(uint32_t irq)
{
    GPC_EnableIRQ(GPC, irq);
}

void LPM_DisableWakeupSource(uint32_t irq)
{
    GPC_DisableIRQ(GPC, irq);
}

void LPM_PreEnterWaitMode(void)
{
    g_savedPrimask = DisableGlobalIRQ();
    __DSB();
    __ISB();
}

void LPM_PostExitWaitMode(void)
{
    EnableGlobalIRQ(g_savedPrimask);
    __DSB();
    __ISB();
}

void LPM_PreEnterStopMode(void)
{
    g_savedPrimask = DisableGlobalIRQ();
    __DSB();
    __ISB();
}

void LPM_PostExitStopMode(void)
{
    EnableGlobalIRQ(g_savedPrimask);
    __DSB();
    __ISB();
}

void LPM_SetRunModeConfig(void)
{
    CCM->CLPCR &= ~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK);
}

void LPM_SetWaitModeConfig(void)
{
    uint32_t clpcr;

    /*
     * ERR007265: CCM: When improper low-power sequence is used,
     * the SoC enters low power mode before the ARM core executes WFI.
     *
     * Software workaround:
     * 1) Software should trigger IRQ #41 (GPR_IRQ) to be always pending
     *      by setting IOMUXC_GPR_GPR1_GINT.
     * 2) Software should then unmask IRQ #41 in GPC before setting CCM
     *      Low-Power mode.
     * 3) Software should mask IRQ #41 right after CCM Low-Power mode
     *      is set (set bits 0-1 of CCM_CLPCR).
     */
    GPC_EnableIRQ(GPC, GPR_IRQ_IRQn);
    clpcr      = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));
    CCM->CLPCR = clpcr | CCM_CLPCR_LPM(kCLOCK_ModeWait) | CCM_CLPCR_MASK_SCU_IDLE_MASK | CCM_CLPCR_MASK_L2CC_IDLE_MASK |
                 CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | CCM_CLPCR_STBY_COUNT_MASK | CCM_CLPCR_BYPASS_LPM_HS0_MASK |
                 CCM_CLPCR_BYPASS_LPM_HS1_MASK;
    GPC_DisableIRQ(GPC, GPR_IRQ_IRQn);
}

void LPM_SetStopModeConfig(void)
{
    uint32_t clpcr;

    /*
     * ERR007265: CCM: When improper low-power sequence is used,
     * the SoC enters low power mode before the ARM core executes WFI.
     *
     * Software workaround:
     * 1) Software should trigger IRQ #41 (GPR_IRQ) to be always pending
     *      by setting IOMUXC_GPR_GPR1_GINT.
     * 2) Software should then unmask IRQ #41 in GPC before setting CCM
     *      Low-Power mode.
     * 3) Software should mask IRQ #41 right after CCM Low-Power mode
     *      is set (set bits 0-1 of CCM_CLPCR).
     */
    GPC_EnableIRQ(GPC, GPR_IRQ_IRQn);
    clpcr      = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));
    CCM->CLPCR = clpcr | CCM_CLPCR_LPM(kCLOCK_ModeStop) | CCM_CLPCR_MASK_L2CC_IDLE_MASK | CCM_CLPCR_MASK_SCU_IDLE_MASK |
                 CCM_CLPCR_VSTBY_MASK | CCM_CLPCR_STBY_COUNT_MASK | CCM_CLPCR_SBYOS_MASK |
                 CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | CCM_CLPCR_BYPASS_LPM_HS0_MASK | CCM_CLPCR_BYPASS_LPM_HS1_MASK;
    GPC_DisableIRQ(GPC, GPR_IRQ_IRQn);
}

void LPM_OverDriveRun(void)
{
    /* CCM Mode */
    DCDC_BootIntoCCM(DCDC);
    /* Connect internal the load resistor */
    DCDC->REG1 |= DCDC_REG1_REG_RLOAD_SW_MASK;
    /* Adjust SOC voltage to 1.275V */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);

    /* Enable FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* Connect vdd_high_in and connect vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    BandgapOn();
    EnableRegularLDO();
    DisableWeakLDO();

    ClockSetToOverDriveRun();
}

void LPM_FullSpeedRun(void)
{
    /* CCM Mode */
    DCDC_BootIntoCCM(DCDC);
    /* Connect internal the load resistor */
    DCDC->REG1 |= DCDC_REG1_REG_RLOAD_SW_MASK;
    /* Adjust SOC voltage to 1.275V */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);

    /* Enable FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* Connect vdd_high_in and connect vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    BandgapOn();
    EnableRegularLDO();
    DisableWeakLDO();

    ClockSetToFullSpeedRun();

    /* Adjust SOC voltage to 1.15V */
    DCDC_AdjustTargetVoltage(DCDC, 0xe, 0x1);
}

void LPM_LowSpeedRun(void)
{
    /* CCM Mode */
    DCDC_BootIntoCCM(DCDC);
    /* Connect internal the load resistor */
    DCDC->REG1 |= DCDC_REG1_REG_RLOAD_SW_MASK;
    /* Adjust SOC voltage to 1.275V */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);

    /* Enable FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* Connect vdd_high_in and connect vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    BandgapOn();
    EnableRegularLDO();
    DisableWeakLDO();

    ClockSetToLowSpeedRun();

    /* Adjust SOC voltage to 1.15V */
    DCDC_AdjustTargetVoltage(DCDC, 0xe, 0x1);
}

void LPM_LowPowerRun(void)
{
    ClockSetToLowPowerRun();

    /* Power down USBPHY */
    PowerDownUSBPHY();

    /* Adjust SOC voltage to 0.95V */
    DCDC_AdjustTargetVoltage(DCDC, 0x6, 0x1);
    /* DCM Mode */
    DCDC_BootIntoDCM(DCDC);
    /* Disconnect internal the load resistor */
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;
    /* Power Down output range comparator */
    DCDC->REG0 |= DCDC_REG0_PWD_CMP_OFFSET_MASK;

    /* Enable FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* Connect vdd_high_in and connect vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    EnableWeakLDO();
    DisableRegularLDO();
    BandgapOff();
}

void LPM_EnterSystemIdle(void)
{
    LPM_SetWaitModeConfig();
    SetLowPowerClockGate();

    ClockSetToSystemIdle();

    /* Power down USBPHY */
    PowerDownUSBPHY();

    /* DCDC to 1.15V */
    DCDC_AdjustTargetVoltage(DCDC, 0xe, 0x1);
    /* DCM Mode */
    DCDC_BootIntoDCM(DCDC);
    /* Disconnect internal the load resistor */
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;
    /* Power Down output range comparator */
    DCDC->REG0 |= DCDC_REG0_PWD_CMP_OFFSET_MASK;

    /* Enable FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* Connect vdd_high_in and connect vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    EnableRegularLDO();
    DisableWeakLDO();
    BandgapOn();

    PeripheralEnterDozeMode();

    __DSB();
    __WFI();
    __ISB();
}

void LPM_ExitSystemIdle(void)
{
    PeripheralExitDozeMode();
    LPM_SetRunModeConfig();
}

void LPM_EnterLowPowerIdle(void)
{
    LPM_SetWaitModeConfig();
    SetLowPowerClockGate();

    ClockSetToLowPowerIdle();

    /* Power down USBPHY */
    PowerDownUSBPHY();

    /* Adjust SOC voltage to 0.95V */
    DCDC_AdjustTargetVoltage(DCDC, 0x6, 0x1);
    /* DCM Mode */
    DCDC_BootIntoDCM(DCDC);
    /* Disconnect internal the load resistor */
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;
    /* Power Down output range comparator */
    DCDC->REG0 |= DCDC_REG0_PWD_CMP_OFFSET_MASK;

    /* Enable FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* Connect vdd_high_in and connect vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    EnableWeakLDO();
    DisableRegularLDO();
    BandgapOff();

    PeripheralEnterDozeMode();

    __DSB();
    __WFI();
    __ISB();
}

void LPM_ExitLowPowerIdle(void)
{
    PeripheralExitDozeMode();
    LPM_SetRunModeConfig();
}

void LPM_EnterSuspend()
{
    uint32_t i;
    uint32_t gpcIMR[LPM_GPC_IMR_NUM];

    LPM_SetStopModeConfig();
    SetLowPowerClockGate();

    /* Disconnect internal the load resistor */
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;

    /* Turn off FlexRAM0 */
    GPC->CNTR |= GPC_CNTR_PDRAM0_PGE_MASK;
    /* Turn off FlexRAM1 */
    PGC->MEGA_CTRL |= PGC_MEGA_CTRL_PCR_MASK;

    /* Clean and disable data cache to make sure context is saved into RAM */
    SCB_CleanDCache();
    SCB_DisableDCache();

    /* Adjust LP voltage to 0.925V */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);
    /* Switch DCDC to use DCDC internal OSC */
    DCDC_SetClockSource(DCDC, kDCDC_ClockInternalOsc);

    /* Power down USBPHY */
    PowerDownUSBPHY();

    /* Power down CPU when requested */
    PGC->CPU_CTRL = PGC_CPU_CTRL_PCR_MASK;

    /* Enable FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* Connect vdd_high_in and connect vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;
    /* STOP_MODE config, turn off all analog except RTC in stop mode */
    PMU->MISC0_CLR = PMU_MISC0_STOP_MODE_CONFIG_MASK;

    /* Mask all GPC interrupts before enabling the RBC counters to
     * avoid the counter starting too early if an interupt is already
     * pending.
     */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        gpcIMR[i]   = GPC->IMR[i];
        GPC->IMR[i] = 0xFFFFFFFFU;
    }

    /*
     * ERR006223: CCM: Failure to resuem from wait/stop mode with power gating
     *   Configure REG_BYPASS_COUNTER to 2
     *   Enable the RBC bypass counter here to hold off the interrupts. RBC counter
     *  needs to be no less than 2.
     */
    CCM->CCR = (CCM->CCR & ~CCM_CCR_REG_BYPASS_COUNT_MASK) | CCM_CCR_REG_BYPASS_COUNT(2);
    CCM->CCR |= (CCM_CCR_OSCNT(0xAF) | CCM_CCR_COSC_EN_MASK | CCM_CCR_RBC_EN_MASK);

    /* Now delay for a short while (3usec) at this point
     * so a short loop should be enough. This delay is required to ensure that
     * the RBC counter can start counting in case an interrupt is already pending
     * or in case an interrupt arrives just as ARM is about to assert DSM_request.
     */
    SDK_DelayAtLeastUs(3);

    /* Recover all the GPC interrupts. */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = gpcIMR[i];
    }

    PeripheralEnterStopMode();

    __DSB();
    __WFI();
    __ISB();
}

void LPM_EnterSNVS(void)
{
    SNVS->LPCR |= SNVS_LPCR_TOP_MASK;
    while (1) /* Shutdown */
    {
    }
}
