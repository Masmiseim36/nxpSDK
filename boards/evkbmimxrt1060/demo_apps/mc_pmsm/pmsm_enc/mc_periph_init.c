/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"
#include "fsl_common.h"
#include "mcdrv_enc_qd.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Motor 1 */
/* Structure for current and voltage measurement */
mcdrv_adcetc_t g_sM1AdcSensor;

/* Structure for 3-phase PWM MC driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* Structure for Encoder driver */
mcdrv_qd_enc_t g_sM1Enc;

/* Clock setup structure */
clock_setup_t g_sClockSetup;

extern const clock_arm_pll_config_t armPllConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief   void MCDRV_Init_M1(void)
 *           - Motor control driver main initialization
 *           - Calls initialization functions of peripherals required for motor
 *             control functionality
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_Init_M1(void)
{
    /* Init application clock dependent variables */
    InitClock();

    /* Init ADC */
    M1_MCDRV_ADC_PERIPH_INIT();

    /* Init ADC_ETC */
    InitADC_ETC();

    /* Init TMR1 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();

    /* 6-channel PWM peripheral init for M1 */
    M1_MCDRV_PWM_PERIPH_INIT();

    /* Qudrature decoder peripheral init */
    M1_MCDRV_QD_PERIPH_INIT();

    /* Comparator CMP2 */
    M1_MCDRV_CMP2_INIT();

}

/*!
 * @brief      Core, bus, flash clock setup
 *
 * @param      void
 *
 * @return     none
 */
void InitClock(void)
{
    uint32_t ui32CyclesNumber = 0U;

    /* Calculate clock dependant variables for PMSM control algorithm */
    g_sClockSetup.ui32FastPeripheralClock = CLOCK_GetFreq(kCLOCK_IpgClk);
    g_sClockSetup.ui32SysPllClock = CLOCK_GetFreq(kCLOCK_SysPllClk);
    g_sClockSetup.ui32CpuFrequency = CLOCK_GetFreq(kCLOCK_CpuClk);

    /* Parameters for motor M1 */
    g_sClockSetup.ui16M1PwmFreq   = M1_PWM_FREQ; /* 16 kHz */
    g_sClockSetup.ui16M1PwmModulo = g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16M1PwmFreq;
    ui32CyclesNumber =
        ((M1_PWM_DEADTIME * (g_sClockSetup.ui32FastPeripheralClock / 1000000U)) / 1000U); /* max 2047 cycles */
    g_sClockSetup.ui16M1PwmDeadTime   = ui32CyclesNumber;
    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 1kHz */
}

/*!
 * @brief   void InitADC(void)
 *           - Initialization of the ADC peripheral
 *
 * @param   void
 *
 * @return  none
 */
