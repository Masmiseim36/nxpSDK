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
#include "fsl_xbar.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Structure for current and voltage measurement*/
mcdrv_adc_t g_sM1AdcSensor;

/* Structure for 3-phase PWM MC driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* Structure for Encoder driver */
mcdrv_eqd_enc_t g_sM1Enc;

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

    /* Init TMR1 (slow loop counter) */
    //M1_MCDRV_TMR_SLOWLOOP_INIT();

    /* 6-channel PWM peripheral init for M1 */
    M1_MCDRV_PWM_PERIPH_INIT();

    /* Qudrature decoder peripheral init */
    M1_MCDRV_QD_PERIPH_INIT();

    /* Comparator CMP */
    //M1_MCDRV_CMP_INIT();
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
    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;

    /* Init ADC clock */
    clock_root_config_t adc1ClkRoot;
    adc1ClkRoot.mux      = 1U;
    adc1ClkRoot.div      = 7U;
    adc1ClkRoot.clockOff = false;
    CLOCK_SetRootClock(kCLOCK_Root_Adc1, &adc1ClkRoot);
    CLOCK_SetRootClock(kCLOCK_Root_Adc2, &adc1ClkRoot);

    LPADC_GetDefaultConfig(&mLpadcConfigStruct);

    mLpadcConfigStruct.enableAnalogPreliminary = true;
    mLpadcConfigStruct.referenceVoltageSource = kLPADC_ReferenceVoltageAlt1;
    mLpadcConfigStruct.conversionAverageMode = kLPADC_ConversionAverage1;
    mLpadcConfigStruct.FIFO1Watermark = 1U;

    LPADC_Init(ADC1, &mLpadcConfigStruct);
    LPADC_Init(ADC2, &mLpadcConfigStruct);

    LPADC_SetOffsetCalibrationMode(ADC1, kLPADC_OffsetCalibration16bitMode);
    LPADC_DoOffsetCalibration(ADC1);
    LPADC_DoAutoCalibration(ADC1);

    LPADC_SetOffsetCalibrationMode(ADC2, kLPADC_OffsetCalibration16bitMode);
    LPADC_DoOffsetCalibration(ADC2);
    LPADC_DoAutoCalibration(ADC2);

    /* *********************************************************************************
     *  ADC1                                                                           *
     *                  FIFO0                            FIFO1                         *
     *  Conversion 1    I_A   - ADC1_A5  GPIO_AD_06      I_B    - ADC1_B5  GPIO_AD_07  *
     *  Conversion 2                                     UDCBus - ADC1_B4  GPIO_AD_09  *
     *                                                                                 *
     *  ADC2                                                                           *
     *                  FIFO0                            FIFO1                         *
     *  Conversion 1    I_C   - ADC2_A2  GPIO_AD_22      I_A    - ADC2_B1  GPIO_AD_21  *
     *                                                                                 *
     **********************************************************************************/

    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.hardwareAverageMode = kLPADC_HardwareAverageCount1;
    mLpadcCommandConfigStruct.sampleTimeMode = kLPADC_SampleTimeADCK3;
    mLpadcCommandConfigStruct.sampleScaleMode = kLPADC_SamplePartScale;
    mLpadcCommandConfigStruct.enableWaitTrigger = FALSE;

    /* ADC1 */
     /* Set conversion CMD1 configuration. */
    mLpadcCommandConfigStruct.channelNumber = M1_ADC1_PH_A; //ADC1_A5 (CUR_A) and ADC1_B5 (CUR_B)
    mLpadcCommandConfigStruct.chainedNextCommandNumber = 2U; /* Next execuited CMD will be CMD2 */
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelDualSingleEndBothSide; /* two simultaneous conversions Channel A and Channel B */
    LPADC_SetConvCommandConfig(ADC1, 1U, &mLpadcCommandConfigStruct); //CONFIGURE CMD 1 (CMDL1, CMDH1)

    /* Set conversion CMD2 configuration. */
    mLpadcCommandConfigStruct.channelNumber = M1_ADC1_UDCB; //ADC1_B4 (UDCBus)
    mLpadcCommandConfigStruct.chainedNextCommandNumber = 0U; /* This was last execuited command */
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelDualSingleEndBothSide;
    LPADC_SetConvCommandConfig(ADC1, 2U, &mLpadcCommandConfigStruct);  /* Configure the CMD 2 */ //CONFIGURE CMD 2 (CMDL2, CMDH2)

    /* ADC2 */
    mLpadcCommandConfigStruct.channelNumber = M1_ADC2_PH_C; //ADC2_A2 (CUR_C)
    mLpadcCommandConfigStruct.chainedNextCommandNumber = 0U; /* Next execuited CMD will be CMD2 */
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelDualSingleEndBothSide; /* two simultaneous conversions Channel A and Channel B */
    LPADC_SetConvCommandConfig(ADC2, 1U, &mLpadcCommandConfigStruct); //CONFIGURE CMD 1 (CMDL1, CMDH1)

    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = 1U; /* CMD1 is executed. */ //SET CMD CALLED FROM TRIGGER
    mLpadcTriggerConfigStruct.enableHardwareTrigger = 1U; //HW trigger ON
    mLpadcTriggerConfigStruct.channelAFIFOSelect = 0U; //Channels A store to FIFO0
    mLpadcTriggerConfigStruct.channelBFIFOSelect = 1U; //Channels B store to FIFO1
    mLpadcTriggerConfigStruct.delayPower = 0U;
    mLpadcTriggerConfigStruct.priority = 0U; //Trigger priority highest
    LPADC_SetConvTriggerConfig(ADC1, 0U, &mLpadcTriggerConfigStruct); /* Configurate the trigger0. */
    LPADC_SetConvTriggerConfig(ADC2, 4U, &mLpadcTriggerConfigStruct); /* Configurate the trigger4. */

    /* Offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;

    /* Enable the watermark interrupt. */
    LPADC_EnableInterrupts(ADC1, kLPADC_FIFO1WatermarkInterruptEnable);
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

    /* Enable & setup interrupt from TMR1 */
    EnableIRQ(TMR1_IRQn);
    NVIC_SetPriority(TMR1_IRQn, 2U);

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

    PWMBase->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)((-(g_sClockSetup.ui16M1PwmModulo / 2) + 10)));
    PWMBase->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* PWM0 module 0 trigger on VAL4 enabled for ADC synchronization */
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4) | PWM_TCTRL_TRGFRQ(1);
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 5) | PWM_TCTRL_TRGFRQ(1);

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

    /* Master reload active for modules one and two */
    PWMBase->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMBase->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master reload is generated every one opportunity */
    PWMBase->SM[0].CTRL = (PWMBase->SM[0].CTRL & ~PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(M1_FOC_FREQ_VS_PWM_FREQ - 1);

    /* Master sync active for modules one and two*/
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

    /* Initialize MC driver */
    g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWMBase;

    g_sM1Pwm3ph.ui16PhASubNum = 0U; /* PWMA phase A sub-module number */
    g_sM1Pwm3ph.ui16PhBSubNum = 1U; /* PWMA phase B sub-module number */
    g_sM1Pwm3ph.ui16PhCSubNum = 2U; /* PWMA phase C sub-module number */

    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
}

