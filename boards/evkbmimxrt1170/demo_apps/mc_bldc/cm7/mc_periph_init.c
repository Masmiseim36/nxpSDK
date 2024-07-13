/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"
#include "fsl_lpadc.h"
#include "fsl_common.h"
#include "fsl_xbara.h"
#include "fsl_adc_etc.h"
#include "mcdrv_cmt_pwma.h"
#include "fsl_acmp.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Commutation table */
const uint16_t bldcCommutationTableComp[16] = {
    /*swap, mask*/
    0x440, 0x040, /* [0] - sector 0 */
    0x220, 0x400, /* [1] - sector 1 */
    0x110, 0x400, /* [2] - sector 2 */
    0x440, 0x004, /* [3] - sector 3 */
    0x220, 0x004, /* [4] - sector 4 */
    0x110, 0x040, /* [5] - sector 5 */
    0x000, 0x440, /* [6] - alignment vector (combination of sectors 0 & 1) */
    0xFF0, 0x000  /* [7] - PWM off */
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Motor 1 */
/* Structure for current and voltage measurement */
mcdrv_adcetc_t g_sM1AdcSensor;

/* Structure for 3-phase PWM MC driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* Structure for time event driver */
mcdrv_cmt_pwma_t g_sM1CmtTmr;

/* Clock setup structure */
clock_setup_t g_sClockSetup;

/* BLDC table for control ADC phase assigment for measurement */
bldc_adc_table_t bldcAdcTable;

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

    /* Init XBAR  */
    InitXBARA();

    /* Init ADC_ETC */
    InitADC_ETC();

    /* Init TMR1 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();

    /* 6-channel PWM peripheral init for M1 */
    M1_MCDRV_PWM_PERIPH_INIT();

    /* Init CMT */
    M1_MCDRV_TMR_CMT_PERIPH_INIT();

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

    /* Calculate clock dependant variables for control algorithm */
    g_sClockSetup.ui32FastPeripheralClock = CLOCK_GetRootClockFreq(kCLOCK_Root_Bus);

    /* Parameters for motor M1 */
    g_sClockSetup.ui16M1PwmFreq   = M1_PWM_FREQ; /* 20 kHz */
                                                 /* PWM module calculated as follows:
                                                  * PWM_MOD = BUS_CLOCK / PWM_FREQUNCY = 40 MHz / 20 kHz = 2000   */
    g_sClockSetup.ui16M1PwmModulo = g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16M1PwmFreq;
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

    lpadc_config_t lpadcConfig;
    lpadc_conv_command_config_t lpadcCommandConfig;
    lpadc_conv_trigger_config_t lpadcTriggerConfig;

    /* Initialize the ADC module. */
    LPADC_GetDefaultConfig(&lpadcConfig);

    lpadcConfig.enableAnalogPreliminary = true;
    lpadcConfig.referenceVoltageSource = kLPADC_ReferenceVoltageAlt2;
    lpadcConfig.powerLevelMode = kLPADC_PowerLevelAlt4;
    lpadcConfig.powerUpDelay = 0U;

    LPADC_Init(LPADC1, &lpadcConfig);
    LPADC_Init(LPADC2, &lpadcConfig);

    /**************************************/
    /*             ADC1                   */
    /**************************************/

    /* BEMF phase C measurement */
    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&lpadcCommandConfig);
    lpadcCommandConfig.channelNumber = M1_ADC1_PH_C_CHNL;
    lpadcCommandConfig.sampleChannelMode = M1_ADC1_PH_C_SIDE;
    lpadcCommandConfig.sampleTimeMode = kLPADC_SampleTimeADCK3;
    lpadcCommandConfig.sampleScaleMode = kLPADC_SamplePartScale;
    LPADC_SetConvCommandConfig(LPADC1, 1U, &lpadcCommandConfig);

    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig);
    lpadcTriggerConfig.targetCommandId = 1U;
    lpadcTriggerConfig.enableHardwareTrigger = true;
    LPADC_SetConvTriggerConfig(LPADC1, 0U, &lpadcTriggerConfig);

    
    
    
    /* DC-bus voltage measurement */
    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&lpadcCommandConfig);
    lpadcCommandConfig.channelNumber = M1_ADC1_UDCB_CHNL;
    lpadcCommandConfig.sampleChannelMode = M1_ADC1_UDCB_SIDE;
    lpadcCommandConfig.sampleTimeMode = kLPADC_SampleTimeADCK3;
    lpadcCommandConfig.sampleScaleMode = kLPADC_SamplePartScale;
    LPADC_SetConvCommandConfig(LPADC1, 2U, &lpadcCommandConfig);
    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig);
    lpadcTriggerConfig.targetCommandId = 2U;
    lpadcTriggerConfig.enableHardwareTrigger = true;
    LPADC_SetConvTriggerConfig(LPADC1, 1U, &lpadcTriggerConfig);
    /**************************************/
    /*             ADC2                   */
    /**************************************/

    /* DC-bus current measurement */
    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&lpadcCommandConfig);
    lpadcCommandConfig.channelNumber = M1_ADC2_IDCB_CHNL;
    lpadcCommandConfig.sampleChannelMode = M1_ADC2_IDCB_SIDE;
    lpadcCommandConfig.sampleTimeMode = kLPADC_SampleTimeADCK3;
    lpadcCommandConfig.sampleScaleMode = kLPADC_SamplePartScale;
    LPADC_SetConvCommandConfig(LPADC2, 1U, &lpadcCommandConfig);
    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig);
    lpadcTriggerConfig.targetCommandId = 1U;
    lpadcTriggerConfig.enableHardwareTrigger = true;
    lpadcCommandConfig.sampleScaleMode = kLPADC_SamplePartScale;
    LPADC_SetConvTriggerConfig(LPADC2, 0U, &lpadcTriggerConfig);
    
    
    
    
    /* Currently measures on same pin as ADC2 channel 4A - DCbus Current */
    /* AUXiliary measurement */
    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&lpadcCommandConfig);
    lpadcCommandConfig.channelNumber = 4U;
    lpadcCommandConfig.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;
    LPADC_SetConvCommandConfig(LPADC2, 2U, &lpadcCommandConfig);
    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig);
    lpadcTriggerConfig.targetCommandId = 2U;
    lpadcTriggerConfig.enableHardwareTrigger = true;
    LPADC_SetConvTriggerConfig(LPADC2, 1U, &lpadcTriggerConfig);
    
    
    
    
    /* Phase current measurement */
    /* Sector 0,3 - measured BEMF phase C*/
    bldcAdcTable.ui16BemfChannelNumber[0] = M1_ADC1_PH_C_CHNL;                        
    bldcAdcTable.ui16BemfChannelMode[0] = M1_ADC1_PH_C_SIDE; 
    bldcAdcTable.ui16BemfChannelNumber[3] = M1_ADC1_PH_C_CHNL;                        
    bldcAdcTable.ui16BemfChannelMode[3] = M1_ADC1_PH_C_SIDE; 
    
    /* Sector 1,4 - measured BEMF phase B*/
    bldcAdcTable.ui16BemfChannelNumber[1] = M1_ADC1_PH_B_CHNL;                        
    bldcAdcTable.ui16BemfChannelMode[1] = M1_ADC1_PH_B_SIDE;
    bldcAdcTable.ui16BemfChannelNumber[4] = M1_ADC1_PH_B_CHNL;                        
    bldcAdcTable.ui16BemfChannelMode[4] = M1_ADC1_PH_B_SIDE;
    
    /* Sector 2,5 - measured BEMF phase A*/
    bldcAdcTable.ui16BemfChannelNumber[2] = M1_ADC1_PH_A_CHNL;                        
    bldcAdcTable.ui16BemfChannelMode[2] = M1_ADC1_PH_A_SIDE;
    bldcAdcTable.ui16BemfChannelNumber[5] = M1_ADC1_PH_A_CHNL;                        
    bldcAdcTable.ui16BemfChannelMode[5] = M1_ADC1_PH_A_SIDE;
    
    /* Assign BEMF phase table for measurement */
    g_sM1AdcSensor.bldcAdcTable = bldcAdcTable;
    g_sM1AdcSensor.ui16Sector = 0U;
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
    EnableIRQ(TMR1_IRQn);
    NVIC_SetPriority(TMR1_IRQn, 3U);

}