void InitADC(void)
{
    /* Enable clock to ADC module 1 */
    CLOCK_EnableClock(kCLOCK_Adc1);
    /* Enable clock to ADC module 2 */
    CLOCK_EnableClock(kCLOCK_Adc2);

    /* Single-ended 12-bit conversion (MODE = 0x2) */
    /* Set divide ratio to 2 (ADIV = 0x1) */
    /* 4 samples averaged (AVGS = 0x0) */
    /* IPG clock / 2 (ADICLK = 0x1) */
    /* High speed conversion selected (ADHCS = 0x1) */
    ADC1->CFG = (ADC_CFG_ADICLK(1U) | ADC_CFG_MODE(2U) | ADC_CFG_ADIV(1U) | ADC_CFG_AVGS(0U) | ADC_CFG_ADTRG(0U) | ADC_CFG_ADHSC(1U));
    ADC2->CFG = (ADC_CFG_ADICLK(1U) | ADC_CFG_MODE(2U) | ADC_CFG_ADIV(1U) | ADC_CFG_AVGS(0U) | ADC_CFG_ADTRG(0U) | ADC_CFG_ADHSC(1U));

    /* HW averaging enabled (AVGE = 1) */
    /* One conversion or one set of conversion (ADCO = 0) */
    ADC1->GC = (ADC_GC_AVGE(1U) | ADC_GC_ADCO(0U));
    ADC2->GC = (ADC_GC_AVGE(1U) | ADC_GC_ADCO(0U));

    /* Asynchronous clock output disabled */
    ADC1->GC |= ADC_GC_ADACKEN(0U);
    ADC2->GC |= ADC_GC_ADACKEN(0U);

    /* ------- ADC self calibration procedure start ----- */
    /* Starting the calibration of ADC1 */
    /* Clear the CALF and launch the calibration. */
    ADC1->GS = ADC_GS_CALF_MASK; /* Clear the CALF. */
    ADC1->GC |= ADC_GC_CAL_MASK; /* Launch the calibration. */

    /* Check the status of CALF bit in ADC_GS and the CAL bit in ADC_GC. */
    while (0U != (ADC1->GC & ADC_GC_CAL_MASK))
    {
        /* Check the CALF when the calibration is active. */
        if (0U != (ADC1->GS & ADC_GS_CALF_MASK))
        {
            /* Breakpoint. */
            __asm("BKPT 1");
            break;
        }
    }

    /* Starting the calibration of ADC2 */
    /* Clear the CALF and launch the calibration. */
    ADC2->GS = ADC_GS_CALF_MASK; /* Clear the CALF. */
    ADC2->GC |= ADC_GC_CAL_MASK; /* Launch the calibration. */

    /* Check the status of CALF bit in ADC_GS and the CAL bit in ADC_GC. */
    while (0U != (ADC2->GC & ADC_GC_CAL_MASK))
    {
        /* Check the CALF when the calibration is active. */
        if (0U != (ADC2->GS & ADC_GS_CALF_MASK))
        {
            /* Breakpoint. */
            __asm("BKPT 1");
            break;
        }
    }

    /* ------- ADC self calibration procedure end ----- */
    /* hardware trigger selected */
    ADC1->CFG |= ADC_CFG_ADTRG(1U);
    /* hardware trigger selected */
    ADC2->CFG |= ADC_CFG_ADTRG(1U);

    /* ADC channels controled from ADC_ETC */
    ADC1->HC[0] = ADC_HC_ADCH(0x10U);
    ADC1->HC[1] = ADC_HC_ADCH(0x10U);
    ADC2->HC[0] = ADC_HC_ADCH(0x10U);
    ADC2->HC[1] = ADC_HC_ADCH(0x10U);

    /**************************************
     * motor M1 ADC driver initialization *
     **************************************/
    /* offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;

    /* Phase current measurement */
    /* Sector 1,6 - measured currents Ib & Ic */
    /* ADC1, channel Ib = M1_ADC1_PH_B (ADC_ETC trigger 0, chain 0) */
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaB = M1_ADC1_PH_B;
    /* ADC2, channel Ic = M1_ADC2_PH_C (ADC_ETC trigger 4, chain 0) */
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaC = M1_ADC2_PH_C;

    /* Sector 2,3 - measured currents Ia & Ic*/
    /* ADC1, channel Ia = M1_ADC1_PH_A (ADC_ETC trigger 0, chain 0) */
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaA = M1_ADC1_PH_A;
    /* ADC2, channel Ic = M1_ADC2_PH_C (ADC_ETC trigger 4, chain 0) */
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaC = M1_ADC2_PH_C;

    /* Sector 4,5 - measured currents Ia & Ib */
    /* ADC1, channel Ia = M1_ADC1_PH_A (ADC_ETC trigger 0, chain 0) */
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaA = M1_ADC1_PH_A;
    /* ADC2, channel Ib = M1_ADC2_PH_B (ADC_ETC trigger 4, chain 0) */
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaB = M1_ADC2_PH_B;

    /* UDCbus channel measurement */
    /* ADC1, channel Udcb = M1_ADC1_UDCB (ADC_ETC trigger 0, chain 1) */
    g_sM1AdcSensor.ui16ChanNumVDcb  = M1_ADC1_UDCB;
    /* ADC2, channel AUX (ADC_ETC trigger 4, chain 1) */
    g_sM1AdcSensor.ui16ChanNumAux = 25U;

}

