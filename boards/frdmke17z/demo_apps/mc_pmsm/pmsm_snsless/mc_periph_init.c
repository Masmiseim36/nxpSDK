/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"
#include "fsl_common.h"
#include "fsl_ftm.h"
#include "peripherals.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;

/* structure for current and voltage measurement*/
mcdrv_adc16_t g_sM1AdcSensor;

/* structure containing clock frequencies and modulos */
clock_setup_t g_sClockSetup;

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
    InitADC16();
    
    /* Init LPIT */
    InitLPIT();
        
    /* 6-channel PWM peripheral init */
    InitFTM0();

    /* Init slow loop counter*/
    InitFTM1();
    
    /* Comparator CMP1 */
    InitCMP1();
    
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
    g_sClockSetup.ui32SystemClock     = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    g_sClockSetup.ui32SysOscAsyncDiv2 = CLOCK_GetFreq(kCLOCK_ScgSysOscAsyncDiv2Clk);
    g_sClockSetup.ui16M1PwmFreq       = M1_PWM_FREQ; /* 10 kHz */
                                                     /* PWM module calculated as follows:
                                                      * PWM_MOD = SYS_CLOCK / PWM_FREQUNCY = 48 MHz / 10 kHz = 4800   */

    g_sClockSetup.ui16M1PwmModulo   = g_sClockSetup.ui32SystemClock / g_sClockSetup.ui16M1PwmFreq;
    g_sClockSetup.ui16M1PwmDeadTime = (g_sClockSetup.ui32SystemClock / (1000000000U / M1_PWM_DEADTIME)) / 4;

    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SLOW_LOOP_FREQ; /* 1kHz */
    
}


/*!
 * @brief   void InitFTM0(void)
 *           - Initialization of the FTM0 peripheral for motor M1
 *           - 3-phase center-aligned PWM
 *
 * @param   void
 *
 * @return  none
 */
