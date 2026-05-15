/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021, 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Structure for 3-phase PWM MC driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
mcdrv_pwm3ph_pwma_t g_sM2Pwm3ph;

/* Structure for current and voltage measurement */
mcdrv_sinc_t g_sM1Curr3phDcBus = { 
    .fltDCBvoltageScale = M1_U_DCB_MAX, \
    .fltCurrentScale = M1_I_MAX, \
    .ui32ENOB = 13U, \
    .ui32OSR = (SINC3_OSR + 1U), \
    .ui32ORD = SINC3_ORD, \
    .ui8ChannelCurrA = 3U, \
    .ui8ChannelCurrB = 2U, \
    .ui8ChannelCurrC = 0U, \
    .ui8ChannelVoltDCB = 1U 
};

mcdrv_sinc_t g_sM2Curr3phDcBus = { 
    .fltDCBvoltageScale = M2_U_DCB_MAX, \
    .fltCurrentScale = M2_I_MAX, \
    .ui32ENOB = 13U, \
    .ui32OSR = (SINC3_OSR + 1U), \
    .ui32ORD = SINC3_ORD, \
    .ui8ChannelCurrA = 3U, \
    .ui8ChannelCurrB = 2U, \
    .ui8ChannelCurrC = 0U, \
    .ui8ChannelVoltDCB = 1U 
};

/* Structure for FlexIO EnDat2.2 driver */
mcdrv_endat2p2_t g_sM1Enc = {
    .endat2Master.flexio =      M1_FLEXIO_INSTANCE,
    .endat2Master.RxdPinIndex = M1_FLEXIO_ENDAT2P2_RXD_CHANNEL,
    .endat2Master.TxdPinIndex = M1_FLEXIO_ENDAT2P2_TXD_CHANNEL,
    .endat2Master.ClkPinIndex = M1_FLEXIO_ENDAT2P2_CLK_CHANNEL,
    .endat2Master.DirPinIndex = M1_FLEXIO_ENDAT2P2_DIR_CHANNEL,
    .endat2Master.txTrigger = kFlexioEndat2_txTriggerSw,
    .endat2Master.triggerInput = 0,
    .endat2Master.mtLen = M1_FLEXIO_ENDAT2P2_MT_LEN,
    .endat2Master.stLen = M1_FLEXIO_ENDAT2P2_ST_LEN,
    .endat2Master.recoveryTimeIII = M1_FLEXIO_ENDAT2P2_RECOVERY_TIME_III
};

mcdrv_endat2p2_t g_sM2Enc = {
    .endat2Master.flexio =      M2_FLEXIO_INSTANCE,
    .endat2Master.RxdPinIndex = M2_FLEXIO_ENDAT2P2_RXD_CHANNEL,
    .endat2Master.TxdPinIndex = M2_FLEXIO_ENDAT2P2_TXD_CHANNEL,
    .endat2Master.ClkPinIndex = M2_FLEXIO_ENDAT2P2_CLK_CHANNEL,
    .endat2Master.DirPinIndex = M2_FLEXIO_ENDAT2P2_DIR_CHANNEL,
    .endat2Master.txTrigger = kFlexioEndat2_txTriggerSw,
    .endat2Master.triggerInput = 0,
    .endat2Master.mtLen = M2_FLEXIO_ENDAT2P2_MT_LEN,
    .endat2Master.stLen = M2_FLEXIO_ENDAT2P2_ST_LEN,
    .endat2Master.recoveryTimeIII = M2_FLEXIO_ENDAT2P2_RECOVERY_TIME_III
};