/*!
 * @brief   void InitTMR1(void)
 *           - Initialization of the TMR1 peripheral
 *           - Performs slow control loop counter
 *
 * @param   void
 *
 * @return  none
 */
void InitTMR1(void)
{
    uint16_t ui16SpeedLoopFreq       = g_sClockSetup.ui16M1SpeedLoopFreq;
    uint32_t ui32FastPeripheralClock = g_sClockSetup.ui32FastPeripheralClock;
    uint16_t ui16CompareReg          = (ui32FastPeripheralClock / (16U * ui16SpeedLoopFreq));

    /* Enable clock to TMR1 module */
    CLOCK_EnableClock(kCLOCK_Timer1);

    /* TMR0_CTRL: CM=0,PCS=0,SCS=0,ONCE=0,LENGTH=1,DIR=0,COINIT=0,OUTMODE=0 */
    /* Stop all functions of the timer */
    TMR1->CHANNEL[0].CTRL = 0x20;

    /* TMR0_SCTRL: TCF=0,TCFIE=0,TOF=0,TOFIE=0,IEF=0,IEFIE=0,IPS=0,INPUT=0,
    Capture_Mode=0,MSTR=0,EEOF=0,VAL=0,FORCE=0,OPS=0,OEN=0 */
    TMR1->CHANNEL[0].SCTRL = 0x00;
    TMR1->CHANNEL[0].LOAD  = 0x00; /* Reset load register */

    TMR1->CHANNEL[0].COMP1  = ui16CompareReg; /* Set up compare 1 register */
    TMR1->CHANNEL[0].CMPLD1 = ui16CompareReg; /* Also set the compare preload register */

    /* TMR0_CSCTRL: DBG_EN=0,FAULT=0,ALT_LOAD=0,ROC=0,TCI=0,UP=0,OFLAG=0,TCF2EN=0,TCF1EN=1,
    TCF2=0,TCF1=0,CL2=0,CL1=1 */
    /* Enable compare 1 interrupt and compare 1 preload*/
    TMR1->CHANNEL[0].CSCTRL = 0x41;

    /* Primary Count Source to IP_bus_clk */
    TMR1->CHANNEL[0].CTRL |= TMR_CTRL_PCS(0x0C); /* Frequency = IP_bus clock/PCS */

    /* Reset counter register */
    TMR1->CHANNEL[0].CNTR = 0x00;

    /* Run counter */
    TMR1->CHANNEL[0].CTRL |= TMR_CTRL_CM(0x01);

    /* Enable & setup interrupt from TMR1 */
    NVIC_EnableIRQ(TMR1_IRQn);
    NVIC_SetPriority(TMR1_IRQn, 3U);

}

/*!
 * @brief   void InitPWM_M1(void)
 *           - Initialization of the eFlexPWMA peripheral for motor M1
 *           - 3-phase center-aligned PWM
 *
 * @param   void
 *
 * @return  none
 */
