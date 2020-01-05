/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_hvpke18f.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;

/* structure for current and voltage measurement */
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
    
    /* Init ADC */
    M1_MCDRV_ADC_PERIPH_INIT();

    /* Init PDB */
    M1_MCDRV_PDB_INIT();

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
    g_sClockSetup.ui32SystemClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    g_sClockSetup.ui32BusClock    = CLOCK_GetFreq(kCLOCK_BusClk);
    g_sClockSetup.ui32AsynClock = CLOCK_GetFreq(kCLOCK_ScgSysPllAsyncDiv2Clk);
    g_sClockSetup.ui16M1PwmFreq = M1_PWM_FREQ; /* 10kHz */
                                             /* PWM module calculated as follows:
                                              * PWM_MOD = BUS_CLOCK / PWM_FREQUNCY = 60 MHz / 10 kHz = 6000   */
    g_sClockSetup.ui16M1PwmModulo = g_sClockSetup.ui32SystemClock / g_sClockSetup.ui16M1PwmFreq;
    g_sClockSetup.ui16M1PwmDeadTime = (g_sClockSetup.ui32SystemClock / (1000000000U / M1_PWM_DEADTIME) / 4U);
    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 1kHz */
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

    /* initial value of the FTM counter */
    FTM0->CNTIN = (uint32_t)(-(g_sClockSetup.ui16M1PwmModulo) / 2);

    /* modulo value */
    FTM0->MOD = (uint32_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1);

    /* maximum loading point enable */
    FTM0->SYNC = FTM_SYNC_CNTMAX_MASK;

    /* For center aligned PWM using combine mode:
     * COMBINE = 1 - combine mode set
     * COMP = 1 - complementary PWM set
     * DTEN = 1 - dead-time enabled
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
    FTM0->CONTROLS[0].CnV = (uint32_t)(-g_sClockSetup.ui16M1PwmModulo / 4);
    FTM0->CONTROLS[1].CnV = (uint32_t)(g_sClockSetup.ui16M1PwmModulo / 4);
    FTM0->CONTROLS[2].CnV = (uint32_t)(-g_sClockSetup.ui16M1PwmModulo / 4);
    FTM0->CONTROLS[3].CnV = (uint32_t)(g_sClockSetup.ui16M1PwmModulo / 4);
    FTM0->CONTROLS[4].CnV = (uint32_t)(-g_sClockSetup.ui16M1PwmModulo / 4);
    FTM0->CONTROLS[5].CnV = (uint32_t)(g_sClockSetup.ui16M1PwmModulo / 4);

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

    /* Set LOAD ok register */
    /* enable loading updated values */
    FTM0->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

    /* Initialization Trigger Enable */
    FTM0->EXTTRIG = FTM_EXTTRIG_INITTRIGEN_MASK;

    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    FTM0->SC = FTM_SC_CLKS(1);

    /* Fault 0 polarity setting - for FNB41560 is polarity active low */
    FTM0->FLTPOL |= FTM_FLTPOL_FLT0POL_MASK;

    /* Enable Fault 0 - connected to FNB41560 over current comparator output */
    FTM0->FLTCTRL |= FTM_FLTCTRL_FAULT0EN_MASK;

    /* Enable PWM */
    FTM0->SC |= (FTM_SC_PWMEN0(TRUE) | FTM_SC_PWMEN1(TRUE) | FTM_SC_PWMEN2(TRUE) | FTM_SC_PWMEN3(TRUE) |
                 FTM_SC_PWMEN4(TRUE) | FTM_SC_PWMEN5(TRUE));

    /* ---------------------------------------- */
    /* Initialization FTM 3-phase PWM mc driver */
    g_sM1Pwm3ph.pui32PwmBase = (FTM_Type *)(FTM0); /* FTM0 base address */
    g_sM1Pwm3ph.ui16ChanPhA = M1_PWM_PAIR_PHA; /* PWM phase A top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhB = M1_PWM_PAIR_PHB; /* PWM phase B top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhC = M1_PWM_PAIR_PHC; /* PWM phase C top&bottom channel pair number */

    /* FTM Fault number for over current fault detection */
    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM;
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
    g_sClockSetup.ui16M1SpeedLoopModulo =
        (g_sClockSetup.ui32SystemClock / g_sClockSetup.ui16M1SpeedLoopFreq) >> (FTM2->SC & FTM_SC_PS_MASK);
    FTM2->MOD = g_sClockSetup.ui16M1SpeedLoopModulo;

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

    /* enable clock for ADC0 and ADC1 modules*/
    PCC->CLKCFG[PCC_ADC0_INDEX] = (PCC_CLKCFG_CGC(0));
    PCC->CLKCFG[PCC_ADC1_INDEX] = (PCC_CLKCFG_CGC(0));

    PCC->CLKCFG[PCC_ADC0_INDEX] = (PCC_CLKCFG_CGC(1) | PCC_CLKCFG_PCS(2));
    PCC->CLKCFG[PCC_ADC1_INDEX] = (PCC_CLKCFG_CGC(1) | PCC_CLKCFG_PCS(2));

    /* ------- ADC self calibration procedure start ----- */
    /* Setting the clock to 1 MHz (ADIV - div input clock (4 MHz) by 4 and
     * ADICLK bits) and single-ended 12-bit conversion (MODE bits)
     */
    /* single-ended 12-bit conversion */
    /* set divide ratio to 8 */
    /* input clock select - SIRC/2 clock  */
    ADC0->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(2));

    /* single-ended 12-bit conversion */
    /* set divide ratio to 8 */
    /* input clock select - SIRC/2 clock  */
    ADC1->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(2));

    /* HW averaging enabled, 32 samples averaged, continuous conversion must be enabled */
    /* Enable hardware average */
    /* 32 samples averaged */
    ADC0->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) | ADC_SC3_ADCO_MASK);
    ADC1->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) | ADC_SC3_ADCO_MASK);

    /* starting the calibration of ADC0 */
    ADC0->SC3 |= (ADC_SC3_CAL_MASK);

    /* wait until the calibration complets */
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
    {
    }
    
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

    /* wait until the calibration complets */
    while (!(ADC1->SC1[0] & ADC_SC1_COCO_MASK))
    {
    }
    /* ADC calibration failed, place breakpoint here for debug */
    /* e.g.: if (ADC2->SC3 & ADC_SC3_CALF_MASK) __asm ("nop"); */
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

    /* Write default values to SC3 register */
    ADC0->SC3 = 0;
    ADC1->SC3 = 0;

    /* PLL/2 clock source (168 MHz) / 4 (42 MHz), 12-bit */
    ADC0->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(2));
    ADC1->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(2));

    PCC->CLKCFG[PCC_ADC0_INDEX] = (PCC_CLKCFG_CGC(0));
    PCC->CLKCFG[PCC_ADC1_INDEX] = (PCC_CLKCFG_CGC(0));

    PCC->CLKCFG[PCC_ADC0_INDEX] = (PCC_CLKCFG_CGC(1) | PCC_CLKCFG_PCS(6));
    PCC->CLKCFG[PCC_ADC1_INDEX] = (PCC_CLKCFG_CGC(1) | PCC_CLKCFG_PCS(6));


    /* ADC channels interleved */
    SIM->CHIPCTL |= SIM_CHIPCTL_ADC_INTERLEAVE_EN(3);

    /* H/W trigger, DMA disabled, default reference pin pair */
    ADC0->SC2 = (ADC_SC2_ADTRG_MASK);
    ADC1->SC2 = (ADC_SC2_ADTRG_MASK);

    /**************************************/
    /* motor M1 ADC driver initialization */
    /**************************************/
    /* offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;
    
    /* Phase current measurement */
    /* Sector 1,6 - measured currents Ic & Ib */
    /* ADC0, channel Ic = M1_ADC0_PH_C, , SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec16.pui32AdcBasePhaC = (ADC_Type *)ADC0;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaC = M1_ADC0_PH_C;
    /* ADC1, channel Ib = M1_ADC1_PH_B, , SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec16.pui32AdcBasePhaB = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaB = M1_ADC1_PH_B;

    /* Sector 2,3 - measured currents Ic & Ia*/
    /* ADC0, channel Ic = M1_ADC0_PH_C, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.pui32AdcBasePhaC = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaC = M1_ADC1_PH_C;
    /* ADC1, channel Ia = M1_ADC1_PH_A, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.pui32AdcBasePhaA = (ADC_Type *)ADC0;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaA = M1_ADC0_PH_A;
    
    /* Sector 4,5 - measured currents Ia & Ib */
    /* ADC0, channel Ib = M1_ADC0_PH_B, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec45.pui32AdcBasePhaA = (ADC_Type *)ADC0;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaA = M1_ADC0_PH_A;
    /* ADC1, channel Ia = M1_ADC1_PH_A, SAMPLE & RESULT = 0  */
    g_sM1AdcSensor.sCurrSec45.pui32AdcBasePhaB = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaB = M1_ADC1_PH_B;
    
    /* UDCbus channel measurement */
    /*  channel Udcb = M1_ADC1_UDCB, SAMPLE & RESULT = 1 */
    g_sM1AdcSensor.pui32UdcbAdcBase = (ADC_Type *)ADC1;
    g_sM1AdcSensor.ui16ChanNumVDcb = M1_ADC1_UDCB;
    
    /* AUX channel measurement */
    /*  channel Aux = M1_ADC1_UDCB, SAMPLE & RESULT = 1 */
    g_sM1AdcSensor.pui32AuxAdcBase = (ADC_Type *)ADC0;
    g_sM1AdcSensor.ui16ChanNumAux = M1_ADC0_AUX;
    
    /* Assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssignInit(&g_sM1AdcSensor);

    /* Enable & setup interrupt */
    ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
    EnableIRQ(ADC0_IRQn);
    NVIC_SetPriority(ADC0_IRQn, 1);
}

