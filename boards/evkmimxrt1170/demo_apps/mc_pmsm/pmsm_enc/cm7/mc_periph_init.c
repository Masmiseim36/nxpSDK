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

    /* Comparator CMP */
    M1_MCDRV_CMP_INIT();
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
    g_sClockSetup.ui32FastPeripheralClock = CLOCK_GetRootClockFreq(kCLOCK_Root_Bus);
    g_sClockSetup.ui32CpuFrequency = CLOCK_GetRootClockFreq(kCLOCK_Root_M7);

    /* Parameters for motor M1 */
    g_sClockSetup.ui16M1PwmFreq   = M1_PWM_FREQ; /* 16 kHz */
    g_sClockSetup.ui16M1PwmModulo = (g_sClockSetup.ui32FastPeripheralClock) / g_sClockSetup.ui16M1PwmFreq;
    ui32CyclesNumber = ((M1_PWM_DEADTIME * (g_sClockSetup.ui32FastPeripheralClock / 1000000U)) / 1000U);
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
    /* Initialize the ADC1 module. */
    /* Reset all ADC internal logic and registers, except the CTRL register */ 
    LPADC1->CTRL |= ADC_CTRL_RST_MASK;
    LPADC1->CTRL &= ~ADC_CTRL_RST_MASK;
    LPADC1->CTRL |= ADC_CTRL_RSTFIFO_MASK;      /* Reset FIFO conversion */
    
    LPADC1->CTRL &= ~ADC_CTRL_ADCEN_MASK;       /* Disable the module before setting configuration */
    
    /* Configure the module generally. */
    LPADC1->CTRL &= ~ADC_CTRL_DOZEN_MASK;       /* LPADC disabled in Doze mode */
    LPADC1->CFG = ADC_CFG_PWREN_MASK  |         /* Analog pre-enable LPADC */
                  ADC_CFG_REFSEL(1U)  |         /* Reference voltage source selection */
                  ADC_CFG_PWRSEL(3U);           /* Power level selection */
    LPADC1->PAUSE = 0U;                         /* Disable the ADC pausing function */
    LPADC1->FCTRL = ADC_FCTRL_FWMARK(0U);       /* FIFO watermark level selection */
    
    LPADC1->CTRL |= ADC_CTRL_ADCEN_MASK;        /* Enable the module */
    
    
    /* Initialize the ADC2 module. */
    /* Reset all ADC internal logic and registers, except the CTRL register */ 
    LPADC2->CTRL |= ADC_CTRL_RST_MASK;
    LPADC2->CTRL &= ~ADC_CTRL_RST_MASK;
    LPADC2->CTRL |= ADC_CTRL_RSTFIFO_MASK;      /* Reset FIFO conversion */

    LPADC2->CTRL &= ~ADC_CTRL_ADCEN_MASK;       /* Disable the module before setting configuration */
    
    /* Configure the module generally. */
    LPADC2->CTRL &= ~ADC_CTRL_DOZEN_MASK;       /* LPADC disabled in Doze mode */
    LPADC2->CFG = ADC_CFG_PWREN_MASK  |         /* Analog pre-enable LPADC */
                  ADC_CFG_REFSEL(1U)  |         /* Reference voltage source selection */
                  ADC_CFG_PWRSEL(3U);           /* Power level selection */
    LPADC2->PAUSE = 0U;                         /* Disable the ADC pausing function */
    LPADC2->FCTRL = ADC_FCTRL_FWMARK(0U);       /* FIFO watermark level selection */
    
    LPADC2->CTRL |= ADC_CTRL_ADCEN_MASK;        /* Enable the module */
    
    
    /* Phase current measurement */
    /* Sector 1,6 - measured currents Ib & Ic */
    /* ADC1, channel Ib = M1_ADC1_PH_B (ADC_ETC trigger 0, chain 0) */
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaB = M1_ADC1_PH_B_CHNL;
    g_sM1AdcSensor.sCurrSec16.ui16ChanSidePhaB = M1_ADC1_PH_B_SIDE;
    /* ADC2, channel Ic = M1_ADC2_PH_C (ADC_ETC trigger 4, chain 0) */
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaC = M1_ADC2_PH_C_CHNL;
    g_sM1AdcSensor.sCurrSec16.ui16ChanSidePhaC = M1_ADC2_PH_C_SIDE;

    /* Sector 2,3 - measured currents Ia & Ic*/
    /* ADC1, channel Ia = M1_ADC1_PH_A (ADC_ETC trigger 0, chain 0) */
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaA = M1_ADC1_PH_A_CHNL;
    g_sM1AdcSensor.sCurrSec23.ui16ChanSidePhaA = M1_ADC1_PH_A_SIDE;
    /* ADC2, channel Ic = M1_ADC2_PH_C (ADC_ETC trigger 4, chain 0) */
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaC = M1_ADC2_PH_C_CHNL;
    g_sM1AdcSensor.sCurrSec23.ui16ChanSidePhaC = M1_ADC2_PH_C_SIDE;

    /* Sector 4,5 - measured currents Ia & Ib */
    /* ADC1, channel Ia = M1_ADC1_PH_A (ADC_ETC trigger 0, chain 0) */
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaA = M1_ADC1_PH_A_CHNL;
    g_sM1AdcSensor.sCurrSec45.ui16ChanSidePhaA = M1_ADC1_PH_A_SIDE;
    /* ADC2, channel Ib = M1_ADC2_PH_B (ADC_ETC trigger 4, chain 0) */
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaB = M1_ADC2_PH_B_CHNL;
    g_sM1AdcSensor.sCurrSec45.ui16ChanSidePhaB = M1_ADC2_PH_B_SIDE;

    /* UDCbus channel measurement */
    /* ADC1, channel Udcb = M1_ADC1_UDCB (ADC_ETC trigger 0, chain 1) */
    g_sM1AdcSensor.ui16ChanNumVDcb  = M1_ADC1_UDCB_CHNL;
    g_sM1AdcSensor.ui16ChanSideVDcb  = M1_ADC1_UDCB_SIDE;
    /* ADC2, channel AUX (ADC_ETC trigger 4, chain 1) */
    g_sM1AdcSensor.ui16ChanNumAux = 7U;
    g_sM1AdcSensor.ui16ChanSideAux = kLPADC_SampleChannelSingleEndSideB;

        
    /* Default channel assigment for Sector 2 */
    /* The available command number are 1-15, while the index of register group are 0-14. */

    /**************************************/
    /*             ADC1                   */
    /**************************************/
    
    /* Configure the CMD 1. */
    assert(1U < (ADC_CMDL_COUNT + 1U));                         /* Check if the command 1 is available */
    LPADC1->CMD[0].CMDL = ADC_CMDL_ADCH(M1_ADC1_PH_A_CHNL)  |   /* Channel number selection */
                          ADC_CMDL_CSCALE(0U);                  /* Channel scale selection */

    /* Configure the conversion trigger 0 source. */
    assert(0U < ADC_TCTRL_COUNT);                               /* Check if the trigger 0 is available */
    LPADC1->TCTRL[0] = ADC_TCTRL_TCMD(1U)   |                   /* Trigger command select */
                       ADC_TCTRL_HTEN_MASK;                     /* Hardware trigger source enable */
        
    
    /* Configure the CMD 2. */
    assert(2U < (ADC_CMDL_COUNT + 1U));                         /* Check if the command 2 is available */
    LPADC1->CMD[1].CMDL = ADC_CMDL_ADCH(M1_ADC1_UDCB_CHNL)  |   /* Channel number selection */
                          ADC_CMDL_CSCALE(0U)               |   /* Channel scale selection */
                          ADC_CMDL_ABSEL_MASK;                  /* A-side vs. B-side selection */

    /* Configure the conversion trigger 1 source. */
    assert(1U < ADC_TCTRL_COUNT);                               /* Check if the trigger 1 is available */
    LPADC1->TCTRL[1] = ADC_TCTRL_TCMD(2U)   |                   /* Trigger command select */
                       ADC_TCTRL_HTEN_MASK;                     /* Hardware trigger source enable */

    /**************************************/
    /*             ADC2                   */
    /**************************************/
     
    /* Configure the CMD 1. */
    assert(1U < (ADC_CMDL_COUNT + 1U));                         /* Check if the command 1 is available */
    LPADC2->CMD[0].CMDL = ADC_CMDL_ADCH(M1_ADC2_PH_C_CHNL)  |   /* Channel number selection */
                          ADC_CMDL_CSCALE(0U)               |   /* Channel scale selection */
                          ADC_CMDL_ABSEL_MASK;                  /* A-side vs. B-side selection */
    
    /* Configure the conversion trigger 0 source. */
    assert(0U < ADC_TCTRL_COUNT);                               /* Check if the trigger 0 is available */
    LPADC2->TCTRL[0] = ADC_TCTRL_TCMD(1U)   |                   /* Trigger command select */
                       ADC_TCTRL_HTEN_MASK;                     /* Hardware trigger source enable */
    
        
    /* Configure the CMD 2. */
    assert(2U < (ADC_CMDL_COUNT + 1U));                         /* Check if the command 2 is available */
    LPADC2->CMD[1].CMDL = ADC_CMDL_ADCH(7U)                 |   /* Channel number selection */
                          ADC_CMDL_CSCALE(0U)               |   /* Channel scale selection */
                          ADC_CMDL_ABSEL_MASK;                  /* A-side vs. B-side selection */
    
    /* Configure the conversion trigger 1 source. */
    assert(1U < ADC_TCTRL_COUNT);                               /* Check if the trigger 1 is available */
    LPADC2->TCTRL[1] = ADC_TCTRL_TCMD(2U)   |                   /* Trigger command select */
                       ADC_TCTRL_HTEN_MASK;                     /* Hardware trigger source enable */
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

    /* Full cycle reload */
    PWMBase->SM[0].CTRL |= PWM_CTRL_FULL_MASK;
    PWMBase->SM[1].CTRL |= PWM_CTRL_FULL_MASK;
    PWMBase->SM[2].CTRL |= PWM_CTRL_FULL_MASK;

    /* Value register initial values, duty cycle 50% */
    PWMBase->SM[0].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[1].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[2].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));

    PWMBase->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[1].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[2].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));

    PWMBase->SM[0].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[1].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[2].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));

    PWMBase->SM[0].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[1].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[2].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));

    PWMBase->SM[0].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[1].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[2].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));

    /* Trigger for ADC synchronization */
    PWMBase->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)((-(g_sClockSetup.ui16M1PwmModulo / 2) + 10)));
    PWMBase->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMBase->SM[2].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));

    PWMBase->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* PWM0 module 0 trigger on VAL4 enabled for ADC synchronization */
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4);

    /* Set dead-time register */
    PWMBase->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);

    /* Channels A and B disabled when fault 0 occurs */
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));

    /* Modules one and two gets clock from module zero */
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);

    /* Master reload active for modules one and three */
    PWMBase->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMBase->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master reload is generated every one opportunity */
    PWMBase->SM[0].CTRL = (PWMBase->SM[0].CTRL & ~PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(M1_FOC_FREQ_VS_PWM_FREQ - 1);

    /* Master sync active for modules one and three*/
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

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

    /* Enable A&B PWM outputs for submodules zero, one and three */
    PWMBase->OUTEN = (PWMBase->OUTEN & ~PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(0xB);
    PWMBase->OUTEN = (PWMBase->OUTEN & ~PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(0xB);

    /* Start PWMs (set load OK flags and run) */
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0xF);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0xF);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0xF);

    /* Initialize MC driver */
    g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWMBase;

    g_sM1Pwm3ph.ui16PhASubNum = 0U; /* PWMA phase A sub-module number */
    g_sM1Pwm3ph.ui16PhBSubNum = 1U; /* PWMA phase B sub-module number */
    g_sM1Pwm3ph.ui16PhCSubNum = 2U; /* PWMA phase C sub-module number */

    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
}