void M1_InitPWM(void)
{
    /* PWM base pointer (affects the entire initialization) */
    PWM_Type *PWMBase = (PWM_Type *)PWM1;

    /* PWM clock gating register: enabled */
    CCM->CCGR4 = (CCM->CCGR4 & ~(CCM_CCGR4_CG8_MASK)) | CCM_CCGR4_CG8(0x3);

    /* Full and Half cycle reload */
    PWMBase->SM[0].CTRL |= PWM_CTRL_FULL_MASK|PWM_CTRL_HALF_MASK;
    PWMBase->SM[1].CTRL |= PWM_CTRL_FULL_MASK|PWM_CTRL_HALF_MASK;
    PWMBase->SM[3].CTRL |= PWM_CTRL_FULL_MASK|PWM_CTRL_HALF_MASK;

    /* Value register initial values, duty cycle 50% */
    PWMBase->SM[0].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[1].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[3].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));

    PWMBase->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[1].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[3].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));

    PWMBase->SM[0].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[1].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[3].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));

    PWMBase->SM[0].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[1].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[3].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));

    PWMBase->SM[0].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[1].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[3].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));

    /* Trigger for ADC synchronization */
    PWMBase->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)((-(g_sClockSetup.ui16M1PwmModulo / 2) + (g_sClockSetup.ui16M1PwmDeadTime/2))));
    PWMBase->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMBase->SM[3].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));

    PWMBase->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[3].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* PWM0 module 0 trigger on VAL4 enabled for ADC synchronization */
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4) | PWM_TCTRL_TRGFRQ(1);

    /* Set dead-time register */
    PWMBase->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[3].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[3].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);

    /* Channels A and B disabled when fault 0 occurs */
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[3].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMBase->SM[3].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));

    /* Modules one and two gets clock from module zero */
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWMBase->SM[3].CTRL2 = (PWMBase->SM[3].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);

    /* Master reload active for modules one and three */
    PWMBase->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMBase->SM[3].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master reload is generated every one opportunity */
    PWMBase->SM[0].CTRL = (PWMBase->SM[0].CTRL & ~PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(M1_FOC_FREQ_VS_PWM_FREQ - 1);

    /* Master sync active for modules one and three*/
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWMBase->SM[3].CTRL2 = (PWMBase->SM[3].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

    /* Fault 0 active in logic level one, automatic clearing */
    PWMBase->FCTRL = (PWMBase->FCTRL & ~PWM_FCTRL_FLVL_MASK) | PWM_FCTRL_FLVL(0x1);
    PWMBase->FCTRL = (PWMBase->FCTRL & ~PWM_FCTRL_FAUTO_MASK) | PWM_FCTRL_FAUTO(0x1);

    /* Clear fault flags */
    PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);

    /* PWMs are re-enabled at PWM full cycle */
    PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFULL_MASK) | PWM_FSTS_FFULL(0x1);

    /* PWM fault filter - 5 Fast peripheral clocks sample rate, 5 agreeing
       samples to activate */
    PWMBase->FFILT = (PWMBase->FFILT & ~PWM_FFILT_FILT_PER_MASK) | PWM_FFILT_FILT_PER(5);
    PWMBase->FFILT = (PWMBase->FFILT & ~PWM_FFILT_FILT_CNT_MASK) | PWM_FFILT_FILT_CNT(5);
       
    /* Start PWMs (set load OK flags and run) */
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0xF);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0xF);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0x0);

    /* Initialize MC driver */
    g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWMBase;

    g_sM1Pwm3ph.ui16PhASubNum = 0U; /* PWMA phase A sub-module number */
    g_sM1Pwm3ph.ui16PhBSubNum = 1U; /* PWMA phase B sub-module number */
    g_sM1Pwm3ph.ui16PhCSubNum = 3U; /* PWMA phase C sub-module number */

    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
}

/*!
 * @brief   void InitADC_ETC(void)
 *           - Initialization of the ADC_ETC peripheral
 *           - Default channel assigment for Sector 2
 *
 * @param   void
 *
 * @return  none
 */