void InitFTM0(void)
{
    /* enable the clock for FTM0 */
    PCC->CLKCFG[PCC_FLEXTMR0_INDEX] = PCC_CLKCFG_CGC(TRUE);

    /* Disable all channels outputs using the OUTPUT MASK feature.
     * However, the output pins are still driven as GPIO since the
     * channel mode is set to FTM channel disabled after RESET */
    /* channel output is masked */
    FTM0->OUTMASK = (FTM_OUTMASK_CH0OM_MASK | FTM_OUTMASK_CH1OM_MASK | FTM_OUTMASK_CH2OM_MASK | FTM_OUTMASK_CH3OM_MASK |
                     FTM_OUTMASK_CH4OM_MASK | FTM_OUTMASK_CH5OM_MASK);

    /* disable write protection for certain registers
     * free running counter and synchronization are different from TPM
     * Enable the counter
     * Fault Control Mode - Fault control is enabled for all channels,
     * and the selected mode is the manual fault clearing
     * Initialize The Channels Output */
    FTM0->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK | FTM_MODE_FAULTM(3) | FTM_MODE_INIT_MASK;

    /* Counter running in BDM mode */
    FTM0->CONF = FTM_CONF_BDMMODE(3);

    /* initial value of the FTM counter */
    FTM0->CNTIN = (uint32_t)(-(g_sClockSetup.ui16M1PwmModulo) / 2);

    /* modulo value */
    FTM0->MOD = (uint32_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1);

    /* maximum loading point enable */
    FTM0->SYNC = FTM_SYNC_CNTMAX_MASK;

    /* For center aligned PWM using combine mode:
     * COMBINE = 1 - combine mode set
     * COMP = 1 - complementary PWM set
     * DTEN = 1 - dead-time enabled
     * SYNCEN = 1 - PWM update synchronization enabled
     * FAULTEN = 1 - fault control enabled
     */
    /* complementary mode */
    FTM0->COMBINE = FTM_COMBINE_COMBINE0_MASK | FTM_COMBINE_COMP0_MASK | FTM_COMBINE_DTEN0_MASK |
                    FTM_COMBINE_SYNCEN0_MASK | FTM_COMBINE_FAULTEN0_MASK | FTM_COMBINE_COMBINE1_MASK |
                    FTM_COMBINE_COMP1_MASK | FTM_COMBINE_DTEN1_MASK | FTM_COMBINE_SYNCEN1_MASK |
                    FTM_COMBINE_FAULTEN1_MASK | FTM_COMBINE_COMBINE2_MASK | FTM_COMBINE_COMP2_MASK |
                    FTM_COMBINE_DTEN2_MASK | FTM_COMBINE_SYNCEN2_MASK | FTM_COMBINE_FAULTEN2_MASK;

    /* Dead time */
    FTM0->DEADTIME = FTM_DEADTIME_DTPS(2) | FTM_DEADTIME_DTVAL(g_sClockSetup.ui16M1PwmDeadTime);

    /* Initial setting of value registers to 50 % of duty cycle  */
    FTM0->CONTROLS[0].CnV = (uint32_t)(-g_sClockSetup.ui16M1PwmModulo / 4);
    FTM0->CONTROLS[1].CnV = (uint32_t)(g_sClockSetup.ui16M1PwmModulo / 4);
    FTM0->CONTROLS[2].CnV = (uint32_t)(-g_sClockSetup.ui16M1PwmModulo / 4);
    FTM0->CONTROLS[3].CnV = (uint32_t)(g_sClockSetup.ui16M1PwmModulo / 4);
    FTM0->CONTROLS[4].CnV = (uint32_t)(-g_sClockSetup.ui16M1PwmModulo / 4);
    FTM0->CONTROLS[5].CnV = (uint32_t)(g_sClockSetup.ui16M1PwmModulo / 4);

    /* Note:
     * 1. From this moment the output pins are under FTM control. Since the PWM
     *    output is disabled by the FTM0OUTMASK register, there is no change on
     *    PWM outputs. Before the channel mode is set, the correct output pin
     *    polarity has to be defined.
     * 2. Even if the odd channels are generated automatically by complementary
     *    logic, these channels have to be set to be in the same channel mode.
     */
    FTM0->CONTROLS[0].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[1].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[2].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[3].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[4].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[5].CnSC = FTM_CnSC_ELSB_MASK;

    /* Set LOAD ok register */
    /* enable loading updated values */
    FTM0->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

    /* Initialization Trigger Enable */
    FTM0->EXTTRIG = FTM_EXTTRIG_INITTRIGEN_MASK;

    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    FTM0->SC = FTM_SC_CLKS(1);

    /* Enable Fault 1 - over current comparator CMP1 output */
    FTM0->FLTCTRL |= FTM_FLTCTRL_FAULT1EN_MASK;
    
    /* Enable PWM */
    FTM0->SC |= (FTM_SC_PWMEN0(TRUE) | FTM_SC_PWMEN1(TRUE) | FTM_SC_PWMEN2(TRUE) | FTM_SC_PWMEN3(TRUE) |
                 FTM_SC_PWMEN4(TRUE) | FTM_SC_PWMEN5(TRUE));

    /* ---------------------------------------- */
    /* Initialization FTM 3-phase PWM mc driver */
    g_sM1Pwm3ph.pui32PwmBase = (FTM_Type *)(FTM0); /* FTM0 base address */
    g_sM1Pwm3ph.ui16ChanPhA  = M1_PWM_PAIR_PHA;    /* PWM phase A top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhB  = M1_PWM_PAIR_PHB;    /* PWM phase B top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhC  = M1_PWM_PAIR_PHC;    /* PWM phase C top&bottom channel pair number */

    /* FTM Fault number for over current fault detection */
    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM;
    
}

/*!
 * @brief   void InitFTM1(void)
 *           - Initialization of the FTM1 peripheral
 *           - performs slow control loop counter
 *
 * @param   void
 *
 * @return  none
 */
void InitFTM1(void)
{
    /* enable clock to FTM module */
    PCC->CLKCFG[PCC_FLEXTMR1_INDEX] = PCC_CLKCFG_CGC(TRUE);

    /* Configuration of FTM module */
    /* Disable write protection for certain registers */
    /* Enable the counter */
    FTM1->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Settings up FTM SC register for clock setup */
    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    /* Set prescaler to 16 */
    /* Enable interrupt */
    FTM1->SC = FTM_SC_PS(4) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

    /* Counter running in BDM mode */
    FTM1->CONF = FTM_CONF_BDMMODE(3);

    /* Set count value to 0 */
    FTM1->CNTIN = 0x0;

    /* 1kHz Slow Loop */
    g_sClockSetup.ui16M1SpeedLoopModulo = 
         (g_sClockSetup.ui32SystemClock / g_sClockSetup.ui16M1SpeedLoopFreq) >> (FTM1->SC & FTM_SC_PS_MASK);   
    
    FTM1->MOD = g_sClockSetup.ui16M1SpeedLoopModulo;

    /* LOADOK */
    FTM1->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Enable & setup interrupts */
    EnableIRQ(FTM1_IRQn);
    NVIC_SetPriority(FTM1_IRQn, 3);
    
}