/*!
 * @brief   void InitADC_ETC(void)
 *           - Initialization of the ADC_ETC peripheral
 *
 * @param   void
 *
 * @return  none
 */
void InitADC_ETC(void)
{
    /* Initialize the ADC_ETC. */
    ADC_ETC->CTRL &= ~ADC_ETC_CTRL_SOFTRST_MASK;     /* Reset all ADC_CTRL registers to default value */
    ADC_ETC->CTRL = ADC_ETC_CTRL_TRIG_ENABLE(1U);    /* Enable the external XBAR trigger0 */

    
    /* Set the external XBAR trigger0 configuration. */
    /* Configure ADC_ETC_TRG0_CTRL register. */
    assert(0U < ADC_ETC_TRIGn_CTRL_COUNT);
    assert(ADC_ETC_TRIGn_COUNTER_COUNT > 0U);   
    ADC_ETC->TRIG[0].TRIGn_CTRL = 
             ADC_ETC_TRIGn_CTRL_TRIG_CHAIN(1U)  |    /* The number of segments inside the trigger chain of TRIG0 */
             ADC_ETC_TRIGn_CTRL_SYNC_MODE_MASK;      /* Trigger synchronization mode enabled */

    /* Configure ADC_ETC_TRIG0_CHAIN 0 register. */
    assert(0U < ADC_ETC_TRIGn_CTRL_COUNT);
    ADC_ETC->TRIG[0].TRIGn_CHAIN_1_0 |=
             ADC_ETC_TRIGn_CHAIN_1_0_HWTS0(1U)  |    /* Select ADC TRIG0 hardware trigger */
             ADC_ETC_TRIGn_CHAIN_1_0_CSEL0(0U)  |    /* ADC hardware trigger command selection */
             ADC_ETC_TRIGn_CHAIN_1_0_B2B0_MASK;      /* Enable Back-To-Back (B2B0) */

    /* Configure ADC_ETC_TRIG0_CHAIN 1 register. */
    assert(0U < ADC_ETC_TRIGn_CTRL_COUNT);
    ADC_ETC->TRIG[0].TRIGn_CHAIN_1_0 |=
             ADC_ETC_TRIGn_CHAIN_1_0_HWTS1(2U)  |    /* Select ADC TRIG1 hardware trigger */
             ADC_ETC_TRIGn_CHAIN_1_0_CSEL1(0U)  |    /* ADC hardware trigger command selection */
             ADC_ETC_TRIGn_CHAIN_1_0_B2B1_MASK;      /* Enable Back-To-Back (B2B1) */


    /* Set the external XBAR trigger4 configuration. */
    /* Configure ADC_ETC_TRG4_CTRL register. */
    assert(4U < ADC_ETC_TRIGn_CTRL_COUNT);
    assert(ADC_ETC_TRIGn_COUNTER_COUNT > 4U);
    ADC_ETC->TRIG[4].TRIGn_CTRL = 
             ADC_ETC_TRIGn_CTRL_TRIG_CHAIN(1U);      /* The number of segments inside the trigger chain of TRIG0 */

    /* Configure ADC_ETC_TRIG4_CHAIN 0 register. */
    assert(4U < ADC_ETC_TRIGn_CTRL_COUNT);
    ADC_ETC->TRIG[4].TRIGn_CHAIN_1_0 |= 
             ADC_ETC_TRIGn_CHAIN_1_0_HWTS0(1U)  |    /* Select ADC TRIG0 hardware trigger */
             ADC_ETC_TRIGn_CHAIN_1_0_CSEL0(0U)  |    /* ADC hardware trigger command selection */
             ADC_ETC_TRIGn_CHAIN_1_0_B2B0_MASK;      /* Enable Back-To-Back (B2B0) */

    /* Configure ADC_ETC_TRIG4_CHAIN 1 register. */
    assert(4U < ADC_ETC_TRIGn_CTRL_COUNT);
    ADC_ETC->TRIG[4].TRIGn_CHAIN_1_0 |=
             ADC_ETC_TRIGn_CHAIN_1_0_HWTS1(2U)  |    /* Select ADC_TRIG1 hardware trigger */
             ADC_ETC_TRIGn_CHAIN_1_0_CSEL1(0U)  |    /* ADC hardware trigger command selection */
             ADC_ETC_TRIGn_CHAIN_1_0_B2B1_MASK  |    /* Enable Back-To-Back (B2B1) */
             ADC_ETC_TRIGn_CHAIN_1_0_IE1_EN_MASK;    /* Enable the Done interrupt */


    /* Enable & setup interrupt from ADC_ETC. */
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
    g_sM1Enc.sTo.fltPGain  = M1_POSPE_KP_GAIN;
    g_sM1Enc.sTo.fltIGain  = M1_POSPE_KI_GAIN;
    g_sM1Enc.sTo.fltThGain = M1_POSPE_INTEG_GAIN;
    g_sM1Enc.a32PosMeGain  = M1_POSPE_MECH_POS_GAIN;
    g_sM1Enc.ui16Pp        = M1_MOTOR_PP;
    g_sM1Enc.bDirection    = M1_POSPE_ENC_DIRECTION;
    g_sM1Enc.fltSpdEncMin  = M1_POSPE_ENC_N_MIN;
    g_sM1Enc.ui16PulseNumber = M1_POSPE_ENC_PULSES;
    MCDRV_QdEncSetDirection(&g_sM1Enc);

    /* Initialization modulo counter*/
    MCDRV_QdEncSetPulses(&g_sM1Enc);

    /* Enable modulo counting and revolution counter increment on roll-over */
    ENC1->CTRL2 = (ENC_CTRL2_MOD_MASK | ENC_CTRL2_REVMOD_MASK);
}

