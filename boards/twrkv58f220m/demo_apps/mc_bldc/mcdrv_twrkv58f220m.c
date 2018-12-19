/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
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
    /* mask, swap */
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

/* Configuration structure for 3-phase PWM driver */
mcdrv_pwm3ph_pwma_init_t g_sM1Pwm3phInit;

/* Configuration structure for ADC driver - phase currents,
   DC-bus voltage, aux */
mcdrv_hsadc_init_t g_sM1AdcInit;

/* Configuration structure for 3-phase MOSFET pre-driver */
mcdrv_spi_drv3ph_init_t g_sM1Driver3phInit;

/* Configuration structure for time event driver */
mcdrv_ftm_cmt_init_t g_sM1CmtTmrInit;

/* Structure for 3-phase PWM MC driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* Structure for time event driver */
mcdrv_ftm_cmt_t g_sM1CmtTmr;

/* Structure for current and voltage measurement */
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
    /* SPI peripheral init for 3-phase MOSFET pre-driver configuration */
    M1_MCDRV_DRV3PH_INIT();

    /* Init ADC */
    M1_MCDRV_ADC_PERIPH_INIT();

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
    g_sClockSetup.ui32CoreSystemClock = SystemCoreClock;
    g_sClockSetup.ui32FastBusClock =
        SystemCoreClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT) + 1);
    g_sClockSetup.ui32BusFlashClock =
        SystemCoreClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT) + 1);
    g_sClockSetup.ui16PwmFreq = PWM_FREQ; /* 20 kHz */
                                          /* PWM module calculated as follows:
                                           * PWM_MOD = FAST_BUS_CLOCK / PWM_FREQUNCY = 100 MHz / 20 kHz = 5000   */
    g_sClockSetup.ui16PwmModulo = g_sClockSetup.ui32FastBusClock / g_sClockSetup.ui16PwmFreq;
    g_sClockSetup.ui32CmtTimerFreq = g_sClockSetup.ui32FastBusClock / 128;
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
    PWM0->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(50);
    PWM0->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(50);
    PWM0->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(50);

    PWM0->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(50);
    PWM0->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(50);
    PWM0->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(50);

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
    PWM0->OUTEN = (PWM_OUTEN_PWMA_EN(0x7) | PWM_OUTEN_PWMB_EN(0x7));

    /* Start PWMs (set load OK flags and run) */
    PWM0->MCTRL = (PWM_MCTRL_CLDOK(0x7) | PWM_MCTRL_LDOK(0x7) | PWM_MCTRL_RUN(0x7));

    /* Initialize MC driver */
    g_sM1Pwm3phInit.pui32PwmBaseAddress = (PWM_Type *)PWM0;
    g_sM1Pwm3phInit.ui16PhASubNum = M1_PWM_PAIR_PHA; /* PWMA phase A submodule number */
    g_sM1Pwm3phInit.ui16PhBSubNum = M1_PWM_PAIR_PHB; /* PWMA phase B submodule number */
    g_sM1Pwm3phInit.ui16PhCSubNum = M1_PWM_PAIR_PHC; /* PWMA phase C submodule number */

    /* Initialization of PWM modulo */
    g_sM1Pwm3phInit.ui16PwmModulo = g_sClockSetup.ui16PwmModulo;

    /* Initialization of BLDC commutation table */
    g_sM1Pwm3phInit.pcBldcTable = &bldcCommutationTableComp[0];

    MCDRV_eFlexPwm3PhInit(&g_sM1Pwm3ph, &g_sM1Pwm3phInit);
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

    /* Enable Output Compare interrupt, output Compare, Software Output
     * Compare only (ELSnB:ELSnA = 0:0, output pin is not controlled by FTM) */
    FTM1->CONTROLS[0].CnSC = FTM_CnSC_MSA_MASK | FTM_CnSC_CHIE_MASK;

    /* Enable interrupt */
    EnableIRQ(FTM1_IRQn);

    /* Set priority to interrupt */
    NVIC_SetPriority(FTM1_IRQn, 1);

    /* Initialization FTM time event driver */
    g_sM1CmtTmrInit.pui32FtmBase = (FTM_Type *)(FTM1); /* FTM1 base address */
    g_sM1CmtTmrInit.ui16ChannelNum = M1_FTM_CMT_CHAN;  /* FTM1 compare channel selection */

    /* Pass initialization structure to the MC driver */
    MCDRV_FtmCmtInit(&g_sM1CmtTmr, &g_sM1CmtTmrInit);

    /* Timer variables pointer assertion */
    g_sM1CmtTmr.pui16FtmCntAct = &g_sM1Drive.ui16TimeCurrent;
    g_sM1CmtTmr.pui16FtmValueAct = &g_sM1Drive.ui16TimeCurrentEvent;
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

    /* Counter running in BDM mode */
    FTM2->CONF = FTM_CONF_BDMMODE(3);

    /* Set count value to 0 */
    FTM2->CNTIN = 0x0;

    /* 20x slower than fast loop */
    FTM2->MOD = (g_sClockSetup.ui16PwmModulo * 2 / 16 * 10);

    /* LOADOK */
    FTM2->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Settings up FTM SC register for clock setup */
    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    /* Set pre-scaler to 16 */
    /* Enable interrupt from FTM2 */
    FTM2->SC = FTM_SC_PS(4) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

    /* Enable interrupt */
    EnableIRQ(FTM2_IRQn);

    /* Set priority to interrupt */
    NVIC_SetPriority(FTM2_IRQn, 3);
}

