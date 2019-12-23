/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_twrkv58f220m.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Commutation table */
const uint16_t bldcCommutationTableComp[16] = {
    /* swap, mask */
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

/* Structure for 3-phase PWM MC driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* Structure for time event driver */
mcdrv_ftm_cmt_t g_sM1CmtTmr;

/* Structure for current and voltage measurement */
mcdrv_hsadc_init_t g_sM1AdcInit;
mcdrv_hsadc_t g_sM1AdcSensor;

/* Structure for 3-phase MOSFET pre-driver MC driver*/
mcdrv_spi_drv3ph_t g_sM1Driver3ph;

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
	
    /* SPI peripheral init for 3-phase MOSFET pre-driver configuration */
    M1_MCDRV_DRV3PH_INIT();

    /* Init ADC */
    M1_MCDRV_ADC_PERIPH_INIT();

    /* Init FTM2 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();
    
    /* Init XBAR */
    M1_MCDRV_XBAR_PERIPH_INIT();

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
    g_sClockSetup.ui32FastPeripheralClock =  CLOCK_GetFreq(kCLOCK_FastPeriphClk); 
    g_sClockSetup.ui32BusClock = CLOCK_GetFreq(kCLOCK_BusClk); 
        
    g_sClockSetup.ui16PwmFreq = PWM_FREQ; /* 20 kHz */
                                             /* PWM module calculated as follows:
                                              * PWM_MOD = PWM_CLOCK / PWM_FREQUNCY = 75 MHz / 20 kHz = 3750 */
    g_sClockSetup.ui16PwmModulo = g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16PwmFreq;
    g_sClockSetup.ui16PwmDeadTime = g_sClockSetup.ui32FastPeripheralClock / (1000000000U / PWM_DEADTIME);
    g_sClockSetup.ui16CtrlLoopFreq = CTRL_LOOP_FREQ; /* 1 kHz */
}