/* Clock setup structure */
clock_setup_t g_sClockSetup;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief   void MCDRV_Init(void)
 *           - Motor control driver main initialization
 *           - Calls initialization functions of peripherals required for motor
 *             control functionality
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_Init(void)
{
    /* Init application clock dependent variables */
    InitClock();

    /* Init TMR1 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();

    /* 6-channel PWM peripheral init for M1 and M2 */
    M1_MCDRV_PWM_PERIPH_INIT();
    M2_MCDRV_PWM_PERIPH_INIT();
    
    /* Init SINC filters */
    M1_MCDRV_SINC_INIT();
    M2_MCDRV_SINC_INIT();
        
    /* Init EnDat2p2 using FlexIO */
    M1_MCDRV_FLEXIO_ENDAT2P2_INIT();
    M2_MCDRV_FLEXIO_ENDAT2P2_INIT();
    
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
    g_sClockSetup.ui32FastPeripheralClock = CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Wakeup);
    g_sClockSetup.ui32CpuFrequency = CLOCK_GetRootClockFreq(kCLOCK_Root_M7);

    /* Parameters for motor M1 */
    g_sClockSetup.ui16M1PwmFreq   = M1_PWM_FREQ;
    g_sClockSetup.ui16M1PwmModulo = (g_sClockSetup.ui32FastPeripheralClock) / g_sClockSetup.ui16M1PwmFreq;
    ui32CyclesNumber = ((M1_PWM_DEADTIME * (g_sClockSetup.ui32FastPeripheralClock / 1000000U)) / 1000U);
    g_sClockSetup.ui16M1PwmDeadTime   = ui32CyclesNumber;
    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SPEED_LOOP_FREQ;
    
    /* Parameters for motor M1 */
    g_sClockSetup.ui16M2PwmFreq   = M2_PWM_FREQ;
    g_sClockSetup.ui16M2PwmModulo = (g_sClockSetup.ui32FastPeripheralClock) / g_sClockSetup.ui16M2PwmFreq;
    ui32CyclesNumber = ((M2_PWM_DEADTIME * (g_sClockSetup.ui32FastPeripheralClock / 1000000U)) / 1000U);
    g_sClockSetup.ui16M2PwmDeadTime   = ui32CyclesNumber;
    g_sClockSetup.ui16M2SpeedLoopFreq = M1_SPEED_LOOP_FREQ;
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
    
    CLOCK_EnableClock(kCLOCK_Qtimer1); 
    
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
    TMR1->CHANNEL[0].CSCTRL |= TMR_CSCTRL_DBG_EN(1U);

    /* Primary Count Source to IP_bus_clk */
    TMR1->CHANNEL[0].CTRL |= TMR_CTRL_PCS(0x0C); /* Frequency = IP_bus clock/PCS */

    /* Reset counter register */
    TMR1->CHANNEL[0].CNTR = 0x00;

    /* Run counter */
    TMR1->CHANNEL[0].CTRL |= TMR_CTRL_CM(0x01);

    /* Enable & setup interrupt from QTMR1 */
    EnableIRQ(TMR1_IRQn);
    NVIC_SetPriority(TMR1_IRQn, 2U);

}