/*!
 * @brief   void InitPWM(void)
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
    PWMBase->SM[0].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo) / 2));
    PWMBase->SM[1].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo) / 2));
    PWMBase->SM[2].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo) / 2));
    
    /* Value register of first edge PWM */
    PWMBase->SM[0].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[1].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[2].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    
    /* Value register of value0 */
    PWMBase->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[1].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[2].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    
    /* Value register of second edge PWM */
    PWMBase->SM[0].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[1].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[2].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    
    /* Value register of modulo */
    PWMBase->SM[0].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[1].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[2].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    
    /* PWM channels set as complementary pair */
    PWMBase->SM[0].CTRL2 = (PWMBase->SM[0].CTRL2 & ~PWM_CTRL2_INDEP_MASK) | PWM_CTRL2_INDEP(0x0);
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_INDEP_MASK) | PWM_CTRL2_INDEP(0x0);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_INDEP_MASK) | PWM_CTRL2_INDEP(0x0);
        
    /* The submodules 1 and 2 are synchronized with submodule 0 (Reload on master reload signal - submodule 0) */
    PWMBase->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMBase->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Modules one and two get clock from module zero */
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    
    /* Master reload is generated every one opportunity */
    PWMBase->SM[0].CTRL = (PWMBase->SM[0].CTRL & ~PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(M1_FOC_FREQ_VS_PWM_FREQ - 1);
    
    /* Master sync active for modules one and two*/
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    
    /* Set dead-time register */
    PWMBase->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    
    /* Trigger for ADC synchronization */
    PWMBase->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)((-(g_sClockSetup.ui16M1PwmModulo / 2) + 10)));
    PWMBase->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMBase->SM[2].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));

    PWMBase->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* PWM0 module 0 trigger on VAL4 enabled for ADC synchronization */
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4) | PWM_TCTRL_TRGFRQ(1);
    
    /* Channels A and B disabled when fault 0 occurs */
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));

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
    

    /* Set PWM23 as the source for the generation of the complementary PWM pair output*/
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_IPOL_MASK) | PWM_MCTRL_IPOL(0x0);
    
    
     /* Enable A&B PWM outputs for submodules zero, one and two */
    PWMBase->OUTEN = (PWMBase->OUTEN & ~PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(0x7);
    PWMBase->OUTEN = (PWMBase->OUTEN & ~PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(0x7);
        
    /* Start of PWMs (Manual clear and set LDOK)*/
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0x7);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0x7);
    
    /* Enables the clocks to the PWM generator (modules 2 - 0)*/
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0x7);
        
    /* Initialize MC driver */
    g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWMBase;

    g_sM1Pwm3ph.ui16PhASubNum = 0U; /* PWMA phase A sub-module number */
    g_sM1Pwm3ph.ui16PhBSubNum = 1U; /* PWMA phase B sub-module number */
    g_sM1Pwm3ph.ui16PhCSubNum = 2U; /* PWMA phase C sub-module number */
    
    /* initialization of PWM modulo */
    g_sM1Pwm3ph.ui16PwmModulo = g_sClockSetup.ui16M1PwmModulo;
    
    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
    
    g_sM1Pwm3ph.pcBldcTable = &bldcCommutationTableComp[0]; /* Address of BLDC commutation table*/
}

