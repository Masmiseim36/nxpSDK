/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_hvpkv11z75m.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Commutation table */
const char bldcCommutationTableComp[16] = {
    /*swap, mask*/
    0x30, 0x02, /* [0] - sector 0 */
    0x0C, 0x04, /* [1] - sector 1 */
    0x03, 0x04, /* [2] - sector 2 */
    0x30, 0x01, /* [3] - sector 3 */
    0x0C, 0x01, /* [4] - sector 4 */
    0x03, 0x02, /* [5] - sector 5 */
    0x00, 0x06, /* [6] - alignment vector (combination of sectors 0 & 1) */
    0x3F, 0x00  /* [7] - PWM off */
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;

/* Structure for time event driver */
mcdrv_ftm_cmt_t g_sM1CmtTmr;

/* Structure for current and voltage measurement*/
mcdrv_adc16_init_t g_sM1Adc16Init;
mcdrv_adc16_t g_sM1AdcSensor;

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

    /* Init PDB */
    M1_MCDRV_PDB_INIT();

    /* Init FTM2 (slow loop counter) */
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
    g_sClockSetup.ui32SystemClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    g_sClockSetup.ui32BusClock    = CLOCK_GetFreq(kCLOCK_BusClk);
    g_sClockSetup.ui16PwmFreq = PWM_FREQ; /* 20 kHz */
                                             /* PWM module calculated as follows:
                                              * PWM_MOD = PWM_CLOCK / PWM_FREQUNCY = 75 MHz / 20 kHz = 3750 */
    g_sClockSetup.ui16PwmModulo = g_sClockSetup.ui32SystemClock / g_sClockSetup.ui16PwmFreq;
    g_sClockSetup.ui16M1PwmDeadTime = (g_sClockSetup.ui32SystemClock / (1000000000U / M1_PWM_DEADTIME) / 4U);    
    g_sClockSetup.ui16CtrlLoopFreq = CTRL_LOOP_FREQ; /* 1 kHz */
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
    /* Enable the clock for FTM0 */
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
    
    /*Enable Clock to port C - Fault pin */
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    
    /*Configure PTC0 as FTM0 fault pin*/
    PORTC->PCR[0] = PORT_PCR_MUX(6);

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
    FTM0->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK | FTM_MODE_FAULTM(2) | FTM_MODE_INIT_MASK;

    /* Counter running in BDM mode */
    FTM0->CONF = FTM_CONF_BDMMODE(3);

    /* Initial value of the FTM counter */
    FTM0->CNTIN = (uint32_t)(-(g_sClockSetup.ui16PwmModulo) / 2);

    /* Modulo value */
    FTM0->MOD = (uint32_t)((g_sClockSetup.ui16PwmModulo / 2) - 1);

    /* Maximum loading point enable */
    FTM0->SYNC = FTM_SYNC_CNTMAX_MASK | FTM_SYNC_SYNCHOM_MASK;

    /* For center aligned PWM using combine mode:
     * COMBINE = 1 - combine mode set
     * COMP = 1 - complementary PWM set
     * DTEN = 1 - deadtime enabled
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
    FTM0->CONTROLS[0].CnV = (uint32_t)(-g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[1].CnV = (uint32_t)(g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[2].CnV = (uint32_t)(-g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[3].CnV = (uint32_t)(g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[4].CnV = (uint32_t)(-g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[5].CnV = (uint32_t)(g_sClockSetup.ui16PwmModulo / 4);

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

     /* Enable loading updated values */
    FTM0->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

    /* Enable generation of trigger when FTM counter is equal to CNTIN register */
    FTM0->EXTTRIG = FTM_EXTTRIG_INITTRIGEN_MASK;

    /* No ISR, counting up, bus clock, divide by 1 */
    FTM0->SC = FTM_SC_CLKS(1);
	
    /* Fault 0 polarity setting - for FNB41560 is polarity active low */
    FTM0->FLTPOL |= FTM_FLTPOL_FLT0POL_MASK;

    /* Enable Fault 0 - connected to FNB41560 over current comparator output */
    FTM0->FLTCTRL |= FTM_FLTCTRL_FAULT0EN_MASK;	

    /* following lines configure:
     - enhanced PWM synchronization, FTM counter reset on SW sync
     - output SW control / polarity registers updated on PWM synchronization
       (not on rising edge of clock)
     - output SW control/inverting(swap)/mask registers updated from buffers on
       SW synchronization */
    FTM0->SYNCONF = FTM_SYNCONF_INVC_MASK | FTM_SYNCONF_SWOC_MASK | FTM_SYNCONF_SYNCMODE_MASK |
                    FTM_SYNCONF_SWRSTCNT_MASK | FTM_SYNCONF_SWOM_MASK | FTM_SYNCONF_SWINVC_MASK |
                    FTM_SYNCONF_SWSOC_MASK;

    /* Update of FTM settings */
    FTM0->CNT = 0x0001;

    /* software trigger for PWM synchronization is selected */
    FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK;

    /* Initialization FTM 3-phase PWM mc driver */
    g_sM1Pwm3ph.pui32PwmBase = (FTM_Type *)(FTM0);    /* FTM0 base address */
    g_sM1Pwm3ph.ui16ChanPhA = M1_PWM_PAIR_PHA; /* PWM phase A top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhB = M1_PWM_PAIR_PHB; /* PWM phase B top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhC = M1_PWM_PAIR_PHC; /* PWM phase C top&bottom channel pair number */

    /* Initialization of PWM modulo */
    g_sM1Pwm3ph.ui16PwmModulo = g_sClockSetup.ui16PwmModulo;

    /* Initialization of BLDC commutation table */
    g_sM1Pwm3ph.pcBldcTable = &bldcCommutationTableComp[0];
    
    /* FTM Fault number for over current fault detection */
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
       behaviour */
    FTM1->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Free running timer */
    /* Modulo value */
    FTM1->MOD = 0xffff;

    /* Core clock, divide by 128, 1.25 MHz clock */
    /* Core clock as source clock for FTM */
    /* Pre-scale factor 128 */
    FTM1->SC = FTM_SC_PS(7) | FTM_SC_CLKS(1);

    /* calculate frequency of timer used for forced commutation
     * System clock divided by 2^FTM_prescaler */
    g_sClockSetup.ui32CmtTimerFreq = g_sClockSetup.ui32SystemClock >> (FTM1->SC&FTM_SC_PS_MASK);

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
* @brief   void InitFTM2(void)
*           - Initialization of the FTM2 peripheral
*           - Performs slow control loop counter
*
* @param   void
*
* @return  none
*/
void InitFTM2(void)
{
    /* Enable clock to FTM module */
    SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;

    /* Configuration of FTM module */
    /* Disable write protection for certain registers */
    /* Enable the counter */
    FTM2->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Counter running in BDM mode */
    FTM2->CONF = FTM_CONF_BDMMODE(3);

    /* Set count value to 0 */
    FTM2->CNTIN = 0x0;

    /* Settings up FTM SC register for clock setup */
    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    /* Set prescaler to 16 */
    /* Enable interrupt */
    FTM2->SC = FTM_SC_PS(4) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

    /* 1kHz Slow Loop */
    g_sClockSetup.ui16CtrlLoopModulo =
        (g_sClockSetup.ui32SystemClock / g_sClockSetup.ui16CtrlLoopFreq) >> (FTM2->SC & FTM_SC_PS_MASK);
    FTM2->MOD = g_sClockSetup.ui16CtrlLoopModulo;

    /* LOADOK */
    FTM2->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Enable & setup interrupts */
    EnableIRQ(FTM2_IRQn);
    NVIC_SetPriority(FTM2_IRQn, 3);
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

    /* ADC channel number assignment array to be passed to MC ADC driver */
    static uint16_t ui16AdcArray[12] = {ADC0_PH_A, ADC1_PH_A, ADC0_PH_B, ADC1_PH_B,
                                        ADC0_PH_C, ADC1_PH_C, ADC0_UDCB, ADC1_UDCB,
                                        ADC0_IDCB, ADC1_IDCB, ADC0_AUX,  ADC1_AUX};

    /* Enable clock for ADC0 and ADC1 modules*/
    SIM->SCGC6 |= (SIM_SCGC6_ADC0_MASK | SIM_SCGC6_ADC1_MASK);

    /* ------- ADC self calibration procedure start ----- */
    /* Setting the clock to 3.125 MHz (ADIV - div bus clock (25 MHz) by 8 and
     * ADICLK bits) and single-ended 12-bit conversion (MODE bits)
     */
    /* Single-ended 12-bit conversion */
    /* Set divide ratio to 8 */
    /* Input clock select - BUS clock  */
    ADC0->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(3));

    /* Single-ended 12-bit conversion */
    /* Set divide ratio to 8 */
    /* Input clock select - BUS clock  */
    ADC1->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(3));

    /* HW averaging enabled, 32 samples averaged, continuous conversion must be enabled */
    /* Enable hardware average */
    /* 32 samples averaged */
    ADC0->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) | ADC_SC3_ADCO_MASK);
    ADC1->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) | ADC_SC3_ADCO_MASK);

    /* Starting the calibration of ADC0 */
    ADC0->SC3 |= (ADC_SC3_CAL_MASK);

    /* Wait until the calibration completes */
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
    {
    }
    /* ADC calibration failed, place breakpoint here for debug */
    if (ADC0->SC3 & ADC_SC3_CALF_MASK)
        __asm("nop");

    ui16Calib = ADC0->CLP0;
    ui16Calib += ADC0->CLP1;
    ui16Calib += ADC0->CLP2;
    ui16Calib += ADC0->CLP3;
    ui16Calib += ADC0->CLP4;
    ui16Calib += ADC0->CLPS;
    ui16Calib = ui16Calib >> 1;
    ui16Calib = ui16Calib | 0x8000; /* Set the MSB */
    /* Set the plus side gain register */
    ADC0->PG = ui16Calib;

    ui16Calib = ADC0->CLM0;
    ui16Calib += ADC0->CLM1;
    ui16Calib += ADC0->CLM2;
    ui16Calib += ADC0->CLM3;
    ui16Calib += ADC0->CLM4;
    ui16Calib += ADC0->CLMS;
    ui16Calib = ui16Calib >> 1;
    ui16Calib = ui16Calib | 0x8000; /* Set the MSB */
    /* Set the minus side gain register */
    ADC0->MG = ui16Calib;

    /* Starting the calibration of ADC1 */
    ADC1->SC3 |= (ADC_SC3_CAL_MASK);

    /* Wait until the calibration completes */
    while (!(ADC1->SC1[0] & ADC_SC1_COCO_MASK))
    {
    }
    /* ADC calibration failed, place breakpoint here for debug */
    if (ADC1->SC3 & ADC_SC3_CALF_MASK)
        __asm("nop");

    ui16Calib = ADC1->CLP0;
    ui16Calib += ADC1->CLP1;
    ui16Calib += ADC1->CLP2;
    ui16Calib += ADC1->CLP3;
    ui16Calib += ADC1->CLP4;
    ui16Calib += ADC1->CLPS;
    ui16Calib = ui16Calib >> 1;
    ui16Calib = ui16Calib | 0x8000; /* Set the MSB */
    /* Set the plus side gain register */
    ADC1->PG = ui16Calib;

    ui16Calib = ADC1->CLM0;
    ui16Calib += ADC1->CLM1;
    ui16Calib += ADC1->CLM2;
    ui16Calib += ADC1->CLM3;
    ui16Calib += ADC1->CLM4;
    ui16Calib += ADC1->CLMS;
    ui16Calib = ui16Calib >> 1;
    ui16Calib = ui16Calib | 0x8000; /* Set the MSB */
    /* Set the minus side gain register */
    ADC1->MG = ui16Calib;
    /* ------- ADC self calibration procedure end ----- */

    /* Write default values to SC3 register */
    ADC0->SC3 = 0;
    ADC1->SC3 = 0;
	
    /* Setting the clock to 12.5 MHz (ADIV - div bus clock (25 MHz) by 2 */
    ADC0->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(1));
    ADC1->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(1));

    /* H/W trigger, DMA disabled, default reference pin pair */
    ADC0->SC2 = (ADC_SC2_ADTRG_MASK);
    ADC1->SC2 = (ADC_SC2_ADTRG_MASK);

    /* Prepare first measurement */
    /* Pass initialization structure to ADC MC driver */
    g_sM1Adc16Init.ui16AdcArray = (&ui16AdcArray[0]);
    g_sM1Adc16Init.pui32Adc0Base = (ADC_Type *)ADC0;
    g_sM1Adc16Init.pui32Adc1Base = (ADC_Type *)ADC1;
    MCDRV_Adc16Init(&g_sM1AdcSensor, &g_sM1Adc16Init);

    /* Enable & setup interrupt */
    ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
    EnableIRQ(ADC0_IRQn);
    NVIC_SetPriority(ADC0_IRQn, 1);
}