/*!
 * @brief   void M1_InitPWM(void)
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
    PWM_Type *PWMBase = (PWM_Type *)PWM4;

    /* Full and Half cycle reload */
    PWMBase->SM[0].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    PWMBase->SM[1].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    PWMBase->SM[2].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    
    /* Value register initial values, duty cycle 50% */
    PWMBase->SM[0].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[1].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[2].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));

    PWMBase->SM[0].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[1].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[2].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));

    PWMBase->SM[0].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[1].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[2].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));

    PWMBase->SM[0].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[1].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[2].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));

    /* Trigger for SINC filter measurement (PWM2_SM0_VAL0) ... sampling window centered with PWM2 full cycle reload */      
    PWMBase->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1)) - (SINC3_PRETRIGGER_VAL * SINC3_PWM_CLK_RATION)));
    PWMBase->SM[1].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[2].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    
    /* Trigger for SINC filter measurement (PWM2_SM0_VAL4) ... sampling window centered with PWM2 half cycle reload */ 
    PWMBase->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)(0U - (SINC3_PRETRIGGER_VAL * SINC3_PWM_CLK_RATION)));
    PWMBase->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMBase->SM[2].VAL4 = PWM_VAL4_VAL4((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));

    /* Trigger for sync PWM1 on VAL5 */
    PWMBase->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* PWM sub-module 0 trigger0 on VAL0 and VAL4 enabled for trigger SINC. SINC is in continuous mode. */
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 0) | PWM_TCTRL_OUT_TRIG_EN(1 << 4);
    
    /* Submodule 0 trigger1 on VAL5 enabled for PWM M2 synchronization */
    PWMBase->SM[1].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 5);
    
    /* PWM sub-module 1 trigger0 on VAL0 and VAL4 enabled for trigger Endat2.2. */
    PWMBase->SM[2].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 0) | PWM_TCTRL_OUT_TRIG_EN(1 << 4);
    
    /* Set dead-time register */
    PWMBase->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);

    /* Channels A and B disabled when faults 0 and 1 occur */
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x3));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x3));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x3));
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x3));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x3));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x3));

    /* Modules one and two gets clock from module zero */
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);

    /* Master reload active for modules one and two */
    PWMBase->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMBase->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master sync active for modules one and two*/
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

    /* Fault 0 active in logic level 0, fault 1  active in level 1, automatic clearing */
    PWMBase->FCTRL = (PWMBase->FCTRL & ~PWM_FCTRL_FLVL_MASK) | PWM_FCTRL_FLVL(0x0) | PWM_FCTRL_FLVL(0x2);
    PWMBase->FCTRL = (PWMBase->FCTRL & ~PWM_FCTRL_FAUTO_MASK) | PWM_FCTRL_FAUTO(0x1) | PWM_FCTRL_FAUTO(0x2);

    /* Clear fault flags */
    PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);

    /* PWMs are re-enabled at PWM full cycle */
    PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFULL_MASK) | PWM_FSTS_FFULL(0x1) | PWM_FSTS_FFULL(0x2);

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
    g_sM1Pwm3ph.ui16PhCSubNum = 2U; /* PWMA phase C sub-module number */

    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
    g_sM1Pwm3ph.ui16Fault2FixNum = M1_FAULT_OV_NUM; /* PWMA fixed-value over-voltage fault number */
    
    g_sM1Pwm3ph.ui16Modulo = PWMBase->SM[0].VAL1;
}

/*!
 * @brief   void M2_InitPWM(void)
 *           - Initialization of the eFlexPWMA peripheral for motor M2
 *           - 3-phase center-aligned PWM
 *
 * @param   void
 *
 * @return  none
 */
