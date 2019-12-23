/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_hvpkv46f150m.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Commutation table */
const uint16_t bldcCommutationTableComp[16] = {
    /*swap, mask*/
    0x0440, 0x0040, /* [0] - sector 0 */
    0x0220, 0x0400, /* [1] - sector 1 */
    0x0110, 0x0400, /* [2] - sector 2 */
    0x0440, 0x0004, /* [3] - sector 3 */
    0x0220, 0x0004, /* [4] - sector 4 */
    0x0110, 0x0040, /* [5] - sector 5 */
    0x0000, 0x0440, /* [6] - alignment vector (combination of sectors 0 & 1) */
    0x0770, 0x0000  /* [7] - PWM off */
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* Structure for time event driver */
mcdrv_ftm_cmt_t g_sM1CmtTmr;

/* Structure for current and voltage measurement*/
mcdrv_adc12_init_t g_sM1AdcInit;
mcdrv_adc12_t g_sM1AdcSensor;

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

    /* Init FTM3 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();

    /* Init FTM1 */
    M1_MCDRV_TMR_CMT_PERIPH_INIT();

    /* 6-channel PWM peripheral init */
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
    /* Calculate clock dependant variables for BLDC sensorless control algorithm */
    g_sClockSetup.ui32FastPeripheralClock  = CLOCK_GetFreq(kCLOCK_FastPeriphClk);
    g_sClockSetup.ui32BusClock             = CLOCK_GetFreq(kCLOCK_BusClk);
    g_sClockSetup.ui16PwmFreq = PWM_FREQ; /* 20 kHz */
                                             /* PWM module calculated as follows:
                                              * PWM_MOD = PWM_CLOCK / PWM_FREQUNCY = 74 MHz / 20 kHz = 3700 */
    g_sClockSetup.ui16PwmModulo = g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16PwmFreq;
    g_sClockSetup.ui16PwmDeadTime = (g_sClockSetup.ui32FastPeripheralClock / (1000000000U / M1_PWM_DEADTIME) / 4U);    
    g_sClockSetup.ui16CtrlLoopFreq = CTRL_LOOP_FREQ; /* 1 kHz */
}

