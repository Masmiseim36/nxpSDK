/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_periph_init.h"
#include "safety_error_handler.h"
#include "safety_flow_check.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Three-phase PWM periphery (FTM0) counter modulo. */
#define CLOCK_PWM_MODULO        (CLOCK_SYSTEM_FREQ / M1_PWM_FREQ)

/* Three-phase PWM dead-time. */
#define CLOCK_PWM_DEADTIME      (CLOCK_SYSTEM_FREQ / (1000000000U / M1_PWM_DEADTIME) / 4U)

/* Calculate the slow control loop timer (FTM2) modulo. */
#define CLOCK_SLOW_LOOP_MODULO  (CLOCK_SYSTEM_FREQ / IRQ_SLOW_LOOP_FREQ)


/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief   Initialization of the FTM0 peripheral. A center-aligned three-phase
 *          PWM is used.
 *
 * @param   void
 *
 * @return  none
 */
static void fsInitFTM0(void);

/*!
* @brief   Initialization of the FTM2 peripheral to performs slow control loop
*          timing.
*
* @param   void
*
* @return  none
*/
static void fsInitFTM2(void);

/*!
 * @brief   Initialization of the ADC16 periphery for current and voltage
 *          sensing.
 *
 * @param   void
 *
 * @return  none
 */
static void fsInitADC16(void);

/*!
 * @brief   Initialization of the PDB for ADC triggering and synchronization
 *          between FTM0 and ADC16.
 *
 * @param   void
 *
 * @return  none
 */
static void fsInitPDB(void);

/*!
 * @brief   Function waits for completion of ADC16 calibration and process the
 *          results afterwards.
 *
 * @details The ADC periphery must be properly configured and the calibration
 *          process launched prior calling this function.
 *
 * @param   psADC - Pointer to the beginning of ADC16 periphery address space.
 *
 * @return  none
 */
static void fsCalibADC16(ADC_Type *psADC);


/*******************************************************************************
 * Code - functions with global scope
 ******************************************************************************/

/*!
 * Motor control driver main initialization function. Peripherals
 * required for safe motor-control functionality are configured.
 */
void MCDRV_fsInit(void)
{
    /* Initialize ADC periphery. */
    fsInitADC16();

    /* Initialize PDB periphery. */
    fsInitPDB();

    /* Initialize FTM2 as slow-control loop timer. */
    fsInitFTM2();

    /* Six-channel PWM peripheral initialization. */
    fsInitFTM0();
}

/*!
 * The function starts the fast and slow loop timers.
 */
void MCDRV_fsStartTimers(void)
{
    /* Enable the system clock for FTM2 periphery. */
    FTM2->SC |= FTM_SC_CLKS(1U);

    /* Enable the system clock for FTM0 periphery. */
    M1_PWM_PERIPH->SC |= FTM_SC_CLKS(1U);
}


/*******************************************************************************
 * Code - functions with local scope
 ******************************************************************************/

/*!
 * Initialization of the FTM0 peripheral for motor M1. A center-aligned
 * three-phase PWM is used.
 */
