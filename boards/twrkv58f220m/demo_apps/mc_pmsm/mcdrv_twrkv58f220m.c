/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "mcdrv_twrkv58f220m.h"
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
    g_sClockSetup.ui32FastPeripheralClock =
        SystemCoreClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT) + 1);
    g_sClockSetup.ui32BusClock =
        SystemCoreClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT) + 1);
    g_sClockSetup.ui16PwmFreq = M1_PWM_FREQ; /* 10kHz */
                                             /* PWM module calculated as follows:
                                              * PWM_MOD = PWM_CLOCK / PWM_FREQUENCY = 240 MHz / 10 kHz = 24000 */
    g_sClockSetup.ui16PwmModulo = g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16PwmFreq;
    g_sClockSetup.ui16PwmDeadTime = g_sClockSetup.ui32FastPeripheralClock / (1000000000U / M1_PWM_DEADTIME);
    g_sClockSetup.ui16SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 1kHz */
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
    PWM0->SM[3].CTRL = ((PWM0->SM[3].CTRL & ~PWM_CTRL_PRSC_MASK) | (PWM_CTRL_PRSC(0x1)));

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
    PWM0->SM[3].VAL4 = PWM_VAL4_VAL4((uint16_t)(-(g_sClockSetup.ui16PwmModulo / 2) + 13));

    PWM0->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWM0->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWM0->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWM0->SM[3].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* Invert polarity for top transistors (PWMA A outputs) */
    PWM0->SM[0].OCTRL |= PWM_OCTRL_POLA_MASK;
    PWM0->SM[1].OCTRL |= PWM_OCTRL_POLA_MASK;
    PWM0->SM[2].OCTRL |= PWM_OCTRL_POLA_MASK;

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
    PWM0->SM[0].DISMAP[0] = ((PWM0->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWM0->SM[1].DISMAP[0] = ((PWM0->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWM0->SM[2].DISMAP[0] = ((PWM0->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWM0->SM[0].DISMAP[0] = ((PWM0->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWM0->SM[1].DISMAP[0] = ((PWM0->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWM0->SM[2].DISMAP[0] = ((PWM0->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));

    /* Modules one and two gets clock from module zero */
    PWM0->SM[1].CTRL2 = (PWM0->SM[1].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWM0->SM[2].CTRL2 = (PWM0->SM[2].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);

    /* Master reload active for modules one and two*/
    PWM0->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWM0->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWM0->SM[3].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master reload is generated every n-th opportunity */
    PWM0->SM[0].CTRL = (PWM0->SM[0].CTRL & ~PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(M1_FOC_FREQ_VS_PWM_FREQ - 1);

    /* Master sync active for modules one and two*/
    PWM0->SM[1].CTRL2 = (PWM0->SM[1].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWM0->SM[2].CTRL2 = (PWM0->SM[2].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

    /* Master reload active for module three*/
    PWM0->SM[3].CTRL2 = (PWM0->SM[3].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x1);

    /* Fault 0 active in logic level one, automatic clearing */
    PWM0->FCTRL = (PWM0->FCTRL & ~PWM_FCTRL_FLVL_MASK) | PWM_FCTRL_FLVL(0x1);
    PWM0->FCTRL = (PWM0->FCTRL & ~PWM_FCTRL_FAUTO_MASK) | PWM_FCTRL_FAUTO(0x1);

    /* Clear fault flags */
    PWM0->FSTS = (PWM0->FSTS & ~PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);

    /* PWMs are re-enabled at PWM full cycle */
    PWM0->FSTS = (PWM0->FSTS & ~PWM_FSTS_FFULL_MASK) | PWM_FSTS_FFULL(0x1);

    /* PWM fault filter - 5 Fast peripheral clocks sample rate, 5 agreeing
       samples to activate */
    PWM0->FFILT = (PWM0->FFILT & ~PWM_FFILT_FILT_PER_MASK) | PWM_FFILT_FILT_PER(5);
    PWM0->FFILT = (PWM0->FFILT & ~PWM_FFILT_FILT_CNT_MASK) | PWM_FFILT_FILT_CNT(5);

    /* Enable A&B PWM outputs for submodules one, two and three */
    PWM0->OUTEN = (PWM0->OUTEN & ~PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(7);
    PWM0->OUTEN = (PWM0->OUTEN & ~PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(7);

    /* PWMA3_TRG0 to ADC0 - XBARA_IN26 -> XBAR_OUT12*/
    /* PWMA3_TRG0 to ADC1 - XBARA_IN26 -> XBAR_OUT42*/
    XBARA->SEL6 = XBARA_SEL6_SEL12(26);
    XBARA->SEL21 = XBARA_SEL21_SEL42(26);
    
    /* Start PWMs (set load OK flags and run) */
    PWM0->MCTRL = (PWM0->MCTRL & ~PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0xF);
    PWM0->MCTRL = (PWM0->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0xF);
    PWM0->MCTRL = (PWM0->MCTRL & ~PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0xF);

    /* Initialize MC driver */
    g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWM0;

    g_sM1Pwm3ph.ui16PhASubNum = M1_PWM_PAIR_PHA; /* PWMA phase A sub-module number */
    g_sM1Pwm3ph.ui16PhBSubNum = M1_PWM_PAIR_PHB; /* PWMA phase B sub-module number */
    g_sM1Pwm3ph.ui16PhCSubNum = M1_PWM_PAIR_PHC; /* PWMA phase C sub-module number */

    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
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
        (g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16SpeedLoopFreq) >> (FTM2->SC & FTM_SC_PS_MASK);
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
    /* Enable clock for HSADC0(HSADCA+HSADCB) and HSADC1(HSADCC+HSADCD) modules */
    SIM->SCGC5 |= SIM_SCGC5_HSADC0_MASK;
    SIM->SCGC2 |= SIM_SCGC2_HSADC1_MASK;

    /* Triggered sequential mode */
    HSADC0->CTRL1 = (HSADC0->CTRL1 & ~HSADC_CTRL1_SMODE_MASK) | HSADC_CTRL1_SMODE(0x04);
    HSADC1->CTRL1 = (HSADC1->CTRL1 & ~HSADC_CTRL1_SMODE_MASK) | HSADC_CTRL1_SMODE(0x04);

    /* Enable end-of-scan interrupt */
    HSADC0->CTRL1 |= HSADC_CTRL1_EOSIEA_MASK;

    /* Enable hardware triggering */
    HSADC0->CTRL1 |= HSADC_CTRL1_SYNCA_MASK;
    HSADC1->CTRL1 |= HSADC_CTRL1_SYNCA_MASK;

    /* Start HSADCA and HSADCC */
    HSADC0->CTRL1 &= ~HSADC_CTRL1_STOPA_MASK;
    HSADC1->CTRL1 &= ~HSADC_CTRL1_STOPA_MASK;

    /* Input clock is 25MHz (100MHz Fast Peripheral clock divided by two),
       single ended */
    HSADC0->CTRL2 = (HSADC0->CTRL2 & ~HSADC_CTRL2_DIVA_MASK) | HSADC_CTRL2_DIVA(0x03);
    HSADC1->CTRL2 = (HSADC1->CTRL2 & ~HSADC_CTRL2_DIVA_MASK) | HSADC_CTRL2_DIVA(0x03);

    /* Disable HSADCB and HSADCD */
    HSADC0->CTRL2 |= HSADC_CTRL2_STOPB_MASK;
    HSADC1->CTRL2 |= HSADC_CTRL2_STOPB_MASK;

    /* Enable first two samples for HSADCA and HSADCC */
    HSADC0->SDIS = HSADC_SDIS_DS(0xFFFC);
    HSADC1->SDIS = HSADC_SDIS_DS(0xFFFC);

    /* Select PWMA0_trig source (routed using XBARA) as sync signal */
    SIM->SOPT7 = (SIM->SOPT7 & ~SIM_SOPT7_HSADC0AALTTRGEN_MASK) | SIM_SOPT7_HSADC0AALTTRGEN(0x00);
    SIM->SOPT7 = (SIM->SOPT7 & ~SIM_SOPT7_HSADC1AALTTRGEN_MASK) | SIM_SOPT7_HSADC1AALTTRGEN(0x00);
    
    /* Configure HSADC calibration process */
    /* Single-ended input calibration on HSADC0A and HSADC1C */
    HSADC0->CALIB |= HSADC_CALIB_REQSINGA_MASK;
    HSADC1->CALIB |= HSADC_CALIB_REQSINGA_MASK;

    /* Power-up HSADCA and HSADCC */
    HSADC0->PWR &= ~HSADC_PWR_PDA_MASK;
    HSADC1->PWR &= ~HSADC_PWR_PDA_MASK;

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
    /*  channel Udcb = M1_ADC1_UDCB, SAMPLE & RESULT = 1 */
    g_sM1AdcSensor.pui32AdcBaseVDcb = (HSADC_Type *)HSADC0;
    g_sM1AdcSensor.ui16ChanNumVDcb = M1_ADC0_UDCB;
    
    /* Assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssignInit(&g_sM1AdcSensor);

    /* Enable & setup interrupt from HSADCA */
    EnableIRQ(HSADC0_CCA_IRQn);           /* Enable interrupt */
    NVIC_SetPriority(HSADC0_CCA_IRQn, 1); /* Set priority to interrupt */
}
