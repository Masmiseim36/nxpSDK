/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_twrkv11z75m.h"
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
    g_sClockSetup.ui32BusClock = CLOCK_GetFreq(kCLOCK_BusClk);  
    
    g_sClockSetup.ui16PwmFreq = PWM_FREQ; /* 20 kHz */
                                             /* PWM module calculated as follows:
                                              * PWM_MOD = PWM_CLOCK / PWM_FREQUNCY = 75 MHz / 20 kHz = 3750 */
    g_sClockSetup.ui16PwmModulo = g_sClockSetup.ui32SystemClock / g_sClockSetup.ui16PwmFreq;
    g_sClockSetup.ui16PwmDeadTime = g_sClockSetup.ui32SystemClock / (1000000000U / PWM_DEADTIME);
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

    /* Disable all channels outputs using the OUTPUT MASK feature.
     * However, the output pins are still driven as GPIO since the
     * channel mode is set to FTM channel disabled after RESET */

    /* Disable write protection for certain registers */
    /* Free running counter and synchronization are different from TPM */
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
    FTM0->DEADTIME = FTM_DEADTIME_DTVAL(g_sClockSetup.ui16PwmDeadTime);

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
    FTM0->OUTMASK = FTM_OUTMASK_CH0OM_MASK | FTM_OUTMASK_CH1OM_MASK | FTM_OUTMASK_CH2OM_MASK | FTM_OUTMASK_CH3OM_MASK |
                    FTM_OUTMASK_CH4OM_MASK | FTM_OUTMASK_CH5OM_MASK;

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
    g_sM1Pwm3ph.pui32PwmBase = (FTM_Type *)(FTM0);    /* FTM0 base address */
    g_sM1Pwm3ph.ui16ChanPhA = M1_PWM_PAIR_PHA; /* PWM phase A top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhB = M1_PWM_PAIR_PHB; /* PWM phase B top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhC = M1_PWM_PAIR_PHC; /* PWM phase C top&bottom channel pair number */

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

    /* Bus clock, divide by 128, 1.706us @ 75 MHz clock */
    /* Bus clock as source clock for FTM */
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
*           - performs slow control loop counter
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
    FTM2->MOD = (g_sClockSetup.ui16PwmModulo / 16 * 20);

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
    g_sM1Driver3ph.sSpiData.ui32GpioEnPin = 29; /* pin number for driver enabled */

    /* Interrupt PIN & PORT */
    g_sM1Driver3ph.sSpiData.pGpioIntBase = (GPIO_Type *)(GPIOE); /* GPIOx Base Address */
    g_sM1Driver3ph.sSpiData.ui32GpioIntPin = 30; /* pin number for interrupt detection */

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
    static uint16_t ui16AdcArray[12] = {ADC0_PH_A, ADC1_PH_A, ADC0_PH_B, ADC1_PH_B, ADC0_PH_C, ADC1_PH_C,
                                        ADC0_UDCB, ADC1_UDCB, ADC0_IDCB, ADC1_IDCB, ADC0_AUX,  ADC1_AUX};

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
    ADC1->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(3));

    /* HW averaging enabled, 32 samples averaged, continuous conversion must be
       enabled */
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
    /* set the plus side gain register */
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

    /* Enable ADC0 IRQ */
    ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
    NVIC_EnableIRQ(ADC0_IRQn);
    NVIC_SetPriority(ADC0_IRQn, 1);

    /* Bus clock source (25 MHz), ADC0 clock = Bus clock / 1 (25 MHz), 12-bit */
    ADC0->CFG1 = (ADC_CFG1_MODE(1));
    ADC1->CFG1 = (ADC_CFG1_MODE(1));

    /* High-speed mode, 2 extra ADC clock cycles; 6 ADCK cycles sample time */
    ADC0->CFG2 = (ADC_CFG2_ADHSC_MASK | ADC_CFG2_ADLSTS(3));
    ADC1->CFG2 = (ADC_CFG2_ADHSC_MASK | ADC_CFG2_ADLSTS(3));

    /* H/W trigger, DMA disabled, default reference pin pair */
    ADC0->SC2 = (ADC_SC2_ADTRG_MASK);
    ADC1->SC2 = (ADC_SC2_ADTRG_MASK);

    /* Average disabled, single conversion mode */
    ADC0->SC3 = 0;
    ADC1->SC3 = 0;

    /* Prepare first measurement */
    /* Pass initialization structure to ADC MC driver */
    g_sM1Adc16Init.ui16AdcArray = (&ui16AdcArray[0]);
    g_sM1Adc16Init.pui32Adc0Base = (ADC_Type *)ADC0;
    g_sM1Adc16Init.pui32Adc1Base = (ADC_Type *)ADC1;
    MCDRV_Adc16Init_twr_kv11(&g_sM1AdcSensor, &g_sM1Adc16Init);
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

    /* CH0&1: enable pre-trigger 0&1, back2back on pre-trigger 1 */
    /* Enable channel 0, pre-trigger 0 - ADC0_RA */
    /* Enable channel 0, pre-trigger 1 - ADC0_RB */
    PDB0->CH[0].C1 = (PDB_C1_EN(3) | PDB_C1_TOS(3) | PDB_C1_BB(2));

    /* CH0&1: enable pre-trigger 0&1, back2back on pre-trigger 1 */
    /* Enable channel 1, pre-trigger 0 - ADC1_RA */
    /* Enable channel 1, pre-trigger 1 - ADC1_RB */

    PDB0->CH[1].C1 = (PDB_C1_EN(3) | PDB_C1_TOS(3) | PDB_C1_BB(2));

    /* CH0&1: pre-trigger 0 delay = middle of PWM period*/
    PDB0->CH[0].DLY[0] = (g_sClockSetup.ui16PwmModulo / 6); /* Set delay */
    PDB0->CH[1].DLY[0] = (g_sClockSetup.ui16PwmModulo / 6); /* Set delay */

    /* Set counters modulo */
    PDB0->MOD = 0x7fff;

    /* LDOK immediately updates internal registers, enable PDB0 */
    /* Internal registers are update upon trigger and LDOK, enable interrupt,
       no pre-scaler, trig. src 8 (FTM0-EXTTRIG), enable PDB, multiplication
       factor is 1, set LDOK */
    PDB0->SC = (PDB_SC_PDBEN_MASK | PDB_SC_LDMOD(2) | PDB_SC_TRGSEL(8) | PDB_SC_PDBEIE_MASK | PDB_SC_LDOK_MASK);

    /* Enable PDB ISR */
    EnableIRQ(PDB0_PDB1_IRQn);

    /* Set priority to interrupt */
    NVIC_SetPriority(PDB0_PDB1_IRQn, 1);
}