static void fsInitFTM0(void)
{
    /* Enable the clock for FTM0 in SIM periphery. */
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;

    /* Disable all channels output using the OUTPUT MASK feature. */
    FTM0->OUTMASK = (FTM_OUTMASK_CH0OM_MASK | FTM_OUTMASK_CH1OM_MASK |
                     FTM_OUTMASK_CH2OM_MASK | FTM_OUTMASK_CH3OM_MASK |
                     FTM_OUTMASK_CH4OM_MASK | FTM_OUTMASK_CH5OM_MASK);

    /* -Disable write protection for certain registers.
     * -Free running counter and synchronization are different from TPM.
     * -Enable the counter.
     * -Fault Control Mode - Fault control is enabled for all channels,
     *  and the selected mode is the manual fault clearing.
     * -Initialize output of the channels. */
    FTM0->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK |
                 FTM_MODE_FAULTM(2U) | FTM_MODE_INIT_MASK;

    /* Counter is running in BDM mode. */
    FTM0->CONF = FTM_CONF_BDMMODE(3U);

    /* Initial value of the FTM counter. */
    FTM0->CNTIN = (uint32_t)(uint16_t)(-(int16_t)(CLOCK_PWM_MODULO >> 1U));

    /* Initialize the counter itself as well. */
    FTM0->CNT = FTM0->CNTIN;

    /* FTM counter modulo value. */
    FTM0->MOD = (uint32_t)(uint16_t)((CLOCK_PWM_MODULO >> 1U) - 1U);

    /* Enable maximum loading point (modulo value).  */
    FTM0->SYNC = FTM_SYNC_CNTMAX_MASK;

    /* Center-aligned PWM is achieved using the combine mode:
     * COMBINE = 1 - Combine mode set
     * COMP    = 1 - Complementary PWM set
     * DTEN    = 1 - Dead-time enabled
     * SYNCEN  = 1 - PWM update synchronization enabled
     * FAULTEN = 1 - Fault control enabled
    */
    /* Use complementary PWM mode. */
    FTM0->COMBINE = FTM_COMBINE_COMBINE0_MASK | FTM_COMBINE_COMP0_MASK |
                    FTM_COMBINE_DTEN0_MASK    | FTM_COMBINE_SYNCEN0_MASK |
                    FTM_COMBINE_FAULTEN0_MASK |
                    FTM_COMBINE_COMBINE1_MASK | FTM_COMBINE_COMP1_MASK |
                    FTM_COMBINE_DTEN1_MASK    | FTM_COMBINE_SYNCEN1_MASK |
                    FTM_COMBINE_FAULTEN1_MASK |
                    FTM_COMBINE_COMBINE2_MASK | FTM_COMBINE_COMP2_MASK |
                    FTM_COMBINE_DTEN2_MASK    | FTM_COMBINE_SYNCEN2_MASK |
                    FTM_COMBINE_FAULTEN2_MASK;

    /* Dead-time value setup. */
    FTM0->DEADTIME = FTM_DEADTIME_DTPS(2U) | FTM_DEADTIME_DTVAL(CLOCK_PWM_DEADTIME);

    /* initialize value registers for 50% duty cycle generation. */
    FTM0->CONTROLS[0U].CnV = (uint32_t)(uint16_t)(-(int16_t)(CLOCK_PWM_MODULO >> 2U));
    FTM0->CONTROLS[1U].CnV = (uint32_t)(uint16_t)( (CLOCK_PWM_MODULO >> 2U));
    FTM0->CONTROLS[2U].CnV = (uint32_t)(uint16_t)(-(int16_t)(CLOCK_PWM_MODULO >> 2U));
    FTM0->CONTROLS[3U].CnV = (uint32_t)(uint16_t)( (CLOCK_PWM_MODULO >> 2U));
    FTM0->CONTROLS[4U].CnV = (uint32_t)(uint16_t)(-(int16_t)(CLOCK_PWM_MODULO >> 2U));
    FTM0->CONTROLS[5U].CnV = (uint32_t)(uint16_t)( (CLOCK_PWM_MODULO >> 2U));

    /*! @note From this moment the output pins are under FTM control. Since the PWM
              output is disabled by the FTM0->OUTMASK register, there is no change
              on PWM outputs. Before the channel mode is set, the correct output pin
              polarity has to be defined.*/
    /*! @note Even if the odd channels are generated automatically by complementary
              logic, these channels have to be set to be in the same channel mode. */
    FTM0->CONTROLS[0U].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[1U].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[2U].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[3U].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[4U].CnSC = FTM_CnSC_ELSB_MASK;
    FTM0->CONTROLS[5U].CnSC = FTM_CnSC_ELSB_MASK;

    /* Set LOAD OK register. */
    /* Enable the loading of the MOD, CNTIN, and CV registers from the buffered
       values. */
    FTM0->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Enable the generation of initialization trigger. */
    FTM0->EXTTRIG = FTM_EXTTRIG_INITTRIGEN_MASK;

    /* FTM fault 0 is connected to the FNB41560 VFO output (FAULT_1 signal on
       HVP-MC3PH and HVP-KV11Z75M). */
    /* The FTM fault 0 is active low. */
    FTM0->FLTPOL  = (uint16_t)(1U << M1_PWM_FAULT_IDX);

    /* Enable the FTM fault 0. */
    FTM0->FLTCTRL = (uint16_t)(1U << M1_PWM_FAULT_IDX);

    /* Motor-control PWM periphery initialization node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_MCPER_INIT_PWM);
}


/*!
 * Initialization of the FTM2 peripheral to perform slow control loop timing.
 */
static void fsInitFTM2(void)
{
    /* Enable clock to the FTM module. */
    SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;

    /* Configure the FTM module: */
    /* -Disable write protection for certain registers. */
    /* -Enable the counter. */
    FTM2->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Settings up FTM SC register for clock setup: */
    /* -Disable the timer clock for now. */
    /* -Set prescaler to 16. */
    /* -Enable the interrupt. */
    FTM2->SC = FTM_SC_PS(4U) | FTM_SC_CLKS(0U) | FTM_SC_TOIE_MASK;

    /* Run counter in BDM mode. */
    FTM2->CONF = FTM_CONF_BDMMODE(3U);

    /* Set count value to zero. */
    FTM2->CNTIN = 0U;

    /* Initialize modulo to acquire 1kHz period. */
    FTM2->MOD = (uint16_t)(CLOCK_SLOW_LOOP_MODULO >> (FTM2->SC & FTM_SC_PS_MASK));

    /* Load the registers. */
    FTM2->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Enable & setup interrupts */
    NVIC_EnableIRQ(FTM2_IRQn);
    NVIC_SetPriority(FTM2_IRQn, IRQ_SLOW_LOOP_PRIO);

    /* Motor-control slow-loop timer periphery initialization node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_MCPER_INIT_TMR);
}

/*!
 * Process calibration of ADC16 periphery.
 */
static void fsCalibADC16(ADC_Type *psADC)
{
    uint32_t ui32Calib; /* Accumulator for ADC calibration. */

    /* Wait until the calibration completes for ADC. */
    while (!(psADC->SC1[0] & ADC_SC1_COCO_MASK))
    {
    }

    /* Check whether the ADC calibration failed. */
    if(psADC->SC3 & ADC_SC3_CALF_MASK)
    {
        /* ADC periphery calibration failure occurred. */
        FS_fsErrorHandling(FS_ERROR_MCDRV_ADC_FAIL, SAFETY_ERROR_ADC_CALIB_FAIL);
    }

    /* Accumulate the calibration results to get the plus gain value. */
    ui32Calib  = psADC->CLP0;
    ui32Calib += psADC->CLP1;
    ui32Calib += psADC->CLP2;
    ui32Calib += psADC->CLP3;
    ui32Calib += psADC->CLP4;
    ui32Calib += psADC->CLPS;
    ui32Calib >>= 1U;
    ui32Calib |= 0x8000UL; /* Set the bit 15. */
    psADC->PG  = (uint16_t)ui32Calib; /* Set the plus side gain register. */

    /* Accumulate the calibration results to get the minus gain value. */
    ui32Calib  = psADC->CLM0;
    ui32Calib += psADC->CLM1;
    ui32Calib += psADC->CLM2;
    ui32Calib += psADC->CLM3;
    ui32Calib += psADC->CLM4;
    ui32Calib += psADC->CLMS;
    ui32Calib >>= 1U;
    ui32Calib |= 0x8000UL; /* Set the bit 15. */
    psADC->MG  = (uint16_t)ui32Calib; /* Set the minus side gain register. */
}

/*!
 * Initialization of the ADC16 periphery for current and voltage
 * sensing.
 */
static void fsInitADC16(void)
{
    /* Enable clock for ADC0 and ADC1 modules. */
    SIM->SCGC6 |= (SIM_SCGC6_ADC0_MASK | SIM_SCGC6_ADC1_MASK);

    /* ------- ADC0&1 self-calibration procedure start. ----- */
    /* -Perform single-ended 12-bit conversion.
       -Use ALTCLK clock source (by default set to ~18.67 MHz)
       -Set clock divide ratio to 8 to obtain ~2.33 MHz input ADC clock */
    ADC0->CFG1 = ADC_CFG1_MODE(1U) | ADC_CFG1_ADICLK(2U) |
                 ADC_CFG1_ADIV(CLOCK_ADC_CAL_ADC16_DIV);
    ADC1->CFG1 = ADC0->CFG1;

    /* -Enable HW averaging over 32 samples.
       -Enable continuous conversion. */
    ADC0->SC3 = ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3U) | ADC_SC3_ADCO_MASK;
    ADC1->SC3 = ADC0->SC3;

    /* Start the calibration process. */
    ADC0->SC3 |= ADC_SC3_CAL_MASK;
    ADC1->SC3 |= ADC_SC3_CAL_MASK;

    /* Wait for calibration to complete and store the results. */
    fsCalibADC16(ADC0);
    fsCalibADC16(ADC1);

    /* ------- Initialize ADC0&1 for motor-control operation. ----- */
    /* Write default values to SC3 register (HW averaging is disabled). */
    ADC0->SC3 = 0UL;
    ADC1->SC3 = 0UL;

    /* -Perform single-ended 12-bit conversion.
       -Use ALTCLK clock source (by default set to ~18.67 MHz).
       -Do not divide the input clock (by default use ~18.67 MHz). */
    ADC0->CFG1 = ADC_CFG1_MODE(1U) | ADC_CFG1_ADICLK(2U) | ADC_CFG1_ADIV(0U);
    ADC1->CFG1 = ADC0->CFG1;

    /* -Use HW triggers.
       -No DMA.
       -Use default V_REFH and V_REFL reference. */
    ADC0->SC2 = ADC_SC2_ADTRG_MASK;
    ADC1->SC2 = ADC_SC2_ADTRG_MASK;

    /* Enable interrupt request generation for ADC0. */
    ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;

    /* Enable ADC0 interrupt and set the priority. */
    NVIC_EnableIRQ(ADC0_IRQn);
    NVIC_SetPriority(ADC0_IRQn, IRQ_FAST_LOOP_PRIO);

    /* Motor-control ADC periphery initialization node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_MCPER_INIT_ADC);
}

/*!
 * Initialization of the PDB for ADC triggering and synchronization
 * between FTM0 and ADC16.
 */
static void fsInitPDB(void)
{
    /* Enable clock for PDB module. */
    SIM->SCGC6 |= SIM_SCGC6_PDB0_MASK;

    /* -Configure PDB triggers for ADC0 and ADC1 module.
       -Set delay 0.75 us after the PWM reload (the dead-time is 1.5 us). */
    PDB0->CH[0U].DLY[0U] = 19U;
    PDB0->CH[1U].DLY[0U] = 19U;

    /* -Select the FTM0 external trigger output.
       -PDB sequence error interrupt is enabled.
       -PDB interrupt is enabled.
       -MOD and CHnDLYm values will be updated from buffer by writing LDOK bit. */
    PDB0->SC = PDB_SC_PDBEN_MASK | PDB_SC_LDMOD(2U) | PDB_SC_TRGSEL(8U) |
               PDB_SC_PDBEIE_MASK | PDB_SC_PDBIE_MASK;

    /* Setup the PDB interrupt delay. The PDB is clocked from the bus clock
       which is by default a third of system clock (results in ~24.9 MHz).
       The calculation includes the FOC execution multiplier. */
    PDB0->IDLY = (uint32_t)(uint16_t)
        ((CLOCK_PWM_MODULO * M1_FOC_FREQ_VS_PWM_FREQ) - (CLOCK_PWM_MODULO >> 1U)) / 3U;

    /* Update the MOD and CHnDLYm values. */
    PDB0->SC |= PDB_SC_LDOK_MASK;

    /* -Enable PDB channel back-to-back function.
       -Enable channel zero pretrigger zero for ADC0_RA.
       -Enable channel zero pretrigger one for ADC0_RB. */
    PDB0->CH[0U].C1 = PDB_C1_EN(3U) | PDB_C1_TOS(3U) | PDB_C1_BB(2U);

    /* -Enable PDB channel back-to-back function.
       -Enable channel one pretrigger zero for ADC1_RA.
       -Enable channel one pretrigger one for ADC1_RB. */
    PDB0->CH[1U].C1 = PDB_C1_EN(3U) | PDB_C1_TOS(3U) | PDB_C1_BB(2U);

    /* Enable and setup interrupt. */
    NVIC_EnableIRQ(PDB0_PDB1_IRQn);
    NVIC_SetPriority(PDB0_PDB1_IRQn, IRQ_PDB0_PRIO);

    /* Motor-control PDB periphery initialization node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_MCPER_INIT_PDB);
}