/*!
* @brief      Initialization of the SPI for MOSFET pre-driver MC33937 configuration
*
* @param      void
*
* @return     none
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

    /* Initialization of pins required by MC33937 pre-driver */
    g_sM1Driver3phInit.pSpiBase = (SPI_Type *)(SPI0); /* SPI Base Address */
    g_sM1Driver3phInit.ui32Pcs = (0);                 /* PCS number */

    /* Enable PIN & PORT */
    CLOCK_EnableClock(kCLOCK_PortE);
    g_sM1Driver3phInit.pGpioEnBase = (GPIO_Type *)(GPIOE); /* GPIOx Base Address */
    g_sM1Driver3phInit.pPortEnBase = (PORT_Type *)(PORTE); /* PORTx Base Address */
    g_sM1Driver3phInit.ui32GpioEnPin = 28;                 /* Pin number for driver enabled */

    /* Interrupt PIN & PORT */
    CLOCK_EnableClock(kCLOCK_PortB);
    g_sM1Driver3phInit.pGpioIntBase = (GPIO_Type *)(GPIOB); /* GPIOx Base Address */
    g_sM1Driver3phInit.pPortIntBase = (PORT_Type *)(PORTB); /* PORTx Base Address */
    g_sM1Driver3phInit.ui32GpioIntPin = 8;                  /* Pin number for interrupt detection */

    /* Reset PIN & PORT */
    g_sM1Driver3phInit.pGpioResetBase = (GPIO_Type *)(GPIOB); /* GPIOx Base Address */
    g_sM1Driver3phInit.pPortResetBase = (PORT_Type *)(PORTB); /* PORTx Base Address */
    g_sM1Driver3phInit.ui32GpioResetPin = 4;                  /* Pin number for reset, driver */

    /* Pass initialization structure to the MC driver */
    MCDRV_Driver3PhInit(&g_sM1Driver3ph, &g_sM1Driver3phInit);

    /* Zero dead-time set in MC33937, dead time control from FTM */
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

    /* Triggered sequential mode on HSADCA and HSADCC */
    HSADC0->CTRL1 |= HSADC_CTRL1_SMODE(0x4);
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

    /* input clock is 60MHz (240MHz CPU clock divided by four),
       single ended */
    HSADC0->CTRL2 = (HSADC0->CTRL2 & ~(uint16_t)HSADC_CTRL2_DIVA_MASK) | HSADC_CTRL2_DIVA(0x03);
    HSADC1->CTRL2 = (HSADC1->CTRL2 & ~(uint16_t)HSADC_CTRL2_DIVA_MASK) | HSADC_CTRL2_DIVA(0x03);

    /* Enable first two samples for HSADCA and HSADCC */
    HSADC0->SDIS = 0x0000;
    HSADC1->SDIS = 0x0000;

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