/*!
* @brief   void InitPWMA(void)
*           - Initialization of the eFlexPWMA peripheral for motor M1
*           - 3-phase center-aligned PWM
*           - Top signals have negative polarity due to MC33937
*
* @param   void
*
* @return  none
*/
void InitPWMA0(void)
{
    /* Enable clock for eFlexPWM modules 0,1 and 2 in SIM module */
    SIM->SCGC4 |= (SIM_SCGC4_PWM0_SM0_MASK | SIM_SCGC4_PWM0_SM1_MASK | SIM_SCGC4_PWM0_SM2_MASK);

    /* Full cycle reload */
    PWM0->SM[0].CTRL |= PWM_CTRL_FULL(1);
    PWM0->SM[1].CTRL |= PWM_CTRL_FULL(1);
    PWM0->SM[2].CTRL |= PWM_CTRL_FULL(1);

    /* Value register initial values, duty cycle 50% */
    PWM0->SM[0].INIT = (uint16_t)(-(g_sClockSetup.ui16PwmModulo) / 2);
    PWM0->SM[1].INIT = (uint16_t)(-(g_sClockSetup.ui16PwmModulo) / 2);
    PWM0->SM[2].INIT = (uint16_t)(-(g_sClockSetup.ui16PwmModulo) / 2);

    PWM0->SM[0].VAL0 = (uint16_t)(0);
    PWM0->SM[1].VAL0 = (uint16_t)(0);
    PWM0->SM[2].VAL0 = (uint16_t)(0);

    PWM0->SM[0].VAL1 = (uint16_t)((g_sClockSetup.ui16PwmModulo / 2) - 1);
    PWM0->SM[1].VAL1 = (uint16_t)((g_sClockSetup.ui16PwmModulo / 2) - 1);
    PWM0->SM[2].VAL1 = (uint16_t)((g_sClockSetup.ui16PwmModulo / 2) - 1);

    PWM0->SM[0].VAL2 = (uint16_t)(-(g_sClockSetup.ui16PwmModulo) / 4);
    PWM0->SM[1].VAL2 = (uint16_t)(-(g_sClockSetup.ui16PwmModulo) / 4);
    PWM0->SM[2].VAL2 = (uint16_t)(-(g_sClockSetup.ui16PwmModulo) / 4);

    PWM0->SM[0].VAL3 = (uint16_t)(-(g_sClockSetup.ui16PwmModulo) / 4);
    PWM0->SM[1].VAL3 = (uint16_t)(-(g_sClockSetup.ui16PwmModulo) / 4);
    PWM0->SM[2].VAL3 = (uint16_t)(-(g_sClockSetup.ui16PwmModulo) / 4);

    PWM0->SM[0].VAL4 = (uint16_t)(0);
    PWM0->SM[1].VAL4 = (uint16_t)(0);
    PWM0->SM[2].VAL4 = (uint16_t)(0);

    PWM0->SM[0].VAL5 = (uint16_t)(0);
    PWM0->SM[1].VAL5 = (uint16_t)(0);
    PWM0->SM[2].VAL5 = (uint16_t)(0);

    /* Invert polarity for top transistors (PWMA A outputs) */
    PWM0->SM[0].OCTRL = PWM_OCTRL_POLA(TRUE);
    PWM0->SM[1].OCTRL = PWM_OCTRL_POLA(TRUE);
    PWM0->SM[2].OCTRL = PWM_OCTRL_POLA(TRUE);

    /* PWMA module 0 trigger on VAL4 enabled for HSADC synchronization */
    PWM0->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN((1 << 4));

    /* Set dead-time (number of Fast Peripheral Clocks)
       DTCNT0,1 = T_dead * f_fpc = 0.5us * 100MHz = 50 */
    PWM0->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16PwmDeadTime);
    PWM0->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16PwmDeadTime);
    PWM0->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16PwmDeadTime);

    PWM0->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16PwmDeadTime);
    PWM0->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16PwmDeadTime);
    PWM0->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16PwmDeadTime);

    /* Channels A and B disabled when fault 0 occurs */
    PWM0->SM[0].DISMAP[0] = PWM_DISMAP_DIS0A(0x00);
    PWM0->SM[1].DISMAP[0] = PWM_DISMAP_DIS0A(0x00);
    PWM0->SM[2].DISMAP[0] = PWM_DISMAP_DIS0A(0x00);
    PWM0->SM[0].DISMAP[0] = PWM_DISMAP_DIS0B(0x00);
    PWM0->SM[1].DISMAP[0] = PWM_DISMAP_DIS0B(0x00);
    PWM0->SM[2].DISMAP[0] = PWM_DISMAP_DIS0B(0x00);

    /* Modules one and two gets clock from module zero */
    /* Master reload active for modules one and two */
    /* Master sync active for modules one and two */
    PWM0->SM[1].CTRL2 = (PWM_CTRL2_CLK_SEL(0x02) | PWM_CTRL2_FRCEN(1) | PWM_CTRL2_FORCE_SEL(1) |
                         PWM_CTRL2_RELOAD_SEL(TRUE) | PWM_CTRL2_INIT_SEL(2));
    PWM0->SM[2].CTRL2 = (PWM_CTRL2_CLK_SEL(0x02) | PWM_CTRL2_FRCEN(1) | PWM_CTRL2_FORCE_SEL(1) |
                         PWM_CTRL2_RELOAD_SEL(TRUE) | PWM_CTRL2_INIT_SEL(2));

    /* Fault 0 active in logic level one, automatic clearing */
    PWM0->FCTRL = PWM_FCTRL_FLVL(0x01);
    PWM0->FCTRL |= PWM_FCTRL_FAUTO(0x01);

    /* PWMs are re-enabled at PWM full cycle */
    PWM0->FSTS |= PWM_FSTS_FFULL(0x01);

    /* PWM Fault filter - 20 Fast periph. clocks sample rate, 7 agreeing
       samples to activate */
    PWM0->FFILT = (PWM_FFILT_FILT_PER(20) | PWM_FFILT_FILT_CNT(7));

    /* Enable A&B PWM outputs for SM0 & 1 & 2  */
    PWM0->OUTEN = (PWM0->OUTEN & ~(uint16_t)PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(7);
    PWM0->OUTEN = (PWM0->OUTEN & ~(uint16_t)PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(7);

    /* Start PWMs (set load OK flags and run) */
    PWM0->MCTRL = (PWM0->MCTRL & ~(uint16_t)PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0xF);
    PWM0->MCTRL = (PWM0->MCTRL & ~(uint16_t)PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0xF);
    PWM0->MCTRL = (PWM0->MCTRL & ~(uint16_t)PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0xF);

    /* Initialize MC driver */
    g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWM0;
    g_sM1Pwm3ph.ui16PhASubNum = M1_PWM_PAIR_PHA; /* PWMA phase A submodule number */
    g_sM1Pwm3ph.ui16PhBSubNum = M1_PWM_PAIR_PHB; /* PWMA phase B submodule number */
    g_sM1Pwm3ph.ui16PhCSubNum = M1_PWM_PAIR_PHC; /* PWMA phase C submodule number */

    /* Initialization of PWM modulo */
    g_sM1Pwm3ph.ui16PwmModulo = g_sClockSetup.ui16PwmModulo;

    /* Initialization of BLDC commutation table */
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

    /* Bus clock, divide by 128, 1.28us @ 100 MHz clock */
    /* Bus clock as source clock for FTM */
    /* Pre-scale factor 128 */
    FTM1->SC = FTM_SC_PS(7) | FTM_SC_CLKS(1);
    
    /* calculate frequency of timer used for forced commutation
     * System clock divided by 2^FTM_prescaler */
    g_sClockSetup.ui32CmtTimerFreq = g_sClockSetup.ui32FastPeripheralClock >> (FTM1->SC&FTM_SC_PS_MASK);

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
    /* Enable clock to FTM2 module */
    SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;

    /* Configuration of FTM2 module */
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
    g_sClockSetup.ui16CtrlLoopModulo =
        (g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16CtrlLoopFreq) >> (FTM2->SC & FTM_SC_PS_MASK);
    FTM2->MOD = g_sClockSetup.ui16CtrlLoopModulo;

    /* LOADOK */
    FTM2->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Enable & setup interrupts */
    EnableIRQ(FTM2_IRQn);

    /* Set priority to interrupt */
    NVIC_SetPriority(FTM2_IRQn, 3);
}

/*
* @brief   void InitSPI(void)
*           - Initialization of the SPI peripheral for motor M1 3-phase MOSFET
*             pre-driver
*           - SPI configuration for MOSFET pre-driver MC33937
*
* @param   void
*
* @return  none
*/
void InitSPI(void)
{
    uint32_t srcClock_Hz;
    dspi_master_config_t masterConfig;

    /* Master config */
    masterConfig.whichCtar = kDSPI_Ctar0;
    masterConfig.ctarConfig.baudRate = 500000U;
    masterConfig.ctarConfig.bitsPerFrame = 8U;
    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveLow;
    masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
    masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / masterConfig.ctarConfig.baudRate;
    masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / masterConfig.ctarConfig.baudRate;
    masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / masterConfig.ctarConfig.baudRate;

    masterConfig.whichPcs = kDSPI_Pcs0;
    masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

    masterConfig.enableContinuousSCK = false;
    masterConfig.enableRxFifoOverWrite = false;
    masterConfig.enableModifiedTimingFormat = false;
    masterConfig.samplePoint = kDSPI_SckToSin0Clock;

    srcClock_Hz = CLOCK_GetFreq(SYS_CLK);
    DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);

    /* ---------------------------------------------------- */
    /* Initialization of pins required by MC33937 predriver */
    g_sM1Driver3ph.sSpiData.pSpiBase = (SPI_Type *)(SPI0); /* SPI Base Address */
    g_sM1Driver3ph.sSpiData.ui32Pcs = (1 << 0); /* 1 << PCS_number */

    /* Enable PIN & PORT */
    g_sM1Driver3ph.sSpiData.pGpioEnBase = (GPIO_Type *)(GPIOE); /* GPIOx Base Address */
    g_sM1Driver3ph.sSpiData.ui32GpioEnPin = 28; /* pin number for driver enabled */

    /* Interrupt PIN & PORT */
    g_sM1Driver3ph.sSpiData.pGpioIntBase = (GPIO_Type *)(GPIOB); /* GPIOx Base Address */
    g_sM1Driver3ph.sSpiData.ui32GpioIntPin = 8; /* pin number for interrupt detection */

    /* Reset PIN & PORT */
    g_sM1Driver3ph.sSpiData.bResetPinControl = FALSE;

    /* zero deadtime set in MC33937, dead time control from FTM*/
    g_sM1Driver3ph.ui16Deadtime = 0;
    MCDRV_Driver3PhConfig(&g_sM1Driver3ph);
    MCDRV_Driver3PhGetSr0(&g_sM1Driver3ph);
    MCDRV_Driver3PhGetSr1(&g_sM1Driver3ph);
    MCDRV_Driver3PhGetSr2(&g_sM1Driver3ph);
    MCDRV_Driver3PhGetSr3(&g_sM1Driver3ph);
    MCDRV_Driver3PhClearFlags(&g_sM1Driver3ph);
    MCDRV_Driver3PhSetEn(&g_sM1Driver3ph);
}

