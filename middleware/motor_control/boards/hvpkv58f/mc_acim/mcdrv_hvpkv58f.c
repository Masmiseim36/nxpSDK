/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_hvpkv58f.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* structure for current and voltage measurement*/
mcdrv_hsadc_t g_sM1AdcSensor;

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
    M1_MCDRV_ADC_PERIPH_INIT();

    /* Init slow loop counter*/
    M1_MCDRV_TMR_SLOWLOOP_INIT();

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
    /* Calculate clock dependant variables for PMSM sensorless control algorithm */
    g_sClockSetup.ui32FastPeripheralClock  = CLOCK_GetFreq(kCLOCK_FastPeriphClk);
    g_sClockSetup.ui32BusClock             = CLOCK_GetFreq(kCLOCK_BusClk);
    g_sClockSetup.ui16PwmFreq = M1_PWM_FREQ; /* 10kHz */
                                             /* PWM module calculated as follows:
                                              * PWM_MOD = PWM_CLOCK / PWM_FREQUENCY = 240 MHz / 10 kHz = 24000 */
    g_sClockSetup.ui16PwmModulo = g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16PwmFreq;
    g_sClockSetup.ui16PwmDeadTime = g_sClockSetup.ui32FastPeripheralClock / (1000000000U / M1_PWM_DEADTIME);
    g_sClockSetup.ui16FastLoopFreq = (g_sClockSetup.ui16PwmFreq/M1_FOC_FREQ_VS_PWM_FREQ);
    g_sClockSetup.ui16SlowLoopFreq = M1_SLOW_LOOP_FREQ; /* 1kHz */
}