/*!
* @brief      void InitPDB(void)
*              - Initialization of the PDB for ADC triggering and
*		         synchronization between FTM0 and ADC16
*
* @param      void
*
* @return     none
*/
void InitPDB(void)
{
    /* enable clock for PDB module */
    PCC->CLKCFG[PCC_PDB0_INDEX] = PCC_CLKCFG_CGC(TRUE);
    PCC->CLKCFG[PCC_PDB1_INDEX] = PCC_CLKCFG_CGC(TRUE); /* Sys Clock - 168MHz */
    /* PDB triggers for ADC0 module */
    /* PDB triggers for ADC1 module */
    /* PDB | CHANNEL | PRETRIGGER | VALUE */
    /* Set Delay 0.25us after PWM reload (dead time = 0.5us ) */
    PDB0->CH[0].DLY[0] = 42;
    PDB1->CH[0].DLY[0] = 42;

    /* Configure PDB SC register */
    /* Select trigger */
    /* PDB error interrupt enable */
    /* PDB interrupt enable */
    /* PDB enable */
    /* Mod and ChnDlym values will be updated from buffer by writing LDOK bit */
    PDB0->SC = (PDB_SC_PDBEN_MASK | PDB_SC_LDMOD(2) | PDB_SC_TRGSEL(0) | PDB_SC_PDBEIE_MASK | PDB_SC_PDBIE_MASK);
    PDB1->SC = (PDB_SC_PDBEN_MASK | PDB_SC_LDMOD(2) | PDB_SC_TRGSEL(0) | PDB_SC_PDBEIE_MASK);

    /* PDB interrupt delay, PDB is clocked from Bus clock(3x slower) */
    PDB0->IDLY = (g_sClockSetup.ui16M1PwmModulo * M1_FOC_FREQ_VS_PWM_FREQ - g_sClockSetup.ui16M1PwmModulo / 2);

    /* LDOK needed for update of PDBn->MOD and PDBn_CH0DLYm values */
    PDB0->SC |= PDB_SC_LDOK_MASK;
    PDB1->SC |= PDB_SC_LDOK_MASK;

    /* Enable channel 0, pretrigger 0 - ADC0_RA */
    /* Enable channel 0, pretrigger 1 - ADC0_RB */
    /* PDB Channel Back-to-back Enable */
    /* PDB Channel Pre-Trigger Enable */
    /* PDB Channel Pre-Trigger Output Select */
    PDB0->CH[0].C1 = (PDB_C1_EN(3) | PDB_C1_TOS(3) | PDB_C1_BB(2));

    /* Enable channel 1, pretrigger 0 - ADC2_RA */
    /* Enable channel 1, pretrigger 1 - ADC2_RB */
    /* PDB Channel Back-to-back Enable */
    /* PDB Channel Pre-Trigger Enable */
    /* PDB Channel Pre-Trigger Output Select */
    PDB1->CH[0].C1 = (PDB_C1_EN(3) | PDB_C1_TOS(3) | PDB_C1_BB(2));

    /* Enable & setup interrupts */
    EnableIRQ(PDB0_IRQn);
    EnableIRQ(PDB1_IRQn);
    NVIC_SetPriority(PDB0_IRQn, 2);
    NVIC_SetPriority(PDB1_IRQn, 2);
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
    GPIOE->PSOR = GPIO_PSOR_PTSO(1 << 2);
}