/*!
* @brief   void InitHSADC(void)
*           - Initialization of the HSADC peripheral
*           - Initialization of the A/D converter for current and voltage sensing
*
* @param   void
*
* @return  none
*/
void InitHSADC(void)
{
    /* ADC channel number assignment array to be passed to MC ADC driver */
    static uint16_t ui16AdcArray[12] = {HSADCA_PH_A, HSADCC_PH_A, HSADCA_PH_B, HSADCC_PH_B, HSADCA_PH_C, HSADCC_PH_C,
                                        HSADCA_UDCB, HSADCC_UDCB, HSADCA_IDCB, HSADCC_IDCB, HSADCA_AUX,  HSADCC_AUX};

    /* Enable clock for HSADC0(HSADCA+HSADCB) and HSADC1(HSADCC+HSADCD)
       modules */
    SIM->SCGC5 |= SIM_SCGC5_HSADC0_MASK;
    SIM->SCGC2 |= SIM_SCGC2_HSADC1_MASK;

    /* Triggered parallel mode on HSADCA and HSADCC */
    HSADC0->CTRL1 |= HSADC_CTRL1_SMODE(0x5);
    HSADC1->CTRL1 |= HSADC_CTRL1_SMODE(0x5);

    /* Enable end-of-scan interrupt */
    HSADC0->CTRL1 |= (HSADC_CTRL1_EOSIEA(TRUE));

    /* Enable hardware triggering of HSADCA and HSADCC */
    HSADC0->CTRL1 |= (HSADC_CTRL1_SYNCA(TRUE));
    HSADC1->CTRL1 |= (HSADC_CTRL1_SYNCA(TRUE));

    /* Start HSADCA and HSADCC */
    HSADC0->CTRL1 &= ~(HSADC_CTRL1_STOPA(TRUE));
    HSADC1->CTRL1 &= ~(HSADC_CTRL1_STOPA(TRUE));

    HSADC0->CTRL2 &= ~(HSADC_CTRL2_STOPB(TRUE));
    HSADC1->CTRL2 &= ~(HSADC_CTRL2_STOPB(TRUE));

    /* input clock is 50MHz (150MHz CPU clock divided by tree),
       single ended */
    HSADC0->CTRL2 = (HSADC0->CTRL2 & ~HSADC_CTRL2_DIVA_MASK) | HSADC_CTRL2_DIVA(0x02);
    HSADC1->CTRL2 = (HSADC1->CTRL2 & ~HSADC_CTRL2_DIVA_MASK) | HSADC_CTRL2_DIVA(0x02);
	
    /* Enable first two samples for HSADCA and HSADCC */
    HSADC0->SDIS = 0x0000;
    HSADC1->SDIS = 0x0000;

    /* Select PWMA0_trig source (routed using XBARA) as sync signal */
    SIM->SOPT7 = (SIM->SOPT7 & ~SIM_SOPT7_HSADC0AALTTRGEN_MASK) | SIM_SOPT7_HSADC0AALTTRGEN(0x00);
    SIM->SOPT7 = (SIM->SOPT7 & ~SIM_SOPT7_HSADC1AALTTRGEN_MASK) | SIM_SOPT7_HSADC1AALTTRGEN(0x00);

    /* Configure HSADC calibration process */
    /* Single-ended input calibration on HSADC0A and HSADC1C */
    HSADC0->CALIB |= HSADC_CALIB_REQSINGA_MASK;
    HSADC1->CALIB |= HSADC_CALIB_REQSINGA_MASK;

    /* Power-up HSADCA and HSADCC */
    HSADC0->PWR &= ~(HSADC_PWR_PDA_MASK | HSADC_PWR_PDB_MASK);
    HSADC1->PWR &= ~(HSADC_PWR_PDA_MASK | HSADC_PWR_PDB_MASK);

    /* Wait until calibration is completed */
    while (!(HSADC0->STAT & HSADC_STAT_EOCALIA_MASK))
    {
    }
    while (!(HSADC1->STAT & HSADC_STAT_EOCALIA_MASK))
    {
    }
    /* Prepare first measurement */
    /* Pass initialization structure to ADC MC driver */
    g_sM1AdcInit.ui16AdcArray = (&ui16AdcArray[0]);
    g_sM1AdcInit.pui32Adc0Base = (HSADC_Type *)HSADC0;
    g_sM1AdcInit.pui32Adc1Base = (HSADC_Type *)HSADC1;

    MCDRV_HSAdcInit(&g_sM1AdcSensor, &g_sM1AdcInit);

    /* Enable HSADCA IRQ */
    EnableIRQ(HSADC0_CCA_IRQn);
    NVIC_SetPriority(HSADC0_CCA_IRQn, 1);
}


/*!
* @brief   void InitXBAR(void)
*           - Initialization of the XBARA for triggers
*
* @param   void
*
* @return  none
*/
void InitXBAR(void)
{
    /* PWMA0_TRIG0 (XBARA_IN20) is connected to HSADC0A_TRIG (XBARA_OUT12) and HSADC1A_TRIG (XBARA_OUT42) */
    XBARA_Init(XBARA);
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputPwm0Ch0Trg0, kXBARA_OutputHsadc0ATrig);
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputPwm0Ch0Trg0, kXBARA_OutputHsadc1ATrig);
}