void M1_OpenPWM(void)
{
    /* Enable PWM outputs */
    M1_MCDRV_PWM3PH_EN(&g_sM1Pwm3ph);
}

void M1_ClosePWM(void)
{
    /* Disable PWM outputs */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);
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
    CLOCK_EnableClock(kCLOCK_Xbar1);

    XBAR_SetSignalsConnection(kXBAR3_InputQtimer3Timer1, kXBAR1_OutputEqdc1Phasea);
    XBAR_SetSignalsConnection(kXBAR3_InputQtimer3Timer2, kXBAR1_OutputEqdc1Phaseb);

    EQDC1->CTRL2 &= ~EQDC_CTRL2_LDMOD_MASK;
    EQDC1->CTRL &= ~EQDC_CTRL_LDOK_MASK;
    EQDC1->CTRL |= EQDC_CTRL_LDOK_MASK;
    while (EQDC1->CTRL & EQDC_CTRL_LDOK_MASK);

    /* Pass initialization data into encoder driver structure */
    /* encoder position and speed measurement */
    g_sM1Enc.pui32QdBase   = (EQDC_Type *)EQDC1;
    g_sM1Enc.sTo.fltPGain  = M1_POSPE_TO_KP_GAIN;
    g_sM1Enc.sTo.fltIGain  = M1_POSPE_TO_KI_GAIN;
    g_sM1Enc.sTo.fltThGain = M1_POSPE_TO_THETA_GAIN;
    g_sM1Enc.a32PosMeGain  = M1_POSPE_MECH_POS_GAIN;
    g_sM1Enc.ui16Pp        = M1_MOTOR_PP;
    g_sM1Enc.bDirection    = M1_POSPE_ENC_DIRECTION;
    g_sM1Enc.fltSpdEncMin  = M1_POSPE_ENC_N_MIN;
    g_sM1Enc.ui16PulseNumber = M1_POSPE_ENC_PULSES;

    /* Enable modulo counting and revolution counter increment on roll-over */
    EQDC1->CTRL2 = EQDC_CTRL2_REVMOD_MASK;

    /* Prescaler for the timer within QDC, the prescaling value is 2^Mx_QDC_TIMER_PRESCALER */
    EQDC1->FILT = EQDC_FILT_FILT_CNT(2) | EQDC_FILT_FILT_PER(1) | EQDC_FILT_PRSC(6);
    EQDC1->CTRL2 = EQDC_CTRL2_REVMOD_MASK | EQDC_CTRL2_PMEN_MASK;

    /* Speed calculation (based on QDC HW) init */
    g_sM1Enc.sSpeedEncFilter.sFltCoeff.f32B0 = M1_QDC_SPEED_FILTER_IIR_B0_FRAC;
    g_sM1Enc.sSpeedEncFilter.sFltCoeff.f32B1 = M1_QDC_SPEED_FILTER_IIR_B1_FRAC;
    g_sM1Enc.sSpeedEncFilter.sFltCoeff.f32A1 = M1_QDC_SPEED_FILTER_IIR_A1_FRAC;
    GDFLIB_FilterIIR1Init_F16(&g_sM1Enc.sSpeedEncFilter); 

    g_sM1Enc.ui32QDTimerFrequency = (CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Wakeup)) >> ((EQDC1->FILT & EQDC_FILT_PRSC_MASK) >> EQDC_FILT_PRSC_SHIFT);
    g_sM1Enc.i32Q10Cnt2PosGain = ((0xffffffffU/(4*(1*g_sM1Enc.ui16PulseNumber)))*1024); // #define M1_QDC_LINE_RECIPROCAL_4_POS_GEN
    g_sM1Enc.f32SpeedCalConst = (frac32_t)((60.0*g_sM1Enc.ui32QDTimerFrequency/(g_sM1Enc.ui16Pp*(4*g_sM1Enc.ui16PulseNumber)*M1_N_MAX)) * 134217728); // #define M1_SPEED_CAL_CONST
    g_sM1Enc.fltSpeedFrac16ToAngularCoeff = (float_t)(2*PI*M1_N_MAX*g_sM1Enc.ui16Pp/60.0); // #define M1_SPEED_FRAC_TO_ANGULAR_COEFF
    g_sM1Enc.f32PosMechInit = FRAC32(0.0);
    g_sM1Enc.f32PosMechOffset = FRAC32(0.0);

    M1_MCDRV_QD_SET_DIRECTION(&g_sM1Enc);

    /* Initialization modulo counter*/
    M1_MCDRV_QD_SET_PULSES(&g_sM1Enc);
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

  /* Enable CMP clock */
  CLOCK_EnableClock(kCLOCK_Acmp3);

  /* Enable high speed */
  CMP3->C0 |= CMP_C0_PMODE_MASK;

  /* Enable Negative channel discrete mode, Enable resistor divider */
  CMP3->C3 = CMP_C3_NCHCTEN_MASK | CMP_C3_RDIVE_MASK;

  /* Configure channel. Select the positive port input (3U) from DAC and negative port input (7U) from minus mux input. */
  CMP3->C1 |= CMP_C1_PSEL(3U) | CMP_C1_MSEL(7U);

  /* Voltage reference 3V PAD, DAC value 230 (range 0 - 255), High speed mode */
  CMP3->C1 |= (CMP_C1_VRSEL(1U) | CMP_C1_VOSEL(150U) | CMP_C1_DACEN_MASK | CMP_C1_DMODE_MASK);

  /* Enable CMP */
  CMP3->C0 |= CMP_C0_EN_MASK;

}