/*!
* @brief      Initialization of the PDB for current and voltage sensing
*
* @param      void
*
* @return     none
*/
void InitPDB(void)
{
    /* Enable clock for PDB module */
    SIM->SCGC6 |= SIM_SCGC6_PDB0_MASK;

    /* PDB triggers for ADC0 module */
    /* PDB triggers for ADC1 module */
    /* PDB | CHANNEL | PRETRIGGER | VALUE */
    /* Set Delay middle of pwm period */
    PDB0->CH[0].DLY[0] = (g_sClockSetup.ui16PwmModulo / 6);
    PDB0->CH[1].DLY[0] = (g_sClockSetup.ui16PwmModulo / 6);
 
    /* Configure PDB SC register */
    /* Select trigger */
    /* PDB error interrupt enable */
    /* PDB interrupt enable */
    /* PDB enable */
    /* Mod and ChnDlym values will be updated from buffer by writing LDOK bit */
    PDB0->SC = (PDB_SC_PDBEN_MASK | PDB_SC_LDMOD(2) | PDB_SC_TRGSEL(8) | PDB_SC_PDBEIE_MASK | PDB_SC_PDBIE_MASK);

    /* LDOK needed for update of PDB0->MOD and PDBn_CH0DLYm values */
    PDB0->SC |= PDB_SC_LDOK_MASK;

    /* Enable channel 0, pretrigger 0 - ADC0_RA */
    /* Enable channel 0, pretrigger 1 - ADC0_RB */
    /* PDB Channel Back-to-back Enable */
    /* PDB Channel Pre-Trigger Enable */
    /* PDB Channel Pre-Trigger Output Select */
    PDB0->CH[0].C1 = (PDB_C1_EN(3) | PDB_C1_TOS(3) | PDB_C1_BB(2));

    /* Enable channel 1, pretrigger 0 - ADC1_RA */
    /* Enable channel 1, pretrigger 1 - ADC1_RB */
    /* PDB Channel Back-to-back Enable */
    /* PDB Channel Pre-Trigger Enable */
    /* PDB Channel Pre-Trigger Output Select */
    PDB0->CH[1].C1 = (PDB_C1_EN(3) | PDB_C1_TOS(3) | PDB_C1_BB(2));

    /* set counters modulo */
    PDB0->MOD = 0x7fff;

    /* Enable & setup interrupts */
    EnableIRQ(PDB0_PDB1_IRQn);
    NVIC_SetPriority(PDB0_PDB1_IRQn, 1); 
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
    GPIOA->PSOR = GPIO_PSOR_PTSO(1 << 18);
}