void M2_InitPWM(void)
{
    /* PWM base pointer (affects the entire initialization) */
    PWM_Type *PWMBase = (PWM_Type *)PWM2;

    /* Full and Half cycle reload */
    PWMBase->SM[0].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    PWMBase->SM[1].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    PWMBase->SM[2].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    
    /* Value register initial values, duty cycle 50% */
    PWMBase->SM[0].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M2PwmModulo / 2)));
    PWMBase->SM[1].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M2PwmModulo / 2)));
    PWMBase->SM[2].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M2PwmModulo / 2)));

    PWMBase->SM[0].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M2PwmModulo / 2) - 1));
    PWMBase->SM[1].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M2PwmModulo / 2) - 1));
    PWMBase->SM[2].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M2PwmModulo / 2) - 1));

    PWMBase->SM[0].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M2PwmModulo / 4)));
    PWMBase->SM[1].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M2PwmModulo / 4)));
    PWMBase->SM[2].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M2PwmModulo / 4)));

    PWMBase->SM[0].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M2PwmModulo / 4));
    PWMBase->SM[1].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M2PwmModulo / 4));
    PWMBase->SM[2].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M2PwmModulo / 4));

    /* Trigger for SINC filter measurement (PWM2_SM0_VAL0) ... sampling window centered with PWM2 full cycle reload */      
    PWMBase->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(((uint16_t)((g_sClockSetup.ui16M2PwmModulo / 2) - 1)) - (SINC2_PRETRIGGER_VAL * SINC2_PWM_CLK_RATION)));
    PWMBase->SM[1].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[2].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    
    /* Trigger for SINC filter measurement (PWM2_SM0_VAL4) ... sampling window centered with PWM2 half cycle reload */ 
    PWMBase->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)(0U - (SINC2_PRETRIGGER_VAL * SINC2_PWM_CLK_RATION)));
    PWMBase->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[2].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));

    PWMBase->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* PWM sub-module 0 trigger0 on VAL0 and VAL4 enabled for trigger SINC. SINC is in continuous mode. */
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 0) | PWM_TCTRL_OUT_TRIG_EN(1 << 4);
    
    /* PWM sub-module 1 trigger0 on VAL0 and VAL4 enabled for trigger EnDat2.2. */
    PWMBase->SM[1].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 0) | PWM_TCTRL_OUT_TRIG_EN(1 << 4);
    
    /* Set dead-time register */
    PWMBase->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M2PwmDeadTime);
    PWMBase->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M2PwmDeadTime);
    PWMBase->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M2PwmDeadTime);
    PWMBase->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M2PwmDeadTime);
    PWMBase->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M2PwmDeadTime);
    PWMBase->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M2PwmDeadTime);

    /* Channels A and B disabled when faults 0 and 1 occur */
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x3));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x3));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x3));
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x3));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x3));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x3));

    /* Modules one and two gets clock from module zero */
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);

    /* External synchronization for submodule 0 from PWM M1 */
    PWMBase->SM[0].CTRL2 = (PWMBase->SM[0].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x3);
    
    /* Master reload active for modules one and two */
    PWMBase->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMBase->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master sync active for modules one and two*/
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

    /* Fault 0 active in logic level 0, fault 1  active in level 1, automatic clearing */
    PWMBase->FCTRL = (PWMBase->FCTRL & ~PWM_FCTRL_FLVL_MASK) | PWM_FCTRL_FLVL(0x0) | PWM_FCTRL_FLVL(0x2);
    PWMBase->FCTRL = (PWMBase->FCTRL & ~PWM_FCTRL_FAUTO_MASK) | PWM_FCTRL_FAUTO(0x1) | PWM_FCTRL_FAUTO(0x2);

    /* Clear fault flags */
    PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);

    /* PWMs are re-enabled at PWM full cycle */
    PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFULL_MASK) | PWM_FSTS_FFULL(0x1) | PWM_FSTS_FFULL(0x2);

    /* PWM fault filter - 5 Fast peripheral clocks sample rate, 5 agreeing
       samples to activate */
    PWMBase->FFILT = (PWMBase->FFILT & ~PWM_FFILT_FILT_PER_MASK) | PWM_FFILT_FILT_PER(5);
    PWMBase->FFILT = (PWMBase->FFILT & ~PWM_FFILT_FILT_CNT_MASK) | PWM_FFILT_FILT_CNT(5);
       
    /* Start PWMs (set load OK flags and run) */
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0xF);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0xF);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0x0);
    
    /* Initialize MC driver */
    g_sM2Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWMBase;

    g_sM2Pwm3ph.ui16PhASubNum = 0U; /* PWMA phase A sub-module number */
    g_sM2Pwm3ph.ui16PhBSubNum = 1U; /* PWMA phase B sub-module number */
    g_sM2Pwm3ph.ui16PhCSubNum = 2U; /* PWMA phase C sub-module number */

    g_sM2Pwm3ph.ui16FaultFixNum = M2_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM2Pwm3ph.ui16FaultAdjNum = M2_FAULT_NUM; /* PWMA adjustable over-current fault number */
    g_sM2Pwm3ph.ui16Fault2FixNum = M2_FAULT_OV_NUM; /* PWMA fixed-value over-voltage fault number */
    
    g_sM2Pwm3ph.ui16Modulo = PWMBase->SM[0].VAL1;
}

/*!
 * @brief   void M1_InitSinc(void)
 *           - Initialization of the SINC peripheral for motor M1
 *           - SINC filter - phase currents measurements
 *
 * @param   void
 *
 * @return  none
 */
