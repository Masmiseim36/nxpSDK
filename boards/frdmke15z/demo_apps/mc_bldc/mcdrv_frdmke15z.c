/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_frdmke15z.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* commutation table */
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

/* configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;

/* structure for time event driver */
mcdrv_ftm_cmt_t g_sM1CmtTmr;

/* structure for current and voltage measurement*/
mcdrv_adc16_init_t g_sM1Adc16Init;
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
    
    /* init ADC */
    M1_MCDRV_ADC_PERIPH_INIT();

    /* init PDB */
    M1_MCDRV_PDB_INIT();

    /* init FTM2 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();

    /* init FTM1 */
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
    g_sClockSetup.ui32SystemClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    g_sClockSetup.ui32SysOscAsyncDiv2 = CLOCK_GetFreq(kCLOCK_ScgSysOscAsyncDiv2Clk);

    g_sClockSetup.ui16PwmFreq = PWM_FREQ; /* 20 kHz */
                                          /* PWM module calculated as follows:
                                          * PWM_MOD = CORE_CLOCK / PWM_FREQUNCY = 72 MHz / 20 kHz = 3600   */
    g_sClockSetup.ui16PwmModulo = g_sClockSetup.ui32SystemClock / g_sClockSetup.ui16PwmFreq;
    g_sClockSetup.ui16PwmDeadTime = (g_sClockSetup.ui32SystemClock / (1000000000U / PWM_DEADTIME))/4;
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
    /* enable the clock for FTM0 */
    PCC->CLKCFG[PCC_FLEXTMR0_INDEX] = PCC_CLKCFG_CGC(TRUE);

    /* Disable all channels outputs using the OUTPUT MASK feature */
    FTM0->OUTMASK = (FTM_OUTMASK_CH0OM_MASK | FTM_OUTMASK_CH1OM_MASK | FTM_OUTMASK_CH2OM_MASK | FTM_OUTMASK_CH3OM_MASK |
                     FTM_OUTMASK_CH4OM_MASK | FTM_OUTMASK_CH5OM_MASK);

    /* disable write protection for certain registers
     * Enable the counter */
    FTM0->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* initial value of the FTM counter */
    FTM0->CNTIN = (uint32_t)(-(g_sClockSetup.ui16PwmModulo) / 2);

    /* modulo value */
    FTM0->MOD = (uint32_t)((g_sClockSetup.ui16PwmModulo / 2) - 1);

    /* edge alligned PWM */
    FTM0->CONTROLS[0].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[1].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[2].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[3].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[4].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[5].CnSC = FTM_CnSC_ELSB_MASK;

    /* maximum loading point enable */
    FTM0->SYNC = FTM_SYNC_CNTMAX_MASK;

    /* For center aligned PWM using combine mode:
     * COMBINE = 1 - combine mode set
     * COMP = 1 - complementary PWM set
     * DTEN = 1 - dead-time enabled
     * SYNCEN = 1 - PWM update synchronization enabled
    */
    /* complementary mode */
    FTM0->COMBINE = FTM_COMBINE_COMBINE0_MASK | FTM_COMBINE_COMP0_MASK | FTM_COMBINE_DTEN0_MASK |
                    FTM_COMBINE_SYNCEN0_MASK | FTM_COMBINE_COMBINE1_MASK | FTM_COMBINE_COMP1_MASK |
                    FTM_COMBINE_DTEN1_MASK | FTM_COMBINE_SYNCEN1_MASK | FTM_COMBINE_COMBINE2_MASK |
                    FTM_COMBINE_COMP2_MASK | FTM_COMBINE_DTEN2_MASK | FTM_COMBINE_SYNCEN2_MASK;

    /* Dead time 0.5us */
    FTM0->DEADTIME = FTM_DEADTIME_DTPS(2) | FTM_DEADTIME_DTVAL(g_sClockSetup.ui16PwmDeadTime);

    /* Enable generation of trigger when FTM counter is equal to CNTIN register */
    FTM0->EXTTRIG = FTM_EXTTRIG_INITTRIGEN_MASK;

    /* following lines configure:
     - enhanced PWM synchronization, FTM counter reset on SW sync
     - output SW control / polarity registers updated on PWM synchronization
       (not on rising edge of clock)
     - output SW control/inverting(swap)/mask registers updated from buffers on
       SW synchronization */
    FTM0->SYNCONF = FTM_SYNCONF_INVC_MASK | FTM_SYNCONF_SWOC_MASK | FTM_SYNCONF_SYNCMODE_MASK |
                    FTM_SYNCONF_SWRSTCNT_MASK | FTM_SYNCONF_SWOM_MASK | FTM_SYNCONF_SWINVC_MASK |
                    FTM_SYNCONF_SWSOC_MASK;

    /* update of FTM settings */
    FTM0->CNT = 0x0001;

    /* no ISR, counting up, bus clock, divide by 1 */
    FTM0->SC = FTM_SC_CLKS(1);

    /* Initial setting of value registers to 50 % of duty cycle  */
    FTM0->CONTROLS[0].CnV = (uint32_t)(-g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[1].CnV = (uint32_t)(g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[2].CnV = (uint32_t)(-g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[3].CnV = (uint32_t)(g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[4].CnV = (uint32_t)(-g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[5].CnV = (uint32_t)(g_sClockSetup.ui16PwmModulo / 4);

    /* software trigger for PWM synchronization is selected */
    FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK;

    /* enable loading updated values */
    FTM0->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

    /* Enable PWM */
    FTM0->SC |= (FTM_SC_PWMEN0(TRUE) | FTM_SC_PWMEN1(TRUE) | FTM_SC_PWMEN2(TRUE) | FTM_SC_PWMEN3(TRUE) |
                 FTM_SC_PWMEN4(TRUE) | FTM_SC_PWMEN5(TRUE));

    /* Initialization FTM 3-phase PWM mc driver */
    g_sM1Pwm3ph.pui32PwmBase = (FTM_Type *)(FTM0);    /* FTM0 base address */
    g_sM1Pwm3ph.ui16ChanPhA = M1_PWM_PAIR_PHA; /* PWM phase A top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhB = M1_PWM_PAIR_PHB; /* PWM phase B top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhC = M1_PWM_PAIR_PHC; /* PWM phase C top&bottom channel pair number */

    /* initialization of PWM modulo */
    g_sM1Pwm3ph.ui16PwmModulo = g_sClockSetup.ui16PwmModulo;

    /* initialization of BLDC commucation table */
    g_sM1Pwm3ph.pcBldcTable = &bldcCommutationTableComp[0];
    
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
    /* enable clock to FTM1 module */
    PCC->CLKCFG[PCC_FLEXTMR1_INDEX] = PCC_CLKCFG_CGC(TRUE);
    /* configuration of FTM1 module */
    /* write protection is disabled */
    /* free running counter and synchronization are different from TPM
       behaviour */
    FTM1->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* free running timer */
    /* modulo value */
    FTM1->MOD = 0xffff;

    /* bus clock, divide by 128, 1.7777us @ 72 MHz clock */
    /* core clock as source clock for FTM */
    /* Prescale factor 128 */
    FTM1->SC = FTM_SC_PS(7) | FTM_SC_CLKS(1);
      
    /* calculate frequency of timer used for forced commutation
     * System clock divided by 2^FTM_prescaler */
    g_sClockSetup.ui32CmtTimerFreq = g_sClockSetup.ui32SystemClock >> (FTM1->SC&FTM_SC_PS_MASK);  

    /* enable Output Compare interrupt, output Compare, Software Output
     * Compare only (ELSnB:ELSnA = 0:0, output pin is not controlled by FTM) */
    FTM1->CONTROLS[0].CnSC = FTM_CnSC_MSA_MASK | FTM_CnSC_CHIE_MASK;

    /* enable interrupt */
    NVIC_EnableIRQ(FTM1_IRQn);

    /* Set priority to interrupt */
    NVIC_SetPriority(FTM1_IRQn, 1);

    /* initialization FTM time event driver */
    g_sM1CmtTmr.pui32FtmBase = (FTM_Type *)(FTM1); /* FTM1 base address */
    g_sM1CmtTmr.ui16ChannelNum = M1_FTM_CMT_CHAN;  /* FTM1 compare channel selection */
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
    PCC->CLKCFG[PCC_FLEXTMR2_INDEX] = PCC_CLKCFG_CGC(TRUE);

    /* Configuration of FTM module */
    /* Disable write protection for certain registers */
    /* Enable the counter */
    FTM2->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Counter running in BDM mode */
    FTM2->CONF = FTM_CONF_BDMMODE(3);

    /* Set count value to 0 */
    FTM2->CNTIN = 0x0;

    /* 20x slower than fast loop */
    FTM2->MOD = (g_sClockSetup.ui16PwmModulo / 16 * 20);

    /* LOADOK */
    FTM2->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Settings up FTM SC register for clock setup */
    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    /* Set pre-scaler to 16 */
    /* Enable interrupt */
    FTM2->SC = FTM_SC_PS(4) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

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

    /* ADC channel number assignment array to be passeed to MC ADC driver */
    static uint16_t ui16AdcArray[12] = {M1_ADC0_PH_A, M1_ADC1_PH_A, M1_ADC0_PH_B, M1_ADC1_PH_B,
                                        M1_ADC0_PH_C, M1_ADC1_PH_C, M1_ADC0_UDCB, M1_ADC1_UDCB,
                                        M1_ADC0_IDCB, M1_ADC1_IDCB, M1_ADC0_AUX,  M1_ADC1_AUX};

    /* enable clock for ADC0 and ADC1 modules*/
    PCC->CLKCFG[PCC_ADC1_INDEX] = (PCC_CLKCFG_CGC(0));
    PCC->CLKCFG[PCC_ADC0_INDEX] = (PCC_CLKCFG_CGC(0));

    PCC->CLKCFG[PCC_ADC1_INDEX] = (PCC_CLKCFG_CGC(TRUE) | PCC_CLKCFG_PCS(1));
    PCC->CLKCFG[PCC_ADC0_INDEX] = (PCC_CLKCFG_CGC(TRUE) | PCC_CLKCFG_PCS(1));

    /* ------- ADC self calibration procedure start ----- */
    /* setting the clock to 3.125 MHz (ADIV - div bus clock (25 MHz) by 8 and
     * ADICLK bits) and single-ended 12-bit conversion (MODE bits)
     */
    /* single-ended 12-bit conversion */
    /* set divide ratio to 8 */
    /* input clock select - BUS clock  */
    ADC0->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(3));

    /* single-ended 12-bit conversion */
    /* set divide ratio to 8 */
    /* input clock select - BUS clock  */
    ADC1->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(3));

    /* HW averaging enabled, 32 samples averaged, continuous conversion must be enabled */
    /* Enable hardware average */
    /* 32 samples averaged */
    ADC0->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3));
    ADC1->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3));

    /* starting the calibration of ADC0 */
    ADC0->SC3 |= (ADC_SC3_CAL_MASK);

    /* wait until the calibration completes */
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
    {
    }
    /* ADC calibration failed, place breakpoint here for debug */
    /* e.g.: if (ADC0->SC3 & ADC_SC3_CALF_MASK) __asm ("nop"); */
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

    /* starting the calibration of ADC1 */
    ADC1->SC3 |= (ADC_SC3_CAL_MASK);

    /* wait until the calibration completes */
    while (!(ADC1->SC1[0] & ADC_SC1_COCO_MASK))
    {
    }
    /* adc calibration failed, place breakpoint here for debug */
    /* e.g.: if (ADC0->SC3 & ADC_SC3_CALF_MASK) __asm ("nop"); */

    ui16Calib = ADC1->UG;
    ui16Calib += ADC1->CLP0;
    ui16Calib += ADC1->CLP1;
    ui16Calib += ADC1->CLP2;
    ui16Calib += ADC1->CLP3;
    ui16Calib += ADC1->CLPS;

    /* set the plus side gain register */
    if (ui16Calib >= 0x800)
        ui16Calib = 0xffff;
    ADC1->G = ui16Calib;
    /* ------- ADC self calibration procedure end ----- */

    /* Write default values to SC3 register */
    ADC0->SC3 = 0;
    ADC1->SC3 = 0;

    /* bus clock source (25 MHz), ADC0 clock = Bus clock / 1 (25 MHz), 12-bit */
    ADC0->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(1));
    ADC1->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(1));

    /* H/W trigger, DMA disabled, default reference pin pair */
    ADC0->SC2 = (ADC_SC2_ADTRG_MASK);
    ADC1->SC2 = (ADC_SC2_ADTRG_MASK);

    /* prepare first measurement */
    /* pass initialization structure to ADC MC driver */
    g_sM1Adc16Init.ui16AdcArray = (&ui16AdcArray[0]);
    g_sM1Adc16Init.pui32Adc0Base = (ADC_Type *)ADC0;
    g_sM1Adc16Init.pui32Adc1Base = (ADC_Type *)ADC1;
    MCDRV_Adc16Init(&g_sM1AdcSensor, &g_sM1Adc16Init);

    /* enable & setup interrupt */
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
    /* enable clock for PDB module */
    PCC->CLKCFG[PCC_PDB0_INDEX] = PCC_CLKCFG_CGC(TRUE);

    /* PDB triggers for ADC0 module */
    /* PDB triggers for ADC1 module */
    /* PDB | CHANNEL | PRETRIGGER | VALUE */
    /* Set pre-trigger 0 delay=middle of pwm period */
    PDB0->CH[0].DLY[0] = (g_sClockSetup.ui16PwmModulo / 2);
    PDB0->CH[1].DLY[0] = (g_sClockSetup.ui16PwmModulo / 2);

    /* Enable channel 0, pre-trigger 0 - ADC0_RA */
    /* Enable channel 0, pre-trigger 1 - ADC0_RB */
    /* PDB Channel Back-to-back Enable */
    /* PDB Channel Pre-Trigger Enable */
    /* PDB Channel Pre-Trigger Output Select */
    PDB0->CH[0].C1 = (PDB_C1_EN(3) | PDB_C1_TOS(3) | PDB_C1_BB(2));

    /* Enable channel 1, pre-trigger 0 - ADC1_RA */
    /* Enable channel 1, pre-trigger 1 - ADC1_RB */
    /* PDB Channel Back-to-back Enable */
    /* PDB Channel Pre-Trigger Enable */
    /* PDB Channel Pre-Trigger Output Select */
    PDB0->CH[1].C1 = (PDB_C1_EN(3) | PDB_C1_TOS(3) | PDB_C1_BB(2));

    /* set counters modulo */
    PDB0->MOD = 0x7fff;

    /* LDOK immediately updates internal registers, enable PDB0 */
    /* internal registers are update upon trigger and LDOK, enable interrupt,
       no pre-scaler, trig. src 0 (FTM0-EXTTRIG), enable PDB, multiplication
       factor is 1, set LDOK */

    PDB0->SC = (PDB_SC_PDBEN_MASK | PDB_SC_LDMOD(2) | PDB_SC_TRGSEL(0) | PDB_SC_PDBEIE_MASK | PDB_SC_LDOK_MASK);

    /* Enable & setup interrupts */
    EnableIRQ(PDB0_IRQn);
    NVIC_SetPriority(PDB0_IRQn, 1);
}
