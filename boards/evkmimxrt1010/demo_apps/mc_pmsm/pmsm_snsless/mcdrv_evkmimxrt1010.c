/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_evkmimxrt1010.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Motor 1 */
/* Structure for current and voltage measurement */
mcdrv_adc_t g_sM1AdcSensor;

/* Structure for 3-phase PWM MC driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* Clock setup structure */
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
    M1_MCDRV_ADC_PERIPH_INIT();

    /* init XBAR  */
    InitXBARA();

    /* Init ADC_ETC */
    InitADC_ETC();

    /* Init TMR1 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();

    /* 6-channel PWM peripheral init for M1 */
    M1_MCDRV_PWM_PERIPH_INIT();
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
    g_sClockSetup.ui32SysPllClock         = CLOCK_GetFreq(kCLOCK_SysPllClk);

    /* Parameters for motor M1 */
    g_sClockSetup.ui16M1PwmFreq   = M1_PWM_FREQ; /* 10 kHz */
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
    /* Single-ended 10-bit conversion (MODE = 0x1) */
    /* Set divide ratio to 2 (ADIV = 0x1) */
    /* 4 samples averaged (AVGS = 0x0) */
    /* IPG clock select (ADICLK = 0x0) */
    ADC1->CFG = (ADC_CFG_ADICLK(0U) | ADC_CFG_MODE(1U) | ADC_CFG_ADIV(1U) | ADC_CFG_AVGS(0U) | ADC_CFG_ADTRG(0U));

    /* HW averaging disabled (AVGE = 0) */
    /* One conversion or one set of conversion (ADCO = 0) */
    ADC1->GC = (ADC_GC_AVGE(0U) | ADC_GC_ADCO(0U));

    /* Asynchronous clock output disabled */
    ADC1->GC |= ADC_GC_ADACKEN(0U);

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

    /* ------- ADC self calibration procedure end ----- */
    /* hardware trigger selected */
    ADC1->CFG |= ADC_CFG_ADTRG(1U);
    /* hardware trigger selected */

    /* Set ADC channels for ADC_ETC */
    /* Withouth this setting ADC_ETC not working properly!!! */
    ADC1->HC[0] = ADC_HC_ADCH(0x01U);
    ADC1->HC[1] = ADC_HC_ADCH(0x02U);
    ADC1->HC[2] = ADC_HC_ADCH(0x03U);

    /**************************************/
    /* motor M1 ADC driver initialization */
    /**************************************/
    /* offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;

    /* Phase current measurement */
    /* Sector 1,6 - measured currents Ic & Ib */
    /* ADC1, channel Ic = M1_ADC1_PH_C, , RESULT = 0 */
    g_sM1AdcSensor.sCurrSec16.pAdcBasePhaC    = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaC = M1_ADC1_PH_C;
    g_sM1AdcSensor.sCurrSec16.ui16RsltRegPhaC = 0U;
    /* ADC1, channel Ib = M1_ADC1_PH_B, RESULT = 1 */
    g_sM1AdcSensor.sCurrSec16.pAdcBasePhaB    = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaB = M1_ADC1_PH_B;
    g_sM1AdcSensor.sCurrSec16.ui16RsltRegPhaB = 1U;

    /* Sector 2,3 - measured currents Ic & Ia*/
    /* ADC1, channel Ic = M1_ADC1_PH_C, RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.pAdcBasePhaC    = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaC = M1_ADC1_PH_C;
    g_sM1AdcSensor.sCurrSec23.ui16RsltRegPhaC = 0U;
    /* ADC1, channel Ia = M1_ADC1_PH_A, RESULT = 1 */
    g_sM1AdcSensor.sCurrSec23.pAdcBasePhaA    = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaA = M1_ADC1_PH_A;
    g_sM1AdcSensor.sCurrSec23.ui16RsltRegPhaA = 1U;

    /* Sector 4,5 - measured currents Ia & Ib */
    /* ADC1, channel Ia = M1_ADC1_PH_A, RESULT = 0 */
    g_sM1AdcSensor.sCurrSec45.pAdcBasePhaA    = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaA = M1_ADC1_PH_A;
    g_sM1AdcSensor.sCurrSec45.ui16RsltRegPhaA = 0U;
    /* ADC1, channel Ib = M1_ADC1_PH_B, RESULT = 1 */
    g_sM1AdcSensor.sCurrSec45.pAdcBasePhaB    = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaB = M1_ADC1_PH_B;
    g_sM1AdcSensor.sCurrSec45.ui16RsltRegPhaB = 1U;

    /* UDCbus channel measurement */
    /* ADC1, channel Udcb = M1_ADC1_UDCB, RESULT = 2 */
    g_sM1AdcSensor.pui32UdcbAdcBase = (ADC_Type *)ADC1;
    g_sM1AdcSensor.ui16ChanNumVDcb  = M1_ADC1_UDCB;
    g_sM1AdcSensor.ui16RsltRegVDcb  = 2U;

    /* Assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssignInit(&g_sM1AdcSensor);

    /* Enable & setup interrupt from ADC */
    EnableIRQ(ADC1_IRQn);
    NVIC_SetPriority(ADC1_IRQn, 0U);
}