void M1_InitSinc(void)
{
    uint32_t u32ChannelId;    
    SINC_Type *SINCBase = (SINC_Type *)SINC3;

    /* Configure MCLKOUT0 to hal_clock_buswakeup / (3+1) = 132000000/4 */       //CLK1 - 16.5MHz, CLK0 - 33MHz
    SINCBase->MCR = SINC_MCR_PRESCALE(0U) | SINC_MCR_MCLKDIV(3U);

    /* Disable MCLKOUT 0 & 2 */
    SINCBase->MCR |= SINC_MCR_MCLK0DIS(1U) | SINC_MCR_MCLK2DIS(1U);

    /* Continuous conversion mode, set filter order, set oversampling ratio */
    SINCBase->CHANNEL[0].CDR =  SINC_CDR_PFCM(0U) | SINC_CDR_PFORD(SINC3_ORD) |	SINC_CDR_PFOSR(SINC3_OSR);
    SINCBase->CHANNEL[1].CDR =  SINC_CDR_PFCM(0U) | SINC_CDR_PFORD(SINC3_ORD) |	SINC_CDR_PFOSR(SINC3_OSR);
    SINCBase->CHANNEL[2].CDR =  SINC_CDR_PFCM(0U) | SINC_CDR_PFORD(SINC3_ORD) |	SINC_CDR_PFOSR(SINC3_OSR);
    SINCBase->CHANNEL[3].CDR =  SINC_CDR_PFCM(0U) | SINC_CDR_PFORD(SINC3_ORD) |	SINC_CDR_PFOSR(SINC3_OSR);

    /* Edge triggering, external bitstream from the MBIT[n], sample on clock negative edge, set FIFO watermark, left justified, signed, left shift of 4 bits */     
    SINCBase->CHANNEL[0].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) | SINC_CCFR_FIFOWMK(0U) | SINC_CCFR_RDFMT(0U) | SINC_CCFR_PFSFT(0x14U) | SINC_CCFR_ICSEL(1U) | SINC_CCFR_ITSEL(1U); /* MCLK_OUT1 clock output with internal loopback, HW trigger conversion */
    SINCBase->CHANNEL[1].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) | SINC_CCFR_FIFOWMK(0U) | SINC_CCFR_RDFMT(1U) | SINC_CCFR_PFSFT(0x14U) | SINC_CCFR_ICSEL(7U) | SINC_CCFR_ITSEL(3U); /* Tie the clock internally to adjacent channel, grouped trigger shared with adjacent channel */
    SINCBase->CHANNEL[2].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) | SINC_CCFR_FIFOWMK(0U) | SINC_CCFR_RDFMT(0U) | SINC_CCFR_PFSFT(0x14U) | SINC_CCFR_ICSEL(7U) | SINC_CCFR_ITSEL(3U);
    SINCBase->CHANNEL[3].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) | SINC_CCFR_FIFOWMK(0U) | SINC_CCFR_RDFMT(0U) | SINC_CCFR_PFSFT(0x14U) | SINC_CCFR_ICSEL(7U) | SINC_CCFR_ITSEL(3U);
    
    /* Primary filter enable, channel enable, FIFO enable */ 
    SINCBase->CHANNEL[0].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);		
    SINCBase->CHANNEL[1].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);
    SINCBase->CHANNEL[2].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);
    SINCBase->CHANNEL[3].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);

    /* Enable Data Output Ready interrupt CHFIE0 */
    SINCBase->NIE |= (1U << SINC_NIE_CHFIE0_SHIFT);
    
    /* Enable SINC1 interrupt */
    NVIC_SetPriority(SINC3_CH0_CH1_CH2_CH3_IRQn, 0);
    NVIC_EnableIRQ(SINC3_CH0_CH1_CH2_CH3_IRQn);
    
    /* Set base pointer used in the SINC MC driver */
    g_sM1Curr3phDcBus.pui32SincBaseAddress = (SINC_Type *)SINCBase;

    /* Master enable */
    SINCBase->MCR |= SINC_MCR_MEN(1U);

    /* Wait for the channel ready flags */
    for( u32ChannelId = 0U; u32ChannelId  < SINC3_NUM_CHANNELS_USED; ++u32ChannelId )
    {
    	while( 0U == (SINCBase->SR & (1U << (u32ChannelId + SINC_SR_CHRDY0_SHIFT)) ) )
    	{}
    }
    
}

/*!
 * @brief   void M2_InitSinc(void)
 *           - Initialization of the SINC peripheral for motor M2
 *           - SINC filter - phase currents measurements
 *
 * @param   void
 *
 * @return  none
 */
