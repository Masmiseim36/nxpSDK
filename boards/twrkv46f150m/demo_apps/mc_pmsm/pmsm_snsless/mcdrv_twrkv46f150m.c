/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_twrkv46f150m.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* structure for 3-phase MOSFET predriver mc driver*/
mcdrv_spi_drv3ph_t g_sM1Driver3ph;

/* structure for current and voltage measurement*/
mcdrv_adc12_t g_sM1AdcSensor;

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

    /* SPI peripheral init for 3-phase MOSFET predriver configuration */
    M1_MCDRV_DRV3PH_INIT();

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
    g_sClockSetup.ui32FastPeripheralClock =  CLOCK_GetFreq(kCLOCK_FastPeriphClk);    
    g_sClockSetup.ui32BusClock = CLOCK_GetFreq(kCLOCK_BusClk);
    g_sClockSetup.ui16M1PwmFreq = M1_PWM_FREQ; /* 10kHz */
                                             /* PWM module calculated as follows:
                                              * PWM_MOD = PWM_CLOCK / PWM_FREQUNCY = 84 MHz / 10 kHz = 8400   */
    g_sClockSetup.ui16M1PwmModulo = g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16M1PwmFreq;
    g_sClockSetup.ui16M1PwmDeadTime = g_sClockSetup.ui32FastPeripheralClock / (1000000000U / M1_PWM_DEADTIME);
    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 1kHz */
}