void InitADC_ETC(void)
{

  /****************************************************************************
   *                          ADC1                          ADC2              *
   *                          trigger0   <-synchronized->   trigger4          *
   *                                                                          *
   * chain0 --> (SVM sec16)   Ib (M1_ADC1_PH_B)             Ic (M1_ADC2_PH_C) *
   *            (SVM sec23)   Ia (M1_ADC1_PH_A)             Ic (M1_ADC2_PH_C) *
   *            (SVM sec45)   Ia (M1_ADC1_PH_A)             Ib (M1_ADC2_PH_B) *
   * chain1 -->               Udcb (M1_ADC1_UDCB)           Aux               *
   *                                                                          *
   ****************************************************************************/

    /* Initialize the ADC_ETC. */
    ADC_ETC->CTRL &= ~ADC_ETC_CTRL_SOFTRST_MASK;     /* Reset all ADC_CTRL registers to default value */
    ADC_ETC->CTRL = ADC_ETC_CTRL_TRIG_ENABLE(1U);    /* Enable the external XBAR trigger0 */
    
    
    /* Set the external XBAR trigger0 configuration. */
    /* Configure ADC_ETC_TRG0_CTRL register. */
    assert(0U < ADC_ETC_TRIGn_CTRL_COUNT);
    assert(ADC_ETC_TRIGn_COUNTER_COUNT > 0U);
    ADC_ETC->TRIG[0].TRIGn_CTRL = 
             ADC_ETC_TRIGn_CTRL_TRIG_CHAIN(1U)           |    /* The number of segments inside the trigger chain of TRIG0 */
             ADC_ETC_TRIGn_CTRL_SYNC_MODE_MASK;               /* Trigger synchronization mode enabled */
    
    /*Configure ADC_ETC_TRIG0_CHAIN 0 register.*/
    assert(0U < ADC_ETC_TRIGn_CTRL_COUNT);
    ADC_ETC->TRIG[0].TRIGn_CHAIN_1_0 |= 
             ADC_ETC_TRIGn_CHAIN_1_0_HWTS0(1U)           |    /* Select ADC TRIG0 hardware trigger */
             ADC_ETC_TRIGn_CHAIN_1_0_CSEL0(M1_ADC1_PH_A) |    /* Select ADC channel 0 */
             ADC_ETC_TRIGn_CHAIN_1_0_B2B0_MASK;               /* Enable Back-To-Back (B2B0) */

    /*Configure ADC_ETC_TRIG0_CHAIN 1 register.*/
    assert(0U < ADC_ETC_TRIGn_CTRL_COUNT);
    ADC_ETC->TRIG[0].TRIGn_CHAIN_1_0 |=
             ADC_ETC_TRIGn_CHAIN_1_0_HWTS1(2U)           |    /* Select ADC_TRIG1 hardware trigger */    
             ADC_ETC_TRIGn_CHAIN_1_0_CSEL1(M1_ADC1_UDCB) |    /* Select ADC channel 7 */
             ADC_ETC_TRIGn_CHAIN_1_0_B2B1_MASK;               /* Enable Back-To-Back (B2B1) */

    
    /* Set the external XBAR trigger4 configuration. */
    /* Configure ADC_ETC_TRG4_CTRL register. */
    assert(4U < ADC_ETC_TRIGn_CTRL_COUNT);
    assert(ADC_ETC_TRIGn_COUNTER_COUNT > 4U);
    ADC_ETC->TRIG[4].TRIGn_CTRL = 
             ADC_ETC_TRIGn_CTRL_TRIG_CHAIN(1U);               /* The number of segments inside the trigger chain of TRIG4. */
    
    /* Configure ADC_ETC_TRIG4_CHAIN 0 register.*/
    assert(4U < ADC_ETC_TRIGn_CTRL_COUNT);
    ADC_ETC->TRIG[4].TRIGn_CHAIN_1_0 |= 
             ADC_ETC_TRIGn_CHAIN_1_0_HWTS0(1U)           |    /* Select ADC TRIG0 hardware trigger */  
             ADC_ETC_TRIGn_CHAIN_1_0_CSEL0(M1_ADC2_PH_C) |    /* Select ADC channel 6. */
             ADC_ETC_TRIGn_CHAIN_1_0_B2B0_MASK;               /* Enable Back-To-Back (B2B0) */
    
    /* Configure ADC_ETC_TRIGn_CHAIN 1 register.*/
    assert(4U < ADC_ETC_TRIGn_CTRL_COUNT);
    ADC_ETC->TRIG[4].TRIGn_CHAIN_1_0 |=
             ADC_ETC_TRIGn_CHAIN_1_0_HWTS1(2U)  |             /* Select ADC_TRIG1 hardware trigger. */
             ADC_ETC_TRIGn_CHAIN_1_0_CSEL1(9U)  |             /* Select ADC channel 9. */
             ADC_ETC_TRIGn_CHAIN_1_0_B2B1_MASK  |             /* Enable Back-To-Back (B2B1) */ 
             ADC_ETC_TRIGn_CHAIN_1_0_IE1(1U);                 /* Enable the Done1 interrupt */

    
    /* Enable & setup interrupt from ADC_ETC */
    NVIC_EnableIRQ(ADC_ETC_IRQ0_IRQn);
    NVIC_SetPriority(ADC_ETC_IRQ0_IRQn, 0U);
}