void M2_InitSinc(void)
{
  
    SINC_Type *SINC2Base = (SINC_Type *)SINC2;

    /* Configure MCLKOUT0 to hal_clock_buswakeup / (7+1) = 132000000/8 */
    SINC2Base->MCR = SINC_MCR_PRESCALE(0U) | SINC_MCR_MCLKDIV(7U);

    /* Disable MCLKOUT 0 & 2 */
    SINC2Base->MCR |= SINC_MCR_MCLK1DIS(1U) | SINC_MCR_MCLK2DIS(1U);

    /* Continuous conversion mode, set filter order, set oversampling ratio */
    SINC2Base->CHANNEL[1].CDR =  SINC_CDR_PFCM(0U) | SINC_CDR_PFORD(SINC3_ORD) |	SINC_CDR_PFOSR(SINC3_OSR);
    SINC2Base->CHANNEL[2].CDR =  SINC_CDR_PFCM(0U) | SINC_CDR_PFORD(SINC3_ORD) |	SINC_CDR_PFOSR(SINC3_OSR);
    SINC2Base->CHANNEL[3].CDR =  SINC_CDR_PFCM(0U) | SINC_CDR_PFORD(SINC3_ORD) |	SINC_CDR_PFOSR(SINC3_OSR);

    /* Edge triggering, external bitstream from the MBIT[n], sample on clock negative edge, set FIFO watermark, left justified, signed, left shift of 4 bits */     
    SINC2Base->CHANNEL[1].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) | SINC_CCFR_FIFOWMK(0U) | SINC_CCFR_RDFMT(1U) | SINC_CCFR_PFSFT(0x14U) | SINC_CCFR_ICSEL(0U) | SINC_CCFR_ITSEL(1U); /* Tie the clock internally to adjacent channel, grouped trigger shared with adjacent channel */
    SINC2Base->CHANNEL[2].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) | SINC_CCFR_FIFOWMK(0U) | SINC_CCFR_RDFMT(0U) | SINC_CCFR_PFSFT(0x14U) | SINC_CCFR_ICSEL(7U) | SINC_CCFR_ITSEL(3U);
    SINC2Base->CHANNEL[3].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) | SINC_CCFR_FIFOWMK(0U) | SINC_CCFR_RDFMT(0U) | SINC_CCFR_PFSFT(0x14U) | SINC_CCFR_ICSEL(7U) | SINC_CCFR_ITSEL(3U);
    
    /* Primary filter enable, channel enable, FIFO enable */ 	
    SINC2Base->CHANNEL[1].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);
    SINC2Base->CHANNEL[2].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);
    SINC2Base->CHANNEL[3].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);

    /* Enable Data Output Ready interrupt CHFIE0 */
    SINC2Base->NIE |= (1U << SINC_NIE_CHFIE1_SHIFT);
    
    /* Enable SINC1 interrupt */
    NVIC_SetPriority(SINC2_CH1_IRQn, 0);
    NVIC_EnableIRQ(SINC2_CH1_IRQn);
    
    /* Set base pointer used in the SINC MC driver */
    g_sM2Curr3phDcBus.pui32SincBaseAddress = (SINC_Type *)SINC2Base;

    /* Master enable */
    SINC2Base->MCR |= SINC_MCR_MEN(1U);
    
    
    while( 0U == (SINC2Base->SR & (1U << (1U + SINC_SR_CHRDY0_SHIFT)) ) )
    {}

    while( 0U == (SINC2Base->SR & (1U << (2U + SINC_SR_CHRDY0_SHIFT)) ) )
    {}

    while( 0U == (SINC2Base->SR & (1U << (3U + SINC_SR_CHRDY0_SHIFT)) ) )
    {}
    
}


/*!
 * @brief      Init FlexIO EnDat2.2 for M1
 *
 * @param      void
 *
 * @return     none
 */