/*!
* @brief   void InitPWMA(void)
*           - Initialization of the eFlexPWMA peripheral for motor M1
*           - 3-phase center-aligned PWM
*           - top signals have negative polarity due to MC33937
*
* @param   void
*
* @return  none
*/
void InitPWMA(void)
{
    /* Enable clock for eFlexPWM modules 0,1 and 2 in SIM module */
    SIM->SCGC4 |=
        SIM_SCGC4_eFlexPWM0_MASK | SIM_SCGC4_eFlexPWM1_MASK | SIM_SCGC4_eFlexPWM2_MASK | SIM_SCGC4_eFlexPWM3_MASK;
    
    /* Enable clock for XBARA module */
    SIM->SCGC5 |= SIM_SCGC5_XBARA_MASK;

    /* SM3 clock divided by 2 */
    PWMA->SM[3].CTRL = ((PWMA->SM[3].CTRL & ~(uint16_t)PWM_CTRL_PRSC_MASK) | (PWM_CTRL_PRSC(0x1)));

    /* Full cycle reload */
    PWMA->SM[0].CTRL |= PWM_CTRL_FULL_MASK;
    PWMA->SM[1].CTRL |= PWM_CTRL_FULL_MASK;
    PWMA->SM[2].CTRL |= PWM_CTRL_FULL_MASK;

    /* Value register initial values, duty cycle 50% */
    PWMA->SM[0].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMA->SM[1].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMA->SM[2].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMA->SM[3].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));

    PWMA->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMA->SM[1].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMA->SM[2].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));

    PWMA->SM[0].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMA->SM[1].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMA->SM[2].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMA->SM[3].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo * 
                                                 M1_FOC_FREQ_VS_PWM_FREQ) - 
                                                (g_sClockSetup.ui16M1PwmModulo / 2)) / 2 - 1);

    PWMA->SM[0].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMA->SM[1].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMA->SM[2].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));

    PWMA->SM[0].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMA->SM[1].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMA->SM[2].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));

    PWMA->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMA->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMA->SM[2].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMA->SM[3].VAL4 = PWM_VAL4_VAL4((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2) + 10));

    PWMA->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMA->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMA->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMA->SM[3].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* Invert polarity for top transistors (PWMA A outputs) */
    PWMA->SM[0].OCTRL |= PWM_OCTRL_POLA_MASK;
    PWMA->SM[1].OCTRL |= PWM_OCTRL_POLA_MASK;
    PWMA->SM[2].OCTRL |= PWM_OCTRL_POLA_MASK;

    /* PWMA module 3 trigger on VAL4 enabled for ADC synchronization */
    PWMA->SM[3].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4);

    /* set dead-time (number of Fast Peripheral Clocks) */
    PWMA->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMA->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMA->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMA->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMA->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMA->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);

    /* Channels A and B disabled when fault 0 occurs */
    PWMA->SM[0].DISMAP[0] = ((PWMA->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMA->SM[1].DISMAP[0] = ((PWMA->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMA->SM[2].DISMAP[0] = ((PWMA->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMA->SM[0].DISMAP[0] = ((PWMA->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMA->SM[1].DISMAP[0] = ((PWMA->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMA->SM[2].DISMAP[0] = ((PWMA->SM[0].DISMAP[0] & ~(uint16_t)PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));

    /* Modules one and two gets clock from module zero */
    PWMA->SM[1].CTRL2 = (PWMA->SM[1].CTRL2 & ~(uint16_t)PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWMA->SM[2].CTRL2 = (PWMA->SM[2].CTRL2 & ~(uint16_t)PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);

    /* Master reload active for modules one and two*/
    PWMA->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMA->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMA->SM[3].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master reload is generated every n-th opportunity */
    PWMA->SM[0].CTRL = (PWMA->SM[0].CTRL & ~(uint16_t)PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(M1_FOC_FREQ_VS_PWM_FREQ - 1);

    /* Master sync active for modules one and two*/
    PWMA->SM[1].CTRL2 = (PWMA->SM[1].CTRL2 & ~(uint16_t)PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWMA->SM[2].CTRL2 = (PWMA->SM[2].CTRL2 & ~(uint16_t)PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

    /* Master reload active for module three*/
    PWMA->SM[3].CTRL2 = (PWMA->SM[3].CTRL2 & ~(uint16_t)PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x1);

    /* Fault 0 active in logic level one, automatic clearing */
    PWMA->FCTRL = (PWMA->FCTRL & ~(uint16_t)PWM_FCTRL_FLVL_MASK) | PWM_FCTRL_FLVL(0x1);
    PWMA->FCTRL = (PWMA->FCTRL & ~(uint16_t)PWM_FCTRL_FAUTO_MASK) | PWM_FCTRL_FAUTO(0x1);

    /* Clear fault flags */
    PWMA->FSTS = (PWMA->FCTRL & ~(uint16_t)PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);

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

    /* ---------------------------------------- */
	/* Initialize MC driver */
    g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWMA;

    g_sM1Pwm3ph.ui16PhASubNum = M1_PWM_PAIR_PHA; /* PWMA phase A sub-module number */
    g_sM1Pwm3ph.ui16PhBSubNum = M1_PWM_PAIR_PHB; /* PWMA phase B sub-module number */
    g_sM1Pwm3ph.ui16PhCSubNum = M1_PWM_PAIR_PHC; /* PWMA phase C sub-module number */

    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
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
    g_sM1Driver3ph.sSpiData.pSpiBase = (SPI_Type *)(SPI); /* SPI Base Address */
    g_sM1Driver3ph.sSpiData.ui32Pcs = (1 << 0); /* 1 << PCS_number */

    /* Enable PIN & PORT */
    g_sM1Driver3ph.sSpiData.pGpioEnBase = (GPIO_Type *)(PTB); /* GPIOx Base Address */
    g_sM1Driver3ph.sSpiData.ui32GpioEnPin = 16; /* pin number for driver enabled */

    /* Interrupt PIN & PORT */
    g_sM1Driver3ph.sSpiData.pGpioIntBase = (GPIO_Type *)(PTC); /* GPIOx Base Address */
    g_sM1Driver3ph.sSpiData.ui32GpioIntPin = 6; /* pin number for interrupt detection */

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

/*
* @brief   void InitFTM0(void)
*           - Initialization of the FTM2 peripheral
*           - performs slow control loop counter
*
* @param   void
*
* @return  none
*/
void InitFTM0(void)
{
    /* enable clock to FTM module */
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;

    /* Configuration of FTM module */
    /* Disable write protection for certain registers */
    /* Enable the counter */
    FTM0->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Settings up FTM SC register for clock setup */
    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    /* Set prescaler to 16 */
    /* Enable interrupt */
    FTM0->SC = FTM_SC_PS(4) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

    /* Counter running in BDM mode */
    FTM0->CONF = FTM_CONF_BDMMODE(3);

    /* Set count value to 0 */
    FTM0->CNTIN = 0x0;

    /* 1kHz Slow Loop */
    g_sClockSetup.ui16M1SpeedLoopModulo =
        (g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16M1SpeedLoopFreq) >> (FTM0->SC & FTM_SC_PS_MASK);
    FTM0->MOD = g_sClockSetup.ui16M1SpeedLoopModulo;

    /* LOADOK */
    FTM0->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Enable & setup interrupts */
    EnableIRQ(FTM0_IRQn);
    NVIC_SetPriority(FTM0_IRQn, 3);
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
    /* Enable clock for ADC modules */
    SIM->SCGC5 |= SIM_SCGC5_ADC_MASK;

    /* Triggered parallel mode */
    /* Enable end-of-scan interrupt */
    /* Enable hardware triggering */
    ADC->CTRL1 = (ADC_CTRL1_SMODE(5) | ADC_CTRL1_EOSIE0_MASK | ADC_CTRL1_SYNC0_MASK);

    /* Start ADC */
    ADC->CTRL1 &= ~(ADC_CTRL1_STOP0_MASK);

    /* input clock is 24.66MHz (74MHz Fast Peripheral clock divided by 3), 
       single ended */
    /* Simultaneous mode */
    ADC->CTRL2 =(ADC_CTRL2_DIV0(2) | ADC_CTRL2_STOP1(1) | ADC_CTRL2_SYNC1(1) | ADC_CTRL2_SIMULT(1)); 

    /* Enable samples 0, 1, 8, 9 */
    ADC->SDIS = ADC_SDIS_DS(0xFCFC);

    /* Power-up ADCA and ADCB */
    ADC->PWR &= ~(ADC_PWR_PD0_MASK | ADC_PWR_PD1_MASK);

    /**************************************/
    /* motor M1 ADC driver initialization */
    /**************************************/
    /* offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;
    
    /* adc base address */
    g_sM1AdcSensor.pui32AdcBase = (ADC_Type *)ADC;
        
    /* Phase current measurement */
    /* Sector 1,6 - measured currents Ic & Ib */
    /* ADC0, channel Ic = M1_ADC0_PH_C, , SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaC = M1_ADC0_PH_C;
    g_sM1AdcSensor.sCurrSec16.ui16AdcNumPhaC = ADC0;
    /* ADC1, channel Ib = M1_ADC1_PH_B, , SAMPLE & RESULT = 8 */
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaB = M1_ADC1_PH_B;
    g_sM1AdcSensor.sCurrSec16.ui16AdcNumPhaB = ADC1;

    /* Sector 2,3 - measured currents Ic & Ia*/
    /* ADC0, channel Ic = M1_ADC0_PH_C, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaC = M1_ADC0_PH_C;
    g_sM1AdcSensor.sCurrSec23.ui16AdcNumPhaC = ADC0;
    /* ADC1, channel Ia = M1_ADC1_PH_A, SAMPLE & RESULT = 8 */
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaA = M1_ADC1_PH_A;
    g_sM1AdcSensor.sCurrSec23.ui16AdcNumPhaA = ADC1;

    /* Sector 4,5 - measured currents Ia & Ib */
    /* ADC0, channel Ia = M1_ADC0_PH_A, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaA = M1_ADC0_PH_A;
    g_sM1AdcSensor.sCurrSec45.ui16AdcNumPhaA = ADC0;
    /* ADC1, channel Ib = M1_ADC1_PH_B, SAMPLE & RESULT = 8  */
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaB = M1_ADC1_PH_B;
    g_sM1AdcSensor.sCurrSec45.ui16AdcNumPhaB = ADC1;
    
    /* UDCbus channel measurement */
    /*  channel Udcb = M1_ADC1_UDCB, SAMPLE & RESULT = 9 */
    g_sM1AdcSensor.ui16ChanNumVDcb = M1_ADC1_UDCB;
    g_sM1AdcSensor.ui16AdcNumVDcb = ADC1;
    
    /* Assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssignInit(&g_sM1AdcSensor);

    /* Enable & setup interrupt from ADC */
    EnableIRQ(ADCA_IRQn);           /* Enable interrupt */
    NVIC_SetPriority(ADCA_IRQn, 1); /* set priority to interrupt */
}