/*!
 * @brief   void InitQD(void)
 *           - Initialization of the Quadrature Encoder peripheral
 *           - performs speed and position sensor processing
 *
 * @param   void
 *
 * @return  none
 */
void M1_InitQD(void)
{
    /* Enable clock to ENC modules */
    CLOCK_EnableClock(kCLOCK_Enc1);

    /* Pass initialization data into encoder driver structure */
    /* encoder position and speed measurement */
    g_sM1Enc.pui32QdBase   = (ENC_Type *)ENC1;
    g_sM1Enc.sTo.fltPGain  = M1_POSPE_TO_KP_GAIN;
    g_sM1Enc.sTo.fltIGain  = M1_POSPE_TO_KI_GAIN;
    g_sM1Enc.sTo.fltThGain = M1_POSPE_TO_THETA_GAIN;
    g_sM1Enc.a32PosMeGain  = M1_POSPE_MECH_POS_GAIN;
    g_sM1Enc.ui16Pp        = M1_MOTOR_PP;
    g_sM1Enc.bDirection    = M1_POSPE_ENC_DIRECTION;
    g_sM1Enc.fltSpdEncMin  = M1_POSPE_ENC_N_MIN;
    g_sM1Enc.ui16PulseNumber = M1_POSPE_ENC_PULSES;
    M1_MCDRV_QD_SET_DIRECTION(&g_sM1Enc);
      
    /* Initialization modulo counter*/
    M1_MCDRV_QD_SET_PULSES(&g_sM1Enc);

    /* Enable modulo counting and revolution counter increment on roll-over */
    ENC1->CTRL2 = (ENC_CTRL2_MOD_MASK | ENC_CTRL2_REVMOD_MASK);
}

/*!
@brief   void InitCMP2(void)
          - Initialization of the comparator module for dc-bus over current
            detection to generate PWM fault

@param   void

@return  none
*/
void InitCMP2(void)
{
    /* Enable clock for CMP module */
    CLOCK_EnableClock(kCLOCK_Acmp2);

    /* Filter - 4 consecutive samples must agree */
    CMP2->CR0 = CMP_CR0_FILTER_CNT(4);

    /* DAC output set to 3.197V ~ 7.73A (for 8.25A scale) */
    /* Reference voltage will be VDD */
    /* Enable DAC */
    CMP2->DACCR = CMP_DACCR_VOSEL(60) | CMP_DACCR_VRSEL_MASK | CMP_DACCR_DACEN_MASK;

    /* Plus is CMP2_IN3 ~ overcurrent pin */
    /* Minus is CMP2_IN7 ~ 6bit reference */
    CMP2->MUXCR = CMP_MUXCR_PSEL(3) | CMP_MUXCR_MSEL(7);

    /* Enable analog comparator */
    CMP2->CR1 = CMP_CR1_EN_MASK;
}
