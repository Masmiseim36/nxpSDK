/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"
#include "peripherals.h"
#include "fsl_common.h"

/*******************************************************************************
 * Functions
 ******************************************************************************/
static void InitADC(void);
static void InitPDB(void);
static void InitSlowLoop(void);
static void InitPWM(void);
#if M1_FAULT_CMP_ENABLE
static void InitCMP(void);
#endif /* M1_FAULT_CMP_ENABLE */
#if M1_INRUSH_ENABLE
static void InitRelay(void);
#endif /* M1_INRUSH_ENABLE */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;

/* structure for current and voltage measurement*/
mcdrv_adc16_t g_sM1AdcSensor;

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
#if M1_INRUSH_ENABLE
    InitRelay();
#endif /* M1_INRUSH_ENABLE */
    /* Init ADC */
    InitADC();

    /* Init PDB */
    InitPDB();

    /* Init slow loop counter*/
    InitSlowLoop();

    /* 6-channel PWM peripheral init */
    InitPWM();
#if M1_FAULT_CMP_ENABLE
    /* Comparator CMP1 */
    InitCMP();
#endif /* M1_FAULT_CMP_ENABLE */
}

/*!
 * @brief   void InitPWM(void)
 *           - Initialization of the FTM0 peripheral for motor M1
 *           - 3-phase center-aligned PWM
 *
 * @param   void
 *
 * @return  none
 */