/*!
* @brief   Initialization of the FTM0 peripheral for motor M1
*
* @param   void
*
* @return  none
*/
void InitPWMA0(void)
{
    /* Enable clock for eFlexPWM modules 0,1 and 2 in SIM module */
    SIM->SCGC4 |=
        SIM_SCGC4_eFlexPWM0_MASK | SIM_SCGC4_eFlexPWM1_MASK | SIM_SCGC4_eFlexPWM2_MASK | SIM_SCGC4_eFlexPWM3_MASK;

    /* Full cycle reload */
    PWMA->SM[0].CTRL |= PWM_CTRL_FULL_MASK;
    PWMA->SM[1].CTRL |= PWM_CTRL_FULL_MASK;
    PWMA->SM[2].CTRL |= PWM_CTRL_FULL_MASK;
    PWMA->SM[3].CTRL |= PWM_CTRL_FULL_MASK;   
    
    /* Enable clock for XBARA module */
    SIM->SCGC5 |= SIM_SCGC5_XBARA_MASK;

    /* Value register initial values, duty cycle 50% */
    PWMA->SM[0].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 2)));
    PWMA->SM[1].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 2)));
    PWMA->SM[2].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 2)));
    PWMA->SM[3].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 2)));  

    PWMA->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMA->SM[1].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMA->SM[2].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));

    PWMA->SM[0].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16PwmModulo / 2) - 1));
    PWMA->SM[1].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16PwmModulo / 2) - 1));
    PWMA->SM[2].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16PwmModulo / 2) - 1));
    PWMA->SM[3].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16PwmModulo / 2) - 1)); 

    PWMA->SM[0].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 4)));
    PWMA->SM[1].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 4)));
    PWMA->SM[2].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 4)));

    PWMA->SM[0].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16PwmModulo / 4));
    PWMA->SM[1].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16PwmModulo / 4));
    PWMA->SM[2].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16PwmModulo / 4));

    PWMA->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMA->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMA->SM[2].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMA->SM[3].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));

    PWMA->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMA->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMA->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMA->SM[3].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));


    /* PWMA module 3 trigger on VAL4 enabled for ADC synchronization */
    PWMA->SM[3].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4);

    /* set dead-time (number of Fast Peripheral Clocks) */
    PWMA->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16PwmDeadTime);
    PWMA->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16PwmDeadTime);
    PWMA->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16PwmDeadTime);
       
    PWMA->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16PwmDeadTime);
    PWMA->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16PwmDeadTime);
    PWMA->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16PwmDeadTime);

    /* Channels A and B disabled when fault 0 occurs */
    PWMA->SM[0].DISMAP[0] = ((PWMA->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMA->SM[1].DISMAP[0] = ((PWMA->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMA->SM[2].DISMAP[0] = ((PWMA->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMA->SM[0].DISMAP[0] = ((PWMA->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMA->SM[1].DISMAP[0] = ((PWMA->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMA->SM[2].DISMAP[0] = ((PWMA->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));

    /* Modules one and two gets clock from module zero */
    /* Master reload active for modules one and two */
    /* Master sync active for modules one and two */
    PWMA->SM[1].CTRL2 = (PWM_CTRL2_CLK_SEL(0x02) | PWM_CTRL2_FRCEN(1) | PWM_CTRL2_FORCE_SEL(1) |
                         PWM_CTRL2_RELOAD_SEL(TRUE) | PWM_CTRL2_INIT_SEL(2));
    PWMA->SM[2].CTRL2 = (PWM_CTRL2_CLK_SEL(0x02) | PWM_CTRL2_FRCEN(1) | PWM_CTRL2_FORCE_SEL(1) |
                         PWM_CTRL2_RELOAD_SEL(TRUE) | PWM_CTRL2_INIT_SEL(2));
    
    /* Master reload active for module three*/
    PWMA->SM[3].CTRL2 = (PWMA->SM[3].CTRL2 & ~(uint16_t)PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

    /* Fault 0 active in logic level zero, manual clearing */
    PWMA->FCTRL = (PWMA->FCTRL & ~(uint16_t)PWM_FCTRL_FLVL_MASK) | PWM_FCTRL_FLVL(0x0);
    PWMA->FCTRL = (PWMA->FCTRL & ~(uint16_t)PWM_FCTRL_FAUTO_MASK) | PWM_FCTRL_FAUTO(0x0);

    /* Clear fault flags */
    PWMA->FSTS = (PWMA->FSTS & ~(uint16_t)PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);

    /* PWMs are re-enabled at PWM full cycle */
    PWMA->FSTS = (PWMA->FSTS & ~(uint16_t)PWM_FSTS_FFULL_MASK) | PWM_FSTS_FFULL(0x1);

    /* PWM fault filter - 5 Fast peripheral clocks sample rate, 5 agreeing
       samples to activate */
    PWMA->FFILT = (PWMA->FFILT & ~(uint16_t)PWM_FFILT_FILT_PER_MASK) | PWM_FFILT_FILT_PER(5);
    PWMA->FFILT = (PWMA->FFILT & ~(uint16_t)PWM_FFILT_FILT_CNT_MASK) | PWM_FFILT_FILT_CNT(5);

    /* Enable A&B PWM outputs for submodules one, two and three */
    PWMA->OUTEN = (PWMA->OUTEN & ~(uint16_t)PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(7);
    PWMA->OUTEN = (PWMA->OUTEN & ~(uint16_t)PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(7);

    /* PWMA3_TRG0 to ADC0 */
    XBARA->SEL6 = XBARA_SEL6_SEL12(26);
    
    /* Start PWMs (set load OK flags and run) */
    PWMA->MCTRL = (PWMA->MCTRL & ~(uint16_t)PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0xF);
    PWMA->MCTRL = (PWMA->MCTRL & ~(uint16_t)PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0xF);
    PWMA->MCTRL = (PWMA->MCTRL & ~(uint16_t)PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0xF);

    /* Initialize MC driver */
    g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWMA;
    g_sM1Pwm3ph.ui16PhASubNum = M1_PWM_PAIR_PHA; /* PWMA phase A submodule number */
    g_sM1Pwm3ph.ui16PhBSubNum = M1_PWM_PAIR_PHB; /* PWMA phase B submodule number */
    g_sM1Pwm3ph.ui16PhCSubNum = M1_PWM_PAIR_PHC; /* PWMA phase C submodule number */

    /* Initialization of PWM modulo */
    g_sM1Pwm3ph.ui16PwmModulo = g_sClockSetup.ui16PwmModulo;

    /* Initialization of BLDC commutation table */
    g_sM1Pwm3ph.pcBldcTable = &bldcCommutationTableComp[0];
	
    /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM;  
}

/*!
* @brief      Initialization of the FTM1 for forced commutation control
*
* @param      void
*
* @return     none
*/
void InitFTM1(void)
{
    /* Enable clock to FTM1 module */
    SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;

    /* Configuration of FTM1 module */
    /* Write protection is disabled */
    /* Free running counter and synchronization are different from TPM
       behavior */
    FTM1->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Free running timer */
    /* Modulo value */
    FTM1->MOD = 0xffff;

    /* Bus clock, divide by 128, 2.1333us @ 60 MHz clock */
    /* Bus clock as source clock for FTM */
    /* Pre-scale factor 128 */
    FTM1->SC = FTM_SC_PS(7) | FTM_SC_CLKS(1);

    /* Calculate frequency of timer used for forced commutation
     * Bus clock divided by 2^FTM_prescaler */
    g_sClockSetup.ui32CmtTimerFreq = g_sClockSetup.ui32FastPeripheralClock >> (FTM1->SC & FTM_SC_PS_MASK);

    /* Enable Output Compare interrupt, output Compare, Software Output
     * Compare only (ELSnB:ELSnA = 0:0, output pin is not controlled by FTM) */
    FTM1->CONTROLS[0].CnSC = FTM_CnSC_MSA_MASK | FTM_CnSC_CHIE_MASK;

    /* Enable interrupt */
    EnableIRQ(FTM1_IRQn);

    /* Set priority to interrupt */
    NVIC_SetPriority(FTM1_IRQn, 1);

    /* initialization FTM time event driver */
    g_sM1CmtTmr.pui32FtmBase = (FTM_Type *)(FTM1); /* FTM1 base address */
    g_sM1CmtTmr.ui16ChannelNum = M1_FTM_CMT_CHAN;  /* FTM1 compare channel selection */
}

/*!
* @brief   void InitFTM3(void)
*           - Initialization of the FTM2 peripheral
*           - Performs slow control loop counter
*
* @param   void
*
* @return  none
*/
void InitFTM3(void)
{
    /* Enable clock to FTM3 module */
    SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;

    /* Configuration of FTM3 module */
    /* Disable write protection for certain registers */
    /* Enable the counter */
    FTM3->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Counter running in BDM mode */
    FTM3->CONF = FTM_CONF_BDMMODE(3);

    /* Set count value to 0 */
    FTM3->CNTIN = 0x0;

    /* Settings up FTM SC register for clock setup */
    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    /* Set pre-scaler to 16 */
    /* Enable interrupt from FTM2 */
    FTM3->SC = FTM_SC_PS(4) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

    /* 1kHz Slow Loop */
    g_sClockSetup.ui16CtrlLoopModulo =
        ((g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16CtrlLoopFreq) >> (FTM3->SC & FTM_SC_PS_MASK));
    FTM3->MOD = g_sClockSetup.ui16CtrlLoopModulo;

    /* LOADOK */
    FTM3->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Enable & setup interrupts */
    EnableIRQ(FTM3_IRQn);
    NVIC_SetPriority(FTM3_IRQn, 3);
}

/*!
* @brief   void InitADC12(void)
*           - Initialization of the ADC12 peripheral
*           - Initialization of the A/D converter for current and voltage sensing
*
* @param   void
*
* @return  none
*/
void InitADC12(void)
{
     /* ADC channel number assignment array to be passed to MC ADC driver */
    static uint16_t ui16AdcArray[12] = {ADC0_PH_A, ADC1_PH_A,
                                        ADC0_PH_B, ADC1_PH_B,
                                        ADC0_PH_C, ADC1_PH_C,
                                        ADC0_UDCB, ADC1_UDCB,
                                        ADC0_IDCB, ADC1_IDCB,
                                        ADC0_AUX,  ADC1_AUX};

    /* Enable clock for ADC modules */
    SIM->SCGC5 |= SIM_SCGC5_ADC_MASK;

    /* Select ADCA MUX selection for ADCA channel D - BEMFA */
    SIM->ADCOPT |= SIM_ADCOPT_ADCACH6SEL(3);

    /* Triggered parallel mode */
    /* Enable end-of-scan interrupt */
    /* Enable hardware triggering */
    ADC->CTRL1 = (ADC_CTRL1_SMODE(5) | ADC_CTRL1_EOSIE0(1) | ADC_CTRL1_SYNC0(1));

    /* Start ADC */
    ADC->CTRL1 &= ~(ADC_CTRL1_STOP0_MASK);

    /* Input clock is 25MHz (150MHz Fast Peripheral clock divided by 6),
       single ended */
    /* Simultaneous mode */
    ADC->CTRL2 = (ADC_CTRL2_DIV0(5) | ADC_CTRL2_STOP1(1) | ADC_CTRL2_SYNC1(1) | ADC_CTRL2_SIMULT(1));

    /* Enable samples 0, 1, 8, 9 */
    ADC->SDIS = ADC_SDIS_DS(0xF8F8);

    /* Power-up ADCA and ADCB */
    ADC->PWR &= ~(ADC_PWR_PD0_MASK | ADC_PWR_PD1_MASK);

    /* Prepare first measurement */
    /* Pass initialization structure to ADC MC driver */
    g_sM1AdcInit.ui16AdcArray = (&ui16AdcArray[0]);
    g_sM1AdcInit.pui32AdcBase = (ADC_Type *)ADC;

    MCDRV_Adc12Init(&g_sM1AdcSensor, &g_sM1AdcInit);

    /* Enable ADC0 IRQ */
    EnableIRQ(ADCA_IRQn);
    NVIC_SetPriority(ADCA_IRQn, 1);
}

/*!
*@brief      Set Inrush relay on HVP
*
*@param      none
*
*@return     none
*/
void InitRelay(void)
{
    volatile register uint32_t ui32DelayMs = 0;

    /* setup SysTick */
    SysTick->LOAD = 0xFFFFFF;
    SysTick->VAL = SysTick->LOAD;
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    /* wait 1000 milliseconds to turn on the relay */
    while(ui32DelayMs++ < 1000)
    {
        while(((SysTick->LOAD - SysTick->VAL) * 1000) <
                SystemCoreClock){}
        SysTick->VAL = SysTick->LOAD;
    }

    /* turn on relay */
    GPIOC->PSOR = GPIO_PSOR_PTSO(1 << 13);
}