/*!
 * @brief   void InitXBARA(void)
 *           - Initialization of the XBARA peripheral
 *
 * @param   void
 *
 * @return  none
 */
void InitXBARA(void)
{
    /* Enable clock to XBAR. */
    CLOCK_EnableClock(kCLOCK_Xbar1);

    /* Configure the XBARA signal connections. (set for sync mode in ADC_ETC) */
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputFlexpwm1Pwm0OutTrig0, kXBARA1_OutputAdcEtc0Coco0);

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
    adc_etc_config_t adcEtcConfig;
    adc_etc_trigger_config_t adcEtcTriggerConfig;
    adc_etc_trigger_chain_config_t adcEtcTriggerChainConfig;

    /* Initialize the ADC_ETC. */
    ADC_ETC_GetDefaultConfig(&adcEtcConfig);
    adcEtcConfig.XBARtriggerMask = 1U; /* Enable the external XBAR trigger0. */
    ADC_ETC_Init(ADC_ETC, &adcEtcConfig);

    /* Set the external XBAR trigger0 configuration. */
    adcEtcTriggerConfig.enableSyncMode      = true;
    adcEtcTriggerConfig.enableSWTriggerMode = false;
    adcEtcTriggerConfig.triggerChainLength  = 1U; /* Chain length 1. */
    adcEtcTriggerConfig.triggerPriority     = 0U;
    adcEtcTriggerConfig.sampleIntervalDelay = 0U;
    adcEtcTriggerConfig.initialDelay        = 0U;
    ADC_ETC_SetTriggerConfig(ADC_ETC, 0U, &adcEtcTriggerConfig);

    /* Set the external XBAR trigger0 chain configuration. */
    adcEtcTriggerChainConfig.enableB2BMode       = true;

    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 0U; /* Select ADC_HC0 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = 0U; /* ADC_HC0 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable; /* Enable the Done0 interrupt. */
    adcEtcTriggerChainConfig.enableIrq = false; /* Enable the IRQ. */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 0U, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain0. */

    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 1U; /* Select ADC_HC1 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = 0U; /* ADC_HC1 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable; /* Enable the Done1 interrupt. */
    adcEtcTriggerChainConfig.enableIrq = false; /* Enable the IRQ. */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 1U, &adcEtcTriggerChainConfig); /* Configure the trigger0 group chain 1. */


    /* Set the external XBAR trigger4 configuration. */
    adcEtcTriggerConfig.enableSyncMode      = false;
    adcEtcTriggerConfig.enableSWTriggerMode = false;
    adcEtcTriggerConfig.triggerChainLength  = 1U; /* Chain length 2. */
    adcEtcTriggerConfig.triggerPriority     = 0U;
    adcEtcTriggerConfig.sampleIntervalDelay = 0U;
    adcEtcTriggerConfig.initialDelay        = 0U;
    ADC_ETC_SetTriggerConfig(ADC_ETC, 4U, &adcEtcTriggerConfig);

    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 0U; /* Select ADC_HC0 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = 0U; /* ADC_HC0 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable; /* Enable the Done0 interrupt. */
    adcEtcTriggerChainConfig.enableIrq = false; /* Enable the IRQ. */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 4U, 0U, &adcEtcTriggerChainConfig); /* Configure the trigger4 chain0. */

    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 1U; /* Select ADC_HC1 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = 0U; /* ADC_HC1 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable; /* Enable the Done1 interrupt. */
    adcEtcTriggerChainConfig.enableIrq = true; /* Enable the IRQ. */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 4U, 1U, &adcEtcTriggerChainConfig); /* Configure the trigger4 group chain 1. */

    /* Enable the NVIC. */
    EnableIRQ(ADC_ETC_IRQ0_IRQn);
    NVIC_SetPriority(ADC_ETC_IRQ0_IRQn, 0U);

}