static void InitPWM(void)
{
#if defined(M1_PWM_TIMER_FTM0)
    /* enable the clock for FTM0 */
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
#elif defined(M1_PWM_TIMER_FTM3)
    /* enable the clock for FTM3 */
    SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;
#endif

    /* Disable all channels outputs using the OUTPUT MASK feature.
     * However, the output pins are still driven as GPIO since the
     * channel mode is set to FTM channel disabled after RESET */
    /* channel output is masked */
    M1_PWM_TIMER->OUTMASK = (FTM_OUTMASK_CH0OM_MASK | FTM_OUTMASK_CH1OM_MASK | FTM_OUTMASK_CH2OM_MASK | FTM_OUTMASK_CH3OM_MASK |
                     FTM_OUTMASK_CH4OM_MASK | FTM_OUTMASK_CH5OM_MASK);

    /* disable write protection for certain registers
     * free running counter and synchronization are different from TPM
     * Enable the counter
     * Fault Control Mode - Fault control is enabled for all channels,
     * and the selected mode is the automatic fault clearing
     * Initialize The Channels Output */
    M1_PWM_TIMER->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK | FTM_MODE_FAULTM(3) | FTM_MODE_INIT_MASK;

    /* Counter running in BDM mode */
    M1_PWM_TIMER->CONF = FTM_CONF_BDMMODE(3);

    /* initial value of the FTM counter */
    M1_PWM_TIMER->CNTIN = (uint32_t)(-M1_PWM_MODULO / 2);

    /* modulo value */
    M1_PWM_TIMER->MOD = (uint32_t)((M1_PWM_MODULO / 2) - 1);

    /* maximum loading point enable */
    M1_PWM_TIMER->SYNC = FTM_SYNC_CNTMAX_MASK;

    /* For center aligned PWM using combine mode:
     * COMBINE = 1 - combine mode set
     * COMP = 1 - complementary PWM set
     * SYNCEN = 1 - PWM update synchronization enabled
     */
    /* complementary mode */
    M1_PWM_TIMER->COMBINE = FTM_COMBINE_COMBINE0_MASK | FTM_COMBINE_COMP0_MASK | FTM_COMBINE_SYNCEN0_MASK |
                            FTM_COMBINE_COMBINE1_MASK | FTM_COMBINE_COMP1_MASK | FTM_COMBINE_SYNCEN1_MASK |
                            FTM_COMBINE_COMBINE2_MASK | FTM_COMBINE_COMP2_MASK | FTM_COMBINE_SYNCEN2_MASK;

#if M1_PWM_DEADTIME_ENABLE
    /* Enable deatime */
    M1_PWM_TIMER->COMBINE |= FTM_COMBINE_DTEN0_MASK | FTM_COMBINE_DTEN1_MASK | FTM_COMBINE_DTEN2_MASK;

    /* Dead time length */
    M1_PWM_TIMER->DEADTIME = FTM_DEADTIME_DTPS(M1_PWM_DEADTIME_LENGTH_DTPS) | FTM_DEADTIME_DTVAL(M1_PWM_DEADTIME_LENGTH_DTVAL);
#endif /* M1_PWM_DEADTIME_ENABLE */

#if M1_FAULT_ENABLE
    /* Enable deatime */
    M1_PWM_TIMER->COMBINE |= FTM_COMBINE_FAULTEN0_MASK | FTM_COMBINE_FAULTEN1_MASK | FTM_COMBINE_FAULTEN2_MASK;

    #if M1_FAULT_NUM == 0U
    /* Enable Fault 0 */
    M1_PWM_TIMER->FLTCTRL |= FTM_FLTCTRL_FAULT0EN_MASK;
    /* Polarity Fault 0 */
    M1_PWM_TIMER->FLTPOL |= FTM_FLTPOL_FLT0POL(M1_FAULT_POL);

#if M1_FAULT_CMP_ENABLE == 1U
    /* Fault 1 taken from CMP1 output */
    SIM->SOPT4 |= SIM_SOPT4_FTM0FLT0_MASK;

#endif /* M1_FAULT_CMP_ENABLE */
#endif /* M1_FAULT_NUM */

#if M1_FAULT_NUM == 1U
    /* Enable Fault 1 */
    M1_PWM_TIMER->FLTCTRL |= FTM_FLTCTRL_FAULT1EN_MASK;
    /* Polarity Fault 1 */
    M1_PWM_TIMER->FLTPOL |= FTM_FLTPOL_FLT1POL(M1_FAULT_POL);

#if M1_FAULT_CMP_ENABLE == 1U
    /* Fault 1 taken from CMP1 output */
    SIM->SOPT4 |= SIM_SOPT4_FTM0FLT1_MASK;

#endif /* M1_FAULT_CMP_ENABLE */
#endif /* M1_FAULT_NUM */
#endif /* M1_FAULT_ENABLE */

#if M1_PWM_POL_TOP == 0U
    /* PWM polarity setting - high side */
    M1_PWM_TIMER->POL |= FTM_POL_POL0_MASK | FTM_POL_POL2_MASK | FTM_POL_POL4_MASK;
#endif

#if M1_PWM_POL_BOTTOM == 0U
    /* PWM polarity setting - low side */
    M1_PWM_TIMER->POL |= FTM_POL_POL1_MASK | FTM_POL_POL3_MASK | FTM_POL_POL5_MASK;
#endif

    /* Initial setting of value registers to 50 % of duty cycle  */
    M1_PWM_TIMER->CONTROLS[0].CnV = (uint32_t)(-M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[1].CnV = (uint32_t)(M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[2].CnV = (uint32_t)(-M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[3].CnV = (uint32_t)(M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[4].CnV = (uint32_t)(-M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[5].CnV = (uint32_t)(M1_PWM_MODULO / 4);

    /* Note:
     * 1. From this moment the output pins are under FTM control. Since the PWM
     *    output is disabled by the FTM0OUTMASK register, there is no change on
     *    PWM outputs. Before the channel mode is set, the correct output pin
     *    polarity has to be defined.
     * 2. Even if the odd channels are generated automatically by complementary
     *    logic, these channels have to be set to be in the same channel mode.
     */
    M1_PWM_TIMER->CONTROLS[0].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[1].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[2].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[3].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[4].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[5].CnSC = FTM_CnSC_ELSB_MASK;

    /* Set LOAD ok register */
    /* enable loading updated values */
    M1_PWM_TIMER->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

    /* Initialization Trigger Enable */
    M1_PWM_TIMER->EXTTRIG = FTM_EXTTRIG_INITTRIGEN_MASK;

    /* Set system clock as source for FTM (CLKS[1:0] = 01) */
    M1_PWM_TIMER->SC = FTM_SC_CLKS(1);

    /* ---------------------------------------- */
    /* Initialization FTM 3-phase PWM mc driver */
    g_sM1Pwm3ph.pui32PwmBase = (FTM_Type *)(M1_PWM_TIMER); /* FTM base address */
    g_sM1Pwm3ph.ui16ChanPhA  = M1_PWM_PAIR_PHA;    /* PWM phase A top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhB  = M1_PWM_PAIR_PHB;    /* PWM phase B top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhC  = M1_PWM_PAIR_PHC;    /* PWM phase C top&bottom channel pair number */

    /* FTM Fault number for over current fault detection */
    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM;
}

/*!
 * @brief   void InitSlowLoop(void)
 *           - Initialization of the FTM peripheral
 *           - performs slow control loop counter
 *
 * @param   void
 *
 * @return  none
 */
static void InitSlowLoop(void)
{
#if defined(M1_SLOW_LOOP_TIMER_FTM0)
    /* enable clock to FTM module */
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
#elif defined(M1_SLOW_LOOP_TIMER_FTM1)
    /* enable clock to FTM module */
    SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
#elif defined(M1_SLOW_LOOP_TIMER_FTM2)
    /* enable clock to FTM module */
    SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;
#elif defined(M1_SLOW_LOOP_TIMER_FTM3)
    /* enable clock to FTM module */
    SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;
#elif defined(M1_SLOW_LOOP_TIMER_FTM4)
    /* enable clock to FTM module */
    SIM->SCGC6 |= SIM_SCGC6_FTM4_MASK;
#elif defined(M1_SLOW_LOOP_TIMER_FTM5)
    /* enable clock to FTM module */
    SIM->SCGC6 |= SIM_SCGC6_FTM5_MASK;
#endif

    /* Configuration of FTM module */
    /* Disable write protection for certain registers */
    /* Enable the counter */
    M1_SLOW_LOOP_TIMER->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Settings up FTM SC register for clock setup */
    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    /* Set prescaler to 16 */
    /* Enable interrupt */
    M1_SLOW_LOOP_TIMER->SC = FTM_SC_PS(4) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

    /* Counter running in BDM mode */
    M1_SLOW_LOOP_TIMER->CONF = FTM_CONF_BDMMODE(3);

    /* Set count value to 0 */
    M1_SLOW_LOOP_TIMER->CNTIN = 0x0;

    /* Slow Loop freq */
    M1_SLOW_LOOP_TIMER->MOD = (M1_SLOW_LOOP_MODULO) >> (M1_SLOW_LOOP_TIMER->SC & FTM_SC_PS_MASK);

    /* LOADOK */
    M1_SLOW_LOOP_TIMER->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Enable & setup interrupts */
#if (!MC_PMSM_DISABLE)
    EnableIRQ(SLOW_LOOP_IRQN);
    NVIC_SetPriority(SLOW_LOOP_IRQN, 3);
#endif
}

/*!
 * @brief   void InitADC(void)
 *           - Initialization of the ADC16 peripheral
 *           - Initialization of the A/D converter for current and voltage sensing
 *
 * @param   void
 *
 * @return  none
 */
static void InitADC(void)
{
    uint16_t ui16Calib;

    /* enable clock for ADC0 and ADC1 modules*/
    SIM->SCGC6 |= (SIM_SCGC6_ADC0_MASK | SIM_SCGC6_ADC1_MASK);

    /* ------- ADC self calibration procedure start ----- */
    /* Divide actual bus clock by 2 for ADC calibration */
    SIM->CLKDIV1 = ((SIM->CLKDIV1 & ~(SIM_CLKDIV1_OUTDIV2_MASK)) | SIM_CLKDIV1_OUTDIV2((((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT) << 1U) + 1U));

    /* single-ended 12-bit conversion */
    /* set divide ratio to 8 */
    /* input clock select - Bus clock */
    ADC0->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(3) | ADC_CFG1_ADICLK(0));
    ADC1->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(3) | ADC_CFG1_ADICLK(0));

    /* HW averaging enabled, 32 samples averaged, continuous conversion is enabled */
    /* Enable hardware average */
    /* 32 samples averaged */
    /* Continuous Conversion Enable */
    ADC0->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) | ADC_SC3_ADCO_MASK);
    ADC1->SC3 = (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) | ADC_SC3_ADCO_MASK);

    /* starting the calibration of ADC0 */
    ADC0->SC3 |= (ADC_SC3_CAL_MASK);

    /* wait until the calibration completes */
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
    ADC0->PG  = ui16Calib;          /* set the plus side gain register */

    ui16Calib = ADC0->CLM0;
    ui16Calib += ADC0->CLM1;
    ui16Calib += ADC0->CLM2;
    ui16Calib += ADC0->CLM3;
    ui16Calib += ADC0->CLM4;
    ui16Calib += ADC0->CLMS;
    ui16Calib = ui16Calib >> 1;
    ui16Calib = ui16Calib | 0x8000; /* set the MSB */
    ADC0->MG  = ui16Calib;          /* set the minus side gain register */

    /* starting the calibration of ADC1 */
    ADC1->SC3 |= (ADC_SC3_CAL_MASK);

    /* wait until the calibration completes */
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
    ADC1->PG  = ui16Calib;          /* set the plus side gain register */

    ui16Calib = ADC1->CLM0;
    ui16Calib += ADC1->CLM1;
    ui16Calib += ADC1->CLM2;
    ui16Calib += ADC1->CLM3;
    ui16Calib += ADC1->CLM4;
    ui16Calib += ADC1->CLMS;
    ui16Calib = ui16Calib >> 1;
    ui16Calib = ui16Calib | 0x8000; /* set the MSB */
    ADC1->MG  = ui16Calib;          /* set the minus side gain register */
    /* ------- ADC self calibration procedure end ----- */

    /* Write default values to SC3 register */
    ADC0->SC3 = 0;
    ADC1->SC3 = 0;

    /* Multiply bus clock by 2 */
    SIM->CLKDIV1 = ((SIM->CLKDIV1 & ~(SIM_CLKDIV1_OUTDIV2_MASK)) | SIM_CLKDIV1_OUTDIV2((((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT) - 1U) >> 1U));

    /* setting the clock to 15 MHz (ADIV - div bus clock (60 MHz) by 4 */
    ADC0->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(2));
    ADC1->CFG1 = (ADC_CFG1_MODE(1) | ADC_CFG1_ADIV(2));

    /* Set hardware trigger */
    ADC0->SC2 = (ADC_SC2_ADTRG_MASK);
    ADC1->SC2 = (ADC_SC2_ADTRG_MASK);

    /* Set ADC0 MUXSEL*/
    ADC0->CFG2 |= ADC_CFG2_MUXSEL(ADC0_MUXSEL);
    /* Set ADC1 MUXSEL*/
    ADC1->CFG2 |= ADC_CFG2_MUXSEL(ADC1_MUXSEL);

    /**************************************/
    /* motor M1 ADC driver initialization */
    /**************************************/
    /* offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;

    /* Phase current measurement */
    /* Sector 1,6 - measured currents Ic & Ib */
    /* Ic */
    g_sM1AdcSensor.sCurrSec16.pui32AdcBasePhaC = (ADC_Type *)M1_SEC16_PH_C_BASE;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaC  = M1_SEC16_PH_C_CHANNEL;
    /* Ib */
    g_sM1AdcSensor.sCurrSec16.pui32AdcBasePhaB = (ADC_Type *)M1_SEC16_PH_B_BASE;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaB  = M1_SEC16_PH_B_CHANNEL;

    /* Sector 2,3 - measured currents Ic & Ia*/
    /* Ic */
    g_sM1AdcSensor.sCurrSec23.pui32AdcBasePhaC = (ADC_Type *)M1_SEC23_PH_C_BASE;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaC  = M1_SEC23_PH_C_CHANNEL;
    /* Ia */
    g_sM1AdcSensor.sCurrSec23.pui32AdcBasePhaA = (ADC_Type *)M1_SEC23_PH_A_BASE;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaA  = M1_SEC23_PH_A_CHANNEL;

    /* Sector 4,5 - measured currents Ia & Ib */
    /* Ia */
    g_sM1AdcSensor.sCurrSec45.pui32AdcBasePhaA = (ADC_Type *)M1_SEC45_PH_A_BASE;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaA  = M1_SEC45_PH_A_CHANNEL;
    /* Ib */
    g_sM1AdcSensor.sCurrSec45.pui32AdcBasePhaB = (ADC_Type *)M1_SEC45_PH_B_BASE;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaB  = M1_SEC45_PH_B_CHANNEL;

    /* UDCbus channel measurement */
    g_sM1AdcSensor.pui32UdcbAdcBase = (ADC_Type *)M1_UDCB_BASE;
    g_sM1AdcSensor.ui16ChanNumVDcb  = M1_UDCB_CHANNEL;

    /* Aux channel measurement */
    g_sM1AdcSensor.pui32AuxAdcBase = (ADC_Type *)M1_AUX_BASE;
    g_sM1AdcSensor.ui16ChanNumAux  = M1_AUX_CHANNEL;

    /* Assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssignInit(&g_sM1AdcSensor);

    /* Enable & setup interrupt */
#if (!MC_PMSM_DISABLE)
    ADC1->SC1[0] |= ADC_SC1_AIEN_MASK;
    EnableIRQ(FAST_LOOP_IRQN);
    NVIC_SetPriority(FAST_LOOP_IRQN, 1);
#endif
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
static void InitPDB(void)
{
    /* enable clock for PDB module */
    SIM->SCGC6 |= SIM_SCGC6_PDB_MASK;

    /* PDB triggers for ADC0 module */
    /* PDB triggers for ADC1 module */
    /* PDB | CHANNEL | PRETRIGGER | VALUE */
    /* Set Delay 0.25us after PWM reload (dead time = 0.5us ) */
    PDB0->CH[0].DLY[0] = PDB_PRETRIG_DELAY;
    PDB0->CH[1].DLY[0] = PDB_PRETRIG_DELAY;

    /* Configure PDB SC register */
    /* Select trigger */
    /* PDB error interrupt enable */
    /* PDB interrupt enable */
    /* PDB enable */
    /* Mod and ChnDlym values will be updated from buffer by writing LDOK bit */
    PDB0->SC = (PDB_SC_PDBEN_MASK | PDB_SC_LDMOD(2) | PDB_SC_TRGSEL(8) | PDB_SC_PDBEIE_MASK | PDB_SC_PDBIE_MASK);

    /* PDB interrupt delay */
    PDB0->IDLY = (M1_PWM_MODULO * M1_FOC_FREQ_VS_PWM_FREQ - M1_PWM_MODULO / 2);

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
#if (!MC_PMSM_DISABLE)
    EnableIRQ(PDB_ERROR_IRQN);
    NVIC_SetPriority(PDB_ERROR_IRQN, 2);
#endif
}
/*!
@brief   void InitCMP(void)
          - Initialization of the comparator 1 module for dc-bus over current
            detection to generate FTM0 fault

@param   void

@return  none
*/
#if M1_FAULT_CMP_ENABLE
static void InitCMP(void)
{

    /* Enable clock for CMP module */
    SIM->SCGC4 |= SIM_SCGC4_CMP_MASK;

    /* Filter - 4 consecutive samples must agree */
    M1_FAULT_CMP_INSTANCE->CR0 = CMP_CR0_FILTER_CNT(4);

    /* DAC output set to M1_FAULT_CMP_THRESHOLD */
    /* Reference voltage will be VDD */
    /* Enable DAC */
    M1_FAULT_CMP_INSTANCE->DACCR = CMP_DACCR_VOSEL(M1_FAULT_CMP_THRESHOLD) | CMP_DACCR_VRSEL_MASK | CMP_DACCR_DACEN_MASK;

    /* Plus is CMP1_IN5 ~ overcurrent pin */
    /* Minus is CMP1_IN7 ~ 6bit reference */
    M1_FAULT_CMP_INSTANCE->MUXCR = CMP_MUXCR_PSEL(5) | CMP_MUXCR_MSEL(7);

    /* Enable analog comparator */
    M1_FAULT_CMP_INSTANCE->CR1 = CMP_CR1_EN_MASK;
}
#endif /* M1_FAULT_CMP_ENABLE */

/*!
 *@brief      Set Inrush relay on HVP
 *
 *@param      none
 *
 *@return     none
 */
#if M1_INRUSH_ENABLE
static void InitRelay(void)
{
    volatile register uint32_t ui32DelayMs = 0;

    /* setup SysTick */
    SysTick->LOAD = 0xFFFFFF;
    SysTick->VAL  = SysTick->LOAD;
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    /* wait M1_INRUSH_DELAY milliseconds to turn on the relay */
    while (ui32DelayMs++ < M1_INRUSH_DELAY)
    {
        while (((SysTick->LOAD - SysTick->VAL) * 1000) < SystemCoreClock)
        {
        };
        SysTick->VAL = SysTick->LOAD;
    }

    /* turn on relay */
    M1_INRUSH_SET();
}
#endif /* M1_INRUSH_ENABLE */
