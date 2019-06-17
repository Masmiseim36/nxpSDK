/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_evkbimxrt1050.h"
#include "fsl_common.h"
#include "fsl_xbara.h"
#include "fsl_adc_etc.h"
#include "mcdrv_enc_qd.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Motor 1 */
/* Structure for current and voltage measurement */
mcdrv_adc_t g_sM1AdcSensor;

/* Structure for 3-phase PWM MC driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* Structure for Encoder driver */
mcdrv_qd_enc_t g_sM1Enc;

/* Structure for Encoder driver slow loop*/
mcdrv_qd_enc_t g_sM1EncSlow;

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
        
    /* init XBAR  */
    InitXBARA();
    
    /* Init ADC_ETC */
    InitADC_ETC();
        
     /* Init TMR1 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();
    
    /* 6-channel PWM peripheral init for M1 */
    M1_MCDRV_PWM_PERIPH_INIT();

    /* Qudrature decoder peripheral init */
    M1_MCDRV_QD_PERIPH_INIT();
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

    /* Parameters for motor M1 */
    g_sClockSetup.ui16M1PwmFreq = M1_PWM_FREQ; /* 10 kHz */
    g_sClockSetup.ui16M1PwmModulo = g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16M1PwmFreq;
    ui32CyclesNumber = ((M1_PWM_DEADTIME * (g_sClockSetup.ui32FastPeripheralClock / 1000000U))/1000U); /* max 2047 cycles */
    g_sClockSetup.ui16M1PwmDeadTime = ui32CyclesNumber;
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
                     
    /* Single-ended 10-bit conversion (MODE = 0x1) */
    /* Set divide ratio to 2 (ADIV = 0x1) */
    /* 4 samples averaged (AVGS = 0x0) */
    /* IPG clock select (ADICLK = 0x0) */
    ADC1->CFG = ( ADC_CFG_ADICLK(0U) | ADC_CFG_MODE(1U) | ADC_CFG_ADIV(1U) | ADC_CFG_AVGS(0U) | ADC_CFG_ADTRG(0U) );
    ADC2->CFG = ( ADC_CFG_ADICLK(0U) | ADC_CFG_MODE(1U) | ADC_CFG_ADIV(1U) | ADC_CFG_AVGS(0U) | ADC_CFG_ADTRG(0U) );

    /* HW averaging disabled (AVGE = 0) */
    /* One conversion or one set of conversion (ADCO = 0) */ 
    ADC1->GC = (ADC_GC_AVGE(0U) | ADC_GC_ADCO(0U));
    ADC2->GC = (ADC_GC_AVGE(0U) | ADC_GC_ADCO(0U));
    
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
    
    /* Set ADC channels for ADC_ETC */
    /* Withouth this setting ADC_ETC not working properly!!! */  
    ADC1->HC[0] = ADC_HC_ADCH(0x01U);
    ADC1->HC[1] = ADC_HC_ADCH(0x02U);
    ADC1->HC[2] = ADC_HC_ADCH(0x03U);
    ADC2->HC[0] = ADC_HC_ADCH(0x04U);
    ADC2->HC[1] = ADC_HC_ADCH(0x05U);
    ADC2->HC[2] = ADC_HC_ADCH(0x06U);
    
    /**************************************/
    /* motor M1 ADC driver initialization */
    /**************************************/
    /* offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;
 
    /* Phase current measurement */
    /* Sector 1,6 - measured currents Ic & Ib */
    /* ADC1, channel Ic = M1_ADC1_PH_C, , RESULT = 0 */
    g_sM1AdcSensor.sCurrSec16.pAdcBasePhaC = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaC = M1_ADC1_PH_C;
    g_sM1AdcSensor.sCurrSec16.ui16RsltRegPhaC = 0U;
    /* ADC2, channel Ib = M1_ADC2_PH_B, RESULT = 0 */
    g_sM1AdcSensor.sCurrSec16.pAdcBasePhaB = (ADC_Type *)ADC2;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaB = M1_ADC2_PH_B;
    g_sM1AdcSensor.sCurrSec16.ui16RsltRegPhaB = 0U;

    /* Sector 2,3 - measured currents Ic & Ia*/
    /* ADC1, channel Ic = M1_ADC1_PH_C, RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.pAdcBasePhaC = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaC = M1_ADC1_PH_C;
    g_sM1AdcSensor.sCurrSec23.ui16RsltRegPhaC = 0U;
    /* ADC2, channel Ia = M1_ADC2_PH_A, RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.pAdcBasePhaA = (ADC_Type *)ADC2;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaA = M1_ADC2_PH_A;
    g_sM1AdcSensor.sCurrSec23.ui16RsltRegPhaA = 0U;

    /* Sector 4,5 - measured currents Ia & Ib */
    /* ADC1, channel Ia = M1_ADC1_PH_A, RESULT = 0 */
    g_sM1AdcSensor.sCurrSec45.pAdcBasePhaA = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaA = M1_ADC1_PH_A;
    g_sM1AdcSensor.sCurrSec45.ui16RsltRegPhaA = 0U;
    /* ADC2, channel Ib = M1_ADC2_PH_B, RESULT = 0 */
    g_sM1AdcSensor.sCurrSec45.pAdcBasePhaB = (ADC_Type *)ADC2;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaB = M1_ADC2_PH_B;
    g_sM1AdcSensor.sCurrSec45.ui16RsltRegPhaB = 0U;
    
    /* UDCbus channel measurement */
    /* ADC1, channel Udcb = M1_ADC1_UDCB, RESULT = 2 */
    g_sM1AdcSensor.pui32UdcbAdcBase = (ADC_Type *)ADC1;
    g_sM1AdcSensor.ui16ChanNumVDcb = M1_ADC1_UDCB;
    g_sM1AdcSensor.ui16RsltRegVDcb = 1U;
    
    /* Assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssignInit(&g_sM1AdcSensor);
    
    /* Enable & setup interrupt from ADC */
    EnableIRQ(ADC1_IRQn);
    NVIC_SetPriority(ADC1_IRQn, 1U); 
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
    uint16_t    ui16SpeedLoopFreq = g_sClockSetup.ui16M1SpeedLoopFreq;
    uint32_t    ui32FastPeripheralClock = g_sClockSetup.ui32FastPeripheralClock;
    uint16_t    ui16CompareReg = (ui32FastPeripheralClock/(16U * ui16SpeedLoopFreq)); 
  
    /* Enable clock to TMR1 module */
    CLOCK_EnableClock(kCLOCK_Timer1);
    
    /* TMR0_CTRL: CM=0,PCS=0,SCS=0,ONCE=0,LENGTH=1,DIR=0,COINIT=0,OUTMODE=0 */
    /* Stop all functions of the timer */
    TMR1->CHANNEL[0].CTRL = 0x20;
    
    /* TMR0_SCTRL: TCF=0,TCFIE=0,TOF=0,TOFIE=0,IEF=0,IEFIE=0,IPS=0,INPUT=0,
    Capture_Mode=0,MSTR=0,EEOF=0,VAL=0,FORCE=0,OPS=0,OEN=0 */
    TMR1->CHANNEL[0].SCTRL = 0x00;    
    TMR1->CHANNEL[0].LOAD = 0x00; /* Reset load register */
    
    TMR1->CHANNEL[0].COMP1 = ui16CompareReg; /* Set up compare 1 register */
    TMR1->CHANNEL[0].CMPLD1 = ui16CompareReg; /* Also set the compare preload register */
    
    /* TMR0_CSCTRL: DBG_EN=0,FAULT=0,ALT_LOAD=0,ROC=0,TCI=0,UP=0,OFLAG=0,TCF2EN=0,TCF1EN=1,
    TCF2=0,TCF1=0,CL2=0,CL1=1 */
    /* Enable compare 1 interrupt and compare 1 preload*/
    TMR1->CHANNEL[0].CSCTRL = 0x41; 
    
    /* Primary Count Source to IP_bus_clk */
    TMR1->CHANNEL[0].CTRL |= TMR_CTRL_PCS(0x0C);  /* Frequency = IP_bus clock/PCS */

    /* Reset counter register */
    TMR1->CHANNEL[0].CNTR = 0x00;
    
    /* Run counter */
    TMR1->CHANNEL[0].CTRL |= TMR_CTRL_CM(0x01);
     
    /* Enable & setup interrupt from TMR1 */
    EnableIRQ(TMR1_IRQn);   
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
  
    /* Full cycle reload */
    PWMBase->SM[0].CTRL |= PWM_CTRL_FULL_MASK;
    PWMBase->SM[1].CTRL |= PWM_CTRL_FULL_MASK;
    PWMBase->SM[3].CTRL |= PWM_CTRL_FULL_MASK;

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
    PWMBase->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)((-(g_sClockSetup.ui16M1PwmModulo / 2) + 10)));
    PWMBase->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMBase->SM[3].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));

    PWMBase->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[3].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* PWM0 module 0 trigger on VAL4 enabled for ADC synchronization */
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4);

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
    PWMBase->FSTS = (PWMBase->FCTRL & ~PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);

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

    g_sM1Pwm3ph.ui16PhASubNum = M1_PWM_PAIR_PHA; /* PWMA phase A sub-module number */
    g_sM1Pwm3ph.ui16PhBSubNum = M1_PWM_PAIR_PHB; /* PWMA phase B sub-module number */
    g_sM1Pwm3ph.ui16PhCSubNum = M1_PWM_PAIR_PHC; /* PWMA phase C sub-module number */

    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
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
    /* Init xbara module. */
    XBARA_Init(XBARA1); 
    
    /* Configure the XBARA signal connections. (set for sync mode in ADC_ETC) */       
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputFlexpwm1Pwm1OutTrig01, kXBARA1_OutputAdcEtcXbar0Trig0);
    
    /* M1 Enc phase A */
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc1PhaseAInput);     
    
    /* M1 Enc phase B */
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc1PhaseBInput);     
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
    adcEtcConfig.XBARtriggerMask = 0x01U; /* Enable the external XBAR trigger0. */
    adcEtcConfig.enableTSCBypass = false; /* To use ADC2, this bit should be cleared. */
    ADC_ETC_Init(ADC_ETC, &adcEtcConfig);

    /* Set the external XBAR trigger0 configuration. */
    adcEtcTriggerConfig.enableSyncMode = true;
    adcEtcTriggerConfig.enableSWTriggerMode = false;
    adcEtcTriggerConfig.triggerChainLength = 1U; 
    adcEtcTriggerConfig.triggerPriority = 0U;
    adcEtcTriggerConfig.sampleIntervalDelay = 0U;
    adcEtcTriggerConfig.initialDelay = 0U;
    
    ADC_ETC_SetTriggerConfig(ADC_ETC, 0U, &adcEtcTriggerConfig);

    adcEtcTriggerChainConfig.enableB2BMode = true;  
       
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 0U; /* Select ADC_HC0 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = 0U; /* ADC_HC0 will be triggered to sample Corresponding channel. (ch.0) */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_InterruptDisable; /* Enable the Done0 interrupt. */   
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 0U, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain0. */
       
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 1U; /* Select ADC_HC1 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = 1U; /* ADC_HC1 will be triggered to sample Corresponding channel. (ch.1) */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_InterruptDisable; /* Enable the Done1 interrupt. */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 1U, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain1. */
    
    /* Set the external XBAR trigger4 configuration. */
    adcEtcTriggerConfig.enableSyncMode = false;
    adcEtcTriggerConfig.enableSWTriggerMode = false;
    adcEtcTriggerConfig.triggerChainLength = 1U; 
    adcEtcTriggerConfig.triggerPriority = 0U;
    adcEtcTriggerConfig.sampleIntervalDelay = 0U;
    adcEtcTriggerConfig.initialDelay = 0U;
    
    ADC_ETC_SetTriggerConfig(ADC_ETC, 4U, &adcEtcTriggerConfig);
 
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 0U; /* Select ADC_HC0 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = 0U; /* ADC_HC0 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_InterruptDisable; /* Enable the Done0 interrupt. */   
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 4U, 0U, &adcEtcTriggerChainConfig); /* Configure the trigger4 chain0. */
     
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 1U; /* Select ADC_HC1 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect = 15U; /* ADC_HC1 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_InterruptDisable; /* Enable the Done1 interrupt. */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 4U, 1U, &adcEtcTriggerChainConfig); /* Configure the trigger4 chain1. */

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
    
    /* Initialization modulo counter to encoder number of pulses * 4 - 1 */
    ENC1->LMOD = (M1_POSPE_ENC_PULSES * 4U) - 1U;
    
    /* Enable modulo counting and revolution counter increment on roll-over */
    ENC1->CTRL2 = (ENC_CTRL2_MOD_MASK | ENC_CTRL2_REVMOD_MASK);
    
    /* Pass initialization data into encoder driver structure */     
    /* encoder position and speed measurement */
    g_sM1Enc.pui32QdBase = (ENC_Type *) ENC1;
    g_sM1Enc.sTo.fltPGain = M1_POSPE_KP_GAIN;
    g_sM1Enc.sTo.fltIGain = M1_POSPE_KI_GAIN;
    g_sM1Enc.sTo.fltThGain = M1_POSPE_INTEG_GAIN;
    g_sM1Enc.a32PosMeGain = M1_POSPE_MECH_POS_GAIN;
    g_sM1Enc.ui16Pp = M1_MOTOR_PP;
    g_sM1Enc.bDirection = M1_POSPE_ENC_DIRECTION;
    g_sM1Enc.fltSpdEncMin = M1_POSPE_ENC_N_MIN;
    MCDRV_QdEncSetDirection(&g_sM1Enc);        
}
