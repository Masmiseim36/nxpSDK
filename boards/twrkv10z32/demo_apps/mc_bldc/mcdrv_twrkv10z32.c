/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
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

#include "mcdrv_twrkv10z32.h"
#include "fsl_common.h"
#include "clock_config.h"

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

/* Configuration structure for 3-phase PWM driver */
mcdrv_ftm_pwm3ph_init_t g_sM1Pwm3phInit;

/* configuration structure for 3-phase MOSFET predriver */
mcdrv_spi_drv3ph_init_t g_sM1Driver3phInit;

/* Configuration structure for ADC driver - phase currents,
   DC-bus voltage, aux */
mcdrv_adc16_init_t g_sM1Adc16Init;

/* Configuration structure for time event driver */
mcdrv_ftm_cmt_init_t g_sM1CmtTmrInit;

/* configuration structure for 3-phase MOSFET predriver */
mcdrv_spi_drv3ph_t g_sM1Driver3ph;

/* Structure for 3-phase PWM mc driver */
mcdrv_ftm_pwm3ph_t g_sM1Pwm3ph;

/* Structure for time event driver */
mcdrv_ftm_cmt_t g_sM1CmtTmr;

/* Structure for current and voltage measurement */
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
    /* SPI peripheral init for 3-phase MOSFET predriver configuration */
    M1_MCDRV_DRV3PH_INIT();

    /* init ADC */
    M1_MCDRV_ADC_PERIPH_INIT();

    /* init PDB */
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
    /* Core Clock: 75MHz */
    BOARD_BootClockRUN();

    /* Calculate clock dependant variables for BLDC sensorless control algorithm */
    g_sClockSetup.ui32CoreSystemClock = SystemCoreClock;
    g_sClockSetup.ui32BusFlashClock =
        SystemCoreClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT) + 1);
    g_sClockSetup.ui16PwmFreq = PWM_FREQ; /* 20 kHz */
                                          /* PWM module calculated as follows:
                                          * PWM_MOD = CPU_CLOCK / PWM_FREQUNCY = 75 MHz / 20 kHz = 3750   */
    g_sClockSetup.ui16PwmModulo = SystemCoreClock / g_sClockSetup.ui16PwmFreq;
    g_sClockSetup.ui32CmtTimerFreq = SystemCoreClock / 128;
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
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;

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
    FTM0->SYNC = FTM_SYNC_CNTMAX_MASK | FTM_SYNC_SYNCHOM_MASK;

    /* For center aligned PWM using combine mode:
     * COMBINE = 1 - combine mode set
     * COMP = 1 - complementary PWM set
     * DTEN = 1 - deadtime enabled
     * SYNCEN = 1 - PWM update synchronization enabled
    */
    /* complementary mode */
    FTM0->COMBINE = FTM_COMBINE_COMBINE0_MASK | FTM_COMBINE_COMP0_MASK | FTM_COMBINE_DTEN0_MASK |
                    FTM_COMBINE_SYNCEN0_MASK | FTM_COMBINE_COMBINE1_MASK | FTM_COMBINE_COMP1_MASK |
                    FTM_COMBINE_DTEN1_MASK | FTM_COMBINE_SYNCEN1_MASK | FTM_COMBINE_COMBINE2_MASK |
                    FTM_COMBINE_COMP2_MASK | FTM_COMBINE_DTEN2_MASK | FTM_COMBINE_SYNCEN2_MASK;

    /* polarity setting, 3ppa driver high sides are active low */
    FTM0->POL = FTM_POL_POL0_MASK | FTM_POL_POL2_MASK | FTM_POL_POL4_MASK;

    /* DTPS - deadtime prescaler: 0,1 = 1; 2 = 4; 3 = 16 */
    /* DTVAL - deadtime value (0-63): deadtime period = DTPS x DTVAL */
    FTM0->DEADTIME = FTM_DEADTIME_DTVAL(63);

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

    /* Initialization FTM 3-phase PWM mc driver */
    g_sM1Pwm3phInit.pui32PwmBase = (FTM_Type *)(FTM0);    /* FTM0 base address */
    g_sM1Pwm3phInit.ui16ChanPairNumPhA = M1_PWM_PAIR_PHA; /* PWM phase A top&bottom channel pair number */
    g_sM1Pwm3phInit.ui16ChanPairNumPhB = M1_PWM_PAIR_PHB; /* PWM phase B top&bottom channel pair number */
    g_sM1Pwm3phInit.ui16ChanPairNumPhC = M1_PWM_PAIR_PHC; /* PWM phase C top&bottom channel pair number */

    /* initialization of PWM modulo */
    g_sM1Pwm3phInit.ui16PwmModulo = g_sClockSetup.ui16PwmModulo;

    /* initialization of BLDC commucation table */
    g_sM1Pwm3phInit.pcBldcTable = &bldcCommutationTableComp[0];

    /* Pass initialization structure to the mc driver */
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
    /* enable clock to FTM1 module */
    SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;

    /* configuration of FTM1 module */
    /* write protection is disabled */
    /* free running counter and synchronization are different from TPM
       behaviour */
    FTM1->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Free running timer */
    /* Modulo value */
    FTM1->MOD = 0xffff;

    /* bus clock, divide by 128, 1.706us @ 75 MHz clock */
    /* bus clock as source clock for FTM */
    /* Prescale factor 128 */
    FTM1->SC = FTM_SC_PS(7) | FTM_SC_CLKS(1);

    /* Enable Output Compare interrupt, output Compare, Software Output
     * Compare only (ELSnB:ELSnA = 0:0, output pin is not controlled by FTM) */
    FTM1->CONTROLS[0].CnSC = FTM_CnSC_MSA_MASK | FTM_CnSC_CHIE_MASK;

    /* Enable interrupt */
    EnableIRQ(FTM1_IRQn);

    /* Set priority to interrupt */
    NVIC_SetPriority(FTM1_IRQn, ISR_PRIORITY_FORCED_CMT);

    /* Initialization FTM time event driver */
    g_sM1CmtTmrInit.pui32FtmBase = (FTM_Type *)(FTM1); /* FTM1 base address */
    g_sM1CmtTmrInit.ui16ChannelNum = M1_FTM_CMT_CHAN;  /* FTM1 compare channel selection */

    /* Pass initialization structure to the MC driver */
    MCDRV_FtmCmtInit(&g_sM1CmtTmr, &g_sM1CmtTmrInit); /* MC driver initialization */
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
    /* Set prescaler to 16 */
    /* Enable interrupt */
    FTM2->SC = FTM_SC_PS(4) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

    /* Enable & setup interrupts */
    EnableIRQ(FTM2_IRQn);                                 /* Enable Interrupt */
    NVIC_SetPriority(FTM2_IRQn, ISR_PRIORITY_SLOW_TIMER); /* Set priority to interrupt */
}
/*!
@brief   void InitSPI(void)
          - Initialization of the SPI peripheral for motor M1 3-phase MOSFET
            predriver
          - SPI configuration for MOSFET predriver MC33937
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

    /* initialization of pins required by MC33937 predriver */
    g_sM1Driver3phInit.pSpiBase = (SPI_Type *)(SPI0); /* SPI Base Address */
    g_sM1Driver3phInit.ui32Pcs = 0;                   /* PCS number */

    /* overcurrent PIN & PORT */
    CLOCK_EnableClock(kCLOCK_PortB);
    g_sM1Driver3phInit.bFaultOcEnabled = TRUE;             /* disable setting of GPIO pin to detect OC fault */
    g_sM1Driver3phInit.pGpioOcBase = (GPIO_Type *)(GPIOB); /* GPIOx Base Address */
    g_sM1Driver3phInit.pPortOcBase = (PORT_Type *)(PORTB); /* PORTx Base Address */
    g_sM1Driver3phInit.ui32GpioOcPin = 2;                  /* pin number for driver enabled */

    /* enable PIN & PORT */
    CLOCK_EnableClock(kCLOCK_PortD);
    g_sM1Driver3phInit.pGpioEnBase = (GPIO_Type *)(GPIOD); /* GPIOx Base Address */
    g_sM1Driver3phInit.pPortEnBase = (PORT_Type *)(PORTD); /* PORTx Base Address */
    g_sM1Driver3phInit.ui32GpioEnPin = 7;                  /* pin number for driver enabled */

    /* interrupt PIN & PORT */
    g_sM1Driver3phInit.pGpioIntBase = (GPIO_Type *)(GPIOB); /* GPIOx Base Address */
    g_sM1Driver3phInit.pPortIntBase = (PORT_Type *)(PORTB); /* PORTx Base Address */
    g_sM1Driver3phInit.ui32GpioIntPin = 3;                  /* pin number for interrupt detection */

    /* reset PIN & PORT */
    CLOCK_EnableClock(kCLOCK_PortC);
    g_sM1Driver3phInit.pGpioResetBase = (GPIO_Type *)(GPIOC); /* GPIOx Base Address */
    g_sM1Driver3phInit.pPortResetBase = (PORT_Type *)(PORTC); /* PORTx Base Address */
    g_sM1Driver3phInit.ui32GpioResetPin = 2;                  /* pin number for reset, driver */

    /* pass initialization structure to the mc driver */
    MCDRV_Driver3PhInit(&g_sM1Driver3ph, &g_sM1Driver3phInit); /* MC driver initialization */

    /* zero deadtime set in MC33937, dead time control from FTM */
    g_sM1Driver3ph.ui16Deadtime = 0;
    MCDRV_Driver3PhConfig(&g_sM1Driver3ph);
    MCDRV_Driver3PhGetSr0(&g_sM1Driver3ph);
    MCDRV_Driver3PhGetSr1(&g_sM1Driver3ph);
    MCDRV_Driver3PhGetSr2(&g_sM1Driver3ph);
    MCDRV_Driver3PhGetSr3(&g_sM1Driver3ph);
    MCDRV_Driver3PhClearFlags(&g_sM1Driver3ph);
    MCDRV_Driver3PhSetEn(&g_sM1Driver3ph);
}

