/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_twrkv46f150m.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Commutation table */
const char bldcCommutationTableComp[16] = {
    /* swap, mask */
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

/* Configuration structure for 3-phase PWM driver */
mcdrv_pwm3ph_ftm_init_t g_sM1Pwm3phInit;

/* Configuration structure for ADC driver - phase currents,
   DC-bus voltage, aux */
mcdrv_adc12_init_t g_sM1AdcInit;

/* Configuration structure for 3-phase MOSFET pre-driver */
mcdrv_spi_drv3ph_init_t g_sM1Driver3phInit;

/* Configuration structure for time event driver */
mcdrv_ftm_cmt_init_t g_sM1CmtTmrInit;

/* Structure for 3-phase PWM MC driver */
mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;

/* Structure for time event driver */
mcdrv_ftm_cmt_t g_sM1CmtTmr;

/* Structure for current and voltage measurement */
mcdrv_adc12_t g_sM1AdcSensor;

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

    /* Init PDB */
    M1_MCDRV_PDB_INIT();

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
    g_sClockSetup.ui32CoreSystemClock = SystemCoreClock;
    g_sClockSetup.ui32FastBusClock =
        SystemCoreClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT) + 1);
    g_sClockSetup.ui32BusFlashClock =
        SystemCoreClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT) + 1);
    g_sClockSetup.ui16PwmFreq = PWM_FREQ; /* 20 kHz */
                                          /* PWM module calculated as follows:
                                           * PWM_MOD = FAST_BUS_CLOCK / PWM_FREQUNCY = 74 MHz / 20 kHz = 3750   */
    g_sClockSetup.ui16PwmModulo = g_sClockSetup.ui32FastBusClock / g_sClockSetup.ui16PwmFreq;
    g_sClockSetup.ui32CmtTimerFreq = g_sClockSetup.ui32FastBusClock / 128;
    g_sClockSetup.ui16CtrlLoopFreq = CTRL_LOOP_FREQ; /* 1 kHz */
}