/*!
* @brief   void InitPWMA(void)
*           - Initialization of the eFlexPWMA peripheral for motor M1
*           - 3-phase center-aligned PWM
*
* @param   void
*
* @return  none
*/
void InitPWMA0(void)
{
    /* Enable clock for eFlexPWM modules 0,1,2 & 3 in SIM module */
    SIM->SCGC4 |= SIM_SCGC4_PWM0_SM0_MASK;
    SIM->SCGC4 |= SIM_SCGC4_PWM0_SM1_MASK;
    SIM->SCGC4 |= SIM_SCGC4_PWM0_SM2_MASK;
    SIM->SCGC4 |= SIM_SCGC4_PWM0_SM3_MASK;
    
    /* Enable clock for XBARA module */
    SIM->SCGC5 |= SIM_SCGC5_XBARA_MASK;

    /* SM3 clock divided by 2 */
    PWM0->SM[3].CTRL = ((PWM0->SM[3].CTRL & ~(uint16_t)PWM_CTRL_PRSC_MASK) | (PWM_CTRL_PRSC(0x1)));

    /* Full cycle reload */
    PWM0->SM[0].CTRL |= PWM_CTRL_FULL_MASK;
    PWM0->SM[1].CTRL |= PWM_CTRL_FULL_MASK;
    PWM0->SM[2].CTRL |= PWM_CTRL_FULL_MASK;
    PWM0->SM[3].CTRL |= PWM_CTRL_FULL_MASK;

    /* Value register initial values, duty cycle 50% */
    PWM0->SM[0].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 2)));
    PWM0->SM[1].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 2)));
    PWM0->SM[2].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 2)));
    PWM0->SM[3].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 2)));

    PWM0->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWM0->SM[1].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWM0->SM[2].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));

    PWM0->SM[0].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16PwmModulo / 2) - 1));
    PWM0->SM[1].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16PwmModulo / 2) - 1));
    PWM0->SM[2].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16PwmModulo / 2) - 1));
    PWM0->SM[3].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16PwmModulo * 
                                                 M1_FOC_FREQ_VS_PWM_FREQ) - 
                                                (g_sClockSetup.ui16PwmModulo / 2)) / 2 - 1);

    PWM0->SM[0].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 4)));
    PWM0->SM[1].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 4)));
    PWM0->SM[2].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 4)));

    PWM0->SM[0].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16PwmModulo / 4));
    PWM0->SM[1].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16PwmModulo / 4));
    PWM0->SM[2].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16PwmModulo / 4));

    PWM0->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWM0->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWM0->SM[2].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWM0->SM[3].VAL4 = PWM_VAL4_VAL4((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 2) + 45));

    PWM0->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWM0->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWM0->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWM0->SM[3].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* PWMA module 3 trigger on VAL4 enabled for ADC synchronization */
    PWM0->SM[3].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4);

    /* set dead-time (number of Fast Peripheral Clocks) */
    PWM0->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16PwmDeadTime);
    PWM0->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16PwmDeadTime);
    PWM0->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16PwmDeadTime);

    PWM0->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16PwmDeadTime);
    PWM0->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16PwmDeadTime);
    PWM0->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16PwmDeadTime);

    /* Channels A and B disabled when fault 0 occurs */
    PWM0->SM[0].DISMAP[0] = ((PWM0->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWM0->SM[1].DISMAP[0] = ((PWM0->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWM0->SM[2].DISMAP[0] = ((PWM0->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWM0->SM[0].DISMAP[0] = ((PWM0->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWM0->SM[1].DISMAP[0] = ((PWM0->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWM0->SM[2].DISMAP[0] = ((PWM0->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));

    /* Modules one and two gets clock from module zero */
    PWM0->SM[1].CTRL2 = (PWM0->SM[1].CTRL2 & ~(uint16_t)PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWM0->SM[2].CTRL2 = (PWM0->SM[2].CTRL2 & ~(uint16_t)PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);

    /* Master reload active for modules one and two*/
    PWM0->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWM0->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWM0->SM[3].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master reload is generated every n-th opportunity */
    PWM0->SM[0].CTRL = (PWM0->SM[0].CTRL & ~(uint16_t)PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(M1_FOC_FREQ_VS_PWM_FREQ - 1);

    /* Master sync active for modules one and two*/
    PWM0->SM[1].CTRL2 = (PWM0->SM[1].CTRL2 & ~(uint16_t)PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWM0->SM[2].CTRL2 = (PWM0->SM[2].CTRL2 & ~(uint16_t)PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

    /* Master reload active for module three*/
    PWM0->SM[3].CTRL2 = (PWM0->SM[3].CTRL2 & ~(uint16_t)PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

    /* Fault 0 active in logic level zero, manual clearing */
    PWM0->FCTRL = (PWM0->FCTRL & ~(uint16_t)PWM_FCTRL_FLVL_MASK) | PWM_FCTRL_FLVL(0x0);
    PWM0->FCTRL = (PWM0->FCTRL & ~(uint16_t)PWM_FCTRL_FAUTO_MASK) | PWM_FCTRL_FAUTO(0x0);

    /* Clear fault flags */
    PWM0->FSTS = (PWM0->FSTS & ~(uint16_t)PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);

    /* PWMs are re-enabled at PWM full cycle */
    PWM0->FSTS = (PWM0->FSTS & ~(uint16_t)PWM_FSTS_FFULL_MASK) | PWM_FSTS_FFULL(0x1);

    /* PWM fault filter - 5 Fast peripheral clocks sample rate, 5 agreeing
       samples to activate */
    PWM0->FFILT = (PWM0->FFILT & ~(uint16_t)PWM_FFILT_FILT_PER_MASK) | PWM_FFILT_FILT_PER(5);
    PWM0->FFILT = (PWM0->FFILT & ~(uint16_t)PWM_FFILT_FILT_CNT_MASK) | PWM_FFILT_FILT_CNT(5);

    /* Enable A&B PWM outputs for submodules one, two and three */
    PWM0->OUTEN = (PWM0->OUTEN & ~(uint16_t)PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(7);
    PWM0->OUTEN = (PWM0->OUTEN & ~(uint16_t)PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(7);

    /* PWMA3_TRG0 to ADC0 - XBARA_IN26 -> XBAR_OUT12*/
    /* PWMA3_TRG0 to ADC1 - XBARA_IN26 -> XBAR_OUT42*/
    XBARA->SEL6 = XBARA_SEL6_SEL12(26);
    XBARA->SEL21 = XBARA_SEL21_SEL42(26);
    
    /* Start PWMs (set load OK flags and run) */
    PWM0->MCTRL = (PWM0->MCTRL & ~(uint16_t)PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0xF);
    PWM0->MCTRL = (PWM0->MCTRL & ~(uint16_t)PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0xF);
    PWM0->MCTRL = (PWM0->MCTRL & ~(uint16_t)PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0xF);

    /* Initialize MC driver */
    g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWM0;

    g_sM1Pwm3ph.ui16PhASubNum = M1_PWM_PAIR_PHA; /* PWMA phase A sub-module number */
    g_sM1Pwm3ph.ui16PhBSubNum = M1_PWM_PAIR_PHB; /* PWMA phase B sub-module number */
    g_sM1Pwm3ph.ui16PhCSubNum = M1_PWM_PAIR_PHC; /* PWMA phase C sub-module number */

    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
}

/*!
* @brief   void InitFTM2(void)
*           - Initialization of the FTM2 peripheral
*           - performs slow control loop counter
*
* @param   void
*
* @return  none
*/
void InitFTM2(void)
{
    /* enable clock to FTM module */
    SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;

    /* Configuration of FTM module */
    /* Disable write protection for certain registers */
    /* Enable the counter */
    FTM2->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Settings up FTM SC register for clock setup */
    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    /* Set prescaler to 16 */
    /* Enable interrupt */
    FTM2->SC = FTM_SC_PS(4) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

    /* Counter running in BDM mode */
    FTM2->CONF = FTM_CONF_BDMMODE(3);

    /* Set count value to 0 */
    FTM2->CNTIN = 0x0;

    /* 1kHz Slow Loop */
    g_sClockSetup.ui16SpeedLoopModulo =
        (g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16SlowLoopFreq) >> (FTM2->SC & FTM_SC_PS_MASK);
    FTM2->MOD = g_sClockSetup.ui16SpeedLoopModulo;

    /* LOADOK */
    FTM2->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Enable & setup interrupts */
    EnableIRQ(FTM2_IRQn);
    NVIC_SetPriority(FTM2_IRQn, 3);
}

/*!
* @brief   void InitHSADC(void)
*           - Initialization of the HSADC0 & HSADC1 peripheral
*           - Initialization of the A/D converter for current and voltage sensing
*
* @param   void
*
* @return  none
*/
void InitHSADC(void)
{  
    /* Enable clock for ADC modules */
    SIM->SCGC5 |= SIM_SCGC5_HSADC0_MASK;
    SIM->SCGC2 |= SIM_SCGC2_HSADC1_MASK;

    /* HSADC module 0:
       -triggered parallel mode 
       -enable end-of-scan interrupt
       -enable hardware triggering for HSADCA and HSADCB
       -input clock is 25MHz (100MHz Fast Peripheral clock divided by 4)
       -simultaneous conversion on HSADCB */
    HSADC0->CTRL1 = HSADC_CTRL1_SMODE(5) |
                    HSADC_CTRL1_EOSIEA_MASK |
                    HSADC_CTRL1_SYNCA_MASK;
    HSADC0->CTRL2 = HSADC_CTRL2_SYNCB_MASK |
                    HSADC_CTRL2_DIVA(0x3) |
                    HSADC_CTRL2_SIMULT_MASK;
    
    /* HSADC module 1:
       -triggered sequential mode 
       -enable hardware triggering for HSADCC
       -stop HSADCD
       -input clock is 25MHz (100MHz Fast Peripheral clock divided by 4) */
    HSADC1->CTRL1 = HSADC_CTRL1_SMODE(4) |
                    HSADC_CTRL1_SYNCA_MASK;
    HSADC1->CTRL2 = HSADC_CTRL2_STOPB_MASK |
                    HSADC_CTRL2_DIVA(0x3);

    /* Set HSADC0A channel 6A muxing */
    HSADC0->MUX67_SEL = (HSADC0->MUX67_SEL & ~HSADC_MUX67_SEL_CH6_SELA_MASK) | HSADC_MUX67_SEL_CH6_SELA(0x02);

    /* Enable first two samples for HSADCA, HSADCB and HSADCC */
    HSADC0->SDIS = HSADC_SDIS_DS(0xFCFC);
    HSADC1->SDIS = HSADC_SDIS_DS(0xFFFC);

    /* Select PWMA0_trig source (routed using XBARA) as sync signal */
    SIM->SOPT7 = (SIM->SOPT7 & ~SIM_SOPT7_HSADC0AALTTRGEN_MASK) | SIM_SOPT7_HSADC0AALTTRGEN(0x00);
    SIM->SOPT7 = (SIM->SOPT7 & ~SIM_SOPT7_HSADC1AALTTRGEN_MASK) | SIM_SOPT7_HSADC1AALTTRGEN(0x00);

    /* Configure HSADC calibration process */
    /* Single-ended input calibration on HSADC0A and HSADC1C */
    HSADC0->CALIB |= HSADC_CALIB_REQSINGA_MASK;
    HSADC1->CALIB |= HSADC_CALIB_REQSINGA_MASK;

    /* Power-up HSADCA, HSADCB and HSADCC */
    HSADC0->PWR &= (~(HSADC_PWR_PDA_MASK | HSADC_PWR_PDB_MASK));
    HSADC1->PWR &= (~HSADC_PWR_PDA_MASK);     

    /* Wait until calibration is completed */
    while (!(HSADC0->STAT & HSADC_STAT_EOCALIA_MASK))
    {
    }
    while (!(HSADC1->STAT & HSADC_STAT_EOCALIA_MASK))
    {
    }

    /**************************************/
    /* motor M1 ADC driver initialization */
    /**************************************/
    /* offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;
    
    /* Phase current measurement */
    /* Sector 1,6 - measured currents Ic & Ib */
    /* ADC0, channel Ic = M1_ADC0_PH_C, , SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec16.pui32AdcBasePhaC = (HSADC_Type *)HSADC0;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaC = M1_ADC0_PH_C;
    /* ADC1, channel Ib = M1_ADC1_PH_B, , SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec16.pui32AdcBasePhaB = (HSADC_Type *)HSADC1;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaB = M1_ADC1_PH_B;

    /* Sector 2,3 - measured currents Ic & Ia*/
    /* ADC0, channel Ic = M1_ADC0_PH_C, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.pui32AdcBasePhaC = (HSADC_Type *)HSADC0;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaC = M1_ADC0_PH_C;
    /* ADC1, channel Ia = M1_ADC1_PH_A, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.pui32AdcBasePhaA = (HSADC_Type *)HSADC1;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaA = M1_ADC1_PH_A;
    
    /* Sector 4,5 - measured currents Ia & Ib */
    /* ADC0, channel Ia = M1_ADC0_PH_A, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec45.pui32AdcBasePhaA = (HSADC_Type *)HSADC0;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaA = M1_ADC0_PH_A;
    /* ADC1, channel Ib = M1_ADC1_PH_B, SAMPLE & RESULT = 0  */
    g_sM1AdcSensor.sCurrSec45.pui32AdcBasePhaB = (HSADC_Type *)HSADC1;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaB = M1_ADC1_PH_B;
    
    /* UDCbus channel measurement */
    /*  channel Udcb = M1_ADC0_UDCB, SAMPLE & RESULT = 1 */
    g_sM1AdcSensor.pui32AdcBaseVDcb = (HSADC_Type *)HSADC0;
    g_sM1AdcSensor.ui16ChanNumVDcb = M1_ADC0_UDCB;
    
    /* Auxilliary channel measurement */
    /*  channel Aux = M1_ADC2_AUX, SAMPLE & RESULT = 9 */
    g_sM1AdcSensor.pui32AdcBaseAux = (HSADC_Type *)HSADC0;
    g_sM1AdcSensor.ui16ChanNumAux = M1_ADC2_AUX;    

    /* Assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssignInit(&g_sM1AdcSensor);

    /* Enable & setup interrupt from HSADCA */
    EnableIRQ(HSADC0_CCA_IRQn);           /* Enable interrupt */
    NVIC_SetPriority(HSADC0_CCA_IRQn, 1); /* Set priority to interrupt */
    
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
                SystemCoreClock){};
        SysTick->VAL = SysTick->LOAD;
    }
    
    /* turn on relay */
    GPIOC->PSOR = GPIO_PSOR_PTSO(1 << 13);
}

/*!
*@brief      Transform raw FNB41560 IPM temperature measurement to Celsius degrees
*            -fourth degree polynomial approximation is used 
*
*@param      ui16RawData - raw ADC measurement
*            
*@return     float_t
*/
float_t M1_GetIPMTemperature(frac16_t f16RawData)
{
    float_t fltRawData, fltTemperature;
    
    /* convert data to float */
    fltRawData = (float_t)(f16RawData);
    
    /* get temperature in Celsius degrees */
    fltTemperature =  ((((IPM_TEMP_APROX_P1 * fltRawData) + 
                          IPM_TEMP_APROX_P2) * fltRawData + 
                          IPM_TEMP_APROX_P3) * fltRawData + 
                          IPM_TEMP_APROX_P4) * fltRawData + 
                          IPM_TEMP_APROX_P5;

    return fltTemperature; 
}