/*!
@brief   void InitCMP(void)
          - Initialization of the comparator module for dc-bus over current
            detection to generate PWM fault

@param   void

@return  none
*/
void InitCMP(void)
{
    /* Configure ACMP. */
    CMP3->C0 &= ~(CMP_C0_EN_MASK | CMP_C0_CFR_MASK | CMP_C0_CFF_MASK);    /* Disable ACMP3 */
    CMP3->C0 |= CMP_C0_PMODE_MASK;       /* Set HS comparison mode */

    /* Configure negative inputs coming from 3v domain. */
    CMP3->C3 |= CMP_C3_RDIVE_MASK    |   /* Resistor divider enable */
                CMP_C3_NCHCTEN_MASK;     /* Enable negative channel continuous mode */

    /* Configure channel. Select the positive port input from DAC and negative port input from minus mux input. */
    CMP3->C1 |= CMP_C1_PSEL(3U)      |   /* Plus Input MUX Control - GPIO_AD_30 */
                CMP_C1_MSEL(7U)      |   /* Minus Input MUX Control */
                CMP_C1_VRSEL(1U)     |   /* Supply voltage reference source select - 3V PAD */
                CMP_C1_VOSEL(166U)   |   /* DAC voltage output select - range is 0-255U */
                CMP_C1_DACEN_MASK    |   /* Enable DAC */
                CMP_C1_DMODE_MASK;       /* Set DAC to work in HS power mode */

    /* Enable ACMP3. */
    CMP3->C0 |= CMP_C0_EN_MASK;
    CMP3->C0 &= ~(CMP_C0_CFR_MASK | CMP_C0_CFF_MASK);        
}