/*!
* @brief   Initialization of the FTM0 peripheral for motor M1
*
* @param   void
*
* @return  none
*/
void InitFTM0(void)
{
    /* Enable the clock for FTM0 */
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;

    /* Disable all channels outputs using the OUTPUT MASK feature.
     * However, the output pins are still driven as GPIO since the
     * channel mode is set to FTM channel disabled after RESET */

    /* disable write protection for certain registers */
    /* free running counter and synchronization are different from TPM */
    FTM0->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Initial value of the FTM counter */
    FTM0->CNTIN = (uint32_t)(-(g_sClockSetup.ui16PwmModulo) / 2);

    /* Modulo value */
    FTM0->MOD = (uint32_t)((g_sClockSetup.ui16PwmModulo / 2) - 1);

    /* Edge aligned PWM */
    FTM0->CONTROLS[0].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[1].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[2].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[3].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[4].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[5].CnSC = FTM_CnSC_ELSB_MASK;

    /* Maximum loading point enable */
    /* OUTMASK register is updated  of its buffer only by the PWM
       synchronization */
    FTM0->SYNC = FTM_SYNC_CNTMAX_MASK | FTM_SYNC_SYNCHOM_MASK;

    /* Complementary mode */
    FTM0->COMBINE = FTM_COMBINE_COMBINE0_MASK | FTM_COMBINE_COMP0_MASK | FTM_COMBINE_DTEN0_MASK |
                    FTM_COMBINE_SYNCEN0_MASK | FTM_COMBINE_COMBINE1_MASK | FTM_COMBINE_COMP1_MASK |
                    FTM_COMBINE_DTEN1_MASK | FTM_COMBINE_SYNCEN1_MASK | FTM_COMBINE_COMBINE2_MASK |
                    FTM_COMBINE_COMP2_MASK | FTM_COMBINE_DTEN2_MASK | FTM_COMBINE_SYNCEN2_MASK;

    /* Polarity setting, 3ppa driver high sides are active low */
    FTM0->POL = FTM_POL_POL0_MASK | FTM_POL_POL2_MASK | FTM_POL_POL4_MASK;

    /* DTPS - dead-time pre-scaler: 0,1 = 1; 2 = 4; 3 = 16 */
    /* DTVAL - dead-time value (0-63): dead-time period = DTPS x DTVAL */
    FTM0->DEADTIME = FTM_DEADTIME_DTVAL(63);

    /* Enable generation of trigger when FTM counter is equal to CNTIN register */
    FTM0->EXTTRIG = FTM_EXTTRIG_INITTRIGEN_MASK;

    /* Following lines configure:
     - enhanced PWM synchronization, FTM counter reset on SW sync
     - output SW control / polarity registers updated on PWM synchronization
       (not on rising edge of clock)
     - output SW control/inverting(swap)/mask registers updated from buffers on
       SW synchronization */
    FTM0->SYNCONF = FTM_SYNCONF_INVC_MASK | FTM_SYNCONF_SWOC_MASK | FTM_SYNCONF_SYNCMODE_MASK |
                    FTM_SYNCONF_SWRSTCNT_MASK | FTM_SYNCONF_SWOM_MASK | FTM_SYNCONF_SWINVC_MASK |
                    FTM_SYNCONF_SWSOC_MASK;

    /* Channel output is masked */
    FTM0->OUTMASK = 0x3f;

    /* Update of FTM settings */
    FTM0->CNT = 0x0001;

    /* No ISR, counting up, bus clock, divide by 1 */
    FTM0->SC = FTM_SC_CLKS(1);

    /* Channel value */
    FTM0->CONTROLS[0].CnV = (uint32_t)(-g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[1].CnV = (uint32_t)(g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[2].CnV = (uint32_t)(-g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[3].CnV = (uint32_t)(g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[4].CnV = (uint32_t)(-g_sClockSetup.ui16PwmModulo / 4);
    FTM0->CONTROLS[5].CnV = (uint32_t)(g_sClockSetup.ui16PwmModulo / 4);

    /* Software trigger for PWM synchronization is selected */
    FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK;

    /* Enable loading updated values */
    FTM0->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

    /* Initialization FTM 3-phase PWM MC driver */
    g_sM1Pwm3phInit.pui32PwmBase = (FTM_Type *)(FTM0);    /* FTM0 base address */
    g_sM1Pwm3phInit.ui16ChanPairNumPhA = M1_PWM_PAIR_PHA; /* PWM phase A */
    g_sM1Pwm3phInit.ui16ChanPairNumPhB = M1_PWM_PAIR_PHB; /* PWM phase B */
    g_sM1Pwm3phInit.ui16ChanPairNumPhC = M1_PWM_PAIR_PHC; /* PWM phase C */

    /* Initialization of PWM modulo */
    g_sM1Pwm3phInit.ui16PwmModulo = g_sClockSetup.ui16PwmModulo;

    /* Initialization of BLDC commutation table */
    g_sM1Pwm3phInit.pcBldcTable = &bldcCommutationTableComp[0];

    /* Pass initialization structure to the MC driver */
    MCDRV_FtmPwm3PhInit(&g_sM1Pwm3ph, &g_sM1Pwm3phInit); /* MC driver initialization */
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

    /* 20x slower than fast loop */
    FTM3->MOD = (g_sClockSetup.ui16PwmModulo / 16 * 20);

    /* LOADOK */
    FTM3->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Settings up FTM SC register for clock setup */
    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    /* Set pre-scaler to 16 */
    /* Enable interrupt from FTM2 */
    FTM3->SC = FTM_SC_PS(4) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

    /* Enable & setup interrupts */
    EnableIRQ(FTM3_IRQn);
    NVIC_SetPriority(FTM3_IRQn, 3);
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
    g_sM1Driver3phInit.pSpiBase = (SPI_Type *)(SPI); /* SPI Base Address */
    g_sM1Driver3phInit.ui32Pcs = (0);                /* PCS number */

    /* Enable PIN & PORT */
    CLOCK_EnableClock(kCLOCK_PortB);
    g_sM1Driver3phInit.pGpioEnBase = (GPIO_Type *)(PTB);   /* GPIOx Base Address */
    g_sM1Driver3phInit.pPortEnBase = (PORT_Type *)(PORTB); /* PORTx Base Address */
    g_sM1Driver3phInit.ui32GpioEnPin = 16;                 /* Pin number for driver enabled */

    /* Interrupt PIN & PORT */
    CLOCK_EnableClock(kCLOCK_PortC);
    g_sM1Driver3phInit.pGpioIntBase = (GPIO_Type *)(PTC);   /* GPIOx Base Address */
    g_sM1Driver3phInit.pPortIntBase = (PORT_Type *)(PORTC); /* PORTx Base Address */
    g_sM1Driver3phInit.ui32GpioIntPin = 6;                  /* Pin number for interrupt detection */

    /* Reset PIN & PORT */
    g_sM1Driver3phInit.pGpioResetBase = (GPIO_Type *)(PTC);   /* GPIOx Base Address */
    g_sM1Driver3phInit.pPortResetBase = (PORT_Type *)(PORTC); /* PORTx Base Address */
    g_sM1Driver3phInit.ui32GpioResetPin = 7;                  /* Pin number for reset, driver */

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
    static uint16_t ui16AdcArray[12] = {ADC0_PH_A, ADC1_PH_A, ADC0_PH_B, ADC1_PH_B, ADC0_PH_C, ADC1_PH_C,
                                        ADC0_UDCB, ADC1_UDCB, ADC0_IDCB, ADC1_IDCB, ADC0_AUX,  ADC1_AUX};

    /* Enable clock for ADC modules */
    SIM->SCGC5 |= SIM_SCGC5_ADC_MASK;

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
    ADC->SDIS = ADC_SDIS_DS(0xFCFC);

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
* @brief   Initialization of the PDB for current and voltage sensing
*
* @param   void
*
* @return  none
*/
void InitPDB(void)
{
    /* Enable clock for PDB module */
    SIM->SCGC6 |= SIM_SCGC6_PDB0_MASK;
    SIM->SOPT7 |= SIM_SOPT7_ADCAALTTRGEN(1);

    /* CH0: enable pre-trigger 0, */
    /* Enable channel 0, pre-trigger 0 - ADC0_RA */
    PDB0->CH[0].C1 = (PDB_C1_EN(1) | PDB_C1_TOS(1));

    /* CH0: pre-trigger 0 delay=middle of pwm period*/
    PDB0->CH[0].DLY[0] = (g_sClockSetup.ui16PwmModulo / 2);

    /* Set counters modulo */
    PDB0->MOD = 0x7fff;

    /* LDOK immediately updates internal registers, enable PDB0 */
    /* internal registers are update upon trigger and LDOK, enable interrupt,
       no pre-scaler, trig. src 8 (FTM0-EXTTRIG), enable PDB, multiplication
       factor is 1, set LDOK */
    PDB0->SC = (PDB_SC_PDBEN_MASK | PDB_SC_LDMOD(2) | PDB_SC_TRGSEL(8) | PDB_SC_PDBEIE_MASK | PDB_SC_LDOK_MASK);

    /* Enable PDB ISR */
    EnableIRQ(PDB0_IRQn);

    /* Set priority to interrupt */
    NVIC_SetPriority(PDB0_IRQn, 1);
}