/*!
 * @brief   void InitPIT(void)
 *           - Initialization of the PIT peripheral
 *
 * @param   void
 *
 * @return  none
 */
void InitPIT(void)
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;
    /* Set PERCLK_CLK source to OSC_CLK*/
    CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
    /* Set PERCLK_CLK divider to 1 */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);

    /* pitConfig.enableRunInDebug = false; */
    PIT_GetDefaultConfig(&pitConfig);
    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, MSEC_TO_COUNT(1U, CLOCK_GetFreq(kCLOCK_OscClk)));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    /* Enable & setup interrupt from PIT */
    EnableIRQ(PIT_IRQn);
    NVIC_SetPriority(PIT_IRQn, 3U);

    /* Start channel 0 */
    PIT_StartTimer(PIT, kPIT_Chnl_0);
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
    PWMBase->SM[M1_PWM_PAIR_PHA].CTRL |= PWM_CTRL_FULL_MASK;
    PWMBase->SM[M1_PWM_PAIR_PHB].CTRL |= PWM_CTRL_FULL_MASK;
    PWMBase->SM[M1_PWM_PAIR_PHC].CTRL |= PWM_CTRL_FULL_MASK;

    /* Value register initial values, duty cycle 50% */
    PWMBase->SM[M1_PWM_PAIR_PHA].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[M1_PWM_PAIR_PHB].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[M1_PWM_PAIR_PHC].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));

    PWMBase->SM[M1_PWM_PAIR_PHA].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[M1_PWM_PAIR_PHB].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[M1_PWM_PAIR_PHC].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));

    PWMBase->SM[M1_PWM_PAIR_PHA].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[M1_PWM_PAIR_PHB].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[M1_PWM_PAIR_PHC].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));

    PWMBase->SM[M1_PWM_PAIR_PHA].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[M1_PWM_PAIR_PHB].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[M1_PWM_PAIR_PHC].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));

    PWMBase->SM[M1_PWM_PAIR_PHA].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[M1_PWM_PAIR_PHB].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[M1_PWM_PAIR_PHC].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));

    /* Trigger for ADC synchronization */
    PWMBase->SM[M1_PWM_PAIR_PHA].VAL4 = PWM_VAL4_VAL4((uint16_t)((-(g_sClockSetup.ui16M1PwmModulo / 2) + 10)));
    PWMBase->SM[M1_PWM_PAIR_PHB].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMBase->SM[M1_PWM_PAIR_PHC].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));

    PWMBase->SM[M1_PWM_PAIR_PHA].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[M1_PWM_PAIR_PHB].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[M1_PWM_PAIR_PHC].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* PWM0 module 0 trigger on VAL4 enabled for ADC synchronization */
    PWMBase->SM[M1_PWM_PAIR_PHA].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4);

    /* Set dead-time register */
    PWMBase->SM[M1_PWM_PAIR_PHA].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[M1_PWM_PAIR_PHB].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[M1_PWM_PAIR_PHC].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[M1_PWM_PAIR_PHA].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[M1_PWM_PAIR_PHB].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[M1_PWM_PAIR_PHC].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);

    /* Channels A and B disabled when fault 0 occurs */
    PWMBase->SM[M1_PWM_PAIR_PHA].DISMAP[0] =
        ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[M1_PWM_PAIR_PHB].DISMAP[0] =
        ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[M1_PWM_PAIR_PHC].DISMAP[0] =
        ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[M1_PWM_PAIR_PHA].DISMAP[0] =
        ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMBase->SM[M1_PWM_PAIR_PHB].DISMAP[0] =
        ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMBase->SM[M1_PWM_PAIR_PHC].DISMAP[0] =
        ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));

    /* Modules one and two gets clock from module zero */
    PWMBase->SM[M1_PWM_PAIR_PHB].CTRL2 =
        (PWMBase->SM[M1_PWM_PAIR_PHB].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWMBase->SM[M1_PWM_PAIR_PHC].CTRL2 =
        (PWMBase->SM[M1_PWM_PAIR_PHC].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);

    /* Master reload active for modules one and three */
    PWMBase->SM[M1_PWM_PAIR_PHB].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMBase->SM[M1_PWM_PAIR_PHC].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master reload is generated every one opportunity */
    PWMBase->SM[M1_PWM_PAIR_PHA].CTRL =
        (PWMBase->SM[M1_PWM_PAIR_PHA].CTRL & ~PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(M1_FOC_FREQ_VS_PWM_FREQ - 1);

    /* Master sync active for modules one and three*/
    PWMBase->SM[M1_PWM_PAIR_PHB].CTRL2 =
        (PWMBase->SM[M1_PWM_PAIR_PHB].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWMBase->SM[M1_PWM_PAIR_PHC].CTRL2 =
        (PWMBase->SM[M1_PWM_PAIR_PHC].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

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
    PWMBase->OUTEN = (PWMBase->OUTEN & ~PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(0xD);
    PWMBase->OUTEN = (PWMBase->OUTEN & ~PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(0xD);

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
    XBARA_Init(XBARA);

    /* Configure the XBARA signal connections. (set for sync mode in ADC_ETC) */
    XBARA_SetSignalsConnection(XBARA, kXBARA1_InputFlexpwm1Pwm1OutTrig01, kXBARA1_OutputAdcEtcTrig00);
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
    adcEtcTriggerConfig.enableSyncMode      = false;
    adcEtcTriggerConfig.enableSWTriggerMode = false;
    adcEtcTriggerConfig.triggerChainLength  = 2U;
    adcEtcTriggerConfig.triggerPriority     = 0U;
    adcEtcTriggerConfig.sampleIntervalDelay = 0U;
    adcEtcTriggerConfig.initialDelay        = 0U;

    ADC_ETC_SetTriggerConfig(ADC_ETC, 0U, &adcEtcTriggerConfig);

    adcEtcTriggerChainConfig.enableB2BMode = true;

    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 0U; /* Select ADC_HC0 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect =
        M1_ADC1_PH_A; /* ADC_HC0 will be triggered to sample Corresponding channel.  */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable;  /* Enable the Done0 interrupt. */
    adcEtcTriggerChainConfig.enableB2BMode   = 1U;                             /* Enable B2B mode */
    adcEtcTriggerChainConfig.enableIrq       = 0U;                             /* Interrupt disabled */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 0U, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain0. */

    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 1U; /* Select ADC_HC1 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect =
        M1_ADC1_PH_B; /* ADC_HC1 will be triggered to sample Corresponding channel.  */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 1U, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain1. */

    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 2U; /* Select ADC_HC2 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect =
        M1_ADC1_UDCB; /* ADC_HC1 will be triggered to sample Corresponding channel.  */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 2U, &adcEtcTriggerChainConfig); /* Configure the trigger0 chain2. */
}
