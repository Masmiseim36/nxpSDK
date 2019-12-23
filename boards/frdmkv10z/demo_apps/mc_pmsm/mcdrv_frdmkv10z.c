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

#include "mcdrv_frdmkv10z.h"
#include "fsl_common.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* configuration structure for 3-phase PWM driver */
mcdrv_ftm_pwm3ph_init_t g_sM1Pwm3phInit;

/* configuration structure for ADC driver - phase currents, DC-bus voltage, ... */
mcdrv_adc16_init_t g_sM1AdcInit;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* configuration structure for 3-phase PWM mc driver */
mcdrv_ftm_pwm3ph_t g_sM1Pwm3ph;

/* structure for current and voltage measurement*/
mcdrv_adc16_t g_sM1AdcSensor;

/* structure containing clock frequencies and modules */
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
    /* Init ADC */
    M1_MCDRV_ADC_PERIPH_INIT();

    /* init PDB */
    M1_MCDRV_PDB_INIT();

    /* init FTM2 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();

    /* 6-channel PWM peripheral init  */
    M1_MCDRV_PWM_PERIPH_INIT();

    /* Comparator CMP1 */
    M1_MCDRV_CMP_INIT();
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
    /* Core clock = 75 MHz */
    BOARD_BootClockRUN();

    g_sClockSetup.ui32CoreSystemClock = SystemCoreClock;
    g_sClockSetup.ui32BusClock =
        SystemCoreClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT) + 1);
    g_sClockSetup.ui16PwmFreq = M1_PWM_FREQ; /* 10kHz */
                                             /* PWM module calculated as follows:
                                             * PWM_MOD = BUS_CLOCK / PWM_FREQUNCY = 60 MHz / 10 kHz = 6000   */
    g_sClockSetup.ui16PwmModulo = g_sClockSetup.ui32BusClock / g_sClockSetup.ui16PwmFreq;
    g_sClockSetup.ui16SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 1kHz */
    g_sClockSetup.ui16SpeedLoopModulo =
        g_sClockSetup.ui32BusClock / 16 / g_sClockSetup.ui16SpeedLoopFreq; /* 60000/16/1000 */
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
    FTM0->CNTIN = (uint32_t)(-(g_sClockSetup.ui16PwmModulo) / 2);

    /* modulo value */
    FTM0->MOD = (uint32_t)((g_sClockSetup.ui16PwmModulo / 2) - 1);

    /* maximum loading point enable */
    FTM0->SYNC = FTM_SYNC_CNTMAX_MASK;

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

    /* Dead time 0.5us */
    /* 75MHz, 1/75MHz * 38 = 0.5us */
    FTM0->DEADTIME = FTM_DEADTIME_DTVAL(38);

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

    /* Set LOAD ok register */
    /* enable loading updated values */
    FTM0->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

    /* Initialization Trigger Enable */
    FTM0->EXTTRIG = FTM_EXTTRIG_INITTRIGEN_MASK;

    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    FTM0->SC = FTM_SC_CLKS(1);

    /* Fault 1 taken from CMP1 output */
    SIM->SOPT4 |= SIM_SOPT4_FTM0FLT1_MASK;

    /* Enable Fault 1 - over current comparator CMP1 output */
    FTM0->FLTCTRL |= FTM_FLTCTRL_FAULT1EN_MASK;

    /* ---------------------------------------- */
    /* Initialization FTM 3-phase PWM mc driver */
    g_sM1Pwm3phInit.pui32PwmBase = (FTM_Type *)(FTM0);    /* FTM0 base address */
    g_sM1Pwm3phInit.ui16ChanPairNumPhA = M1_PWM_PAIR_PHA; /* PWM phase A top&bottom channel pair number */
    g_sM1Pwm3phInit.ui16ChanPairNumPhB = M1_PWM_PAIR_PHB; /* PWM phase B top&bottom channel pair number */
    g_sM1Pwm3phInit.ui16ChanPairNumPhC = M1_PWM_PAIR_PHC; /* PWM phase C top&bottom channel pair number */

    /* FTM fault */
    g_sM1Pwm3phInit.ui16FaultFixNum = M1_FAULT_NUM; /* FTM Fault number for over current fault detection */

    /* Pass initialization structure to the mc driver */
    MCDRV_FtmPwm3PhInit(&g_sM1Pwm3ph, &g_sM1Pwm3phInit); /* MC driver initialization */
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

    /* 1kHz Slow Loop */
    FTM2->MOD = g_sClockSetup.ui16SpeedLoopModulo;

    /* LOADOK */
    FTM2->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Settings up FTM SC register for clock setup */
    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    /* Set prescaler to 16 */
    /* Enable interrupt */
    FTM2->SC = FTM_SC_PS(4) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

    /* Enable & setup interrupts */
    EnableIRQ(FTM2_IRQn);           /* Enable Interrupt */
    NVIC_SetPriority(FTM2_IRQn, 3); /* Set priority to interrupt */
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
    static uint16_t ui16AdcArray[10] = {M1_ADC0_PH_A, M1_ADC1_PH_A, /* Phase current A */
                                        M1_ADC0_PH_B, M1_ADC1_PH_B, /* Phase current B */
                                        M1_ADC0_PH_C, M1_ADC1_PH_C, /* Phase current C */
                                        M1_ADC0_UDCB, M1_ADC1_UDCB, /* Udcb voltage */
                                        M1_ADC0_AUX,  M1_ADC1_AUX}; /* Auxiliary channel */

    /* enable clock for ADC0 and ADC1 modules*/
    SIM->SCGC6 |= (SIM_SCGC6_ADC0_MASK | SIM_SCGC6_ADC1_MASK);

    /* ------- ADC self calibration procedure start ----- *
     * setting the clock to 3.125 MHz (ADIV - div bus clock (25 MHz) by 4 and
     * ADICLK bits) and single-ended 12-bit conversion (MODE bits)
     */
    /* single-ended 12-bit conversion */
    /* set divide ratio to 4 */
    /* input clock select - Alternate clock (OSCERCLK)  */
    ADC0->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(2) | ADC_CFG1_ADICLK(1));
    ADC1->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(2) | ADC_CFG1_ADICLK(1));

    /* HW averaging enabled, 32 samples averaged, continuous conversion must be enabled */
    /* Enable hardware average */
    /* 32 samples averaged */
    /* Continuous Conversion Enable */
    ADC0->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) | ADC_SC3_ADCO_MASK);
    ADC1->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) | ADC_SC3_ADCO_MASK);

    /* starting the calibration of ADC0 */
    ADC0->SC3 |= (ADC_SC3_CAL_MASK);

    /* wait until the calibration complete */
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
    ADC0->PG = ui16Calib;           /* set the plus side gain register */

    ui16Calib = ADC0->CLM0;
    ui16Calib += ADC0->CLM1;
    ui16Calib += ADC0->CLM2;
    ui16Calib += ADC0->CLM3;
    ui16Calib += ADC0->CLM4;
    ui16Calib += ADC0->CLMS;
    ui16Calib = ui16Calib >> 1;
    ui16Calib = ui16Calib | 0x8000; /* set the MSB */
    ADC0->MG = ui16Calib;           /* set the minus side gain register */

    /* starting the calibration of ADC1 */
    ADC1->SC3 |= (ADC_SC3_CAL_MASK);

    /* wait until the calibration complete */
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
    ADC1->PG = ui16Calib;           /* set the plus side gain register */

    ui16Calib = ADC1->CLM0;
    ui16Calib += ADC1->CLM1;
    ui16Calib += ADC1->CLM2;
    ui16Calib += ADC1->CLM3;
    ui16Calib += ADC1->CLM4;
    ui16Calib += ADC1->CLMS;
    ui16Calib = ui16Calib >> 1;
    ui16Calib = ui16Calib | 0x8000; /* set the MSB */
    ADC1->MG = ui16Calib;           /* set the minus side gain register */
    /* ------- ADC self calibration procedure end ----- */

    /* Write default values to SC3 register */
    ADC0->SC3 = 0;
    ADC1->SC3 = 0;

    /* setting the clock to 12.5 MHz (ADIV - div bus clock (25 MHz) by 2 and */
    ADC0->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(1));
    ADC1->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(1));

    /* Set hardware trigger */
    ADC0->SC2 = (ADC_SC2_ADTRG_MASK);
    ADC1->SC2 = (ADC_SC2_ADTRG_MASK);

    /* ---------------------------------------------- */
    /* pass initialization structure to ADC MC driver */
    g_sM1AdcInit.ui16AdcArray = (&ui16AdcArray[0]);
    g_sM1AdcInit.pui32Adc0Base = (ADC_Type *)ADC0;
    g_sM1AdcInit.pui32Adc1Base = (ADC_Type *)ADC1;
    MCDRV_Adc16Init(&g_sM1AdcSensor, &g_sM1AdcInit);

    /* assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssign(&g_sM1AdcSensor);

    /* Enable & setup interrupt */
    ADC1->SC1[0] |= ADC_SC1_AIEN_MASK;
    EnableIRQ(ADC1_IRQn);
    NVIC_SetPriority(ADC1_IRQn, 1);
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

    /* PDB triggers for ADC0 module */
    /* PDB triggers for ADC1 module */
    /* PDB | CHANNEL | PRETRIGGER | VALUE */
    /* Set Delay 0.25us after PWM reload (dead time = 0.5us ) */
    PDB0->CH[0].DLY[0] = 15;
    PDB0->CH[1].DLY[0] = 15;

    /* Configure PDB SC register */
    /* Select trigger */
    /* PDB error interrupt enable */
    /* PDB interrupt enable */
    /* PDB enable */
    /* Mod and ChnDlym values will be updated from buffer by writing LDOK bit */
    PDB0->SC = (PDB_SC_PDBEN_MASK | PDB_SC_LDMOD(2) | PDB_SC_TRGSEL(8) | PDB_SC_PDBEIE_MASK | PDB_SC_PDBIE_MASK);

    /* PDB interrupt delay, PDB is clocked from Bus clock(3x slower) */
    PDB0->IDLY = (g_sClockSetup.ui16PwmModulo * M1_FOC_FREQ_VS_PWM_FREQ - g_sClockSetup.ui16PwmModulo / 2) / 3;

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

    /* Enable & setup interrupts */
    EnableIRQ(PDB0_IRQn);           /* Enable Interrupt */
    NVIC_SetPriority(PDB0_IRQn, 2); /* Set priority to interrupt */
}
/*!
@brief   void InitCMP1(void)
          - Initialization of the comparator 1 module for dc-bus over current
            detection to generate FTM0 fault

@param   void

@return  none
*/
void InitCMP1(void)
{
    /* Enable clock for CMP module */
    SIM->SCGC4 |= SIM_SCGC4_CMP_MASK;

    CMP1->CR0 = CMP_CR0_FILTER_CNT(4); /* Filter - 4 consecutive samples must agree */

    /* DAC output set to 3.197V ~ 7.73A (for 8.25A scale) */
    /* Reference voltage will be VDD */
    /* Enable DAC */
    CMP1->DACCR = CMP_DACCR_VOSEL(62) | CMP_DACCR_VRSEL_MASK | CMP_DACCR_DACEN_MASK;

    /* Plus is CMP1_IN5 ~ overcurrent pin */
    /* Minus is CMP1_IN7 ~ 6bit reference */
    CMP1->MUXCR = CMP_MUXCR_PSEL(4) | CMP_MUXCR_MSEL(7);

    /* Enable analog comparator */
    CMP1->CR1 = CMP_CR1_EN_MASK;
}