/*!
 * @brief   void InitADC16(void)
 *           - Initialization of the ADC16 peripheral
 *           - Initialization of the A/D converter for current and voltage sensing
 *
 * @param   void
 *
 * @return  none
 */
void InitADC16(void)
{
    uint16_t ui16Calib;
    
    /* Set the source for the ADC module */
    CLOCK_SetIpSrc(kCLOCK_Adc0, kCLOCK_IpSrcLpFllAsync);

    /* enable clock for ADC0 and ADC1 modules*/
    PCC->CLKCFG[PCC_ADC0_INDEX] = (PCC_CLKCFG_CGC(0));
    PCC->CLKCFG[PCC_ADC0_INDEX] = (PCC_CLKCFG_CGC(TRUE) | PCC_CLKCFG_PCS(1));

    /* ------- ADC self calibration procedure start ----- *
     * setting the clock to 3 MHz (ADIV - div bus clock (24 MHz) by 8 and
     * ADICLK bits) and single-ended 12-bit conversion (MODE bits)
     */
    /* single-ended 12-bit conversion */
    /* set divide ratio to 8 */
    /* input clock select - BUS clock  */
    ADC0->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(3));

    /* HW averaging enabled, 32 samples averaged, continuous conversion is enabled */
    /* Enable hardware average */
    /* 32 samples averaged */
    ADC0->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) | ADC_SC3_ADCO_MASK);

    /* starting the calibration of ADC0 */
    ADC0->SC3 |= (ADC_SC3_CAL_MASK);

    /* wait until the calibration completes */
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
    {
    }

    ui16Calib = ADC0->UG;
    ui16Calib += ADC0->CLP0;
    ui16Calib += ADC0->CLP1;
    ui16Calib += ADC0->CLP2;
    ui16Calib += ADC0->CLP3;
    ui16Calib += ADC0->CLPS;
    /* set the plus side gain register */
    if (ui16Calib >= 0x800)
        ui16Calib = 0xffff;

    ADC0->G = ui16Calib;

    /* ------- ADC self calibration procedure end ----- */

    /* Write default values to SC3 register */
    ADC0->SC3 = 0;

    /* bus clock source (24 MHz), ADC0 clock = Bus clock / 1 (24 MHz), 12-bit */
    ADC0->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(0));

    /* Set hardware trigger */
    ADC0->SC2 = (ADC_SC2_ADTRG_MASK);

    /**************************************/
    /* motor M1 ADC driver initialization */
    /**************************************/
    /* offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;

    /* Phase current measurement */
    /* Sector 1,6 - measured currents Ic & Ib */
    /* ADC0, channel Ic = M1_ADC0_PH_C, , SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec16.pui32AdcBasePhaC = (ADC_Type *)ADC0;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaC  = M1_ADC0_PH_C;
    /* ADC0, channel Ib = M1_ADC0_PH_B, , SAMPLE & RESULT = 1 */
    g_sM1AdcSensor.sCurrSec16.pui32AdcBasePhaB = (ADC_Type *)ADC0;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaB  = M1_ADC0_PH_B;

    /* Sector 2,3 - measured currents Ic & Ia*/
    /* ADC0, channel Ic = M1_ADC0_PH_C, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.pui32AdcBasePhaC = (ADC_Type *)ADC0;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaC  = M1_ADC0_PH_C;
    /* ADC0, channel Ia = M1_ADC0_PH_A, SAMPLE & RESULT = 1 */
    g_sM1AdcSensor.sCurrSec23.pui32AdcBasePhaA = (ADC_Type *)ADC0;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaA  = M1_ADC0_PH_A;

    /* Sector 4,5 - measured currents Ia & Ib */
    /* ADC0, channel Ia = M1_ADC0_PH_A, SAMPLE & RESULT = 0  */
    g_sM1AdcSensor.sCurrSec45.pui32AdcBasePhaA = (ADC_Type *)ADC0;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaA  = M1_ADC0_PH_A;
    /* ADC0, channel Ib = M1_ADC0_PH_B, SAMPLE & RESULT = 1 */
    g_sM1AdcSensor.sCurrSec45.pui32AdcBasePhaB = (ADC_Type *)ADC0;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaB  = M1_ADC0_PH_B;

    /* UDCbus channel measurement */
    /*  channel Udcb = M1_ADC0_UDCB, SAMPLE & RESULT = 2 */
    g_sM1AdcSensor.pui32UdcbAdcBase = (ADC_Type *)ADC0;
    g_sM1AdcSensor.ui16ChanNumVDcb  = M1_ADC0_UDCB;

    /* Assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssignInit(&g_sM1AdcSensor);

    /* Enable & setup COCO interrupt generated after 3rd conversion is completed*/
    ADC0->SC1[2] |= ADC_SC1_AIEN_MASK;
    
    /* Enable & setup interrupts */
    EnableIRQ(ADC0_IRQn);
    NVIC_SetPriority(ADC0_IRQn, 0); 
    
}