void M1_InitFlexIOEndat2p2(void)
{
  
    clock_root_config_t rootCfg = {0};
    rootCfg.mux = M1_FLEXIO_CLOCK_ROOT_MUX;
    rootCfg.div = 2;     /* Set Flexio clk to 120Mhz */
    CLOCK_SetRootClock(M1_FLEXIO_CLOCK_ROOT, &rootCfg);
  
    flexio_endat2_config_t config;
    FLEXIO_ENDAT2_GetDefaultConfig(&config);
    config.baudRate_Bps = M1_FLEXIO_ENDAT2P2_BIT_RATE;
    uint32_t srcClock_Hz = CLOCK_GetRootClockFreq(M1_FLEXIO_CLOCK_ROOT);
    uint32_t baudRate = M1_FLEXIO_ENDAT2P2_BIT_RATE;  
    
    while (FLEXIO_ENDAT2_FlexIO_BuadrateVerify(srcClock_Hz, baudRate) != kStatus_Success) {
        baudRate++;
    }

    if (baudRate != config.baudRate_Bps) {
        config.baudRate_Bps = baudRate;
    }

    g_sM1Enc.endat2Master.txTrigger = kFlexioEndat2_txTriggerHw;
    FLEXIO_ENDAT2_Init(&g_sM1Enc.endat2Master, &config, CLOCK_GetRootClockFreq(M1_FLEXIO_CLOCK_ROOT));
    FLEXIO_ENDAT2_WriteCmd(&g_sM1Enc.endat2Master, kFlexIO_ENDAT2_CmdIdxEncSendPosVal);
    FLEXIO_ENDAT2_EnableInterrupts(&g_sM1Enc.endat2Master);
    
    //Trigger extension   
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL1 |= BLK_CTRL_WAKEUPMIX_XBAR_TRIG_SYNC_CTRL1_SYNC_ENABLE(1U);
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL2 |= BLK_CTRL_WAKEUPMIX_XBAR_TRIG_SYNC_CTRL2_PULSE_WIDTH0(7U);
    
    g_sM1Enc.ui16Pp = M1_MOTOR_PP;
    g_sM1Enc.ui16SpeedLoopFreq = M1_SPEED_LOOP_FREQ;
    
    EnableIRQ(M1_FLEXIO_IRQn);
    NVIC_SetPriority(M1_FLEXIO_IRQn, 0U);

}

/*!
 * @brief      Init FlexIO EnDat2.2 for M2
 *
 * @param      void
 *
 * @return     none
 */
void M2_InitFlexIOEndat2p2(void)
{
  
    clock_root_config_t rootCfg = {0};
    rootCfg.mux = M2_FLEXIO_CLOCK_ROOT_MUX;
    rootCfg.div = 2;     /* Set Flexio clk to 120Mhz */
    CLOCK_SetRootClock(M2_FLEXIO_CLOCK_ROOT, &rootCfg);
  
    flexio_endat2_config_t config;
    FLEXIO_ENDAT2_GetDefaultConfig(&config);
    config.baudRate_Bps = M2_FLEXIO_ENDAT2P2_BIT_RATE;
    uint32_t srcClock_Hz = CLOCK_GetRootClockFreq(M2_FLEXIO_CLOCK_ROOT);
    uint32_t baudRate = M2_FLEXIO_ENDAT2P2_BIT_RATE;  
    
    while (FLEXIO_ENDAT2_FlexIO_BuadrateVerify(srcClock_Hz, baudRate) != kStatus_Success) {
        baudRate++;
    }

    if (baudRate != config.baudRate_Bps) {
        config.baudRate_Bps = baudRate;
    }

    g_sM2Enc.endat2Master.txTrigger = kFlexioEndat2_txTriggerHw;
    FLEXIO_ENDAT2_Init(&g_sM2Enc.endat2Master, &config, CLOCK_GetRootClockFreq(M2_FLEXIO_CLOCK_ROOT));
    FLEXIO_ENDAT2_WriteCmd(&g_sM2Enc.endat2Master, kFlexIO_ENDAT2_CmdIdxEncSendPosVal);
    FLEXIO_ENDAT2_EnableInterrupts(&g_sM2Enc.endat2Master);
    
    //Trigger extension   
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL1 |= BLK_CTRL_WAKEUPMIX_XBAR_TRIG_SYNC_CTRL1_SYNC_ENABLE(2U);
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL2 |= BLK_CTRL_WAKEUPMIX_XBAR_TRIG_SYNC_CTRL2_PULSE_WIDTH1(7U);
    
    g_sM2Enc.ui16Pp = M2_MOTOR_PP;
    g_sM2Enc.ui16SpeedLoopFreq = M1_SPEED_LOOP_FREQ;
    
    EnableIRQ(M2_FLEXIO_IRQn);
    NVIC_SetPriority(M2_FLEXIO_IRQn, 0U);

}