/******************************************************************************
@brief      Initialization of the ADC16 peripheral

@param      void

@return     none

@details    Initialization of the A/D converter for current and voltage sensing
*******************************************************************************/
void InitADC16(void)
{
    uint16_t ui16Calib;

    /* ADC channel number assignment array to be passeed to MC ADC driver */
    static uint16_t ui16AdcArray[12] = {M1_ADC0_PH_A, M1_ADC1_PH_A, M1_ADC0_PH_B, M1_ADC1_PH_B,
                                        M1_ADC0_PH_C, M1_ADC1_PH_C, M1_ADC0_UDCB, M1_ADC1_UDCB,
                                        M1_ADC0_IDCB, M1_ADC1_IDCB, M1_ADC0_AUX,  M1_ADC1_AUX};

    /* enable clock for ADC0 and ADC1 modules*/
    SIM->SCGC6 |= (SIM_SCGC6_ADC0_MASK | SIM_SCGC6_ADC1_MASK);

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

    /* HW averaging enabled, 32 samples averaged, continuous conversion must be
       enabled */
    ADC0->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) | ADC_SC3_ADCO_MASK);

    ADC1->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) | ADC_SC3_ADCO_MASK);

    /* starting the calibration of ADC0 */
    ADC0->SC3 |= (ADC_SC3_CAL_MASK);

    /* wait until the calibration complets */
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
    ui16Calib = ui16Calib | 0x8000; /* set the MSB */
    /* set the plus side gain register */
    ADC0->PG = ui16Calib;

    ui16Calib = ADC0->CLM0;
    ui16Calib += ADC0->CLM1;
    ui16Calib += ADC0->CLM2;
    ui16Calib += ADC0->CLM3;
    ui16Calib += ADC0->CLM4;
    ui16Calib += ADC0->CLMS;
    ui16Calib = ui16Calib >> 1;
    ui16Calib = ui16Calib | 0x8000; /* set the MSB */
    /* set the minus side gain register */
    ADC0->MG = ui16Calib;

    /* starting the calibration of ADC1 */
    ADC1->SC3 |= (ADC_SC3_CAL_MASK);

    /* wait until the calibration complets */
    while (!(ADC1->SC1[0] & ADC_SC1_COCO_MASK))
    {
    }
    /* adc calibration failed, place breakpoint here for debug */
    if (ADC1->SC3 & ADC_SC3_CALF_MASK)
        __asm("nop");

    ui16Calib = ADC1->CLP0;
    ui16Calib += ADC1->CLP1;
    ui16Calib += ADC1->CLP2;
    ui16Calib += ADC1->CLP3;
    ui16Calib += ADC1->CLP4;
    ui16Calib += ADC1->CLPS;
    ui16Calib = ui16Calib >> 1;
    ui16Calib = ui16Calib | 0x8000; /* set the MSB */
    /* set the plus side gain register */
    ADC1->PG = ui16Calib;

    ui16Calib = ADC1->CLM0;
    ui16Calib += ADC1->CLM1;
    ui16Calib += ADC1->CLM2;
    ui16Calib += ADC1->CLM3;
    ui16Calib += ADC1->CLM4;
    ui16Calib += ADC1->CLMS;
    ui16Calib = ui16Calib >> 1;
    ui16Calib = ui16Calib | 0x8000; /* set the MSB */
    /* set the minus side gain register */
    ADC1->MG = ui16Calib;
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

    /* prepare first meaurement */
    /* pass initialization structure to ADC MC driver */
    g_sM1Adc16Init.ui16AdcArray = (&ui16AdcArray[0]);
    g_sM1Adc16Init.pui32Adc0Base = (ADC_Type *)ADC0;
    g_sM1Adc16Init.pui32Adc1Base = (ADC_Type *)ADC1;
    MCDRV_Adc16Init(&g_sM1AdcSensor, &g_sM1Adc16Init);

    /* enable & setup interrupt */
    ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
    EnableIRQ(ADC0_IRQn);
    NVIC_SetPriority(ADC0_IRQn, ISR_PRIORITY_ADC0);
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
    SIM->SCGC6 |= SIM_SCGC6_PDB_MASK;

    /* CH0&1: enable pre-trigger 0&1, back2back on pre-tritgger 1 */
    /* enable channel 0, pretrigger 0 - ADC0_RA */
    /* enable channel 0, pretrigger 1 - ADC0_RB */
    PDB0->CH[0].C1 = (PDB_C1_EN(3) | PDB_C1_TOS(3) | PDB_C1_BB(2));

    /* Enable channel 1, pretrigger 0 - ADC1_RA */
    /* Enable channel 1, pretrigger 1 - ADC1_RB */
    /* PDB Channel Back-to-back Enable */
    /* PDB Channel Pre-Trigger Enable */
    /* PDB Channel Pre-Trigger Output Select */
    PDB0->CH[1].C1 = (PDB_C1_EN(3) | PDB_C1_TOS(3) | PDB_C1_BB(2));

    /* CH0&1: pre-trigger 0 delay=middle of pwm period*/
    PDB0->CH[0].DLY[0] = (g_sClockSetup.ui16PwmModulo / 6); /* Set delay */
    PDB0->CH[1].DLY[0] = (g_sClockSetup.ui16PwmModulo / 6); /* Set delay */

    /* set counters modulo */
    PDB0->MOD = 0x7fff;

    /* LDOK immediately updates internal registers, enable PDB0 */
    /* internal registers are update upon trigger and LDOK, enable interrupt,
       no prescaler, trig. src 8 (FTM0-EXTTRIG), enable PDB, multiplication
       factor is 1, set LDOK */

    PDB0->SC = (PDB_SC_PDBEN_MASK | PDB_SC_LDMOD(2) | PDB_SC_TRGSEL(8) | PDB_SC_PDBEIE_MASK | PDB_SC_LDOK_MASK);

    /* Enable & setup interrupts */
    EnableIRQ(PDB0_IRQn);
    NVIC_SetPriority(PDB0_IRQn, ISR_PRIORITY_PDB0);
}