/*!
 * @brief      void InitPDB(void)
 *              - Initialization of the PDB for ADC triggering and
 *		         synchronization between FTM0 and ADC16
 *
 * @param      void
 *
 * @return     none
 */
void InitLPIT(void)
{
    /* Set the source for the LPIT module */
    CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcLpFllAsync);
    
    /* Structure of initialize LPIT */
    lpit_config_t lpitConfig;
    lpit_chnl_params_t lpitChannelConfig;

    LPIT_GetDefaultConfig(&lpitConfig);
    /* Init lpit module */
    LPIT_Init(LPIT0, &lpitConfig);

    lpitChannelConfig.chainChannel          = false;
    lpitChannelConfig.enableReloadOnTrigger = false;
    /* Timer starts to decrement when rising edge on selected trigger is detected */    
    lpitChannelConfig.enableStartOnTrigger  = true;
    lpitChannelConfig.enableStopOnTimeout   = true;
    lpitChannelConfig.timerMode             = kLPIT_PeriodicCounter;
    
    /* Set default values for the trigger source */ 
    lpitChannelConfig.triggerSource = kLPIT_TriggerSource_External;

    /* Init lpit channel 0-3 */
    lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn0;
    LPIT_SetupChannel(LPIT0, kLPIT_Chnl_0, &lpitChannelConfig);
    
    lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn1;
    LPIT_SetupChannel(LPIT0, kLPIT_Chnl_1, &lpitChannelConfig);
    
    lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn2;
    LPIT_SetupChannel(LPIT0, kLPIT_Chnl_2, &lpitChannelConfig);

    /* Set timer period for channel 0-3*/   
    LPIT0->CHANNEL[0].TVAL = 1U;
    LPIT0->CHANNEL[1].TVAL = 1U;
    LPIT0->CHANNEL[2].TVAL = 1U;
    
    /* Enable timer interrupts for channel 2 */
    LPIT_EnableInterrupts(LPIT0, kLPIT_Channel2TimerInterruptEnable);
    
    /* Enable & setup interrupts */
    NVIC_EnableIRQ(LPIT0_IRQn);
    NVIC_SetPriority(LPIT0_IRQn, 1);
    
    /* Start the timer */
    LPIT_StartTimer(LPIT0, kLPIT_Chnl_0);
    LPIT_StartTimer(LPIT0, kLPIT_Chnl_1);
    LPIT_StartTimer(LPIT0, kLPIT_Chnl_2);
    
}

/*!
@brief   void InitCMP1(void)
          - Initialization of the comparator 1 module for dc-bus over current
            detection to generate FTM0 fault

@param   void

@return  none
*/
void InitCMP1(void)
{
    /* Structure of initialize ACMP */
    acmp_config_t acmpConfigStruct;
    acmp_channel_config_t channelConfigStruct;
    acmp_dac_config_t dacConfigStruct;  
  
    /* Configure ACMP. */
    /*
     * acmpConfigStruct.enableHighSpeed = false;
     * acmpConfigStruct.enableInvertOutput = false;
     * acmpConfigStruct.useUnfilteredOutput = false;
     * acmpConfigStruct.enablePinOut = false;
     * acmpConfigStruct.offsetMode = kACMP_OffsetLevel0;
     * acmpConfigStruct.hysteresisMode = kACMP_HysteresisLevel0;
     */
    ACMP_GetDefaultConfig(&acmpConfigStruct);
    ACMP_Init(CMP0, &acmpConfigStruct);

    /* Configure channel. Select the positive port input from MUX and negative port input from minus DAC input. */
    channelConfigStruct.positivePortInput = kACMP_PortInputFromMux;
    channelConfigStruct.negativePortInput = kACMP_PortInputFromDAC;
    channelConfigStruct.plusMuxInput  = 5U; /* IN 5 */
    channelConfigStruct.minusMuxInput = 0U; /* Dummy channel. */
    ACMP_SetChannelConfig(CMP0, &channelConfigStruct);

    /* Configure DAC. */
    dacConfigStruct.referenceVoltageSource = kACMP_VrefSourceVin1;
    dacConfigStruct.DACValue               = 250U;

    ACMP_SetDACConfig(CMP0, &dacConfigStruct);

    ACMP_Enable(CMP0, true);
  
}