/*!
 * @brief   void InitCMT(void)
 *           - Initialization of the CMT_PWMA Timer
 *           - performs forced commutation control, time event
 *
 * @param   void
 *
 * @return  none
 */
void InitCMT(void)
{
    PWM_Type *PWMBase = (PWM_Type *)PWM1;
    
    /* INIT counter value */
    PWMBase->SM[3].INIT = PWM_INIT_INIT(0x0);
    
    /* Module three gets clock from module zero */
    PWMBase->SM[3].CTRL2 = (PWMBase->SM[3].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    
    /* Prescale factor 40MHz/128 - 312.5kHz */
    PWMBase->SM[3].CTRL = (PWMBase->SM[3].CTRL & ~PWM_CTRL_PRSC_MASK) | PWM_CTRL_PRSC(0x7);
    
    /* Changed registers of submodule three take effect immediately after LDOK being set */
    PWMBase->SM[3].CTRL = (PWMBase->SM[3].CTRL & ~PWM_CTRL_LDMOD_MASK) | PWM_CTRL_LDMOD(0x1);
    
    /* Modulo value of counter */
    PWMBase->SM[3].VAL1 = PWM_VAL1_VAL1(0xFFFF);
    
    /* Value for output compare interrupt */
    PWMBase->SM[3].VAL2 = PWM_VAL2_VAL2(0x5555);
    
    /* Enables flags for compare interrupt */
    PWMBase->SM[3].INTEN = (PWMBase->SM[3].CTRL2 & ~PWM_INTEN_CMPIE_MASK) | PWM_INTEN_CMPIE(0x4);
    
    /* Load values of registers */
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0x8);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0x8);
    
    /* Enable and set priority of interrupt */
    EnableIRQ(PWM1_3_IRQn);
    NVIC_SetPriority(PWM1_3_IRQn, 0U);
    
     /* calculate frequency of timer used for forced commutation
     * System clock divided by 2^FTM_prescaler */
    g_sClockSetup.ui32CmtTimerFreq = ((g_sClockSetup.ui32FastPeripheralClock) >> (0x7));
    
    g_sM1CmtTmr.pui32PwmBase = (PWM_Type *)PWMBase; 
    g_sM1CmtTmr.ui8PwmSubmoduleNumber = 3U;
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
    acmp_config_t acmpConfigStruct;
    acmp_channel_config_t channelConfigStruct;
    acmp_dac_config_t dacConfigStruct;
    acmp_discrete_mode_config_t acmpDiscreteconfig;

    /* Configure ACMP. */
    ACMP_GetDefaultConfig(&acmpConfigStruct);
    acmpConfigStruct.enableInvertOutput = false;
    acmpConfigStruct.enableHighSpeed = true;

    ACMP_Init(CMP3, &acmpConfigStruct);

    /* Configure negative inputs are coming from 3v domain. */
    ACMP_GetDefaultDiscreteModeConfig(&acmpDiscreteconfig);
    acmpDiscreteconfig.enablePositiveChannelDiscreteMode = true;
    acmpDiscreteconfig.enableResistorDivider = true;

    ACMP_SetDiscreteModeConfig(CMP3, &acmpDiscreteconfig);

    /* Configure channel. Select the positive port input from DAC and negative port input from minus mux input. */
    channelConfigStruct.plusMuxInput  = 3U; /* GPIO_AD_30 */
    channelConfigStruct.minusMuxInput = 7U;
    ACMP_SetChannelConfig(CMP3, &channelConfigStruct);

    /* Configure DAC. */
    dacConfigStruct.referenceVoltageSource = kACMP_VrefSourceVin2; /* 3V PAD */
    dacConfigStruct.DACValue = 166U; /* range is 0-255U */
    dacConfigStruct.workMode = kACMP_DACWorkHighSpeedMode;

    ACMP_SetDACConfig(CMP3, &dacConfigStruct);
    ACMP_Enable(CMP3, true);
}

